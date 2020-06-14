//
//  woz.c
//  A2Mac
//
//  Created by Tamas Rudnai on 2/15/20.
//  Copyright © 2020 GameAlloy. All rights reserved.
//

#include <stdlib.h>
#include <string.h>

#include "woz.h"
#include "disk.h"
#include "6502.h"
#include "common.h"


WOZread_t WOZread = {0};
uint8_t   WOZlatch = 0;

int trackOffset = 0;
int bitOffset = 0;
uint64_t clkelpased;


woz_header_t woz_header;
woz_chunk_header_t woz_chunk_header;
woz_tmap_t woz_tmap;
woz_trks_t woz_trks;
int track_loaded = -1;



#pragma pack(push, 1)

typedef union trackEntry_u {
    struct {
        uint8_t data;
        uint8_t shift;
    };
    uint16_t shift16;
} trackEntry_t;

#pragma pack(pop)

trackEntry_t prepared_track[WOZ_TRACK_BYTE_COUNT];

typedef enum readState_e {
    readNormal = 0,
    readHold,
} readState_t;

readState_t readState = readNormal;
uint8_t readLatch;


void woz_loadTrack_old( int track ) {
    trackEntry_t reg = {0};

    reg.shift = woz_trks[track].data[0];
    reg.data =  woz_trks[track].data[1];
    prepared_track[0] = reg;

    for ( int offs = 1; offs < WOZ_TRACK_BYTE_COUNT; offs++ ) {
        
        for ( int i = 0; i < 8; i++ ) {
            if (reg.shift & 0x80) {
                reg.shift = 0;
            }
            
            reg.shift16 <<= 1;
        }
        
        reg.data = woz_trks[track].data[ (offs + 1) % WOZ_TRACK_BYTE_COUNT ];
        prepared_track[offs] = reg;
    }
}


typedef enum wozTrackState_e {
    wozTrackState_Start = 0,
    wozTrackState_D5,
    wozTrackState_D5_AA,
    wozTrackState_D5_AA_96,
    wozTrackState_vol1,
    wozTrackState_vol2,
    wozTrackState_trk1,
    wozTrackState_trk2,
    wozTrackState_sec1,
    wozTrackState_sec2,
} wozTrackState_t;


void woz_loadTrack( int track ) {
    trackEntry_t reg = {0};
    wozTrackState_t wozTrackState  = wozTrackState_Start;

    reg.shift = 0;
    reg.data  = 0;
    prepared_track[0] = reg;
    
    int vol = 0;
    int trk = 0;
    int sec = 0;

    int bitOffs_D5_SecHdr = 0; // bit offset of D5 Sector Header
    
    int bitOffs = 0;
    
    for ( int byteOffs = 0; byteOffs < WOZ_TRACK_BYTE_COUNT; byteOffs++ ) {

        reg.data = woz_trks[track].data[ byteOffs ];

        for ( int i = 0; i < 8; i++ ) {
            reg.shift16 <<= 1;
            
            if (reg.shift & 0x80) {
                switch (wozTrackState) {
                    case wozTrackState_D5:
                        switch (reg.shift) {
                            case 0xAA:
//                                printf("D5 AA at bitOffset:%d\n", bitOffs);
                                wozTrackState = wozTrackState_D5_AA;
                                break;
                                
                            default:
                                wozTrackState = wozTrackState_Start;
                                break;
                        }
                        
                        break;
                        
                    case wozTrackState_D5_AA:
                        switch (reg.shift) {
                            case 0x96:
                                wozTrackState = wozTrackState_vol1;
//                                printf("D5 AA 96 at bitOffset:%d\n", bitOffs);
//                                printf("Sector Header at bitOffset:%d\n", bitOffs_D5_SecHdr);
                                break;
                                
                            default:
                                wozTrackState = wozTrackState_Start;
                                break;
                        }
                        
                        break;
                        
                    case wozTrackState_vol1:
                        vol = (reg.shift << 1) | 1;
                        wozTrackState = wozTrackState_vol2;
                        break;
                        
                    case wozTrackState_vol2:
                        vol &= reg.shift;
                        wozTrackState = wozTrackState_trk1;
                        break;
                        
                    case wozTrackState_trk1:
                        trk = (reg.shift << 1) | 1;
                        wozTrackState = wozTrackState_trk2;
                        break;
                        
                    case wozTrackState_trk2:
                        trk &= reg.shift;
                        wozTrackState = wozTrackState_sec1;
                        break;
                        
                    case wozTrackState_sec1:
                        sec = (reg.shift << 1) | 1;
                        wozTrackState = wozTrackState_sec2;
                        break;
                        
                    case wozTrackState_sec2:
                        sec &= reg.shift;
                        wozTrackState = wozTrackState_Start;
                        
                        printf("Vol:%d Track:%d Sector:%d at bitOffset:%d\n", vol, trk, sec, bitOffs_D5_SecHdr);

                        break;
                        
                    default:
                        if ( reg.shift == 0xD5 ) {
//                            printf("D5 at bitOffset:%d\n", bitOffs);
                            wozTrackState = wozTrackState_D5;
                            bitOffs_D5_SecHdr = bitOffs;
                        }
                        break;
                }
                
                reg.shift = 0;
            }
            
            bitOffs++;
        }
    }
}


uint8_t woz_read() {

    int track = woz_tmap.phase[disk.phase.count];
    if (outdev) fprintf(outdev, "track: %d (%d) ", track, disk.phase.count);
    if ( track >= 40 ) {
        dbgPrintf("TRCK TOO HIGH!\n");
        return rand();
    }
    
    static int clkBeforeSync = 0;
    
    clkelpased = m6502.clktime + clkfrm - m6502.clklast;
    m6502.clklast = m6502.clktime + clkfrm;
    
    clkBeforeSync += clkelpased;
    
    const int clkBeforeAdjusting = 512;
    const int magicShiftOffset = 50;
    
    uint16_t usedBytes = woz_trks[track].bytes_used < WOZ_TRACK_BYTE_COUNT ? woz_trks[track].bytes_used : WOZ_TRACK_BYTE_COUNT;
    
    if ( usedBytes ) {
//        printf("elpased : %llu (clkBefRd:%d)\n", clkelpased, clkBeforeSync);
        
        if ( clkelpased > clkBeforeAdjusting ) {
//            printf("NEED SYNC : %llu (clkBefRd:%d)\n", clkelpased, clkBeforeSync);
            clkBeforeSync = 0;
            bitOffset = (clkelpased >> 2) & 7;
            trackOffset += clkelpased >> 5;
            trackOffset %= usedBytes;

            // preroll data stream
            WOZread.shift16 = 0;
            WOZread.data = woz_trks[track].data[trackOffset++];
            trackOffset %= usedBytes;

            WOZread.shift16 <<= bitOffset;

            for ( int i = 0; i < magicShiftOffset; i++ ) {
                for ( ; bitOffset < 8; bitOffset++ ) {
                    WOZread.shift16 <<= 1;

                    if ( WOZread.valid ) {
                        WOZread.shift = 0;
                    }
                }
                WOZread.data = woz_trks[track].data[trackOffset++];
                trackOffset %= usedBytes;
                bitOffset = 0;
            }
        }
        else {
            uint64_t bitForward = (clkelpased >> 2);

            // to avoid infinite loop and to search for bit 7 high
            for ( uint64_t i = 0; i < bitForward; i++ ) {
                if ( ++bitOffset >= 8 ) {
                    bitOffset = 0;
                    trackOffset++;
                    trackOffset %= usedBytes;

                    WOZread.data = woz_trks[track].data[trackOffset];
                }

                WOZread.shift16 <<= 1;

                if ( WOZread.valid ) {
                    WOZread.shift = 0;
                }
            }
        }

        // to avoid infinite loop and to search for bit 7 high
        for ( int i = 0; i < usedBytes * 8; i++ ) {
            if ( WOZread.valid ) {
                uint8_t byte = WOZread.shift;
                WOZread.shift = 0;
//                if (outdev) fprintf(outdev, "byte: %02X\n", byte);
                
                return byte;
            }
            
            if ( ++bitOffset >= 8 ) {
                bitOffset = 0;
                trackOffset++;
                trackOffset %= usedBytes;

                WOZread.data = woz_trks[track].data[trackOffset];
            }
            
            WOZread.shift16 <<= 1;
        }
//        if (outdev) fprintf(outdev, "TIME OUT!\n");
    }
    
    return rand();
}


int woz_loadFile( const char * filename ) {
    
//    char fullpath[256];
//
//    strcpy(fullpath, resourcePath);
//    strcat(fullpath, "/");
//    strcat(fullpath, filename);
    
    FILE * f = fopen(filename, "rb");
    if (f == NULL) {
        perror("Failed to read WOZ: ");
        return WOZ_ERR_FILE_NOT_FOUND;
    }
    
    fread( &woz_header, 1, sizeof(woz_header_t), f);
    if ( woz_header.magic != WOZ1_MAGIC ) {
        return WOZ_ERR_NOT_WOZ_FILE;
    }
    
    while ( ! feof(f) ) {
        // beginning of the chunk, so we can skip it later
        
        long r = fread( &woz_chunk_header, 1, sizeof(woz_chunk_header_t), f);
        if ( r != sizeof(woz_chunk_header_t) ) {
            if ( r ) {
                return WOZ_ERR_BAD_CHUNK_HDR;
            }
            // ok we just reached the end of the file, we should exit properly, close file handle etc
            break;
        }
        long foffs = ftell(f);
        
        void * buf = NULL;

        switch ( woz_chunk_header.magic ) {
            case WOZ_INFO_CHUNK_ID:
                break;

            case WOZ_TMAP_CHUNK_ID:
                buf = &woz_tmap;
                break;

            case WOZ_TRKS_CHUNK_ID:
                buf = woz_trks;
                break;

            case WOZ_META_CHUNK_ID:
                break;

            default:
                break;
        }
        
        if (buf) {
            r = fread( buf, 1, woz_chunk_header.size, f);
            if ( r != woz_chunk_header.size ) {
                return WOZ_ERR_BAD_DATA;
            }
        }

        // make sure we are skipping unhandled chunks correctly
        fseek(f, foffs + woz_chunk_header.size, SEEK_SET);
    }
    
    fclose(f);
    
    
    // DO NOT COMMIT THIS! ONLY FOR DEBUG!!!
//    woz_loadTrack(0x11);

    return WOZ_ERR_OK;
}



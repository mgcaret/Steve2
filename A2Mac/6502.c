//
//  main.c
//  6502
//
//  Created by Tamas Rudnai on 7/14/19.
//  Copyright © 2019 GameAlloy. All rights reserved.
//

#define CLK_WAIT
#define NO_SPEED_TEST

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#ifdef DEBUG
#define INLINE
#else
#define INLINE static __attribute__((always_inline))
#endif

#include "common.h"
#include "Apple2_mmio.h"


#define SOFTRESET_VECTOR    0x3F2

#define NMI_VECTOR          0xFFFA
#define RESET_VECTOR        0xFFFC
#define IRQ_VECTOR          0xFFFE


/**
 Instruction Implementations
 !!!! `his has to be here!!!
 This idea is that "INLINE" would work only if it is
 located in the same source file -- hence the include...
**/
#include "6502_instructions.h"


/////
unsigned long long int clktime = 0;

m6502_t m6502 = {0};


INLINE int m6502_step() {
    
//    switch ( fetch16() ) {
//        case 0xFCD0: // D0 FC BNE
//            BNE( 0xFC ); return 2;
//
//        case 0x01E9: // E9 01 SBC
//            SBC( 0x01 ) ; return 6;
//
//        default:
//            m6502.pc -= 2;
//
    
#ifdef DEBUG
    switch ( m6502.PC ) {
        case 0x400:
            dbgPrintf("START...\n");
            break;
        
        case 0x9D1:
            dbgPrintf("BREAK POINT...\n");
            break;

        case 0x35BD:
            if ( ( m6502.A == 0x35 ) && ( m6502.C ) )
            dbgPrintf("BREAK POINT...\n");
            break;
            
        default:
            break;
    }
#endif
    switch ( fetch() ) {
        case 0x00: BRK(); return 2;                                    // BRK
        case 0x01: ORA( src_X_ind() ); return 6;                       // ORA X,ind
//        case 0x02: // t jams
//        case 0x03: // SLO* (undocumented)
//        case 0x04: // NOP* (undocumented)
        case 0x05: ORA( src_zp() ); return 3;                          // ORA zpg
        case 0x06: ASL( dest_zp() ); return 5;                         // ASL zpg
//        case 0x07: // SLO* (undocumented)
        case 0x08: PHP(); return 3;                                    // PHP
        case 0x09: ORA( imm() ); return 2;                             // ORA imm
        case 0x0A: ASLA(); return 2;                                   // ASL A
//        case 0x0B: // ANC** (undocumented)
//        case 0x0C: // NOP* (undocumented)
        case 0x0D: ORA( src_abs() ); return 4;                         // ORA abs
        case 0x0E: ASL( dest_abs() ); return 6;                        // ASL abs
//            case 0x0F: // SLO* (undocumented)
        case 0x10: BPL( rel_addr() ); return 2;                        // BPL rel
        case 0x11: ORA( src_ind_Y() ); return 5;                       // ORA ind,Y
//        case 0x12: // t jams
//        case 0x13: // SLO* (undocumented)
//        case 0x14: // NOP* (undocumented)
        case 0x15: ORA( src_zp_X() ); return 4;                        // ORA zpg,X
        case 0x16: ASL( dest_zp_X() ); return 6;                       // ASL zpg,X
//            case 0x17: // SLO* (undocumented)
        case 0x18: CLC(); return 2;                                    // CLC
        case 0x19: ORA( src_abs_Y() ); return 4;                       // ORA abs,Y
//        case 0x1A: // NOP* (undocumented)
//        case 0x1B: // SLO* (undocumented)
//        case 0x1C: // NOP* (undocumented)
        case 0x1D: ORA( src_abs_X() ); return 4;                       // ORA abs,X
        case 0x1E: ASL( dest_abs_X() ); return 7;                      // ASL abs,X
//        case 0x1F: // SLO* (undocumented)
        case 0x20: JSR( abs_addr() ); return 6;                        // JSR abs
        case 0x21: AND( src_X_ind() ); return 6;                       // AND X,ind
//        case 0x22: KIL
//        case 0x23: RLA izx 8
        case 0x24: BIT( src_zp() ); return 3;                          // BIT zpg
        case 0x25: AND( src_zp() ); return 3;                          // AND zpg
        case 0x26: ROL( dest_zp() ); return 5;                         // ROL zpg
//        case 0x27: RLA zp 5
        case 0x28: PLP(); return 4;                                    // PLP
        case 0x29: AND( imm() ); return 2;                             // AND imm
        case 0x2A: ROLA(); return 2;                                   // ROL A
//        case 0x2B: ANC imm 2
        case 0x2C: BIT( src_abs() ); return 4;                         // BIT abs
        case 0x2D: AND( src_abs() ); return 4;                         // AND abs
        case 0x2E: ROL( dest_abs() ); return 6;                        // ROL abs
//        case 0x2F: RLA abs 6
        case 0x30: BMI( rel_addr() ); return 2;                        // BMI rel
        case 0x31: AND( src_ind_Y() ); return 5;                       // AND ind,Y
//        case 0x32: KIL
//        case 0x33: RLA izy 8
//        case 0x34: NOP zpx 4
        case 0x35: AND( src_zp_X() ); return 4;                        // AND zpg,X
        case 0x36: ROL( dest_zp_X() ); return 6;                       // ROL zpg,X
//        case 0x37: RLA zpx 6
        case 0x38: SEC(); return 2;                                    // SEC
        case 0x39: AND( src_abs_Y() ); return 4;                       // AND abs,Y
//        case 0x3A: NOP 2
//        case 0x3B: RLA aby 7
//        case 0x3C: NOP abx 4
        case 0x3D: AND( src_abs_X() ); return 4;                       // AND abs,X
        case 0x3E: ROL( dest_abs_X() ); return 7;                      // ROL abs,X
//            case 0x3F: RLA abx 7
        case 0x40: RTI(); return 6;                                    // RTI
        case 0x41: EOR( src_X_ind() ); return 6;                       // EOR X,ind
//        case 0x42: KIL
//        case 0x43: SRE izx 8
//        case 0x44: NOP zp 3
        case 0x45: EOR( src_zp() ); return 3;                          // EOR zpg
        case 0x46: LSR( dest_zp() ); return 5;                         // LSR zpg
//        case 0x47: SRE zp 5
        case 0x48: PHA(); return 3;                                    // PHA
        case 0x49: EOR( imm() ); return 2;                             // EOR imm
        case 0x4A: LSRA(); return 2;                                   // LSR A
//        case 0x4B: ALR imm 2
        case 0x4C: JMP( abs_addr() ); return 3;                        // JMP abs
        case 0x4D: EOR( src_abs() ); return 4;                         // EOR abs
        case 0x4E: LSR( dest_abs() ); return 6;                        // LSR abs
//        case 0x4F: SRE abs 6
        case 0x50: BVC( rel_addr() ); return 2;                        // BVC rel
        case 0x51: EOR( src_ind_Y() ); return 5;                       // EOR ind,Y
//        case 0x52: KIL
//        case 0x53: SRE izy 8
//        case 0x54: NOP zpx 4
        case 0x55: EOR( src_zp_X() ); return 4;                        // AND zpg,X
        case 0x56: LSR( dest_zp_X() ); return 6;                       // LSR zpg,X
//            case 0x57: SRE zpx 6
        case 0x58: CLI(); return 2;                                    // CLI
        case 0x59: EOR( src_abs_Y() ); return 4;                       // EOR abs,Y
//        case 0x5A: NOP 2
//        case 0x5B: SRE aby 7
//        case 0x5C: NOP abx 4
        case 0x5D: EOR( src_abs_X() ); return 4;                       // EOR abs,X
        case 0x5E: LSR( dest_abs_X() ); return 7;                      // LSR abs,X
//            case 0x5F: SRE abx 7
        case 0x60: RTS(); return 6;                                    // RTS
        case 0x61: ADC( src_X_ind() ); return 6;                       // ADC X,ind
//        case 0x62: KIL
//        case 0x63: RRA izx 8
//        case 0x64: NOP zp 3
        case 0x65: ADC( src_zp() ); return 3;                          // ADC zpg
        case 0x66: ROR( dest_zp() ); return 5;                         // ROR zpg
//        case 0x67: RRA zp 5
        case 0x68: PLA(); break;                                       // PLA
        case 0x69: ADC( imm() ); return 2;                             // ADC imm
        case 0x6A: RORA(); return 2;                                   // ROR A
//        case 0x6B: ARR imm 2
        case 0x6C: JMP( ind_addr() ); return 5;                        // JMP ind
        case 0x6D: ADC( src_abs() ); return 4;                         // ADC abs
        case 0x6E: ROR( dest_abs() ); return 6;                        // ROR abs
//        case 0x6F: RRA abs 6
        case 0x70: BVS( rel_addr() ); break;                           // BVS rel
        case 0x71: ADC( src_ind_Y() ); return 5;                       // ADC ind,Y
//        case 0x72:
//        case 0x73:
//        case 0x74:
        case 0x75: ADC( src_zp_X() ); return 4;                        // ADC zpg,X
        case 0x76: ROR( dest_zp_X() ); return 6;                       // ROR zpg,X
//            case 0x77:
        case 0x78: SEI(); break;                                       // SEI
        case 0x79: ADC( src_abs_Y() ); return 4;                       // ADC abs,Y
//        case 0x7A:
//        case 0x7B:
//        case 0x7C:
        case 0x7D: ADC( src_abs_X() ); return 4;                       // ADC abs,X
        case 0x7E: ROR( dest_abs_X() ); return 7;                      // ROR abs,X
//        case 0x7F:
//        case 0x80:
        case 0x81: STA( dest_X_ind() ) ; return 6;                     // STA X,ind
//        case 0x82:
//        case 0x83:
        case 0x84: STY( dest_zp() ); return 3;                         // STY zpg
        case 0x85: STA( dest_zp() ); return 3;                         // STA zpg
        case 0x86: STX( dest_zp() ); return 3;                         // STX zpg
//        case 0x87:
        case 0x88: DEY(); return 2;                                    // DEY
//        case 0x89: NOP(); imm(); return 4;                             // NOP imm
        case 0x8A: TXA(); return 2;                                    // TXA
//        case 0x8B:
        case 0x8C: STY( dest_abs() ); return 4;                        // STY abs
        case 0x8D: STA( dest_abs() ); return 4;                        // STA abs
        case 0x8E: STX( dest_abs() ); return 4;                        // STX abs
//        case 0x8F:
        case 0x90: BCC( rel_addr() ); return 2;                        // BCC rel
        case 0x91: STA( dest_ind_Y() ); return 6;                      // STA ind,Y
//        case 0x92:
//        case 0x93:
        case 0x94: STY( dest_zp_X() ); return 4;                       // STY zpg,X
        case 0x95: STA( dest_zp_X() ); return 4;                       // STA zpg,X
        case 0x96: STX( dest_zp_Y() ); return 4;                       // STX zpg,Y
//        case 0x97:
        case 0x98: TYA(); return 2;                                    // TYA
        case 0x99: STA( dest_abs_Y() ); return 5;                      // STA abs,Y
        case 0x9A: TXS(); return 2;                                    // TXS
//        case 0x9B:
//        case 0x9C:
        case 0x9D: STA( dest_abs_X() ); return 5;                      // STA abs,X
//        case 0x9E:
//        case 0x9F:
        case 0xA0: LDY( imm() ); return 2;                             // LDY imm
        case 0xA1: LDA( src_X_ind() ) ; return 6;                      // LDA X,ind
        case 0xA2: LDX( imm() ); return 2;                             // LDX imm
//        case 0xA3:
        case 0xA4: LDY( src_zp() ); return 3;                          // LDY zpg
        case 0xA5: LDA( src_zp() ); return 3;                          // LDA zpg
        case 0xA6: LDX( src_zp() ); return 3;                          // LDX zpg
//        case 0xA7:
        case 0xA8: TAY(); return 2;                                    // TAY
        case 0xA9: LDA( imm() ); return 2;                             // LDA imm
        case 0xAA: TAX(); return 2;                                    // TAX
//        case 0xAB:
        case 0xAC: LDY( src_abs() ); return 4;                         // LDY abs
        case 0xAD: LDA( src_abs() ); return 4;                         // LDA abs
        case 0xAE: LDX( src_abs() ); return 4;                         // LDX abs
//        case 0xAF:
        case 0xB0: BCS( rel_addr() ); return 2;                        // BCS rel
        case 0xB1: LDA( src_ind_Y() ); return 5;                       // LDA ind,Y
//        case 0xB2:
//        case 0xB3:
        case 0xB4: LDY( src_zp_X() ); return 4;                        // LDY zpg,X
        case 0xB5: LDA( src_zp_X() ); return 4;                        // LDA zpg,X
        case 0xB6: LDX( src_zp_Y() ); return 4;                        // LDX zpg,Y
//        case 0xB7:
        case 0xB8: CLV(); return 2;                                    // CLV
        case 0xB9: LDA( src_abs_Y() ); return 4;                       // LDA abs,Y
        case 0xBA: TSX(); return 2;                                    // TSX
//        case 0xBB:
        case 0xBC: LDY( src_abs_X() ); return 4;                       // LDY abs,X
        case 0xBD: LDA( src_abs_X() ); return 4;                       // LDA abs,X
        case 0xBE: LDX( src_abs_Y() ); return 4;                       // LDX abs,Y
//        case 0xBF:
        case 0xC0: CPY( imm() ); break;                                // CPY imm
        case 0xC1: CMP( src_X_ind() ) ; break;                         // LDA X,ind
//        case 0xC2:
//        case 0xC3:
        case 0xC4: CPY( src_zp() ); return 3;                          // CPY zpg
        case 0xC5: CMP( src_zp() ); return 3;                          // CMP zpg
        case 0xC6: DEC( dest_zp() ); return 5;                         // DEC zpg
//        case 0xC7:
        case 0xC8: INY(); return 2;                                    // INY
        case 0xC9: CMP( imm() ); return 2;                             // CMP imm
        case 0xCA: DEX(); return 2;                                    // DEX
//        case 0xCB:
        case 0xCC: CPY( src_abs() ); return 4;                         // CPY abs
        case 0xCD: CMP( src_abs() ); return 4;                         // CMP abs
        case 0xCE: DEC( dest_abs() ); return 4;                        // DEC abs
//        case 0xCF:
        case 0xD0: BNE( rel_addr() ); return 2;                        // BNE rel
        case 0xD1: CMP( src_ind_Y() ); return 5;                       // CMP ind,Y
//        case 0xD2:
//        case 0xD3:
//        case 0xD4:
        case 0xD5: CMP( src_zp_X() ); return 4;                        // CMP zpg,X
        case 0xD6: DEC( dest_zp_X() ); return 6;                       // DEC zpg,X
//        case 0xD7:
        case 0xD8: CLD(); return 2;                                    // CLD
        case 0xD9: CMP( src_abs_Y() ); return 4;                       // CMP abs,Y
//        case 0xDA:
//        case 0xDB:
//        case 0xDC:
        case 0xDD: CMP( src_abs_X() ); return 4;                       // CMP abs,X
        case 0xDE: DEC( dest_abs_X() ); return 7;                      // DEC abs,X
//        case 0xDF:
        case 0xE0: CPX( imm() ); return 2;                             // CPX imm
        case 0xE1: SBC( src_X_ind() ) ; return 6;                      // SBC (X,ind)
//        case 0xE2:
//        case 0xE3:
        case 0xE4: CPX( src_zp() ); return 3;                          // CPX zpg
        case 0xE5: SBC( src_zp() ); return 3;                          // SBC zpg
        case 0xE6: INC( dest_zp() ); return 5;                         // INC zpg
//        case 0xE7:
        case 0xE8: INX(); return 2;                                    // INX
        case 0xE9: SBC( imm() ); return 2;                             // SBC imm
        case 0xEA: NOP(); return 2;                                    // NOP
//        case 0xEB:
        case 0xEC: CPX( src_abs() ); return 4;                         // CPX abs
        case 0xED: SBC( src_abs() ); return 4;                         // SBC abs
        case 0xEE: INC( dest_abs() ); return 6;                        // INC abs
//        case 0xEF:
        case 0xF0: BEQ( rel_addr() ); return 2;                        // BEQ rel
        case 0xF1: SBC( src_ind_Y() ); return 5;                       // SBC ind,Y
//        case 0xF2:
//        case 0xF3:
//        case 0xF4:
        case 0xF5: SBC( src_zp_X() ); return 4;                        // SBC zpg,X
        case 0xF6: INC( dest_zp_X() ); return 6;                       // INC zpg,X
//        case 0xF7:
        case 0xF8: SED(); break;                                       // SED
        case 0xF9: SBC( src_abs_Y() ); return 4;                       // SBC abs,Y
//        case 0xFA:
//        case 0xFB:
//        case 0xFC:
        case 0xFD: SBC( src_abs_X() ); return 4;                       // SBC abs,X
        case 0xFE: INC( dest_abs_X() ); return 6;                      // INC abs,X
//            case 0xFF:

        default:
            printf("%04X: Unimplemented Instruction 0x%02X\n", m6502.PC -1, memread( m6502.PC -1 ));
            break;
    }
//    } // fetch16
    
    return 4;
}

const unsigned long long int iterations = G;

unsigned long long tick_per_sec = G;
unsigned long long tick_6502_per_sec = 0;
unsigned long long MHz_6502 = 1.023 * M;

static __attribute__((always_inline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi) );
    return ( (unsigned long long)lo) | ( ((unsigned long long)hi) << 32 );
}

// nanosec does not work very well for some reason
struct timespec tim, tim2;

INLINE void m6502_run() {
    // init time
#ifdef CLK_WAIT
    unsigned long long s = rdtsc();
    unsigned long long e = (unsigned long long)-1LL;
#endif

#ifdef SPEED_TEST
    for ( unsigned long long int i = 0; i < iterations ; i++ )
#else
//    for ( ; m6502.pc ; )
    
    tim.tv_sec = 0;
    tim.tv_nsec = 500L;
    
    for ( ; ; )
#endif
    {
        if ( m6502.IF ) {
            switch (m6502.interrupt) {
                case NMI:
                    break;
                    
                case HARDRESET:
                    break;
                    
                case SOFTRESET:
                    m6502.PC = memread16(SOFTRESET_VECTOR);
                    break;
                    
                default:
                    break;
            }
            
            m6502.IF = 0;
        }
        
        dbgPrintf("%llu %04X: ", clktime, m6502.PC);
        clktime += m6502_step();

#ifdef CLK_WAIT
        e = tick_6502_per_sec * clktime;
        // query time + wait

        // TODO: We should use nanosleep
        usleep(1); // this is good enough for debugging

//        nanosleep(&tim, &tim2);

        // tight loop gives us the most precise wait time
//        while ( rdtsc() - s < e ) {}
#endif
        
        dbgPrintf("\n");
    }
}

void init() {
    unsigned long long s = rdtsc();
    sleep(1);
    unsigned long long e = rdtsc();
    tick_per_sec = e - s;
    tick_6502_per_sec = tick_per_sec / MHz_6502;
    
    memset( RAM, 0, sizeof(RAM) );

    
//    RAM[ 0 ] = 0x4C;
//    RAM[ 1 ] = 0;
//    RAM[ 2 ] = 0;
//
//    RAM[ 0xBFFD ] = 0x4C;
//    RAM[ 0xBFFE ] = 0;
//    RAM[ 0xBFFF ] = 0;
    
    m6502.A = m6502.X = m6502.Y = 0xFF;
    // reset vector
    m6502.SP = 0xFF -3;
    m6502.SR = 0x30;

    // memory size
    *((uint16_t*)(&RAM[0x73])) = 0xC000;
    
#define NO_FUNCTIONTEST
    
#ifdef FUNCTIONTEST
    FILE * f = fopen("/Users/trudnai/Library/Containers/com.gamealloy.A2Mac/Data/6502_functional_test.bin", "rb");
    if (f == NULL) {
        perror("Failed: ");
        return;
    }
    
    fread( RAM, 1, 65536, f);
    fclose(f);
    
    m6502.PC = 0x400;

#else
    FILE * f = fopen("/Users/trudnai/Library/Containers/com.gamealloy.A2Mac/Data/apple.rom", "rb");
    if (f == NULL) {
        perror("Failed: ");
        return;
    }
    
    fread( RAM + 0xD000, 1, 0x3000, f);
    fclose(f);

    m6502.PC = memread16( RESET_VECTOR );
#endif
    
    
    uint8_t counter[] = {
                           // 1    * COUNTER2
                           // 2
                           // 3             ORG   $1000
                           // 4    SCREEN   EQU   $400
                           // 5    HOME     EQU   $FC58
                           // 6    DIGITS   EQU   $06
                           // 7    ZERO     EQU   $B0
                           // 8    CARRY    EQU   $BA
                           // 9    RDKEY    EQU   $FD0C
                           //10

        // I have placed NOP to keep addresses
        
        0xA0, 0x09, 0xEA,  //11            LDY   #$09 ; NOP
        0x84, 0x06,        //12            STY   #DIGITS
        0xEA, 0xEA,        //13            NOP NOP
        0xEA, 0xEA, 0xEA,  //14            NOP NOP NOP

        0xA6, 0x06,        //15            LDY   DIGITS
        0xA9, 0xB0,        //16   CLEAR    LDA   #ZERO
        0x99, 0x00, 0x04,  //17            STA   SCREEN,Y
        0x88,              //18            DEY
        0x10, 0xF8,        //19            BPL   CLEAR
        
        0xA4, 0x06,        //20   START    LDY   DIGITS
        0x20, 0x36, 0x10,  //21   ONES     JSR   INC
        0xB9, 0x00, 0x04,  //22            LDA   SCREEN,Y
        0xC9, 0xBA,        //23            CMP   #CARRY
        0xD0, 0xF6,        //24            BNE   ONES
        
        0xA9, 0xB0,        //25   NEXT     LDA   #ZERO
        0x99, 0x00, 0x04,  //26            STA   SCREEN,Y
        0x88,              //27            DEY
        0x30, 0x0D,        //28            BMI   END
        
        0x20, 0x36, 0x10,  //29            JSR   INC
        0xB9, 0x00, 0x04,  //30            LDA   SCREEN,Y
        0xC9, 0xBA,        //31            CMP   #CARRY
        0xD0, 0xE2,        //32            BNE   START
        
        0x4C, 0x20, 0x10,  //33            JMP   NEXT
        0x60,              //34   END      RTS

        0xB9, 0x00, 0x04,  //36   INC      LDA   SCREEN,Y
        0xAA,              //37            TAX
        0xE8,              //38            INX
        0x8A,              //39            TXA
        0x99, 0x00, 0x04,  //40            STA   SCREEN,Y
        0x60,              //41            RTS
    };

    
    uint8_t counter_fast[] = {
        // 1    * COUNTER2
        // 2
        // 3             ORG   $1000
        // 4    SCREEN   EQU   $400
        // 5    HOME     EQU   $FC58
        // 6    DIGITS   EQU   $06
        // 7    ZERO     EQU   $B0
        // 8    CARRY    EQU   $BA
        // 9    RDKEY    EQU   $FD0C
        //10

        // I have placed NOP to keep addresses
        
        0xA0, 0x06,        // 00            LDY   #$09
        0x84, 0x06,        // 02            STY   #DIGITS
        
        0xA6, 0x06,        // 04            LDY   DIGITS
        0xA9, 0xB0,        // 06   CLEAR    LDA   #ZERO
        0x99, 0x00, 0x04,  // 08            STA   SCREEN,Y
        0x88,              // 0B            DEY
        0x10, 0xF8,        // 0C            BPL   CLEAR
        
        0xA6, 0x06,        // 0E   START    LDX   DIGITS
        0xA9, 0xBA,        // 10            LDA   #CARRY
        0xFE, 0x00, 0x04,  // 12   ONES     INC   SCREEN,X
        0xDD, 0x00, 0x04,  // 15            CMP   SCREEN,X
        0xD0, 0xF8,        // 18            BNE   ONES
        
        0xA9, 0xB0,        // 1A   NEXT     LDA   #ZERO
        0x9D, 0x00, 0x04,  // 1C            STA   SCREEN,X
        0xCA,              // 1F            DEX
        0x30, 0x0C,        // 20            BMI   END
        
        0xFE, 0x00, 0x04,  // 22            INC   SCREEN,X
        0xBD, 0x00, 0x04,  // 25            LDA   SCREEN,X
        0xC9, 0xBA,        // 28            CMP   #CARRY
        0xD0, 0xE2,        // 2A            BNE   START
        
        0xF0, 0xEC,        // 2C            BEQ   NEXT
        
        0x60,              // 2E   END      RTS
        
    };

    
//    memcpy( RAM + 0x1000, counter_fast, sizeof(counter));
    
//    m6502.pc = 0x1000;

}


void tst6502() {
    // insert code here...
    printf("6502\n");
    
    init();
    
    //    clock_t start = clock();
    unsigned long long s = rdtsc();
    m6502_run();
    //    clock_t end = clock();
    //    double execution_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    unsigned long long e = rdtsc();
    unsigned long long t = e - s;
    double execution_time = (double)t / tick_per_sec;
    
    double mips = iterations / (execution_time * M);
    double mhz = clktime / (execution_time * M);
    printf("clk:%llu Elpased time: (%llu / %llu / %llu), %.3lfs (%.3lf MIPS, %.3lf MHz)\n", clktime, tick_per_sec, MHz_6502, tick_6502_per_sec, execution_time, mips, mhz);
}

int ___main(int argc, const char * argv[]) {
    tst6502();
    return 0;
}


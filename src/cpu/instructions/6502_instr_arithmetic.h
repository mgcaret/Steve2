//
//  main.c
//  6502
//
//  Created by Tamas Rudnai on 7/14/19.
//  Copyright © 2019 GameAlloy. All rights reserved.
//


#ifndef __6502_INSTR_ARITHMETIC_H__
#define __6502_INSTR_ARITHMETIC_H__

#include "common.h"

/**
 ADC  Add Memory to Accumulator with Carry
 
 A + M + C -> A, C                N Z C I D V
                                  + + + - - +
 
 addressing    assembler    opc  bytes  cyles
 --------------------------------------------
 immidiate     ADC #oper     69    2     2
 zeropage      ADC oper      65    2     3
 zeropage,X    ADC oper,X    75    2     4
 absolute      ADC oper      6D    3     4
 absolute,X    ADC oper,X    7D    3     4*
 absolute,Y    ADC oper,Y    79    3     4*
 (indirect,X)  ADC (oper,X)  61    2     6
 (indirect),Y  ADC (oper),Y  71    2     5*
**/
INLINE void _ADC( uint8_t src ) {
    uint16_t tmp;
    
    // V = C7 != C6
    m6502.V = ((m6502.A & 0x7F) + (src & 0x7F) + (m6502.C != 0)) > 0x7F;
    
    if ( m6502.D ) {
        if ( (tmp = (m6502.A & 0x0F) + (src & 0x0F) + (m6502.C != 0)) > 0x09 ) {
            tmp += 0x06;
        }
        if ( (tmp += (m6502.A & 0xF0) + (src & 0xF0)) > 0x99 ) {
            tmp += 0x60;
        }
    }
    else {
        tmp = (uint16_t)m6502.A + src + (m6502.C != 0);
    }
    
    set_flags_NZ( m6502.A = tmp );
    m6502.C = tmp > 0xFF;
    m6502.V ^= m6502.C;
    
    //    // this is good but slow:
    //    uint16_t tmp = (uint16_t)m6502.A + src + m6502.C;
    //    m6502.V = ( !((m6502.A ^ src) & 0x80)) && ( (m6502.A ^ tmp) & 0x80);
    //    m6502.C = tmp > 0xFF;
    //    set_flags_NZ( m6502.A = tmp );
    
    //    // this is good but slow:
    //    uint16_t tmp = (uint16_t)m6502.A + src + m6502.C;
    //    m6502.V = ( ((m6502.A ^ src) ^ (m6502.A ^ tmp)) & 0x80 ) != 0;
    //    m6502.C = tmp > 0xFF;
    //    set_flags_NZ( m6502.A = tmp );
    
}
INLINE void ADC( uint8_t src ) {
    dbgPrintf("ADC(%02X) ", src);
    disPrintf(disassembly.inst, "ADC");
    
    _ADC(src);
}

/**
 SBC  Subtract Memory from Accumulator with Borrow
 
 A - M - C -> A                   N Z C I D V
                                  + + + - - +
 
 addressing    assembler    opc  bytes  cyles
 --------------------------------------------
 immidiate     SBC #oper     E9    2     2
 zeropage      SBC oper      E5    2     3
 zeropage,X    SBC oper,X    F5    2     4
 absolute      SBC oper      ED    3     4
 absolute,X    SBC oper,X    FD    3     4*
 absolute,Y    SBC oper,Y    F9    3     4*
 (indirect,X)  SBC (oper,X)  E1    2     6
 (indirect),Y  SBC (oper),Y  F1    2     5*
**/
INLINE void SBC( uint8_t src ) {
    dbgPrintf("SBC(%02X) ", src);
    disPrintf(disassembly.inst, "SBC");
//    ADC( ~src );
    
    uint16_t tmp;
    
    if( m6502.D ) {
        tmp = (m6502.A & 0x0F) - ( src & 0x0F ) - !m6502.C;
        if( (tmp & 0x10) != 0) {
            tmp = ( (tmp - 0x06 ) & 0x0F ) | ( (m6502.A & 0xF0) - (src & 0xF0) - 0x10 );
        }
        else {
            tmp = (tmp & 0x0F) | ( (m6502.A & 0xF0) - (src & 0xF0) );
        }
        
        if(( tmp & 0x100 ) != 0) {
            tmp -= 0x60;
        }
    }
    else
    {
        tmp = m6502.A - src - !m6502.C;
    }
    
    m6502.C = tmp < 0x100;
    m6502.V = ( (m6502.A ^ tmp) & 0x80 ) && ( (m6502.A ^ src) & 0x80 );
    set_flags_NZ( m6502.A = tmp );
}

#endif // __6502_INSTR_ARITHMETIC_H__


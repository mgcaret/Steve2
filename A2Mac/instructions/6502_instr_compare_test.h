//
//  main.c
//  6502
//
//  Created by Tamas Rudnai on 7/14/19.
//  Copyright © 2019 GameAlloy. All rights reserved.
//

#ifndef __6502_INSTR_COMPARE_TEST_H__
#define __6502_INSTR_COMPARE_TEST_H__

/**
 BIT  Test Bits in Memory with Accumulator
 
 bits 7 and 6 of operand are transfered to bit 7 and 6 of SR (N,V);
 the zeroflag is set to the result of operand AND accumulator.
 
 A AND M, M7 -> N, M6 -> V        N Z C I D V
                                 M7 + - - - M6
 
 addressing    assembler    opc  bytes  cyles
 --------------------------------------------
 zeropage      BIT oper      24    2     3
 absolute      BIT oper      2C    3     4
 
 **/
static inline void BIT( uint8_t imm ) {
    dbgPrintf("BIT(%02X) ", imm);
    uint8_t m = m6502.A & imm;
    m6502.flags.N = BITTEST(m, 7);
    m6502.flags.V = BITTEST(m, 6);
    m6502.flags.Z = m == 0;
}

/**
 CMP  Compare Memory with Accumulator
 
 A - M                            N Z C I D V
                                  + + + - - -
 
 addressing    assembler    opc  bytes  cyles
 --------------------------------------------
 immidiate     CMP #oper     C9    2     2
 zeropage      CMP oper      C5    2     3
 zeropage,X    CMP oper,X    D5    2     4
 absolute      CMP oper      CD    3     4
 absolute,X    CMP oper,X    DD    3     4*
 absolute,Y    CMP oper,Y    D9    3     4*
 (indirect,X)  CMP (oper,X)  C1    2     6
 (indirect),Y  CMP (oper),Y  D1    2     5*
 **/
static inline void CMP( uint8_t imm ) {
    dbgPrintf("CMP(%02X) ", imm);
    set_flags_NZC( (int)m6502.A - imm );
}

/**
 CPX  Compare Memory and Index X
 
 X - M                            N Z C I D V
                                  + + + - - -
 
 addressing    assembler    opc  bytes  cyles
 --------------------------------------------
 immidiate     CPX #oper     E0    2     2
 zeropage      CPX oper      E4    2     3
 absolute      CPX oper      EC    3     4
 **/
static inline void CPX( uint8_t imm ) {
    dbgPrintf("CPX(%02X) ", imm);
    set_flags_NZC( (int)m6502.X - imm );
}

/**
 CPY  Compare Memory and Index Y
 
 Y - M                            N Z C I D V
                                  + + + - - -
 
 addressing    assembler    opc  bytes  cyles
 --------------------------------------------
 immidiate     CPY #oper     C0    2     2
 zeropage      CPY oper      C4    2     3
 absolute      CPY oper      CC    3     4
 **/
static inline void CPY( uint8_t imm ) {
    dbgPrintf("CPY(%02X) ", imm);
    set_flags_NZC( (int)m6502.Y - imm );
}

#endif // __6502_INSTR_COMPARE_TEST_H__


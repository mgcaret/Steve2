//
//  main.c
//  6502
//
//  Created by Tamas Rudnai on 7/14/19.
//  Copyright © 2019 GameAlloy. All rights reserved.
//

#ifndef __6502_INSTR_MISC_H__
#define __6502_INSTR_MISC_H__


/**
 BRK  Force Break
 
 interrupt,                       N Z C I D V
 push PC+2, push SR               - - - 1 - -
 
 addressing    assembler    opc  bytes  cyles
 --------------------------------------------
 implied       BRK           00    1     7
 **/
INLINE int BRK() {
    dbgPrintf("BRK ");
    disPrintf(disassembly.inst, "BRK");
    PUSH_addr(m6502.PC +1); // PC +2, however, fetch already incremented it by 1
    // B flag should be set before pushing flags onto the stack
    m6502.B = 1;
    PUSH( getFlags().SR );
    m6502.I = 1;
    m6502.PC = memread16(IRQ_VECTOR);
    
    return 7;
}

/**
 NOP  No Operation
 
 ---                              N Z C I D V
                                 - - - - - -
 
 addressing    assembler    opc  bytes  cyles
 --------------------------------------------
 implied       NOP           EA    1     2
 **/
INLINE void NOP() {
    dbgPrintf("NOP ");
    disPrintf(disassembly.inst, "NOP");
}

#endif // __6502_INSTR_MISC_H__


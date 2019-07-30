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
static inline void BRK() {
    dbgPrintf("BRK ");
}

/**
 NOP  No Operation
 
 ---                              N Z C I D V
 - - - - - -
 
 addressing    assembler    opc  bytes  cyles
 --------------------------------------------
 implied       NOP           EA    1     2
 **/
static inline void NOP() {
    dbgPrintf("STA ");
}

#endif // __6502_INSTR_MISC_H__


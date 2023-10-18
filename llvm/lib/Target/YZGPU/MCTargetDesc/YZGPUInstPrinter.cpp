//===-- YZGPUInstPrinter.cpp - Convert YZGPU MCInst to asm syntax ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This class prints an YZGPU MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "YZGPUInstPrinter.h"
#include "YZGPUBaseInfo.h"
// #include "YZGPUMCExpr.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
using namespace llvm;

#define DEBUG_TYPE "asm-printer"

// Include the auto-generated portion of the assembly writer.
#define PRINT_ALIAS_INSTR
#include "YZGPUGenAsmWriter.inc"

void YZGPUInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                 StringRef Annot, const MCSubtargetInfo &STI,
                                 raw_ostream &O) {
  O.flush();
  printInstruction(MI, Address, STI, O);
  printAnnotation(O, Annot);
}

void YZGPUInstPrinter::printRegName(raw_ostream &O, MCRegister Reg) const {
  O << getRegisterName(Reg);
}

void YZGPUInstPrinter::printOperandInputMods(const MCInst *MI, unsigned OpNo,
                                             const MCSubtargetInfo &STI,
                                             raw_ostream &O) {
  // const MCInstrDesc &Desc = MII.get(MI->getOpcode());
  unsigned InputModifiers = MI->getOperand(OpNo).getImm();

  printOperand(MI, OpNo + 1, STI, O);
  if (InputModifiers & YZGPUSrcMods::NEG)
    O << ".neg";
  else if (InputModifiers & YZGPUSrcMods::ABS)
    O << ".abs";
}

void YZGPUInstPrinter::printOpSel(const MCInst *MI, unsigned OpNo,
                                  raw_ostream &O) {
  unsigned Imm = MI->getOperand(OpNo).getImm();
  switch (Imm) {
  // case SdwaSel::BYTE_0: O << "BYTE_0"; break;
  // case SdwaSel::BYTE_1: O << "BYTE_1"; break;
  // case SdwaSel::BYTE_2: O << "BYTE_2"; break;
  // case SdwaSel::BYTE_3: O << "BYTE_3"; break;
  case ALUOpSel::WORD_0:
    O << "WORD_0";
    break;
  case ALUOpSel::WORD_1:
    O << "WORD_1";
    break;
  default:
    llvm_unreachable("Invalid operand select data");
  }
}

void YZGPUInstPrinter::printDstSel(const MCInst *MI, unsigned OpNo,
                                   const MCSubtargetInfo &STI, raw_ostream &O) {
  O << "dst_sel:";
  printOpSel(MI, OpNo, O);
}

void YZGPUInstPrinter::printSrc0Sel(const MCInst *MI, unsigned OpNo,
                                    const MCSubtargetInfo &STI,
                                    raw_ostream &O) {
  O << "src0_sel:";
  printOpSel(MI, OpNo, O);
}

void YZGPUInstPrinter::printSrc1Sel(const MCInst *MI, unsigned OpNo,
                                    const MCSubtargetInfo &STI,
                                    raw_ostream &O) {
  O << "src1_sel:";
  printOpSel(MI, OpNo, O);
}

void YZGPUInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                    const MCSubtargetInfo &STI, raw_ostream &O,
                                    const char *Modifier) {
  assert((Modifier == nullptr || Modifier[0] == 0) && "No modifiers supported");
  const MCOperand &MO = MI->getOperand(OpNo);

  if (MO.isReg()) {
    printRegName(O, MO.getReg());
    return;
  }

  if (MO.isImm()) {
    O << MO.getImm();
    return;
  }

  assert(MO.isExpr() && "Unknown operand kind in printOperand");
  MO.getExpr()->print(O, &MAI);
}
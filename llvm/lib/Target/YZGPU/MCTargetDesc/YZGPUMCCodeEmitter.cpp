//===-- YZGPUMCCodeEmitter.cpp - YZGPU Code Emitter -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
/// \file
/// The YZGPU code emitter produces machine code that can be executed
/// directly on the GPU device.
//
//===----------------------------------------------------------------------===//

// #include "MCTargetDesc/YZGPUFixupKinds.h"
#include "MCTargetDesc/YZGPUMCCodeEmitter.h"
#include "MCTargetDesc/YZGPUMCTargetDesc.h"
#include "YZGPUBaseInfo.h"
#include "llvm/ADT/APInt.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/SubtargetFeature.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/raw_ostream.h"
#include <optional>

using namespace llvm;

MCCodeEmitter *llvm::createYZGPUMCCodeEmitter(const MCInstrInfo &MCII,
                                              MCContext &Ctx) {
  return new YZGPUMCCodeEmitter(MCII, Ctx);
}

void YZGPUMCCodeEmitter::encodeInstruction(const MCInst &MI, raw_ostream &OS,
                                           SmallVectorImpl<MCFixup> &Fixups,
                                           const MCSubtargetInfo &STI) const {
  const MCInstrDesc &Desc = MCII.get(MI.getOpcode());
  // Get byte count of instruction.
  unsigned Size = Desc.getSize();

  switch (Size) {
  default:
    llvm_unreachable("Unhandled encodeInstruction length!");
  case 8: {
    uint64_t Bits = getBinaryCodeForInstr(MI, Fixups, STI);
    support::endian::write(OS, Bits, support::little);
    break;
  }
  }
}

unsigned
YZGPUMCCodeEmitter::getVSrc32Encoding(const MCInst &MI, unsigned OpNo,
                                      SmallVectorImpl<MCFixup> &Fixups,
                                      const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  unsigned RegEnc = 0;
  const MCRegisterInfo *MRI = Ctx.getRegisterInfo();
  if (MO.isReg()) {
    RegEnc |= MRI->getEncodingValue(MO.getReg());
    if (YZGPU::isSGPR(MO.getReg(), MRI))
      RegEnc |= YZGPURegMask::SRC_SGPR_MASK;
    else if (YZGPU::isVGPR(MO.getReg(), MRI))
      RegEnc |= YZGPURegMask::SRC_VGPR_MASK;
  }
  return RegEnc;
}

unsigned
YZGPUMCCodeEmitter::getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                                      SmallVectorImpl<MCFixup> &Fixups,
                                      const MCSubtargetInfo &STI) const {
  if (MO.isReg())
    return Ctx.getRegisterInfo()->getEncodingValue(MO.getReg());

  if (MO.isImm())
    return static_cast<unsigned>(MO.getImm());

  llvm_unreachable("Unhandled expression!");
  return 0;
}

#include "YZGPUGenMCCodeEmitter.inc"

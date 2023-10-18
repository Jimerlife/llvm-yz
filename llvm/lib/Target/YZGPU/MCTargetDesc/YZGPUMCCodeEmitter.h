//===-- YZGPUMCCodeEmitter.cpp - YZGPU Code Emitter interface -------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the YZGPUMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_YZGPU_MCTARGETDESC_YZGPUMCCODEEMITTER_H
#define LLVM_LIB_TARGET_YZGPU_MCTARGETDESC_YZGPUMCCODEEMITTER_H

// #include "MCTargetDesc/YZGPUFixupKinds.h"
// #include "MCTargetDesc/YZGPUMCExpr.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"

namespace llvm {

class MCInstrInfo;

class YZGPUMCCodeEmitter : public MCCodeEmitter {
  YZGPUMCCodeEmitter &operator=(const YZGPUMCCodeEmitter &) = delete;
  YZGPUMCCodeEmitter(const YZGPUMCCodeEmitter &) = delete;
  const MCInstrInfo &MCII;
  MCContext &Ctx;

public:
  YZGPUMCCodeEmitter(const MCInstrInfo &mcii, MCContext &ctx)
      : MCII(mcii), Ctx(ctx) {}

  ~YZGPUMCCodeEmitter() override = default;

  /// Encode the instruction and write it to the OS.
  void encodeInstruction(const MCInst &MI, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;

  /// TableGen'erated function for getting the binary encoding for an
  /// instruction.
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  /// Return binary encoding of operand. If the machine operand requires
  /// relocation, record the relocation and return zero.
  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

  unsigned getVSrc32Encoding(const MCInst &MI, unsigned OpNo,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_YZGPU_MCTARGETDESC_YZGPUMCCODEEMITTER_H

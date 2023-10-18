//===-- YZGPUAsmBackend.h - YZGPU Assembler Backend -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_YZGPU_MCTARGETDESC_YZGPUASMBACKEND_H
#define LLVM_LIB_TARGET_YZGPU_MCTARGETDESC_YZGPUASMBACKEND_H

// #include "MCTargetDesc/YZGPUBaseInfo.h"
// #include "MCTargetDesc/YZGPUFixupKinds.h"
#include "MCTargetDesc/YZGPUMCTargetDesc.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCFixupKindInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"

namespace llvm {
class MCAssembler;
class MCObjectTargetWriter;
class raw_ostream;

class YZGPUAsmBackend : public MCAsmBackend {
  const MCSubtargetInfo &STI;

public:
  YZGPUAsmBackend(const MCSubtargetInfo &STI)
      : MCAsmBackend(support::little), STI(STI) {}

  unsigned getNumFixupKinds() const override;

  void applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                  const MCValue &Target, MutableArrayRef<char> Data,
                  uint64_t Value, bool IsResolved,
                  const MCSubtargetInfo *STI) const override;
  bool fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
                            const MCRelaxableFragment *DF,
                            const MCAsmLayout &Layout) const override;

  void relaxInstruction(MCInst &Inst,
                        const MCSubtargetInfo &STI) const override;

  bool mayNeedRelaxation(const MCInst &Inst,
                         const MCSubtargetInfo &STI) const override;

  unsigned getMinimumNopSize() const override;
  bool writeNopData(raw_ostream &OS, uint64_t Count,
                    const MCSubtargetInfo *STI) const override;

  std::optional<MCFixupKind> getFixupKind(StringRef Name) const override;
  const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override;

  bool shouldForceRelocation(const MCAssembler &Asm, const MCFixup &Fixup,
                             const MCValue &Target) override;

  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override;
};
} // namespace llvm

#endif

//===-- YZGPUAsmBackend.cpp - YZGPU Assembler Backend -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
/// \file
//===----------------------------------------------------------------------===//

// #include "MCTargetDesc/YZGPUFixupKinds.h"
#include "YZGPUAsmBackend.h"
#include "MCTargetDesc/YZGPUMCTargetDesc.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCFixupKindInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/TargetParser.h"

using namespace llvm;

// FIXME: need add fixup kind
std::optional<MCFixupKind> YZGPUAsmBackend::getFixupKind(StringRef Name) const {
  return std::nullopt;
}

// FIXME: need add fixup kind
const MCFixupKindInfo &
YZGPUAsmBackend::getFixupKindInfo(MCFixupKind Kind) const {
  return MCAsmBackend::getFixupKindInfo(FK_NONE);
}

void YZGPUAsmBackend::relaxInstruction(MCInst &Inst,
                                       const MCSubtargetInfo &STI) const {}

bool YZGPUAsmBackend::fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
                                           const MCRelaxableFragment *DF,
                                           const MCAsmLayout &Layout) const {
  return false;
}

bool YZGPUAsmBackend::mayNeedRelaxation(const MCInst &Inst,
                                        const MCSubtargetInfo &STI) const {
  return false;
}

void YZGPUAsmBackend::applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                                 const MCValue &Target,
                                 MutableArrayRef<char> Data, uint64_t Value,
                                 bool IsResolved,
                                 const MCSubtargetInfo *STI) const {}

bool YZGPUAsmBackend::shouldForceRelocation(const MCAssembler &,
                                            const MCFixup &Fixup,
                                            const MCValue &) {
  return false;
}

unsigned YZGPUAsmBackend::getNumFixupKinds() const { return 0; }

unsigned YZGPUAsmBackend::getMinimumNopSize() const { return 4; }

bool YZGPUAsmBackend::writeNopData(raw_ostream &OS, uint64_t Count,
                                   const MCSubtargetInfo *STI) const {
  OS.write_zeros(Count);
  return true;
}

MCAsmBackend *llvm::createYZGPUAsmBackend(const Target &T,
                                          const MCSubtargetInfo &STI,
                                          const MCRegisterInfo &MRI,
                                          const MCTargetOptions &Options) {
  return new YZGPUAsmBackend(STI);
}

std::unique_ptr<MCObjectTargetWriter>
YZGPUAsmBackend::createObjectTargetWriter() const {
  return createYZGPUELFObjectWriter();
}
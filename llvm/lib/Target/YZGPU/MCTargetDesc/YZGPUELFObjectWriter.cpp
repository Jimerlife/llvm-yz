//===-- YZGPUELFObjectWriter.cpp - YZGPU ELF Writer -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// #include "MCTargetDesc/YZGPUFixupKinds.h"
// #include "MCTargetDesc/YZGPUMCExpr.h"
#include "MCTargetDesc/YZGPUMCTargetDesc.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {
class YZGPUELFObjectWriter : public MCELFObjectTargetWriter {
public:
  YZGPUELFObjectWriter(uint8_t OSABI = 0, bool Is64Bit = true)
      : MCELFObjectTargetWriter(Is64Bit, OSABI, ELF::EM_YZGPU,
                                /*HasRelocationAddend*/ true) {}
  ~YZGPUELFObjectWriter() {}

  // Return true if the given relocation must be with a symbol rather than
  // section plus offset.
  bool needsRelocateWithSymbol(const MCSymbol &Sym,
                               unsigned Type) const override {
    // TODO: this is very conservative, update once YZGPU psABI requirements
    //       are clarified.
    return true;
  }

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
};
} // namespace

unsigned YZGPUELFObjectWriter::getRelocType(MCContext &Ctx,
                                            const MCValue &Target,
                                            const MCFixup &Fixup,
                                            bool IsPCRel) const {

  return 0;
}

std::unique_ptr<MCObjectTargetWriter> llvm::createYZGPUELFObjectWriter() {
  return std::make_unique<YZGPUELFObjectWriter>();
}

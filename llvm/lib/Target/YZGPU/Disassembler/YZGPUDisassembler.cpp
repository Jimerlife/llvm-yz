//===-- YZGPUDisassembler.cpp - Disassembler for YZGPU --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the YZGPUDisassembler class.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/YZGPUBaseInfo.h"
#include "MCTargetDesc/YZGPUMCTargetDesc.h"
#include "TargetInfo/YZGPUTargetInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCDecoderOps.h"
#include "llvm/MC/MCDisassembler/MCDisassembler.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Endian.h"

using namespace llvm;

#define DEBUG_TYPE "yzgpu-disassembler"

typedef MCDisassembler::DecodeStatus DecodeStatus;

namespace {
class YZGPUDisassembler : public MCDisassembler {
  std::unique_ptr<MCInstrInfo const> const MCII;

public:
  YZGPUDisassembler(const MCSubtargetInfo &STI, MCContext &Ctx,
                    MCInstrInfo const *MCII)
      : MCDisassembler(STI, Ctx), MCII(MCII) {}

  DecodeStatus getInstruction(MCInst &Instr, uint64_t &Size,
                              ArrayRef<uint8_t> Bytes, uint64_t Address,
                              raw_ostream &CStream) const override;
};
} // end namespace

static MCDisassembler *createYZGPUDisassembler(const Target &T,
                                               const MCSubtargetInfo &STI,
                                               MCContext &Ctx) {
  return new YZGPUDisassembler(STI, Ctx, T.createMCInstrInfo());
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeYZGPUDisassembler() {
  // Register the disassembler for each target.
  TargetRegistry::RegisterMCDisassembler(getTheYZGPUTarget(),
                                         createYZGPUDisassembler);
}

static DecodeStatus DecodeVGPR_32RegisterClass(MCInst &Inst, uint64_t RegNo,
                                               uint64_t Address,
                                               const MCDisassembler *Decoder) {
  if (RegNo >= 256)
    return MCDisassembler::Fail;
  Inst.addOperand(MCOperand::createReg(YZGPU::VGPR0 + RegNo));
  return MCDisassembler::Success;
}

static DecodeStatus DecodeSGPR_32RegisterClass(MCInst &Inst, uint64_t RegNo,
                                               uint64_t Address,
                                               const MCDisassembler *Decoder) {
  if (RegNo >= 128)
    return MCDisassembler::Fail;
  Inst.addOperand(MCOperand::createReg(YZGPU::SGPR0 + RegNo));
  return MCDisassembler::Success;
}

// FIXME: May need update CGPR/AGPR
static DecodeStatus decodeOperand_VSrc32(MCInst &Inst, uint64_t Val,
                                         uint64_t Address,
                                         const MCDisassembler *Decoder) {
  if (Val & YZGPURegMask::SRC_SGPR_MASK) {
    Val &= 0xFF;
    Inst.addOperand(MCOperand::createReg(YZGPU::SGPR0 + Val));
    return MCDisassembler::Success;
  } else if (Val & YZGPURegMask::SRC_VGPR_MASK) {
    Val &= 0xFF;
    Inst.addOperand(MCOperand::createReg(YZGPU::VGPR0 + Val));
    return MCDisassembler::Success;
  }

  return MCDisassembler::Fail;
}

#include "YZGPUGenDisassemblerTables.inc"

DecodeStatus YZGPUDisassembler::getInstruction(MCInst &MI, uint64_t &Size,
                                               ArrayRef<uint8_t> Bytes,
                                               uint64_t Address,
                                               raw_ostream &CS) const {
  uint64_t Insn;
  DecodeStatus Result;

  // We want to read exactly 8 bytes of data because all YZGPU instructions
  // are fixed 64 bits.
  if (Bytes.size() < 8) {
    Size = 0;
    return MCDisassembler::Fail;
  }

  Insn = support::endian::read64le(Bytes.data());
  // Calling the auto-generated decoder function.
  Result = decodeInstruction(DecoderTable64, MI, Insn, Address, this, STI);
  Size = 8;

  return Result;
}

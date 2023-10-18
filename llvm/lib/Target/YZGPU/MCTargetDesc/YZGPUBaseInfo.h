//===-- YZGPUBaseInfo.h - Top level definitions for YZGPU MC ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains small standalone enum definitions for the YZGPU target
// useful for the compiler back-end and the MC libraries.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_LIB_TARGET_YZGPU_MCTARGETDESC_YZGPUBASEINFO_H
#define LLVM_LIB_TARGET_YZGPU_MCTARGETDESC_YZGPUBASEINFO_H

#include "MCTargetDesc/YZGPUMCTargetDesc.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/MC/MCInstrDesc.h"
#include "llvm/MC/MCRegister.h"
#include "llvm/MC/SubtargetFeature.h"

namespace llvm {
class MCRegisterClass;
class MCRegisterInfo;

namespace YZGPU {
enum OperandType : unsigned {
  OPERAND_FIRST_YZGPU_IMM = MCOI::OPERAND_FIRST_TARGET,
  OPERAND_INPUT_MODS,
  Operand_INT32,
  Operand_INT16,
};

bool isSGPR(unsigned Reg, const MCRegisterInfo *MRI);

bool isVGPR(unsigned Reg, const MCRegisterInfo *MRI);

} // namespace YZGPU

namespace YZGPUSrcMods {
enum SrcModsKind : unsigned {
  NEG = 1 << 0, // negate modifier
  ABS = 1 << 1, // absolute modifier
};
}

namespace YZGPURegMask {
enum SrcRegMask : unsigned {
  SRC_SGPR_MASK = 0x100,
  SRC_VGPR_MASK = 0x200,
};
}

namespace ALUOpSel {
enum OpSel : unsigned {
  WORD_0 = 1,
  WORD_1 = 4,
};
}

} // namespace llvm

#endif

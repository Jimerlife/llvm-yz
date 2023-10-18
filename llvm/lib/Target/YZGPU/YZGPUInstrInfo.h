//===-- YZGPUInstrInfo.h - YZGPU Instruction Information --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the YZGPU implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_YZGPU_YZGPUINSTRINFO_H
#define LLVM_LIB_TARGET_YZGPU_YZGPUINSTRINFO_H

#include "YZGPURegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/IR/DiagnosticInfo.h"

#define GET_INSTRINFO_HEADER
#define GET_INSTRINFO_OPERAND_ENUM
#include "YZGPUGenInstrInfo.inc"

namespace llvm {

class YZGPUSubtarget;

class YZGPUInstrInfo : public YZGPUGenInstrInfo {

public:
  explicit YZGPUInstrInfo(YZGPUSubtarget &STI);

protected:
  const YZGPUSubtarget &STI;
};

} // end namespace llvm
#endif
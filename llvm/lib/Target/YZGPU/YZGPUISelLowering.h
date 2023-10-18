//===-- YZGPUISelLowering.h - YZGPU DAG Lowering Interface ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that YZGPU uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_YZGPU_YZGPUISELLOWERING_H
#define LLVM_LIB_TARGET_YZGPU_YZGPUISELLOWERING_H

#include "YZGPU.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"
// #include "llvm/TargetParser/YZGPUTargetParser.h"
#include <optional>

namespace llvm {
class YZGPUSubtarget;
struct YZGPURegisterInfo;
namespace YZGPUISD {
enum NodeType : unsigned {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,
};
} // namespace YZGPUISD

class YZGPUTargetLowering : public TargetLowering {
  const YZGPUSubtarget &Subtarget;

public:
  explicit YZGPUTargetLowering(const TargetMachine &TM,
                               const YZGPUSubtarget &STI);

  const YZGPUSubtarget &getSubtarget() const { return Subtarget; }
};

} // end namespace llvm
#endif

//===-- YZGPUTargetInfo.cpp - YZGPU Target Implementation -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/YZGPUTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
using namespace llvm;

Target &llvm::getTheYZGPUTarget() {
  static Target TheYZGPUTarget;
  return TheYZGPUTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeYZGPUTargetInfo() {
  RegisterTarget<Triple::yzgpu, /*HasJIT=*/false> X(
      getTheYZGPUTarget(), "yzgpu", "YZ ReRam GPU", "YZGPU");
}

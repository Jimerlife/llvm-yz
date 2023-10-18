//===-- YZGPUSubtarget.cpp - YZGPU Subtarget Information ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the YZGPU specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "YZGPUSubtarget.h"
#include "YZGPU.h"
#include "YZGPUFrameLowering.h"
#include "YZGPUTargetMachine.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define DEBUG_TYPE "yzgpu-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "YZGPUGenSubtargetInfo.inc"

YZGPUSubtarget &YZGPUSubtarget::initializeSubtargetDependencies(
    const Triple &TT, StringRef CPU, StringRef TuneCPU, StringRef FS) {
  // Determine default and user-specified characteristics
  if (CPU.empty() || CPU == "reram")
    CPU = "reram-gpu";

  if (TuneCPU.empty())
    TuneCPU = CPU;

  ParseSubtargetFeatures(CPU, TuneCPU, FS);
  return *this;
}

YZGPUSubtarget::YZGPUSubtarget(const Triple &TT, StringRef CPU,
                               StringRef TuneCPU, StringRef FS,
                               const TargetMachine &TM)
    : YZGPUGenSubtargetInfo(TT, CPU, TuneCPU, FS),
      FrameLowering(initializeSubtargetDependencies(TT, CPU, TuneCPU, FS)),
      InstrInfo(*this), RegInfo(getHwMode()), TLInfo(TM, *this) {}
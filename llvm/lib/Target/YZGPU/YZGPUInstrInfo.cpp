//===-- YZGPUInstrInfo.cpp - YZGPU Instruction Information ------*- C++ -*-===//
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

#include "YZGPUInstrInfo.h"
// #include "MCTargetDesc/YZGPUMatInt.h"
// #include "YZGPU.h"
// #include "YZGPUMachineFunctionInfo.h"
#include "YZGPUSubtarget.h"
#include "YZGPUTargetMachine.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Analysis/MemoryLocation.h"
#include "llvm/CodeGen/LiveIntervals.h"
#include "llvm/CodeGen/LiveVariables.h"
#include "llvm/CodeGen/MachineCombinerPattern.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/MC/MCInstBuilder.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#define GET_INSTRINFO_NAMED_OPS
#include "YZGPUGenInstrInfo.inc"

YZGPUInstrInfo::YZGPUInstrInfo(YZGPUSubtarget &STI)
    : YZGPUGenInstrInfo(), STI(STI) {}

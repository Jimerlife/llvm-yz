//===-- YZGPUFrameLowering.cpp - YZGPU Frame Information ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the YZGPU implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "YZGPUFrameLowering.h"
// #include "YZGPUMachineFunctionInfo.h"
#include "YZGPUSubtarget.h"
#include "llvm/BinaryFormat/Dwarf.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/DiagnosticInfo.h"
#include "llvm/MC/MCDwarf.h"
#include "llvm/Support/LEB128.h"

#include <algorithm>

using namespace llvm;

// Return true if the specified function should have a dedicated frame
// pointer register.  This is true if frame pointer elimination is
// disabled, if it needs dynamic stack realignment, if the function has
// variable sized allocas, or if the frame address is taken.
bool YZGPUFrameLowering::hasFP(const MachineFunction &MF) const {
  return false;
}

bool YZGPUFrameLowering::hasBP(const MachineFunction &MF) const {
  return false;
}

void YZGPUFrameLowering::emitPrologue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {}

void YZGPUFrameLowering::emitEpilogue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {}
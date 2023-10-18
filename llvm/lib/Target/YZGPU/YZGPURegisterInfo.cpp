//===-- YZGPURegisterInfo.cpp - YZGPU Register Information ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the YZGPU implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "YZGPURegisterInfo.h"
#include "MCTargetDesc/YZGPUMCTargetDesc.h"
#include "YZGPU.h"
// #include "YZGPUMachineFunctionInfo.h"
#include "YZGPUSubtarget.h"
#include "llvm/BinaryFormat/Dwarf.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/Support/ErrorHandling.h"

#define GET_REGINFO_TARGET_DESC
#include "YZGPUGenRegisterInfo.inc"

using namespace llvm;

YZGPURegisterInfo::YZGPURegisterInfo(unsigned HwMode)
    : YZGPUGenRegisterInfo(0, /*DwarfFlavour*/ 0, /*EHFlavor*/ 0,
                           /*PC*/ 0, HwMode) {}

const MCPhysReg *
YZGPURegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  // auto &Subtarget = MF->getSubtarget<YZGPUSubtarget>();
  return nullptr;
}

BitVector YZGPURegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  markSuperRegs(Reserved, 1);
  return Reserved;
}

bool YZGPURegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                            int SPAdj, unsigned FIOperandNum,
                                            RegScavenger *RS) const {
  return false;
}

// FIXME: set right frameregister
Register YZGPURegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  // return YZGPU::SGPR2;
  return 2;
}

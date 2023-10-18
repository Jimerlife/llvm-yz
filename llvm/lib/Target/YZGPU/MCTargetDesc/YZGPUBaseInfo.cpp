//===-- YZGPUBaseInfo.cpp - Top level definitions for YZGPU MC ------------===//
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

#include "YZGPUBaseInfo.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/Triple.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/TargetParser.h"
#include "llvm/Support/raw_ostream.h"

namespace llvm {

namespace YZGPU {

bool isSGPR(unsigned Reg, const MCRegisterInfo *MRI) {
  const MCRegisterClass SGPRClass = MRI->getRegClass(YZGPU::SGPR_32RegClassID);
  return SGPRClass.contains(Reg);
}

bool isVGPR(unsigned Reg, const MCRegisterInfo *MRI) {
  const MCRegisterClass VGPRClass = MRI->getRegClass(YZGPU::VGPR_32RegClassID);
  return VGPRClass.contains(Reg);
}

} // namespace YZGPU
} // namespace llvm
//===-- YZGPUFrameLowering.h - Define frame lowering for YZGPU -*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This class implements YZGPU-specific bits of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_YZGPU_YZGPUFRAMELOWERING_H
#define LLVM_LIB_TARGET_YZGPU_YZGPUFRAMELOWERING_H

#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Support/TypeSize.h"

namespace llvm {
class YZGPUSubtarget;

class YZGPUFrameLowering : public TargetFrameLowering {
public:
  explicit YZGPUFrameLowering(const YZGPUSubtarget &STI)
      : TargetFrameLowering(StackGrowsDown,
                            /*StackAlignment=*/Align(16),
                            /*LocalAreaOffset=*/0,
                            /*TransientStackAlignment=*/Align(16)),
        STI(STI) {}

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  bool hasFP(const MachineFunction &MF) const override;

  bool hasBP(const MachineFunction &MF) const;

protected:
  const YZGPUSubtarget &STI;
};
} // namespace llvm
#endif

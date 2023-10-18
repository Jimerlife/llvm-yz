//===-- YZGPUMCAsmInfo.h - YZGPU Asm Info ----------------------*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the YZGPUMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_YZGPU_MCTARGETDESC_YZGPUMCASMINFO_H
#define LLVM_LIB_TARGET_YZGPU_MCTARGETDESC_YZGPUMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class YZGPUMCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit YZGPUMCAsmInfo(const Triple &TargetTriple);
};

} // namespace llvm

#endif

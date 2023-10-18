//===-- YZGPUTargetStreamer.h - YZGPU Target Streamer --------*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_YZGPU_MCTARGETDESC_YZGPUTARGETSTREAMER_H
#define LLVM_LIB_TARGET_YZGPU_MCTARGETDESC_YZGPUTARGETSTREAMER_H

#include "YZGPU.h"
#include "YZGPUELFStreamer.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"

// FIXME: metadata and kernel_code
using namespace llvm;

namespace llvm {

class YZGPUELFStreamer;
class formatted_raw_ostream;

class YZGPUTargetStreamer : public MCTargetStreamer {
protected:
  MCContext &getContext() const;

public:
  YZGPUTargetStreamer(MCStreamer &S);
};

// This part is for ascii assembly output
class YZGPUTargetAsmStreamer final : public YZGPUTargetStreamer {
  formatted_raw_ostream &OS;

public:
  YZGPUTargetAsmStreamer(MCStreamer &S, formatted_raw_ostream &OS);

  void finish() override;
};

class YZGPUTargetELFStreamer final : public YZGPUTargetStreamer {
  const MCSubtargetInfo &STI;
  MCStreamer &Streamer;

public:
  YZGPUTargetELFStreamer(MCStreamer &S, const MCSubtargetInfo &STI);

  YZGPUELFStreamer &getStreamer();

  void finish() override;
};
} // namespace llvm
#endif

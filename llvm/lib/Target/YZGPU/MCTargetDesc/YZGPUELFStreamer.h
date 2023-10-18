//===-------- YZGPUELFStreamer.h - ELF Object Output -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This is a custom MCELFStreamer which allows us to insert some hooks before
// emitting data into an actual object file.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_YZGPU_MCTARGETDESC_YZGPUELFSTREAMER_H
#define LLVM_LIB_TARGET_YZGPU_MCTARGETDESC_YZGPUELFSTREAMER_H

#include "YZGPUTargetStreamer.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCObjectWriter.h"

#include <memory>
namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCELFStreamer;
class MCObjectWriter;

class YZGPUELFStreamer : public MCELFStreamer {

public:
  YZGPUELFStreamer(MCContext &C, std::unique_ptr<MCAsmBackend> MAB,
                   std::unique_ptr<MCObjectWriter> MOW,
                   std::unique_ptr<MCCodeEmitter> MCE)
      : MCELFStreamer(C, std::move(MAB), std::move(MOW), std::move(MCE)) {}
};

MCELFStreamer *createYZGPUELFStreamer(MCContext &C,
                                      std::unique_ptr<MCAsmBackend> MAB,
                                      std::unique_ptr<MCObjectWriter> MOW,
                                      std::unique_ptr<MCCodeEmitter> MCE,
                                      bool RelaxAll);
} // namespace llvm.

#endif

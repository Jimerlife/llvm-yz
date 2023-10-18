//===-- YZGPUTargetStreamer.cpp - YZGPU Target Streamer Methods -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides YZGPU specific target streamer methods.
//
//===----------------------------------------------------------------------===//

#include "YZGPUTargetStreamer.h"
#include "YZGPUMCTargetDesc.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCSectionELF.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetParser.h"

using namespace llvm;

//===----------------------------------------------------------------------===//
// YZGPUTargetStreamer
//===----------------------------------------------------------------------===//

YZGPUTargetStreamer::YZGPUTargetStreamer(MCStreamer &S) : MCTargetStreamer(S) {}

MCContext &YZGPUTargetStreamer::getContext() const {
  return Streamer.getContext();
}

//===----------------------------------------------------------------------===//
// YZGPUTargetAsmStreamer
//===----------------------------------------------------------------------===//

YZGPUTargetAsmStreamer::YZGPUTargetAsmStreamer(MCStreamer &S,
                                               formatted_raw_ostream &OS)
    : YZGPUTargetStreamer(S), OS(OS) {}

// A hook for emitting stuff at the end.
// We use it for emitting the accumulated PAL metadata as directives.
// The PAL metadata is reset after it is emitted.
void YZGPUTargetAsmStreamer::finish() {}

//===----------------------------------------------------------------------===//
// YZGPUTargetELFStreamer
//===----------------------------------------------------------------------===//

YZGPUTargetELFStreamer::YZGPUTargetELFStreamer(MCStreamer &S,
                                               const MCSubtargetInfo &STI)
    : YZGPUTargetStreamer(S), STI(STI), Streamer(S) {}

YZGPUELFStreamer &YZGPUTargetELFStreamer::getStreamer() {
  return static_cast<YZGPUELFStreamer &>(Streamer);
}

// A hook for emitting stuff at the end.
// We use it for emitting the accumulated PAL metadata as a .note record.
// The PAL metadata is reset after it is emitted.
void YZGPUTargetELFStreamer::finish() {}
//===-------- YZGPUELFStreamer.cpp - ELF Object Output -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "YZGPUELFStreamer.h"

using namespace llvm;

MCELFStreamer *llvm::createYZGPUELFStreamer(MCContext &C,
                                            std::unique_ptr<MCAsmBackend> MAB,
                                            std::unique_ptr<MCObjectWriter> MOW,
                                            std::unique_ptr<MCCodeEmitter> MCE,
                                            bool RelaxAll) {
  return new YZGPUELFStreamer(C, std::move(MAB), std::move(MOW),
                              std::move(MCE));
}

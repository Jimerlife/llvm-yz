//===-- YZGPUMCAsmInfo.cpp - YZGPU Asm properties -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the YZGPUMCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "YZGPUMCAsmInfo.h"
// #include "MCTargetDesc/YZGPUMCExpr.h"
#include "llvm/ADT/Triple.h"
#include "llvm/BinaryFormat/Dwarf.h"
#include "llvm/MC/MCStreamer.h"
using namespace llvm;

void YZGPUMCAsmInfo::anchor() {}

YZGPUMCAsmInfo::YZGPUMCAsmInfo(const Triple &TT) {
  CodePointerSize = 8;
  CommentString = "#";
  AlignmentIsInBytes = false;
  SupportsDebugInformation = true;
  ExceptionsType = ExceptionHandling::DwarfCFI;
  Data16bitsDirective = "\t.half\t";
  Data32bitsDirective = "\t.word\t";
}
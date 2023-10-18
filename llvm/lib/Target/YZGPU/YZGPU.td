//===-- YZGPU.td - Describe the YZGPU Target Machine -------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Registers, calling conventions, instruction descriptions.
//===----------------------------------------------------------------------===//

include "YZGPURegisterInfo.td"
include "YZGPUInstrInfo.td"

//===----------------------------------------------------------------------===//
// Define the YZGPU target.
//===----------------------------------------------------------------------===//

def YZGPUInstrInfo : InstrInfo {
  let guessInstructionProperties = 0;
}

def YZGPUAsmParser : AsmParser {
  let AllowDuplicateRegisterNames = 1;
}

def YZGPUAsmWriter : AsmWriter {
  int PassSubtarget = 1;
}

def YZGPU : Target {
  let InstructionSet = YZGPUInstrInfo;
  let AssemblyParsers = [YZGPUAsmParser];
  let AssemblyWriters = [YZGPUAsmWriter];
  let AllowRegisterRenaming = 1;
}

//===------------------------------------------------------------===//
// YZGPU ProcessorModel.
//===------------------------------------------------------------===//

def : ProcessorModel<"reram-gpu", NoSchedModel, []>;
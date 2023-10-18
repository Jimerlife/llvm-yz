//===-- YZGPU.h - Top-level interface for YZGPU -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// YZGPU back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_YZGPU_YZGPU_H
#define LLVM_LIB_TARGET_YZGPU_YZGPU_H

// #include "MCTargetDesc/YZGPUBaseInfo.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class AsmPrinter;
class FunctionPass;
class InstructionSelector;
class MCInst;
class MCOperand;
class MachineInstr;
class MachineOperand;
class PassRegistry;
class YZGPUSubtarget;
class YZGPUTargetMachine;

// FunctionPass *createYZGPUCodeGenPreparePass();
// void initializeYZGPUCodeGenPreparePass(PassRegistry &);

// bool lowerYZGPUMachineInstrToMCInst(const MachineInstr *MI, MCInst &OutMI,
//                                     AsmPrinter &AP);
// bool lowerYZGPUMachineOperandToMCOperand(const MachineOperand &MO,
//                                          MCOperand &MCOp, const AsmPrinter
//                                          &AP);

// InstructionSelector *createYZGPUInstructionSelector(const YZGPUTargetMachine
// &,
//                                                     YZGPUSubtarget &,
//                                                     YZGPURegisterBankInfo &);
// void initializeYZGPUDAGToDAGISelPass(PassRegistry &);
} // namespace llvm

#endif

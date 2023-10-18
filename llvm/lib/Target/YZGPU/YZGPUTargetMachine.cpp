//===-- YZGPUTargetMachine.cpp - TargetMachine for hw codegen targets-----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
/// \file
/// Implements the info about YZGPU target spec.
//
//===----------------------------------------------------------------------===//

#include "YZGPUTargetMachine.h"
// #include "YZGPU.h"
#include "YZGPUTargetObjectFile.h"
// #include "YZGPUTargetTransformInfo.h"
#include "TargetInfo/YZGPUTargetInfo.h"
#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/CodeGen/GlobalISel/CSEInfo.h"
#include "llvm/CodeGen/GlobalISel/IRTranslator.h"
#include "llvm/CodeGen/GlobalISel/InstructionSelect.h"
#include "llvm/CodeGen/GlobalISel/Legalizer.h"
#include "llvm/CodeGen/GlobalISel/Localizer.h"
#include "llvm/CodeGen/GlobalISel/RegBankSelect.h"
#include "llvm/CodeGen/MIRParser/MIParser.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/RegAllocRegistry.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/PatternMatch.h"
#include "llvm/InitializePasses.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/IPO/AlwaysInliner.h"
#include "llvm/Transforms/IPO/GlobalDCE.h"
#include "llvm/Transforms/IPO/Internalize.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/InferAddressSpaces.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/Utils/SimplifyLibCalls.h"
#include "llvm/Transforms/Vectorize.h"
#include <optional>

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeYZGPUTarget() {
  // Register the target
  RegisterTargetMachine<YZGPUTargetMachine> X(getTheYZGPUTarget());
}

static std::unique_ptr<TargetLoweringObjectFile> createTLOF(const Triple &TT) {
  return std::make_unique<YZGPUTargetObjectFile>();
}

// FIXME: DataLayout not right
static StringRef computeDataLayout(const Triple &TT) {
  // 32-bit private, local, and region pointers. 64-bit global, constant and
  // flat, non-integral buffer fat pointers.
  return "e-p:64:64-p1:64:64-p2:32:32-p3:32:32-p4:64:64-p5:32:32-p6:32:32"
         "-i64:64-v16:16-v24:32-v32:32-v48:64-v96:128"
         "-v192:256-v256:256-v512:512-v1024:1024-v2048:2048-n32:64-S32-A5-G1"
         "-ni:7";
}

// FIXME: RelocModel may not right
static Reloc::Model getEffectiveRelocModel(std::optional<Reloc::Model> RM) {
  // The YZGPU toolchain only supports generating shared objects, so we
  // must always use PIC.
  return Reloc::PIC_;
}

YZGPUTargetMachine::YZGPUTargetMachine(const Target &T, const Triple &TT,
                                       StringRef CPU, StringRef FS,
                                       TargetOptions Options,
                                       std::optional<Reloc::Model> RM,
                                       std::optional<CodeModel::Model> CM,
                                       CodeGenOpt::Level OptLevel, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT), TT, CPU, FS, Options,
                        getEffectiveRelocModel(RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OptLevel),
      TLOF(createTLOF(getTargetTriple())) {
  initAsmInfo();
}

const YZGPUSubtarget *
YZGPUTargetMachine::getSubtargetImpl(const Function &F) const {
  return nullptr;
}

TargetPassConfig *YZGPUTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new YZGPUPassConfig(*this, PM);
}

MachineFunctionInfo *YZGPUTargetMachine::createMachineFunctionInfo(
    BumpPtrAllocator &Allocator, const Function &F,
    const TargetSubtargetInfo *STI) const {
  return nullptr;
}

bool YZGPUTargetMachine::isNoopAddrSpaceCast(unsigned SrcAS,
                                             unsigned DstAS) const {
  return false;
}

yaml::MachineFunctionInfo *
YZGPUTargetMachine::createDefaultFuncInfoYAML() const {
  return nullptr;
}

yaml::MachineFunctionInfo *
YZGPUTargetMachine::convertFuncInfoToYAML(const MachineFunction &MF) const {
  return nullptr;
}

bool YZGPUTargetMachine::parseMachineFunctionInfo(
    const yaml::MachineFunctionInfo &, PerFunctionMIParsingState &PFS,
    SMDiagnostic &Error, SMRange &SourceRange) const {
  return false;
}
//===----------------------------------------------------------------------===//
// YZGPU Pass Setup
//===----------------------------------------------------------------------===//

ScheduleDAGInstrs *
YZGPUPassConfig::createMachineScheduler(MachineSchedContext *C) const {
  return nullptr;
}

void YZGPUPassConfig::addIRPasses() {}

void YZGPUPassConfig::addCodeGenPrepare() {}

bool YZGPUPassConfig::addPreISel() { return false; }

bool YZGPUPassConfig::addInstSelector() { return false; }

bool YZGPUPassConfig::addGCPasses() { return false; }

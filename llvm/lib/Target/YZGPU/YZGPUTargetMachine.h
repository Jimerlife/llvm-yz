//===-- YZGPUTargetMachine.h - YZGPU TargetMachine Interface --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
/// \file
/// The YZGPU TargetMachine interface definition for hw codegen targets.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_YZGPU_YZGPUTARGETMACHINE_H
#define LLVM_LIB_TARGET_YZGPU_YZGPUTARGETMACHINE_H

#include "YZGPUSubtarget.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Target/TargetMachine.h"
#include <optional>
#include <utility>

namespace llvm {

//===----------------------------------------------------------------------===//
// YZGPU Target Machine
//===----------------------------------------------------------------------===//

class YZGPUTargetMachine : public LLVMTargetMachine {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  mutable StringMap<std::unique_ptr<YZGPUSubtarget>> SubtargetMap;

public:
  YZGPUTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                     StringRef FS, TargetOptions Options,
                     std::optional<Reloc::Model> RM,
                     std::optional<CodeModel::Model> CM, CodeGenOpt::Level OL,
                     bool JIT);

  const YZGPUSubtarget *getSubtargetImpl(const Function &F) const override;
  // DO NOT IMPLEMENT: There is no such thing as a valid default subtarget,
  // subtargets are per-function entities based on the target-specific
  // attributes of each function.
  const YZGPUSubtarget *getSubtargetImpl() const = delete;

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

  MachineFunctionInfo *
  createMachineFunctionInfo(BumpPtrAllocator &Allocator, const Function &F,
                            const TargetSubtargetInfo *STI) const override;

  // TargetTransformInfo getTargetTransformInfo(const Function &F) const
  // override;

  bool isNoopAddrSpaceCast(unsigned SrcAS, unsigned DstAS) const override;

  yaml::MachineFunctionInfo *createDefaultFuncInfoYAML() const override;
  yaml::MachineFunctionInfo *
  convertFuncInfoToYAML(const MachineFunction &MF) const override;
  bool parseMachineFunctionInfo(const yaml::MachineFunctionInfo &,
                                PerFunctionMIParsingState &PFS,
                                SMDiagnostic &Error,
                                SMRange &SourceRange) const override;
};

//===----------------------------------------------------------------------===//
// YZGPU Pass Setup
//===----------------------------------------------------------------------===//

class YZGPUPassConfig : public TargetPassConfig {
public:
  YZGPUPassConfig(LLVMTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  YZGPUTargetMachine &getYZGPUTargetMachine() const {
    return getTM<YZGPUTargetMachine>();
  }

  ScheduleDAGInstrs *
  createMachineScheduler(MachineSchedContext *C) const override;

  void addIRPasses() override;
  void addCodeGenPrepare() override;
  bool addPreISel() override;
  bool addInstSelector() override;
  bool addGCPasses() override;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_YZGPU_YZGPUTARGETMACHINE_H

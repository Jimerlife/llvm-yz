//===-- YZGPUMCTargetDesc.cpp - YZGPU Target Descriptions -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// This file provides YZGPU-specific target descriptions.
///
//===----------------------------------------------------------------------===//

#include "YZGPUMCTargetDesc.h"
#include "TargetInfo/YZGPUTargetInfo.h"
#include "YZGPUBaseInfo.h"
#include "YZGPUELFStreamer.h"
#include "YZGPUInstPrinter.h"
#include "YZGPUMCAsmInfo.h"
#include "YZGPUTargetStreamer.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCObjectFileInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"

#define GET_INSTRINFO_MC_DESC
#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "YZGPUGenInstrInfo.inc"

#define GET_REGINFO_MC_DESC
#include "YZGPUGenRegisterInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "YZGPUGenSubtargetInfo.inc"

using namespace llvm;

static MCInstrInfo *createYZGPUMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitYZGPUMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createYZGPUMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitYZGPUMCRegisterInfo(X, YZGPU::SGPR0);
  return X;
}

static MCAsmInfo *createYZGPUMCAsmInfo(const MCRegisterInfo &MRI,
                                       const Triple &TT,
                                       const MCTargetOptions &Options) {

  MCAsmInfo *MAI = new YZGPUMCAsmInfo(TT);
  return MAI;
  ;
}

static MCSubtargetInfo *
createYZGPUMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  return createYZGPUMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

static MCInstPrinter *createYZGPUMCInstPrinter(const Triple &T,
                                               unsigned SyntaxVariant,
                                               const MCAsmInfo &MAI,
                                               const MCInstrInfo &MII,
                                               const MCRegisterInfo &MRI) {
  return new YZGPUInstPrinter(MAI, MII, MRI);
}

static MCTargetStreamer *
createYZGPUObjectTargetStreamer(MCStreamer &S, const MCSubtargetInfo &STI) {
  return new YZGPUTargetELFStreamer(S, STI);
}

static MCTargetStreamer *createYZGPUAsmTargetStreamer(MCStreamer &S,
                                                      formatted_raw_ostream &OS,
                                                      MCInstPrinter *InstPrint,
                                                      bool isVerboseAsm) {
  return new YZGPUTargetAsmStreamer(S, OS);
}

static MCTargetStreamer *createYZGPUNullTargetStreamer(MCStreamer &S) {
  return new YZGPUTargetStreamer(S);
}

namespace {

class YZGPUMCInstrAnalysis : public MCInstrAnalysis {
public:
  explicit YZGPUMCInstrAnalysis(const MCInstrInfo *Info)
      : MCInstrAnalysis(Info) {}

  bool evaluateBranch(const MCInst &Inst, uint64_t Addr, uint64_t Size,
                      uint64_t &Target) const override {
    return false;
  }
};

} // end anonymous namespace

static MCInstrAnalysis *createYZGPUInstrAnalysis(const MCInstrInfo *Info) {
  return new YZGPUMCInstrAnalysis(Info);
}

namespace {
MCStreamer *createYZGPUELFStreamer(const Triple &T, MCContext &Context,
                                   std::unique_ptr<MCAsmBackend> &&MAB,
                                   std::unique_ptr<MCObjectWriter> &&MOW,
                                   std::unique_ptr<MCCodeEmitter> &&MCE,
                                   bool RelaxAll) {
  return createYZGPUELFStreamer(Context, std::move(MAB), std::move(MOW),
                                std::move(MCE), RelaxAll);
}
} // end anonymous namespace

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeYZGPUTargetMC() {
  auto &YZGPUTarget = getTheYZGPUTarget();
  TargetRegistry::RegisterMCAsmInfo(YZGPUTarget, createYZGPUMCAsmInfo);
  TargetRegistry::RegisterMCInstrInfo(YZGPUTarget, createYZGPUMCInstrInfo);
  TargetRegistry::RegisterMCRegInfo(YZGPUTarget, createYZGPUMCRegisterInfo);
  TargetRegistry::RegisterMCAsmBackend(YZGPUTarget, createYZGPUAsmBackend);
  TargetRegistry::RegisterMCCodeEmitter(YZGPUTarget, createYZGPUMCCodeEmitter);
  TargetRegistry::RegisterMCInstPrinter(YZGPUTarget, createYZGPUMCInstPrinter);
  TargetRegistry::RegisterMCSubtargetInfo(YZGPUTarget,
                                          createYZGPUMCSubtargetInfo);
  TargetRegistry::RegisterELFStreamer(YZGPUTarget, createYZGPUELFStreamer);
  TargetRegistry::RegisterObjectTargetStreamer(YZGPUTarget,
                                               createYZGPUObjectTargetStreamer);
  // TargetRegistry::RegisterMCInstrAnalysis(YZGPUTarget,
  // createYZGPUInstrAnalysis);

  // Register the asm target streamer.
  TargetRegistry::RegisterAsmTargetStreamer(YZGPUTarget,
                                            createYZGPUAsmTargetStreamer);
  // Register the null target streamer.
  TargetRegistry::RegisterNullTargetStreamer(YZGPUTarget,
                                             createYZGPUNullTargetStreamer);
}

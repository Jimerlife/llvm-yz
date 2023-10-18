add_llvm_component_group(YZGPU)

set(LLVM_TARGET_DEFINITIONS YZGPU.td)

tablegen(LLVM YZGPUGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM YZGPUGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM YZGPUGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM YZGPUGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM YZGPUGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM YZGPUGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM YZGPUGenDisassemblerTables.inc -gen-disassembler)

add_public_tablegen_target(YZGPUCommonTableGen)

add_llvm_target(YZGPUCodeGen
  YZGPUSubtarget.cpp
  YZGPUInstrInfo.cpp
  YZGPURegisterInfo.cpp
  YZGPUISelLowering.cpp
  YZGPUFrameLowering.cpp
  YZGPUTargetMachine.cpp
  YZGPUTargetObjectFile.cpp

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  Core
  IPO
  CodeGen
  MC
  YZGPUDesc
  YZGPUInfo
  SelectionDAG
  Support
  Target
  TargetParser
  TransformUtils
  GlobalISel

  ADD_TO_COMPONENT
  YZGPU
  )

add_subdirectory(AsmParser)
add_subdirectory(Disassembler)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)

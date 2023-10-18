add_llvm_component_library(LLVMYZGPUDesc
  YZGPUMCTargetDesc.cpp
  YZGPUAsmBackend.cpp
  YZGPUBaseInfo.cpp
  YZGPUMCCodeEmitter.cpp
  YZGPUTargetStreamer.cpp
  YZGPUELFStreamer.cpp
  YZGPUMCAsmInfo.cpp
  YZGPUInstPrinter.cpp
  YZGPUELFObjectWriter.cpp

  LINK_COMPONENTS
  MC
  YZGPUInfo
  Support
  TargetParser

  ADD_TO_COMPONENT
  YZGPU
)

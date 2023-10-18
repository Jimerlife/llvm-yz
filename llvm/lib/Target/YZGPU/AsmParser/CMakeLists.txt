add_llvm_component_library(LLVMYZGPUAsmParser
  YZGPUAsmParser.cpp

  LINK_COMPONENTS
  MC
  MCParser
  YZGPUDesc
  YZGPUInfo
  Support
  TargetParser

  ADD_TO_COMPONENT
  YZGPU
  )

add_llvm_component_library(LLVMYZGPUDisassembler
  YZGPUDisassembler.cpp

  LINK_COMPONENTS
  MC
  MCDisassembler
  YZGPUDesc
  YZGPUInfo
  Support

  ADD_TO_COMPONENT
  YZGPU
  )

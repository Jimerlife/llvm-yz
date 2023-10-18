add_llvm_component_library(LLVMYZGPUInfo
  YZGPUTargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  YZGPU
  )

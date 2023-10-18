//===-- YZGPURegisterInfo.td - YZGPU Register defs ---------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the YZGPU registers
//===----------------------------------------------------------------------===//
class YZGPUReg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
  let Namespace = "YZGPU";
}

// For register classes that use TSFlags.
class YZGPURegisterClass <string name, list<ValueType> regType, int Align, dag regList>
  : RegisterClass <name, regType, Align, regList> {
  // For vector register classes.
  bit HasVGPR = 0;

  // For scalar register classes.
  bit HasSGPR = 0;

  let TSFlags{0} = HasVGPR;
  let TSFlags{1} = HasSGPR;
}

// SGPR registers
foreach Index = 0...127 in {
  def SGPR#Index : YZGPUReg <Index, "s"#Index>;
}

// VGPR registers
foreach Index = 0...255 in {
  def VGPR#Index : YZGPUReg <Index, "v"#Index>;
}

//===----------------------------------------------------------------------===//
//  Groupings using register classes and tuples
//===----------------------------------------------------------------------===//

// SGPR 32-bit registers
def SGPR_32 : YZGPURegisterClass<"YZGPU", [i32, i16], 32,
                                 (add (sequence "SGPR%u", 0, 127))> {
  // Give all SGPR classes higher priority than VGPR classes, because
  // we want to spill SGPRs to VGPRs.
  //let AllocationPriority = 0;
  let HasSGPR = 1;
}

// VGPR 32-bit registers
def VGPR_32 : YZGPURegisterClass<"YZGPU", [i32, i16], 32,
                                 (add (sequence "VGPR%u", 0, 255))> {
  //let AllocationPriority = 0;
  let HasVGPR = 1;
}

def VS_32 : YZGPURegisterClass<"YZGPU", [i32, i16], 32,
                               (add VGPR_32, SGPR_32)> {
  let HasVGPR = 1;
  let HasSGPR = 1;
}

//===----------------------------------------------------------------------===//
//  Register operands
//===----------------------------------------------------------------------===//
multiclass YZGPURegOperand32 <string rc, string rc_suffix = "_32"> {
  let OperandNamespace = "YZGPU" in {
    // def _b16 : RegisterOperand<!cast<RegisterClass>(rc#rc_suffix)> {
    //   let OperandType = "Operand_INT16";
    //   let EncoderMethod = "getVSrc16Encoding";
    //   let DecoderMethod = "decodeOperand_VSrc16";
    // }

    def _b32 : RegisterOperand<!cast<RegisterClass>(rc#rc_suffix)> {
      let OperandType = "Operand_INT32";
      let EncoderMethod = "getVSrc32Encoding";
      let DecoderMethod = "decodeOperand_VSrc32";
    }
  }
}

defm VSrc : YZGPURegOperand32<"VS">;

/*
def VSrc_b32 : RegisterOperand<VGPR_32> {
  let EncoderMethod = "getVSrcEncoding";
  let DecoderMethod = "decodeOperandVSrc";
}
*/
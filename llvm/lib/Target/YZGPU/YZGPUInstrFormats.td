//===-- YZGPUInstrFormats.td - YZGPU Instruction Formats ---*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//
// YZGPU Instruction format definitions.
// 
//===----------------------------------------------------------------------===//

class YZGPUInst<dag outs, dag ins, string opcodestr, string argstr,
             list<dag> pattern>
    : Instruction {
  field bits<64> Inst;
  // SoftFail is a field the disassembler can use to provide a way for
  // instructions to not match without killing the whole decode process. It is
  // mainly used for ARM, but Tablegen expects this field to exist or it fails
  // to build the decode table.
  field bits<64> SoftFail = 0;
  let Size = 8;

  let Namespace = "YZGPU";

  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = opcodestr # "\t" # argstr;
  let Pattern = pattern;
}

// Instruction formats for VOP2
class YZGPUBase<bits<1>optype, bits<10> opcode, dag outs, dag ins, string opcodestr, string argstr>
    : YZGPUInst<outs, ins, opcodestr, argstr, []> {
  bits<2>  src0_modifiers;
  bits<10> src0;
  bits<2>  src1_modifiers;
  bits<10> src1;
  bits<8>  vdst;
  
  let Inst{0} = optype;
  let Inst{10-1} = opcode;
  let Inst{12-11} = 0b00;
  let Inst{20-13} = vdst;           // dst
  let Inst{23-21} = 0b000;          // round mode
  let Inst{24} = 0;                 // dst.rel
  let Inst{32-25} = src0{7-0};      // src0
  let Inst{34-33} = src0{9-8};      // src0.gpr
  let Inst{36-35} = src0_modifiers;
  let Inst{37} = 0;                 // src0.rel
  let Inst{45-38} = src1{7-0};      // src1
  let Inst{47-46} = src1{9-8};      // src1.gpr
  let Inst{49-48} = src1_modifiers;
  let Inst{50} = 0;                 // src1.rel
  let Inst{62-51} = 0b111111111111;
  let Inst{63} = 0;
}

class ALUOpSel<bits<1>optype, bits<10> opcode, dag outs, dag ins, string opcodestr, string argstr>
    : YZGPUBase<optype, opcode, outs, ins, opcodestr, argstr> {
  bits<4> src0_sel;
  bits<4> src1_sel;
  bits<4> dst_sel;

  let Inst{54-51} = dst_sel;
  let Inst{58-55} = src0_sel;
  let Inst{62-59} = src1_sel;
}
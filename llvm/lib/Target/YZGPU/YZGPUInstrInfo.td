//===-- YZGPUInstrInfo.td - Target Description for YZGPU ---*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes the YZGPU instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Instruction Formats
//===----------------------------------------------------------------------===//

include "YZGPUInstrFormats.td"

//===----------------------------------------------------------------------===//
// Instruction Class Templates
//===----------------------------------------------------------------------===//
class InputModsMatchClass : AsmOperandClass {
  let Name = "RegWithInputMods";
  let ParserMethod = "parseRegWithInputMods";
  let PredicateMethod = "isRegWithInputMods";
}

class InputMods <AsmOperandClass MatchClass> : Operand<i32> {
  let OperandNamespace = "YZGPU";
  let OperandType = "OPERAND_INPUT_MODS";
  let PrintMethod = "printOperandInputMods";
  let ParserMatchClass = MatchClass;
}

class OpSelMatchClass : AsmOperandClass {
  let Name = "OpSel";
  let ParserMethod = "parseOpSel";
  let IsOptional = 1;
}

class OpSel<string Name, AsmOperandClass MatchClass> : Operand<i32> {
  let PrintMethod = "print"#Name;
  let ParserMatchClass = MatchClass;
}

def OperandModsMatchClass : InputModsMatchClass;
def OperandMods : InputMods<OperandModsMatchClass>;

def OperandSel : OpSelMatchClass;
def dst_sel : OpSel<"DstSel", OperandSel>;
def src0_sel : OpSel<"Src0Sel", OperandSel>;
def src1_sel : OpSel<"Src1Sel", OperandSel>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
class VOP2_B32<bits<1> optype, bits<10> opcode, string opcodestr>
    : YZGPUBase<optype, opcode, (outs VGPR_32:$vdst), 
                                (ins OperandMods:$src0_modifiers, VSrc_b32:$src0,
                                 OperandMods:$src1_modifiers, VSrc_b32:$src1),
               opcodestr, "$vdst, $src0_modifiers, $src1_modifiers"> {
  let AsmMatchConverter = "cvtInputMods";
}

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
class VOP2_B16<bits<1> optype, bits<10> opcode, string opcodestr>
    : ALUOpSel<optype, opcode, (outs VGPR_32:$vdst),
                               (ins OperandMods:$src0_modifiers, VSrc_b32:$src0,
                                OperandMods:$src1_modifiers, VSrc_b32:$src1,
                                dst_sel:$dst_sel, src0_sel:$src0_sel, src1_sel:$src1_sel),
               opcodestr,
               "$vdst, $src0_modifiers, $src1_modifiers, $dst_sel, $src0_sel, $src1_sel"> {
  let AsmMatchConverter = "cvtInputModsAndOpSel";
}

//===----------------------------------------------------------------------===//
// Instructions
//===----------------------------------------------------------------------===//
def VADD_U32 : VOP2_B32<0b0, 0b0000010001, "v.add.u32">;
def VADD_S32 : VOP2_B32<0b0, 0b0000000001, "v.add.s32">;

def VADD_S16 : VOP2_B16<0b0, 0b0100000001, "v.add.s16">;

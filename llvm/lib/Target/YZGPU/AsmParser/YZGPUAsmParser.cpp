//===- YZGPUAsmParser.cpp - Parse YZGPU assembly to MCInst instructions
//----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/YZGPUBaseInfo.h"
#include "MCTargetDesc/YZGPUMCTargetDesc.h"
#include "MCTargetDesc/YZGPUTargetStreamer.h"
#include "TargetInfo/YZGPUTargetInfo.h"
#include "YZGPUInstrInfo.h"
#include "YZGPURegisterInfo.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/SmallBitVector.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/ADT/Twine.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrDesc.h"
#include "llvm/MC/MCParser/MCAsmLexer.h"
#include "llvm/MC/MCParser/MCAsmParser.h"
#include "llvm/MC/MCParser/MCParsedAsmOperand.h"
#include "llvm/MC/MCParser/MCTargetAsmParser.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/MachineValueType.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/TargetParser.h"
#include <optional>

using namespace llvm;

#define DEBUG_TYPE "yzgpu-asm-parser"

namespace {
class YZGPUOperand;

class YZGPUAsmParser : public MCTargetAsmParser {
  MCAsmParser &Parser;
  // KernelScopeInfo KernelScope;

  bool parseRegister(MCRegister &RegNo, SMLoc &StartLoc,
                     SMLoc &EndLoc) override;

  OperandMatchResultTy tryParseRegister(MCRegister &RegNo, SMLoc &StartLoc,
                                        SMLoc &EndLoc) override;

  unsigned checkTargetMatchPredicate(MCInst &Inst) override;

  unsigned validateTargetOperandClass(MCParsedAsmOperand &Op,
                                      unsigned Kind) override;

  bool MatchAndEmitInstruction(SMLoc IDLoc, unsigned &Opcode,
                               OperandVector &Operands, MCStreamer &Out,
                               uint64_t &ErrorInfo,
                               bool MatchingInlineAsm) override;

  bool ParseDirective(AsmToken DirectiveID) override;

  bool ParseInstruction(ParseInstructionInfo &Info, StringRef Name,
                        SMLoc NameLoc, OperandVector &Operands) override;

  /// Helper for processing MC instructions that have been successfully matched
  /// by MatchAndEmitInstruction.
  bool processInstruction(MCInst &Inst, SMLoc IDLoc, OperandVector &Operands,
                          MCStreamer &Out);

  /// @name Auto-generated Match Functions
#define GET_ASSEMBLER_HEADER
#include "YZGPUGenAsmMatcher.inc"

  OperandMatchResultTy parseRegister(OperandVector &Operands, StringRef Name);
  bool parseOperand(OperandVector &Operands, StringRef Mnemonic);
  OperandMatchResultTy parseTokenOp(StringRef Name, OperandVector &Operands);
  OperandMatchResultTy parseRegWithInputMods(OperandVector &Operands);
  OperandMatchResultTy parseOpSel(OperandVector &Operands);

  void cvtInputMods(MCInst &Inst, const OperandVector &Operands);
  void cvtInputModsAndOpSel(MCInst &Inst, const OperandVector &Operands);
  SMLoc getOperandLoc(std::function<bool(const YZGPUOperand &)> Test,
                      const OperandVector &Operands) const;
  SMLoc getRegLoc(unsigned Reg, const OperandVector &Operands) const;
  SMLoc getInstLoc(const OperandVector &Operands) const;

  bool isId(const StringRef Id) const;
  bool isId(const AsmToken &Token, const StringRef Id) const;
  bool isToken(const AsmToken::TokenKind Kind) const;
  StringRef getId() const;
  bool trySkipId(const StringRef Id);
  bool trySkipId(const StringRef Pref, const StringRef Id);
  bool trySkipId(const StringRef Id, const AsmToken::TokenKind Kind);
  bool trySkipToken(const AsmToken::TokenKind Kind);
  bool skipToken(const AsmToken::TokenKind Kind, const StringRef ErrMsg);
  bool parseString(StringRef &Val,
                   const StringRef ErrMsg = "expected a string");
  bool parseId(StringRef &Val, const StringRef ErrMsg = "");

  void peekTokens(MutableArrayRef<AsmToken> Tokens);
  AsmToken::TokenKind getTokenKind() const;
  StringRef getTokenStr() const;
  AsmToken peekToken(bool ShouldSkipSpace = true);
  AsmToken getToken() const;
  SMLoc getLoc() const;
  void lex();

public:
  enum YZGPUMatchResultTy {
    Match_Dummy = FIRST_TARGET_MATCH_RESULT_TY,
#define GET_OPERAND_DIAGNOSTIC_TYPES
#include "YZGPUGenAsmMatcher.inc"
#undef GET_OPERAND_DIAGNOSTIC_TYPES
  };

  YZGPUAsmParser(const MCSubtargetInfo &STI, MCAsmParser &_Parser,
                 const MCInstrInfo &MII, const MCTargetOptions &Options)
      : MCTargetAsmParser(Options, STI, MII), Parser(_Parser) {
    MCAsmParserExtension::Initialize(Parser);
    setAvailableFeatures(ComputeAvailableFeatures(getFeatureBits()));

    // KernelScope.initialize(getContext());
  }

  YZGPUTargetStreamer &getTargetStreamer() {
    assert(getParser().getStreamer().getTargetStreamer() &&
           "do not have a target streamer");
    MCTargetStreamer &TS = *getParser().getStreamer().getTargetStreamer();
    return static_cast<YZGPUTargetStreamer &>(TS);
  }

  const MCRegisterInfo *getMRI() const {
    // We need this const_cast because for some reason getContext() is not const
    // in MCAsmParser.
    return const_cast<YZGPUAsmParser *>(this)->getContext().getRegisterInfo();
  }

  const MCInstrInfo *getMII() const { return &MII; }

  const FeatureBitset &getFeatureBits() const {
    return getSTI().getFeatureBits();
  }
};

class YZGPUOperand : public MCParsedAsmOperand {
public:
  struct Modifiers {
    bool Abs = false;
    bool Neg = false;

    bool hasModifiers() const { return Abs || Neg; }

    int64_t getModifiersOperand() const {
      int64_t Operand = 0;
      Operand |= Abs ? YZGPUSrcMods::ABS : 0u;
      Operand |= Neg ? YZGPUSrcMods::NEG : 0u;
      return Operand;
    }
  };

private:
  enum KindTy {
    Token,
    Immediate,
    Register,
  } Kind;

  SMLoc StartLoc, EndLoc;
  const YZGPUAsmParser *AsmParser;

  struct TokOp {
    const char *Data;
    unsigned Length;
  };

  struct ImmOp {
    const MCExpr *Val;
    StringRef Name;
  };

  struct RegOp {
    unsigned RegNo;
    Modifiers Mods;
  };

  union {
    TokOp Tok;
    RegOp Reg;
    ImmOp Imm;
  };

public:
  YZGPUOperand(KindTy Kind_, const YZGPUAsmParser *AsmParser_)
      : Kind(Kind_), AsmParser(AsmParser_) {}

  void setModifiers(Modifiers Mods) {
    assert(isReg());
    Reg.Mods = Mods;
  }

  Modifiers getModifiers() const {
    assert(isReg());
    return Reg.Mods;
  }

  bool hasModifiers() const { return getModifiers().hasModifiers(); }

  bool isToken() const override { return Kind == Token; }

  bool isImm() const override { return Kind == Immediate; }

  bool isReg() const override { return Kind == Register; }

  bool isMem() const override { return false; }

  bool isRegWithInputMods() const {
    return isReg() &&
           YZGPUMCRegisterClasses[YZGPU::VS_32RegClassID].contains(getReg());
  }

  bool isOpSel() const { return true; }

  StringRef getImmName() {
    assert(isImm());
    return Imm.Name;
  }

  StringRef getToken() const {
    assert(isToken());
    return StringRef(Tok.Data, Tok.Length);
  }

  const MCExpr *getImm() const {
    assert(isImm());
    return Imm.Val;
  }

  unsigned getReg() const override {
    assert(isReg());
    return Reg.RegNo;
  }

  SMLoc getStartLoc() const override { return StartLoc; }

  SMLoc getEndLoc() const override { return EndLoc; }

  SMRange getLocRange() const { return SMRange(StartLoc, EndLoc); }

  bool isRegClass(unsigned RCID) const {
    return isReg() && AsmParser->getMRI()->getRegClass(RCID).contains(getReg());
  }

  void addExpr(MCInst &Inst, const MCExpr *Expr) const {
    assert(Expr && "Expr shouldn't be null!");
    if (auto *CE = dyn_cast<MCConstantExpr>(Expr))
      Inst.addOperand(MCOperand::createImm(CE->getValue()));
    else
      Inst.addOperand(MCOperand::createExpr(Expr));
  }

  void addImmOperands(MCInst &Inst, unsigned N) const {
    // assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getImm());
  }

  void addRegOperands(MCInst &Inst, unsigned N) const {
    Inst.addOperand(MCOperand::createReg(getReg()));
  }

  void addRegWithInputModsOperands(MCInst &Inst, unsigned N) const {
    Modifiers Mods = getModifiers();
    Inst.addOperand(MCOperand::createImm(Mods.getModifiersOperand()));
    addRegOperands(Inst, N);
  }

  void print(raw_ostream &OS) const override {
    switch (Kind) {
    case Register:
      OS << "<register " << getReg() << ">";
      break;
    case Immediate:
      OS << *getImm();
      break;
    case Token:
      OS << "'" << getToken() << "'";
      break;
    }
  }

  static std::unique_ptr<YZGPUOperand>
  CreateImm(const YZGPUAsmParser *AsmParser, const MCExpr *Val, SMLoc S,
            SMLoc E, StringRef Name = "default") {
    auto Op = std::make_unique<YZGPUOperand>(Immediate, AsmParser);
    Op->Imm.Val = Val;
    Op->Imm.Name = Name;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<YZGPUOperand>
  CreateToken(const YZGPUAsmParser *AsmParser, StringRef Str, SMLoc Loc) {
    auto Res = std::make_unique<YZGPUOperand>(Token, AsmParser);
    Res->Tok.Data = Str.data();
    Res->Tok.Length = Str.size();
    Res->StartLoc = Loc;
    Res->EndLoc = Loc;
    return Res;
  }

  static std::unique_ptr<YZGPUOperand>
  CreateReg(const YZGPUAsmParser *AsmParser, unsigned RegNo, SMLoc S, SMLoc E) {
    auto Op = std::make_unique<YZGPUOperand>(Register, AsmParser);
    Op->Reg.RegNo = RegNo;
    Op->Reg.Mods = Modifiers();
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }
};
} // end anonymous namespace

//===----------------------------------------------------------------------===//
// AsmParser
//===----------------------------------------------------------------------===//

#define GET_REGISTER_MATCHER
#define GET_MATCHER_IMPLEMENTATION
#define GET_MNEMONIC_SPELL_CHECKER
#define GET_MNEMONIC_CHECKER
#include "YZGPUGenAsmMatcher.inc"

// Attempts to match Name as a register (either using the default name or
// alternative ABI names), setting RegNo to the matching register. Upon
// failure, returns true and sets RegNo to 0.
static bool matchRegisterNameHelper(const MCSubtargetInfo &STI,
                                    MCRegister &RegNo, StringRef Name) {
  RegNo = MatchRegisterName(Name);

  return RegNo == YZGPU::NoRegister;
}

static bool isRegWithInputMods(const MCInstrDesc &Desc, unsigned OpNum) {
  return
      // 1. This operand is input modifiers
      Desc.operands()[OpNum].OperandType == YZGPU::OPERAND_INPUT_MODS
      // 2. This is not last operand
      && Desc.NumOperands > (OpNum + 1)
      // 3. Next operand is register class
      && Desc.operands()[OpNum + 1].RegClass != -1;
}

void YZGPUAsmParser::cvtInputMods(MCInst &Inst, const OperandVector &Operands) {
  unsigned Opc = Inst.getOpcode();

  unsigned I = 1;
  const MCInstrDesc &Desc = MII.get(Opc);
  for (unsigned J = 0; J < Desc.getNumDefs(); ++J) {
    ((YZGPUOperand &)*Operands[I++]).addRegOperands(Inst, 1);
  }

  for (unsigned E = Operands.size(); I != E; ++I) {
    YZGPUOperand &Op = ((YZGPUOperand &)*Operands[I]);
    if (isRegWithInputMods(Desc, Inst.getNumOperands())) {
      Op.addRegWithInputModsOperands(Inst, 2);
    } else {
      llvm_unreachable("unhandled operand type");
    }
  }
}

static void addOpSelImmOperand(MCInst &Inst, const OperandVector &Operands,
                               std::map<StringRef, int64_t> &OpSelMap,
                               StringRef Name, int64_t Default = 0) {
  auto i = OpSelMap.find(Name);
  if (i != OpSelMap.end()) {
    unsigned Idx = i->second;
    ((YZGPUOperand &)*Operands[Idx]).addImmOperands(Inst, 1);
  } else {
    Inst.addOperand(MCOperand::createImm(Default));
  }
}

void YZGPUAsmParser::cvtInputModsAndOpSel(MCInst &Inst,
                                          const OperandVector &Operands) {
  unsigned Opc = Inst.getOpcode();
  std::map<StringRef, int64_t> OpSelMap;

  unsigned I = 1;
  const MCInstrDesc &Desc = MII.get(Opc);
  for (unsigned J = 0; J < Desc.getNumDefs(); ++J) {
    ((YZGPUOperand &)*Operands[I++]).addRegOperands(Inst, 1);
  }

  for (unsigned E = Operands.size(); I != E; ++I) {
    YZGPUOperand &Op = ((YZGPUOperand &)*Operands[I]);
    if (isRegWithInputMods(Desc, Inst.getNumOperands())) {
      Op.addRegWithInputModsOperands(Inst, 2);
    } else if (Op.isImm()) {
      OpSelMap[Op.getImmName()] = I;
    } else {
      llvm_unreachable("unhandled operand type");
    }
  }

  addOpSelImmOperand(Inst, Operands, OpSelMap, "dst_sel", ALUOpSel::WORD_0);
  addOpSelImmOperand(Inst, Operands, OpSelMap, "src0_sel", ALUOpSel::WORD_0);
  addOpSelImmOperand(Inst, Operands, OpSelMap, "src1_sel", ALUOpSel::WORD_0);
}

OperandMatchResultTy
YZGPUAsmParser::parseRegWithInputMods(OperandVector &Operands) {
  StringRef Name;
  if (isToken(AsmToken::Identifier)) {
    Name = getLexer().getTok().getIdentifier();
  } else {
    Error(getLoc(), "expect a register identifier");
    return MatchOperand_ParseFail;
  }

  bool Abs = Name.contains(".abs");
  bool Neg = Name.contains(".neg");

  if (Name.contains("."))
    Name = Name.substr(0, Name.find("."));

  if (parseRegister(Operands, Name) == MatchOperand_NoMatch) {
    Error(getLoc(), "expect a correct register name");
    return MatchOperand_ParseFail;
  }

  if (Abs || Neg) {
    YZGPUOperand::Modifiers Mods;
    Mods.Abs = Abs;
    Mods.Neg = Neg;
    YZGPUOperand &Op = static_cast<YZGPUOperand &>(*Operands.back());
    Op.setModifiers(Mods);
  }

  return MatchOperand_Success;
}

OperandMatchResultTy YZGPUAsmParser::parseOpSel(OperandVector &Operands) {
  SMLoc S = getLoc();
  StringRef Prefix;
  StringRef Value;

  if (isToken(AsmToken::Identifier)) {
    Prefix = getLexer().getTok().getIdentifier();
  } else {
    Error(getLoc(), "expect an opsel prefix");
    return MatchOperand_ParseFail;
  }

  if ((Prefix == "dst_sel" || Prefix == "src0_sel" || Prefix == "src1_sel") &&
      getLexer().peekTok().is(AsmToken::Colon)) {
    getLexer().Lex();
    getLexer().Lex();
  } else {
    Error(getLoc(), "expect an opsel prefix and colon");
    return MatchOperand_ParseFail;
  }

  if (isToken(AsmToken::Identifier)) {
    Value = getLexer().getTok().getIdentifier();
  } else {
    Error(getLoc(), "expect an opsel identifier");
    return MatchOperand_ParseFail;
  }

  int64_t Int = StringSwitch<int64_t>(Value)
                    .Case("WORD_0", ALUOpSel::WORD_0)
                    .Case("WORD_1", ALUOpSel::WORD_1)
                    .Default(0xffffffff);

  if (Int == 0xffffffff) {
    Error(getLoc(), "invalid " + Twine(Prefix) + " value");
    return MatchOperand_ParseFail;
  }

  SMLoc E = SMLoc::getFromPointer(getLoc().getPointer() + Value.size());
  const MCExpr *Res = MCConstantExpr::create(Int, getContext());
  Operands.push_back(YZGPUOperand::CreateImm(this, Res, S, E, Prefix));
  getLexer().Lex(); // eat opsel value
  return MatchOperand_Success;
}

bool YZGPUAsmParser::parseRegister(MCRegister &RegNo, SMLoc &StartLoc,
                                   SMLoc &EndLoc) {
  return Error(getLoc(), "invalid register number");
}

OperandMatchResultTy YZGPUAsmParser::tryParseRegister(MCRegister &RegNo,
                                                      SMLoc &StartLoc,
                                                      SMLoc &EndLoc) {
  llvm_unreachable("Unimplemented function.");
}

OperandMatchResultTy YZGPUAsmParser::parseRegister(OperandVector &Operands,
                                                   StringRef Name) {
  switch (getLexer().getKind()) {
  default:
    return MatchOperand_NoMatch;
  case AsmToken::Identifier: {
    // StringRef Name = getLexer().getTok().getIdentifier();
    MCRegister RegNo;

    if (matchRegisterNameHelper(getSTI(), (MCRegister &)RegNo, Name))
      return MatchOperand_NoMatch;
    SMLoc S = getLoc();
    SMLoc E = SMLoc::getFromPointer(S.getPointer() + Name.size());
    getLexer().Lex();
    Operands.push_back(YZGPUOperand::CreateReg(this, RegNo, S, E));

    return MatchOperand_Success;
  }
  }
}

bool YZGPUAsmParser::processInstruction(MCInst &Inst, SMLoc IDLoc,
                                        OperandVector &Operands,
                                        MCStreamer &Out) {
  Inst.setLoc(IDLoc);
  Out.emitInstruction(Inst, getSTI());
  return false;
}

bool YZGPUAsmParser::MatchAndEmitInstruction(SMLoc IDLoc, unsigned &Opcode,
                                             OperandVector &Operands,
                                             MCStreamer &Out,
                                             uint64_t &ErrorInfo,
                                             bool MatchingInlineAsm) {
  MCInst Inst;
  FeatureBitset MissingFeatures;

  auto Result = MatchInstructionImpl(Operands, Inst, ErrorInfo, MissingFeatures,
                                     MatchingInlineAsm);
  switch (Result) {
  default:
    break;
  case Match_Success:
    return processInstruction(Inst, IDLoc, Operands, Out);
  case Match_MissingFeature: {
    break;
  }
  case Match_MnemonicFail: {
    break;
  }
  case Match_InvalidOperand: {
    SMLoc ErrorLoc = IDLoc;
    if (ErrorInfo != ~0U) {
      if (ErrorInfo >= Operands.size())
        return Error(ErrorLoc, "too few operands for instruction");

      ErrorLoc = ((YZGPUOperand &)*Operands[ErrorInfo]).getStartLoc();
      if (ErrorLoc == SMLoc())
        ErrorLoc = IDLoc;
    }
    return Error(ErrorLoc, "invalid operand for instruction");
  }
  }
}

// FIXME: need complete directive
bool YZGPUAsmParser::ParseDirective(AsmToken DirectiveID) {
  StringRef IDVal = DirectiveID.getString();
  return true;
}

bool YZGPUAsmParser::parseOperand(OperandVector &Operands, StringRef Mnemonic) {
  // Check if the current operand has a custom associated parser, if so, try to
  // custom parse the operand, or fallback to the general approach.
  // FIXME: need update
  OperandMatchResultTy Result =
      MatchOperandParserImpl(Operands, Mnemonic, /*ParseForAllFeatures=*/true);
  if (Result == MatchOperand_Success)
    return false;
  if (Result == MatchOperand_ParseFail)
    return true;

  // Attempt to parse token as a register.
  StringRef Name = "default-reg";
  if (isToken(AsmToken::Identifier)) {
    Name = getLexer().getTok().getIdentifier();
  }
  if (parseRegister(Operands, Name) == MatchOperand_Success)
    return false;

  // Finally we have exhausted all options and must declare defeat.
  Error(getLoc(), "unknown operand");
  return true;
}

bool YZGPUAsmParser::ParseInstruction(ParseInstructionInfo &Info,
                                      StringRef Name, SMLoc NameLoc,
                                      OperandVector &Operands) {
  // First operand is token for instruction.
  Operands.push_back(YZGPUOperand::CreateToken(this, Name, NameLoc));

  // If there are no more operands, then finish.
  if (getLexer().is(AsmToken::EndOfStatement))
    return false;

  // Parse first operand.
  if (parseOperand(Operands, Name))
    return true;

  // Parse until end of statement, consuming commas between operands.
  while (getLexer().is(AsmToken::Comma)) {
    // Consume comma token.
    getLexer().Lex();

    // Parse next operand.
    if (parseOperand(Operands, Name))
      return true;
  }

  if (getLexer().isNot(AsmToken::EndOfStatement)) {
    SMLoc Loc = getLexer().getLoc();
    getParser().eatToEndOfStatement();
    return Error(Loc, "unexpected token");
  }

  getParser().Lex(); // Consume the EndOfStatement.
  return false;
}

// FIXME: YZGPU
unsigned YZGPUAsmParser::checkTargetMatchPredicate(MCInst &Inst) {
  unsigned Opc = Inst.getOpcode();
  return Match_Success;
}

// FIXME: YZGPU
unsigned YZGPUAsmParser::validateTargetOperandClass(MCParsedAsmOperand &AsmOp,
                                                    unsigned Kind) {
  YZGPUOperand &Op = static_cast<YZGPUOperand &>(AsmOp);
  if (!Op.isReg())
    return Match_InvalidOperand;

  MCRegister Reg = Op.getReg();

  return Match_InvalidOperand;
}

//===----------------------------------------------------------------------===//
// Utility functions
//===----------------------------------------------------------------------===//

OperandMatchResultTy YZGPUAsmParser::parseTokenOp(StringRef Name,
                                                  OperandVector &Operands) {
  SMLoc S = getLoc();
  if (!trySkipId(Name))
    return MatchOperand_NoMatch;

  Operands.push_back(YZGPUOperand::CreateToken(this, Name, S));
  return MatchOperand_Success;
}

//===----------------------------------------------------------------------===//
// parser helpers
//===----------------------------------------------------------------------===//

bool YZGPUAsmParser::isId(const AsmToken &Token, const StringRef Id) const {
  return Token.is(AsmToken::Identifier) && Token.getString() == Id;
}

bool YZGPUAsmParser::isId(const StringRef Id) const {
  return isId(getToken(), Id);
}

bool YZGPUAsmParser::isToken(const AsmToken::TokenKind Kind) const {
  return getTokenKind() == Kind;
}

StringRef YZGPUAsmParser::getId() const {
  return isToken(AsmToken::Identifier) ? getTokenStr() : StringRef();
}

bool YZGPUAsmParser::trySkipId(const StringRef Id) {
  if (isId(Id)) {
    lex();
    return true;
  }
  return false;
}

bool YZGPUAsmParser::trySkipId(const StringRef Pref, const StringRef Id) {
  if (isToken(AsmToken::Identifier)) {
    StringRef Tok = getTokenStr();
    if (Tok.startswith(Pref) && Tok.drop_front(Pref.size()) == Id) {
      lex();
      return true;
    }
  }
  return false;
}

bool YZGPUAsmParser::trySkipId(const StringRef Id,
                               const AsmToken::TokenKind Kind) {
  if (isId(Id) && peekToken().is(Kind)) {
    lex();
    lex();
    return true;
  }
  return false;
}

bool YZGPUAsmParser::trySkipToken(const AsmToken::TokenKind Kind) {
  if (isToken(Kind)) {
    lex();
    return true;
  }
  return false;
}

bool YZGPUAsmParser::skipToken(const AsmToken::TokenKind Kind,
                               const StringRef ErrMsg) {
  if (!trySkipToken(Kind)) {
    Error(getLoc(), ErrMsg);
    return false;
  }
  return true;
}

bool YZGPUAsmParser::parseString(StringRef &Val, const StringRef ErrMsg) {
  if (isToken(AsmToken::String)) {
    Val = getToken().getStringContents();
    lex();
    return true;
  } else {
    Error(getLoc(), ErrMsg);
    return false;
  }
}

bool YZGPUAsmParser::parseId(StringRef &Val, const StringRef ErrMsg) {
  if (isToken(AsmToken::Identifier)) {
    Val = getTokenStr();
    lex();
    return true;
  } else {
    if (!ErrMsg.empty())
      Error(getLoc(), ErrMsg);
    return false;
  }
}

AsmToken YZGPUAsmParser::getToken() const { return Parser.getTok(); }

AsmToken YZGPUAsmParser::peekToken(bool ShouldSkipSpace) {
  return isToken(AsmToken::EndOfStatement)
             ? getToken()
             : getLexer().peekTok(ShouldSkipSpace);
}

void YZGPUAsmParser::peekTokens(MutableArrayRef<AsmToken> Tokens) {
  auto TokCount = getLexer().peekTokens(Tokens);

  for (auto Idx = TokCount; Idx < Tokens.size(); ++Idx)
    Tokens[Idx] = AsmToken(AsmToken::Error, "");
}

AsmToken::TokenKind YZGPUAsmParser::getTokenKind() const {
  return getLexer().getKind();
}

SMLoc YZGPUAsmParser::getLoc() const { return getToken().getLoc(); }

StringRef YZGPUAsmParser::getTokenStr() const { return getToken().getString(); }

void YZGPUAsmParser::lex() { Parser.Lex(); }

SMLoc YZGPUAsmParser::getInstLoc(const OperandVector &Operands) const {
  return ((YZGPUOperand &)*Operands[0]).getStartLoc();
}

SMLoc YZGPUAsmParser::getOperandLoc(
    std::function<bool(const YZGPUOperand &)> Test,
    const OperandVector &Operands) const {
  for (unsigned i = Operands.size() - 1; i > 0; --i) {
    YZGPUOperand &Op = ((YZGPUOperand &)*Operands[i]);
    if (Test(Op))
      return Op.getStartLoc();
  }
  return getInstLoc(Operands);
}

SMLoc YZGPUAsmParser::getRegLoc(unsigned Reg,
                                const OperandVector &Operands) const {
  auto Test = [=](const YZGPUOperand &Op) {
    return Op.isReg() && Op.getReg() == Reg;
  };
  return getOperandLoc(Test, Operands);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeYZGPUAsmParser() {
  RegisterMCAsmParser<YZGPUAsmParser> X(getTheYZGPUTarget());
}
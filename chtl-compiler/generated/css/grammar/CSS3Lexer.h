
// Generated from grammar/CSS3.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"


namespace chtl {


class  CSS3Lexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, Comment = 19, CDO = 20, 
    CDC = 21, Includes = 22, DashMatch = 23, Hash = 24, Import = 25, Page = 26, 
    Media = 27, Namespace = 28, CharSet = 29, Important = 30, FontFace = 31, 
    Supports = 32, Or = 33, Keyframes = 34, From = 35, To = 36, Calc = 37, 
    Viewport = 38, CounterStyle = 39, FontFeatureValues = 40, Var = 41, 
    MediaOnly = 42, Not = 43, And = 44, Dimension = 45, UnknownDimension = 46, 
    Percentage = 47, Number = 48, Uri = 49, String_ = 50, Function_ = 51, 
    UnicodeRange = 52, Ident = 53, Variable = 54, Plus = 55, Minus = 56, 
    Greater = 57, Tilde = 58, PseudoNot = 59, Space = 60, Newline = 61
  };

  explicit CSS3Lexer(antlr4::CharStream *input);

  ~CSS3Lexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

}  // namespace chtl

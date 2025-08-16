
// Generated from grammars/CSS3.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  CSS3Lexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, COMMENT = 7, 
    WS = 8, LBRACE = 9, RBRACE = 10, LPAREN = 11, RPAREN = 12, LBRACKET = 13, 
    RBRACKET = 14, SEMICOLON = 15, COLON = 16, DOT = 17, COMMA = 18, PLUS = 19, 
    GREATER = 20, TILDE = 21, INCLUDES = 22, DASHMATCH = 23, PREFIXMATCH = 24, 
    SUFFIXMATCH = 25, SUBSTRINGMATCH = 26, IMPORTANT_SYM = 27, AT_RULE_NAME = 28, 
    FUNCTION = 29, URI = 30, HASH = 31, NUMBER = 32, PERCENTAGE = 33, LENGTH = 34, 
    EMS = 35, EXS = 36, ANGLE = 37, TIME = 38, FREQ = 39, STRING = 40, IDENT = 41, 
    HEX_DIGIT = 42
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


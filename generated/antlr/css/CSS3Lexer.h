
// Generated from CSS3.g4 by ANTLR 4.9.2

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
  ~CSS3Lexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};


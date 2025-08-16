
// Generated from JavaScript.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"




class  JavaScriptLexer : public antlr4::Lexer {
public:
  enum {
    BREAK = 1, CASE = 2, CATCH = 3, CONTINUE = 4, DEFAULT = 5, DELETE = 6, 
    DO = 7, ELSE = 8, FINALLY = 9, FOR = 10, FUNCTION = 11, IF = 12, IN = 13, 
    INSTANCEOF = 14, NEW = 15, RETURN = 16, SWITCH = 17, THIS = 18, THROW = 19, 
    TRY = 20, TYPEOF = 21, VAR = 22, VOID = 23, WHILE = 24, WITH = 25, LET = 26, 
    CONST = 27, ASSIGN = 28, MUL_ASSIGN = 29, DIV_ASSIGN = 30, MOD_ASSIGN = 31, 
    PLUS_ASSIGN = 32, MINUS_ASSIGN = 33, QUESTION = 34, COLON = 35, OR = 36, 
    AND = 37, BIT_OR = 38, BIT_XOR = 39, BIT_AND = 40, EQUALS = 41, NOT_EQUALS = 42, 
    STRICT_EQUALS = 43, STRICT_NOT_EQUALS = 44, LT = 45, GT = 46, LE = 47, 
    GE = 48, LEFT_SHIFT = 49, RIGHT_SHIFT = 50, UNSIGNED_RIGHT_SHIFT = 51, 
    PLUS = 52, MINUS = 53, MULTIPLY = 54, DIVIDE = 55, MODULUS = 56, INCREMENT = 57, 
    DECREMENT = 58, BIT_NOT = 59, NOT = 60, LBRACE = 61, RBRACE = 62, LPAREN = 63, 
    RPAREN = 64, LBRACKET = 65, RBRACKET = 66, SEMICOLON = 67, COMMA = 68, 
    DOT = 69, NullLiteral = 70, BooleanLiteral = 71, NumericLiteral = 72, 
    StringLiteral = 73, Identifier = 74, WS = 75, COMMENT = 76, LINE_COMMENT = 77
  };

  explicit JavaScriptLexer(antlr4::CharStream *input);
  ~JavaScriptLexer();

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


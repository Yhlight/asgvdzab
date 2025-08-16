
// Generated from grammars/JavaScript.g4 by ANTLR 4.13.1

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

  ~JavaScriptLexer() override;


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


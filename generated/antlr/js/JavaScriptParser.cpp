
// Generated from grammars/JavaScript.g4 by ANTLR 4.13.1


#include "JavaScriptListener.h"
#include "JavaScriptVisitor.h"

#include "JavaScriptParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct JavaScriptParserStaticData final {
  JavaScriptParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  JavaScriptParserStaticData(const JavaScriptParserStaticData&) = delete;
  JavaScriptParserStaticData(JavaScriptParserStaticData&&) = delete;
  JavaScriptParserStaticData& operator=(const JavaScriptParserStaticData&) = delete;
  JavaScriptParserStaticData& operator=(JavaScriptParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag javascriptParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
JavaScriptParserStaticData *javascriptParserStaticData = nullptr;

void javascriptParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (javascriptParserStaticData != nullptr) {
    return;
  }
#else
  assert(javascriptParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<JavaScriptParserStaticData>(
    std::vector<std::string>{
      "program", "sourceElements", "sourceElement", "statement", "block", 
      "statementList", "variableStatement", "variableDeclarationList", "variableDeclaration", 
      "emptyStatement", "expressionStatement", "ifStatement", "iterationStatement", 
      "forInit", "continueStatement", "breakStatement", "returnStatement", 
      "tryStatement", "catchClause", "finallyClause", "functionDeclaration", 
      "formalParameterList", "functionBody", "expression", "assignmentExpression", 
      "assignmentOperator", "conditionalExpression", "logicalORExpression", 
      "logicalANDExpression", "bitwiseORExpression", "bitwiseXORExpression", 
      "bitwiseANDExpression", "equalityExpression", "relationalExpression", 
      "shiftExpression", "additiveExpression", "multiplicativeExpression", 
      "unaryExpression", "postfixExpression", "leftHandSideExpression", 
      "callExpression", "memberExpression", "primaryExpression", "arrayLiteral", 
      "elementList", "objectLiteral", "propertyNameAndValueList", "propertyAssignment", 
      "propertyName", "arguments", "argumentList", "functionExpression", 
      "literal"
    },
    std::vector<std::string>{
      "", "'break'", "'case'", "'catch'", "'continue'", "'default'", "'delete'", 
      "'do'", "'else'", "'finally'", "'for'", "'function'", "'if'", "'in'", 
      "'instanceof'", "'new'", "'return'", "'switch'", "'this'", "'throw'", 
      "'try'", "'typeof'", "'var'", "'void'", "'while'", "'with'", "'let'", 
      "'const'", "'='", "'*='", "'/='", "'%='", "'+='", "'-='", "'\\u003F'", 
      "':'", "'||'", "'&&'", "'|'", "'^'", "'&'", "'=='", "'!='", "'==='", 
      "'!=='", "'<'", "'>'", "'<='", "'>='", "'<<'", "'>>'", "'>>>'", "'+'", 
      "'-'", "'*'", "'/'", "'%'", "'++'", "'--'", "'~'", "'!'", "'{'", "'}'", 
      "'('", "')'", "'['", "']'", "';'", "','", "'.'", "'null'"
    },
    std::vector<std::string>{
      "", "BREAK", "CASE", "CATCH", "CONTINUE", "DEFAULT", "DELETE", "DO", 
      "ELSE", "FINALLY", "FOR", "FUNCTION", "IF", "IN", "INSTANCEOF", "NEW", 
      "RETURN", "SWITCH", "THIS", "THROW", "TRY", "TYPEOF", "VAR", "VOID", 
      "WHILE", "WITH", "LET", "CONST", "ASSIGN", "MUL_ASSIGN", "DIV_ASSIGN", 
      "MOD_ASSIGN", "PLUS_ASSIGN", "MINUS_ASSIGN", "QUESTION", "COLON", 
      "OR", "AND", "BIT_OR", "BIT_XOR", "BIT_AND", "EQUALS", "NOT_EQUALS", 
      "STRICT_EQUALS", "STRICT_NOT_EQUALS", "LT", "GT", "LE", "GE", "LEFT_SHIFT", 
      "RIGHT_SHIFT", "UNSIGNED_RIGHT_SHIFT", "PLUS", "MINUS", "MULTIPLY", 
      "DIVIDE", "MODULUS", "INCREMENT", "DECREMENT", "BIT_NOT", "NOT", "LBRACE", 
      "RBRACE", "LPAREN", "RPAREN", "LBRACKET", "RBRACKET", "SEMICOLON", 
      "COMMA", "DOT", "NullLiteral", "BooleanLiteral", "NumericLiteral", 
      "StringLiteral", "Identifier", "WS", "COMMENT", "LINE_COMMENT"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,77,497,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,7,
  	49,2,50,7,50,2,51,7,51,2,52,7,52,1,0,3,0,108,8,0,1,0,1,0,1,1,4,1,113,
  	8,1,11,1,12,1,114,1,2,1,2,3,2,119,8,2,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,
  	1,3,1,3,3,3,131,8,3,1,4,1,4,3,4,135,8,4,1,4,1,4,1,5,4,5,140,8,5,11,5,
  	12,5,141,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,3,6,156,8,6,
  	1,7,1,7,1,7,5,7,161,8,7,10,7,12,7,164,9,7,1,8,1,8,1,8,3,8,169,8,8,1,9,
  	1,9,1,10,1,10,1,10,1,11,1,11,1,11,1,11,1,11,1,11,1,11,3,11,183,8,11,1,
  	12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,3,12,194,8,12,1,12,1,12,3,
  	12,198,8,12,1,12,1,12,3,12,202,8,12,1,12,1,12,3,12,206,8,12,1,13,1,13,
  	3,13,210,8,13,1,14,1,14,3,14,214,8,14,1,14,1,14,1,15,1,15,3,15,220,8,
  	15,1,15,1,15,1,16,1,16,3,16,226,8,16,1,16,1,16,1,17,1,17,1,17,1,17,3,
  	17,234,8,17,1,17,3,17,237,8,17,1,18,1,18,1,18,1,18,1,18,1,18,1,19,1,19,
  	1,19,1,20,1,20,1,20,1,20,3,20,252,8,20,1,20,1,20,1,20,1,20,1,20,1,21,
  	1,21,1,21,5,21,262,8,21,10,21,12,21,265,9,21,1,22,3,22,268,8,22,1,23,
  	1,23,1,24,1,24,1,24,1,24,1,24,3,24,277,8,24,1,25,1,25,1,26,1,26,1,26,
  	1,26,1,26,1,26,3,26,287,8,26,1,27,1,27,1,27,5,27,292,8,27,10,27,12,27,
  	295,9,27,1,28,1,28,1,28,5,28,300,8,28,10,28,12,28,303,9,28,1,29,1,29,
  	1,29,5,29,308,8,29,10,29,12,29,311,9,29,1,30,1,30,1,30,5,30,316,8,30,
  	10,30,12,30,319,9,30,1,31,1,31,1,31,5,31,324,8,31,10,31,12,31,327,9,31,
  	1,32,1,32,1,32,5,32,332,8,32,10,32,12,32,335,9,32,1,33,1,33,1,33,5,33,
  	340,8,33,10,33,12,33,343,9,33,1,34,1,34,1,34,5,34,348,8,34,10,34,12,34,
  	351,9,34,1,35,1,35,1,35,5,35,356,8,35,10,35,12,35,359,9,35,1,36,1,36,
  	1,36,5,36,364,8,36,10,36,12,36,367,9,36,1,37,1,37,1,37,3,37,372,8,37,
  	1,38,1,38,3,38,376,8,38,1,39,1,39,3,39,380,8,39,1,40,1,40,1,40,1,40,1,
  	40,1,40,1,40,1,40,1,40,1,40,1,40,1,40,1,40,1,40,5,40,396,8,40,10,40,12,
  	40,399,9,40,1,41,1,41,1,41,1,41,1,41,1,41,1,41,3,41,408,8,41,1,41,1,41,
  	1,41,1,41,1,41,1,41,1,41,1,41,5,41,418,8,41,10,41,12,41,421,9,41,1,42,
  	1,42,1,42,1,42,1,42,1,42,1,42,1,42,1,42,3,42,432,8,42,1,43,1,43,3,43,
  	436,8,43,1,43,1,43,1,44,1,44,1,44,5,44,443,8,44,10,44,12,44,446,9,44,
  	1,45,1,45,3,45,450,8,45,1,45,1,45,1,46,1,46,1,46,5,46,457,8,46,10,46,
  	12,46,460,9,46,1,47,1,47,1,47,1,47,1,48,1,48,1,49,1,49,3,49,470,8,49,
  	1,49,1,49,1,50,1,50,1,50,5,50,477,8,50,10,50,12,50,480,9,50,1,51,1,51,
  	3,51,484,8,51,1,51,1,51,3,51,488,8,51,1,51,1,51,1,51,1,51,1,51,1,52,1,
  	52,1,52,0,2,80,82,53,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,
  	36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,
  	82,84,86,88,90,92,94,96,98,100,102,104,0,10,1,0,28,33,1,0,41,44,2,0,13,
  	14,45,48,1,0,49,51,1,0,52,53,1,0,54,56,5,0,6,6,21,21,23,23,52,53,57,60,
  	1,0,57,58,1,0,72,74,1,0,70,73,510,0,107,1,0,0,0,2,112,1,0,0,0,4,118,1,
  	0,0,0,6,130,1,0,0,0,8,132,1,0,0,0,10,139,1,0,0,0,12,155,1,0,0,0,14,157,
  	1,0,0,0,16,165,1,0,0,0,18,170,1,0,0,0,20,172,1,0,0,0,22,175,1,0,0,0,24,
  	205,1,0,0,0,26,209,1,0,0,0,28,211,1,0,0,0,30,217,1,0,0,0,32,223,1,0,0,
  	0,34,229,1,0,0,0,36,238,1,0,0,0,38,244,1,0,0,0,40,247,1,0,0,0,42,258,
  	1,0,0,0,44,267,1,0,0,0,46,269,1,0,0,0,48,276,1,0,0,0,50,278,1,0,0,0,52,
  	280,1,0,0,0,54,288,1,0,0,0,56,296,1,0,0,0,58,304,1,0,0,0,60,312,1,0,0,
  	0,62,320,1,0,0,0,64,328,1,0,0,0,66,336,1,0,0,0,68,344,1,0,0,0,70,352,
  	1,0,0,0,72,360,1,0,0,0,74,371,1,0,0,0,76,373,1,0,0,0,78,379,1,0,0,0,80,
  	381,1,0,0,0,82,407,1,0,0,0,84,431,1,0,0,0,86,433,1,0,0,0,88,439,1,0,0,
  	0,90,447,1,0,0,0,92,453,1,0,0,0,94,461,1,0,0,0,96,465,1,0,0,0,98,467,
  	1,0,0,0,100,473,1,0,0,0,102,481,1,0,0,0,104,494,1,0,0,0,106,108,3,2,1,
  	0,107,106,1,0,0,0,107,108,1,0,0,0,108,109,1,0,0,0,109,110,5,0,0,1,110,
  	1,1,0,0,0,111,113,3,4,2,0,112,111,1,0,0,0,113,114,1,0,0,0,114,112,1,0,
  	0,0,114,115,1,0,0,0,115,3,1,0,0,0,116,119,3,6,3,0,117,119,3,40,20,0,118,
  	116,1,0,0,0,118,117,1,0,0,0,119,5,1,0,0,0,120,131,3,8,4,0,121,131,3,12,
  	6,0,122,131,3,18,9,0,123,131,3,20,10,0,124,131,3,22,11,0,125,131,3,24,
  	12,0,126,131,3,28,14,0,127,131,3,30,15,0,128,131,3,32,16,0,129,131,3,
  	34,17,0,130,120,1,0,0,0,130,121,1,0,0,0,130,122,1,0,0,0,130,123,1,0,0,
  	0,130,124,1,0,0,0,130,125,1,0,0,0,130,126,1,0,0,0,130,127,1,0,0,0,130,
  	128,1,0,0,0,130,129,1,0,0,0,131,7,1,0,0,0,132,134,5,61,0,0,133,135,3,
  	10,5,0,134,133,1,0,0,0,134,135,1,0,0,0,135,136,1,0,0,0,136,137,5,62,0,
  	0,137,9,1,0,0,0,138,140,3,6,3,0,139,138,1,0,0,0,140,141,1,0,0,0,141,139,
  	1,0,0,0,141,142,1,0,0,0,142,11,1,0,0,0,143,144,5,22,0,0,144,145,3,14,
  	7,0,145,146,5,67,0,0,146,156,1,0,0,0,147,148,5,26,0,0,148,149,3,14,7,
  	0,149,150,5,67,0,0,150,156,1,0,0,0,151,152,5,27,0,0,152,153,3,14,7,0,
  	153,154,5,67,0,0,154,156,1,0,0,0,155,143,1,0,0,0,155,147,1,0,0,0,155,
  	151,1,0,0,0,156,13,1,0,0,0,157,162,3,16,8,0,158,159,5,68,0,0,159,161,
  	3,16,8,0,160,158,1,0,0,0,161,164,1,0,0,0,162,160,1,0,0,0,162,163,1,0,
  	0,0,163,15,1,0,0,0,164,162,1,0,0,0,165,168,5,74,0,0,166,167,5,28,0,0,
  	167,169,3,48,24,0,168,166,1,0,0,0,168,169,1,0,0,0,169,17,1,0,0,0,170,
  	171,5,67,0,0,171,19,1,0,0,0,172,173,3,46,23,0,173,174,5,67,0,0,174,21,
  	1,0,0,0,175,176,5,12,0,0,176,177,5,63,0,0,177,178,3,46,23,0,178,179,5,
  	64,0,0,179,182,3,6,3,0,180,181,5,8,0,0,181,183,3,6,3,0,182,180,1,0,0,
  	0,182,183,1,0,0,0,183,23,1,0,0,0,184,185,5,24,0,0,185,186,5,63,0,0,186,
  	187,3,46,23,0,187,188,5,64,0,0,188,189,3,6,3,0,189,206,1,0,0,0,190,191,
  	5,10,0,0,191,193,5,63,0,0,192,194,3,26,13,0,193,192,1,0,0,0,193,194,1,
  	0,0,0,194,195,1,0,0,0,195,197,5,67,0,0,196,198,3,46,23,0,197,196,1,0,
  	0,0,197,198,1,0,0,0,198,199,1,0,0,0,199,201,5,67,0,0,200,202,3,46,23,
  	0,201,200,1,0,0,0,201,202,1,0,0,0,202,203,1,0,0,0,203,204,5,64,0,0,204,
  	206,3,6,3,0,205,184,1,0,0,0,205,190,1,0,0,0,206,25,1,0,0,0,207,210,3,
  	14,7,0,208,210,3,46,23,0,209,207,1,0,0,0,209,208,1,0,0,0,210,27,1,0,0,
  	0,211,213,5,4,0,0,212,214,5,74,0,0,213,212,1,0,0,0,213,214,1,0,0,0,214,
  	215,1,0,0,0,215,216,5,67,0,0,216,29,1,0,0,0,217,219,5,1,0,0,218,220,5,
  	74,0,0,219,218,1,0,0,0,219,220,1,0,0,0,220,221,1,0,0,0,221,222,5,67,0,
  	0,222,31,1,0,0,0,223,225,5,16,0,0,224,226,3,46,23,0,225,224,1,0,0,0,225,
  	226,1,0,0,0,226,227,1,0,0,0,227,228,5,67,0,0,228,33,1,0,0,0,229,230,5,
  	20,0,0,230,236,3,8,4,0,231,233,3,36,18,0,232,234,3,38,19,0,233,232,1,
  	0,0,0,233,234,1,0,0,0,234,237,1,0,0,0,235,237,3,38,19,0,236,231,1,0,0,
  	0,236,235,1,0,0,0,237,35,1,0,0,0,238,239,5,3,0,0,239,240,5,63,0,0,240,
  	241,5,74,0,0,241,242,5,64,0,0,242,243,3,8,4,0,243,37,1,0,0,0,244,245,
  	5,9,0,0,245,246,3,8,4,0,246,39,1,0,0,0,247,248,5,11,0,0,248,249,5,74,
  	0,0,249,251,5,63,0,0,250,252,3,42,21,0,251,250,1,0,0,0,251,252,1,0,0,
  	0,252,253,1,0,0,0,253,254,5,64,0,0,254,255,5,61,0,0,255,256,3,44,22,0,
  	256,257,5,62,0,0,257,41,1,0,0,0,258,263,5,74,0,0,259,260,5,68,0,0,260,
  	262,5,74,0,0,261,259,1,0,0,0,262,265,1,0,0,0,263,261,1,0,0,0,263,264,
  	1,0,0,0,264,43,1,0,0,0,265,263,1,0,0,0,266,268,3,2,1,0,267,266,1,0,0,
  	0,267,268,1,0,0,0,268,45,1,0,0,0,269,270,3,48,24,0,270,47,1,0,0,0,271,
  	277,3,52,26,0,272,273,3,78,39,0,273,274,3,50,25,0,274,275,3,48,24,0,275,
  	277,1,0,0,0,276,271,1,0,0,0,276,272,1,0,0,0,277,49,1,0,0,0,278,279,7,
  	0,0,0,279,51,1,0,0,0,280,286,3,54,27,0,281,282,5,34,0,0,282,283,3,48,
  	24,0,283,284,5,35,0,0,284,285,3,48,24,0,285,287,1,0,0,0,286,281,1,0,0,
  	0,286,287,1,0,0,0,287,53,1,0,0,0,288,293,3,56,28,0,289,290,5,36,0,0,290,
  	292,3,56,28,0,291,289,1,0,0,0,292,295,1,0,0,0,293,291,1,0,0,0,293,294,
  	1,0,0,0,294,55,1,0,0,0,295,293,1,0,0,0,296,301,3,58,29,0,297,298,5,37,
  	0,0,298,300,3,58,29,0,299,297,1,0,0,0,300,303,1,0,0,0,301,299,1,0,0,0,
  	301,302,1,0,0,0,302,57,1,0,0,0,303,301,1,0,0,0,304,309,3,60,30,0,305,
  	306,5,38,0,0,306,308,3,60,30,0,307,305,1,0,0,0,308,311,1,0,0,0,309,307,
  	1,0,0,0,309,310,1,0,0,0,310,59,1,0,0,0,311,309,1,0,0,0,312,317,3,62,31,
  	0,313,314,5,39,0,0,314,316,3,62,31,0,315,313,1,0,0,0,316,319,1,0,0,0,
  	317,315,1,0,0,0,317,318,1,0,0,0,318,61,1,0,0,0,319,317,1,0,0,0,320,325,
  	3,64,32,0,321,322,5,40,0,0,322,324,3,64,32,0,323,321,1,0,0,0,324,327,
  	1,0,0,0,325,323,1,0,0,0,325,326,1,0,0,0,326,63,1,0,0,0,327,325,1,0,0,
  	0,328,333,3,66,33,0,329,330,7,1,0,0,330,332,3,66,33,0,331,329,1,0,0,0,
  	332,335,1,0,0,0,333,331,1,0,0,0,333,334,1,0,0,0,334,65,1,0,0,0,335,333,
  	1,0,0,0,336,341,3,68,34,0,337,338,7,2,0,0,338,340,3,68,34,0,339,337,1,
  	0,0,0,340,343,1,0,0,0,341,339,1,0,0,0,341,342,1,0,0,0,342,67,1,0,0,0,
  	343,341,1,0,0,0,344,349,3,70,35,0,345,346,7,3,0,0,346,348,3,70,35,0,347,
  	345,1,0,0,0,348,351,1,0,0,0,349,347,1,0,0,0,349,350,1,0,0,0,350,69,1,
  	0,0,0,351,349,1,0,0,0,352,357,3,72,36,0,353,354,7,4,0,0,354,356,3,72,
  	36,0,355,353,1,0,0,0,356,359,1,0,0,0,357,355,1,0,0,0,357,358,1,0,0,0,
  	358,71,1,0,0,0,359,357,1,0,0,0,360,365,3,74,37,0,361,362,7,5,0,0,362,
  	364,3,74,37,0,363,361,1,0,0,0,364,367,1,0,0,0,365,363,1,0,0,0,365,366,
  	1,0,0,0,366,73,1,0,0,0,367,365,1,0,0,0,368,372,3,76,38,0,369,370,7,6,
  	0,0,370,372,3,74,37,0,371,368,1,0,0,0,371,369,1,0,0,0,372,75,1,0,0,0,
  	373,375,3,78,39,0,374,376,7,7,0,0,375,374,1,0,0,0,375,376,1,0,0,0,376,
  	77,1,0,0,0,377,380,3,80,40,0,378,380,3,82,41,0,379,377,1,0,0,0,379,378,
  	1,0,0,0,380,79,1,0,0,0,381,382,6,40,-1,0,382,383,3,82,41,0,383,384,3,
  	98,49,0,384,397,1,0,0,0,385,386,10,3,0,0,386,396,3,98,49,0,387,388,10,
  	2,0,0,388,389,5,65,0,0,389,390,3,46,23,0,390,391,5,66,0,0,391,396,1,0,
  	0,0,392,393,10,1,0,0,393,394,5,69,0,0,394,396,5,74,0,0,395,385,1,0,0,
  	0,395,387,1,0,0,0,395,392,1,0,0,0,396,399,1,0,0,0,397,395,1,0,0,0,397,
  	398,1,0,0,0,398,81,1,0,0,0,399,397,1,0,0,0,400,401,6,41,-1,0,401,408,
  	3,84,42,0,402,408,3,102,51,0,403,404,5,15,0,0,404,405,3,82,41,0,405,406,
  	3,98,49,0,406,408,1,0,0,0,407,400,1,0,0,0,407,402,1,0,0,0,407,403,1,0,
  	0,0,408,419,1,0,0,0,409,410,10,3,0,0,410,411,5,65,0,0,411,412,3,46,23,
  	0,412,413,5,66,0,0,413,418,1,0,0,0,414,415,10,2,0,0,415,416,5,69,0,0,
  	416,418,5,74,0,0,417,409,1,0,0,0,417,414,1,0,0,0,418,421,1,0,0,0,419,
  	417,1,0,0,0,419,420,1,0,0,0,420,83,1,0,0,0,421,419,1,0,0,0,422,432,5,
  	18,0,0,423,432,5,74,0,0,424,432,3,104,52,0,425,432,3,86,43,0,426,432,
  	3,90,45,0,427,428,5,63,0,0,428,429,3,46,23,0,429,430,5,64,0,0,430,432,
  	1,0,0,0,431,422,1,0,0,0,431,423,1,0,0,0,431,424,1,0,0,0,431,425,1,0,0,
  	0,431,426,1,0,0,0,431,427,1,0,0,0,432,85,1,0,0,0,433,435,5,65,0,0,434,
  	436,3,88,44,0,435,434,1,0,0,0,435,436,1,0,0,0,436,437,1,0,0,0,437,438,
  	5,66,0,0,438,87,1,0,0,0,439,444,3,48,24,0,440,441,5,68,0,0,441,443,3,
  	48,24,0,442,440,1,0,0,0,443,446,1,0,0,0,444,442,1,0,0,0,444,445,1,0,0,
  	0,445,89,1,0,0,0,446,444,1,0,0,0,447,449,5,61,0,0,448,450,3,92,46,0,449,
  	448,1,0,0,0,449,450,1,0,0,0,450,451,1,0,0,0,451,452,5,62,0,0,452,91,1,
  	0,0,0,453,458,3,94,47,0,454,455,5,68,0,0,455,457,3,94,47,0,456,454,1,
  	0,0,0,457,460,1,0,0,0,458,456,1,0,0,0,458,459,1,0,0,0,459,93,1,0,0,0,
  	460,458,1,0,0,0,461,462,3,96,48,0,462,463,5,35,0,0,463,464,3,48,24,0,
  	464,95,1,0,0,0,465,466,7,8,0,0,466,97,1,0,0,0,467,469,5,63,0,0,468,470,
  	3,100,50,0,469,468,1,0,0,0,469,470,1,0,0,0,470,471,1,0,0,0,471,472,5,
  	64,0,0,472,99,1,0,0,0,473,478,3,48,24,0,474,475,5,68,0,0,475,477,3,48,
  	24,0,476,474,1,0,0,0,477,480,1,0,0,0,478,476,1,0,0,0,478,479,1,0,0,0,
  	479,101,1,0,0,0,480,478,1,0,0,0,481,483,5,11,0,0,482,484,5,74,0,0,483,
  	482,1,0,0,0,483,484,1,0,0,0,484,485,1,0,0,0,485,487,5,63,0,0,486,488,
  	3,42,21,0,487,486,1,0,0,0,487,488,1,0,0,0,488,489,1,0,0,0,489,490,5,64,
  	0,0,490,491,5,61,0,0,491,492,3,44,22,0,492,493,5,62,0,0,493,103,1,0,0,
  	0,494,495,7,9,0,0,495,105,1,0,0,0,52,107,114,118,130,134,141,155,162,
  	168,182,193,197,201,205,209,213,219,225,233,236,251,263,267,276,286,293,
  	301,309,317,325,333,341,349,357,365,371,375,379,395,397,407,417,419,431,
  	435,444,449,458,469,478,483,487
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  javascriptParserStaticData = staticData.release();
}

}

JavaScriptParser::JavaScriptParser(TokenStream *input) : JavaScriptParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

JavaScriptParser::JavaScriptParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  JavaScriptParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *javascriptParserStaticData->atn, javascriptParserStaticData->decisionToDFA, javascriptParserStaticData->sharedContextCache, options);
}

JavaScriptParser::~JavaScriptParser() {
  delete _interpreter;
}

const atn::ATN& JavaScriptParser::getATN() const {
  return *javascriptParserStaticData->atn;
}

std::string JavaScriptParser::getGrammarFileName() const {
  return "JavaScript.g4";
}

const std::vector<std::string>& JavaScriptParser::getRuleNames() const {
  return javascriptParserStaticData->ruleNames;
}

const dfa::Vocabulary& JavaScriptParser::getVocabulary() const {
  return javascriptParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView JavaScriptParser::getSerializedATN() const {
  return javascriptParserStaticData->serializedATN;
}


//----------------- ProgramContext ------------------------------------------------------------------

JavaScriptParser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::ProgramContext::EOF() {
  return getToken(JavaScriptParser::EOF, 0);
}

JavaScriptParser::SourceElementsContext* JavaScriptParser::ProgramContext::sourceElements() {
  return getRuleContext<JavaScriptParser::SourceElementsContext>(0);
}


size_t JavaScriptParser::ProgramContext::getRuleIndex() const {
  return JavaScriptParser::RuleProgram;
}

void JavaScriptParser::ProgramContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterProgram(this);
}

void JavaScriptParser::ProgramContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitProgram(this);
}


std::any JavaScriptParser::ProgramContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitProgram(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::ProgramContext* JavaScriptParser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, JavaScriptParser::RuleProgram);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(107);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -4742290407386932142) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 65)) & 997) != 0)) {
      setState(106);
      sourceElements();
    }
    setState(109);
    match(JavaScriptParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SourceElementsContext ------------------------------------------------------------------

JavaScriptParser::SourceElementsContext::SourceElementsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<JavaScriptParser::SourceElementContext *> JavaScriptParser::SourceElementsContext::sourceElement() {
  return getRuleContexts<JavaScriptParser::SourceElementContext>();
}

JavaScriptParser::SourceElementContext* JavaScriptParser::SourceElementsContext::sourceElement(size_t i) {
  return getRuleContext<JavaScriptParser::SourceElementContext>(i);
}


size_t JavaScriptParser::SourceElementsContext::getRuleIndex() const {
  return JavaScriptParser::RuleSourceElements;
}

void JavaScriptParser::SourceElementsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSourceElements(this);
}

void JavaScriptParser::SourceElementsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSourceElements(this);
}


std::any JavaScriptParser::SourceElementsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitSourceElements(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::SourceElementsContext* JavaScriptParser::sourceElements() {
  SourceElementsContext *_localctx = _tracker.createInstance<SourceElementsContext>(_ctx, getState());
  enterRule(_localctx, 2, JavaScriptParser::RuleSourceElements);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(112); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(111);
      sourceElement();
      setState(114); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -4742290407386932142) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 65)) & 997) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SourceElementContext ------------------------------------------------------------------

JavaScriptParser::SourceElementContext::SourceElementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JavaScriptParser::StatementContext* JavaScriptParser::SourceElementContext::statement() {
  return getRuleContext<JavaScriptParser::StatementContext>(0);
}

JavaScriptParser::FunctionDeclarationContext* JavaScriptParser::SourceElementContext::functionDeclaration() {
  return getRuleContext<JavaScriptParser::FunctionDeclarationContext>(0);
}


size_t JavaScriptParser::SourceElementContext::getRuleIndex() const {
  return JavaScriptParser::RuleSourceElement;
}

void JavaScriptParser::SourceElementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSourceElement(this);
}

void JavaScriptParser::SourceElementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSourceElement(this);
}


std::any JavaScriptParser::SourceElementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitSourceElement(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::SourceElementContext* JavaScriptParser::sourceElement() {
  SourceElementContext *_localctx = _tracker.createInstance<SourceElementContext>(_ctx, getState());
  enterRule(_localctx, 4, JavaScriptParser::RuleSourceElement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(118);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(116);
      statement();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(117);
      functionDeclaration();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

JavaScriptParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JavaScriptParser::BlockContext* JavaScriptParser::StatementContext::block() {
  return getRuleContext<JavaScriptParser::BlockContext>(0);
}

JavaScriptParser::VariableStatementContext* JavaScriptParser::StatementContext::variableStatement() {
  return getRuleContext<JavaScriptParser::VariableStatementContext>(0);
}

JavaScriptParser::EmptyStatementContext* JavaScriptParser::StatementContext::emptyStatement() {
  return getRuleContext<JavaScriptParser::EmptyStatementContext>(0);
}

JavaScriptParser::ExpressionStatementContext* JavaScriptParser::StatementContext::expressionStatement() {
  return getRuleContext<JavaScriptParser::ExpressionStatementContext>(0);
}

JavaScriptParser::IfStatementContext* JavaScriptParser::StatementContext::ifStatement() {
  return getRuleContext<JavaScriptParser::IfStatementContext>(0);
}

JavaScriptParser::IterationStatementContext* JavaScriptParser::StatementContext::iterationStatement() {
  return getRuleContext<JavaScriptParser::IterationStatementContext>(0);
}

JavaScriptParser::ContinueStatementContext* JavaScriptParser::StatementContext::continueStatement() {
  return getRuleContext<JavaScriptParser::ContinueStatementContext>(0);
}

JavaScriptParser::BreakStatementContext* JavaScriptParser::StatementContext::breakStatement() {
  return getRuleContext<JavaScriptParser::BreakStatementContext>(0);
}

JavaScriptParser::ReturnStatementContext* JavaScriptParser::StatementContext::returnStatement() {
  return getRuleContext<JavaScriptParser::ReturnStatementContext>(0);
}

JavaScriptParser::TryStatementContext* JavaScriptParser::StatementContext::tryStatement() {
  return getRuleContext<JavaScriptParser::TryStatementContext>(0);
}


size_t JavaScriptParser::StatementContext::getRuleIndex() const {
  return JavaScriptParser::RuleStatement;
}

void JavaScriptParser::StatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStatement(this);
}

void JavaScriptParser::StatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStatement(this);
}


std::any JavaScriptParser::StatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::StatementContext* JavaScriptParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 6, JavaScriptParser::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(130);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(120);
      block();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(121);
      variableStatement();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(122);
      emptyStatement();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(123);
      expressionStatement();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(124);
      ifStatement();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(125);
      iterationStatement();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(126);
      continueStatement();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(127);
      breakStatement();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(128);
      returnStatement();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(129);
      tryStatement();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

JavaScriptParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::BlockContext::LBRACE() {
  return getToken(JavaScriptParser::LBRACE, 0);
}

tree::TerminalNode* JavaScriptParser::BlockContext::RBRACE() {
  return getToken(JavaScriptParser::RBRACE, 0);
}

JavaScriptParser::StatementListContext* JavaScriptParser::BlockContext::statementList() {
  return getRuleContext<JavaScriptParser::StatementListContext>(0);
}


size_t JavaScriptParser::BlockContext::getRuleIndex() const {
  return JavaScriptParser::RuleBlock;
}

void JavaScriptParser::BlockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlock(this);
}

void JavaScriptParser::BlockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlock(this);
}


std::any JavaScriptParser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::BlockContext* JavaScriptParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 8, JavaScriptParser::RuleBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(132);
    match(JavaScriptParser::LBRACE);
    setState(134);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -4742290407386932142) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 65)) & 997) != 0)) {
      setState(133);
      statementList();
    }
    setState(136);
    match(JavaScriptParser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementListContext ------------------------------------------------------------------

JavaScriptParser::StatementListContext::StatementListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<JavaScriptParser::StatementContext *> JavaScriptParser::StatementListContext::statement() {
  return getRuleContexts<JavaScriptParser::StatementContext>();
}

JavaScriptParser::StatementContext* JavaScriptParser::StatementListContext::statement(size_t i) {
  return getRuleContext<JavaScriptParser::StatementContext>(i);
}


size_t JavaScriptParser::StatementListContext::getRuleIndex() const {
  return JavaScriptParser::RuleStatementList;
}

void JavaScriptParser::StatementListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStatementList(this);
}

void JavaScriptParser::StatementListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStatementList(this);
}


std::any JavaScriptParser::StatementListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitStatementList(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::StatementListContext* JavaScriptParser::statementList() {
  StatementListContext *_localctx = _tracker.createInstance<StatementListContext>(_ctx, getState());
  enterRule(_localctx, 10, JavaScriptParser::RuleStatementList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(139); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(138);
      statement();
      setState(141); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -4742290407386932142) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 65)) & 997) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VariableStatementContext ------------------------------------------------------------------

JavaScriptParser::VariableStatementContext::VariableStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::VariableStatementContext::VAR() {
  return getToken(JavaScriptParser::VAR, 0);
}

JavaScriptParser::VariableDeclarationListContext* JavaScriptParser::VariableStatementContext::variableDeclarationList() {
  return getRuleContext<JavaScriptParser::VariableDeclarationListContext>(0);
}

tree::TerminalNode* JavaScriptParser::VariableStatementContext::SEMICOLON() {
  return getToken(JavaScriptParser::SEMICOLON, 0);
}

tree::TerminalNode* JavaScriptParser::VariableStatementContext::LET() {
  return getToken(JavaScriptParser::LET, 0);
}

tree::TerminalNode* JavaScriptParser::VariableStatementContext::CONST() {
  return getToken(JavaScriptParser::CONST, 0);
}


size_t JavaScriptParser::VariableStatementContext::getRuleIndex() const {
  return JavaScriptParser::RuleVariableStatement;
}

void JavaScriptParser::VariableStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVariableStatement(this);
}

void JavaScriptParser::VariableStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVariableStatement(this);
}


std::any JavaScriptParser::VariableStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitVariableStatement(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::VariableStatementContext* JavaScriptParser::variableStatement() {
  VariableStatementContext *_localctx = _tracker.createInstance<VariableStatementContext>(_ctx, getState());
  enterRule(_localctx, 12, JavaScriptParser::RuleVariableStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(155);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case JavaScriptParser::VAR: {
        enterOuterAlt(_localctx, 1);
        setState(143);
        match(JavaScriptParser::VAR);
        setState(144);
        variableDeclarationList();
        setState(145);
        match(JavaScriptParser::SEMICOLON);
        break;
      }

      case JavaScriptParser::LET: {
        enterOuterAlt(_localctx, 2);
        setState(147);
        match(JavaScriptParser::LET);
        setState(148);
        variableDeclarationList();
        setState(149);
        match(JavaScriptParser::SEMICOLON);
        break;
      }

      case JavaScriptParser::CONST: {
        enterOuterAlt(_localctx, 3);
        setState(151);
        match(JavaScriptParser::CONST);
        setState(152);
        variableDeclarationList();
        setState(153);
        match(JavaScriptParser::SEMICOLON);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VariableDeclarationListContext ------------------------------------------------------------------

JavaScriptParser::VariableDeclarationListContext::VariableDeclarationListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<JavaScriptParser::VariableDeclarationContext *> JavaScriptParser::VariableDeclarationListContext::variableDeclaration() {
  return getRuleContexts<JavaScriptParser::VariableDeclarationContext>();
}

JavaScriptParser::VariableDeclarationContext* JavaScriptParser::VariableDeclarationListContext::variableDeclaration(size_t i) {
  return getRuleContext<JavaScriptParser::VariableDeclarationContext>(i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::VariableDeclarationListContext::COMMA() {
  return getTokens(JavaScriptParser::COMMA);
}

tree::TerminalNode* JavaScriptParser::VariableDeclarationListContext::COMMA(size_t i) {
  return getToken(JavaScriptParser::COMMA, i);
}


size_t JavaScriptParser::VariableDeclarationListContext::getRuleIndex() const {
  return JavaScriptParser::RuleVariableDeclarationList;
}

void JavaScriptParser::VariableDeclarationListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVariableDeclarationList(this);
}

void JavaScriptParser::VariableDeclarationListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVariableDeclarationList(this);
}


std::any JavaScriptParser::VariableDeclarationListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitVariableDeclarationList(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::VariableDeclarationListContext* JavaScriptParser::variableDeclarationList() {
  VariableDeclarationListContext *_localctx = _tracker.createInstance<VariableDeclarationListContext>(_ctx, getState());
  enterRule(_localctx, 14, JavaScriptParser::RuleVariableDeclarationList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(157);
    variableDeclaration();
    setState(162);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == JavaScriptParser::COMMA) {
      setState(158);
      match(JavaScriptParser::COMMA);
      setState(159);
      variableDeclaration();
      setState(164);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VariableDeclarationContext ------------------------------------------------------------------

JavaScriptParser::VariableDeclarationContext::VariableDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::VariableDeclarationContext::Identifier() {
  return getToken(JavaScriptParser::Identifier, 0);
}

tree::TerminalNode* JavaScriptParser::VariableDeclarationContext::ASSIGN() {
  return getToken(JavaScriptParser::ASSIGN, 0);
}

JavaScriptParser::AssignmentExpressionContext* JavaScriptParser::VariableDeclarationContext::assignmentExpression() {
  return getRuleContext<JavaScriptParser::AssignmentExpressionContext>(0);
}


size_t JavaScriptParser::VariableDeclarationContext::getRuleIndex() const {
  return JavaScriptParser::RuleVariableDeclaration;
}

void JavaScriptParser::VariableDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVariableDeclaration(this);
}

void JavaScriptParser::VariableDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVariableDeclaration(this);
}


std::any JavaScriptParser::VariableDeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitVariableDeclaration(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::VariableDeclarationContext* JavaScriptParser::variableDeclaration() {
  VariableDeclarationContext *_localctx = _tracker.createInstance<VariableDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 16, JavaScriptParser::RuleVariableDeclaration);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(165);
    match(JavaScriptParser::Identifier);
    setState(168);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == JavaScriptParser::ASSIGN) {
      setState(166);
      match(JavaScriptParser::ASSIGN);
      setState(167);
      assignmentExpression();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EmptyStatementContext ------------------------------------------------------------------

JavaScriptParser::EmptyStatementContext::EmptyStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::EmptyStatementContext::SEMICOLON() {
  return getToken(JavaScriptParser::SEMICOLON, 0);
}


size_t JavaScriptParser::EmptyStatementContext::getRuleIndex() const {
  return JavaScriptParser::RuleEmptyStatement;
}

void JavaScriptParser::EmptyStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEmptyStatement(this);
}

void JavaScriptParser::EmptyStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEmptyStatement(this);
}


std::any JavaScriptParser::EmptyStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitEmptyStatement(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::EmptyStatementContext* JavaScriptParser::emptyStatement() {
  EmptyStatementContext *_localctx = _tracker.createInstance<EmptyStatementContext>(_ctx, getState());
  enterRule(_localctx, 18, JavaScriptParser::RuleEmptyStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(170);
    match(JavaScriptParser::SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionStatementContext ------------------------------------------------------------------

JavaScriptParser::ExpressionStatementContext::ExpressionStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JavaScriptParser::ExpressionContext* JavaScriptParser::ExpressionStatementContext::expression() {
  return getRuleContext<JavaScriptParser::ExpressionContext>(0);
}

tree::TerminalNode* JavaScriptParser::ExpressionStatementContext::SEMICOLON() {
  return getToken(JavaScriptParser::SEMICOLON, 0);
}


size_t JavaScriptParser::ExpressionStatementContext::getRuleIndex() const {
  return JavaScriptParser::RuleExpressionStatement;
}

void JavaScriptParser::ExpressionStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpressionStatement(this);
}

void JavaScriptParser::ExpressionStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpressionStatement(this);
}


std::any JavaScriptParser::ExpressionStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitExpressionStatement(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::ExpressionStatementContext* JavaScriptParser::expressionStatement() {
  ExpressionStatementContext *_localctx = _tracker.createInstance<ExpressionStatementContext>(_ctx, getState());
  enterRule(_localctx, 20, JavaScriptParser::RuleExpressionStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(172);
    expression();
    setState(173);
    match(JavaScriptParser::SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IfStatementContext ------------------------------------------------------------------

JavaScriptParser::IfStatementContext::IfStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::IfStatementContext::IF() {
  return getToken(JavaScriptParser::IF, 0);
}

tree::TerminalNode* JavaScriptParser::IfStatementContext::LPAREN() {
  return getToken(JavaScriptParser::LPAREN, 0);
}

JavaScriptParser::ExpressionContext* JavaScriptParser::IfStatementContext::expression() {
  return getRuleContext<JavaScriptParser::ExpressionContext>(0);
}

tree::TerminalNode* JavaScriptParser::IfStatementContext::RPAREN() {
  return getToken(JavaScriptParser::RPAREN, 0);
}

std::vector<JavaScriptParser::StatementContext *> JavaScriptParser::IfStatementContext::statement() {
  return getRuleContexts<JavaScriptParser::StatementContext>();
}

JavaScriptParser::StatementContext* JavaScriptParser::IfStatementContext::statement(size_t i) {
  return getRuleContext<JavaScriptParser::StatementContext>(i);
}

tree::TerminalNode* JavaScriptParser::IfStatementContext::ELSE() {
  return getToken(JavaScriptParser::ELSE, 0);
}


size_t JavaScriptParser::IfStatementContext::getRuleIndex() const {
  return JavaScriptParser::RuleIfStatement;
}

void JavaScriptParser::IfStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIfStatement(this);
}

void JavaScriptParser::IfStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIfStatement(this);
}


std::any JavaScriptParser::IfStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitIfStatement(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::IfStatementContext* JavaScriptParser::ifStatement() {
  IfStatementContext *_localctx = _tracker.createInstance<IfStatementContext>(_ctx, getState());
  enterRule(_localctx, 22, JavaScriptParser::RuleIfStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(175);
    match(JavaScriptParser::IF);
    setState(176);
    match(JavaScriptParser::LPAREN);
    setState(177);
    expression();
    setState(178);
    match(JavaScriptParser::RPAREN);
    setState(179);
    statement();
    setState(182);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 9, _ctx)) {
    case 1: {
      setState(180);
      match(JavaScriptParser::ELSE);
      setState(181);
      statement();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IterationStatementContext ------------------------------------------------------------------

JavaScriptParser::IterationStatementContext::IterationStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t JavaScriptParser::IterationStatementContext::getRuleIndex() const {
  return JavaScriptParser::RuleIterationStatement;
}

void JavaScriptParser::IterationStatementContext::copyFrom(IterationStatementContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- WhileStatementContext ------------------------------------------------------------------

tree::TerminalNode* JavaScriptParser::WhileStatementContext::WHILE() {
  return getToken(JavaScriptParser::WHILE, 0);
}

tree::TerminalNode* JavaScriptParser::WhileStatementContext::LPAREN() {
  return getToken(JavaScriptParser::LPAREN, 0);
}

JavaScriptParser::ExpressionContext* JavaScriptParser::WhileStatementContext::expression() {
  return getRuleContext<JavaScriptParser::ExpressionContext>(0);
}

tree::TerminalNode* JavaScriptParser::WhileStatementContext::RPAREN() {
  return getToken(JavaScriptParser::RPAREN, 0);
}

JavaScriptParser::StatementContext* JavaScriptParser::WhileStatementContext::statement() {
  return getRuleContext<JavaScriptParser::StatementContext>(0);
}

JavaScriptParser::WhileStatementContext::WhileStatementContext(IterationStatementContext *ctx) { copyFrom(ctx); }

void JavaScriptParser::WhileStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterWhileStatement(this);
}
void JavaScriptParser::WhileStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitWhileStatement(this);
}

std::any JavaScriptParser::WhileStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitWhileStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ForStatementContext ------------------------------------------------------------------

tree::TerminalNode* JavaScriptParser::ForStatementContext::FOR() {
  return getToken(JavaScriptParser::FOR, 0);
}

tree::TerminalNode* JavaScriptParser::ForStatementContext::LPAREN() {
  return getToken(JavaScriptParser::LPAREN, 0);
}

std::vector<tree::TerminalNode *> JavaScriptParser::ForStatementContext::SEMICOLON() {
  return getTokens(JavaScriptParser::SEMICOLON);
}

tree::TerminalNode* JavaScriptParser::ForStatementContext::SEMICOLON(size_t i) {
  return getToken(JavaScriptParser::SEMICOLON, i);
}

tree::TerminalNode* JavaScriptParser::ForStatementContext::RPAREN() {
  return getToken(JavaScriptParser::RPAREN, 0);
}

JavaScriptParser::StatementContext* JavaScriptParser::ForStatementContext::statement() {
  return getRuleContext<JavaScriptParser::StatementContext>(0);
}

JavaScriptParser::ForInitContext* JavaScriptParser::ForStatementContext::forInit() {
  return getRuleContext<JavaScriptParser::ForInitContext>(0);
}

std::vector<JavaScriptParser::ExpressionContext *> JavaScriptParser::ForStatementContext::expression() {
  return getRuleContexts<JavaScriptParser::ExpressionContext>();
}

JavaScriptParser::ExpressionContext* JavaScriptParser::ForStatementContext::expression(size_t i) {
  return getRuleContext<JavaScriptParser::ExpressionContext>(i);
}

JavaScriptParser::ForStatementContext::ForStatementContext(IterationStatementContext *ctx) { copyFrom(ctx); }

void JavaScriptParser::ForStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForStatement(this);
}
void JavaScriptParser::ForStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForStatement(this);
}

std::any JavaScriptParser::ForStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitForStatement(this);
  else
    return visitor->visitChildren(this);
}
JavaScriptParser::IterationStatementContext* JavaScriptParser::iterationStatement() {
  IterationStatementContext *_localctx = _tracker.createInstance<IterationStatementContext>(_ctx, getState());
  enterRule(_localctx, 24, JavaScriptParser::RuleIterationStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(205);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case JavaScriptParser::WHILE: {
        _localctx = _tracker.createInstance<JavaScriptParser::WhileStatementContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(184);
        match(JavaScriptParser::WHILE);
        setState(185);
        match(JavaScriptParser::LPAREN);
        setState(186);
        expression();
        setState(187);
        match(JavaScriptParser::RPAREN);
        setState(188);
        statement();
        break;
      }

      case JavaScriptParser::FOR: {
        _localctx = _tracker.createInstance<JavaScriptParser::ForStatementContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(190);
        match(JavaScriptParser::FOR);
        setState(191);
        match(JavaScriptParser::LPAREN);
        setState(193);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & -4742290407610349504) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 65)) & 993) != 0)) {
          setState(192);
          forInit();
        }
        setState(195);
        match(JavaScriptParser::SEMICOLON);
        setState(197);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & -4742290407610349504) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 65)) & 993) != 0)) {
          setState(196);
          expression();
        }
        setState(199);
        match(JavaScriptParser::SEMICOLON);
        setState(201);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & -4742290407610349504) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 65)) & 993) != 0)) {
          setState(200);
          expression();
        }
        setState(203);
        match(JavaScriptParser::RPAREN);
        setState(204);
        statement();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ForInitContext ------------------------------------------------------------------

JavaScriptParser::ForInitContext::ForInitContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JavaScriptParser::VariableDeclarationListContext* JavaScriptParser::ForInitContext::variableDeclarationList() {
  return getRuleContext<JavaScriptParser::VariableDeclarationListContext>(0);
}

JavaScriptParser::ExpressionContext* JavaScriptParser::ForInitContext::expression() {
  return getRuleContext<JavaScriptParser::ExpressionContext>(0);
}


size_t JavaScriptParser::ForInitContext::getRuleIndex() const {
  return JavaScriptParser::RuleForInit;
}

void JavaScriptParser::ForInitContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForInit(this);
}

void JavaScriptParser::ForInitContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForInit(this);
}


std::any JavaScriptParser::ForInitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitForInit(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::ForInitContext* JavaScriptParser::forInit() {
  ForInitContext *_localctx = _tracker.createInstance<ForInitContext>(_ctx, getState());
  enterRule(_localctx, 26, JavaScriptParser::RuleForInit);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(209);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(207);
      variableDeclarationList();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(208);
      expression();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ContinueStatementContext ------------------------------------------------------------------

JavaScriptParser::ContinueStatementContext::ContinueStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::ContinueStatementContext::CONTINUE() {
  return getToken(JavaScriptParser::CONTINUE, 0);
}

tree::TerminalNode* JavaScriptParser::ContinueStatementContext::SEMICOLON() {
  return getToken(JavaScriptParser::SEMICOLON, 0);
}

tree::TerminalNode* JavaScriptParser::ContinueStatementContext::Identifier() {
  return getToken(JavaScriptParser::Identifier, 0);
}


size_t JavaScriptParser::ContinueStatementContext::getRuleIndex() const {
  return JavaScriptParser::RuleContinueStatement;
}

void JavaScriptParser::ContinueStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterContinueStatement(this);
}

void JavaScriptParser::ContinueStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitContinueStatement(this);
}


std::any JavaScriptParser::ContinueStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitContinueStatement(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::ContinueStatementContext* JavaScriptParser::continueStatement() {
  ContinueStatementContext *_localctx = _tracker.createInstance<ContinueStatementContext>(_ctx, getState());
  enterRule(_localctx, 28, JavaScriptParser::RuleContinueStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(211);
    match(JavaScriptParser::CONTINUE);
    setState(213);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == JavaScriptParser::Identifier) {
      setState(212);
      match(JavaScriptParser::Identifier);
    }
    setState(215);
    match(JavaScriptParser::SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BreakStatementContext ------------------------------------------------------------------

JavaScriptParser::BreakStatementContext::BreakStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::BreakStatementContext::BREAK() {
  return getToken(JavaScriptParser::BREAK, 0);
}

tree::TerminalNode* JavaScriptParser::BreakStatementContext::SEMICOLON() {
  return getToken(JavaScriptParser::SEMICOLON, 0);
}

tree::TerminalNode* JavaScriptParser::BreakStatementContext::Identifier() {
  return getToken(JavaScriptParser::Identifier, 0);
}


size_t JavaScriptParser::BreakStatementContext::getRuleIndex() const {
  return JavaScriptParser::RuleBreakStatement;
}

void JavaScriptParser::BreakStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBreakStatement(this);
}

void JavaScriptParser::BreakStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBreakStatement(this);
}


std::any JavaScriptParser::BreakStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitBreakStatement(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::BreakStatementContext* JavaScriptParser::breakStatement() {
  BreakStatementContext *_localctx = _tracker.createInstance<BreakStatementContext>(_ctx, getState());
  enterRule(_localctx, 30, JavaScriptParser::RuleBreakStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(217);
    match(JavaScriptParser::BREAK);
    setState(219);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == JavaScriptParser::Identifier) {
      setState(218);
      match(JavaScriptParser::Identifier);
    }
    setState(221);
    match(JavaScriptParser::SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnStatementContext ------------------------------------------------------------------

JavaScriptParser::ReturnStatementContext::ReturnStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::ReturnStatementContext::RETURN() {
  return getToken(JavaScriptParser::RETURN, 0);
}

tree::TerminalNode* JavaScriptParser::ReturnStatementContext::SEMICOLON() {
  return getToken(JavaScriptParser::SEMICOLON, 0);
}

JavaScriptParser::ExpressionContext* JavaScriptParser::ReturnStatementContext::expression() {
  return getRuleContext<JavaScriptParser::ExpressionContext>(0);
}


size_t JavaScriptParser::ReturnStatementContext::getRuleIndex() const {
  return JavaScriptParser::RuleReturnStatement;
}

void JavaScriptParser::ReturnStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterReturnStatement(this);
}

void JavaScriptParser::ReturnStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitReturnStatement(this);
}


std::any JavaScriptParser::ReturnStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitReturnStatement(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::ReturnStatementContext* JavaScriptParser::returnStatement() {
  ReturnStatementContext *_localctx = _tracker.createInstance<ReturnStatementContext>(_ctx, getState());
  enterRule(_localctx, 32, JavaScriptParser::RuleReturnStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(223);
    match(JavaScriptParser::RETURN);
    setState(225);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -4742290407610349504) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 65)) & 993) != 0)) {
      setState(224);
      expression();
    }
    setState(227);
    match(JavaScriptParser::SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TryStatementContext ------------------------------------------------------------------

JavaScriptParser::TryStatementContext::TryStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::TryStatementContext::TRY() {
  return getToken(JavaScriptParser::TRY, 0);
}

JavaScriptParser::BlockContext* JavaScriptParser::TryStatementContext::block() {
  return getRuleContext<JavaScriptParser::BlockContext>(0);
}

JavaScriptParser::CatchClauseContext* JavaScriptParser::TryStatementContext::catchClause() {
  return getRuleContext<JavaScriptParser::CatchClauseContext>(0);
}

JavaScriptParser::FinallyClauseContext* JavaScriptParser::TryStatementContext::finallyClause() {
  return getRuleContext<JavaScriptParser::FinallyClauseContext>(0);
}


size_t JavaScriptParser::TryStatementContext::getRuleIndex() const {
  return JavaScriptParser::RuleTryStatement;
}

void JavaScriptParser::TryStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTryStatement(this);
}

void JavaScriptParser::TryStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTryStatement(this);
}


std::any JavaScriptParser::TryStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitTryStatement(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::TryStatementContext* JavaScriptParser::tryStatement() {
  TryStatementContext *_localctx = _tracker.createInstance<TryStatementContext>(_ctx, getState());
  enterRule(_localctx, 34, JavaScriptParser::RuleTryStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(229);
    match(JavaScriptParser::TRY);
    setState(230);
    block();
    setState(236);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case JavaScriptParser::CATCH: {
        setState(231);
        catchClause();
        setState(233);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == JavaScriptParser::FINALLY) {
          setState(232);
          finallyClause();
        }
        break;
      }

      case JavaScriptParser::FINALLY: {
        setState(235);
        finallyClause();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CatchClauseContext ------------------------------------------------------------------

JavaScriptParser::CatchClauseContext::CatchClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::CatchClauseContext::CATCH() {
  return getToken(JavaScriptParser::CATCH, 0);
}

tree::TerminalNode* JavaScriptParser::CatchClauseContext::LPAREN() {
  return getToken(JavaScriptParser::LPAREN, 0);
}

tree::TerminalNode* JavaScriptParser::CatchClauseContext::Identifier() {
  return getToken(JavaScriptParser::Identifier, 0);
}

tree::TerminalNode* JavaScriptParser::CatchClauseContext::RPAREN() {
  return getToken(JavaScriptParser::RPAREN, 0);
}

JavaScriptParser::BlockContext* JavaScriptParser::CatchClauseContext::block() {
  return getRuleContext<JavaScriptParser::BlockContext>(0);
}


size_t JavaScriptParser::CatchClauseContext::getRuleIndex() const {
  return JavaScriptParser::RuleCatchClause;
}

void JavaScriptParser::CatchClauseContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCatchClause(this);
}

void JavaScriptParser::CatchClauseContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCatchClause(this);
}


std::any JavaScriptParser::CatchClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitCatchClause(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::CatchClauseContext* JavaScriptParser::catchClause() {
  CatchClauseContext *_localctx = _tracker.createInstance<CatchClauseContext>(_ctx, getState());
  enterRule(_localctx, 36, JavaScriptParser::RuleCatchClause);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(238);
    match(JavaScriptParser::CATCH);
    setState(239);
    match(JavaScriptParser::LPAREN);
    setState(240);
    match(JavaScriptParser::Identifier);
    setState(241);
    match(JavaScriptParser::RPAREN);
    setState(242);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FinallyClauseContext ------------------------------------------------------------------

JavaScriptParser::FinallyClauseContext::FinallyClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::FinallyClauseContext::FINALLY() {
  return getToken(JavaScriptParser::FINALLY, 0);
}

JavaScriptParser::BlockContext* JavaScriptParser::FinallyClauseContext::block() {
  return getRuleContext<JavaScriptParser::BlockContext>(0);
}


size_t JavaScriptParser::FinallyClauseContext::getRuleIndex() const {
  return JavaScriptParser::RuleFinallyClause;
}

void JavaScriptParser::FinallyClauseContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFinallyClause(this);
}

void JavaScriptParser::FinallyClauseContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFinallyClause(this);
}


std::any JavaScriptParser::FinallyClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitFinallyClause(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::FinallyClauseContext* JavaScriptParser::finallyClause() {
  FinallyClauseContext *_localctx = _tracker.createInstance<FinallyClauseContext>(_ctx, getState());
  enterRule(_localctx, 38, JavaScriptParser::RuleFinallyClause);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(244);
    match(JavaScriptParser::FINALLY);
    setState(245);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionDeclarationContext ------------------------------------------------------------------

JavaScriptParser::FunctionDeclarationContext::FunctionDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::FunctionDeclarationContext::FUNCTION() {
  return getToken(JavaScriptParser::FUNCTION, 0);
}

tree::TerminalNode* JavaScriptParser::FunctionDeclarationContext::Identifier() {
  return getToken(JavaScriptParser::Identifier, 0);
}

tree::TerminalNode* JavaScriptParser::FunctionDeclarationContext::LPAREN() {
  return getToken(JavaScriptParser::LPAREN, 0);
}

tree::TerminalNode* JavaScriptParser::FunctionDeclarationContext::RPAREN() {
  return getToken(JavaScriptParser::RPAREN, 0);
}

tree::TerminalNode* JavaScriptParser::FunctionDeclarationContext::LBRACE() {
  return getToken(JavaScriptParser::LBRACE, 0);
}

JavaScriptParser::FunctionBodyContext* JavaScriptParser::FunctionDeclarationContext::functionBody() {
  return getRuleContext<JavaScriptParser::FunctionBodyContext>(0);
}

tree::TerminalNode* JavaScriptParser::FunctionDeclarationContext::RBRACE() {
  return getToken(JavaScriptParser::RBRACE, 0);
}

JavaScriptParser::FormalParameterListContext* JavaScriptParser::FunctionDeclarationContext::formalParameterList() {
  return getRuleContext<JavaScriptParser::FormalParameterListContext>(0);
}


size_t JavaScriptParser::FunctionDeclarationContext::getRuleIndex() const {
  return JavaScriptParser::RuleFunctionDeclaration;
}

void JavaScriptParser::FunctionDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionDeclaration(this);
}

void JavaScriptParser::FunctionDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionDeclaration(this);
}


std::any JavaScriptParser::FunctionDeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitFunctionDeclaration(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::FunctionDeclarationContext* JavaScriptParser::functionDeclaration() {
  FunctionDeclarationContext *_localctx = _tracker.createInstance<FunctionDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 40, JavaScriptParser::RuleFunctionDeclaration);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(247);
    match(JavaScriptParser::FUNCTION);
    setState(248);
    match(JavaScriptParser::Identifier);
    setState(249);
    match(JavaScriptParser::LPAREN);
    setState(251);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == JavaScriptParser::Identifier) {
      setState(250);
      formalParameterList();
    }
    setState(253);
    match(JavaScriptParser::RPAREN);
    setState(254);
    match(JavaScriptParser::LBRACE);
    setState(255);
    functionBody();
    setState(256);
    match(JavaScriptParser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FormalParameterListContext ------------------------------------------------------------------

JavaScriptParser::FormalParameterListContext::FormalParameterListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> JavaScriptParser::FormalParameterListContext::Identifier() {
  return getTokens(JavaScriptParser::Identifier);
}

tree::TerminalNode* JavaScriptParser::FormalParameterListContext::Identifier(size_t i) {
  return getToken(JavaScriptParser::Identifier, i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::FormalParameterListContext::COMMA() {
  return getTokens(JavaScriptParser::COMMA);
}

tree::TerminalNode* JavaScriptParser::FormalParameterListContext::COMMA(size_t i) {
  return getToken(JavaScriptParser::COMMA, i);
}


size_t JavaScriptParser::FormalParameterListContext::getRuleIndex() const {
  return JavaScriptParser::RuleFormalParameterList;
}

void JavaScriptParser::FormalParameterListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFormalParameterList(this);
}

void JavaScriptParser::FormalParameterListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFormalParameterList(this);
}


std::any JavaScriptParser::FormalParameterListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitFormalParameterList(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::FormalParameterListContext* JavaScriptParser::formalParameterList() {
  FormalParameterListContext *_localctx = _tracker.createInstance<FormalParameterListContext>(_ctx, getState());
  enterRule(_localctx, 42, JavaScriptParser::RuleFormalParameterList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(258);
    match(JavaScriptParser::Identifier);
    setState(263);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == JavaScriptParser::COMMA) {
      setState(259);
      match(JavaScriptParser::COMMA);
      setState(260);
      match(JavaScriptParser::Identifier);
      setState(265);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionBodyContext ------------------------------------------------------------------

JavaScriptParser::FunctionBodyContext::FunctionBodyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JavaScriptParser::SourceElementsContext* JavaScriptParser::FunctionBodyContext::sourceElements() {
  return getRuleContext<JavaScriptParser::SourceElementsContext>(0);
}


size_t JavaScriptParser::FunctionBodyContext::getRuleIndex() const {
  return JavaScriptParser::RuleFunctionBody;
}

void JavaScriptParser::FunctionBodyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionBody(this);
}

void JavaScriptParser::FunctionBodyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionBody(this);
}


std::any JavaScriptParser::FunctionBodyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitFunctionBody(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::FunctionBodyContext* JavaScriptParser::functionBody() {
  FunctionBodyContext *_localctx = _tracker.createInstance<FunctionBodyContext>(_ctx, getState());
  enterRule(_localctx, 44, JavaScriptParser::RuleFunctionBody);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(267);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -4742290407386932142) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 65)) & 997) != 0)) {
      setState(266);
      sourceElements();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

JavaScriptParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JavaScriptParser::AssignmentExpressionContext* JavaScriptParser::ExpressionContext::assignmentExpression() {
  return getRuleContext<JavaScriptParser::AssignmentExpressionContext>(0);
}


size_t JavaScriptParser::ExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RuleExpression;
}

void JavaScriptParser::ExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpression(this);
}

void JavaScriptParser::ExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpression(this);
}


std::any JavaScriptParser::ExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitExpression(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::ExpressionContext* JavaScriptParser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 46, JavaScriptParser::RuleExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(269);
    assignmentExpression();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentExpressionContext ------------------------------------------------------------------

JavaScriptParser::AssignmentExpressionContext::AssignmentExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JavaScriptParser::ConditionalExpressionContext* JavaScriptParser::AssignmentExpressionContext::conditionalExpression() {
  return getRuleContext<JavaScriptParser::ConditionalExpressionContext>(0);
}

JavaScriptParser::LeftHandSideExpressionContext* JavaScriptParser::AssignmentExpressionContext::leftHandSideExpression() {
  return getRuleContext<JavaScriptParser::LeftHandSideExpressionContext>(0);
}

JavaScriptParser::AssignmentOperatorContext* JavaScriptParser::AssignmentExpressionContext::assignmentOperator() {
  return getRuleContext<JavaScriptParser::AssignmentOperatorContext>(0);
}

JavaScriptParser::AssignmentExpressionContext* JavaScriptParser::AssignmentExpressionContext::assignmentExpression() {
  return getRuleContext<JavaScriptParser::AssignmentExpressionContext>(0);
}


size_t JavaScriptParser::AssignmentExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RuleAssignmentExpression;
}

void JavaScriptParser::AssignmentExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssignmentExpression(this);
}

void JavaScriptParser::AssignmentExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssignmentExpression(this);
}


std::any JavaScriptParser::AssignmentExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitAssignmentExpression(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::AssignmentExpressionContext* JavaScriptParser::assignmentExpression() {
  AssignmentExpressionContext *_localctx = _tracker.createInstance<AssignmentExpressionContext>(_ctx, getState());
  enterRule(_localctx, 48, JavaScriptParser::RuleAssignmentExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(276);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(271);
      conditionalExpression();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(272);
      leftHandSideExpression();
      setState(273);
      assignmentOperator();
      setState(274);
      assignmentExpression();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentOperatorContext ------------------------------------------------------------------

JavaScriptParser::AssignmentOperatorContext::AssignmentOperatorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::AssignmentOperatorContext::ASSIGN() {
  return getToken(JavaScriptParser::ASSIGN, 0);
}

tree::TerminalNode* JavaScriptParser::AssignmentOperatorContext::MUL_ASSIGN() {
  return getToken(JavaScriptParser::MUL_ASSIGN, 0);
}

tree::TerminalNode* JavaScriptParser::AssignmentOperatorContext::DIV_ASSIGN() {
  return getToken(JavaScriptParser::DIV_ASSIGN, 0);
}

tree::TerminalNode* JavaScriptParser::AssignmentOperatorContext::MOD_ASSIGN() {
  return getToken(JavaScriptParser::MOD_ASSIGN, 0);
}

tree::TerminalNode* JavaScriptParser::AssignmentOperatorContext::PLUS_ASSIGN() {
  return getToken(JavaScriptParser::PLUS_ASSIGN, 0);
}

tree::TerminalNode* JavaScriptParser::AssignmentOperatorContext::MINUS_ASSIGN() {
  return getToken(JavaScriptParser::MINUS_ASSIGN, 0);
}


size_t JavaScriptParser::AssignmentOperatorContext::getRuleIndex() const {
  return JavaScriptParser::RuleAssignmentOperator;
}

void JavaScriptParser::AssignmentOperatorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssignmentOperator(this);
}

void JavaScriptParser::AssignmentOperatorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssignmentOperator(this);
}


std::any JavaScriptParser::AssignmentOperatorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitAssignmentOperator(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::AssignmentOperatorContext* JavaScriptParser::assignmentOperator() {
  AssignmentOperatorContext *_localctx = _tracker.createInstance<AssignmentOperatorContext>(_ctx, getState());
  enterRule(_localctx, 50, JavaScriptParser::RuleAssignmentOperator);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(278);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 16911433728) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConditionalExpressionContext ------------------------------------------------------------------

JavaScriptParser::ConditionalExpressionContext::ConditionalExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JavaScriptParser::LogicalORExpressionContext* JavaScriptParser::ConditionalExpressionContext::logicalORExpression() {
  return getRuleContext<JavaScriptParser::LogicalORExpressionContext>(0);
}

tree::TerminalNode* JavaScriptParser::ConditionalExpressionContext::QUESTION() {
  return getToken(JavaScriptParser::QUESTION, 0);
}

std::vector<JavaScriptParser::AssignmentExpressionContext *> JavaScriptParser::ConditionalExpressionContext::assignmentExpression() {
  return getRuleContexts<JavaScriptParser::AssignmentExpressionContext>();
}

JavaScriptParser::AssignmentExpressionContext* JavaScriptParser::ConditionalExpressionContext::assignmentExpression(size_t i) {
  return getRuleContext<JavaScriptParser::AssignmentExpressionContext>(i);
}

tree::TerminalNode* JavaScriptParser::ConditionalExpressionContext::COLON() {
  return getToken(JavaScriptParser::COLON, 0);
}


size_t JavaScriptParser::ConditionalExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RuleConditionalExpression;
}

void JavaScriptParser::ConditionalExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConditionalExpression(this);
}

void JavaScriptParser::ConditionalExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConditionalExpression(this);
}


std::any JavaScriptParser::ConditionalExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitConditionalExpression(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::ConditionalExpressionContext* JavaScriptParser::conditionalExpression() {
  ConditionalExpressionContext *_localctx = _tracker.createInstance<ConditionalExpressionContext>(_ctx, getState());
  enterRule(_localctx, 52, JavaScriptParser::RuleConditionalExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(280);
    logicalORExpression();
    setState(286);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == JavaScriptParser::QUESTION) {
      setState(281);
      match(JavaScriptParser::QUESTION);
      setState(282);
      assignmentExpression();
      setState(283);
      match(JavaScriptParser::COLON);
      setState(284);
      assignmentExpression();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LogicalORExpressionContext ------------------------------------------------------------------

JavaScriptParser::LogicalORExpressionContext::LogicalORExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<JavaScriptParser::LogicalANDExpressionContext *> JavaScriptParser::LogicalORExpressionContext::logicalANDExpression() {
  return getRuleContexts<JavaScriptParser::LogicalANDExpressionContext>();
}

JavaScriptParser::LogicalANDExpressionContext* JavaScriptParser::LogicalORExpressionContext::logicalANDExpression(size_t i) {
  return getRuleContext<JavaScriptParser::LogicalANDExpressionContext>(i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::LogicalORExpressionContext::OR() {
  return getTokens(JavaScriptParser::OR);
}

tree::TerminalNode* JavaScriptParser::LogicalORExpressionContext::OR(size_t i) {
  return getToken(JavaScriptParser::OR, i);
}


size_t JavaScriptParser::LogicalORExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RuleLogicalORExpression;
}

void JavaScriptParser::LogicalORExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLogicalORExpression(this);
}

void JavaScriptParser::LogicalORExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLogicalORExpression(this);
}


std::any JavaScriptParser::LogicalORExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitLogicalORExpression(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::LogicalORExpressionContext* JavaScriptParser::logicalORExpression() {
  LogicalORExpressionContext *_localctx = _tracker.createInstance<LogicalORExpressionContext>(_ctx, getState());
  enterRule(_localctx, 54, JavaScriptParser::RuleLogicalORExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(288);
    logicalANDExpression();
    setState(293);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == JavaScriptParser::OR) {
      setState(289);
      match(JavaScriptParser::OR);
      setState(290);
      logicalANDExpression();
      setState(295);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LogicalANDExpressionContext ------------------------------------------------------------------

JavaScriptParser::LogicalANDExpressionContext::LogicalANDExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<JavaScriptParser::BitwiseORExpressionContext *> JavaScriptParser::LogicalANDExpressionContext::bitwiseORExpression() {
  return getRuleContexts<JavaScriptParser::BitwiseORExpressionContext>();
}

JavaScriptParser::BitwiseORExpressionContext* JavaScriptParser::LogicalANDExpressionContext::bitwiseORExpression(size_t i) {
  return getRuleContext<JavaScriptParser::BitwiseORExpressionContext>(i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::LogicalANDExpressionContext::AND() {
  return getTokens(JavaScriptParser::AND);
}

tree::TerminalNode* JavaScriptParser::LogicalANDExpressionContext::AND(size_t i) {
  return getToken(JavaScriptParser::AND, i);
}


size_t JavaScriptParser::LogicalANDExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RuleLogicalANDExpression;
}

void JavaScriptParser::LogicalANDExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLogicalANDExpression(this);
}

void JavaScriptParser::LogicalANDExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLogicalANDExpression(this);
}


std::any JavaScriptParser::LogicalANDExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitLogicalANDExpression(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::LogicalANDExpressionContext* JavaScriptParser::logicalANDExpression() {
  LogicalANDExpressionContext *_localctx = _tracker.createInstance<LogicalANDExpressionContext>(_ctx, getState());
  enterRule(_localctx, 56, JavaScriptParser::RuleLogicalANDExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(296);
    bitwiseORExpression();
    setState(301);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == JavaScriptParser::AND) {
      setState(297);
      match(JavaScriptParser::AND);
      setState(298);
      bitwiseORExpression();
      setState(303);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BitwiseORExpressionContext ------------------------------------------------------------------

JavaScriptParser::BitwiseORExpressionContext::BitwiseORExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<JavaScriptParser::BitwiseXORExpressionContext *> JavaScriptParser::BitwiseORExpressionContext::bitwiseXORExpression() {
  return getRuleContexts<JavaScriptParser::BitwiseXORExpressionContext>();
}

JavaScriptParser::BitwiseXORExpressionContext* JavaScriptParser::BitwiseORExpressionContext::bitwiseXORExpression(size_t i) {
  return getRuleContext<JavaScriptParser::BitwiseXORExpressionContext>(i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::BitwiseORExpressionContext::BIT_OR() {
  return getTokens(JavaScriptParser::BIT_OR);
}

tree::TerminalNode* JavaScriptParser::BitwiseORExpressionContext::BIT_OR(size_t i) {
  return getToken(JavaScriptParser::BIT_OR, i);
}


size_t JavaScriptParser::BitwiseORExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RuleBitwiseORExpression;
}

void JavaScriptParser::BitwiseORExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBitwiseORExpression(this);
}

void JavaScriptParser::BitwiseORExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBitwiseORExpression(this);
}


std::any JavaScriptParser::BitwiseORExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitBitwiseORExpression(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::BitwiseORExpressionContext* JavaScriptParser::bitwiseORExpression() {
  BitwiseORExpressionContext *_localctx = _tracker.createInstance<BitwiseORExpressionContext>(_ctx, getState());
  enterRule(_localctx, 58, JavaScriptParser::RuleBitwiseORExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(304);
    bitwiseXORExpression();
    setState(309);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == JavaScriptParser::BIT_OR) {
      setState(305);
      match(JavaScriptParser::BIT_OR);
      setState(306);
      bitwiseXORExpression();
      setState(311);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BitwiseXORExpressionContext ------------------------------------------------------------------

JavaScriptParser::BitwiseXORExpressionContext::BitwiseXORExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<JavaScriptParser::BitwiseANDExpressionContext *> JavaScriptParser::BitwiseXORExpressionContext::bitwiseANDExpression() {
  return getRuleContexts<JavaScriptParser::BitwiseANDExpressionContext>();
}

JavaScriptParser::BitwiseANDExpressionContext* JavaScriptParser::BitwiseXORExpressionContext::bitwiseANDExpression(size_t i) {
  return getRuleContext<JavaScriptParser::BitwiseANDExpressionContext>(i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::BitwiseXORExpressionContext::BIT_XOR() {
  return getTokens(JavaScriptParser::BIT_XOR);
}

tree::TerminalNode* JavaScriptParser::BitwiseXORExpressionContext::BIT_XOR(size_t i) {
  return getToken(JavaScriptParser::BIT_XOR, i);
}


size_t JavaScriptParser::BitwiseXORExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RuleBitwiseXORExpression;
}

void JavaScriptParser::BitwiseXORExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBitwiseXORExpression(this);
}

void JavaScriptParser::BitwiseXORExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBitwiseXORExpression(this);
}


std::any JavaScriptParser::BitwiseXORExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitBitwiseXORExpression(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::BitwiseXORExpressionContext* JavaScriptParser::bitwiseXORExpression() {
  BitwiseXORExpressionContext *_localctx = _tracker.createInstance<BitwiseXORExpressionContext>(_ctx, getState());
  enterRule(_localctx, 60, JavaScriptParser::RuleBitwiseXORExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(312);
    bitwiseANDExpression();
    setState(317);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == JavaScriptParser::BIT_XOR) {
      setState(313);
      match(JavaScriptParser::BIT_XOR);
      setState(314);
      bitwiseANDExpression();
      setState(319);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BitwiseANDExpressionContext ------------------------------------------------------------------

JavaScriptParser::BitwiseANDExpressionContext::BitwiseANDExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<JavaScriptParser::EqualityExpressionContext *> JavaScriptParser::BitwiseANDExpressionContext::equalityExpression() {
  return getRuleContexts<JavaScriptParser::EqualityExpressionContext>();
}

JavaScriptParser::EqualityExpressionContext* JavaScriptParser::BitwiseANDExpressionContext::equalityExpression(size_t i) {
  return getRuleContext<JavaScriptParser::EqualityExpressionContext>(i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::BitwiseANDExpressionContext::BIT_AND() {
  return getTokens(JavaScriptParser::BIT_AND);
}

tree::TerminalNode* JavaScriptParser::BitwiseANDExpressionContext::BIT_AND(size_t i) {
  return getToken(JavaScriptParser::BIT_AND, i);
}


size_t JavaScriptParser::BitwiseANDExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RuleBitwiseANDExpression;
}

void JavaScriptParser::BitwiseANDExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBitwiseANDExpression(this);
}

void JavaScriptParser::BitwiseANDExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBitwiseANDExpression(this);
}


std::any JavaScriptParser::BitwiseANDExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitBitwiseANDExpression(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::BitwiseANDExpressionContext* JavaScriptParser::bitwiseANDExpression() {
  BitwiseANDExpressionContext *_localctx = _tracker.createInstance<BitwiseANDExpressionContext>(_ctx, getState());
  enterRule(_localctx, 62, JavaScriptParser::RuleBitwiseANDExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(320);
    equalityExpression();
    setState(325);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == JavaScriptParser::BIT_AND) {
      setState(321);
      match(JavaScriptParser::BIT_AND);
      setState(322);
      equalityExpression();
      setState(327);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EqualityExpressionContext ------------------------------------------------------------------

JavaScriptParser::EqualityExpressionContext::EqualityExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<JavaScriptParser::RelationalExpressionContext *> JavaScriptParser::EqualityExpressionContext::relationalExpression() {
  return getRuleContexts<JavaScriptParser::RelationalExpressionContext>();
}

JavaScriptParser::RelationalExpressionContext* JavaScriptParser::EqualityExpressionContext::relationalExpression(size_t i) {
  return getRuleContext<JavaScriptParser::RelationalExpressionContext>(i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::EqualityExpressionContext::EQUALS() {
  return getTokens(JavaScriptParser::EQUALS);
}

tree::TerminalNode* JavaScriptParser::EqualityExpressionContext::EQUALS(size_t i) {
  return getToken(JavaScriptParser::EQUALS, i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::EqualityExpressionContext::NOT_EQUALS() {
  return getTokens(JavaScriptParser::NOT_EQUALS);
}

tree::TerminalNode* JavaScriptParser::EqualityExpressionContext::NOT_EQUALS(size_t i) {
  return getToken(JavaScriptParser::NOT_EQUALS, i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::EqualityExpressionContext::STRICT_EQUALS() {
  return getTokens(JavaScriptParser::STRICT_EQUALS);
}

tree::TerminalNode* JavaScriptParser::EqualityExpressionContext::STRICT_EQUALS(size_t i) {
  return getToken(JavaScriptParser::STRICT_EQUALS, i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::EqualityExpressionContext::STRICT_NOT_EQUALS() {
  return getTokens(JavaScriptParser::STRICT_NOT_EQUALS);
}

tree::TerminalNode* JavaScriptParser::EqualityExpressionContext::STRICT_NOT_EQUALS(size_t i) {
  return getToken(JavaScriptParser::STRICT_NOT_EQUALS, i);
}


size_t JavaScriptParser::EqualityExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RuleEqualityExpression;
}

void JavaScriptParser::EqualityExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEqualityExpression(this);
}

void JavaScriptParser::EqualityExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEqualityExpression(this);
}


std::any JavaScriptParser::EqualityExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitEqualityExpression(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::EqualityExpressionContext* JavaScriptParser::equalityExpression() {
  EqualityExpressionContext *_localctx = _tracker.createInstance<EqualityExpressionContext>(_ctx, getState());
  enterRule(_localctx, 64, JavaScriptParser::RuleEqualityExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(328);
    relationalExpression();
    setState(333);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 32985348833280) != 0)) {
      setState(329);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 32985348833280) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(330);
      relationalExpression();
      setState(335);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RelationalExpressionContext ------------------------------------------------------------------

JavaScriptParser::RelationalExpressionContext::RelationalExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<JavaScriptParser::ShiftExpressionContext *> JavaScriptParser::RelationalExpressionContext::shiftExpression() {
  return getRuleContexts<JavaScriptParser::ShiftExpressionContext>();
}

JavaScriptParser::ShiftExpressionContext* JavaScriptParser::RelationalExpressionContext::shiftExpression(size_t i) {
  return getRuleContext<JavaScriptParser::ShiftExpressionContext>(i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::RelationalExpressionContext::LT() {
  return getTokens(JavaScriptParser::LT);
}

tree::TerminalNode* JavaScriptParser::RelationalExpressionContext::LT(size_t i) {
  return getToken(JavaScriptParser::LT, i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::RelationalExpressionContext::GT() {
  return getTokens(JavaScriptParser::GT);
}

tree::TerminalNode* JavaScriptParser::RelationalExpressionContext::GT(size_t i) {
  return getToken(JavaScriptParser::GT, i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::RelationalExpressionContext::LE() {
  return getTokens(JavaScriptParser::LE);
}

tree::TerminalNode* JavaScriptParser::RelationalExpressionContext::LE(size_t i) {
  return getToken(JavaScriptParser::LE, i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::RelationalExpressionContext::GE() {
  return getTokens(JavaScriptParser::GE);
}

tree::TerminalNode* JavaScriptParser::RelationalExpressionContext::GE(size_t i) {
  return getToken(JavaScriptParser::GE, i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::RelationalExpressionContext::INSTANCEOF() {
  return getTokens(JavaScriptParser::INSTANCEOF);
}

tree::TerminalNode* JavaScriptParser::RelationalExpressionContext::INSTANCEOF(size_t i) {
  return getToken(JavaScriptParser::INSTANCEOF, i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::RelationalExpressionContext::IN() {
  return getTokens(JavaScriptParser::IN);
}

tree::TerminalNode* JavaScriptParser::RelationalExpressionContext::IN(size_t i) {
  return getToken(JavaScriptParser::IN, i);
}


size_t JavaScriptParser::RelationalExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RuleRelationalExpression;
}

void JavaScriptParser::RelationalExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRelationalExpression(this);
}

void JavaScriptParser::RelationalExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRelationalExpression(this);
}


std::any JavaScriptParser::RelationalExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitRelationalExpression(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::RelationalExpressionContext* JavaScriptParser::relationalExpression() {
  RelationalExpressionContext *_localctx = _tracker.createInstance<RelationalExpressionContext>(_ctx, getState());
  enterRule(_localctx, 66, JavaScriptParser::RuleRelationalExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(336);
    shiftExpression();
    setState(341);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 527765581357056) != 0)) {
      setState(337);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 527765581357056) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(338);
      shiftExpression();
      setState(343);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ShiftExpressionContext ------------------------------------------------------------------

JavaScriptParser::ShiftExpressionContext::ShiftExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<JavaScriptParser::AdditiveExpressionContext *> JavaScriptParser::ShiftExpressionContext::additiveExpression() {
  return getRuleContexts<JavaScriptParser::AdditiveExpressionContext>();
}

JavaScriptParser::AdditiveExpressionContext* JavaScriptParser::ShiftExpressionContext::additiveExpression(size_t i) {
  return getRuleContext<JavaScriptParser::AdditiveExpressionContext>(i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::ShiftExpressionContext::LEFT_SHIFT() {
  return getTokens(JavaScriptParser::LEFT_SHIFT);
}

tree::TerminalNode* JavaScriptParser::ShiftExpressionContext::LEFT_SHIFT(size_t i) {
  return getToken(JavaScriptParser::LEFT_SHIFT, i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::ShiftExpressionContext::RIGHT_SHIFT() {
  return getTokens(JavaScriptParser::RIGHT_SHIFT);
}

tree::TerminalNode* JavaScriptParser::ShiftExpressionContext::RIGHT_SHIFT(size_t i) {
  return getToken(JavaScriptParser::RIGHT_SHIFT, i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::ShiftExpressionContext::UNSIGNED_RIGHT_SHIFT() {
  return getTokens(JavaScriptParser::UNSIGNED_RIGHT_SHIFT);
}

tree::TerminalNode* JavaScriptParser::ShiftExpressionContext::UNSIGNED_RIGHT_SHIFT(size_t i) {
  return getToken(JavaScriptParser::UNSIGNED_RIGHT_SHIFT, i);
}


size_t JavaScriptParser::ShiftExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RuleShiftExpression;
}

void JavaScriptParser::ShiftExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterShiftExpression(this);
}

void JavaScriptParser::ShiftExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitShiftExpression(this);
}


std::any JavaScriptParser::ShiftExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitShiftExpression(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::ShiftExpressionContext* JavaScriptParser::shiftExpression() {
  ShiftExpressionContext *_localctx = _tracker.createInstance<ShiftExpressionContext>(_ctx, getState());
  enterRule(_localctx, 68, JavaScriptParser::RuleShiftExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(344);
    additiveExpression();
    setState(349);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3940649673949184) != 0)) {
      setState(345);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 3940649673949184) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(346);
      additiveExpression();
      setState(351);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AdditiveExpressionContext ------------------------------------------------------------------

JavaScriptParser::AdditiveExpressionContext::AdditiveExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<JavaScriptParser::MultiplicativeExpressionContext *> JavaScriptParser::AdditiveExpressionContext::multiplicativeExpression() {
  return getRuleContexts<JavaScriptParser::MultiplicativeExpressionContext>();
}

JavaScriptParser::MultiplicativeExpressionContext* JavaScriptParser::AdditiveExpressionContext::multiplicativeExpression(size_t i) {
  return getRuleContext<JavaScriptParser::MultiplicativeExpressionContext>(i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::AdditiveExpressionContext::PLUS() {
  return getTokens(JavaScriptParser::PLUS);
}

tree::TerminalNode* JavaScriptParser::AdditiveExpressionContext::PLUS(size_t i) {
  return getToken(JavaScriptParser::PLUS, i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::AdditiveExpressionContext::MINUS() {
  return getTokens(JavaScriptParser::MINUS);
}

tree::TerminalNode* JavaScriptParser::AdditiveExpressionContext::MINUS(size_t i) {
  return getToken(JavaScriptParser::MINUS, i);
}


size_t JavaScriptParser::AdditiveExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RuleAdditiveExpression;
}

void JavaScriptParser::AdditiveExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAdditiveExpression(this);
}

void JavaScriptParser::AdditiveExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAdditiveExpression(this);
}


std::any JavaScriptParser::AdditiveExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitAdditiveExpression(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::AdditiveExpressionContext* JavaScriptParser::additiveExpression() {
  AdditiveExpressionContext *_localctx = _tracker.createInstance<AdditiveExpressionContext>(_ctx, getState());
  enterRule(_localctx, 70, JavaScriptParser::RuleAdditiveExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(352);
    multiplicativeExpression();
    setState(357);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == JavaScriptParser::PLUS

    || _la == JavaScriptParser::MINUS) {
      setState(353);
      _la = _input->LA(1);
      if (!(_la == JavaScriptParser::PLUS

      || _la == JavaScriptParser::MINUS)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(354);
      multiplicativeExpression();
      setState(359);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MultiplicativeExpressionContext ------------------------------------------------------------------

JavaScriptParser::MultiplicativeExpressionContext::MultiplicativeExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<JavaScriptParser::UnaryExpressionContext *> JavaScriptParser::MultiplicativeExpressionContext::unaryExpression() {
  return getRuleContexts<JavaScriptParser::UnaryExpressionContext>();
}

JavaScriptParser::UnaryExpressionContext* JavaScriptParser::MultiplicativeExpressionContext::unaryExpression(size_t i) {
  return getRuleContext<JavaScriptParser::UnaryExpressionContext>(i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::MultiplicativeExpressionContext::MULTIPLY() {
  return getTokens(JavaScriptParser::MULTIPLY);
}

tree::TerminalNode* JavaScriptParser::MultiplicativeExpressionContext::MULTIPLY(size_t i) {
  return getToken(JavaScriptParser::MULTIPLY, i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::MultiplicativeExpressionContext::DIVIDE() {
  return getTokens(JavaScriptParser::DIVIDE);
}

tree::TerminalNode* JavaScriptParser::MultiplicativeExpressionContext::DIVIDE(size_t i) {
  return getToken(JavaScriptParser::DIVIDE, i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::MultiplicativeExpressionContext::MODULUS() {
  return getTokens(JavaScriptParser::MODULUS);
}

tree::TerminalNode* JavaScriptParser::MultiplicativeExpressionContext::MODULUS(size_t i) {
  return getToken(JavaScriptParser::MODULUS, i);
}


size_t JavaScriptParser::MultiplicativeExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RuleMultiplicativeExpression;
}

void JavaScriptParser::MultiplicativeExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMultiplicativeExpression(this);
}

void JavaScriptParser::MultiplicativeExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMultiplicativeExpression(this);
}


std::any JavaScriptParser::MultiplicativeExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitMultiplicativeExpression(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::MultiplicativeExpressionContext* JavaScriptParser::multiplicativeExpression() {
  MultiplicativeExpressionContext *_localctx = _tracker.createInstance<MultiplicativeExpressionContext>(_ctx, getState());
  enterRule(_localctx, 72, JavaScriptParser::RuleMultiplicativeExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(360);
    unaryExpression();
    setState(365);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 126100789566373888) != 0)) {
      setState(361);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 126100789566373888) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(362);
      unaryExpression();
      setState(367);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryExpressionContext ------------------------------------------------------------------

JavaScriptParser::UnaryExpressionContext::UnaryExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JavaScriptParser::PostfixExpressionContext* JavaScriptParser::UnaryExpressionContext::postfixExpression() {
  return getRuleContext<JavaScriptParser::PostfixExpressionContext>(0);
}

JavaScriptParser::UnaryExpressionContext* JavaScriptParser::UnaryExpressionContext::unaryExpression() {
  return getRuleContext<JavaScriptParser::UnaryExpressionContext>(0);
}

tree::TerminalNode* JavaScriptParser::UnaryExpressionContext::DELETE() {
  return getToken(JavaScriptParser::DELETE, 0);
}

tree::TerminalNode* JavaScriptParser::UnaryExpressionContext::VOID() {
  return getToken(JavaScriptParser::VOID, 0);
}

tree::TerminalNode* JavaScriptParser::UnaryExpressionContext::TYPEOF() {
  return getToken(JavaScriptParser::TYPEOF, 0);
}

tree::TerminalNode* JavaScriptParser::UnaryExpressionContext::INCREMENT() {
  return getToken(JavaScriptParser::INCREMENT, 0);
}

tree::TerminalNode* JavaScriptParser::UnaryExpressionContext::DECREMENT() {
  return getToken(JavaScriptParser::DECREMENT, 0);
}

tree::TerminalNode* JavaScriptParser::UnaryExpressionContext::PLUS() {
  return getToken(JavaScriptParser::PLUS, 0);
}

tree::TerminalNode* JavaScriptParser::UnaryExpressionContext::MINUS() {
  return getToken(JavaScriptParser::MINUS, 0);
}

tree::TerminalNode* JavaScriptParser::UnaryExpressionContext::BIT_NOT() {
  return getToken(JavaScriptParser::BIT_NOT, 0);
}

tree::TerminalNode* JavaScriptParser::UnaryExpressionContext::NOT() {
  return getToken(JavaScriptParser::NOT, 0);
}


size_t JavaScriptParser::UnaryExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RuleUnaryExpression;
}

void JavaScriptParser::UnaryExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnaryExpression(this);
}

void JavaScriptParser::UnaryExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnaryExpression(this);
}


std::any JavaScriptParser::UnaryExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitUnaryExpression(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::UnaryExpressionContext* JavaScriptParser::unaryExpression() {
  UnaryExpressionContext *_localctx = _tracker.createInstance<UnaryExpressionContext>(_ctx, getState());
  enterRule(_localctx, 74, JavaScriptParser::RuleUnaryExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(371);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case JavaScriptParser::FUNCTION:
      case JavaScriptParser::NEW:
      case JavaScriptParser::THIS:
      case JavaScriptParser::LBRACE:
      case JavaScriptParser::LPAREN:
      case JavaScriptParser::LBRACKET:
      case JavaScriptParser::NullLiteral:
      case JavaScriptParser::BooleanLiteral:
      case JavaScriptParser::NumericLiteral:
      case JavaScriptParser::StringLiteral:
      case JavaScriptParser::Identifier: {
        enterOuterAlt(_localctx, 1);
        setState(368);
        postfixExpression();
        break;
      }

      case JavaScriptParser::DELETE:
      case JavaScriptParser::TYPEOF:
      case JavaScriptParser::VOID:
      case JavaScriptParser::PLUS:
      case JavaScriptParser::MINUS:
      case JavaScriptParser::INCREMENT:
      case JavaScriptParser::DECREMENT:
      case JavaScriptParser::BIT_NOT:
      case JavaScriptParser::NOT: {
        enterOuterAlt(_localctx, 2);
        setState(369);
        _la = _input->LA(1);
        if (!((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 2175238620030435392) != 0))) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(370);
        unaryExpression();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PostfixExpressionContext ------------------------------------------------------------------

JavaScriptParser::PostfixExpressionContext::PostfixExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JavaScriptParser::LeftHandSideExpressionContext* JavaScriptParser::PostfixExpressionContext::leftHandSideExpression() {
  return getRuleContext<JavaScriptParser::LeftHandSideExpressionContext>(0);
}

tree::TerminalNode* JavaScriptParser::PostfixExpressionContext::INCREMENT() {
  return getToken(JavaScriptParser::INCREMENT, 0);
}

tree::TerminalNode* JavaScriptParser::PostfixExpressionContext::DECREMENT() {
  return getToken(JavaScriptParser::DECREMENT, 0);
}


size_t JavaScriptParser::PostfixExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RulePostfixExpression;
}

void JavaScriptParser::PostfixExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPostfixExpression(this);
}

void JavaScriptParser::PostfixExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPostfixExpression(this);
}


std::any JavaScriptParser::PostfixExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitPostfixExpression(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::PostfixExpressionContext* JavaScriptParser::postfixExpression() {
  PostfixExpressionContext *_localctx = _tracker.createInstance<PostfixExpressionContext>(_ctx, getState());
  enterRule(_localctx, 76, JavaScriptParser::RulePostfixExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(373);
    leftHandSideExpression();
    setState(375);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == JavaScriptParser::INCREMENT

    || _la == JavaScriptParser::DECREMENT) {
      setState(374);
      _la = _input->LA(1);
      if (!(_la == JavaScriptParser::INCREMENT

      || _la == JavaScriptParser::DECREMENT)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LeftHandSideExpressionContext ------------------------------------------------------------------

JavaScriptParser::LeftHandSideExpressionContext::LeftHandSideExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JavaScriptParser::CallExpressionContext* JavaScriptParser::LeftHandSideExpressionContext::callExpression() {
  return getRuleContext<JavaScriptParser::CallExpressionContext>(0);
}

JavaScriptParser::MemberExpressionContext* JavaScriptParser::LeftHandSideExpressionContext::memberExpression() {
  return getRuleContext<JavaScriptParser::MemberExpressionContext>(0);
}


size_t JavaScriptParser::LeftHandSideExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RuleLeftHandSideExpression;
}

void JavaScriptParser::LeftHandSideExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLeftHandSideExpression(this);
}

void JavaScriptParser::LeftHandSideExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLeftHandSideExpression(this);
}


std::any JavaScriptParser::LeftHandSideExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitLeftHandSideExpression(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::LeftHandSideExpressionContext* JavaScriptParser::leftHandSideExpression() {
  LeftHandSideExpressionContext *_localctx = _tracker.createInstance<LeftHandSideExpressionContext>(_ctx, getState());
  enterRule(_localctx, 78, JavaScriptParser::RuleLeftHandSideExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(379);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(377);
      callExpression(0);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(378);
      memberExpression(0);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CallExpressionContext ------------------------------------------------------------------

JavaScriptParser::CallExpressionContext::CallExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JavaScriptParser::MemberExpressionContext* JavaScriptParser::CallExpressionContext::memberExpression() {
  return getRuleContext<JavaScriptParser::MemberExpressionContext>(0);
}

JavaScriptParser::ArgumentsContext* JavaScriptParser::CallExpressionContext::arguments() {
  return getRuleContext<JavaScriptParser::ArgumentsContext>(0);
}

JavaScriptParser::CallExpressionContext* JavaScriptParser::CallExpressionContext::callExpression() {
  return getRuleContext<JavaScriptParser::CallExpressionContext>(0);
}

tree::TerminalNode* JavaScriptParser::CallExpressionContext::LBRACKET() {
  return getToken(JavaScriptParser::LBRACKET, 0);
}

JavaScriptParser::ExpressionContext* JavaScriptParser::CallExpressionContext::expression() {
  return getRuleContext<JavaScriptParser::ExpressionContext>(0);
}

tree::TerminalNode* JavaScriptParser::CallExpressionContext::RBRACKET() {
  return getToken(JavaScriptParser::RBRACKET, 0);
}

tree::TerminalNode* JavaScriptParser::CallExpressionContext::DOT() {
  return getToken(JavaScriptParser::DOT, 0);
}

tree::TerminalNode* JavaScriptParser::CallExpressionContext::Identifier() {
  return getToken(JavaScriptParser::Identifier, 0);
}


size_t JavaScriptParser::CallExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RuleCallExpression;
}

void JavaScriptParser::CallExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCallExpression(this);
}

void JavaScriptParser::CallExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCallExpression(this);
}


std::any JavaScriptParser::CallExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitCallExpression(this);
  else
    return visitor->visitChildren(this);
}


JavaScriptParser::CallExpressionContext* JavaScriptParser::callExpression() {
   return callExpression(0);
}

JavaScriptParser::CallExpressionContext* JavaScriptParser::callExpression(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  JavaScriptParser::CallExpressionContext *_localctx = _tracker.createInstance<CallExpressionContext>(_ctx, parentState);
  JavaScriptParser::CallExpressionContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 80;
  enterRecursionRule(_localctx, 80, JavaScriptParser::RuleCallExpression, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(382);
    memberExpression(0);
    setState(383);
    arguments();
    _ctx->stop = _input->LT(-1);
    setState(397);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(395);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx)) {
        case 1: {
          _localctx = _tracker.createInstance<CallExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleCallExpression);
          setState(385);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(386);
          arguments();
          break;
        }

        case 2: {
          _localctx = _tracker.createInstance<CallExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleCallExpression);
          setState(387);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(388);
          match(JavaScriptParser::LBRACKET);
          setState(389);
          expression();
          setState(390);
          match(JavaScriptParser::RBRACKET);
          break;
        }

        case 3: {
          _localctx = _tracker.createInstance<CallExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleCallExpression);
          setState(392);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(393);
          match(JavaScriptParser::DOT);
          setState(394);
          match(JavaScriptParser::Identifier);
          break;
        }

        default:
          break;
        } 
      }
      setState(399);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- MemberExpressionContext ------------------------------------------------------------------

JavaScriptParser::MemberExpressionContext::MemberExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JavaScriptParser::PrimaryExpressionContext* JavaScriptParser::MemberExpressionContext::primaryExpression() {
  return getRuleContext<JavaScriptParser::PrimaryExpressionContext>(0);
}

JavaScriptParser::FunctionExpressionContext* JavaScriptParser::MemberExpressionContext::functionExpression() {
  return getRuleContext<JavaScriptParser::FunctionExpressionContext>(0);
}

tree::TerminalNode* JavaScriptParser::MemberExpressionContext::NEW() {
  return getToken(JavaScriptParser::NEW, 0);
}

JavaScriptParser::MemberExpressionContext* JavaScriptParser::MemberExpressionContext::memberExpression() {
  return getRuleContext<JavaScriptParser::MemberExpressionContext>(0);
}

JavaScriptParser::ArgumentsContext* JavaScriptParser::MemberExpressionContext::arguments() {
  return getRuleContext<JavaScriptParser::ArgumentsContext>(0);
}

tree::TerminalNode* JavaScriptParser::MemberExpressionContext::LBRACKET() {
  return getToken(JavaScriptParser::LBRACKET, 0);
}

JavaScriptParser::ExpressionContext* JavaScriptParser::MemberExpressionContext::expression() {
  return getRuleContext<JavaScriptParser::ExpressionContext>(0);
}

tree::TerminalNode* JavaScriptParser::MemberExpressionContext::RBRACKET() {
  return getToken(JavaScriptParser::RBRACKET, 0);
}

tree::TerminalNode* JavaScriptParser::MemberExpressionContext::DOT() {
  return getToken(JavaScriptParser::DOT, 0);
}

tree::TerminalNode* JavaScriptParser::MemberExpressionContext::Identifier() {
  return getToken(JavaScriptParser::Identifier, 0);
}


size_t JavaScriptParser::MemberExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RuleMemberExpression;
}

void JavaScriptParser::MemberExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMemberExpression(this);
}

void JavaScriptParser::MemberExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMemberExpression(this);
}


std::any JavaScriptParser::MemberExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitMemberExpression(this);
  else
    return visitor->visitChildren(this);
}


JavaScriptParser::MemberExpressionContext* JavaScriptParser::memberExpression() {
   return memberExpression(0);
}

JavaScriptParser::MemberExpressionContext* JavaScriptParser::memberExpression(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  JavaScriptParser::MemberExpressionContext *_localctx = _tracker.createInstance<MemberExpressionContext>(_ctx, parentState);
  JavaScriptParser::MemberExpressionContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 82;
  enterRecursionRule(_localctx, 82, JavaScriptParser::RuleMemberExpression, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(407);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case JavaScriptParser::THIS:
      case JavaScriptParser::LBRACE:
      case JavaScriptParser::LPAREN:
      case JavaScriptParser::LBRACKET:
      case JavaScriptParser::NullLiteral:
      case JavaScriptParser::BooleanLiteral:
      case JavaScriptParser::NumericLiteral:
      case JavaScriptParser::StringLiteral:
      case JavaScriptParser::Identifier: {
        setState(401);
        primaryExpression();
        break;
      }

      case JavaScriptParser::FUNCTION: {
        setState(402);
        functionExpression();
        break;
      }

      case JavaScriptParser::NEW: {
        setState(403);
        match(JavaScriptParser::NEW);
        setState(404);
        memberExpression(0);
        setState(405);
        arguments();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(419);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 42, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(417);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx)) {
        case 1: {
          _localctx = _tracker.createInstance<MemberExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleMemberExpression);
          setState(409);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(410);
          match(JavaScriptParser::LBRACKET);
          setState(411);
          expression();
          setState(412);
          match(JavaScriptParser::RBRACKET);
          break;
        }

        case 2: {
          _localctx = _tracker.createInstance<MemberExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleMemberExpression);
          setState(414);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(415);
          match(JavaScriptParser::DOT);
          setState(416);
          match(JavaScriptParser::Identifier);
          break;
        }

        default:
          break;
        } 
      }
      setState(421);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 42, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- PrimaryExpressionContext ------------------------------------------------------------------

JavaScriptParser::PrimaryExpressionContext::PrimaryExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::PrimaryExpressionContext::THIS() {
  return getToken(JavaScriptParser::THIS, 0);
}

tree::TerminalNode* JavaScriptParser::PrimaryExpressionContext::Identifier() {
  return getToken(JavaScriptParser::Identifier, 0);
}

JavaScriptParser::LiteralContext* JavaScriptParser::PrimaryExpressionContext::literal() {
  return getRuleContext<JavaScriptParser::LiteralContext>(0);
}

JavaScriptParser::ArrayLiteralContext* JavaScriptParser::PrimaryExpressionContext::arrayLiteral() {
  return getRuleContext<JavaScriptParser::ArrayLiteralContext>(0);
}

JavaScriptParser::ObjectLiteralContext* JavaScriptParser::PrimaryExpressionContext::objectLiteral() {
  return getRuleContext<JavaScriptParser::ObjectLiteralContext>(0);
}

tree::TerminalNode* JavaScriptParser::PrimaryExpressionContext::LPAREN() {
  return getToken(JavaScriptParser::LPAREN, 0);
}

JavaScriptParser::ExpressionContext* JavaScriptParser::PrimaryExpressionContext::expression() {
  return getRuleContext<JavaScriptParser::ExpressionContext>(0);
}

tree::TerminalNode* JavaScriptParser::PrimaryExpressionContext::RPAREN() {
  return getToken(JavaScriptParser::RPAREN, 0);
}


size_t JavaScriptParser::PrimaryExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RulePrimaryExpression;
}

void JavaScriptParser::PrimaryExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPrimaryExpression(this);
}

void JavaScriptParser::PrimaryExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPrimaryExpression(this);
}


std::any JavaScriptParser::PrimaryExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitPrimaryExpression(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::PrimaryExpressionContext* JavaScriptParser::primaryExpression() {
  PrimaryExpressionContext *_localctx = _tracker.createInstance<PrimaryExpressionContext>(_ctx, getState());
  enterRule(_localctx, 84, JavaScriptParser::RulePrimaryExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(431);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case JavaScriptParser::THIS: {
        enterOuterAlt(_localctx, 1);
        setState(422);
        match(JavaScriptParser::THIS);
        break;
      }

      case JavaScriptParser::Identifier: {
        enterOuterAlt(_localctx, 2);
        setState(423);
        match(JavaScriptParser::Identifier);
        break;
      }

      case JavaScriptParser::NullLiteral:
      case JavaScriptParser::BooleanLiteral:
      case JavaScriptParser::NumericLiteral:
      case JavaScriptParser::StringLiteral: {
        enterOuterAlt(_localctx, 3);
        setState(424);
        literal();
        break;
      }

      case JavaScriptParser::LBRACKET: {
        enterOuterAlt(_localctx, 4);
        setState(425);
        arrayLiteral();
        break;
      }

      case JavaScriptParser::LBRACE: {
        enterOuterAlt(_localctx, 5);
        setState(426);
        objectLiteral();
        break;
      }

      case JavaScriptParser::LPAREN: {
        enterOuterAlt(_localctx, 6);
        setState(427);
        match(JavaScriptParser::LPAREN);
        setState(428);
        expression();
        setState(429);
        match(JavaScriptParser::RPAREN);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArrayLiteralContext ------------------------------------------------------------------

JavaScriptParser::ArrayLiteralContext::ArrayLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::ArrayLiteralContext::LBRACKET() {
  return getToken(JavaScriptParser::LBRACKET, 0);
}

tree::TerminalNode* JavaScriptParser::ArrayLiteralContext::RBRACKET() {
  return getToken(JavaScriptParser::RBRACKET, 0);
}

JavaScriptParser::ElementListContext* JavaScriptParser::ArrayLiteralContext::elementList() {
  return getRuleContext<JavaScriptParser::ElementListContext>(0);
}


size_t JavaScriptParser::ArrayLiteralContext::getRuleIndex() const {
  return JavaScriptParser::RuleArrayLiteral;
}

void JavaScriptParser::ArrayLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArrayLiteral(this);
}

void JavaScriptParser::ArrayLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArrayLiteral(this);
}


std::any JavaScriptParser::ArrayLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitArrayLiteral(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::ArrayLiteralContext* JavaScriptParser::arrayLiteral() {
  ArrayLiteralContext *_localctx = _tracker.createInstance<ArrayLiteralContext>(_ctx, getState());
  enterRule(_localctx, 86, JavaScriptParser::RuleArrayLiteral);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(433);
    match(JavaScriptParser::LBRACKET);
    setState(435);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -4742290407610349504) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 65)) & 993) != 0)) {
      setState(434);
      elementList();
    }
    setState(437);
    match(JavaScriptParser::RBRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ElementListContext ------------------------------------------------------------------

JavaScriptParser::ElementListContext::ElementListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<JavaScriptParser::AssignmentExpressionContext *> JavaScriptParser::ElementListContext::assignmentExpression() {
  return getRuleContexts<JavaScriptParser::AssignmentExpressionContext>();
}

JavaScriptParser::AssignmentExpressionContext* JavaScriptParser::ElementListContext::assignmentExpression(size_t i) {
  return getRuleContext<JavaScriptParser::AssignmentExpressionContext>(i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::ElementListContext::COMMA() {
  return getTokens(JavaScriptParser::COMMA);
}

tree::TerminalNode* JavaScriptParser::ElementListContext::COMMA(size_t i) {
  return getToken(JavaScriptParser::COMMA, i);
}


size_t JavaScriptParser::ElementListContext::getRuleIndex() const {
  return JavaScriptParser::RuleElementList;
}

void JavaScriptParser::ElementListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterElementList(this);
}

void JavaScriptParser::ElementListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitElementList(this);
}


std::any JavaScriptParser::ElementListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitElementList(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::ElementListContext* JavaScriptParser::elementList() {
  ElementListContext *_localctx = _tracker.createInstance<ElementListContext>(_ctx, getState());
  enterRule(_localctx, 88, JavaScriptParser::RuleElementList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(439);
    assignmentExpression();
    setState(444);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == JavaScriptParser::COMMA) {
      setState(440);
      match(JavaScriptParser::COMMA);
      setState(441);
      assignmentExpression();
      setState(446);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectLiteralContext ------------------------------------------------------------------

JavaScriptParser::ObjectLiteralContext::ObjectLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::ObjectLiteralContext::LBRACE() {
  return getToken(JavaScriptParser::LBRACE, 0);
}

tree::TerminalNode* JavaScriptParser::ObjectLiteralContext::RBRACE() {
  return getToken(JavaScriptParser::RBRACE, 0);
}

JavaScriptParser::PropertyNameAndValueListContext* JavaScriptParser::ObjectLiteralContext::propertyNameAndValueList() {
  return getRuleContext<JavaScriptParser::PropertyNameAndValueListContext>(0);
}


size_t JavaScriptParser::ObjectLiteralContext::getRuleIndex() const {
  return JavaScriptParser::RuleObjectLiteral;
}

void JavaScriptParser::ObjectLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterObjectLiteral(this);
}

void JavaScriptParser::ObjectLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitObjectLiteral(this);
}


std::any JavaScriptParser::ObjectLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitObjectLiteral(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::ObjectLiteralContext* JavaScriptParser::objectLiteral() {
  ObjectLiteralContext *_localctx = _tracker.createInstance<ObjectLiteralContext>(_ctx, getState());
  enterRule(_localctx, 90, JavaScriptParser::RuleObjectLiteral);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(447);
    match(JavaScriptParser::LBRACE);
    setState(449);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 72) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 72)) & 7) != 0)) {
      setState(448);
      propertyNameAndValueList();
    }
    setState(451);
    match(JavaScriptParser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertyNameAndValueListContext ------------------------------------------------------------------

JavaScriptParser::PropertyNameAndValueListContext::PropertyNameAndValueListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<JavaScriptParser::PropertyAssignmentContext *> JavaScriptParser::PropertyNameAndValueListContext::propertyAssignment() {
  return getRuleContexts<JavaScriptParser::PropertyAssignmentContext>();
}

JavaScriptParser::PropertyAssignmentContext* JavaScriptParser::PropertyNameAndValueListContext::propertyAssignment(size_t i) {
  return getRuleContext<JavaScriptParser::PropertyAssignmentContext>(i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::PropertyNameAndValueListContext::COMMA() {
  return getTokens(JavaScriptParser::COMMA);
}

tree::TerminalNode* JavaScriptParser::PropertyNameAndValueListContext::COMMA(size_t i) {
  return getToken(JavaScriptParser::COMMA, i);
}


size_t JavaScriptParser::PropertyNameAndValueListContext::getRuleIndex() const {
  return JavaScriptParser::RulePropertyNameAndValueList;
}

void JavaScriptParser::PropertyNameAndValueListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPropertyNameAndValueList(this);
}

void JavaScriptParser::PropertyNameAndValueListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPropertyNameAndValueList(this);
}


std::any JavaScriptParser::PropertyNameAndValueListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitPropertyNameAndValueList(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::PropertyNameAndValueListContext* JavaScriptParser::propertyNameAndValueList() {
  PropertyNameAndValueListContext *_localctx = _tracker.createInstance<PropertyNameAndValueListContext>(_ctx, getState());
  enterRule(_localctx, 92, JavaScriptParser::RulePropertyNameAndValueList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(453);
    propertyAssignment();
    setState(458);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == JavaScriptParser::COMMA) {
      setState(454);
      match(JavaScriptParser::COMMA);
      setState(455);
      propertyAssignment();
      setState(460);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertyAssignmentContext ------------------------------------------------------------------

JavaScriptParser::PropertyAssignmentContext::PropertyAssignmentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JavaScriptParser::PropertyNameContext* JavaScriptParser::PropertyAssignmentContext::propertyName() {
  return getRuleContext<JavaScriptParser::PropertyNameContext>(0);
}

tree::TerminalNode* JavaScriptParser::PropertyAssignmentContext::COLON() {
  return getToken(JavaScriptParser::COLON, 0);
}

JavaScriptParser::AssignmentExpressionContext* JavaScriptParser::PropertyAssignmentContext::assignmentExpression() {
  return getRuleContext<JavaScriptParser::AssignmentExpressionContext>(0);
}


size_t JavaScriptParser::PropertyAssignmentContext::getRuleIndex() const {
  return JavaScriptParser::RulePropertyAssignment;
}

void JavaScriptParser::PropertyAssignmentContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPropertyAssignment(this);
}

void JavaScriptParser::PropertyAssignmentContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPropertyAssignment(this);
}


std::any JavaScriptParser::PropertyAssignmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitPropertyAssignment(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::PropertyAssignmentContext* JavaScriptParser::propertyAssignment() {
  PropertyAssignmentContext *_localctx = _tracker.createInstance<PropertyAssignmentContext>(_ctx, getState());
  enterRule(_localctx, 94, JavaScriptParser::RulePropertyAssignment);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(461);
    propertyName();
    setState(462);
    match(JavaScriptParser::COLON);
    setState(463);
    assignmentExpression();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertyNameContext ------------------------------------------------------------------

JavaScriptParser::PropertyNameContext::PropertyNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::PropertyNameContext::Identifier() {
  return getToken(JavaScriptParser::Identifier, 0);
}

tree::TerminalNode* JavaScriptParser::PropertyNameContext::StringLiteral() {
  return getToken(JavaScriptParser::StringLiteral, 0);
}

tree::TerminalNode* JavaScriptParser::PropertyNameContext::NumericLiteral() {
  return getToken(JavaScriptParser::NumericLiteral, 0);
}


size_t JavaScriptParser::PropertyNameContext::getRuleIndex() const {
  return JavaScriptParser::RulePropertyName;
}

void JavaScriptParser::PropertyNameContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPropertyName(this);
}

void JavaScriptParser::PropertyNameContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPropertyName(this);
}


std::any JavaScriptParser::PropertyNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitPropertyName(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::PropertyNameContext* JavaScriptParser::propertyName() {
  PropertyNameContext *_localctx = _tracker.createInstance<PropertyNameContext>(_ctx, getState());
  enterRule(_localctx, 96, JavaScriptParser::RulePropertyName);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(465);
    _la = _input->LA(1);
    if (!(((((_la - 72) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 72)) & 7) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgumentsContext ------------------------------------------------------------------

JavaScriptParser::ArgumentsContext::ArgumentsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::ArgumentsContext::LPAREN() {
  return getToken(JavaScriptParser::LPAREN, 0);
}

tree::TerminalNode* JavaScriptParser::ArgumentsContext::RPAREN() {
  return getToken(JavaScriptParser::RPAREN, 0);
}

JavaScriptParser::ArgumentListContext* JavaScriptParser::ArgumentsContext::argumentList() {
  return getRuleContext<JavaScriptParser::ArgumentListContext>(0);
}


size_t JavaScriptParser::ArgumentsContext::getRuleIndex() const {
  return JavaScriptParser::RuleArguments;
}

void JavaScriptParser::ArgumentsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArguments(this);
}

void JavaScriptParser::ArgumentsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArguments(this);
}


std::any JavaScriptParser::ArgumentsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitArguments(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::ArgumentsContext* JavaScriptParser::arguments() {
  ArgumentsContext *_localctx = _tracker.createInstance<ArgumentsContext>(_ctx, getState());
  enterRule(_localctx, 98, JavaScriptParser::RuleArguments);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(467);
    match(JavaScriptParser::LPAREN);
    setState(469);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -4742290407610349504) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 65)) & 993) != 0)) {
      setState(468);
      argumentList();
    }
    setState(471);
    match(JavaScriptParser::RPAREN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgumentListContext ------------------------------------------------------------------

JavaScriptParser::ArgumentListContext::ArgumentListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<JavaScriptParser::AssignmentExpressionContext *> JavaScriptParser::ArgumentListContext::assignmentExpression() {
  return getRuleContexts<JavaScriptParser::AssignmentExpressionContext>();
}

JavaScriptParser::AssignmentExpressionContext* JavaScriptParser::ArgumentListContext::assignmentExpression(size_t i) {
  return getRuleContext<JavaScriptParser::AssignmentExpressionContext>(i);
}

std::vector<tree::TerminalNode *> JavaScriptParser::ArgumentListContext::COMMA() {
  return getTokens(JavaScriptParser::COMMA);
}

tree::TerminalNode* JavaScriptParser::ArgumentListContext::COMMA(size_t i) {
  return getToken(JavaScriptParser::COMMA, i);
}


size_t JavaScriptParser::ArgumentListContext::getRuleIndex() const {
  return JavaScriptParser::RuleArgumentList;
}

void JavaScriptParser::ArgumentListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArgumentList(this);
}

void JavaScriptParser::ArgumentListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArgumentList(this);
}


std::any JavaScriptParser::ArgumentListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitArgumentList(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::ArgumentListContext* JavaScriptParser::argumentList() {
  ArgumentListContext *_localctx = _tracker.createInstance<ArgumentListContext>(_ctx, getState());
  enterRule(_localctx, 100, JavaScriptParser::RuleArgumentList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(473);
    assignmentExpression();
    setState(478);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == JavaScriptParser::COMMA) {
      setState(474);
      match(JavaScriptParser::COMMA);
      setState(475);
      assignmentExpression();
      setState(480);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionExpressionContext ------------------------------------------------------------------

JavaScriptParser::FunctionExpressionContext::FunctionExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::FunctionExpressionContext::FUNCTION() {
  return getToken(JavaScriptParser::FUNCTION, 0);
}

tree::TerminalNode* JavaScriptParser::FunctionExpressionContext::LPAREN() {
  return getToken(JavaScriptParser::LPAREN, 0);
}

tree::TerminalNode* JavaScriptParser::FunctionExpressionContext::RPAREN() {
  return getToken(JavaScriptParser::RPAREN, 0);
}

tree::TerminalNode* JavaScriptParser::FunctionExpressionContext::LBRACE() {
  return getToken(JavaScriptParser::LBRACE, 0);
}

JavaScriptParser::FunctionBodyContext* JavaScriptParser::FunctionExpressionContext::functionBody() {
  return getRuleContext<JavaScriptParser::FunctionBodyContext>(0);
}

tree::TerminalNode* JavaScriptParser::FunctionExpressionContext::RBRACE() {
  return getToken(JavaScriptParser::RBRACE, 0);
}

tree::TerminalNode* JavaScriptParser::FunctionExpressionContext::Identifier() {
  return getToken(JavaScriptParser::Identifier, 0);
}

JavaScriptParser::FormalParameterListContext* JavaScriptParser::FunctionExpressionContext::formalParameterList() {
  return getRuleContext<JavaScriptParser::FormalParameterListContext>(0);
}


size_t JavaScriptParser::FunctionExpressionContext::getRuleIndex() const {
  return JavaScriptParser::RuleFunctionExpression;
}

void JavaScriptParser::FunctionExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionExpression(this);
}

void JavaScriptParser::FunctionExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionExpression(this);
}


std::any JavaScriptParser::FunctionExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitFunctionExpression(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::FunctionExpressionContext* JavaScriptParser::functionExpression() {
  FunctionExpressionContext *_localctx = _tracker.createInstance<FunctionExpressionContext>(_ctx, getState());
  enterRule(_localctx, 102, JavaScriptParser::RuleFunctionExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(481);
    match(JavaScriptParser::FUNCTION);
    setState(483);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == JavaScriptParser::Identifier) {
      setState(482);
      match(JavaScriptParser::Identifier);
    }
    setState(485);
    match(JavaScriptParser::LPAREN);
    setState(487);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == JavaScriptParser::Identifier) {
      setState(486);
      formalParameterList();
    }
    setState(489);
    match(JavaScriptParser::RPAREN);
    setState(490);
    match(JavaScriptParser::LBRACE);
    setState(491);
    functionBody();
    setState(492);
    match(JavaScriptParser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LiteralContext ------------------------------------------------------------------

JavaScriptParser::LiteralContext::LiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JavaScriptParser::LiteralContext::NullLiteral() {
  return getToken(JavaScriptParser::NullLiteral, 0);
}

tree::TerminalNode* JavaScriptParser::LiteralContext::BooleanLiteral() {
  return getToken(JavaScriptParser::BooleanLiteral, 0);
}

tree::TerminalNode* JavaScriptParser::LiteralContext::NumericLiteral() {
  return getToken(JavaScriptParser::NumericLiteral, 0);
}

tree::TerminalNode* JavaScriptParser::LiteralContext::StringLiteral() {
  return getToken(JavaScriptParser::StringLiteral, 0);
}


size_t JavaScriptParser::LiteralContext::getRuleIndex() const {
  return JavaScriptParser::RuleLiteral;
}

void JavaScriptParser::LiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLiteral(this);
}

void JavaScriptParser::LiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JavaScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLiteral(this);
}


std::any JavaScriptParser::LiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<JavaScriptVisitor*>(visitor))
    return parserVisitor->visitLiteral(this);
  else
    return visitor->visitChildren(this);
}

JavaScriptParser::LiteralContext* JavaScriptParser::literal() {
  LiteralContext *_localctx = _tracker.createInstance<LiteralContext>(_ctx, getState());
  enterRule(_localctx, 104, JavaScriptParser::RuleLiteral);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(494);
    _la = _input->LA(1);
    if (!(((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & 15) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool JavaScriptParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 40: return callExpressionSempred(antlrcpp::downCast<CallExpressionContext *>(context), predicateIndex);
    case 41: return memberExpressionSempred(antlrcpp::downCast<MemberExpressionContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool JavaScriptParser::callExpressionSempred(CallExpressionContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 3);
    case 1: return precpred(_ctx, 2);
    case 2: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool JavaScriptParser::memberExpressionSempred(MemberExpressionContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 3: return precpred(_ctx, 3);
    case 4: return precpred(_ctx, 2);

  default:
    break;
  }
  return true;
}

void JavaScriptParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  javascriptParserInitialize();
#else
  ::antlr4::internal::call_once(javascriptParserOnceFlag, javascriptParserInitialize);
#endif
}

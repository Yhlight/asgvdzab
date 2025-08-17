
// Generated from grammar/ECMAScript.g4 by ANTLR 4.13.1


#include "ECMAScriptListener.h"

#include "ECMAScriptParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct ECMAScriptParserStaticData final {
  ECMAScriptParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  ECMAScriptParserStaticData(const ECMAScriptParserStaticData&) = delete;
  ECMAScriptParserStaticData(ECMAScriptParserStaticData&&) = delete;
  ECMAScriptParserStaticData& operator=(const ECMAScriptParserStaticData&) = delete;
  ECMAScriptParserStaticData& operator=(ECMAScriptParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag ecmascriptParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
ECMAScriptParserStaticData *ecmascriptParserStaticData = nullptr;

void ecmascriptParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (ecmascriptParserStaticData != nullptr) {
    return;
  }
#else
  assert(ecmascriptParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<ECMAScriptParserStaticData>(
    std::vector<std::string>{
      "program", "sourceElements", "sourceElement", "chtlJsElement", "chtlEnhancedSelector", 
      "selectorExpression", "chtlArrowAccess", "chtlListenFunction", "chtlDelegateFunction", 
      "chtlAnimateFunction", "chtlVirDeclaration", "chtlVirAccess", "chtlVariableUsage", 
      "chtlOriginEmbed", "statement", "block", "statementList", "variableStatement", 
      "varModifier", "variableDeclarationList", "variableDeclaration", "emptyStatement", 
      "expressionStatement", "ifStatement", "iterationStatement", "forControl", 
      "continueStatement", "breakStatement", "returnStatement", "withStatement", 
      "switchStatement", "caseBlock", "caseClauses", "caseClause", "throwStatement", 
      "tryStatement", "catchProduction", "finallyProduction", "debuggerStatement", 
      "labelledStatement", "functionDeclaration", "formalParameterList", 
      "functionBody", "expressionSequence", "singleExpression", "assignmentOperator", 
      "primaryExpression", "functionExpression", "arrowFunctionExpression", 
      "arrowFunctionParameters", "arrowFunctionBody", "arguments", "argumentList", 
      "literal", "nullLiteral", "booleanLiteral", "stringLiteral", "templateLiteral", 
      "templateStringAtom", "regularExpressionLiteral", "numericLiteral", 
      "arrayLiteral", "elementList", "arrayElement", "objectLiteral", "propertyAssignment", 
      "propertyName", "getter", "setter", "identifier", "eos"
    },
    std::vector<std::string>{
      "", "'{{'", "'}}'", "'->'", "'listen'", "'('", "')'", "'delegate'", 
      "'animate'", "'vir'", "'='", "'.'", "'[Origin]'", "'@JavaScript'", 
      "';'", "'{'", "'}'", "'var'", "'let'", "'const'", "','", "'if'", "'else'", 
      "'do'", "'while'", "'for'", "'in'", "'of'", "'continue'", "'break'", 
      "'return'", "'with'", "'switch'", "'case'", "':'", "'default'", "'throw'", 
      "'try'", "'catch'", "'finally'", "'debugger'", "'function'", "'['", 
      "']'", "'new'", "'++'", "'--'", "'delete'", "'void'", "'typeof'", 
      "'+'", "'-'", "'~'", "'!'", "'*'", "'/'", "'%'", "'<<'", "'>>'", "'>>>'", 
      "'<'", "'>'", "'<='", "'>='", "'instanceof'", "'=='", "'!='", "'==='", 
      "'!=='", "'&'", "'^'", "'|'", "'&&'", "'||'", "'\\u003F'", "'*='", 
      "'/='", "'%='", "'+='", "'-='", "'<<='", "'>>='", "'>>>='", "'&='", 
      "'^='", "'|='", "'this'", "'=>'", "'null'", "'true'", "'false'", "'`'", 
      "'${'", "'get'", "'set'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "StringLiteral", "TemplateStringAtom", 
      "RegularExpressionLiteral", "DecimalLiteral", "HexIntegerLiteral", 
      "OctalIntegerLiteral", "BinaryIntegerLiteral", "Identifier", "WhiteSpaces", 
      "LineTerminatorHidden", "MultiLineComment", "SingleLineComment", "GeneratorComment", 
      "SELECTOR_CONTENT"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,108,741,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,7,
  	49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,7,
  	56,2,57,7,57,2,58,7,58,2,59,7,59,2,60,7,60,2,61,7,61,2,62,7,62,2,63,7,
  	63,2,64,7,64,2,65,7,65,2,66,7,66,2,67,7,67,2,68,7,68,2,69,7,69,2,70,7,
  	70,1,0,3,0,144,8,0,1,0,1,0,1,1,4,1,149,8,1,11,1,12,1,150,1,2,1,2,1,2,
  	3,2,156,8,2,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,3,3,167,8,3,1,4,1,4,1,
  	4,1,4,1,5,1,5,1,6,1,6,1,6,1,6,1,7,1,7,1,7,1,7,1,7,1,8,1,8,1,8,1,8,1,8,
  	1,9,1,9,1,9,1,9,1,9,1,10,1,10,1,10,1,10,1,10,1,11,1,11,1,11,1,11,1,12,
  	1,12,1,12,1,12,1,12,1,13,1,13,1,13,1,13,1,13,1,14,1,14,1,14,1,14,1,14,
  	1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,14,3,14,228,8,14,1,15,
  	1,15,3,15,232,8,15,1,15,1,15,1,16,4,16,237,8,16,11,16,12,16,238,1,17,
  	1,17,1,17,1,17,1,18,1,18,1,19,1,19,1,19,5,19,250,8,19,10,19,12,19,253,
  	9,19,1,20,1,20,1,20,3,20,258,8,20,1,21,1,21,1,22,1,22,1,22,1,23,1,23,
  	1,23,1,23,1,23,1,23,1,23,3,23,272,8,23,1,24,1,24,1,24,1,24,1,24,1,24,
  	1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,
  	3,24,294,8,24,1,25,3,25,297,8,25,1,25,1,25,3,25,301,8,25,1,25,1,25,3,
  	25,305,8,25,1,25,1,25,1,25,1,25,3,25,311,8,25,1,25,1,25,3,25,315,8,25,
  	1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,
  	1,25,1,25,1,25,1,25,3,25,335,8,25,1,26,1,26,3,26,339,8,26,1,26,1,26,1,
  	27,1,27,3,27,345,8,27,1,27,1,27,1,28,1,28,3,28,351,8,28,1,28,1,28,1,29,
  	1,29,1,29,1,29,1,29,1,29,1,30,1,30,1,30,1,30,1,30,1,30,1,31,1,31,3,31,
  	369,8,31,1,31,1,31,1,32,4,32,374,8,32,11,32,12,32,375,1,33,1,33,1,33,
  	1,33,3,33,382,8,33,1,33,1,33,1,33,3,33,387,8,33,3,33,389,8,33,1,34,1,
  	34,1,34,1,34,1,35,1,35,1,35,1,35,3,35,399,8,35,1,35,3,35,402,8,35,1,36,
  	1,36,1,36,1,36,1,36,1,36,1,37,1,37,1,37,1,38,1,38,1,38,1,39,1,39,1,39,
  	1,39,1,40,1,40,1,40,1,40,3,40,424,8,40,1,40,1,40,1,40,1,40,1,40,1,41,
  	1,41,1,41,5,41,434,8,41,10,41,12,41,437,9,41,1,42,3,42,440,8,42,1,43,
  	1,43,1,43,5,43,445,8,43,10,43,12,43,448,9,43,1,44,1,44,1,44,1,44,1,44,
  	1,44,3,44,456,8,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,
  	1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,3,44,478,8,44,1,44,
  	1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,
  	1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,
  	1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,
  	1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,
  	1,44,1,44,1,44,1,44,1,44,1,44,5,44,543,8,44,10,44,12,44,546,9,44,1,45,
  	1,45,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,1,46,
  	1,46,1,46,3,46,565,8,46,1,47,1,47,3,47,569,8,47,1,47,1,47,3,47,573,8,
  	47,1,47,1,47,1,47,1,47,1,47,1,48,1,48,1,48,1,48,1,49,1,49,1,49,3,49,587,
  	8,49,1,49,3,49,590,8,49,1,50,1,50,1,50,1,50,1,50,3,50,597,8,50,1,51,1,
  	51,3,51,601,8,51,1,51,1,51,1,52,1,52,1,52,5,52,608,8,52,10,52,12,52,611,
  	9,52,1,53,1,53,1,53,1,53,1,53,1,53,3,53,619,8,53,1,54,1,54,1,55,1,55,
  	1,56,1,56,1,57,1,57,5,57,629,8,57,10,57,12,57,632,9,57,1,57,1,57,1,58,
  	1,58,1,58,1,58,1,58,3,58,641,8,58,1,59,1,59,1,60,1,60,1,61,1,61,3,61,
  	649,8,61,1,61,1,61,1,62,5,62,654,8,62,10,62,12,62,657,9,62,1,62,3,62,
  	660,8,62,1,62,4,62,663,8,62,11,62,12,62,664,1,62,5,62,668,8,62,10,62,
  	12,62,671,9,62,1,62,5,62,674,8,62,10,62,12,62,677,9,62,1,63,1,63,1,64,
  	1,64,3,64,683,8,64,1,64,1,64,5,64,687,8,64,10,64,12,64,690,9,64,1,64,
  	3,64,693,8,64,1,64,1,64,1,65,1,65,1,65,1,65,1,65,1,65,1,65,1,65,1,65,
  	1,65,1,65,1,65,1,65,3,65,710,8,65,1,66,1,66,1,66,3,66,715,8,66,1,67,1,
  	67,1,67,1,67,1,67,1,67,1,67,1,67,1,68,1,68,1,68,1,68,1,68,1,68,1,68,1,
  	68,1,68,1,69,1,69,1,70,1,70,1,70,3,70,739,8,70,1,70,0,1,88,71,0,2,4,6,
  	8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,
  	56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,
  	102,104,106,108,110,112,114,116,118,120,122,124,126,128,130,132,134,136,
  	138,140,0,10,1,0,17,19,1,0,54,56,1,0,50,51,1,0,57,59,1,0,60,63,1,0,65,
  	68,1,0,75,85,1,0,89,90,1,0,98,101,10,0,17,19,21,33,35,41,44,44,47,49,
  	64,64,86,86,88,90,93,94,102,102,802,0,143,1,0,0,0,2,148,1,0,0,0,4,155,
  	1,0,0,0,6,166,1,0,0,0,8,168,1,0,0,0,10,172,1,0,0,0,12,174,1,0,0,0,14,
  	178,1,0,0,0,16,183,1,0,0,0,18,188,1,0,0,0,20,193,1,0,0,0,22,198,1,0,0,
  	0,24,202,1,0,0,0,26,207,1,0,0,0,28,227,1,0,0,0,30,229,1,0,0,0,32,236,
  	1,0,0,0,34,240,1,0,0,0,36,244,1,0,0,0,38,246,1,0,0,0,40,254,1,0,0,0,42,
  	259,1,0,0,0,44,261,1,0,0,0,46,264,1,0,0,0,48,293,1,0,0,0,50,334,1,0,0,
  	0,52,336,1,0,0,0,54,342,1,0,0,0,56,348,1,0,0,0,58,354,1,0,0,0,60,360,
  	1,0,0,0,62,366,1,0,0,0,64,373,1,0,0,0,66,388,1,0,0,0,68,390,1,0,0,0,70,
  	394,1,0,0,0,72,403,1,0,0,0,74,409,1,0,0,0,76,412,1,0,0,0,78,415,1,0,0,
  	0,80,419,1,0,0,0,82,430,1,0,0,0,84,439,1,0,0,0,86,441,1,0,0,0,88,477,
  	1,0,0,0,90,547,1,0,0,0,92,564,1,0,0,0,94,566,1,0,0,0,96,579,1,0,0,0,98,
  	589,1,0,0,0,100,596,1,0,0,0,102,598,1,0,0,0,104,604,1,0,0,0,106,618,1,
  	0,0,0,108,620,1,0,0,0,110,622,1,0,0,0,112,624,1,0,0,0,114,626,1,0,0,0,
  	116,640,1,0,0,0,118,642,1,0,0,0,120,644,1,0,0,0,122,646,1,0,0,0,124,655,
  	1,0,0,0,126,678,1,0,0,0,128,680,1,0,0,0,130,709,1,0,0,0,132,714,1,0,0,
  	0,134,716,1,0,0,0,136,724,1,0,0,0,138,733,1,0,0,0,140,738,1,0,0,0,142,
  	144,3,2,1,0,143,142,1,0,0,0,143,144,1,0,0,0,144,145,1,0,0,0,145,146,5,
  	0,0,1,146,1,1,0,0,0,147,149,3,4,2,0,148,147,1,0,0,0,149,150,1,0,0,0,150,
  	148,1,0,0,0,150,151,1,0,0,0,151,3,1,0,0,0,152,156,3,28,14,0,153,156,3,
  	80,40,0,154,156,3,6,3,0,155,152,1,0,0,0,155,153,1,0,0,0,155,154,1,0,0,
  	0,156,5,1,0,0,0,157,167,3,8,4,0,158,167,3,12,6,0,159,167,3,14,7,0,160,
  	167,3,16,8,0,161,167,3,18,9,0,162,167,3,20,10,0,163,167,3,22,11,0,164,
  	167,3,24,12,0,165,167,3,26,13,0,166,157,1,0,0,0,166,158,1,0,0,0,166,159,
  	1,0,0,0,166,160,1,0,0,0,166,161,1,0,0,0,166,162,1,0,0,0,166,163,1,0,0,
  	0,166,164,1,0,0,0,166,165,1,0,0,0,167,7,1,0,0,0,168,169,5,1,0,0,169,170,
  	3,10,5,0,170,171,5,2,0,0,171,9,1,0,0,0,172,173,5,108,0,0,173,11,1,0,0,
  	0,174,175,3,92,46,0,175,176,5,3,0,0,176,177,3,138,69,0,177,13,1,0,0,0,
  	178,179,5,4,0,0,179,180,5,5,0,0,180,181,3,102,51,0,181,182,5,6,0,0,182,
  	15,1,0,0,0,183,184,5,7,0,0,184,185,5,5,0,0,185,186,3,102,51,0,186,187,
  	5,6,0,0,187,17,1,0,0,0,188,189,5,8,0,0,189,190,5,5,0,0,190,191,3,102,
  	51,0,191,192,5,6,0,0,192,19,1,0,0,0,193,194,5,9,0,0,194,195,3,138,69,
  	0,195,196,5,10,0,0,196,197,3,128,64,0,197,21,1,0,0,0,198,199,3,138,69,
  	0,199,200,5,11,0,0,200,201,3,138,69,0,201,23,1,0,0,0,202,203,3,138,69,
  	0,203,204,5,5,0,0,204,205,3,138,69,0,205,206,5,6,0,0,206,25,1,0,0,0,207,
  	208,5,12,0,0,208,209,5,13,0,0,209,210,3,138,69,0,210,211,5,14,0,0,211,
  	27,1,0,0,0,212,228,3,30,15,0,213,228,3,34,17,0,214,228,3,42,21,0,215,
  	228,3,44,22,0,216,228,3,46,23,0,217,228,3,48,24,0,218,228,3,52,26,0,219,
  	228,3,54,27,0,220,228,3,56,28,0,221,228,3,58,29,0,222,228,3,60,30,0,223,
  	228,3,68,34,0,224,228,3,70,35,0,225,228,3,76,38,0,226,228,3,78,39,0,227,
  	212,1,0,0,0,227,213,1,0,0,0,227,214,1,0,0,0,227,215,1,0,0,0,227,216,1,
  	0,0,0,227,217,1,0,0,0,227,218,1,0,0,0,227,219,1,0,0,0,227,220,1,0,0,0,
  	227,221,1,0,0,0,227,222,1,0,0,0,227,223,1,0,0,0,227,224,1,0,0,0,227,225,
  	1,0,0,0,227,226,1,0,0,0,228,29,1,0,0,0,229,231,5,15,0,0,230,232,3,32,
  	16,0,231,230,1,0,0,0,231,232,1,0,0,0,232,233,1,0,0,0,233,234,5,16,0,0,
  	234,31,1,0,0,0,235,237,3,28,14,0,236,235,1,0,0,0,237,238,1,0,0,0,238,
  	236,1,0,0,0,238,239,1,0,0,0,239,33,1,0,0,0,240,241,3,36,18,0,241,242,
  	3,38,19,0,242,243,3,140,70,0,243,35,1,0,0,0,244,245,7,0,0,0,245,37,1,
  	0,0,0,246,251,3,40,20,0,247,248,5,20,0,0,248,250,3,40,20,0,249,247,1,
  	0,0,0,250,253,1,0,0,0,251,249,1,0,0,0,251,252,1,0,0,0,252,39,1,0,0,0,
  	253,251,1,0,0,0,254,257,3,138,69,0,255,256,5,10,0,0,256,258,3,88,44,0,
  	257,255,1,0,0,0,257,258,1,0,0,0,258,41,1,0,0,0,259,260,5,14,0,0,260,43,
  	1,0,0,0,261,262,3,86,43,0,262,263,3,140,70,0,263,45,1,0,0,0,264,265,5,
  	21,0,0,265,266,5,5,0,0,266,267,3,86,43,0,267,268,5,6,0,0,268,271,3,28,
  	14,0,269,270,5,22,0,0,270,272,3,28,14,0,271,269,1,0,0,0,271,272,1,0,0,
  	0,272,47,1,0,0,0,273,274,5,23,0,0,274,275,3,28,14,0,275,276,5,24,0,0,
  	276,277,5,5,0,0,277,278,3,86,43,0,278,279,5,6,0,0,279,280,3,140,70,0,
  	280,294,1,0,0,0,281,282,5,24,0,0,282,283,5,5,0,0,283,284,3,86,43,0,284,
  	285,5,6,0,0,285,286,3,28,14,0,286,294,1,0,0,0,287,288,5,25,0,0,288,289,
  	5,5,0,0,289,290,3,50,25,0,290,291,5,6,0,0,291,292,3,28,14,0,292,294,1,
  	0,0,0,293,273,1,0,0,0,293,281,1,0,0,0,293,287,1,0,0,0,294,49,1,0,0,0,
  	295,297,3,86,43,0,296,295,1,0,0,0,296,297,1,0,0,0,297,298,1,0,0,0,298,
  	300,5,14,0,0,299,301,3,86,43,0,300,299,1,0,0,0,300,301,1,0,0,0,301,302,
  	1,0,0,0,302,304,5,14,0,0,303,305,3,86,43,0,304,303,1,0,0,0,304,305,1,
  	0,0,0,305,335,1,0,0,0,306,307,5,17,0,0,307,308,3,38,19,0,308,310,5,14,
  	0,0,309,311,3,86,43,0,310,309,1,0,0,0,310,311,1,0,0,0,311,312,1,0,0,0,
  	312,314,5,14,0,0,313,315,3,86,43,0,314,313,1,0,0,0,314,315,1,0,0,0,315,
  	335,1,0,0,0,316,317,3,88,44,0,317,318,5,26,0,0,318,319,3,86,43,0,319,
  	335,1,0,0,0,320,321,5,17,0,0,321,322,3,40,20,0,322,323,5,26,0,0,323,324,
  	3,86,43,0,324,335,1,0,0,0,325,326,3,88,44,0,326,327,5,27,0,0,327,328,
  	3,86,43,0,328,335,1,0,0,0,329,330,5,17,0,0,330,331,3,40,20,0,331,332,
  	5,27,0,0,332,333,3,86,43,0,333,335,1,0,0,0,334,296,1,0,0,0,334,306,1,
  	0,0,0,334,316,1,0,0,0,334,320,1,0,0,0,334,325,1,0,0,0,334,329,1,0,0,0,
  	335,51,1,0,0,0,336,338,5,28,0,0,337,339,3,138,69,0,338,337,1,0,0,0,338,
  	339,1,0,0,0,339,340,1,0,0,0,340,341,3,140,70,0,341,53,1,0,0,0,342,344,
  	5,29,0,0,343,345,3,138,69,0,344,343,1,0,0,0,344,345,1,0,0,0,345,346,1,
  	0,0,0,346,347,3,140,70,0,347,55,1,0,0,0,348,350,5,30,0,0,349,351,3,86,
  	43,0,350,349,1,0,0,0,350,351,1,0,0,0,351,352,1,0,0,0,352,353,3,140,70,
  	0,353,57,1,0,0,0,354,355,5,31,0,0,355,356,5,5,0,0,356,357,3,86,43,0,357,
  	358,5,6,0,0,358,359,3,28,14,0,359,59,1,0,0,0,360,361,5,32,0,0,361,362,
  	5,5,0,0,362,363,3,86,43,0,363,364,5,6,0,0,364,365,3,62,31,0,365,61,1,
  	0,0,0,366,368,5,15,0,0,367,369,3,64,32,0,368,367,1,0,0,0,368,369,1,0,
  	0,0,369,370,1,0,0,0,370,371,5,16,0,0,371,63,1,0,0,0,372,374,3,66,33,0,
  	373,372,1,0,0,0,374,375,1,0,0,0,375,373,1,0,0,0,375,376,1,0,0,0,376,65,
  	1,0,0,0,377,378,5,33,0,0,378,379,3,86,43,0,379,381,5,34,0,0,380,382,3,
  	32,16,0,381,380,1,0,0,0,381,382,1,0,0,0,382,389,1,0,0,0,383,384,5,35,
  	0,0,384,386,5,34,0,0,385,387,3,32,16,0,386,385,1,0,0,0,386,387,1,0,0,
  	0,387,389,1,0,0,0,388,377,1,0,0,0,388,383,1,0,0,0,389,67,1,0,0,0,390,
  	391,5,36,0,0,391,392,3,86,43,0,392,393,3,140,70,0,393,69,1,0,0,0,394,
  	395,5,37,0,0,395,401,3,30,15,0,396,398,3,72,36,0,397,399,3,74,37,0,398,
  	397,1,0,0,0,398,399,1,0,0,0,399,402,1,0,0,0,400,402,3,74,37,0,401,396,
  	1,0,0,0,401,400,1,0,0,0,402,71,1,0,0,0,403,404,5,38,0,0,404,405,5,5,0,
  	0,405,406,3,138,69,0,406,407,5,6,0,0,407,408,3,30,15,0,408,73,1,0,0,0,
  	409,410,5,39,0,0,410,411,3,30,15,0,411,75,1,0,0,0,412,413,5,40,0,0,413,
  	414,3,140,70,0,414,77,1,0,0,0,415,416,3,138,69,0,416,417,5,34,0,0,417,
  	418,3,28,14,0,418,79,1,0,0,0,419,420,5,41,0,0,420,421,3,138,69,0,421,
  	423,5,5,0,0,422,424,3,82,41,0,423,422,1,0,0,0,423,424,1,0,0,0,424,425,
  	1,0,0,0,425,426,5,6,0,0,426,427,5,15,0,0,427,428,3,84,42,0,428,429,5,
  	16,0,0,429,81,1,0,0,0,430,435,3,138,69,0,431,432,5,20,0,0,432,434,3,138,
  	69,0,433,431,1,0,0,0,434,437,1,0,0,0,435,433,1,0,0,0,435,436,1,0,0,0,
  	436,83,1,0,0,0,437,435,1,0,0,0,438,440,3,2,1,0,439,438,1,0,0,0,439,440,
  	1,0,0,0,440,85,1,0,0,0,441,446,3,88,44,0,442,443,5,20,0,0,443,445,3,88,
  	44,0,444,442,1,0,0,0,445,448,1,0,0,0,446,444,1,0,0,0,446,447,1,0,0,0,
  	447,87,1,0,0,0,448,446,1,0,0,0,449,450,6,44,-1,0,450,478,3,94,47,0,451,
  	478,3,96,48,0,452,453,5,44,0,0,453,455,3,88,44,0,454,456,3,102,51,0,455,
  	454,1,0,0,0,455,456,1,0,0,0,456,478,1,0,0,0,457,458,5,47,0,0,458,478,
  	3,88,44,26,459,460,5,48,0,0,460,478,3,88,44,25,461,462,5,49,0,0,462,478,
  	3,88,44,24,463,464,5,45,0,0,464,478,3,88,44,23,465,466,5,46,0,0,466,478,
  	3,88,44,22,467,468,5,50,0,0,468,478,3,88,44,21,469,470,5,51,0,0,470,478,
  	3,88,44,20,471,472,5,52,0,0,472,478,3,88,44,19,473,474,5,53,0,0,474,478,
  	3,88,44,18,475,478,3,92,46,0,476,478,3,12,6,0,477,449,1,0,0,0,477,451,
  	1,0,0,0,477,452,1,0,0,0,477,457,1,0,0,0,477,459,1,0,0,0,477,461,1,0,0,
  	0,477,463,1,0,0,0,477,465,1,0,0,0,477,467,1,0,0,0,477,469,1,0,0,0,477,
  	471,1,0,0,0,477,473,1,0,0,0,477,475,1,0,0,0,477,476,1,0,0,0,478,544,1,
  	0,0,0,479,480,10,17,0,0,480,481,7,1,0,0,481,543,3,88,44,18,482,483,10,
  	16,0,0,483,484,7,2,0,0,484,543,3,88,44,17,485,486,10,15,0,0,486,487,7,
  	3,0,0,487,543,3,88,44,16,488,489,10,14,0,0,489,490,7,4,0,0,490,543,3,
  	88,44,15,491,492,10,13,0,0,492,493,5,64,0,0,493,543,3,88,44,14,494,495,
  	10,12,0,0,495,496,5,26,0,0,496,543,3,88,44,13,497,498,10,11,0,0,498,499,
  	7,5,0,0,499,543,3,88,44,12,500,501,10,10,0,0,501,502,5,69,0,0,502,543,
  	3,88,44,11,503,504,10,9,0,0,504,505,5,70,0,0,505,543,3,88,44,10,506,507,
  	10,8,0,0,507,508,5,71,0,0,508,543,3,88,44,9,509,510,10,7,0,0,510,511,
  	5,72,0,0,511,543,3,88,44,8,512,513,10,6,0,0,513,514,5,73,0,0,514,543,
  	3,88,44,7,515,516,10,5,0,0,516,517,5,74,0,0,517,518,3,88,44,0,518,519,
  	5,34,0,0,519,520,3,88,44,6,520,543,1,0,0,0,521,522,10,4,0,0,522,523,5,
  	10,0,0,523,543,3,88,44,5,524,525,10,3,0,0,525,526,3,90,45,0,526,527,3,
  	88,44,4,527,543,1,0,0,0,528,529,10,32,0,0,529,530,5,42,0,0,530,531,3,
  	86,43,0,531,532,5,43,0,0,532,543,1,0,0,0,533,534,10,31,0,0,534,535,5,
  	11,0,0,535,543,3,138,69,0,536,537,10,30,0,0,537,543,3,102,51,0,538,539,
  	10,28,0,0,539,543,5,45,0,0,540,541,10,27,0,0,541,543,5,46,0,0,542,479,
  	1,0,0,0,542,482,1,0,0,0,542,485,1,0,0,0,542,488,1,0,0,0,542,491,1,0,0,
  	0,542,494,1,0,0,0,542,497,1,0,0,0,542,500,1,0,0,0,542,503,1,0,0,0,542,
  	506,1,0,0,0,542,509,1,0,0,0,542,512,1,0,0,0,542,515,1,0,0,0,542,521,1,
  	0,0,0,542,524,1,0,0,0,542,528,1,0,0,0,542,533,1,0,0,0,542,536,1,0,0,0,
  	542,538,1,0,0,0,542,540,1,0,0,0,543,546,1,0,0,0,544,542,1,0,0,0,544,545,
  	1,0,0,0,545,89,1,0,0,0,546,544,1,0,0,0,547,548,7,6,0,0,548,91,1,0,0,0,
  	549,565,5,86,0,0,550,565,3,138,69,0,551,565,3,106,53,0,552,565,3,122,
  	61,0,553,565,3,128,64,0,554,555,5,5,0,0,555,556,3,86,43,0,556,557,5,6,
  	0,0,557,565,1,0,0,0,558,565,3,8,4,0,559,565,3,14,7,0,560,565,3,16,8,0,
  	561,565,3,18,9,0,562,565,3,22,11,0,563,565,3,24,12,0,564,549,1,0,0,0,
  	564,550,1,0,0,0,564,551,1,0,0,0,564,552,1,0,0,0,564,553,1,0,0,0,564,554,
  	1,0,0,0,564,558,1,0,0,0,564,559,1,0,0,0,564,560,1,0,0,0,564,561,1,0,0,
  	0,564,562,1,0,0,0,564,563,1,0,0,0,565,93,1,0,0,0,566,568,5,41,0,0,567,
  	569,3,138,69,0,568,567,1,0,0,0,568,569,1,0,0,0,569,570,1,0,0,0,570,572,
  	5,5,0,0,571,573,3,82,41,0,572,571,1,0,0,0,572,573,1,0,0,0,573,574,1,0,
  	0,0,574,575,5,6,0,0,575,576,5,15,0,0,576,577,3,84,42,0,577,578,5,16,0,
  	0,578,95,1,0,0,0,579,580,3,98,49,0,580,581,5,87,0,0,581,582,3,100,50,
  	0,582,97,1,0,0,0,583,590,3,138,69,0,584,586,5,5,0,0,585,587,3,82,41,0,
  	586,585,1,0,0,0,586,587,1,0,0,0,587,588,1,0,0,0,588,590,5,6,0,0,589,583,
  	1,0,0,0,589,584,1,0,0,0,590,99,1,0,0,0,591,597,3,88,44,0,592,593,5,15,
  	0,0,593,594,3,84,42,0,594,595,5,16,0,0,595,597,1,0,0,0,596,591,1,0,0,
  	0,596,592,1,0,0,0,597,101,1,0,0,0,598,600,5,5,0,0,599,601,3,104,52,0,
  	600,599,1,0,0,0,600,601,1,0,0,0,601,602,1,0,0,0,602,603,5,6,0,0,603,103,
  	1,0,0,0,604,609,3,88,44,0,605,606,5,20,0,0,606,608,3,88,44,0,607,605,
  	1,0,0,0,608,611,1,0,0,0,609,607,1,0,0,0,609,610,1,0,0,0,610,105,1,0,0,
  	0,611,609,1,0,0,0,612,619,3,108,54,0,613,619,3,110,55,0,614,619,3,112,
  	56,0,615,619,3,114,57,0,616,619,3,118,59,0,617,619,3,120,60,0,618,612,
  	1,0,0,0,618,613,1,0,0,0,618,614,1,0,0,0,618,615,1,0,0,0,618,616,1,0,0,
  	0,618,617,1,0,0,0,619,107,1,0,0,0,620,621,5,88,0,0,621,109,1,0,0,0,622,
  	623,7,7,0,0,623,111,1,0,0,0,624,625,5,95,0,0,625,113,1,0,0,0,626,630,
  	5,91,0,0,627,629,3,116,58,0,628,627,1,0,0,0,629,632,1,0,0,0,630,628,1,
  	0,0,0,630,631,1,0,0,0,631,633,1,0,0,0,632,630,1,0,0,0,633,634,5,91,0,
  	0,634,115,1,0,0,0,635,641,5,96,0,0,636,637,5,92,0,0,637,638,3,88,44,0,
  	638,639,5,16,0,0,639,641,1,0,0,0,640,635,1,0,0,0,640,636,1,0,0,0,641,
  	117,1,0,0,0,642,643,5,97,0,0,643,119,1,0,0,0,644,645,7,8,0,0,645,121,
  	1,0,0,0,646,648,5,42,0,0,647,649,3,124,62,0,648,647,1,0,0,0,648,649,1,
  	0,0,0,649,650,1,0,0,0,650,651,5,43,0,0,651,123,1,0,0,0,652,654,5,20,0,
  	0,653,652,1,0,0,0,654,657,1,0,0,0,655,653,1,0,0,0,655,656,1,0,0,0,656,
  	659,1,0,0,0,657,655,1,0,0,0,658,660,3,126,63,0,659,658,1,0,0,0,659,660,
  	1,0,0,0,660,669,1,0,0,0,661,663,5,20,0,0,662,661,1,0,0,0,663,664,1,0,
  	0,0,664,662,1,0,0,0,664,665,1,0,0,0,665,666,1,0,0,0,666,668,3,126,63,
  	0,667,662,1,0,0,0,668,671,1,0,0,0,669,667,1,0,0,0,669,670,1,0,0,0,670,
  	675,1,0,0,0,671,669,1,0,0,0,672,674,5,20,0,0,673,672,1,0,0,0,674,677,
  	1,0,0,0,675,673,1,0,0,0,675,676,1,0,0,0,676,125,1,0,0,0,677,675,1,0,0,
  	0,678,679,3,88,44,0,679,127,1,0,0,0,680,682,5,15,0,0,681,683,3,130,65,
  	0,682,681,1,0,0,0,682,683,1,0,0,0,683,688,1,0,0,0,684,685,5,20,0,0,685,
  	687,3,130,65,0,686,684,1,0,0,0,687,690,1,0,0,0,688,686,1,0,0,0,688,689,
  	1,0,0,0,689,692,1,0,0,0,690,688,1,0,0,0,691,693,5,20,0,0,692,691,1,0,
  	0,0,692,693,1,0,0,0,693,694,1,0,0,0,694,695,5,16,0,0,695,129,1,0,0,0,
  	696,697,3,132,66,0,697,698,5,34,0,0,698,699,3,88,44,0,699,710,1,0,0,0,
  	700,701,5,42,0,0,701,702,3,88,44,0,702,703,5,43,0,0,703,704,5,34,0,0,
  	704,705,3,88,44,0,705,710,1,0,0,0,706,710,3,138,69,0,707,710,3,134,67,
  	0,708,710,3,136,68,0,709,696,1,0,0,0,709,700,1,0,0,0,709,706,1,0,0,0,
  	709,707,1,0,0,0,709,708,1,0,0,0,710,131,1,0,0,0,711,715,3,138,69,0,712,
  	715,3,112,56,0,713,715,3,120,60,0,714,711,1,0,0,0,714,712,1,0,0,0,714,
  	713,1,0,0,0,715,133,1,0,0,0,716,717,5,93,0,0,717,718,3,132,66,0,718,719,
  	5,5,0,0,719,720,5,6,0,0,720,721,5,15,0,0,721,722,3,84,42,0,722,723,5,
  	16,0,0,723,135,1,0,0,0,724,725,5,94,0,0,725,726,3,132,66,0,726,727,5,
  	5,0,0,727,728,3,138,69,0,728,729,5,6,0,0,729,730,5,15,0,0,730,731,3,84,
  	42,0,731,732,5,16,0,0,732,137,1,0,0,0,733,734,7,9,0,0,734,139,1,0,0,0,
  	735,739,5,14,0,0,736,739,5,0,0,1,737,739,4,70,20,0,738,735,1,0,0,0,738,
  	736,1,0,0,0,738,737,1,0,0,0,739,141,1,0,0,0,58,143,150,155,166,227,231,
  	238,251,257,271,293,296,300,304,310,314,334,338,344,350,368,375,381,386,
  	388,398,401,423,435,439,446,455,477,542,544,564,568,572,586,589,596,600,
  	609,618,630,640,648,655,659,664,669,675,682,688,692,709,714,738
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  ecmascriptParserStaticData = staticData.release();
}

}

ECMAScriptParser::ECMAScriptParser(TokenStream *input) : ECMAScriptParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

ECMAScriptParser::ECMAScriptParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  ECMAScriptParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *ecmascriptParserStaticData->atn, ecmascriptParserStaticData->decisionToDFA, ecmascriptParserStaticData->sharedContextCache, options);
}

ECMAScriptParser::~ECMAScriptParser() {
  delete _interpreter;
}

const atn::ATN& ECMAScriptParser::getATN() const {
  return *ecmascriptParserStaticData->atn;
}

std::string ECMAScriptParser::getGrammarFileName() const {
  return "ECMAScript.g4";
}

const std::vector<std::string>& ECMAScriptParser::getRuleNames() const {
  return ecmascriptParserStaticData->ruleNames;
}

const dfa::Vocabulary& ECMAScriptParser::getVocabulary() const {
  return ecmascriptParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView ECMAScriptParser::getSerializedATN() const {
  return ecmascriptParserStaticData->serializedATN;
}


//----------------- ProgramContext ------------------------------------------------------------------

ECMAScriptParser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ECMAScriptParser::ProgramContext::EOF() {
  return getToken(ECMAScriptParser::EOF, 0);
}

ECMAScriptParser::SourceElementsContext* ECMAScriptParser::ProgramContext::sourceElements() {
  return getRuleContext<ECMAScriptParser::SourceElementsContext>(0);
}


size_t ECMAScriptParser::ProgramContext::getRuleIndex() const {
  return ECMAScriptParser::RuleProgram;
}

void ECMAScriptParser::ProgramContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterProgram(this);
}

void ECMAScriptParser::ProgramContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitProgram(this);
}

ECMAScriptParser::ProgramContext* ECMAScriptParser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, ECMAScriptParser::RuleProgram);
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
    setState(143);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 18005585235465138) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 545179828225) != 0)) {
      setState(142);
      sourceElements();
    }
    setState(145);
    match(ECMAScriptParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SourceElementsContext ------------------------------------------------------------------

ECMAScriptParser::SourceElementsContext::SourceElementsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ECMAScriptParser::SourceElementContext *> ECMAScriptParser::SourceElementsContext::sourceElement() {
  return getRuleContexts<ECMAScriptParser::SourceElementContext>();
}

ECMAScriptParser::SourceElementContext* ECMAScriptParser::SourceElementsContext::sourceElement(size_t i) {
  return getRuleContext<ECMAScriptParser::SourceElementContext>(i);
}


size_t ECMAScriptParser::SourceElementsContext::getRuleIndex() const {
  return ECMAScriptParser::RuleSourceElements;
}

void ECMAScriptParser::SourceElementsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSourceElements(this);
}

void ECMAScriptParser::SourceElementsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSourceElements(this);
}

ECMAScriptParser::SourceElementsContext* ECMAScriptParser::sourceElements() {
  SourceElementsContext *_localctx = _tracker.createInstance<SourceElementsContext>(_ctx, getState());
  enterRule(_localctx, 2, ECMAScriptParser::RuleSourceElements);
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
    setState(148); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(147);
      sourceElement();
      setState(150); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 18005585235465138) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 545179828225) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SourceElementContext ------------------------------------------------------------------

ECMAScriptParser::SourceElementContext::SourceElementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::StatementContext* ECMAScriptParser::SourceElementContext::statement() {
  return getRuleContext<ECMAScriptParser::StatementContext>(0);
}

ECMAScriptParser::FunctionDeclarationContext* ECMAScriptParser::SourceElementContext::functionDeclaration() {
  return getRuleContext<ECMAScriptParser::FunctionDeclarationContext>(0);
}

ECMAScriptParser::ChtlJsElementContext* ECMAScriptParser::SourceElementContext::chtlJsElement() {
  return getRuleContext<ECMAScriptParser::ChtlJsElementContext>(0);
}


size_t ECMAScriptParser::SourceElementContext::getRuleIndex() const {
  return ECMAScriptParser::RuleSourceElement;
}

void ECMAScriptParser::SourceElementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSourceElement(this);
}

void ECMAScriptParser::SourceElementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSourceElement(this);
}

ECMAScriptParser::SourceElementContext* ECMAScriptParser::sourceElement() {
  SourceElementContext *_localctx = _tracker.createInstance<SourceElementContext>(_ctx, getState());
  enterRule(_localctx, 4, ECMAScriptParser::RuleSourceElement);

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
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(152);
      statement();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(153);
      functionDeclaration();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(154);
      chtlJsElement();
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

//----------------- ChtlJsElementContext ------------------------------------------------------------------

ECMAScriptParser::ChtlJsElementContext::ChtlJsElementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::ChtlEnhancedSelectorContext* ECMAScriptParser::ChtlJsElementContext::chtlEnhancedSelector() {
  return getRuleContext<ECMAScriptParser::ChtlEnhancedSelectorContext>(0);
}

ECMAScriptParser::ChtlArrowAccessContext* ECMAScriptParser::ChtlJsElementContext::chtlArrowAccess() {
  return getRuleContext<ECMAScriptParser::ChtlArrowAccessContext>(0);
}

ECMAScriptParser::ChtlListenFunctionContext* ECMAScriptParser::ChtlJsElementContext::chtlListenFunction() {
  return getRuleContext<ECMAScriptParser::ChtlListenFunctionContext>(0);
}

ECMAScriptParser::ChtlDelegateFunctionContext* ECMAScriptParser::ChtlJsElementContext::chtlDelegateFunction() {
  return getRuleContext<ECMAScriptParser::ChtlDelegateFunctionContext>(0);
}

ECMAScriptParser::ChtlAnimateFunctionContext* ECMAScriptParser::ChtlJsElementContext::chtlAnimateFunction() {
  return getRuleContext<ECMAScriptParser::ChtlAnimateFunctionContext>(0);
}

ECMAScriptParser::ChtlVirDeclarationContext* ECMAScriptParser::ChtlJsElementContext::chtlVirDeclaration() {
  return getRuleContext<ECMAScriptParser::ChtlVirDeclarationContext>(0);
}

ECMAScriptParser::ChtlVirAccessContext* ECMAScriptParser::ChtlJsElementContext::chtlVirAccess() {
  return getRuleContext<ECMAScriptParser::ChtlVirAccessContext>(0);
}

ECMAScriptParser::ChtlVariableUsageContext* ECMAScriptParser::ChtlJsElementContext::chtlVariableUsage() {
  return getRuleContext<ECMAScriptParser::ChtlVariableUsageContext>(0);
}

ECMAScriptParser::ChtlOriginEmbedContext* ECMAScriptParser::ChtlJsElementContext::chtlOriginEmbed() {
  return getRuleContext<ECMAScriptParser::ChtlOriginEmbedContext>(0);
}


size_t ECMAScriptParser::ChtlJsElementContext::getRuleIndex() const {
  return ECMAScriptParser::RuleChtlJsElement;
}

void ECMAScriptParser::ChtlJsElementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlJsElement(this);
}

void ECMAScriptParser::ChtlJsElementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlJsElement(this);
}

ECMAScriptParser::ChtlJsElementContext* ECMAScriptParser::chtlJsElement() {
  ChtlJsElementContext *_localctx = _tracker.createInstance<ChtlJsElementContext>(_ctx, getState());
  enterRule(_localctx, 6, ECMAScriptParser::RuleChtlJsElement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(166);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(157);
      chtlEnhancedSelector();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(158);
      chtlArrowAccess();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(159);
      chtlListenFunction();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(160);
      chtlDelegateFunction();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(161);
      chtlAnimateFunction();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(162);
      chtlVirDeclaration();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(163);
      chtlVirAccess();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(164);
      chtlVariableUsage();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(165);
      chtlOriginEmbed();
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

//----------------- ChtlEnhancedSelectorContext ------------------------------------------------------------------

ECMAScriptParser::ChtlEnhancedSelectorContext::ChtlEnhancedSelectorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::SelectorExpressionContext* ECMAScriptParser::ChtlEnhancedSelectorContext::selectorExpression() {
  return getRuleContext<ECMAScriptParser::SelectorExpressionContext>(0);
}


size_t ECMAScriptParser::ChtlEnhancedSelectorContext::getRuleIndex() const {
  return ECMAScriptParser::RuleChtlEnhancedSelector;
}

void ECMAScriptParser::ChtlEnhancedSelectorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlEnhancedSelector(this);
}

void ECMAScriptParser::ChtlEnhancedSelectorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlEnhancedSelector(this);
}

ECMAScriptParser::ChtlEnhancedSelectorContext* ECMAScriptParser::chtlEnhancedSelector() {
  ChtlEnhancedSelectorContext *_localctx = _tracker.createInstance<ChtlEnhancedSelectorContext>(_ctx, getState());
  enterRule(_localctx, 8, ECMAScriptParser::RuleChtlEnhancedSelector);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(168);
    match(ECMAScriptParser::T__0);
    setState(169);
    selectorExpression();
    setState(170);
    match(ECMAScriptParser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SelectorExpressionContext ------------------------------------------------------------------

ECMAScriptParser::SelectorExpressionContext::SelectorExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ECMAScriptParser::SelectorExpressionContext::SELECTOR_CONTENT() {
  return getToken(ECMAScriptParser::SELECTOR_CONTENT, 0);
}


size_t ECMAScriptParser::SelectorExpressionContext::getRuleIndex() const {
  return ECMAScriptParser::RuleSelectorExpression;
}

void ECMAScriptParser::SelectorExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSelectorExpression(this);
}

void ECMAScriptParser::SelectorExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSelectorExpression(this);
}

ECMAScriptParser::SelectorExpressionContext* ECMAScriptParser::selectorExpression() {
  SelectorExpressionContext *_localctx = _tracker.createInstance<SelectorExpressionContext>(_ctx, getState());
  enterRule(_localctx, 10, ECMAScriptParser::RuleSelectorExpression);

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
    match(ECMAScriptParser::SELECTOR_CONTENT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChtlArrowAccessContext ------------------------------------------------------------------

ECMAScriptParser::ChtlArrowAccessContext::ChtlArrowAccessContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::PrimaryExpressionContext* ECMAScriptParser::ChtlArrowAccessContext::primaryExpression() {
  return getRuleContext<ECMAScriptParser::PrimaryExpressionContext>(0);
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::ChtlArrowAccessContext::identifier() {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(0);
}


size_t ECMAScriptParser::ChtlArrowAccessContext::getRuleIndex() const {
  return ECMAScriptParser::RuleChtlArrowAccess;
}

void ECMAScriptParser::ChtlArrowAccessContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlArrowAccess(this);
}

void ECMAScriptParser::ChtlArrowAccessContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlArrowAccess(this);
}

ECMAScriptParser::ChtlArrowAccessContext* ECMAScriptParser::chtlArrowAccess() {
  ChtlArrowAccessContext *_localctx = _tracker.createInstance<ChtlArrowAccessContext>(_ctx, getState());
  enterRule(_localctx, 12, ECMAScriptParser::RuleChtlArrowAccess);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(174);
    primaryExpression();
    setState(175);
    match(ECMAScriptParser::T__2);
    setState(176);
    identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChtlListenFunctionContext ------------------------------------------------------------------

ECMAScriptParser::ChtlListenFunctionContext::ChtlListenFunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::ArgumentsContext* ECMAScriptParser::ChtlListenFunctionContext::arguments() {
  return getRuleContext<ECMAScriptParser::ArgumentsContext>(0);
}


size_t ECMAScriptParser::ChtlListenFunctionContext::getRuleIndex() const {
  return ECMAScriptParser::RuleChtlListenFunction;
}

void ECMAScriptParser::ChtlListenFunctionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlListenFunction(this);
}

void ECMAScriptParser::ChtlListenFunctionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlListenFunction(this);
}

ECMAScriptParser::ChtlListenFunctionContext* ECMAScriptParser::chtlListenFunction() {
  ChtlListenFunctionContext *_localctx = _tracker.createInstance<ChtlListenFunctionContext>(_ctx, getState());
  enterRule(_localctx, 14, ECMAScriptParser::RuleChtlListenFunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(178);
    match(ECMAScriptParser::T__3);
    setState(179);
    match(ECMAScriptParser::T__4);
    setState(180);
    arguments();
    setState(181);
    match(ECMAScriptParser::T__5);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChtlDelegateFunctionContext ------------------------------------------------------------------

ECMAScriptParser::ChtlDelegateFunctionContext::ChtlDelegateFunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::ArgumentsContext* ECMAScriptParser::ChtlDelegateFunctionContext::arguments() {
  return getRuleContext<ECMAScriptParser::ArgumentsContext>(0);
}


size_t ECMAScriptParser::ChtlDelegateFunctionContext::getRuleIndex() const {
  return ECMAScriptParser::RuleChtlDelegateFunction;
}

void ECMAScriptParser::ChtlDelegateFunctionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlDelegateFunction(this);
}

void ECMAScriptParser::ChtlDelegateFunctionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlDelegateFunction(this);
}

ECMAScriptParser::ChtlDelegateFunctionContext* ECMAScriptParser::chtlDelegateFunction() {
  ChtlDelegateFunctionContext *_localctx = _tracker.createInstance<ChtlDelegateFunctionContext>(_ctx, getState());
  enterRule(_localctx, 16, ECMAScriptParser::RuleChtlDelegateFunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(183);
    match(ECMAScriptParser::T__6);
    setState(184);
    match(ECMAScriptParser::T__4);
    setState(185);
    arguments();
    setState(186);
    match(ECMAScriptParser::T__5);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChtlAnimateFunctionContext ------------------------------------------------------------------

ECMAScriptParser::ChtlAnimateFunctionContext::ChtlAnimateFunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::ArgumentsContext* ECMAScriptParser::ChtlAnimateFunctionContext::arguments() {
  return getRuleContext<ECMAScriptParser::ArgumentsContext>(0);
}


size_t ECMAScriptParser::ChtlAnimateFunctionContext::getRuleIndex() const {
  return ECMAScriptParser::RuleChtlAnimateFunction;
}

void ECMAScriptParser::ChtlAnimateFunctionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlAnimateFunction(this);
}

void ECMAScriptParser::ChtlAnimateFunctionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlAnimateFunction(this);
}

ECMAScriptParser::ChtlAnimateFunctionContext* ECMAScriptParser::chtlAnimateFunction() {
  ChtlAnimateFunctionContext *_localctx = _tracker.createInstance<ChtlAnimateFunctionContext>(_ctx, getState());
  enterRule(_localctx, 18, ECMAScriptParser::RuleChtlAnimateFunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(188);
    match(ECMAScriptParser::T__7);
    setState(189);
    match(ECMAScriptParser::T__4);
    setState(190);
    arguments();
    setState(191);
    match(ECMAScriptParser::T__5);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChtlVirDeclarationContext ------------------------------------------------------------------

ECMAScriptParser::ChtlVirDeclarationContext::ChtlVirDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::ChtlVirDeclarationContext::identifier() {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(0);
}

ECMAScriptParser::ObjectLiteralContext* ECMAScriptParser::ChtlVirDeclarationContext::objectLiteral() {
  return getRuleContext<ECMAScriptParser::ObjectLiteralContext>(0);
}


size_t ECMAScriptParser::ChtlVirDeclarationContext::getRuleIndex() const {
  return ECMAScriptParser::RuleChtlVirDeclaration;
}

void ECMAScriptParser::ChtlVirDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlVirDeclaration(this);
}

void ECMAScriptParser::ChtlVirDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlVirDeclaration(this);
}

ECMAScriptParser::ChtlVirDeclarationContext* ECMAScriptParser::chtlVirDeclaration() {
  ChtlVirDeclarationContext *_localctx = _tracker.createInstance<ChtlVirDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 20, ECMAScriptParser::RuleChtlVirDeclaration);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(193);
    match(ECMAScriptParser::T__8);
    setState(194);
    identifier();
    setState(195);
    match(ECMAScriptParser::T__9);
    setState(196);
    objectLiteral();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChtlVirAccessContext ------------------------------------------------------------------

ECMAScriptParser::ChtlVirAccessContext::ChtlVirAccessContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ECMAScriptParser::IdentifierContext *> ECMAScriptParser::ChtlVirAccessContext::identifier() {
  return getRuleContexts<ECMAScriptParser::IdentifierContext>();
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::ChtlVirAccessContext::identifier(size_t i) {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(i);
}


size_t ECMAScriptParser::ChtlVirAccessContext::getRuleIndex() const {
  return ECMAScriptParser::RuleChtlVirAccess;
}

void ECMAScriptParser::ChtlVirAccessContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlVirAccess(this);
}

void ECMAScriptParser::ChtlVirAccessContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlVirAccess(this);
}

ECMAScriptParser::ChtlVirAccessContext* ECMAScriptParser::chtlVirAccess() {
  ChtlVirAccessContext *_localctx = _tracker.createInstance<ChtlVirAccessContext>(_ctx, getState());
  enterRule(_localctx, 22, ECMAScriptParser::RuleChtlVirAccess);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(198);
    identifier();
    setState(199);
    match(ECMAScriptParser::T__10);
    setState(200);
    identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChtlVariableUsageContext ------------------------------------------------------------------

ECMAScriptParser::ChtlVariableUsageContext::ChtlVariableUsageContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ECMAScriptParser::IdentifierContext *> ECMAScriptParser::ChtlVariableUsageContext::identifier() {
  return getRuleContexts<ECMAScriptParser::IdentifierContext>();
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::ChtlVariableUsageContext::identifier(size_t i) {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(i);
}


size_t ECMAScriptParser::ChtlVariableUsageContext::getRuleIndex() const {
  return ECMAScriptParser::RuleChtlVariableUsage;
}

void ECMAScriptParser::ChtlVariableUsageContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlVariableUsage(this);
}

void ECMAScriptParser::ChtlVariableUsageContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlVariableUsage(this);
}

ECMAScriptParser::ChtlVariableUsageContext* ECMAScriptParser::chtlVariableUsage() {
  ChtlVariableUsageContext *_localctx = _tracker.createInstance<ChtlVariableUsageContext>(_ctx, getState());
  enterRule(_localctx, 24, ECMAScriptParser::RuleChtlVariableUsage);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(202);
    identifier();
    setState(203);
    match(ECMAScriptParser::T__4);
    setState(204);
    identifier();
    setState(205);
    match(ECMAScriptParser::T__5);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChtlOriginEmbedContext ------------------------------------------------------------------

ECMAScriptParser::ChtlOriginEmbedContext::ChtlOriginEmbedContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::ChtlOriginEmbedContext::identifier() {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(0);
}


size_t ECMAScriptParser::ChtlOriginEmbedContext::getRuleIndex() const {
  return ECMAScriptParser::RuleChtlOriginEmbed;
}

void ECMAScriptParser::ChtlOriginEmbedContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlOriginEmbed(this);
}

void ECMAScriptParser::ChtlOriginEmbedContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlOriginEmbed(this);
}

ECMAScriptParser::ChtlOriginEmbedContext* ECMAScriptParser::chtlOriginEmbed() {
  ChtlOriginEmbedContext *_localctx = _tracker.createInstance<ChtlOriginEmbedContext>(_ctx, getState());
  enterRule(_localctx, 26, ECMAScriptParser::RuleChtlOriginEmbed);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(207);
    match(ECMAScriptParser::T__11);
    setState(208);
    match(ECMAScriptParser::T__12);
    setState(209);
    identifier();
    setState(210);
    match(ECMAScriptParser::T__13);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

ECMAScriptParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::BlockContext* ECMAScriptParser::StatementContext::block() {
  return getRuleContext<ECMAScriptParser::BlockContext>(0);
}

ECMAScriptParser::VariableStatementContext* ECMAScriptParser::StatementContext::variableStatement() {
  return getRuleContext<ECMAScriptParser::VariableStatementContext>(0);
}

ECMAScriptParser::EmptyStatementContext* ECMAScriptParser::StatementContext::emptyStatement() {
  return getRuleContext<ECMAScriptParser::EmptyStatementContext>(0);
}

ECMAScriptParser::ExpressionStatementContext* ECMAScriptParser::StatementContext::expressionStatement() {
  return getRuleContext<ECMAScriptParser::ExpressionStatementContext>(0);
}

ECMAScriptParser::IfStatementContext* ECMAScriptParser::StatementContext::ifStatement() {
  return getRuleContext<ECMAScriptParser::IfStatementContext>(0);
}

ECMAScriptParser::IterationStatementContext* ECMAScriptParser::StatementContext::iterationStatement() {
  return getRuleContext<ECMAScriptParser::IterationStatementContext>(0);
}

ECMAScriptParser::ContinueStatementContext* ECMAScriptParser::StatementContext::continueStatement() {
  return getRuleContext<ECMAScriptParser::ContinueStatementContext>(0);
}

ECMAScriptParser::BreakStatementContext* ECMAScriptParser::StatementContext::breakStatement() {
  return getRuleContext<ECMAScriptParser::BreakStatementContext>(0);
}

ECMAScriptParser::ReturnStatementContext* ECMAScriptParser::StatementContext::returnStatement() {
  return getRuleContext<ECMAScriptParser::ReturnStatementContext>(0);
}

ECMAScriptParser::WithStatementContext* ECMAScriptParser::StatementContext::withStatement() {
  return getRuleContext<ECMAScriptParser::WithStatementContext>(0);
}

ECMAScriptParser::SwitchStatementContext* ECMAScriptParser::StatementContext::switchStatement() {
  return getRuleContext<ECMAScriptParser::SwitchStatementContext>(0);
}

ECMAScriptParser::ThrowStatementContext* ECMAScriptParser::StatementContext::throwStatement() {
  return getRuleContext<ECMAScriptParser::ThrowStatementContext>(0);
}

ECMAScriptParser::TryStatementContext* ECMAScriptParser::StatementContext::tryStatement() {
  return getRuleContext<ECMAScriptParser::TryStatementContext>(0);
}

ECMAScriptParser::DebuggerStatementContext* ECMAScriptParser::StatementContext::debuggerStatement() {
  return getRuleContext<ECMAScriptParser::DebuggerStatementContext>(0);
}

ECMAScriptParser::LabelledStatementContext* ECMAScriptParser::StatementContext::labelledStatement() {
  return getRuleContext<ECMAScriptParser::LabelledStatementContext>(0);
}


size_t ECMAScriptParser::StatementContext::getRuleIndex() const {
  return ECMAScriptParser::RuleStatement;
}

void ECMAScriptParser::StatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStatement(this);
}

void ECMAScriptParser::StatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStatement(this);
}

ECMAScriptParser::StatementContext* ECMAScriptParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 28, ECMAScriptParser::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(227);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(212);
      block();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(213);
      variableStatement();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(214);
      emptyStatement();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(215);
      expressionStatement();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(216);
      ifStatement();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(217);
      iterationStatement();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(218);
      continueStatement();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(219);
      breakStatement();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(220);
      returnStatement();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(221);
      withStatement();
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(222);
      switchStatement();
      break;
    }

    case 12: {
      enterOuterAlt(_localctx, 12);
      setState(223);
      throwStatement();
      break;
    }

    case 13: {
      enterOuterAlt(_localctx, 13);
      setState(224);
      tryStatement();
      break;
    }

    case 14: {
      enterOuterAlt(_localctx, 14);
      setState(225);
      debuggerStatement();
      break;
    }

    case 15: {
      enterOuterAlt(_localctx, 15);
      setState(226);
      labelledStatement();
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

ECMAScriptParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::StatementListContext* ECMAScriptParser::BlockContext::statementList() {
  return getRuleContext<ECMAScriptParser::StatementListContext>(0);
}


size_t ECMAScriptParser::BlockContext::getRuleIndex() const {
  return ECMAScriptParser::RuleBlock;
}

void ECMAScriptParser::BlockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlock(this);
}

void ECMAScriptParser::BlockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlock(this);
}

ECMAScriptParser::BlockContext* ECMAScriptParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 30, ECMAScriptParser::RuleBlock);
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
    match(ECMAScriptParser::T__14);
    setState(231);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 18005585235460530) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 545179828225) != 0)) {
      setState(230);
      statementList();
    }
    setState(233);
    match(ECMAScriptParser::T__15);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementListContext ------------------------------------------------------------------

ECMAScriptParser::StatementListContext::StatementListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ECMAScriptParser::StatementContext *> ECMAScriptParser::StatementListContext::statement() {
  return getRuleContexts<ECMAScriptParser::StatementContext>();
}

ECMAScriptParser::StatementContext* ECMAScriptParser::StatementListContext::statement(size_t i) {
  return getRuleContext<ECMAScriptParser::StatementContext>(i);
}


size_t ECMAScriptParser::StatementListContext::getRuleIndex() const {
  return ECMAScriptParser::RuleStatementList;
}

void ECMAScriptParser::StatementListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStatementList(this);
}

void ECMAScriptParser::StatementListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStatementList(this);
}

ECMAScriptParser::StatementListContext* ECMAScriptParser::statementList() {
  StatementListContext *_localctx = _tracker.createInstance<StatementListContext>(_ctx, getState());
  enterRule(_localctx, 32, ECMAScriptParser::RuleStatementList);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(236); 
    _errHandler->sync(this);
    alt = 1;
    do {
      switch (alt) {
        case 1: {
              setState(235);
              statement();
              break;
            }

      default:
        throw NoViableAltException(this);
      }
      setState(238); 
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx);
    } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VariableStatementContext ------------------------------------------------------------------

ECMAScriptParser::VariableStatementContext::VariableStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::VarModifierContext* ECMAScriptParser::VariableStatementContext::varModifier() {
  return getRuleContext<ECMAScriptParser::VarModifierContext>(0);
}

ECMAScriptParser::VariableDeclarationListContext* ECMAScriptParser::VariableStatementContext::variableDeclarationList() {
  return getRuleContext<ECMAScriptParser::VariableDeclarationListContext>(0);
}

ECMAScriptParser::EosContext* ECMAScriptParser::VariableStatementContext::eos() {
  return getRuleContext<ECMAScriptParser::EosContext>(0);
}


size_t ECMAScriptParser::VariableStatementContext::getRuleIndex() const {
  return ECMAScriptParser::RuleVariableStatement;
}

void ECMAScriptParser::VariableStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVariableStatement(this);
}

void ECMAScriptParser::VariableStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVariableStatement(this);
}

ECMAScriptParser::VariableStatementContext* ECMAScriptParser::variableStatement() {
  VariableStatementContext *_localctx = _tracker.createInstance<VariableStatementContext>(_ctx, getState());
  enterRule(_localctx, 34, ECMAScriptParser::RuleVariableStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(240);
    varModifier();
    setState(241);
    variableDeclarationList();
    setState(242);
    eos();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarModifierContext ------------------------------------------------------------------

ECMAScriptParser::VarModifierContext::VarModifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ECMAScriptParser::VarModifierContext::getRuleIndex() const {
  return ECMAScriptParser::RuleVarModifier;
}

void ECMAScriptParser::VarModifierContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVarModifier(this);
}

void ECMAScriptParser::VarModifierContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVarModifier(this);
}

ECMAScriptParser::VarModifierContext* ECMAScriptParser::varModifier() {
  VarModifierContext *_localctx = _tracker.createInstance<VarModifierContext>(_ctx, getState());
  enterRule(_localctx, 36, ECMAScriptParser::RuleVarModifier);
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
    setState(244);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 917504) != 0))) {
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

//----------------- VariableDeclarationListContext ------------------------------------------------------------------

ECMAScriptParser::VariableDeclarationListContext::VariableDeclarationListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ECMAScriptParser::VariableDeclarationContext *> ECMAScriptParser::VariableDeclarationListContext::variableDeclaration() {
  return getRuleContexts<ECMAScriptParser::VariableDeclarationContext>();
}

ECMAScriptParser::VariableDeclarationContext* ECMAScriptParser::VariableDeclarationListContext::variableDeclaration(size_t i) {
  return getRuleContext<ECMAScriptParser::VariableDeclarationContext>(i);
}


size_t ECMAScriptParser::VariableDeclarationListContext::getRuleIndex() const {
  return ECMAScriptParser::RuleVariableDeclarationList;
}

void ECMAScriptParser::VariableDeclarationListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVariableDeclarationList(this);
}

void ECMAScriptParser::VariableDeclarationListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVariableDeclarationList(this);
}

ECMAScriptParser::VariableDeclarationListContext* ECMAScriptParser::variableDeclarationList() {
  VariableDeclarationListContext *_localctx = _tracker.createInstance<VariableDeclarationListContext>(_ctx, getState());
  enterRule(_localctx, 38, ECMAScriptParser::RuleVariableDeclarationList);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(246);
    variableDeclaration();
    setState(251);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(247);
        match(ECMAScriptParser::T__19);
        setState(248);
        variableDeclaration(); 
      }
      setState(253);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
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

ECMAScriptParser::VariableDeclarationContext::VariableDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::VariableDeclarationContext::identifier() {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(0);
}

ECMAScriptParser::SingleExpressionContext* ECMAScriptParser::VariableDeclarationContext::singleExpression() {
  return getRuleContext<ECMAScriptParser::SingleExpressionContext>(0);
}


size_t ECMAScriptParser::VariableDeclarationContext::getRuleIndex() const {
  return ECMAScriptParser::RuleVariableDeclaration;
}

void ECMAScriptParser::VariableDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVariableDeclaration(this);
}

void ECMAScriptParser::VariableDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVariableDeclaration(this);
}

ECMAScriptParser::VariableDeclarationContext* ECMAScriptParser::variableDeclaration() {
  VariableDeclarationContext *_localctx = _tracker.createInstance<VariableDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 40, ECMAScriptParser::RuleVariableDeclaration);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(254);
    identifier();
    setState(257);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx)) {
    case 1: {
      setState(255);
      match(ECMAScriptParser::T__9);
      setState(256);
      singleExpression(0);
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

//----------------- EmptyStatementContext ------------------------------------------------------------------

ECMAScriptParser::EmptyStatementContext::EmptyStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ECMAScriptParser::EmptyStatementContext::getRuleIndex() const {
  return ECMAScriptParser::RuleEmptyStatement;
}

void ECMAScriptParser::EmptyStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEmptyStatement(this);
}

void ECMAScriptParser::EmptyStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEmptyStatement(this);
}

ECMAScriptParser::EmptyStatementContext* ECMAScriptParser::emptyStatement() {
  EmptyStatementContext *_localctx = _tracker.createInstance<EmptyStatementContext>(_ctx, getState());
  enterRule(_localctx, 42, ECMAScriptParser::RuleEmptyStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(259);
    match(ECMAScriptParser::T__13);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionStatementContext ------------------------------------------------------------------

ECMAScriptParser::ExpressionStatementContext::ExpressionStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::ExpressionSequenceContext* ECMAScriptParser::ExpressionStatementContext::expressionSequence() {
  return getRuleContext<ECMAScriptParser::ExpressionSequenceContext>(0);
}

ECMAScriptParser::EosContext* ECMAScriptParser::ExpressionStatementContext::eos() {
  return getRuleContext<ECMAScriptParser::EosContext>(0);
}


size_t ECMAScriptParser::ExpressionStatementContext::getRuleIndex() const {
  return ECMAScriptParser::RuleExpressionStatement;
}

void ECMAScriptParser::ExpressionStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpressionStatement(this);
}

void ECMAScriptParser::ExpressionStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpressionStatement(this);
}

ECMAScriptParser::ExpressionStatementContext* ECMAScriptParser::expressionStatement() {
  ExpressionStatementContext *_localctx = _tracker.createInstance<ExpressionStatementContext>(_ctx, getState());
  enterRule(_localctx, 44, ECMAScriptParser::RuleExpressionStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(261);
    expressionSequence();
    setState(262);
    eos();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IfStatementContext ------------------------------------------------------------------

ECMAScriptParser::IfStatementContext::IfStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::ExpressionSequenceContext* ECMAScriptParser::IfStatementContext::expressionSequence() {
  return getRuleContext<ECMAScriptParser::ExpressionSequenceContext>(0);
}

std::vector<ECMAScriptParser::StatementContext *> ECMAScriptParser::IfStatementContext::statement() {
  return getRuleContexts<ECMAScriptParser::StatementContext>();
}

ECMAScriptParser::StatementContext* ECMAScriptParser::IfStatementContext::statement(size_t i) {
  return getRuleContext<ECMAScriptParser::StatementContext>(i);
}


size_t ECMAScriptParser::IfStatementContext::getRuleIndex() const {
  return ECMAScriptParser::RuleIfStatement;
}

void ECMAScriptParser::IfStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIfStatement(this);
}

void ECMAScriptParser::IfStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIfStatement(this);
}

ECMAScriptParser::IfStatementContext* ECMAScriptParser::ifStatement() {
  IfStatementContext *_localctx = _tracker.createInstance<IfStatementContext>(_ctx, getState());
  enterRule(_localctx, 46, ECMAScriptParser::RuleIfStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(264);
    match(ECMAScriptParser::T__20);
    setState(265);
    match(ECMAScriptParser::T__4);
    setState(266);
    expressionSequence();
    setState(267);
    match(ECMAScriptParser::T__5);
    setState(268);
    statement();
    setState(271);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 9, _ctx)) {
    case 1: {
      setState(269);
      match(ECMAScriptParser::T__21);
      setState(270);
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

ECMAScriptParser::IterationStatementContext::IterationStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::StatementContext* ECMAScriptParser::IterationStatementContext::statement() {
  return getRuleContext<ECMAScriptParser::StatementContext>(0);
}

ECMAScriptParser::ExpressionSequenceContext* ECMAScriptParser::IterationStatementContext::expressionSequence() {
  return getRuleContext<ECMAScriptParser::ExpressionSequenceContext>(0);
}

ECMAScriptParser::EosContext* ECMAScriptParser::IterationStatementContext::eos() {
  return getRuleContext<ECMAScriptParser::EosContext>(0);
}

ECMAScriptParser::ForControlContext* ECMAScriptParser::IterationStatementContext::forControl() {
  return getRuleContext<ECMAScriptParser::ForControlContext>(0);
}


size_t ECMAScriptParser::IterationStatementContext::getRuleIndex() const {
  return ECMAScriptParser::RuleIterationStatement;
}

void ECMAScriptParser::IterationStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIterationStatement(this);
}

void ECMAScriptParser::IterationStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIterationStatement(this);
}

ECMAScriptParser::IterationStatementContext* ECMAScriptParser::iterationStatement() {
  IterationStatementContext *_localctx = _tracker.createInstance<IterationStatementContext>(_ctx, getState());
  enterRule(_localctx, 48, ECMAScriptParser::RuleIterationStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(293);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ECMAScriptParser::T__22: {
        enterOuterAlt(_localctx, 1);
        setState(273);
        match(ECMAScriptParser::T__22);
        setState(274);
        statement();
        setState(275);
        match(ECMAScriptParser::T__23);
        setState(276);
        match(ECMAScriptParser::T__4);
        setState(277);
        expressionSequence();
        setState(278);
        match(ECMAScriptParser::T__5);
        setState(279);
        eos();
        break;
      }

      case ECMAScriptParser::T__23: {
        enterOuterAlt(_localctx, 2);
        setState(281);
        match(ECMAScriptParser::T__23);
        setState(282);
        match(ECMAScriptParser::T__4);
        setState(283);
        expressionSequence();
        setState(284);
        match(ECMAScriptParser::T__5);
        setState(285);
        statement();
        break;
      }

      case ECMAScriptParser::T__24: {
        enterOuterAlt(_localctx, 3);
        setState(287);
        match(ECMAScriptParser::T__24);
        setState(288);
        match(ECMAScriptParser::T__4);
        setState(289);
        forControl();
        setState(290);
        match(ECMAScriptParser::T__5);
        setState(291);
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

//----------------- ForControlContext ------------------------------------------------------------------

ECMAScriptParser::ForControlContext::ForControlContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ECMAScriptParser::ExpressionSequenceContext *> ECMAScriptParser::ForControlContext::expressionSequence() {
  return getRuleContexts<ECMAScriptParser::ExpressionSequenceContext>();
}

ECMAScriptParser::ExpressionSequenceContext* ECMAScriptParser::ForControlContext::expressionSequence(size_t i) {
  return getRuleContext<ECMAScriptParser::ExpressionSequenceContext>(i);
}

ECMAScriptParser::VariableDeclarationListContext* ECMAScriptParser::ForControlContext::variableDeclarationList() {
  return getRuleContext<ECMAScriptParser::VariableDeclarationListContext>(0);
}

ECMAScriptParser::SingleExpressionContext* ECMAScriptParser::ForControlContext::singleExpression() {
  return getRuleContext<ECMAScriptParser::SingleExpressionContext>(0);
}

ECMAScriptParser::VariableDeclarationContext* ECMAScriptParser::ForControlContext::variableDeclaration() {
  return getRuleContext<ECMAScriptParser::VariableDeclarationContext>(0);
}


size_t ECMAScriptParser::ForControlContext::getRuleIndex() const {
  return ECMAScriptParser::RuleForControl;
}

void ECMAScriptParser::ForControlContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForControl(this);
}

void ECMAScriptParser::ForControlContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForControl(this);
}

ECMAScriptParser::ForControlContext* ECMAScriptParser::forControl() {
  ForControlContext *_localctx = _tracker.createInstance<ForControlContext>(_ctx, getState());
  enterRule(_localctx, 50, ECMAScriptParser::RuleForControl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(334);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 16, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(296);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 18005585235444146) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & 545179828225) != 0)) {
        setState(295);
        expressionSequence();
      }
      setState(298);
      match(ECMAScriptParser::T__13);
      setState(300);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 18005585235444146) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & 545179828225) != 0)) {
        setState(299);
        expressionSequence();
      }
      setState(302);
      match(ECMAScriptParser::T__13);
      setState(304);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 18005585235444146) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & 545179828225) != 0)) {
        setState(303);
        expressionSequence();
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(306);
      match(ECMAScriptParser::T__16);
      setState(307);
      variableDeclarationList();
      setState(308);
      match(ECMAScriptParser::T__13);
      setState(310);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 18005585235444146) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & 545179828225) != 0)) {
        setState(309);
        expressionSequence();
      }
      setState(312);
      match(ECMAScriptParser::T__13);
      setState(314);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 18005585235444146) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & 545179828225) != 0)) {
        setState(313);
        expressionSequence();
      }
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(316);
      singleExpression(0);
      setState(317);
      match(ECMAScriptParser::T__25);
      setState(318);
      expressionSequence();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(320);
      match(ECMAScriptParser::T__16);
      setState(321);
      variableDeclaration();
      setState(322);
      match(ECMAScriptParser::T__25);
      setState(323);
      expressionSequence();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(325);
      singleExpression(0);
      setState(326);
      match(ECMAScriptParser::T__26);
      setState(327);
      expressionSequence();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(329);
      match(ECMAScriptParser::T__16);
      setState(330);
      variableDeclaration();
      setState(331);
      match(ECMAScriptParser::T__26);
      setState(332);
      expressionSequence();
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

ECMAScriptParser::ContinueStatementContext::ContinueStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::EosContext* ECMAScriptParser::ContinueStatementContext::eos() {
  return getRuleContext<ECMAScriptParser::EosContext>(0);
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::ContinueStatementContext::identifier() {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(0);
}


size_t ECMAScriptParser::ContinueStatementContext::getRuleIndex() const {
  return ECMAScriptParser::RuleContinueStatement;
}

void ECMAScriptParser::ContinueStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterContinueStatement(this);
}

void ECMAScriptParser::ContinueStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitContinueStatement(this);
}

ECMAScriptParser::ContinueStatementContext* ECMAScriptParser::continueStatement() {
  ContinueStatementContext *_localctx = _tracker.createInstance<ContinueStatementContext>(_ctx, getState());
  enterRule(_localctx, 52, ECMAScriptParser::RuleContinueStatement);

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
    match(ECMAScriptParser::T__27);
    setState(338);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 17, _ctx)) {
    case 1: {
      setState(337);
      identifier();
      break;
    }

    default:
      break;
    }
    setState(340);
    eos();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BreakStatementContext ------------------------------------------------------------------

ECMAScriptParser::BreakStatementContext::BreakStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::EosContext* ECMAScriptParser::BreakStatementContext::eos() {
  return getRuleContext<ECMAScriptParser::EosContext>(0);
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::BreakStatementContext::identifier() {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(0);
}


size_t ECMAScriptParser::BreakStatementContext::getRuleIndex() const {
  return ECMAScriptParser::RuleBreakStatement;
}

void ECMAScriptParser::BreakStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBreakStatement(this);
}

void ECMAScriptParser::BreakStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBreakStatement(this);
}

ECMAScriptParser::BreakStatementContext* ECMAScriptParser::breakStatement() {
  BreakStatementContext *_localctx = _tracker.createInstance<BreakStatementContext>(_ctx, getState());
  enterRule(_localctx, 54, ECMAScriptParser::RuleBreakStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(342);
    match(ECMAScriptParser::T__28);
    setState(344);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx)) {
    case 1: {
      setState(343);
      identifier();
      break;
    }

    default:
      break;
    }
    setState(346);
    eos();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnStatementContext ------------------------------------------------------------------

ECMAScriptParser::ReturnStatementContext::ReturnStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::EosContext* ECMAScriptParser::ReturnStatementContext::eos() {
  return getRuleContext<ECMAScriptParser::EosContext>(0);
}

ECMAScriptParser::ExpressionSequenceContext* ECMAScriptParser::ReturnStatementContext::expressionSequence() {
  return getRuleContext<ECMAScriptParser::ExpressionSequenceContext>(0);
}


size_t ECMAScriptParser::ReturnStatementContext::getRuleIndex() const {
  return ECMAScriptParser::RuleReturnStatement;
}

void ECMAScriptParser::ReturnStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterReturnStatement(this);
}

void ECMAScriptParser::ReturnStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitReturnStatement(this);
}

ECMAScriptParser::ReturnStatementContext* ECMAScriptParser::returnStatement() {
  ReturnStatementContext *_localctx = _tracker.createInstance<ReturnStatementContext>(_ctx, getState());
  enterRule(_localctx, 56, ECMAScriptParser::RuleReturnStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(348);
    match(ECMAScriptParser::T__29);
    setState(350);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx)) {
    case 1: {
      setState(349);
      expressionSequence();
      break;
    }

    default:
      break;
    }
    setState(352);
    eos();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WithStatementContext ------------------------------------------------------------------

ECMAScriptParser::WithStatementContext::WithStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::ExpressionSequenceContext* ECMAScriptParser::WithStatementContext::expressionSequence() {
  return getRuleContext<ECMAScriptParser::ExpressionSequenceContext>(0);
}

ECMAScriptParser::StatementContext* ECMAScriptParser::WithStatementContext::statement() {
  return getRuleContext<ECMAScriptParser::StatementContext>(0);
}


size_t ECMAScriptParser::WithStatementContext::getRuleIndex() const {
  return ECMAScriptParser::RuleWithStatement;
}

void ECMAScriptParser::WithStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterWithStatement(this);
}

void ECMAScriptParser::WithStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitWithStatement(this);
}

ECMAScriptParser::WithStatementContext* ECMAScriptParser::withStatement() {
  WithStatementContext *_localctx = _tracker.createInstance<WithStatementContext>(_ctx, getState());
  enterRule(_localctx, 58, ECMAScriptParser::RuleWithStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(354);
    match(ECMAScriptParser::T__30);
    setState(355);
    match(ECMAScriptParser::T__4);
    setState(356);
    expressionSequence();
    setState(357);
    match(ECMAScriptParser::T__5);
    setState(358);
    statement();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SwitchStatementContext ------------------------------------------------------------------

ECMAScriptParser::SwitchStatementContext::SwitchStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::ExpressionSequenceContext* ECMAScriptParser::SwitchStatementContext::expressionSequence() {
  return getRuleContext<ECMAScriptParser::ExpressionSequenceContext>(0);
}

ECMAScriptParser::CaseBlockContext* ECMAScriptParser::SwitchStatementContext::caseBlock() {
  return getRuleContext<ECMAScriptParser::CaseBlockContext>(0);
}


size_t ECMAScriptParser::SwitchStatementContext::getRuleIndex() const {
  return ECMAScriptParser::RuleSwitchStatement;
}

void ECMAScriptParser::SwitchStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSwitchStatement(this);
}

void ECMAScriptParser::SwitchStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSwitchStatement(this);
}

ECMAScriptParser::SwitchStatementContext* ECMAScriptParser::switchStatement() {
  SwitchStatementContext *_localctx = _tracker.createInstance<SwitchStatementContext>(_ctx, getState());
  enterRule(_localctx, 60, ECMAScriptParser::RuleSwitchStatement);

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
    match(ECMAScriptParser::T__31);
    setState(361);
    match(ECMAScriptParser::T__4);
    setState(362);
    expressionSequence();
    setState(363);
    match(ECMAScriptParser::T__5);
    setState(364);
    caseBlock();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CaseBlockContext ------------------------------------------------------------------

ECMAScriptParser::CaseBlockContext::CaseBlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::CaseClausesContext* ECMAScriptParser::CaseBlockContext::caseClauses() {
  return getRuleContext<ECMAScriptParser::CaseClausesContext>(0);
}


size_t ECMAScriptParser::CaseBlockContext::getRuleIndex() const {
  return ECMAScriptParser::RuleCaseBlock;
}

void ECMAScriptParser::CaseBlockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCaseBlock(this);
}

void ECMAScriptParser::CaseBlockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCaseBlock(this);
}

ECMAScriptParser::CaseBlockContext* ECMAScriptParser::caseBlock() {
  CaseBlockContext *_localctx = _tracker.createInstance<CaseBlockContext>(_ctx, getState());
  enterRule(_localctx, 62, ECMAScriptParser::RuleCaseBlock);
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
    setState(366);
    match(ECMAScriptParser::T__14);
    setState(368);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ECMAScriptParser::T__32

    || _la == ECMAScriptParser::T__34) {
      setState(367);
      caseClauses();
    }
    setState(370);
    match(ECMAScriptParser::T__15);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CaseClausesContext ------------------------------------------------------------------

ECMAScriptParser::CaseClausesContext::CaseClausesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ECMAScriptParser::CaseClauseContext *> ECMAScriptParser::CaseClausesContext::caseClause() {
  return getRuleContexts<ECMAScriptParser::CaseClauseContext>();
}

ECMAScriptParser::CaseClauseContext* ECMAScriptParser::CaseClausesContext::caseClause(size_t i) {
  return getRuleContext<ECMAScriptParser::CaseClauseContext>(i);
}


size_t ECMAScriptParser::CaseClausesContext::getRuleIndex() const {
  return ECMAScriptParser::RuleCaseClauses;
}

void ECMAScriptParser::CaseClausesContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCaseClauses(this);
}

void ECMAScriptParser::CaseClausesContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCaseClauses(this);
}

ECMAScriptParser::CaseClausesContext* ECMAScriptParser::caseClauses() {
  CaseClausesContext *_localctx = _tracker.createInstance<CaseClausesContext>(_ctx, getState());
  enterRule(_localctx, 64, ECMAScriptParser::RuleCaseClauses);
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
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(372);
      caseClause();
      setState(375); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == ECMAScriptParser::T__32

    || _la == ECMAScriptParser::T__34);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CaseClauseContext ------------------------------------------------------------------

ECMAScriptParser::CaseClauseContext::CaseClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::ExpressionSequenceContext* ECMAScriptParser::CaseClauseContext::expressionSequence() {
  return getRuleContext<ECMAScriptParser::ExpressionSequenceContext>(0);
}

ECMAScriptParser::StatementListContext* ECMAScriptParser::CaseClauseContext::statementList() {
  return getRuleContext<ECMAScriptParser::StatementListContext>(0);
}


size_t ECMAScriptParser::CaseClauseContext::getRuleIndex() const {
  return ECMAScriptParser::RuleCaseClause;
}

void ECMAScriptParser::CaseClauseContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCaseClause(this);
}

void ECMAScriptParser::CaseClauseContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCaseClause(this);
}

ECMAScriptParser::CaseClauseContext* ECMAScriptParser::caseClause() {
  CaseClauseContext *_localctx = _tracker.createInstance<CaseClauseContext>(_ctx, getState());
  enterRule(_localctx, 66, ECMAScriptParser::RuleCaseClause);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(388);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ECMAScriptParser::T__32: {
        enterOuterAlt(_localctx, 1);
        setState(377);
        match(ECMAScriptParser::T__32);
        setState(378);
        expressionSequence();
        setState(379);
        match(ECMAScriptParser::T__33);
        setState(381);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx)) {
        case 1: {
          setState(380);
          statementList();
          break;
        }

        default:
          break;
        }
        break;
      }

      case ECMAScriptParser::T__34: {
        enterOuterAlt(_localctx, 2);
        setState(383);
        match(ECMAScriptParser::T__34);
        setState(384);
        match(ECMAScriptParser::T__33);
        setState(386);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
        case 1: {
          setState(385);
          statementList();
          break;
        }

        default:
          break;
        }
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

//----------------- ThrowStatementContext ------------------------------------------------------------------

ECMAScriptParser::ThrowStatementContext::ThrowStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::ExpressionSequenceContext* ECMAScriptParser::ThrowStatementContext::expressionSequence() {
  return getRuleContext<ECMAScriptParser::ExpressionSequenceContext>(0);
}

ECMAScriptParser::EosContext* ECMAScriptParser::ThrowStatementContext::eos() {
  return getRuleContext<ECMAScriptParser::EosContext>(0);
}


size_t ECMAScriptParser::ThrowStatementContext::getRuleIndex() const {
  return ECMAScriptParser::RuleThrowStatement;
}

void ECMAScriptParser::ThrowStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterThrowStatement(this);
}

void ECMAScriptParser::ThrowStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitThrowStatement(this);
}

ECMAScriptParser::ThrowStatementContext* ECMAScriptParser::throwStatement() {
  ThrowStatementContext *_localctx = _tracker.createInstance<ThrowStatementContext>(_ctx, getState());
  enterRule(_localctx, 68, ECMAScriptParser::RuleThrowStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(390);
    match(ECMAScriptParser::T__35);
    setState(391);
    expressionSequence();
    setState(392);
    eos();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TryStatementContext ------------------------------------------------------------------

ECMAScriptParser::TryStatementContext::TryStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::BlockContext* ECMAScriptParser::TryStatementContext::block() {
  return getRuleContext<ECMAScriptParser::BlockContext>(0);
}

ECMAScriptParser::CatchProductionContext* ECMAScriptParser::TryStatementContext::catchProduction() {
  return getRuleContext<ECMAScriptParser::CatchProductionContext>(0);
}

ECMAScriptParser::FinallyProductionContext* ECMAScriptParser::TryStatementContext::finallyProduction() {
  return getRuleContext<ECMAScriptParser::FinallyProductionContext>(0);
}


size_t ECMAScriptParser::TryStatementContext::getRuleIndex() const {
  return ECMAScriptParser::RuleTryStatement;
}

void ECMAScriptParser::TryStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTryStatement(this);
}

void ECMAScriptParser::TryStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTryStatement(this);
}

ECMAScriptParser::TryStatementContext* ECMAScriptParser::tryStatement() {
  TryStatementContext *_localctx = _tracker.createInstance<TryStatementContext>(_ctx, getState());
  enterRule(_localctx, 70, ECMAScriptParser::RuleTryStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(394);
    match(ECMAScriptParser::T__36);
    setState(395);
    block();
    setState(401);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ECMAScriptParser::T__37: {
        setState(396);
        catchProduction();
        setState(398);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx)) {
        case 1: {
          setState(397);
          finallyProduction();
          break;
        }

        default:
          break;
        }
        break;
      }

      case ECMAScriptParser::T__38: {
        setState(400);
        finallyProduction();
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

//----------------- CatchProductionContext ------------------------------------------------------------------

ECMAScriptParser::CatchProductionContext::CatchProductionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::CatchProductionContext::identifier() {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(0);
}

ECMAScriptParser::BlockContext* ECMAScriptParser::CatchProductionContext::block() {
  return getRuleContext<ECMAScriptParser::BlockContext>(0);
}


size_t ECMAScriptParser::CatchProductionContext::getRuleIndex() const {
  return ECMAScriptParser::RuleCatchProduction;
}

void ECMAScriptParser::CatchProductionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCatchProduction(this);
}

void ECMAScriptParser::CatchProductionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCatchProduction(this);
}

ECMAScriptParser::CatchProductionContext* ECMAScriptParser::catchProduction() {
  CatchProductionContext *_localctx = _tracker.createInstance<CatchProductionContext>(_ctx, getState());
  enterRule(_localctx, 72, ECMAScriptParser::RuleCatchProduction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(403);
    match(ECMAScriptParser::T__37);
    setState(404);
    match(ECMAScriptParser::T__4);
    setState(405);
    identifier();
    setState(406);
    match(ECMAScriptParser::T__5);
    setState(407);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FinallyProductionContext ------------------------------------------------------------------

ECMAScriptParser::FinallyProductionContext::FinallyProductionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::BlockContext* ECMAScriptParser::FinallyProductionContext::block() {
  return getRuleContext<ECMAScriptParser::BlockContext>(0);
}


size_t ECMAScriptParser::FinallyProductionContext::getRuleIndex() const {
  return ECMAScriptParser::RuleFinallyProduction;
}

void ECMAScriptParser::FinallyProductionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFinallyProduction(this);
}

void ECMAScriptParser::FinallyProductionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFinallyProduction(this);
}

ECMAScriptParser::FinallyProductionContext* ECMAScriptParser::finallyProduction() {
  FinallyProductionContext *_localctx = _tracker.createInstance<FinallyProductionContext>(_ctx, getState());
  enterRule(_localctx, 74, ECMAScriptParser::RuleFinallyProduction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(409);
    match(ECMAScriptParser::T__38);
    setState(410);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DebuggerStatementContext ------------------------------------------------------------------

ECMAScriptParser::DebuggerStatementContext::DebuggerStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::EosContext* ECMAScriptParser::DebuggerStatementContext::eos() {
  return getRuleContext<ECMAScriptParser::EosContext>(0);
}


size_t ECMAScriptParser::DebuggerStatementContext::getRuleIndex() const {
  return ECMAScriptParser::RuleDebuggerStatement;
}

void ECMAScriptParser::DebuggerStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDebuggerStatement(this);
}

void ECMAScriptParser::DebuggerStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDebuggerStatement(this);
}

ECMAScriptParser::DebuggerStatementContext* ECMAScriptParser::debuggerStatement() {
  DebuggerStatementContext *_localctx = _tracker.createInstance<DebuggerStatementContext>(_ctx, getState());
  enterRule(_localctx, 76, ECMAScriptParser::RuleDebuggerStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(412);
    match(ECMAScriptParser::T__39);
    setState(413);
    eos();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LabelledStatementContext ------------------------------------------------------------------

ECMAScriptParser::LabelledStatementContext::LabelledStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::LabelledStatementContext::identifier() {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(0);
}

ECMAScriptParser::StatementContext* ECMAScriptParser::LabelledStatementContext::statement() {
  return getRuleContext<ECMAScriptParser::StatementContext>(0);
}


size_t ECMAScriptParser::LabelledStatementContext::getRuleIndex() const {
  return ECMAScriptParser::RuleLabelledStatement;
}

void ECMAScriptParser::LabelledStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLabelledStatement(this);
}

void ECMAScriptParser::LabelledStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLabelledStatement(this);
}

ECMAScriptParser::LabelledStatementContext* ECMAScriptParser::labelledStatement() {
  LabelledStatementContext *_localctx = _tracker.createInstance<LabelledStatementContext>(_ctx, getState());
  enterRule(_localctx, 78, ECMAScriptParser::RuleLabelledStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(415);
    identifier();
    setState(416);
    match(ECMAScriptParser::T__33);
    setState(417);
    statement();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionDeclarationContext ------------------------------------------------------------------

ECMAScriptParser::FunctionDeclarationContext::FunctionDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::FunctionDeclarationContext::identifier() {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(0);
}

ECMAScriptParser::FunctionBodyContext* ECMAScriptParser::FunctionDeclarationContext::functionBody() {
  return getRuleContext<ECMAScriptParser::FunctionBodyContext>(0);
}

ECMAScriptParser::FormalParameterListContext* ECMAScriptParser::FunctionDeclarationContext::formalParameterList() {
  return getRuleContext<ECMAScriptParser::FormalParameterListContext>(0);
}


size_t ECMAScriptParser::FunctionDeclarationContext::getRuleIndex() const {
  return ECMAScriptParser::RuleFunctionDeclaration;
}

void ECMAScriptParser::FunctionDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionDeclaration(this);
}

void ECMAScriptParser::FunctionDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionDeclaration(this);
}

ECMAScriptParser::FunctionDeclarationContext* ECMAScriptParser::functionDeclaration() {
  FunctionDeclarationContext *_localctx = _tracker.createInstance<FunctionDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 80, ECMAScriptParser::RuleFunctionDeclaration);
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
    setState(419);
    match(ECMAScriptParser::T__40);
    setState(420);
    identifier();
    setState(421);
    match(ECMAScriptParser::T__4);
    setState(423);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 1007135469993984) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 276610154497) != 0)) {
      setState(422);
      formalParameterList();
    }
    setState(425);
    match(ECMAScriptParser::T__5);
    setState(426);
    match(ECMAScriptParser::T__14);
    setState(427);
    functionBody();
    setState(428);
    match(ECMAScriptParser::T__15);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FormalParameterListContext ------------------------------------------------------------------

ECMAScriptParser::FormalParameterListContext::FormalParameterListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ECMAScriptParser::IdentifierContext *> ECMAScriptParser::FormalParameterListContext::identifier() {
  return getRuleContexts<ECMAScriptParser::IdentifierContext>();
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::FormalParameterListContext::identifier(size_t i) {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(i);
}


size_t ECMAScriptParser::FormalParameterListContext::getRuleIndex() const {
  return ECMAScriptParser::RuleFormalParameterList;
}

void ECMAScriptParser::FormalParameterListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFormalParameterList(this);
}

void ECMAScriptParser::FormalParameterListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFormalParameterList(this);
}

ECMAScriptParser::FormalParameterListContext* ECMAScriptParser::formalParameterList() {
  FormalParameterListContext *_localctx = _tracker.createInstance<FormalParameterListContext>(_ctx, getState());
  enterRule(_localctx, 82, ECMAScriptParser::RuleFormalParameterList);
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
    setState(430);
    identifier();
    setState(435);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ECMAScriptParser::T__19) {
      setState(431);
      match(ECMAScriptParser::T__19);
      setState(432);
      identifier();
      setState(437);
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

ECMAScriptParser::FunctionBodyContext::FunctionBodyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::SourceElementsContext* ECMAScriptParser::FunctionBodyContext::sourceElements() {
  return getRuleContext<ECMAScriptParser::SourceElementsContext>(0);
}


size_t ECMAScriptParser::FunctionBodyContext::getRuleIndex() const {
  return ECMAScriptParser::RuleFunctionBody;
}

void ECMAScriptParser::FunctionBodyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionBody(this);
}

void ECMAScriptParser::FunctionBodyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionBody(this);
}

ECMAScriptParser::FunctionBodyContext* ECMAScriptParser::functionBody() {
  FunctionBodyContext *_localctx = _tracker.createInstance<FunctionBodyContext>(_ctx, getState());
  enterRule(_localctx, 84, ECMAScriptParser::RuleFunctionBody);
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
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 18005585235465138) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 545179828225) != 0)) {
      setState(438);
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

//----------------- ExpressionSequenceContext ------------------------------------------------------------------

ECMAScriptParser::ExpressionSequenceContext::ExpressionSequenceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ECMAScriptParser::SingleExpressionContext *> ECMAScriptParser::ExpressionSequenceContext::singleExpression() {
  return getRuleContexts<ECMAScriptParser::SingleExpressionContext>();
}

ECMAScriptParser::SingleExpressionContext* ECMAScriptParser::ExpressionSequenceContext::singleExpression(size_t i) {
  return getRuleContext<ECMAScriptParser::SingleExpressionContext>(i);
}


size_t ECMAScriptParser::ExpressionSequenceContext::getRuleIndex() const {
  return ECMAScriptParser::RuleExpressionSequence;
}

void ECMAScriptParser::ExpressionSequenceContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpressionSequence(this);
}

void ECMAScriptParser::ExpressionSequenceContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpressionSequence(this);
}

ECMAScriptParser::ExpressionSequenceContext* ECMAScriptParser::expressionSequence() {
  ExpressionSequenceContext *_localctx = _tracker.createInstance<ExpressionSequenceContext>(_ctx, getState());
  enterRule(_localctx, 86, ECMAScriptParser::RuleExpressionSequence);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(441);
    singleExpression(0);
    setState(446);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(442);
        match(ECMAScriptParser::T__19);
        setState(443);
        singleExpression(0); 
      }
      setState(448);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SingleExpressionContext ------------------------------------------------------------------

ECMAScriptParser::SingleExpressionContext::SingleExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::FunctionExpressionContext* ECMAScriptParser::SingleExpressionContext::functionExpression() {
  return getRuleContext<ECMAScriptParser::FunctionExpressionContext>(0);
}

ECMAScriptParser::ArrowFunctionExpressionContext* ECMAScriptParser::SingleExpressionContext::arrowFunctionExpression() {
  return getRuleContext<ECMAScriptParser::ArrowFunctionExpressionContext>(0);
}

std::vector<ECMAScriptParser::SingleExpressionContext *> ECMAScriptParser::SingleExpressionContext::singleExpression() {
  return getRuleContexts<ECMAScriptParser::SingleExpressionContext>();
}

ECMAScriptParser::SingleExpressionContext* ECMAScriptParser::SingleExpressionContext::singleExpression(size_t i) {
  return getRuleContext<ECMAScriptParser::SingleExpressionContext>(i);
}

ECMAScriptParser::ArgumentsContext* ECMAScriptParser::SingleExpressionContext::arguments() {
  return getRuleContext<ECMAScriptParser::ArgumentsContext>(0);
}

ECMAScriptParser::PrimaryExpressionContext* ECMAScriptParser::SingleExpressionContext::primaryExpression() {
  return getRuleContext<ECMAScriptParser::PrimaryExpressionContext>(0);
}

ECMAScriptParser::ChtlArrowAccessContext* ECMAScriptParser::SingleExpressionContext::chtlArrowAccess() {
  return getRuleContext<ECMAScriptParser::ChtlArrowAccessContext>(0);
}

ECMAScriptParser::AssignmentOperatorContext* ECMAScriptParser::SingleExpressionContext::assignmentOperator() {
  return getRuleContext<ECMAScriptParser::AssignmentOperatorContext>(0);
}

ECMAScriptParser::ExpressionSequenceContext* ECMAScriptParser::SingleExpressionContext::expressionSequence() {
  return getRuleContext<ECMAScriptParser::ExpressionSequenceContext>(0);
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::SingleExpressionContext::identifier() {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(0);
}


size_t ECMAScriptParser::SingleExpressionContext::getRuleIndex() const {
  return ECMAScriptParser::RuleSingleExpression;
}

void ECMAScriptParser::SingleExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSingleExpression(this);
}

void ECMAScriptParser::SingleExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSingleExpression(this);
}


ECMAScriptParser::SingleExpressionContext* ECMAScriptParser::singleExpression() {
   return singleExpression(0);
}

ECMAScriptParser::SingleExpressionContext* ECMAScriptParser::singleExpression(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  ECMAScriptParser::SingleExpressionContext *_localctx = _tracker.createInstance<SingleExpressionContext>(_ctx, parentState);
  ECMAScriptParser::SingleExpressionContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 88;
  enterRecursionRule(_localctx, 88, ECMAScriptParser::RuleSingleExpression, precedence);

    size_t _la = 0;

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
    setState(477);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx)) {
    case 1: {
      setState(450);
      functionExpression();
      break;
    }

    case 2: {
      setState(451);
      arrowFunctionExpression();
      break;
    }

    case 3: {
      setState(452);
      match(ECMAScriptParser::T__43);
      setState(453);
      singleExpression(0);
      setState(455);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx)) {
      case 1: {
        setState(454);
        arguments();
        break;
      }

      default:
        break;
      }
      break;
    }

    case 4: {
      setState(457);
      match(ECMAScriptParser::T__46);
      setState(458);
      singleExpression(26);
      break;
    }

    case 5: {
      setState(459);
      match(ECMAScriptParser::T__47);
      setState(460);
      singleExpression(25);
      break;
    }

    case 6: {
      setState(461);
      match(ECMAScriptParser::T__48);
      setState(462);
      singleExpression(24);
      break;
    }

    case 7: {
      setState(463);
      match(ECMAScriptParser::T__44);
      setState(464);
      singleExpression(23);
      break;
    }

    case 8: {
      setState(465);
      match(ECMAScriptParser::T__45);
      setState(466);
      singleExpression(22);
      break;
    }

    case 9: {
      setState(467);
      match(ECMAScriptParser::T__49);
      setState(468);
      singleExpression(21);
      break;
    }

    case 10: {
      setState(469);
      match(ECMAScriptParser::T__50);
      setState(470);
      singleExpression(20);
      break;
    }

    case 11: {
      setState(471);
      match(ECMAScriptParser::T__51);
      setState(472);
      singleExpression(19);
      break;
    }

    case 12: {
      setState(473);
      match(ECMAScriptParser::T__52);
      setState(474);
      singleExpression(18);
      break;
    }

    case 13: {
      setState(475);
      primaryExpression();
      break;
    }

    case 14: {
      setState(476);
      chtlArrowAccess();
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(544);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(542);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx)) {
        case 1: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(479);

          if (!(precpred(_ctx, 17))) throw FailedPredicateException(this, "precpred(_ctx, 17)");
          setState(480);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 126100789566373888) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(481);
          singleExpression(18);
          break;
        }

        case 2: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(482);

          if (!(precpred(_ctx, 16))) throw FailedPredicateException(this, "precpred(_ctx, 16)");
          setState(483);
          _la = _input->LA(1);
          if (!(_la == ECMAScriptParser::T__49

          || _la == ECMAScriptParser::T__50)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(484);
          singleExpression(17);
          break;
        }

        case 3: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(485);

          if (!(precpred(_ctx, 15))) throw FailedPredicateException(this, "precpred(_ctx, 15)");
          setState(486);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 1008806316530991104) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(487);
          singleExpression(16);
          break;
        }

        case 4: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(488);

          if (!(precpred(_ctx, 14))) throw FailedPredicateException(this, "precpred(_ctx, 14)");
          setState(489);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & -1152921504606846976) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(490);
          singleExpression(15);
          break;
        }

        case 5: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(491);

          if (!(precpred(_ctx, 13))) throw FailedPredicateException(this, "precpred(_ctx, 13)");
          setState(492);
          match(ECMAScriptParser::T__63);
          setState(493);
          singleExpression(14);
          break;
        }

        case 6: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(494);

          if (!(precpred(_ctx, 12))) throw FailedPredicateException(this, "precpred(_ctx, 12)");
          setState(495);
          match(ECMAScriptParser::T__25);
          setState(496);
          singleExpression(13);
          break;
        }

        case 7: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(497);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(498);
          _la = _input->LA(1);
          if (!(((((_la - 65) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 65)) & 15) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(499);
          singleExpression(12);
          break;
        }

        case 8: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(500);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(501);
          match(ECMAScriptParser::T__68);
          setState(502);
          singleExpression(11);
          break;
        }

        case 9: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(503);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(504);
          match(ECMAScriptParser::T__69);
          setState(505);
          singleExpression(10);
          break;
        }

        case 10: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(506);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
          setState(507);
          match(ECMAScriptParser::T__70);
          setState(508);
          singleExpression(9);
          break;
        }

        case 11: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(509);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(510);
          match(ECMAScriptParser::T__71);
          setState(511);
          singleExpression(8);
          break;
        }

        case 12: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(512);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(513);
          match(ECMAScriptParser::T__72);
          setState(514);
          singleExpression(7);
          break;
        }

        case 13: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(515);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(516);
          match(ECMAScriptParser::T__73);
          setState(517);
          singleExpression(0);
          setState(518);
          match(ECMAScriptParser::T__33);
          setState(519);
          singleExpression(6);
          break;
        }

        case 14: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(521);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(522);
          match(ECMAScriptParser::T__9);
          setState(523);
          singleExpression(5);
          break;
        }

        case 15: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(524);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(525);
          assignmentOperator();
          setState(526);
          singleExpression(4);
          break;
        }

        case 16: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(528);

          if (!(precpred(_ctx, 32))) throw FailedPredicateException(this, "precpred(_ctx, 32)");
          setState(529);
          match(ECMAScriptParser::T__41);
          setState(530);
          expressionSequence();
          setState(531);
          match(ECMAScriptParser::T__42);
          break;
        }

        case 17: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(533);

          if (!(precpred(_ctx, 31))) throw FailedPredicateException(this, "precpred(_ctx, 31)");
          setState(534);
          match(ECMAScriptParser::T__10);
          setState(535);
          identifier();
          break;
        }

        case 18: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(536);

          if (!(precpred(_ctx, 30))) throw FailedPredicateException(this, "precpred(_ctx, 30)");
          setState(537);
          arguments();
          break;
        }

        case 19: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(538);

          if (!(precpred(_ctx, 28))) throw FailedPredicateException(this, "precpred(_ctx, 28)");
          setState(539);
          match(ECMAScriptParser::T__44);
          break;
        }

        case 20: {
          _localctx = _tracker.createInstance<SingleExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleSingleExpression);
          setState(540);

          if (!(precpred(_ctx, 27))) throw FailedPredicateException(this, "precpred(_ctx, 27)");
          setState(541);
          match(ECMAScriptParser::T__45);
          break;
        }

        default:
          break;
        } 
      }
      setState(546);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx);
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

ECMAScriptParser::AssignmentOperatorContext::AssignmentOperatorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ECMAScriptParser::AssignmentOperatorContext::getRuleIndex() const {
  return ECMAScriptParser::RuleAssignmentOperator;
}

void ECMAScriptParser::AssignmentOperatorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssignmentOperator(this);
}

void ECMAScriptParser::AssignmentOperatorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssignmentOperator(this);
}

ECMAScriptParser::AssignmentOperatorContext* ECMAScriptParser::assignmentOperator() {
  AssignmentOperatorContext *_localctx = _tracker.createInstance<AssignmentOperatorContext>(_ctx, getState());
  enterRule(_localctx, 90, ECMAScriptParser::RuleAssignmentOperator);
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
    setState(547);
    _la = _input->LA(1);
    if (!(((((_la - 75) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 75)) & 2047) != 0))) {
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

//----------------- PrimaryExpressionContext ------------------------------------------------------------------

ECMAScriptParser::PrimaryExpressionContext::PrimaryExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::PrimaryExpressionContext::identifier() {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(0);
}

ECMAScriptParser::LiteralContext* ECMAScriptParser::PrimaryExpressionContext::literal() {
  return getRuleContext<ECMAScriptParser::LiteralContext>(0);
}

ECMAScriptParser::ArrayLiteralContext* ECMAScriptParser::PrimaryExpressionContext::arrayLiteral() {
  return getRuleContext<ECMAScriptParser::ArrayLiteralContext>(0);
}

ECMAScriptParser::ObjectLiteralContext* ECMAScriptParser::PrimaryExpressionContext::objectLiteral() {
  return getRuleContext<ECMAScriptParser::ObjectLiteralContext>(0);
}

ECMAScriptParser::ExpressionSequenceContext* ECMAScriptParser::PrimaryExpressionContext::expressionSequence() {
  return getRuleContext<ECMAScriptParser::ExpressionSequenceContext>(0);
}

ECMAScriptParser::ChtlEnhancedSelectorContext* ECMAScriptParser::PrimaryExpressionContext::chtlEnhancedSelector() {
  return getRuleContext<ECMAScriptParser::ChtlEnhancedSelectorContext>(0);
}

ECMAScriptParser::ChtlListenFunctionContext* ECMAScriptParser::PrimaryExpressionContext::chtlListenFunction() {
  return getRuleContext<ECMAScriptParser::ChtlListenFunctionContext>(0);
}

ECMAScriptParser::ChtlDelegateFunctionContext* ECMAScriptParser::PrimaryExpressionContext::chtlDelegateFunction() {
  return getRuleContext<ECMAScriptParser::ChtlDelegateFunctionContext>(0);
}

ECMAScriptParser::ChtlAnimateFunctionContext* ECMAScriptParser::PrimaryExpressionContext::chtlAnimateFunction() {
  return getRuleContext<ECMAScriptParser::ChtlAnimateFunctionContext>(0);
}

ECMAScriptParser::ChtlVirAccessContext* ECMAScriptParser::PrimaryExpressionContext::chtlVirAccess() {
  return getRuleContext<ECMAScriptParser::ChtlVirAccessContext>(0);
}

ECMAScriptParser::ChtlVariableUsageContext* ECMAScriptParser::PrimaryExpressionContext::chtlVariableUsage() {
  return getRuleContext<ECMAScriptParser::ChtlVariableUsageContext>(0);
}


size_t ECMAScriptParser::PrimaryExpressionContext::getRuleIndex() const {
  return ECMAScriptParser::RulePrimaryExpression;
}

void ECMAScriptParser::PrimaryExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPrimaryExpression(this);
}

void ECMAScriptParser::PrimaryExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPrimaryExpression(this);
}

ECMAScriptParser::PrimaryExpressionContext* ECMAScriptParser::primaryExpression() {
  PrimaryExpressionContext *_localctx = _tracker.createInstance<PrimaryExpressionContext>(_ctx, getState());
  enterRule(_localctx, 92, ECMAScriptParser::RulePrimaryExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(564);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(549);
      match(ECMAScriptParser::T__85);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(550);
      identifier();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(551);
      literal();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(552);
      arrayLiteral();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(553);
      objectLiteral();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(554);
      match(ECMAScriptParser::T__4);
      setState(555);
      expressionSequence();
      setState(556);
      match(ECMAScriptParser::T__5);
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(558);
      chtlEnhancedSelector();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(559);
      chtlListenFunction();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(560);
      chtlDelegateFunction();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(561);
      chtlAnimateFunction();
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(562);
      chtlVirAccess();
      break;
    }

    case 12: {
      enterOuterAlt(_localctx, 12);
      setState(563);
      chtlVariableUsage();
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

//----------------- FunctionExpressionContext ------------------------------------------------------------------

ECMAScriptParser::FunctionExpressionContext::FunctionExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::FunctionBodyContext* ECMAScriptParser::FunctionExpressionContext::functionBody() {
  return getRuleContext<ECMAScriptParser::FunctionBodyContext>(0);
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::FunctionExpressionContext::identifier() {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(0);
}

ECMAScriptParser::FormalParameterListContext* ECMAScriptParser::FunctionExpressionContext::formalParameterList() {
  return getRuleContext<ECMAScriptParser::FormalParameterListContext>(0);
}


size_t ECMAScriptParser::FunctionExpressionContext::getRuleIndex() const {
  return ECMAScriptParser::RuleFunctionExpression;
}

void ECMAScriptParser::FunctionExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionExpression(this);
}

void ECMAScriptParser::FunctionExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionExpression(this);
}

ECMAScriptParser::FunctionExpressionContext* ECMAScriptParser::functionExpression() {
  FunctionExpressionContext *_localctx = _tracker.createInstance<FunctionExpressionContext>(_ctx, getState());
  enterRule(_localctx, 94, ECMAScriptParser::RuleFunctionExpression);
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
    setState(566);
    match(ECMAScriptParser::T__40);
    setState(568);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 1007135469993984) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 276610154497) != 0)) {
      setState(567);
      identifier();
    }
    setState(570);
    match(ECMAScriptParser::T__4);
    setState(572);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 1007135469993984) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 276610154497) != 0)) {
      setState(571);
      formalParameterList();
    }
    setState(574);
    match(ECMAScriptParser::T__5);
    setState(575);
    match(ECMAScriptParser::T__14);
    setState(576);
    functionBody();
    setState(577);
    match(ECMAScriptParser::T__15);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArrowFunctionExpressionContext ------------------------------------------------------------------

ECMAScriptParser::ArrowFunctionExpressionContext::ArrowFunctionExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::ArrowFunctionParametersContext* ECMAScriptParser::ArrowFunctionExpressionContext::arrowFunctionParameters() {
  return getRuleContext<ECMAScriptParser::ArrowFunctionParametersContext>(0);
}

ECMAScriptParser::ArrowFunctionBodyContext* ECMAScriptParser::ArrowFunctionExpressionContext::arrowFunctionBody() {
  return getRuleContext<ECMAScriptParser::ArrowFunctionBodyContext>(0);
}


size_t ECMAScriptParser::ArrowFunctionExpressionContext::getRuleIndex() const {
  return ECMAScriptParser::RuleArrowFunctionExpression;
}

void ECMAScriptParser::ArrowFunctionExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArrowFunctionExpression(this);
}

void ECMAScriptParser::ArrowFunctionExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArrowFunctionExpression(this);
}

ECMAScriptParser::ArrowFunctionExpressionContext* ECMAScriptParser::arrowFunctionExpression() {
  ArrowFunctionExpressionContext *_localctx = _tracker.createInstance<ArrowFunctionExpressionContext>(_ctx, getState());
  enterRule(_localctx, 96, ECMAScriptParser::RuleArrowFunctionExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(579);
    arrowFunctionParameters();
    setState(580);
    match(ECMAScriptParser::T__86);
    setState(581);
    arrowFunctionBody();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArrowFunctionParametersContext ------------------------------------------------------------------

ECMAScriptParser::ArrowFunctionParametersContext::ArrowFunctionParametersContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::ArrowFunctionParametersContext::identifier() {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(0);
}

ECMAScriptParser::FormalParameterListContext* ECMAScriptParser::ArrowFunctionParametersContext::formalParameterList() {
  return getRuleContext<ECMAScriptParser::FormalParameterListContext>(0);
}


size_t ECMAScriptParser::ArrowFunctionParametersContext::getRuleIndex() const {
  return ECMAScriptParser::RuleArrowFunctionParameters;
}

void ECMAScriptParser::ArrowFunctionParametersContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArrowFunctionParameters(this);
}

void ECMAScriptParser::ArrowFunctionParametersContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArrowFunctionParameters(this);
}

ECMAScriptParser::ArrowFunctionParametersContext* ECMAScriptParser::arrowFunctionParameters() {
  ArrowFunctionParametersContext *_localctx = _tracker.createInstance<ArrowFunctionParametersContext>(_ctx, getState());
  enterRule(_localctx, 98, ECMAScriptParser::RuleArrowFunctionParameters);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(589);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ECMAScriptParser::T__16:
      case ECMAScriptParser::T__17:
      case ECMAScriptParser::T__18:
      case ECMAScriptParser::T__20:
      case ECMAScriptParser::T__21:
      case ECMAScriptParser::T__22:
      case ECMAScriptParser::T__23:
      case ECMAScriptParser::T__24:
      case ECMAScriptParser::T__25:
      case ECMAScriptParser::T__26:
      case ECMAScriptParser::T__27:
      case ECMAScriptParser::T__28:
      case ECMAScriptParser::T__29:
      case ECMAScriptParser::T__30:
      case ECMAScriptParser::T__31:
      case ECMAScriptParser::T__32:
      case ECMAScriptParser::T__34:
      case ECMAScriptParser::T__35:
      case ECMAScriptParser::T__36:
      case ECMAScriptParser::T__37:
      case ECMAScriptParser::T__38:
      case ECMAScriptParser::T__39:
      case ECMAScriptParser::T__40:
      case ECMAScriptParser::T__43:
      case ECMAScriptParser::T__46:
      case ECMAScriptParser::T__47:
      case ECMAScriptParser::T__48:
      case ECMAScriptParser::T__63:
      case ECMAScriptParser::T__85:
      case ECMAScriptParser::T__87:
      case ECMAScriptParser::T__88:
      case ECMAScriptParser::T__89:
      case ECMAScriptParser::T__92:
      case ECMAScriptParser::T__93:
      case ECMAScriptParser::Identifier: {
        enterOuterAlt(_localctx, 1);
        setState(583);
        identifier();
        break;
      }

      case ECMAScriptParser::T__4: {
        enterOuterAlt(_localctx, 2);
        setState(584);
        match(ECMAScriptParser::T__4);
        setState(586);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 1007135469993984) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & 276610154497) != 0)) {
          setState(585);
          formalParameterList();
        }
        setState(588);
        match(ECMAScriptParser::T__5);
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

//----------------- ArrowFunctionBodyContext ------------------------------------------------------------------

ECMAScriptParser::ArrowFunctionBodyContext::ArrowFunctionBodyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::SingleExpressionContext* ECMAScriptParser::ArrowFunctionBodyContext::singleExpression() {
  return getRuleContext<ECMAScriptParser::SingleExpressionContext>(0);
}

ECMAScriptParser::FunctionBodyContext* ECMAScriptParser::ArrowFunctionBodyContext::functionBody() {
  return getRuleContext<ECMAScriptParser::FunctionBodyContext>(0);
}


size_t ECMAScriptParser::ArrowFunctionBodyContext::getRuleIndex() const {
  return ECMAScriptParser::RuleArrowFunctionBody;
}

void ECMAScriptParser::ArrowFunctionBodyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArrowFunctionBody(this);
}

void ECMAScriptParser::ArrowFunctionBodyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArrowFunctionBody(this);
}

ECMAScriptParser::ArrowFunctionBodyContext* ECMAScriptParser::arrowFunctionBody() {
  ArrowFunctionBodyContext *_localctx = _tracker.createInstance<ArrowFunctionBodyContext>(_ctx, getState());
  enterRule(_localctx, 100, ECMAScriptParser::RuleArrowFunctionBody);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(596);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 40, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(591);
      singleExpression(0);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(592);
      match(ECMAScriptParser::T__14);
      setState(593);
      functionBody();
      setState(594);
      match(ECMAScriptParser::T__15);
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

//----------------- ArgumentsContext ------------------------------------------------------------------

ECMAScriptParser::ArgumentsContext::ArgumentsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::ArgumentListContext* ECMAScriptParser::ArgumentsContext::argumentList() {
  return getRuleContext<ECMAScriptParser::ArgumentListContext>(0);
}


size_t ECMAScriptParser::ArgumentsContext::getRuleIndex() const {
  return ECMAScriptParser::RuleArguments;
}

void ECMAScriptParser::ArgumentsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArguments(this);
}

void ECMAScriptParser::ArgumentsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArguments(this);
}

ECMAScriptParser::ArgumentsContext* ECMAScriptParser::arguments() {
  ArgumentsContext *_localctx = _tracker.createInstance<ArgumentsContext>(_ctx, getState());
  enterRule(_localctx, 102, ECMAScriptParser::RuleArguments);
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
    setState(598);
    match(ECMAScriptParser::T__4);
    setState(600);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 18005585235444146) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 545179828225) != 0)) {
      setState(599);
      argumentList();
    }
    setState(602);
    match(ECMAScriptParser::T__5);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgumentListContext ------------------------------------------------------------------

ECMAScriptParser::ArgumentListContext::ArgumentListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ECMAScriptParser::SingleExpressionContext *> ECMAScriptParser::ArgumentListContext::singleExpression() {
  return getRuleContexts<ECMAScriptParser::SingleExpressionContext>();
}

ECMAScriptParser::SingleExpressionContext* ECMAScriptParser::ArgumentListContext::singleExpression(size_t i) {
  return getRuleContext<ECMAScriptParser::SingleExpressionContext>(i);
}


size_t ECMAScriptParser::ArgumentListContext::getRuleIndex() const {
  return ECMAScriptParser::RuleArgumentList;
}

void ECMAScriptParser::ArgumentListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArgumentList(this);
}

void ECMAScriptParser::ArgumentListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArgumentList(this);
}

ECMAScriptParser::ArgumentListContext* ECMAScriptParser::argumentList() {
  ArgumentListContext *_localctx = _tracker.createInstance<ArgumentListContext>(_ctx, getState());
  enterRule(_localctx, 104, ECMAScriptParser::RuleArgumentList);
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
    setState(604);
    singleExpression(0);
    setState(609);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ECMAScriptParser::T__19) {
      setState(605);
      match(ECMAScriptParser::T__19);
      setState(606);
      singleExpression(0);
      setState(611);
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

//----------------- LiteralContext ------------------------------------------------------------------

ECMAScriptParser::LiteralContext::LiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::NullLiteralContext* ECMAScriptParser::LiteralContext::nullLiteral() {
  return getRuleContext<ECMAScriptParser::NullLiteralContext>(0);
}

ECMAScriptParser::BooleanLiteralContext* ECMAScriptParser::LiteralContext::booleanLiteral() {
  return getRuleContext<ECMAScriptParser::BooleanLiteralContext>(0);
}

ECMAScriptParser::StringLiteralContext* ECMAScriptParser::LiteralContext::stringLiteral() {
  return getRuleContext<ECMAScriptParser::StringLiteralContext>(0);
}

ECMAScriptParser::TemplateLiteralContext* ECMAScriptParser::LiteralContext::templateLiteral() {
  return getRuleContext<ECMAScriptParser::TemplateLiteralContext>(0);
}

ECMAScriptParser::RegularExpressionLiteralContext* ECMAScriptParser::LiteralContext::regularExpressionLiteral() {
  return getRuleContext<ECMAScriptParser::RegularExpressionLiteralContext>(0);
}

ECMAScriptParser::NumericLiteralContext* ECMAScriptParser::LiteralContext::numericLiteral() {
  return getRuleContext<ECMAScriptParser::NumericLiteralContext>(0);
}


size_t ECMAScriptParser::LiteralContext::getRuleIndex() const {
  return ECMAScriptParser::RuleLiteral;
}

void ECMAScriptParser::LiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLiteral(this);
}

void ECMAScriptParser::LiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLiteral(this);
}

ECMAScriptParser::LiteralContext* ECMAScriptParser::literal() {
  LiteralContext *_localctx = _tracker.createInstance<LiteralContext>(_ctx, getState());
  enterRule(_localctx, 106, ECMAScriptParser::RuleLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(618);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ECMAScriptParser::T__87: {
        enterOuterAlt(_localctx, 1);
        setState(612);
        nullLiteral();
        break;
      }

      case ECMAScriptParser::T__88:
      case ECMAScriptParser::T__89: {
        enterOuterAlt(_localctx, 2);
        setState(613);
        booleanLiteral();
        break;
      }

      case ECMAScriptParser::StringLiteral: {
        enterOuterAlt(_localctx, 3);
        setState(614);
        stringLiteral();
        break;
      }

      case ECMAScriptParser::T__90: {
        enterOuterAlt(_localctx, 4);
        setState(615);
        templateLiteral();
        break;
      }

      case ECMAScriptParser::RegularExpressionLiteral: {
        enterOuterAlt(_localctx, 5);
        setState(616);
        regularExpressionLiteral();
        break;
      }

      case ECMAScriptParser::DecimalLiteral:
      case ECMAScriptParser::HexIntegerLiteral:
      case ECMAScriptParser::OctalIntegerLiteral:
      case ECMAScriptParser::BinaryIntegerLiteral: {
        enterOuterAlt(_localctx, 6);
        setState(617);
        numericLiteral();
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

//----------------- NullLiteralContext ------------------------------------------------------------------

ECMAScriptParser::NullLiteralContext::NullLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ECMAScriptParser::NullLiteralContext::getRuleIndex() const {
  return ECMAScriptParser::RuleNullLiteral;
}

void ECMAScriptParser::NullLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNullLiteral(this);
}

void ECMAScriptParser::NullLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNullLiteral(this);
}

ECMAScriptParser::NullLiteralContext* ECMAScriptParser::nullLiteral() {
  NullLiteralContext *_localctx = _tracker.createInstance<NullLiteralContext>(_ctx, getState());
  enterRule(_localctx, 108, ECMAScriptParser::RuleNullLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(620);
    match(ECMAScriptParser::T__87);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BooleanLiteralContext ------------------------------------------------------------------

ECMAScriptParser::BooleanLiteralContext::BooleanLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ECMAScriptParser::BooleanLiteralContext::getRuleIndex() const {
  return ECMAScriptParser::RuleBooleanLiteral;
}

void ECMAScriptParser::BooleanLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBooleanLiteral(this);
}

void ECMAScriptParser::BooleanLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBooleanLiteral(this);
}

ECMAScriptParser::BooleanLiteralContext* ECMAScriptParser::booleanLiteral() {
  BooleanLiteralContext *_localctx = _tracker.createInstance<BooleanLiteralContext>(_ctx, getState());
  enterRule(_localctx, 110, ECMAScriptParser::RuleBooleanLiteral);
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
    setState(622);
    _la = _input->LA(1);
    if (!(_la == ECMAScriptParser::T__88

    || _la == ECMAScriptParser::T__89)) {
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

//----------------- StringLiteralContext ------------------------------------------------------------------

ECMAScriptParser::StringLiteralContext::StringLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ECMAScriptParser::StringLiteralContext::StringLiteral() {
  return getToken(ECMAScriptParser::StringLiteral, 0);
}


size_t ECMAScriptParser::StringLiteralContext::getRuleIndex() const {
  return ECMAScriptParser::RuleStringLiteral;
}

void ECMAScriptParser::StringLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStringLiteral(this);
}

void ECMAScriptParser::StringLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStringLiteral(this);
}

ECMAScriptParser::StringLiteralContext* ECMAScriptParser::stringLiteral() {
  StringLiteralContext *_localctx = _tracker.createInstance<StringLiteralContext>(_ctx, getState());
  enterRule(_localctx, 112, ECMAScriptParser::RuleStringLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(624);
    match(ECMAScriptParser::StringLiteral);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TemplateLiteralContext ------------------------------------------------------------------

ECMAScriptParser::TemplateLiteralContext::TemplateLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ECMAScriptParser::TemplateStringAtomContext *> ECMAScriptParser::TemplateLiteralContext::templateStringAtom() {
  return getRuleContexts<ECMAScriptParser::TemplateStringAtomContext>();
}

ECMAScriptParser::TemplateStringAtomContext* ECMAScriptParser::TemplateLiteralContext::templateStringAtom(size_t i) {
  return getRuleContext<ECMAScriptParser::TemplateStringAtomContext>(i);
}


size_t ECMAScriptParser::TemplateLiteralContext::getRuleIndex() const {
  return ECMAScriptParser::RuleTemplateLiteral;
}

void ECMAScriptParser::TemplateLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTemplateLiteral(this);
}

void ECMAScriptParser::TemplateLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTemplateLiteral(this);
}

ECMAScriptParser::TemplateLiteralContext* ECMAScriptParser::templateLiteral() {
  TemplateLiteralContext *_localctx = _tracker.createInstance<TemplateLiteralContext>(_ctx, getState());
  enterRule(_localctx, 114, ECMAScriptParser::RuleTemplateLiteral);
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
    setState(626);
    match(ECMAScriptParser::T__90);
    setState(630);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ECMAScriptParser::T__91

    || _la == ECMAScriptParser::TemplateStringAtom) {
      setState(627);
      templateStringAtom();
      setState(632);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(633);
    match(ECMAScriptParser::T__90);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TemplateStringAtomContext ------------------------------------------------------------------

ECMAScriptParser::TemplateStringAtomContext::TemplateStringAtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ECMAScriptParser::TemplateStringAtomContext::TemplateStringAtom() {
  return getToken(ECMAScriptParser::TemplateStringAtom, 0);
}

ECMAScriptParser::SingleExpressionContext* ECMAScriptParser::TemplateStringAtomContext::singleExpression() {
  return getRuleContext<ECMAScriptParser::SingleExpressionContext>(0);
}


size_t ECMAScriptParser::TemplateStringAtomContext::getRuleIndex() const {
  return ECMAScriptParser::RuleTemplateStringAtom;
}

void ECMAScriptParser::TemplateStringAtomContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTemplateStringAtom(this);
}

void ECMAScriptParser::TemplateStringAtomContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTemplateStringAtom(this);
}

ECMAScriptParser::TemplateStringAtomContext* ECMAScriptParser::templateStringAtom() {
  TemplateStringAtomContext *_localctx = _tracker.createInstance<TemplateStringAtomContext>(_ctx, getState());
  enterRule(_localctx, 116, ECMAScriptParser::RuleTemplateStringAtom);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(640);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ECMAScriptParser::TemplateStringAtom: {
        enterOuterAlt(_localctx, 1);
        setState(635);
        match(ECMAScriptParser::TemplateStringAtom);
        break;
      }

      case ECMAScriptParser::T__91: {
        enterOuterAlt(_localctx, 2);
        setState(636);
        match(ECMAScriptParser::T__91);
        setState(637);
        singleExpression(0);
        setState(638);
        match(ECMAScriptParser::T__15);
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

//----------------- RegularExpressionLiteralContext ------------------------------------------------------------------

ECMAScriptParser::RegularExpressionLiteralContext::RegularExpressionLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ECMAScriptParser::RegularExpressionLiteralContext::RegularExpressionLiteral() {
  return getToken(ECMAScriptParser::RegularExpressionLiteral, 0);
}


size_t ECMAScriptParser::RegularExpressionLiteralContext::getRuleIndex() const {
  return ECMAScriptParser::RuleRegularExpressionLiteral;
}

void ECMAScriptParser::RegularExpressionLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRegularExpressionLiteral(this);
}

void ECMAScriptParser::RegularExpressionLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRegularExpressionLiteral(this);
}

ECMAScriptParser::RegularExpressionLiteralContext* ECMAScriptParser::regularExpressionLiteral() {
  RegularExpressionLiteralContext *_localctx = _tracker.createInstance<RegularExpressionLiteralContext>(_ctx, getState());
  enterRule(_localctx, 118, ECMAScriptParser::RuleRegularExpressionLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(642);
    match(ECMAScriptParser::RegularExpressionLiteral);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumericLiteralContext ------------------------------------------------------------------

ECMAScriptParser::NumericLiteralContext::NumericLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ECMAScriptParser::NumericLiteralContext::DecimalLiteral() {
  return getToken(ECMAScriptParser::DecimalLiteral, 0);
}

tree::TerminalNode* ECMAScriptParser::NumericLiteralContext::HexIntegerLiteral() {
  return getToken(ECMAScriptParser::HexIntegerLiteral, 0);
}

tree::TerminalNode* ECMAScriptParser::NumericLiteralContext::OctalIntegerLiteral() {
  return getToken(ECMAScriptParser::OctalIntegerLiteral, 0);
}

tree::TerminalNode* ECMAScriptParser::NumericLiteralContext::BinaryIntegerLiteral() {
  return getToken(ECMAScriptParser::BinaryIntegerLiteral, 0);
}


size_t ECMAScriptParser::NumericLiteralContext::getRuleIndex() const {
  return ECMAScriptParser::RuleNumericLiteral;
}

void ECMAScriptParser::NumericLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNumericLiteral(this);
}

void ECMAScriptParser::NumericLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNumericLiteral(this);
}

ECMAScriptParser::NumericLiteralContext* ECMAScriptParser::numericLiteral() {
  NumericLiteralContext *_localctx = _tracker.createInstance<NumericLiteralContext>(_ctx, getState());
  enterRule(_localctx, 120, ECMAScriptParser::RuleNumericLiteral);
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
    setState(644);
    _la = _input->LA(1);
    if (!(((((_la - 98) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 98)) & 15) != 0))) {
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

//----------------- ArrayLiteralContext ------------------------------------------------------------------

ECMAScriptParser::ArrayLiteralContext::ArrayLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::ElementListContext* ECMAScriptParser::ArrayLiteralContext::elementList() {
  return getRuleContext<ECMAScriptParser::ElementListContext>(0);
}


size_t ECMAScriptParser::ArrayLiteralContext::getRuleIndex() const {
  return ECMAScriptParser::RuleArrayLiteral;
}

void ECMAScriptParser::ArrayLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArrayLiteral(this);
}

void ECMAScriptParser::ArrayLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArrayLiteral(this);
}

ECMAScriptParser::ArrayLiteralContext* ECMAScriptParser::arrayLiteral() {
  ArrayLiteralContext *_localctx = _tracker.createInstance<ArrayLiteralContext>(_ctx, getState());
  enterRule(_localctx, 122, ECMAScriptParser::RuleArrayLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(646);
    match(ECMAScriptParser::T__41);
    setState(648);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 46, _ctx)) {
    case 1: {
      setState(647);
      elementList();
      break;
    }

    default:
      break;
    }
    setState(650);
    match(ECMAScriptParser::T__42);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ElementListContext ------------------------------------------------------------------

ECMAScriptParser::ElementListContext::ElementListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ECMAScriptParser::ArrayElementContext *> ECMAScriptParser::ElementListContext::arrayElement() {
  return getRuleContexts<ECMAScriptParser::ArrayElementContext>();
}

ECMAScriptParser::ArrayElementContext* ECMAScriptParser::ElementListContext::arrayElement(size_t i) {
  return getRuleContext<ECMAScriptParser::ArrayElementContext>(i);
}


size_t ECMAScriptParser::ElementListContext::getRuleIndex() const {
  return ECMAScriptParser::RuleElementList;
}

void ECMAScriptParser::ElementListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterElementList(this);
}

void ECMAScriptParser::ElementListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitElementList(this);
}

ECMAScriptParser::ElementListContext* ECMAScriptParser::elementList() {
  ElementListContext *_localctx = _tracker.createInstance<ElementListContext>(_ctx, getState());
  enterRule(_localctx, 124, ECMAScriptParser::RuleElementList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(655);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 47, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(652);
        match(ECMAScriptParser::T__19); 
      }
      setState(657);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 47, _ctx);
    }
    setState(659);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 18005585235444146) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 545179828225) != 0)) {
      setState(658);
      arrayElement();
    }
    setState(669);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 50, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(662); 
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
          setState(661);
          match(ECMAScriptParser::T__19);
          setState(664); 
          _errHandler->sync(this);
          _la = _input->LA(1);
        } while (_la == ECMAScriptParser::T__19);
        setState(666);
        arrayElement(); 
      }
      setState(671);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 50, _ctx);
    }
    setState(675);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ECMAScriptParser::T__19) {
      setState(672);
      match(ECMAScriptParser::T__19);
      setState(677);
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

//----------------- ArrayElementContext ------------------------------------------------------------------

ECMAScriptParser::ArrayElementContext::ArrayElementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::SingleExpressionContext* ECMAScriptParser::ArrayElementContext::singleExpression() {
  return getRuleContext<ECMAScriptParser::SingleExpressionContext>(0);
}


size_t ECMAScriptParser::ArrayElementContext::getRuleIndex() const {
  return ECMAScriptParser::RuleArrayElement;
}

void ECMAScriptParser::ArrayElementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArrayElement(this);
}

void ECMAScriptParser::ArrayElementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArrayElement(this);
}

ECMAScriptParser::ArrayElementContext* ECMAScriptParser::arrayElement() {
  ArrayElementContext *_localctx = _tracker.createInstance<ArrayElementContext>(_ctx, getState());
  enterRule(_localctx, 126, ECMAScriptParser::RuleArrayElement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(678);
    singleExpression(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectLiteralContext ------------------------------------------------------------------

ECMAScriptParser::ObjectLiteralContext::ObjectLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ECMAScriptParser::PropertyAssignmentContext *> ECMAScriptParser::ObjectLiteralContext::propertyAssignment() {
  return getRuleContexts<ECMAScriptParser::PropertyAssignmentContext>();
}

ECMAScriptParser::PropertyAssignmentContext* ECMAScriptParser::ObjectLiteralContext::propertyAssignment(size_t i) {
  return getRuleContext<ECMAScriptParser::PropertyAssignmentContext>(i);
}


size_t ECMAScriptParser::ObjectLiteralContext::getRuleIndex() const {
  return ECMAScriptParser::RuleObjectLiteral;
}

void ECMAScriptParser::ObjectLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterObjectLiteral(this);
}

void ECMAScriptParser::ObjectLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitObjectLiteral(this);
}

ECMAScriptParser::ObjectLiteralContext* ECMAScriptParser::objectLiteral() {
  ObjectLiteralContext *_localctx = _tracker.createInstance<ObjectLiteralContext>(_ctx, getState());
  enterRule(_localctx, 128, ECMAScriptParser::RuleObjectLiteral);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(680);
    match(ECMAScriptParser::T__14);
    setState(682);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 1011533516505088) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 536455675905) != 0)) {
      setState(681);
      propertyAssignment();
    }
    setState(688);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 53, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(684);
        match(ECMAScriptParser::T__19);
        setState(685);
        propertyAssignment(); 
      }
      setState(690);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 53, _ctx);
    }
    setState(692);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ECMAScriptParser::T__19) {
      setState(691);
      match(ECMAScriptParser::T__19);
    }
    setState(694);
    match(ECMAScriptParser::T__15);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertyAssignmentContext ------------------------------------------------------------------

ECMAScriptParser::PropertyAssignmentContext::PropertyAssignmentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::PropertyNameContext* ECMAScriptParser::PropertyAssignmentContext::propertyName() {
  return getRuleContext<ECMAScriptParser::PropertyNameContext>(0);
}

std::vector<ECMAScriptParser::SingleExpressionContext *> ECMAScriptParser::PropertyAssignmentContext::singleExpression() {
  return getRuleContexts<ECMAScriptParser::SingleExpressionContext>();
}

ECMAScriptParser::SingleExpressionContext* ECMAScriptParser::PropertyAssignmentContext::singleExpression(size_t i) {
  return getRuleContext<ECMAScriptParser::SingleExpressionContext>(i);
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::PropertyAssignmentContext::identifier() {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(0);
}

ECMAScriptParser::GetterContext* ECMAScriptParser::PropertyAssignmentContext::getter() {
  return getRuleContext<ECMAScriptParser::GetterContext>(0);
}

ECMAScriptParser::SetterContext* ECMAScriptParser::PropertyAssignmentContext::setter() {
  return getRuleContext<ECMAScriptParser::SetterContext>(0);
}


size_t ECMAScriptParser::PropertyAssignmentContext::getRuleIndex() const {
  return ECMAScriptParser::RulePropertyAssignment;
}

void ECMAScriptParser::PropertyAssignmentContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPropertyAssignment(this);
}

void ECMAScriptParser::PropertyAssignmentContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPropertyAssignment(this);
}

ECMAScriptParser::PropertyAssignmentContext* ECMAScriptParser::propertyAssignment() {
  PropertyAssignmentContext *_localctx = _tracker.createInstance<PropertyAssignmentContext>(_ctx, getState());
  enterRule(_localctx, 130, ECMAScriptParser::RulePropertyAssignment);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(709);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 55, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(696);
      propertyName();
      setState(697);
      match(ECMAScriptParser::T__33);
      setState(698);
      singleExpression(0);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(700);
      match(ECMAScriptParser::T__41);
      setState(701);
      singleExpression(0);
      setState(702);
      match(ECMAScriptParser::T__42);
      setState(703);
      match(ECMAScriptParser::T__33);
      setState(704);
      singleExpression(0);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(706);
      identifier();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(707);
      getter();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(708);
      setter();
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

//----------------- PropertyNameContext ------------------------------------------------------------------

ECMAScriptParser::PropertyNameContext::PropertyNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::PropertyNameContext::identifier() {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(0);
}

ECMAScriptParser::StringLiteralContext* ECMAScriptParser::PropertyNameContext::stringLiteral() {
  return getRuleContext<ECMAScriptParser::StringLiteralContext>(0);
}

ECMAScriptParser::NumericLiteralContext* ECMAScriptParser::PropertyNameContext::numericLiteral() {
  return getRuleContext<ECMAScriptParser::NumericLiteralContext>(0);
}


size_t ECMAScriptParser::PropertyNameContext::getRuleIndex() const {
  return ECMAScriptParser::RulePropertyName;
}

void ECMAScriptParser::PropertyNameContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPropertyName(this);
}

void ECMAScriptParser::PropertyNameContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPropertyName(this);
}

ECMAScriptParser::PropertyNameContext* ECMAScriptParser::propertyName() {
  PropertyNameContext *_localctx = _tracker.createInstance<PropertyNameContext>(_ctx, getState());
  enterRule(_localctx, 132, ECMAScriptParser::RulePropertyName);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(714);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ECMAScriptParser::T__16:
      case ECMAScriptParser::T__17:
      case ECMAScriptParser::T__18:
      case ECMAScriptParser::T__20:
      case ECMAScriptParser::T__21:
      case ECMAScriptParser::T__22:
      case ECMAScriptParser::T__23:
      case ECMAScriptParser::T__24:
      case ECMAScriptParser::T__25:
      case ECMAScriptParser::T__26:
      case ECMAScriptParser::T__27:
      case ECMAScriptParser::T__28:
      case ECMAScriptParser::T__29:
      case ECMAScriptParser::T__30:
      case ECMAScriptParser::T__31:
      case ECMAScriptParser::T__32:
      case ECMAScriptParser::T__34:
      case ECMAScriptParser::T__35:
      case ECMAScriptParser::T__36:
      case ECMAScriptParser::T__37:
      case ECMAScriptParser::T__38:
      case ECMAScriptParser::T__39:
      case ECMAScriptParser::T__40:
      case ECMAScriptParser::T__43:
      case ECMAScriptParser::T__46:
      case ECMAScriptParser::T__47:
      case ECMAScriptParser::T__48:
      case ECMAScriptParser::T__63:
      case ECMAScriptParser::T__85:
      case ECMAScriptParser::T__87:
      case ECMAScriptParser::T__88:
      case ECMAScriptParser::T__89:
      case ECMAScriptParser::T__92:
      case ECMAScriptParser::T__93:
      case ECMAScriptParser::Identifier: {
        enterOuterAlt(_localctx, 1);
        setState(711);
        identifier();
        break;
      }

      case ECMAScriptParser::StringLiteral: {
        enterOuterAlt(_localctx, 2);
        setState(712);
        stringLiteral();
        break;
      }

      case ECMAScriptParser::DecimalLiteral:
      case ECMAScriptParser::HexIntegerLiteral:
      case ECMAScriptParser::OctalIntegerLiteral:
      case ECMAScriptParser::BinaryIntegerLiteral: {
        enterOuterAlt(_localctx, 3);
        setState(713);
        numericLiteral();
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

//----------------- GetterContext ------------------------------------------------------------------

ECMAScriptParser::GetterContext::GetterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::PropertyNameContext* ECMAScriptParser::GetterContext::propertyName() {
  return getRuleContext<ECMAScriptParser::PropertyNameContext>(0);
}

ECMAScriptParser::FunctionBodyContext* ECMAScriptParser::GetterContext::functionBody() {
  return getRuleContext<ECMAScriptParser::FunctionBodyContext>(0);
}


size_t ECMAScriptParser::GetterContext::getRuleIndex() const {
  return ECMAScriptParser::RuleGetter;
}

void ECMAScriptParser::GetterContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGetter(this);
}

void ECMAScriptParser::GetterContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGetter(this);
}

ECMAScriptParser::GetterContext* ECMAScriptParser::getter() {
  GetterContext *_localctx = _tracker.createInstance<GetterContext>(_ctx, getState());
  enterRule(_localctx, 134, ECMAScriptParser::RuleGetter);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(716);
    match(ECMAScriptParser::T__92);
    setState(717);
    propertyName();
    setState(718);
    match(ECMAScriptParser::T__4);
    setState(719);
    match(ECMAScriptParser::T__5);
    setState(720);
    match(ECMAScriptParser::T__14);
    setState(721);
    functionBody();
    setState(722);
    match(ECMAScriptParser::T__15);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SetterContext ------------------------------------------------------------------

ECMAScriptParser::SetterContext::SetterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ECMAScriptParser::PropertyNameContext* ECMAScriptParser::SetterContext::propertyName() {
  return getRuleContext<ECMAScriptParser::PropertyNameContext>(0);
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::SetterContext::identifier() {
  return getRuleContext<ECMAScriptParser::IdentifierContext>(0);
}

ECMAScriptParser::FunctionBodyContext* ECMAScriptParser::SetterContext::functionBody() {
  return getRuleContext<ECMAScriptParser::FunctionBodyContext>(0);
}


size_t ECMAScriptParser::SetterContext::getRuleIndex() const {
  return ECMAScriptParser::RuleSetter;
}

void ECMAScriptParser::SetterContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSetter(this);
}

void ECMAScriptParser::SetterContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSetter(this);
}

ECMAScriptParser::SetterContext* ECMAScriptParser::setter() {
  SetterContext *_localctx = _tracker.createInstance<SetterContext>(_ctx, getState());
  enterRule(_localctx, 136, ECMAScriptParser::RuleSetter);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(724);
    match(ECMAScriptParser::T__93);
    setState(725);
    propertyName();
    setState(726);
    match(ECMAScriptParser::T__4);
    setState(727);
    identifier();
    setState(728);
    match(ECMAScriptParser::T__5);
    setState(729);
    match(ECMAScriptParser::T__14);
    setState(730);
    functionBody();
    setState(731);
    match(ECMAScriptParser::T__15);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IdentifierContext ------------------------------------------------------------------

ECMAScriptParser::IdentifierContext::IdentifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ECMAScriptParser::IdentifierContext::Identifier() {
  return getToken(ECMAScriptParser::Identifier, 0);
}


size_t ECMAScriptParser::IdentifierContext::getRuleIndex() const {
  return ECMAScriptParser::RuleIdentifier;
}

void ECMAScriptParser::IdentifierContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIdentifier(this);
}

void ECMAScriptParser::IdentifierContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIdentifier(this);
}

ECMAScriptParser::IdentifierContext* ECMAScriptParser::identifier() {
  IdentifierContext *_localctx = _tracker.createInstance<IdentifierContext>(_ctx, getState());
  enterRule(_localctx, 138, ECMAScriptParser::RuleIdentifier);
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
    setState(733);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 1007135469993984) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 276610154497) != 0))) {
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

//----------------- EosContext ------------------------------------------------------------------

ECMAScriptParser::EosContext::EosContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ECMAScriptParser::EosContext::EOF() {
  return getToken(ECMAScriptParser::EOF, 0);
}


size_t ECMAScriptParser::EosContext::getRuleIndex() const {
  return ECMAScriptParser::RuleEos;
}

void ECMAScriptParser::EosContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEos(this);
}

void ECMAScriptParser::EosContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ECMAScriptListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEos(this);
}

ECMAScriptParser::EosContext* ECMAScriptParser::eos() {
  EosContext *_localctx = _tracker.createInstance<EosContext>(_ctx, getState());
  enterRule(_localctx, 140, ECMAScriptParser::RuleEos);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(738);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 57, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(735);
      match(ECMAScriptParser::T__13);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(736);
      match(ECMAScriptParser::EOF);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(737);

      if (!(lineTerminatorAhead())) throw FailedPredicateException(this, "lineTerminatorAhead()");
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

bool ECMAScriptParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 44: return singleExpressionSempred(antlrcpp::downCast<SingleExpressionContext *>(context), predicateIndex);
    case 70: return eosSempred(antlrcpp::downCast<EosContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool ECMAScriptParser::singleExpressionSempred(SingleExpressionContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 17);
    case 1: return precpred(_ctx, 16);
    case 2: return precpred(_ctx, 15);
    case 3: return precpred(_ctx, 14);
    case 4: return precpred(_ctx, 13);
    case 5: return precpred(_ctx, 12);
    case 6: return precpred(_ctx, 11);
    case 7: return precpred(_ctx, 10);
    case 8: return precpred(_ctx, 9);
    case 9: return precpred(_ctx, 8);
    case 10: return precpred(_ctx, 7);
    case 11: return precpred(_ctx, 6);
    case 12: return precpred(_ctx, 5);
    case 13: return precpred(_ctx, 4);
    case 14: return precpred(_ctx, 3);
    case 15: return precpred(_ctx, 32);
    case 16: return precpred(_ctx, 31);
    case 17: return precpred(_ctx, 30);
    case 18: return precpred(_ctx, 28);
    case 19: return precpred(_ctx, 27);

  default:
    break;
  }
  return true;
}

bool ECMAScriptParser::eosSempred(EosContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 20: return lineTerminatorAhead();

  default:
    break;
  }
  return true;
}

void ECMAScriptParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  ecmascriptParserInitialize();
#else
  ::antlr4::internal::call_once(ecmascriptParserOnceFlag, ecmascriptParserInitialize);
#endif
}

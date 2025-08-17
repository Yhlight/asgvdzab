
// Generated from grammar/CSS3.g4 by ANTLR 4.13.1


#include "CSS3Listener.h"

#include "CSS3Parser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct CSS3ParserStaticData final {
  CSS3ParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  CSS3ParserStaticData(const CSS3ParserStaticData&) = delete;
  CSS3ParserStaticData(CSS3ParserStaticData&&) = delete;
  CSS3ParserStaticData& operator=(const CSS3ParserStaticData&) = delete;
  CSS3ParserStaticData& operator=(CSS3ParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag css3ParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
CSS3ParserStaticData *css3ParserStaticData = nullptr;

void css3ParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (css3ParserStaticData != nullptr) {
    return;
  }
#else
  assert(css3ParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<CSS3ParserStaticData>(
    std::vector<std::string>{
      "stylesheet", "styleRule", "ruleSet", "selectorList", "selector", 
      "combinator", "simpleSelector", "typeSelector", "tagName", "qualifiers", 
      "hashSelector", "classSelector", "attributeSelector", "attributeOperator", 
      "attributeValue", "pseudoClass", "pseudoElement", "declarationList", 
      "declaration", "property", "valueList", "value", "calc", "function_", 
      "expressionList", "expression", "term", "factor", "prio", "chtlTemplateUsage", 
      "chtlCustomUsage", "chtlSpecialization", "chtlVariableUsage", "chtlVariableReference", 
      "chtlStyleGroupUsage", "chtlDeleteProperty", "chtlInherit", "chtlOriginEmbed", 
      "atRule", "atRuleBlock", "var", "hexcolor", "number", "percentage", 
      "dimension", "unit", "uri", "string", "ident", "hexdigit", "ws"
    },
    std::vector<std::string>{
      "", "'{'", "'}'", "','", "'+'", "'>'", "'~'", "'||'", "'*'", "'#'", 
      "'.'", "'['", "']'", "'='", "'~='", "'|='", "'^='", "'$='", "'*='", 
      "':'", "'('", "')'", "'::'", "';'", "'calc('", "'-'", "'/'", "'!'", 
      "'important'", "'@'", "'Style'", "'style'", "'CSS'", "'Css'", "'css'", 
      "'delete'", "'inherit'", "'[Origin]'", "'var('", "'--'", "'%'", "'em'", 
      "'ex'", "'ch'", "'rem'", "'vw'", "'vh'", "'vmin'", "'vmax'", "'cm'", 
      "'mm'", "'in'", "'px'", "'pt'", "'pc'", "'deg'", "'rad'", "'grad'", 
      "'turn'", "'s'", "'ms'", "'Hz'", "'kHz'", "'dpi'", "'dpcm'", "'dppx'", 
      "'url('"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "NUMBER", 
      "STRING", "IDENT", "HEXDIGIT", "WS", "NONASCII", "ESCAPE", "URL_CONTENT", 
      "COMMENT", "LINE_COMMENT"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,76,803,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,7,
  	49,2,50,7,50,1,0,5,0,104,8,0,10,0,12,0,107,9,0,1,0,5,0,110,8,0,10,0,12,
  	0,113,9,0,1,0,5,0,116,8,0,10,0,12,0,119,9,0,1,0,1,0,1,1,1,1,1,1,1,1,1,
  	1,1,1,3,1,129,8,1,1,2,1,2,5,2,133,8,2,10,2,12,2,136,9,2,1,2,1,2,5,2,140,
  	8,2,10,2,12,2,143,9,2,1,2,3,2,146,8,2,1,2,5,2,149,8,2,10,2,12,2,152,9,
  	2,1,2,1,2,1,3,1,3,5,3,158,8,3,10,3,12,3,161,9,3,1,3,1,3,5,3,165,8,3,10,
  	3,12,3,168,9,3,1,3,5,3,171,8,3,10,3,12,3,174,9,3,1,4,1,4,1,4,1,4,5,4,
  	180,8,4,10,4,12,4,183,9,4,1,5,5,5,186,8,5,10,5,12,5,189,9,5,1,5,1,5,5,
  	5,193,8,5,10,5,12,5,196,9,5,1,5,4,5,199,8,5,11,5,12,5,200,3,5,203,8,5,
  	1,6,1,6,5,6,207,8,6,10,6,12,6,210,9,6,1,6,4,6,213,8,6,11,6,12,6,214,3,
  	6,217,8,6,1,7,1,7,3,7,221,8,7,1,8,1,8,1,9,1,9,1,9,1,9,1,9,3,9,230,8,9,
  	1,10,1,10,1,10,1,11,1,11,1,11,1,12,1,12,5,12,240,8,12,10,12,12,12,243,
  	9,12,1,12,1,12,5,12,247,8,12,10,12,12,12,250,9,12,1,12,1,12,5,12,254,
  	8,12,10,12,12,12,257,9,12,1,12,1,12,5,12,261,8,12,10,12,12,12,264,9,12,
  	3,12,266,8,12,1,12,1,12,1,13,1,13,1,14,1,14,3,14,274,8,14,1,15,1,15,1,
  	15,1,15,5,15,280,8,15,10,15,12,15,283,9,15,1,15,1,15,5,15,287,8,15,10,
  	15,12,15,290,9,15,1,15,1,15,3,15,294,8,15,1,16,1,16,1,16,1,17,1,17,5,
  	17,301,8,17,10,17,12,17,304,9,17,1,17,1,17,5,17,308,8,17,10,17,12,17,
  	311,9,17,1,17,5,17,314,8,17,10,17,12,17,317,9,17,1,17,5,17,320,8,17,10,
  	17,12,17,323,9,17,1,17,3,17,326,8,17,1,18,1,18,5,18,330,8,18,10,18,12,
  	18,333,9,18,1,18,1,18,5,18,337,8,18,10,18,12,18,340,9,18,1,18,1,18,3,
  	18,344,8,18,1,18,1,18,1,18,3,18,349,8,18,1,19,1,19,3,19,353,8,19,1,20,
  	1,20,4,20,357,8,20,11,20,12,20,358,1,20,1,20,5,20,363,8,20,10,20,12,20,
  	366,9,20,1,21,1,21,3,21,370,8,21,1,21,1,21,1,21,1,21,1,21,1,21,1,21,1,
  	21,1,21,1,21,3,21,382,8,21,1,22,1,22,5,22,386,8,22,10,22,12,22,389,9,
  	22,1,22,1,22,5,22,393,8,22,10,22,12,22,396,9,22,1,22,1,22,1,23,1,23,1,
  	23,5,23,403,8,23,10,23,12,23,406,9,23,1,23,3,23,409,8,23,1,23,5,23,412,
  	8,23,10,23,12,23,415,9,23,1,23,1,23,1,24,1,24,5,24,421,8,24,10,24,12,
  	24,424,9,24,1,24,1,24,5,24,428,8,24,10,24,12,24,431,9,24,1,24,5,24,434,
  	8,24,10,24,12,24,437,9,24,1,25,1,25,1,25,5,25,442,8,25,10,25,12,25,445,
  	9,25,1,25,5,25,448,8,25,10,25,12,25,451,9,25,1,26,1,26,1,26,5,26,456,
  	8,26,10,26,12,26,459,9,26,1,26,5,26,462,8,26,10,26,12,26,465,9,26,1,27,
  	1,27,3,27,469,8,27,1,27,1,27,1,27,1,27,5,27,475,8,27,10,27,12,27,478,
  	9,27,1,27,1,27,5,27,482,8,27,10,27,12,27,485,9,27,1,27,1,27,1,27,3,27,
  	490,8,27,1,28,5,28,493,8,28,10,28,12,28,496,9,28,1,28,1,28,5,28,500,8,
  	28,10,28,12,28,503,9,28,1,28,1,28,5,28,507,8,28,10,28,12,28,510,9,28,
  	1,29,1,29,1,29,4,29,515,8,29,11,29,12,29,516,1,29,1,29,5,29,521,8,29,
  	10,29,12,29,524,9,29,1,29,3,29,527,8,29,1,29,5,29,530,8,29,10,29,12,29,
  	533,9,29,1,29,1,29,1,30,1,30,1,30,4,30,540,8,30,11,30,12,30,541,1,30,
  	1,30,5,30,546,8,30,10,30,12,30,549,9,30,1,30,3,30,552,8,30,1,30,5,30,
  	555,8,30,10,30,12,30,558,9,30,1,30,1,30,1,31,1,31,5,31,564,8,31,10,31,
  	12,31,567,9,31,1,31,3,31,570,8,31,1,31,5,31,573,8,31,10,31,12,31,576,
  	9,31,1,31,1,31,1,32,1,32,1,32,1,32,1,32,5,32,585,8,32,10,32,12,32,588,
  	9,32,1,32,1,32,1,33,1,33,1,33,1,33,1,33,1,34,1,34,1,34,4,34,600,8,34,
  	11,34,12,34,601,1,34,1,34,5,34,606,8,34,10,34,12,34,609,9,34,1,34,3,34,
  	612,8,34,1,35,1,35,4,35,616,8,35,11,35,12,35,617,1,35,1,35,5,35,622,8,
  	35,10,35,12,35,625,9,35,1,36,1,36,4,36,629,8,36,11,36,12,36,630,1,36,
  	1,36,1,36,4,36,636,8,36,11,36,12,36,637,1,36,1,36,5,36,642,8,36,10,36,
  	12,36,645,9,36,1,37,1,37,5,37,649,8,37,10,37,12,37,652,9,37,1,37,1,37,
  	1,37,4,37,657,8,37,11,37,12,37,658,1,37,1,37,5,37,663,8,37,10,37,12,37,
  	666,9,37,1,37,1,37,1,38,1,38,1,38,5,38,673,8,38,10,38,12,38,676,9,38,
  	1,38,1,38,3,38,680,8,38,1,39,1,39,5,39,684,8,39,10,39,12,39,687,9,39,
  	1,39,1,39,5,39,691,8,39,10,39,12,39,694,9,39,1,39,5,39,697,8,39,10,39,
  	12,39,700,9,39,1,39,1,39,1,39,1,39,5,39,706,8,39,10,39,12,39,709,9,39,
  	1,39,5,39,712,8,39,10,39,12,39,715,9,39,1,39,5,39,718,8,39,10,39,12,39,
  	721,9,39,1,39,1,39,3,39,725,8,39,1,40,1,40,5,40,729,8,40,10,40,12,40,
  	732,9,40,1,40,1,40,1,40,5,40,737,8,40,10,40,12,40,740,9,40,1,40,1,40,
  	5,40,744,8,40,10,40,12,40,747,9,40,1,40,3,40,750,8,40,1,40,5,40,753,8,
  	40,10,40,12,40,756,9,40,1,40,1,40,1,41,1,41,4,41,762,8,41,11,41,12,41,
  	763,1,42,1,42,1,43,1,43,1,43,1,44,1,44,1,44,1,45,1,45,1,46,1,46,5,46,
  	778,8,46,10,46,12,46,781,9,46,1,46,1,46,3,46,785,8,46,1,46,5,46,788,8,
  	46,10,46,12,46,791,9,46,1,46,1,46,1,47,1,47,1,48,1,48,1,49,1,49,1,50,
  	1,50,1,50,0,0,51,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,
  	38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,
  	84,86,88,90,92,94,96,98,100,0,6,1,0,4,7,1,0,13,18,2,0,4,4,25,25,2,0,8,
  	8,26,26,1,0,30,34,1,0,40,65,877,0,105,1,0,0,0,2,128,1,0,0,0,4,130,1,0,
  	0,0,6,155,1,0,0,0,8,175,1,0,0,0,10,202,1,0,0,0,12,216,1,0,0,0,14,220,
  	1,0,0,0,16,222,1,0,0,0,18,229,1,0,0,0,20,231,1,0,0,0,22,234,1,0,0,0,24,
  	237,1,0,0,0,26,269,1,0,0,0,28,273,1,0,0,0,30,275,1,0,0,0,32,295,1,0,0,
  	0,34,298,1,0,0,0,36,348,1,0,0,0,38,352,1,0,0,0,40,354,1,0,0,0,42,381,
  	1,0,0,0,44,383,1,0,0,0,46,399,1,0,0,0,48,418,1,0,0,0,50,438,1,0,0,0,52,
  	452,1,0,0,0,54,489,1,0,0,0,56,494,1,0,0,0,58,511,1,0,0,0,60,536,1,0,0,
  	0,62,561,1,0,0,0,64,579,1,0,0,0,66,591,1,0,0,0,68,596,1,0,0,0,70,613,
  	1,0,0,0,72,626,1,0,0,0,74,646,1,0,0,0,76,669,1,0,0,0,78,724,1,0,0,0,80,
  	726,1,0,0,0,82,759,1,0,0,0,84,765,1,0,0,0,86,767,1,0,0,0,88,770,1,0,0,
  	0,90,773,1,0,0,0,92,775,1,0,0,0,94,794,1,0,0,0,96,796,1,0,0,0,98,798,
  	1,0,0,0,100,800,1,0,0,0,102,104,3,100,50,0,103,102,1,0,0,0,104,107,1,
  	0,0,0,105,103,1,0,0,0,105,106,1,0,0,0,106,111,1,0,0,0,107,105,1,0,0,0,
  	108,110,3,2,1,0,109,108,1,0,0,0,110,113,1,0,0,0,111,109,1,0,0,0,111,112,
  	1,0,0,0,112,117,1,0,0,0,113,111,1,0,0,0,114,116,3,100,50,0,115,114,1,
  	0,0,0,116,119,1,0,0,0,117,115,1,0,0,0,117,118,1,0,0,0,118,120,1,0,0,0,
  	119,117,1,0,0,0,120,121,5,0,0,1,121,1,1,0,0,0,122,129,3,4,2,0,123,129,
  	3,76,38,0,124,129,3,58,29,0,125,129,3,60,30,0,126,129,3,64,32,0,127,129,
  	3,74,37,0,128,122,1,0,0,0,128,123,1,0,0,0,128,124,1,0,0,0,128,125,1,0,
  	0,0,128,126,1,0,0,0,128,127,1,0,0,0,129,3,1,0,0,0,130,134,3,6,3,0,131,
  	133,3,100,50,0,132,131,1,0,0,0,133,136,1,0,0,0,134,132,1,0,0,0,134,135,
  	1,0,0,0,135,137,1,0,0,0,136,134,1,0,0,0,137,141,5,1,0,0,138,140,3,100,
  	50,0,139,138,1,0,0,0,140,143,1,0,0,0,141,139,1,0,0,0,141,142,1,0,0,0,
  	142,145,1,0,0,0,143,141,1,0,0,0,144,146,3,34,17,0,145,144,1,0,0,0,145,
  	146,1,0,0,0,146,150,1,0,0,0,147,149,3,100,50,0,148,147,1,0,0,0,149,152,
  	1,0,0,0,150,148,1,0,0,0,150,151,1,0,0,0,151,153,1,0,0,0,152,150,1,0,0,
  	0,153,154,5,2,0,0,154,5,1,0,0,0,155,172,3,8,4,0,156,158,3,100,50,0,157,
  	156,1,0,0,0,158,161,1,0,0,0,159,157,1,0,0,0,159,160,1,0,0,0,160,162,1,
  	0,0,0,161,159,1,0,0,0,162,166,5,3,0,0,163,165,3,100,50,0,164,163,1,0,
  	0,0,165,168,1,0,0,0,166,164,1,0,0,0,166,167,1,0,0,0,167,169,1,0,0,0,168,
  	166,1,0,0,0,169,171,3,8,4,0,170,159,1,0,0,0,171,174,1,0,0,0,172,170,1,
  	0,0,0,172,173,1,0,0,0,173,7,1,0,0,0,174,172,1,0,0,0,175,181,3,12,6,0,
  	176,177,3,10,5,0,177,178,3,12,6,0,178,180,1,0,0,0,179,176,1,0,0,0,180,
  	183,1,0,0,0,181,179,1,0,0,0,181,182,1,0,0,0,182,9,1,0,0,0,183,181,1,0,
  	0,0,184,186,3,100,50,0,185,184,1,0,0,0,186,189,1,0,0,0,187,185,1,0,0,
  	0,187,188,1,0,0,0,188,190,1,0,0,0,189,187,1,0,0,0,190,194,7,0,0,0,191,
  	193,3,100,50,0,192,191,1,0,0,0,193,196,1,0,0,0,194,192,1,0,0,0,194,195,
  	1,0,0,0,195,203,1,0,0,0,196,194,1,0,0,0,197,199,3,100,50,0,198,197,1,
  	0,0,0,199,200,1,0,0,0,200,198,1,0,0,0,200,201,1,0,0,0,201,203,1,0,0,0,
  	202,187,1,0,0,0,202,198,1,0,0,0,203,11,1,0,0,0,204,208,3,14,7,0,205,207,
  	3,18,9,0,206,205,1,0,0,0,207,210,1,0,0,0,208,206,1,0,0,0,208,209,1,0,
  	0,0,209,217,1,0,0,0,210,208,1,0,0,0,211,213,3,18,9,0,212,211,1,0,0,0,
  	213,214,1,0,0,0,214,212,1,0,0,0,214,215,1,0,0,0,215,217,1,0,0,0,216,204,
  	1,0,0,0,216,212,1,0,0,0,217,13,1,0,0,0,218,221,3,16,8,0,219,221,5,8,0,
  	0,220,218,1,0,0,0,220,219,1,0,0,0,221,15,1,0,0,0,222,223,3,96,48,0,223,
  	17,1,0,0,0,224,230,3,20,10,0,225,230,3,22,11,0,226,230,3,24,12,0,227,
  	230,3,30,15,0,228,230,3,32,16,0,229,224,1,0,0,0,229,225,1,0,0,0,229,226,
  	1,0,0,0,229,227,1,0,0,0,229,228,1,0,0,0,230,19,1,0,0,0,231,232,5,9,0,
  	0,232,233,3,96,48,0,233,21,1,0,0,0,234,235,5,10,0,0,235,236,3,96,48,0,
  	236,23,1,0,0,0,237,241,5,11,0,0,238,240,3,100,50,0,239,238,1,0,0,0,240,
  	243,1,0,0,0,241,239,1,0,0,0,241,242,1,0,0,0,242,244,1,0,0,0,243,241,1,
  	0,0,0,244,248,3,96,48,0,245,247,3,100,50,0,246,245,1,0,0,0,247,250,1,
  	0,0,0,248,246,1,0,0,0,248,249,1,0,0,0,249,265,1,0,0,0,250,248,1,0,0,0,
  	251,255,3,26,13,0,252,254,3,100,50,0,253,252,1,0,0,0,254,257,1,0,0,0,
  	255,253,1,0,0,0,255,256,1,0,0,0,256,258,1,0,0,0,257,255,1,0,0,0,258,262,
  	3,28,14,0,259,261,3,100,50,0,260,259,1,0,0,0,261,264,1,0,0,0,262,260,
  	1,0,0,0,262,263,1,0,0,0,263,266,1,0,0,0,264,262,1,0,0,0,265,251,1,0,0,
  	0,265,266,1,0,0,0,266,267,1,0,0,0,267,268,5,12,0,0,268,25,1,0,0,0,269,
  	270,7,1,0,0,270,27,1,0,0,0,271,274,3,96,48,0,272,274,3,94,47,0,273,271,
  	1,0,0,0,273,272,1,0,0,0,274,29,1,0,0,0,275,276,5,19,0,0,276,293,3,96,
  	48,0,277,281,5,20,0,0,278,280,3,100,50,0,279,278,1,0,0,0,280,283,1,0,
  	0,0,281,279,1,0,0,0,281,282,1,0,0,0,282,284,1,0,0,0,283,281,1,0,0,0,284,
  	288,3,50,25,0,285,287,3,100,50,0,286,285,1,0,0,0,287,290,1,0,0,0,288,
  	286,1,0,0,0,288,289,1,0,0,0,289,291,1,0,0,0,290,288,1,0,0,0,291,292,5,
  	21,0,0,292,294,1,0,0,0,293,277,1,0,0,0,293,294,1,0,0,0,294,31,1,0,0,0,
  	295,296,5,22,0,0,296,297,3,96,48,0,297,33,1,0,0,0,298,315,3,36,18,0,299,
  	301,3,100,50,0,300,299,1,0,0,0,301,304,1,0,0,0,302,300,1,0,0,0,302,303,
  	1,0,0,0,303,305,1,0,0,0,304,302,1,0,0,0,305,309,5,23,0,0,306,308,3,100,
  	50,0,307,306,1,0,0,0,308,311,1,0,0,0,309,307,1,0,0,0,309,310,1,0,0,0,
  	310,312,1,0,0,0,311,309,1,0,0,0,312,314,3,36,18,0,313,302,1,0,0,0,314,
  	317,1,0,0,0,315,313,1,0,0,0,315,316,1,0,0,0,316,321,1,0,0,0,317,315,1,
  	0,0,0,318,320,3,100,50,0,319,318,1,0,0,0,320,323,1,0,0,0,321,319,1,0,
  	0,0,321,322,1,0,0,0,322,325,1,0,0,0,323,321,1,0,0,0,324,326,5,23,0,0,
  	325,324,1,0,0,0,325,326,1,0,0,0,326,35,1,0,0,0,327,331,3,38,19,0,328,
  	330,3,100,50,0,329,328,1,0,0,0,330,333,1,0,0,0,331,329,1,0,0,0,331,332,
  	1,0,0,0,332,334,1,0,0,0,333,331,1,0,0,0,334,338,5,19,0,0,335,337,3,100,
  	50,0,336,335,1,0,0,0,337,340,1,0,0,0,338,336,1,0,0,0,338,339,1,0,0,0,
  	339,341,1,0,0,0,340,338,1,0,0,0,341,343,3,40,20,0,342,344,3,56,28,0,343,
  	342,1,0,0,0,343,344,1,0,0,0,344,349,1,0,0,0,345,349,3,68,34,0,346,349,
  	3,70,35,0,347,349,3,72,36,0,348,327,1,0,0,0,348,345,1,0,0,0,348,346,1,
  	0,0,0,348,347,1,0,0,0,349,37,1,0,0,0,350,353,3,96,48,0,351,353,3,80,40,
  	0,352,350,1,0,0,0,352,351,1,0,0,0,353,39,1,0,0,0,354,364,3,42,21,0,355,
  	357,3,100,50,0,356,355,1,0,0,0,357,358,1,0,0,0,358,356,1,0,0,0,358,359,
  	1,0,0,0,359,360,1,0,0,0,360,361,3,42,21,0,361,363,1,0,0,0,362,356,1,0,
  	0,0,363,366,1,0,0,0,364,362,1,0,0,0,364,365,1,0,0,0,365,41,1,0,0,0,366,
  	364,1,0,0,0,367,369,3,84,42,0,368,370,3,90,45,0,369,368,1,0,0,0,369,370,
  	1,0,0,0,370,382,1,0,0,0,371,382,3,86,43,0,372,382,3,88,44,0,373,382,3,
  	94,47,0,374,382,3,96,48,0,375,382,3,92,46,0,376,382,3,82,41,0,377,382,
  	3,44,22,0,378,382,3,80,40,0,379,382,3,66,33,0,380,382,3,46,23,0,381,367,
  	1,0,0,0,381,371,1,0,0,0,381,372,1,0,0,0,381,373,1,0,0,0,381,374,1,0,0,
  	0,381,375,1,0,0,0,381,376,1,0,0,0,381,377,1,0,0,0,381,378,1,0,0,0,381,
  	379,1,0,0,0,381,380,1,0,0,0,382,43,1,0,0,0,383,387,5,24,0,0,384,386,3,
  	100,50,0,385,384,1,0,0,0,386,389,1,0,0,0,387,385,1,0,0,0,387,388,1,0,
  	0,0,388,390,1,0,0,0,389,387,1,0,0,0,390,394,3,50,25,0,391,393,3,100,50,
  	0,392,391,1,0,0,0,393,396,1,0,0,0,394,392,1,0,0,0,394,395,1,0,0,0,395,
  	397,1,0,0,0,396,394,1,0,0,0,397,398,5,21,0,0,398,45,1,0,0,0,399,400,3,
  	96,48,0,400,404,5,20,0,0,401,403,3,100,50,0,402,401,1,0,0,0,403,406,1,
  	0,0,0,404,402,1,0,0,0,404,405,1,0,0,0,405,408,1,0,0,0,406,404,1,0,0,0,
  	407,409,3,48,24,0,408,407,1,0,0,0,408,409,1,0,0,0,409,413,1,0,0,0,410,
  	412,3,100,50,0,411,410,1,0,0,0,412,415,1,0,0,0,413,411,1,0,0,0,413,414,
  	1,0,0,0,414,416,1,0,0,0,415,413,1,0,0,0,416,417,5,21,0,0,417,47,1,0,0,
  	0,418,435,3,50,25,0,419,421,3,100,50,0,420,419,1,0,0,0,421,424,1,0,0,
  	0,422,420,1,0,0,0,422,423,1,0,0,0,423,425,1,0,0,0,424,422,1,0,0,0,425,
  	429,5,3,0,0,426,428,3,100,50,0,427,426,1,0,0,0,428,431,1,0,0,0,429,427,
  	1,0,0,0,429,430,1,0,0,0,430,432,1,0,0,0,431,429,1,0,0,0,432,434,3,50,
  	25,0,433,422,1,0,0,0,434,437,1,0,0,0,435,433,1,0,0,0,435,436,1,0,0,0,
  	436,49,1,0,0,0,437,435,1,0,0,0,438,449,3,52,26,0,439,443,7,2,0,0,440,
  	442,3,100,50,0,441,440,1,0,0,0,442,445,1,0,0,0,443,441,1,0,0,0,443,444,
  	1,0,0,0,444,446,1,0,0,0,445,443,1,0,0,0,446,448,3,52,26,0,447,439,1,0,
  	0,0,448,451,1,0,0,0,449,447,1,0,0,0,449,450,1,0,0,0,450,51,1,0,0,0,451,
  	449,1,0,0,0,452,463,3,54,27,0,453,457,7,3,0,0,454,456,3,100,50,0,455,
  	454,1,0,0,0,456,459,1,0,0,0,457,455,1,0,0,0,457,458,1,0,0,0,458,460,1,
  	0,0,0,459,457,1,0,0,0,460,462,3,54,27,0,461,453,1,0,0,0,462,465,1,0,0,
  	0,463,461,1,0,0,0,463,464,1,0,0,0,464,53,1,0,0,0,465,463,1,0,0,0,466,
  	468,3,84,42,0,467,469,3,90,45,0,468,467,1,0,0,0,468,469,1,0,0,0,469,490,
  	1,0,0,0,470,490,3,86,43,0,471,490,3,88,44,0,472,476,5,20,0,0,473,475,
  	3,100,50,0,474,473,1,0,0,0,475,478,1,0,0,0,476,474,1,0,0,0,476,477,1,
  	0,0,0,477,479,1,0,0,0,478,476,1,0,0,0,479,483,3,50,25,0,480,482,3,100,
  	50,0,481,480,1,0,0,0,482,485,1,0,0,0,483,481,1,0,0,0,483,484,1,0,0,0,
  	484,486,1,0,0,0,485,483,1,0,0,0,486,487,5,21,0,0,487,490,1,0,0,0,488,
  	490,3,42,21,0,489,466,1,0,0,0,489,470,1,0,0,0,489,471,1,0,0,0,489,472,
  	1,0,0,0,489,488,1,0,0,0,490,55,1,0,0,0,491,493,3,100,50,0,492,491,1,0,
  	0,0,493,496,1,0,0,0,494,492,1,0,0,0,494,495,1,0,0,0,495,497,1,0,0,0,496,
  	494,1,0,0,0,497,501,5,27,0,0,498,500,3,100,50,0,499,498,1,0,0,0,500,503,
  	1,0,0,0,501,499,1,0,0,0,501,502,1,0,0,0,502,504,1,0,0,0,503,501,1,0,0,
  	0,504,508,5,28,0,0,505,507,3,100,50,0,506,505,1,0,0,0,507,510,1,0,0,0,
  	508,506,1,0,0,0,508,509,1,0,0,0,509,57,1,0,0,0,510,508,1,0,0,0,511,512,
  	5,29,0,0,512,514,7,4,0,0,513,515,3,100,50,0,514,513,1,0,0,0,515,516,1,
  	0,0,0,516,514,1,0,0,0,516,517,1,0,0,0,517,518,1,0,0,0,518,522,3,96,48,
  	0,519,521,3,100,50,0,520,519,1,0,0,0,521,524,1,0,0,0,522,520,1,0,0,0,
  	522,523,1,0,0,0,523,526,1,0,0,0,524,522,1,0,0,0,525,527,3,62,31,0,526,
  	525,1,0,0,0,526,527,1,0,0,0,527,531,1,0,0,0,528,530,3,100,50,0,529,528,
  	1,0,0,0,530,533,1,0,0,0,531,529,1,0,0,0,531,532,1,0,0,0,532,534,1,0,0,
  	0,533,531,1,0,0,0,534,535,5,23,0,0,535,59,1,0,0,0,536,537,5,29,0,0,537,
  	539,7,4,0,0,538,540,3,100,50,0,539,538,1,0,0,0,540,541,1,0,0,0,541,539,
  	1,0,0,0,541,542,1,0,0,0,542,543,1,0,0,0,543,547,3,96,48,0,544,546,3,100,
  	50,0,545,544,1,0,0,0,546,549,1,0,0,0,547,545,1,0,0,0,547,548,1,0,0,0,
  	548,551,1,0,0,0,549,547,1,0,0,0,550,552,3,62,31,0,551,550,1,0,0,0,551,
  	552,1,0,0,0,552,556,1,0,0,0,553,555,3,100,50,0,554,553,1,0,0,0,555,558,
  	1,0,0,0,556,554,1,0,0,0,556,557,1,0,0,0,557,559,1,0,0,0,558,556,1,0,0,
  	0,559,560,5,23,0,0,560,61,1,0,0,0,561,565,5,1,0,0,562,564,3,100,50,0,
  	563,562,1,0,0,0,564,567,1,0,0,0,565,563,1,0,0,0,565,566,1,0,0,0,566,569,
  	1,0,0,0,567,565,1,0,0,0,568,570,3,34,17,0,569,568,1,0,0,0,569,570,1,0,
  	0,0,570,574,1,0,0,0,571,573,3,100,50,0,572,571,1,0,0,0,573,576,1,0,0,
  	0,574,572,1,0,0,0,574,575,1,0,0,0,575,577,1,0,0,0,576,574,1,0,0,0,577,
  	578,5,2,0,0,578,63,1,0,0,0,579,580,3,96,48,0,580,581,5,20,0,0,581,582,
  	3,96,48,0,582,586,5,21,0,0,583,585,3,100,50,0,584,583,1,0,0,0,585,588,
  	1,0,0,0,586,584,1,0,0,0,586,587,1,0,0,0,587,589,1,0,0,0,588,586,1,0,0,
  	0,589,590,5,23,0,0,590,65,1,0,0,0,591,592,3,96,48,0,592,593,5,20,0,0,
  	593,594,3,96,48,0,594,595,5,21,0,0,595,67,1,0,0,0,596,597,5,29,0,0,597,
  	599,7,4,0,0,598,600,3,100,50,0,599,598,1,0,0,0,600,601,1,0,0,0,601,599,
  	1,0,0,0,601,602,1,0,0,0,602,603,1,0,0,0,603,607,3,96,48,0,604,606,3,100,
  	50,0,605,604,1,0,0,0,606,609,1,0,0,0,607,605,1,0,0,0,607,608,1,0,0,0,
  	608,611,1,0,0,0,609,607,1,0,0,0,610,612,3,62,31,0,611,610,1,0,0,0,611,
  	612,1,0,0,0,612,69,1,0,0,0,613,615,5,35,0,0,614,616,3,100,50,0,615,614,
  	1,0,0,0,616,617,1,0,0,0,617,615,1,0,0,0,617,618,1,0,0,0,618,619,1,0,0,
  	0,619,623,3,38,19,0,620,622,3,100,50,0,621,620,1,0,0,0,622,625,1,0,0,
  	0,623,621,1,0,0,0,623,624,1,0,0,0,624,71,1,0,0,0,625,623,1,0,0,0,626,
  	628,5,36,0,0,627,629,3,100,50,0,628,627,1,0,0,0,629,630,1,0,0,0,630,628,
  	1,0,0,0,630,631,1,0,0,0,631,632,1,0,0,0,632,633,5,29,0,0,633,635,7,4,
  	0,0,634,636,3,100,50,0,635,634,1,0,0,0,636,637,1,0,0,0,637,635,1,0,0,
  	0,637,638,1,0,0,0,638,639,1,0,0,0,639,643,3,96,48,0,640,642,3,100,50,
  	0,641,640,1,0,0,0,642,645,1,0,0,0,643,641,1,0,0,0,643,644,1,0,0,0,644,
  	73,1,0,0,0,645,643,1,0,0,0,646,650,5,37,0,0,647,649,3,100,50,0,648,647,
  	1,0,0,0,649,652,1,0,0,0,650,648,1,0,0,0,650,651,1,0,0,0,651,653,1,0,0,
  	0,652,650,1,0,0,0,653,654,5,29,0,0,654,656,7,4,0,0,655,657,3,100,50,0,
  	656,655,1,0,0,0,657,658,1,0,0,0,658,656,1,0,0,0,658,659,1,0,0,0,659,660,
  	1,0,0,0,660,664,3,96,48,0,661,663,3,100,50,0,662,661,1,0,0,0,663,666,
  	1,0,0,0,664,662,1,0,0,0,664,665,1,0,0,0,665,667,1,0,0,0,666,664,1,0,0,
  	0,667,668,5,23,0,0,668,75,1,0,0,0,669,670,5,29,0,0,670,674,3,96,48,0,
  	671,673,3,100,50,0,672,671,1,0,0,0,673,676,1,0,0,0,674,672,1,0,0,0,674,
  	675,1,0,0,0,675,679,1,0,0,0,676,674,1,0,0,0,677,680,3,78,39,0,678,680,
  	5,23,0,0,679,677,1,0,0,0,679,678,1,0,0,0,680,77,1,0,0,0,681,685,5,1,0,
  	0,682,684,3,100,50,0,683,682,1,0,0,0,684,687,1,0,0,0,685,683,1,0,0,0,
  	685,686,1,0,0,0,686,692,1,0,0,0,687,685,1,0,0,0,688,691,3,2,1,0,689,691,
  	3,36,18,0,690,688,1,0,0,0,690,689,1,0,0,0,691,694,1,0,0,0,692,690,1,0,
  	0,0,692,693,1,0,0,0,693,698,1,0,0,0,694,692,1,0,0,0,695,697,3,100,50,
  	0,696,695,1,0,0,0,697,700,1,0,0,0,698,696,1,0,0,0,698,699,1,0,0,0,699,
  	701,1,0,0,0,700,698,1,0,0,0,701,725,5,2,0,0,702,703,3,40,20,0,703,707,
  	5,1,0,0,704,706,3,100,50,0,705,704,1,0,0,0,706,709,1,0,0,0,707,705,1,
  	0,0,0,707,708,1,0,0,0,708,713,1,0,0,0,709,707,1,0,0,0,710,712,3,2,1,0,
  	711,710,1,0,0,0,712,715,1,0,0,0,713,711,1,0,0,0,713,714,1,0,0,0,714,719,
  	1,0,0,0,715,713,1,0,0,0,716,718,3,100,50,0,717,716,1,0,0,0,718,721,1,
  	0,0,0,719,717,1,0,0,0,719,720,1,0,0,0,720,722,1,0,0,0,721,719,1,0,0,0,
  	722,723,5,2,0,0,723,725,1,0,0,0,724,681,1,0,0,0,724,702,1,0,0,0,725,79,
  	1,0,0,0,726,730,5,38,0,0,727,729,3,100,50,0,728,727,1,0,0,0,729,732,1,
  	0,0,0,730,728,1,0,0,0,730,731,1,0,0,0,731,733,1,0,0,0,732,730,1,0,0,0,
  	733,734,5,39,0,0,734,738,3,96,48,0,735,737,3,100,50,0,736,735,1,0,0,0,
  	737,740,1,0,0,0,738,736,1,0,0,0,738,739,1,0,0,0,739,749,1,0,0,0,740,738,
  	1,0,0,0,741,745,5,3,0,0,742,744,3,100,50,0,743,742,1,0,0,0,744,747,1,
  	0,0,0,745,743,1,0,0,0,745,746,1,0,0,0,746,748,1,0,0,0,747,745,1,0,0,0,
  	748,750,3,42,21,0,749,741,1,0,0,0,749,750,1,0,0,0,750,754,1,0,0,0,751,
  	753,3,100,50,0,752,751,1,0,0,0,753,756,1,0,0,0,754,752,1,0,0,0,754,755,
  	1,0,0,0,755,757,1,0,0,0,756,754,1,0,0,0,757,758,5,21,0,0,758,81,1,0,0,
  	0,759,761,5,9,0,0,760,762,3,98,49,0,761,760,1,0,0,0,762,763,1,0,0,0,763,
  	761,1,0,0,0,763,764,1,0,0,0,764,83,1,0,0,0,765,766,5,67,0,0,766,85,1,
  	0,0,0,767,768,3,84,42,0,768,769,5,40,0,0,769,87,1,0,0,0,770,771,3,84,
  	42,0,771,772,3,90,45,0,772,89,1,0,0,0,773,774,7,5,0,0,774,91,1,0,0,0,
  	775,779,5,66,0,0,776,778,3,100,50,0,777,776,1,0,0,0,778,781,1,0,0,0,779,
  	777,1,0,0,0,779,780,1,0,0,0,780,784,1,0,0,0,781,779,1,0,0,0,782,785,3,
  	94,47,0,783,785,5,74,0,0,784,782,1,0,0,0,784,783,1,0,0,0,785,789,1,0,
  	0,0,786,788,3,100,50,0,787,786,1,0,0,0,788,791,1,0,0,0,789,787,1,0,0,
  	0,789,790,1,0,0,0,790,792,1,0,0,0,791,789,1,0,0,0,792,793,5,21,0,0,793,
  	93,1,0,0,0,794,795,5,68,0,0,795,95,1,0,0,0,796,797,5,69,0,0,797,97,1,
  	0,0,0,798,799,5,70,0,0,799,99,1,0,0,0,800,801,5,71,0,0,801,101,1,0,0,
  	0,105,105,111,117,128,134,141,145,150,159,166,172,181,187,194,200,202,
  	208,214,216,220,229,241,248,255,262,265,273,281,288,293,302,309,315,321,
  	325,331,338,343,348,352,358,364,369,381,387,394,404,408,413,422,429,435,
  	443,449,457,463,468,476,483,489,494,501,508,516,522,526,531,541,547,551,
  	556,565,569,574,586,601,607,611,617,623,630,637,643,650,658,664,674,679,
  	685,690,692,698,707,713,719,724,730,738,745,749,754,763,779,784,789
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  css3ParserStaticData = staticData.release();
}

}

CSS3Parser::CSS3Parser(TokenStream *input) : CSS3Parser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

CSS3Parser::CSS3Parser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  CSS3Parser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *css3ParserStaticData->atn, css3ParserStaticData->decisionToDFA, css3ParserStaticData->sharedContextCache, options);
}

CSS3Parser::~CSS3Parser() {
  delete _interpreter;
}

const atn::ATN& CSS3Parser::getATN() const {
  return *css3ParserStaticData->atn;
}

std::string CSS3Parser::getGrammarFileName() const {
  return "CSS3.g4";
}

const std::vector<std::string>& CSS3Parser::getRuleNames() const {
  return css3ParserStaticData->ruleNames;
}

const dfa::Vocabulary& CSS3Parser::getVocabulary() const {
  return css3ParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView CSS3Parser::getSerializedATN() const {
  return css3ParserStaticData->serializedATN;
}


//----------------- StylesheetContext ------------------------------------------------------------------

CSS3Parser::StylesheetContext::StylesheetContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CSS3Parser::StylesheetContext::EOF() {
  return getToken(CSS3Parser::EOF, 0);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::StylesheetContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::StylesheetContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}

std::vector<CSS3Parser::StyleRuleContext *> CSS3Parser::StylesheetContext::styleRule() {
  return getRuleContexts<CSS3Parser::StyleRuleContext>();
}

CSS3Parser::StyleRuleContext* CSS3Parser::StylesheetContext::styleRule(size_t i) {
  return getRuleContext<CSS3Parser::StyleRuleContext>(i);
}


size_t CSS3Parser::StylesheetContext::getRuleIndex() const {
  return CSS3Parser::RuleStylesheet;
}

void CSS3Parser::StylesheetContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStylesheet(this);
}

void CSS3Parser::StylesheetContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStylesheet(this);
}

CSS3Parser::StylesheetContext* CSS3Parser::stylesheet() {
  StylesheetContext *_localctx = _tracker.createInstance<StylesheetContext>(_ctx, getState());
  enterRule(_localctx, 0, CSS3Parser::RuleStylesheet);
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
    setState(105);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(102);
        ws(); 
      }
      setState(107);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
    }
    setState(111);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 8) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 8)) & 2305843009752680463) != 0)) {
      setState(108);
      styleRule();
      setState(113);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(117);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(114);
      ws();
      setState(119);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(120);
    match(CSS3Parser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StyleRuleContext ------------------------------------------------------------------

CSS3Parser::StyleRuleContext::StyleRuleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::RuleSetContext* CSS3Parser::StyleRuleContext::ruleSet() {
  return getRuleContext<CSS3Parser::RuleSetContext>(0);
}

CSS3Parser::AtRuleContext* CSS3Parser::StyleRuleContext::atRule() {
  return getRuleContext<CSS3Parser::AtRuleContext>(0);
}

CSS3Parser::ChtlTemplateUsageContext* CSS3Parser::StyleRuleContext::chtlTemplateUsage() {
  return getRuleContext<CSS3Parser::ChtlTemplateUsageContext>(0);
}

CSS3Parser::ChtlCustomUsageContext* CSS3Parser::StyleRuleContext::chtlCustomUsage() {
  return getRuleContext<CSS3Parser::ChtlCustomUsageContext>(0);
}

CSS3Parser::ChtlVariableUsageContext* CSS3Parser::StyleRuleContext::chtlVariableUsage() {
  return getRuleContext<CSS3Parser::ChtlVariableUsageContext>(0);
}

CSS3Parser::ChtlOriginEmbedContext* CSS3Parser::StyleRuleContext::chtlOriginEmbed() {
  return getRuleContext<CSS3Parser::ChtlOriginEmbedContext>(0);
}


size_t CSS3Parser::StyleRuleContext::getRuleIndex() const {
  return CSS3Parser::RuleStyleRule;
}

void CSS3Parser::StyleRuleContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStyleRule(this);
}

void CSS3Parser::StyleRuleContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStyleRule(this);
}

CSS3Parser::StyleRuleContext* CSS3Parser::styleRule() {
  StyleRuleContext *_localctx = _tracker.createInstance<StyleRuleContext>(_ctx, getState());
  enterRule(_localctx, 2, CSS3Parser::RuleStyleRule);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(128);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(122);
      ruleSet();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(123);
      atRule();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(124);
      chtlTemplateUsage();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(125);
      chtlCustomUsage();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(126);
      chtlVariableUsage();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(127);
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

//----------------- RuleSetContext ------------------------------------------------------------------

CSS3Parser::RuleSetContext::RuleSetContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::SelectorListContext* CSS3Parser::RuleSetContext::selectorList() {
  return getRuleContext<CSS3Parser::SelectorListContext>(0);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::RuleSetContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::RuleSetContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}

CSS3Parser::DeclarationListContext* CSS3Parser::RuleSetContext::declarationList() {
  return getRuleContext<CSS3Parser::DeclarationListContext>(0);
}


size_t CSS3Parser::RuleSetContext::getRuleIndex() const {
  return CSS3Parser::RuleRuleSet;
}

void CSS3Parser::RuleSetContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRuleSet(this);
}

void CSS3Parser::RuleSetContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRuleSet(this);
}

CSS3Parser::RuleSetContext* CSS3Parser::ruleSet() {
  RuleSetContext *_localctx = _tracker.createInstance<RuleSetContext>(_ctx, getState());
  enterRule(_localctx, 4, CSS3Parser::RuleRuleSet);
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
    setState(130);
    selectorList();
    setState(134);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(131);
      ws();
      setState(136);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(137);
    match(CSS3Parser::T__0);
    setState(141);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(138);
        ws(); 
      }
      setState(143);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
    }
    setState(145);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 29) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 29)) & 1099511628481) != 0)) {
      setState(144);
      declarationList();
    }
    setState(150);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(147);
      ws();
      setState(152);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(153);
    match(CSS3Parser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SelectorListContext ------------------------------------------------------------------

CSS3Parser::SelectorListContext::SelectorListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CSS3Parser::SelectorContext *> CSS3Parser::SelectorListContext::selector() {
  return getRuleContexts<CSS3Parser::SelectorContext>();
}

CSS3Parser::SelectorContext* CSS3Parser::SelectorListContext::selector(size_t i) {
  return getRuleContext<CSS3Parser::SelectorContext>(i);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::SelectorListContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::SelectorListContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}


size_t CSS3Parser::SelectorListContext::getRuleIndex() const {
  return CSS3Parser::RuleSelectorList;
}

void CSS3Parser::SelectorListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSelectorList(this);
}

void CSS3Parser::SelectorListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSelectorList(this);
}

CSS3Parser::SelectorListContext* CSS3Parser::selectorList() {
  SelectorListContext *_localctx = _tracker.createInstance<SelectorListContext>(_ctx, getState());
  enterRule(_localctx, 6, CSS3Parser::RuleSelectorList);
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
    setState(155);
    selector();
    setState(172);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(159);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == CSS3Parser::WS) {
          setState(156);
          ws();
          setState(161);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(162);
        match(CSS3Parser::T__2);
        setState(166);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == CSS3Parser::WS) {
          setState(163);
          ws();
          setState(168);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(169);
        selector(); 
      }
      setState(174);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SelectorContext ------------------------------------------------------------------

CSS3Parser::SelectorContext::SelectorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CSS3Parser::SimpleSelectorContext *> CSS3Parser::SelectorContext::simpleSelector() {
  return getRuleContexts<CSS3Parser::SimpleSelectorContext>();
}

CSS3Parser::SimpleSelectorContext* CSS3Parser::SelectorContext::simpleSelector(size_t i) {
  return getRuleContext<CSS3Parser::SimpleSelectorContext>(i);
}

std::vector<CSS3Parser::CombinatorContext *> CSS3Parser::SelectorContext::combinator() {
  return getRuleContexts<CSS3Parser::CombinatorContext>();
}

CSS3Parser::CombinatorContext* CSS3Parser::SelectorContext::combinator(size_t i) {
  return getRuleContext<CSS3Parser::CombinatorContext>(i);
}


size_t CSS3Parser::SelectorContext::getRuleIndex() const {
  return CSS3Parser::RuleSelector;
}

void CSS3Parser::SelectorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSelector(this);
}

void CSS3Parser::SelectorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSelector(this);
}

CSS3Parser::SelectorContext* CSS3Parser::selector() {
  SelectorContext *_localctx = _tracker.createInstance<SelectorContext>(_ctx, getState());
  enterRule(_localctx, 8, CSS3Parser::RuleSelector);

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
    setState(175);
    simpleSelector();
    setState(181);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(176);
        combinator();
        setState(177);
        simpleSelector(); 
      }
      setState(183);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CombinatorContext ------------------------------------------------------------------

CSS3Parser::CombinatorContext::CombinatorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::CombinatorContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::CombinatorContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}


size_t CSS3Parser::CombinatorContext::getRuleIndex() const {
  return CSS3Parser::RuleCombinator;
}

void CSS3Parser::CombinatorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCombinator(this);
}

void CSS3Parser::CombinatorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCombinator(this);
}

CSS3Parser::CombinatorContext* CSS3Parser::combinator() {
  CombinatorContext *_localctx = _tracker.createInstance<CombinatorContext>(_ctx, getState());
  enterRule(_localctx, 10, CSS3Parser::RuleCombinator);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(202);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(187);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CSS3Parser::WS) {
        setState(184);
        ws();
        setState(189);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(190);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 240) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(194);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CSS3Parser::WS) {
        setState(191);
        ws();
        setState(196);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(198); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(197);
        ws();
        setState(200); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == CSS3Parser::WS);
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

//----------------- SimpleSelectorContext ------------------------------------------------------------------

CSS3Parser::SimpleSelectorContext::SimpleSelectorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::TypeSelectorContext* CSS3Parser::SimpleSelectorContext::typeSelector() {
  return getRuleContext<CSS3Parser::TypeSelectorContext>(0);
}

std::vector<CSS3Parser::QualifiersContext *> CSS3Parser::SimpleSelectorContext::qualifiers() {
  return getRuleContexts<CSS3Parser::QualifiersContext>();
}

CSS3Parser::QualifiersContext* CSS3Parser::SimpleSelectorContext::qualifiers(size_t i) {
  return getRuleContext<CSS3Parser::QualifiersContext>(i);
}


size_t CSS3Parser::SimpleSelectorContext::getRuleIndex() const {
  return CSS3Parser::RuleSimpleSelector;
}

void CSS3Parser::SimpleSelectorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSimpleSelector(this);
}

void CSS3Parser::SimpleSelectorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSimpleSelector(this);
}

CSS3Parser::SimpleSelectorContext* CSS3Parser::simpleSelector() {
  SimpleSelectorContext *_localctx = _tracker.createInstance<SimpleSelectorContext>(_ctx, getState());
  enterRule(_localctx, 12, CSS3Parser::RuleSimpleSelector);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(216);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CSS3Parser::T__7:
      case CSS3Parser::IDENT: {
        enterOuterAlt(_localctx, 1);
        setState(204);
        typeSelector();
        setState(208);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 4722176) != 0)) {
          setState(205);
          qualifiers();
          setState(210);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        break;
      }

      case CSS3Parser::T__8:
      case CSS3Parser::T__9:
      case CSS3Parser::T__10:
      case CSS3Parser::T__18:
      case CSS3Parser::T__21: {
        enterOuterAlt(_localctx, 2);
        setState(212); 
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
          setState(211);
          qualifiers();
          setState(214); 
          _errHandler->sync(this);
          _la = _input->LA(1);
        } while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 4722176) != 0));
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

//----------------- TypeSelectorContext ------------------------------------------------------------------

CSS3Parser::TypeSelectorContext::TypeSelectorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::TagNameContext* CSS3Parser::TypeSelectorContext::tagName() {
  return getRuleContext<CSS3Parser::TagNameContext>(0);
}


size_t CSS3Parser::TypeSelectorContext::getRuleIndex() const {
  return CSS3Parser::RuleTypeSelector;
}

void CSS3Parser::TypeSelectorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeSelector(this);
}

void CSS3Parser::TypeSelectorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeSelector(this);
}

CSS3Parser::TypeSelectorContext* CSS3Parser::typeSelector() {
  TypeSelectorContext *_localctx = _tracker.createInstance<TypeSelectorContext>(_ctx, getState());
  enterRule(_localctx, 14, CSS3Parser::RuleTypeSelector);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(220);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CSS3Parser::IDENT: {
        enterOuterAlt(_localctx, 1);
        setState(218);
        tagName();
        break;
      }

      case CSS3Parser::T__7: {
        enterOuterAlt(_localctx, 2);
        setState(219);
        match(CSS3Parser::T__7);
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

//----------------- TagNameContext ------------------------------------------------------------------

CSS3Parser::TagNameContext::TagNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::IdentContext* CSS3Parser::TagNameContext::ident() {
  return getRuleContext<CSS3Parser::IdentContext>(0);
}


size_t CSS3Parser::TagNameContext::getRuleIndex() const {
  return CSS3Parser::RuleTagName;
}

void CSS3Parser::TagNameContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTagName(this);
}

void CSS3Parser::TagNameContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTagName(this);
}

CSS3Parser::TagNameContext* CSS3Parser::tagName() {
  TagNameContext *_localctx = _tracker.createInstance<TagNameContext>(_ctx, getState());
  enterRule(_localctx, 16, CSS3Parser::RuleTagName);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(222);
    ident();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QualifiersContext ------------------------------------------------------------------

CSS3Parser::QualifiersContext::QualifiersContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::HashSelectorContext* CSS3Parser::QualifiersContext::hashSelector() {
  return getRuleContext<CSS3Parser::HashSelectorContext>(0);
}

CSS3Parser::ClassSelectorContext* CSS3Parser::QualifiersContext::classSelector() {
  return getRuleContext<CSS3Parser::ClassSelectorContext>(0);
}

CSS3Parser::AttributeSelectorContext* CSS3Parser::QualifiersContext::attributeSelector() {
  return getRuleContext<CSS3Parser::AttributeSelectorContext>(0);
}

CSS3Parser::PseudoClassContext* CSS3Parser::QualifiersContext::pseudoClass() {
  return getRuleContext<CSS3Parser::PseudoClassContext>(0);
}

CSS3Parser::PseudoElementContext* CSS3Parser::QualifiersContext::pseudoElement() {
  return getRuleContext<CSS3Parser::PseudoElementContext>(0);
}


size_t CSS3Parser::QualifiersContext::getRuleIndex() const {
  return CSS3Parser::RuleQualifiers;
}

void CSS3Parser::QualifiersContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterQualifiers(this);
}

void CSS3Parser::QualifiersContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitQualifiers(this);
}

CSS3Parser::QualifiersContext* CSS3Parser::qualifiers() {
  QualifiersContext *_localctx = _tracker.createInstance<QualifiersContext>(_ctx, getState());
  enterRule(_localctx, 18, CSS3Parser::RuleQualifiers);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(229);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CSS3Parser::T__8: {
        enterOuterAlt(_localctx, 1);
        setState(224);
        hashSelector();
        break;
      }

      case CSS3Parser::T__9: {
        enterOuterAlt(_localctx, 2);
        setState(225);
        classSelector();
        break;
      }

      case CSS3Parser::T__10: {
        enterOuterAlt(_localctx, 3);
        setState(226);
        attributeSelector();
        break;
      }

      case CSS3Parser::T__18: {
        enterOuterAlt(_localctx, 4);
        setState(227);
        pseudoClass();
        break;
      }

      case CSS3Parser::T__21: {
        enterOuterAlt(_localctx, 5);
        setState(228);
        pseudoElement();
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

//----------------- HashSelectorContext ------------------------------------------------------------------

CSS3Parser::HashSelectorContext::HashSelectorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::IdentContext* CSS3Parser::HashSelectorContext::ident() {
  return getRuleContext<CSS3Parser::IdentContext>(0);
}


size_t CSS3Parser::HashSelectorContext::getRuleIndex() const {
  return CSS3Parser::RuleHashSelector;
}

void CSS3Parser::HashSelectorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterHashSelector(this);
}

void CSS3Parser::HashSelectorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitHashSelector(this);
}

CSS3Parser::HashSelectorContext* CSS3Parser::hashSelector() {
  HashSelectorContext *_localctx = _tracker.createInstance<HashSelectorContext>(_ctx, getState());
  enterRule(_localctx, 20, CSS3Parser::RuleHashSelector);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(231);
    match(CSS3Parser::T__8);
    setState(232);
    ident();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassSelectorContext ------------------------------------------------------------------

CSS3Parser::ClassSelectorContext::ClassSelectorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::IdentContext* CSS3Parser::ClassSelectorContext::ident() {
  return getRuleContext<CSS3Parser::IdentContext>(0);
}


size_t CSS3Parser::ClassSelectorContext::getRuleIndex() const {
  return CSS3Parser::RuleClassSelector;
}

void CSS3Parser::ClassSelectorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterClassSelector(this);
}

void CSS3Parser::ClassSelectorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitClassSelector(this);
}

CSS3Parser::ClassSelectorContext* CSS3Parser::classSelector() {
  ClassSelectorContext *_localctx = _tracker.createInstance<ClassSelectorContext>(_ctx, getState());
  enterRule(_localctx, 22, CSS3Parser::RuleClassSelector);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(234);
    match(CSS3Parser::T__9);
    setState(235);
    ident();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AttributeSelectorContext ------------------------------------------------------------------

CSS3Parser::AttributeSelectorContext::AttributeSelectorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::IdentContext* CSS3Parser::AttributeSelectorContext::ident() {
  return getRuleContext<CSS3Parser::IdentContext>(0);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::AttributeSelectorContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::AttributeSelectorContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}

CSS3Parser::AttributeOperatorContext* CSS3Parser::AttributeSelectorContext::attributeOperator() {
  return getRuleContext<CSS3Parser::AttributeOperatorContext>(0);
}

CSS3Parser::AttributeValueContext* CSS3Parser::AttributeSelectorContext::attributeValue() {
  return getRuleContext<CSS3Parser::AttributeValueContext>(0);
}


size_t CSS3Parser::AttributeSelectorContext::getRuleIndex() const {
  return CSS3Parser::RuleAttributeSelector;
}

void CSS3Parser::AttributeSelectorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAttributeSelector(this);
}

void CSS3Parser::AttributeSelectorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAttributeSelector(this);
}

CSS3Parser::AttributeSelectorContext* CSS3Parser::attributeSelector() {
  AttributeSelectorContext *_localctx = _tracker.createInstance<AttributeSelectorContext>(_ctx, getState());
  enterRule(_localctx, 24, CSS3Parser::RuleAttributeSelector);
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
    setState(237);
    match(CSS3Parser::T__10);
    setState(241);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(238);
      ws();
      setState(243);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(244);
    ident();
    setState(248);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(245);
      ws();
      setState(250);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(265);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 516096) != 0)) {
      setState(251);
      attributeOperator();
      setState(255);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CSS3Parser::WS) {
        setState(252);
        ws();
        setState(257);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(258);
      attributeValue();
      setState(262);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CSS3Parser::WS) {
        setState(259);
        ws();
        setState(264);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(267);
    match(CSS3Parser::T__11);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AttributeOperatorContext ------------------------------------------------------------------

CSS3Parser::AttributeOperatorContext::AttributeOperatorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CSS3Parser::AttributeOperatorContext::getRuleIndex() const {
  return CSS3Parser::RuleAttributeOperator;
}

void CSS3Parser::AttributeOperatorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAttributeOperator(this);
}

void CSS3Parser::AttributeOperatorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAttributeOperator(this);
}

CSS3Parser::AttributeOperatorContext* CSS3Parser::attributeOperator() {
  AttributeOperatorContext *_localctx = _tracker.createInstance<AttributeOperatorContext>(_ctx, getState());
  enterRule(_localctx, 26, CSS3Parser::RuleAttributeOperator);
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
    setState(269);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 516096) != 0))) {
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

//----------------- AttributeValueContext ------------------------------------------------------------------

CSS3Parser::AttributeValueContext::AttributeValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::IdentContext* CSS3Parser::AttributeValueContext::ident() {
  return getRuleContext<CSS3Parser::IdentContext>(0);
}

CSS3Parser::StringContext* CSS3Parser::AttributeValueContext::string() {
  return getRuleContext<CSS3Parser::StringContext>(0);
}


size_t CSS3Parser::AttributeValueContext::getRuleIndex() const {
  return CSS3Parser::RuleAttributeValue;
}

void CSS3Parser::AttributeValueContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAttributeValue(this);
}

void CSS3Parser::AttributeValueContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAttributeValue(this);
}

CSS3Parser::AttributeValueContext* CSS3Parser::attributeValue() {
  AttributeValueContext *_localctx = _tracker.createInstance<AttributeValueContext>(_ctx, getState());
  enterRule(_localctx, 28, CSS3Parser::RuleAttributeValue);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(273);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CSS3Parser::IDENT: {
        enterOuterAlt(_localctx, 1);
        setState(271);
        ident();
        break;
      }

      case CSS3Parser::STRING: {
        enterOuterAlt(_localctx, 2);
        setState(272);
        string();
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

//----------------- PseudoClassContext ------------------------------------------------------------------

CSS3Parser::PseudoClassContext::PseudoClassContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::IdentContext* CSS3Parser::PseudoClassContext::ident() {
  return getRuleContext<CSS3Parser::IdentContext>(0);
}

CSS3Parser::ExpressionContext* CSS3Parser::PseudoClassContext::expression() {
  return getRuleContext<CSS3Parser::ExpressionContext>(0);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::PseudoClassContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::PseudoClassContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}


size_t CSS3Parser::PseudoClassContext::getRuleIndex() const {
  return CSS3Parser::RulePseudoClass;
}

void CSS3Parser::PseudoClassContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPseudoClass(this);
}

void CSS3Parser::PseudoClassContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPseudoClass(this);
}

CSS3Parser::PseudoClassContext* CSS3Parser::pseudoClass() {
  PseudoClassContext *_localctx = _tracker.createInstance<PseudoClassContext>(_ctx, getState());
  enterRule(_localctx, 30, CSS3Parser::RulePseudoClass);
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
    setState(275);
    match(CSS3Parser::T__18);
    setState(276);
    ident();
    setState(293);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CSS3Parser::T__19) {
      setState(277);
      match(CSS3Parser::T__19);
      setState(281);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CSS3Parser::WS) {
        setState(278);
        ws();
        setState(283);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(284);
      expression();
      setState(288);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CSS3Parser::WS) {
        setState(285);
        ws();
        setState(290);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(291);
      match(CSS3Parser::T__20);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PseudoElementContext ------------------------------------------------------------------

CSS3Parser::PseudoElementContext::PseudoElementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::IdentContext* CSS3Parser::PseudoElementContext::ident() {
  return getRuleContext<CSS3Parser::IdentContext>(0);
}


size_t CSS3Parser::PseudoElementContext::getRuleIndex() const {
  return CSS3Parser::RulePseudoElement;
}

void CSS3Parser::PseudoElementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPseudoElement(this);
}

void CSS3Parser::PseudoElementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPseudoElement(this);
}

CSS3Parser::PseudoElementContext* CSS3Parser::pseudoElement() {
  PseudoElementContext *_localctx = _tracker.createInstance<PseudoElementContext>(_ctx, getState());
  enterRule(_localctx, 32, CSS3Parser::RulePseudoElement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(295);
    match(CSS3Parser::T__21);
    setState(296);
    ident();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclarationListContext ------------------------------------------------------------------

CSS3Parser::DeclarationListContext::DeclarationListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CSS3Parser::DeclarationContext *> CSS3Parser::DeclarationListContext::declaration() {
  return getRuleContexts<CSS3Parser::DeclarationContext>();
}

CSS3Parser::DeclarationContext* CSS3Parser::DeclarationListContext::declaration(size_t i) {
  return getRuleContext<CSS3Parser::DeclarationContext>(i);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::DeclarationListContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::DeclarationListContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}


size_t CSS3Parser::DeclarationListContext::getRuleIndex() const {
  return CSS3Parser::RuleDeclarationList;
}

void CSS3Parser::DeclarationListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDeclarationList(this);
}

void CSS3Parser::DeclarationListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDeclarationList(this);
}

CSS3Parser::DeclarationListContext* CSS3Parser::declarationList() {
  DeclarationListContext *_localctx = _tracker.createInstance<DeclarationListContext>(_ctx, getState());
  enterRule(_localctx, 34, CSS3Parser::RuleDeclarationList);
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
    setState(298);
    declaration();
    setState(315);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(302);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == CSS3Parser::WS) {
          setState(299);
          ws();
          setState(304);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(305);
        match(CSS3Parser::T__22);
        setState(309);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == CSS3Parser::WS) {
          setState(306);
          ws();
          setState(311);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(312);
        declaration(); 
      }
      setState(317);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx);
    }
    setState(321);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(318);
        ws(); 
      }
      setState(323);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx);
    }
    setState(325);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CSS3Parser::T__22) {
      setState(324);
      match(CSS3Parser::T__22);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclarationContext ------------------------------------------------------------------

CSS3Parser::DeclarationContext::DeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::PropertyContext* CSS3Parser::DeclarationContext::property() {
  return getRuleContext<CSS3Parser::PropertyContext>(0);
}

CSS3Parser::ValueListContext* CSS3Parser::DeclarationContext::valueList() {
  return getRuleContext<CSS3Parser::ValueListContext>(0);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::DeclarationContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::DeclarationContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}

CSS3Parser::PrioContext* CSS3Parser::DeclarationContext::prio() {
  return getRuleContext<CSS3Parser::PrioContext>(0);
}

CSS3Parser::ChtlStyleGroupUsageContext* CSS3Parser::DeclarationContext::chtlStyleGroupUsage() {
  return getRuleContext<CSS3Parser::ChtlStyleGroupUsageContext>(0);
}

CSS3Parser::ChtlDeletePropertyContext* CSS3Parser::DeclarationContext::chtlDeleteProperty() {
  return getRuleContext<CSS3Parser::ChtlDeletePropertyContext>(0);
}

CSS3Parser::ChtlInheritContext* CSS3Parser::DeclarationContext::chtlInherit() {
  return getRuleContext<CSS3Parser::ChtlInheritContext>(0);
}


size_t CSS3Parser::DeclarationContext::getRuleIndex() const {
  return CSS3Parser::RuleDeclaration;
}

void CSS3Parser::DeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDeclaration(this);
}

void CSS3Parser::DeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDeclaration(this);
}

CSS3Parser::DeclarationContext* CSS3Parser::declaration() {
  DeclarationContext *_localctx = _tracker.createInstance<DeclarationContext>(_ctx, getState());
  enterRule(_localctx, 36, CSS3Parser::RuleDeclaration);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(348);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CSS3Parser::T__37:
      case CSS3Parser::IDENT: {
        enterOuterAlt(_localctx, 1);
        setState(327);
        property();
        setState(331);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == CSS3Parser::WS) {
          setState(328);
          ws();
          setState(333);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(334);
        match(CSS3Parser::T__18);
        setState(338);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == CSS3Parser::WS) {
          setState(335);
          ws();
          setState(340);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(341);
        valueList();
        setState(343);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx)) {
        case 1: {
          setState(342);
          prio();
          break;
        }

        default:
          break;
        }
        break;
      }

      case CSS3Parser::T__28: {
        enterOuterAlt(_localctx, 2);
        setState(345);
        chtlStyleGroupUsage();
        break;
      }

      case CSS3Parser::T__34: {
        enterOuterAlt(_localctx, 3);
        setState(346);
        chtlDeleteProperty();
        break;
      }

      case CSS3Parser::T__35: {
        enterOuterAlt(_localctx, 4);
        setState(347);
        chtlInherit();
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

//----------------- PropertyContext ------------------------------------------------------------------

CSS3Parser::PropertyContext::PropertyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::IdentContext* CSS3Parser::PropertyContext::ident() {
  return getRuleContext<CSS3Parser::IdentContext>(0);
}

CSS3Parser::VarContext* CSS3Parser::PropertyContext::var() {
  return getRuleContext<CSS3Parser::VarContext>(0);
}


size_t CSS3Parser::PropertyContext::getRuleIndex() const {
  return CSS3Parser::RuleProperty;
}

void CSS3Parser::PropertyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterProperty(this);
}

void CSS3Parser::PropertyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitProperty(this);
}

CSS3Parser::PropertyContext* CSS3Parser::property() {
  PropertyContext *_localctx = _tracker.createInstance<PropertyContext>(_ctx, getState());
  enterRule(_localctx, 38, CSS3Parser::RuleProperty);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(352);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CSS3Parser::IDENT: {
        enterOuterAlt(_localctx, 1);
        setState(350);
        ident();
        break;
      }

      case CSS3Parser::T__37: {
        enterOuterAlt(_localctx, 2);
        setState(351);
        var();
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

//----------------- ValueListContext ------------------------------------------------------------------

CSS3Parser::ValueListContext::ValueListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CSS3Parser::ValueContext *> CSS3Parser::ValueListContext::value() {
  return getRuleContexts<CSS3Parser::ValueContext>();
}

CSS3Parser::ValueContext* CSS3Parser::ValueListContext::value(size_t i) {
  return getRuleContext<CSS3Parser::ValueContext>(i);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::ValueListContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::ValueListContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}


size_t CSS3Parser::ValueListContext::getRuleIndex() const {
  return CSS3Parser::RuleValueList;
}

void CSS3Parser::ValueListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterValueList(this);
}

void CSS3Parser::ValueListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitValueList(this);
}

CSS3Parser::ValueListContext* CSS3Parser::valueList() {
  ValueListContext *_localctx = _tracker.createInstance<ValueListContext>(_ctx, getState());
  enterRule(_localctx, 40, CSS3Parser::RuleValueList);
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
    setState(354);
    value();
    setState(364);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(356); 
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
          setState(355);
          ws();
          setState(358); 
          _errHandler->sync(this);
          _la = _input->LA(1);
        } while (_la == CSS3Parser::WS);
        setState(360);
        value(); 
      }
      setState(366);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueContext ------------------------------------------------------------------

CSS3Parser::ValueContext::ValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::NumberContext* CSS3Parser::ValueContext::number() {
  return getRuleContext<CSS3Parser::NumberContext>(0);
}

CSS3Parser::UnitContext* CSS3Parser::ValueContext::unit() {
  return getRuleContext<CSS3Parser::UnitContext>(0);
}

CSS3Parser::PercentageContext* CSS3Parser::ValueContext::percentage() {
  return getRuleContext<CSS3Parser::PercentageContext>(0);
}

CSS3Parser::DimensionContext* CSS3Parser::ValueContext::dimension() {
  return getRuleContext<CSS3Parser::DimensionContext>(0);
}

CSS3Parser::StringContext* CSS3Parser::ValueContext::string() {
  return getRuleContext<CSS3Parser::StringContext>(0);
}

CSS3Parser::IdentContext* CSS3Parser::ValueContext::ident() {
  return getRuleContext<CSS3Parser::IdentContext>(0);
}

CSS3Parser::UriContext* CSS3Parser::ValueContext::uri() {
  return getRuleContext<CSS3Parser::UriContext>(0);
}

CSS3Parser::HexcolorContext* CSS3Parser::ValueContext::hexcolor() {
  return getRuleContext<CSS3Parser::HexcolorContext>(0);
}

CSS3Parser::CalcContext* CSS3Parser::ValueContext::calc() {
  return getRuleContext<CSS3Parser::CalcContext>(0);
}

CSS3Parser::VarContext* CSS3Parser::ValueContext::var() {
  return getRuleContext<CSS3Parser::VarContext>(0);
}

CSS3Parser::ChtlVariableReferenceContext* CSS3Parser::ValueContext::chtlVariableReference() {
  return getRuleContext<CSS3Parser::ChtlVariableReferenceContext>(0);
}

CSS3Parser::Function_Context* CSS3Parser::ValueContext::function_() {
  return getRuleContext<CSS3Parser::Function_Context>(0);
}


size_t CSS3Parser::ValueContext::getRuleIndex() const {
  return CSS3Parser::RuleValue;
}

void CSS3Parser::ValueContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterValue(this);
}

void CSS3Parser::ValueContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitValue(this);
}

CSS3Parser::ValueContext* CSS3Parser::value() {
  ValueContext *_localctx = _tracker.createInstance<ValueContext>(_ctx, getState());
  enterRule(_localctx, 42, CSS3Parser::RuleValue);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(381);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 43, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(367);
      number();
      setState(369);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 40) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 40)) & 67108863) != 0)) {
        setState(368);
        unit();
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(371);
      percentage();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(372);
      dimension();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(373);
      string();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(374);
      ident();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(375);
      uri();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(376);
      hexcolor();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(377);
      calc();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(378);
      var();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(379);
      chtlVariableReference();
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(380);
      function_();
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

//----------------- CalcContext ------------------------------------------------------------------

CSS3Parser::CalcContext::CalcContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::ExpressionContext* CSS3Parser::CalcContext::expression() {
  return getRuleContext<CSS3Parser::ExpressionContext>(0);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::CalcContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::CalcContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}


size_t CSS3Parser::CalcContext::getRuleIndex() const {
  return CSS3Parser::RuleCalc;
}

void CSS3Parser::CalcContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCalc(this);
}

void CSS3Parser::CalcContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCalc(this);
}

CSS3Parser::CalcContext* CSS3Parser::calc() {
  CalcContext *_localctx = _tracker.createInstance<CalcContext>(_ctx, getState());
  enterRule(_localctx, 44, CSS3Parser::RuleCalc);
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
    setState(383);
    match(CSS3Parser::T__23);
    setState(387);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(384);
      ws();
      setState(389);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(390);
    expression();
    setState(394);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(391);
      ws();
      setState(396);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(397);
    match(CSS3Parser::T__20);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Function_Context ------------------------------------------------------------------

CSS3Parser::Function_Context::Function_Context(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::IdentContext* CSS3Parser::Function_Context::ident() {
  return getRuleContext<CSS3Parser::IdentContext>(0);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::Function_Context::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::Function_Context::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}

CSS3Parser::ExpressionListContext* CSS3Parser::Function_Context::expressionList() {
  return getRuleContext<CSS3Parser::ExpressionListContext>(0);
}


size_t CSS3Parser::Function_Context::getRuleIndex() const {
  return CSS3Parser::RuleFunction_;
}

void CSS3Parser::Function_Context::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunction_(this);
}

void CSS3Parser::Function_Context::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunction_(this);
}

CSS3Parser::Function_Context* CSS3Parser::function_() {
  Function_Context *_localctx = _tracker.createInstance<Function_Context>(_ctx, getState());
  enterRule(_localctx, 46, CSS3Parser::RuleFunction_);
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
    setState(399);
    ident();
    setState(400);
    match(CSS3Parser::T__19);
    setState(404);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 46, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(401);
        ws(); 
      }
      setState(406);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 46, _ctx);
    }
    setState(408);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 9) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 9)) & 2161727821674743809) != 0)) {
      setState(407);
      expressionList();
    }
    setState(413);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(410);
      ws();
      setState(415);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(416);
    match(CSS3Parser::T__20);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionListContext ------------------------------------------------------------------

CSS3Parser::ExpressionListContext::ExpressionListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CSS3Parser::ExpressionContext *> CSS3Parser::ExpressionListContext::expression() {
  return getRuleContexts<CSS3Parser::ExpressionContext>();
}

CSS3Parser::ExpressionContext* CSS3Parser::ExpressionListContext::expression(size_t i) {
  return getRuleContext<CSS3Parser::ExpressionContext>(i);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::ExpressionListContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::ExpressionListContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}


size_t CSS3Parser::ExpressionListContext::getRuleIndex() const {
  return CSS3Parser::RuleExpressionList;
}

void CSS3Parser::ExpressionListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpressionList(this);
}

void CSS3Parser::ExpressionListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpressionList(this);
}

CSS3Parser::ExpressionListContext* CSS3Parser::expressionList() {
  ExpressionListContext *_localctx = _tracker.createInstance<ExpressionListContext>(_ctx, getState());
  enterRule(_localctx, 48, CSS3Parser::RuleExpressionList);
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
    setState(418);
    expression();
    setState(435);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 51, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(422);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == CSS3Parser::WS) {
          setState(419);
          ws();
          setState(424);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(425);
        match(CSS3Parser::T__2);
        setState(429);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == CSS3Parser::WS) {
          setState(426);
          ws();
          setState(431);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(432);
        expression(); 
      }
      setState(437);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 51, _ctx);
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

CSS3Parser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CSS3Parser::TermContext *> CSS3Parser::ExpressionContext::term() {
  return getRuleContexts<CSS3Parser::TermContext>();
}

CSS3Parser::TermContext* CSS3Parser::ExpressionContext::term(size_t i) {
  return getRuleContext<CSS3Parser::TermContext>(i);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::ExpressionContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::ExpressionContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}


size_t CSS3Parser::ExpressionContext::getRuleIndex() const {
  return CSS3Parser::RuleExpression;
}

void CSS3Parser::ExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpression(this);
}

void CSS3Parser::ExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpression(this);
}

CSS3Parser::ExpressionContext* CSS3Parser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 50, CSS3Parser::RuleExpression);
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
    setState(438);
    term();
    setState(449);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::T__3

    || _la == CSS3Parser::T__24) {
      setState(439);
      _la = _input->LA(1);
      if (!(_la == CSS3Parser::T__3

      || _la == CSS3Parser::T__24)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(443);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CSS3Parser::WS) {
        setState(440);
        ws();
        setState(445);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(446);
      term();
      setState(451);
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

//----------------- TermContext ------------------------------------------------------------------

CSS3Parser::TermContext::TermContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CSS3Parser::FactorContext *> CSS3Parser::TermContext::factor() {
  return getRuleContexts<CSS3Parser::FactorContext>();
}

CSS3Parser::FactorContext* CSS3Parser::TermContext::factor(size_t i) {
  return getRuleContext<CSS3Parser::FactorContext>(i);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::TermContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::TermContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}


size_t CSS3Parser::TermContext::getRuleIndex() const {
  return CSS3Parser::RuleTerm;
}

void CSS3Parser::TermContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTerm(this);
}

void CSS3Parser::TermContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTerm(this);
}

CSS3Parser::TermContext* CSS3Parser::term() {
  TermContext *_localctx = _tracker.createInstance<TermContext>(_ctx, getState());
  enterRule(_localctx, 52, CSS3Parser::RuleTerm);
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
    setState(452);
    factor();
    setState(463);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::T__7

    || _la == CSS3Parser::T__25) {
      setState(453);
      _la = _input->LA(1);
      if (!(_la == CSS3Parser::T__7

      || _la == CSS3Parser::T__25)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(457);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CSS3Parser::WS) {
        setState(454);
        ws();
        setState(459);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(460);
      factor();
      setState(465);
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

//----------------- FactorContext ------------------------------------------------------------------

CSS3Parser::FactorContext::FactorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::NumberContext* CSS3Parser::FactorContext::number() {
  return getRuleContext<CSS3Parser::NumberContext>(0);
}

CSS3Parser::UnitContext* CSS3Parser::FactorContext::unit() {
  return getRuleContext<CSS3Parser::UnitContext>(0);
}

CSS3Parser::PercentageContext* CSS3Parser::FactorContext::percentage() {
  return getRuleContext<CSS3Parser::PercentageContext>(0);
}

CSS3Parser::DimensionContext* CSS3Parser::FactorContext::dimension() {
  return getRuleContext<CSS3Parser::DimensionContext>(0);
}

CSS3Parser::ExpressionContext* CSS3Parser::FactorContext::expression() {
  return getRuleContext<CSS3Parser::ExpressionContext>(0);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::FactorContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::FactorContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}

CSS3Parser::ValueContext* CSS3Parser::FactorContext::value() {
  return getRuleContext<CSS3Parser::ValueContext>(0);
}


size_t CSS3Parser::FactorContext::getRuleIndex() const {
  return CSS3Parser::RuleFactor;
}

void CSS3Parser::FactorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFactor(this);
}

void CSS3Parser::FactorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFactor(this);
}

CSS3Parser::FactorContext* CSS3Parser::factor() {
  FactorContext *_localctx = _tracker.createInstance<FactorContext>(_ctx, getState());
  enterRule(_localctx, 54, CSS3Parser::RuleFactor);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(489);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 59, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(466);
      number();
      setState(468);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 40) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 40)) & 67108863) != 0)) {
        setState(467);
        unit();
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(470);
      percentage();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(471);
      dimension();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(472);
      match(CSS3Parser::T__19);
      setState(476);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CSS3Parser::WS) {
        setState(473);
        ws();
        setState(478);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(479);
      expression();
      setState(483);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CSS3Parser::WS) {
        setState(480);
        ws();
        setState(485);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(486);
      match(CSS3Parser::T__20);
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(488);
      value();
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

//----------------- PrioContext ------------------------------------------------------------------

CSS3Parser::PrioContext::PrioContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::PrioContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::PrioContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}


size_t CSS3Parser::PrioContext::getRuleIndex() const {
  return CSS3Parser::RulePrio;
}

void CSS3Parser::PrioContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPrio(this);
}

void CSS3Parser::PrioContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPrio(this);
}

CSS3Parser::PrioContext* CSS3Parser::prio() {
  PrioContext *_localctx = _tracker.createInstance<PrioContext>(_ctx, getState());
  enterRule(_localctx, 56, CSS3Parser::RulePrio);
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
    setState(494);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(491);
      ws();
      setState(496);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(497);
    match(CSS3Parser::T__26);
    setState(501);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(498);
      ws();
      setState(503);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(504);
    match(CSS3Parser::T__27);
    setState(508);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 62, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(505);
        ws(); 
      }
      setState(510);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 62, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChtlTemplateUsageContext ------------------------------------------------------------------

CSS3Parser::ChtlTemplateUsageContext::ChtlTemplateUsageContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::IdentContext* CSS3Parser::ChtlTemplateUsageContext::ident() {
  return getRuleContext<CSS3Parser::IdentContext>(0);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::ChtlTemplateUsageContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::ChtlTemplateUsageContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}

CSS3Parser::ChtlSpecializationContext* CSS3Parser::ChtlTemplateUsageContext::chtlSpecialization() {
  return getRuleContext<CSS3Parser::ChtlSpecializationContext>(0);
}


size_t CSS3Parser::ChtlTemplateUsageContext::getRuleIndex() const {
  return CSS3Parser::RuleChtlTemplateUsage;
}

void CSS3Parser::ChtlTemplateUsageContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlTemplateUsage(this);
}

void CSS3Parser::ChtlTemplateUsageContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlTemplateUsage(this);
}

CSS3Parser::ChtlTemplateUsageContext* CSS3Parser::chtlTemplateUsage() {
  ChtlTemplateUsageContext *_localctx = _tracker.createInstance<ChtlTemplateUsageContext>(_ctx, getState());
  enterRule(_localctx, 58, CSS3Parser::RuleChtlTemplateUsage);
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
    setState(511);
    match(CSS3Parser::T__28);
    setState(512);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 33285996544) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(514); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(513);
      ws();
      setState(516); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == CSS3Parser::WS);
    setState(518);
    ident();
    setState(522);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 64, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(519);
        ws(); 
      }
      setState(524);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 64, _ctx);
    }
    setState(526);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CSS3Parser::T__0) {
      setState(525);
      chtlSpecialization();
    }
    setState(531);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(528);
      ws();
      setState(533);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(534);
    match(CSS3Parser::T__22);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChtlCustomUsageContext ------------------------------------------------------------------

CSS3Parser::ChtlCustomUsageContext::ChtlCustomUsageContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::IdentContext* CSS3Parser::ChtlCustomUsageContext::ident() {
  return getRuleContext<CSS3Parser::IdentContext>(0);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::ChtlCustomUsageContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::ChtlCustomUsageContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}

CSS3Parser::ChtlSpecializationContext* CSS3Parser::ChtlCustomUsageContext::chtlSpecialization() {
  return getRuleContext<CSS3Parser::ChtlSpecializationContext>(0);
}


size_t CSS3Parser::ChtlCustomUsageContext::getRuleIndex() const {
  return CSS3Parser::RuleChtlCustomUsage;
}

void CSS3Parser::ChtlCustomUsageContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlCustomUsage(this);
}

void CSS3Parser::ChtlCustomUsageContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlCustomUsage(this);
}

CSS3Parser::ChtlCustomUsageContext* CSS3Parser::chtlCustomUsage() {
  ChtlCustomUsageContext *_localctx = _tracker.createInstance<ChtlCustomUsageContext>(_ctx, getState());
  enterRule(_localctx, 60, CSS3Parser::RuleChtlCustomUsage);
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
    setState(536);
    match(CSS3Parser::T__28);
    setState(537);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 33285996544) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(539); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(538);
      ws();
      setState(541); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == CSS3Parser::WS);
    setState(543);
    ident();
    setState(547);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 68, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(544);
        ws(); 
      }
      setState(549);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 68, _ctx);
    }
    setState(551);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CSS3Parser::T__0) {
      setState(550);
      chtlSpecialization();
    }
    setState(556);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(553);
      ws();
      setState(558);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(559);
    match(CSS3Parser::T__22);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChtlSpecializationContext ------------------------------------------------------------------

CSS3Parser::ChtlSpecializationContext::ChtlSpecializationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::ChtlSpecializationContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::ChtlSpecializationContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}

CSS3Parser::DeclarationListContext* CSS3Parser::ChtlSpecializationContext::declarationList() {
  return getRuleContext<CSS3Parser::DeclarationListContext>(0);
}


size_t CSS3Parser::ChtlSpecializationContext::getRuleIndex() const {
  return CSS3Parser::RuleChtlSpecialization;
}

void CSS3Parser::ChtlSpecializationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlSpecialization(this);
}

void CSS3Parser::ChtlSpecializationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlSpecialization(this);
}

CSS3Parser::ChtlSpecializationContext* CSS3Parser::chtlSpecialization() {
  ChtlSpecializationContext *_localctx = _tracker.createInstance<ChtlSpecializationContext>(_ctx, getState());
  enterRule(_localctx, 62, CSS3Parser::RuleChtlSpecialization);
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
    setState(561);
    match(CSS3Parser::T__0);
    setState(565);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 71, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(562);
        ws(); 
      }
      setState(567);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 71, _ctx);
    }
    setState(569);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 29) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 29)) & 1099511628481) != 0)) {
      setState(568);
      declarationList();
    }
    setState(574);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(571);
      ws();
      setState(576);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(577);
    match(CSS3Parser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChtlVariableUsageContext ------------------------------------------------------------------

CSS3Parser::ChtlVariableUsageContext::ChtlVariableUsageContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CSS3Parser::IdentContext *> CSS3Parser::ChtlVariableUsageContext::ident() {
  return getRuleContexts<CSS3Parser::IdentContext>();
}

CSS3Parser::IdentContext* CSS3Parser::ChtlVariableUsageContext::ident(size_t i) {
  return getRuleContext<CSS3Parser::IdentContext>(i);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::ChtlVariableUsageContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::ChtlVariableUsageContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}


size_t CSS3Parser::ChtlVariableUsageContext::getRuleIndex() const {
  return CSS3Parser::RuleChtlVariableUsage;
}

void CSS3Parser::ChtlVariableUsageContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlVariableUsage(this);
}

void CSS3Parser::ChtlVariableUsageContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlVariableUsage(this);
}

CSS3Parser::ChtlVariableUsageContext* CSS3Parser::chtlVariableUsage() {
  ChtlVariableUsageContext *_localctx = _tracker.createInstance<ChtlVariableUsageContext>(_ctx, getState());
  enterRule(_localctx, 64, CSS3Parser::RuleChtlVariableUsage);
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
    setState(579);
    ident();
    setState(580);
    match(CSS3Parser::T__19);
    setState(581);
    ident();
    setState(582);
    match(CSS3Parser::T__20);
    setState(586);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(583);
      ws();
      setState(588);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(589);
    match(CSS3Parser::T__22);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChtlVariableReferenceContext ------------------------------------------------------------------

CSS3Parser::ChtlVariableReferenceContext::ChtlVariableReferenceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CSS3Parser::IdentContext *> CSS3Parser::ChtlVariableReferenceContext::ident() {
  return getRuleContexts<CSS3Parser::IdentContext>();
}

CSS3Parser::IdentContext* CSS3Parser::ChtlVariableReferenceContext::ident(size_t i) {
  return getRuleContext<CSS3Parser::IdentContext>(i);
}


size_t CSS3Parser::ChtlVariableReferenceContext::getRuleIndex() const {
  return CSS3Parser::RuleChtlVariableReference;
}

void CSS3Parser::ChtlVariableReferenceContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlVariableReference(this);
}

void CSS3Parser::ChtlVariableReferenceContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlVariableReference(this);
}

CSS3Parser::ChtlVariableReferenceContext* CSS3Parser::chtlVariableReference() {
  ChtlVariableReferenceContext *_localctx = _tracker.createInstance<ChtlVariableReferenceContext>(_ctx, getState());
  enterRule(_localctx, 66, CSS3Parser::RuleChtlVariableReference);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(591);
    ident();
    setState(592);
    match(CSS3Parser::T__19);
    setState(593);
    ident();
    setState(594);
    match(CSS3Parser::T__20);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChtlStyleGroupUsageContext ------------------------------------------------------------------

CSS3Parser::ChtlStyleGroupUsageContext::ChtlStyleGroupUsageContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::IdentContext* CSS3Parser::ChtlStyleGroupUsageContext::ident() {
  return getRuleContext<CSS3Parser::IdentContext>(0);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::ChtlStyleGroupUsageContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::ChtlStyleGroupUsageContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}

CSS3Parser::ChtlSpecializationContext* CSS3Parser::ChtlStyleGroupUsageContext::chtlSpecialization() {
  return getRuleContext<CSS3Parser::ChtlSpecializationContext>(0);
}


size_t CSS3Parser::ChtlStyleGroupUsageContext::getRuleIndex() const {
  return CSS3Parser::RuleChtlStyleGroupUsage;
}

void CSS3Parser::ChtlStyleGroupUsageContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlStyleGroupUsage(this);
}

void CSS3Parser::ChtlStyleGroupUsageContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlStyleGroupUsage(this);
}

CSS3Parser::ChtlStyleGroupUsageContext* CSS3Parser::chtlStyleGroupUsage() {
  ChtlStyleGroupUsageContext *_localctx = _tracker.createInstance<ChtlStyleGroupUsageContext>(_ctx, getState());
  enterRule(_localctx, 68, CSS3Parser::RuleChtlStyleGroupUsage);
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
    setState(596);
    match(CSS3Parser::T__28);
    setState(597);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 33285996544) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(599); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(598);
      ws();
      setState(601); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == CSS3Parser::WS);
    setState(603);
    ident();
    setState(607);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 76, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(604);
        ws(); 
      }
      setState(609);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 76, _ctx);
    }
    setState(611);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CSS3Parser::T__0) {
      setState(610);
      chtlSpecialization();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChtlDeletePropertyContext ------------------------------------------------------------------

CSS3Parser::ChtlDeletePropertyContext::ChtlDeletePropertyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::PropertyContext* CSS3Parser::ChtlDeletePropertyContext::property() {
  return getRuleContext<CSS3Parser::PropertyContext>(0);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::ChtlDeletePropertyContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::ChtlDeletePropertyContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}


size_t CSS3Parser::ChtlDeletePropertyContext::getRuleIndex() const {
  return CSS3Parser::RuleChtlDeleteProperty;
}

void CSS3Parser::ChtlDeletePropertyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlDeleteProperty(this);
}

void CSS3Parser::ChtlDeletePropertyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlDeleteProperty(this);
}

CSS3Parser::ChtlDeletePropertyContext* CSS3Parser::chtlDeleteProperty() {
  ChtlDeletePropertyContext *_localctx = _tracker.createInstance<ChtlDeletePropertyContext>(_ctx, getState());
  enterRule(_localctx, 70, CSS3Parser::RuleChtlDeleteProperty);
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
    setState(613);
    match(CSS3Parser::T__34);
    setState(615); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(614);
      ws();
      setState(617); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == CSS3Parser::WS);
    setState(619);
    property();
    setState(623);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 79, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(620);
        ws(); 
      }
      setState(625);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 79, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChtlInheritContext ------------------------------------------------------------------

CSS3Parser::ChtlInheritContext::ChtlInheritContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::IdentContext* CSS3Parser::ChtlInheritContext::ident() {
  return getRuleContext<CSS3Parser::IdentContext>(0);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::ChtlInheritContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::ChtlInheritContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}


size_t CSS3Parser::ChtlInheritContext::getRuleIndex() const {
  return CSS3Parser::RuleChtlInherit;
}

void CSS3Parser::ChtlInheritContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlInherit(this);
}

void CSS3Parser::ChtlInheritContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlInherit(this);
}

CSS3Parser::ChtlInheritContext* CSS3Parser::chtlInherit() {
  ChtlInheritContext *_localctx = _tracker.createInstance<ChtlInheritContext>(_ctx, getState());
  enterRule(_localctx, 72, CSS3Parser::RuleChtlInherit);
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
    setState(626);
    match(CSS3Parser::T__35);
    setState(628); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(627);
      ws();
      setState(630); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == CSS3Parser::WS);
    setState(632);
    match(CSS3Parser::T__28);
    setState(633);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 33285996544) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(635); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(634);
      ws();
      setState(637); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == CSS3Parser::WS);
    setState(639);
    ident();
    setState(643);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 82, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(640);
        ws(); 
      }
      setState(645);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 82, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChtlOriginEmbedContext ------------------------------------------------------------------

CSS3Parser::ChtlOriginEmbedContext::ChtlOriginEmbedContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::IdentContext* CSS3Parser::ChtlOriginEmbedContext::ident() {
  return getRuleContext<CSS3Parser::IdentContext>(0);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::ChtlOriginEmbedContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::ChtlOriginEmbedContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}


size_t CSS3Parser::ChtlOriginEmbedContext::getRuleIndex() const {
  return CSS3Parser::RuleChtlOriginEmbed;
}

void CSS3Parser::ChtlOriginEmbedContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterChtlOriginEmbed(this);
}

void CSS3Parser::ChtlOriginEmbedContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitChtlOriginEmbed(this);
}

CSS3Parser::ChtlOriginEmbedContext* CSS3Parser::chtlOriginEmbed() {
  ChtlOriginEmbedContext *_localctx = _tracker.createInstance<ChtlOriginEmbedContext>(_ctx, getState());
  enterRule(_localctx, 74, CSS3Parser::RuleChtlOriginEmbed);
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
    setState(646);
    match(CSS3Parser::T__36);
    setState(650);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(647);
      ws();
      setState(652);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(653);
    match(CSS3Parser::T__28);
    setState(654);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 33285996544) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(656); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(655);
      ws();
      setState(658); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == CSS3Parser::WS);
    setState(660);
    ident();
    setState(664);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(661);
      ws();
      setState(666);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(667);
    match(CSS3Parser::T__22);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AtRuleContext ------------------------------------------------------------------

CSS3Parser::AtRuleContext::AtRuleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::IdentContext* CSS3Parser::AtRuleContext::ident() {
  return getRuleContext<CSS3Parser::IdentContext>(0);
}

CSS3Parser::AtRuleBlockContext* CSS3Parser::AtRuleContext::atRuleBlock() {
  return getRuleContext<CSS3Parser::AtRuleBlockContext>(0);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::AtRuleContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::AtRuleContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}


size_t CSS3Parser::AtRuleContext::getRuleIndex() const {
  return CSS3Parser::RuleAtRule;
}

void CSS3Parser::AtRuleContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAtRule(this);
}

void CSS3Parser::AtRuleContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAtRule(this);
}

CSS3Parser::AtRuleContext* CSS3Parser::atRule() {
  AtRuleContext *_localctx = _tracker.createInstance<AtRuleContext>(_ctx, getState());
  enterRule(_localctx, 76, CSS3Parser::RuleAtRule);
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
    setState(669);
    match(CSS3Parser::T__28);
    setState(670);
    ident();
    setState(674);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(671);
      ws();
      setState(676);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(679);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CSS3Parser::T__0:
      case CSS3Parser::T__8:
      case CSS3Parser::T__23:
      case CSS3Parser::T__37:
      case CSS3Parser::T__65:
      case CSS3Parser::NUMBER:
      case CSS3Parser::STRING:
      case CSS3Parser::IDENT: {
        setState(677);
        atRuleBlock();
        break;
      }

      case CSS3Parser::T__22: {
        setState(678);
        match(CSS3Parser::T__22);
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

//----------------- AtRuleBlockContext ------------------------------------------------------------------

CSS3Parser::AtRuleBlockContext::AtRuleBlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::AtRuleBlockContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::AtRuleBlockContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}

std::vector<CSS3Parser::StyleRuleContext *> CSS3Parser::AtRuleBlockContext::styleRule() {
  return getRuleContexts<CSS3Parser::StyleRuleContext>();
}

CSS3Parser::StyleRuleContext* CSS3Parser::AtRuleBlockContext::styleRule(size_t i) {
  return getRuleContext<CSS3Parser::StyleRuleContext>(i);
}

std::vector<CSS3Parser::DeclarationContext *> CSS3Parser::AtRuleBlockContext::declaration() {
  return getRuleContexts<CSS3Parser::DeclarationContext>();
}

CSS3Parser::DeclarationContext* CSS3Parser::AtRuleBlockContext::declaration(size_t i) {
  return getRuleContext<CSS3Parser::DeclarationContext>(i);
}

CSS3Parser::ValueListContext* CSS3Parser::AtRuleBlockContext::valueList() {
  return getRuleContext<CSS3Parser::ValueListContext>(0);
}


size_t CSS3Parser::AtRuleBlockContext::getRuleIndex() const {
  return CSS3Parser::RuleAtRuleBlock;
}

void CSS3Parser::AtRuleBlockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAtRuleBlock(this);
}

void CSS3Parser::AtRuleBlockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAtRuleBlock(this);
}

CSS3Parser::AtRuleBlockContext* CSS3Parser::atRuleBlock() {
  AtRuleBlockContext *_localctx = _tracker.createInstance<AtRuleBlockContext>(_ctx, getState());
  enterRule(_localctx, 78, CSS3Parser::RuleAtRuleBlock);
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
    setState(724);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CSS3Parser::T__0: {
        enterOuterAlt(_localctx, 1);
        setState(681);
        match(CSS3Parser::T__0);
        setState(685);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 88, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
          if (alt == 1) {
            setState(682);
            ws(); 
          }
          setState(687);
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 88, _ctx);
        }
        setState(692);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (((((_la - 8) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 8)) & 2305843011229075471) != 0)) {
          setState(690);
          _errHandler->sync(this);
          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 89, _ctx)) {
          case 1: {
            setState(688);
            styleRule();
            break;
          }

          case 2: {
            setState(689);
            declaration();
            break;
          }

          default:
            break;
          }
          setState(694);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(698);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == CSS3Parser::WS) {
          setState(695);
          ws();
          setState(700);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(701);
        match(CSS3Parser::T__1);
        break;
      }

      case CSS3Parser::T__8:
      case CSS3Parser::T__23:
      case CSS3Parser::T__37:
      case CSS3Parser::T__65:
      case CSS3Parser::NUMBER:
      case CSS3Parser::STRING:
      case CSS3Parser::IDENT: {
        enterOuterAlt(_localctx, 2);
        setState(702);
        valueList();
        setState(703);
        match(CSS3Parser::T__0);
        setState(707);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 92, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
          if (alt == 1) {
            setState(704);
            ws(); 
          }
          setState(709);
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 92, _ctx);
        }
        setState(713);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (((((_la - 8) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 8)) & 2305843009752680463) != 0)) {
          setState(710);
          styleRule();
          setState(715);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(719);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == CSS3Parser::WS) {
          setState(716);
          ws();
          setState(721);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(722);
        match(CSS3Parser::T__1);
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

//----------------- VarContext ------------------------------------------------------------------

CSS3Parser::VarContext::VarContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::IdentContext* CSS3Parser::VarContext::ident() {
  return getRuleContext<CSS3Parser::IdentContext>(0);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::VarContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::VarContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}

CSS3Parser::ValueContext* CSS3Parser::VarContext::value() {
  return getRuleContext<CSS3Parser::ValueContext>(0);
}


size_t CSS3Parser::VarContext::getRuleIndex() const {
  return CSS3Parser::RuleVar;
}

void CSS3Parser::VarContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVar(this);
}

void CSS3Parser::VarContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVar(this);
}

CSS3Parser::VarContext* CSS3Parser::var() {
  VarContext *_localctx = _tracker.createInstance<VarContext>(_ctx, getState());
  enterRule(_localctx, 80, CSS3Parser::RuleVar);
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
    setState(726);
    match(CSS3Parser::T__37);
    setState(730);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(727);
      ws();
      setState(732);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(733);
    match(CSS3Parser::T__38);
    setState(734);
    ident();
    setState(738);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 97, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(735);
        ws(); 
      }
      setState(740);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 97, _ctx);
    }
    setState(749);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CSS3Parser::T__2) {
      setState(741);
      match(CSS3Parser::T__2);
      setState(745);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CSS3Parser::WS) {
        setState(742);
        ws();
        setState(747);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(748);
      value();
    }
    setState(754);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(751);
      ws();
      setState(756);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(757);
    match(CSS3Parser::T__20);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- HexcolorContext ------------------------------------------------------------------

CSS3Parser::HexcolorContext::HexcolorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CSS3Parser::HexdigitContext *> CSS3Parser::HexcolorContext::hexdigit() {
  return getRuleContexts<CSS3Parser::HexdigitContext>();
}

CSS3Parser::HexdigitContext* CSS3Parser::HexcolorContext::hexdigit(size_t i) {
  return getRuleContext<CSS3Parser::HexdigitContext>(i);
}


size_t CSS3Parser::HexcolorContext::getRuleIndex() const {
  return CSS3Parser::RuleHexcolor;
}

void CSS3Parser::HexcolorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterHexcolor(this);
}

void CSS3Parser::HexcolorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitHexcolor(this);
}

CSS3Parser::HexcolorContext* CSS3Parser::hexcolor() {
  HexcolorContext *_localctx = _tracker.createInstance<HexcolorContext>(_ctx, getState());
  enterRule(_localctx, 82, CSS3Parser::RuleHexcolor);
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
    setState(759);
    match(CSS3Parser::T__8);
    setState(761); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(760);
      hexdigit();
      setState(763); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == CSS3Parser::HEXDIGIT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumberContext ------------------------------------------------------------------

CSS3Parser::NumberContext::NumberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CSS3Parser::NumberContext::NUMBER() {
  return getToken(CSS3Parser::NUMBER, 0);
}


size_t CSS3Parser::NumberContext::getRuleIndex() const {
  return CSS3Parser::RuleNumber;
}

void CSS3Parser::NumberContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNumber(this);
}

void CSS3Parser::NumberContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNumber(this);
}

CSS3Parser::NumberContext* CSS3Parser::number() {
  NumberContext *_localctx = _tracker.createInstance<NumberContext>(_ctx, getState());
  enterRule(_localctx, 84, CSS3Parser::RuleNumber);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(765);
    match(CSS3Parser::NUMBER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PercentageContext ------------------------------------------------------------------

CSS3Parser::PercentageContext::PercentageContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::NumberContext* CSS3Parser::PercentageContext::number() {
  return getRuleContext<CSS3Parser::NumberContext>(0);
}


size_t CSS3Parser::PercentageContext::getRuleIndex() const {
  return CSS3Parser::RulePercentage;
}

void CSS3Parser::PercentageContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPercentage(this);
}

void CSS3Parser::PercentageContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPercentage(this);
}

CSS3Parser::PercentageContext* CSS3Parser::percentage() {
  PercentageContext *_localctx = _tracker.createInstance<PercentageContext>(_ctx, getState());
  enterRule(_localctx, 86, CSS3Parser::RulePercentage);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(767);
    number();
    setState(768);
    match(CSS3Parser::T__39);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DimensionContext ------------------------------------------------------------------

CSS3Parser::DimensionContext::DimensionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::NumberContext* CSS3Parser::DimensionContext::number() {
  return getRuleContext<CSS3Parser::NumberContext>(0);
}

CSS3Parser::UnitContext* CSS3Parser::DimensionContext::unit() {
  return getRuleContext<CSS3Parser::UnitContext>(0);
}


size_t CSS3Parser::DimensionContext::getRuleIndex() const {
  return CSS3Parser::RuleDimension;
}

void CSS3Parser::DimensionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDimension(this);
}

void CSS3Parser::DimensionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDimension(this);
}

CSS3Parser::DimensionContext* CSS3Parser::dimension() {
  DimensionContext *_localctx = _tracker.createInstance<DimensionContext>(_ctx, getState());
  enterRule(_localctx, 88, CSS3Parser::RuleDimension);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(770);
    number();
    setState(771);
    unit();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnitContext ------------------------------------------------------------------

CSS3Parser::UnitContext::UnitContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CSS3Parser::UnitContext::getRuleIndex() const {
  return CSS3Parser::RuleUnit;
}

void CSS3Parser::UnitContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnit(this);
}

void CSS3Parser::UnitContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnit(this);
}

CSS3Parser::UnitContext* CSS3Parser::unit() {
  UnitContext *_localctx = _tracker.createInstance<UnitContext>(_ctx, getState());
  enterRule(_localctx, 90, CSS3Parser::RuleUnit);
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
    setState(773);
    _la = _input->LA(1);
    if (!(((((_la - 40) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 40)) & 67108863) != 0))) {
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

//----------------- UriContext ------------------------------------------------------------------

CSS3Parser::UriContext::UriContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::StringContext* CSS3Parser::UriContext::string() {
  return getRuleContext<CSS3Parser::StringContext>(0);
}

tree::TerminalNode* CSS3Parser::UriContext::URL_CONTENT() {
  return getToken(CSS3Parser::URL_CONTENT, 0);
}

std::vector<CSS3Parser::WsContext *> CSS3Parser::UriContext::ws() {
  return getRuleContexts<CSS3Parser::WsContext>();
}

CSS3Parser::WsContext* CSS3Parser::UriContext::ws(size_t i) {
  return getRuleContext<CSS3Parser::WsContext>(i);
}


size_t CSS3Parser::UriContext::getRuleIndex() const {
  return CSS3Parser::RuleUri;
}

void CSS3Parser::UriContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUri(this);
}

void CSS3Parser::UriContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUri(this);
}

CSS3Parser::UriContext* CSS3Parser::uri() {
  UriContext *_localctx = _tracker.createInstance<UriContext>(_ctx, getState());
  enterRule(_localctx, 92, CSS3Parser::RuleUri);
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
    setState(775);
    match(CSS3Parser::T__65);
    setState(779);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(776);
      ws();
      setState(781);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(784);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CSS3Parser::STRING: {
        setState(782);
        string();
        break;
      }

      case CSS3Parser::URL_CONTENT: {
        setState(783);
        match(CSS3Parser::URL_CONTENT);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(789);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::WS) {
      setState(786);
      ws();
      setState(791);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(792);
    match(CSS3Parser::T__20);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StringContext ------------------------------------------------------------------

CSS3Parser::StringContext::StringContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CSS3Parser::StringContext::STRING() {
  return getToken(CSS3Parser::STRING, 0);
}


size_t CSS3Parser::StringContext::getRuleIndex() const {
  return CSS3Parser::RuleString;
}

void CSS3Parser::StringContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterString(this);
}

void CSS3Parser::StringContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitString(this);
}

CSS3Parser::StringContext* CSS3Parser::string() {
  StringContext *_localctx = _tracker.createInstance<StringContext>(_ctx, getState());
  enterRule(_localctx, 94, CSS3Parser::RuleString);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(794);
    match(CSS3Parser::STRING);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IdentContext ------------------------------------------------------------------

CSS3Parser::IdentContext::IdentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CSS3Parser::IdentContext::IDENT() {
  return getToken(CSS3Parser::IDENT, 0);
}


size_t CSS3Parser::IdentContext::getRuleIndex() const {
  return CSS3Parser::RuleIdent;
}

void CSS3Parser::IdentContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIdent(this);
}

void CSS3Parser::IdentContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIdent(this);
}

CSS3Parser::IdentContext* CSS3Parser::ident() {
  IdentContext *_localctx = _tracker.createInstance<IdentContext>(_ctx, getState());
  enterRule(_localctx, 96, CSS3Parser::RuleIdent);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(796);
    match(CSS3Parser::IDENT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- HexdigitContext ------------------------------------------------------------------

CSS3Parser::HexdigitContext::HexdigitContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CSS3Parser::HexdigitContext::HEXDIGIT() {
  return getToken(CSS3Parser::HEXDIGIT, 0);
}


size_t CSS3Parser::HexdigitContext::getRuleIndex() const {
  return CSS3Parser::RuleHexdigit;
}

void CSS3Parser::HexdigitContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterHexdigit(this);
}

void CSS3Parser::HexdigitContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitHexdigit(this);
}

CSS3Parser::HexdigitContext* CSS3Parser::hexdigit() {
  HexdigitContext *_localctx = _tracker.createInstance<HexdigitContext>(_ctx, getState());
  enterRule(_localctx, 98, CSS3Parser::RuleHexdigit);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(798);
    match(CSS3Parser::HEXDIGIT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WsContext ------------------------------------------------------------------

CSS3Parser::WsContext::WsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CSS3Parser::WsContext::WS() {
  return getToken(CSS3Parser::WS, 0);
}


size_t CSS3Parser::WsContext::getRuleIndex() const {
  return CSS3Parser::RuleWs;
}

void CSS3Parser::WsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterWs(this);
}

void CSS3Parser::WsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitWs(this);
}

CSS3Parser::WsContext* CSS3Parser::ws() {
  WsContext *_localctx = _tracker.createInstance<WsContext>(_ctx, getState());
  enterRule(_localctx, 100, CSS3Parser::RuleWs);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(800);
    match(CSS3Parser::WS);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void CSS3Parser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  css3ParserInitialize();
#else
  ::antlr4::internal::call_once(css3ParserOnceFlag, css3ParserInitialize);
#endif
}

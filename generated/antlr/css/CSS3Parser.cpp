
// Generated from grammars/CSS3.g4 by ANTLR 4.13.1


#include "CSS3Listener.h"
#include "CSS3Visitor.h"

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
      "stylesheet", "statement", "atRule", "block", "ruleset", "selector", 
      "combinator", "simpleSelector", "elementName", "declaration", "prio", 
      "expr", "term", "function", "unicodeRange", "hexcolor", "cssOperator", 
      "unaryOperator", "property", "any", "comment"
    },
    std::vector<std::string>{
      "", "'='", "'*'", "'U+'", "'\\u003F'", "'/'", "'-'", "", "", "'{'", 
      "'}'", "'('", "')'", "'['", "']'", "';'", "':'", "'.'", "','", "'+'", 
      "'>'", "'~'", "'~='", "'|='", "'^='", "'$='", "'*='", "'!important'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "COMMENT", "WS", "LBRACE", "RBRACE", "LPAREN", 
      "RPAREN", "LBRACKET", "RBRACKET", "SEMICOLON", "COLON", "DOT", "COMMA", 
      "PLUS", "GREATER", "TILDE", "INCLUDES", "DASHMATCH", "PREFIXMATCH", 
      "SUFFIXMATCH", "SUBSTRINGMATCH", "IMPORTANT_SYM", "AT_RULE_NAME", 
      "FUNCTION", "URI", "HASH", "NUMBER", "PERCENTAGE", "LENGTH", "EMS", 
      "EXS", "ANGLE", "TIME", "FREQ", "STRING", "IDENT", "HEX_DIGIT"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,42,224,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,1,0,5,
  	0,44,8,0,10,0,12,0,47,9,0,1,1,1,1,1,1,3,1,52,8,1,1,2,1,2,5,2,56,8,2,10,
  	2,12,2,59,9,2,1,2,1,2,3,2,63,8,2,1,3,1,3,5,3,67,8,3,10,3,12,3,70,9,3,
  	1,3,1,3,1,4,1,4,1,4,5,4,77,8,4,10,4,12,4,80,9,4,1,4,1,4,5,4,84,8,4,10,
  	4,12,4,87,9,4,1,4,1,4,1,5,1,5,1,5,1,5,5,5,95,8,5,10,5,12,5,98,9,5,1,6,
  	1,6,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,3,7,110,8,7,1,7,1,7,1,7,1,7,1,7,1,
  	7,3,7,118,8,7,1,7,1,7,1,7,3,7,123,8,7,1,8,1,8,1,9,1,9,1,9,1,9,3,9,131,
  	8,9,1,9,3,9,134,8,9,1,10,1,10,1,11,1,11,1,11,1,11,5,11,142,8,11,10,11,
  	12,11,145,9,11,1,12,3,12,148,8,12,1,12,1,12,3,12,152,8,12,1,12,1,12,1,
  	12,1,12,1,12,1,12,1,12,3,12,161,8,12,1,13,1,13,1,13,1,13,1,14,1,14,1,
  	14,5,14,170,8,14,10,14,12,14,173,9,14,1,15,1,15,1,15,1,15,1,15,1,15,1,
  	15,3,15,182,8,15,1,16,1,16,1,17,1,17,1,18,1,18,1,19,1,19,1,19,1,19,1,
  	19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,
  	19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,1,19,3,19,220,8,
  	19,1,20,1,20,1,20,0,0,21,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,
  	34,36,38,40,0,9,2,0,8,8,19,21,2,0,1,1,22,26,1,0,40,41,2,0,32,32,40,41,
  	2,0,2,2,41,41,2,0,33,39,41,41,2,0,4,4,42,42,2,0,5,5,18,18,2,0,6,6,19,
  	19,261,0,45,1,0,0,0,2,51,1,0,0,0,4,53,1,0,0,0,6,64,1,0,0,0,8,73,1,0,0,
  	0,10,90,1,0,0,0,12,99,1,0,0,0,14,122,1,0,0,0,16,124,1,0,0,0,18,126,1,
  	0,0,0,20,135,1,0,0,0,22,137,1,0,0,0,24,147,1,0,0,0,26,162,1,0,0,0,28,
  	166,1,0,0,0,30,174,1,0,0,0,32,183,1,0,0,0,34,185,1,0,0,0,36,187,1,0,0,
  	0,38,219,1,0,0,0,40,221,1,0,0,0,42,44,3,2,1,0,43,42,1,0,0,0,44,47,1,0,
  	0,0,45,43,1,0,0,0,45,46,1,0,0,0,46,1,1,0,0,0,47,45,1,0,0,0,48,52,3,8,
  	4,0,49,52,3,4,2,0,50,52,3,40,20,0,51,48,1,0,0,0,51,49,1,0,0,0,51,50,1,
  	0,0,0,52,3,1,0,0,0,53,57,5,28,0,0,54,56,3,38,19,0,55,54,1,0,0,0,56,59,
  	1,0,0,0,57,55,1,0,0,0,57,58,1,0,0,0,58,62,1,0,0,0,59,57,1,0,0,0,60,63,
  	3,6,3,0,61,63,5,15,0,0,62,60,1,0,0,0,62,61,1,0,0,0,63,5,1,0,0,0,64,68,
  	5,9,0,0,65,67,3,18,9,0,66,65,1,0,0,0,67,70,1,0,0,0,68,66,1,0,0,0,68,69,
  	1,0,0,0,69,71,1,0,0,0,70,68,1,0,0,0,71,72,5,10,0,0,72,7,1,0,0,0,73,78,
  	3,10,5,0,74,75,5,18,0,0,75,77,3,10,5,0,76,74,1,0,0,0,77,80,1,0,0,0,78,
  	76,1,0,0,0,78,79,1,0,0,0,79,81,1,0,0,0,80,78,1,0,0,0,81,85,5,9,0,0,82,
  	84,3,18,9,0,83,82,1,0,0,0,84,87,1,0,0,0,85,83,1,0,0,0,85,86,1,0,0,0,86,
  	88,1,0,0,0,87,85,1,0,0,0,88,89,5,10,0,0,89,9,1,0,0,0,90,96,3,14,7,0,91,
  	92,3,12,6,0,92,93,3,14,7,0,93,95,1,0,0,0,94,91,1,0,0,0,95,98,1,0,0,0,
  	96,94,1,0,0,0,96,97,1,0,0,0,97,11,1,0,0,0,98,96,1,0,0,0,99,100,7,0,0,
  	0,100,13,1,0,0,0,101,123,3,16,8,0,102,123,5,31,0,0,103,104,5,17,0,0,104,
  	123,5,41,0,0,105,106,5,13,0,0,106,109,5,41,0,0,107,108,7,1,0,0,108,110,
  	7,2,0,0,109,107,1,0,0,0,109,110,1,0,0,0,110,111,1,0,0,0,111,123,5,14,
  	0,0,112,113,5,16,0,0,113,117,5,41,0,0,114,115,5,11,0,0,115,116,7,3,0,
  	0,116,118,5,12,0,0,117,114,1,0,0,0,117,118,1,0,0,0,118,123,1,0,0,0,119,
  	120,5,16,0,0,120,121,5,16,0,0,121,123,5,41,0,0,122,101,1,0,0,0,122,102,
  	1,0,0,0,122,103,1,0,0,0,122,105,1,0,0,0,122,112,1,0,0,0,122,119,1,0,0,
  	0,123,15,1,0,0,0,124,125,7,4,0,0,125,17,1,0,0,0,126,127,3,36,18,0,127,
  	128,5,16,0,0,128,130,3,22,11,0,129,131,3,20,10,0,130,129,1,0,0,0,130,
  	131,1,0,0,0,131,133,1,0,0,0,132,134,5,15,0,0,133,132,1,0,0,0,133,134,
  	1,0,0,0,134,19,1,0,0,0,135,136,5,27,0,0,136,21,1,0,0,0,137,143,3,24,12,
  	0,138,139,3,32,16,0,139,140,3,24,12,0,140,142,1,0,0,0,141,138,1,0,0,0,
  	142,145,1,0,0,0,143,141,1,0,0,0,143,144,1,0,0,0,144,23,1,0,0,0,145,143,
  	1,0,0,0,146,148,3,34,17,0,147,146,1,0,0,0,147,148,1,0,0,0,148,160,1,0,
  	0,0,149,151,5,32,0,0,150,152,7,5,0,0,151,150,1,0,0,0,151,152,1,0,0,0,
  	152,161,1,0,0,0,153,161,5,40,0,0,154,161,5,41,0,0,155,161,5,30,0,0,156,
  	161,5,31,0,0,157,161,3,28,14,0,158,161,3,30,15,0,159,161,3,26,13,0,160,
  	149,1,0,0,0,160,153,1,0,0,0,160,154,1,0,0,0,160,155,1,0,0,0,160,156,1,
  	0,0,0,160,157,1,0,0,0,160,158,1,0,0,0,160,159,1,0,0,0,161,25,1,0,0,0,
  	162,163,5,29,0,0,163,164,3,22,11,0,164,165,5,12,0,0,165,27,1,0,0,0,166,
  	167,5,3,0,0,167,171,5,42,0,0,168,170,7,6,0,0,169,168,1,0,0,0,170,173,
  	1,0,0,0,171,169,1,0,0,0,171,172,1,0,0,0,172,29,1,0,0,0,173,171,1,0,0,
  	0,174,175,5,31,0,0,175,176,5,42,0,0,176,177,5,42,0,0,177,181,5,42,0,0,
  	178,179,5,42,0,0,179,180,5,42,0,0,180,182,5,42,0,0,181,178,1,0,0,0,181,
  	182,1,0,0,0,182,31,1,0,0,0,183,184,7,7,0,0,184,33,1,0,0,0,185,186,7,8,
  	0,0,186,35,1,0,0,0,187,188,5,41,0,0,188,37,1,0,0,0,189,220,5,41,0,0,190,
  	220,5,32,0,0,191,220,5,33,0,0,192,220,5,34,0,0,193,220,5,35,0,0,194,220,
  	5,36,0,0,195,220,5,37,0,0,196,220,5,38,0,0,197,220,5,39,0,0,198,220,5,
  	40,0,0,199,220,5,30,0,0,200,220,5,31,0,0,201,220,5,19,0,0,202,220,5,20,
  	0,0,203,220,5,18,0,0,204,220,5,21,0,0,205,220,5,16,0,0,206,220,5,15,0,
  	0,207,220,5,9,0,0,208,220,5,10,0,0,209,220,5,11,0,0,210,220,5,12,0,0,
  	211,220,5,13,0,0,212,220,5,14,0,0,213,220,3,26,13,0,214,220,5,22,0,0,
  	215,220,5,23,0,0,216,220,5,24,0,0,217,220,5,25,0,0,218,220,5,26,0,0,219,
  	189,1,0,0,0,219,190,1,0,0,0,219,191,1,0,0,0,219,192,1,0,0,0,219,193,1,
  	0,0,0,219,194,1,0,0,0,219,195,1,0,0,0,219,196,1,0,0,0,219,197,1,0,0,0,
  	219,198,1,0,0,0,219,199,1,0,0,0,219,200,1,0,0,0,219,201,1,0,0,0,219,202,
  	1,0,0,0,219,203,1,0,0,0,219,204,1,0,0,0,219,205,1,0,0,0,219,206,1,0,0,
  	0,219,207,1,0,0,0,219,208,1,0,0,0,219,209,1,0,0,0,219,210,1,0,0,0,219,
  	211,1,0,0,0,219,212,1,0,0,0,219,213,1,0,0,0,219,214,1,0,0,0,219,215,1,
  	0,0,0,219,216,1,0,0,0,219,217,1,0,0,0,219,218,1,0,0,0,220,39,1,0,0,0,
  	221,222,5,7,0,0,222,41,1,0,0,0,20,45,51,57,62,68,78,85,96,109,117,122,
  	130,133,143,147,151,160,171,181,219
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

std::vector<CSS3Parser::StatementContext *> CSS3Parser::StylesheetContext::statement() {
  return getRuleContexts<CSS3Parser::StatementContext>();
}

CSS3Parser::StatementContext* CSS3Parser::StylesheetContext::statement(size_t i) {
  return getRuleContext<CSS3Parser::StatementContext>(i);
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


std::any CSS3Parser::StylesheetContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitStylesheet(this);
  else
    return visitor->visitChildren(this);
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
    enterOuterAlt(_localctx, 1);
    setState(45);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 2201439379588) != 0)) {
      setState(42);
      statement();
      setState(47);
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

//----------------- StatementContext ------------------------------------------------------------------

CSS3Parser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::RulesetContext* CSS3Parser::StatementContext::ruleset() {
  return getRuleContext<CSS3Parser::RulesetContext>(0);
}

CSS3Parser::AtRuleContext* CSS3Parser::StatementContext::atRule() {
  return getRuleContext<CSS3Parser::AtRuleContext>(0);
}

CSS3Parser::CommentContext* CSS3Parser::StatementContext::comment() {
  return getRuleContext<CSS3Parser::CommentContext>(0);
}


size_t CSS3Parser::StatementContext::getRuleIndex() const {
  return CSS3Parser::RuleStatement;
}

void CSS3Parser::StatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStatement(this);
}

void CSS3Parser::StatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStatement(this);
}


std::any CSS3Parser::StatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::StatementContext* CSS3Parser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 2, CSS3Parser::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(51);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CSS3Parser::T__1:
      case CSS3Parser::LBRACKET:
      case CSS3Parser::COLON:
      case CSS3Parser::DOT:
      case CSS3Parser::HASH:
      case CSS3Parser::IDENT: {
        enterOuterAlt(_localctx, 1);
        setState(48);
        ruleset();
        break;
      }

      case CSS3Parser::AT_RULE_NAME: {
        enterOuterAlt(_localctx, 2);
        setState(49);
        atRule();
        break;
      }

      case CSS3Parser::COMMENT: {
        enterOuterAlt(_localctx, 3);
        setState(50);
        comment();
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

//----------------- AtRuleContext ------------------------------------------------------------------

CSS3Parser::AtRuleContext::AtRuleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CSS3Parser::AtRuleContext::AT_RULE_NAME() {
  return getToken(CSS3Parser::AT_RULE_NAME, 0);
}

CSS3Parser::BlockContext* CSS3Parser::AtRuleContext::block() {
  return getRuleContext<CSS3Parser::BlockContext>(0);
}

tree::TerminalNode* CSS3Parser::AtRuleContext::SEMICOLON() {
  return getToken(CSS3Parser::SEMICOLON, 0);
}

std::vector<CSS3Parser::AnyContext *> CSS3Parser::AtRuleContext::any() {
  return getRuleContexts<CSS3Parser::AnyContext>();
}

CSS3Parser::AnyContext* CSS3Parser::AtRuleContext::any(size_t i) {
  return getRuleContext<CSS3Parser::AnyContext>(i);
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


std::any CSS3Parser::AtRuleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitAtRule(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::AtRuleContext* CSS3Parser::atRule() {
  AtRuleContext *_localctx = _tracker.createInstance<AtRuleContext>(_ctx, getState());
  enterRule(_localctx, 4, CSS3Parser::RuleAtRule);

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
    setState(53);
    match(CSS3Parser::AT_RULE_NAME);
    setState(57);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(54);
        any(); 
      }
      setState(59);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);
    }
    setState(62);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CSS3Parser::LBRACE: {
        setState(60);
        block();
        break;
      }

      case CSS3Parser::SEMICOLON: {
        setState(61);
        match(CSS3Parser::SEMICOLON);
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

//----------------- BlockContext ------------------------------------------------------------------

CSS3Parser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CSS3Parser::BlockContext::LBRACE() {
  return getToken(CSS3Parser::LBRACE, 0);
}

tree::TerminalNode* CSS3Parser::BlockContext::RBRACE() {
  return getToken(CSS3Parser::RBRACE, 0);
}

std::vector<CSS3Parser::DeclarationContext *> CSS3Parser::BlockContext::declaration() {
  return getRuleContexts<CSS3Parser::DeclarationContext>();
}

CSS3Parser::DeclarationContext* CSS3Parser::BlockContext::declaration(size_t i) {
  return getRuleContext<CSS3Parser::DeclarationContext>(i);
}


size_t CSS3Parser::BlockContext::getRuleIndex() const {
  return CSS3Parser::RuleBlock;
}

void CSS3Parser::BlockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlock(this);
}

void CSS3Parser::BlockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlock(this);
}


std::any CSS3Parser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::BlockContext* CSS3Parser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 6, CSS3Parser::RuleBlock);
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
    setState(64);
    match(CSS3Parser::LBRACE);
    setState(68);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::IDENT) {
      setState(65);
      declaration();
      setState(70);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(71);
    match(CSS3Parser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RulesetContext ------------------------------------------------------------------

CSS3Parser::RulesetContext::RulesetContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CSS3Parser::SelectorContext *> CSS3Parser::RulesetContext::selector() {
  return getRuleContexts<CSS3Parser::SelectorContext>();
}

CSS3Parser::SelectorContext* CSS3Parser::RulesetContext::selector(size_t i) {
  return getRuleContext<CSS3Parser::SelectorContext>(i);
}

tree::TerminalNode* CSS3Parser::RulesetContext::LBRACE() {
  return getToken(CSS3Parser::LBRACE, 0);
}

tree::TerminalNode* CSS3Parser::RulesetContext::RBRACE() {
  return getToken(CSS3Parser::RBRACE, 0);
}

std::vector<tree::TerminalNode *> CSS3Parser::RulesetContext::COMMA() {
  return getTokens(CSS3Parser::COMMA);
}

tree::TerminalNode* CSS3Parser::RulesetContext::COMMA(size_t i) {
  return getToken(CSS3Parser::COMMA, i);
}

std::vector<CSS3Parser::DeclarationContext *> CSS3Parser::RulesetContext::declaration() {
  return getRuleContexts<CSS3Parser::DeclarationContext>();
}

CSS3Parser::DeclarationContext* CSS3Parser::RulesetContext::declaration(size_t i) {
  return getRuleContext<CSS3Parser::DeclarationContext>(i);
}


size_t CSS3Parser::RulesetContext::getRuleIndex() const {
  return CSS3Parser::RuleRuleset;
}

void CSS3Parser::RulesetContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRuleset(this);
}

void CSS3Parser::RulesetContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRuleset(this);
}


std::any CSS3Parser::RulesetContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitRuleset(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::RulesetContext* CSS3Parser::ruleset() {
  RulesetContext *_localctx = _tracker.createInstance<RulesetContext>(_ctx, getState());
  enterRule(_localctx, 8, CSS3Parser::RuleRuleset);
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
    setState(73);
    selector();
    setState(78);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::COMMA) {
      setState(74);
      match(CSS3Parser::COMMA);
      setState(75);
      selector();
      setState(80);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(81);
    match(CSS3Parser::LBRACE);
    setState(85);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::IDENT) {
      setState(82);
      declaration();
      setState(87);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(88);
    match(CSS3Parser::RBRACE);
   
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


std::any CSS3Parser::SelectorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitSelector(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::SelectorContext* CSS3Parser::selector() {
  SelectorContext *_localctx = _tracker.createInstance<SelectorContext>(_ctx, getState());
  enterRule(_localctx, 10, CSS3Parser::RuleSelector);
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
    setState(90);
    simpleSelector();
    setState(96);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3670272) != 0)) {
      setState(91);
      combinator();
      setState(92);
      simpleSelector();
      setState(98);
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

//----------------- CombinatorContext ------------------------------------------------------------------

CSS3Parser::CombinatorContext::CombinatorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CSS3Parser::CombinatorContext::PLUS() {
  return getToken(CSS3Parser::PLUS, 0);
}

tree::TerminalNode* CSS3Parser::CombinatorContext::GREATER() {
  return getToken(CSS3Parser::GREATER, 0);
}

tree::TerminalNode* CSS3Parser::CombinatorContext::TILDE() {
  return getToken(CSS3Parser::TILDE, 0);
}

tree::TerminalNode* CSS3Parser::CombinatorContext::WS() {
  return getToken(CSS3Parser::WS, 0);
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


std::any CSS3Parser::CombinatorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitCombinator(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::CombinatorContext* CSS3Parser::combinator() {
  CombinatorContext *_localctx = _tracker.createInstance<CombinatorContext>(_ctx, getState());
  enterRule(_localctx, 12, CSS3Parser::RuleCombinator);
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
    setState(99);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3670272) != 0))) {
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

//----------------- SimpleSelectorContext ------------------------------------------------------------------

CSS3Parser::SimpleSelectorContext::SimpleSelectorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::ElementNameContext* CSS3Parser::SimpleSelectorContext::elementName() {
  return getRuleContext<CSS3Parser::ElementNameContext>(0);
}

tree::TerminalNode* CSS3Parser::SimpleSelectorContext::HASH() {
  return getToken(CSS3Parser::HASH, 0);
}

tree::TerminalNode* CSS3Parser::SimpleSelectorContext::DOT() {
  return getToken(CSS3Parser::DOT, 0);
}

std::vector<tree::TerminalNode *> CSS3Parser::SimpleSelectorContext::IDENT() {
  return getTokens(CSS3Parser::IDENT);
}

tree::TerminalNode* CSS3Parser::SimpleSelectorContext::IDENT(size_t i) {
  return getToken(CSS3Parser::IDENT, i);
}

tree::TerminalNode* CSS3Parser::SimpleSelectorContext::LBRACKET() {
  return getToken(CSS3Parser::LBRACKET, 0);
}

tree::TerminalNode* CSS3Parser::SimpleSelectorContext::RBRACKET() {
  return getToken(CSS3Parser::RBRACKET, 0);
}

tree::TerminalNode* CSS3Parser::SimpleSelectorContext::PREFIXMATCH() {
  return getToken(CSS3Parser::PREFIXMATCH, 0);
}

tree::TerminalNode* CSS3Parser::SimpleSelectorContext::SUFFIXMATCH() {
  return getToken(CSS3Parser::SUFFIXMATCH, 0);
}

tree::TerminalNode* CSS3Parser::SimpleSelectorContext::SUBSTRINGMATCH() {
  return getToken(CSS3Parser::SUBSTRINGMATCH, 0);
}

tree::TerminalNode* CSS3Parser::SimpleSelectorContext::INCLUDES() {
  return getToken(CSS3Parser::INCLUDES, 0);
}

tree::TerminalNode* CSS3Parser::SimpleSelectorContext::DASHMATCH() {
  return getToken(CSS3Parser::DASHMATCH, 0);
}

tree::TerminalNode* CSS3Parser::SimpleSelectorContext::STRING() {
  return getToken(CSS3Parser::STRING, 0);
}

std::vector<tree::TerminalNode *> CSS3Parser::SimpleSelectorContext::COLON() {
  return getTokens(CSS3Parser::COLON);
}

tree::TerminalNode* CSS3Parser::SimpleSelectorContext::COLON(size_t i) {
  return getToken(CSS3Parser::COLON, i);
}

tree::TerminalNode* CSS3Parser::SimpleSelectorContext::LPAREN() {
  return getToken(CSS3Parser::LPAREN, 0);
}

tree::TerminalNode* CSS3Parser::SimpleSelectorContext::RPAREN() {
  return getToken(CSS3Parser::RPAREN, 0);
}

tree::TerminalNode* CSS3Parser::SimpleSelectorContext::NUMBER() {
  return getToken(CSS3Parser::NUMBER, 0);
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


std::any CSS3Parser::SimpleSelectorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitSimpleSelector(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::SimpleSelectorContext* CSS3Parser::simpleSelector() {
  SimpleSelectorContext *_localctx = _tracker.createInstance<SimpleSelectorContext>(_ctx, getState());
  enterRule(_localctx, 14, CSS3Parser::RuleSimpleSelector);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(122);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(101);
      elementName();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(102);
      match(CSS3Parser::HASH);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(103);
      match(CSS3Parser::DOT);
      setState(104);
      match(CSS3Parser::IDENT);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(105);
      match(CSS3Parser::LBRACKET);
      setState(106);
      match(CSS3Parser::IDENT);
      setState(109);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 130023426) != 0)) {
        setState(107);
        _la = _input->LA(1);
        if (!((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 130023426) != 0))) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(108);
        _la = _input->LA(1);
        if (!(_la == CSS3Parser::STRING

        || _la == CSS3Parser::IDENT)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
      }
      setState(111);
      match(CSS3Parser::RBRACKET);
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(112);
      match(CSS3Parser::COLON);
      setState(113);
      match(CSS3Parser::IDENT);
      setState(117);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == CSS3Parser::LPAREN) {
        setState(114);
        match(CSS3Parser::LPAREN);
        setState(115);
        _la = _input->LA(1);
        if (!((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 3302829850624) != 0))) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(116);
        match(CSS3Parser::RPAREN);
      }
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(119);
      match(CSS3Parser::COLON);
      setState(120);
      match(CSS3Parser::COLON);
      setState(121);
      match(CSS3Parser::IDENT);
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

//----------------- ElementNameContext ------------------------------------------------------------------

CSS3Parser::ElementNameContext::ElementNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CSS3Parser::ElementNameContext::IDENT() {
  return getToken(CSS3Parser::IDENT, 0);
}


size_t CSS3Parser::ElementNameContext::getRuleIndex() const {
  return CSS3Parser::RuleElementName;
}

void CSS3Parser::ElementNameContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterElementName(this);
}

void CSS3Parser::ElementNameContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitElementName(this);
}


std::any CSS3Parser::ElementNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitElementName(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::ElementNameContext* CSS3Parser::elementName() {
  ElementNameContext *_localctx = _tracker.createInstance<ElementNameContext>(_ctx, getState());
  enterRule(_localctx, 16, CSS3Parser::RuleElementName);
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
    setState(124);
    _la = _input->LA(1);
    if (!(_la == CSS3Parser::T__1

    || _la == CSS3Parser::IDENT)) {
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

//----------------- DeclarationContext ------------------------------------------------------------------

CSS3Parser::DeclarationContext::DeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CSS3Parser::PropertyContext* CSS3Parser::DeclarationContext::property() {
  return getRuleContext<CSS3Parser::PropertyContext>(0);
}

tree::TerminalNode* CSS3Parser::DeclarationContext::COLON() {
  return getToken(CSS3Parser::COLON, 0);
}

CSS3Parser::ExprContext* CSS3Parser::DeclarationContext::expr() {
  return getRuleContext<CSS3Parser::ExprContext>(0);
}

CSS3Parser::PrioContext* CSS3Parser::DeclarationContext::prio() {
  return getRuleContext<CSS3Parser::PrioContext>(0);
}

tree::TerminalNode* CSS3Parser::DeclarationContext::SEMICOLON() {
  return getToken(CSS3Parser::SEMICOLON, 0);
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


std::any CSS3Parser::DeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitDeclaration(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::DeclarationContext* CSS3Parser::declaration() {
  DeclarationContext *_localctx = _tracker.createInstance<DeclarationContext>(_ctx, getState());
  enterRule(_localctx, 18, CSS3Parser::RuleDeclaration);
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
    setState(126);
    property();
    setState(127);
    match(CSS3Parser::COLON);
    setState(128);
    expr();
    setState(130);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CSS3Parser::IMPORTANT_SYM) {
      setState(129);
      prio();
    }
    setState(133);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CSS3Parser::SEMICOLON) {
      setState(132);
      match(CSS3Parser::SEMICOLON);
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

tree::TerminalNode* CSS3Parser::PrioContext::IMPORTANT_SYM() {
  return getToken(CSS3Parser::IMPORTANT_SYM, 0);
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


std::any CSS3Parser::PrioContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitPrio(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::PrioContext* CSS3Parser::prio() {
  PrioContext *_localctx = _tracker.createInstance<PrioContext>(_ctx, getState());
  enterRule(_localctx, 20, CSS3Parser::RulePrio);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(135);
    match(CSS3Parser::IMPORTANT_SYM);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

CSS3Parser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CSS3Parser::TermContext *> CSS3Parser::ExprContext::term() {
  return getRuleContexts<CSS3Parser::TermContext>();
}

CSS3Parser::TermContext* CSS3Parser::ExprContext::term(size_t i) {
  return getRuleContext<CSS3Parser::TermContext>(i);
}

std::vector<CSS3Parser::CssOperatorContext *> CSS3Parser::ExprContext::cssOperator() {
  return getRuleContexts<CSS3Parser::CssOperatorContext>();
}

CSS3Parser::CssOperatorContext* CSS3Parser::ExprContext::cssOperator(size_t i) {
  return getRuleContext<CSS3Parser::CssOperatorContext>(i);
}


size_t CSS3Parser::ExprContext::getRuleIndex() const {
  return CSS3Parser::RuleExpr;
}

void CSS3Parser::ExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpr(this);
}

void CSS3Parser::ExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpr(this);
}


std::any CSS3Parser::ExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitExpr(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::ExprContext* CSS3Parser::expr() {
  ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, getState());
  enterRule(_localctx, 22, CSS3Parser::RuleExpr);
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
    setState(137);
    term();
    setState(143);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::T__4

    || _la == CSS3Parser::COMMA) {
      setState(138);
      cssOperator();
      setState(139);
      term();
      setState(145);
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

tree::TerminalNode* CSS3Parser::TermContext::NUMBER() {
  return getToken(CSS3Parser::NUMBER, 0);
}

tree::TerminalNode* CSS3Parser::TermContext::STRING() {
  return getToken(CSS3Parser::STRING, 0);
}

tree::TerminalNode* CSS3Parser::TermContext::IDENT() {
  return getToken(CSS3Parser::IDENT, 0);
}

tree::TerminalNode* CSS3Parser::TermContext::URI() {
  return getToken(CSS3Parser::URI, 0);
}

tree::TerminalNode* CSS3Parser::TermContext::HASH() {
  return getToken(CSS3Parser::HASH, 0);
}

CSS3Parser::UnicodeRangeContext* CSS3Parser::TermContext::unicodeRange() {
  return getRuleContext<CSS3Parser::UnicodeRangeContext>(0);
}

CSS3Parser::HexcolorContext* CSS3Parser::TermContext::hexcolor() {
  return getRuleContext<CSS3Parser::HexcolorContext>(0);
}

CSS3Parser::FunctionContext* CSS3Parser::TermContext::function() {
  return getRuleContext<CSS3Parser::FunctionContext>(0);
}

CSS3Parser::UnaryOperatorContext* CSS3Parser::TermContext::unaryOperator() {
  return getRuleContext<CSS3Parser::UnaryOperatorContext>(0);
}

tree::TerminalNode* CSS3Parser::TermContext::PERCENTAGE() {
  return getToken(CSS3Parser::PERCENTAGE, 0);
}

tree::TerminalNode* CSS3Parser::TermContext::LENGTH() {
  return getToken(CSS3Parser::LENGTH, 0);
}

tree::TerminalNode* CSS3Parser::TermContext::EMS() {
  return getToken(CSS3Parser::EMS, 0);
}

tree::TerminalNode* CSS3Parser::TermContext::EXS() {
  return getToken(CSS3Parser::EXS, 0);
}

tree::TerminalNode* CSS3Parser::TermContext::ANGLE() {
  return getToken(CSS3Parser::ANGLE, 0);
}

tree::TerminalNode* CSS3Parser::TermContext::TIME() {
  return getToken(CSS3Parser::TIME, 0);
}

tree::TerminalNode* CSS3Parser::TermContext::FREQ() {
  return getToken(CSS3Parser::FREQ, 0);
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


std::any CSS3Parser::TermContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitTerm(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::TermContext* CSS3Parser::term() {
  TermContext *_localctx = _tracker.createInstance<TermContext>(_ctx, getState());
  enterRule(_localctx, 24, CSS3Parser::RuleTerm);
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
    setState(147);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CSS3Parser::T__5

    || _la == CSS3Parser::PLUS) {
      setState(146);
      unaryOperator();
    }
    setState(160);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 16, _ctx)) {
    case 1: {
      setState(149);
      match(CSS3Parser::NUMBER);
      setState(151);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx)) {
      case 1: {
        setState(150);
        _la = _input->LA(1);
        if (!((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 3289944948736) != 0))) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        break;
      }

      default:
        break;
      }
      break;
    }

    case 2: {
      setState(153);
      match(CSS3Parser::STRING);
      break;
    }

    case 3: {
      setState(154);
      match(CSS3Parser::IDENT);
      break;
    }

    case 4: {
      setState(155);
      match(CSS3Parser::URI);
      break;
    }

    case 5: {
      setState(156);
      match(CSS3Parser::HASH);
      break;
    }

    case 6: {
      setState(157);
      unicodeRange();
      break;
    }

    case 7: {
      setState(158);
      hexcolor();
      break;
    }

    case 8: {
      setState(159);
      function();
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

//----------------- FunctionContext ------------------------------------------------------------------

CSS3Parser::FunctionContext::FunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CSS3Parser::FunctionContext::FUNCTION() {
  return getToken(CSS3Parser::FUNCTION, 0);
}

CSS3Parser::ExprContext* CSS3Parser::FunctionContext::expr() {
  return getRuleContext<CSS3Parser::ExprContext>(0);
}

tree::TerminalNode* CSS3Parser::FunctionContext::RPAREN() {
  return getToken(CSS3Parser::RPAREN, 0);
}


size_t CSS3Parser::FunctionContext::getRuleIndex() const {
  return CSS3Parser::RuleFunction;
}

void CSS3Parser::FunctionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunction(this);
}

void CSS3Parser::FunctionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunction(this);
}


std::any CSS3Parser::FunctionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitFunction(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::FunctionContext* CSS3Parser::function() {
  FunctionContext *_localctx = _tracker.createInstance<FunctionContext>(_ctx, getState());
  enterRule(_localctx, 26, CSS3Parser::RuleFunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(162);
    match(CSS3Parser::FUNCTION);
    setState(163);
    expr();
    setState(164);
    match(CSS3Parser::RPAREN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnicodeRangeContext ------------------------------------------------------------------

CSS3Parser::UnicodeRangeContext::UnicodeRangeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> CSS3Parser::UnicodeRangeContext::HEX_DIGIT() {
  return getTokens(CSS3Parser::HEX_DIGIT);
}

tree::TerminalNode* CSS3Parser::UnicodeRangeContext::HEX_DIGIT(size_t i) {
  return getToken(CSS3Parser::HEX_DIGIT, i);
}


size_t CSS3Parser::UnicodeRangeContext::getRuleIndex() const {
  return CSS3Parser::RuleUnicodeRange;
}

void CSS3Parser::UnicodeRangeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnicodeRange(this);
}

void CSS3Parser::UnicodeRangeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnicodeRange(this);
}


std::any CSS3Parser::UnicodeRangeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitUnicodeRange(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::UnicodeRangeContext* CSS3Parser::unicodeRange() {
  UnicodeRangeContext *_localctx = _tracker.createInstance<UnicodeRangeContext>(_ctx, getState());
  enterRule(_localctx, 28, CSS3Parser::RuleUnicodeRange);
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
    setState(166);
    match(CSS3Parser::T__2);
    setState(167);
    match(CSS3Parser::HEX_DIGIT);
    setState(171);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CSS3Parser::T__3

    || _la == CSS3Parser::HEX_DIGIT) {
      setState(168);
      _la = _input->LA(1);
      if (!(_la == CSS3Parser::T__3

      || _la == CSS3Parser::HEX_DIGIT)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(173);
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

//----------------- HexcolorContext ------------------------------------------------------------------

CSS3Parser::HexcolorContext::HexcolorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CSS3Parser::HexcolorContext::HASH() {
  return getToken(CSS3Parser::HASH, 0);
}

std::vector<tree::TerminalNode *> CSS3Parser::HexcolorContext::HEX_DIGIT() {
  return getTokens(CSS3Parser::HEX_DIGIT);
}

tree::TerminalNode* CSS3Parser::HexcolorContext::HEX_DIGIT(size_t i) {
  return getToken(CSS3Parser::HEX_DIGIT, i);
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


std::any CSS3Parser::HexcolorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitHexcolor(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::HexcolorContext* CSS3Parser::hexcolor() {
  HexcolorContext *_localctx = _tracker.createInstance<HexcolorContext>(_ctx, getState());
  enterRule(_localctx, 30, CSS3Parser::RuleHexcolor);
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
    setState(174);
    match(CSS3Parser::HASH);
    setState(175);
    match(CSS3Parser::HEX_DIGIT);
    setState(176);
    match(CSS3Parser::HEX_DIGIT);
    setState(177);
    match(CSS3Parser::HEX_DIGIT);
    setState(181);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CSS3Parser::HEX_DIGIT) {
      setState(178);
      match(CSS3Parser::HEX_DIGIT);
      setState(179);
      match(CSS3Parser::HEX_DIGIT);
      setState(180);
      match(CSS3Parser::HEX_DIGIT);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CssOperatorContext ------------------------------------------------------------------

CSS3Parser::CssOperatorContext::CssOperatorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CSS3Parser::CssOperatorContext::COMMA() {
  return getToken(CSS3Parser::COMMA, 0);
}


size_t CSS3Parser::CssOperatorContext::getRuleIndex() const {
  return CSS3Parser::RuleCssOperator;
}

void CSS3Parser::CssOperatorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCssOperator(this);
}

void CSS3Parser::CssOperatorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCssOperator(this);
}


std::any CSS3Parser::CssOperatorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitCssOperator(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::CssOperatorContext* CSS3Parser::cssOperator() {
  CssOperatorContext *_localctx = _tracker.createInstance<CssOperatorContext>(_ctx, getState());
  enterRule(_localctx, 32, CSS3Parser::RuleCssOperator);
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
    setState(183);
    _la = _input->LA(1);
    if (!(_la == CSS3Parser::T__4

    || _la == CSS3Parser::COMMA)) {
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

//----------------- UnaryOperatorContext ------------------------------------------------------------------

CSS3Parser::UnaryOperatorContext::UnaryOperatorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CSS3Parser::UnaryOperatorContext::PLUS() {
  return getToken(CSS3Parser::PLUS, 0);
}


size_t CSS3Parser::UnaryOperatorContext::getRuleIndex() const {
  return CSS3Parser::RuleUnaryOperator;
}

void CSS3Parser::UnaryOperatorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnaryOperator(this);
}

void CSS3Parser::UnaryOperatorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnaryOperator(this);
}


std::any CSS3Parser::UnaryOperatorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitUnaryOperator(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::UnaryOperatorContext* CSS3Parser::unaryOperator() {
  UnaryOperatorContext *_localctx = _tracker.createInstance<UnaryOperatorContext>(_ctx, getState());
  enterRule(_localctx, 34, CSS3Parser::RuleUnaryOperator);
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
    setState(185);
    _la = _input->LA(1);
    if (!(_la == CSS3Parser::T__5

    || _la == CSS3Parser::PLUS)) {
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

//----------------- PropertyContext ------------------------------------------------------------------

CSS3Parser::PropertyContext::PropertyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CSS3Parser::PropertyContext::IDENT() {
  return getToken(CSS3Parser::IDENT, 0);
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


std::any CSS3Parser::PropertyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitProperty(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::PropertyContext* CSS3Parser::property() {
  PropertyContext *_localctx = _tracker.createInstance<PropertyContext>(_ctx, getState());
  enterRule(_localctx, 36, CSS3Parser::RuleProperty);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(187);
    match(CSS3Parser::IDENT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AnyContext ------------------------------------------------------------------

CSS3Parser::AnyContext::AnyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CSS3Parser::AnyContext::IDENT() {
  return getToken(CSS3Parser::IDENT, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::NUMBER() {
  return getToken(CSS3Parser::NUMBER, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::PERCENTAGE() {
  return getToken(CSS3Parser::PERCENTAGE, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::LENGTH() {
  return getToken(CSS3Parser::LENGTH, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::EMS() {
  return getToken(CSS3Parser::EMS, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::EXS() {
  return getToken(CSS3Parser::EXS, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::ANGLE() {
  return getToken(CSS3Parser::ANGLE, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::TIME() {
  return getToken(CSS3Parser::TIME, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::FREQ() {
  return getToken(CSS3Parser::FREQ, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::STRING() {
  return getToken(CSS3Parser::STRING, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::URI() {
  return getToken(CSS3Parser::URI, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::HASH() {
  return getToken(CSS3Parser::HASH, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::PLUS() {
  return getToken(CSS3Parser::PLUS, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::GREATER() {
  return getToken(CSS3Parser::GREATER, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::COMMA() {
  return getToken(CSS3Parser::COMMA, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::TILDE() {
  return getToken(CSS3Parser::TILDE, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::COLON() {
  return getToken(CSS3Parser::COLON, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::SEMICOLON() {
  return getToken(CSS3Parser::SEMICOLON, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::LBRACE() {
  return getToken(CSS3Parser::LBRACE, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::RBRACE() {
  return getToken(CSS3Parser::RBRACE, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::LPAREN() {
  return getToken(CSS3Parser::LPAREN, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::RPAREN() {
  return getToken(CSS3Parser::RPAREN, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::LBRACKET() {
  return getToken(CSS3Parser::LBRACKET, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::RBRACKET() {
  return getToken(CSS3Parser::RBRACKET, 0);
}

CSS3Parser::FunctionContext* CSS3Parser::AnyContext::function() {
  return getRuleContext<CSS3Parser::FunctionContext>(0);
}

tree::TerminalNode* CSS3Parser::AnyContext::INCLUDES() {
  return getToken(CSS3Parser::INCLUDES, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::DASHMATCH() {
  return getToken(CSS3Parser::DASHMATCH, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::PREFIXMATCH() {
  return getToken(CSS3Parser::PREFIXMATCH, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::SUFFIXMATCH() {
  return getToken(CSS3Parser::SUFFIXMATCH, 0);
}

tree::TerminalNode* CSS3Parser::AnyContext::SUBSTRINGMATCH() {
  return getToken(CSS3Parser::SUBSTRINGMATCH, 0);
}


size_t CSS3Parser::AnyContext::getRuleIndex() const {
  return CSS3Parser::RuleAny;
}

void CSS3Parser::AnyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAny(this);
}

void CSS3Parser::AnyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAny(this);
}


std::any CSS3Parser::AnyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitAny(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::AnyContext* CSS3Parser::any() {
  AnyContext *_localctx = _tracker.createInstance<AnyContext>(_ctx, getState());
  enterRule(_localctx, 38, CSS3Parser::RuleAny);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(219);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CSS3Parser::IDENT: {
        enterOuterAlt(_localctx, 1);
        setState(189);
        match(CSS3Parser::IDENT);
        break;
      }

      case CSS3Parser::NUMBER: {
        enterOuterAlt(_localctx, 2);
        setState(190);
        match(CSS3Parser::NUMBER);
        break;
      }

      case CSS3Parser::PERCENTAGE: {
        enterOuterAlt(_localctx, 3);
        setState(191);
        match(CSS3Parser::PERCENTAGE);
        break;
      }

      case CSS3Parser::LENGTH: {
        enterOuterAlt(_localctx, 4);
        setState(192);
        match(CSS3Parser::LENGTH);
        break;
      }

      case CSS3Parser::EMS: {
        enterOuterAlt(_localctx, 5);
        setState(193);
        match(CSS3Parser::EMS);
        break;
      }

      case CSS3Parser::EXS: {
        enterOuterAlt(_localctx, 6);
        setState(194);
        match(CSS3Parser::EXS);
        break;
      }

      case CSS3Parser::ANGLE: {
        enterOuterAlt(_localctx, 7);
        setState(195);
        match(CSS3Parser::ANGLE);
        break;
      }

      case CSS3Parser::TIME: {
        enterOuterAlt(_localctx, 8);
        setState(196);
        match(CSS3Parser::TIME);
        break;
      }

      case CSS3Parser::FREQ: {
        enterOuterAlt(_localctx, 9);
        setState(197);
        match(CSS3Parser::FREQ);
        break;
      }

      case CSS3Parser::STRING: {
        enterOuterAlt(_localctx, 10);
        setState(198);
        match(CSS3Parser::STRING);
        break;
      }

      case CSS3Parser::URI: {
        enterOuterAlt(_localctx, 11);
        setState(199);
        match(CSS3Parser::URI);
        break;
      }

      case CSS3Parser::HASH: {
        enterOuterAlt(_localctx, 12);
        setState(200);
        match(CSS3Parser::HASH);
        break;
      }

      case CSS3Parser::PLUS: {
        enterOuterAlt(_localctx, 13);
        setState(201);
        match(CSS3Parser::PLUS);
        break;
      }

      case CSS3Parser::GREATER: {
        enterOuterAlt(_localctx, 14);
        setState(202);
        match(CSS3Parser::GREATER);
        break;
      }

      case CSS3Parser::COMMA: {
        enterOuterAlt(_localctx, 15);
        setState(203);
        match(CSS3Parser::COMMA);
        break;
      }

      case CSS3Parser::TILDE: {
        enterOuterAlt(_localctx, 16);
        setState(204);
        match(CSS3Parser::TILDE);
        break;
      }

      case CSS3Parser::COLON: {
        enterOuterAlt(_localctx, 17);
        setState(205);
        match(CSS3Parser::COLON);
        break;
      }

      case CSS3Parser::SEMICOLON: {
        enterOuterAlt(_localctx, 18);
        setState(206);
        match(CSS3Parser::SEMICOLON);
        break;
      }

      case CSS3Parser::LBRACE: {
        enterOuterAlt(_localctx, 19);
        setState(207);
        match(CSS3Parser::LBRACE);
        break;
      }

      case CSS3Parser::RBRACE: {
        enterOuterAlt(_localctx, 20);
        setState(208);
        match(CSS3Parser::RBRACE);
        break;
      }

      case CSS3Parser::LPAREN: {
        enterOuterAlt(_localctx, 21);
        setState(209);
        match(CSS3Parser::LPAREN);
        break;
      }

      case CSS3Parser::RPAREN: {
        enterOuterAlt(_localctx, 22);
        setState(210);
        match(CSS3Parser::RPAREN);
        break;
      }

      case CSS3Parser::LBRACKET: {
        enterOuterAlt(_localctx, 23);
        setState(211);
        match(CSS3Parser::LBRACKET);
        break;
      }

      case CSS3Parser::RBRACKET: {
        enterOuterAlt(_localctx, 24);
        setState(212);
        match(CSS3Parser::RBRACKET);
        break;
      }

      case CSS3Parser::FUNCTION: {
        enterOuterAlt(_localctx, 25);
        setState(213);
        function();
        break;
      }

      case CSS3Parser::INCLUDES: {
        enterOuterAlt(_localctx, 26);
        setState(214);
        match(CSS3Parser::INCLUDES);
        break;
      }

      case CSS3Parser::DASHMATCH: {
        enterOuterAlt(_localctx, 27);
        setState(215);
        match(CSS3Parser::DASHMATCH);
        break;
      }

      case CSS3Parser::PREFIXMATCH: {
        enterOuterAlt(_localctx, 28);
        setState(216);
        match(CSS3Parser::PREFIXMATCH);
        break;
      }

      case CSS3Parser::SUFFIXMATCH: {
        enterOuterAlt(_localctx, 29);
        setState(217);
        match(CSS3Parser::SUFFIXMATCH);
        break;
      }

      case CSS3Parser::SUBSTRINGMATCH: {
        enterOuterAlt(_localctx, 30);
        setState(218);
        match(CSS3Parser::SUBSTRINGMATCH);
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

//----------------- CommentContext ------------------------------------------------------------------

CSS3Parser::CommentContext::CommentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CSS3Parser::CommentContext::COMMENT() {
  return getToken(CSS3Parser::COMMENT, 0);
}


size_t CSS3Parser::CommentContext::getRuleIndex() const {
  return CSS3Parser::RuleComment;
}

void CSS3Parser::CommentContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterComment(this);
}

void CSS3Parser::CommentContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CSS3Listener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitComment(this);
}


std::any CSS3Parser::CommentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CSS3Visitor*>(visitor))
    return parserVisitor->visitComment(this);
  else
    return visitor->visitChildren(this);
}

CSS3Parser::CommentContext* CSS3Parser::comment() {
  CommentContext *_localctx = _tracker.createInstance<CommentContext>(_ctx, getState());
  enterRule(_localctx, 40, CSS3Parser::RuleComment);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(221);
    match(CSS3Parser::COMMENT);
   
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

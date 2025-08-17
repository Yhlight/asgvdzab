
// Generated from grammar/SimpleCSS.g4 by ANTLR 4.13.1


#include "SimpleCSSVisitor.h"

#include "SimpleCSSParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct SimpleCSSParserStaticData final {
  SimpleCSSParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  SimpleCSSParserStaticData(const SimpleCSSParserStaticData&) = delete;
  SimpleCSSParserStaticData(SimpleCSSParserStaticData&&) = delete;
  SimpleCSSParserStaticData& operator=(const SimpleCSSParserStaticData&) = delete;
  SimpleCSSParserStaticData& operator=(SimpleCSSParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag simplecssParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
SimpleCSSParserStaticData *simplecssParserStaticData = nullptr;

void simplecssParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (simplecssParserStaticData != nullptr) {
    return;
  }
#else
  assert(simplecssParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<SimpleCSSParserStaticData>(
    std::vector<std::string>{
      "stylesheet", "statement", "ruleSet", "selector", "simpleSelector", 
      "elementName", "className", "idName", "pseudoClass", "pseudoElement", 
      "attribute", "attributeOperator", "declaration", "property", "value", 
      "term", "unit", "function", "expression", "atRule", "block", "any"
    },
    std::vector<std::string>{
      "", "','", "'{'", "'}'", "'.'", "'#'", "'*'", "':'", "'::'", "'['", 
      "']'", "'('", "')'", "'='", "'~='", "'|='", "'^='", "'$='", "'*='", 
      "';'", "'px'", "'em'", "'rem'", "'%'", "'vh'", "'vw'", "'pt'", "'cm'", 
      "'mm'", "'in'", "'@'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "COMMENT", 
      "WS", "COLOR", "NUMBER", "PERCENTAGE", "STRING", "URI", "IDENT", "ANY_CHAR"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,39,196,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,1,0,5,0,46,8,0,10,0,12,0,49,9,0,1,0,1,0,1,1,1,1,3,1,55,8,1,1,2,1,2,
  	1,2,5,2,60,8,2,10,2,12,2,63,9,2,1,2,1,2,5,2,67,8,2,10,2,12,2,70,9,2,1,
  	2,1,2,1,3,4,3,75,8,3,11,3,12,3,76,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,
  	1,4,1,4,1,4,1,4,1,4,3,4,93,8,4,1,5,1,5,1,6,1,6,1,7,1,7,1,8,1,8,1,8,1,
  	8,1,8,3,8,106,8,8,1,9,1,9,1,10,1,10,1,10,1,10,3,10,114,8,10,1,11,1,11,
  	1,12,1,12,1,12,4,12,121,8,12,11,12,12,12,122,1,12,3,12,126,8,12,1,13,
  	1,13,1,14,1,14,1,14,1,14,1,14,1,14,5,14,136,8,14,10,14,12,14,139,9,14,
  	1,15,1,15,3,15,143,8,15,1,15,1,15,1,15,1,15,1,15,1,15,3,15,151,8,15,1,
  	16,1,16,1,17,1,17,1,17,1,17,1,17,1,18,4,18,161,8,18,11,18,12,18,162,1,
  	19,1,19,1,19,5,19,168,8,19,10,19,12,19,171,9,19,1,19,1,19,3,19,175,8,
  	19,1,20,1,20,5,20,179,8,20,10,20,12,20,182,9,20,1,20,1,20,1,21,1,21,1,
  	21,1,21,1,21,1,21,1,21,1,21,3,21,194,8,21,1,21,0,1,28,22,0,2,4,6,8,10,
  	12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,0,3,2,0,36,36,38,38,1,
  	0,13,18,1,0,20,29,207,0,47,1,0,0,0,2,54,1,0,0,0,4,56,1,0,0,0,6,74,1,0,
  	0,0,8,92,1,0,0,0,10,94,1,0,0,0,12,96,1,0,0,0,14,98,1,0,0,0,16,100,1,0,
  	0,0,18,107,1,0,0,0,20,109,1,0,0,0,22,115,1,0,0,0,24,117,1,0,0,0,26,127,
  	1,0,0,0,28,129,1,0,0,0,30,150,1,0,0,0,32,152,1,0,0,0,34,154,1,0,0,0,36,
  	160,1,0,0,0,38,164,1,0,0,0,40,176,1,0,0,0,42,193,1,0,0,0,44,46,3,2,1,
  	0,45,44,1,0,0,0,46,49,1,0,0,0,47,45,1,0,0,0,47,48,1,0,0,0,48,50,1,0,0,
  	0,49,47,1,0,0,0,50,51,5,0,0,1,51,1,1,0,0,0,52,55,3,4,2,0,53,55,3,38,19,
  	0,54,52,1,0,0,0,54,53,1,0,0,0,55,3,1,0,0,0,56,61,3,6,3,0,57,58,5,1,0,
  	0,58,60,3,6,3,0,59,57,1,0,0,0,60,63,1,0,0,0,61,59,1,0,0,0,61,62,1,0,0,
  	0,62,64,1,0,0,0,63,61,1,0,0,0,64,68,5,2,0,0,65,67,3,24,12,0,66,65,1,0,
  	0,0,67,70,1,0,0,0,68,66,1,0,0,0,68,69,1,0,0,0,69,71,1,0,0,0,70,68,1,0,
  	0,0,71,72,5,3,0,0,72,5,1,0,0,0,73,75,3,8,4,0,74,73,1,0,0,0,75,76,1,0,
  	0,0,76,74,1,0,0,0,76,77,1,0,0,0,77,7,1,0,0,0,78,93,3,10,5,0,79,80,5,4,
  	0,0,80,93,3,12,6,0,81,82,5,5,0,0,82,93,3,14,7,0,83,93,5,6,0,0,84,85,5,
  	7,0,0,85,93,3,16,8,0,86,87,5,8,0,0,87,93,3,18,9,0,88,89,5,9,0,0,89,90,
  	3,20,10,0,90,91,5,10,0,0,91,93,1,0,0,0,92,78,1,0,0,0,92,79,1,0,0,0,92,
  	81,1,0,0,0,92,83,1,0,0,0,92,84,1,0,0,0,92,86,1,0,0,0,92,88,1,0,0,0,93,
  	9,1,0,0,0,94,95,5,38,0,0,95,11,1,0,0,0,96,97,5,38,0,0,97,13,1,0,0,0,98,
  	99,5,38,0,0,99,15,1,0,0,0,100,105,5,38,0,0,101,102,5,11,0,0,102,103,3,
  	36,18,0,103,104,5,12,0,0,104,106,1,0,0,0,105,101,1,0,0,0,105,106,1,0,
  	0,0,106,17,1,0,0,0,107,108,5,38,0,0,108,19,1,0,0,0,109,113,5,38,0,0,110,
  	111,3,22,11,0,111,112,7,0,0,0,112,114,1,0,0,0,113,110,1,0,0,0,113,114,
  	1,0,0,0,114,21,1,0,0,0,115,116,7,1,0,0,116,23,1,0,0,0,117,118,3,26,13,
  	0,118,120,5,7,0,0,119,121,3,28,14,0,120,119,1,0,0,0,121,122,1,0,0,0,122,
  	120,1,0,0,0,122,123,1,0,0,0,123,125,1,0,0,0,124,126,5,19,0,0,125,124,
  	1,0,0,0,125,126,1,0,0,0,126,25,1,0,0,0,127,128,5,38,0,0,128,27,1,0,0,
  	0,129,130,6,14,-1,0,130,131,3,30,15,0,131,137,1,0,0,0,132,133,10,1,0,
  	0,133,134,5,1,0,0,134,136,3,28,14,2,135,132,1,0,0,0,136,139,1,0,0,0,137,
  	135,1,0,0,0,137,138,1,0,0,0,138,29,1,0,0,0,139,137,1,0,0,0,140,142,5,
  	34,0,0,141,143,3,32,16,0,142,141,1,0,0,0,142,143,1,0,0,0,143,151,1,0,
  	0,0,144,151,5,35,0,0,145,151,5,33,0,0,146,151,5,36,0,0,147,151,5,38,0,
  	0,148,151,5,37,0,0,149,151,3,34,17,0,150,140,1,0,0,0,150,144,1,0,0,0,
  	150,145,1,0,0,0,150,146,1,0,0,0,150,147,1,0,0,0,150,148,1,0,0,0,150,149,
  	1,0,0,0,151,31,1,0,0,0,152,153,7,2,0,0,153,33,1,0,0,0,154,155,5,38,0,
  	0,155,156,5,11,0,0,156,157,3,36,18,0,157,158,5,12,0,0,158,35,1,0,0,0,
  	159,161,3,30,15,0,160,159,1,0,0,0,161,162,1,0,0,0,162,160,1,0,0,0,162,
  	163,1,0,0,0,163,37,1,0,0,0,164,165,5,30,0,0,165,169,5,38,0,0,166,168,
  	3,42,21,0,167,166,1,0,0,0,168,171,1,0,0,0,169,167,1,0,0,0,169,170,1,0,
  	0,0,170,174,1,0,0,0,171,169,1,0,0,0,172,175,3,40,20,0,173,175,5,19,0,
  	0,174,172,1,0,0,0,174,173,1,0,0,0,175,39,1,0,0,0,176,180,5,2,0,0,177,
  	179,3,42,21,0,178,177,1,0,0,0,179,182,1,0,0,0,180,178,1,0,0,0,180,181,
  	1,0,0,0,181,183,1,0,0,0,182,180,1,0,0,0,183,184,5,3,0,0,184,41,1,0,0,
  	0,185,194,5,38,0,0,186,194,5,34,0,0,187,194,5,36,0,0,188,194,5,33,0,0,
  	189,194,5,1,0,0,190,194,5,7,0,0,191,194,5,19,0,0,192,194,3,40,20,0,193,
  	185,1,0,0,0,193,186,1,0,0,0,193,187,1,0,0,0,193,188,1,0,0,0,193,189,1,
  	0,0,0,193,190,1,0,0,0,193,191,1,0,0,0,193,192,1,0,0,0,194,43,1,0,0,0,
  	18,47,54,61,68,76,92,105,113,122,125,137,142,150,162,169,174,180,193
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  simplecssParserStaticData = staticData.release();
}

}

SimpleCSSParser::SimpleCSSParser(TokenStream *input) : SimpleCSSParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

SimpleCSSParser::SimpleCSSParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  SimpleCSSParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *simplecssParserStaticData->atn, simplecssParserStaticData->decisionToDFA, simplecssParserStaticData->sharedContextCache, options);
}

SimpleCSSParser::~SimpleCSSParser() {
  delete _interpreter;
}

const atn::ATN& SimpleCSSParser::getATN() const {
  return *simplecssParserStaticData->atn;
}

std::string SimpleCSSParser::getGrammarFileName() const {
  return "SimpleCSS.g4";
}

const std::vector<std::string>& SimpleCSSParser::getRuleNames() const {
  return simplecssParserStaticData->ruleNames;
}

const dfa::Vocabulary& SimpleCSSParser::getVocabulary() const {
  return simplecssParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView SimpleCSSParser::getSerializedATN() const {
  return simplecssParserStaticData->serializedATN;
}


//----------------- StylesheetContext ------------------------------------------------------------------

SimpleCSSParser::StylesheetContext::StylesheetContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SimpleCSSParser::StylesheetContext::EOF() {
  return getToken(SimpleCSSParser::EOF, 0);
}

std::vector<SimpleCSSParser::StatementContext *> SimpleCSSParser::StylesheetContext::statement() {
  return getRuleContexts<SimpleCSSParser::StatementContext>();
}

SimpleCSSParser::StatementContext* SimpleCSSParser::StylesheetContext::statement(size_t i) {
  return getRuleContext<SimpleCSSParser::StatementContext>(i);
}


size_t SimpleCSSParser::StylesheetContext::getRuleIndex() const {
  return SimpleCSSParser::RuleStylesheet;
}


std::any SimpleCSSParser::StylesheetContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitStylesheet(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::StylesheetContext* SimpleCSSParser::stylesheet() {
  StylesheetContext *_localctx = _tracker.createInstance<StylesheetContext>(_ctx, getState());
  enterRule(_localctx, 0, SimpleCSSParser::RuleStylesheet);
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
    setState(47);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 275951649776) != 0)) {
      setState(44);
      statement();
      setState(49);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(50);
    match(SimpleCSSParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

SimpleCSSParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SimpleCSSParser::RuleSetContext* SimpleCSSParser::StatementContext::ruleSet() {
  return getRuleContext<SimpleCSSParser::RuleSetContext>(0);
}

SimpleCSSParser::AtRuleContext* SimpleCSSParser::StatementContext::atRule() {
  return getRuleContext<SimpleCSSParser::AtRuleContext>(0);
}


size_t SimpleCSSParser::StatementContext::getRuleIndex() const {
  return SimpleCSSParser::RuleStatement;
}


std::any SimpleCSSParser::StatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::StatementContext* SimpleCSSParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 2, SimpleCSSParser::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(54);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SimpleCSSParser::T__3:
      case SimpleCSSParser::T__4:
      case SimpleCSSParser::T__5:
      case SimpleCSSParser::T__6:
      case SimpleCSSParser::T__7:
      case SimpleCSSParser::T__8:
      case SimpleCSSParser::IDENT: {
        enterOuterAlt(_localctx, 1);
        setState(52);
        ruleSet();
        break;
      }

      case SimpleCSSParser::T__29: {
        enterOuterAlt(_localctx, 2);
        setState(53);
        atRule();
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

//----------------- RuleSetContext ------------------------------------------------------------------

SimpleCSSParser::RuleSetContext::RuleSetContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SimpleCSSParser::SelectorContext *> SimpleCSSParser::RuleSetContext::selector() {
  return getRuleContexts<SimpleCSSParser::SelectorContext>();
}

SimpleCSSParser::SelectorContext* SimpleCSSParser::RuleSetContext::selector(size_t i) {
  return getRuleContext<SimpleCSSParser::SelectorContext>(i);
}

std::vector<SimpleCSSParser::DeclarationContext *> SimpleCSSParser::RuleSetContext::declaration() {
  return getRuleContexts<SimpleCSSParser::DeclarationContext>();
}

SimpleCSSParser::DeclarationContext* SimpleCSSParser::RuleSetContext::declaration(size_t i) {
  return getRuleContext<SimpleCSSParser::DeclarationContext>(i);
}


size_t SimpleCSSParser::RuleSetContext::getRuleIndex() const {
  return SimpleCSSParser::RuleRuleSet;
}


std::any SimpleCSSParser::RuleSetContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitRuleSet(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::RuleSetContext* SimpleCSSParser::ruleSet() {
  RuleSetContext *_localctx = _tracker.createInstance<RuleSetContext>(_ctx, getState());
  enterRule(_localctx, 4, SimpleCSSParser::RuleRuleSet);
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
    setState(56);
    selector();
    setState(61);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SimpleCSSParser::T__0) {
      setState(57);
      match(SimpleCSSParser::T__0);
      setState(58);
      selector();
      setState(63);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(64);
    match(SimpleCSSParser::T__1);
    setState(68);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SimpleCSSParser::IDENT) {
      setState(65);
      declaration();
      setState(70);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(71);
    match(SimpleCSSParser::T__2);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SelectorContext ------------------------------------------------------------------

SimpleCSSParser::SelectorContext::SelectorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SimpleCSSParser::SimpleSelectorContext *> SimpleCSSParser::SelectorContext::simpleSelector() {
  return getRuleContexts<SimpleCSSParser::SimpleSelectorContext>();
}

SimpleCSSParser::SimpleSelectorContext* SimpleCSSParser::SelectorContext::simpleSelector(size_t i) {
  return getRuleContext<SimpleCSSParser::SimpleSelectorContext>(i);
}


size_t SimpleCSSParser::SelectorContext::getRuleIndex() const {
  return SimpleCSSParser::RuleSelector;
}


std::any SimpleCSSParser::SelectorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitSelector(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::SelectorContext* SimpleCSSParser::selector() {
  SelectorContext *_localctx = _tracker.createInstance<SelectorContext>(_ctx, getState());
  enterRule(_localctx, 6, SimpleCSSParser::RuleSelector);
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
    setState(74); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(73);
      simpleSelector();
      setState(76); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 274877907952) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SimpleSelectorContext ------------------------------------------------------------------

SimpleCSSParser::SimpleSelectorContext::SimpleSelectorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SimpleCSSParser::ElementNameContext* SimpleCSSParser::SimpleSelectorContext::elementName() {
  return getRuleContext<SimpleCSSParser::ElementNameContext>(0);
}

SimpleCSSParser::ClassNameContext* SimpleCSSParser::SimpleSelectorContext::className() {
  return getRuleContext<SimpleCSSParser::ClassNameContext>(0);
}

SimpleCSSParser::IdNameContext* SimpleCSSParser::SimpleSelectorContext::idName() {
  return getRuleContext<SimpleCSSParser::IdNameContext>(0);
}

SimpleCSSParser::PseudoClassContext* SimpleCSSParser::SimpleSelectorContext::pseudoClass() {
  return getRuleContext<SimpleCSSParser::PseudoClassContext>(0);
}

SimpleCSSParser::PseudoElementContext* SimpleCSSParser::SimpleSelectorContext::pseudoElement() {
  return getRuleContext<SimpleCSSParser::PseudoElementContext>(0);
}

SimpleCSSParser::AttributeContext* SimpleCSSParser::SimpleSelectorContext::attribute() {
  return getRuleContext<SimpleCSSParser::AttributeContext>(0);
}


size_t SimpleCSSParser::SimpleSelectorContext::getRuleIndex() const {
  return SimpleCSSParser::RuleSimpleSelector;
}


std::any SimpleCSSParser::SimpleSelectorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitSimpleSelector(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::SimpleSelectorContext* SimpleCSSParser::simpleSelector() {
  SimpleSelectorContext *_localctx = _tracker.createInstance<SimpleSelectorContext>(_ctx, getState());
  enterRule(_localctx, 8, SimpleCSSParser::RuleSimpleSelector);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(92);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SimpleCSSParser::IDENT: {
        enterOuterAlt(_localctx, 1);
        setState(78);
        elementName();
        break;
      }

      case SimpleCSSParser::T__3: {
        enterOuterAlt(_localctx, 2);
        setState(79);
        match(SimpleCSSParser::T__3);
        setState(80);
        className();
        break;
      }

      case SimpleCSSParser::T__4: {
        enterOuterAlt(_localctx, 3);
        setState(81);
        match(SimpleCSSParser::T__4);
        setState(82);
        idName();
        break;
      }

      case SimpleCSSParser::T__5: {
        enterOuterAlt(_localctx, 4);
        setState(83);
        match(SimpleCSSParser::T__5);
        break;
      }

      case SimpleCSSParser::T__6: {
        enterOuterAlt(_localctx, 5);
        setState(84);
        match(SimpleCSSParser::T__6);
        setState(85);
        pseudoClass();
        break;
      }

      case SimpleCSSParser::T__7: {
        enterOuterAlt(_localctx, 6);
        setState(86);
        match(SimpleCSSParser::T__7);
        setState(87);
        pseudoElement();
        break;
      }

      case SimpleCSSParser::T__8: {
        enterOuterAlt(_localctx, 7);
        setState(88);
        match(SimpleCSSParser::T__8);
        setState(89);
        attribute();
        setState(90);
        match(SimpleCSSParser::T__9);
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

//----------------- ElementNameContext ------------------------------------------------------------------

SimpleCSSParser::ElementNameContext::ElementNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SimpleCSSParser::ElementNameContext::IDENT() {
  return getToken(SimpleCSSParser::IDENT, 0);
}


size_t SimpleCSSParser::ElementNameContext::getRuleIndex() const {
  return SimpleCSSParser::RuleElementName;
}


std::any SimpleCSSParser::ElementNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitElementName(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::ElementNameContext* SimpleCSSParser::elementName() {
  ElementNameContext *_localctx = _tracker.createInstance<ElementNameContext>(_ctx, getState());
  enterRule(_localctx, 10, SimpleCSSParser::RuleElementName);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(94);
    match(SimpleCSSParser::IDENT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassNameContext ------------------------------------------------------------------

SimpleCSSParser::ClassNameContext::ClassNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SimpleCSSParser::ClassNameContext::IDENT() {
  return getToken(SimpleCSSParser::IDENT, 0);
}


size_t SimpleCSSParser::ClassNameContext::getRuleIndex() const {
  return SimpleCSSParser::RuleClassName;
}


std::any SimpleCSSParser::ClassNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitClassName(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::ClassNameContext* SimpleCSSParser::className() {
  ClassNameContext *_localctx = _tracker.createInstance<ClassNameContext>(_ctx, getState());
  enterRule(_localctx, 12, SimpleCSSParser::RuleClassName);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(96);
    match(SimpleCSSParser::IDENT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IdNameContext ------------------------------------------------------------------

SimpleCSSParser::IdNameContext::IdNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SimpleCSSParser::IdNameContext::IDENT() {
  return getToken(SimpleCSSParser::IDENT, 0);
}


size_t SimpleCSSParser::IdNameContext::getRuleIndex() const {
  return SimpleCSSParser::RuleIdName;
}


std::any SimpleCSSParser::IdNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitIdName(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::IdNameContext* SimpleCSSParser::idName() {
  IdNameContext *_localctx = _tracker.createInstance<IdNameContext>(_ctx, getState());
  enterRule(_localctx, 14, SimpleCSSParser::RuleIdName);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(98);
    match(SimpleCSSParser::IDENT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PseudoClassContext ------------------------------------------------------------------

SimpleCSSParser::PseudoClassContext::PseudoClassContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SimpleCSSParser::PseudoClassContext::IDENT() {
  return getToken(SimpleCSSParser::IDENT, 0);
}

SimpleCSSParser::ExpressionContext* SimpleCSSParser::PseudoClassContext::expression() {
  return getRuleContext<SimpleCSSParser::ExpressionContext>(0);
}


size_t SimpleCSSParser::PseudoClassContext::getRuleIndex() const {
  return SimpleCSSParser::RulePseudoClass;
}


std::any SimpleCSSParser::PseudoClassContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitPseudoClass(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::PseudoClassContext* SimpleCSSParser::pseudoClass() {
  PseudoClassContext *_localctx = _tracker.createInstance<PseudoClassContext>(_ctx, getState());
  enterRule(_localctx, 16, SimpleCSSParser::RulePseudoClass);
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
    setState(100);
    match(SimpleCSSParser::IDENT);
    setState(105);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SimpleCSSParser::T__10) {
      setState(101);
      match(SimpleCSSParser::T__10);
      setState(102);
      expression();
      setState(103);
      match(SimpleCSSParser::T__11);
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

SimpleCSSParser::PseudoElementContext::PseudoElementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SimpleCSSParser::PseudoElementContext::IDENT() {
  return getToken(SimpleCSSParser::IDENT, 0);
}


size_t SimpleCSSParser::PseudoElementContext::getRuleIndex() const {
  return SimpleCSSParser::RulePseudoElement;
}


std::any SimpleCSSParser::PseudoElementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitPseudoElement(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::PseudoElementContext* SimpleCSSParser::pseudoElement() {
  PseudoElementContext *_localctx = _tracker.createInstance<PseudoElementContext>(_ctx, getState());
  enterRule(_localctx, 18, SimpleCSSParser::RulePseudoElement);

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
    match(SimpleCSSParser::IDENT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AttributeContext ------------------------------------------------------------------

SimpleCSSParser::AttributeContext::AttributeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> SimpleCSSParser::AttributeContext::IDENT() {
  return getTokens(SimpleCSSParser::IDENT);
}

tree::TerminalNode* SimpleCSSParser::AttributeContext::IDENT(size_t i) {
  return getToken(SimpleCSSParser::IDENT, i);
}

SimpleCSSParser::AttributeOperatorContext* SimpleCSSParser::AttributeContext::attributeOperator() {
  return getRuleContext<SimpleCSSParser::AttributeOperatorContext>(0);
}

tree::TerminalNode* SimpleCSSParser::AttributeContext::STRING() {
  return getToken(SimpleCSSParser::STRING, 0);
}


size_t SimpleCSSParser::AttributeContext::getRuleIndex() const {
  return SimpleCSSParser::RuleAttribute;
}


std::any SimpleCSSParser::AttributeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitAttribute(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::AttributeContext* SimpleCSSParser::attribute() {
  AttributeContext *_localctx = _tracker.createInstance<AttributeContext>(_ctx, getState());
  enterRule(_localctx, 20, SimpleCSSParser::RuleAttribute);
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
    setState(109);
    match(SimpleCSSParser::IDENT);
    setState(113);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 516096) != 0)) {
      setState(110);
      attributeOperator();
      setState(111);
      _la = _input->LA(1);
      if (!(_la == SimpleCSSParser::STRING

      || _la == SimpleCSSParser::IDENT)) {
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

//----------------- AttributeOperatorContext ------------------------------------------------------------------

SimpleCSSParser::AttributeOperatorContext::AttributeOperatorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SimpleCSSParser::AttributeOperatorContext::getRuleIndex() const {
  return SimpleCSSParser::RuleAttributeOperator;
}


std::any SimpleCSSParser::AttributeOperatorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitAttributeOperator(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::AttributeOperatorContext* SimpleCSSParser::attributeOperator() {
  AttributeOperatorContext *_localctx = _tracker.createInstance<AttributeOperatorContext>(_ctx, getState());
  enterRule(_localctx, 22, SimpleCSSParser::RuleAttributeOperator);
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
    setState(115);
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

//----------------- DeclarationContext ------------------------------------------------------------------

SimpleCSSParser::DeclarationContext::DeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SimpleCSSParser::PropertyContext* SimpleCSSParser::DeclarationContext::property() {
  return getRuleContext<SimpleCSSParser::PropertyContext>(0);
}

std::vector<SimpleCSSParser::ValueContext *> SimpleCSSParser::DeclarationContext::value() {
  return getRuleContexts<SimpleCSSParser::ValueContext>();
}

SimpleCSSParser::ValueContext* SimpleCSSParser::DeclarationContext::value(size_t i) {
  return getRuleContext<SimpleCSSParser::ValueContext>(i);
}


size_t SimpleCSSParser::DeclarationContext::getRuleIndex() const {
  return SimpleCSSParser::RuleDeclaration;
}


std::any SimpleCSSParser::DeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitDeclaration(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::DeclarationContext* SimpleCSSParser::declaration() {
  DeclarationContext *_localctx = _tracker.createInstance<DeclarationContext>(_ctx, getState());
  enterRule(_localctx, 24, SimpleCSSParser::RuleDeclaration);
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
    setState(117);
    property();
    setState(118);
    match(SimpleCSSParser::T__6);
    setState(120); 
    _errHandler->sync(this);
    alt = 1;
    do {
      switch (alt) {
        case 1: {
              setState(119);
              value(0);
              break;
            }

      default:
        throw NoViableAltException(this);
      }
      setState(122); 
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx);
    } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
    setState(125);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SimpleCSSParser::T__18) {
      setState(124);
      match(SimpleCSSParser::T__18);
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

SimpleCSSParser::PropertyContext::PropertyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SimpleCSSParser::PropertyContext::IDENT() {
  return getToken(SimpleCSSParser::IDENT, 0);
}


size_t SimpleCSSParser::PropertyContext::getRuleIndex() const {
  return SimpleCSSParser::RuleProperty;
}


std::any SimpleCSSParser::PropertyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitProperty(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::PropertyContext* SimpleCSSParser::property() {
  PropertyContext *_localctx = _tracker.createInstance<PropertyContext>(_ctx, getState());
  enterRule(_localctx, 26, SimpleCSSParser::RuleProperty);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(127);
    match(SimpleCSSParser::IDENT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueContext ------------------------------------------------------------------

SimpleCSSParser::ValueContext::ValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SimpleCSSParser::TermContext* SimpleCSSParser::ValueContext::term() {
  return getRuleContext<SimpleCSSParser::TermContext>(0);
}

std::vector<SimpleCSSParser::ValueContext *> SimpleCSSParser::ValueContext::value() {
  return getRuleContexts<SimpleCSSParser::ValueContext>();
}

SimpleCSSParser::ValueContext* SimpleCSSParser::ValueContext::value(size_t i) {
  return getRuleContext<SimpleCSSParser::ValueContext>(i);
}


size_t SimpleCSSParser::ValueContext::getRuleIndex() const {
  return SimpleCSSParser::RuleValue;
}


std::any SimpleCSSParser::ValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitValue(this);
  else
    return visitor->visitChildren(this);
}


SimpleCSSParser::ValueContext* SimpleCSSParser::value() {
   return value(0);
}

SimpleCSSParser::ValueContext* SimpleCSSParser::value(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SimpleCSSParser::ValueContext *_localctx = _tracker.createInstance<ValueContext>(_ctx, parentState);
  SimpleCSSParser::ValueContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 28;
  enterRecursionRule(_localctx, 28, SimpleCSSParser::RuleValue, precedence);

    

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
    setState(130);
    term();
    _ctx->stop = _input->LT(-1);
    setState(137);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<ValueContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleValue);
        setState(132);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(133);
        match(SimpleCSSParser::T__0);
        setState(134);
        value(2); 
      }
      setState(139);
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

//----------------- TermContext ------------------------------------------------------------------

SimpleCSSParser::TermContext::TermContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SimpleCSSParser::TermContext::NUMBER() {
  return getToken(SimpleCSSParser::NUMBER, 0);
}

SimpleCSSParser::UnitContext* SimpleCSSParser::TermContext::unit() {
  return getRuleContext<SimpleCSSParser::UnitContext>(0);
}

tree::TerminalNode* SimpleCSSParser::TermContext::PERCENTAGE() {
  return getToken(SimpleCSSParser::PERCENTAGE, 0);
}

tree::TerminalNode* SimpleCSSParser::TermContext::COLOR() {
  return getToken(SimpleCSSParser::COLOR, 0);
}

tree::TerminalNode* SimpleCSSParser::TermContext::STRING() {
  return getToken(SimpleCSSParser::STRING, 0);
}

tree::TerminalNode* SimpleCSSParser::TermContext::IDENT() {
  return getToken(SimpleCSSParser::IDENT, 0);
}

tree::TerminalNode* SimpleCSSParser::TermContext::URI() {
  return getToken(SimpleCSSParser::URI, 0);
}

SimpleCSSParser::FunctionContext* SimpleCSSParser::TermContext::function() {
  return getRuleContext<SimpleCSSParser::FunctionContext>(0);
}


size_t SimpleCSSParser::TermContext::getRuleIndex() const {
  return SimpleCSSParser::RuleTerm;
}


std::any SimpleCSSParser::TermContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitTerm(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::TermContext* SimpleCSSParser::term() {
  TermContext *_localctx = _tracker.createInstance<TermContext>(_ctx, getState());
  enterRule(_localctx, 30, SimpleCSSParser::RuleTerm);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(150);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(140);
      match(SimpleCSSParser::NUMBER);
      setState(142);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx)) {
      case 1: {
        setState(141);
        unit();
        break;
      }

      default:
        break;
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(144);
      match(SimpleCSSParser::PERCENTAGE);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(145);
      match(SimpleCSSParser::COLOR);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(146);
      match(SimpleCSSParser::STRING);
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(147);
      match(SimpleCSSParser::IDENT);
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(148);
      match(SimpleCSSParser::URI);
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(149);
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

//----------------- UnitContext ------------------------------------------------------------------

SimpleCSSParser::UnitContext::UnitContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SimpleCSSParser::UnitContext::getRuleIndex() const {
  return SimpleCSSParser::RuleUnit;
}


std::any SimpleCSSParser::UnitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitUnit(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::UnitContext* SimpleCSSParser::unit() {
  UnitContext *_localctx = _tracker.createInstance<UnitContext>(_ctx, getState());
  enterRule(_localctx, 32, SimpleCSSParser::RuleUnit);
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
    setState(152);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 1072693248) != 0))) {
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

//----------------- FunctionContext ------------------------------------------------------------------

SimpleCSSParser::FunctionContext::FunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SimpleCSSParser::FunctionContext::IDENT() {
  return getToken(SimpleCSSParser::IDENT, 0);
}

SimpleCSSParser::ExpressionContext* SimpleCSSParser::FunctionContext::expression() {
  return getRuleContext<SimpleCSSParser::ExpressionContext>(0);
}


size_t SimpleCSSParser::FunctionContext::getRuleIndex() const {
  return SimpleCSSParser::RuleFunction;
}


std::any SimpleCSSParser::FunctionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitFunction(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::FunctionContext* SimpleCSSParser::function() {
  FunctionContext *_localctx = _tracker.createInstance<FunctionContext>(_ctx, getState());
  enterRule(_localctx, 34, SimpleCSSParser::RuleFunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(154);
    match(SimpleCSSParser::IDENT);
    setState(155);
    match(SimpleCSSParser::T__10);
    setState(156);
    expression();
    setState(157);
    match(SimpleCSSParser::T__11);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

SimpleCSSParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SimpleCSSParser::TermContext *> SimpleCSSParser::ExpressionContext::term() {
  return getRuleContexts<SimpleCSSParser::TermContext>();
}

SimpleCSSParser::TermContext* SimpleCSSParser::ExpressionContext::term(size_t i) {
  return getRuleContext<SimpleCSSParser::TermContext>(i);
}


size_t SimpleCSSParser::ExpressionContext::getRuleIndex() const {
  return SimpleCSSParser::RuleExpression;
}


std::any SimpleCSSParser::ExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitExpression(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::ExpressionContext* SimpleCSSParser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 36, SimpleCSSParser::RuleExpression);
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
    setState(160); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(159);
      term();
      setState(162); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 541165879296) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AtRuleContext ------------------------------------------------------------------

SimpleCSSParser::AtRuleContext::AtRuleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SimpleCSSParser::AtRuleContext::IDENT() {
  return getToken(SimpleCSSParser::IDENT, 0);
}

SimpleCSSParser::BlockContext* SimpleCSSParser::AtRuleContext::block() {
  return getRuleContext<SimpleCSSParser::BlockContext>(0);
}

std::vector<SimpleCSSParser::AnyContext *> SimpleCSSParser::AtRuleContext::any() {
  return getRuleContexts<SimpleCSSParser::AnyContext>();
}

SimpleCSSParser::AnyContext* SimpleCSSParser::AtRuleContext::any(size_t i) {
  return getRuleContext<SimpleCSSParser::AnyContext>(i);
}


size_t SimpleCSSParser::AtRuleContext::getRuleIndex() const {
  return SimpleCSSParser::RuleAtRule;
}


std::any SimpleCSSParser::AtRuleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitAtRule(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::AtRuleContext* SimpleCSSParser::atRule() {
  AtRuleContext *_localctx = _tracker.createInstance<AtRuleContext>(_ctx, getState());
  enterRule(_localctx, 38, SimpleCSSParser::RuleAtRule);

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
    setState(164);
    match(SimpleCSSParser::T__29);
    setState(165);
    match(SimpleCSSParser::IDENT);
    setState(169);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(166);
        any(); 
      }
      setState(171);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx);
    }
    setState(174);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SimpleCSSParser::T__1: {
        setState(172);
        block();
        break;
      }

      case SimpleCSSParser::T__18: {
        setState(173);
        match(SimpleCSSParser::T__18);
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

SimpleCSSParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SimpleCSSParser::AnyContext *> SimpleCSSParser::BlockContext::any() {
  return getRuleContexts<SimpleCSSParser::AnyContext>();
}

SimpleCSSParser::AnyContext* SimpleCSSParser::BlockContext::any(size_t i) {
  return getRuleContext<SimpleCSSParser::AnyContext>(i);
}


size_t SimpleCSSParser::BlockContext::getRuleIndex() const {
  return SimpleCSSParser::RuleBlock;
}


std::any SimpleCSSParser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::BlockContext* SimpleCSSParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 40, SimpleCSSParser::RuleBlock);
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
    setState(176);
    match(SimpleCSSParser::T__1);
    setState(180);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 369367711878) != 0)) {
      setState(177);
      any();
      setState(182);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(183);
    match(SimpleCSSParser::T__2);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AnyContext ------------------------------------------------------------------

SimpleCSSParser::AnyContext::AnyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SimpleCSSParser::AnyContext::IDENT() {
  return getToken(SimpleCSSParser::IDENT, 0);
}

tree::TerminalNode* SimpleCSSParser::AnyContext::NUMBER() {
  return getToken(SimpleCSSParser::NUMBER, 0);
}

tree::TerminalNode* SimpleCSSParser::AnyContext::STRING() {
  return getToken(SimpleCSSParser::STRING, 0);
}

tree::TerminalNode* SimpleCSSParser::AnyContext::COLOR() {
  return getToken(SimpleCSSParser::COLOR, 0);
}

SimpleCSSParser::BlockContext* SimpleCSSParser::AnyContext::block() {
  return getRuleContext<SimpleCSSParser::BlockContext>(0);
}


size_t SimpleCSSParser::AnyContext::getRuleIndex() const {
  return SimpleCSSParser::RuleAny;
}


std::any SimpleCSSParser::AnyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SimpleCSSVisitor*>(visitor))
    return parserVisitor->visitAny(this);
  else
    return visitor->visitChildren(this);
}

SimpleCSSParser::AnyContext* SimpleCSSParser::any() {
  AnyContext *_localctx = _tracker.createInstance<AnyContext>(_ctx, getState());
  enterRule(_localctx, 42, SimpleCSSParser::RuleAny);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(193);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SimpleCSSParser::IDENT: {
        enterOuterAlt(_localctx, 1);
        setState(185);
        match(SimpleCSSParser::IDENT);
        break;
      }

      case SimpleCSSParser::NUMBER: {
        enterOuterAlt(_localctx, 2);
        setState(186);
        match(SimpleCSSParser::NUMBER);
        break;
      }

      case SimpleCSSParser::STRING: {
        enterOuterAlt(_localctx, 3);
        setState(187);
        match(SimpleCSSParser::STRING);
        break;
      }

      case SimpleCSSParser::COLOR: {
        enterOuterAlt(_localctx, 4);
        setState(188);
        match(SimpleCSSParser::COLOR);
        break;
      }

      case SimpleCSSParser::T__0: {
        enterOuterAlt(_localctx, 5);
        setState(189);
        match(SimpleCSSParser::T__0);
        break;
      }

      case SimpleCSSParser::T__6: {
        enterOuterAlt(_localctx, 6);
        setState(190);
        match(SimpleCSSParser::T__6);
        break;
      }

      case SimpleCSSParser::T__18: {
        enterOuterAlt(_localctx, 7);
        setState(191);
        match(SimpleCSSParser::T__18);
        break;
      }

      case SimpleCSSParser::T__1: {
        enterOuterAlt(_localctx, 8);
        setState(192);
        block();
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

bool SimpleCSSParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 14: return valueSempred(antlrcpp::downCast<ValueContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool SimpleCSSParser::valueSempred(ValueContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

void SimpleCSSParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  simplecssParserInitialize();
#else
  ::antlr4::internal::call_once(simplecssParserOnceFlag, simplecssParserInitialize);
#endif
}


// Generated from CSS3.g4 by ANTLR 4.9.2


#include "CSS3Listener.h"

#include "CSS3Parser.h"


using namespace antlrcpp;
using namespace antlr4;

CSS3Parser::CSS3Parser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

CSS3Parser::~CSS3Parser() {
  delete _interpreter;
}

std::string CSS3Parser::getGrammarFileName() const {
  return "CSS3.g4";
}

const std::vector<std::string>& CSS3Parser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& CSS3Parser::getVocabulary() const {
  return _vocabulary;
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
      ((1ULL << _la) & ((1ULL << CSS3Parser::T__1)
      | (1ULL << CSS3Parser::COMMENT)
      | (1ULL << CSS3Parser::LBRACKET)
      | (1ULL << CSS3Parser::COLON)
      | (1ULL << CSS3Parser::DOT)
      | (1ULL << CSS3Parser::AT_RULE_NAME)
      | (1ULL << CSS3Parser::HASH)
      | (1ULL << CSS3Parser::IDENT))) != 0)) {
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
      ((1ULL << _la) & ((1ULL << CSS3Parser::WS)
      | (1ULL << CSS3Parser::PLUS)
      | (1ULL << CSS3Parser::GREATER)
      | (1ULL << CSS3Parser::TILDE))) != 0)) {
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
      ((1ULL << _la) & ((1ULL << CSS3Parser::WS)
      | (1ULL << CSS3Parser::PLUS)
      | (1ULL << CSS3Parser::GREATER)
      | (1ULL << CSS3Parser::TILDE))) != 0))) {
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
        ((1ULL << _la) & ((1ULL << CSS3Parser::T__0)
        | (1ULL << CSS3Parser::INCLUDES)
        | (1ULL << CSS3Parser::DASHMATCH)
        | (1ULL << CSS3Parser::PREFIXMATCH)
        | (1ULL << CSS3Parser::SUFFIXMATCH)
        | (1ULL << CSS3Parser::SUBSTRINGMATCH))) != 0)) {
        setState(107);
        _la = _input->LA(1);
        if (!((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << CSS3Parser::T__0)
          | (1ULL << CSS3Parser::INCLUDES)
          | (1ULL << CSS3Parser::DASHMATCH)
          | (1ULL << CSS3Parser::PREFIXMATCH)
          | (1ULL << CSS3Parser::SUFFIXMATCH)
          | (1ULL << CSS3Parser::SUBSTRINGMATCH))) != 0))) {
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
          ((1ULL << _la) & ((1ULL << CSS3Parser::NUMBER)
          | (1ULL << CSS3Parser::STRING)
          | (1ULL << CSS3Parser::IDENT))) != 0))) {
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
          ((1ULL << _la) & ((1ULL << CSS3Parser::PERCENTAGE)
          | (1ULL << CSS3Parser::LENGTH)
          | (1ULL << CSS3Parser::EMS)
          | (1ULL << CSS3Parser::EXS)
          | (1ULL << CSS3Parser::ANGLE)
          | (1ULL << CSS3Parser::TIME)
          | (1ULL << CSS3Parser::FREQ)
          | (1ULL << CSS3Parser::IDENT))) != 0))) {
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

// Static vars and initialization.
std::vector<dfa::DFA> CSS3Parser::_decisionToDFA;
atn::PredictionContextCache CSS3Parser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN CSS3Parser::_atn;
std::vector<uint16_t> CSS3Parser::_serializedATN;

std::vector<std::string> CSS3Parser::_ruleNames = {
  "stylesheet", "statement", "atRule", "block", "ruleset", "selector", "combinator", 
  "simpleSelector", "elementName", "declaration", "prio", "expr", "term", 
  "function", "unicodeRange", "hexcolor", "cssOperator", "unaryOperator", 
  "property", "any", "comment"
};

std::vector<std::string> CSS3Parser::_literalNames = {
  "", "'='", "'*'", "'U+'", "'\u003F'", "'/'", "'-'", "", "", "'{'", "'}'", 
  "'('", "')'", "'['", "']'", "';'", "':'", "'.'", "','", "'+'", "'>'", 
  "'~'", "'~='", "'|='", "'^='", "'$='", "'*='", "'!important'"
};

std::vector<std::string> CSS3Parser::_symbolicNames = {
  "", "", "", "", "", "", "", "COMMENT", "WS", "LBRACE", "RBRACE", "LPAREN", 
  "RPAREN", "LBRACKET", "RBRACKET", "SEMICOLON", "COLON", "DOT", "COMMA", 
  "PLUS", "GREATER", "TILDE", "INCLUDES", "DASHMATCH", "PREFIXMATCH", "SUFFIXMATCH", 
  "SUBSTRINGMATCH", "IMPORTANT_SYM", "AT_RULE_NAME", "FUNCTION", "URI", 
  "HASH", "NUMBER", "PERCENTAGE", "LENGTH", "EMS", "EXS", "ANGLE", "TIME", 
  "FREQ", "STRING", "IDENT", "HEX_DIGIT"
};

dfa::Vocabulary CSS3Parser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> CSS3Parser::_tokenNames;

CSS3Parser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  static const uint16_t serializedATNSegment0[] = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
       0x3, 0x2c, 0xe2, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
       0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 
       0x4, 0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 
       0xe, 0x9, 0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 
       0x9, 0x11, 0x4, 0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 
       0x9, 0x14, 0x4, 0x15, 0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x3, 0x2, 
       0x7, 0x2, 0x2e, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x31, 0xb, 0x2, 0x3, 
       0x3, 0x3, 0x3, 0x3, 0x3, 0x5, 0x3, 0x36, 0xa, 0x3, 0x3, 0x4, 0x3, 
       0x4, 0x7, 0x4, 0x3a, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x3d, 0xb, 0x4, 
       0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x41, 0xa, 0x4, 0x3, 0x5, 0x3, 0x5, 
       0x7, 0x5, 0x45, 0xa, 0x5, 0xc, 0x5, 0xe, 0x5, 0x48, 0xb, 0x5, 0x3, 
       0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x7, 0x6, 0x4f, 0xa, 
       0x6, 0xc, 0x6, 0xe, 0x6, 0x52, 0xb, 0x6, 0x3, 0x6, 0x3, 0x6, 0x7, 
       0x6, 0x56, 0xa, 0x6, 0xc, 0x6, 0xe, 0x6, 0x59, 0xb, 0x6, 0x3, 0x6, 
       0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x7, 0x7, 0x61, 
       0xa, 0x7, 0xc, 0x7, 0xe, 0x7, 0x64, 0xb, 0x7, 0x3, 0x8, 0x3, 0x8, 
       0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 
       0x9, 0x3, 0x9, 0x5, 0x9, 0x70, 0xa, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 
       0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x5, 0x9, 0x78, 0xa, 0x9, 0x3, 
       0x9, 0x3, 0x9, 0x3, 0x9, 0x5, 0x9, 0x7d, 0xa, 0x9, 0x3, 0xa, 0x3, 
       0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0x85, 0xa, 
       0xb, 0x3, 0xb, 0x5, 0xb, 0x88, 0xa, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 
       0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x7, 0xd, 0x90, 0xa, 0xd, 0xc, 
       0xd, 0xe, 0xd, 0x93, 0xb, 0xd, 0x3, 0xe, 0x5, 0xe, 0x96, 0xa, 0xe, 
       0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 0x9a, 0xa, 0xe, 0x3, 0xe, 0x3, 0xe, 
       0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 0xa3, 
       0xa, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 0x3, 
       0x10, 0x3, 0x10, 0x7, 0x10, 0xac, 0xa, 0x10, 0xc, 0x10, 0xe, 0x10, 
       0xaf, 0xb, 0x10, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 
       0x11, 0x3, 0x11, 0x3, 0x11, 0x5, 0x11, 0xb8, 0xa, 0x11, 0x3, 0x12, 
       0x3, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x14, 0x3, 0x14, 0x3, 0x15, 
       0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
       0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
       0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
       0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
       0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 
       0xde, 0xa, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x2, 0x2, 0x17, 
       0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 
       0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2, 0xb, 0x4, 
       0x2, 0xa, 0xa, 0x15, 0x17, 0x4, 0x2, 0x3, 0x3, 0x18, 0x1c, 0x3, 0x2, 
       0x2a, 0x2b, 0x4, 0x2, 0x22, 0x22, 0x2a, 0x2b, 0x4, 0x2, 0x4, 0x4, 
       0x2b, 0x2b, 0x4, 0x2, 0x23, 0x29, 0x2b, 0x2b, 0x4, 0x2, 0x6, 0x6, 
       0x2c, 0x2c, 0x4, 0x2, 0x7, 0x7, 0x14, 0x14, 0x4, 0x2, 0x8, 0x8, 0x15, 
       0x15, 0x2, 0x107, 0x2, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x4, 0x35, 0x3, 
       0x2, 0x2, 0x2, 0x6, 0x37, 0x3, 0x2, 0x2, 0x2, 0x8, 0x42, 0x3, 0x2, 
       0x2, 0x2, 0xa, 0x4b, 0x3, 0x2, 0x2, 0x2, 0xc, 0x5c, 0x3, 0x2, 0x2, 
       0x2, 0xe, 0x65, 0x3, 0x2, 0x2, 0x2, 0x10, 0x7c, 0x3, 0x2, 0x2, 0x2, 
       0x12, 0x7e, 0x3, 0x2, 0x2, 0x2, 0x14, 0x80, 0x3, 0x2, 0x2, 0x2, 0x16, 
       0x89, 0x3, 0x2, 0x2, 0x2, 0x18, 0x8b, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x95, 
       0x3, 0x2, 0x2, 0x2, 0x1c, 0xa4, 0x3, 0x2, 0x2, 0x2, 0x1e, 0xa8, 0x3, 
       0x2, 0x2, 0x2, 0x20, 0xb0, 0x3, 0x2, 0x2, 0x2, 0x22, 0xb9, 0x3, 0x2, 
       0x2, 0x2, 0x24, 0xbb, 0x3, 0x2, 0x2, 0x2, 0x26, 0xbd, 0x3, 0x2, 0x2, 
       0x2, 0x28, 0xdd, 0x3, 0x2, 0x2, 0x2, 0x2a, 0xdf, 0x3, 0x2, 0x2, 0x2, 
       0x2c, 0x2e, 0x5, 0x4, 0x3, 0x2, 0x2d, 0x2c, 0x3, 0x2, 0x2, 0x2, 0x2e, 
       0x31, 0x3, 0x2, 0x2, 0x2, 0x2f, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x2f, 0x30, 
       0x3, 0x2, 0x2, 0x2, 0x30, 0x3, 0x3, 0x2, 0x2, 0x2, 0x31, 0x2f, 0x3, 
       0x2, 0x2, 0x2, 0x32, 0x36, 0x5, 0xa, 0x6, 0x2, 0x33, 0x36, 0x5, 0x6, 
       0x4, 0x2, 0x34, 0x36, 0x5, 0x2a, 0x16, 0x2, 0x35, 0x32, 0x3, 0x2, 
       0x2, 0x2, 0x35, 0x33, 0x3, 0x2, 0x2, 0x2, 0x35, 0x34, 0x3, 0x2, 0x2, 
       0x2, 0x36, 0x5, 0x3, 0x2, 0x2, 0x2, 0x37, 0x3b, 0x7, 0x1e, 0x2, 0x2, 
       0x38, 0x3a, 0x5, 0x28, 0x15, 0x2, 0x39, 0x38, 0x3, 0x2, 0x2, 0x2, 
       0x3a, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x3b, 0x39, 0x3, 0x2, 0x2, 0x2, 0x3b, 
       0x3c, 0x3, 0x2, 0x2, 0x2, 0x3c, 0x40, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x3b, 
       0x3, 0x2, 0x2, 0x2, 0x3e, 0x41, 0x5, 0x8, 0x5, 0x2, 0x3f, 0x41, 0x7, 
       0x11, 0x2, 0x2, 0x40, 0x3e, 0x3, 0x2, 0x2, 0x2, 0x40, 0x3f, 0x3, 
       0x2, 0x2, 0x2, 0x41, 0x7, 0x3, 0x2, 0x2, 0x2, 0x42, 0x46, 0x7, 0xb, 
       0x2, 0x2, 0x43, 0x45, 0x5, 0x14, 0xb, 0x2, 0x44, 0x43, 0x3, 0x2, 
       0x2, 0x2, 0x45, 0x48, 0x3, 0x2, 0x2, 0x2, 0x46, 0x44, 0x3, 0x2, 0x2, 
       0x2, 0x46, 0x47, 0x3, 0x2, 0x2, 0x2, 0x47, 0x49, 0x3, 0x2, 0x2, 0x2, 
       0x48, 0x46, 0x3, 0x2, 0x2, 0x2, 0x49, 0x4a, 0x7, 0xc, 0x2, 0x2, 0x4a, 
       0x9, 0x3, 0x2, 0x2, 0x2, 0x4b, 0x50, 0x5, 0xc, 0x7, 0x2, 0x4c, 0x4d, 
       0x7, 0x14, 0x2, 0x2, 0x4d, 0x4f, 0x5, 0xc, 0x7, 0x2, 0x4e, 0x4c, 
       0x3, 0x2, 0x2, 0x2, 0x4f, 0x52, 0x3, 0x2, 0x2, 0x2, 0x50, 0x4e, 0x3, 
       0x2, 0x2, 0x2, 0x50, 0x51, 0x3, 0x2, 0x2, 0x2, 0x51, 0x53, 0x3, 0x2, 
       0x2, 0x2, 0x52, 0x50, 0x3, 0x2, 0x2, 0x2, 0x53, 0x57, 0x7, 0xb, 0x2, 
       0x2, 0x54, 0x56, 0x5, 0x14, 0xb, 0x2, 0x55, 0x54, 0x3, 0x2, 0x2, 
       0x2, 0x56, 0x59, 0x3, 0x2, 0x2, 0x2, 0x57, 0x55, 0x3, 0x2, 0x2, 0x2, 
       0x57, 0x58, 0x3, 0x2, 0x2, 0x2, 0x58, 0x5a, 0x3, 0x2, 0x2, 0x2, 0x59, 
       0x57, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x5b, 0x7, 0xc, 0x2, 0x2, 0x5b, 0xb, 
       0x3, 0x2, 0x2, 0x2, 0x5c, 0x62, 0x5, 0x10, 0x9, 0x2, 0x5d, 0x5e, 
       0x5, 0xe, 0x8, 0x2, 0x5e, 0x5f, 0x5, 0x10, 0x9, 0x2, 0x5f, 0x61, 
       0x3, 0x2, 0x2, 0x2, 0x60, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x61, 0x64, 0x3, 
       0x2, 0x2, 0x2, 0x62, 0x60, 0x3, 0x2, 0x2, 0x2, 0x62, 0x63, 0x3, 0x2, 
       0x2, 0x2, 0x63, 0xd, 0x3, 0x2, 0x2, 0x2, 0x64, 0x62, 0x3, 0x2, 0x2, 
       0x2, 0x65, 0x66, 0x9, 0x2, 0x2, 0x2, 0x66, 0xf, 0x3, 0x2, 0x2, 0x2, 
       0x67, 0x7d, 0x5, 0x12, 0xa, 0x2, 0x68, 0x7d, 0x7, 0x21, 0x2, 0x2, 
       0x69, 0x6a, 0x7, 0x13, 0x2, 0x2, 0x6a, 0x7d, 0x7, 0x2b, 0x2, 0x2, 
       0x6b, 0x6c, 0x7, 0xf, 0x2, 0x2, 0x6c, 0x6f, 0x7, 0x2b, 0x2, 0x2, 
       0x6d, 0x6e, 0x9, 0x3, 0x2, 0x2, 0x6e, 0x70, 0x9, 0x4, 0x2, 0x2, 0x6f, 
       0x6d, 0x3, 0x2, 0x2, 0x2, 0x6f, 0x70, 0x3, 0x2, 0x2, 0x2, 0x70, 0x71, 
       0x3, 0x2, 0x2, 0x2, 0x71, 0x7d, 0x7, 0x10, 0x2, 0x2, 0x72, 0x73, 
       0x7, 0x12, 0x2, 0x2, 0x73, 0x77, 0x7, 0x2b, 0x2, 0x2, 0x74, 0x75, 
       0x7, 0xd, 0x2, 0x2, 0x75, 0x76, 0x9, 0x5, 0x2, 0x2, 0x76, 0x78, 0x7, 
       0xe, 0x2, 0x2, 0x77, 0x74, 0x3, 0x2, 0x2, 0x2, 0x77, 0x78, 0x3, 0x2, 
       0x2, 0x2, 0x78, 0x7d, 0x3, 0x2, 0x2, 0x2, 0x79, 0x7a, 0x7, 0x12, 
       0x2, 0x2, 0x7a, 0x7b, 0x7, 0x12, 0x2, 0x2, 0x7b, 0x7d, 0x7, 0x2b, 
       0x2, 0x2, 0x7c, 0x67, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x68, 0x3, 0x2, 0x2, 
       0x2, 0x7c, 0x69, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x6b, 0x3, 0x2, 0x2, 0x2, 
       0x7c, 0x72, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x79, 0x3, 0x2, 0x2, 0x2, 0x7d, 
       0x11, 0x3, 0x2, 0x2, 0x2, 0x7e, 0x7f, 0x9, 0x6, 0x2, 0x2, 0x7f, 0x13, 
       0x3, 0x2, 0x2, 0x2, 0x80, 0x81, 0x5, 0x26, 0x14, 0x2, 0x81, 0x82, 
       0x7, 0x12, 0x2, 0x2, 0x82, 0x84, 0x5, 0x18, 0xd, 0x2, 0x83, 0x85, 
       0x5, 0x16, 0xc, 0x2, 0x84, 0x83, 0x3, 0x2, 0x2, 0x2, 0x84, 0x85, 
       0x3, 0x2, 0x2, 0x2, 0x85, 0x87, 0x3, 0x2, 0x2, 0x2, 0x86, 0x88, 0x7, 
       0x11, 0x2, 0x2, 0x87, 0x86, 0x3, 0x2, 0x2, 0x2, 0x87, 0x88, 0x3, 
       0x2, 0x2, 0x2, 0x88, 0x15, 0x3, 0x2, 0x2, 0x2, 0x89, 0x8a, 0x7, 0x1d, 
       0x2, 0x2, 0x8a, 0x17, 0x3, 0x2, 0x2, 0x2, 0x8b, 0x91, 0x5, 0x1a, 
       0xe, 0x2, 0x8c, 0x8d, 0x5, 0x22, 0x12, 0x2, 0x8d, 0x8e, 0x5, 0x1a, 
       0xe, 0x2, 0x8e, 0x90, 0x3, 0x2, 0x2, 0x2, 0x8f, 0x8c, 0x3, 0x2, 0x2, 
       0x2, 0x90, 0x93, 0x3, 0x2, 0x2, 0x2, 0x91, 0x8f, 0x3, 0x2, 0x2, 0x2, 
       0x91, 0x92, 0x3, 0x2, 0x2, 0x2, 0x92, 0x19, 0x3, 0x2, 0x2, 0x2, 0x93, 
       0x91, 0x3, 0x2, 0x2, 0x2, 0x94, 0x96, 0x5, 0x24, 0x13, 0x2, 0x95, 
       0x94, 0x3, 0x2, 0x2, 0x2, 0x95, 0x96, 0x3, 0x2, 0x2, 0x2, 0x96, 0xa2, 
       0x3, 0x2, 0x2, 0x2, 0x97, 0x99, 0x7, 0x22, 0x2, 0x2, 0x98, 0x9a, 
       0x9, 0x7, 0x2, 0x2, 0x99, 0x98, 0x3, 0x2, 0x2, 0x2, 0x99, 0x9a, 0x3, 
       0x2, 0x2, 0x2, 0x9a, 0xa3, 0x3, 0x2, 0x2, 0x2, 0x9b, 0xa3, 0x7, 0x2a, 
       0x2, 0x2, 0x9c, 0xa3, 0x7, 0x2b, 0x2, 0x2, 0x9d, 0xa3, 0x7, 0x20, 
       0x2, 0x2, 0x9e, 0xa3, 0x7, 0x21, 0x2, 0x2, 0x9f, 0xa3, 0x5, 0x1e, 
       0x10, 0x2, 0xa0, 0xa3, 0x5, 0x20, 0x11, 0x2, 0xa1, 0xa3, 0x5, 0x1c, 
       0xf, 0x2, 0xa2, 0x97, 0x3, 0x2, 0x2, 0x2, 0xa2, 0x9b, 0x3, 0x2, 0x2, 
       0x2, 0xa2, 0x9c, 0x3, 0x2, 0x2, 0x2, 0xa2, 0x9d, 0x3, 0x2, 0x2, 0x2, 
       0xa2, 0x9e, 0x3, 0x2, 0x2, 0x2, 0xa2, 0x9f, 0x3, 0x2, 0x2, 0x2, 0xa2, 
       0xa0, 0x3, 0x2, 0x2, 0x2, 0xa2, 0xa1, 0x3, 0x2, 0x2, 0x2, 0xa3, 0x1b, 
       0x3, 0x2, 0x2, 0x2, 0xa4, 0xa5, 0x7, 0x1f, 0x2, 0x2, 0xa5, 0xa6, 
       0x5, 0x18, 0xd, 0x2, 0xa6, 0xa7, 0x7, 0xe, 0x2, 0x2, 0xa7, 0x1d, 
       0x3, 0x2, 0x2, 0x2, 0xa8, 0xa9, 0x7, 0x5, 0x2, 0x2, 0xa9, 0xad, 0x7, 
       0x2c, 0x2, 0x2, 0xaa, 0xac, 0x9, 0x8, 0x2, 0x2, 0xab, 0xaa, 0x3, 
       0x2, 0x2, 0x2, 0xac, 0xaf, 0x3, 0x2, 0x2, 0x2, 0xad, 0xab, 0x3, 0x2, 
       0x2, 0x2, 0xad, 0xae, 0x3, 0x2, 0x2, 0x2, 0xae, 0x1f, 0x3, 0x2, 0x2, 
       0x2, 0xaf, 0xad, 0x3, 0x2, 0x2, 0x2, 0xb0, 0xb1, 0x7, 0x21, 0x2, 
       0x2, 0xb1, 0xb2, 0x7, 0x2c, 0x2, 0x2, 0xb2, 0xb3, 0x7, 0x2c, 0x2, 
       0x2, 0xb3, 0xb7, 0x7, 0x2c, 0x2, 0x2, 0xb4, 0xb5, 0x7, 0x2c, 0x2, 
       0x2, 0xb5, 0xb6, 0x7, 0x2c, 0x2, 0x2, 0xb6, 0xb8, 0x7, 0x2c, 0x2, 
       0x2, 0xb7, 0xb4, 0x3, 0x2, 0x2, 0x2, 0xb7, 0xb8, 0x3, 0x2, 0x2, 0x2, 
       0xb8, 0x21, 0x3, 0x2, 0x2, 0x2, 0xb9, 0xba, 0x9, 0x9, 0x2, 0x2, 0xba, 
       0x23, 0x3, 0x2, 0x2, 0x2, 0xbb, 0xbc, 0x9, 0xa, 0x2, 0x2, 0xbc, 0x25, 
       0x3, 0x2, 0x2, 0x2, 0xbd, 0xbe, 0x7, 0x2b, 0x2, 0x2, 0xbe, 0x27, 
       0x3, 0x2, 0x2, 0x2, 0xbf, 0xde, 0x7, 0x2b, 0x2, 0x2, 0xc0, 0xde, 
       0x7, 0x22, 0x2, 0x2, 0xc1, 0xde, 0x7, 0x23, 0x2, 0x2, 0xc2, 0xde, 
       0x7, 0x24, 0x2, 0x2, 0xc3, 0xde, 0x7, 0x25, 0x2, 0x2, 0xc4, 0xde, 
       0x7, 0x26, 0x2, 0x2, 0xc5, 0xde, 0x7, 0x27, 0x2, 0x2, 0xc6, 0xde, 
       0x7, 0x28, 0x2, 0x2, 0xc7, 0xde, 0x7, 0x29, 0x2, 0x2, 0xc8, 0xde, 
       0x7, 0x2a, 0x2, 0x2, 0xc9, 0xde, 0x7, 0x20, 0x2, 0x2, 0xca, 0xde, 
       0x7, 0x21, 0x2, 0x2, 0xcb, 0xde, 0x7, 0x15, 0x2, 0x2, 0xcc, 0xde, 
       0x7, 0x16, 0x2, 0x2, 0xcd, 0xde, 0x7, 0x14, 0x2, 0x2, 0xce, 0xde, 
       0x7, 0x17, 0x2, 0x2, 0xcf, 0xde, 0x7, 0x12, 0x2, 0x2, 0xd0, 0xde, 
       0x7, 0x11, 0x2, 0x2, 0xd1, 0xde, 0x7, 0xb, 0x2, 0x2, 0xd2, 0xde, 
       0x7, 0xc, 0x2, 0x2, 0xd3, 0xde, 0x7, 0xd, 0x2, 0x2, 0xd4, 0xde, 0x7, 
       0xe, 0x2, 0x2, 0xd5, 0xde, 0x7, 0xf, 0x2, 0x2, 0xd6, 0xde, 0x7, 0x10, 
       0x2, 0x2, 0xd7, 0xde, 0x5, 0x1c, 0xf, 0x2, 0xd8, 0xde, 0x7, 0x18, 
       0x2, 0x2, 0xd9, 0xde, 0x7, 0x19, 0x2, 0x2, 0xda, 0xde, 0x7, 0x1a, 
       0x2, 0x2, 0xdb, 0xde, 0x7, 0x1b, 0x2, 0x2, 0xdc, 0xde, 0x7, 0x1c, 
       0x2, 0x2, 0xdd, 0xbf, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xc0, 0x3, 0x2, 0x2, 
       0x2, 0xdd, 0xc1, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xc2, 0x3, 0x2, 0x2, 0x2, 
       0xdd, 0xc3, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xc4, 0x3, 0x2, 0x2, 0x2, 0xdd, 
       0xc5, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xc6, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xc7, 
       0x3, 0x2, 0x2, 0x2, 0xdd, 0xc8, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xc9, 0x3, 
       0x2, 0x2, 0x2, 0xdd, 0xca, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xcb, 0x3, 0x2, 
       0x2, 0x2, 0xdd, 0xcc, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xcd, 0x3, 0x2, 0x2, 
       0x2, 0xdd, 0xce, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xcf, 0x3, 0x2, 0x2, 0x2, 
       0xdd, 0xd0, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xd1, 0x3, 0x2, 0x2, 0x2, 0xdd, 
       0xd2, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xd3, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xd4, 
       0x3, 0x2, 0x2, 0x2, 0xdd, 0xd5, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xd6, 0x3, 
       0x2, 0x2, 0x2, 0xdd, 0xd7, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xd8, 0x3, 0x2, 
       0x2, 0x2, 0xdd, 0xd9, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xda, 0x3, 0x2, 0x2, 
       0x2, 0xdd, 0xdb, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xdc, 0x3, 0x2, 0x2, 0x2, 
       0xde, 0x29, 0x3, 0x2, 0x2, 0x2, 0xdf, 0xe0, 0x7, 0x9, 0x2, 0x2, 0xe0, 
       0x2b, 0x3, 0x2, 0x2, 0x2, 0x16, 0x2f, 0x35, 0x3b, 0x40, 0x46, 0x50, 
       0x57, 0x62, 0x6f, 0x77, 0x7c, 0x84, 0x87, 0x91, 0x95, 0x99, 0xa2, 
       0xad, 0xb7, 0xdd, 
  };

  _serializedATN.insert(_serializedATN.end(), serializedATNSegment0,
    serializedATNSegment0 + sizeof(serializedATNSegment0) / sizeof(serializedATNSegment0[0]));


  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

CSS3Parser::Initializer CSS3Parser::_init;

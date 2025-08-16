
// Generated from JavaScript.g4 by ANTLR 4.9.2


#include "JavaScriptListener.h"

#include "JavaScriptParser.h"


using namespace antlrcpp;
using namespace antlr4;

JavaScriptParser::JavaScriptParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

JavaScriptParser::~JavaScriptParser() {
  delete _interpreter;
}

std::string JavaScriptParser::getGrammarFileName() const {
  return "JavaScript.g4";
}

const std::vector<std::string>& JavaScriptParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& JavaScriptParser::getVocabulary() const {
  return _vocabulary;
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
      ((1ULL << _la) & ((1ULL << JavaScriptParser::BREAK)
      | (1ULL << JavaScriptParser::CONTINUE)
      | (1ULL << JavaScriptParser::DELETE)
      | (1ULL << JavaScriptParser::FOR)
      | (1ULL << JavaScriptParser::FUNCTION)
      | (1ULL << JavaScriptParser::IF)
      | (1ULL << JavaScriptParser::NEW)
      | (1ULL << JavaScriptParser::RETURN)
      | (1ULL << JavaScriptParser::THIS)
      | (1ULL << JavaScriptParser::TRY)
      | (1ULL << JavaScriptParser::TYPEOF)
      | (1ULL << JavaScriptParser::VAR)
      | (1ULL << JavaScriptParser::VOID)
      | (1ULL << JavaScriptParser::WHILE)
      | (1ULL << JavaScriptParser::LET)
      | (1ULL << JavaScriptParser::CONST)
      | (1ULL << JavaScriptParser::PLUS)
      | (1ULL << JavaScriptParser::MINUS)
      | (1ULL << JavaScriptParser::INCREMENT)
      | (1ULL << JavaScriptParser::DECREMENT)
      | (1ULL << JavaScriptParser::BIT_NOT)
      | (1ULL << JavaScriptParser::NOT)
      | (1ULL << JavaScriptParser::LBRACE)
      | (1ULL << JavaScriptParser::LPAREN))) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 65)) & ((1ULL << (JavaScriptParser::LBRACKET - 65))
      | (1ULL << (JavaScriptParser::SEMICOLON - 65))
      | (1ULL << (JavaScriptParser::NullLiteral - 65))
      | (1ULL << (JavaScriptParser::BooleanLiteral - 65))
      | (1ULL << (JavaScriptParser::NumericLiteral - 65))
      | (1ULL << (JavaScriptParser::StringLiteral - 65))
      | (1ULL << (JavaScriptParser::Identifier - 65)))) != 0)) {
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
      ((1ULL << _la) & ((1ULL << JavaScriptParser::BREAK)
      | (1ULL << JavaScriptParser::CONTINUE)
      | (1ULL << JavaScriptParser::DELETE)
      | (1ULL << JavaScriptParser::FOR)
      | (1ULL << JavaScriptParser::FUNCTION)
      | (1ULL << JavaScriptParser::IF)
      | (1ULL << JavaScriptParser::NEW)
      | (1ULL << JavaScriptParser::RETURN)
      | (1ULL << JavaScriptParser::THIS)
      | (1ULL << JavaScriptParser::TRY)
      | (1ULL << JavaScriptParser::TYPEOF)
      | (1ULL << JavaScriptParser::VAR)
      | (1ULL << JavaScriptParser::VOID)
      | (1ULL << JavaScriptParser::WHILE)
      | (1ULL << JavaScriptParser::LET)
      | (1ULL << JavaScriptParser::CONST)
      | (1ULL << JavaScriptParser::PLUS)
      | (1ULL << JavaScriptParser::MINUS)
      | (1ULL << JavaScriptParser::INCREMENT)
      | (1ULL << JavaScriptParser::DECREMENT)
      | (1ULL << JavaScriptParser::BIT_NOT)
      | (1ULL << JavaScriptParser::NOT)
      | (1ULL << JavaScriptParser::LBRACE)
      | (1ULL << JavaScriptParser::LPAREN))) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 65)) & ((1ULL << (JavaScriptParser::LBRACKET - 65))
      | (1ULL << (JavaScriptParser::SEMICOLON - 65))
      | (1ULL << (JavaScriptParser::NullLiteral - 65))
      | (1ULL << (JavaScriptParser::BooleanLiteral - 65))
      | (1ULL << (JavaScriptParser::NumericLiteral - 65))
      | (1ULL << (JavaScriptParser::StringLiteral - 65))
      | (1ULL << (JavaScriptParser::Identifier - 65)))) != 0));
   
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
      ((1ULL << _la) & ((1ULL << JavaScriptParser::BREAK)
      | (1ULL << JavaScriptParser::CONTINUE)
      | (1ULL << JavaScriptParser::DELETE)
      | (1ULL << JavaScriptParser::FOR)
      | (1ULL << JavaScriptParser::FUNCTION)
      | (1ULL << JavaScriptParser::IF)
      | (1ULL << JavaScriptParser::NEW)
      | (1ULL << JavaScriptParser::RETURN)
      | (1ULL << JavaScriptParser::THIS)
      | (1ULL << JavaScriptParser::TRY)
      | (1ULL << JavaScriptParser::TYPEOF)
      | (1ULL << JavaScriptParser::VAR)
      | (1ULL << JavaScriptParser::VOID)
      | (1ULL << JavaScriptParser::WHILE)
      | (1ULL << JavaScriptParser::LET)
      | (1ULL << JavaScriptParser::CONST)
      | (1ULL << JavaScriptParser::PLUS)
      | (1ULL << JavaScriptParser::MINUS)
      | (1ULL << JavaScriptParser::INCREMENT)
      | (1ULL << JavaScriptParser::DECREMENT)
      | (1ULL << JavaScriptParser::BIT_NOT)
      | (1ULL << JavaScriptParser::NOT)
      | (1ULL << JavaScriptParser::LBRACE)
      | (1ULL << JavaScriptParser::LPAREN))) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 65)) & ((1ULL << (JavaScriptParser::LBRACKET - 65))
      | (1ULL << (JavaScriptParser::SEMICOLON - 65))
      | (1ULL << (JavaScriptParser::NullLiteral - 65))
      | (1ULL << (JavaScriptParser::BooleanLiteral - 65))
      | (1ULL << (JavaScriptParser::NumericLiteral - 65))
      | (1ULL << (JavaScriptParser::StringLiteral - 65))
      | (1ULL << (JavaScriptParser::Identifier - 65)))) != 0)) {
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
      ((1ULL << _la) & ((1ULL << JavaScriptParser::BREAK)
      | (1ULL << JavaScriptParser::CONTINUE)
      | (1ULL << JavaScriptParser::DELETE)
      | (1ULL << JavaScriptParser::FOR)
      | (1ULL << JavaScriptParser::FUNCTION)
      | (1ULL << JavaScriptParser::IF)
      | (1ULL << JavaScriptParser::NEW)
      | (1ULL << JavaScriptParser::RETURN)
      | (1ULL << JavaScriptParser::THIS)
      | (1ULL << JavaScriptParser::TRY)
      | (1ULL << JavaScriptParser::TYPEOF)
      | (1ULL << JavaScriptParser::VAR)
      | (1ULL << JavaScriptParser::VOID)
      | (1ULL << JavaScriptParser::WHILE)
      | (1ULL << JavaScriptParser::LET)
      | (1ULL << JavaScriptParser::CONST)
      | (1ULL << JavaScriptParser::PLUS)
      | (1ULL << JavaScriptParser::MINUS)
      | (1ULL << JavaScriptParser::INCREMENT)
      | (1ULL << JavaScriptParser::DECREMENT)
      | (1ULL << JavaScriptParser::BIT_NOT)
      | (1ULL << JavaScriptParser::NOT)
      | (1ULL << JavaScriptParser::LBRACE)
      | (1ULL << JavaScriptParser::LPAREN))) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 65)) & ((1ULL << (JavaScriptParser::LBRACKET - 65))
      | (1ULL << (JavaScriptParser::SEMICOLON - 65))
      | (1ULL << (JavaScriptParser::NullLiteral - 65))
      | (1ULL << (JavaScriptParser::BooleanLiteral - 65))
      | (1ULL << (JavaScriptParser::NumericLiteral - 65))
      | (1ULL << (JavaScriptParser::StringLiteral - 65))
      | (1ULL << (JavaScriptParser::Identifier - 65)))) != 0));
   
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
        _localctx = dynamic_cast<IterationStatementContext *>(_tracker.createInstance<JavaScriptParser::WhileStatementContext>(_localctx));
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
        _localctx = dynamic_cast<IterationStatementContext *>(_tracker.createInstance<JavaScriptParser::ForStatementContext>(_localctx));
        enterOuterAlt(_localctx, 2);
        setState(190);
        match(JavaScriptParser::FOR);
        setState(191);
        match(JavaScriptParser::LPAREN);
        setState(193);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << JavaScriptParser::DELETE)
          | (1ULL << JavaScriptParser::FUNCTION)
          | (1ULL << JavaScriptParser::NEW)
          | (1ULL << JavaScriptParser::THIS)
          | (1ULL << JavaScriptParser::TYPEOF)
          | (1ULL << JavaScriptParser::VOID)
          | (1ULL << JavaScriptParser::PLUS)
          | (1ULL << JavaScriptParser::MINUS)
          | (1ULL << JavaScriptParser::INCREMENT)
          | (1ULL << JavaScriptParser::DECREMENT)
          | (1ULL << JavaScriptParser::BIT_NOT)
          | (1ULL << JavaScriptParser::NOT)
          | (1ULL << JavaScriptParser::LBRACE)
          | (1ULL << JavaScriptParser::LPAREN))) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 65)) & ((1ULL << (JavaScriptParser::LBRACKET - 65))
          | (1ULL << (JavaScriptParser::NullLiteral - 65))
          | (1ULL << (JavaScriptParser::BooleanLiteral - 65))
          | (1ULL << (JavaScriptParser::NumericLiteral - 65))
          | (1ULL << (JavaScriptParser::StringLiteral - 65))
          | (1ULL << (JavaScriptParser::Identifier - 65)))) != 0)) {
          setState(192);
          forInit();
        }
        setState(195);
        match(JavaScriptParser::SEMICOLON);
        setState(197);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << JavaScriptParser::DELETE)
          | (1ULL << JavaScriptParser::FUNCTION)
          | (1ULL << JavaScriptParser::NEW)
          | (1ULL << JavaScriptParser::THIS)
          | (1ULL << JavaScriptParser::TYPEOF)
          | (1ULL << JavaScriptParser::VOID)
          | (1ULL << JavaScriptParser::PLUS)
          | (1ULL << JavaScriptParser::MINUS)
          | (1ULL << JavaScriptParser::INCREMENT)
          | (1ULL << JavaScriptParser::DECREMENT)
          | (1ULL << JavaScriptParser::BIT_NOT)
          | (1ULL << JavaScriptParser::NOT)
          | (1ULL << JavaScriptParser::LBRACE)
          | (1ULL << JavaScriptParser::LPAREN))) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 65)) & ((1ULL << (JavaScriptParser::LBRACKET - 65))
          | (1ULL << (JavaScriptParser::NullLiteral - 65))
          | (1ULL << (JavaScriptParser::BooleanLiteral - 65))
          | (1ULL << (JavaScriptParser::NumericLiteral - 65))
          | (1ULL << (JavaScriptParser::StringLiteral - 65))
          | (1ULL << (JavaScriptParser::Identifier - 65)))) != 0)) {
          setState(196);
          expression();
        }
        setState(199);
        match(JavaScriptParser::SEMICOLON);
        setState(201);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << JavaScriptParser::DELETE)
          | (1ULL << JavaScriptParser::FUNCTION)
          | (1ULL << JavaScriptParser::NEW)
          | (1ULL << JavaScriptParser::THIS)
          | (1ULL << JavaScriptParser::TYPEOF)
          | (1ULL << JavaScriptParser::VOID)
          | (1ULL << JavaScriptParser::PLUS)
          | (1ULL << JavaScriptParser::MINUS)
          | (1ULL << JavaScriptParser::INCREMENT)
          | (1ULL << JavaScriptParser::DECREMENT)
          | (1ULL << JavaScriptParser::BIT_NOT)
          | (1ULL << JavaScriptParser::NOT)
          | (1ULL << JavaScriptParser::LBRACE)
          | (1ULL << JavaScriptParser::LPAREN))) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 65)) & ((1ULL << (JavaScriptParser::LBRACKET - 65))
          | (1ULL << (JavaScriptParser::NullLiteral - 65))
          | (1ULL << (JavaScriptParser::BooleanLiteral - 65))
          | (1ULL << (JavaScriptParser::NumericLiteral - 65))
          | (1ULL << (JavaScriptParser::StringLiteral - 65))
          | (1ULL << (JavaScriptParser::Identifier - 65)))) != 0)) {
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
      ((1ULL << _la) & ((1ULL << JavaScriptParser::DELETE)
      | (1ULL << JavaScriptParser::FUNCTION)
      | (1ULL << JavaScriptParser::NEW)
      | (1ULL << JavaScriptParser::THIS)
      | (1ULL << JavaScriptParser::TYPEOF)
      | (1ULL << JavaScriptParser::VOID)
      | (1ULL << JavaScriptParser::PLUS)
      | (1ULL << JavaScriptParser::MINUS)
      | (1ULL << JavaScriptParser::INCREMENT)
      | (1ULL << JavaScriptParser::DECREMENT)
      | (1ULL << JavaScriptParser::BIT_NOT)
      | (1ULL << JavaScriptParser::NOT)
      | (1ULL << JavaScriptParser::LBRACE)
      | (1ULL << JavaScriptParser::LPAREN))) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 65)) & ((1ULL << (JavaScriptParser::LBRACKET - 65))
      | (1ULL << (JavaScriptParser::NullLiteral - 65))
      | (1ULL << (JavaScriptParser::BooleanLiteral - 65))
      | (1ULL << (JavaScriptParser::NumericLiteral - 65))
      | (1ULL << (JavaScriptParser::StringLiteral - 65))
      | (1ULL << (JavaScriptParser::Identifier - 65)))) != 0)) {
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
      ((1ULL << _la) & ((1ULL << JavaScriptParser::BREAK)
      | (1ULL << JavaScriptParser::CONTINUE)
      | (1ULL << JavaScriptParser::DELETE)
      | (1ULL << JavaScriptParser::FOR)
      | (1ULL << JavaScriptParser::FUNCTION)
      | (1ULL << JavaScriptParser::IF)
      | (1ULL << JavaScriptParser::NEW)
      | (1ULL << JavaScriptParser::RETURN)
      | (1ULL << JavaScriptParser::THIS)
      | (1ULL << JavaScriptParser::TRY)
      | (1ULL << JavaScriptParser::TYPEOF)
      | (1ULL << JavaScriptParser::VAR)
      | (1ULL << JavaScriptParser::VOID)
      | (1ULL << JavaScriptParser::WHILE)
      | (1ULL << JavaScriptParser::LET)
      | (1ULL << JavaScriptParser::CONST)
      | (1ULL << JavaScriptParser::PLUS)
      | (1ULL << JavaScriptParser::MINUS)
      | (1ULL << JavaScriptParser::INCREMENT)
      | (1ULL << JavaScriptParser::DECREMENT)
      | (1ULL << JavaScriptParser::BIT_NOT)
      | (1ULL << JavaScriptParser::NOT)
      | (1ULL << JavaScriptParser::LBRACE)
      | (1ULL << JavaScriptParser::LPAREN))) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 65)) & ((1ULL << (JavaScriptParser::LBRACKET - 65))
      | (1ULL << (JavaScriptParser::SEMICOLON - 65))
      | (1ULL << (JavaScriptParser::NullLiteral - 65))
      | (1ULL << (JavaScriptParser::BooleanLiteral - 65))
      | (1ULL << (JavaScriptParser::NumericLiteral - 65))
      | (1ULL << (JavaScriptParser::StringLiteral - 65))
      | (1ULL << (JavaScriptParser::Identifier - 65)))) != 0)) {
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
      ((1ULL << _la) & ((1ULL << JavaScriptParser::ASSIGN)
      | (1ULL << JavaScriptParser::MUL_ASSIGN)
      | (1ULL << JavaScriptParser::DIV_ASSIGN)
      | (1ULL << JavaScriptParser::MOD_ASSIGN)
      | (1ULL << JavaScriptParser::PLUS_ASSIGN)
      | (1ULL << JavaScriptParser::MINUS_ASSIGN))) != 0))) {
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
      ((1ULL << _la) & ((1ULL << JavaScriptParser::EQUALS)
      | (1ULL << JavaScriptParser::NOT_EQUALS)
      | (1ULL << JavaScriptParser::STRICT_EQUALS)
      | (1ULL << JavaScriptParser::STRICT_NOT_EQUALS))) != 0)) {
      setState(329);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << JavaScriptParser::EQUALS)
        | (1ULL << JavaScriptParser::NOT_EQUALS)
        | (1ULL << JavaScriptParser::STRICT_EQUALS)
        | (1ULL << JavaScriptParser::STRICT_NOT_EQUALS))) != 0))) {
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
      ((1ULL << _la) & ((1ULL << JavaScriptParser::IN)
      | (1ULL << JavaScriptParser::INSTANCEOF)
      | (1ULL << JavaScriptParser::LT)
      | (1ULL << JavaScriptParser::GT)
      | (1ULL << JavaScriptParser::LE)
      | (1ULL << JavaScriptParser::GE))) != 0)) {
      setState(337);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << JavaScriptParser::IN)
        | (1ULL << JavaScriptParser::INSTANCEOF)
        | (1ULL << JavaScriptParser::LT)
        | (1ULL << JavaScriptParser::GT)
        | (1ULL << JavaScriptParser::LE)
        | (1ULL << JavaScriptParser::GE))) != 0))) {
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
      ((1ULL << _la) & ((1ULL << JavaScriptParser::LEFT_SHIFT)
      | (1ULL << JavaScriptParser::RIGHT_SHIFT)
      | (1ULL << JavaScriptParser::UNSIGNED_RIGHT_SHIFT))) != 0)) {
      setState(345);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << JavaScriptParser::LEFT_SHIFT)
        | (1ULL << JavaScriptParser::RIGHT_SHIFT)
        | (1ULL << JavaScriptParser::UNSIGNED_RIGHT_SHIFT))) != 0))) {
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
      ((1ULL << _la) & ((1ULL << JavaScriptParser::MULTIPLY)
      | (1ULL << JavaScriptParser::DIVIDE)
      | (1ULL << JavaScriptParser::MODULUS))) != 0)) {
      setState(361);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << JavaScriptParser::MULTIPLY)
        | (1ULL << JavaScriptParser::DIVIDE)
        | (1ULL << JavaScriptParser::MODULUS))) != 0))) {
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
          ((1ULL << _la) & ((1ULL << JavaScriptParser::DELETE)
          | (1ULL << JavaScriptParser::TYPEOF)
          | (1ULL << JavaScriptParser::VOID)
          | (1ULL << JavaScriptParser::PLUS)
          | (1ULL << JavaScriptParser::MINUS)
          | (1ULL << JavaScriptParser::INCREMENT)
          | (1ULL << JavaScriptParser::DECREMENT)
          | (1ULL << JavaScriptParser::BIT_NOT)
          | (1ULL << JavaScriptParser::NOT))) != 0))) {
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
      ((1ULL << _la) & ((1ULL << JavaScriptParser::DELETE)
      | (1ULL << JavaScriptParser::FUNCTION)
      | (1ULL << JavaScriptParser::NEW)
      | (1ULL << JavaScriptParser::THIS)
      | (1ULL << JavaScriptParser::TYPEOF)
      | (1ULL << JavaScriptParser::VOID)
      | (1ULL << JavaScriptParser::PLUS)
      | (1ULL << JavaScriptParser::MINUS)
      | (1ULL << JavaScriptParser::INCREMENT)
      | (1ULL << JavaScriptParser::DECREMENT)
      | (1ULL << JavaScriptParser::BIT_NOT)
      | (1ULL << JavaScriptParser::NOT)
      | (1ULL << JavaScriptParser::LBRACE)
      | (1ULL << JavaScriptParser::LPAREN))) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 65)) & ((1ULL << (JavaScriptParser::LBRACKET - 65))
      | (1ULL << (JavaScriptParser::NullLiteral - 65))
      | (1ULL << (JavaScriptParser::BooleanLiteral - 65))
      | (1ULL << (JavaScriptParser::NumericLiteral - 65))
      | (1ULL << (JavaScriptParser::StringLiteral - 65))
      | (1ULL << (JavaScriptParser::Identifier - 65)))) != 0)) {
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
      ((1ULL << (_la - 72)) & ((1ULL << (JavaScriptParser::NumericLiteral - 72))
      | (1ULL << (JavaScriptParser::StringLiteral - 72))
      | (1ULL << (JavaScriptParser::Identifier - 72)))) != 0)) {
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
      ((1ULL << (_la - 72)) & ((1ULL << (JavaScriptParser::NumericLiteral - 72))
      | (1ULL << (JavaScriptParser::StringLiteral - 72))
      | (1ULL << (JavaScriptParser::Identifier - 72)))) != 0))) {
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
      ((1ULL << _la) & ((1ULL << JavaScriptParser::DELETE)
      | (1ULL << JavaScriptParser::FUNCTION)
      | (1ULL << JavaScriptParser::NEW)
      | (1ULL << JavaScriptParser::THIS)
      | (1ULL << JavaScriptParser::TYPEOF)
      | (1ULL << JavaScriptParser::VOID)
      | (1ULL << JavaScriptParser::PLUS)
      | (1ULL << JavaScriptParser::MINUS)
      | (1ULL << JavaScriptParser::INCREMENT)
      | (1ULL << JavaScriptParser::DECREMENT)
      | (1ULL << JavaScriptParser::BIT_NOT)
      | (1ULL << JavaScriptParser::NOT)
      | (1ULL << JavaScriptParser::LBRACE)
      | (1ULL << JavaScriptParser::LPAREN))) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 65)) & ((1ULL << (JavaScriptParser::LBRACKET - 65))
      | (1ULL << (JavaScriptParser::NullLiteral - 65))
      | (1ULL << (JavaScriptParser::BooleanLiteral - 65))
      | (1ULL << (JavaScriptParser::NumericLiteral - 65))
      | (1ULL << (JavaScriptParser::StringLiteral - 65))
      | (1ULL << (JavaScriptParser::Identifier - 65)))) != 0)) {
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
      ((1ULL << (_la - 70)) & ((1ULL << (JavaScriptParser::NullLiteral - 70))
      | (1ULL << (JavaScriptParser::BooleanLiteral - 70))
      | (1ULL << (JavaScriptParser::NumericLiteral - 70))
      | (1ULL << (JavaScriptParser::StringLiteral - 70)))) != 0))) {
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
    case 40: return callExpressionSempred(dynamic_cast<CallExpressionContext *>(context), predicateIndex);
    case 41: return memberExpressionSempred(dynamic_cast<MemberExpressionContext *>(context), predicateIndex);

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

// Static vars and initialization.
std::vector<dfa::DFA> JavaScriptParser::_decisionToDFA;
atn::PredictionContextCache JavaScriptParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN JavaScriptParser::_atn;
std::vector<uint16_t> JavaScriptParser::_serializedATN;

std::vector<std::string> JavaScriptParser::_ruleNames = {
  "program", "sourceElements", "sourceElement", "statement", "block", "statementList", 
  "variableStatement", "variableDeclarationList", "variableDeclaration", 
  "emptyStatement", "expressionStatement", "ifStatement", "iterationStatement", 
  "forInit", "continueStatement", "breakStatement", "returnStatement", "tryStatement", 
  "catchClause", "finallyClause", "functionDeclaration", "formalParameterList", 
  "functionBody", "expression", "assignmentExpression", "assignmentOperator", 
  "conditionalExpression", "logicalORExpression", "logicalANDExpression", 
  "bitwiseORExpression", "bitwiseXORExpression", "bitwiseANDExpression", 
  "equalityExpression", "relationalExpression", "shiftExpression", "additiveExpression", 
  "multiplicativeExpression", "unaryExpression", "postfixExpression", "leftHandSideExpression", 
  "callExpression", "memberExpression", "primaryExpression", "arrayLiteral", 
  "elementList", "objectLiteral", "propertyNameAndValueList", "propertyAssignment", 
  "propertyName", "arguments", "argumentList", "functionExpression", "literal"
};

std::vector<std::string> JavaScriptParser::_literalNames = {
  "", "'break'", "'case'", "'catch'", "'continue'", "'default'", "'delete'", 
  "'do'", "'else'", "'finally'", "'for'", "'function'", "'if'", "'in'", 
  "'instanceof'", "'new'", "'return'", "'switch'", "'this'", "'throw'", 
  "'try'", "'typeof'", "'var'", "'void'", "'while'", "'with'", "'let'", 
  "'const'", "'='", "'*='", "'/='", "'%='", "'+='", "'-='", "'\u003F'", 
  "':'", "'||'", "'&&'", "'|'", "'^'", "'&'", "'=='", "'!='", "'==='", "'!=='", 
  "'<'", "'>'", "'<='", "'>='", "'<<'", "'>>'", "'>>>'", "'+'", "'-'", "'*'", 
  "'/'", "'%'", "'++'", "'--'", "'~'", "'!'", "'{'", "'}'", "'('", "')'", 
  "'['", "']'", "';'", "','", "'.'", "'null'"
};

std::vector<std::string> JavaScriptParser::_symbolicNames = {
  "", "BREAK", "CASE", "CATCH", "CONTINUE", "DEFAULT", "DELETE", "DO", "ELSE", 
  "FINALLY", "FOR", "FUNCTION", "IF", "IN", "INSTANCEOF", "NEW", "RETURN", 
  "SWITCH", "THIS", "THROW", "TRY", "TYPEOF", "VAR", "VOID", "WHILE", "WITH", 
  "LET", "CONST", "ASSIGN", "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN", "PLUS_ASSIGN", 
  "MINUS_ASSIGN", "QUESTION", "COLON", "OR", "AND", "BIT_OR", "BIT_XOR", 
  "BIT_AND", "EQUALS", "NOT_EQUALS", "STRICT_EQUALS", "STRICT_NOT_EQUALS", 
  "LT", "GT", "LE", "GE", "LEFT_SHIFT", "RIGHT_SHIFT", "UNSIGNED_RIGHT_SHIFT", 
  "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "MODULUS", "INCREMENT", "DECREMENT", 
  "BIT_NOT", "NOT", "LBRACE", "RBRACE", "LPAREN", "RPAREN", "LBRACKET", 
  "RBRACKET", "SEMICOLON", "COMMA", "DOT", "NullLiteral", "BooleanLiteral", 
  "NumericLiteral", "StringLiteral", "Identifier", "WS", "COMMENT", "LINE_COMMENT"
};

dfa::Vocabulary JavaScriptParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> JavaScriptParser::_tokenNames;

JavaScriptParser::Initializer::Initializer() {
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
       0x3, 0x4f, 0x1f3, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
       0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 
       0x4, 0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 
       0xe, 0x9, 0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 
       0x9, 0x11, 0x4, 0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 
       0x9, 0x14, 0x4, 0x15, 0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 
       0x9, 0x17, 0x4, 0x18, 0x9, 0x18, 0x4, 0x19, 0x9, 0x19, 0x4, 0x1a, 
       0x9, 0x1a, 0x4, 0x1b, 0x9, 0x1b, 0x4, 0x1c, 0x9, 0x1c, 0x4, 0x1d, 
       0x9, 0x1d, 0x4, 0x1e, 0x9, 0x1e, 0x4, 0x1f, 0x9, 0x1f, 0x4, 0x20, 
       0x9, 0x20, 0x4, 0x21, 0x9, 0x21, 0x4, 0x22, 0x9, 0x22, 0x4, 0x23, 
       0x9, 0x23, 0x4, 0x24, 0x9, 0x24, 0x4, 0x25, 0x9, 0x25, 0x4, 0x26, 
       0x9, 0x26, 0x4, 0x27, 0x9, 0x27, 0x4, 0x28, 0x9, 0x28, 0x4, 0x29, 
       0x9, 0x29, 0x4, 0x2a, 0x9, 0x2a, 0x4, 0x2b, 0x9, 0x2b, 0x4, 0x2c, 
       0x9, 0x2c, 0x4, 0x2d, 0x9, 0x2d, 0x4, 0x2e, 0x9, 0x2e, 0x4, 0x2f, 
       0x9, 0x2f, 0x4, 0x30, 0x9, 0x30, 0x4, 0x31, 0x9, 0x31, 0x4, 0x32, 
       0x9, 0x32, 0x4, 0x33, 0x9, 0x33, 0x4, 0x34, 0x9, 0x34, 0x4, 0x35, 
       0x9, 0x35, 0x4, 0x36, 0x9, 0x36, 0x3, 0x2, 0x5, 0x2, 0x6e, 0xa, 0x2, 
       0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x6, 0x3, 0x73, 0xa, 0x3, 0xd, 0x3, 
       0xe, 0x3, 0x74, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x79, 0xa, 0x4, 0x3, 
       0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 
       0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 0x85, 0xa, 0x5, 0x3, 0x6, 
       0x3, 0x6, 0x5, 0x6, 0x89, 0xa, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 
       0x6, 0x7, 0x8e, 0xa, 0x7, 0xd, 0x7, 0xe, 0x7, 0x8f, 0x3, 0x8, 0x3, 
       0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 
       0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x5, 0x8, 0x9e, 0xa, 0x8, 
       0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x7, 0x9, 0xa3, 0xa, 0x9, 0xc, 0x9, 
       0xe, 0x9, 0xa6, 0xb, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x5, 0xa, 
       0xab, 0xa, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 
       0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 
       0xd, 0x5, 0xd, 0xb9, 0xa, 0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 
       0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 
       0xc4, 0xa, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 0xc8, 0xa, 0xe, 0x3, 
       0xe, 0x3, 0xe, 0x5, 0xe, 0xcc, 0xa, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 
       0xe, 0xd0, 0xa, 0xe, 0x3, 0xf, 0x3, 0xf, 0x5, 0xf, 0xd4, 0xa, 0xf, 
       0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0xd8, 0xa, 0x10, 0x3, 0x10, 0x3, 
       0x10, 0x3, 0x11, 0x3, 0x11, 0x5, 0x11, 0xde, 0xa, 0x11, 0x3, 0x11, 
       0x3, 0x11, 0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 0xe4, 0xa, 0x12, 0x3, 
       0x12, 0x3, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x5, 
       0x13, 0xec, 0xa, 0x13, 0x3, 0x13, 0x5, 0x13, 0xef, 0xa, 0x13, 0x3, 
       0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 
       0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 
       0x16, 0x5, 0x16, 0xfe, 0xa, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 
       0x3, 0x16, 0x3, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x7, 0x17, 
       0x108, 0xa, 0x17, 0xc, 0x17, 0xe, 0x17, 0x10b, 0xb, 0x17, 0x3, 0x18, 
       0x5, 0x18, 0x10e, 0xa, 0x18, 0x3, 0x19, 0x3, 0x19, 0x3, 0x1a, 0x3, 
       0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x5, 0x1a, 0x117, 0xa, 0x1a, 
       0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 
       0x3, 0x1c, 0x3, 0x1c, 0x5, 0x1c, 0x121, 0xa, 0x1c, 0x3, 0x1d, 0x3, 
       0x1d, 0x3, 0x1d, 0x7, 0x1d, 0x126, 0xa, 0x1d, 0xc, 0x1d, 0xe, 0x1d, 
       0x129, 0xb, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x7, 0x1e, 0x12e, 
       0xa, 0x1e, 0xc, 0x1e, 0xe, 0x1e, 0x131, 0xb, 0x1e, 0x3, 0x1f, 0x3, 
       0x1f, 0x3, 0x1f, 0x7, 0x1f, 0x136, 0xa, 0x1f, 0xc, 0x1f, 0xe, 0x1f, 
       0x139, 0xb, 0x1f, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x7, 0x20, 0x13e, 
       0xa, 0x20, 0xc, 0x20, 0xe, 0x20, 0x141, 0xb, 0x20, 0x3, 0x21, 0x3, 
       0x21, 0x3, 0x21, 0x7, 0x21, 0x146, 0xa, 0x21, 0xc, 0x21, 0xe, 0x21, 
       0x149, 0xb, 0x21, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x7, 0x22, 0x14e, 
       0xa, 0x22, 0xc, 0x22, 0xe, 0x22, 0x151, 0xb, 0x22, 0x3, 0x23, 0x3, 
       0x23, 0x3, 0x23, 0x7, 0x23, 0x156, 0xa, 0x23, 0xc, 0x23, 0xe, 0x23, 
       0x159, 0xb, 0x23, 0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 0x7, 0x24, 0x15e, 
       0xa, 0x24, 0xc, 0x24, 0xe, 0x24, 0x161, 0xb, 0x24, 0x3, 0x25, 0x3, 
       0x25, 0x3, 0x25, 0x7, 0x25, 0x166, 0xa, 0x25, 0xc, 0x25, 0xe, 0x25, 
       0x169, 0xb, 0x25, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x7, 0x26, 0x16e, 
       0xa, 0x26, 0xc, 0x26, 0xe, 0x26, 0x171, 0xb, 0x26, 0x3, 0x27, 0x3, 
       0x27, 0x3, 0x27, 0x5, 0x27, 0x176, 0xa, 0x27, 0x3, 0x28, 0x3, 0x28, 
       0x5, 0x28, 0x17a, 0xa, 0x28, 0x3, 0x29, 0x3, 0x29, 0x5, 0x29, 0x17e, 
       0xa, 0x29, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 
       0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 
       0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x7, 0x2a, 0x18e, 0xa, 0x2a, 0xc, 
       0x2a, 0xe, 0x2a, 0x191, 0xb, 0x2a, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 
       0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x19a, 0xa, 
       0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 
       0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x7, 0x2b, 0x1a4, 0xa, 0x2b, 0xc, 0x2b, 
       0xe, 0x2b, 0x1a7, 0xb, 0x2b, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x3, 
       0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x5, 
       0x2c, 0x1b2, 0xa, 0x2c, 0x3, 0x2d, 0x3, 0x2d, 0x5, 0x2d, 0x1b6, 0xa, 
       0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x7, 
       0x2e, 0x1bd, 0xa, 0x2e, 0xc, 0x2e, 0xe, 0x2e, 0x1c0, 0xb, 0x2e, 0x3, 
       0x2f, 0x3, 0x2f, 0x5, 0x2f, 0x1c4, 0xa, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 
       0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x7, 0x30, 0x1cb, 0xa, 0x30, 0xc, 
       0x30, 0xe, 0x30, 0x1ce, 0xb, 0x30, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 
       0x3, 0x31, 0x3, 0x32, 0x3, 0x32, 0x3, 0x33, 0x3, 0x33, 0x5, 0x33, 
       0x1d8, 0xa, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x34, 0x3, 0x34, 0x3, 
       0x34, 0x7, 0x34, 0x1df, 0xa, 0x34, 0xc, 0x34, 0xe, 0x34, 0x1e2, 0xb, 
       0x34, 0x3, 0x35, 0x3, 0x35, 0x5, 0x35, 0x1e6, 0xa, 0x35, 0x3, 0x35, 
       0x3, 0x35, 0x5, 0x35, 0x1ea, 0xa, 0x35, 0x3, 0x35, 0x3, 0x35, 0x3, 
       0x35, 0x3, 0x35, 0x3, 0x35, 0x3, 0x36, 0x3, 0x36, 0x3, 0x36, 0x2, 
       0x4, 0x52, 0x54, 0x37, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 
       0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 
       0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 
       0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 
       0x56, 0x58, 0x5a, 0x5c, 0x5e, 0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 
       0x2, 0xc, 0x3, 0x2, 0x1e, 0x23, 0x3, 0x2, 0x2b, 0x2e, 0x4, 0x2, 0xf, 
       0x10, 0x2f, 0x32, 0x3, 0x2, 0x33, 0x35, 0x3, 0x2, 0x36, 0x37, 0x3, 
       0x2, 0x38, 0x3a, 0x7, 0x2, 0x8, 0x8, 0x17, 0x17, 0x19, 0x19, 0x36, 
       0x37, 0x3b, 0x3e, 0x3, 0x2, 0x3b, 0x3c, 0x3, 0x2, 0x4a, 0x4c, 0x3, 
       0x2, 0x48, 0x4b, 0x2, 0x200, 0x2, 0x6d, 0x3, 0x2, 0x2, 0x2, 0x4, 
       0x72, 0x3, 0x2, 0x2, 0x2, 0x6, 0x78, 0x3, 0x2, 0x2, 0x2, 0x8, 0x84, 
       0x3, 0x2, 0x2, 0x2, 0xa, 0x86, 0x3, 0x2, 0x2, 0x2, 0xc, 0x8d, 0x3, 
       0x2, 0x2, 0x2, 0xe, 0x9d, 0x3, 0x2, 0x2, 0x2, 0x10, 0x9f, 0x3, 0x2, 
       0x2, 0x2, 0x12, 0xa7, 0x3, 0x2, 0x2, 0x2, 0x14, 0xac, 0x3, 0x2, 0x2, 
       0x2, 0x16, 0xae, 0x3, 0x2, 0x2, 0x2, 0x18, 0xb1, 0x3, 0x2, 0x2, 0x2, 
       0x1a, 0xcf, 0x3, 0x2, 0x2, 0x2, 0x1c, 0xd3, 0x3, 0x2, 0x2, 0x2, 0x1e, 
       0xd5, 0x3, 0x2, 0x2, 0x2, 0x20, 0xdb, 0x3, 0x2, 0x2, 0x2, 0x22, 0xe1, 
       0x3, 0x2, 0x2, 0x2, 0x24, 0xe7, 0x3, 0x2, 0x2, 0x2, 0x26, 0xf0, 0x3, 
       0x2, 0x2, 0x2, 0x28, 0xf6, 0x3, 0x2, 0x2, 0x2, 0x2a, 0xf9, 0x3, 0x2, 
       0x2, 0x2, 0x2c, 0x104, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x10d, 0x3, 0x2, 
       0x2, 0x2, 0x30, 0x10f, 0x3, 0x2, 0x2, 0x2, 0x32, 0x116, 0x3, 0x2, 
       0x2, 0x2, 0x34, 0x118, 0x3, 0x2, 0x2, 0x2, 0x36, 0x11a, 0x3, 0x2, 
       0x2, 0x2, 0x38, 0x122, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x12a, 0x3, 0x2, 
       0x2, 0x2, 0x3c, 0x132, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x13a, 0x3, 0x2, 
       0x2, 0x2, 0x40, 0x142, 0x3, 0x2, 0x2, 0x2, 0x42, 0x14a, 0x3, 0x2, 
       0x2, 0x2, 0x44, 0x152, 0x3, 0x2, 0x2, 0x2, 0x46, 0x15a, 0x3, 0x2, 
       0x2, 0x2, 0x48, 0x162, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x16a, 0x3, 0x2, 
       0x2, 0x2, 0x4c, 0x175, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x177, 0x3, 0x2, 
       0x2, 0x2, 0x50, 0x17d, 0x3, 0x2, 0x2, 0x2, 0x52, 0x17f, 0x3, 0x2, 
       0x2, 0x2, 0x54, 0x199, 0x3, 0x2, 0x2, 0x2, 0x56, 0x1b1, 0x3, 0x2, 
       0x2, 0x2, 0x58, 0x1b3, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x1b9, 0x3, 0x2, 
       0x2, 0x2, 0x5c, 0x1c1, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x1c7, 0x3, 0x2, 
       0x2, 0x2, 0x60, 0x1cf, 0x3, 0x2, 0x2, 0x2, 0x62, 0x1d3, 0x3, 0x2, 
       0x2, 0x2, 0x64, 0x1d5, 0x3, 0x2, 0x2, 0x2, 0x66, 0x1db, 0x3, 0x2, 
       0x2, 0x2, 0x68, 0x1e3, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x1f0, 0x3, 0x2, 
       0x2, 0x2, 0x6c, 0x6e, 0x5, 0x4, 0x3, 0x2, 0x6d, 0x6c, 0x3, 0x2, 0x2, 
       0x2, 0x6d, 0x6e, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x6f, 0x3, 0x2, 0x2, 0x2, 
       0x6f, 0x70, 0x7, 0x2, 0x2, 0x3, 0x70, 0x3, 0x3, 0x2, 0x2, 0x2, 0x71, 
       0x73, 0x5, 0x6, 0x4, 0x2, 0x72, 0x71, 0x3, 0x2, 0x2, 0x2, 0x73, 0x74, 
       0x3, 0x2, 0x2, 0x2, 0x74, 0x72, 0x3, 0x2, 0x2, 0x2, 0x74, 0x75, 0x3, 
       0x2, 0x2, 0x2, 0x75, 0x5, 0x3, 0x2, 0x2, 0x2, 0x76, 0x79, 0x5, 0x8, 
       0x5, 0x2, 0x77, 0x79, 0x5, 0x2a, 0x16, 0x2, 0x78, 0x76, 0x3, 0x2, 
       0x2, 0x2, 0x78, 0x77, 0x3, 0x2, 0x2, 0x2, 0x79, 0x7, 0x3, 0x2, 0x2, 
       0x2, 0x7a, 0x85, 0x5, 0xa, 0x6, 0x2, 0x7b, 0x85, 0x5, 0xe, 0x8, 0x2, 
       0x7c, 0x85, 0x5, 0x14, 0xb, 0x2, 0x7d, 0x85, 0x5, 0x16, 0xc, 0x2, 
       0x7e, 0x85, 0x5, 0x18, 0xd, 0x2, 0x7f, 0x85, 0x5, 0x1a, 0xe, 0x2, 
       0x80, 0x85, 0x5, 0x1e, 0x10, 0x2, 0x81, 0x85, 0x5, 0x20, 0x11, 0x2, 
       0x82, 0x85, 0x5, 0x22, 0x12, 0x2, 0x83, 0x85, 0x5, 0x24, 0x13, 0x2, 
       0x84, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x84, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x84, 
       0x7c, 0x3, 0x2, 0x2, 0x2, 0x84, 0x7d, 0x3, 0x2, 0x2, 0x2, 0x84, 0x7e, 
       0x3, 0x2, 0x2, 0x2, 0x84, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x84, 0x80, 0x3, 
       0x2, 0x2, 0x2, 0x84, 0x81, 0x3, 0x2, 0x2, 0x2, 0x84, 0x82, 0x3, 0x2, 
       0x2, 0x2, 0x84, 0x83, 0x3, 0x2, 0x2, 0x2, 0x85, 0x9, 0x3, 0x2, 0x2, 
       0x2, 0x86, 0x88, 0x7, 0x3f, 0x2, 0x2, 0x87, 0x89, 0x5, 0xc, 0x7, 
       0x2, 0x88, 0x87, 0x3, 0x2, 0x2, 0x2, 0x88, 0x89, 0x3, 0x2, 0x2, 0x2, 
       0x89, 0x8a, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x8b, 0x7, 0x40, 0x2, 0x2, 
       0x8b, 0xb, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x8e, 0x5, 0x8, 0x5, 0x2, 0x8d, 
       0x8c, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x8f, 0x3, 0x2, 0x2, 0x2, 0x8f, 0x8d, 
       0x3, 0x2, 0x2, 0x2, 0x8f, 0x90, 0x3, 0x2, 0x2, 0x2, 0x90, 0xd, 0x3, 
       0x2, 0x2, 0x2, 0x91, 0x92, 0x7, 0x18, 0x2, 0x2, 0x92, 0x93, 0x5, 
       0x10, 0x9, 0x2, 0x93, 0x94, 0x7, 0x45, 0x2, 0x2, 0x94, 0x9e, 0x3, 
       0x2, 0x2, 0x2, 0x95, 0x96, 0x7, 0x1c, 0x2, 0x2, 0x96, 0x97, 0x5, 
       0x10, 0x9, 0x2, 0x97, 0x98, 0x7, 0x45, 0x2, 0x2, 0x98, 0x9e, 0x3, 
       0x2, 0x2, 0x2, 0x99, 0x9a, 0x7, 0x1d, 0x2, 0x2, 0x9a, 0x9b, 0x5, 
       0x10, 0x9, 0x2, 0x9b, 0x9c, 0x7, 0x45, 0x2, 0x2, 0x9c, 0x9e, 0x3, 
       0x2, 0x2, 0x2, 0x9d, 0x91, 0x3, 0x2, 0x2, 0x2, 0x9d, 0x95, 0x3, 0x2, 
       0x2, 0x2, 0x9d, 0x99, 0x3, 0x2, 0x2, 0x2, 0x9e, 0xf, 0x3, 0x2, 0x2, 
       0x2, 0x9f, 0xa4, 0x5, 0x12, 0xa, 0x2, 0xa0, 0xa1, 0x7, 0x46, 0x2, 
       0x2, 0xa1, 0xa3, 0x5, 0x12, 0xa, 0x2, 0xa2, 0xa0, 0x3, 0x2, 0x2, 
       0x2, 0xa3, 0xa6, 0x3, 0x2, 0x2, 0x2, 0xa4, 0xa2, 0x3, 0x2, 0x2, 0x2, 
       0xa4, 0xa5, 0x3, 0x2, 0x2, 0x2, 0xa5, 0x11, 0x3, 0x2, 0x2, 0x2, 0xa6, 
       0xa4, 0x3, 0x2, 0x2, 0x2, 0xa7, 0xaa, 0x7, 0x4c, 0x2, 0x2, 0xa8, 
       0xa9, 0x7, 0x1e, 0x2, 0x2, 0xa9, 0xab, 0x5, 0x32, 0x1a, 0x2, 0xaa, 
       0xa8, 0x3, 0x2, 0x2, 0x2, 0xaa, 0xab, 0x3, 0x2, 0x2, 0x2, 0xab, 0x13, 
       0x3, 0x2, 0x2, 0x2, 0xac, 0xad, 0x7, 0x45, 0x2, 0x2, 0xad, 0x15, 
       0x3, 0x2, 0x2, 0x2, 0xae, 0xaf, 0x5, 0x30, 0x19, 0x2, 0xaf, 0xb0, 
       0x7, 0x45, 0x2, 0x2, 0xb0, 0x17, 0x3, 0x2, 0x2, 0x2, 0xb1, 0xb2, 
       0x7, 0xe, 0x2, 0x2, 0xb2, 0xb3, 0x7, 0x41, 0x2, 0x2, 0xb3, 0xb4, 
       0x5, 0x30, 0x19, 0x2, 0xb4, 0xb5, 0x7, 0x42, 0x2, 0x2, 0xb5, 0xb8, 
       0x5, 0x8, 0x5, 0x2, 0xb6, 0xb7, 0x7, 0xa, 0x2, 0x2, 0xb7, 0xb9, 0x5, 
       0x8, 0x5, 0x2, 0xb8, 0xb6, 0x3, 0x2, 0x2, 0x2, 0xb8, 0xb9, 0x3, 0x2, 
       0x2, 0x2, 0xb9, 0x19, 0x3, 0x2, 0x2, 0x2, 0xba, 0xbb, 0x7, 0x1a, 
       0x2, 0x2, 0xbb, 0xbc, 0x7, 0x41, 0x2, 0x2, 0xbc, 0xbd, 0x5, 0x30, 
       0x19, 0x2, 0xbd, 0xbe, 0x7, 0x42, 0x2, 0x2, 0xbe, 0xbf, 0x5, 0x8, 
       0x5, 0x2, 0xbf, 0xd0, 0x3, 0x2, 0x2, 0x2, 0xc0, 0xc1, 0x7, 0xc, 0x2, 
       0x2, 0xc1, 0xc3, 0x7, 0x41, 0x2, 0x2, 0xc2, 0xc4, 0x5, 0x1c, 0xf, 
       0x2, 0xc3, 0xc2, 0x3, 0x2, 0x2, 0x2, 0xc3, 0xc4, 0x3, 0x2, 0x2, 0x2, 
       0xc4, 0xc5, 0x3, 0x2, 0x2, 0x2, 0xc5, 0xc7, 0x7, 0x45, 0x2, 0x2, 
       0xc6, 0xc8, 0x5, 0x30, 0x19, 0x2, 0xc7, 0xc6, 0x3, 0x2, 0x2, 0x2, 
       0xc7, 0xc8, 0x3, 0x2, 0x2, 0x2, 0xc8, 0xc9, 0x3, 0x2, 0x2, 0x2, 0xc9, 
       0xcb, 0x7, 0x45, 0x2, 0x2, 0xca, 0xcc, 0x5, 0x30, 0x19, 0x2, 0xcb, 
       0xca, 0x3, 0x2, 0x2, 0x2, 0xcb, 0xcc, 0x3, 0x2, 0x2, 0x2, 0xcc, 0xcd, 
       0x3, 0x2, 0x2, 0x2, 0xcd, 0xce, 0x7, 0x42, 0x2, 0x2, 0xce, 0xd0, 
       0x5, 0x8, 0x5, 0x2, 0xcf, 0xba, 0x3, 0x2, 0x2, 0x2, 0xcf, 0xc0, 0x3, 
       0x2, 0x2, 0x2, 0xd0, 0x1b, 0x3, 0x2, 0x2, 0x2, 0xd1, 0xd4, 0x5, 0x10, 
       0x9, 0x2, 0xd2, 0xd4, 0x5, 0x30, 0x19, 0x2, 0xd3, 0xd1, 0x3, 0x2, 
       0x2, 0x2, 0xd3, 0xd2, 0x3, 0x2, 0x2, 0x2, 0xd4, 0x1d, 0x3, 0x2, 0x2, 
       0x2, 0xd5, 0xd7, 0x7, 0x6, 0x2, 0x2, 0xd6, 0xd8, 0x7, 0x4c, 0x2, 
       0x2, 0xd7, 0xd6, 0x3, 0x2, 0x2, 0x2, 0xd7, 0xd8, 0x3, 0x2, 0x2, 0x2, 
       0xd8, 0xd9, 0x3, 0x2, 0x2, 0x2, 0xd9, 0xda, 0x7, 0x45, 0x2, 0x2, 
       0xda, 0x1f, 0x3, 0x2, 0x2, 0x2, 0xdb, 0xdd, 0x7, 0x3, 0x2, 0x2, 0xdc, 
       0xde, 0x7, 0x4c, 0x2, 0x2, 0xdd, 0xdc, 0x3, 0x2, 0x2, 0x2, 0xdd, 
       0xde, 0x3, 0x2, 0x2, 0x2, 0xde, 0xdf, 0x3, 0x2, 0x2, 0x2, 0xdf, 0xe0, 
       0x7, 0x45, 0x2, 0x2, 0xe0, 0x21, 0x3, 0x2, 0x2, 0x2, 0xe1, 0xe3, 
       0x7, 0x12, 0x2, 0x2, 0xe2, 0xe4, 0x5, 0x30, 0x19, 0x2, 0xe3, 0xe2, 
       0x3, 0x2, 0x2, 0x2, 0xe3, 0xe4, 0x3, 0x2, 0x2, 0x2, 0xe4, 0xe5, 0x3, 
       0x2, 0x2, 0x2, 0xe5, 0xe6, 0x7, 0x45, 0x2, 0x2, 0xe6, 0x23, 0x3, 
       0x2, 0x2, 0x2, 0xe7, 0xe8, 0x7, 0x16, 0x2, 0x2, 0xe8, 0xee, 0x5, 
       0xa, 0x6, 0x2, 0xe9, 0xeb, 0x5, 0x26, 0x14, 0x2, 0xea, 0xec, 0x5, 
       0x28, 0x15, 0x2, 0xeb, 0xea, 0x3, 0x2, 0x2, 0x2, 0xeb, 0xec, 0x3, 
       0x2, 0x2, 0x2, 0xec, 0xef, 0x3, 0x2, 0x2, 0x2, 0xed, 0xef, 0x5, 0x28, 
       0x15, 0x2, 0xee, 0xe9, 0x3, 0x2, 0x2, 0x2, 0xee, 0xed, 0x3, 0x2, 
       0x2, 0x2, 0xef, 0x25, 0x3, 0x2, 0x2, 0x2, 0xf0, 0xf1, 0x7, 0x5, 0x2, 
       0x2, 0xf1, 0xf2, 0x7, 0x41, 0x2, 0x2, 0xf2, 0xf3, 0x7, 0x4c, 0x2, 
       0x2, 0xf3, 0xf4, 0x7, 0x42, 0x2, 0x2, 0xf4, 0xf5, 0x5, 0xa, 0x6, 
       0x2, 0xf5, 0x27, 0x3, 0x2, 0x2, 0x2, 0xf6, 0xf7, 0x7, 0xb, 0x2, 0x2, 
       0xf7, 0xf8, 0x5, 0xa, 0x6, 0x2, 0xf8, 0x29, 0x3, 0x2, 0x2, 0x2, 0xf9, 
       0xfa, 0x7, 0xd, 0x2, 0x2, 0xfa, 0xfb, 0x7, 0x4c, 0x2, 0x2, 0xfb, 
       0xfd, 0x7, 0x41, 0x2, 0x2, 0xfc, 0xfe, 0x5, 0x2c, 0x17, 0x2, 0xfd, 
       0xfc, 0x3, 0x2, 0x2, 0x2, 0xfd, 0xfe, 0x3, 0x2, 0x2, 0x2, 0xfe, 0xff, 
       0x3, 0x2, 0x2, 0x2, 0xff, 0x100, 0x7, 0x42, 0x2, 0x2, 0x100, 0x101, 
       0x7, 0x3f, 0x2, 0x2, 0x101, 0x102, 0x5, 0x2e, 0x18, 0x2, 0x102, 0x103, 
       0x7, 0x40, 0x2, 0x2, 0x103, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x104, 0x109, 
       0x7, 0x4c, 0x2, 0x2, 0x105, 0x106, 0x7, 0x46, 0x2, 0x2, 0x106, 0x108, 
       0x7, 0x4c, 0x2, 0x2, 0x107, 0x105, 0x3, 0x2, 0x2, 0x2, 0x108, 0x10b, 
       0x3, 0x2, 0x2, 0x2, 0x109, 0x107, 0x3, 0x2, 0x2, 0x2, 0x109, 0x10a, 
       0x3, 0x2, 0x2, 0x2, 0x10a, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x10b, 0x109, 
       0x3, 0x2, 0x2, 0x2, 0x10c, 0x10e, 0x5, 0x4, 0x3, 0x2, 0x10d, 0x10c, 
       0x3, 0x2, 0x2, 0x2, 0x10d, 0x10e, 0x3, 0x2, 0x2, 0x2, 0x10e, 0x2f, 
       0x3, 0x2, 0x2, 0x2, 0x10f, 0x110, 0x5, 0x32, 0x1a, 0x2, 0x110, 0x31, 
       0x3, 0x2, 0x2, 0x2, 0x111, 0x117, 0x5, 0x36, 0x1c, 0x2, 0x112, 0x113, 
       0x5, 0x50, 0x29, 0x2, 0x113, 0x114, 0x5, 0x34, 0x1b, 0x2, 0x114, 
       0x115, 0x5, 0x32, 0x1a, 0x2, 0x115, 0x117, 0x3, 0x2, 0x2, 0x2, 0x116, 
       0x111, 0x3, 0x2, 0x2, 0x2, 0x116, 0x112, 0x3, 0x2, 0x2, 0x2, 0x117, 
       0x33, 0x3, 0x2, 0x2, 0x2, 0x118, 0x119, 0x9, 0x2, 0x2, 0x2, 0x119, 
       0x35, 0x3, 0x2, 0x2, 0x2, 0x11a, 0x120, 0x5, 0x38, 0x1d, 0x2, 0x11b, 
       0x11c, 0x7, 0x24, 0x2, 0x2, 0x11c, 0x11d, 0x5, 0x32, 0x1a, 0x2, 0x11d, 
       0x11e, 0x7, 0x25, 0x2, 0x2, 0x11e, 0x11f, 0x5, 0x32, 0x1a, 0x2, 0x11f, 
       0x121, 0x3, 0x2, 0x2, 0x2, 0x120, 0x11b, 0x3, 0x2, 0x2, 0x2, 0x120, 
       0x121, 0x3, 0x2, 0x2, 0x2, 0x121, 0x37, 0x3, 0x2, 0x2, 0x2, 0x122, 
       0x127, 0x5, 0x3a, 0x1e, 0x2, 0x123, 0x124, 0x7, 0x26, 0x2, 0x2, 0x124, 
       0x126, 0x5, 0x3a, 0x1e, 0x2, 0x125, 0x123, 0x3, 0x2, 0x2, 0x2, 0x126, 
       0x129, 0x3, 0x2, 0x2, 0x2, 0x127, 0x125, 0x3, 0x2, 0x2, 0x2, 0x127, 
       0x128, 0x3, 0x2, 0x2, 0x2, 0x128, 0x39, 0x3, 0x2, 0x2, 0x2, 0x129, 
       0x127, 0x3, 0x2, 0x2, 0x2, 0x12a, 0x12f, 0x5, 0x3c, 0x1f, 0x2, 0x12b, 
       0x12c, 0x7, 0x27, 0x2, 0x2, 0x12c, 0x12e, 0x5, 0x3c, 0x1f, 0x2, 0x12d, 
       0x12b, 0x3, 0x2, 0x2, 0x2, 0x12e, 0x131, 0x3, 0x2, 0x2, 0x2, 0x12f, 
       0x12d, 0x3, 0x2, 0x2, 0x2, 0x12f, 0x130, 0x3, 0x2, 0x2, 0x2, 0x130, 
       0x3b, 0x3, 0x2, 0x2, 0x2, 0x131, 0x12f, 0x3, 0x2, 0x2, 0x2, 0x132, 
       0x137, 0x5, 0x3e, 0x20, 0x2, 0x133, 0x134, 0x7, 0x28, 0x2, 0x2, 0x134, 
       0x136, 0x5, 0x3e, 0x20, 0x2, 0x135, 0x133, 0x3, 0x2, 0x2, 0x2, 0x136, 
       0x139, 0x3, 0x2, 0x2, 0x2, 0x137, 0x135, 0x3, 0x2, 0x2, 0x2, 0x137, 
       0x138, 0x3, 0x2, 0x2, 0x2, 0x138, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x139, 
       0x137, 0x3, 0x2, 0x2, 0x2, 0x13a, 0x13f, 0x5, 0x40, 0x21, 0x2, 0x13b, 
       0x13c, 0x7, 0x29, 0x2, 0x2, 0x13c, 0x13e, 0x5, 0x40, 0x21, 0x2, 0x13d, 
       0x13b, 0x3, 0x2, 0x2, 0x2, 0x13e, 0x141, 0x3, 0x2, 0x2, 0x2, 0x13f, 
       0x13d, 0x3, 0x2, 0x2, 0x2, 0x13f, 0x140, 0x3, 0x2, 0x2, 0x2, 0x140, 
       0x3f, 0x3, 0x2, 0x2, 0x2, 0x141, 0x13f, 0x3, 0x2, 0x2, 0x2, 0x142, 
       0x147, 0x5, 0x42, 0x22, 0x2, 0x143, 0x144, 0x7, 0x2a, 0x2, 0x2, 0x144, 
       0x146, 0x5, 0x42, 0x22, 0x2, 0x145, 0x143, 0x3, 0x2, 0x2, 0x2, 0x146, 
       0x149, 0x3, 0x2, 0x2, 0x2, 0x147, 0x145, 0x3, 0x2, 0x2, 0x2, 0x147, 
       0x148, 0x3, 0x2, 0x2, 0x2, 0x148, 0x41, 0x3, 0x2, 0x2, 0x2, 0x149, 
       0x147, 0x3, 0x2, 0x2, 0x2, 0x14a, 0x14f, 0x5, 0x44, 0x23, 0x2, 0x14b, 
       0x14c, 0x9, 0x3, 0x2, 0x2, 0x14c, 0x14e, 0x5, 0x44, 0x23, 0x2, 0x14d, 
       0x14b, 0x3, 0x2, 0x2, 0x2, 0x14e, 0x151, 0x3, 0x2, 0x2, 0x2, 0x14f, 
       0x14d, 0x3, 0x2, 0x2, 0x2, 0x14f, 0x150, 0x3, 0x2, 0x2, 0x2, 0x150, 
       0x43, 0x3, 0x2, 0x2, 0x2, 0x151, 0x14f, 0x3, 0x2, 0x2, 0x2, 0x152, 
       0x157, 0x5, 0x46, 0x24, 0x2, 0x153, 0x154, 0x9, 0x4, 0x2, 0x2, 0x154, 
       0x156, 0x5, 0x46, 0x24, 0x2, 0x155, 0x153, 0x3, 0x2, 0x2, 0x2, 0x156, 
       0x159, 0x3, 0x2, 0x2, 0x2, 0x157, 0x155, 0x3, 0x2, 0x2, 0x2, 0x157, 
       0x158, 0x3, 0x2, 0x2, 0x2, 0x158, 0x45, 0x3, 0x2, 0x2, 0x2, 0x159, 
       0x157, 0x3, 0x2, 0x2, 0x2, 0x15a, 0x15f, 0x5, 0x48, 0x25, 0x2, 0x15b, 
       0x15c, 0x9, 0x5, 0x2, 0x2, 0x15c, 0x15e, 0x5, 0x48, 0x25, 0x2, 0x15d, 
       0x15b, 0x3, 0x2, 0x2, 0x2, 0x15e, 0x161, 0x3, 0x2, 0x2, 0x2, 0x15f, 
       0x15d, 0x3, 0x2, 0x2, 0x2, 0x15f, 0x160, 0x3, 0x2, 0x2, 0x2, 0x160, 
       0x47, 0x3, 0x2, 0x2, 0x2, 0x161, 0x15f, 0x3, 0x2, 0x2, 0x2, 0x162, 
       0x167, 0x5, 0x4a, 0x26, 0x2, 0x163, 0x164, 0x9, 0x6, 0x2, 0x2, 0x164, 
       0x166, 0x5, 0x4a, 0x26, 0x2, 0x165, 0x163, 0x3, 0x2, 0x2, 0x2, 0x166, 
       0x169, 0x3, 0x2, 0x2, 0x2, 0x167, 0x165, 0x3, 0x2, 0x2, 0x2, 0x167, 
       0x168, 0x3, 0x2, 0x2, 0x2, 0x168, 0x49, 0x3, 0x2, 0x2, 0x2, 0x169, 
       0x167, 0x3, 0x2, 0x2, 0x2, 0x16a, 0x16f, 0x5, 0x4c, 0x27, 0x2, 0x16b, 
       0x16c, 0x9, 0x7, 0x2, 0x2, 0x16c, 0x16e, 0x5, 0x4c, 0x27, 0x2, 0x16d, 
       0x16b, 0x3, 0x2, 0x2, 0x2, 0x16e, 0x171, 0x3, 0x2, 0x2, 0x2, 0x16f, 
       0x16d, 0x3, 0x2, 0x2, 0x2, 0x16f, 0x170, 0x3, 0x2, 0x2, 0x2, 0x170, 
       0x4b, 0x3, 0x2, 0x2, 0x2, 0x171, 0x16f, 0x3, 0x2, 0x2, 0x2, 0x172, 
       0x176, 0x5, 0x4e, 0x28, 0x2, 0x173, 0x174, 0x9, 0x8, 0x2, 0x2, 0x174, 
       0x176, 0x5, 0x4c, 0x27, 0x2, 0x175, 0x172, 0x3, 0x2, 0x2, 0x2, 0x175, 
       0x173, 0x3, 0x2, 0x2, 0x2, 0x176, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x177, 
       0x179, 0x5, 0x50, 0x29, 0x2, 0x178, 0x17a, 0x9, 0x9, 0x2, 0x2, 0x179, 
       0x178, 0x3, 0x2, 0x2, 0x2, 0x179, 0x17a, 0x3, 0x2, 0x2, 0x2, 0x17a, 
       0x4f, 0x3, 0x2, 0x2, 0x2, 0x17b, 0x17e, 0x5, 0x52, 0x2a, 0x2, 0x17c, 
       0x17e, 0x5, 0x54, 0x2b, 0x2, 0x17d, 0x17b, 0x3, 0x2, 0x2, 0x2, 0x17d, 
       0x17c, 0x3, 0x2, 0x2, 0x2, 0x17e, 0x51, 0x3, 0x2, 0x2, 0x2, 0x17f, 
       0x180, 0x8, 0x2a, 0x1, 0x2, 0x180, 0x181, 0x5, 0x54, 0x2b, 0x2, 0x181, 
       0x182, 0x5, 0x64, 0x33, 0x2, 0x182, 0x18f, 0x3, 0x2, 0x2, 0x2, 0x183, 
       0x184, 0xc, 0x5, 0x2, 0x2, 0x184, 0x18e, 0x5, 0x64, 0x33, 0x2, 0x185, 
       0x186, 0xc, 0x4, 0x2, 0x2, 0x186, 0x187, 0x7, 0x43, 0x2, 0x2, 0x187, 
       0x188, 0x5, 0x30, 0x19, 0x2, 0x188, 0x189, 0x7, 0x44, 0x2, 0x2, 0x189, 
       0x18e, 0x3, 0x2, 0x2, 0x2, 0x18a, 0x18b, 0xc, 0x3, 0x2, 0x2, 0x18b, 
       0x18c, 0x7, 0x47, 0x2, 0x2, 0x18c, 0x18e, 0x7, 0x4c, 0x2, 0x2, 0x18d, 
       0x183, 0x3, 0x2, 0x2, 0x2, 0x18d, 0x185, 0x3, 0x2, 0x2, 0x2, 0x18d, 
       0x18a, 0x3, 0x2, 0x2, 0x2, 0x18e, 0x191, 0x3, 0x2, 0x2, 0x2, 0x18f, 
       0x18d, 0x3, 0x2, 0x2, 0x2, 0x18f, 0x190, 0x3, 0x2, 0x2, 0x2, 0x190, 
       0x53, 0x3, 0x2, 0x2, 0x2, 0x191, 0x18f, 0x3, 0x2, 0x2, 0x2, 0x192, 
       0x193, 0x8, 0x2b, 0x1, 0x2, 0x193, 0x19a, 0x5, 0x56, 0x2c, 0x2, 0x194, 
       0x19a, 0x5, 0x68, 0x35, 0x2, 0x195, 0x196, 0x7, 0x11, 0x2, 0x2, 0x196, 
       0x197, 0x5, 0x54, 0x2b, 0x2, 0x197, 0x198, 0x5, 0x64, 0x33, 0x2, 
       0x198, 0x19a, 0x3, 0x2, 0x2, 0x2, 0x199, 0x192, 0x3, 0x2, 0x2, 0x2, 
       0x199, 0x194, 0x3, 0x2, 0x2, 0x2, 0x199, 0x195, 0x3, 0x2, 0x2, 0x2, 
       0x19a, 0x1a5, 0x3, 0x2, 0x2, 0x2, 0x19b, 0x19c, 0xc, 0x5, 0x2, 0x2, 
       0x19c, 0x19d, 0x7, 0x43, 0x2, 0x2, 0x19d, 0x19e, 0x5, 0x30, 0x19, 
       0x2, 0x19e, 0x19f, 0x7, 0x44, 0x2, 0x2, 0x19f, 0x1a4, 0x3, 0x2, 0x2, 
       0x2, 0x1a0, 0x1a1, 0xc, 0x4, 0x2, 0x2, 0x1a1, 0x1a2, 0x7, 0x47, 0x2, 
       0x2, 0x1a2, 0x1a4, 0x7, 0x4c, 0x2, 0x2, 0x1a3, 0x19b, 0x3, 0x2, 0x2, 
       0x2, 0x1a3, 0x1a0, 0x3, 0x2, 0x2, 0x2, 0x1a4, 0x1a7, 0x3, 0x2, 0x2, 
       0x2, 0x1a5, 0x1a3, 0x3, 0x2, 0x2, 0x2, 0x1a5, 0x1a6, 0x3, 0x2, 0x2, 
       0x2, 0x1a6, 0x55, 0x3, 0x2, 0x2, 0x2, 0x1a7, 0x1a5, 0x3, 0x2, 0x2, 
       0x2, 0x1a8, 0x1b2, 0x7, 0x14, 0x2, 0x2, 0x1a9, 0x1b2, 0x7, 0x4c, 
       0x2, 0x2, 0x1aa, 0x1b2, 0x5, 0x6a, 0x36, 0x2, 0x1ab, 0x1b2, 0x5, 
       0x58, 0x2d, 0x2, 0x1ac, 0x1b2, 0x5, 0x5c, 0x2f, 0x2, 0x1ad, 0x1ae, 
       0x7, 0x41, 0x2, 0x2, 0x1ae, 0x1af, 0x5, 0x30, 0x19, 0x2, 0x1af, 0x1b0, 
       0x7, 0x42, 0x2, 0x2, 0x1b0, 0x1b2, 0x3, 0x2, 0x2, 0x2, 0x1b1, 0x1a8, 
       0x3, 0x2, 0x2, 0x2, 0x1b1, 0x1a9, 0x3, 0x2, 0x2, 0x2, 0x1b1, 0x1aa, 
       0x3, 0x2, 0x2, 0x2, 0x1b1, 0x1ab, 0x3, 0x2, 0x2, 0x2, 0x1b1, 0x1ac, 
       0x3, 0x2, 0x2, 0x2, 0x1b1, 0x1ad, 0x3, 0x2, 0x2, 0x2, 0x1b2, 0x57, 
       0x3, 0x2, 0x2, 0x2, 0x1b3, 0x1b5, 0x7, 0x43, 0x2, 0x2, 0x1b4, 0x1b6, 
       0x5, 0x5a, 0x2e, 0x2, 0x1b5, 0x1b4, 0x3, 0x2, 0x2, 0x2, 0x1b5, 0x1b6, 
       0x3, 0x2, 0x2, 0x2, 0x1b6, 0x1b7, 0x3, 0x2, 0x2, 0x2, 0x1b7, 0x1b8, 
       0x7, 0x44, 0x2, 0x2, 0x1b8, 0x59, 0x3, 0x2, 0x2, 0x2, 0x1b9, 0x1be, 
       0x5, 0x32, 0x1a, 0x2, 0x1ba, 0x1bb, 0x7, 0x46, 0x2, 0x2, 0x1bb, 0x1bd, 
       0x5, 0x32, 0x1a, 0x2, 0x1bc, 0x1ba, 0x3, 0x2, 0x2, 0x2, 0x1bd, 0x1c0, 
       0x3, 0x2, 0x2, 0x2, 0x1be, 0x1bc, 0x3, 0x2, 0x2, 0x2, 0x1be, 0x1bf, 
       0x3, 0x2, 0x2, 0x2, 0x1bf, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x1c0, 0x1be, 
       0x3, 0x2, 0x2, 0x2, 0x1c1, 0x1c3, 0x7, 0x3f, 0x2, 0x2, 0x1c2, 0x1c4, 
       0x5, 0x5e, 0x30, 0x2, 0x1c3, 0x1c2, 0x3, 0x2, 0x2, 0x2, 0x1c3, 0x1c4, 
       0x3, 0x2, 0x2, 0x2, 0x1c4, 0x1c5, 0x3, 0x2, 0x2, 0x2, 0x1c5, 0x1c6, 
       0x7, 0x40, 0x2, 0x2, 0x1c6, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x1c7, 0x1cc, 
       0x5, 0x60, 0x31, 0x2, 0x1c8, 0x1c9, 0x7, 0x46, 0x2, 0x2, 0x1c9, 0x1cb, 
       0x5, 0x60, 0x31, 0x2, 0x1ca, 0x1c8, 0x3, 0x2, 0x2, 0x2, 0x1cb, 0x1ce, 
       0x3, 0x2, 0x2, 0x2, 0x1cc, 0x1ca, 0x3, 0x2, 0x2, 0x2, 0x1cc, 0x1cd, 
       0x3, 0x2, 0x2, 0x2, 0x1cd, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x1ce, 0x1cc, 
       0x3, 0x2, 0x2, 0x2, 0x1cf, 0x1d0, 0x5, 0x62, 0x32, 0x2, 0x1d0, 0x1d1, 
       0x7, 0x25, 0x2, 0x2, 0x1d1, 0x1d2, 0x5, 0x32, 0x1a, 0x2, 0x1d2, 0x61, 
       0x3, 0x2, 0x2, 0x2, 0x1d3, 0x1d4, 0x9, 0xa, 0x2, 0x2, 0x1d4, 0x63, 
       0x3, 0x2, 0x2, 0x2, 0x1d5, 0x1d7, 0x7, 0x41, 0x2, 0x2, 0x1d6, 0x1d8, 
       0x5, 0x66, 0x34, 0x2, 0x1d7, 0x1d6, 0x3, 0x2, 0x2, 0x2, 0x1d7, 0x1d8, 
       0x3, 0x2, 0x2, 0x2, 0x1d8, 0x1d9, 0x3, 0x2, 0x2, 0x2, 0x1d9, 0x1da, 
       0x7, 0x42, 0x2, 0x2, 0x1da, 0x65, 0x3, 0x2, 0x2, 0x2, 0x1db, 0x1e0, 
       0x5, 0x32, 0x1a, 0x2, 0x1dc, 0x1dd, 0x7, 0x46, 0x2, 0x2, 0x1dd, 0x1df, 
       0x5, 0x32, 0x1a, 0x2, 0x1de, 0x1dc, 0x3, 0x2, 0x2, 0x2, 0x1df, 0x1e2, 
       0x3, 0x2, 0x2, 0x2, 0x1e0, 0x1de, 0x3, 0x2, 0x2, 0x2, 0x1e0, 0x1e1, 
       0x3, 0x2, 0x2, 0x2, 0x1e1, 0x67, 0x3, 0x2, 0x2, 0x2, 0x1e2, 0x1e0, 
       0x3, 0x2, 0x2, 0x2, 0x1e3, 0x1e5, 0x7, 0xd, 0x2, 0x2, 0x1e4, 0x1e6, 
       0x7, 0x4c, 0x2, 0x2, 0x1e5, 0x1e4, 0x3, 0x2, 0x2, 0x2, 0x1e5, 0x1e6, 
       0x3, 0x2, 0x2, 0x2, 0x1e6, 0x1e7, 0x3, 0x2, 0x2, 0x2, 0x1e7, 0x1e9, 
       0x7, 0x41, 0x2, 0x2, 0x1e8, 0x1ea, 0x5, 0x2c, 0x17, 0x2, 0x1e9, 0x1e8, 
       0x3, 0x2, 0x2, 0x2, 0x1e9, 0x1ea, 0x3, 0x2, 0x2, 0x2, 0x1ea, 0x1eb, 
       0x3, 0x2, 0x2, 0x2, 0x1eb, 0x1ec, 0x7, 0x42, 0x2, 0x2, 0x1ec, 0x1ed, 
       0x7, 0x3f, 0x2, 0x2, 0x1ed, 0x1ee, 0x5, 0x2e, 0x18, 0x2, 0x1ee, 0x1ef, 
       0x7, 0x40, 0x2, 0x2, 0x1ef, 0x69, 0x3, 0x2, 0x2, 0x2, 0x1f0, 0x1f1, 
       0x9, 0xb, 0x2, 0x2, 0x1f1, 0x6b, 0x3, 0x2, 0x2, 0x2, 0x36, 0x6d, 
       0x74, 0x78, 0x84, 0x88, 0x8f, 0x9d, 0xa4, 0xaa, 0xb8, 0xc3, 0xc7, 
       0xcb, 0xcf, 0xd3, 0xd7, 0xdd, 0xe3, 0xeb, 0xee, 0xfd, 0x109, 0x10d, 
       0x116, 0x120, 0x127, 0x12f, 0x137, 0x13f, 0x147, 0x14f, 0x157, 0x15f, 
       0x167, 0x16f, 0x175, 0x179, 0x17d, 0x18d, 0x18f, 0x199, 0x1a3, 0x1a5, 
       0x1b1, 0x1b5, 0x1be, 0x1c3, 0x1cc, 0x1d7, 0x1e0, 0x1e5, 0x1e9, 
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

JavaScriptParser::Initializer JavaScriptParser::_init;

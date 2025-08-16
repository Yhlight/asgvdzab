
// Generated from CSS3.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"
#include "CSS3Listener.h"


/**
 * This class provides an empty implementation of CSS3Listener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  CSS3BaseListener : public CSS3Listener {
public:

  virtual void enterStylesheet(CSS3Parser::StylesheetContext * /*ctx*/) override { }
  virtual void exitStylesheet(CSS3Parser::StylesheetContext * /*ctx*/) override { }

  virtual void enterStatement(CSS3Parser::StatementContext * /*ctx*/) override { }
  virtual void exitStatement(CSS3Parser::StatementContext * /*ctx*/) override { }

  virtual void enterAtRule(CSS3Parser::AtRuleContext * /*ctx*/) override { }
  virtual void exitAtRule(CSS3Parser::AtRuleContext * /*ctx*/) override { }

  virtual void enterBlock(CSS3Parser::BlockContext * /*ctx*/) override { }
  virtual void exitBlock(CSS3Parser::BlockContext * /*ctx*/) override { }

  virtual void enterRuleset(CSS3Parser::RulesetContext * /*ctx*/) override { }
  virtual void exitRuleset(CSS3Parser::RulesetContext * /*ctx*/) override { }

  virtual void enterSelector(CSS3Parser::SelectorContext * /*ctx*/) override { }
  virtual void exitSelector(CSS3Parser::SelectorContext * /*ctx*/) override { }

  virtual void enterCombinator(CSS3Parser::CombinatorContext * /*ctx*/) override { }
  virtual void exitCombinator(CSS3Parser::CombinatorContext * /*ctx*/) override { }

  virtual void enterSimpleSelector(CSS3Parser::SimpleSelectorContext * /*ctx*/) override { }
  virtual void exitSimpleSelector(CSS3Parser::SimpleSelectorContext * /*ctx*/) override { }

  virtual void enterElementName(CSS3Parser::ElementNameContext * /*ctx*/) override { }
  virtual void exitElementName(CSS3Parser::ElementNameContext * /*ctx*/) override { }

  virtual void enterDeclaration(CSS3Parser::DeclarationContext * /*ctx*/) override { }
  virtual void exitDeclaration(CSS3Parser::DeclarationContext * /*ctx*/) override { }

  virtual void enterPrio(CSS3Parser::PrioContext * /*ctx*/) override { }
  virtual void exitPrio(CSS3Parser::PrioContext * /*ctx*/) override { }

  virtual void enterExpr(CSS3Parser::ExprContext * /*ctx*/) override { }
  virtual void exitExpr(CSS3Parser::ExprContext * /*ctx*/) override { }

  virtual void enterTerm(CSS3Parser::TermContext * /*ctx*/) override { }
  virtual void exitTerm(CSS3Parser::TermContext * /*ctx*/) override { }

  virtual void enterFunction(CSS3Parser::FunctionContext * /*ctx*/) override { }
  virtual void exitFunction(CSS3Parser::FunctionContext * /*ctx*/) override { }

  virtual void enterUnicodeRange(CSS3Parser::UnicodeRangeContext * /*ctx*/) override { }
  virtual void exitUnicodeRange(CSS3Parser::UnicodeRangeContext * /*ctx*/) override { }

  virtual void enterHexcolor(CSS3Parser::HexcolorContext * /*ctx*/) override { }
  virtual void exitHexcolor(CSS3Parser::HexcolorContext * /*ctx*/) override { }

  virtual void enterCssOperator(CSS3Parser::CssOperatorContext * /*ctx*/) override { }
  virtual void exitCssOperator(CSS3Parser::CssOperatorContext * /*ctx*/) override { }

  virtual void enterUnaryOperator(CSS3Parser::UnaryOperatorContext * /*ctx*/) override { }
  virtual void exitUnaryOperator(CSS3Parser::UnaryOperatorContext * /*ctx*/) override { }

  virtual void enterProperty(CSS3Parser::PropertyContext * /*ctx*/) override { }
  virtual void exitProperty(CSS3Parser::PropertyContext * /*ctx*/) override { }

  virtual void enterAny(CSS3Parser::AnyContext * /*ctx*/) override { }
  virtual void exitAny(CSS3Parser::AnyContext * /*ctx*/) override { }

  virtual void enterComment(CSS3Parser::CommentContext * /*ctx*/) override { }
  virtual void exitComment(CSS3Parser::CommentContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};


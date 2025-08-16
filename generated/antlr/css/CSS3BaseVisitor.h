
// Generated from grammars/CSS3.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "CSS3Visitor.h"


/**
 * This class provides an empty implementation of CSS3Visitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  CSS3BaseVisitor : public CSS3Visitor {
public:

  virtual std::any visitStylesheet(CSS3Parser::StylesheetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatement(CSS3Parser::StatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAtRule(CSS3Parser::AtRuleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlock(CSS3Parser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRuleset(CSS3Parser::RulesetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelector(CSS3Parser::SelectorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCombinator(CSS3Parser::CombinatorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSimpleSelector(CSS3Parser::SimpleSelectorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitElementName(CSS3Parser::ElementNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclaration(CSS3Parser::DeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrio(CSS3Parser::PrioContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpr(CSS3Parser::ExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTerm(CSS3Parser::TermContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunction(CSS3Parser::FunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnicodeRange(CSS3Parser::UnicodeRangeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitHexcolor(CSS3Parser::HexcolorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCssOperator(CSS3Parser::CssOperatorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnaryOperator(CSS3Parser::UnaryOperatorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitProperty(CSS3Parser::PropertyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAny(CSS3Parser::AnyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitComment(CSS3Parser::CommentContext *ctx) override {
    return visitChildren(ctx);
  }


};


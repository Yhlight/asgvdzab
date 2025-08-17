
// Generated from grammar/SimpleCSS.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "SimpleCSSVisitor.h"


/**
 * This class provides an empty implementation of SimpleCSSVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  SimpleCSSBaseVisitor : public SimpleCSSVisitor {
public:

  virtual std::any visitStylesheet(SimpleCSSParser::StylesheetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatement(SimpleCSSParser::StatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRuleSet(SimpleCSSParser::RuleSetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelector(SimpleCSSParser::SelectorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSimpleSelector(SimpleCSSParser::SimpleSelectorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitElementName(SimpleCSSParser::ElementNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitClassName(SimpleCSSParser::ClassNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIdName(SimpleCSSParser::IdNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPseudoClass(SimpleCSSParser::PseudoClassContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPseudoElement(SimpleCSSParser::PseudoElementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttribute(SimpleCSSParser::AttributeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttributeOperator(SimpleCSSParser::AttributeOperatorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclaration(SimpleCSSParser::DeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitProperty(SimpleCSSParser::PropertyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValue(SimpleCSSParser::ValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTerm(SimpleCSSParser::TermContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnit(SimpleCSSParser::UnitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunction(SimpleCSSParser::FunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpression(SimpleCSSParser::ExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAtRule(SimpleCSSParser::AtRuleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlock(SimpleCSSParser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAny(SimpleCSSParser::AnyContext *ctx) override {
    return visitChildren(ctx);
  }


};



// Generated from grammar/SimpleCSS.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "SimpleCSSParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by SimpleCSSParser.
 */
class  SimpleCSSVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by SimpleCSSParser.
   */
    virtual std::any visitStylesheet(SimpleCSSParser::StylesheetContext *context) = 0;

    virtual std::any visitStatement(SimpleCSSParser::StatementContext *context) = 0;

    virtual std::any visitRuleSet(SimpleCSSParser::RuleSetContext *context) = 0;

    virtual std::any visitSelector(SimpleCSSParser::SelectorContext *context) = 0;

    virtual std::any visitSimpleSelector(SimpleCSSParser::SimpleSelectorContext *context) = 0;

    virtual std::any visitElementName(SimpleCSSParser::ElementNameContext *context) = 0;

    virtual std::any visitClassName(SimpleCSSParser::ClassNameContext *context) = 0;

    virtual std::any visitIdName(SimpleCSSParser::IdNameContext *context) = 0;

    virtual std::any visitPseudoClass(SimpleCSSParser::PseudoClassContext *context) = 0;

    virtual std::any visitPseudoElement(SimpleCSSParser::PseudoElementContext *context) = 0;

    virtual std::any visitAttribute(SimpleCSSParser::AttributeContext *context) = 0;

    virtual std::any visitAttributeOperator(SimpleCSSParser::AttributeOperatorContext *context) = 0;

    virtual std::any visitDeclaration(SimpleCSSParser::DeclarationContext *context) = 0;

    virtual std::any visitProperty(SimpleCSSParser::PropertyContext *context) = 0;

    virtual std::any visitValue(SimpleCSSParser::ValueContext *context) = 0;

    virtual std::any visitTerm(SimpleCSSParser::TermContext *context) = 0;

    virtual std::any visitUnit(SimpleCSSParser::UnitContext *context) = 0;

    virtual std::any visitFunction(SimpleCSSParser::FunctionContext *context) = 0;

    virtual std::any visitExpression(SimpleCSSParser::ExpressionContext *context) = 0;

    virtual std::any visitAtRule(SimpleCSSParser::AtRuleContext *context) = 0;

    virtual std::any visitBlock(SimpleCSSParser::BlockContext *context) = 0;

    virtual std::any visitAny(SimpleCSSParser::AnyContext *context) = 0;


};


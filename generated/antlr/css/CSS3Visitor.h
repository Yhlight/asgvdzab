
// Generated from grammars/CSS3.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "CSS3Parser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by CSS3Parser.
 */
class  CSS3Visitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by CSS3Parser.
   */
    virtual std::any visitStylesheet(CSS3Parser::StylesheetContext *context) = 0;

    virtual std::any visitStatement(CSS3Parser::StatementContext *context) = 0;

    virtual std::any visitAtRule(CSS3Parser::AtRuleContext *context) = 0;

    virtual std::any visitBlock(CSS3Parser::BlockContext *context) = 0;

    virtual std::any visitRuleset(CSS3Parser::RulesetContext *context) = 0;

    virtual std::any visitSelector(CSS3Parser::SelectorContext *context) = 0;

    virtual std::any visitCombinator(CSS3Parser::CombinatorContext *context) = 0;

    virtual std::any visitSimpleSelector(CSS3Parser::SimpleSelectorContext *context) = 0;

    virtual std::any visitElementName(CSS3Parser::ElementNameContext *context) = 0;

    virtual std::any visitDeclaration(CSS3Parser::DeclarationContext *context) = 0;

    virtual std::any visitPrio(CSS3Parser::PrioContext *context) = 0;

    virtual std::any visitExpr(CSS3Parser::ExprContext *context) = 0;

    virtual std::any visitTerm(CSS3Parser::TermContext *context) = 0;

    virtual std::any visitFunction(CSS3Parser::FunctionContext *context) = 0;

    virtual std::any visitUnicodeRange(CSS3Parser::UnicodeRangeContext *context) = 0;

    virtual std::any visitHexcolor(CSS3Parser::HexcolorContext *context) = 0;

    virtual std::any visitCssOperator(CSS3Parser::CssOperatorContext *context) = 0;

    virtual std::any visitUnaryOperator(CSS3Parser::UnaryOperatorContext *context) = 0;

    virtual std::any visitProperty(CSS3Parser::PropertyContext *context) = 0;

    virtual std::any visitAny(CSS3Parser::AnyContext *context) = 0;

    virtual std::any visitComment(CSS3Parser::CommentContext *context) = 0;


};


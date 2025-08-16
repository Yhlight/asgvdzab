
// Generated from grammars/JavaScript.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "JavaScriptParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by JavaScriptParser.
 */
class  JavaScriptVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by JavaScriptParser.
   */
    virtual std::any visitProgram(JavaScriptParser::ProgramContext *context) = 0;

    virtual std::any visitSourceElements(JavaScriptParser::SourceElementsContext *context) = 0;

    virtual std::any visitSourceElement(JavaScriptParser::SourceElementContext *context) = 0;

    virtual std::any visitStatement(JavaScriptParser::StatementContext *context) = 0;

    virtual std::any visitBlock(JavaScriptParser::BlockContext *context) = 0;

    virtual std::any visitStatementList(JavaScriptParser::StatementListContext *context) = 0;

    virtual std::any visitVariableStatement(JavaScriptParser::VariableStatementContext *context) = 0;

    virtual std::any visitVariableDeclarationList(JavaScriptParser::VariableDeclarationListContext *context) = 0;

    virtual std::any visitVariableDeclaration(JavaScriptParser::VariableDeclarationContext *context) = 0;

    virtual std::any visitEmptyStatement(JavaScriptParser::EmptyStatementContext *context) = 0;

    virtual std::any visitExpressionStatement(JavaScriptParser::ExpressionStatementContext *context) = 0;

    virtual std::any visitIfStatement(JavaScriptParser::IfStatementContext *context) = 0;

    virtual std::any visitWhileStatement(JavaScriptParser::WhileStatementContext *context) = 0;

    virtual std::any visitForStatement(JavaScriptParser::ForStatementContext *context) = 0;

    virtual std::any visitForInit(JavaScriptParser::ForInitContext *context) = 0;

    virtual std::any visitContinueStatement(JavaScriptParser::ContinueStatementContext *context) = 0;

    virtual std::any visitBreakStatement(JavaScriptParser::BreakStatementContext *context) = 0;

    virtual std::any visitReturnStatement(JavaScriptParser::ReturnStatementContext *context) = 0;

    virtual std::any visitTryStatement(JavaScriptParser::TryStatementContext *context) = 0;

    virtual std::any visitCatchClause(JavaScriptParser::CatchClauseContext *context) = 0;

    virtual std::any visitFinallyClause(JavaScriptParser::FinallyClauseContext *context) = 0;

    virtual std::any visitFunctionDeclaration(JavaScriptParser::FunctionDeclarationContext *context) = 0;

    virtual std::any visitFormalParameterList(JavaScriptParser::FormalParameterListContext *context) = 0;

    virtual std::any visitFunctionBody(JavaScriptParser::FunctionBodyContext *context) = 0;

    virtual std::any visitExpression(JavaScriptParser::ExpressionContext *context) = 0;

    virtual std::any visitAssignmentExpression(JavaScriptParser::AssignmentExpressionContext *context) = 0;

    virtual std::any visitAssignmentOperator(JavaScriptParser::AssignmentOperatorContext *context) = 0;

    virtual std::any visitConditionalExpression(JavaScriptParser::ConditionalExpressionContext *context) = 0;

    virtual std::any visitLogicalORExpression(JavaScriptParser::LogicalORExpressionContext *context) = 0;

    virtual std::any visitLogicalANDExpression(JavaScriptParser::LogicalANDExpressionContext *context) = 0;

    virtual std::any visitBitwiseORExpression(JavaScriptParser::BitwiseORExpressionContext *context) = 0;

    virtual std::any visitBitwiseXORExpression(JavaScriptParser::BitwiseXORExpressionContext *context) = 0;

    virtual std::any visitBitwiseANDExpression(JavaScriptParser::BitwiseANDExpressionContext *context) = 0;

    virtual std::any visitEqualityExpression(JavaScriptParser::EqualityExpressionContext *context) = 0;

    virtual std::any visitRelationalExpression(JavaScriptParser::RelationalExpressionContext *context) = 0;

    virtual std::any visitShiftExpression(JavaScriptParser::ShiftExpressionContext *context) = 0;

    virtual std::any visitAdditiveExpression(JavaScriptParser::AdditiveExpressionContext *context) = 0;

    virtual std::any visitMultiplicativeExpression(JavaScriptParser::MultiplicativeExpressionContext *context) = 0;

    virtual std::any visitUnaryExpression(JavaScriptParser::UnaryExpressionContext *context) = 0;

    virtual std::any visitPostfixExpression(JavaScriptParser::PostfixExpressionContext *context) = 0;

    virtual std::any visitLeftHandSideExpression(JavaScriptParser::LeftHandSideExpressionContext *context) = 0;

    virtual std::any visitCallExpression(JavaScriptParser::CallExpressionContext *context) = 0;

    virtual std::any visitMemberExpression(JavaScriptParser::MemberExpressionContext *context) = 0;

    virtual std::any visitPrimaryExpression(JavaScriptParser::PrimaryExpressionContext *context) = 0;

    virtual std::any visitArrayLiteral(JavaScriptParser::ArrayLiteralContext *context) = 0;

    virtual std::any visitElementList(JavaScriptParser::ElementListContext *context) = 0;

    virtual std::any visitObjectLiteral(JavaScriptParser::ObjectLiteralContext *context) = 0;

    virtual std::any visitPropertyNameAndValueList(JavaScriptParser::PropertyNameAndValueListContext *context) = 0;

    virtual std::any visitPropertyAssignment(JavaScriptParser::PropertyAssignmentContext *context) = 0;

    virtual std::any visitPropertyName(JavaScriptParser::PropertyNameContext *context) = 0;

    virtual std::any visitArguments(JavaScriptParser::ArgumentsContext *context) = 0;

    virtual std::any visitArgumentList(JavaScriptParser::ArgumentListContext *context) = 0;

    virtual std::any visitFunctionExpression(JavaScriptParser::FunctionExpressionContext *context) = 0;

    virtual std::any visitLiteral(JavaScriptParser::LiteralContext *context) = 0;


};



// Generated from grammars/JavaScript.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "JavaScriptVisitor.h"


/**
 * This class provides an empty implementation of JavaScriptVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  JavaScriptBaseVisitor : public JavaScriptVisitor {
public:

  virtual std::any visitProgram(JavaScriptParser::ProgramContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSourceElements(JavaScriptParser::SourceElementsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSourceElement(JavaScriptParser::SourceElementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatement(JavaScriptParser::StatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlock(JavaScriptParser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatementList(JavaScriptParser::StatementListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVariableStatement(JavaScriptParser::VariableStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVariableDeclarationList(JavaScriptParser::VariableDeclarationListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVariableDeclaration(JavaScriptParser::VariableDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEmptyStatement(JavaScriptParser::EmptyStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpressionStatement(JavaScriptParser::ExpressionStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIfStatement(JavaScriptParser::IfStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitWhileStatement(JavaScriptParser::WhileStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitForStatement(JavaScriptParser::ForStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitForInit(JavaScriptParser::ForInitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitContinueStatement(JavaScriptParser::ContinueStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBreakStatement(JavaScriptParser::BreakStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnStatement(JavaScriptParser::ReturnStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTryStatement(JavaScriptParser::TryStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCatchClause(JavaScriptParser::CatchClauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFinallyClause(JavaScriptParser::FinallyClauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionDeclaration(JavaScriptParser::FunctionDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFormalParameterList(JavaScriptParser::FormalParameterListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionBody(JavaScriptParser::FunctionBodyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpression(JavaScriptParser::ExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssignmentExpression(JavaScriptParser::AssignmentExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssignmentOperator(JavaScriptParser::AssignmentOperatorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConditionalExpression(JavaScriptParser::ConditionalExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLogicalORExpression(JavaScriptParser::LogicalORExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLogicalANDExpression(JavaScriptParser::LogicalANDExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBitwiseORExpression(JavaScriptParser::BitwiseORExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBitwiseXORExpression(JavaScriptParser::BitwiseXORExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBitwiseANDExpression(JavaScriptParser::BitwiseANDExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEqualityExpression(JavaScriptParser::EqualityExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelationalExpression(JavaScriptParser::RelationalExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitShiftExpression(JavaScriptParser::ShiftExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAdditiveExpression(JavaScriptParser::AdditiveExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMultiplicativeExpression(JavaScriptParser::MultiplicativeExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnaryExpression(JavaScriptParser::UnaryExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPostfixExpression(JavaScriptParser::PostfixExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLeftHandSideExpression(JavaScriptParser::LeftHandSideExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCallExpression(JavaScriptParser::CallExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMemberExpression(JavaScriptParser::MemberExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrimaryExpression(JavaScriptParser::PrimaryExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArrayLiteral(JavaScriptParser::ArrayLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitElementList(JavaScriptParser::ElementListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitObjectLiteral(JavaScriptParser::ObjectLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPropertyNameAndValueList(JavaScriptParser::PropertyNameAndValueListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPropertyAssignment(JavaScriptParser::PropertyAssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPropertyName(JavaScriptParser::PropertyNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArguments(JavaScriptParser::ArgumentsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArgumentList(JavaScriptParser::ArgumentListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionExpression(JavaScriptParser::FunctionExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLiteral(JavaScriptParser::LiteralContext *ctx) override {
    return visitChildren(ctx);
  }


};


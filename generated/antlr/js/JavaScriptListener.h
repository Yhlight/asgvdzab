
// Generated from grammars/JavaScript.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "JavaScriptParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by JavaScriptParser.
 */
class  JavaScriptListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterProgram(JavaScriptParser::ProgramContext *ctx) = 0;
  virtual void exitProgram(JavaScriptParser::ProgramContext *ctx) = 0;

  virtual void enterSourceElements(JavaScriptParser::SourceElementsContext *ctx) = 0;
  virtual void exitSourceElements(JavaScriptParser::SourceElementsContext *ctx) = 0;

  virtual void enterSourceElement(JavaScriptParser::SourceElementContext *ctx) = 0;
  virtual void exitSourceElement(JavaScriptParser::SourceElementContext *ctx) = 0;

  virtual void enterStatement(JavaScriptParser::StatementContext *ctx) = 0;
  virtual void exitStatement(JavaScriptParser::StatementContext *ctx) = 0;

  virtual void enterBlock(JavaScriptParser::BlockContext *ctx) = 0;
  virtual void exitBlock(JavaScriptParser::BlockContext *ctx) = 0;

  virtual void enterStatementList(JavaScriptParser::StatementListContext *ctx) = 0;
  virtual void exitStatementList(JavaScriptParser::StatementListContext *ctx) = 0;

  virtual void enterVariableStatement(JavaScriptParser::VariableStatementContext *ctx) = 0;
  virtual void exitVariableStatement(JavaScriptParser::VariableStatementContext *ctx) = 0;

  virtual void enterVariableDeclarationList(JavaScriptParser::VariableDeclarationListContext *ctx) = 0;
  virtual void exitVariableDeclarationList(JavaScriptParser::VariableDeclarationListContext *ctx) = 0;

  virtual void enterVariableDeclaration(JavaScriptParser::VariableDeclarationContext *ctx) = 0;
  virtual void exitVariableDeclaration(JavaScriptParser::VariableDeclarationContext *ctx) = 0;

  virtual void enterEmptyStatement(JavaScriptParser::EmptyStatementContext *ctx) = 0;
  virtual void exitEmptyStatement(JavaScriptParser::EmptyStatementContext *ctx) = 0;

  virtual void enterExpressionStatement(JavaScriptParser::ExpressionStatementContext *ctx) = 0;
  virtual void exitExpressionStatement(JavaScriptParser::ExpressionStatementContext *ctx) = 0;

  virtual void enterIfStatement(JavaScriptParser::IfStatementContext *ctx) = 0;
  virtual void exitIfStatement(JavaScriptParser::IfStatementContext *ctx) = 0;

  virtual void enterWhileStatement(JavaScriptParser::WhileStatementContext *ctx) = 0;
  virtual void exitWhileStatement(JavaScriptParser::WhileStatementContext *ctx) = 0;

  virtual void enterForStatement(JavaScriptParser::ForStatementContext *ctx) = 0;
  virtual void exitForStatement(JavaScriptParser::ForStatementContext *ctx) = 0;

  virtual void enterForInit(JavaScriptParser::ForInitContext *ctx) = 0;
  virtual void exitForInit(JavaScriptParser::ForInitContext *ctx) = 0;

  virtual void enterContinueStatement(JavaScriptParser::ContinueStatementContext *ctx) = 0;
  virtual void exitContinueStatement(JavaScriptParser::ContinueStatementContext *ctx) = 0;

  virtual void enterBreakStatement(JavaScriptParser::BreakStatementContext *ctx) = 0;
  virtual void exitBreakStatement(JavaScriptParser::BreakStatementContext *ctx) = 0;

  virtual void enterReturnStatement(JavaScriptParser::ReturnStatementContext *ctx) = 0;
  virtual void exitReturnStatement(JavaScriptParser::ReturnStatementContext *ctx) = 0;

  virtual void enterTryStatement(JavaScriptParser::TryStatementContext *ctx) = 0;
  virtual void exitTryStatement(JavaScriptParser::TryStatementContext *ctx) = 0;

  virtual void enterCatchClause(JavaScriptParser::CatchClauseContext *ctx) = 0;
  virtual void exitCatchClause(JavaScriptParser::CatchClauseContext *ctx) = 0;

  virtual void enterFinallyClause(JavaScriptParser::FinallyClauseContext *ctx) = 0;
  virtual void exitFinallyClause(JavaScriptParser::FinallyClauseContext *ctx) = 0;

  virtual void enterFunctionDeclaration(JavaScriptParser::FunctionDeclarationContext *ctx) = 0;
  virtual void exitFunctionDeclaration(JavaScriptParser::FunctionDeclarationContext *ctx) = 0;

  virtual void enterFormalParameterList(JavaScriptParser::FormalParameterListContext *ctx) = 0;
  virtual void exitFormalParameterList(JavaScriptParser::FormalParameterListContext *ctx) = 0;

  virtual void enterFunctionBody(JavaScriptParser::FunctionBodyContext *ctx) = 0;
  virtual void exitFunctionBody(JavaScriptParser::FunctionBodyContext *ctx) = 0;

  virtual void enterExpression(JavaScriptParser::ExpressionContext *ctx) = 0;
  virtual void exitExpression(JavaScriptParser::ExpressionContext *ctx) = 0;

  virtual void enterAssignmentExpression(JavaScriptParser::AssignmentExpressionContext *ctx) = 0;
  virtual void exitAssignmentExpression(JavaScriptParser::AssignmentExpressionContext *ctx) = 0;

  virtual void enterAssignmentOperator(JavaScriptParser::AssignmentOperatorContext *ctx) = 0;
  virtual void exitAssignmentOperator(JavaScriptParser::AssignmentOperatorContext *ctx) = 0;

  virtual void enterConditionalExpression(JavaScriptParser::ConditionalExpressionContext *ctx) = 0;
  virtual void exitConditionalExpression(JavaScriptParser::ConditionalExpressionContext *ctx) = 0;

  virtual void enterLogicalORExpression(JavaScriptParser::LogicalORExpressionContext *ctx) = 0;
  virtual void exitLogicalORExpression(JavaScriptParser::LogicalORExpressionContext *ctx) = 0;

  virtual void enterLogicalANDExpression(JavaScriptParser::LogicalANDExpressionContext *ctx) = 0;
  virtual void exitLogicalANDExpression(JavaScriptParser::LogicalANDExpressionContext *ctx) = 0;

  virtual void enterBitwiseORExpression(JavaScriptParser::BitwiseORExpressionContext *ctx) = 0;
  virtual void exitBitwiseORExpression(JavaScriptParser::BitwiseORExpressionContext *ctx) = 0;

  virtual void enterBitwiseXORExpression(JavaScriptParser::BitwiseXORExpressionContext *ctx) = 0;
  virtual void exitBitwiseXORExpression(JavaScriptParser::BitwiseXORExpressionContext *ctx) = 0;

  virtual void enterBitwiseANDExpression(JavaScriptParser::BitwiseANDExpressionContext *ctx) = 0;
  virtual void exitBitwiseANDExpression(JavaScriptParser::BitwiseANDExpressionContext *ctx) = 0;

  virtual void enterEqualityExpression(JavaScriptParser::EqualityExpressionContext *ctx) = 0;
  virtual void exitEqualityExpression(JavaScriptParser::EqualityExpressionContext *ctx) = 0;

  virtual void enterRelationalExpression(JavaScriptParser::RelationalExpressionContext *ctx) = 0;
  virtual void exitRelationalExpression(JavaScriptParser::RelationalExpressionContext *ctx) = 0;

  virtual void enterShiftExpression(JavaScriptParser::ShiftExpressionContext *ctx) = 0;
  virtual void exitShiftExpression(JavaScriptParser::ShiftExpressionContext *ctx) = 0;

  virtual void enterAdditiveExpression(JavaScriptParser::AdditiveExpressionContext *ctx) = 0;
  virtual void exitAdditiveExpression(JavaScriptParser::AdditiveExpressionContext *ctx) = 0;

  virtual void enterMultiplicativeExpression(JavaScriptParser::MultiplicativeExpressionContext *ctx) = 0;
  virtual void exitMultiplicativeExpression(JavaScriptParser::MultiplicativeExpressionContext *ctx) = 0;

  virtual void enterUnaryExpression(JavaScriptParser::UnaryExpressionContext *ctx) = 0;
  virtual void exitUnaryExpression(JavaScriptParser::UnaryExpressionContext *ctx) = 0;

  virtual void enterPostfixExpression(JavaScriptParser::PostfixExpressionContext *ctx) = 0;
  virtual void exitPostfixExpression(JavaScriptParser::PostfixExpressionContext *ctx) = 0;

  virtual void enterLeftHandSideExpression(JavaScriptParser::LeftHandSideExpressionContext *ctx) = 0;
  virtual void exitLeftHandSideExpression(JavaScriptParser::LeftHandSideExpressionContext *ctx) = 0;

  virtual void enterCallExpression(JavaScriptParser::CallExpressionContext *ctx) = 0;
  virtual void exitCallExpression(JavaScriptParser::CallExpressionContext *ctx) = 0;

  virtual void enterMemberExpression(JavaScriptParser::MemberExpressionContext *ctx) = 0;
  virtual void exitMemberExpression(JavaScriptParser::MemberExpressionContext *ctx) = 0;

  virtual void enterPrimaryExpression(JavaScriptParser::PrimaryExpressionContext *ctx) = 0;
  virtual void exitPrimaryExpression(JavaScriptParser::PrimaryExpressionContext *ctx) = 0;

  virtual void enterArrayLiteral(JavaScriptParser::ArrayLiteralContext *ctx) = 0;
  virtual void exitArrayLiteral(JavaScriptParser::ArrayLiteralContext *ctx) = 0;

  virtual void enterElementList(JavaScriptParser::ElementListContext *ctx) = 0;
  virtual void exitElementList(JavaScriptParser::ElementListContext *ctx) = 0;

  virtual void enterObjectLiteral(JavaScriptParser::ObjectLiteralContext *ctx) = 0;
  virtual void exitObjectLiteral(JavaScriptParser::ObjectLiteralContext *ctx) = 0;

  virtual void enterPropertyNameAndValueList(JavaScriptParser::PropertyNameAndValueListContext *ctx) = 0;
  virtual void exitPropertyNameAndValueList(JavaScriptParser::PropertyNameAndValueListContext *ctx) = 0;

  virtual void enterPropertyAssignment(JavaScriptParser::PropertyAssignmentContext *ctx) = 0;
  virtual void exitPropertyAssignment(JavaScriptParser::PropertyAssignmentContext *ctx) = 0;

  virtual void enterPropertyName(JavaScriptParser::PropertyNameContext *ctx) = 0;
  virtual void exitPropertyName(JavaScriptParser::PropertyNameContext *ctx) = 0;

  virtual void enterArguments(JavaScriptParser::ArgumentsContext *ctx) = 0;
  virtual void exitArguments(JavaScriptParser::ArgumentsContext *ctx) = 0;

  virtual void enterArgumentList(JavaScriptParser::ArgumentListContext *ctx) = 0;
  virtual void exitArgumentList(JavaScriptParser::ArgumentListContext *ctx) = 0;

  virtual void enterFunctionExpression(JavaScriptParser::FunctionExpressionContext *ctx) = 0;
  virtual void exitFunctionExpression(JavaScriptParser::FunctionExpressionContext *ctx) = 0;

  virtual void enterLiteral(JavaScriptParser::LiteralContext *ctx) = 0;
  virtual void exitLiteral(JavaScriptParser::LiteralContext *ctx) = 0;


};


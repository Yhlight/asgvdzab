
// Generated from grammar/ECMAScript.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "ECMAScriptParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by ECMAScriptParser.
 */
class  ECMAScriptListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterProgram(ECMAScriptParser::ProgramContext *ctx) = 0;
  virtual void exitProgram(ECMAScriptParser::ProgramContext *ctx) = 0;

  virtual void enterSourceElements(ECMAScriptParser::SourceElementsContext *ctx) = 0;
  virtual void exitSourceElements(ECMAScriptParser::SourceElementsContext *ctx) = 0;

  virtual void enterSourceElement(ECMAScriptParser::SourceElementContext *ctx) = 0;
  virtual void exitSourceElement(ECMAScriptParser::SourceElementContext *ctx) = 0;

  virtual void enterChtlJsElement(ECMAScriptParser::ChtlJsElementContext *ctx) = 0;
  virtual void exitChtlJsElement(ECMAScriptParser::ChtlJsElementContext *ctx) = 0;

  virtual void enterChtlEnhancedSelector(ECMAScriptParser::ChtlEnhancedSelectorContext *ctx) = 0;
  virtual void exitChtlEnhancedSelector(ECMAScriptParser::ChtlEnhancedSelectorContext *ctx) = 0;

  virtual void enterSelectorExpression(ECMAScriptParser::SelectorExpressionContext *ctx) = 0;
  virtual void exitSelectorExpression(ECMAScriptParser::SelectorExpressionContext *ctx) = 0;

  virtual void enterChtlArrowAccess(ECMAScriptParser::ChtlArrowAccessContext *ctx) = 0;
  virtual void exitChtlArrowAccess(ECMAScriptParser::ChtlArrowAccessContext *ctx) = 0;

  virtual void enterChtlListenFunction(ECMAScriptParser::ChtlListenFunctionContext *ctx) = 0;
  virtual void exitChtlListenFunction(ECMAScriptParser::ChtlListenFunctionContext *ctx) = 0;

  virtual void enterChtlDelegateFunction(ECMAScriptParser::ChtlDelegateFunctionContext *ctx) = 0;
  virtual void exitChtlDelegateFunction(ECMAScriptParser::ChtlDelegateFunctionContext *ctx) = 0;

  virtual void enterChtlAnimateFunction(ECMAScriptParser::ChtlAnimateFunctionContext *ctx) = 0;
  virtual void exitChtlAnimateFunction(ECMAScriptParser::ChtlAnimateFunctionContext *ctx) = 0;

  virtual void enterChtlVirDeclaration(ECMAScriptParser::ChtlVirDeclarationContext *ctx) = 0;
  virtual void exitChtlVirDeclaration(ECMAScriptParser::ChtlVirDeclarationContext *ctx) = 0;

  virtual void enterChtlVirAccess(ECMAScriptParser::ChtlVirAccessContext *ctx) = 0;
  virtual void exitChtlVirAccess(ECMAScriptParser::ChtlVirAccessContext *ctx) = 0;

  virtual void enterChtlVariableUsage(ECMAScriptParser::ChtlVariableUsageContext *ctx) = 0;
  virtual void exitChtlVariableUsage(ECMAScriptParser::ChtlVariableUsageContext *ctx) = 0;

  virtual void enterChtlOriginEmbed(ECMAScriptParser::ChtlOriginEmbedContext *ctx) = 0;
  virtual void exitChtlOriginEmbed(ECMAScriptParser::ChtlOriginEmbedContext *ctx) = 0;

  virtual void enterStatement(ECMAScriptParser::StatementContext *ctx) = 0;
  virtual void exitStatement(ECMAScriptParser::StatementContext *ctx) = 0;

  virtual void enterBlock(ECMAScriptParser::BlockContext *ctx) = 0;
  virtual void exitBlock(ECMAScriptParser::BlockContext *ctx) = 0;

  virtual void enterStatementList(ECMAScriptParser::StatementListContext *ctx) = 0;
  virtual void exitStatementList(ECMAScriptParser::StatementListContext *ctx) = 0;

  virtual void enterVariableStatement(ECMAScriptParser::VariableStatementContext *ctx) = 0;
  virtual void exitVariableStatement(ECMAScriptParser::VariableStatementContext *ctx) = 0;

  virtual void enterVarModifier(ECMAScriptParser::VarModifierContext *ctx) = 0;
  virtual void exitVarModifier(ECMAScriptParser::VarModifierContext *ctx) = 0;

  virtual void enterVariableDeclarationList(ECMAScriptParser::VariableDeclarationListContext *ctx) = 0;
  virtual void exitVariableDeclarationList(ECMAScriptParser::VariableDeclarationListContext *ctx) = 0;

  virtual void enterVariableDeclaration(ECMAScriptParser::VariableDeclarationContext *ctx) = 0;
  virtual void exitVariableDeclaration(ECMAScriptParser::VariableDeclarationContext *ctx) = 0;

  virtual void enterEmptyStatement(ECMAScriptParser::EmptyStatementContext *ctx) = 0;
  virtual void exitEmptyStatement(ECMAScriptParser::EmptyStatementContext *ctx) = 0;

  virtual void enterExpressionStatement(ECMAScriptParser::ExpressionStatementContext *ctx) = 0;
  virtual void exitExpressionStatement(ECMAScriptParser::ExpressionStatementContext *ctx) = 0;

  virtual void enterIfStatement(ECMAScriptParser::IfStatementContext *ctx) = 0;
  virtual void exitIfStatement(ECMAScriptParser::IfStatementContext *ctx) = 0;

  virtual void enterIterationStatement(ECMAScriptParser::IterationStatementContext *ctx) = 0;
  virtual void exitIterationStatement(ECMAScriptParser::IterationStatementContext *ctx) = 0;

  virtual void enterForControl(ECMAScriptParser::ForControlContext *ctx) = 0;
  virtual void exitForControl(ECMAScriptParser::ForControlContext *ctx) = 0;

  virtual void enterContinueStatement(ECMAScriptParser::ContinueStatementContext *ctx) = 0;
  virtual void exitContinueStatement(ECMAScriptParser::ContinueStatementContext *ctx) = 0;

  virtual void enterBreakStatement(ECMAScriptParser::BreakStatementContext *ctx) = 0;
  virtual void exitBreakStatement(ECMAScriptParser::BreakStatementContext *ctx) = 0;

  virtual void enterReturnStatement(ECMAScriptParser::ReturnStatementContext *ctx) = 0;
  virtual void exitReturnStatement(ECMAScriptParser::ReturnStatementContext *ctx) = 0;

  virtual void enterWithStatement(ECMAScriptParser::WithStatementContext *ctx) = 0;
  virtual void exitWithStatement(ECMAScriptParser::WithStatementContext *ctx) = 0;

  virtual void enterSwitchStatement(ECMAScriptParser::SwitchStatementContext *ctx) = 0;
  virtual void exitSwitchStatement(ECMAScriptParser::SwitchStatementContext *ctx) = 0;

  virtual void enterCaseBlock(ECMAScriptParser::CaseBlockContext *ctx) = 0;
  virtual void exitCaseBlock(ECMAScriptParser::CaseBlockContext *ctx) = 0;

  virtual void enterCaseClauses(ECMAScriptParser::CaseClausesContext *ctx) = 0;
  virtual void exitCaseClauses(ECMAScriptParser::CaseClausesContext *ctx) = 0;

  virtual void enterCaseClause(ECMAScriptParser::CaseClauseContext *ctx) = 0;
  virtual void exitCaseClause(ECMAScriptParser::CaseClauseContext *ctx) = 0;

  virtual void enterThrowStatement(ECMAScriptParser::ThrowStatementContext *ctx) = 0;
  virtual void exitThrowStatement(ECMAScriptParser::ThrowStatementContext *ctx) = 0;

  virtual void enterTryStatement(ECMAScriptParser::TryStatementContext *ctx) = 0;
  virtual void exitTryStatement(ECMAScriptParser::TryStatementContext *ctx) = 0;

  virtual void enterCatchProduction(ECMAScriptParser::CatchProductionContext *ctx) = 0;
  virtual void exitCatchProduction(ECMAScriptParser::CatchProductionContext *ctx) = 0;

  virtual void enterFinallyProduction(ECMAScriptParser::FinallyProductionContext *ctx) = 0;
  virtual void exitFinallyProduction(ECMAScriptParser::FinallyProductionContext *ctx) = 0;

  virtual void enterDebuggerStatement(ECMAScriptParser::DebuggerStatementContext *ctx) = 0;
  virtual void exitDebuggerStatement(ECMAScriptParser::DebuggerStatementContext *ctx) = 0;

  virtual void enterLabelledStatement(ECMAScriptParser::LabelledStatementContext *ctx) = 0;
  virtual void exitLabelledStatement(ECMAScriptParser::LabelledStatementContext *ctx) = 0;

  virtual void enterFunctionDeclaration(ECMAScriptParser::FunctionDeclarationContext *ctx) = 0;
  virtual void exitFunctionDeclaration(ECMAScriptParser::FunctionDeclarationContext *ctx) = 0;

  virtual void enterFormalParameterList(ECMAScriptParser::FormalParameterListContext *ctx) = 0;
  virtual void exitFormalParameterList(ECMAScriptParser::FormalParameterListContext *ctx) = 0;

  virtual void enterFunctionBody(ECMAScriptParser::FunctionBodyContext *ctx) = 0;
  virtual void exitFunctionBody(ECMAScriptParser::FunctionBodyContext *ctx) = 0;

  virtual void enterExpressionSequence(ECMAScriptParser::ExpressionSequenceContext *ctx) = 0;
  virtual void exitExpressionSequence(ECMAScriptParser::ExpressionSequenceContext *ctx) = 0;

  virtual void enterSingleExpression(ECMAScriptParser::SingleExpressionContext *ctx) = 0;
  virtual void exitSingleExpression(ECMAScriptParser::SingleExpressionContext *ctx) = 0;

  virtual void enterAssignmentOperator(ECMAScriptParser::AssignmentOperatorContext *ctx) = 0;
  virtual void exitAssignmentOperator(ECMAScriptParser::AssignmentOperatorContext *ctx) = 0;

  virtual void enterPrimaryExpression(ECMAScriptParser::PrimaryExpressionContext *ctx) = 0;
  virtual void exitPrimaryExpression(ECMAScriptParser::PrimaryExpressionContext *ctx) = 0;

  virtual void enterFunctionExpression(ECMAScriptParser::FunctionExpressionContext *ctx) = 0;
  virtual void exitFunctionExpression(ECMAScriptParser::FunctionExpressionContext *ctx) = 0;

  virtual void enterArrowFunctionExpression(ECMAScriptParser::ArrowFunctionExpressionContext *ctx) = 0;
  virtual void exitArrowFunctionExpression(ECMAScriptParser::ArrowFunctionExpressionContext *ctx) = 0;

  virtual void enterArrowFunctionParameters(ECMAScriptParser::ArrowFunctionParametersContext *ctx) = 0;
  virtual void exitArrowFunctionParameters(ECMAScriptParser::ArrowFunctionParametersContext *ctx) = 0;

  virtual void enterArrowFunctionBody(ECMAScriptParser::ArrowFunctionBodyContext *ctx) = 0;
  virtual void exitArrowFunctionBody(ECMAScriptParser::ArrowFunctionBodyContext *ctx) = 0;

  virtual void enterArguments(ECMAScriptParser::ArgumentsContext *ctx) = 0;
  virtual void exitArguments(ECMAScriptParser::ArgumentsContext *ctx) = 0;

  virtual void enterArgumentList(ECMAScriptParser::ArgumentListContext *ctx) = 0;
  virtual void exitArgumentList(ECMAScriptParser::ArgumentListContext *ctx) = 0;

  virtual void enterLiteral(ECMAScriptParser::LiteralContext *ctx) = 0;
  virtual void exitLiteral(ECMAScriptParser::LiteralContext *ctx) = 0;

  virtual void enterNullLiteral(ECMAScriptParser::NullLiteralContext *ctx) = 0;
  virtual void exitNullLiteral(ECMAScriptParser::NullLiteralContext *ctx) = 0;

  virtual void enterBooleanLiteral(ECMAScriptParser::BooleanLiteralContext *ctx) = 0;
  virtual void exitBooleanLiteral(ECMAScriptParser::BooleanLiteralContext *ctx) = 0;

  virtual void enterStringLiteral(ECMAScriptParser::StringLiteralContext *ctx) = 0;
  virtual void exitStringLiteral(ECMAScriptParser::StringLiteralContext *ctx) = 0;

  virtual void enterTemplateLiteral(ECMAScriptParser::TemplateLiteralContext *ctx) = 0;
  virtual void exitTemplateLiteral(ECMAScriptParser::TemplateLiteralContext *ctx) = 0;

  virtual void enterTemplateStringAtom(ECMAScriptParser::TemplateStringAtomContext *ctx) = 0;
  virtual void exitTemplateStringAtom(ECMAScriptParser::TemplateStringAtomContext *ctx) = 0;

  virtual void enterRegularExpressionLiteral(ECMAScriptParser::RegularExpressionLiteralContext *ctx) = 0;
  virtual void exitRegularExpressionLiteral(ECMAScriptParser::RegularExpressionLiteralContext *ctx) = 0;

  virtual void enterNumericLiteral(ECMAScriptParser::NumericLiteralContext *ctx) = 0;
  virtual void exitNumericLiteral(ECMAScriptParser::NumericLiteralContext *ctx) = 0;

  virtual void enterArrayLiteral(ECMAScriptParser::ArrayLiteralContext *ctx) = 0;
  virtual void exitArrayLiteral(ECMAScriptParser::ArrayLiteralContext *ctx) = 0;

  virtual void enterElementList(ECMAScriptParser::ElementListContext *ctx) = 0;
  virtual void exitElementList(ECMAScriptParser::ElementListContext *ctx) = 0;

  virtual void enterArrayElement(ECMAScriptParser::ArrayElementContext *ctx) = 0;
  virtual void exitArrayElement(ECMAScriptParser::ArrayElementContext *ctx) = 0;

  virtual void enterObjectLiteral(ECMAScriptParser::ObjectLiteralContext *ctx) = 0;
  virtual void exitObjectLiteral(ECMAScriptParser::ObjectLiteralContext *ctx) = 0;

  virtual void enterPropertyAssignment(ECMAScriptParser::PropertyAssignmentContext *ctx) = 0;
  virtual void exitPropertyAssignment(ECMAScriptParser::PropertyAssignmentContext *ctx) = 0;

  virtual void enterPropertyName(ECMAScriptParser::PropertyNameContext *ctx) = 0;
  virtual void exitPropertyName(ECMAScriptParser::PropertyNameContext *ctx) = 0;

  virtual void enterGetter(ECMAScriptParser::GetterContext *ctx) = 0;
  virtual void exitGetter(ECMAScriptParser::GetterContext *ctx) = 0;

  virtual void enterSetter(ECMAScriptParser::SetterContext *ctx) = 0;
  virtual void exitSetter(ECMAScriptParser::SetterContext *ctx) = 0;

  virtual void enterIdentifier(ECMAScriptParser::IdentifierContext *ctx) = 0;
  virtual void exitIdentifier(ECMAScriptParser::IdentifierContext *ctx) = 0;

  virtual void enterEos(ECMAScriptParser::EosContext *ctx) = 0;
  virtual void exitEos(ECMAScriptParser::EosContext *ctx) = 0;


};


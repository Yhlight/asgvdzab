
// Generated from grammar/ECMAScript.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "ECMAScriptListener.h"


/**
 * This class provides an empty implementation of ECMAScriptListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  ECMAScriptBaseListener : public ECMAScriptListener {
public:

  virtual void enterProgram(ECMAScriptParser::ProgramContext * /*ctx*/) override { }
  virtual void exitProgram(ECMAScriptParser::ProgramContext * /*ctx*/) override { }

  virtual void enterSourceElements(ECMAScriptParser::SourceElementsContext * /*ctx*/) override { }
  virtual void exitSourceElements(ECMAScriptParser::SourceElementsContext * /*ctx*/) override { }

  virtual void enterSourceElement(ECMAScriptParser::SourceElementContext * /*ctx*/) override { }
  virtual void exitSourceElement(ECMAScriptParser::SourceElementContext * /*ctx*/) override { }

  virtual void enterChtlJsElement(ECMAScriptParser::ChtlJsElementContext * /*ctx*/) override { }
  virtual void exitChtlJsElement(ECMAScriptParser::ChtlJsElementContext * /*ctx*/) override { }

  virtual void enterChtlEnhancedSelector(ECMAScriptParser::ChtlEnhancedSelectorContext * /*ctx*/) override { }
  virtual void exitChtlEnhancedSelector(ECMAScriptParser::ChtlEnhancedSelectorContext * /*ctx*/) override { }

  virtual void enterSelectorExpression(ECMAScriptParser::SelectorExpressionContext * /*ctx*/) override { }
  virtual void exitSelectorExpression(ECMAScriptParser::SelectorExpressionContext * /*ctx*/) override { }

  virtual void enterChtlArrowAccess(ECMAScriptParser::ChtlArrowAccessContext * /*ctx*/) override { }
  virtual void exitChtlArrowAccess(ECMAScriptParser::ChtlArrowAccessContext * /*ctx*/) override { }

  virtual void enterChtlListenFunction(ECMAScriptParser::ChtlListenFunctionContext * /*ctx*/) override { }
  virtual void exitChtlListenFunction(ECMAScriptParser::ChtlListenFunctionContext * /*ctx*/) override { }

  virtual void enterChtlDelegateFunction(ECMAScriptParser::ChtlDelegateFunctionContext * /*ctx*/) override { }
  virtual void exitChtlDelegateFunction(ECMAScriptParser::ChtlDelegateFunctionContext * /*ctx*/) override { }

  virtual void enterChtlAnimateFunction(ECMAScriptParser::ChtlAnimateFunctionContext * /*ctx*/) override { }
  virtual void exitChtlAnimateFunction(ECMAScriptParser::ChtlAnimateFunctionContext * /*ctx*/) override { }

  virtual void enterChtlVirDeclaration(ECMAScriptParser::ChtlVirDeclarationContext * /*ctx*/) override { }
  virtual void exitChtlVirDeclaration(ECMAScriptParser::ChtlVirDeclarationContext * /*ctx*/) override { }

  virtual void enterChtlVirAccess(ECMAScriptParser::ChtlVirAccessContext * /*ctx*/) override { }
  virtual void exitChtlVirAccess(ECMAScriptParser::ChtlVirAccessContext * /*ctx*/) override { }

  virtual void enterChtlVariableUsage(ECMAScriptParser::ChtlVariableUsageContext * /*ctx*/) override { }
  virtual void exitChtlVariableUsage(ECMAScriptParser::ChtlVariableUsageContext * /*ctx*/) override { }

  virtual void enterChtlOriginEmbed(ECMAScriptParser::ChtlOriginEmbedContext * /*ctx*/) override { }
  virtual void exitChtlOriginEmbed(ECMAScriptParser::ChtlOriginEmbedContext * /*ctx*/) override { }

  virtual void enterStatement(ECMAScriptParser::StatementContext * /*ctx*/) override { }
  virtual void exitStatement(ECMAScriptParser::StatementContext * /*ctx*/) override { }

  virtual void enterBlock(ECMAScriptParser::BlockContext * /*ctx*/) override { }
  virtual void exitBlock(ECMAScriptParser::BlockContext * /*ctx*/) override { }

  virtual void enterStatementList(ECMAScriptParser::StatementListContext * /*ctx*/) override { }
  virtual void exitStatementList(ECMAScriptParser::StatementListContext * /*ctx*/) override { }

  virtual void enterVariableStatement(ECMAScriptParser::VariableStatementContext * /*ctx*/) override { }
  virtual void exitVariableStatement(ECMAScriptParser::VariableStatementContext * /*ctx*/) override { }

  virtual void enterVarModifier(ECMAScriptParser::VarModifierContext * /*ctx*/) override { }
  virtual void exitVarModifier(ECMAScriptParser::VarModifierContext * /*ctx*/) override { }

  virtual void enterVariableDeclarationList(ECMAScriptParser::VariableDeclarationListContext * /*ctx*/) override { }
  virtual void exitVariableDeclarationList(ECMAScriptParser::VariableDeclarationListContext * /*ctx*/) override { }

  virtual void enterVariableDeclaration(ECMAScriptParser::VariableDeclarationContext * /*ctx*/) override { }
  virtual void exitVariableDeclaration(ECMAScriptParser::VariableDeclarationContext * /*ctx*/) override { }

  virtual void enterEmptyStatement(ECMAScriptParser::EmptyStatementContext * /*ctx*/) override { }
  virtual void exitEmptyStatement(ECMAScriptParser::EmptyStatementContext * /*ctx*/) override { }

  virtual void enterExpressionStatement(ECMAScriptParser::ExpressionStatementContext * /*ctx*/) override { }
  virtual void exitExpressionStatement(ECMAScriptParser::ExpressionStatementContext * /*ctx*/) override { }

  virtual void enterIfStatement(ECMAScriptParser::IfStatementContext * /*ctx*/) override { }
  virtual void exitIfStatement(ECMAScriptParser::IfStatementContext * /*ctx*/) override { }

  virtual void enterIterationStatement(ECMAScriptParser::IterationStatementContext * /*ctx*/) override { }
  virtual void exitIterationStatement(ECMAScriptParser::IterationStatementContext * /*ctx*/) override { }

  virtual void enterForControl(ECMAScriptParser::ForControlContext * /*ctx*/) override { }
  virtual void exitForControl(ECMAScriptParser::ForControlContext * /*ctx*/) override { }

  virtual void enterContinueStatement(ECMAScriptParser::ContinueStatementContext * /*ctx*/) override { }
  virtual void exitContinueStatement(ECMAScriptParser::ContinueStatementContext * /*ctx*/) override { }

  virtual void enterBreakStatement(ECMAScriptParser::BreakStatementContext * /*ctx*/) override { }
  virtual void exitBreakStatement(ECMAScriptParser::BreakStatementContext * /*ctx*/) override { }

  virtual void enterReturnStatement(ECMAScriptParser::ReturnStatementContext * /*ctx*/) override { }
  virtual void exitReturnStatement(ECMAScriptParser::ReturnStatementContext * /*ctx*/) override { }

  virtual void enterWithStatement(ECMAScriptParser::WithStatementContext * /*ctx*/) override { }
  virtual void exitWithStatement(ECMAScriptParser::WithStatementContext * /*ctx*/) override { }

  virtual void enterSwitchStatement(ECMAScriptParser::SwitchStatementContext * /*ctx*/) override { }
  virtual void exitSwitchStatement(ECMAScriptParser::SwitchStatementContext * /*ctx*/) override { }

  virtual void enterCaseBlock(ECMAScriptParser::CaseBlockContext * /*ctx*/) override { }
  virtual void exitCaseBlock(ECMAScriptParser::CaseBlockContext * /*ctx*/) override { }

  virtual void enterCaseClauses(ECMAScriptParser::CaseClausesContext * /*ctx*/) override { }
  virtual void exitCaseClauses(ECMAScriptParser::CaseClausesContext * /*ctx*/) override { }

  virtual void enterCaseClause(ECMAScriptParser::CaseClauseContext * /*ctx*/) override { }
  virtual void exitCaseClause(ECMAScriptParser::CaseClauseContext * /*ctx*/) override { }

  virtual void enterThrowStatement(ECMAScriptParser::ThrowStatementContext * /*ctx*/) override { }
  virtual void exitThrowStatement(ECMAScriptParser::ThrowStatementContext * /*ctx*/) override { }

  virtual void enterTryStatement(ECMAScriptParser::TryStatementContext * /*ctx*/) override { }
  virtual void exitTryStatement(ECMAScriptParser::TryStatementContext * /*ctx*/) override { }

  virtual void enterCatchProduction(ECMAScriptParser::CatchProductionContext * /*ctx*/) override { }
  virtual void exitCatchProduction(ECMAScriptParser::CatchProductionContext * /*ctx*/) override { }

  virtual void enterFinallyProduction(ECMAScriptParser::FinallyProductionContext * /*ctx*/) override { }
  virtual void exitFinallyProduction(ECMAScriptParser::FinallyProductionContext * /*ctx*/) override { }

  virtual void enterDebuggerStatement(ECMAScriptParser::DebuggerStatementContext * /*ctx*/) override { }
  virtual void exitDebuggerStatement(ECMAScriptParser::DebuggerStatementContext * /*ctx*/) override { }

  virtual void enterLabelledStatement(ECMAScriptParser::LabelledStatementContext * /*ctx*/) override { }
  virtual void exitLabelledStatement(ECMAScriptParser::LabelledStatementContext * /*ctx*/) override { }

  virtual void enterFunctionDeclaration(ECMAScriptParser::FunctionDeclarationContext * /*ctx*/) override { }
  virtual void exitFunctionDeclaration(ECMAScriptParser::FunctionDeclarationContext * /*ctx*/) override { }

  virtual void enterFormalParameterList(ECMAScriptParser::FormalParameterListContext * /*ctx*/) override { }
  virtual void exitFormalParameterList(ECMAScriptParser::FormalParameterListContext * /*ctx*/) override { }

  virtual void enterFunctionBody(ECMAScriptParser::FunctionBodyContext * /*ctx*/) override { }
  virtual void exitFunctionBody(ECMAScriptParser::FunctionBodyContext * /*ctx*/) override { }

  virtual void enterExpressionSequence(ECMAScriptParser::ExpressionSequenceContext * /*ctx*/) override { }
  virtual void exitExpressionSequence(ECMAScriptParser::ExpressionSequenceContext * /*ctx*/) override { }

  virtual void enterSingleExpression(ECMAScriptParser::SingleExpressionContext * /*ctx*/) override { }
  virtual void exitSingleExpression(ECMAScriptParser::SingleExpressionContext * /*ctx*/) override { }

  virtual void enterAssignmentOperator(ECMAScriptParser::AssignmentOperatorContext * /*ctx*/) override { }
  virtual void exitAssignmentOperator(ECMAScriptParser::AssignmentOperatorContext * /*ctx*/) override { }

  virtual void enterPrimaryExpression(ECMAScriptParser::PrimaryExpressionContext * /*ctx*/) override { }
  virtual void exitPrimaryExpression(ECMAScriptParser::PrimaryExpressionContext * /*ctx*/) override { }

  virtual void enterFunctionExpression(ECMAScriptParser::FunctionExpressionContext * /*ctx*/) override { }
  virtual void exitFunctionExpression(ECMAScriptParser::FunctionExpressionContext * /*ctx*/) override { }

  virtual void enterArrowFunctionExpression(ECMAScriptParser::ArrowFunctionExpressionContext * /*ctx*/) override { }
  virtual void exitArrowFunctionExpression(ECMAScriptParser::ArrowFunctionExpressionContext * /*ctx*/) override { }

  virtual void enterArrowFunctionParameters(ECMAScriptParser::ArrowFunctionParametersContext * /*ctx*/) override { }
  virtual void exitArrowFunctionParameters(ECMAScriptParser::ArrowFunctionParametersContext * /*ctx*/) override { }

  virtual void enterArrowFunctionBody(ECMAScriptParser::ArrowFunctionBodyContext * /*ctx*/) override { }
  virtual void exitArrowFunctionBody(ECMAScriptParser::ArrowFunctionBodyContext * /*ctx*/) override { }

  virtual void enterArguments(ECMAScriptParser::ArgumentsContext * /*ctx*/) override { }
  virtual void exitArguments(ECMAScriptParser::ArgumentsContext * /*ctx*/) override { }

  virtual void enterArgumentList(ECMAScriptParser::ArgumentListContext * /*ctx*/) override { }
  virtual void exitArgumentList(ECMAScriptParser::ArgumentListContext * /*ctx*/) override { }

  virtual void enterLiteral(ECMAScriptParser::LiteralContext * /*ctx*/) override { }
  virtual void exitLiteral(ECMAScriptParser::LiteralContext * /*ctx*/) override { }

  virtual void enterNullLiteral(ECMAScriptParser::NullLiteralContext * /*ctx*/) override { }
  virtual void exitNullLiteral(ECMAScriptParser::NullLiteralContext * /*ctx*/) override { }

  virtual void enterBooleanLiteral(ECMAScriptParser::BooleanLiteralContext * /*ctx*/) override { }
  virtual void exitBooleanLiteral(ECMAScriptParser::BooleanLiteralContext * /*ctx*/) override { }

  virtual void enterStringLiteral(ECMAScriptParser::StringLiteralContext * /*ctx*/) override { }
  virtual void exitStringLiteral(ECMAScriptParser::StringLiteralContext * /*ctx*/) override { }

  virtual void enterTemplateLiteral(ECMAScriptParser::TemplateLiteralContext * /*ctx*/) override { }
  virtual void exitTemplateLiteral(ECMAScriptParser::TemplateLiteralContext * /*ctx*/) override { }

  virtual void enterTemplateStringAtom(ECMAScriptParser::TemplateStringAtomContext * /*ctx*/) override { }
  virtual void exitTemplateStringAtom(ECMAScriptParser::TemplateStringAtomContext * /*ctx*/) override { }

  virtual void enterRegularExpressionLiteral(ECMAScriptParser::RegularExpressionLiteralContext * /*ctx*/) override { }
  virtual void exitRegularExpressionLiteral(ECMAScriptParser::RegularExpressionLiteralContext * /*ctx*/) override { }

  virtual void enterNumericLiteral(ECMAScriptParser::NumericLiteralContext * /*ctx*/) override { }
  virtual void exitNumericLiteral(ECMAScriptParser::NumericLiteralContext * /*ctx*/) override { }

  virtual void enterArrayLiteral(ECMAScriptParser::ArrayLiteralContext * /*ctx*/) override { }
  virtual void exitArrayLiteral(ECMAScriptParser::ArrayLiteralContext * /*ctx*/) override { }

  virtual void enterElementList(ECMAScriptParser::ElementListContext * /*ctx*/) override { }
  virtual void exitElementList(ECMAScriptParser::ElementListContext * /*ctx*/) override { }

  virtual void enterArrayElement(ECMAScriptParser::ArrayElementContext * /*ctx*/) override { }
  virtual void exitArrayElement(ECMAScriptParser::ArrayElementContext * /*ctx*/) override { }

  virtual void enterObjectLiteral(ECMAScriptParser::ObjectLiteralContext * /*ctx*/) override { }
  virtual void exitObjectLiteral(ECMAScriptParser::ObjectLiteralContext * /*ctx*/) override { }

  virtual void enterPropertyAssignment(ECMAScriptParser::PropertyAssignmentContext * /*ctx*/) override { }
  virtual void exitPropertyAssignment(ECMAScriptParser::PropertyAssignmentContext * /*ctx*/) override { }

  virtual void enterPropertyName(ECMAScriptParser::PropertyNameContext * /*ctx*/) override { }
  virtual void exitPropertyName(ECMAScriptParser::PropertyNameContext * /*ctx*/) override { }

  virtual void enterGetter(ECMAScriptParser::GetterContext * /*ctx*/) override { }
  virtual void exitGetter(ECMAScriptParser::GetterContext * /*ctx*/) override { }

  virtual void enterSetter(ECMAScriptParser::SetterContext * /*ctx*/) override { }
  virtual void exitSetter(ECMAScriptParser::SetterContext * /*ctx*/) override { }

  virtual void enterIdentifier(ECMAScriptParser::IdentifierContext * /*ctx*/) override { }
  virtual void exitIdentifier(ECMAScriptParser::IdentifierContext * /*ctx*/) override { }

  virtual void enterEos(ECMAScriptParser::EosContext * /*ctx*/) override { }
  virtual void exitEos(ECMAScriptParser::EosContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};


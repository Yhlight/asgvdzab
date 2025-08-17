
// Generated from grammar/CSS3.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "CSS3Parser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by CSS3Parser.
 */
class  CSS3Listener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterStylesheet(CSS3Parser::StylesheetContext *ctx) = 0;
  virtual void exitStylesheet(CSS3Parser::StylesheetContext *ctx) = 0;

  virtual void enterStyleRule(CSS3Parser::StyleRuleContext *ctx) = 0;
  virtual void exitStyleRule(CSS3Parser::StyleRuleContext *ctx) = 0;

  virtual void enterRuleSet(CSS3Parser::RuleSetContext *ctx) = 0;
  virtual void exitRuleSet(CSS3Parser::RuleSetContext *ctx) = 0;

  virtual void enterSelectorList(CSS3Parser::SelectorListContext *ctx) = 0;
  virtual void exitSelectorList(CSS3Parser::SelectorListContext *ctx) = 0;

  virtual void enterSelector(CSS3Parser::SelectorContext *ctx) = 0;
  virtual void exitSelector(CSS3Parser::SelectorContext *ctx) = 0;

  virtual void enterCombinator(CSS3Parser::CombinatorContext *ctx) = 0;
  virtual void exitCombinator(CSS3Parser::CombinatorContext *ctx) = 0;

  virtual void enterSimpleSelector(CSS3Parser::SimpleSelectorContext *ctx) = 0;
  virtual void exitSimpleSelector(CSS3Parser::SimpleSelectorContext *ctx) = 0;

  virtual void enterTypeSelector(CSS3Parser::TypeSelectorContext *ctx) = 0;
  virtual void exitTypeSelector(CSS3Parser::TypeSelectorContext *ctx) = 0;

  virtual void enterTagName(CSS3Parser::TagNameContext *ctx) = 0;
  virtual void exitTagName(CSS3Parser::TagNameContext *ctx) = 0;

  virtual void enterQualifiers(CSS3Parser::QualifiersContext *ctx) = 0;
  virtual void exitQualifiers(CSS3Parser::QualifiersContext *ctx) = 0;

  virtual void enterHashSelector(CSS3Parser::HashSelectorContext *ctx) = 0;
  virtual void exitHashSelector(CSS3Parser::HashSelectorContext *ctx) = 0;

  virtual void enterClassSelector(CSS3Parser::ClassSelectorContext *ctx) = 0;
  virtual void exitClassSelector(CSS3Parser::ClassSelectorContext *ctx) = 0;

  virtual void enterAttributeSelector(CSS3Parser::AttributeSelectorContext *ctx) = 0;
  virtual void exitAttributeSelector(CSS3Parser::AttributeSelectorContext *ctx) = 0;

  virtual void enterAttributeOperator(CSS3Parser::AttributeOperatorContext *ctx) = 0;
  virtual void exitAttributeOperator(CSS3Parser::AttributeOperatorContext *ctx) = 0;

  virtual void enterAttributeValue(CSS3Parser::AttributeValueContext *ctx) = 0;
  virtual void exitAttributeValue(CSS3Parser::AttributeValueContext *ctx) = 0;

  virtual void enterPseudoClass(CSS3Parser::PseudoClassContext *ctx) = 0;
  virtual void exitPseudoClass(CSS3Parser::PseudoClassContext *ctx) = 0;

  virtual void enterPseudoElement(CSS3Parser::PseudoElementContext *ctx) = 0;
  virtual void exitPseudoElement(CSS3Parser::PseudoElementContext *ctx) = 0;

  virtual void enterDeclarationList(CSS3Parser::DeclarationListContext *ctx) = 0;
  virtual void exitDeclarationList(CSS3Parser::DeclarationListContext *ctx) = 0;

  virtual void enterDeclaration(CSS3Parser::DeclarationContext *ctx) = 0;
  virtual void exitDeclaration(CSS3Parser::DeclarationContext *ctx) = 0;

  virtual void enterProperty(CSS3Parser::PropertyContext *ctx) = 0;
  virtual void exitProperty(CSS3Parser::PropertyContext *ctx) = 0;

  virtual void enterValueList(CSS3Parser::ValueListContext *ctx) = 0;
  virtual void exitValueList(CSS3Parser::ValueListContext *ctx) = 0;

  virtual void enterValue(CSS3Parser::ValueContext *ctx) = 0;
  virtual void exitValue(CSS3Parser::ValueContext *ctx) = 0;

  virtual void enterCalc(CSS3Parser::CalcContext *ctx) = 0;
  virtual void exitCalc(CSS3Parser::CalcContext *ctx) = 0;

  virtual void enterFunction_(CSS3Parser::Function_Context *ctx) = 0;
  virtual void exitFunction_(CSS3Parser::Function_Context *ctx) = 0;

  virtual void enterExpressionList(CSS3Parser::ExpressionListContext *ctx) = 0;
  virtual void exitExpressionList(CSS3Parser::ExpressionListContext *ctx) = 0;

  virtual void enterExpression(CSS3Parser::ExpressionContext *ctx) = 0;
  virtual void exitExpression(CSS3Parser::ExpressionContext *ctx) = 0;

  virtual void enterTerm(CSS3Parser::TermContext *ctx) = 0;
  virtual void exitTerm(CSS3Parser::TermContext *ctx) = 0;

  virtual void enterFactor(CSS3Parser::FactorContext *ctx) = 0;
  virtual void exitFactor(CSS3Parser::FactorContext *ctx) = 0;

  virtual void enterPrio(CSS3Parser::PrioContext *ctx) = 0;
  virtual void exitPrio(CSS3Parser::PrioContext *ctx) = 0;

  virtual void enterChtlTemplateUsage(CSS3Parser::ChtlTemplateUsageContext *ctx) = 0;
  virtual void exitChtlTemplateUsage(CSS3Parser::ChtlTemplateUsageContext *ctx) = 0;

  virtual void enterChtlCustomUsage(CSS3Parser::ChtlCustomUsageContext *ctx) = 0;
  virtual void exitChtlCustomUsage(CSS3Parser::ChtlCustomUsageContext *ctx) = 0;

  virtual void enterChtlSpecialization(CSS3Parser::ChtlSpecializationContext *ctx) = 0;
  virtual void exitChtlSpecialization(CSS3Parser::ChtlSpecializationContext *ctx) = 0;

  virtual void enterChtlVariableUsage(CSS3Parser::ChtlVariableUsageContext *ctx) = 0;
  virtual void exitChtlVariableUsage(CSS3Parser::ChtlVariableUsageContext *ctx) = 0;

  virtual void enterChtlVariableReference(CSS3Parser::ChtlVariableReferenceContext *ctx) = 0;
  virtual void exitChtlVariableReference(CSS3Parser::ChtlVariableReferenceContext *ctx) = 0;

  virtual void enterChtlStyleGroupUsage(CSS3Parser::ChtlStyleGroupUsageContext *ctx) = 0;
  virtual void exitChtlStyleGroupUsage(CSS3Parser::ChtlStyleGroupUsageContext *ctx) = 0;

  virtual void enterChtlDeleteProperty(CSS3Parser::ChtlDeletePropertyContext *ctx) = 0;
  virtual void exitChtlDeleteProperty(CSS3Parser::ChtlDeletePropertyContext *ctx) = 0;

  virtual void enterChtlInherit(CSS3Parser::ChtlInheritContext *ctx) = 0;
  virtual void exitChtlInherit(CSS3Parser::ChtlInheritContext *ctx) = 0;

  virtual void enterChtlOriginEmbed(CSS3Parser::ChtlOriginEmbedContext *ctx) = 0;
  virtual void exitChtlOriginEmbed(CSS3Parser::ChtlOriginEmbedContext *ctx) = 0;

  virtual void enterAtRule(CSS3Parser::AtRuleContext *ctx) = 0;
  virtual void exitAtRule(CSS3Parser::AtRuleContext *ctx) = 0;

  virtual void enterAtRuleBlock(CSS3Parser::AtRuleBlockContext *ctx) = 0;
  virtual void exitAtRuleBlock(CSS3Parser::AtRuleBlockContext *ctx) = 0;

  virtual void enterVar(CSS3Parser::VarContext *ctx) = 0;
  virtual void exitVar(CSS3Parser::VarContext *ctx) = 0;

  virtual void enterHexcolor(CSS3Parser::HexcolorContext *ctx) = 0;
  virtual void exitHexcolor(CSS3Parser::HexcolorContext *ctx) = 0;

  virtual void enterNumber(CSS3Parser::NumberContext *ctx) = 0;
  virtual void exitNumber(CSS3Parser::NumberContext *ctx) = 0;

  virtual void enterPercentage(CSS3Parser::PercentageContext *ctx) = 0;
  virtual void exitPercentage(CSS3Parser::PercentageContext *ctx) = 0;

  virtual void enterDimension(CSS3Parser::DimensionContext *ctx) = 0;
  virtual void exitDimension(CSS3Parser::DimensionContext *ctx) = 0;

  virtual void enterUnit(CSS3Parser::UnitContext *ctx) = 0;
  virtual void exitUnit(CSS3Parser::UnitContext *ctx) = 0;

  virtual void enterUri(CSS3Parser::UriContext *ctx) = 0;
  virtual void exitUri(CSS3Parser::UriContext *ctx) = 0;

  virtual void enterString(CSS3Parser::StringContext *ctx) = 0;
  virtual void exitString(CSS3Parser::StringContext *ctx) = 0;

  virtual void enterIdent(CSS3Parser::IdentContext *ctx) = 0;
  virtual void exitIdent(CSS3Parser::IdentContext *ctx) = 0;

  virtual void enterHexdigit(CSS3Parser::HexdigitContext *ctx) = 0;
  virtual void exitHexdigit(CSS3Parser::HexdigitContext *ctx) = 0;

  virtual void enterWs(CSS3Parser::WsContext *ctx) = 0;
  virtual void exitWs(CSS3Parser::WsContext *ctx) = 0;


};


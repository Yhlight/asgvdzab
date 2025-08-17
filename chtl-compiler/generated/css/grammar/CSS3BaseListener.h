
// Generated from grammar/CSS3.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "CSS3Listener.h"


/**
 * This class provides an empty implementation of CSS3Listener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  CSS3BaseListener : public CSS3Listener {
public:

  virtual void enterStylesheet(CSS3Parser::StylesheetContext * /*ctx*/) override { }
  virtual void exitStylesheet(CSS3Parser::StylesheetContext * /*ctx*/) override { }

  virtual void enterStyleRule(CSS3Parser::StyleRuleContext * /*ctx*/) override { }
  virtual void exitStyleRule(CSS3Parser::StyleRuleContext * /*ctx*/) override { }

  virtual void enterRuleSet(CSS3Parser::RuleSetContext * /*ctx*/) override { }
  virtual void exitRuleSet(CSS3Parser::RuleSetContext * /*ctx*/) override { }

  virtual void enterSelectorList(CSS3Parser::SelectorListContext * /*ctx*/) override { }
  virtual void exitSelectorList(CSS3Parser::SelectorListContext * /*ctx*/) override { }

  virtual void enterSelector(CSS3Parser::SelectorContext * /*ctx*/) override { }
  virtual void exitSelector(CSS3Parser::SelectorContext * /*ctx*/) override { }

  virtual void enterCombinator(CSS3Parser::CombinatorContext * /*ctx*/) override { }
  virtual void exitCombinator(CSS3Parser::CombinatorContext * /*ctx*/) override { }

  virtual void enterSimpleSelector(CSS3Parser::SimpleSelectorContext * /*ctx*/) override { }
  virtual void exitSimpleSelector(CSS3Parser::SimpleSelectorContext * /*ctx*/) override { }

  virtual void enterTypeSelector(CSS3Parser::TypeSelectorContext * /*ctx*/) override { }
  virtual void exitTypeSelector(CSS3Parser::TypeSelectorContext * /*ctx*/) override { }

  virtual void enterTagName(CSS3Parser::TagNameContext * /*ctx*/) override { }
  virtual void exitTagName(CSS3Parser::TagNameContext * /*ctx*/) override { }

  virtual void enterQualifiers(CSS3Parser::QualifiersContext * /*ctx*/) override { }
  virtual void exitQualifiers(CSS3Parser::QualifiersContext * /*ctx*/) override { }

  virtual void enterHashSelector(CSS3Parser::HashSelectorContext * /*ctx*/) override { }
  virtual void exitHashSelector(CSS3Parser::HashSelectorContext * /*ctx*/) override { }

  virtual void enterClassSelector(CSS3Parser::ClassSelectorContext * /*ctx*/) override { }
  virtual void exitClassSelector(CSS3Parser::ClassSelectorContext * /*ctx*/) override { }

  virtual void enterAttributeSelector(CSS3Parser::AttributeSelectorContext * /*ctx*/) override { }
  virtual void exitAttributeSelector(CSS3Parser::AttributeSelectorContext * /*ctx*/) override { }

  virtual void enterAttributeOperator(CSS3Parser::AttributeOperatorContext * /*ctx*/) override { }
  virtual void exitAttributeOperator(CSS3Parser::AttributeOperatorContext * /*ctx*/) override { }

  virtual void enterAttributeValue(CSS3Parser::AttributeValueContext * /*ctx*/) override { }
  virtual void exitAttributeValue(CSS3Parser::AttributeValueContext * /*ctx*/) override { }

  virtual void enterPseudoClass(CSS3Parser::PseudoClassContext * /*ctx*/) override { }
  virtual void exitPseudoClass(CSS3Parser::PseudoClassContext * /*ctx*/) override { }

  virtual void enterPseudoElement(CSS3Parser::PseudoElementContext * /*ctx*/) override { }
  virtual void exitPseudoElement(CSS3Parser::PseudoElementContext * /*ctx*/) override { }

  virtual void enterDeclarationList(CSS3Parser::DeclarationListContext * /*ctx*/) override { }
  virtual void exitDeclarationList(CSS3Parser::DeclarationListContext * /*ctx*/) override { }

  virtual void enterDeclaration(CSS3Parser::DeclarationContext * /*ctx*/) override { }
  virtual void exitDeclaration(CSS3Parser::DeclarationContext * /*ctx*/) override { }

  virtual void enterProperty(CSS3Parser::PropertyContext * /*ctx*/) override { }
  virtual void exitProperty(CSS3Parser::PropertyContext * /*ctx*/) override { }

  virtual void enterValueList(CSS3Parser::ValueListContext * /*ctx*/) override { }
  virtual void exitValueList(CSS3Parser::ValueListContext * /*ctx*/) override { }

  virtual void enterValue(CSS3Parser::ValueContext * /*ctx*/) override { }
  virtual void exitValue(CSS3Parser::ValueContext * /*ctx*/) override { }

  virtual void enterCalc(CSS3Parser::CalcContext * /*ctx*/) override { }
  virtual void exitCalc(CSS3Parser::CalcContext * /*ctx*/) override { }

  virtual void enterFunction_(CSS3Parser::Function_Context * /*ctx*/) override { }
  virtual void exitFunction_(CSS3Parser::Function_Context * /*ctx*/) override { }

  virtual void enterExpressionList(CSS3Parser::ExpressionListContext * /*ctx*/) override { }
  virtual void exitExpressionList(CSS3Parser::ExpressionListContext * /*ctx*/) override { }

  virtual void enterExpression(CSS3Parser::ExpressionContext * /*ctx*/) override { }
  virtual void exitExpression(CSS3Parser::ExpressionContext * /*ctx*/) override { }

  virtual void enterTerm(CSS3Parser::TermContext * /*ctx*/) override { }
  virtual void exitTerm(CSS3Parser::TermContext * /*ctx*/) override { }

  virtual void enterFactor(CSS3Parser::FactorContext * /*ctx*/) override { }
  virtual void exitFactor(CSS3Parser::FactorContext * /*ctx*/) override { }

  virtual void enterPrio(CSS3Parser::PrioContext * /*ctx*/) override { }
  virtual void exitPrio(CSS3Parser::PrioContext * /*ctx*/) override { }

  virtual void enterChtlTemplateUsage(CSS3Parser::ChtlTemplateUsageContext * /*ctx*/) override { }
  virtual void exitChtlTemplateUsage(CSS3Parser::ChtlTemplateUsageContext * /*ctx*/) override { }

  virtual void enterChtlCustomUsage(CSS3Parser::ChtlCustomUsageContext * /*ctx*/) override { }
  virtual void exitChtlCustomUsage(CSS3Parser::ChtlCustomUsageContext * /*ctx*/) override { }

  virtual void enterChtlSpecialization(CSS3Parser::ChtlSpecializationContext * /*ctx*/) override { }
  virtual void exitChtlSpecialization(CSS3Parser::ChtlSpecializationContext * /*ctx*/) override { }

  virtual void enterChtlVariableUsage(CSS3Parser::ChtlVariableUsageContext * /*ctx*/) override { }
  virtual void exitChtlVariableUsage(CSS3Parser::ChtlVariableUsageContext * /*ctx*/) override { }

  virtual void enterChtlVariableReference(CSS3Parser::ChtlVariableReferenceContext * /*ctx*/) override { }
  virtual void exitChtlVariableReference(CSS3Parser::ChtlVariableReferenceContext * /*ctx*/) override { }

  virtual void enterChtlStyleGroupUsage(CSS3Parser::ChtlStyleGroupUsageContext * /*ctx*/) override { }
  virtual void exitChtlStyleGroupUsage(CSS3Parser::ChtlStyleGroupUsageContext * /*ctx*/) override { }

  virtual void enterChtlDeleteProperty(CSS3Parser::ChtlDeletePropertyContext * /*ctx*/) override { }
  virtual void exitChtlDeleteProperty(CSS3Parser::ChtlDeletePropertyContext * /*ctx*/) override { }

  virtual void enterChtlInherit(CSS3Parser::ChtlInheritContext * /*ctx*/) override { }
  virtual void exitChtlInherit(CSS3Parser::ChtlInheritContext * /*ctx*/) override { }

  virtual void enterChtlOriginEmbed(CSS3Parser::ChtlOriginEmbedContext * /*ctx*/) override { }
  virtual void exitChtlOriginEmbed(CSS3Parser::ChtlOriginEmbedContext * /*ctx*/) override { }

  virtual void enterAtRule(CSS3Parser::AtRuleContext * /*ctx*/) override { }
  virtual void exitAtRule(CSS3Parser::AtRuleContext * /*ctx*/) override { }

  virtual void enterAtRuleBlock(CSS3Parser::AtRuleBlockContext * /*ctx*/) override { }
  virtual void exitAtRuleBlock(CSS3Parser::AtRuleBlockContext * /*ctx*/) override { }

  virtual void enterVar(CSS3Parser::VarContext * /*ctx*/) override { }
  virtual void exitVar(CSS3Parser::VarContext * /*ctx*/) override { }

  virtual void enterHexcolor(CSS3Parser::HexcolorContext * /*ctx*/) override { }
  virtual void exitHexcolor(CSS3Parser::HexcolorContext * /*ctx*/) override { }

  virtual void enterNumber(CSS3Parser::NumberContext * /*ctx*/) override { }
  virtual void exitNumber(CSS3Parser::NumberContext * /*ctx*/) override { }

  virtual void enterPercentage(CSS3Parser::PercentageContext * /*ctx*/) override { }
  virtual void exitPercentage(CSS3Parser::PercentageContext * /*ctx*/) override { }

  virtual void enterDimension(CSS3Parser::DimensionContext * /*ctx*/) override { }
  virtual void exitDimension(CSS3Parser::DimensionContext * /*ctx*/) override { }

  virtual void enterUnit(CSS3Parser::UnitContext * /*ctx*/) override { }
  virtual void exitUnit(CSS3Parser::UnitContext * /*ctx*/) override { }

  virtual void enterUri(CSS3Parser::UriContext * /*ctx*/) override { }
  virtual void exitUri(CSS3Parser::UriContext * /*ctx*/) override { }

  virtual void enterString(CSS3Parser::StringContext * /*ctx*/) override { }
  virtual void exitString(CSS3Parser::StringContext * /*ctx*/) override { }

  virtual void enterIdent(CSS3Parser::IdentContext * /*ctx*/) override { }
  virtual void exitIdent(CSS3Parser::IdentContext * /*ctx*/) override { }

  virtual void enterHexdigit(CSS3Parser::HexdigitContext * /*ctx*/) override { }
  virtual void exitHexdigit(CSS3Parser::HexdigitContext * /*ctx*/) override { }

  virtual void enterWs(CSS3Parser::WsContext * /*ctx*/) override { }
  virtual void exitWs(CSS3Parser::WsContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};


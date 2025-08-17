
// Generated from grammar/CSS3.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  CSS3Parser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, T__19 = 20, 
    T__20 = 21, T__21 = 22, T__22 = 23, T__23 = 24, T__24 = 25, T__25 = 26, 
    T__26 = 27, T__27 = 28, T__28 = 29, T__29 = 30, T__30 = 31, T__31 = 32, 
    T__32 = 33, T__33 = 34, T__34 = 35, T__35 = 36, T__36 = 37, T__37 = 38, 
    T__38 = 39, T__39 = 40, T__40 = 41, T__41 = 42, T__42 = 43, T__43 = 44, 
    T__44 = 45, T__45 = 46, T__46 = 47, T__47 = 48, T__48 = 49, T__49 = 50, 
    T__50 = 51, T__51 = 52, T__52 = 53, T__53 = 54, T__54 = 55, T__55 = 56, 
    T__56 = 57, T__57 = 58, T__58 = 59, T__59 = 60, T__60 = 61, T__61 = 62, 
    T__62 = 63, T__63 = 64, T__64 = 65, T__65 = 66, NUMBER = 67, STRING = 68, 
    IDENT = 69, HEXDIGIT = 70, WS = 71, NONASCII = 72, ESCAPE = 73, URL_CONTENT = 74, 
    COMMENT = 75, LINE_COMMENT = 76
  };

  enum {
    RuleStylesheet = 0, RuleStyleRule = 1, RuleRuleSet = 2, RuleSelectorList = 3, 
    RuleSelector = 4, RuleCombinator = 5, RuleSimpleSelector = 6, RuleTypeSelector = 7, 
    RuleTagName = 8, RuleQualifiers = 9, RuleHashSelector = 10, RuleClassSelector = 11, 
    RuleAttributeSelector = 12, RuleAttributeOperator = 13, RuleAttributeValue = 14, 
    RulePseudoClass = 15, RulePseudoElement = 16, RuleDeclarationList = 17, 
    RuleDeclaration = 18, RuleProperty = 19, RuleValueList = 20, RuleValue = 21, 
    RuleCalc = 22, RuleFunction_ = 23, RuleExpressionList = 24, RuleExpression = 25, 
    RuleTerm = 26, RuleFactor = 27, RulePrio = 28, RuleChtlTemplateUsage = 29, 
    RuleChtlCustomUsage = 30, RuleChtlSpecialization = 31, RuleChtlVariableUsage = 32, 
    RuleChtlVariableReference = 33, RuleChtlStyleGroupUsage = 34, RuleChtlDeleteProperty = 35, 
    RuleChtlInherit = 36, RuleChtlOriginEmbed = 37, RuleAtRule = 38, RuleAtRuleBlock = 39, 
    RuleVar = 40, RuleHexcolor = 41, RuleNumber = 42, RulePercentage = 43, 
    RuleDimension = 44, RuleUnit = 45, RuleUri = 46, RuleString = 47, RuleIdent = 48, 
    RuleHexdigit = 49, RuleWs = 50
  };

  explicit CSS3Parser(antlr4::TokenStream *input);

  CSS3Parser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~CSS3Parser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class StylesheetContext;
  class StyleRuleContext;
  class RuleSetContext;
  class SelectorListContext;
  class SelectorContext;
  class CombinatorContext;
  class SimpleSelectorContext;
  class TypeSelectorContext;
  class TagNameContext;
  class QualifiersContext;
  class HashSelectorContext;
  class ClassSelectorContext;
  class AttributeSelectorContext;
  class AttributeOperatorContext;
  class AttributeValueContext;
  class PseudoClassContext;
  class PseudoElementContext;
  class DeclarationListContext;
  class DeclarationContext;
  class PropertyContext;
  class ValueListContext;
  class ValueContext;
  class CalcContext;
  class Function_Context;
  class ExpressionListContext;
  class ExpressionContext;
  class TermContext;
  class FactorContext;
  class PrioContext;
  class ChtlTemplateUsageContext;
  class ChtlCustomUsageContext;
  class ChtlSpecializationContext;
  class ChtlVariableUsageContext;
  class ChtlVariableReferenceContext;
  class ChtlStyleGroupUsageContext;
  class ChtlDeletePropertyContext;
  class ChtlInheritContext;
  class ChtlOriginEmbedContext;
  class AtRuleContext;
  class AtRuleBlockContext;
  class VarContext;
  class HexcolorContext;
  class NumberContext;
  class PercentageContext;
  class DimensionContext;
  class UnitContext;
  class UriContext;
  class StringContext;
  class IdentContext;
  class HexdigitContext;
  class WsContext; 

  class  StylesheetContext : public antlr4::ParserRuleContext {
  public:
    StylesheetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    std::vector<StyleRuleContext *> styleRule();
    StyleRuleContext* styleRule(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  StylesheetContext* stylesheet();

  class  StyleRuleContext : public antlr4::ParserRuleContext {
  public:
    StyleRuleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RuleSetContext *ruleSet();
    AtRuleContext *atRule();
    ChtlTemplateUsageContext *chtlTemplateUsage();
    ChtlCustomUsageContext *chtlCustomUsage();
    ChtlVariableUsageContext *chtlVariableUsage();
    ChtlOriginEmbedContext *chtlOriginEmbed();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  StyleRuleContext* styleRule();

  class  RuleSetContext : public antlr4::ParserRuleContext {
  public:
    RuleSetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SelectorListContext *selectorList();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    DeclarationListContext *declarationList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  RuleSetContext* ruleSet();

  class  SelectorListContext : public antlr4::ParserRuleContext {
  public:
    SelectorListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SelectorContext *> selector();
    SelectorContext* selector(size_t i);
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SelectorListContext* selectorList();

  class  SelectorContext : public antlr4::ParserRuleContext {
  public:
    SelectorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SimpleSelectorContext *> simpleSelector();
    SimpleSelectorContext* simpleSelector(size_t i);
    std::vector<CombinatorContext *> combinator();
    CombinatorContext* combinator(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SelectorContext* selector();

  class  CombinatorContext : public antlr4::ParserRuleContext {
  public:
    CombinatorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  CombinatorContext* combinator();

  class  SimpleSelectorContext : public antlr4::ParserRuleContext {
  public:
    SimpleSelectorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeSelectorContext *typeSelector();
    std::vector<QualifiersContext *> qualifiers();
    QualifiersContext* qualifiers(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SimpleSelectorContext* simpleSelector();

  class  TypeSelectorContext : public antlr4::ParserRuleContext {
  public:
    TypeSelectorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TagNameContext *tagName();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TypeSelectorContext* typeSelector();

  class  TagNameContext : public antlr4::ParserRuleContext {
  public:
    TagNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TagNameContext* tagName();

  class  QualifiersContext : public antlr4::ParserRuleContext {
  public:
    QualifiersContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    HashSelectorContext *hashSelector();
    ClassSelectorContext *classSelector();
    AttributeSelectorContext *attributeSelector();
    PseudoClassContext *pseudoClass();
    PseudoElementContext *pseudoElement();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  QualifiersContext* qualifiers();

  class  HashSelectorContext : public antlr4::ParserRuleContext {
  public:
    HashSelectorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  HashSelectorContext* hashSelector();

  class  ClassSelectorContext : public antlr4::ParserRuleContext {
  public:
    ClassSelectorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ClassSelectorContext* classSelector();

  class  AttributeSelectorContext : public antlr4::ParserRuleContext {
  public:
    AttributeSelectorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    AttributeOperatorContext *attributeOperator();
    AttributeValueContext *attributeValue();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AttributeSelectorContext* attributeSelector();

  class  AttributeOperatorContext : public antlr4::ParserRuleContext {
  public:
    AttributeOperatorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AttributeOperatorContext* attributeOperator();

  class  AttributeValueContext : public antlr4::ParserRuleContext {
  public:
    AttributeValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();
    StringContext *string();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AttributeValueContext* attributeValue();

  class  PseudoClassContext : public antlr4::ParserRuleContext {
  public:
    PseudoClassContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();
    ExpressionContext *expression();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PseudoClassContext* pseudoClass();

  class  PseudoElementContext : public antlr4::ParserRuleContext {
  public:
    PseudoElementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PseudoElementContext* pseudoElement();

  class  DeclarationListContext : public antlr4::ParserRuleContext {
  public:
    DeclarationListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<DeclarationContext *> declaration();
    DeclarationContext* declaration(size_t i);
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  DeclarationListContext* declarationList();

  class  DeclarationContext : public antlr4::ParserRuleContext {
  public:
    DeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PropertyContext *property();
    ValueListContext *valueList();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    PrioContext *prio();
    ChtlStyleGroupUsageContext *chtlStyleGroupUsage();
    ChtlDeletePropertyContext *chtlDeleteProperty();
    ChtlInheritContext *chtlInherit();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  DeclarationContext* declaration();

  class  PropertyContext : public antlr4::ParserRuleContext {
  public:
    PropertyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();
    VarContext *var();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PropertyContext* property();

  class  ValueListContext : public antlr4::ParserRuleContext {
  public:
    ValueListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ValueContext *> value();
    ValueContext* value(size_t i);
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ValueListContext* valueList();

  class  ValueContext : public antlr4::ParserRuleContext {
  public:
    ValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NumberContext *number();
    UnitContext *unit();
    PercentageContext *percentage();
    DimensionContext *dimension();
    StringContext *string();
    IdentContext *ident();
    UriContext *uri();
    HexcolorContext *hexcolor();
    CalcContext *calc();
    VarContext *var();
    ChtlVariableReferenceContext *chtlVariableReference();
    Function_Context *function_();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ValueContext* value();

  class  CalcContext : public antlr4::ParserRuleContext {
  public:
    CalcContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  CalcContext* calc();

  class  Function_Context : public antlr4::ParserRuleContext {
  public:
    Function_Context(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    ExpressionListContext *expressionList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Function_Context* function_();

  class  ExpressionListContext : public antlr4::ParserRuleContext {
  public:
    ExpressionListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ExpressionListContext* expressionList();

  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<TermContext *> term();
    TermContext* term(size_t i);
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ExpressionContext* expression();

  class  TermContext : public antlr4::ParserRuleContext {
  public:
    TermContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<FactorContext *> factor();
    FactorContext* factor(size_t i);
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TermContext* term();

  class  FactorContext : public antlr4::ParserRuleContext {
  public:
    FactorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NumberContext *number();
    UnitContext *unit();
    PercentageContext *percentage();
    DimensionContext *dimension();
    ExpressionContext *expression();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    ValueContext *value();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  FactorContext* factor();

  class  PrioContext : public antlr4::ParserRuleContext {
  public:
    PrioContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PrioContext* prio();

  class  ChtlTemplateUsageContext : public antlr4::ParserRuleContext {
  public:
    ChtlTemplateUsageContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    ChtlSpecializationContext *chtlSpecialization();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ChtlTemplateUsageContext* chtlTemplateUsage();

  class  ChtlCustomUsageContext : public antlr4::ParserRuleContext {
  public:
    ChtlCustomUsageContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    ChtlSpecializationContext *chtlSpecialization();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ChtlCustomUsageContext* chtlCustomUsage();

  class  ChtlSpecializationContext : public antlr4::ParserRuleContext {
  public:
    ChtlSpecializationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    DeclarationListContext *declarationList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ChtlSpecializationContext* chtlSpecialization();

  class  ChtlVariableUsageContext : public antlr4::ParserRuleContext {
  public:
    ChtlVariableUsageContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdentContext *> ident();
    IdentContext* ident(size_t i);
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ChtlVariableUsageContext* chtlVariableUsage();

  class  ChtlVariableReferenceContext : public antlr4::ParserRuleContext {
  public:
    ChtlVariableReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdentContext *> ident();
    IdentContext* ident(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ChtlVariableReferenceContext* chtlVariableReference();

  class  ChtlStyleGroupUsageContext : public antlr4::ParserRuleContext {
  public:
    ChtlStyleGroupUsageContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    ChtlSpecializationContext *chtlSpecialization();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ChtlStyleGroupUsageContext* chtlStyleGroupUsage();

  class  ChtlDeletePropertyContext : public antlr4::ParserRuleContext {
  public:
    ChtlDeletePropertyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PropertyContext *property();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ChtlDeletePropertyContext* chtlDeleteProperty();

  class  ChtlInheritContext : public antlr4::ParserRuleContext {
  public:
    ChtlInheritContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ChtlInheritContext* chtlInherit();

  class  ChtlOriginEmbedContext : public antlr4::ParserRuleContext {
  public:
    ChtlOriginEmbedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ChtlOriginEmbedContext* chtlOriginEmbed();

  class  AtRuleContext : public antlr4::ParserRuleContext {
  public:
    AtRuleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();
    AtRuleBlockContext *atRuleBlock();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AtRuleContext* atRule();

  class  AtRuleBlockContext : public antlr4::ParserRuleContext {
  public:
    AtRuleBlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    std::vector<StyleRuleContext *> styleRule();
    StyleRuleContext* styleRule(size_t i);
    std::vector<DeclarationContext *> declaration();
    DeclarationContext* declaration(size_t i);
    ValueListContext *valueList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AtRuleBlockContext* atRuleBlock();

  class  VarContext : public antlr4::ParserRuleContext {
  public:
    VarContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    ValueContext *value();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  VarContext* var();

  class  HexcolorContext : public antlr4::ParserRuleContext {
  public:
    HexcolorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<HexdigitContext *> hexdigit();
    HexdigitContext* hexdigit(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  HexcolorContext* hexcolor();

  class  NumberContext : public antlr4::ParserRuleContext {
  public:
    NumberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  NumberContext* number();

  class  PercentageContext : public antlr4::ParserRuleContext {
  public:
    PercentageContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NumberContext *number();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PercentageContext* percentage();

  class  DimensionContext : public antlr4::ParserRuleContext {
  public:
    DimensionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NumberContext *number();
    UnitContext *unit();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  DimensionContext* dimension();

  class  UnitContext : public antlr4::ParserRuleContext {
  public:
    UnitContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  UnitContext* unit();

  class  UriContext : public antlr4::ParserRuleContext {
  public:
    UriContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    StringContext *string();
    antlr4::tree::TerminalNode *URL_CONTENT();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  UriContext* uri();

  class  StringContext : public antlr4::ParserRuleContext {
  public:
    StringContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  StringContext* string();

  class  IdentContext : public antlr4::ParserRuleContext {
  public:
    IdentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  IdentContext* ident();

  class  HexdigitContext : public antlr4::ParserRuleContext {
  public:
    HexdigitContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *HEXDIGIT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  HexdigitContext* hexdigit();

  class  WsContext : public antlr4::ParserRuleContext {
  public:
    WsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WS();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  WsContext* ws();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};


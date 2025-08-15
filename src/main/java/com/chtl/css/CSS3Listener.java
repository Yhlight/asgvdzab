// Generated from com/chtl/css/CSS3.g4 by ANTLR 4.13.1
package com.chtl.css;
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link CSS3Parser}.
 */
public interface CSS3Listener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#stylesheet}.
	 * @param ctx the parse tree
	 */
	void enterStylesheet(CSS3Parser.StylesheetContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#stylesheet}.
	 * @param ctx the parse tree
	 */
	void exitStylesheet(CSS3Parser.StylesheetContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#charsetRule}.
	 * @param ctx the parse tree
	 */
	void enterCharsetRule(CSS3Parser.CharsetRuleContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#charsetRule}.
	 * @param ctx the parse tree
	 */
	void exitCharsetRule(CSS3Parser.CharsetRuleContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#importRule}.
	 * @param ctx the parse tree
	 */
	void enterImportRule(CSS3Parser.ImportRuleContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#importRule}.
	 * @param ctx the parse tree
	 */
	void exitImportRule(CSS3Parser.ImportRuleContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#namespaceRule}.
	 * @param ctx the parse tree
	 */
	void enterNamespaceRule(CSS3Parser.NamespaceRuleContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#namespaceRule}.
	 * @param ctx the parse tree
	 */
	void exitNamespaceRule(CSS3Parser.NamespaceRuleContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#namespacePrefix}.
	 * @param ctx the parse tree
	 */
	void enterNamespacePrefix(CSS3Parser.NamespacePrefixContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#namespacePrefix}.
	 * @param ctx the parse tree
	 */
	void exitNamespacePrefix(CSS3Parser.NamespacePrefixContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#mediaQueryList}.
	 * @param ctx the parse tree
	 */
	void enterMediaQueryList(CSS3Parser.MediaQueryListContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#mediaQueryList}.
	 * @param ctx the parse tree
	 */
	void exitMediaQueryList(CSS3Parser.MediaQueryListContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#mediaQuery}.
	 * @param ctx the parse tree
	 */
	void enterMediaQuery(CSS3Parser.MediaQueryContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#mediaQuery}.
	 * @param ctx the parse tree
	 */
	void exitMediaQuery(CSS3Parser.MediaQueryContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#mediaType}.
	 * @param ctx the parse tree
	 */
	void enterMediaType(CSS3Parser.MediaTypeContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#mediaType}.
	 * @param ctx the parse tree
	 */
	void exitMediaType(CSS3Parser.MediaTypeContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#mediaExpression}.
	 * @param ctx the parse tree
	 */
	void enterMediaExpression(CSS3Parser.MediaExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#mediaExpression}.
	 * @param ctx the parse tree
	 */
	void exitMediaExpression(CSS3Parser.MediaExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#mediaFeature}.
	 * @param ctx the parse tree
	 */
	void enterMediaFeature(CSS3Parser.MediaFeatureContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#mediaFeature}.
	 * @param ctx the parse tree
	 */
	void exitMediaFeature(CSS3Parser.MediaFeatureContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#statement}.
	 * @param ctx the parse tree
	 */
	void enterStatement(CSS3Parser.StatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#statement}.
	 * @param ctx the parse tree
	 */
	void exitStatement(CSS3Parser.StatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#ruleset}.
	 * @param ctx the parse tree
	 */
	void enterRuleset(CSS3Parser.RulesetContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#ruleset}.
	 * @param ctx the parse tree
	 */
	void exitRuleset(CSS3Parser.RulesetContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#selectors}.
	 * @param ctx the parse tree
	 */
	void enterSelectors(CSS3Parser.SelectorsContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#selectors}.
	 * @param ctx the parse tree
	 */
	void exitSelectors(CSS3Parser.SelectorsContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#selector}.
	 * @param ctx the parse tree
	 */
	void enterSelector(CSS3Parser.SelectorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#selector}.
	 * @param ctx the parse tree
	 */
	void exitSelector(CSS3Parser.SelectorContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#combinator}.
	 * @param ctx the parse tree
	 */
	void enterCombinator(CSS3Parser.CombinatorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#combinator}.
	 * @param ctx the parse tree
	 */
	void exitCombinator(CSS3Parser.CombinatorContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#simpleSelectorSequence}.
	 * @param ctx the parse tree
	 */
	void enterSimpleSelectorSequence(CSS3Parser.SimpleSelectorSequenceContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#simpleSelectorSequence}.
	 * @param ctx the parse tree
	 */
	void exitSimpleSelectorSequence(CSS3Parser.SimpleSelectorSequenceContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#typeSelector}.
	 * @param ctx the parse tree
	 */
	void enterTypeSelector(CSS3Parser.TypeSelectorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#typeSelector}.
	 * @param ctx the parse tree
	 */
	void exitTypeSelector(CSS3Parser.TypeSelectorContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#typeNamespacePrefix}.
	 * @param ctx the parse tree
	 */
	void enterTypeNamespacePrefix(CSS3Parser.TypeNamespacePrefixContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#typeNamespacePrefix}.
	 * @param ctx the parse tree
	 */
	void exitTypeNamespacePrefix(CSS3Parser.TypeNamespacePrefixContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#elementName}.
	 * @param ctx the parse tree
	 */
	void enterElementName(CSS3Parser.ElementNameContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#elementName}.
	 * @param ctx the parse tree
	 */
	void exitElementName(CSS3Parser.ElementNameContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#universal}.
	 * @param ctx the parse tree
	 */
	void enterUniversal(CSS3Parser.UniversalContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#universal}.
	 * @param ctx the parse tree
	 */
	void exitUniversal(CSS3Parser.UniversalContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#classSelector}.
	 * @param ctx the parse tree
	 */
	void enterClassSelector(CSS3Parser.ClassSelectorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#classSelector}.
	 * @param ctx the parse tree
	 */
	void exitClassSelector(CSS3Parser.ClassSelectorContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#attrib}.
	 * @param ctx the parse tree
	 */
	void enterAttrib(CSS3Parser.AttribContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#attrib}.
	 * @param ctx the parse tree
	 */
	void exitAttrib(CSS3Parser.AttribContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#attribOperator}.
	 * @param ctx the parse tree
	 */
	void enterAttribOperator(CSS3Parser.AttribOperatorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#attribOperator}.
	 * @param ctx the parse tree
	 */
	void exitAttribOperator(CSS3Parser.AttribOperatorContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#pseudo}.
	 * @param ctx the parse tree
	 */
	void enterPseudo(CSS3Parser.PseudoContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#pseudo}.
	 * @param ctx the parse tree
	 */
	void exitPseudo(CSS3Parser.PseudoContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#functionalPseudo}.
	 * @param ctx the parse tree
	 */
	void enterFunctionalPseudo(CSS3Parser.FunctionalPseudoContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#functionalPseudo}.
	 * @param ctx the parse tree
	 */
	void exitFunctionalPseudo(CSS3Parser.FunctionalPseudoContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#expression}.
	 * @param ctx the parse tree
	 */
	void enterExpression(CSS3Parser.ExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#expression}.
	 * @param ctx the parse tree
	 */
	void exitExpression(CSS3Parser.ExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#negation}.
	 * @param ctx the parse tree
	 */
	void enterNegation(CSS3Parser.NegationContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#negation}.
	 * @param ctx the parse tree
	 */
	void exitNegation(CSS3Parser.NegationContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#negationArg}.
	 * @param ctx the parse tree
	 */
	void enterNegationArg(CSS3Parser.NegationArgContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#negationArg}.
	 * @param ctx the parse tree
	 */
	void exitNegationArg(CSS3Parser.NegationArgContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#declarationList}.
	 * @param ctx the parse tree
	 */
	void enterDeclarationList(CSS3Parser.DeclarationListContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#declarationList}.
	 * @param ctx the parse tree
	 */
	void exitDeclarationList(CSS3Parser.DeclarationListContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#declaration}.
	 * @param ctx the parse tree
	 */
	void enterDeclaration(CSS3Parser.DeclarationContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#declaration}.
	 * @param ctx the parse tree
	 */
	void exitDeclaration(CSS3Parser.DeclarationContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#property}.
	 * @param ctx the parse tree
	 */
	void enterProperty(CSS3Parser.PropertyContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#property}.
	 * @param ctx the parse tree
	 */
	void exitProperty(CSS3Parser.PropertyContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#important}.
	 * @param ctx the parse tree
	 */
	void enterImportant(CSS3Parser.ImportantContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#important}.
	 * @param ctx the parse tree
	 */
	void exitImportant(CSS3Parser.ImportantContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#expr}.
	 * @param ctx the parse tree
	 */
	void enterExpr(CSS3Parser.ExprContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#expr}.
	 * @param ctx the parse tree
	 */
	void exitExpr(CSS3Parser.ExprContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#term}.
	 * @param ctx the parse tree
	 */
	void enterTerm(CSS3Parser.TermContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#term}.
	 * @param ctx the parse tree
	 */
	void exitTerm(CSS3Parser.TermContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#operator}.
	 * @param ctx the parse tree
	 */
	void enterOperator(CSS3Parser.OperatorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#operator}.
	 * @param ctx the parse tree
	 */
	void exitOperator(CSS3Parser.OperatorContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#unaryOperator}.
	 * @param ctx the parse tree
	 */
	void enterUnaryOperator(CSS3Parser.UnaryOperatorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#unaryOperator}.
	 * @param ctx the parse tree
	 */
	void exitUnaryOperator(CSS3Parser.UnaryOperatorContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#url}.
	 * @param ctx the parse tree
	 */
	void enterUrl(CSS3Parser.UrlContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#url}.
	 * @param ctx the parse tree
	 */
	void exitUrl(CSS3Parser.UrlContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#media}.
	 * @param ctx the parse tree
	 */
	void enterMedia(CSS3Parser.MediaContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#media}.
	 * @param ctx the parse tree
	 */
	void exitMedia(CSS3Parser.MediaContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#page}.
	 * @param ctx the parse tree
	 */
	void enterPage(CSS3Parser.PageContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#page}.
	 * @param ctx the parse tree
	 */
	void exitPage(CSS3Parser.PageContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#pseudoPage}.
	 * @param ctx the parse tree
	 */
	void enterPseudoPage(CSS3Parser.PseudoPageContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#pseudoPage}.
	 * @param ctx the parse tree
	 */
	void exitPseudoPage(CSS3Parser.PseudoPageContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#fontFace}.
	 * @param ctx the parse tree
	 */
	void enterFontFace(CSS3Parser.FontFaceContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#fontFace}.
	 * @param ctx the parse tree
	 */
	void exitFontFace(CSS3Parser.FontFaceContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#keyframes}.
	 * @param ctx the parse tree
	 */
	void enterKeyframes(CSS3Parser.KeyframesContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#keyframes}.
	 * @param ctx the parse tree
	 */
	void exitKeyframes(CSS3Parser.KeyframesContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#keyframesBlocks}.
	 * @param ctx the parse tree
	 */
	void enterKeyframesBlocks(CSS3Parser.KeyframesBlocksContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#keyframesBlocks}.
	 * @param ctx the parse tree
	 */
	void exitKeyframesBlocks(CSS3Parser.KeyframesBlocksContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#keyframeSelector}.
	 * @param ctx the parse tree
	 */
	void enterKeyframeSelector(CSS3Parser.KeyframeSelectorContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#keyframeSelector}.
	 * @param ctx the parse tree
	 */
	void exitKeyframeSelector(CSS3Parser.KeyframeSelectorContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#supports}.
	 * @param ctx the parse tree
	 */
	void enterSupports(CSS3Parser.SupportsContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#supports}.
	 * @param ctx the parse tree
	 */
	void exitSupports(CSS3Parser.SupportsContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#supportsCondition}.
	 * @param ctx the parse tree
	 */
	void enterSupportsCondition(CSS3Parser.SupportsConditionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#supportsCondition}.
	 * @param ctx the parse tree
	 */
	void exitSupportsCondition(CSS3Parser.SupportsConditionContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#supportsConditionInParens}.
	 * @param ctx the parse tree
	 */
	void enterSupportsConditionInParens(CSS3Parser.SupportsConditionInParensContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#supportsConditionInParens}.
	 * @param ctx the parse tree
	 */
	void exitSupportsConditionInParens(CSS3Parser.SupportsConditionInParensContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#supportsNegation}.
	 * @param ctx the parse tree
	 */
	void enterSupportsNegation(CSS3Parser.SupportsNegationContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#supportsNegation}.
	 * @param ctx the parse tree
	 */
	void exitSupportsNegation(CSS3Parser.SupportsNegationContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#supportsConjunction}.
	 * @param ctx the parse tree
	 */
	void enterSupportsConjunction(CSS3Parser.SupportsConjunctionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#supportsConjunction}.
	 * @param ctx the parse tree
	 */
	void exitSupportsConjunction(CSS3Parser.SupportsConjunctionContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#supportsDisjunction}.
	 * @param ctx the parse tree
	 */
	void enterSupportsDisjunction(CSS3Parser.SupportsDisjunctionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#supportsDisjunction}.
	 * @param ctx the parse tree
	 */
	void exitSupportsDisjunction(CSS3Parser.SupportsDisjunctionContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#supportsDeclarationCondition}.
	 * @param ctx the parse tree
	 */
	void enterSupportsDeclarationCondition(CSS3Parser.SupportsDeclarationConditionContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#supportsDeclarationCondition}.
	 * @param ctx the parse tree
	 */
	void exitSupportsDeclarationCondition(CSS3Parser.SupportsDeclarationConditionContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#generalEnclosed}.
	 * @param ctx the parse tree
	 */
	void enterGeneralEnclosed(CSS3Parser.GeneralEnclosedContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#generalEnclosed}.
	 * @param ctx the parse tree
	 */
	void exitGeneralEnclosed(CSS3Parser.GeneralEnclosedContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#any}.
	 * @param ctx the parse tree
	 */
	void enterAny(CSS3Parser.AnyContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#any}.
	 * @param ctx the parse tree
	 */
	void exitAny(CSS3Parser.AnyContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#viewport}.
	 * @param ctx the parse tree
	 */
	void enterViewport(CSS3Parser.ViewportContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#viewport}.
	 * @param ctx the parse tree
	 */
	void exitViewport(CSS3Parser.ViewportContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#counterStyle}.
	 * @param ctx the parse tree
	 */
	void enterCounterStyle(CSS3Parser.CounterStyleContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#counterStyle}.
	 * @param ctx the parse tree
	 */
	void exitCounterStyle(CSS3Parser.CounterStyleContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#fontFeatureValues}.
	 * @param ctx the parse tree
	 */
	void enterFontFeatureValues(CSS3Parser.FontFeatureValuesContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#fontFeatureValues}.
	 * @param ctx the parse tree
	 */
	void exitFontFeatureValues(CSS3Parser.FontFeatureValuesContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#fontFamilyNameList}.
	 * @param ctx the parse tree
	 */
	void enterFontFamilyNameList(CSS3Parser.FontFamilyNameListContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#fontFamilyNameList}.
	 * @param ctx the parse tree
	 */
	void exitFontFamilyNameList(CSS3Parser.FontFamilyNameListContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#fontFamilyName}.
	 * @param ctx the parse tree
	 */
	void enterFontFamilyName(CSS3Parser.FontFamilyNameContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#fontFamilyName}.
	 * @param ctx the parse tree
	 */
	void exitFontFamilyName(CSS3Parser.FontFamilyNameContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#fontFeatureValueBlock}.
	 * @param ctx the parse tree
	 */
	void enterFontFeatureValueBlock(CSS3Parser.FontFeatureValueBlockContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#fontFeatureValueBlock}.
	 * @param ctx the parse tree
	 */
	void exitFontFeatureValueBlock(CSS3Parser.FontFeatureValueBlockContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#fontFeatureType}.
	 * @param ctx the parse tree
	 */
	void enterFontFeatureType(CSS3Parser.FontFeatureTypeContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#fontFeatureType}.
	 * @param ctx the parse tree
	 */
	void exitFontFeatureType(CSS3Parser.FontFeatureTypeContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#fontFeatureValueList}.
	 * @param ctx the parse tree
	 */
	void enterFontFeatureValueList(CSS3Parser.FontFeatureValueListContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#fontFeatureValueList}.
	 * @param ctx the parse tree
	 */
	void exitFontFeatureValueList(CSS3Parser.FontFeatureValueListContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#fontFeatureValue}.
	 * @param ctx the parse tree
	 */
	void enterFontFeatureValue(CSS3Parser.FontFeatureValueContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#fontFeatureValue}.
	 * @param ctx the parse tree
	 */
	void exitFontFeatureValue(CSS3Parser.FontFeatureValueContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#groupRuleBody}.
	 * @param ctx the parse tree
	 */
	void enterGroupRuleBody(CSS3Parser.GroupRuleBodyContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#groupRuleBody}.
	 * @param ctx the parse tree
	 */
	void exitGroupRuleBody(CSS3Parser.GroupRuleBodyContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#unknownRule}.
	 * @param ctx the parse tree
	 */
	void enterUnknownRule(CSS3Parser.UnknownRuleContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#unknownRule}.
	 * @param ctx the parse tree
	 */
	void exitUnknownRule(CSS3Parser.UnknownRuleContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#unknownAtRule}.
	 * @param ctx the parse tree
	 */
	void enterUnknownAtRule(CSS3Parser.UnknownAtRuleContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#unknownAtRule}.
	 * @param ctx the parse tree
	 */
	void exitUnknownAtRule(CSS3Parser.UnknownAtRuleContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#unknownBlock}.
	 * @param ctx the parse tree
	 */
	void enterUnknownBlock(CSS3Parser.UnknownBlockContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#unknownBlock}.
	 * @param ctx the parse tree
	 */
	void exitUnknownBlock(CSS3Parser.UnknownBlockContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#cdoCdc}.
	 * @param ctx the parse tree
	 */
	void enterCdoCdc(CSS3Parser.CdoCdcContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#cdoCdc}.
	 * @param ctx the parse tree
	 */
	void exitCdoCdc(CSS3Parser.CdoCdcContext ctx);
	/**
	 * Enter a parse tree produced by {@link CSS3Parser#ws}.
	 * @param ctx the parse tree
	 */
	void enterWs(CSS3Parser.WsContext ctx);
	/**
	 * Exit a parse tree produced by {@link CSS3Parser#ws}.
	 * @param ctx the parse tree
	 */
	void exitWs(CSS3Parser.WsContext ctx);
}
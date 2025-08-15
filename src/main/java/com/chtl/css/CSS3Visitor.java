// Generated from com/chtl/css/CSS3.g4 by ANTLR 4.13.1
package com.chtl.css;
import org.antlr.v4.runtime.tree.ParseTreeVisitor;

/**
 * This interface defines a complete generic visitor for a parse tree produced
 * by {@link CSS3Parser}.
 *
 * @param <T> The return type of the visit operation. Use {@link Void} for
 * operations with no return type.
 */
public interface CSS3Visitor<T> extends ParseTreeVisitor<T> {
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#stylesheet}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitStylesheet(CSS3Parser.StylesheetContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#charsetRule}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitCharsetRule(CSS3Parser.CharsetRuleContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#importRule}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitImportRule(CSS3Parser.ImportRuleContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#namespaceRule}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitNamespaceRule(CSS3Parser.NamespaceRuleContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#namespacePrefix}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitNamespacePrefix(CSS3Parser.NamespacePrefixContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#mediaQueryList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitMediaQueryList(CSS3Parser.MediaQueryListContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#mediaQuery}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitMediaQuery(CSS3Parser.MediaQueryContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#mediaType}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitMediaType(CSS3Parser.MediaTypeContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#mediaExpression}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitMediaExpression(CSS3Parser.MediaExpressionContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#mediaFeature}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitMediaFeature(CSS3Parser.MediaFeatureContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#statement}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitStatement(CSS3Parser.StatementContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#ruleset}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitRuleset(CSS3Parser.RulesetContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#selectors}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitSelectors(CSS3Parser.SelectorsContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#selector}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitSelector(CSS3Parser.SelectorContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#combinator}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitCombinator(CSS3Parser.CombinatorContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#simpleSelectorSequence}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitSimpleSelectorSequence(CSS3Parser.SimpleSelectorSequenceContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#typeSelector}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeSelector(CSS3Parser.TypeSelectorContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#typeNamespacePrefix}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeNamespacePrefix(CSS3Parser.TypeNamespacePrefixContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#elementName}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitElementName(CSS3Parser.ElementNameContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#universal}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitUniversal(CSS3Parser.UniversalContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#classSelector}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitClassSelector(CSS3Parser.ClassSelectorContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#attrib}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAttrib(CSS3Parser.AttribContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#attribOperator}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAttribOperator(CSS3Parser.AttribOperatorContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#pseudo}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitPseudo(CSS3Parser.PseudoContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#functionalPseudo}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFunctionalPseudo(CSS3Parser.FunctionalPseudoContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#expression}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitExpression(CSS3Parser.ExpressionContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#negation}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitNegation(CSS3Parser.NegationContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#negationArg}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitNegationArg(CSS3Parser.NegationArgContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#declarationList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitDeclarationList(CSS3Parser.DeclarationListContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#declaration}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitDeclaration(CSS3Parser.DeclarationContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#property}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitProperty(CSS3Parser.PropertyContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#important}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitImportant(CSS3Parser.ImportantContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#expr}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitExpr(CSS3Parser.ExprContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#term}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTerm(CSS3Parser.TermContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#operator}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitOperator(CSS3Parser.OperatorContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#unaryOperator}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitUnaryOperator(CSS3Parser.UnaryOperatorContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#url}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitUrl(CSS3Parser.UrlContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#media}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitMedia(CSS3Parser.MediaContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#page}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitPage(CSS3Parser.PageContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#pseudoPage}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitPseudoPage(CSS3Parser.PseudoPageContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#fontFace}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFontFace(CSS3Parser.FontFaceContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#keyframes}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitKeyframes(CSS3Parser.KeyframesContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#keyframesBlocks}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitKeyframesBlocks(CSS3Parser.KeyframesBlocksContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#keyframeSelector}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitKeyframeSelector(CSS3Parser.KeyframeSelectorContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#supports}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitSupports(CSS3Parser.SupportsContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#supportsCondition}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitSupportsCondition(CSS3Parser.SupportsConditionContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#supportsConditionInParens}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitSupportsConditionInParens(CSS3Parser.SupportsConditionInParensContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#supportsNegation}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitSupportsNegation(CSS3Parser.SupportsNegationContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#supportsConjunction}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitSupportsConjunction(CSS3Parser.SupportsConjunctionContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#supportsDisjunction}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitSupportsDisjunction(CSS3Parser.SupportsDisjunctionContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#supportsDeclarationCondition}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitSupportsDeclarationCondition(CSS3Parser.SupportsDeclarationConditionContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#generalEnclosed}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitGeneralEnclosed(CSS3Parser.GeneralEnclosedContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#any}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAny(CSS3Parser.AnyContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#viewport}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitViewport(CSS3Parser.ViewportContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#counterStyle}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitCounterStyle(CSS3Parser.CounterStyleContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#fontFeatureValues}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFontFeatureValues(CSS3Parser.FontFeatureValuesContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#fontFamilyNameList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFontFamilyNameList(CSS3Parser.FontFamilyNameListContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#fontFamilyName}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFontFamilyName(CSS3Parser.FontFamilyNameContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#fontFeatureValueBlock}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFontFeatureValueBlock(CSS3Parser.FontFeatureValueBlockContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#fontFeatureType}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFontFeatureType(CSS3Parser.FontFeatureTypeContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#fontFeatureValueList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFontFeatureValueList(CSS3Parser.FontFeatureValueListContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#fontFeatureValue}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFontFeatureValue(CSS3Parser.FontFeatureValueContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#groupRuleBody}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitGroupRuleBody(CSS3Parser.GroupRuleBodyContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#unknownRule}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitUnknownRule(CSS3Parser.UnknownRuleContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#unknownAtRule}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitUnknownAtRule(CSS3Parser.UnknownAtRuleContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#unknownBlock}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitUnknownBlock(CSS3Parser.UnknownBlockContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#cdoCdc}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitCdoCdc(CSS3Parser.CdoCdcContext ctx);
	/**
	 * Visit a parse tree produced by {@link CSS3Parser#ws}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitWs(CSS3Parser.WsContext ctx);
}
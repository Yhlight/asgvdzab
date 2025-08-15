// Generated from com/chtl/css/CSS3.g4 by ANTLR 4.13.1
package com.chtl.css;import org.antlr.v4.runtime.Parser;
import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.atn.ATN;
import org.antlr.v4.runtime.tree.ParseTreeVisitor;

import com.chtl.css.CSS3Parser.AnyContext;
import com.chtl.css.CSS3Parser.AttribContext;
import com.chtl.css.CSS3Parser.AttribOperatorContext;
import com.chtl.css.CSS3Parser.CdoCdcContext;
import com.chtl.css.CSS3Parser.CharsetRuleContext;
import com.chtl.css.CSS3Parser.ClassSelectorContext;
import com.chtl.css.CSS3Parser.CombinatorContext;
import com.chtl.css.CSS3Parser.CounterStyleContext;
import com.chtl.css.CSS3Parser.DeclarationContext;
import com.chtl.css.CSS3Parser.DeclarationListContext;
import com.chtl.css.CSS3Parser.ElementNameContext;
import com.chtl.css.CSS3Parser.ExprContext;
import com.chtl.css.CSS3Parser.ExpressionContext;
import com.chtl.css.CSS3Parser.FontFaceContext;
import com.chtl.css.CSS3Parser.FontFamilyNameContext;
import com.chtl.css.CSS3Parser.FontFamilyNameListContext;
import com.chtl.css.CSS3Parser.FontFeatureTypeContext;
import com.chtl.css.CSS3Parser.FontFeatureValueBlockContext;
import com.chtl.css.CSS3Parser.FontFeatureValueContext;
import com.chtl.css.CSS3Parser.FontFeatureValueListContext;
import com.chtl.css.CSS3Parser.FontFeatureValuesContext;
import com.chtl.css.CSS3Parser.FunctionalPseudoContext;
import com.chtl.css.CSS3Parser.GeneralEnclosedContext;
import com.chtl.css.CSS3Parser.GroupRuleBodyContext;
import com.chtl.css.CSS3Parser.ImportRuleContext;
import com.chtl.css.CSS3Parser.ImportantContext;
import com.chtl.css.CSS3Parser.KeyframeSelectorContext;
import com.chtl.css.CSS3Parser.KeyframesBlocksContext;
import com.chtl.css.CSS3Parser.KeyframesContext;
import com.chtl.css.CSS3Parser.MediaContext;
import com.chtl.css.CSS3Parser.MediaExpressionContext;
import com.chtl.css.CSS3Parser.MediaFeatureContext;
import com.chtl.css.CSS3Parser.MediaQueryContext;
import com.chtl.css.CSS3Parser.MediaQueryListContext;
import com.chtl.css.CSS3Parser.MediaTypeContext;
import com.chtl.css.CSS3Parser.NamespacePrefixContext;
import com.chtl.css.CSS3Parser.NamespaceRuleContext;
import com.chtl.css.CSS3Parser.NegationArgContext;
import com.chtl.css.CSS3Parser.NegationContext;
import com.chtl.css.CSS3Parser.OperatorContext;
import com.chtl.css.CSS3Parser.PageContext;
import com.chtl.css.CSS3Parser.PropertyContext;
import com.chtl.css.CSS3Parser.PseudoContext;
import com.chtl.css.CSS3Parser.PseudoPageContext;
import com.chtl.css.CSS3Parser.RulesetContext;
import com.chtl.css.CSS3Parser.SelectorContext;
import com.chtl.css.CSS3Parser.SelectorsContext;
import com.chtl.css.CSS3Parser.SimpleSelectorSequenceContext;
import com.chtl.css.CSS3Parser.StatementContext;
import com.chtl.css.CSS3Parser.StylesheetContext;
import com.chtl.css.CSS3Parser.SupportsConditionContext;
import com.chtl.css.CSS3Parser.SupportsConditionInParensContext;
import com.chtl.css.CSS3Parser.SupportsConjunctionContext;
import com.chtl.css.CSS3Parser.SupportsContext;
import com.chtl.css.CSS3Parser.SupportsDeclarationConditionContext;
import com.chtl.css.CSS3Parser.SupportsDisjunctionContext;
import com.chtl.css.CSS3Parser.SupportsNegationContext;
import com.chtl.css.CSS3Parser.TermContext;
import com.chtl.css.CSS3Parser.TypeNamespacePrefixContext;
import com.chtl.css.CSS3Parser.TypeSelectorContext;
import com.chtl.css.CSS3Parser.UnaryOperatorContext;
import com.chtl.css.CSS3Parser.UniversalContext;
import com.chtl.css.CSS3Parser.UnknownAtRuleContext;
import com.chtl.css.CSS3Parser.UnknownBlockContext;
import com.chtl.css.CSS3Parser.UnknownRuleContext;
import com.chtl.css.CSS3Parser.UrlContext;
import com.chtl.css.CSS3Parser.ViewportContext;
import com.chtl.css.CSS3Parser.WsContext;

import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast", "CheckReturnValue"})
public class CSS3Parser extends Parser {
	static { RuntimeMetaData.checkVersion("4.13.1", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, T__7=8, T__8=9, 
		T__9=10, T__10=11, T__11=12, T__12=13, T__13=14, T__14=15, T__15=16, T__16=17, 
		T__17=18, T__18=19, T__19=20, T__20=21, T__21=22, T__22=23, T__23=24, 
		T__24=25, T__25=26, T__26=27, T__27=28, T__28=29, T__29=30, T__30=31, 
		T__31=32, T__32=33, T__33=34, T__34=35, T__35=36, T__36=37, T__37=38, 
		T__38=39, T__39=40, T__40=41, T__41=42, T__42=43, T__43=44, CDO=45, CDC=46, 
		INCLUDES=47, DASHMATCH=48, FROM=49, TO=50, KEYFRAMES=51, ONLY=52, NOT=53, 
		AND=54, OR=55, HASH=56, STRING=57, NUMBER=58, PERCENTAGE=59, LENGTH=60, 
		EMS=61, EXS=62, ANGLE=63, TIME=64, FREQ=65, RESOLUTION=66, UNICODE_RANGE=67, 
		IDENT=68, FUNCTION=69, URL=70, COMMENT=71, WS=72;
	public static final int
		RULE_stylesheet = 0, RULE_charsetRule = 1, RULE_importRule = 2, RULE_namespaceRule = 3, 
		RULE_namespacePrefix = 4, RULE_mediaQueryList = 5, RULE_mediaQuery = 6, 
		RULE_mediaType = 7, RULE_mediaExpression = 8, RULE_mediaFeature = 9, RULE_statement = 10, 
		RULE_ruleset = 11, RULE_selectors = 12, RULE_selector = 13, RULE_combinator = 14, 
		RULE_simpleSelectorSequence = 15, RULE_typeSelector = 16, RULE_typeNamespacePrefix = 17, 
		RULE_elementName = 18, RULE_universal = 19, RULE_classSelector = 20, RULE_attrib = 21, 
		RULE_attribOperator = 22, RULE_pseudo = 23, RULE_functionalPseudo = 24, 
		RULE_expression = 25, RULE_negation = 26, RULE_negationArg = 27, RULE_declarationList = 28, 
		RULE_declaration = 29, RULE_property = 30, RULE_important = 31, RULE_expr = 32, 
		RULE_term = 33, RULE_operator = 34, RULE_unaryOperator = 35, RULE_url = 36, 
		RULE_media = 37, RULE_page = 38, RULE_pseudoPage = 39, RULE_fontFace = 40, 
		RULE_keyframes = 41, RULE_keyframesBlocks = 42, RULE_keyframeSelector = 43, 
		RULE_supports = 44, RULE_supportsCondition = 45, RULE_supportsConditionInParens = 46, 
		RULE_supportsNegation = 47, RULE_supportsConjunction = 48, RULE_supportsDisjunction = 49, 
		RULE_supportsDeclarationCondition = 50, RULE_generalEnclosed = 51, RULE_any = 52, 
		RULE_viewport = 53, RULE_counterStyle = 54, RULE_fontFeatureValues = 55, 
		RULE_fontFamilyNameList = 56, RULE_fontFamilyName = 57, RULE_fontFeatureValueBlock = 58, 
		RULE_fontFeatureType = 59, RULE_fontFeatureValueList = 60, RULE_fontFeatureValue = 61, 
		RULE_groupRuleBody = 62, RULE_unknownRule = 63, RULE_unknownAtRule = 64, 
		RULE_unknownBlock = 65, RULE_cdoCdc = 66, RULE_ws = 67;
	private static String[] makeRuleNames() {
		return new String[] {
			"stylesheet", "charsetRule", "importRule", "namespaceRule", "namespacePrefix", 
			"mediaQueryList", "mediaQuery", "mediaType", "mediaExpression", "mediaFeature", 
			"statement", "ruleset", "selectors", "selector", "combinator", "simpleSelectorSequence", 
			"typeSelector", "typeNamespacePrefix", "elementName", "universal", "classSelector", 
			"attrib", "attribOperator", "pseudo", "functionalPseudo", "expression", 
			"negation", "negationArg", "declarationList", "declaration", "property", 
			"important", "expr", "term", "operator", "unaryOperator", "url", "media", 
			"page", "pseudoPage", "fontFace", "keyframes", "keyframesBlocks", "keyframeSelector", 
			"supports", "supportsCondition", "supportsConditionInParens", "supportsNegation", 
			"supportsConjunction", "supportsDisjunction", "supportsDeclarationCondition", 
			"generalEnclosed", "any", "viewport", "counterStyle", "fontFeatureValues", 
			"fontFamilyNameList", "fontFamilyName", "fontFeatureValueBlock", "fontFeatureType", 
			"fontFeatureValueList", "fontFeatureValue", "groupRuleBody", "unknownRule", 
			"unknownAtRule", "unknownBlock", "cdoCdc", "ws"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'@charset'", "';'", "'@import'", "'@namespace'", "','", "'('", 
			"':'", "')'", "'{'", "'}'", "'+'", "'>'", "'~'", "'//'", "'*'", "'|'", 
			"'.'", "'['", "']'", "'='", "'^='", "'$='", "'*='", "':not('", "'!'", 
			"'important'", "'/'", "'-'", "'url('", "'@media'", "'@page'", "'@font-face'", 
			"'@'", "'@supports'", "'@viewport'", "'@counter-style'", "'@font-feature-values'", 
			"'@stylistic'", "'@historical-forms'", "'@styleset'", "'@character-variant'", 
			"'@swash'", "'@ornaments'", "'@annotation'", "'<!--'", "'-->'", "'~='", 
			"'|='", "'from'", "'to'", null, "'only'", "'not'", "'and'", "'or'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, "CDO", "CDC", "INCLUDES", 
			"DASHMATCH", "FROM", "TO", "KEYFRAMES", "ONLY", "NOT", "AND", "OR", "HASH", 
			"STRING", "NUMBER", "PERCENTAGE", "LENGTH", "EMS", "EXS", "ANGLE", "TIME", 
			"FREQ", "RESOLUTION", "UNICODE_RANGE", "IDENT", "FUNCTION", "URL", "COMMENT", 
			"WS"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "CSS3.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public CSS3Parser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@SuppressWarnings("CheckReturnValue")
	public static class StylesheetContext extends ParserRuleContext {
		public TerminalNode EOF() { return getToken(CSS3Parser.EOF, 0); }
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public CharsetRuleContext charsetRule() {
			return getRuleContext(CharsetRuleContext.class,0);
		}
		public List<ImportRuleContext> importRule() {
			return getRuleContexts(ImportRuleContext.class);
		}
		public ImportRuleContext importRule(int i) {
			return getRuleContext(ImportRuleContext.class,i);
		}
		public List<NamespaceRuleContext> namespaceRule() {
			return getRuleContexts(NamespaceRuleContext.class);
		}
		public NamespaceRuleContext namespaceRule(int i) {
			return getRuleContext(NamespaceRuleContext.class,i);
		}
		public List<CdoCdcContext> cdoCdc() {
			return getRuleContexts(CdoCdcContext.class);
		}
		public CdoCdcContext cdoCdc(int i) {
			return getRuleContext(CdoCdcContext.class,i);
		}
		public List<StatementContext> statement() {
			return getRuleContexts(StatementContext.class);
		}
		public StatementContext statement(int i) {
			return getRuleContext(StatementContext.class,i);
		}
		public StylesheetContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_stylesheet; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterStylesheet(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitStylesheet(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitStylesheet(this);
			else return visitor.visitChildren(this);
		}
	}

	public final StylesheetContext stylesheet() throws RecognitionException {
		StylesheetContext _localctx = new StylesheetContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_stylesheet);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(137);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(136);
				ws();
				}
			}

			setState(140);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__0) {
				{
				setState(139);
				charsetRule();
				}
			}

			setState(148);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__2) {
				{
				{
				setState(142);
				importRule();
				setState(144);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==COMMENT || _la==WS) {
					{
					setState(143);
					ws();
					}
				}

				}
				}
				setState(150);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(157);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__3) {
				{
				{
				setState(151);
				namespaceRule();
				setState(153);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==COMMENT || _la==WS) {
					{
					setState(152);
					ws();
					}
				}

				}
				}
				setState(159);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(170);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (((((_la - 7)) & ~0x3f) == 0 && ((1L << (_la - 7)) & 2306406785940066049L) != 0)) {
				{
				setState(168);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case CDO:
				case CDC:
					{
					setState(160);
					cdoCdc();
					setState(162);
					_errHandler.sync(this);
					_la = _input.LA(1);
					if (_la==COMMENT || _la==WS) {
						{
						setState(161);
						ws();
						}
					}

					}
					break;
				case T__6:
				case T__14:
				case T__15:
				case T__16:
				case T__17:
				case T__23:
				case T__29:
				case T__30:
				case T__31:
				case T__32:
				case T__33:
				case T__34:
				case T__35:
				case T__36:
				case HASH:
				case IDENT:
					{
					setState(164);
					statement();
					setState(166);
					_errHandler.sync(this);
					_la = _input.LA(1);
					if (_la==COMMENT || _la==WS) {
						{
						setState(165);
						ws();
						}
					}

					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				setState(172);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(173);
			match(EOF);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class CharsetRuleContext extends ParserRuleContext {
		public TerminalNode STRING() { return getToken(CSS3Parser.STRING, 0); }
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public CharsetRuleContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_charsetRule; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterCharsetRule(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitCharsetRule(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitCharsetRule(this);
			else return visitor.visitChildren(this);
		}
	}

	public final CharsetRuleContext charsetRule() throws RecognitionException {
		CharsetRuleContext _localctx = new CharsetRuleContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_charsetRule);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(175);
			match(T__0);
			setState(177);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(176);
				ws();
				}
			}

			setState(179);
			match(STRING);
			setState(181);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(180);
				ws();
				}
			}

			setState(183);
			match(T__1);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ImportRuleContext extends ParserRuleContext {
		public TerminalNode STRING() { return getToken(CSS3Parser.STRING, 0); }
		public UrlContext url() {
			return getRuleContext(UrlContext.class,0);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public MediaQueryListContext mediaQueryList() {
			return getRuleContext(MediaQueryListContext.class,0);
		}
		public ImportRuleContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_importRule; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterImportRule(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitImportRule(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitImportRule(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ImportRuleContext importRule() throws RecognitionException {
		ImportRuleContext _localctx = new ImportRuleContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_importRule);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(185);
			match(T__2);
			setState(187);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(186);
				ws();
				}
			}

			setState(191);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case STRING:
				{
				setState(189);
				match(STRING);
				}
				break;
			case T__28:
				{
				setState(190);
				url();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			setState(194);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,14,_ctx) ) {
			case 1:
				{
				setState(193);
				ws();
				}
				break;
			}
			setState(197);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 13510798882111552L) != 0) || ((((_la - 68)) & ~0x3f) == 0 && ((1L << (_la - 68)) & 25L) != 0)) {
				{
				setState(196);
				mediaQueryList();
				}
			}

			setState(199);
			match(T__1);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class NamespaceRuleContext extends ParserRuleContext {
		public TerminalNode STRING() { return getToken(CSS3Parser.STRING, 0); }
		public UrlContext url() {
			return getRuleContext(UrlContext.class,0);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public NamespacePrefixContext namespacePrefix() {
			return getRuleContext(NamespacePrefixContext.class,0);
		}
		public NamespaceRuleContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_namespaceRule; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterNamespaceRule(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitNamespaceRule(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitNamespaceRule(this);
			else return visitor.visitChildren(this);
		}
	}

	public final NamespaceRuleContext namespaceRule() throws RecognitionException {
		NamespaceRuleContext _localctx = new NamespaceRuleContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_namespaceRule);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(201);
			match(T__3);
			setState(203);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(202);
				ws();
				}
			}

			setState(209);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==IDENT) {
				{
				setState(205);
				namespacePrefix();
				setState(207);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==COMMENT || _la==WS) {
					{
					setState(206);
					ws();
					}
				}

				}
			}

			setState(213);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case STRING:
				{
				setState(211);
				match(STRING);
				}
				break;
			case T__28:
				{
				setState(212);
				url();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			setState(216);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(215);
				ws();
				}
			}

			setState(218);
			match(T__1);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class NamespacePrefixContext extends ParserRuleContext {
		public TerminalNode IDENT() { return getToken(CSS3Parser.IDENT, 0); }
		public NamespacePrefixContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_namespacePrefix; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterNamespacePrefix(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitNamespacePrefix(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitNamespacePrefix(this);
			else return visitor.visitChildren(this);
		}
	}

	public final NamespacePrefixContext namespacePrefix() throws RecognitionException {
		NamespacePrefixContext _localctx = new NamespacePrefixContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_namespacePrefix);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(220);
			match(IDENT);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class MediaQueryListContext extends ParserRuleContext {
		public List<MediaQueryContext> mediaQuery() {
			return getRuleContexts(MediaQueryContext.class);
		}
		public MediaQueryContext mediaQuery(int i) {
			return getRuleContext(MediaQueryContext.class,i);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public MediaQueryListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_mediaQueryList; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterMediaQueryList(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitMediaQueryList(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitMediaQueryList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final MediaQueryListContext mediaQueryList() throws RecognitionException {
		MediaQueryListContext _localctx = new MediaQueryListContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_mediaQueryList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(223);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,21,_ctx) ) {
			case 1:
				{
				setState(222);
				ws();
				}
				break;
			}
			setState(225);
			mediaQuery();
			setState(236);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__4 || _la==COMMENT || _la==WS) {
				{
				{
				setState(227);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==COMMENT || _la==WS) {
					{
					setState(226);
					ws();
					}
				}

				setState(229);
				match(T__4);
				setState(231);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,23,_ctx) ) {
				case 1:
					{
					setState(230);
					ws();
					}
					break;
				}
				setState(233);
				mediaQuery();
				}
				}
				setState(238);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class MediaQueryContext extends ParserRuleContext {
		public MediaTypeContext mediaType() {
			return getRuleContext(MediaTypeContext.class,0);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public List<TerminalNode> AND() { return getTokens(CSS3Parser.AND); }
		public TerminalNode AND(int i) {
			return getToken(CSS3Parser.AND, i);
		}
		public List<MediaExpressionContext> mediaExpression() {
			return getRuleContexts(MediaExpressionContext.class);
		}
		public MediaExpressionContext mediaExpression(int i) {
			return getRuleContext(MediaExpressionContext.class,i);
		}
		public TerminalNode ONLY() { return getToken(CSS3Parser.ONLY, 0); }
		public TerminalNode NOT() { return getToken(CSS3Parser.NOT, 0); }
		public MediaQueryContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_mediaQuery; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterMediaQuery(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitMediaQuery(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitMediaQuery(this);
			else return visitor.visitChildren(this);
		}
	}

	public final MediaQueryContext mediaQuery() throws RecognitionException {
		MediaQueryContext _localctx = new MediaQueryContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_mediaQuery);
		int _la;
		try {
			int _alt;
			setState(273);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case ONLY:
			case NOT:
			case IDENT:
			case COMMENT:
			case WS:
				enterOuterAlt(_localctx, 1);
				{
				setState(240);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==ONLY || _la==NOT) {
					{
					setState(239);
					_la = _input.LA(1);
					if ( !(_la==ONLY || _la==NOT) ) {
					_errHandler.recoverInline(this);
					}
					else {
						if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
						_errHandler.reportMatch(this);
						consume();
					}
					}
				}

				setState(243);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==COMMENT || _la==WS) {
					{
					setState(242);
					ws();
					}
				}

				setState(245);
				mediaType();
				setState(247);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,27,_ctx) ) {
				case 1:
					{
					setState(246);
					ws();
					}
					break;
				}
				setState(256);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==AND) {
					{
					{
					setState(249);
					match(AND);
					setState(251);
					_errHandler.sync(this);
					_la = _input.LA(1);
					if (_la==COMMENT || _la==WS) {
						{
						setState(250);
						ws();
						}
					}

					setState(253);
					mediaExpression();
					}
					}
					setState(258);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
				break;
			case T__5:
				enterOuterAlt(_localctx, 2);
				{
				setState(259);
				mediaExpression();
				setState(270);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,32,_ctx);
				while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
					if ( _alt==1 ) {
						{
						{
						setState(261);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if (_la==COMMENT || _la==WS) {
							{
							setState(260);
							ws();
							}
						}

						setState(263);
						match(AND);
						setState(265);
						_errHandler.sync(this);
						_la = _input.LA(1);
						if (_la==COMMENT || _la==WS) {
							{
							setState(264);
							ws();
							}
						}

						setState(267);
						mediaExpression();
						}
						} 
					}
					setState(272);
					_errHandler.sync(this);
					_alt = getInterpreter().adaptivePredict(_input,32,_ctx);
				}
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class MediaTypeContext extends ParserRuleContext {
		public TerminalNode IDENT() { return getToken(CSS3Parser.IDENT, 0); }
		public MediaTypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_mediaType; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterMediaType(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitMediaType(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitMediaType(this);
			else return visitor.visitChildren(this);
		}
	}

	public final MediaTypeContext mediaType() throws RecognitionException {
		MediaTypeContext _localctx = new MediaTypeContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_mediaType);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(275);
			match(IDENT);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class MediaExpressionContext extends ParserRuleContext {
		public MediaFeatureContext mediaFeature() {
			return getRuleContext(MediaFeatureContext.class,0);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public MediaExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_mediaExpression; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterMediaExpression(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitMediaExpression(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitMediaExpression(this);
			else return visitor.visitChildren(this);
		}
	}

	public final MediaExpressionContext mediaExpression() throws RecognitionException {
		MediaExpressionContext _localctx = new MediaExpressionContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_mediaExpression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(277);
			match(T__5);
			setState(279);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(278);
				ws();
				}
			}

			setState(281);
			mediaFeature();
			setState(283);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,35,_ctx) ) {
			case 1:
				{
				setState(282);
				ws();
				}
				break;
			}
			setState(290);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__6) {
				{
				setState(285);
				match(T__6);
				setState(287);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==COMMENT || _la==WS) {
					{
					setState(286);
					ws();
					}
				}

				setState(289);
				expr();
				}
			}

			setState(293);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(292);
				ws();
				}
			}

			setState(295);
			match(T__7);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class MediaFeatureContext extends ParserRuleContext {
		public TerminalNode IDENT() { return getToken(CSS3Parser.IDENT, 0); }
		public MediaFeatureContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_mediaFeature; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterMediaFeature(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitMediaFeature(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitMediaFeature(this);
			else return visitor.visitChildren(this);
		}
	}

	public final MediaFeatureContext mediaFeature() throws RecognitionException {
		MediaFeatureContext _localctx = new MediaFeatureContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_mediaFeature);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(297);
			match(IDENT);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class StatementContext extends ParserRuleContext {
		public RulesetContext ruleset() {
			return getRuleContext(RulesetContext.class,0);
		}
		public MediaContext media() {
			return getRuleContext(MediaContext.class,0);
		}
		public PageContext page() {
			return getRuleContext(PageContext.class,0);
		}
		public FontFaceContext fontFace() {
			return getRuleContext(FontFaceContext.class,0);
		}
		public KeyframesContext keyframes() {
			return getRuleContext(KeyframesContext.class,0);
		}
		public SupportsContext supports() {
			return getRuleContext(SupportsContext.class,0);
		}
		public ViewportContext viewport() {
			return getRuleContext(ViewportContext.class,0);
		}
		public CounterStyleContext counterStyle() {
			return getRuleContext(CounterStyleContext.class,0);
		}
		public FontFeatureValuesContext fontFeatureValues() {
			return getRuleContext(FontFeatureValuesContext.class,0);
		}
		public UnknownRuleContext unknownRule() {
			return getRuleContext(UnknownRuleContext.class,0);
		}
		public StatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_statement; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterStatement(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitStatement(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitStatement(this);
			else return visitor.visitChildren(this);
		}
	}

	public final StatementContext statement() throws RecognitionException {
		StatementContext _localctx = new StatementContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_statement);
		try {
			setState(309);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,39,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(299);
				ruleset();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(300);
				media();
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(301);
				page();
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(302);
				fontFace();
				}
				break;
			case 5:
				enterOuterAlt(_localctx, 5);
				{
				setState(303);
				keyframes();
				}
				break;
			case 6:
				enterOuterAlt(_localctx, 6);
				{
				setState(304);
				supports();
				}
				break;
			case 7:
				enterOuterAlt(_localctx, 7);
				{
				setState(305);
				viewport();
				}
				break;
			case 8:
				enterOuterAlt(_localctx, 8);
				{
				setState(306);
				counterStyle();
				}
				break;
			case 9:
				enterOuterAlt(_localctx, 9);
				{
				setState(307);
				fontFeatureValues();
				}
				break;
			case 10:
				enterOuterAlt(_localctx, 10);
				{
				setState(308);
				unknownRule();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class RulesetContext extends ParserRuleContext {
		public SelectorsContext selectors() {
			return getRuleContext(SelectorsContext.class,0);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public DeclarationListContext declarationList() {
			return getRuleContext(DeclarationListContext.class,0);
		}
		public RulesetContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_ruleset; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterRuleset(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitRuleset(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitRuleset(this);
			else return visitor.visitChildren(this);
		}
	}

	public final RulesetContext ruleset() throws RecognitionException {
		RulesetContext _localctx = new RulesetContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_ruleset);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(311);
			selectors();
			setState(313);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(312);
				ws();
				}
			}

			setState(315);
			match(T__8);
			setState(317);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(316);
				ws();
				}
			}

			setState(320);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,42,_ctx) ) {
			case 1:
				{
				setState(319);
				declarationList();
				}
				break;
			}
			setState(322);
			match(T__9);
			setState(324);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,43,_ctx) ) {
			case 1:
				{
				setState(323);
				ws();
				}
				break;
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class SelectorsContext extends ParserRuleContext {
		public List<SelectorContext> selector() {
			return getRuleContexts(SelectorContext.class);
		}
		public SelectorContext selector(int i) {
			return getRuleContext(SelectorContext.class,i);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public SelectorsContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_selectors; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterSelectors(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitSelectors(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitSelectors(this);
			else return visitor.visitChildren(this);
		}
	}

	public final SelectorsContext selectors() throws RecognitionException {
		SelectorsContext _localctx = new SelectorsContext(_ctx, getState());
		enterRule(_localctx, 24, RULE_selectors);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(326);
			selector();
			setState(337);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,46,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(328);
					_errHandler.sync(this);
					_la = _input.LA(1);
					if (_la==COMMENT || _la==WS) {
						{
						setState(327);
						ws();
						}
					}

					setState(330);
					match(T__4);
					setState(332);
					_errHandler.sync(this);
					_la = _input.LA(1);
					if (_la==COMMENT || _la==WS) {
						{
						setState(331);
						ws();
						}
					}

					setState(334);
					selector();
					}
					} 
				}
				setState(339);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,46,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class SelectorContext extends ParserRuleContext {
		public List<SimpleSelectorSequenceContext> simpleSelectorSequence() {
			return getRuleContexts(SimpleSelectorSequenceContext.class);
		}
		public SimpleSelectorSequenceContext simpleSelectorSequence(int i) {
			return getRuleContext(SimpleSelectorSequenceContext.class,i);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public List<CombinatorContext> combinator() {
			return getRuleContexts(CombinatorContext.class);
		}
		public CombinatorContext combinator(int i) {
			return getRuleContext(CombinatorContext.class,i);
		}
		public SelectorContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_selector; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterSelector(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitSelector(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitSelector(this);
			else return visitor.visitChildren(this);
		}
	}

	public final SelectorContext selector() throws RecognitionException {
		SelectorContext _localctx = new SelectorContext(_ctx, getState());
		enterRule(_localctx, 26, RULE_selector);
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(340);
			simpleSelectorSequence();
			setState(342);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,47,_ctx) ) {
			case 1:
				{
				setState(341);
				ws();
				}
				break;
			}
			setState(351);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,49,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(344);
					combinator();
					setState(345);
					simpleSelectorSequence();
					setState(347);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,48,_ctx) ) {
					case 1:
						{
						setState(346);
						ws();
						}
						break;
					}
					}
					} 
				}
				setState(353);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,49,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class CombinatorContext extends ParserRuleContext {
		public WsContext ws() {
			return getRuleContext(WsContext.class,0);
		}
		public CombinatorContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_combinator; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterCombinator(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitCombinator(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitCombinator(this);
			else return visitor.visitChildren(this);
		}
	}

	public final CombinatorContext combinator() throws RecognitionException {
		CombinatorContext _localctx = new CombinatorContext(_ctx, getState());
		enterRule(_localctx, 28, RULE_combinator);
		int _la;
		try {
			setState(359);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,51,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(355);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==COMMENT || _la==WS) {
					{
					setState(354);
					ws();
					}
				}

				setState(357);
				_la = _input.LA(1);
				if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 30720L) != 0)) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(358);
				ws();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class SimpleSelectorSequenceContext extends ParserRuleContext {
		public TypeSelectorContext typeSelector() {
			return getRuleContext(TypeSelectorContext.class,0);
		}
		public UniversalContext universal() {
			return getRuleContext(UniversalContext.class,0);
		}
		public List<TerminalNode> HASH() { return getTokens(CSS3Parser.HASH); }
		public TerminalNode HASH(int i) {
			return getToken(CSS3Parser.HASH, i);
		}
		public List<ClassSelectorContext> classSelector() {
			return getRuleContexts(ClassSelectorContext.class);
		}
		public ClassSelectorContext classSelector(int i) {
			return getRuleContext(ClassSelectorContext.class,i);
		}
		public List<AttribContext> attrib() {
			return getRuleContexts(AttribContext.class);
		}
		public AttribContext attrib(int i) {
			return getRuleContext(AttribContext.class,i);
		}
		public List<PseudoContext> pseudo() {
			return getRuleContexts(PseudoContext.class);
		}
		public PseudoContext pseudo(int i) {
			return getRuleContext(PseudoContext.class,i);
		}
		public List<NegationContext> negation() {
			return getRuleContexts(NegationContext.class);
		}
		public NegationContext negation(int i) {
			return getRuleContext(NegationContext.class,i);
		}
		public SimpleSelectorSequenceContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_simpleSelectorSequence; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterSimpleSelectorSequence(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitSimpleSelectorSequence(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitSimpleSelectorSequence(this);
			else return visitor.visitChildren(this);
		}
	}

	public final SimpleSelectorSequenceContext simpleSelectorSequence() throws RecognitionException {
		SimpleSelectorSequenceContext _localctx = new SimpleSelectorSequenceContext(_ctx, getState());
		enterRule(_localctx, 30, RULE_simpleSelectorSequence);
		int _la;
		try {
			setState(384);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__14:
			case T__15:
			case IDENT:
				enterOuterAlt(_localctx, 1);
				{
				setState(363);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,52,_ctx) ) {
				case 1:
					{
					setState(361);
					typeSelector();
					}
					break;
				case 2:
					{
					setState(362);
					universal();
					}
					break;
				}
				setState(372);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 72057594055098496L) != 0)) {
					{
					setState(370);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case HASH:
						{
						setState(365);
						match(HASH);
						}
						break;
					case T__16:
						{
						setState(366);
						classSelector();
						}
						break;
					case T__17:
						{
						setState(367);
						attrib();
						}
						break;
					case T__6:
						{
						setState(368);
						pseudo();
						}
						break;
					case T__23:
						{
						setState(369);
						negation();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					setState(374);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
				break;
			case T__6:
			case T__16:
			case T__17:
			case T__23:
			case HASH:
				enterOuterAlt(_localctx, 2);
				{
				setState(380); 
				_errHandler.sync(this);
				_la = _input.LA(1);
				do {
					{
					setState(380);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case HASH:
						{
						setState(375);
						match(HASH);
						}
						break;
					case T__16:
						{
						setState(376);
						classSelector();
						}
						break;
					case T__17:
						{
						setState(377);
						attrib();
						}
						break;
					case T__6:
						{
						setState(378);
						pseudo();
						}
						break;
					case T__23:
						{
						setState(379);
						negation();
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					}
					setState(382); 
					_errHandler.sync(this);
					_la = _input.LA(1);
				} while ( (((_la) & ~0x3f) == 0 && ((1L << _la) & 72057594055098496L) != 0) );
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class TypeSelectorContext extends ParserRuleContext {
		public ElementNameContext elementName() {
			return getRuleContext(ElementNameContext.class,0);
		}
		public TypeNamespacePrefixContext typeNamespacePrefix() {
			return getRuleContext(TypeNamespacePrefixContext.class,0);
		}
		public TypeSelectorContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_typeSelector; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterTypeSelector(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitTypeSelector(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitTypeSelector(this);
			else return visitor.visitChildren(this);
		}
	}

	public final TypeSelectorContext typeSelector() throws RecognitionException {
		TypeSelectorContext _localctx = new TypeSelectorContext(_ctx, getState());
		enterRule(_localctx, 32, RULE_typeSelector);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(387);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,58,_ctx) ) {
			case 1:
				{
				setState(386);
				typeNamespacePrefix();
				}
				break;
			}
			setState(389);
			elementName();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class TypeNamespacePrefixContext extends ParserRuleContext {
		public NamespacePrefixContext namespacePrefix() {
			return getRuleContext(NamespacePrefixContext.class,0);
		}
		public TypeNamespacePrefixContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_typeNamespacePrefix; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterTypeNamespacePrefix(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitTypeNamespacePrefix(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitTypeNamespacePrefix(this);
			else return visitor.visitChildren(this);
		}
	}

	public final TypeNamespacePrefixContext typeNamespacePrefix() throws RecognitionException {
		TypeNamespacePrefixContext _localctx = new TypeNamespacePrefixContext(_ctx, getState());
		enterRule(_localctx, 34, RULE_typeNamespacePrefix);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(393);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case IDENT:
				{
				setState(391);
				namespacePrefix();
				}
				break;
			case T__14:
				{
				setState(392);
				match(T__14);
				}
				break;
			case T__15:
				break;
			default:
				break;
			}
			setState(395);
			match(T__15);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ElementNameContext extends ParserRuleContext {
		public TerminalNode IDENT() { return getToken(CSS3Parser.IDENT, 0); }
		public ElementNameContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_elementName; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterElementName(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitElementName(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitElementName(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ElementNameContext elementName() throws RecognitionException {
		ElementNameContext _localctx = new ElementNameContext(_ctx, getState());
		enterRule(_localctx, 36, RULE_elementName);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(397);
			match(IDENT);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class UniversalContext extends ParserRuleContext {
		public TypeNamespacePrefixContext typeNamespacePrefix() {
			return getRuleContext(TypeNamespacePrefixContext.class,0);
		}
		public UniversalContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_universal; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterUniversal(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitUniversal(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitUniversal(this);
			else return visitor.visitChildren(this);
		}
	}

	public final UniversalContext universal() throws RecognitionException {
		UniversalContext _localctx = new UniversalContext(_ctx, getState());
		enterRule(_localctx, 38, RULE_universal);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(400);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,60,_ctx) ) {
			case 1:
				{
				setState(399);
				typeNamespacePrefix();
				}
				break;
			}
			setState(402);
			match(T__14);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ClassSelectorContext extends ParserRuleContext {
		public TerminalNode IDENT() { return getToken(CSS3Parser.IDENT, 0); }
		public ClassSelectorContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_classSelector; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterClassSelector(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitClassSelector(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitClassSelector(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ClassSelectorContext classSelector() throws RecognitionException {
		ClassSelectorContext _localctx = new ClassSelectorContext(_ctx, getState());
		enterRule(_localctx, 40, RULE_classSelector);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(404);
			match(T__16);
			setState(405);
			match(IDENT);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class AttribContext extends ParserRuleContext {
		public List<TerminalNode> IDENT() { return getTokens(CSS3Parser.IDENT); }
		public TerminalNode IDENT(int i) {
			return getToken(CSS3Parser.IDENT, i);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public TypeNamespacePrefixContext typeNamespacePrefix() {
			return getRuleContext(TypeNamespacePrefixContext.class,0);
		}
		public AttribOperatorContext attribOperator() {
			return getRuleContext(AttribOperatorContext.class,0);
		}
		public TerminalNode STRING() { return getToken(CSS3Parser.STRING, 0); }
		public AttribContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_attrib; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterAttrib(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitAttrib(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitAttrib(this);
			else return visitor.visitChildren(this);
		}
	}

	public final AttribContext attrib() throws RecognitionException {
		AttribContext _localctx = new AttribContext(_ctx, getState());
		enterRule(_localctx, 42, RULE_attrib);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(407);
			match(T__17);
			setState(409);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(408);
				ws();
				}
			}

			setState(412);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,62,_ctx) ) {
			case 1:
				{
				setState(411);
				typeNamespacePrefix();
				}
				break;
			}
			setState(414);
			match(IDENT);
			setState(416);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(415);
				ws();
				}
			}

			setState(426);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 422212480794624L) != 0)) {
				{
				setState(418);
				attribOperator();
				setState(420);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==COMMENT || _la==WS) {
					{
					setState(419);
					ws();
					}
				}

				setState(422);
				_la = _input.LA(1);
				if ( !(_la==STRING || _la==IDENT) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(424);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==COMMENT || _la==WS) {
					{
					setState(423);
					ws();
					}
				}

				}
			}

			setState(428);
			match(T__18);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class AttribOperatorContext extends ParserRuleContext {
		public TerminalNode INCLUDES() { return getToken(CSS3Parser.INCLUDES, 0); }
		public TerminalNode DASHMATCH() { return getToken(CSS3Parser.DASHMATCH, 0); }
		public AttribOperatorContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_attribOperator; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterAttribOperator(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitAttribOperator(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitAttribOperator(this);
			else return visitor.visitChildren(this);
		}
	}

	public final AttribOperatorContext attribOperator() throws RecognitionException {
		AttribOperatorContext _localctx = new AttribOperatorContext(_ctx, getState());
		enterRule(_localctx, 44, RULE_attribOperator);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(430);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 422212480794624L) != 0)) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class PseudoContext extends ParserRuleContext {
		public TerminalNode IDENT() { return getToken(CSS3Parser.IDENT, 0); }
		public FunctionalPseudoContext functionalPseudo() {
			return getRuleContext(FunctionalPseudoContext.class,0);
		}
		public PseudoContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_pseudo; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterPseudo(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitPseudo(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitPseudo(this);
			else return visitor.visitChildren(this);
		}
	}

	public final PseudoContext pseudo() throws RecognitionException {
		PseudoContext _localctx = new PseudoContext(_ctx, getState());
		enterRule(_localctx, 46, RULE_pseudo);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(432);
			match(T__6);
			setState(434);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__6) {
				{
				setState(433);
				match(T__6);
				}
			}

			setState(438);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case IDENT:
				{
				setState(436);
				match(IDENT);
				}
				break;
			case FUNCTION:
				{
				setState(437);
				functionalPseudo();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class FunctionalPseudoContext extends ParserRuleContext {
		public TerminalNode FUNCTION() { return getToken(CSS3Parser.FUNCTION, 0); }
		public ExpressionContext expression() {
			return getRuleContext(ExpressionContext.class,0);
		}
		public WsContext ws() {
			return getRuleContext(WsContext.class,0);
		}
		public FunctionalPseudoContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_functionalPseudo; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterFunctionalPseudo(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitFunctionalPseudo(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitFunctionalPseudo(this);
			else return visitor.visitChildren(this);
		}
	}

	public final FunctionalPseudoContext functionalPseudo() throws RecognitionException {
		FunctionalPseudoContext _localctx = new FunctionalPseudoContext(_ctx, getState());
		enterRule(_localctx, 48, RULE_functionalPseudo);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(440);
			match(FUNCTION);
			setState(442);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(441);
				ws();
				}
			}

			setState(444);
			expression();
			setState(445);
			match(T__7);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ExpressionContext extends ParserRuleContext {
		public List<TermContext> term() {
			return getRuleContexts(TermContext.class);
		}
		public TermContext term(int i) {
			return getRuleContext(TermContext.class,i);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public ExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expression; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterExpression(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitExpression(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitExpression(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ExpressionContext expression() throws RecognitionException {
		ExpressionContext _localctx = new ExpressionContext(_ctx, getState());
		enterRule(_localctx, 50, RULE_expression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(451); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(447);
				term();
				setState(449);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==COMMENT || _la==WS) {
					{
					setState(448);
					ws();
					}
				}

				}
				}
				setState(453); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( ((((_la - 7)) & ~0x3f) == 0 && ((1L << (_la - 7)) & 9222812385442529297L) != 0) );
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class NegationContext extends ParserRuleContext {
		public NegationArgContext negationArg() {
			return getRuleContext(NegationArgContext.class,0);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public NegationContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_negation; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterNegation(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitNegation(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitNegation(this);
			else return visitor.visitChildren(this);
		}
	}

	public final NegationContext negation() throws RecognitionException {
		NegationContext _localctx = new NegationContext(_ctx, getState());
		enterRule(_localctx, 52, RULE_negation);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(455);
			match(T__23);
			setState(457);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(456);
				ws();
				}
			}

			setState(459);
			negationArg();
			setState(461);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(460);
				ws();
				}
			}

			setState(463);
			match(T__7);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class NegationArgContext extends ParserRuleContext {
		public TypeSelectorContext typeSelector() {
			return getRuleContext(TypeSelectorContext.class,0);
		}
		public UniversalContext universal() {
			return getRuleContext(UniversalContext.class,0);
		}
		public TerminalNode HASH() { return getToken(CSS3Parser.HASH, 0); }
		public ClassSelectorContext classSelector() {
			return getRuleContext(ClassSelectorContext.class,0);
		}
		public AttribContext attrib() {
			return getRuleContext(AttribContext.class,0);
		}
		public PseudoContext pseudo() {
			return getRuleContext(PseudoContext.class,0);
		}
		public NegationArgContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_negationArg; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterNegationArg(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitNegationArg(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitNegationArg(this);
			else return visitor.visitChildren(this);
		}
	}

	public final NegationArgContext negationArg() throws RecognitionException {
		NegationArgContext _localctx = new NegationArgContext(_ctx, getState());
		enterRule(_localctx, 54, RULE_negationArg);
		try {
			setState(471);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,74,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(465);
				typeSelector();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(466);
				universal();
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(467);
				match(HASH);
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(468);
				classSelector();
				}
				break;
			case 5:
				enterOuterAlt(_localctx, 5);
				{
				setState(469);
				attrib();
				}
				break;
			case 6:
				enterOuterAlt(_localctx, 6);
				{
				setState(470);
				pseudo();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class DeclarationListContext extends ParserRuleContext {
		public List<DeclarationContext> declaration() {
			return getRuleContexts(DeclarationContext.class);
		}
		public DeclarationContext declaration(int i) {
			return getRuleContext(DeclarationContext.class,i);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public DeclarationListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_declarationList; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterDeclarationList(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitDeclarationList(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitDeclarationList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final DeclarationListContext declarationList() throws RecognitionException {
		DeclarationListContext _localctx = new DeclarationListContext(_ctx, getState());
		enterRule(_localctx, 56, RULE_declarationList);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(480);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,76,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(473);
					declaration();
					setState(474);
					match(T__1);
					setState(476);
					_errHandler.sync(this);
					_la = _input.LA(1);
					if (_la==COMMENT || _la==WS) {
						{
						setState(475);
						ws();
						}
					}

					}
					} 
				}
				setState(482);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,76,_ctx);
			}
			setState(484);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==IDENT) {
				{
				setState(483);
				declaration();
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class DeclarationContext extends ParserRuleContext {
		public PropertyContext property() {
			return getRuleContext(PropertyContext.class,0);
		}
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public ImportantContext important() {
			return getRuleContext(ImportantContext.class,0);
		}
		public DeclarationContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_declaration; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterDeclaration(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitDeclaration(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitDeclaration(this);
			else return visitor.visitChildren(this);
		}
	}

	public final DeclarationContext declaration() throws RecognitionException {
		DeclarationContext _localctx = new DeclarationContext(_ctx, getState());
		enterRule(_localctx, 58, RULE_declaration);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(486);
			property();
			setState(488);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(487);
				ws();
				}
			}

			setState(490);
			match(T__6);
			setState(492);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(491);
				ws();
				}
			}

			setState(494);
			expr();
			setState(496);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,80,_ctx) ) {
			case 1:
				{
				setState(495);
				important();
				}
				break;
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class PropertyContext extends ParserRuleContext {
		public TerminalNode IDENT() { return getToken(CSS3Parser.IDENT, 0); }
		public PropertyContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_property; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterProperty(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitProperty(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitProperty(this);
			else return visitor.visitChildren(this);
		}
	}

	public final PropertyContext property() throws RecognitionException {
		PropertyContext _localctx = new PropertyContext(_ctx, getState());
		enterRule(_localctx, 60, RULE_property);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(498);
			match(IDENT);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ImportantContext extends ParserRuleContext {
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public ImportantContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_important; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterImportant(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitImportant(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitImportant(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ImportantContext important() throws RecognitionException {
		ImportantContext _localctx = new ImportantContext(_ctx, getState());
		enterRule(_localctx, 62, RULE_important);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(501);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(500);
				ws();
				}
			}

			setState(503);
			match(T__24);
			setState(505);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(504);
				ws();
				}
			}

			setState(507);
			match(T__25);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ExprContext extends ParserRuleContext {
		public List<TermContext> term() {
			return getRuleContexts(TermContext.class);
		}
		public TermContext term(int i) {
			return getRuleContext(TermContext.class,i);
		}
		public List<OperatorContext> operator() {
			return getRuleContexts(OperatorContext.class);
		}
		public OperatorContext operator(int i) {
			return getRuleContext(OperatorContext.class,i);
		}
		public ExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expr; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterExpr(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitExpr(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitExpr(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ExprContext expr() throws RecognitionException {
		ExprContext _localctx = new ExprContext(_ctx, getState());
		enterRule(_localctx, 64, RULE_expr);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(509);
			term();
			setState(516);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,84,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(511);
					_errHandler.sync(this);
					_la = _input.LA(1);
					if (_la==T__4 || _la==T__26 || _la==COMMENT || _la==WS) {
						{
						setState(510);
						operator();
						}
					}

					setState(513);
					term();
					}
					} 
				}
				setState(518);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,84,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class TermContext extends ParserRuleContext {
		public TerminalNode NUMBER() { return getToken(CSS3Parser.NUMBER, 0); }
		public TerminalNode PERCENTAGE() { return getToken(CSS3Parser.PERCENTAGE, 0); }
		public TerminalNode LENGTH() { return getToken(CSS3Parser.LENGTH, 0); }
		public TerminalNode EMS() { return getToken(CSS3Parser.EMS, 0); }
		public TerminalNode EXS() { return getToken(CSS3Parser.EXS, 0); }
		public TerminalNode ANGLE() { return getToken(CSS3Parser.ANGLE, 0); }
		public TerminalNode TIME() { return getToken(CSS3Parser.TIME, 0); }
		public TerminalNode FREQ() { return getToken(CSS3Parser.FREQ, 0); }
		public TerminalNode RESOLUTION() { return getToken(CSS3Parser.RESOLUTION, 0); }
		public UnaryOperatorContext unaryOperator() {
			return getRuleContext(UnaryOperatorContext.class,0);
		}
		public TerminalNode STRING() { return getToken(CSS3Parser.STRING, 0); }
		public TerminalNode IDENT() { return getToken(CSS3Parser.IDENT, 0); }
		public UrlContext url() {
			return getRuleContext(UrlContext.class,0);
		}
		public TerminalNode HASH() { return getToken(CSS3Parser.HASH, 0); }
		public TerminalNode UNICODE_RANGE() { return getToken(CSS3Parser.UNICODE_RANGE, 0); }
		public TerminalNode INCLUDES() { return getToken(CSS3Parser.INCLUDES, 0); }
		public TerminalNode DASHMATCH() { return getToken(CSS3Parser.DASHMATCH, 0); }
		public TerminalNode FUNCTION() { return getToken(CSS3Parser.FUNCTION, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public WsContext ws() {
			return getRuleContext(WsContext.class,0);
		}
		public TermContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_term; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterTerm(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitTerm(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitTerm(this);
			else return visitor.visitChildren(this);
		}
	}

	public final TermContext term() throws RecognitionException {
		TermContext _localctx = new TermContext(_ctx, getState());
		enterRule(_localctx, 66, RULE_term);
		int _la;
		try {
			setState(539);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__10:
			case T__27:
			case NUMBER:
			case PERCENTAGE:
			case LENGTH:
			case EMS:
			case EXS:
			case ANGLE:
			case TIME:
			case FREQ:
			case RESOLUTION:
				enterOuterAlt(_localctx, 1);
				{
				setState(520);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==T__10 || _la==T__27) {
					{
					setState(519);
					unaryOperator();
					}
				}

				setState(522);
				_la = _input.LA(1);
				if ( !(((((_la - 58)) & ~0x3f) == 0 && ((1L << (_la - 58)) & 511L) != 0)) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				}
				break;
			case STRING:
				enterOuterAlt(_localctx, 2);
				{
				setState(523);
				match(STRING);
				}
				break;
			case IDENT:
				enterOuterAlt(_localctx, 3);
				{
				setState(524);
				match(IDENT);
				}
				break;
			case T__28:
				enterOuterAlt(_localctx, 4);
				{
				setState(525);
				url();
				}
				break;
			case HASH:
				enterOuterAlt(_localctx, 5);
				{
				setState(526);
				match(HASH);
				}
				break;
			case UNICODE_RANGE:
				enterOuterAlt(_localctx, 6);
				{
				setState(527);
				match(UNICODE_RANGE);
				}
				break;
			case INCLUDES:
				enterOuterAlt(_localctx, 7);
				{
				setState(528);
				match(INCLUDES);
				}
				break;
			case DASHMATCH:
				enterOuterAlt(_localctx, 8);
				{
				setState(529);
				match(DASHMATCH);
				}
				break;
			case T__6:
				enterOuterAlt(_localctx, 9);
				{
				setState(530);
				match(T__6);
				setState(531);
				match(IDENT);
				}
				break;
			case FUNCTION:
				enterOuterAlt(_localctx, 10);
				{
				setState(532);
				match(FUNCTION);
				setState(534);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==COMMENT || _la==WS) {
					{
					setState(533);
					ws();
					}
				}

				setState(536);
				expr();
				setState(537);
				match(T__7);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class OperatorContext extends ParserRuleContext {
		public WsContext ws() {
			return getRuleContext(WsContext.class,0);
		}
		public OperatorContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_operator; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterOperator(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitOperator(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitOperator(this);
			else return visitor.visitChildren(this);
		}
	}

	public final OperatorContext operator() throws RecognitionException {
		OperatorContext _localctx = new OperatorContext(_ctx, getState());
		enterRule(_localctx, 68, RULE_operator);
		try {
			setState(544);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__26:
				enterOuterAlt(_localctx, 1);
				{
				setState(541);
				match(T__26);
				}
				break;
			case T__4:
				enterOuterAlt(_localctx, 2);
				{
				setState(542);
				match(T__4);
				}
				break;
			case COMMENT:
			case WS:
				enterOuterAlt(_localctx, 3);
				{
				setState(543);
				ws();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class UnaryOperatorContext extends ParserRuleContext {
		public UnaryOperatorContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_unaryOperator; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterUnaryOperator(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitUnaryOperator(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitUnaryOperator(this);
			else return visitor.visitChildren(this);
		}
	}

	public final UnaryOperatorContext unaryOperator() throws RecognitionException {
		UnaryOperatorContext _localctx = new UnaryOperatorContext(_ctx, getState());
		enterRule(_localctx, 70, RULE_unaryOperator);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(546);
			_la = _input.LA(1);
			if ( !(_la==T__10 || _la==T__27) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class UrlContext extends ParserRuleContext {
		public TerminalNode STRING() { return getToken(CSS3Parser.STRING, 0); }
		public TerminalNode URL() { return getToken(CSS3Parser.URL, 0); }
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public UrlContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_url; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterUrl(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitUrl(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitUrl(this);
			else return visitor.visitChildren(this);
		}
	}

	public final UrlContext url() throws RecognitionException {
		UrlContext _localctx = new UrlContext(_ctx, getState());
		enterRule(_localctx, 72, RULE_url);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(548);
			match(T__28);
			setState(550);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(549);
				ws();
				}
			}

			setState(552);
			_la = _input.LA(1);
			if ( !(_la==STRING || _la==URL) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			setState(554);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(553);
				ws();
				}
			}

			setState(556);
			match(T__7);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class MediaContext extends ParserRuleContext {
		public MediaQueryListContext mediaQueryList() {
			return getRuleContext(MediaQueryListContext.class,0);
		}
		public GroupRuleBodyContext groupRuleBody() {
			return getRuleContext(GroupRuleBodyContext.class,0);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public MediaContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_media; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterMedia(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitMedia(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitMedia(this);
			else return visitor.visitChildren(this);
		}
	}

	public final MediaContext media() throws RecognitionException {
		MediaContext _localctx = new MediaContext(_ctx, getState());
		enterRule(_localctx, 74, RULE_media);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(558);
			match(T__29);
			setState(559);
			mediaQueryList();
			setState(560);
			match(T__8);
			setState(562);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(561);
				ws();
				}
			}

			setState(564);
			groupRuleBody();
			setState(565);
			match(T__9);
			setState(567);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,92,_ctx) ) {
			case 1:
				{
				setState(566);
				ws();
				}
				break;
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class PageContext extends ParserRuleContext {
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public PseudoPageContext pseudoPage() {
			return getRuleContext(PseudoPageContext.class,0);
		}
		public DeclarationListContext declarationList() {
			return getRuleContext(DeclarationListContext.class,0);
		}
		public PageContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_page; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterPage(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitPage(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitPage(this);
			else return visitor.visitChildren(this);
		}
	}

	public final PageContext page() throws RecognitionException {
		PageContext _localctx = new PageContext(_ctx, getState());
		enterRule(_localctx, 76, RULE_page);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(569);
			match(T__30);
			setState(571);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(570);
				ws();
				}
			}

			setState(574);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__6) {
				{
				setState(573);
				pseudoPage();
				}
			}

			setState(576);
			match(T__8);
			setState(578);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(577);
				ws();
				}
			}

			setState(581);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,96,_ctx) ) {
			case 1:
				{
				setState(580);
				declarationList();
				}
				break;
			}
			setState(583);
			match(T__9);
			setState(585);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,97,_ctx) ) {
			case 1:
				{
				setState(584);
				ws();
				}
				break;
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class PseudoPageContext extends ParserRuleContext {
		public TerminalNode IDENT() { return getToken(CSS3Parser.IDENT, 0); }
		public PseudoPageContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_pseudoPage; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterPseudoPage(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitPseudoPage(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitPseudoPage(this);
			else return visitor.visitChildren(this);
		}
	}

	public final PseudoPageContext pseudoPage() throws RecognitionException {
		PseudoPageContext _localctx = new PseudoPageContext(_ctx, getState());
		enterRule(_localctx, 78, RULE_pseudoPage);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(587);
			match(T__6);
			setState(588);
			match(IDENT);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class FontFaceContext extends ParserRuleContext {
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public DeclarationListContext declarationList() {
			return getRuleContext(DeclarationListContext.class,0);
		}
		public FontFaceContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_fontFace; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterFontFace(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitFontFace(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitFontFace(this);
			else return visitor.visitChildren(this);
		}
	}

	public final FontFaceContext fontFace() throws RecognitionException {
		FontFaceContext _localctx = new FontFaceContext(_ctx, getState());
		enterRule(_localctx, 80, RULE_fontFace);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(590);
			match(T__31);
			setState(592);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(591);
				ws();
				}
			}

			setState(594);
			match(T__8);
			setState(596);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(595);
				ws();
				}
			}

			setState(599);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,100,_ctx) ) {
			case 1:
				{
				setState(598);
				declarationList();
				}
				break;
			}
			setState(601);
			match(T__9);
			setState(603);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,101,_ctx) ) {
			case 1:
				{
				setState(602);
				ws();
				}
				break;
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class KeyframesContext extends ParserRuleContext {
		public TerminalNode KEYFRAMES() { return getToken(CSS3Parser.KEYFRAMES, 0); }
		public TerminalNode IDENT() { return getToken(CSS3Parser.IDENT, 0); }
		public KeyframesBlocksContext keyframesBlocks() {
			return getRuleContext(KeyframesBlocksContext.class,0);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public KeyframesContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_keyframes; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterKeyframes(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitKeyframes(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitKeyframes(this);
			else return visitor.visitChildren(this);
		}
	}

	public final KeyframesContext keyframes() throws RecognitionException {
		KeyframesContext _localctx = new KeyframesContext(_ctx, getState());
		enterRule(_localctx, 82, RULE_keyframes);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(605);
			match(T__32);
			setState(606);
			match(KEYFRAMES);
			setState(608);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(607);
				ws();
				}
			}

			setState(610);
			match(IDENT);
			setState(612);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(611);
				ws();
				}
			}

			setState(614);
			match(T__8);
			setState(616);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(615);
				ws();
				}
			}

			setState(618);
			keyframesBlocks();
			setState(619);
			match(T__9);
			setState(621);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,105,_ctx) ) {
			case 1:
				{
				setState(620);
				ws();
				}
				break;
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class KeyframesBlocksContext extends ParserRuleContext {
		public List<KeyframeSelectorContext> keyframeSelector() {
			return getRuleContexts(KeyframeSelectorContext.class);
		}
		public KeyframeSelectorContext keyframeSelector(int i) {
			return getRuleContext(KeyframeSelectorContext.class,i);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public List<DeclarationListContext> declarationList() {
			return getRuleContexts(DeclarationListContext.class);
		}
		public DeclarationListContext declarationList(int i) {
			return getRuleContext(DeclarationListContext.class,i);
		}
		public KeyframesBlocksContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_keyframesBlocks; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterKeyframesBlocks(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitKeyframesBlocks(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitKeyframesBlocks(this);
			else return visitor.visitChildren(this);
		}
	}

	public final KeyframesBlocksContext keyframesBlocks() throws RecognitionException {
		KeyframesBlocksContext _localctx = new KeyframesBlocksContext(_ctx, getState());
		enterRule(_localctx, 84, RULE_keyframesBlocks);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(623);
			keyframeSelector();
			setState(625);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(624);
				ws();
				}
			}

			setState(627);
			match(T__8);
			setState(629);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(628);
				ws();
				}
			}

			setState(632);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,108,_ctx) ) {
			case 1:
				{
				setState(631);
				declarationList();
				}
				break;
			}
			setState(634);
			match(T__9);
			setState(636);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(635);
				ws();
				}
			}

			setState(655);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 578149602163687424L) != 0)) {
				{
				{
				setState(638);
				keyframeSelector();
				setState(640);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==COMMENT || _la==WS) {
					{
					setState(639);
					ws();
					}
				}

				setState(642);
				match(T__8);
				setState(644);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==COMMENT || _la==WS) {
					{
					setState(643);
					ws();
					}
				}

				setState(647);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,112,_ctx) ) {
				case 1:
					{
					setState(646);
					declarationList();
					}
					break;
				}
				setState(649);
				match(T__9);
				setState(651);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==COMMENT || _la==WS) {
					{
					setState(650);
					ws();
					}
				}

				}
				}
				setState(657);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class KeyframeSelectorContext extends ParserRuleContext {
		public List<TerminalNode> FROM() { return getTokens(CSS3Parser.FROM); }
		public TerminalNode FROM(int i) {
			return getToken(CSS3Parser.FROM, i);
		}
		public List<TerminalNode> TO() { return getTokens(CSS3Parser.TO); }
		public TerminalNode TO(int i) {
			return getToken(CSS3Parser.TO, i);
		}
		public List<TerminalNode> PERCENTAGE() { return getTokens(CSS3Parser.PERCENTAGE); }
		public TerminalNode PERCENTAGE(int i) {
			return getToken(CSS3Parser.PERCENTAGE, i);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public KeyframeSelectorContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_keyframeSelector; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterKeyframeSelector(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitKeyframeSelector(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitKeyframeSelector(this);
			else return visitor.visitChildren(this);
		}
	}

	public final KeyframeSelectorContext keyframeSelector() throws RecognitionException {
		KeyframeSelectorContext _localctx = new KeyframeSelectorContext(_ctx, getState());
		enterRule(_localctx, 86, RULE_keyframeSelector);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(658);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 578149602163687424L) != 0)) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			setState(669);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,117,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(660);
					_errHandler.sync(this);
					_la = _input.LA(1);
					if (_la==COMMENT || _la==WS) {
						{
						setState(659);
						ws();
						}
					}

					setState(662);
					match(T__4);
					setState(664);
					_errHandler.sync(this);
					_la = _input.LA(1);
					if (_la==COMMENT || _la==WS) {
						{
						setState(663);
						ws();
						}
					}

					setState(666);
					_la = _input.LA(1);
					if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 578149602163687424L) != 0)) ) {
					_errHandler.recoverInline(this);
					}
					else {
						if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
						_errHandler.reportMatch(this);
						consume();
					}
					}
					} 
				}
				setState(671);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,117,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class SupportsContext extends ParserRuleContext {
		public SupportsConditionContext supportsCondition() {
			return getRuleContext(SupportsConditionContext.class,0);
		}
		public GroupRuleBodyContext groupRuleBody() {
			return getRuleContext(GroupRuleBodyContext.class,0);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public SupportsContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_supports; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterSupports(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitSupports(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitSupports(this);
			else return visitor.visitChildren(this);
		}
	}

	public final SupportsContext supports() throws RecognitionException {
		SupportsContext _localctx = new SupportsContext(_ctx, getState());
		enterRule(_localctx, 88, RULE_supports);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(672);
			match(T__33);
			setState(674);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(673);
				ws();
				}
			}

			setState(676);
			supportsCondition();
			setState(678);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(677);
				ws();
				}
			}

			setState(680);
			match(T__8);
			setState(682);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(681);
				ws();
				}
			}

			setState(684);
			groupRuleBody();
			setState(685);
			match(T__9);
			setState(687);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,121,_ctx) ) {
			case 1:
				{
				setState(686);
				ws();
				}
				break;
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class SupportsConditionContext extends ParserRuleContext {
		public SupportsNegationContext supportsNegation() {
			return getRuleContext(SupportsNegationContext.class,0);
		}
		public SupportsConjunctionContext supportsConjunction() {
			return getRuleContext(SupportsConjunctionContext.class,0);
		}
		public SupportsDisjunctionContext supportsDisjunction() {
			return getRuleContext(SupportsDisjunctionContext.class,0);
		}
		public SupportsConditionInParensContext supportsConditionInParens() {
			return getRuleContext(SupportsConditionInParensContext.class,0);
		}
		public SupportsConditionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_supportsCondition; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterSupportsCondition(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitSupportsCondition(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitSupportsCondition(this);
			else return visitor.visitChildren(this);
		}
	}

	public final SupportsConditionContext supportsCondition() throws RecognitionException {
		SupportsConditionContext _localctx = new SupportsConditionContext(_ctx, getState());
		enterRule(_localctx, 90, RULE_supportsCondition);
		try {
			setState(693);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,122,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(689);
				supportsNegation();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(690);
				supportsConjunction();
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(691);
				supportsDisjunction();
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(692);
				supportsConditionInParens();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class SupportsConditionInParensContext extends ParserRuleContext {
		public SupportsConditionContext supportsCondition() {
			return getRuleContext(SupportsConditionContext.class,0);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public SupportsDeclarationConditionContext supportsDeclarationCondition() {
			return getRuleContext(SupportsDeclarationConditionContext.class,0);
		}
		public GeneralEnclosedContext generalEnclosed() {
			return getRuleContext(GeneralEnclosedContext.class,0);
		}
		public SupportsConditionInParensContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_supportsConditionInParens; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterSupportsConditionInParens(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitSupportsConditionInParens(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitSupportsConditionInParens(this);
			else return visitor.visitChildren(this);
		}
	}

	public final SupportsConditionInParensContext supportsConditionInParens() throws RecognitionException {
		SupportsConditionInParensContext _localctx = new SupportsConditionInParensContext(_ctx, getState());
		enterRule(_localctx, 92, RULE_supportsConditionInParens);
		int _la;
		try {
			setState(707);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,125,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(695);
				match(T__5);
				setState(697);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==COMMENT || _la==WS) {
					{
					setState(696);
					ws();
					}
				}

				setState(699);
				supportsCondition();
				setState(701);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==COMMENT || _la==WS) {
					{
					setState(700);
					ws();
					}
				}

				setState(703);
				match(T__7);
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(705);
				supportsDeclarationCondition();
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(706);
				generalEnclosed();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class SupportsNegationContext extends ParserRuleContext {
		public TerminalNode NOT() { return getToken(CSS3Parser.NOT, 0); }
		public WsContext ws() {
			return getRuleContext(WsContext.class,0);
		}
		public SupportsConditionInParensContext supportsConditionInParens() {
			return getRuleContext(SupportsConditionInParensContext.class,0);
		}
		public SupportsNegationContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_supportsNegation; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterSupportsNegation(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitSupportsNegation(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitSupportsNegation(this);
			else return visitor.visitChildren(this);
		}
	}

	public final SupportsNegationContext supportsNegation() throws RecognitionException {
		SupportsNegationContext _localctx = new SupportsNegationContext(_ctx, getState());
		enterRule(_localctx, 94, RULE_supportsNegation);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(709);
			match(NOT);
			setState(710);
			ws();
			setState(711);
			supportsConditionInParens();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class SupportsConjunctionContext extends ParserRuleContext {
		public List<SupportsConditionInParensContext> supportsConditionInParens() {
			return getRuleContexts(SupportsConditionInParensContext.class);
		}
		public SupportsConditionInParensContext supportsConditionInParens(int i) {
			return getRuleContext(SupportsConditionInParensContext.class,i);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public List<TerminalNode> AND() { return getTokens(CSS3Parser.AND); }
		public TerminalNode AND(int i) {
			return getToken(CSS3Parser.AND, i);
		}
		public SupportsConjunctionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_supportsConjunction; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterSupportsConjunction(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitSupportsConjunction(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitSupportsConjunction(this);
			else return visitor.visitChildren(this);
		}
	}

	public final SupportsConjunctionContext supportsConjunction() throws RecognitionException {
		SupportsConjunctionContext _localctx = new SupportsConjunctionContext(_ctx, getState());
		enterRule(_localctx, 96, RULE_supportsConjunction);
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(713);
			supportsConditionInParens();
			setState(719); 
			_errHandler.sync(this);
			_alt = 1;
			do {
				switch (_alt) {
				case 1:
					{
					{
					setState(714);
					ws();
					setState(715);
					match(AND);
					setState(716);
					ws();
					setState(717);
					supportsConditionInParens();
					}
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				setState(721); 
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,126,_ctx);
			} while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER );
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class SupportsDisjunctionContext extends ParserRuleContext {
		public List<SupportsConditionInParensContext> supportsConditionInParens() {
			return getRuleContexts(SupportsConditionInParensContext.class);
		}
		public SupportsConditionInParensContext supportsConditionInParens(int i) {
			return getRuleContext(SupportsConditionInParensContext.class,i);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public List<TerminalNode> OR() { return getTokens(CSS3Parser.OR); }
		public TerminalNode OR(int i) {
			return getToken(CSS3Parser.OR, i);
		}
		public SupportsDisjunctionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_supportsDisjunction; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterSupportsDisjunction(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitSupportsDisjunction(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitSupportsDisjunction(this);
			else return visitor.visitChildren(this);
		}
	}

	public final SupportsDisjunctionContext supportsDisjunction() throws RecognitionException {
		SupportsDisjunctionContext _localctx = new SupportsDisjunctionContext(_ctx, getState());
		enterRule(_localctx, 98, RULE_supportsDisjunction);
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(723);
			supportsConditionInParens();
			setState(729); 
			_errHandler.sync(this);
			_alt = 1;
			do {
				switch (_alt) {
				case 1:
					{
					{
					setState(724);
					ws();
					setState(725);
					match(OR);
					setState(726);
					ws();
					setState(727);
					supportsConditionInParens();
					}
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				setState(731); 
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,127,_ctx);
			} while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER );
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class SupportsDeclarationConditionContext extends ParserRuleContext {
		public DeclarationContext declaration() {
			return getRuleContext(DeclarationContext.class,0);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public SupportsDeclarationConditionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_supportsDeclarationCondition; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterSupportsDeclarationCondition(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitSupportsDeclarationCondition(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitSupportsDeclarationCondition(this);
			else return visitor.visitChildren(this);
		}
	}

	public final SupportsDeclarationConditionContext supportsDeclarationCondition() throws RecognitionException {
		SupportsDeclarationConditionContext _localctx = new SupportsDeclarationConditionContext(_ctx, getState());
		enterRule(_localctx, 100, RULE_supportsDeclarationCondition);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(733);
			match(T__5);
			setState(735);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(734);
				ws();
				}
			}

			setState(737);
			declaration();
			setState(739);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(738);
				ws();
				}
			}

			setState(741);
			match(T__7);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class GeneralEnclosedContext extends ParserRuleContext {
		public TerminalNode FUNCTION() { return getToken(CSS3Parser.FUNCTION, 0); }
		public List<AnyContext> any() {
			return getRuleContexts(AnyContext.class);
		}
		public AnyContext any(int i) {
			return getRuleContext(AnyContext.class,i);
		}
		public List<GeneralEnclosedContext> generalEnclosed() {
			return getRuleContexts(GeneralEnclosedContext.class);
		}
		public GeneralEnclosedContext generalEnclosed(int i) {
			return getRuleContext(GeneralEnclosedContext.class,i);
		}
		public GeneralEnclosedContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_generalEnclosed; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterGeneralEnclosed(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitGeneralEnclosed(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitGeneralEnclosed(this);
			else return visitor.visitChildren(this);
		}
	}

	public final GeneralEnclosedContext generalEnclosed() throws RecognitionException {
		GeneralEnclosedContext _localctx = new GeneralEnclosedContext(_ctx, getState());
		enterRule(_localctx, 102, RULE_generalEnclosed);
		int _la;
		try {
			setState(761);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case FUNCTION:
				enterOuterAlt(_localctx, 1);
				{
				setState(743);
				match(FUNCTION);
				setState(748);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -71635381572599616L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 511L) != 0)) {
					{
					setState(746);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,130,_ctx) ) {
					case 1:
						{
						setState(744);
						any();
						}
						break;
					case 2:
						{
						setState(745);
						generalEnclosed();
						}
						break;
					}
					}
					setState(750);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(751);
				match(T__7);
				}
				break;
			case T__5:
				enterOuterAlt(_localctx, 2);
				{
				setState(752);
				match(T__5);
				setState(757);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -71635381572599616L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 511L) != 0)) {
					{
					setState(755);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,132,_ctx) ) {
					case 1:
						{
						setState(753);
						any();
						}
						break;
					case 2:
						{
						setState(754);
						generalEnclosed();
						}
						break;
					}
					}
					setState(759);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(760);
				match(T__7);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class AnyContext extends ParserRuleContext {
		public TerminalNode IDENT() { return getToken(CSS3Parser.IDENT, 0); }
		public TerminalNode NUMBER() { return getToken(CSS3Parser.NUMBER, 0); }
		public TerminalNode PERCENTAGE() { return getToken(CSS3Parser.PERCENTAGE, 0); }
		public TerminalNode LENGTH() { return getToken(CSS3Parser.LENGTH, 0); }
		public TerminalNode EMS() { return getToken(CSS3Parser.EMS, 0); }
		public TerminalNode EXS() { return getToken(CSS3Parser.EXS, 0); }
		public TerminalNode ANGLE() { return getToken(CSS3Parser.ANGLE, 0); }
		public TerminalNode TIME() { return getToken(CSS3Parser.TIME, 0); }
		public TerminalNode FREQ() { return getToken(CSS3Parser.FREQ, 0); }
		public TerminalNode RESOLUTION() { return getToken(CSS3Parser.RESOLUTION, 0); }
		public TerminalNode STRING() { return getToken(CSS3Parser.STRING, 0); }
		public TerminalNode URL() { return getToken(CSS3Parser.URL, 0); }
		public TerminalNode HASH() { return getToken(CSS3Parser.HASH, 0); }
		public TerminalNode UNICODE_RANGE() { return getToken(CSS3Parser.UNICODE_RANGE, 0); }
		public TerminalNode INCLUDES() { return getToken(CSS3Parser.INCLUDES, 0); }
		public TerminalNode DASHMATCH() { return getToken(CSS3Parser.DASHMATCH, 0); }
		public TerminalNode FUNCTION() { return getToken(CSS3Parser.FUNCTION, 0); }
		public List<AnyContext> any() {
			return getRuleContexts(AnyContext.class);
		}
		public AnyContext any(int i) {
			return getRuleContext(AnyContext.class,i);
		}
		public WsContext ws() {
			return getRuleContext(WsContext.class,0);
		}
		public AnyContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_any; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterAny(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitAny(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitAny(this);
			else return visitor.visitChildren(this);
		}
	}

	public final AnyContext any() throws RecognitionException {
		AnyContext _localctx = new AnyContext(_ctx, getState());
		enterRule(_localctx, 104, RULE_any);
		int _la;
		try {
			setState(798);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case IDENT:
				enterOuterAlt(_localctx, 1);
				{
				setState(763);
				match(IDENT);
				}
				break;
			case NUMBER:
				enterOuterAlt(_localctx, 2);
				{
				setState(764);
				match(NUMBER);
				}
				break;
			case PERCENTAGE:
				enterOuterAlt(_localctx, 3);
				{
				setState(765);
				match(PERCENTAGE);
				}
				break;
			case LENGTH:
				enterOuterAlt(_localctx, 4);
				{
				setState(766);
				match(LENGTH);
				}
				break;
			case EMS:
				enterOuterAlt(_localctx, 5);
				{
				setState(767);
				match(EMS);
				}
				break;
			case EXS:
				enterOuterAlt(_localctx, 6);
				{
				setState(768);
				match(EXS);
				}
				break;
			case ANGLE:
				enterOuterAlt(_localctx, 7);
				{
				setState(769);
				match(ANGLE);
				}
				break;
			case TIME:
				enterOuterAlt(_localctx, 8);
				{
				setState(770);
				match(TIME);
				}
				break;
			case FREQ:
				enterOuterAlt(_localctx, 9);
				{
				setState(771);
				match(FREQ);
				}
				break;
			case RESOLUTION:
				enterOuterAlt(_localctx, 10);
				{
				setState(772);
				match(RESOLUTION);
				}
				break;
			case STRING:
				enterOuterAlt(_localctx, 11);
				{
				setState(773);
				match(STRING);
				}
				break;
			case URL:
				enterOuterAlt(_localctx, 12);
				{
				setState(774);
				match(URL);
				}
				break;
			case HASH:
				enterOuterAlt(_localctx, 13);
				{
				setState(775);
				match(HASH);
				}
				break;
			case UNICODE_RANGE:
				enterOuterAlt(_localctx, 14);
				{
				setState(776);
				match(UNICODE_RANGE);
				}
				break;
			case INCLUDES:
				enterOuterAlt(_localctx, 15);
				{
				setState(777);
				match(INCLUDES);
				}
				break;
			case DASHMATCH:
				enterOuterAlt(_localctx, 16);
				{
				setState(778);
				match(DASHMATCH);
				}
				break;
			case T__6:
				enterOuterAlt(_localctx, 17);
				{
				setState(779);
				match(T__6);
				}
				break;
			case FUNCTION:
				enterOuterAlt(_localctx, 18);
				{
				setState(780);
				match(FUNCTION);
				}
				break;
			case T__5:
				enterOuterAlt(_localctx, 19);
				{
				setState(781);
				match(T__5);
				setState(785);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -71635381572599616L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 511L) != 0)) {
					{
					{
					setState(782);
					any();
					}
					}
					setState(787);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(788);
				match(T__7);
				}
				break;
			case T__17:
				enterOuterAlt(_localctx, 20);
				{
				setState(789);
				match(T__17);
				setState(793);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -71635381572599616L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 511L) != 0)) {
					{
					{
					setState(790);
					any();
					}
					}
					setState(795);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				setState(796);
				match(T__18);
				}
				break;
			case COMMENT:
			case WS:
				enterOuterAlt(_localctx, 21);
				{
				setState(797);
				ws();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ViewportContext extends ParserRuleContext {
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public DeclarationListContext declarationList() {
			return getRuleContext(DeclarationListContext.class,0);
		}
		public ViewportContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_viewport; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterViewport(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitViewport(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitViewport(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ViewportContext viewport() throws RecognitionException {
		ViewportContext _localctx = new ViewportContext(_ctx, getState());
		enterRule(_localctx, 106, RULE_viewport);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(800);
			match(T__34);
			setState(802);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(801);
				ws();
				}
			}

			setState(804);
			match(T__8);
			setState(806);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(805);
				ws();
				}
			}

			setState(809);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,140,_ctx) ) {
			case 1:
				{
				setState(808);
				declarationList();
				}
				break;
			}
			setState(811);
			match(T__9);
			setState(813);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,141,_ctx) ) {
			case 1:
				{
				setState(812);
				ws();
				}
				break;
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class CounterStyleContext extends ParserRuleContext {
		public TerminalNode IDENT() { return getToken(CSS3Parser.IDENT, 0); }
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public DeclarationListContext declarationList() {
			return getRuleContext(DeclarationListContext.class,0);
		}
		public CounterStyleContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_counterStyle; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterCounterStyle(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitCounterStyle(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitCounterStyle(this);
			else return visitor.visitChildren(this);
		}
	}

	public final CounterStyleContext counterStyle() throws RecognitionException {
		CounterStyleContext _localctx = new CounterStyleContext(_ctx, getState());
		enterRule(_localctx, 108, RULE_counterStyle);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(815);
			match(T__35);
			setState(817);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(816);
				ws();
				}
			}

			setState(819);
			match(IDENT);
			setState(821);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(820);
				ws();
				}
			}

			setState(823);
			match(T__8);
			setState(825);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(824);
				ws();
				}
			}

			setState(828);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,145,_ctx) ) {
			case 1:
				{
				setState(827);
				declarationList();
				}
				break;
			}
			setState(830);
			match(T__9);
			setState(832);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,146,_ctx) ) {
			case 1:
				{
				setState(831);
				ws();
				}
				break;
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class FontFeatureValuesContext extends ParserRuleContext {
		public FontFamilyNameListContext fontFamilyNameList() {
			return getRuleContext(FontFamilyNameListContext.class,0);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public List<FontFeatureValueBlockContext> fontFeatureValueBlock() {
			return getRuleContexts(FontFeatureValueBlockContext.class);
		}
		public FontFeatureValueBlockContext fontFeatureValueBlock(int i) {
			return getRuleContext(FontFeatureValueBlockContext.class,i);
		}
		public FontFeatureValuesContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_fontFeatureValues; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterFontFeatureValues(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitFontFeatureValues(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitFontFeatureValues(this);
			else return visitor.visitChildren(this);
		}
	}

	public final FontFeatureValuesContext fontFeatureValues() throws RecognitionException {
		FontFeatureValuesContext _localctx = new FontFeatureValuesContext(_ctx, getState());
		enterRule(_localctx, 110, RULE_fontFeatureValues);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(834);
			match(T__36);
			setState(836);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(835);
				ws();
				}
			}

			setState(838);
			fontFamilyNameList();
			setState(840);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(839);
				ws();
				}
			}

			setState(842);
			match(T__8);
			setState(844);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(843);
				ws();
				}
			}

			setState(849);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & 34909494181888L) != 0)) {
				{
				{
				setState(846);
				fontFeatureValueBlock();
				}
				}
				setState(851);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(852);
			match(T__9);
			setState(854);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,151,_ctx) ) {
			case 1:
				{
				setState(853);
				ws();
				}
				break;
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class FontFamilyNameListContext extends ParserRuleContext {
		public List<FontFamilyNameContext> fontFamilyName() {
			return getRuleContexts(FontFamilyNameContext.class);
		}
		public FontFamilyNameContext fontFamilyName(int i) {
			return getRuleContext(FontFamilyNameContext.class,i);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public FontFamilyNameListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_fontFamilyNameList; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterFontFamilyNameList(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitFontFamilyNameList(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitFontFamilyNameList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final FontFamilyNameListContext fontFamilyNameList() throws RecognitionException {
		FontFamilyNameListContext _localctx = new FontFamilyNameListContext(_ctx, getState());
		enterRule(_localctx, 112, RULE_fontFamilyNameList);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(856);
			fontFamilyName();
			setState(867);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,154,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(858);
					_errHandler.sync(this);
					_la = _input.LA(1);
					if (_la==COMMENT || _la==WS) {
						{
						setState(857);
						ws();
						}
					}

					setState(860);
					match(T__4);
					setState(862);
					_errHandler.sync(this);
					_la = _input.LA(1);
					if (_la==COMMENT || _la==WS) {
						{
						setState(861);
						ws();
						}
					}

					setState(864);
					fontFamilyName();
					}
					} 
				}
				setState(869);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,154,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class FontFamilyNameContext extends ParserRuleContext {
		public TerminalNode STRING() { return getToken(CSS3Parser.STRING, 0); }
		public List<TerminalNode> IDENT() { return getTokens(CSS3Parser.IDENT); }
		public TerminalNode IDENT(int i) {
			return getToken(CSS3Parser.IDENT, i);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public FontFamilyNameContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_fontFamilyName; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterFontFamilyName(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitFontFamilyName(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitFontFamilyName(this);
			else return visitor.visitChildren(this);
		}
	}

	public final FontFamilyNameContext fontFamilyName() throws RecognitionException {
		FontFamilyNameContext _localctx = new FontFamilyNameContext(_ctx, getState());
		enterRule(_localctx, 114, RULE_fontFamilyName);
		int _la;
		try {
			setState(879);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case STRING:
				enterOuterAlt(_localctx, 1);
				{
				setState(870);
				match(STRING);
				}
				break;
			case IDENT:
				enterOuterAlt(_localctx, 2);
				{
				setState(875); 
				_errHandler.sync(this);
				_la = _input.LA(1);
				do {
					{
					{
					setState(871);
					match(IDENT);
					setState(873);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,155,_ctx) ) {
					case 1:
						{
						setState(872);
						ws();
						}
						break;
					}
					}
					}
					setState(877); 
					_errHandler.sync(this);
					_la = _input.LA(1);
				} while ( _la==IDENT );
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class FontFeatureValueBlockContext extends ParserRuleContext {
		public FontFeatureTypeContext fontFeatureType() {
			return getRuleContext(FontFeatureTypeContext.class,0);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public FontFeatureValueListContext fontFeatureValueList() {
			return getRuleContext(FontFeatureValueListContext.class,0);
		}
		public FontFeatureValueBlockContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_fontFeatureValueBlock; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterFontFeatureValueBlock(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitFontFeatureValueBlock(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitFontFeatureValueBlock(this);
			else return visitor.visitChildren(this);
		}
	}

	public final FontFeatureValueBlockContext fontFeatureValueBlock() throws RecognitionException {
		FontFeatureValueBlockContext _localctx = new FontFeatureValueBlockContext(_ctx, getState());
		enterRule(_localctx, 116, RULE_fontFeatureValueBlock);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(881);
			fontFeatureType();
			setState(883);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(882);
				ws();
				}
			}

			setState(885);
			match(T__8);
			setState(887);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(886);
				ws();
				}
			}

			setState(890);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==IDENT) {
				{
				setState(889);
				fontFeatureValueList();
				}
			}

			setState(892);
			match(T__9);
			setState(894);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(893);
				ws();
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class FontFeatureTypeContext extends ParserRuleContext {
		public FontFeatureTypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_fontFeatureType; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterFontFeatureType(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitFontFeatureType(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitFontFeatureType(this);
			else return visitor.visitChildren(this);
		}
	}

	public final FontFeatureTypeContext fontFeatureType() throws RecognitionException {
		FontFeatureTypeContext _localctx = new FontFeatureTypeContext(_ctx, getState());
		enterRule(_localctx, 118, RULE_fontFeatureType);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(896);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 34909494181888L) != 0)) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class FontFeatureValueListContext extends ParserRuleContext {
		public List<FontFeatureValueContext> fontFeatureValue() {
			return getRuleContexts(FontFeatureValueContext.class);
		}
		public FontFeatureValueContext fontFeatureValue(int i) {
			return getRuleContext(FontFeatureValueContext.class,i);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public FontFeatureValueListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_fontFeatureValueList; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterFontFeatureValueList(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitFontFeatureValueList(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitFontFeatureValueList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final FontFeatureValueListContext fontFeatureValueList() throws RecognitionException {
		FontFeatureValueListContext _localctx = new FontFeatureValueListContext(_ctx, getState());
		enterRule(_localctx, 120, RULE_fontFeatureValueList);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(898);
			fontFeatureValue();
			setState(909);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,164,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(900);
					_errHandler.sync(this);
					_la = _input.LA(1);
					if (_la==COMMENT || _la==WS) {
						{
						setState(899);
						ws();
						}
					}

					setState(902);
					match(T__1);
					setState(904);
					_errHandler.sync(this);
					_la = _input.LA(1);
					if (_la==COMMENT || _la==WS) {
						{
						setState(903);
						ws();
						}
					}

					setState(906);
					fontFeatureValue();
					}
					} 
				}
				setState(911);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,164,_ctx);
			}
			setState(913);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__1) {
				{
				setState(912);
				match(T__1);
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class FontFeatureValueContext extends ParserRuleContext {
		public TerminalNode IDENT() { return getToken(CSS3Parser.IDENT, 0); }
		public List<TerminalNode> NUMBER() { return getTokens(CSS3Parser.NUMBER); }
		public TerminalNode NUMBER(int i) {
			return getToken(CSS3Parser.NUMBER, i);
		}
		public List<WsContext> ws() {
			return getRuleContexts(WsContext.class);
		}
		public WsContext ws(int i) {
			return getRuleContext(WsContext.class,i);
		}
		public FontFeatureValueContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_fontFeatureValue; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterFontFeatureValue(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitFontFeatureValue(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitFontFeatureValue(this);
			else return visitor.visitChildren(this);
		}
	}

	public final FontFeatureValueContext fontFeatureValue() throws RecognitionException {
		FontFeatureValueContext _localctx = new FontFeatureValueContext(_ctx, getState());
		enterRule(_localctx, 122, RULE_fontFeatureValue);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(915);
			match(IDENT);
			setState(917);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(916);
				ws();
				}
			}

			setState(919);
			match(T__6);
			setState(921);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT || _la==WS) {
				{
				setState(920);
				ws();
				}
			}

			setState(923);
			match(NUMBER);
			setState(930);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,169,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(925);
					_errHandler.sync(this);
					_la = _input.LA(1);
					if (_la==COMMENT || _la==WS) {
						{
						setState(924);
						ws();
						}
					}

					setState(927);
					match(NUMBER);
					}
					} 
				}
				setState(932);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,169,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class GroupRuleBodyContext extends ParserRuleContext {
		public List<RulesetContext> ruleset() {
			return getRuleContexts(RulesetContext.class);
		}
		public RulesetContext ruleset(int i) {
			return getRuleContext(RulesetContext.class,i);
		}
		public List<MediaContext> media() {
			return getRuleContexts(MediaContext.class);
		}
		public MediaContext media(int i) {
			return getRuleContext(MediaContext.class,i);
		}
		public List<PageContext> page() {
			return getRuleContexts(PageContext.class);
		}
		public PageContext page(int i) {
			return getRuleContext(PageContext.class,i);
		}
		public List<FontFaceContext> fontFace() {
			return getRuleContexts(FontFaceContext.class);
		}
		public FontFaceContext fontFace(int i) {
			return getRuleContext(FontFaceContext.class,i);
		}
		public List<KeyframesContext> keyframes() {
			return getRuleContexts(KeyframesContext.class);
		}
		public KeyframesContext keyframes(int i) {
			return getRuleContext(KeyframesContext.class,i);
		}
		public List<SupportsContext> supports() {
			return getRuleContexts(SupportsContext.class);
		}
		public SupportsContext supports(int i) {
			return getRuleContext(SupportsContext.class,i);
		}
		public List<ViewportContext> viewport() {
			return getRuleContexts(ViewportContext.class);
		}
		public ViewportContext viewport(int i) {
			return getRuleContext(ViewportContext.class,i);
		}
		public List<CounterStyleContext> counterStyle() {
			return getRuleContexts(CounterStyleContext.class);
		}
		public CounterStyleContext counterStyle(int i) {
			return getRuleContext(CounterStyleContext.class,i);
		}
		public List<FontFeatureValuesContext> fontFeatureValues() {
			return getRuleContexts(FontFeatureValuesContext.class);
		}
		public FontFeatureValuesContext fontFeatureValues(int i) {
			return getRuleContext(FontFeatureValuesContext.class,i);
		}
		public List<UnknownRuleContext> unknownRule() {
			return getRuleContexts(UnknownRuleContext.class);
		}
		public UnknownRuleContext unknownRule(int i) {
			return getRuleContext(UnknownRuleContext.class,i);
		}
		public GroupRuleBodyContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_groupRuleBody; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterGroupRuleBody(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitGroupRuleBody(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitGroupRuleBody(this);
			else return visitor.visitChildren(this);
		}
	}

	public final GroupRuleBodyContext groupRuleBody() throws RecognitionException {
		GroupRuleBodyContext _localctx = new GroupRuleBodyContext(_ctx, getState());
		enterRule(_localctx, 124, RULE_groupRuleBody);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(945);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (((((_la - 7)) & ~0x3f) == 0 && ((1L << (_la - 7)) & 2306405961306345217L) != 0)) {
				{
				setState(943);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,170,_ctx) ) {
				case 1:
					{
					setState(933);
					ruleset();
					}
					break;
				case 2:
					{
					setState(934);
					media();
					}
					break;
				case 3:
					{
					setState(935);
					page();
					}
					break;
				case 4:
					{
					setState(936);
					fontFace();
					}
					break;
				case 5:
					{
					setState(937);
					keyframes();
					}
					break;
				case 6:
					{
					setState(938);
					supports();
					}
					break;
				case 7:
					{
					setState(939);
					viewport();
					}
					break;
				case 8:
					{
					setState(940);
					counterStyle();
					}
					break;
				case 9:
					{
					setState(941);
					fontFeatureValues();
					}
					break;
				case 10:
					{
					setState(942);
					unknownRule();
					}
					break;
				}
				}
				setState(947);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class UnknownRuleContext extends ParserRuleContext {
		public UnknownAtRuleContext unknownAtRule() {
			return getRuleContext(UnknownAtRuleContext.class,0);
		}
		public UnknownRuleContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_unknownRule; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterUnknownRule(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitUnknownRule(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitUnknownRule(this);
			else return visitor.visitChildren(this);
		}
	}

	public final UnknownRuleContext unknownRule() throws RecognitionException {
		UnknownRuleContext _localctx = new UnknownRuleContext(_ctx, getState());
		enterRule(_localctx, 126, RULE_unknownRule);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(948);
			unknownAtRule();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class UnknownAtRuleContext extends ParserRuleContext {
		public TerminalNode IDENT() { return getToken(CSS3Parser.IDENT, 0); }
		public List<AnyContext> any() {
			return getRuleContexts(AnyContext.class);
		}
		public AnyContext any(int i) {
			return getRuleContext(AnyContext.class,i);
		}
		public List<UnknownBlockContext> unknownBlock() {
			return getRuleContexts(UnknownBlockContext.class);
		}
		public UnknownBlockContext unknownBlock(int i) {
			return getRuleContext(UnknownBlockContext.class,i);
		}
		public UnknownAtRuleContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_unknownAtRule; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterUnknownAtRule(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitUnknownAtRule(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitUnknownAtRule(this);
			else return visitor.visitChildren(this);
		}
	}

	public final UnknownAtRuleContext unknownAtRule() throws RecognitionException {
		UnknownAtRuleContext _localctx = new UnknownAtRuleContext(_ctx, getState());
		enterRule(_localctx, 128, RULE_unknownAtRule);
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(950);
			match(T__32);
			setState(951);
			match(IDENT);
			setState(960);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,173,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					setState(958);
					_errHandler.sync(this);
					switch (_input.LA(1)) {
					case T__5:
					case T__6:
					case T__17:
					case INCLUDES:
					case DASHMATCH:
					case HASH:
					case STRING:
					case NUMBER:
					case PERCENTAGE:
					case LENGTH:
					case EMS:
					case EXS:
					case ANGLE:
					case TIME:
					case FREQ:
					case RESOLUTION:
					case UNICODE_RANGE:
					case IDENT:
					case FUNCTION:
					case URL:
					case COMMENT:
					case WS:
						{
						setState(952);
						any();
						}
						break;
					case T__1:
						{
						setState(953);
						match(T__1);
						}
						break;
					case T__8:
						{
						setState(954);
						match(T__8);
						setState(955);
						unknownBlock();
						setState(956);
						match(T__9);
						}
						break;
					default:
						throw new NoViableAltException(this);
					}
					} 
				}
				setState(962);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,173,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class UnknownBlockContext extends ParserRuleContext {
		public List<AnyContext> any() {
			return getRuleContexts(AnyContext.class);
		}
		public AnyContext any(int i) {
			return getRuleContext(AnyContext.class,i);
		}
		public UnknownBlockContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_unknownBlock; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterUnknownBlock(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitUnknownBlock(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitUnknownBlock(this);
			else return visitor.visitChildren(this);
		}
	}

	public final UnknownBlockContext unknownBlock() throws RecognitionException {
		UnknownBlockContext _localctx = new UnknownBlockContext(_ctx, getState());
		enterRule(_localctx, 130, RULE_unknownBlock);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(967);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & -71635381572599612L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 511L) != 0)) {
				{
				setState(965);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case T__5:
				case T__6:
				case T__17:
				case INCLUDES:
				case DASHMATCH:
				case HASH:
				case STRING:
				case NUMBER:
				case PERCENTAGE:
				case LENGTH:
				case EMS:
				case EXS:
				case ANGLE:
				case TIME:
				case FREQ:
				case RESOLUTION:
				case UNICODE_RANGE:
				case IDENT:
				case FUNCTION:
				case URL:
				case COMMENT:
				case WS:
					{
					setState(963);
					any();
					}
					break;
				case T__1:
					{
					setState(964);
					match(T__1);
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				setState(969);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class CdoCdcContext extends ParserRuleContext {
		public TerminalNode CDO() { return getToken(CSS3Parser.CDO, 0); }
		public TerminalNode CDC() { return getToken(CSS3Parser.CDC, 0); }
		public CdoCdcContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_cdoCdc; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterCdoCdc(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitCdoCdc(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitCdoCdc(this);
			else return visitor.visitChildren(this);
		}
	}

	public final CdoCdcContext cdoCdc() throws RecognitionException {
		CdoCdcContext _localctx = new CdoCdcContext(_ctx, getState());
		enterRule(_localctx, 132, RULE_cdoCdc);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(970);
			_la = _input.LA(1);
			if ( !(_la==CDO || _la==CDC) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class WsContext extends ParserRuleContext {
		public List<TerminalNode> COMMENT() { return getTokens(CSS3Parser.COMMENT); }
		public TerminalNode COMMENT(int i) {
			return getToken(CSS3Parser.COMMENT, i);
		}
		public List<TerminalNode> WS() { return getTokens(CSS3Parser.WS); }
		public TerminalNode WS(int i) {
			return getToken(CSS3Parser.WS, i);
		}
		public WsContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_ws; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).enterWs(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof CSS3Listener ) ((CSS3Listener)listener).exitWs(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CSS3Visitor ) return ((CSS3Visitor<? extends T>)visitor).visitWs(this);
			else return visitor.visitChildren(this);
		}
	}

	public final WsContext ws() throws RecognitionException {
		WsContext _localctx = new WsContext(_ctx, getState());
		enterRule(_localctx, 134, RULE_ws);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(973); 
			_errHandler.sync(this);
			_alt = 1;
			do {
				switch (_alt) {
				case 1:
					{
					{
					setState(972);
					_la = _input.LA(1);
					if ( !(_la==COMMENT || _la==WS) ) {
					_errHandler.recoverInline(this);
					}
					else {
						if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
						_errHandler.reportMatch(this);
						consume();
					}
					}
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				setState(975); 
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,176,_ctx);
			} while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER );
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\u0004\u0001H\u03d2\u0002\u0000\u0007\u0000\u0002\u0001\u0007\u0001\u0002"+
		"\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002\u0004\u0007\u0004\u0002"+
		"\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002\u0007\u0007\u0007\u0002"+
		"\b\u0007\b\u0002\t\u0007\t\u0002\n\u0007\n\u0002\u000b\u0007\u000b\u0002"+
		"\f\u0007\f\u0002\r\u0007\r\u0002\u000e\u0007\u000e\u0002\u000f\u0007\u000f"+
		"\u0002\u0010\u0007\u0010\u0002\u0011\u0007\u0011\u0002\u0012\u0007\u0012"+
		"\u0002\u0013\u0007\u0013\u0002\u0014\u0007\u0014\u0002\u0015\u0007\u0015"+
		"\u0002\u0016\u0007\u0016\u0002\u0017\u0007\u0017\u0002\u0018\u0007\u0018"+
		"\u0002\u0019\u0007\u0019\u0002\u001a\u0007\u001a\u0002\u001b\u0007\u001b"+
		"\u0002\u001c\u0007\u001c\u0002\u001d\u0007\u001d\u0002\u001e\u0007\u001e"+
		"\u0002\u001f\u0007\u001f\u0002 \u0007 \u0002!\u0007!\u0002\"\u0007\"\u0002"+
		"#\u0007#\u0002$\u0007$\u0002%\u0007%\u0002&\u0007&\u0002\'\u0007\'\u0002"+
		"(\u0007(\u0002)\u0007)\u0002*\u0007*\u0002+\u0007+\u0002,\u0007,\u0002"+
		"-\u0007-\u0002.\u0007.\u0002/\u0007/\u00020\u00070\u00021\u00071\u0002"+
		"2\u00072\u00023\u00073\u00024\u00074\u00025\u00075\u00026\u00076\u0002"+
		"7\u00077\u00028\u00078\u00029\u00079\u0002:\u0007:\u0002;\u0007;\u0002"+
		"<\u0007<\u0002=\u0007=\u0002>\u0007>\u0002?\u0007?\u0002@\u0007@\u0002"+
		"A\u0007A\u0002B\u0007B\u0002C\u0007C\u0001\u0000\u0003\u0000\u008a\b\u0000"+
		"\u0001\u0000\u0003\u0000\u008d\b\u0000\u0001\u0000\u0001\u0000\u0003\u0000"+
		"\u0091\b\u0000\u0005\u0000\u0093\b\u0000\n\u0000\f\u0000\u0096\t\u0000"+
		"\u0001\u0000\u0001\u0000\u0003\u0000\u009a\b\u0000\u0005\u0000\u009c\b"+
		"\u0000\n\u0000\f\u0000\u009f\t\u0000\u0001\u0000\u0001\u0000\u0003\u0000"+
		"\u00a3\b\u0000\u0001\u0000\u0001\u0000\u0003\u0000\u00a7\b\u0000\u0005"+
		"\u0000\u00a9\b\u0000\n\u0000\f\u0000\u00ac\t\u0000\u0001\u0000\u0001\u0000"+
		"\u0001\u0001\u0001\u0001\u0003\u0001\u00b2\b\u0001\u0001\u0001\u0001\u0001"+
		"\u0003\u0001\u00b6\b\u0001\u0001\u0001\u0001\u0001\u0001\u0002\u0001\u0002"+
		"\u0003\u0002\u00bc\b\u0002\u0001\u0002\u0001\u0002\u0003\u0002\u00c0\b"+
		"\u0002\u0001\u0002\u0003\u0002\u00c3\b\u0002\u0001\u0002\u0003\u0002\u00c6"+
		"\b\u0002\u0001\u0002\u0001\u0002\u0001\u0003\u0001\u0003\u0003\u0003\u00cc"+
		"\b\u0003\u0001\u0003\u0001\u0003\u0003\u0003\u00d0\b\u0003\u0003\u0003"+
		"\u00d2\b\u0003\u0001\u0003\u0001\u0003\u0003\u0003\u00d6\b\u0003\u0001"+
		"\u0003\u0003\u0003\u00d9\b\u0003\u0001\u0003\u0001\u0003\u0001\u0004\u0001"+
		"\u0004\u0001\u0005\u0003\u0005\u00e0\b\u0005\u0001\u0005\u0001\u0005\u0003"+
		"\u0005\u00e4\b\u0005\u0001\u0005\u0001\u0005\u0003\u0005\u00e8\b\u0005"+
		"\u0001\u0005\u0005\u0005\u00eb\b\u0005\n\u0005\f\u0005\u00ee\t\u0005\u0001"+
		"\u0006\u0003\u0006\u00f1\b\u0006\u0001\u0006\u0003\u0006\u00f4\b\u0006"+
		"\u0001\u0006\u0001\u0006\u0003\u0006\u00f8\b\u0006\u0001\u0006\u0001\u0006"+
		"\u0003\u0006\u00fc\b\u0006\u0001\u0006\u0005\u0006\u00ff\b\u0006\n\u0006"+
		"\f\u0006\u0102\t\u0006\u0001\u0006\u0001\u0006\u0003\u0006\u0106\b\u0006"+
		"\u0001\u0006\u0001\u0006\u0003\u0006\u010a\b\u0006\u0001\u0006\u0005\u0006"+
		"\u010d\b\u0006\n\u0006\f\u0006\u0110\t\u0006\u0003\u0006\u0112\b\u0006"+
		"\u0001\u0007\u0001\u0007\u0001\b\u0001\b\u0003\b\u0118\b\b\u0001\b\u0001"+
		"\b\u0003\b\u011c\b\b\u0001\b\u0001\b\u0003\b\u0120\b\b\u0001\b\u0003\b"+
		"\u0123\b\b\u0001\b\u0003\b\u0126\b\b\u0001\b\u0001\b\u0001\t\u0001\t\u0001"+
		"\n\u0001\n\u0001\n\u0001\n\u0001\n\u0001\n\u0001\n\u0001\n\u0001\n\u0001"+
		"\n\u0003\n\u0136\b\n\u0001\u000b\u0001\u000b\u0003\u000b\u013a\b\u000b"+
		"\u0001\u000b\u0001\u000b\u0003\u000b\u013e\b\u000b\u0001\u000b\u0003\u000b"+
		"\u0141\b\u000b\u0001\u000b\u0001\u000b\u0003\u000b\u0145\b\u000b\u0001"+
		"\f\u0001\f\u0003\f\u0149\b\f\u0001\f\u0001\f\u0003\f\u014d\b\f\u0001\f"+
		"\u0005\f\u0150\b\f\n\f\f\f\u0153\t\f\u0001\r\u0001\r\u0003\r\u0157\b\r"+
		"\u0001\r\u0001\r\u0001\r\u0003\r\u015c\b\r\u0005\r\u015e\b\r\n\r\f\r\u0161"+
		"\t\r\u0001\u000e\u0003\u000e\u0164\b\u000e\u0001\u000e\u0001\u000e\u0003"+
		"\u000e\u0168\b\u000e\u0001\u000f\u0001\u000f\u0003\u000f\u016c\b\u000f"+
		"\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u000f\u0005\u000f"+
		"\u0173\b\u000f\n\u000f\f\u000f\u0176\t\u000f\u0001\u000f\u0001\u000f\u0001"+
		"\u000f\u0001\u000f\u0001\u000f\u0004\u000f\u017d\b\u000f\u000b\u000f\f"+
		"\u000f\u017e\u0003\u000f\u0181\b\u000f\u0001\u0010\u0003\u0010\u0184\b"+
		"\u0010\u0001\u0010\u0001\u0010\u0001\u0011\u0001\u0011\u0003\u0011\u018a"+
		"\b\u0011\u0001\u0011\u0001\u0011\u0001\u0012\u0001\u0012\u0001\u0013\u0003"+
		"\u0013\u0191\b\u0013\u0001\u0013\u0001\u0013\u0001\u0014\u0001\u0014\u0001"+
		"\u0014\u0001\u0015\u0001\u0015\u0003\u0015\u019a\b\u0015\u0001\u0015\u0003"+
		"\u0015\u019d\b\u0015\u0001\u0015\u0001\u0015\u0003\u0015\u01a1\b\u0015"+
		"\u0001\u0015\u0001\u0015\u0003\u0015\u01a5\b\u0015\u0001\u0015\u0001\u0015"+
		"\u0003\u0015\u01a9\b\u0015\u0003\u0015\u01ab\b\u0015\u0001\u0015\u0001"+
		"\u0015\u0001\u0016\u0001\u0016\u0001\u0017\u0001\u0017\u0003\u0017\u01b3"+
		"\b\u0017\u0001\u0017\u0001\u0017\u0003\u0017\u01b7\b\u0017\u0001\u0018"+
		"\u0001\u0018\u0003\u0018\u01bb\b\u0018\u0001\u0018\u0001\u0018\u0001\u0018"+
		"\u0001\u0019\u0001\u0019\u0003\u0019\u01c2\b\u0019\u0004\u0019\u01c4\b"+
		"\u0019\u000b\u0019\f\u0019\u01c5\u0001\u001a\u0001\u001a\u0003\u001a\u01ca"+
		"\b\u001a\u0001\u001a\u0001\u001a\u0003\u001a\u01ce\b\u001a\u0001\u001a"+
		"\u0001\u001a\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b\u0001\u001b"+
		"\u0001\u001b\u0003\u001b\u01d8\b\u001b\u0001\u001c\u0001\u001c\u0001\u001c"+
		"\u0003\u001c\u01dd\b\u001c\u0005\u001c\u01df\b\u001c\n\u001c\f\u001c\u01e2"+
		"\t\u001c\u0001\u001c\u0003\u001c\u01e5\b\u001c\u0001\u001d\u0001\u001d"+
		"\u0003\u001d\u01e9\b\u001d\u0001\u001d\u0001\u001d\u0003\u001d\u01ed\b"+
		"\u001d\u0001\u001d\u0001\u001d\u0003\u001d\u01f1\b\u001d\u0001\u001e\u0001"+
		"\u001e\u0001\u001f\u0003\u001f\u01f6\b\u001f\u0001\u001f\u0001\u001f\u0003"+
		"\u001f\u01fa\b\u001f\u0001\u001f\u0001\u001f\u0001 \u0001 \u0003 \u0200"+
		"\b \u0001 \u0005 \u0203\b \n \f \u0206\t \u0001!\u0003!\u0209\b!\u0001"+
		"!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001!\u0001"+
		"!\u0001!\u0003!\u0217\b!\u0001!\u0001!\u0001!\u0003!\u021c\b!\u0001\""+
		"\u0001\"\u0001\"\u0003\"\u0221\b\"\u0001#\u0001#\u0001$\u0001$\u0003$"+
		"\u0227\b$\u0001$\u0001$\u0003$\u022b\b$\u0001$\u0001$\u0001%\u0001%\u0001"+
		"%\u0001%\u0003%\u0233\b%\u0001%\u0001%\u0001%\u0003%\u0238\b%\u0001&\u0001"+
		"&\u0003&\u023c\b&\u0001&\u0003&\u023f\b&\u0001&\u0001&\u0003&\u0243\b"+
		"&\u0001&\u0003&\u0246\b&\u0001&\u0001&\u0003&\u024a\b&\u0001\'\u0001\'"+
		"\u0001\'\u0001(\u0001(\u0003(\u0251\b(\u0001(\u0001(\u0003(\u0255\b(\u0001"+
		"(\u0003(\u0258\b(\u0001(\u0001(\u0003(\u025c\b(\u0001)\u0001)\u0001)\u0003"+
		")\u0261\b)\u0001)\u0001)\u0003)\u0265\b)\u0001)\u0001)\u0003)\u0269\b"+
		")\u0001)\u0001)\u0001)\u0003)\u026e\b)\u0001*\u0001*\u0003*\u0272\b*\u0001"+
		"*\u0001*\u0003*\u0276\b*\u0001*\u0003*\u0279\b*\u0001*\u0001*\u0003*\u027d"+
		"\b*\u0001*\u0001*\u0003*\u0281\b*\u0001*\u0001*\u0003*\u0285\b*\u0001"+
		"*\u0003*\u0288\b*\u0001*\u0001*\u0003*\u028c\b*\u0005*\u028e\b*\n*\f*"+
		"\u0291\t*\u0001+\u0001+\u0003+\u0295\b+\u0001+\u0001+\u0003+\u0299\b+"+
		"\u0001+\u0005+\u029c\b+\n+\f+\u029f\t+\u0001,\u0001,\u0003,\u02a3\b,\u0001"+
		",\u0001,\u0003,\u02a7\b,\u0001,\u0001,\u0003,\u02ab\b,\u0001,\u0001,\u0001"+
		",\u0003,\u02b0\b,\u0001-\u0001-\u0001-\u0001-\u0003-\u02b6\b-\u0001.\u0001"+
		".\u0003.\u02ba\b.\u0001.\u0001.\u0003.\u02be\b.\u0001.\u0001.\u0001.\u0001"+
		".\u0003.\u02c4\b.\u0001/\u0001/\u0001/\u0001/\u00010\u00010\u00010\u0001"+
		"0\u00010\u00010\u00040\u02d0\b0\u000b0\f0\u02d1\u00011\u00011\u00011\u0001"+
		"1\u00011\u00011\u00041\u02da\b1\u000b1\f1\u02db\u00012\u00012\u00032\u02e0"+
		"\b2\u00012\u00012\u00032\u02e4\b2\u00012\u00012\u00013\u00013\u00013\u0005"+
		"3\u02eb\b3\n3\f3\u02ee\t3\u00013\u00013\u00013\u00013\u00053\u02f4\b3"+
		"\n3\f3\u02f7\t3\u00013\u00033\u02fa\b3\u00014\u00014\u00014\u00014\u0001"+
		"4\u00014\u00014\u00014\u00014\u00014\u00014\u00014\u00014\u00014\u0001"+
		"4\u00014\u00014\u00014\u00014\u00014\u00054\u0310\b4\n4\f4\u0313\t4\u0001"+
		"4\u00014\u00014\u00054\u0318\b4\n4\f4\u031b\t4\u00014\u00014\u00034\u031f"+
		"\b4\u00015\u00015\u00035\u0323\b5\u00015\u00015\u00035\u0327\b5\u0001"+
		"5\u00035\u032a\b5\u00015\u00015\u00035\u032e\b5\u00016\u00016\u00036\u0332"+
		"\b6\u00016\u00016\u00036\u0336\b6\u00016\u00016\u00036\u033a\b6\u0001"+
		"6\u00036\u033d\b6\u00016\u00016\u00036\u0341\b6\u00017\u00017\u00037\u0345"+
		"\b7\u00017\u00017\u00037\u0349\b7\u00017\u00017\u00037\u034d\b7\u0001"+
		"7\u00057\u0350\b7\n7\f7\u0353\t7\u00017\u00017\u00037\u0357\b7\u00018"+
		"\u00018\u00038\u035b\b8\u00018\u00018\u00038\u035f\b8\u00018\u00058\u0362"+
		"\b8\n8\f8\u0365\t8\u00019\u00019\u00019\u00039\u036a\b9\u00049\u036c\b"+
		"9\u000b9\f9\u036d\u00039\u0370\b9\u0001:\u0001:\u0003:\u0374\b:\u0001"+
		":\u0001:\u0003:\u0378\b:\u0001:\u0003:\u037b\b:\u0001:\u0001:\u0003:\u037f"+
		"\b:\u0001;\u0001;\u0001<\u0001<\u0003<\u0385\b<\u0001<\u0001<\u0003<\u0389"+
		"\b<\u0001<\u0005<\u038c\b<\n<\f<\u038f\t<\u0001<\u0003<\u0392\b<\u0001"+
		"=\u0001=\u0003=\u0396\b=\u0001=\u0001=\u0003=\u039a\b=\u0001=\u0001=\u0003"+
		"=\u039e\b=\u0001=\u0005=\u03a1\b=\n=\f=\u03a4\t=\u0001>\u0001>\u0001>"+
		"\u0001>\u0001>\u0001>\u0001>\u0001>\u0001>\u0001>\u0005>\u03b0\b>\n>\f"+
		">\u03b3\t>\u0001?\u0001?\u0001@\u0001@\u0001@\u0001@\u0001@\u0001@\u0001"+
		"@\u0001@\u0005@\u03bf\b@\n@\f@\u03c2\t@\u0001A\u0001A\u0005A\u03c6\bA"+
		"\nA\fA\u03c9\tA\u0001B\u0001B\u0001C\u0004C\u03ce\bC\u000bC\fC\u03cf\u0001"+
		"C\u0000\u0000D\u0000\u0002\u0004\u0006\b\n\f\u000e\u0010\u0012\u0014\u0016"+
		"\u0018\u001a\u001c\u001e \"$&(*,.02468:<>@BDFHJLNPRTVXZ\\^`bdfhjlnprt"+
		"vxz|~\u0080\u0082\u0084\u0086\u0000\u000b\u0001\u000045\u0001\u0000\u000b"+
		"\u000e\u0002\u000099DD\u0002\u0000\u0014\u0017/0\u0001\u0000:B\u0002\u0000"+
		"\u000b\u000b\u001c\u001c\u0002\u000099FF\u0002\u000012;;\u0001\u0000&"+
		",\u0001\u0000-.\u0001\u0000GH\u0479\u0000\u0089\u0001\u0000\u0000\u0000"+
		"\u0002\u00af\u0001\u0000\u0000\u0000\u0004\u00b9\u0001\u0000\u0000\u0000"+
		"\u0006\u00c9\u0001\u0000\u0000\u0000\b\u00dc\u0001\u0000\u0000\u0000\n"+
		"\u00df\u0001\u0000\u0000\u0000\f\u0111\u0001\u0000\u0000\u0000\u000e\u0113"+
		"\u0001\u0000\u0000\u0000\u0010\u0115\u0001\u0000\u0000\u0000\u0012\u0129"+
		"\u0001\u0000\u0000\u0000\u0014\u0135\u0001\u0000\u0000\u0000\u0016\u0137"+
		"\u0001\u0000\u0000\u0000\u0018\u0146\u0001\u0000\u0000\u0000\u001a\u0154"+
		"\u0001\u0000\u0000\u0000\u001c\u0167\u0001\u0000\u0000\u0000\u001e\u0180"+
		"\u0001\u0000\u0000\u0000 \u0183\u0001\u0000\u0000\u0000\"\u0189\u0001"+
		"\u0000\u0000\u0000$\u018d\u0001\u0000\u0000\u0000&\u0190\u0001\u0000\u0000"+
		"\u0000(\u0194\u0001\u0000\u0000\u0000*\u0197\u0001\u0000\u0000\u0000,"+
		"\u01ae\u0001\u0000\u0000\u0000.\u01b0\u0001\u0000\u0000\u00000\u01b8\u0001"+
		"\u0000\u0000\u00002\u01c3\u0001\u0000\u0000\u00004\u01c7\u0001\u0000\u0000"+
		"\u00006\u01d7\u0001\u0000\u0000\u00008\u01e0\u0001\u0000\u0000\u0000:"+
		"\u01e6\u0001\u0000\u0000\u0000<\u01f2\u0001\u0000\u0000\u0000>\u01f5\u0001"+
		"\u0000\u0000\u0000@\u01fd\u0001\u0000\u0000\u0000B\u021b\u0001\u0000\u0000"+
		"\u0000D\u0220\u0001\u0000\u0000\u0000F\u0222\u0001\u0000\u0000\u0000H"+
		"\u0224\u0001\u0000\u0000\u0000J\u022e\u0001\u0000\u0000\u0000L\u0239\u0001"+
		"\u0000\u0000\u0000N\u024b\u0001\u0000\u0000\u0000P\u024e\u0001\u0000\u0000"+
		"\u0000R\u025d\u0001\u0000\u0000\u0000T\u026f\u0001\u0000\u0000\u0000V"+
		"\u0292\u0001\u0000\u0000\u0000X\u02a0\u0001\u0000\u0000\u0000Z\u02b5\u0001"+
		"\u0000\u0000\u0000\\\u02c3\u0001\u0000\u0000\u0000^\u02c5\u0001\u0000"+
		"\u0000\u0000`\u02c9\u0001\u0000\u0000\u0000b\u02d3\u0001\u0000\u0000\u0000"+
		"d\u02dd\u0001\u0000\u0000\u0000f\u02f9\u0001\u0000\u0000\u0000h\u031e"+
		"\u0001\u0000\u0000\u0000j\u0320\u0001\u0000\u0000\u0000l\u032f\u0001\u0000"+
		"\u0000\u0000n\u0342\u0001\u0000\u0000\u0000p\u0358\u0001\u0000\u0000\u0000"+
		"r\u036f\u0001\u0000\u0000\u0000t\u0371\u0001\u0000\u0000\u0000v\u0380"+
		"\u0001\u0000\u0000\u0000x\u0382\u0001\u0000\u0000\u0000z\u0393\u0001\u0000"+
		"\u0000\u0000|\u03b1\u0001\u0000\u0000\u0000~\u03b4\u0001\u0000\u0000\u0000"+
		"\u0080\u03b6\u0001\u0000\u0000\u0000\u0082\u03c7\u0001\u0000\u0000\u0000"+
		"\u0084\u03ca\u0001\u0000\u0000\u0000\u0086\u03cd\u0001\u0000\u0000\u0000"+
		"\u0088\u008a\u0003\u0086C\u0000\u0089\u0088\u0001\u0000\u0000\u0000\u0089"+
		"\u008a\u0001\u0000\u0000\u0000\u008a\u008c\u0001\u0000\u0000\u0000\u008b"+
		"\u008d\u0003\u0002\u0001\u0000\u008c\u008b\u0001\u0000\u0000\u0000\u008c"+
		"\u008d\u0001\u0000\u0000\u0000\u008d\u0094\u0001\u0000\u0000\u0000\u008e"+
		"\u0090\u0003\u0004\u0002\u0000\u008f\u0091\u0003\u0086C\u0000\u0090\u008f"+
		"\u0001\u0000\u0000\u0000\u0090\u0091\u0001\u0000\u0000\u0000\u0091\u0093"+
		"\u0001\u0000\u0000\u0000\u0092\u008e\u0001\u0000\u0000\u0000\u0093\u0096"+
		"\u0001\u0000\u0000\u0000\u0094\u0092\u0001\u0000\u0000\u0000\u0094\u0095"+
		"\u0001\u0000\u0000\u0000\u0095\u009d\u0001\u0000\u0000\u0000\u0096\u0094"+
		"\u0001\u0000\u0000\u0000\u0097\u0099\u0003\u0006\u0003\u0000\u0098\u009a"+
		"\u0003\u0086C\u0000\u0099\u0098\u0001\u0000\u0000\u0000\u0099\u009a\u0001"+
		"\u0000\u0000\u0000\u009a\u009c\u0001\u0000\u0000\u0000\u009b\u0097\u0001"+
		"\u0000\u0000\u0000\u009c\u009f\u0001\u0000\u0000\u0000\u009d\u009b\u0001"+
		"\u0000\u0000\u0000\u009d\u009e\u0001\u0000\u0000\u0000\u009e\u00aa\u0001"+
		"\u0000\u0000\u0000\u009f\u009d\u0001\u0000\u0000\u0000\u00a0\u00a2\u0003"+
		"\u0084B\u0000\u00a1\u00a3\u0003\u0086C\u0000\u00a2\u00a1\u0001\u0000\u0000"+
		"\u0000\u00a2\u00a3\u0001\u0000\u0000\u0000\u00a3\u00a9\u0001\u0000\u0000"+
		"\u0000\u00a4\u00a6\u0003\u0014\n\u0000\u00a5\u00a7\u0003\u0086C\u0000"+
		"\u00a6\u00a5\u0001\u0000\u0000\u0000\u00a6\u00a7\u0001\u0000\u0000\u0000"+
		"\u00a7\u00a9\u0001\u0000\u0000\u0000\u00a8\u00a0\u0001\u0000\u0000\u0000"+
		"\u00a8\u00a4\u0001\u0000\u0000\u0000\u00a9\u00ac\u0001\u0000\u0000\u0000"+
		"\u00aa\u00a8\u0001\u0000\u0000\u0000\u00aa\u00ab\u0001\u0000\u0000\u0000"+
		"\u00ab\u00ad\u0001\u0000\u0000\u0000\u00ac\u00aa\u0001\u0000\u0000\u0000"+
		"\u00ad\u00ae\u0005\u0000\u0000\u0001\u00ae\u0001\u0001\u0000\u0000\u0000"+
		"\u00af\u00b1\u0005\u0001\u0000\u0000\u00b0\u00b2\u0003\u0086C\u0000\u00b1"+
		"\u00b0\u0001\u0000\u0000\u0000\u00b1\u00b2\u0001\u0000\u0000\u0000\u00b2"+
		"\u00b3\u0001\u0000\u0000\u0000\u00b3\u00b5\u00059\u0000\u0000\u00b4\u00b6"+
		"\u0003\u0086C\u0000\u00b5\u00b4\u0001\u0000\u0000\u0000\u00b5\u00b6\u0001"+
		"\u0000\u0000\u0000\u00b6\u00b7\u0001\u0000\u0000\u0000\u00b7\u00b8\u0005"+
		"\u0002\u0000\u0000\u00b8\u0003\u0001\u0000\u0000\u0000\u00b9\u00bb\u0005"+
		"\u0003\u0000\u0000\u00ba\u00bc\u0003\u0086C\u0000\u00bb\u00ba\u0001\u0000"+
		"\u0000\u0000\u00bb\u00bc\u0001\u0000\u0000\u0000\u00bc\u00bf\u0001\u0000"+
		"\u0000\u0000\u00bd\u00c0\u00059\u0000\u0000\u00be\u00c0\u0003H$\u0000"+
		"\u00bf\u00bd\u0001\u0000\u0000\u0000\u00bf\u00be\u0001\u0000\u0000\u0000"+
		"\u00c0\u00c2\u0001\u0000\u0000\u0000\u00c1\u00c3\u0003\u0086C\u0000\u00c2"+
		"\u00c1\u0001\u0000\u0000\u0000\u00c2\u00c3\u0001\u0000\u0000\u0000\u00c3"+
		"\u00c5\u0001\u0000\u0000\u0000\u00c4\u00c6\u0003\n\u0005\u0000\u00c5\u00c4"+
		"\u0001\u0000\u0000\u0000\u00c5\u00c6\u0001\u0000\u0000\u0000\u00c6\u00c7"+
		"\u0001\u0000\u0000\u0000\u00c7\u00c8\u0005\u0002\u0000\u0000\u00c8\u0005"+
		"\u0001\u0000\u0000\u0000\u00c9\u00cb\u0005\u0004\u0000\u0000\u00ca\u00cc"+
		"\u0003\u0086C\u0000\u00cb\u00ca\u0001\u0000\u0000\u0000\u00cb\u00cc\u0001"+
		"\u0000\u0000\u0000\u00cc\u00d1\u0001\u0000\u0000\u0000\u00cd\u00cf\u0003"+
		"\b\u0004\u0000\u00ce\u00d0\u0003\u0086C\u0000\u00cf\u00ce\u0001\u0000"+
		"\u0000\u0000\u00cf\u00d0\u0001\u0000\u0000\u0000\u00d0\u00d2\u0001\u0000"+
		"\u0000\u0000\u00d1\u00cd\u0001\u0000\u0000\u0000\u00d1\u00d2\u0001\u0000"+
		"\u0000\u0000\u00d2\u00d5\u0001\u0000\u0000\u0000\u00d3\u00d6\u00059\u0000"+
		"\u0000\u00d4\u00d6\u0003H$\u0000\u00d5\u00d3\u0001\u0000\u0000\u0000\u00d5"+
		"\u00d4\u0001\u0000\u0000\u0000\u00d6\u00d8\u0001\u0000\u0000\u0000\u00d7"+
		"\u00d9\u0003\u0086C\u0000\u00d8\u00d7\u0001\u0000\u0000\u0000\u00d8\u00d9"+
		"\u0001\u0000\u0000\u0000\u00d9\u00da\u0001\u0000\u0000\u0000\u00da\u00db"+
		"\u0005\u0002\u0000\u0000\u00db\u0007\u0001\u0000\u0000\u0000\u00dc\u00dd"+
		"\u0005D\u0000\u0000\u00dd\t\u0001\u0000\u0000\u0000\u00de\u00e0\u0003"+
		"\u0086C\u0000\u00df\u00de\u0001\u0000\u0000\u0000\u00df\u00e0\u0001\u0000"+
		"\u0000\u0000\u00e0\u00e1\u0001\u0000\u0000\u0000\u00e1\u00ec\u0003\f\u0006"+
		"\u0000\u00e2\u00e4\u0003\u0086C\u0000\u00e3\u00e2\u0001\u0000\u0000\u0000"+
		"\u00e3\u00e4\u0001\u0000\u0000\u0000\u00e4\u00e5\u0001\u0000\u0000\u0000"+
		"\u00e5\u00e7\u0005\u0005\u0000\u0000\u00e6\u00e8\u0003\u0086C\u0000\u00e7"+
		"\u00e6\u0001\u0000\u0000\u0000\u00e7\u00e8\u0001\u0000\u0000\u0000\u00e8"+
		"\u00e9\u0001\u0000\u0000\u0000\u00e9\u00eb\u0003\f\u0006\u0000\u00ea\u00e3"+
		"\u0001\u0000\u0000\u0000\u00eb\u00ee\u0001\u0000\u0000\u0000\u00ec\u00ea"+
		"\u0001\u0000\u0000\u0000\u00ec\u00ed\u0001\u0000\u0000\u0000\u00ed\u000b"+
		"\u0001\u0000\u0000\u0000\u00ee\u00ec\u0001\u0000\u0000\u0000\u00ef\u00f1"+
		"\u0007\u0000\u0000\u0000\u00f0\u00ef\u0001\u0000\u0000\u0000\u00f0\u00f1"+
		"\u0001\u0000\u0000\u0000\u00f1\u00f3\u0001\u0000\u0000\u0000\u00f2\u00f4"+
		"\u0003\u0086C\u0000\u00f3\u00f2\u0001\u0000\u0000\u0000\u00f3\u00f4\u0001"+
		"\u0000\u0000\u0000\u00f4\u00f5\u0001\u0000\u0000\u0000\u00f5\u00f7\u0003"+
		"\u000e\u0007\u0000\u00f6\u00f8\u0003\u0086C\u0000\u00f7\u00f6\u0001\u0000"+
		"\u0000\u0000\u00f7\u00f8\u0001\u0000\u0000\u0000\u00f8\u0100\u0001\u0000"+
		"\u0000\u0000\u00f9\u00fb\u00056\u0000\u0000\u00fa\u00fc\u0003\u0086C\u0000"+
		"\u00fb\u00fa\u0001\u0000\u0000\u0000\u00fb\u00fc\u0001\u0000\u0000\u0000"+
		"\u00fc\u00fd\u0001\u0000\u0000\u0000\u00fd\u00ff\u0003\u0010\b\u0000\u00fe"+
		"\u00f9\u0001\u0000\u0000\u0000\u00ff\u0102\u0001\u0000\u0000\u0000\u0100"+
		"\u00fe\u0001\u0000\u0000\u0000\u0100\u0101\u0001\u0000\u0000\u0000\u0101"+
		"\u0112\u0001\u0000\u0000\u0000\u0102\u0100\u0001\u0000\u0000\u0000\u0103"+
		"\u010e\u0003\u0010\b\u0000\u0104\u0106\u0003\u0086C\u0000\u0105\u0104"+
		"\u0001\u0000\u0000\u0000\u0105\u0106\u0001\u0000\u0000\u0000\u0106\u0107"+
		"\u0001\u0000\u0000\u0000\u0107\u0109\u00056\u0000\u0000\u0108\u010a\u0003"+
		"\u0086C\u0000\u0109\u0108\u0001\u0000\u0000\u0000\u0109\u010a\u0001\u0000"+
		"\u0000\u0000\u010a\u010b\u0001\u0000\u0000\u0000\u010b\u010d\u0003\u0010"+
		"\b\u0000\u010c\u0105\u0001\u0000\u0000\u0000\u010d\u0110\u0001\u0000\u0000"+
		"\u0000\u010e\u010c\u0001\u0000\u0000\u0000\u010e\u010f\u0001\u0000\u0000"+
		"\u0000\u010f\u0112\u0001\u0000\u0000\u0000\u0110\u010e\u0001\u0000\u0000"+
		"\u0000\u0111\u00f0\u0001\u0000\u0000\u0000\u0111\u0103\u0001\u0000\u0000"+
		"\u0000\u0112\r\u0001\u0000\u0000\u0000\u0113\u0114\u0005D\u0000\u0000"+
		"\u0114\u000f\u0001\u0000\u0000\u0000\u0115\u0117\u0005\u0006\u0000\u0000"+
		"\u0116\u0118\u0003\u0086C\u0000\u0117\u0116\u0001\u0000\u0000\u0000\u0117"+
		"\u0118\u0001\u0000\u0000\u0000\u0118\u0119\u0001\u0000\u0000\u0000\u0119"+
		"\u011b\u0003\u0012\t\u0000\u011a\u011c\u0003\u0086C\u0000\u011b\u011a"+
		"\u0001\u0000\u0000\u0000\u011b\u011c\u0001\u0000\u0000\u0000\u011c\u0122"+
		"\u0001\u0000\u0000\u0000\u011d\u011f\u0005\u0007\u0000\u0000\u011e\u0120"+
		"\u0003\u0086C\u0000\u011f\u011e\u0001\u0000\u0000\u0000\u011f\u0120\u0001"+
		"\u0000\u0000\u0000\u0120\u0121\u0001\u0000\u0000\u0000\u0121\u0123\u0003"+
		"@ \u0000\u0122\u011d\u0001\u0000\u0000\u0000\u0122\u0123\u0001\u0000\u0000"+
		"\u0000\u0123\u0125\u0001\u0000\u0000\u0000\u0124\u0126\u0003\u0086C\u0000"+
		"\u0125\u0124\u0001\u0000\u0000\u0000\u0125\u0126\u0001\u0000\u0000\u0000"+
		"\u0126\u0127\u0001\u0000\u0000\u0000\u0127\u0128\u0005\b\u0000\u0000\u0128"+
		"\u0011\u0001\u0000\u0000\u0000\u0129\u012a\u0005D\u0000\u0000\u012a\u0013"+
		"\u0001\u0000\u0000\u0000\u012b\u0136\u0003\u0016\u000b\u0000\u012c\u0136"+
		"\u0003J%\u0000\u012d\u0136\u0003L&\u0000\u012e\u0136\u0003P(\u0000\u012f"+
		"\u0136\u0003R)\u0000\u0130\u0136\u0003X,\u0000\u0131\u0136\u0003j5\u0000"+
		"\u0132\u0136\u0003l6\u0000\u0133\u0136\u0003n7\u0000\u0134\u0136\u0003"+
		"~?\u0000\u0135\u012b\u0001\u0000\u0000\u0000\u0135\u012c\u0001\u0000\u0000"+
		"\u0000\u0135\u012d\u0001\u0000\u0000\u0000\u0135\u012e\u0001\u0000\u0000"+
		"\u0000\u0135\u012f\u0001\u0000\u0000\u0000\u0135\u0130\u0001\u0000\u0000"+
		"\u0000\u0135\u0131\u0001\u0000\u0000\u0000\u0135\u0132\u0001\u0000\u0000"+
		"\u0000\u0135\u0133\u0001\u0000\u0000\u0000\u0135\u0134\u0001\u0000\u0000"+
		"\u0000\u0136\u0015\u0001\u0000\u0000\u0000\u0137\u0139\u0003\u0018\f\u0000"+
		"\u0138\u013a\u0003\u0086C\u0000\u0139\u0138\u0001\u0000\u0000\u0000\u0139"+
		"\u013a\u0001\u0000\u0000\u0000\u013a\u013b\u0001\u0000\u0000\u0000\u013b"+
		"\u013d\u0005\t\u0000\u0000\u013c\u013e\u0003\u0086C\u0000\u013d\u013c"+
		"\u0001\u0000\u0000\u0000\u013d\u013e\u0001\u0000\u0000\u0000\u013e\u0140"+
		"\u0001\u0000\u0000\u0000\u013f\u0141\u00038\u001c\u0000\u0140\u013f\u0001"+
		"\u0000\u0000\u0000\u0140\u0141\u0001\u0000\u0000\u0000\u0141\u0142\u0001"+
		"\u0000\u0000\u0000\u0142\u0144\u0005\n\u0000\u0000\u0143\u0145\u0003\u0086"+
		"C\u0000\u0144\u0143\u0001\u0000\u0000\u0000\u0144\u0145\u0001\u0000\u0000"+
		"\u0000\u0145\u0017\u0001\u0000\u0000\u0000\u0146\u0151\u0003\u001a\r\u0000"+
		"\u0147\u0149\u0003\u0086C\u0000\u0148\u0147\u0001\u0000\u0000\u0000\u0148"+
		"\u0149\u0001\u0000\u0000\u0000\u0149\u014a\u0001\u0000\u0000\u0000\u014a"+
		"\u014c\u0005\u0005\u0000\u0000\u014b\u014d\u0003\u0086C\u0000\u014c\u014b"+
		"\u0001\u0000\u0000\u0000\u014c\u014d\u0001\u0000\u0000\u0000\u014d\u014e"+
		"\u0001\u0000\u0000\u0000\u014e\u0150\u0003\u001a\r\u0000\u014f\u0148\u0001"+
		"\u0000\u0000\u0000\u0150\u0153\u0001\u0000\u0000\u0000\u0151\u014f\u0001"+
		"\u0000\u0000\u0000\u0151\u0152\u0001\u0000\u0000\u0000\u0152\u0019\u0001"+
		"\u0000\u0000\u0000\u0153\u0151\u0001\u0000\u0000\u0000\u0154\u0156\u0003"+
		"\u001e\u000f\u0000\u0155\u0157\u0003\u0086C\u0000\u0156\u0155\u0001\u0000"+
		"\u0000\u0000\u0156\u0157\u0001\u0000\u0000\u0000\u0157\u015f\u0001\u0000"+
		"\u0000\u0000\u0158\u0159\u0003\u001c\u000e\u0000\u0159\u015b\u0003\u001e"+
		"\u000f\u0000\u015a\u015c\u0003\u0086C\u0000\u015b\u015a\u0001\u0000\u0000"+
		"\u0000\u015b\u015c\u0001\u0000\u0000\u0000\u015c\u015e\u0001\u0000\u0000"+
		"\u0000\u015d\u0158\u0001\u0000\u0000\u0000\u015e\u0161\u0001\u0000\u0000"+
		"\u0000\u015f\u015d\u0001\u0000\u0000\u0000\u015f\u0160\u0001\u0000\u0000"+
		"\u0000\u0160\u001b\u0001\u0000\u0000\u0000\u0161\u015f\u0001\u0000\u0000"+
		"\u0000\u0162\u0164\u0003\u0086C\u0000\u0163\u0162\u0001\u0000\u0000\u0000"+
		"\u0163\u0164\u0001\u0000\u0000\u0000\u0164\u0165\u0001\u0000\u0000\u0000"+
		"\u0165\u0168\u0007\u0001\u0000\u0000\u0166\u0168\u0003\u0086C\u0000\u0167"+
		"\u0163\u0001\u0000\u0000\u0000\u0167\u0166\u0001\u0000\u0000\u0000\u0168"+
		"\u001d\u0001\u0000\u0000\u0000\u0169\u016c\u0003 \u0010\u0000\u016a\u016c"+
		"\u0003&\u0013\u0000\u016b\u0169\u0001\u0000\u0000\u0000\u016b\u016a\u0001"+
		"\u0000\u0000\u0000\u016c\u0174\u0001\u0000\u0000\u0000\u016d\u0173\u0005"+
		"8\u0000\u0000\u016e\u0173\u0003(\u0014\u0000\u016f\u0173\u0003*\u0015"+
		"\u0000\u0170\u0173\u0003.\u0017\u0000\u0171\u0173\u00034\u001a\u0000\u0172"+
		"\u016d\u0001\u0000\u0000\u0000\u0172\u016e\u0001\u0000\u0000\u0000\u0172"+
		"\u016f\u0001\u0000\u0000\u0000\u0172\u0170\u0001\u0000\u0000\u0000\u0172"+
		"\u0171\u0001\u0000\u0000\u0000\u0173\u0176\u0001\u0000\u0000\u0000\u0174"+
		"\u0172\u0001\u0000\u0000\u0000\u0174\u0175\u0001\u0000\u0000\u0000\u0175"+
		"\u0181\u0001\u0000\u0000\u0000\u0176\u0174\u0001\u0000\u0000\u0000\u0177"+
		"\u017d\u00058\u0000\u0000\u0178\u017d\u0003(\u0014\u0000\u0179\u017d\u0003"+
		"*\u0015\u0000\u017a\u017d\u0003.\u0017\u0000\u017b\u017d\u00034\u001a"+
		"\u0000\u017c\u0177\u0001\u0000\u0000\u0000\u017c\u0178\u0001\u0000\u0000"+
		"\u0000\u017c\u0179\u0001\u0000\u0000\u0000\u017c\u017a\u0001\u0000\u0000"+
		"\u0000\u017c\u017b\u0001\u0000\u0000\u0000\u017d\u017e\u0001\u0000\u0000"+
		"\u0000\u017e\u017c\u0001\u0000\u0000\u0000\u017e\u017f\u0001\u0000\u0000"+
		"\u0000\u017f\u0181\u0001\u0000\u0000\u0000\u0180\u016b\u0001\u0000\u0000"+
		"\u0000\u0180\u017c\u0001\u0000\u0000\u0000\u0181\u001f\u0001\u0000\u0000"+
		"\u0000\u0182\u0184\u0003\"\u0011\u0000\u0183\u0182\u0001\u0000\u0000\u0000"+
		"\u0183\u0184\u0001\u0000\u0000\u0000\u0184\u0185\u0001\u0000\u0000\u0000"+
		"\u0185\u0186\u0003$\u0012\u0000\u0186!\u0001\u0000\u0000\u0000\u0187\u018a"+
		"\u0003\b\u0004\u0000\u0188\u018a\u0005\u000f\u0000\u0000\u0189\u0187\u0001"+
		"\u0000\u0000\u0000\u0189\u0188\u0001\u0000\u0000\u0000\u0189\u018a\u0001"+
		"\u0000\u0000\u0000\u018a\u018b\u0001\u0000\u0000\u0000\u018b\u018c\u0005"+
		"\u0010\u0000\u0000\u018c#\u0001\u0000\u0000\u0000\u018d\u018e\u0005D\u0000"+
		"\u0000\u018e%\u0001\u0000\u0000\u0000\u018f\u0191\u0003\"\u0011\u0000"+
		"\u0190\u018f\u0001\u0000\u0000\u0000\u0190\u0191\u0001\u0000\u0000\u0000"+
		"\u0191\u0192\u0001\u0000\u0000\u0000\u0192\u0193\u0005\u000f\u0000\u0000"+
		"\u0193\'\u0001\u0000\u0000\u0000\u0194\u0195\u0005\u0011\u0000\u0000\u0195"+
		"\u0196\u0005D\u0000\u0000\u0196)\u0001\u0000\u0000\u0000\u0197\u0199\u0005"+
		"\u0012\u0000\u0000\u0198\u019a\u0003\u0086C\u0000\u0199\u0198\u0001\u0000"+
		"\u0000\u0000\u0199\u019a\u0001\u0000\u0000\u0000\u019a\u019c\u0001\u0000"+
		"\u0000\u0000\u019b\u019d\u0003\"\u0011\u0000\u019c\u019b\u0001\u0000\u0000"+
		"\u0000\u019c\u019d\u0001\u0000\u0000\u0000\u019d\u019e\u0001\u0000\u0000"+
		"\u0000\u019e\u01a0\u0005D\u0000\u0000\u019f\u01a1\u0003\u0086C\u0000\u01a0"+
		"\u019f\u0001\u0000\u0000\u0000\u01a0\u01a1\u0001\u0000\u0000\u0000\u01a1"+
		"\u01aa\u0001\u0000\u0000\u0000\u01a2\u01a4\u0003,\u0016\u0000\u01a3\u01a5"+
		"\u0003\u0086C\u0000\u01a4\u01a3\u0001\u0000\u0000\u0000\u01a4\u01a5\u0001"+
		"\u0000\u0000\u0000\u01a5\u01a6\u0001\u0000\u0000\u0000\u01a6\u01a8\u0007"+
		"\u0002\u0000\u0000\u01a7\u01a9\u0003\u0086C\u0000\u01a8\u01a7\u0001\u0000"+
		"\u0000\u0000\u01a8\u01a9\u0001\u0000\u0000\u0000\u01a9\u01ab\u0001\u0000"+
		"\u0000\u0000\u01aa\u01a2\u0001\u0000\u0000\u0000\u01aa\u01ab\u0001\u0000"+
		"\u0000\u0000\u01ab\u01ac\u0001\u0000\u0000\u0000\u01ac\u01ad\u0005\u0013"+
		"\u0000\u0000\u01ad+\u0001\u0000\u0000\u0000\u01ae\u01af\u0007\u0003\u0000"+
		"\u0000\u01af-\u0001\u0000\u0000\u0000\u01b0\u01b2\u0005\u0007\u0000\u0000"+
		"\u01b1\u01b3\u0005\u0007\u0000\u0000\u01b2\u01b1\u0001\u0000\u0000\u0000"+
		"\u01b2\u01b3\u0001\u0000\u0000\u0000\u01b3\u01b6\u0001\u0000\u0000\u0000"+
		"\u01b4\u01b7\u0005D\u0000\u0000\u01b5\u01b7\u00030\u0018\u0000\u01b6\u01b4"+
		"\u0001\u0000\u0000\u0000\u01b6\u01b5\u0001\u0000\u0000\u0000\u01b7/\u0001"+
		"\u0000\u0000\u0000\u01b8\u01ba\u0005E\u0000\u0000\u01b9\u01bb\u0003\u0086"+
		"C\u0000\u01ba\u01b9\u0001\u0000\u0000\u0000\u01ba\u01bb\u0001\u0000\u0000"+
		"\u0000\u01bb\u01bc\u0001\u0000\u0000\u0000\u01bc\u01bd\u00032\u0019\u0000"+
		"\u01bd\u01be\u0005\b\u0000\u0000\u01be1\u0001\u0000\u0000\u0000\u01bf"+
		"\u01c1\u0003B!\u0000\u01c0\u01c2\u0003\u0086C\u0000\u01c1\u01c0\u0001"+
		"\u0000\u0000\u0000\u01c1\u01c2\u0001\u0000\u0000\u0000\u01c2\u01c4\u0001"+
		"\u0000\u0000\u0000\u01c3\u01bf\u0001\u0000\u0000\u0000\u01c4\u01c5\u0001"+
		"\u0000\u0000\u0000\u01c5\u01c3\u0001\u0000\u0000\u0000\u01c5\u01c6\u0001"+
		"\u0000\u0000\u0000\u01c63\u0001\u0000\u0000\u0000\u01c7\u01c9\u0005\u0018"+
		"\u0000\u0000\u01c8\u01ca\u0003\u0086C\u0000\u01c9\u01c8\u0001\u0000\u0000"+
		"\u0000\u01c9\u01ca\u0001\u0000\u0000\u0000\u01ca\u01cb\u0001\u0000\u0000"+
		"\u0000\u01cb\u01cd\u00036\u001b\u0000\u01cc\u01ce\u0003\u0086C\u0000\u01cd"+
		"\u01cc\u0001\u0000\u0000\u0000\u01cd\u01ce\u0001\u0000\u0000\u0000\u01ce"+
		"\u01cf\u0001\u0000\u0000\u0000\u01cf\u01d0\u0005\b\u0000\u0000\u01d05"+
		"\u0001\u0000\u0000\u0000\u01d1\u01d8\u0003 \u0010\u0000\u01d2\u01d8\u0003"+
		"&\u0013\u0000\u01d3\u01d8\u00058\u0000\u0000\u01d4\u01d8\u0003(\u0014"+
		"\u0000\u01d5\u01d8\u0003*\u0015\u0000\u01d6\u01d8\u0003.\u0017\u0000\u01d7"+
		"\u01d1\u0001\u0000\u0000\u0000\u01d7\u01d2\u0001\u0000\u0000\u0000\u01d7"+
		"\u01d3\u0001\u0000\u0000\u0000\u01d7\u01d4\u0001\u0000\u0000\u0000\u01d7"+
		"\u01d5\u0001\u0000\u0000\u0000\u01d7\u01d6\u0001\u0000\u0000\u0000\u01d8"+
		"7\u0001\u0000\u0000\u0000\u01d9\u01da\u0003:\u001d\u0000\u01da\u01dc\u0005"+
		"\u0002\u0000\u0000\u01db\u01dd\u0003\u0086C\u0000\u01dc\u01db\u0001\u0000"+
		"\u0000\u0000\u01dc\u01dd\u0001\u0000\u0000\u0000\u01dd\u01df\u0001\u0000"+
		"\u0000\u0000\u01de\u01d9\u0001\u0000\u0000\u0000\u01df\u01e2\u0001\u0000"+
		"\u0000\u0000\u01e0\u01de\u0001\u0000\u0000\u0000\u01e0\u01e1\u0001\u0000"+
		"\u0000\u0000\u01e1\u01e4\u0001\u0000\u0000\u0000\u01e2\u01e0\u0001\u0000"+
		"\u0000\u0000\u01e3\u01e5\u0003:\u001d\u0000\u01e4\u01e3\u0001\u0000\u0000"+
		"\u0000\u01e4\u01e5\u0001\u0000\u0000\u0000\u01e59\u0001\u0000\u0000\u0000"+
		"\u01e6\u01e8\u0003<\u001e\u0000\u01e7\u01e9\u0003\u0086C\u0000\u01e8\u01e7"+
		"\u0001\u0000\u0000\u0000\u01e8\u01e9\u0001\u0000\u0000\u0000\u01e9\u01ea"+
		"\u0001\u0000\u0000\u0000\u01ea\u01ec\u0005\u0007\u0000\u0000\u01eb\u01ed"+
		"\u0003\u0086C\u0000\u01ec\u01eb\u0001\u0000\u0000\u0000\u01ec\u01ed\u0001"+
		"\u0000\u0000\u0000\u01ed\u01ee\u0001\u0000\u0000\u0000\u01ee\u01f0\u0003"+
		"@ \u0000\u01ef\u01f1\u0003>\u001f\u0000\u01f0\u01ef\u0001\u0000\u0000"+
		"\u0000\u01f0\u01f1\u0001\u0000\u0000\u0000\u01f1;\u0001\u0000\u0000\u0000"+
		"\u01f2\u01f3\u0005D\u0000\u0000\u01f3=\u0001\u0000\u0000\u0000\u01f4\u01f6"+
		"\u0003\u0086C\u0000\u01f5\u01f4\u0001\u0000\u0000\u0000\u01f5\u01f6\u0001"+
		"\u0000\u0000\u0000\u01f6\u01f7\u0001\u0000\u0000\u0000\u01f7\u01f9\u0005"+
		"\u0019\u0000\u0000\u01f8\u01fa\u0003\u0086C\u0000\u01f9\u01f8\u0001\u0000"+
		"\u0000\u0000\u01f9\u01fa\u0001\u0000\u0000\u0000\u01fa\u01fb\u0001\u0000"+
		"\u0000\u0000\u01fb\u01fc\u0005\u001a\u0000\u0000\u01fc?\u0001\u0000\u0000"+
		"\u0000\u01fd\u0204\u0003B!\u0000\u01fe\u0200\u0003D\"\u0000\u01ff\u01fe"+
		"\u0001\u0000\u0000\u0000\u01ff\u0200\u0001\u0000\u0000\u0000\u0200\u0201"+
		"\u0001\u0000\u0000\u0000\u0201\u0203\u0003B!\u0000\u0202\u01ff\u0001\u0000"+
		"\u0000\u0000\u0203\u0206\u0001\u0000\u0000\u0000\u0204\u0202\u0001\u0000"+
		"\u0000\u0000\u0204\u0205\u0001\u0000\u0000\u0000\u0205A\u0001\u0000\u0000"+
		"\u0000\u0206\u0204\u0001\u0000\u0000\u0000\u0207\u0209\u0003F#\u0000\u0208"+
		"\u0207\u0001\u0000\u0000\u0000\u0208\u0209\u0001\u0000\u0000\u0000\u0209"+
		"\u020a\u0001\u0000\u0000\u0000\u020a\u021c\u0007\u0004\u0000\u0000\u020b"+
		"\u021c\u00059\u0000\u0000\u020c\u021c\u0005D\u0000\u0000\u020d\u021c\u0003"+
		"H$\u0000\u020e\u021c\u00058\u0000\u0000\u020f\u021c\u0005C\u0000\u0000"+
		"\u0210\u021c\u0005/\u0000\u0000\u0211\u021c\u00050\u0000\u0000\u0212\u0213"+
		"\u0005\u0007\u0000\u0000\u0213\u021c\u0005D\u0000\u0000\u0214\u0216\u0005"+
		"E\u0000\u0000\u0215\u0217\u0003\u0086C\u0000\u0216\u0215\u0001\u0000\u0000"+
		"\u0000\u0216\u0217\u0001\u0000\u0000\u0000\u0217\u0218\u0001\u0000\u0000"+
		"\u0000\u0218\u0219\u0003@ \u0000\u0219\u021a\u0005\b\u0000\u0000\u021a"+
		"\u021c\u0001\u0000\u0000\u0000\u021b\u0208\u0001\u0000\u0000\u0000\u021b"+
		"\u020b\u0001\u0000\u0000\u0000\u021b\u020c\u0001\u0000\u0000\u0000\u021b"+
		"\u020d\u0001\u0000\u0000\u0000\u021b\u020e\u0001\u0000\u0000\u0000\u021b"+
		"\u020f\u0001\u0000\u0000\u0000\u021b\u0210\u0001\u0000\u0000\u0000\u021b"+
		"\u0211\u0001\u0000\u0000\u0000\u021b\u0212\u0001\u0000\u0000\u0000\u021b"+
		"\u0214\u0001\u0000\u0000\u0000\u021cC\u0001\u0000\u0000\u0000\u021d\u0221"+
		"\u0005\u001b\u0000\u0000\u021e\u0221\u0005\u0005\u0000\u0000\u021f\u0221"+
		"\u0003\u0086C\u0000\u0220\u021d\u0001\u0000\u0000\u0000\u0220\u021e\u0001"+
		"\u0000\u0000\u0000\u0220\u021f\u0001\u0000\u0000\u0000\u0221E\u0001\u0000"+
		"\u0000\u0000\u0222\u0223\u0007\u0005\u0000\u0000\u0223G\u0001\u0000\u0000"+
		"\u0000\u0224\u0226\u0005\u001d\u0000\u0000\u0225\u0227\u0003\u0086C\u0000"+
		"\u0226\u0225\u0001\u0000\u0000\u0000\u0226\u0227\u0001\u0000\u0000\u0000"+
		"\u0227\u0228\u0001\u0000\u0000\u0000\u0228\u022a\u0007\u0006\u0000\u0000"+
		"\u0229\u022b\u0003\u0086C\u0000\u022a\u0229\u0001\u0000\u0000\u0000\u022a"+
		"\u022b\u0001\u0000\u0000\u0000\u022b\u022c\u0001\u0000\u0000\u0000\u022c"+
		"\u022d\u0005\b\u0000\u0000\u022dI\u0001\u0000\u0000\u0000\u022e\u022f"+
		"\u0005\u001e\u0000\u0000\u022f\u0230\u0003\n\u0005\u0000\u0230\u0232\u0005"+
		"\t\u0000\u0000\u0231\u0233\u0003\u0086C\u0000\u0232\u0231\u0001\u0000"+
		"\u0000\u0000\u0232\u0233\u0001\u0000\u0000\u0000\u0233\u0234\u0001\u0000"+
		"\u0000\u0000\u0234\u0235\u0003|>\u0000\u0235\u0237\u0005\n\u0000\u0000"+
		"\u0236\u0238\u0003\u0086C\u0000\u0237\u0236\u0001\u0000\u0000\u0000\u0237"+
		"\u0238\u0001\u0000\u0000\u0000\u0238K\u0001\u0000\u0000\u0000\u0239\u023b"+
		"\u0005\u001f\u0000\u0000\u023a\u023c\u0003\u0086C\u0000\u023b\u023a\u0001"+
		"\u0000\u0000\u0000\u023b\u023c\u0001\u0000\u0000\u0000\u023c\u023e\u0001"+
		"\u0000\u0000\u0000\u023d\u023f\u0003N\'\u0000\u023e\u023d\u0001\u0000"+
		"\u0000\u0000\u023e\u023f\u0001\u0000\u0000\u0000\u023f\u0240\u0001\u0000"+
		"\u0000\u0000\u0240\u0242\u0005\t\u0000\u0000\u0241\u0243\u0003\u0086C"+
		"\u0000\u0242\u0241\u0001\u0000\u0000\u0000\u0242\u0243\u0001\u0000\u0000"+
		"\u0000\u0243\u0245\u0001\u0000\u0000\u0000\u0244\u0246\u00038\u001c\u0000"+
		"\u0245\u0244\u0001\u0000\u0000\u0000\u0245\u0246\u0001\u0000\u0000\u0000"+
		"\u0246\u0247\u0001\u0000\u0000\u0000\u0247\u0249\u0005\n\u0000\u0000\u0248"+
		"\u024a\u0003\u0086C\u0000\u0249\u0248\u0001\u0000\u0000\u0000\u0249\u024a"+
		"\u0001\u0000\u0000\u0000\u024aM\u0001\u0000\u0000\u0000\u024b\u024c\u0005"+
		"\u0007\u0000\u0000\u024c\u024d\u0005D\u0000\u0000\u024dO\u0001\u0000\u0000"+
		"\u0000\u024e\u0250\u0005 \u0000\u0000\u024f\u0251\u0003\u0086C\u0000\u0250"+
		"\u024f\u0001\u0000\u0000\u0000\u0250\u0251\u0001\u0000\u0000\u0000\u0251"+
		"\u0252\u0001\u0000\u0000\u0000\u0252\u0254\u0005\t\u0000\u0000\u0253\u0255"+
		"\u0003\u0086C\u0000\u0254\u0253\u0001\u0000\u0000\u0000\u0254\u0255\u0001"+
		"\u0000\u0000\u0000\u0255\u0257\u0001\u0000\u0000\u0000\u0256\u0258\u0003"+
		"8\u001c\u0000\u0257\u0256\u0001\u0000\u0000\u0000\u0257\u0258\u0001\u0000"+
		"\u0000\u0000\u0258\u0259\u0001\u0000\u0000\u0000\u0259\u025b\u0005\n\u0000"+
		"\u0000\u025a\u025c\u0003\u0086C\u0000\u025b\u025a\u0001\u0000\u0000\u0000"+
		"\u025b\u025c\u0001\u0000\u0000\u0000\u025cQ\u0001\u0000\u0000\u0000\u025d"+
		"\u025e\u0005!\u0000\u0000\u025e\u0260\u00053\u0000\u0000\u025f\u0261\u0003"+
		"\u0086C\u0000\u0260\u025f\u0001\u0000\u0000\u0000\u0260\u0261\u0001\u0000"+
		"\u0000\u0000\u0261\u0262\u0001\u0000\u0000\u0000\u0262\u0264\u0005D\u0000"+
		"\u0000\u0263\u0265\u0003\u0086C\u0000\u0264\u0263\u0001\u0000\u0000\u0000"+
		"\u0264\u0265\u0001\u0000\u0000\u0000\u0265\u0266\u0001\u0000\u0000\u0000"+
		"\u0266\u0268\u0005\t\u0000\u0000\u0267\u0269\u0003\u0086C\u0000\u0268"+
		"\u0267\u0001\u0000\u0000\u0000\u0268\u0269\u0001\u0000\u0000\u0000\u0269"+
		"\u026a\u0001\u0000\u0000\u0000\u026a\u026b\u0003T*\u0000\u026b\u026d\u0005"+
		"\n\u0000\u0000\u026c\u026e\u0003\u0086C\u0000\u026d\u026c\u0001\u0000"+
		"\u0000\u0000\u026d\u026e\u0001\u0000\u0000\u0000\u026eS\u0001\u0000\u0000"+
		"\u0000\u026f\u0271\u0003V+\u0000\u0270\u0272\u0003\u0086C\u0000\u0271"+
		"\u0270\u0001\u0000\u0000\u0000\u0271\u0272\u0001\u0000\u0000\u0000\u0272"+
		"\u0273\u0001\u0000\u0000\u0000\u0273\u0275\u0005\t\u0000\u0000\u0274\u0276"+
		"\u0003\u0086C\u0000\u0275\u0274\u0001\u0000\u0000\u0000\u0275\u0276\u0001"+
		"\u0000\u0000\u0000\u0276\u0278\u0001\u0000\u0000\u0000\u0277\u0279\u0003"+
		"8\u001c\u0000\u0278\u0277\u0001\u0000\u0000\u0000\u0278\u0279\u0001\u0000"+
		"\u0000\u0000\u0279\u027a\u0001\u0000\u0000\u0000\u027a\u027c\u0005\n\u0000"+
		"\u0000\u027b\u027d\u0003\u0086C\u0000\u027c\u027b\u0001\u0000\u0000\u0000"+
		"\u027c\u027d\u0001\u0000\u0000\u0000\u027d\u028f\u0001\u0000\u0000\u0000"+
		"\u027e\u0280\u0003V+\u0000\u027f\u0281\u0003\u0086C\u0000\u0280\u027f"+
		"\u0001\u0000\u0000\u0000\u0280\u0281\u0001\u0000\u0000\u0000\u0281\u0282"+
		"\u0001\u0000\u0000\u0000\u0282\u0284\u0005\t\u0000\u0000\u0283\u0285\u0003"+
		"\u0086C\u0000\u0284\u0283\u0001\u0000\u0000\u0000\u0284\u0285\u0001\u0000"+
		"\u0000\u0000\u0285\u0287\u0001\u0000\u0000\u0000\u0286\u0288\u00038\u001c"+
		"\u0000\u0287\u0286\u0001\u0000\u0000\u0000\u0287\u0288\u0001\u0000\u0000"+
		"\u0000\u0288\u0289\u0001\u0000\u0000\u0000\u0289\u028b\u0005\n\u0000\u0000"+
		"\u028a\u028c\u0003\u0086C\u0000\u028b\u028a\u0001\u0000\u0000\u0000\u028b"+
		"\u028c\u0001\u0000\u0000\u0000\u028c\u028e\u0001\u0000\u0000\u0000\u028d"+
		"\u027e\u0001\u0000\u0000\u0000\u028e\u0291\u0001\u0000\u0000\u0000\u028f"+
		"\u028d\u0001\u0000\u0000\u0000\u028f\u0290\u0001\u0000\u0000\u0000\u0290"+
		"U\u0001\u0000\u0000\u0000\u0291\u028f\u0001\u0000\u0000\u0000\u0292\u029d"+
		"\u0007\u0007\u0000\u0000\u0293\u0295\u0003\u0086C\u0000\u0294\u0293\u0001"+
		"\u0000\u0000\u0000\u0294\u0295\u0001\u0000\u0000\u0000\u0295\u0296\u0001"+
		"\u0000\u0000\u0000\u0296\u0298\u0005\u0005\u0000\u0000\u0297\u0299\u0003"+
		"\u0086C\u0000\u0298\u0297\u0001\u0000\u0000\u0000\u0298\u0299\u0001\u0000"+
		"\u0000\u0000\u0299\u029a\u0001\u0000\u0000\u0000\u029a\u029c\u0007\u0007"+
		"\u0000\u0000\u029b\u0294\u0001\u0000\u0000\u0000\u029c\u029f\u0001\u0000"+
		"\u0000\u0000\u029d\u029b\u0001\u0000\u0000\u0000\u029d\u029e\u0001\u0000"+
		"\u0000\u0000\u029eW\u0001\u0000\u0000\u0000\u029f\u029d\u0001\u0000\u0000"+
		"\u0000\u02a0\u02a2\u0005\"\u0000\u0000\u02a1\u02a3\u0003\u0086C\u0000"+
		"\u02a2\u02a1\u0001\u0000\u0000\u0000\u02a2\u02a3\u0001\u0000\u0000\u0000"+
		"\u02a3\u02a4\u0001\u0000\u0000\u0000\u02a4\u02a6\u0003Z-\u0000\u02a5\u02a7"+
		"\u0003\u0086C\u0000\u02a6\u02a5\u0001\u0000\u0000\u0000\u02a6\u02a7\u0001"+
		"\u0000\u0000\u0000\u02a7\u02a8\u0001\u0000\u0000\u0000\u02a8\u02aa\u0005"+
		"\t\u0000\u0000\u02a9\u02ab\u0003\u0086C\u0000\u02aa\u02a9\u0001\u0000"+
		"\u0000\u0000\u02aa\u02ab\u0001\u0000\u0000\u0000\u02ab\u02ac\u0001\u0000"+
		"\u0000\u0000\u02ac\u02ad\u0003|>\u0000\u02ad\u02af\u0005\n\u0000\u0000"+
		"\u02ae\u02b0\u0003\u0086C\u0000\u02af\u02ae\u0001\u0000\u0000\u0000\u02af"+
		"\u02b0\u0001\u0000\u0000\u0000\u02b0Y\u0001\u0000\u0000\u0000\u02b1\u02b6"+
		"\u0003^/\u0000\u02b2\u02b6\u0003`0\u0000\u02b3\u02b6\u0003b1\u0000\u02b4"+
		"\u02b6\u0003\\.\u0000\u02b5\u02b1\u0001\u0000\u0000\u0000\u02b5\u02b2"+
		"\u0001\u0000\u0000\u0000\u02b5\u02b3\u0001\u0000\u0000\u0000\u02b5\u02b4"+
		"\u0001\u0000\u0000\u0000\u02b6[\u0001\u0000\u0000\u0000\u02b7\u02b9\u0005"+
		"\u0006\u0000\u0000\u02b8\u02ba\u0003\u0086C\u0000\u02b9\u02b8\u0001\u0000"+
		"\u0000\u0000\u02b9\u02ba\u0001\u0000\u0000\u0000\u02ba\u02bb\u0001\u0000"+
		"\u0000\u0000\u02bb\u02bd\u0003Z-\u0000\u02bc\u02be\u0003\u0086C\u0000"+
		"\u02bd\u02bc\u0001\u0000\u0000\u0000\u02bd\u02be\u0001\u0000\u0000\u0000"+
		"\u02be\u02bf\u0001\u0000\u0000\u0000\u02bf\u02c0\u0005\b\u0000\u0000\u02c0"+
		"\u02c4\u0001\u0000\u0000\u0000\u02c1\u02c4\u0003d2\u0000\u02c2\u02c4\u0003"+
		"f3\u0000\u02c3\u02b7\u0001\u0000\u0000\u0000\u02c3\u02c1\u0001\u0000\u0000"+
		"\u0000\u02c3\u02c2\u0001\u0000\u0000\u0000\u02c4]\u0001\u0000\u0000\u0000"+
		"\u02c5\u02c6\u00055\u0000\u0000\u02c6\u02c7\u0003\u0086C\u0000\u02c7\u02c8"+
		"\u0003\\.\u0000\u02c8_\u0001\u0000\u0000\u0000\u02c9\u02cf\u0003\\.\u0000"+
		"\u02ca\u02cb\u0003\u0086C\u0000\u02cb\u02cc\u00056\u0000\u0000\u02cc\u02cd"+
		"\u0003\u0086C\u0000\u02cd\u02ce\u0003\\.\u0000\u02ce\u02d0\u0001\u0000"+
		"\u0000\u0000\u02cf\u02ca\u0001\u0000\u0000\u0000\u02d0\u02d1\u0001\u0000"+
		"\u0000\u0000\u02d1\u02cf\u0001\u0000\u0000\u0000\u02d1\u02d2\u0001\u0000"+
		"\u0000\u0000\u02d2a\u0001\u0000\u0000\u0000\u02d3\u02d9\u0003\\.\u0000"+
		"\u02d4\u02d5\u0003\u0086C\u0000\u02d5\u02d6\u00057\u0000\u0000\u02d6\u02d7"+
		"\u0003\u0086C\u0000\u02d7\u02d8\u0003\\.\u0000\u02d8\u02da\u0001\u0000"+
		"\u0000\u0000\u02d9\u02d4\u0001\u0000\u0000\u0000\u02da\u02db\u0001\u0000"+
		"\u0000\u0000\u02db\u02d9\u0001\u0000\u0000\u0000\u02db\u02dc\u0001\u0000"+
		"\u0000\u0000\u02dcc\u0001\u0000\u0000\u0000\u02dd\u02df\u0005\u0006\u0000"+
		"\u0000\u02de\u02e0\u0003\u0086C\u0000\u02df\u02de\u0001\u0000\u0000\u0000"+
		"\u02df\u02e0\u0001\u0000\u0000\u0000\u02e0\u02e1\u0001\u0000\u0000\u0000"+
		"\u02e1\u02e3\u0003:\u001d\u0000\u02e2\u02e4\u0003\u0086C\u0000\u02e3\u02e2"+
		"\u0001\u0000\u0000\u0000\u02e3\u02e4\u0001\u0000\u0000\u0000\u02e4\u02e5"+
		"\u0001\u0000\u0000\u0000\u02e5\u02e6\u0005\b\u0000\u0000\u02e6e\u0001"+
		"\u0000\u0000\u0000\u02e7\u02ec\u0005E\u0000\u0000\u02e8\u02eb\u0003h4"+
		"\u0000\u02e9\u02eb\u0003f3\u0000\u02ea\u02e8\u0001\u0000\u0000\u0000\u02ea"+
		"\u02e9\u0001\u0000\u0000\u0000\u02eb\u02ee\u0001\u0000\u0000\u0000\u02ec"+
		"\u02ea\u0001\u0000\u0000\u0000\u02ec\u02ed\u0001\u0000\u0000\u0000\u02ed"+
		"\u02ef\u0001\u0000\u0000\u0000\u02ee\u02ec\u0001\u0000\u0000\u0000\u02ef"+
		"\u02fa\u0005\b\u0000\u0000\u02f0\u02f5\u0005\u0006\u0000\u0000\u02f1\u02f4"+
		"\u0003h4\u0000\u02f2\u02f4\u0003f3\u0000\u02f3\u02f1\u0001\u0000\u0000"+
		"\u0000\u02f3\u02f2\u0001\u0000\u0000\u0000\u02f4\u02f7\u0001\u0000\u0000"+
		"\u0000\u02f5\u02f3\u0001\u0000\u0000\u0000\u02f5\u02f6\u0001\u0000\u0000"+
		"\u0000\u02f6\u02f8\u0001\u0000\u0000\u0000\u02f7\u02f5\u0001\u0000\u0000"+
		"\u0000\u02f8\u02fa\u0005\b\u0000\u0000\u02f9\u02e7\u0001\u0000\u0000\u0000"+
		"\u02f9\u02f0\u0001\u0000\u0000\u0000\u02fag\u0001\u0000\u0000\u0000\u02fb"+
		"\u031f\u0005D\u0000\u0000\u02fc\u031f\u0005:\u0000\u0000\u02fd\u031f\u0005"+
		";\u0000\u0000\u02fe\u031f\u0005<\u0000\u0000\u02ff\u031f\u0005=\u0000"+
		"\u0000\u0300\u031f\u0005>\u0000\u0000\u0301\u031f\u0005?\u0000\u0000\u0302"+
		"\u031f\u0005@\u0000\u0000\u0303\u031f\u0005A\u0000\u0000\u0304\u031f\u0005"+
		"B\u0000\u0000\u0305\u031f\u00059\u0000\u0000\u0306\u031f\u0005F\u0000"+
		"\u0000\u0307\u031f\u00058\u0000\u0000\u0308\u031f\u0005C\u0000\u0000\u0309"+
		"\u031f\u0005/\u0000\u0000\u030a\u031f\u00050\u0000\u0000\u030b\u031f\u0005"+
		"\u0007\u0000\u0000\u030c\u031f\u0005E\u0000\u0000\u030d\u0311\u0005\u0006"+
		"\u0000\u0000\u030e\u0310\u0003h4\u0000\u030f\u030e\u0001\u0000\u0000\u0000"+
		"\u0310\u0313\u0001\u0000\u0000\u0000\u0311\u030f\u0001\u0000\u0000\u0000"+
		"\u0311\u0312\u0001\u0000\u0000\u0000\u0312\u0314\u0001\u0000\u0000\u0000"+
		"\u0313\u0311\u0001\u0000\u0000\u0000\u0314\u031f\u0005\b\u0000\u0000\u0315"+
		"\u0319\u0005\u0012\u0000\u0000\u0316\u0318\u0003h4\u0000\u0317\u0316\u0001"+
		"\u0000\u0000\u0000\u0318\u031b\u0001\u0000\u0000\u0000\u0319\u0317\u0001"+
		"\u0000\u0000\u0000\u0319\u031a\u0001\u0000\u0000\u0000\u031a\u031c\u0001"+
		"\u0000\u0000\u0000\u031b\u0319\u0001\u0000\u0000\u0000\u031c\u031f\u0005"+
		"\u0013\u0000\u0000\u031d\u031f\u0003\u0086C\u0000\u031e\u02fb\u0001\u0000"+
		"\u0000\u0000\u031e\u02fc\u0001\u0000\u0000\u0000\u031e\u02fd\u0001\u0000"+
		"\u0000\u0000\u031e\u02fe\u0001\u0000\u0000\u0000\u031e\u02ff\u0001\u0000"+
		"\u0000\u0000\u031e\u0300\u0001\u0000\u0000\u0000\u031e\u0301\u0001\u0000"+
		"\u0000\u0000\u031e\u0302\u0001\u0000\u0000\u0000\u031e\u0303\u0001\u0000"+
		"\u0000\u0000\u031e\u0304\u0001\u0000\u0000\u0000\u031e\u0305\u0001\u0000"+
		"\u0000\u0000\u031e\u0306\u0001\u0000\u0000\u0000\u031e\u0307\u0001\u0000"+
		"\u0000\u0000\u031e\u0308\u0001\u0000\u0000\u0000\u031e\u0309\u0001\u0000"+
		"\u0000\u0000\u031e\u030a\u0001\u0000\u0000\u0000\u031e\u030b\u0001\u0000"+
		"\u0000\u0000\u031e\u030c\u0001\u0000\u0000\u0000\u031e\u030d\u0001\u0000"+
		"\u0000\u0000\u031e\u0315\u0001\u0000\u0000\u0000\u031e\u031d\u0001\u0000"+
		"\u0000\u0000\u031fi\u0001\u0000\u0000\u0000\u0320\u0322\u0005#\u0000\u0000"+
		"\u0321\u0323\u0003\u0086C\u0000\u0322\u0321\u0001\u0000\u0000\u0000\u0322"+
		"\u0323\u0001\u0000\u0000\u0000\u0323\u0324\u0001\u0000\u0000\u0000\u0324"+
		"\u0326\u0005\t\u0000\u0000\u0325\u0327\u0003\u0086C\u0000\u0326\u0325"+
		"\u0001\u0000\u0000\u0000\u0326\u0327\u0001\u0000\u0000\u0000\u0327\u0329"+
		"\u0001\u0000\u0000\u0000\u0328\u032a\u00038\u001c\u0000\u0329\u0328\u0001"+
		"\u0000\u0000\u0000\u0329\u032a\u0001\u0000\u0000\u0000\u032a\u032b\u0001"+
		"\u0000\u0000\u0000\u032b\u032d\u0005\n\u0000\u0000\u032c\u032e\u0003\u0086"+
		"C\u0000\u032d\u032c\u0001\u0000\u0000\u0000\u032d\u032e\u0001\u0000\u0000"+
		"\u0000\u032ek\u0001\u0000\u0000\u0000\u032f\u0331\u0005$\u0000\u0000\u0330"+
		"\u0332\u0003\u0086C\u0000\u0331\u0330\u0001\u0000\u0000\u0000\u0331\u0332"+
		"\u0001\u0000\u0000\u0000\u0332\u0333\u0001\u0000\u0000\u0000\u0333\u0335"+
		"\u0005D\u0000\u0000\u0334\u0336\u0003\u0086C\u0000\u0335\u0334\u0001\u0000"+
		"\u0000\u0000\u0335\u0336\u0001\u0000\u0000\u0000\u0336\u0337\u0001\u0000"+
		"\u0000\u0000\u0337\u0339\u0005\t\u0000\u0000\u0338\u033a\u0003\u0086C"+
		"\u0000\u0339\u0338\u0001\u0000\u0000\u0000\u0339\u033a\u0001\u0000\u0000"+
		"\u0000\u033a\u033c\u0001\u0000\u0000\u0000\u033b\u033d\u00038\u001c\u0000"+
		"\u033c\u033b\u0001\u0000\u0000\u0000\u033c\u033d\u0001\u0000\u0000\u0000"+
		"\u033d\u033e\u0001\u0000\u0000\u0000\u033e\u0340\u0005\n\u0000\u0000\u033f"+
		"\u0341\u0003\u0086C\u0000\u0340\u033f\u0001\u0000\u0000\u0000\u0340\u0341"+
		"\u0001\u0000\u0000\u0000\u0341m\u0001\u0000\u0000\u0000\u0342\u0344\u0005"+
		"%\u0000\u0000\u0343\u0345\u0003\u0086C\u0000\u0344\u0343\u0001\u0000\u0000"+
		"\u0000\u0344\u0345\u0001\u0000\u0000\u0000\u0345\u0346\u0001\u0000\u0000"+
		"\u0000\u0346\u0348\u0003p8\u0000\u0347\u0349\u0003\u0086C\u0000\u0348"+
		"\u0347\u0001\u0000\u0000\u0000\u0348\u0349\u0001\u0000\u0000\u0000\u0349"+
		"\u034a\u0001\u0000\u0000\u0000\u034a\u034c\u0005\t\u0000\u0000\u034b\u034d"+
		"\u0003\u0086C\u0000\u034c\u034b\u0001\u0000\u0000\u0000\u034c\u034d\u0001"+
		"\u0000\u0000\u0000\u034d\u0351\u0001\u0000\u0000\u0000\u034e\u0350\u0003"+
		"t:\u0000\u034f\u034e\u0001\u0000\u0000\u0000\u0350\u0353\u0001\u0000\u0000"+
		"\u0000\u0351\u034f\u0001\u0000\u0000\u0000\u0351\u0352\u0001\u0000\u0000"+
		"\u0000\u0352\u0354\u0001\u0000\u0000\u0000\u0353\u0351\u0001\u0000\u0000"+
		"\u0000\u0354\u0356\u0005\n\u0000\u0000\u0355\u0357\u0003\u0086C\u0000"+
		"\u0356\u0355\u0001\u0000\u0000\u0000\u0356\u0357\u0001\u0000\u0000\u0000"+
		"\u0357o\u0001\u0000\u0000\u0000\u0358\u0363\u0003r9\u0000\u0359\u035b"+
		"\u0003\u0086C\u0000\u035a\u0359\u0001\u0000\u0000\u0000\u035a\u035b\u0001"+
		"\u0000\u0000\u0000\u035b\u035c\u0001\u0000\u0000\u0000\u035c\u035e\u0005"+
		"\u0005\u0000\u0000\u035d\u035f\u0003\u0086C\u0000\u035e\u035d\u0001\u0000"+
		"\u0000\u0000\u035e\u035f\u0001\u0000\u0000\u0000\u035f\u0360\u0001\u0000"+
		"\u0000\u0000\u0360\u0362\u0003r9\u0000\u0361\u035a\u0001\u0000\u0000\u0000"+
		"\u0362\u0365\u0001\u0000\u0000\u0000\u0363\u0361\u0001\u0000\u0000\u0000"+
		"\u0363\u0364\u0001\u0000\u0000\u0000\u0364q\u0001\u0000\u0000\u0000\u0365"+
		"\u0363\u0001\u0000\u0000\u0000\u0366\u0370\u00059\u0000\u0000\u0367\u0369"+
		"\u0005D\u0000\u0000\u0368\u036a\u0003\u0086C\u0000\u0369\u0368\u0001\u0000"+
		"\u0000\u0000\u0369\u036a\u0001\u0000\u0000\u0000\u036a\u036c\u0001\u0000"+
		"\u0000\u0000\u036b\u0367\u0001\u0000\u0000\u0000\u036c\u036d\u0001\u0000"+
		"\u0000\u0000\u036d\u036b\u0001\u0000\u0000\u0000\u036d\u036e\u0001\u0000"+
		"\u0000\u0000\u036e\u0370\u0001\u0000\u0000\u0000\u036f\u0366\u0001\u0000"+
		"\u0000\u0000\u036f\u036b\u0001\u0000\u0000\u0000\u0370s\u0001\u0000\u0000"+
		"\u0000\u0371\u0373\u0003v;\u0000\u0372\u0374\u0003\u0086C\u0000\u0373"+
		"\u0372\u0001\u0000\u0000\u0000\u0373\u0374\u0001\u0000\u0000\u0000\u0374"+
		"\u0375\u0001\u0000\u0000\u0000\u0375\u0377\u0005\t\u0000\u0000\u0376\u0378"+
		"\u0003\u0086C\u0000\u0377\u0376\u0001\u0000\u0000\u0000\u0377\u0378\u0001"+
		"\u0000\u0000\u0000\u0378\u037a\u0001\u0000\u0000\u0000\u0379\u037b\u0003"+
		"x<\u0000\u037a\u0379\u0001\u0000\u0000\u0000\u037a\u037b\u0001\u0000\u0000"+
		"\u0000\u037b\u037c\u0001\u0000\u0000\u0000\u037c\u037e\u0005\n\u0000\u0000"+
		"\u037d\u037f\u0003\u0086C\u0000\u037e\u037d\u0001\u0000\u0000\u0000\u037e"+
		"\u037f\u0001\u0000\u0000\u0000\u037fu\u0001\u0000\u0000\u0000\u0380\u0381"+
		"\u0007\b\u0000\u0000\u0381w\u0001\u0000\u0000\u0000\u0382\u038d\u0003"+
		"z=\u0000\u0383\u0385\u0003\u0086C\u0000\u0384\u0383\u0001\u0000\u0000"+
		"\u0000\u0384\u0385\u0001\u0000\u0000\u0000\u0385\u0386\u0001\u0000\u0000"+
		"\u0000\u0386\u0388\u0005\u0002\u0000\u0000\u0387\u0389\u0003\u0086C\u0000"+
		"\u0388\u0387\u0001\u0000\u0000\u0000\u0388\u0389\u0001\u0000\u0000\u0000"+
		"\u0389\u038a\u0001\u0000\u0000\u0000\u038a\u038c\u0003z=\u0000\u038b\u0384"+
		"\u0001\u0000\u0000\u0000\u038c\u038f\u0001\u0000\u0000\u0000\u038d\u038b"+
		"\u0001\u0000\u0000\u0000\u038d\u038e\u0001\u0000\u0000\u0000\u038e\u0391"+
		"\u0001\u0000\u0000\u0000\u038f\u038d\u0001\u0000\u0000\u0000\u0390\u0392"+
		"\u0005\u0002\u0000\u0000\u0391\u0390\u0001\u0000\u0000\u0000\u0391\u0392"+
		"\u0001\u0000\u0000\u0000\u0392y\u0001\u0000\u0000\u0000\u0393\u0395\u0005"+
		"D\u0000\u0000\u0394\u0396\u0003\u0086C\u0000\u0395\u0394\u0001\u0000\u0000"+
		"\u0000\u0395\u0396\u0001\u0000\u0000\u0000\u0396\u0397\u0001\u0000\u0000"+
		"\u0000\u0397\u0399\u0005\u0007\u0000\u0000\u0398\u039a\u0003\u0086C\u0000"+
		"\u0399\u0398\u0001\u0000\u0000\u0000\u0399\u039a\u0001\u0000\u0000\u0000"+
		"\u039a\u039b\u0001\u0000\u0000\u0000\u039b\u03a2\u0005:\u0000\u0000\u039c"+
		"\u039e\u0003\u0086C\u0000\u039d\u039c\u0001\u0000\u0000\u0000\u039d\u039e"+
		"\u0001\u0000\u0000\u0000\u039e\u039f\u0001\u0000\u0000\u0000\u039f\u03a1"+
		"\u0005:\u0000\u0000\u03a0\u039d\u0001\u0000\u0000\u0000\u03a1\u03a4\u0001"+
		"\u0000\u0000\u0000\u03a2\u03a0\u0001\u0000\u0000\u0000\u03a2\u03a3\u0001"+
		"\u0000\u0000\u0000\u03a3{\u0001\u0000\u0000\u0000\u03a4\u03a2\u0001\u0000"+
		"\u0000\u0000\u03a5\u03b0\u0003\u0016\u000b\u0000\u03a6\u03b0\u0003J%\u0000"+
		"\u03a7\u03b0\u0003L&\u0000\u03a8\u03b0\u0003P(\u0000\u03a9\u03b0\u0003"+
		"R)\u0000\u03aa\u03b0\u0003X,\u0000\u03ab\u03b0\u0003j5\u0000\u03ac\u03b0"+
		"\u0003l6\u0000\u03ad\u03b0\u0003n7\u0000\u03ae\u03b0\u0003~?\u0000\u03af"+
		"\u03a5\u0001\u0000\u0000\u0000\u03af\u03a6\u0001\u0000\u0000\u0000\u03af"+
		"\u03a7\u0001\u0000\u0000\u0000\u03af\u03a8\u0001\u0000\u0000\u0000\u03af"+
		"\u03a9\u0001\u0000\u0000\u0000\u03af\u03aa\u0001\u0000\u0000\u0000\u03af"+
		"\u03ab\u0001\u0000\u0000\u0000\u03af\u03ac\u0001\u0000\u0000\u0000\u03af"+
		"\u03ad\u0001\u0000\u0000\u0000\u03af\u03ae\u0001\u0000\u0000\u0000\u03b0"+
		"\u03b3\u0001\u0000\u0000\u0000\u03b1\u03af\u0001\u0000\u0000\u0000\u03b1"+
		"\u03b2\u0001\u0000\u0000\u0000\u03b2}\u0001\u0000\u0000\u0000\u03b3\u03b1"+
		"\u0001\u0000\u0000\u0000\u03b4\u03b5\u0003\u0080@\u0000\u03b5\u007f\u0001"+
		"\u0000\u0000\u0000\u03b6\u03b7\u0005!\u0000\u0000\u03b7\u03c0\u0005D\u0000"+
		"\u0000\u03b8\u03bf\u0003h4\u0000\u03b9\u03bf\u0005\u0002\u0000\u0000\u03ba"+
		"\u03bb\u0005\t\u0000\u0000\u03bb\u03bc\u0003\u0082A\u0000\u03bc\u03bd"+
		"\u0005\n\u0000\u0000\u03bd\u03bf\u0001\u0000\u0000\u0000\u03be\u03b8\u0001"+
		"\u0000\u0000\u0000\u03be\u03b9\u0001\u0000\u0000\u0000\u03be\u03ba\u0001"+
		"\u0000\u0000\u0000\u03bf\u03c2\u0001\u0000\u0000\u0000\u03c0\u03be\u0001"+
		"\u0000\u0000\u0000\u03c0\u03c1\u0001\u0000\u0000\u0000\u03c1\u0081\u0001"+
		"\u0000\u0000\u0000\u03c2\u03c0\u0001\u0000\u0000\u0000\u03c3\u03c6\u0003"+
		"h4\u0000\u03c4\u03c6\u0005\u0002\u0000\u0000\u03c5\u03c3\u0001\u0000\u0000"+
		"\u0000\u03c5\u03c4\u0001\u0000\u0000\u0000\u03c6\u03c9\u0001\u0000\u0000"+
		"\u0000\u03c7\u03c5\u0001\u0000\u0000\u0000\u03c7\u03c8\u0001\u0000\u0000"+
		"\u0000\u03c8\u0083\u0001\u0000\u0000\u0000\u03c9\u03c7\u0001\u0000\u0000"+
		"\u0000\u03ca\u03cb\u0007\t\u0000\u0000\u03cb\u0085\u0001\u0000\u0000\u0000"+
		"\u03cc\u03ce\u0007\n\u0000\u0000\u03cd\u03cc\u0001\u0000\u0000\u0000\u03ce"+
		"\u03cf\u0001\u0000\u0000\u0000\u03cf\u03cd\u0001\u0000\u0000\u0000\u03cf"+
		"\u03d0\u0001\u0000\u0000\u0000\u03d0\u0087\u0001\u0000\u0000\u0000\u00b1"+
		"\u0089\u008c\u0090\u0094\u0099\u009d\u00a2\u00a6\u00a8\u00aa\u00b1\u00b5"+
		"\u00bb\u00bf\u00c2\u00c5\u00cb\u00cf\u00d1\u00d5\u00d8\u00df\u00e3\u00e7"+
		"\u00ec\u00f0\u00f3\u00f7\u00fb\u0100\u0105\u0109\u010e\u0111\u0117\u011b"+
		"\u011f\u0122\u0125\u0135\u0139\u013d\u0140\u0144\u0148\u014c\u0151\u0156"+
		"\u015b\u015f\u0163\u0167\u016b\u0172\u0174\u017c\u017e\u0180\u0183\u0189"+
		"\u0190\u0199\u019c\u01a0\u01a4\u01a8\u01aa\u01b2\u01b6\u01ba\u01c1\u01c5"+
		"\u01c9\u01cd\u01d7\u01dc\u01e0\u01e4\u01e8\u01ec\u01f0\u01f5\u01f9\u01ff"+
		"\u0204\u0208\u0216\u021b\u0220\u0226\u022a\u0232\u0237\u023b\u023e\u0242"+
		"\u0245\u0249\u0250\u0254\u0257\u025b\u0260\u0264\u0268\u026d\u0271\u0275"+
		"\u0278\u027c\u0280\u0284\u0287\u028b\u028f\u0294\u0298\u029d\u02a2\u02a6"+
		"\u02aa\u02af\u02b5\u02b9\u02bd\u02c3\u02d1\u02db\u02df\u02e3\u02ea\u02ec"+
		"\u02f3\u02f5\u02f9\u0311\u0319\u031e\u0322\u0326\u0329\u032d\u0331\u0335"+
		"\u0339\u033c\u0340\u0344\u0348\u034c\u0351\u0356\u035a\u035e\u0363\u0369"+
		"\u036d\u036f\u0373\u0377\u037a\u037e\u0384\u0388\u038d\u0391\u0395\u0399"+
		"\u039d\u03a2\u03af\u03b1\u03be\u03c0\u03c5\u03c7\u03cf";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}
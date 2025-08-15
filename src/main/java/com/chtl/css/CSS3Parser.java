package com.chtl.css;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;
import org.antlr.v4.runtime.atn.ATN;

public class CSS3Parser extends Parser {
    public CSS3Parser(TokenStream input) { super(input); }
    @Override public String getGrammarFileName() { return "CSS3.g4"; }
    @Override public String[] getRuleNames() { return new String[0]; }
    @Override public ATN getATN() { return null; }

    public static class StylesheetContext extends ParserRuleContext {
        public StylesheetContext(ParserRuleContext parent, int invokingState) {
            super(parent, invokingState);
        }
    }

    public static class RulesetContext extends ParserRuleContext {
        public RulesetContext(ParserRuleContext parent, int invokingState) {
            super(parent, invokingState);
        }
    }

    public static class DeclarationContext extends ParserRuleContext {
        public DeclarationContext(ParserRuleContext parent, int invokingState) {
            super(parent, invokingState);
        }
    }

    public static class MediaContext extends ParserRuleContext {
        public MediaContext(ParserRuleContext parent, int invokingState) {
            super(parent, invokingState);
        }
    }

    public StylesheetContext stylesheet() { return new StylesheetContext(null, 0); }
}

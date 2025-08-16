package com.chtl.javascript;import org.antlr.v4.runtime.Parser;
import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.ATN;

public class ECMAScriptParser extends Parser {
    public ECMAScriptParser(TokenStream input) { super(input); }
    @Override public String getGrammarFileName() { return "ECMAScript.g4"; }
    @Override public String[] getRuleNames() { return new String[0]; }
    @Override public ATN getATN() { return null; }

    public static class ProgramContext extends ParserRuleContext {
        public ProgramContext(ParserRuleContext parent, int invokingState) {
            super(parent, invokingState);
        }
    }

    public static class VariableStatementContext extends ParserRuleContext {
        public VariableStatementContext(ParserRuleContext parent, int invokingState) {
            super(parent, invokingState);
        }
    }

    public static class FunctionDeclarationContext extends ParserRuleContext {
        public FunctionDeclarationContext(ParserRuleContext parent, int invokingState) {
            super(parent, invokingState);
        }
    }

    public static class WithStatementContext extends ParserRuleContext {
        public WithStatementContext(ParserRuleContext parent, int invokingState) {
            super(parent, invokingState);
        }
    }

    public ProgramContext program() { return new ProgramContext(null, 0); }
}

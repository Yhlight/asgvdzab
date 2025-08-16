package com.chtl.javascript;import org.antlr.v4.runtime.CharStream;

import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.ATN;

public class ECMAScriptLexer extends Lexer {
    public ECMAScriptLexer(CharStream input) { super(input); }
    @Override public String getGrammarFileName() { return "ECMAScript.g4"; }
    @Override public String[] getRuleNames() { return new String[0]; }
    @Override public String[] getChannelNames() { return new String[0]; }
    @Override public String[] getModeNames() { return new String[0]; }
    @Override public ATN getATN() { return null; }
}

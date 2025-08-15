package com.chtl.css;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.ATN;

public class CSS3Lexer extends Lexer {
    public CSS3Lexer(CharStream input) { super(input); }
    @Override public String getGrammarFileName() { return "CSS3.g4"; }
    @Override public String[] getRuleNames() { return new String[0]; }
    @Override public String[] getChannelNames() { return new String[0]; }
    @Override public String[] getModeNames() { return new String[0]; }
    @Override public ATN getATN() { return null; }
}

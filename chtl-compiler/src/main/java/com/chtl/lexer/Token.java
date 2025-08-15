package com.chtl.lexer;

public class Token {
    private final Enum<?> type;
    private final String lexeme;
    private final int startIndexInclusive;
    private final int endIndexExclusive;

    public Token(Enum<?> type, String lexeme, int startIndexInclusive, int endIndexExclusive) {
        this.type = type;
        this.lexeme = lexeme;
        this.startIndexInclusive = startIndexInclusive;
        this.endIndexExclusive = endIndexExclusive;
    }

    public Enum<?> getType() {
        return type;
    }

    public String getLexeme() {
        return lexeme;
    }

    public int getStartIndexInclusive() {
        return startIndexInclusive;
    }

    public int getEndIndexExclusive() {
        return endIndexExclusive;
    }

    @Override
    public String toString() {
        return type + "('" + lexeme + "')@" + startIndexInclusive + "-" + endIndexExclusive;
    }
}
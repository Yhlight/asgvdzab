package com.chtl.javascript;
import org.antlr.v4.runtime.tree.*;

public class ECMAScriptBaseVisitor<T> extends AbstractParseTreeVisitor<T> implements ECMAScriptVisitor<T> {
    @Override public T visitProgram(ECMAScriptParser.ProgramContext ctx) { return visitChildren(ctx); }
    @Override public T visitVariableStatement(ECMAScriptParser.VariableStatementContext ctx) { return visitChildren(ctx); }
}

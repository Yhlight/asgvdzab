package com.chtl.javascript;

import com.chtl.compiler.javascript.ECMAScriptParser;


import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.Parser;
import org.antlr.v4.runtime.tree.AbstractParseTreeVisitor;

import org.antlr.v4.runtime.tree.*;

public class ECMAScriptBaseVisitor<T> extends AbstractParseTreeVisitor<T> implements ECMAScriptVisitor<T> {
    @Override public T visitProgram(ECMAScriptParser.ProgramContext ctx) { return visitChildren(ctx); }
    @Override public T visitVariableStatement(ECMAScriptParser.VariableStatementContext ctx) { return visitChildren(ctx); }
    @Override public T visitFunctionDeclaration(ECMAScriptParser.FunctionDeclarationContext ctx) { return visitChildren(ctx); }
    @Override public T visitWithStatement(ECMAScriptParser.WithStatementContext ctx) { return visitChildren(ctx); }
}

package com.chtl.javascript;

import org.antlr.v4.runtime.tree.ParseTreeVisitor;

import org.antlr.v4.runtime.tree.*;

public interface ECMAScriptVisitor<T> extends ParseTreeVisitor<T> {
    T visitProgram(ECMAScriptParser.ProgramContext ctx);
    T visitVariableStatement(ECMAScriptParser.VariableStatementContext ctx);
    T visitFunctionDeclaration(ECMAScriptParser.FunctionDeclarationContext ctx);
    T visitWithStatement(ECMAScriptParser.WithStatementContext ctx);
}

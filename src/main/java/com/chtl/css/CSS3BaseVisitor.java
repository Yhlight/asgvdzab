package com.chtl.css;import org.antlr.v4.runtime.tree.AbstractParseTreeVisitor;

import org.antlr.v4.runtime.tree.*;

public class CSS3BaseVisitor<T> extends AbstractParseTreeVisitor<T> implements CSS3Visitor<T> {
    @Override public T visitStylesheet(CSS3Parser.StylesheetContext ctx) { return visitChildren(ctx); }
    @Override public T visitRuleset(CSS3Parser.RulesetContext ctx) { return visitChildren(ctx); }
    @Override public T visitDeclaration(CSS3Parser.DeclarationContext ctx) { return visitChildren(ctx); }
    @Override public T visitMedia(CSS3Parser.MediaContext ctx) { return visitChildren(ctx); }
}

package com.chtl.css;import org.antlr.v4.runtime.tree.ParseTreeVisitor;

import org.antlr.v4.runtime.tree.*;

public interface CSS3Visitor<T> extends ParseTreeVisitor<T> {
    T visitStylesheet(CSS3Parser.StylesheetContext ctx);
    T visitRuleset(CSS3Parser.RulesetContext ctx);
    T visitDeclaration(CSS3Parser.DeclarationContext ctx);
    T visitMedia(CSS3Parser.MediaContext ctx);
}

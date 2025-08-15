// Generated from com/chtl/grammar/JavaScript.g4 by ANTLR 4.13.1
package com.chtl.grammar;
import org.antlr.v4.runtime.tree.ParseTreeVisitor;

/**
 * This interface defines a complete generic visitor for a parse tree produced
 * by {@link JavaScriptParser}.
 *
 * @param <T> The return type of the visit operation. Use {@link Void} for
 * operations with no return type.
 */
public interface JavaScriptVisitor<T> extends ParseTreeVisitor<T> {
	/**
	 * Visit a parse tree produced by {@link JavaScriptParser#file}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFile(JavaScriptParser.FileContext ctx);
}
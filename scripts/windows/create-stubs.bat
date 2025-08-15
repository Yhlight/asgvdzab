@echo off
REM Create stub files for ANTLR generated classes
REM This is a temporary solution to bypass ANTLR errors

echo Creating ANTLR stub files...

REM Create directories
mkdir src\main\java\com\chtl\css 2>nul
mkdir src\main\java\com\chtl\javascript 2>nul

REM Create CSS3Lexer stub
echo package com.chtl.css; > src\main\java\com\chtl\css\CSS3Lexer.java
echo import org.antlr.v4.runtime.*; >> src\main\java\com\chtl\css\CSS3Lexer.java
echo public class CSS3Lexer extends Lexer { >> src\main\java\com\chtl\css\CSS3Lexer.java
echo     public CSS3Lexer(CharStream input) { super(input); } >> src\main\java\com\chtl\css\CSS3Lexer.java
echo     @Override public String getGrammarFileName() { return "CSS3.g4"; } >> src\main\java\com\chtl\css\CSS3Lexer.java
echo     @Override public String[] getRuleNames() { return new String[0]; } >> src\main\java\com\chtl\css\CSS3Lexer.java
echo     @Override public String[] getChannelNames() { return new String[0]; } >> src\main\java\com\chtl\css\CSS3Lexer.java
echo     @Override public String[] getModeNames() { return new String[0]; } >> src\main\java\com\chtl\css\CSS3Lexer.java
echo     @Override public ATN getATN() { return null; } >> src\main\java\com\chtl\css\CSS3Lexer.java
echo } >> src\main\java\com\chtl\css\CSS3Lexer.java

REM Create CSS3Parser stub
echo package com.chtl.css; > src\main\java\com\chtl\css\CSS3Parser.java
echo import org.antlr.v4.runtime.*; >> src\main\java\com\chtl\css\CSS3Parser.java
echo import org.antlr.v4.runtime.tree.*; >> src\main\java\com\chtl\css\CSS3Parser.java
echo public class CSS3Parser extends Parser { >> src\main\java\com\chtl\css\CSS3Parser.java
echo     public CSS3Parser(TokenStream input) { super(input); } >> src\main\java\com\chtl\css\CSS3Parser.java
echo     @Override public String getGrammarFileName() { return "CSS3.g4"; } >> src\main\java\com\chtl\css\CSS3Parser.java
echo     @Override public String[] getRuleNames() { return new String[0]; } >> src\main\java\com\chtl\css\CSS3Parser.java
echo     @Override public ATN getATN() { return null; } >> src\main\java\com\chtl\css\CSS3Parser.java
echo     public static class StylesheetContext extends ParserRuleContext { >> src\main\java\com\chtl\css\CSS3Parser.java
echo         public StylesheetContext(ParserRuleContext parent, int invokingState) { super(parent, invokingState); } >> src\main\java\com\chtl\css\CSS3Parser.java
echo     } >> src\main\java\com\chtl\css\CSS3Parser.java
echo     public static class RulesetContext extends ParserRuleContext { >> src\main\java\com\chtl\css\CSS3Parser.java
echo         public RulesetContext(ParserRuleContext parent, int invokingState) { super(parent, invokingState); } >> src\main\java\com\chtl\css\CSS3Parser.java
echo     } >> src\main\java\com\chtl\css\CSS3Parser.java
echo     public StylesheetContext stylesheet() { return new StylesheetContext(null, 0); } >> src\main\java\com\chtl\css\CSS3Parser.java
echo } >> src\main\java\com\chtl\css\CSS3Parser.java

REM Create CSS3BaseVisitor stub
echo package com.chtl.css; > src\main\java\com\chtl\css\CSS3BaseVisitor.java
echo import org.antlr.v4.runtime.tree.*; >> src\main\java\com\chtl\css\CSS3BaseVisitor.java
echo public class CSS3BaseVisitor^<T^> extends AbstractParseTreeVisitor^<T^> implements CSS3Visitor^<T^> { >> src\main\java\com\chtl\css\CSS3BaseVisitor.java
echo     @Override public T visitStylesheet(CSS3Parser.StylesheetContext ctx) { return visitChildren(ctx); } >> src\main\java\com\chtl\css\CSS3BaseVisitor.java
echo     @Override public T visitRuleset(CSS3Parser.RulesetContext ctx) { return visitChildren(ctx); } >> src\main\java\com\chtl\css\CSS3BaseVisitor.java
echo } >> src\main\java\com\chtl\css\CSS3BaseVisitor.java

REM Create CSS3Visitor interface
echo package com.chtl.css; > src\main\java\com\chtl\css\CSS3Visitor.java
echo import org.antlr.v4.runtime.tree.*; >> src\main\java\com\chtl\css\CSS3Visitor.java
echo public interface CSS3Visitor^<T^> extends ParseTreeVisitor^<T^> { >> src\main\java\com\chtl\css\CSS3Visitor.java
echo     T visitStylesheet(CSS3Parser.StylesheetContext ctx); >> src\main\java\com\chtl\css\CSS3Visitor.java
echo     T visitRuleset(CSS3Parser.RulesetContext ctx); >> src\main\java\com\chtl\css\CSS3Visitor.java
echo } >> src\main\java\com\chtl\css\CSS3Visitor.java

REM Create ECMAScriptLexer stub
echo package com.chtl.javascript; > src\main\java\com\chtl\javascript\ECMAScriptLexer.java
echo import org.antlr.v4.runtime.*; >> src\main\java\com\chtl\javascript\ECMAScriptLexer.java
echo public class ECMAScriptLexer extends Lexer { >> src\main\java\com\chtl\javascript\ECMAScriptLexer.java
echo     public ECMAScriptLexer(CharStream input) { super(input); } >> src\main\java\com\chtl\javascript\ECMAScriptLexer.java
echo     @Override public String getGrammarFileName() { return "ECMAScript.g4"; } >> src\main\java\com\chtl\javascript\ECMAScriptLexer.java
echo     @Override public String[] getRuleNames() { return new String[0]; } >> src\main\java\com\chtl\javascript\ECMAScriptLexer.java
echo     @Override public String[] getChannelNames() { return new String[0]; } >> src\main\java\com\chtl\javascript\ECMAScriptLexer.java
echo     @Override public String[] getModeNames() { return new String[0]; } >> src\main\java\com\chtl\javascript\ECMAScriptLexer.java
echo     @Override public ATN getATN() { return null; } >> src\main\java\com\chtl\javascript\ECMAScriptLexer.java
echo } >> src\main\java\com\chtl\javascript\ECMAScriptLexer.java

REM Create ECMAScriptParser stub
echo package com.chtl.javascript; > src\main\java\com\chtl\javascript\ECMAScriptParser.java
echo import org.antlr.v4.runtime.*; >> src\main\java\com\chtl\javascript\ECMAScriptParser.java
echo import org.antlr.v4.runtime.tree.*; >> src\main\java\com\chtl\javascript\ECMAScriptParser.java
echo public class ECMAScriptParser extends Parser { >> src\main\java\com\chtl\javascript\ECMAScriptParser.java
echo     public ECMAScriptParser(TokenStream input) { super(input); } >> src\main\java\com\chtl\javascript\ECMAScriptParser.java
echo     @Override public String getGrammarFileName() { return "ECMAScript.g4"; } >> src\main\java\com\chtl\javascript\ECMAScriptParser.java
echo     @Override public String[] getRuleNames() { return new String[0]; } >> src\main\java\com\chtl\javascript\ECMAScriptParser.java
echo     @Override public ATN getATN() { return null; } >> src\main\java\com\chtl\javascript\ECMAScriptParser.java
echo     public static class ProgramContext extends ParserRuleContext { >> src\main\java\com\chtl\javascript\ECMAScriptParser.java
echo         public ProgramContext(ParserRuleContext parent, int invokingState) { super(parent, invokingState); } >> src\main\java\com\chtl\javascript\ECMAScriptParser.java
echo     } >> src\main\java\com\chtl\javascript\ECMAScriptParser.java
echo     public static class VariableStatementContext extends ParserRuleContext { >> src\main\java\com\chtl\javascript\ECMAScriptParser.java
echo         public VariableStatementContext(ParserRuleContext parent, int invokingState) { super(parent, invokingState); } >> src\main\java\com\chtl\javascript\ECMAScriptParser.java
echo     } >> src\main\java\com\chtl\javascript\ECMAScriptParser.java
echo     public ProgramContext program() { return new ProgramContext(null, 0); } >> src\main\java\com\chtl\javascript\ECMAScriptParser.java
echo } >> src\main\java\com\chtl\javascript\ECMAScriptParser.java

REM Create ECMAScriptBaseVisitor stub
echo package com.chtl.javascript; > src\main\java\com\chtl\javascript\ECMAScriptBaseVisitor.java
echo import org.antlr.v4.runtime.tree.*; >> src\main\java\com\chtl\javascript\ECMAScriptBaseVisitor.java
echo public class ECMAScriptBaseVisitor^<T^> extends AbstractParseTreeVisitor^<T^> implements ECMAScriptVisitor^<T^> { >> src\main\java\com\chtl\javascript\ECMAScriptBaseVisitor.java
echo     @Override public T visitProgram(ECMAScriptParser.ProgramContext ctx) { return visitChildren(ctx); } >> src\main\java\com\chtl\javascript\ECMAScriptBaseVisitor.java
echo     @Override public T visitVariableStatement(ECMAScriptParser.VariableStatementContext ctx) { return visitChildren(ctx); } >> src\main\java\com\chtl\javascript\ECMAScriptBaseVisitor.java
echo } >> src\main\java\com\chtl\javascript\ECMAScriptBaseVisitor.java

REM Create ECMAScriptVisitor interface
echo package com.chtl.javascript; > src\main\java\com\chtl\javascript\ECMAScriptVisitor.java
echo import org.antlr.v4.runtime.tree.*; >> src\main\java\com\chtl\javascript\ECMAScriptVisitor.java
echo public interface ECMAScriptVisitor^<T^> extends ParseTreeVisitor^<T^> { >> src\main\java\com\chtl\javascript\ECMAScriptVisitor.java
echo     T visitProgram(ECMAScriptParser.ProgramContext ctx); >> src\main\java\com\chtl\javascript\ECMAScriptVisitor.java
echo     T visitVariableStatement(ECMAScriptParser.VariableStatementContext ctx); >> src\main\java\com\chtl\javascript\ECMAScriptVisitor.java
echo } >> src\main\java\com\chtl\javascript\ECMAScriptVisitor.java

echo ANTLR stub files created successfully
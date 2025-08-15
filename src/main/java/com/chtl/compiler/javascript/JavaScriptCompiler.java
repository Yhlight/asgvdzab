package com.chtl.compiler.javascript;

import org.antlr.v4.runtime.CharStreams;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.tree.ParseTree;

import com.chtl.compiler.CompilationResult;
import com.chtl.compiler.javascript.JavaScriptCompiler.ECMAScriptParser;
import com.chtl.compiler.javascript.JavaScriptCompiler.JavaScriptOptimizingVisitor;
import com.chtl.javascript.ECMAScriptBaseVisitor;


import com.chtl.javascript.ECMAScriptLexer;
import com.chtl.javascript.ECMAScriptParser;
import com.chtl.model.CodeFragment;
import com.chtl.model.CompilationResult;
import com.chtl.model.FragmentType;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.HashMap;
import java.util.function.BiPredicate;
import java.util.Map;
import java.util.HashMap;
import java.util.function.BiPredicate;

/**
 * JavaScript编译器 - 基于ANTLR实现
 * 负责编译和优化JavaScript代码
 */
public class JavaScriptCompiler {
    private static final Logger logger = LoggerFactory.getLogger(JavaScriptCompiler.class);
    
    public JavaScriptCompiler() {
        logger.info("JavaScript编译器初始化完成");
    }
    
    /**
     * 编译JavaScript代码片段
     */
    public CompilationResult compile(CodeFragment fragment) {
        logger.debug("开始编译JavaScript代码");
        
        try {
            String jsCode = fragment.getContent();
            
            // 1. 使用ANTLR解析JavaScript
            ECMAScriptParser parser = createParser(jsCode);
            ParseTree tree = parser.program();
            
            // 2. 检查语法错误
            List<String> errors = checkSyntaxErrors(parser);
            if (!errors.isEmpty()) {
                CompilationResult result = new CompilationResult(FragmentType.JAVASCRIPT, jsCode);
                errors.forEach(result::addError);
                return result;
            }
            
            // 3. 访问并优化JavaScript
            JavaScriptOptimizingVisitor visitor = new JavaScriptOptimizingVisitor();
            String optimizedJS = visitor.visit(tree);
            
            // 4. 应用额外的优化
            optimizedJS = applyOptimizations(optimizedJS);
            
            CompilationResult result = new CompilationResult(FragmentType.JAVASCRIPT, optimizedJS);
            
            // 添加警告信息
            visitor.getWarnings().forEach(result::addWarning);
            
            logger.debug("JavaScript编译成功");
            return result;
            
        } catch (Exception e) {
            logger.error("JavaScript编译失败", e);
            return new CompilationResult(FragmentType.JAVASCRIPT, "", e.getMessage());
        }
    }
    
    /**
     * 创建JavaScript解析器
     */
    private ECMAScriptParser createParser(String input) {
        CharStreams.fromString inputStream = CharStreams.fromString(input);
        ECMAScriptLexer lexer = new ECMAScriptLexer(inputStream);
        CommonTokenStream tokens = new CommonTokenStream(lexer);
        ECMAScriptParser parser = new ECMAScriptParser(tokens);
        
        // 添加预测函数
        parser.addPredicate("notOpenBraceAndNotFunction", this::notOpenBraceAndNotFunction);
        parser.addPredicate("notLineTerminator", this::notLineTerminator);
        parser.addPredicate("lineTerminatorAhead", this::lineTerminatorAhead);
        parser.addPredicate("p", this::checkNext);
        
        return parser;
    }
    
    /**
     * 检查语法错误
     */
    private List<String> checkSyntaxErrors(ECMAScriptParser parser) {
        List<String> errors = new ArrayList<>();
        
        parser.removeErrorListeners();
        parser.addErrorListener(new BaseErrorListener() {
            @Override
            public void syntaxError(Recognizer<?, ?> recognizer, Object offendingSymbol,
                                  int line, int charPositionInLine, String msg,
                                  RecognitionException e) {
                errors.add(String.format("JavaScript语法错误 [%d:%d]: %s", line, charPositionInLine, msg));
            }
        });
        
        return errors;
    }
    
    /**
     * 应用JavaScript优化
     */
    private String applyOptimizations(String js) {
        // 1. 移除多余的分号
        js = removeRedundantSemicolons(js);
        
        // 2. 压缩变量名（在生产环境）
        // js = compressVariableNames(js);
        
        // 3. 移除调试代码
        js = removeDebugCode(js);
        
        // 4. 优化条件表达式
        js = optimizeConditionals(js);
        
        return js;
    }
    
    /**
     * 移除多余的分号
     */
    private String removeRedundantSemicolons(String js) {
        // 移除块结束前的分号
        js = js.replaceAll(";\\s*}", "}");
        
        // 移除连续的分号
        js = js.replaceAll(";+", ";");
        
        return js;
    }
    
    /**
     * 移除调试代码
     */
    private String removeDebugCode(String js) {
        // 移除console.log语句（可配置）
        // js = js.replaceAll("console\\.log\\([^)]*\\);?", "");
        
        // 移除debugger语句
        js = js.replaceAll("\\bdebugger\\s*;", "");
        
        return js;
    }
    
    /**
     * 优化条件表达式
     */
    private String optimizeConditionals(String js) {
        // 将 if (x == true) 优化为 if (x)
        js = js.replaceAll("if\\s*\\(([^)]+)\\s*==\\s*true\\)", "if($1)");
        js = js.replaceAll("if\\s*\\(([^)]+)\\s*===\\s*true\\)", "if($1)");
        
        // 将 if (x == false) 优化为 if (!x)
        js = js.replaceAll("if\\s*\\(([^)]+)\\s*==\\s*false\\)", "if(!$1)");
        js = js.replaceAll("if\\s*\\(([^)]+)\\s*===\\s*false\\)", "if(!$1)");
        
        return js;
    }
    
    // 预测函数实现
    private boolean notOpenBraceAndNotFunction(RuleContext context) {
        return true; // 简化实现
    }
    
    private boolean notLineTerminator(RuleContext context) {
        return true; // 简化实现
    }
    
    private boolean lineTerminatorAhead(RuleContext context) {
        return false; // 简化实现
    }
    
    private boolean checkNext(RuleContext context, String value) {
        return true; // 简化实现
    }
    
    /**
     * JavaScript优化访问器
     */
    private static class JavaScriptOptimizingVisitor extends ECMAScriptBaseVisitor<String> {
        private final List<String> warnings = new ArrayList<>();
        private final StringBuilder output = new StringBuilder();
        
        public List<String> getWarnings() {
            return warnings;
        }
        
        @Override
        public String visitProgram(ECMAScriptParser.ProgramContext ctx) {
            if (ctx.sourceElements() != null) {
                visit(ctx.sourceElements());
            }
            return output.toString();
        }
        
        @Override
        public String visitVariableStatement(ECMAScriptParser.VariableStatementContext ctx) {
            String varType = ctx.varModifier().getText();
            
            // 检查var使用
            if ("var".equals(varType)) {
                warnings.add("建议使用 'let' 或 'const' 替代 'var' 以避免作用域问题");
            }
            
            output.append(varType).append(" ");
            visit(ctx.variableDeclarationList());
            output.append(";");
            
            return null;
        }
        
        @Override
        public String visitFunctionDeclaration(ECMAScriptParser.FunctionDeclarationContext ctx) {
            output.append("function ");
            output.append(ctx.Identifier().getText());
            output.append("(");
            
            if (ctx.formalParameterList() != null) {
                visit(ctx.formalParameterList());
            }
            
            output.append("){");
            
            if (ctx.functionBody() != null) {
                visit(ctx.functionBody());
            }
            
            output.append("}");
            
            return null;
        }
        
        @Override
        public String visitWithStatement(ECMAScriptParser.WithStatementContext ctx) {
            warnings.add("不建议使用 'with' 语句，它可能导致性能问题和代码混淆");
            
            output.append("with(");
            visit(ctx.expressionSequence());
            output.append(")");
            visit(ctx.statement());
            
            return null;
        }
        
        @Override
        public String visitTerminal(TerminalNode node) {
            output.append(node.getText()).append(" ");
            return null;
        }
        
        @Override
        protected String aggregateResult(String aggregate, String nextResult) {
            return output.toString();
        }
    }
}

// 扩展ECMAScriptParser以支持预测
class ECMAScriptParser extends ECMAScriptBaseParser {
    private final Map<String, BiPredicate<RuleContext, String>> predicates = new HashMap<>();
    
    public ECMAScriptParser(TokenStream input) {
        super(input);
    }
    
    public void addPredicate(String name, BiPredicate<RuleContext, String> predicate) {
        predicates.put(name, predicate);
    }
    
    @Override
    public boolean sempred(RuleContext _localctx, int ruleIndex, int predIndex) {
        // 实现语义预测
        return true;
    }
}
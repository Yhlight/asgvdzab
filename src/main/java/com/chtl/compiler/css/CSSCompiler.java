package com.chtl.compiler.css;

import com.chtl.css.CSS3Lexer;
import com.chtl.css.CSS3Parser;
import com.chtl.model.CodeFragment;
import com.chtl.model.CompilationResult;
import com.chtl.model.FragmentType;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.ArrayList;
import java.util.List;

/**
 * CSS编译器 - 基于ANTLR实现
 * 负责编译和优化CSS代码
 */
public class CSSCompiler {
    private static final Logger logger = LoggerFactory.getLogger(CSSCompiler.class);
    
    public CSSCompiler() {
        logger.info("CSS编译器初始化完成");
    }
    
    /**
     * 编译CSS代码片段
     */
    public CompilationResult compile(CodeFragment fragment) {
        logger.debug("开始编译CSS代码");
        
        try {
            String cssCode = fragment.getContent();
            
            // 1. 使用ANTLR解析CSS
            CSS3Parser parser = createParser(cssCode);
            ParseTree tree = parser.stylesheet();
            
            // 2. 检查语法错误
            List<String> errors = checkSyntaxErrors(parser);
            if (!errors.isEmpty()) {
                CompilationResult result = new CompilationResult(FragmentType.CSS, cssCode);
                errors.forEach(result::addError);
                return result;
            }
            
            // 3. 访问并优化CSS
            CSSOptimizingVisitor visitor = new CSSOptimizingVisitor();
            String optimizedCSS = visitor.visit(tree);
            
            // 4. 应用额外的优化
            optimizedCSS = applyOptimizations(optimizedCSS);
            
            CompilationResult result = new CompilationResult(FragmentType.CSS, optimizedCSS);
            
            // 添加警告信息
            visitor.getWarnings().forEach(result::addWarning);
            
            logger.debug("CSS编译成功");
            return result;
            
        } catch (Exception e) {
            logger.error("CSS编译失败", e);
            return new CompilationResult(FragmentType.CSS, "", e.getMessage());
        }
    }
    
    /**
     * 创建CSS解析器
     */
    private CSS3Parser createParser(String input) {
        ANTLRInputStream inputStream = new ANTLRInputStream(input);
        CSS3Lexer lexer = new CSS3Lexer(inputStream);
        CommonTokenStream tokens = new CommonTokenStream(lexer);
        return new CSS3Parser(tokens);
    }
    
    /**
     * 检查语法错误
     */
    private List<String> checkSyntaxErrors(CSS3Parser parser) {
        List<String> errors = new ArrayList<>();
        
        parser.removeErrorListeners();
        parser.addErrorListener(new BaseErrorListener() {
            @Override
            public void syntaxError(Recognizer<?, ?> recognizer, Object offendingSymbol,
                                  int line, int charPositionInLine, String msg,
                                  RecognitionException e) {
                errors.add(String.format("CSS语法错误 [%d:%d]: %s", line, charPositionInLine, msg));
            }
        });
        
        return errors;
    }
    
    /**
     * 应用CSS优化
     */
    private String applyOptimizations(String css) {
        // 1. 移除多余的空白
        css = css.replaceAll("\\s+", " ");
        css = css.replaceAll("\\s*([{}:;,])\\s*", "$1");
        
        // 2. 合并相同的规则
        css = mergeDuplicateRules(css);
        
        // 3. 压缩颜色值
        css = compressColors(css);
        
        // 4. 移除不必要的单位
        css = removeUnnecessaryUnits(css);
        
        return css;
    }
    
    /**
     * 合并重复的CSS规则
     */
    private String mergeDuplicateRules(String css) {
        // 简化实现，实际应该更复杂
        return css;
    }
    
    /**
     * 压缩颜色值
     */
    private String compressColors(String css) {
        // 将 #RRGGBB 转换为 #RGB（如果可能）
        css = css.replaceAll("#([0-9a-fA-F])\\1([0-9a-fA-F])\\2([0-9a-fA-F])\\3", "#$1$2$3");
        
        // 将常见颜色名称转换为更短的十六进制
        css = css.replaceAll("\\bwhite\\b", "#fff");
        css = css.replaceAll("\\bblack\\b", "#000");
        
        return css;
    }
    
    /**
     * 移除不必要的单位
     */
    private String removeUnnecessaryUnits(String css) {
        // 移除0值的单位
        css = css.replaceAll("\\b0px\\b", "0");
        css = css.replaceAll("\\b0em\\b", "0");
        css = css.replaceAll("\\b0%\\b", "0");
        
        return css;
    }
    
    /**
     * CSS优化访问器
     */
    private static class CSSOptimizingVisitor extends CSS3BaseVisitor<String> {
        private final List<String> warnings = new ArrayList<>();
        private final StringBuilder output = new StringBuilder();
        
        public List<String> getWarnings() {
            return warnings;
        }
        
        @Override
        public String visitStylesheet(CSS3Parser.StylesheetContext ctx) {
            visitChildren(ctx);
            return output.toString();
        }
        
        @Override
        public String visitRuleset(CSS3Parser.RulesetContext ctx) {
            // 处理规则集
            if (ctx.selectors() != null) {
                output.append(ctx.selectors().getText());
            }
            output.append("{");
            
            if (ctx.declarationList() != null) {
                visit(ctx.declarationList());
            }
            
            output.append("}");
            return null;
        }
        
        @Override
        public String visitDeclaration(CSS3Parser.DeclarationContext ctx) {
            // 处理声明
            String property = ctx.property().getText();
            String value = ctx.expr().getText();
            
            // 检查浏览器兼容性
            checkBrowserCompatibility(property, value);
            
            output.append(property).append(":").append(value);
            
            if (ctx.important() != null) {
                output.append("!important");
            }
            
            return null;
        }
        
        @Override
        public String visitMedia(CSS3Parser.MediaContext ctx) {
            // 处理媒体查询
            output.append("@media");
            if (ctx.mediaQueryList() != null) {
                output.append(" ").append(ctx.mediaQueryList().getText());
            }
            output.append("{");
            
            if (ctx.groupRuleBody() != null) {
                visit(ctx.groupRuleBody());
            }
            
            output.append("}");
            return null;
        }
        
        private void checkBrowserCompatibility(String property, String value) {
            // 检查需要浏览器前缀的属性
            if (property.equals("transform") || property.equals("transition") || 
                property.equals("animation") || property.equals("flex")) {
                warnings.add("属性 '" + property + "' 可能需要浏览器前缀以确保兼容性");
            }
            
            // 检查新的CSS特性
            if (property.startsWith("grid") || property.equals("gap")) {
                warnings.add("属性 '" + property + "' 是较新的CSS特性，请确保目标浏览器支持");
            }
        }
        
        @Override
        protected String aggregateResult(String aggregate, String nextResult) {
            return output.toString();
        }
    }
}
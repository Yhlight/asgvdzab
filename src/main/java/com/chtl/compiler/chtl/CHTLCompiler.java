package com.chtl.compiler.chtl;
import java.util.HashMap;
import java.util.Map;
import com.chtl.compiler.CHTLCompiler;
import com.chtl.compiler.CompilerResult;
import com.chtl.compiler.style.LocalStyleProcessor;
import com.chtl.model.AttributeNode;
import com.chtl.model.CHTLNode;
import com.chtl.model.CHTLNodeVisitor;
import com.chtl.model.CodeFragment;
import com.chtl.model.CustomNode;
import com.chtl.model.ElementNode;
import com.chtl.model.ScriptNode;
import com.chtl.model.StyleNode;
import com.chtl.model.TemplateNode;
import com.chtl.model.TextNode;
import com.chtl.model.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * CHTL编译器 - 手写实现
 * 负责编译CHTL主体代码和局部样式
 */
public class CHTLCompiler {
    private static final Logger logger = LoggerFactory.getLogger(CHTLCompiler.class);
    
    private final CHTLParser parser;
    private final CHTLCodeGenerator codeGenerator;
    private final LocalStyleProcessor localStyleProcessor;
    
    // 存储模板和自定义定义
    private final Map<String, TemplateNode> templates;
    private final Map<String, CustomNode> customs;
    
    public CHTLCompiler() {
        this.parser = new CHTLParser();
        this.codeGenerator = new CHTLCodeGenerator();
        this.localStyleProcessor = new LocalStyleProcessor();
        this.templates = new HashMap<>();
        this.customs = new HashMap<>();
        
        logger.info("CHTL编译器初始化完成");
    }
    
    /**
     * 编译CHTL代码片段
     */
    public CompilationResult compile(CodeFragment fragment) {
        logger.debug("开始编译CHTL代码片段");
        
        try {
            // 1. 解析CHTL代码生成AST
            CHTLNode ast = parser.parse(fragment.getContent());
            
            // 2. 收集模板和自定义定义
            collectDefinitions(ast);
            
            // 3. 处理模板展开和继承
            processTemplates(ast);
            
            // 4. 生成HTML代码
            String html = codeGenerator.generate(ast);
            
            // 5. 提取生成的CSS和JS
            CompilationResult result = new CompilationResult(FragmentType.CHTL, html);
            result.setGeneratedCSS(codeGenerator.getGeneratedCSS());
            result.setGeneratedJS(codeGenerator.getGeneratedJS());
            
            logger.debug("CHTL编译成功");
            return result;
            
        } catch (Exception e) {
            logger.error("CHTL编译失败", e);
            return new CompilationResult(FragmentType.CHTL, "", e.getMessage());
        }
    }
    
    /**
     * 编译局部样式
     */
    public CompilationResult compileLocalStyle(CodeFragment fragment) {
        logger.debug("开始编译局部样式");
        
        try {
            LocalStyleResult result = localStyleProcessor.process(fragment.getContent());
            
            CompilationResult compilationResult = new CompilationResult(
                FragmentType.CHTL_LOCAL_STYLE, 
                result.getInlineStyles()
            );
            compilationResult.setGeneratedCSS(result.getGlobalStyles());
            
            return compilationResult;
            
        } catch (Exception e) {
            logger.error("局部样式编译失败", e);
            return new CompilationResult(FragmentType.CHTL_LOCAL_STYLE, "", e.getMessage());
        }
    }
    
    /**
     * 收集模板和自定义定义
     */
    private void collectDefinitions(CHTLNode root) {
        DefinitionCollector collector = new DefinitionCollector();
        root.accept(collector);
    }
    
    /**
     * 处理模板展开和继承
     */
    private void processTemplates(CHTLNode root) {
        TemplateProcessor processor = new TemplateProcessor(templates, customs);
        processor.process(root);
    }
    
    /**
     * 定义收集器 - 内部访问者类
     */
    private class DefinitionCollector implements CHTLNodeVisitor {
        @Override
        public void visit(ElementNode node) {
            for (CHTLNode child : node.getChildren()) {
                child.accept(this);
            }
        }
        
        @Override
        public void visit(TextNode node) {
            // 文本节点无需处理
        }
        
        @Override
        public void visit(AttributeNode node) {
            // 属性节点无需处理
        }
        
        @Override
        public void visit(StyleNode node) {
            // 样式节点无需处理
        }
        
        @Override
        public void visit(ScriptNode node) {
            // 脚本节点无需处理
        }
        
        @Override
        public void visit(TemplateNode node) {
            templates.put(node.getTemplateName(), node);
            logger.debug("收集到模板: {} - {}", node.getType(), node.getTemplateName());
        }
        
        @Override
        public void visit(CustomNode node) {
            customs.put(node.getCustomName(), node);
            logger.debug("收集到自定义: {} - {}", node.getType(), node.getCustomName());
        }
    }

    @Override
    public void visitTemplate() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }

    @Override
    public void visitText() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }

    @Override
    public void visitElement() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }

    @Override
    public void visitAttribute() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }

    @Override
    public void visitScript() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }

    @Override
    public void visitStyle() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }

    @Override
    public void visitCustom() {
        // TODO: Implement this method
        throw new UnsupportedOperationException("Not implemented yet");
    }
}

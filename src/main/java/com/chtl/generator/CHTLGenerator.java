package com.chtl.generator;

import com.chtl.ast.*;
import com.chtl.ast.node.*;
import java.util.*;

/**
 * CHTL代码生成器
 * 从AST生成HTML、CSS和JavaScript代码
 */
public class CHTLGenerator implements CHTLASTVisitor {
    private StringBuilder html;
    private StringBuilder css;
    private StringBuilder javascript;
    private StringBuilder currentOutput;
    private int indentLevel;
    private String indentString = "    ";
    
    // 存储全局样式规则
    private List<String> globalStyleRules;
    
    // 存储局部脚本
    private List<String> localScripts;
    
    // 自动生成的class/id计数器
    private int autoClassCounter = 0;
    private int autoIdCounter = 0;
    
    // 当前上下文
    private Stack<ElementNode> elementStack;
    
    public CHTLGenerator() {
        this.globalStyleRules = new ArrayList<>();
        this.localScripts = new ArrayList<>();
        this.elementStack = new Stack<>();
    }
    
    /**
     * 生成代码
     */
    public GenerationResult generate(RootNode root) {
        // 初始化
        this.html = new StringBuilder();
        this.css = new StringBuilder();
        this.javascript = new StringBuilder();
        this.currentOutput = html;
        this.indentLevel = 0;
        this.globalStyleRules.clear();
        this.localScripts.clear();
        this.autoClassCounter = 0;
        this.autoIdCounter = 0;
        this.elementStack.clear();
        
        // 遍历AST
        root.accept(this);
        
        // 生成最终CSS
        generateFinalCSS();
        
        // 生成最终JavaScript
        generateFinalJavaScript();
        
        return new GenerationResult(html.toString(), css.toString(), javascript.toString());
    }
    
    @Override
    public void visitRoot(RootNode node) {
        for (CHTLASTNode child : node.getChildren()) {
            child.accept(this);
        }
    }
    
    @Override
    public void visitElement(ElementNode node) {
        elementStack.push(node);
        
        // 生成开始标签
        appendIndent();
        currentOutput.append("<").append(node.getTagName());
        
        // 生成属性
        for (AttributeNode attr : node.getAttributes()) {
            attr.accept(this);
        }
        
        currentOutput.append(">");
        
        // 检查是否是自闭合标签
        if (isSelfClosingTag(node.getTagName())) {
            currentOutput.append("\n");
            elementStack.pop();
            return;
        }
        
        currentOutput.append("\n");
        indentLevel++;
        
        // 处理子节点
        for (CHTLASTNode child : node.getChildren()) {
            child.accept(this);
        }
        
        indentLevel--;
        
        // 生成结束标签
        appendIndent();
        currentOutput.append("</").append(node.getTagName()).append(">\n");
        
        elementStack.pop();
    }
    
    @Override
    public void visitText(TextNode node) {
        String content = node.getContent().trim();
        if (!content.isEmpty()) {
            appendIndent();
            currentOutput.append(content).append("\n");
        }
    }
    
    @Override
    public void visitAttribute(AttributeNode node) {
        currentOutput.append(" ").append(node.getName());
        
        if (node.getValue() != null) {
            currentOutput.append("=\"");
            currentOutput.append(getAttributeValue(node.getValue()));
            currentOutput.append("\"");
        }
    }
    
    @Override
    public void visitComment(CommentNode node) {
        if (node.isGeneratorComment()) {
            // 生成器注释会被包含在HTML中
            appendIndent();
            currentOutput.append("<!-- ").append(node.getContent()).append(" -->\n");
        }
        // 其他注释不会被包含在生成的代码中
    }
    
    @Override
    public void visitStyleBlock(StyleBlockNode node) {
        if (!node.isLocal()) {
            // 全局样式块直接添加到CSS
            for (StylePropertyNode prop : node.getInlineStyles()) {
                css.append(getPropertyString(prop)).append("\n");
            }
            for (StyleRuleNode rule : node.getRules()) {
                rule.accept(this);
            }
            return;
        }
        
        // 局部样式块处理
        ElementNode parentElement = elementStack.peek();
        String elementId = getOrGenerateId(parentElement);
        
        // 处理内联样式
        if (!node.getInlineStyles().isEmpty()) {
            StringBuilder inlineStyle = new StringBuilder();
            for (StylePropertyNode prop : node.getInlineStyles()) {
                inlineStyle.append(getPropertyString(prop)).append(" ");
            }
            
            // 添加style属性到父元素
            AttributeNode styleAttr = new AttributeNode("style", new UnquotedLiteralNode(inlineStyle.toString().trim()));
            parentElement.addAttribute(styleAttr);
        }
        
        // 处理样式规则
        for (StyleRuleNode rule : node.getRules()) {
            StyleSelectorNode selector = rule.getSelector();
            String fullSelector = buildLocalSelector(selector, elementId);
            
            // 生成CSS规则
            StringBuilder cssRule = new StringBuilder();
            cssRule.append(fullSelector).append(" {\n");
            
            for (StylePropertyNode prop : rule.getProperties()) {
                cssRule.append("    ").append(getPropertyString(prop)).append("\n");
            }
            
            cssRule.append("}\n");
            globalStyleRules.add(cssRule.toString());
        }
    }
    
    @Override
    public void visitStyleRule(StyleRuleNode node) {
        StringBuilder cssRule = new StringBuilder();
        cssRule.append(node.getSelector().getFullSelector()).append(" {\n");
        
        for (StylePropertyNode prop : node.getProperties()) {
            cssRule.append("    ").append(getPropertyString(prop)).append("\n");
        }
        
        cssRule.append("}\n");
        css.append(cssRule);
    }
    
    @Override
    public void visitStyleProperty(StylePropertyNode node) {
        // 属性在父节点中处理
    }
    
    @Override
    public void visitStyleSelector(StyleSelectorNode node) {
        // 选择器在父节点中处理
    }
    
    @Override
    public void visitScriptBlock(ScriptBlockNode node) {
        String content = node.getContent();
        
        if (node.isLocal()) {
            // 局部脚本包装在立即执行函数中
            StringBuilder localScript = new StringBuilder();
            localScript.append("(function() {\n");
            
            if (node.isCHTLJS()) {
                // 需要CHTL JS转换
                content = transformCHTLJS(content);
            }
            
            localScript.append(content);
            localScript.append("\n})();\n");
            
            localScripts.add(localScript.toString());
        } else {
            // 全局脚本
            if (node.isCHTLJS()) {
                content = transformCHTLJS(content);
            }
            javascript.append(content).append("\n");
        }
    }
    
    @Override
    public void visitTemplateDefinition(TemplateDefinitionNode node) {
        // 模板定义不生成代码，只在使用时展开
    }
    
    @Override
    public void visitTemplateUsage(TemplateUsageNode node) {
        // TODO: 实现模板展开逻辑
    }
    
    @Override
    public void visitCustomDefinition(CustomDefinitionNode node) {
        // 自定义定义不生成代码，只在使用时展开
    }
    
    @Override
    public void visitCustomUsage(CustomUsageNode node) {
        // TODO: 实现自定义展开逻辑
    }
    
    @Override
    public void visitCustomOperation(CustomOperationNode node) {
        // TODO: 实现自定义操作逻辑
    }
    
    @Override
    public void visitOriginBlock(OriginBlockNode node) {
        String content = node.getContent();
        
        switch (node.getOriginType()) {
            case HTML:
                appendIndent();
                currentOutput.append(content).append("\n");
                break;
            case STYLE:
                css.append(content).append("\n");
                break;
            case JAVASCRIPT:
                javascript.append(content).append("\n");
                break;
        }
    }
    
    @Override
    public void visitImportStatement(ImportStatementNode node) {
        // TODO: 实现导入逻辑
    }
    
    @Override
    public void visitNamespace(NamespaceNode node) {
        // 处理命名空间内的内容
        for (CHTLASTNode child : node.getChildren()) {
            child.accept(this);
        }
    }
    
    @Override
    public void visitConfiguration(ConfigurationNode node) {
        // 配置不生成代码
    }
    
    @Override
    public void visitConstraint(ConstraintNode node) {
        // 约束不生成代码
    }
    
    @Override
    public void visitVarGroupDefinition(VarGroupDefinitionNode node) {
        // 变量组定义不生成代码
    }
    
    @Override
    public void visitVarGroupUsage(VarGroupUsageNode node) {
        // TODO: 实现变量组替换逻辑
    }
    
    @Override
    public void visitInheritStatement(InheritStatementNode node) {
        // TODO: 实现继承逻辑
    }
    
    @Override
    public void visitStringLiteral(StringLiteralNode node) {
        currentOutput.append(node.getValue());
    }
    
    @Override
    public void visitUnquotedLiteral(UnquotedLiteralNode node) {
        currentOutput.append(node.getValue());
    }
    
    @Override
    public void visitNumberLiteral(NumberLiteralNode node) {
        currentOutput.append(node.getValue());
    }
    
    // 辅助方法
    
    private void appendIndent() {
        for (int i = 0; i < indentLevel; i++) {
            currentOutput.append(indentString);
        }
    }
    
    private boolean isSelfClosingTag(String tagName) {
        Set<String> selfClosingTags = new HashSet<>(Arrays.asList(
            "area", "base", "br", "col", "embed", "hr", "img", "input",
            "link", "meta", "param", "source", "track", "wbr"
        ));
        return selfClosingTags.contains(tagName.toLowerCase());
    }
    
    private String getAttributeValue(CHTLASTNode value) {
        if (value instanceof StringLiteralNode) {
            return ((StringLiteralNode) value).getValue();
        } else if (value instanceof UnquotedLiteralNode) {
            return ((UnquotedLiteralNode) value).getValue();
        } else if (value instanceof NumberLiteralNode) {
            return ((NumberLiteralNode) value).getValue();
        } else if (value instanceof VarGroupUsageNode) {
            // TODO: 实现变量组值替换
            return "";
        }
        return "";
    }
    
    private String getPropertyString(StylePropertyNode prop) {
        StringBuilder sb = new StringBuilder();
        sb.append(prop.getProperty()).append(": ");
        
        if (prop.getValue() != null) {
            sb.append(getAttributeValue(prop.getValue()));
        }
        
        if (prop.isImportant()) {
            sb.append(" !important");
        }
        
        sb.append(";");
        return sb.toString();
    }
    
    private String getOrGenerateId(ElementNode element) {
        // 检查是否已有id属性
        AttributeNode idAttr = element.getAttribute("id");
        if (idAttr != null && idAttr.getValue() != null) {
            return getAttributeValue(idAttr.getValue());
        }
        
        // 生成自动id
        String autoId = "chtl-auto-" + (++autoIdCounter);
        element.addAttribute(new AttributeNode("id", new UnquotedLiteralNode(autoId)));
        return autoId;
    }
    
    private String buildLocalSelector(StyleSelectorNode selector, String parentId) {
        String baseSelector = "#" + parentId;
        
        switch (selector.getSelectorType()) {
            case CLASS:
                return baseSelector + " ." + selector.getValue();
            case ID:
                return baseSelector + " #" + selector.getValue();
            case TAG:
                return baseSelector + " " + selector.getValue();
            case CONTEXT:
                // &选择器引用父元素
                return baseSelector + selector.getPseudoValue();
            default:
                return selector.getFullSelector();
        }
    }
    
    private void generateFinalCSS() {
        // 添加局部样式规则到CSS
        for (String rule : globalStyleRules) {
            css.append(rule);
        }
    }
    
    private void generateFinalJavaScript() {
        // 创建CHTL辅助对象
        javascript.insert(0, "// CHTL Generated JavaScript\n");
        javascript.append("\n// Local Scripts\n");
        
        // 添加局部脚本
        for (String script : localScripts) {
            javascript.append(script);
        }
    }
    
    private String transformCHTLJS(String content) {
        // 使用CHTL JS解析器和生成器
        com.chtl.parser.CHTLJSParser parser = new com.chtl.parser.CHTLJSParser();
        List<com.chtl.ast.chtljs.CHTLJSASTNode> statements = parser.parse(content);
        
        if (!parser.getErrors().isEmpty()) {
            // 如果有错误，返回原内容并记录错误
            System.err.println("CHTL JS解析错误:");
            for (String error : parser.getErrors()) {
                System.err.println("  " + error);
            }
            return content;
        }
        
        CHTLJSGenerator generator = new CHTLJSGenerator();
        return generator.generate(statements);
    }
    
    /**
     * 生成结果
     */
    public static class GenerationResult {
        private final String html;
        private final String css;
        private final String javascript;
        
        public GenerationResult(String html, String css, String javascript) {
            this.html = html;
            this.css = css;
            this.javascript = javascript;
        }
        
        public String getHtml() {
            return html;
        }
        
        public String getCss() {
            return css;
        }
        
        public String getJavascript() {
            return javascript;
        }
        
        /**
         * 生成完整的HTML文档
         */
        public String getCompleteHTML() {
            StringBuilder complete = new StringBuilder();
            complete.append("<!DOCTYPE html>\n");
            complete.append("<html>\n");
            complete.append("<head>\n");
            complete.append("    <meta charset=\"UTF-8\">\n");
            
            if (!css.isEmpty()) {
                complete.append("    <style>\n");
                String[] cssLines = css.split("\n");
                for (String line : cssLines) {
                    if (!line.trim().isEmpty()) {
                        complete.append("        ").append(line).append("\n");
                    }
                }
                complete.append("    </style>\n");
            }
            
            complete.append("</head>\n");
            complete.append("<body>\n");
            
            String[] htmlLines = html.split("\n");
            for (String line : htmlLines) {
                if (!line.trim().isEmpty()) {
                    complete.append("    ").append(line).append("\n");
                }
            }
            
            if (!javascript.isEmpty()) {
                complete.append("    <script>\n");
                String[] jsLines = javascript.split("\n");
                for (String line : jsLines) {
                    if (!line.trim().isEmpty()) {
                        complete.append("        ").append(line).append("\n");
                    }
                }
                complete.append("    </script>\n");
            }
            
            complete.append("</body>\n");
            complete.append("</html>");
            
            return complete.toString();
        }
    }
}
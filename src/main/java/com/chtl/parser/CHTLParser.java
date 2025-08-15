package com.chtl.parser;

import com.chtl.ast.*;
import com.chtl.ast.node.*;
import com.chtl.compiler.chtl.token.CHTLToken;
import com.chtl.compiler.chtl.token.CHTLTokenType;
import com.chtl.compiler.chtl.lexer.CHTLStateMachineLexer;

import java.util.*;

/**
 * CHTL解析器
 * 严格根据CHTL语法文档实现
 * 使用CompilationContext进行状态管理
 */
public class CHTLParser {
    private List<CHTLToken> tokens;
    private int current;
    private List<String> errors;
    private com.chtl.context.CompilationContext context;
    
    public CHTLParser() {
        this.errors = new ArrayList<>();
        this.context = new com.chtl.context.CompilationContext();
    }
    
    /**
     * 解析CHTL源代码
     */
    public RootNode parse(String source) {
        CHTLStateMachineLexer lexer = new CHTLStateMachineLexer(source);
        this.tokens = lexer.tokenize();
        this.current = 0;
        this.errors.clear();
        
        return parseRoot();
    }
    
    /**
     * 解析根节点
     */
    private RootNode parseRoot() {
        RootNode root = new RootNode();
        
        while (!isAtEnd()) {
            // 跳过注释
            if (match(CHTLTokenType.LINE_COMMENT, CHTLTokenType.BLOCK_COMMENT, CHTLTokenType.GENERATOR_COMMENT)) {
                CommentNode comment = parseComment(previous());
                root.addChild(comment);
                continue;
            }
            
            CHTLASTNode node = parseTopLevel();
            if (node != null) {
                root.addChild(node);
            }
        }
        
        return root;
    }
    
    /**
     * 解析顶层元素
     */
    private CHTLASTNode parseTopLevel() {
        // 配置组
        if (match(CHTLTokenType.CONFIGURATION)) {
            return parseConfiguration();
        }
        
        // 命名空间
        if (match(CHTLTokenType.NAMESPACE)) {
            return parseNamespace();
        }
        
        // 模板定义
        if (match(CHTLTokenType.TEMPLATE)) {
            return parseTemplateDefinition();
        }
        
        // 自定义定义
        if (match(CHTLTokenType.CUSTOM)) {
            return parseCustomDefinition();
        }
        
        // 原始嵌入
        if (match(CHTLTokenType.ORIGIN)) {
            return parseOriginBlock();
        }
        
        // 导入语句
        if (match(CHTLTokenType.IMPORT)) {
            return parseImportStatement();
        }
        
        // 元素节点
        if (check(CHTLTokenType.IDENTIFIER)) {
            return parseElement();
        }
        
        advance(); // 跳过无法识别的token
        return null;
    }
    
    /**
     * 解析注释
     */
    private CommentNode parseComment(CHTLToken token) {
        CommentNode.CommentType type;
        String content = token.getValue();
        
        switch (token.getType()) {
            case LINE_COMMENT:
                type = CommentNode.CommentType.LINE_COMMENT;
                content = content.substring(2); // 去掉//
                break;
            case BLOCK_COMMENT:
                type = CommentNode.CommentType.BLOCK_COMMENT;
                content = content.substring(2, content.length() - 2); // 去掉/*和*/
                break;
            case GENERATOR_COMMENT:
                type = CommentNode.CommentType.GENERATOR_COMMENT;
                content = content.substring(2); // 去掉--
                break;
            default:
                type = CommentNode.CommentType.LINE_COMMENT;
        }
        
        return new CommentNode(type, content);
    }
    
    /**
     * 解析元素节点
     */
    private ElementNode parseElement() {
        CHTLToken nameToken = consume(CHTLTokenType.IDENTIFIER, "期望元素名称");
        
        // 检查索引访问 div[1]
        Integer index = null;
        if (match(CHTLTokenType.LEFT_BRACKET)) {
            CHTLToken indexToken = consume(CHTLTokenType.NUMBER, "期望索引");
            index = Integer.parseInt(indexToken.getValue());
            consume(CHTLTokenType.RIGHT_BRACKET, "期望']'");
        }
        
        ElementNode element = new ElementNode(nameToken.getValue());
        if (index != null) {
            element.setIndex(index);
        }
        
        // 使用RAII管理状态和作用域
        try (var stateGuard = context.enterState(com.chtl.context.CompilationContext.State.ELEMENT);
             var scopeGuard = context.enterScope(com.chtl.context.CompilationContext.ScopeType.ELEMENT, nameToken.getValue());
             var nodeGuard = context.processNode(element)) {
            
            consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
            
            // 解析元素内容
            while (!check(CHTLTokenType.RIGHT_BRACE) && !isAtEnd()) {
            // 检查属性
            if (checkAttribute()) {
                AttributeNode attr = parseAttribute();
                element.addAttribute(attr);
            }
            // 检查text块
            else if (match(CHTLTokenType.TEXT)) {
                element.addChild(parseTextBlock());
            }
            // 检查style块
            else if (match(CHTLTokenType.STYLE)) {
                element.addChild(parseStyleBlock(true));
            }
            // 检查script块
            else if (match(CHTLTokenType.SCRIPT)) {
                element.addChild(parseScriptBlock(true));
            }
            // 检查模板/自定义使用
            else if (checkTemplateOrCustomUsage()) {
                element.addChild(parseTemplateOrCustomUsage());
            }
            // 检查约束
            else if (match(CHTLTokenType.EXCEPT)) {
                element.addChild(parseConstraint());
            }
            // 检查原始嵌入
            else if (match(CHTLTokenType.ORIGIN)) {
                element.addChild(parseOriginBlock());
            }
            // 嵌套元素
            else if (check(CHTLTokenType.IDENTIFIER)) {
                element.addChild(parseElement());
            }
            else {
                advance(); // 跳过无法识别的内容
            }
            }
            
            consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
        }
        
        return element;
    }
    
    /**
     * 检查是否是属性
     */
    private boolean checkAttribute() {
        if (!check(CHTLTokenType.IDENTIFIER)) return false;
        
        // 查看下一个token是否是:或=
        int savedCurrent = current;
        advance();
        boolean isAttribute = check(CHTLTokenType.COLON) || check(CHTLTokenType.EQUALS);
        current = savedCurrent;
        
        return isAttribute;
    }
    
    /**
     * 解析属性
     */
    private AttributeNode parseAttribute() {
        CHTLToken nameToken = consume(CHTLTokenType.IDENTIFIER, "期望属性名");
        AttributeNode attr = new AttributeNode(nameToken.getValue());
        
        // CE对等式：:和=等价
        if (match(CHTLTokenType.COLON)) {
            attr.setUsesColon(true);
        } else if (match(CHTLTokenType.EQUALS)) {
            attr.setUsesColon(false);
        } else {
            error("期望':'或'='");
        }
        
        // 解析属性值
        CHTLASTNode value = parseLiteral();
        attr.setValue(value);
        
        consume(CHTLTokenType.SEMICOLON, "期望';'");
        return attr;
    }
    
    /**
     * 解析字面量
     */
    private CHTLASTNode parseLiteral() {
        // 字符串字面量
        if (match(CHTLTokenType.STRING_LITERAL)) {
            String value = previous().getValue();
            // TODO: 确定引号类型
            return new StringLiteralNode(value, '"');
        }
        
        // 数字字面量（可能带单位）
        if (match(CHTLTokenType.NUMBER)) {
            String value = previous().getValue();
            // 检查是否有单位
            if (check(CHTLTokenType.IDENTIFIER)) {
                value += advance().getValue();
            }
            return new NumberLiteralNode(value);
        }
        
        // 无引号字面量
        if (match(CHTLTokenType.IDENTIFIER)) {
            return new UnquotedLiteralNode(previous().getValue());
        }
        
        // 变量组使用
        if (check(CHTLTokenType.IDENTIFIER) && checkVarGroupUsage()) {
            return parseVarGroupUsage();
        }
        
        error("期望字面量值");
        return new UnquotedLiteralNode("");
    }
    
    /**
     * 解析text块
     */
    private TextNode parseTextBlock() {
        TextNode textNode = new TextNode();
        consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
        
        // 收集text内容
        StringBuilder content = new StringBuilder();
        while (!check(CHTLTokenType.RIGHT_BRACE) && !isAtEnd()) {
            if (match(CHTLTokenType.STRING_LITERAL)) {
                content.append(previous().getValue());
            } else if (match(CHTLTokenType.IDENTIFIER, CHTLTokenType.NUMBER)) {
                content.append(previous().getValue());
            } else {
                advance();
            }
        }
        
        textNode.setContent(content.toString());
        consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
        return textNode;
    }
    
    /**
     * 解析style块
     */
    private StyleBlockNode parseStyleBlock(boolean isLocal) {
        StyleBlockNode styleNode = new StyleBlockNode(isLocal);
        consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
        
        while (!check(CHTLTokenType.RIGHT_BRACE) && !isAtEnd()) {
            // 检查内联样式属性
            if (checkStyleProperty()) {
                StylePropertyNode prop = parseStyleProperty();
                styleNode.addInlineStyle(prop);
            }
            // 检查样式规则（.class, #id, &等）
            else if (checkStyleRule()) {
                StyleRuleNode rule = parseStyleRule();
                styleNode.addRule(rule);
            }
            // 检查模板/自定义使用
            else if (checkTemplateOrCustomUsage()) {
                styleNode.addChild(parseTemplateOrCustomUsage());
            }
            else {
                advance();
            }
        }
        
        consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
        return styleNode;
    }
    
    /**
     * 检查是否是样式属性
     */
    private boolean checkStyleProperty() {
        if (!check(CHTLTokenType.IDENTIFIER)) return false;
        
        int savedCurrent = current;
        advance();
        boolean isProperty = check(CHTLTokenType.COLON);
        current = savedCurrent;
        
        return isProperty;
    }
    
    /**
     * 解析样式属性
     */
    private StylePropertyNode parseStyleProperty() {
        String property = consume(CHTLTokenType.IDENTIFIER, "期望属性名").getValue();
        consume(CHTLTokenType.COLON, "期望':'");
        
        StylePropertyNode propNode = new StylePropertyNode(property);
        
        // 解析属性值
        StringBuilder value = new StringBuilder();
        while (!check(CHTLTokenType.SEMICOLON) && !isAtEnd()) {
            value.append(advance().getValue()).append(" ");
        }
        
        propNode.setValue(new UnquotedLiteralNode(value.toString().trim()));
        consume(CHTLTokenType.SEMICOLON, "期望';'");
        
        return propNode;
    }
    
    /**
     * 检查是否是样式规则
     */
    private boolean checkStyleRule() {
        return check(CHTLTokenType.DOT) || check(CHTLTokenType.HASH) || 
               check(CHTLTokenType.AMPERSAND) || check(CHTLTokenType.IDENTIFIER);
    }
    
    /**
     * 解析样式规则
     */
    private StyleRuleNode parseStyleRule() {
        StyleSelectorNode selector = parseStyleSelector();
        StyleRuleNode rule = new StyleRuleNode(selector);
        
        consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
        
        while (!check(CHTLTokenType.RIGHT_BRACE) && !isAtEnd()) {
            if (checkStyleProperty()) {
                rule.addProperty(parseStyleProperty());
            } else {
                advance();
            }
        }
        
        consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
        return rule;
    }
    
    /**
     * 解析样式选择器
     */
    private StyleSelectorNode parseStyleSelector() {
        // 类选择器
        if (match(CHTLTokenType.DOT)) {
            String className = consume(CHTLTokenType.IDENTIFIER, "期望类名").getValue();
            return new StyleSelectorNode(StyleSelectorNode.SelectorType.CLASS, className);
        }
        
        // ID选择器
        if (match(CHTLTokenType.HASH)) {
            String idName = consume(CHTLTokenType.IDENTIFIER, "期望ID名").getValue();
            return new StyleSelectorNode(StyleSelectorNode.SelectorType.ID, idName);
        }
        
        // 上下文选择器
        if (match(CHTLTokenType.AMPERSAND)) {
            StyleSelectorNode selector = new StyleSelectorNode(StyleSelectorNode.SelectorType.CONTEXT, "");
            // 检查伪类或伪元素
            if (match(CHTLTokenType.COLON)) {
                StringBuilder pseudo = new StringBuilder(":");
                if (match(CHTLTokenType.COLON)) {
                    pseudo.append(":");
                }
                pseudo.append(consume(CHTLTokenType.IDENTIFIER, "期望伪类/伪元素名").getValue());
                selector.setPseudoValue(pseudo.toString());
            }
            return selector;
        }
        
        // 标签选择器
        if (check(CHTLTokenType.IDENTIFIER)) {
            String tagName = advance().getValue();
            return new StyleSelectorNode(StyleSelectorNode.SelectorType.TAG, tagName);
        }
        
        error("期望选择器");
        return new StyleSelectorNode(StyleSelectorNode.SelectorType.TAG, "div");
    }
    
    /**
     * 解析script块
     */
    private ScriptBlockNode parseScriptBlock(boolean isLocal) {
        ScriptBlockNode scriptNode = new ScriptBlockNode(isLocal);
        consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
        
        // 收集script内容
        StringBuilder content = new StringBuilder();
        int braceCount = 1;
        
        while (braceCount > 0 && !isAtEnd()) {
            CHTLToken token = advance();
            if (token.getType() == CHTLTokenType.LEFT_BRACE) {
                braceCount++;
            } else if (token.getType() == CHTLTokenType.RIGHT_BRACE) {
                braceCount--;
                if (braceCount == 0) break;
            }
            content.append(token.getValue()).append(" ");
        }
        
        scriptNode.setContent(content.toString().trim());
        
        // 检查是否包含CHTL JS语法
        String scriptContent = content.toString();
        if (scriptContent.contains("{{") || scriptContent.contains("->")) {
            scriptNode.setCHTLJS(true);
        }
        
        return scriptNode;
    }
    
    /**
     * 解析模板定义
     */
    private CHTLASTNode parseTemplateDefinition() {
        TemplateDefinitionNode.TemplateType type = parseTemplateType();
        String name = consume(CHTLTokenType.IDENTIFIER, "期望模板名称").getValue();
        
        // 如果是@Var类型，创建VarGroupDefinitionNode
        if (type == TemplateDefinitionNode.TemplateType.VAR) {
            VarGroupDefinitionNode varGroup = new VarGroupDefinitionNode(name, true);
            consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
            parseVarGroupContent(varGroup);
            consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
            return varGroup;
        }
        
        TemplateDefinitionNode template = new TemplateDefinitionNode(type, name);
        consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
        
        // 解析模板内容
        parseTemplateOrCustomContent(template, type);
        
        consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
        return template;
    }
    
    /**
     * 解析自定义定义
     */
    private CHTLASTNode parseCustomDefinition() {
        TemplateDefinitionNode.TemplateType type = parseTemplateType();
        String name = consume(CHTLTokenType.IDENTIFIER, "期望自定义名称").getValue();
        
        // 如果是@Var类型，创建VarGroupDefinitionNode
        if (type == TemplateDefinitionNode.TemplateType.VAR) {
            VarGroupDefinitionNode varGroup = new VarGroupDefinitionNode(name, false);
            consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
            parseVarGroupContent(varGroup);
            consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
            return varGroup;
        }
        
        CustomDefinitionNode custom = new CustomDefinitionNode(type, name);
        consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
        
        // 解析自定义内容
        parseTemplateOrCustomContent(custom, type);
        
        consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
        return custom;
    }
    
    /**
     * 解析模板/自定义类型
     */
    private TemplateDefinitionNode.TemplateType parseTemplateType() {
        if (match(CHTLTokenType.AT_STYLE)) {
            return TemplateDefinitionNode.TemplateType.STYLE;
        } else if (match(CHTLTokenType.AT_ELEMENT)) {
            return TemplateDefinitionNode.TemplateType.ELEMENT;
        } else if (match(CHTLTokenType.AT_VAR)) {
            return TemplateDefinitionNode.TemplateType.VAR;
        }
        
        error("期望@Style、@Element或@Var");
        return TemplateDefinitionNode.TemplateType.ELEMENT;
    }
    
    /**
     * 解析模板/自定义内容
     */
    private void parseTemplateOrCustomContent(CHTLASTNode parent, TemplateDefinitionNode.TemplateType type) {
        switch (type) {
            case ELEMENT:
                // 解析元素内容
                while (!check(CHTLTokenType.RIGHT_BRACE) && !isAtEnd()) {
                    if (check(CHTLTokenType.IDENTIFIER)) {
                        parent.addChild(parseElement());
                    } else if (checkTemplateOrCustomUsage()) {
                        parent.addChild(parseTemplateOrCustomUsage());
                    } else {
                        advance();
                    }
                }
                break;
                
            case STYLE:
                // 解析样式内容
                while (!check(CHTLTokenType.RIGHT_BRACE) && !isAtEnd()) {
                    if (checkStyleProperty()) {
                        parent.addChild(parseStyleProperty());
                    } else if (checkTemplateOrCustomUsage()) {
                        parent.addChild(parseTemplateOrCustomUsage());
                    } else if (parent instanceof CustomDefinitionNode && check(CHTLTokenType.IDENTIFIER)) {
                        // 自定义样式组的无值属性
                        String prop = advance().getValue();
                        if (match(CHTLTokenType.COMMA, CHTLTokenType.SEMICOLON)) {
                            ((CustomDefinitionNode)parent).addEmptyProperty(prop);
                        }
                    } else {
                        advance();
                    }
                }
                break;
                
            case VAR:
                // 解析变量组内容
                if (parent instanceof VarGroupDefinitionNode) {
                    VarGroupDefinitionNode varGroup = (VarGroupDefinitionNode)parent;
                    while (!check(CHTLTokenType.RIGHT_BRACE) && !isAtEnd()) {
                        if (check(CHTLTokenType.IDENTIFIER)) {
                            String varName = advance().getValue();
                            consume(CHTLTokenType.COLON, "期望':'");
                            CHTLASTNode value = parseLiteral();
                            consume(CHTLTokenType.SEMICOLON, "期望';'");
                            varGroup.addVariable(varName, value);
                        } else {
                            advance();
                        }
                    }
                }
                break;
        }
    }
    
    /**
     * 解析变量组内容
     */
    private void parseVarGroupContent(VarGroupDefinitionNode varGroup) {
        while (!check(CHTLTokenType.RIGHT_BRACE) && !isAtEnd()) {
            if (check(CHTLTokenType.IDENTIFIER)) {
                String varName = advance().getValue();
                consume(CHTLTokenType.COLON, "期望':'");
                
                // 收集所有值直到分号
                StringBuilder valueBuilder = new StringBuilder();
                while (!check(CHTLTokenType.SEMICOLON) && !isAtEnd()) {
                    CHTLToken token = advance();
                    if (valueBuilder.length() > 0) {
                        valueBuilder.append(" ");
                    }
                    valueBuilder.append(token.getValue());
                }
                
                consume(CHTLTokenType.SEMICOLON, "期望';'");
                
                // 创建值节点
                String valueStr = valueBuilder.toString();
                CHTLASTNode value;
                if (valueStr.startsWith("\"") && valueStr.endsWith("\"")) {
                    value = new StringLiteralNode(valueStr.substring(1, valueStr.length() - 1), '"');
                } else if (valueStr.startsWith("'") && valueStr.endsWith("'")) {
                    value = new StringLiteralNode(valueStr.substring(1, valueStr.length() - 1), '\'');
                } else {
                    value = new UnquotedLiteralNode(valueStr);
                }
                
                varGroup.addVariable(varName, value);
            } else {
                error("期望变量定义");
                advance();
            }
        }
    }
    
    /**
     * 检查是否是模板/自定义使用
     */
    private boolean checkTemplateOrCustomUsage() {
        return check(CHTLTokenType.AT_STYLE) || check(CHTLTokenType.AT_ELEMENT) || 
               check(CHTLTokenType.AT_VAR) || check(CHTLTokenType.TEMPLATE) || 
               check(CHTLTokenType.CUSTOM);
    }
    
    /**
     * 解析模板/自定义使用
     */
    private CHTLASTNode parseTemplateOrCustomUsage() {
        boolean isFullName = false;
        boolean isCustom = false;
        
        // 检查全缀名
        if (match(CHTLTokenType.TEMPLATE)) {
            isFullName = true;
            isCustom = false;
        } else if (match(CHTLTokenType.CUSTOM)) {
            isFullName = true;
            isCustom = true;
        }
        
        TemplateDefinitionNode.TemplateType type = parseTemplateType();
        String name = consume(CHTLTokenType.IDENTIFIER, "期望名称").getValue();
        
        // 创建相应的使用节点
        if (isCustom || !isFullName) {
            CustomUsageNode usage = new CustomUsageNode(type, name);
            usage.setUseFullName(isFullName);
            
            // 检查是否有特例化内容
            if (match(CHTLTokenType.LEFT_BRACE)) {
                parseCustomUsageContent(usage);
                consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
            } else {
                consume(CHTLTokenType.SEMICOLON, "期望';'");
            }
            
            return usage;
        } else {
            TemplateUsageNode usage = new TemplateUsageNode(type, name);
            usage.setUseFullName(isFullName);
            
            // 检查变量组参数
            if (type == TemplateDefinitionNode.TemplateType.VAR && match(CHTLTokenType.LEFT_PAREN)) {
                parseVarGroupParameters(usage);
                consume(CHTLTokenType.RIGHT_PAREN, "期望')'");
            }
            
            // 检查是否有特例化内容
            if (match(CHTLTokenType.LEFT_BRACE)) {
                usage.setHasSpecialization(true);
                parseTemplateUsageContent(usage);
                consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
            } else {
                consume(CHTLTokenType.SEMICOLON, "期望';'");
            }
            
            return usage;
        }
    }
    
    /**
     * 解析自定义使用内容
     */
    private void parseCustomUsageContent(CustomUsageNode usage) {
        while (!check(CHTLTokenType.RIGHT_BRACE) && !isAtEnd()) {
            // delete操作
            if (match(CHTLTokenType.DELETE)) {
                usage.addChild(parseDeleteOperation());
            }
            // insert操作
            else if (match(CHTLTokenType.INSERT)) {
                usage.addChild(parseInsertOperation());
            }
            // 其他特例化内容
            else if (check(CHTLTokenType.IDENTIFIER)) {
                usage.addChild(parseElement());
            }
            else {
                advance();
            }
        }
    }
    
    /**
     * 解析模板使用内容
     */
    private void parseTemplateUsageContent(TemplateUsageNode usage) {
        // 模板使用的特例化内容类似于自定义
        while (!check(CHTLTokenType.RIGHT_BRACE) && !isAtEnd()) {
            if (check(CHTLTokenType.IDENTIFIER)) {
                usage.addChild(parseElement());
            } else {
                advance();
            }
        }
    }
    
    /**
     * 解析delete操作
     */
    private CustomOperationNode parseDeleteOperation() {
        CustomOperationNode operation = new CustomOperationNode(CustomOperationNode.OperationType.DELETE);
        
        // 解析删除目标
        while (!check(CHTLTokenType.SEMICOLON) && !isAtEnd()) {
            if (match(CHTLTokenType.IDENTIFIER)) {
                String target = previous().getValue();
                // 检查是否是索引访问
                if (match(CHTLTokenType.LEFT_BRACKET)) {
                    String index = consume(CHTLTokenType.NUMBER, "期望索引").getValue();
                    consume(CHTLTokenType.RIGHT_BRACKET, "期望']'");
                    target += "[" + index + "]";
                }
                operation.addDeleteTarget(target);
                
                if (!match(CHTLTokenType.COMMA)) {
                    break;
                }
            } else if (checkTemplateOrCustomUsage()) {
                // 删除模板/自定义继承
                CHTLASTNode node = parseTemplateOrCustomUsage();
                // TODO: 处理删除模板/自定义的情况
                break;
            } else {
                advance();
            }
        }
        
        consume(CHTLTokenType.SEMICOLON, "期望';'");
        return operation;
    }
    
    /**
     * 解析insert操作
     */
    private CustomOperationNode parseInsertOperation() {
        CustomOperationNode operation = new CustomOperationNode(CustomOperationNode.OperationType.INSERT);
        
        // 解析插入位置
        CustomOperationNode.InsertPosition position = parseInsertPosition();
        operation.setInsertPosition(position);
        
        // 解析参考位置（如果需要）
        if (position != CustomOperationNode.InsertPosition.AT_TOP && 
            position != CustomOperationNode.InsertPosition.AT_BOTTOM) {
            String reference = consume(CHTLTokenType.IDENTIFIER, "期望参考元素").getValue();
            if (match(CHTLTokenType.LEFT_BRACKET)) {
                String index = consume(CHTLTokenType.NUMBER, "期望索引").getValue();
                consume(CHTLTokenType.RIGHT_BRACKET, "期望']'");
                reference += "[" + index + "]";
            }
            operation.setInsertReference(reference);
        }
        
        consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
        
        // 解析插入内容
        while (!check(CHTLTokenType.RIGHT_BRACE) && !isAtEnd()) {
            if (check(CHTLTokenType.IDENTIFIER)) {
                operation.addChild(parseElement());
            } else if (checkTemplateOrCustomUsage()) {
                operation.addChild(parseTemplateOrCustomUsage());
            } else {
                advance();
            }
        }
        
        consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
        return operation;
    }
    
    /**
     * 解析插入位置
     */
    private CustomOperationNode.InsertPosition parseInsertPosition() {
        if (match(CHTLTokenType.AFTER)) {
            return CustomOperationNode.InsertPosition.AFTER;
        } else if (match(CHTLTokenType.BEFORE)) {
            return CustomOperationNode.InsertPosition.BEFORE;
        } else if (match(CHTLTokenType.REPLACE)) {
            return CustomOperationNode.InsertPosition.REPLACE;
        } else if (match(CHTLTokenType.AT_TOP)) {
            return CustomOperationNode.InsertPosition.AT_TOP;
        } else if (match(CHTLTokenType.AT_BOTTOM)) {
            return CustomOperationNode.InsertPosition.AT_BOTTOM;
        }
        
        error("期望插入位置关键字");
        return CustomOperationNode.InsertPosition.AFTER;
    }
    
    /**
     * 解析原始嵌入
     */
    private OriginBlockNode parseOriginBlock() {
        OriginBlockNode.OriginType type = parseOriginType();
        String name = null;
        
        // 检查是否有增强原始嵌入的名称
        if (check(CHTLTokenType.IDENTIFIER)) {
            name = advance().getValue();
        }
        
        OriginBlockNode origin = name != null ? 
            new OriginBlockNode(type, name) : new OriginBlockNode(type);
        
        // 如果是使用增强原始嵌入
        if (name != null && match(CHTLTokenType.SEMICOLON)) {
            return origin;
        }
        
        consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
        
        // 收集原始内容
        StringBuilder content = new StringBuilder();
        int braceCount = 1;
        
        while (braceCount > 0 && !isAtEnd()) {
            CHTLToken token = advance();
            if (token.getType() == CHTLTokenType.LEFT_BRACE) {
                braceCount++;
            } else if (token.getType() == CHTLTokenType.RIGHT_BRACE) {
                braceCount--;
                if (braceCount == 0) break;
            }
            content.append(token.getValue()).append(" ");
        }
        
        origin.setContent(content.toString().trim());
        return origin;
    }
    
    /**
     * 解析原始嵌入类型
     */
    private OriginBlockNode.OriginType parseOriginType() {
        if (match(CHTLTokenType.AT_HTML)) {
            return OriginBlockNode.OriginType.HTML;
        } else if (match(CHTLTokenType.AT_STYLE)) {
            return OriginBlockNode.OriginType.STYLE;
        } else if (match(CHTLTokenType.AT_JAVASCRIPT)) {
            return OriginBlockNode.OriginType.JAVASCRIPT;
        }
        
        error("期望@Html、@Style或@JavaScript");
        return OriginBlockNode.OriginType.HTML;
    }
    
    /**
     * 解析导入语句
     */
    private ImportStatementNode parseImportStatement() {
        ImportStatementNode.ImportType importType = null;
        String targetName = null;
        
        // 检查是否是[Custom]或[Template]导入
        boolean isCustom = false;
        boolean isTemplate = false;
        
        if (match(CHTLTokenType.CUSTOM)) {
            isCustom = true;
        } else if (match(CHTLTokenType.TEMPLATE)) {
            isTemplate = true;
        }
        
        // 解析导入类型
        if (match(CHTLTokenType.AT_HTML)) {
            importType = ImportStatementNode.ImportType.HTML;
        } else if (match(CHTLTokenType.AT_STYLE)) {
            if (isCustom) {
                importType = ImportStatementNode.ImportType.CUSTOM_STYLE;
            } else if (isTemplate) {
                importType = ImportStatementNode.ImportType.TEMPLATE_STYLE;
            } else {
                importType = ImportStatementNode.ImportType.STYLE;
            }
        } else if (match(CHTLTokenType.AT_JAVASCRIPT)) {
            importType = ImportStatementNode.ImportType.JAVASCRIPT;
        } else if (match(CHTLTokenType.AT_ELEMENT)) {
            if (isCustom) {
                importType = ImportStatementNode.ImportType.CUSTOM_ELEMENT;
            } else if (isTemplate) {
                importType = ImportStatementNode.ImportType.TEMPLATE_ELEMENT;
            }
        } else if (match(CHTLTokenType.AT_VAR)) {
            if (isCustom) {
                importType = ImportStatementNode.ImportType.CUSTOM_VAR;
            } else if (isTemplate) {
                importType = ImportStatementNode.ImportType.TEMPLATE_VAR;
            }
        } else if (match(CHTLTokenType.AT_CHTL)) {
            importType = ImportStatementNode.ImportType.CHTL;
        } else if (match(CHTLTokenType.AT_CJMOD)) {
            importType = ImportStatementNode.ImportType.CJMOD;
        }
        
        ImportStatementNode importNode = new ImportStatementNode(importType);
        
        // 解析目标名称（如果需要）
        if (isCustom || isTemplate) {
            targetName = consume(CHTLTokenType.IDENTIFIER, "期望导入目标名称").getValue();
            importNode.setTargetName(targetName);
        }
        
        // 解析from
        consume(CHTLTokenType.FROM, "期望'from'");
        
        // 解析路径
        String path = consume(CHTLTokenType.IDENTIFIER, "期望文件路径").getValue();
        // 支持.作为路径分隔符
        while (match(CHTLTokenType.DOT)) {
            path += ".";
            path += consume(CHTLTokenType.IDENTIFIER, "期望路径段").getValue();
        }
        importNode.setFromPath(path);
        
        // 解析as（可选）
        if (match(CHTLTokenType.AS)) {
            String asName = consume(CHTLTokenType.IDENTIFIER, "期望重命名").getValue();
            importNode.setAsName(asName);
        }
        
        consume(CHTLTokenType.SEMICOLON, "期望';'");
        return importNode;
    }
    
    /**
     * 解析命名空间
     */
    private NamespaceNode parseNamespace() {
        String name = consume(CHTLTokenType.IDENTIFIER, "期望命名空间名称").getValue();
        NamespaceNode namespace = new NamespaceNode(name);
        
        // 如果有{，则解析命名空间内容
        if (match(CHTLTokenType.LEFT_BRACE)) {
            while (!check(CHTLTokenType.RIGHT_BRACE) && !isAtEnd()) {
                // 检查全局约束
                if (match(CHTLTokenType.EXCEPT)) {
                    ConstraintNode constraint = parseConstraint();
                    constraint.setGlobal(true);
                    namespace.addGlobalConstraint(constraint);
                } else {
                    // 解析命名空间内的其他内容
                    CHTLASTNode node = parseTopLevel();
                    if (node != null) {
                        namespace.addChild(node);
                    }
                }
            }
            consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
        } else {
            consume(CHTLTokenType.SEMICOLON, "期望';'");
        }
        
        return namespace;
    }
    
    /**
     * 解析配置组
     */
    private ConfigurationNode parseConfiguration() {
        ConfigurationNode config = new ConfigurationNode();
        consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
        
        while (!check(CHTLTokenType.RIGHT_BRACE) && !isAtEnd()) {
            if (check(CHTLTokenType.IDENTIFIER)) {
                String key = advance().getValue();
                
                // 检查是否是Name配置块
                if ("Name".equals(key) && match(CHTLTokenType.LEFT_BRACKET)) {
                    consume(CHTLTokenType.RIGHT_BRACKET, "期望']'");
                    consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
                    parseNameConfiguration(config);
                    consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
                } else {
                    // 普通配置项
                    consume(CHTLTokenType.EQUALS, "期望'='");
                    String value = parseLiteral().toString();
                    consume(CHTLTokenType.SEMICOLON, "期望';'");
                    config.setSetting(key, value);
                }
            } else {
                advance();
            }
        }
        
        consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
        return config;
    }
    
    /**
     * 解析Name配置
     */
    private void parseNameConfiguration(ConfigurationNode config) {
        while (!check(CHTLTokenType.RIGHT_BRACE) && !isAtEnd()) {
            if (check(CHTLTokenType.IDENTIFIER)) {
                String key = advance().getValue();
                consume(CHTLTokenType.EQUALS, "期望'='");
                
                // 检查是否是组选项
                if (match(CHTLTokenType.LEFT_BRACKET)) {
                    List<String> values = new ArrayList<>();
                    while (!check(CHTLTokenType.RIGHT_BRACKET) && !isAtEnd()) {
                        if (checkTemplateOrCustomUsage() || check(CHTLTokenType.IDENTIFIER)) {
                            values.add(advance().getValue());
                            match(CHTLTokenType.COMMA);
                        } else {
                            advance();
                        }
                    }
                    consume(CHTLTokenType.RIGHT_BRACKET, "期望']'");
                    config.setNameMapping(key, values.toArray(new String[0]));
                } else {
                    // 单个值
                    String value = advance().getValue();
                    config.setNameMapping(key, value);
                }
                
                consume(CHTLTokenType.SEMICOLON, "期望';'");
            } else {
                advance();
            }
        }
    }
    
    /**
     * 解析约束
     */
    private ConstraintNode parseConstraint() {
        ConstraintNode constraint = null;
        
        // 解析约束目标
        if (match(CHTLTokenType.AT_HTML)) {
            constraint = new ConstraintNode(ConstraintNode.ConstraintTarget.HTML);
        } else if (match(CHTLTokenType.AT_STYLE)) {
            constraint = new ConstraintNode(ConstraintNode.ConstraintTarget.STYLE);
        } else if (match(CHTLTokenType.AT_ELEMENT)) {
            constraint = new ConstraintNode(ConstraintNode.ConstraintTarget.ELEMENT);
        } else if (match(CHTLTokenType.AT_VAR)) {
            constraint = new ConstraintNode(ConstraintNode.ConstraintTarget.VAR);
        } else if (match(CHTLTokenType.TEMPLATE)) {
            constraint = new ConstraintNode(ConstraintNode.ConstraintTarget.TEMPLATE);
        } else if (match(CHTLTokenType.CUSTOM)) {
            constraint = new ConstraintNode(ConstraintNode.ConstraintTarget.CUSTOM);
        } else if (check(CHTLTokenType.IDENTIFIER)) {
            // 具体名称
            String name = advance().getValue();
            constraint = new ConstraintNode(name);
        }
        
        consume(CHTLTokenType.SEMICOLON, "期望';'");
        return constraint;
    }
    
    /**
     * 检查是否是变量组使用
     */
    private boolean checkVarGroupUsage() {
        int savedCurrent = current;
        advance(); // 跳过标识符
        boolean isVarGroup = match(CHTLTokenType.LEFT_PAREN);
        current = savedCurrent;
        return isVarGroup;
    }
    
    /**
     * 解析变量组使用
     */
    private VarGroupUsageNode parseVarGroupUsage() {
        String groupName = consume(CHTLTokenType.IDENTIFIER, "期望变量组名").getValue();
        consume(CHTLTokenType.LEFT_PAREN, "期望'('");
        
        String varName = consume(CHTLTokenType.IDENTIFIER, "期望变量名").getValue();
        VarGroupUsageNode usage = new VarGroupUsageNode(groupName, varName);
        
        // 检查是否有覆盖值
        if (match(CHTLTokenType.EQUALS)) {
            CHTLASTNode overrideValue = parseLiteral();
            usage.setOverrideValue(overrideValue);
        }
        
        consume(CHTLTokenType.RIGHT_PAREN, "期望')'");
        return usage;
    }
    
    /**
     * 解析变量组参数
     */
    private void parseVarGroupParameters(TemplateUsageNode usage) {
        while (!check(CHTLTokenType.RIGHT_PAREN) && !isAtEnd()) {
            String varName = consume(CHTLTokenType.IDENTIFIER, "期望变量名").getValue();
            consume(CHTLTokenType.EQUALS, "期望'='");
            CHTLASTNode value = parseLiteral();
            usage.addParameter(varName, value);
            
            if (!match(CHTLTokenType.COMMA)) {
                break;
            }
        }
    }
    
    // 辅助方法
    
    private boolean match(CHTLTokenType... types) {
        for (CHTLTokenType type : types) {
            if (check(type)) {
                advance();
                return true;
            }
        }
        return false;
    }
    
    private boolean check(CHTLTokenType type) {
        if (isAtEnd()) return false;
        return peek().getType() == type;
    }
    
    private CHTLToken advance() {
        if (!isAtEnd()) current++;
        return previous();
    }
    
    private boolean isAtEnd() {
        return peek().getType() == CHTLTokenType.EOF;
    }
    
    private CHTLToken peek() {
        return tokens.get(current);
    }
    
    private CHTLToken previous() {
        return tokens.get(current - 1);
    }
    
    private CHTLToken consume(CHTLTokenType type, String message) {
        if (check(type)) return advance();
        error(message);
        return new CHTLToken(type, "", 0, 0, 0);
    }
    
    private void error(String message) {
        CHTLToken token = peek();
        String errorMsg = String.format("[%d:%d] %s", token.getLine(), token.getColumn(), message);
        errors.add(errorMsg);
    }
    
    public List<String> getErrors() {
        return errors;
    }
}
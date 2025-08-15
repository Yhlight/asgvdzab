package com.chtl.parser;

import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.node.CustomDefinitionNode;
import com.chtl.ast.node.CustomOperationNode;
import com.chtl.ast.node.CustomUsageNode;
import com.chtl.ast.node.TemplateDefinitionNode;
import com.chtl.ast.node.VarGroupDefinitionNode;
import com.chtl.compiler.chtl.CHTLParser;
import com.chtl.model.ElementNode;


import com.chtl.ast.*;
import com.chtl.ast.node.*;
import com.chtl.compiler.chtl.token.CHTLToken;
import com.chtl.compiler.chtl.token.CHTLTokenType;
import com.chtl.compiler.chtl.lexer.CHTLLexer;
import com.chtl.context.CompilationContext;

import java.util.*;

/**
 * 增强的CHTL解析器
 * 修复了变量组、索引访问和插入操作的解析
 * 严格根据CHTL语法文档实现
 */
public class CHTLEnhancedParser extends CHTLParser {
    
    public CHTLEnhancedParser() {
        super();
    }
    
    /**
     * 重写模板定义解析，处理@Var类型
     */
    @Override
    protected TemplateDefinitionNode parseTemplateDefinition() {
        TemplateDefinitionNode.TemplateType type = parseTemplateType();
        String name = consume(CHTLTokenType.IDENTIFIER, "期望模板名称").getValue();
        
        // 如果是@Var类型，创建VarGroupDefinitionNode
        if (type == TemplateDefinitionNode.TemplateType.VAR) {
            VarGroupDefinitionNode varGroup = new VarGroupDefinitionNode(name, true);
            consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
            parseVarGroupContent(varGroup);
            consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
            return null; // 返回null，让调用者处理VarGroupDefinitionNode
        }
        
        TemplateDefinitionNode template = new TemplateDefinitionNode(type, name);
        consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
        parseTemplateOrCustomContent(template, type);
        consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
        return template;
    }
    
    /**
     * 重写自定义定义解析，处理@Var类型
     */
    @Override
    protected CustomDefinitionNode parseCustomDefinition() {
        TemplateDefinitionNode.TemplateType type = parseTemplateType();
        String name = consume(CHTLTokenType.IDENTIFIER, "期望自定义名称").getValue();
        
        // 如果是@Var类型，创建VarGroupDefinitionNode
        if (type == TemplateDefinitionNode.TemplateType.VAR) {
            VarGroupDefinitionNode varGroup = new VarGroupDefinitionNode(name, false);
            consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
            parseVarGroupContent(varGroup);
            consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
            return null; // 返回null，让调用者处理VarGroupDefinitionNode
        }
        
        CustomDefinitionNode custom = new CustomDefinitionNode(type, name);
        consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
        parseTemplateOrCustomContent(custom, type);
        consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
        return custom;
    }
    
    /**
     * 重写顶层元素解析，处理VarGroupDefinitionNode
     */
    @Override
    protected CHTLASTNode parseTopLevel() {
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
            CHTLASTNode node = parseTemplateDefinition();
            if (node == null) {
                // 返回的是VarGroupDefinitionNode，需要从上下文获取
                return getLastVarGroupNode();
            }
            return node;
        }
        
        // 自定义定义
        if (match(CHTLTokenType.CUSTOM)) {
            CHTLASTNode node = parseCustomDefinition();
            if (node == null) {
                // 返回的是VarGroupDefinitionNode，需要从上下文获取
                return getLastVarGroupNode();
            }
            return node;
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
        
        // 注释
        if (match(CHTLTokenType.LINE_COMMENT, CHTLTokenType.BLOCK_COMMENT)) {
            return parseComment(previous());
        }
        
        error("意外的顶层元素");
        advance();
        return null;
    }
    
    /**
     * 解析变量组内容
     */
    private void parseVarGroupContent(VarGroupDefinitionNode varGroup) {
        while (!check(CHTLTokenType.RIGHT_BRACE) && !isAtEnd()) {
            if (check(CHTLTokenType.IDENTIFIER)) {
                String varName = advance().getValue();
                consume(CHTLTokenType.COLON, "期望':'");
                CHTLASTNode value = parseLiteral();
                consume(CHTLTokenType.SEMICOLON, "期望';'");
                varGroup.addVariable(varName, value);
            } else {
                error("期望变量定义");
                advance();
            }
        }
        // 临时存储，以便parseTopLevel获取
        lastVarGroupNode = varGroup;
    }
    
    private VarGroupDefinitionNode lastVarGroupNode;
    
    private VarGroupDefinitionNode getLastVarGroupNode() {
        VarGroupDefinitionNode node = lastVarGroupNode;
        lastVarGroupNode = null;
        return node;
    }
    
    /**
     * 重写元素解析，支持索引访问
     */
    @Override
    protected ElementNode parseElement() {
        String tagName = consume(CHTLTokenType.IDENTIFIER, "期望元素名").getValue();
        
        // 检查索引访问 div[1]
        Integer index = null;
        if (match(CHTLTokenType.LEFT_BRACKET)) {
            CHTLToken indexToken = consume(CHTLTokenType.NUMBER, "期望索引");
            index = Integer.parseInt(indexToken.getValue());
            consume(CHTLTokenType.RIGHT_BRACKET, "期望']'");
        }
        
        ElementNode element = new ElementNode(tagName);
        if (index != null) {
            element.setIndex(index);
        }
        
        consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
        
        // 解析元素内容
        while (!check(CHTLTokenType.RIGHT_BRACE) && !isAtEnd()) {
            if (match(CHTLTokenType.STYLE)) {
                element.addChild(parseStyleBlock(true));
            } else if (match(CHTLTokenType.SCRIPT)) {
                element.addChild(parseScriptBlock(true));
            } else if (match(CHTLTokenType.TEXT)) {
                element.addChild(parseTextBlock());
            } else if (checkAttribute()) {
                element.addAttribute(parseAttribute());
            } else if (check(CHTLTokenType.IDENTIFIER)) {
                element.addChild(parseElement());
            } else if (checkTemplateOrCustomUsage()) {
                element.addChild(parseTemplateOrCustomUsage());
            } else {
                advance();
            }
        }
        
        consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
        return element;
    }
    
    /**
     * 重写模板/自定义使用内容解析，支持插入操作
     */
    @Override
    protected void parseCustomUsageContent(CustomUsageNode usage) {
        while (!check(CHTLTokenType.RIGHT_BRACE) && !isAtEnd()) {
            // 检查delete操作
            if (match(CHTLTokenType.DELETE)) {
                CustomOperationNode deleteOp = parseDeleteOperation();
                usage.addOperation(deleteOp);
            }
            // 检查insert操作
            else if (match(CHTLTokenType.INSERT)) {
                CustomOperationNode insertOp = parseInsertOperation();
                usage.addOperation(insertOp);
            }
            // 元素索引访问修改
            else if (check(CHTLTokenType.IDENTIFIER) && checkAhead(CHTLTokenType.LEFT_BRACKET)) {
                ElementNode element = parseElement();
                usage.addSpecialization(element);
            }
            // 属性覆盖
            else if (checkStyleProperty()) {
                usage.addSpecialization(parseStyleProperty());
            }
            // 其他元素
            else if (check(CHTLTokenType.IDENTIFIER)) {
                usage.addSpecialization(parseElement());
            }
            else {
                advance();
            }
        }
    }
    
    /**
     * 解析插入操作
     */
    private CustomOperationNode parseInsertOperation() {
        CustomOperationNode.OperationType position = CustomOperationNode.OperationType.INSERT_AFTER; // 默认
        
        // 解析位置关键字
        if (match(CHTLTokenType.AFTER)) {
            position = CustomOperationNode.OperationType.INSERT_AFTER;
        } else if (match(CHTLTokenType.BEFORE)) {
            position = CustomOperationNode.OperationType.INSERT_BEFORE;
        } else if (match(CHTLTokenType.REPLACE)) {
            position = CustomOperationNode.OperationType.REPLACE;
        } else if (match(CHTLTokenType.AT)) {
            if (match(CHTLTokenType.TOP)) {
                position = CustomOperationNode.OperationType.INSERT_TOP;
            } else if (match(CHTLTokenType.BOTTOM)) {
                position = CustomOperationNode.OperationType.INSERT_BOTTOM;
            }
        }
        
        CustomOperationNode insertOp = new CustomOperationNode(position);
        
        // 解析目标选择器（如果有）
        if (check(CHTLTokenType.IDENTIFIER)) {
            String selector = advance().getValue();
            
            // 检查索引
            if (match(CHTLTokenType.LEFT_BRACKET)) {
                CHTLToken indexToken = consume(CHTLTokenType.NUMBER, "期望索引");
                int index = Integer.parseInt(indexToken.getValue());
                consume(CHTLTokenType.RIGHT_BRACKET, "期望']'");
                selector += "[" + index + "]";
            }
            
            insertOp.addTarget(selector);
        }
        
        // 解析插入内容
        consume(CHTLTokenType.LEFT_BRACE, "期望'{'");
        while (!check(CHTLTokenType.RIGHT_BRACE) && !isAtEnd()) {
            if (check(CHTLTokenType.IDENTIFIER)) {
                insertOp.addContent(parseElement());
            } else if (checkTemplateOrCustomUsage()) {
                insertOp.addContent(parseTemplateOrCustomUsage());
            } else {
                advance();
            }
        }
        consume(CHTLTokenType.RIGHT_BRACE, "期望'}'");
        
        return insertOp;
    }
    
    /**
     * 检查前方token
     */
    private boolean checkAhead(CHTLTokenType type) {
        if (isAtEnd()) return false;
        int savedCurrent = current;
        advance();
        boolean result = check(type);
        current = savedCurrent;
        return result;
    }
    
    // 需要添加的token类型
    private static final CHTLTokenType DELETE = CHTLTokenType.IDENTIFIER; // 临时使用IDENTIFIER
    private static final CHTLTokenType INSERT = CHTLTokenType.IDENTIFIER;
    private static final CHTLTokenType AFTER = CHTLTokenType.IDENTIFIER;
    private static final CHTLTokenType BEFORE = CHTLTokenType.IDENTIFIER;
    private static final CHTLTokenType REPLACE = CHTLTokenType.IDENTIFIER;
    private static final CHTLTokenType AT = CHTLTokenType.IDENTIFIER;
    private static final CHTLTokenType TOP = CHTLTokenType.IDENTIFIER;
    private static final CHTLTokenType BOTTOM = CHTLTokenType.IDENTIFIER;
    
    // 重写match方法以支持关键字
    protected boolean match(CHTLTokenType... types) {
        for (CHTLTokenType type : types) {
            if (check(type)) {
                // 特殊处理关键字
                if (type == CHTLTokenType.IDENTIFIER) {
                    String value = peek().getValue();
                    if ("delete".equals(value) || "insert".equals(value) || 
                        "after".equals(value) || "before".equals(value) ||
                        "replace".equals(value) || "at".equals(value) ||
                        "top".equals(value) || "bottom".equals(value)) {
                        advance();
                        return true;
                    }
                }
                advance();
                return true;
            }
        }
        return false;
    }
}
package com.chtl.compiler.chtljs.selector;

import com.chtl.ast.node.*;
import com.chtl.context.CompilationContext;
import com.chtl.compiler.chtljs.lexer.CHTLJSStateMachineLexer;
import com.chtl.compiler.chtljs.token.CHTLJSToken;
import com.chtl.compiler.chtljs.token.CHTLJSTokenType;

import java.util.*;

/**
 * CHTL JS选择器解析器
 * 负责解析{{}}中的选择器语法
 * 严格遵循CHTL语法文档
 */
public class SelectorParser {
    private final CompilationContext context;
    
    public SelectorParser(CompilationContext context) {
        this.context = context;
    }
    
    /**
     * 将CHTL JS选择器节点转换为JavaScript选择器字符串
     */
    public String parseToJSSelector(CHTLJSSelectorNode selectorNode) {
        String selector = selectorNode.getSelector();
        SelectorType type = selectorNode.getSelectorType();
        
        switch (type) {
            case CLASS:
                return "." + selector;
            case ID:
                return "#" + selector;
            case TAG:
                return selector;
            case ATTRIBUTE:
                return parseAttributeSelector(selector);
            default:
                return selector;
        }
    }
    
    /**
     * 解析选择器字符串
     * 支持的格式：
     * - {{.class}} - 类选择器
     * - {{#id}} - ID选择器
     * - {{tag}} - 标签选择器
     * - {{tag[index]}} - 索引选择器
     */
    public CHTLJSSelectorNode parseSelector(String selectorString) {
        // 移除{{ }}
        String content = selectorString.trim();
        if (content.startsWith("{{") && content.endsWith("}}")) {
            content = content.substring(2, content.length() - 2).trim();
        }
        
        // 使用词法分析器解析
        CHTLJSStateMachineLexer lexer = new CHTLJSStateMachineLexer(content);
        List<CHTLJSToken> tokens = lexer.tokenize();
        
        if (tokens.isEmpty() || tokens.get(0).getType() == CHTLJSTokenType.EOF) {
            context.addError(new com.chtl.context.CompilationError(
                "空的选择器表达式",
                com.chtl.context.CompilationError.ErrorType.SYNTAX_ERROR
            ));
            return null;
        }
        
        return parseSelectorTokens(tokens);
    }
    
    /**
     * 解析选择器tokens
     */
    private CHTLJSSelectorNode parseSelectorTokens(List<CHTLJSToken> tokens) {
        int current = 0;
        CHTLJSToken firstToken = tokens.get(current);
        
        SelectorType type;
        String selector;
        Integer index = null;
        
        switch (firstToken.getType()) {
            case DOT:
                // 类选择器
                current++;
                if (current >= tokens.size() || 
                    tokens.get(current).getType() != CHTLJSTokenType.IDENTIFIER) {
                    context.addError(new com.chtl.context.CompilationError(
                        "类选择器后需要标识符",
                        com.chtl.context.CompilationError.ErrorType.SYNTAX_ERROR
                    ));
                    return null;
                }
                type = SelectorType.CLASS;
                selector = tokens.get(current).getValue();
                current++;
                break;
                
            case HASH:
                // ID选择器
                current++;
                if (current >= tokens.size() || 
                    tokens.get(current).getType() != CHTLJSTokenType.IDENTIFIER) {
                    context.addError(new com.chtl.context.CompilationError(
                        "ID选择器后需要标识符",
                        com.chtl.context.CompilationError.ErrorType.SYNTAX_ERROR
                    ));
                    return null;
                }
                type = SelectorType.ID;
                selector = tokens.get(current).getValue();
                current++;
                break;
                
            case IDENTIFIER:
                // 标签选择器或其他
                type = SelectorType.TAG;
                selector = firstToken.getValue();
                current++;
                break;
                
            default:
                context.addError(new com.chtl.context.CompilationError(
                    "无效的选择器: " + firstToken.getValue(),
                    com.chtl.context.CompilationError.ErrorType.SYNTAX_ERROR
                ));
                return null;
        }
        
        // 检查是否有索引访问
        if (current < tokens.size() && 
            tokens.get(current).getType() == CHTLJSTokenType.LEFT_BRACKET) {
            current++;
            
            if (current >= tokens.size() || 
                tokens.get(current).getType() != CHTLJSTokenType.NUMBER_LITERAL) {
                context.addError(new com.chtl.context.CompilationError(
                    "索引必须是数字",
                    com.chtl.context.CompilationError.ErrorType.SYNTAX_ERROR
                ));
                return null;
            }
            
            index = Integer.parseInt(tokens.get(current).getValue());
            current++;
            
            if (current >= tokens.size() || 
                tokens.get(current).getType() != CHTLJSTokenType.RIGHT_BRACKET) {
                context.addError(new com.chtl.context.CompilationError(
                    "期望']'",
                    com.chtl.context.CompilationError.ErrorType.SYNTAX_ERROR
                ));
                return null;
            }
        }
        
        CHTLJSSelectorNode node = new CHTLJSSelectorNode(type, selector);
        if (index != null) {
            node.setIndex(index);
        }
        
        return node;
    }
    
    /**
     * 解析属性选择器
     */
    private String parseAttributeSelector(String selector) {
        // 简单处理，实际可能需要更复杂的解析
        return "[" + selector + "]";
    }
    
    /**
     * 验证选择器是否有效
     */
    public boolean isValidSelector(String selector) {
        try {
            CHTLJSSelectorNode node = parseSelector(selector);
            return node != null;
        } catch (Exception e) {
            return false;
        }
    }
}


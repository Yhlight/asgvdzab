package com.chtl.parser;

import com.chtl.ast.chtljs.*;
import com.chtl.compiler.chtljs.token.CHTLJSToken;
import com.chtl.compiler.chtljs.token.CHTLJSTokenType;
import com.chtl.compiler.chtljs.lexer.CHTLJSLexer;

import java.util.*;

/**
 * CHTL JS解析器
 * 严格根据CHTL语法文档的CHTL JS部分实现
 */
public class CHTLJSParser {
    private List<CHTLJSToken> tokens;
    private int current;
    private List<String> errors;
    
    public CHTLJSParser() {
        this.errors = new ArrayList<>();
    }
    
    /**
     * 解析CHTL JS代码
     */
    public List<CHTLJSASTNode> parse(String source) {
        CHTLJSLexer lexer = new CHTLJSLexer(source);
        this.tokens = lexer.tokenize();
        this.current = 0;
        this.errors.clear();
        
        List<CHTLJSASTNode> statements = new ArrayList<>();
        
        while (!isAtEnd()) {
            CHTLJSASTNode statement = parseStatement();
            if (statement != null) {
                statements.add(statement);
            }
        }
        
        return statements;
    }
    
    /**
     * 解析语句
     */
    private CHTLJSASTNode parseStatement() {
        // 跳过注释
        if (match(CHTLJSTokenType.LINE_COMMENT, CHTLJSTokenType.BLOCK_COMMENT)) {
            return null;
        }
        
        // animate函数调用
        if (match(CHTLJSTokenType.ANIMATE)) {
            return parseAnimateCall();
        }
        
        // 增强选择器开始
        if (check(CHTLJSTokenType.DOUBLE_LEFT_BRACE)) {
            return parseEnhancedSelectorExpression();
        }
        
        // 普通JavaScript表达式
        return parseExpression();
    }
    
    /**
     * 解析增强选择器表达式
     */
    private CHTLJSASTNode parseEnhancedSelectorExpression() {
        EnhancedSelectorNode selector = parseEnhancedSelector();
        
        // 检查后续操作
        if (check(CHTLJSTokenType.ARROW) || check(CHTLJSTokenType.DOT)) {
            return parseChainedOperation(selector);
        }
        
        return selector;
    }
    
    /**
     * 解析增强选择器
     */
    private EnhancedSelectorNode parseEnhancedSelector() {
        consume(CHTLJSTokenType.DOUBLE_LEFT_BRACE, "期望'{{'");
        
        StringBuilder selectorContent = new StringBuilder();
        
        // 收集选择器内容
        while (!check(CHTLJSTokenType.DOUBLE_RIGHT_BRACE) && !isAtEnd()) {
            CHTLJSToken token = advance();
            selectorContent.append(token.getValue());
            
            // 处理索引访问
            if (token.getType() == CHTLJSTokenType.LEFT_BRACKET) {
                while (!check(CHTLJSTokenType.RIGHT_BRACKET) && !isAtEnd()) {
                    selectorContent.append(advance().getValue());
                }
                if (check(CHTLJSTokenType.RIGHT_BRACKET)) {
                    selectorContent.append(advance().getValue());
                }
            }
        }
        
        consume(CHTLJSTokenType.DOUBLE_RIGHT_BRACE, "期望'}}'");
        
        return new EnhancedSelectorNode(selectorContent.toString());
    }
    
    /**
     * 解析链式操作
     */
    private CHTLJSASTNode parseChainedOperation(CHTLJSASTNode left) {
        // 箭头操作
        if (match(CHTLJSTokenType.ARROW)) {
            return parseArrowOperation(left);
        }
        
        // 点操作（属性访问）
        if (match(CHTLJSTokenType.DOT)) {
            return parseDotOperation(left);
        }
        
        return left;
    }
    
    /**
     * 解析箭头操作
     */
    private CHTLJSASTNode parseArrowOperation(CHTLJSASTNode left) {
        // 检查是否是特殊方法调用
        if (check(CHTLJSTokenType.LISTEN)) {
            return parseListenCall(left);
        }
        
        if (check(CHTLJSTokenType.DELEGATE)) {
            return parseDelegateCall(left);
        }
        
        // 普通箭头操作（等价于.）
        ArrowOperationNode operation = new ArrowOperationNode();
        operation.setLeft(left);
        
        // 解析右侧
        if (check(CHTLJSTokenType.IDENTIFIER)) {
            String property = advance().getValue();
            PropertyAccessNode propAccess = new PropertyAccessNode(property);
            operation.setRight(propAccess);
            
            // 继续链式调用
            if (check(CHTLJSTokenType.ARROW) || check(CHTLJSTokenType.DOT)) {
                return parseChainedOperation(operation);
            }
        }
        
        return operation;
    }
    
    /**
     * 解析点操作
     */
    private CHTLJSASTNode parseDotOperation(CHTLJSASTNode left) {
        String property = consume(CHTLJSTokenType.IDENTIFIER, "期望属性名").getValue();
        PropertyAccessNode propAccess = new PropertyAccessNode(property);
        propAccess.setObject(left);
        
        // 继续链式调用
        if (check(CHTLJSTokenType.ARROW) || check(CHTLJSTokenType.DOT)) {
            return parseChainedOperation(propAccess);
        }
        
        return propAccess;
    }
    
    /**
     * 解析listen调用
     */
    private ListenCallNode parseListenCall(CHTLJSASTNode target) {
        consume(CHTLJSTokenType.LISTEN, "期望'listen'");
        consume(CHTLJSTokenType.LEFT_PAREN, "期望'('");
        consume(CHTLJSTokenType.LEFT_BRACE, "期望'{'");
        
        ListenCallNode listenCall = new ListenCallNode();
        listenCall.setTarget(target);
        
        // 解析事件处理器
        while (!check(CHTLJSTokenType.RIGHT_BRACE) && !isAtEnd()) {
            String eventName = consume(CHTLJSTokenType.IDENTIFIER, "期望事件名").getValue();
            consume(CHTLJSTokenType.COLON, "期望':'");
            
            EventHandlerNode handler = parseEventHandler(eventName);
            listenCall.addEventHandler(eventName, handler);
            
            if (!match(CHTLJSTokenType.COMMA)) {
                break;
            }
        }
        
        consume(CHTLJSTokenType.RIGHT_BRACE, "期望'}'");
        consume(CHTLJSTokenType.RIGHT_PAREN, "期望')'");
        
        return listenCall;
    }
    
    /**
     * 解析delegate调用
     */
    private DelegateCallNode parseDelegateCall(CHTLJSASTNode parent) {
        consume(CHTLJSTokenType.DELEGATE, "期望'delegate'");
        consume(CHTLJSTokenType.LEFT_PAREN, "期望'('");
        consume(CHTLJSTokenType.LEFT_BRACE, "期望'{'");
        
        DelegateCallNode delegateCall = new DelegateCallNode();
        delegateCall.setParentElement(parent);
        
        // 解析配置
        while (!check(CHTLJSTokenType.RIGHT_BRACE) && !isAtEnd()) {
            String key = consume(CHTLJSTokenType.IDENTIFIER, "期望配置项").getValue();
            consume(CHTLJSTokenType.COLON, "期望':'");
            
            if ("target".equals(key)) {
                // 解析目标元素
                if (check(CHTLJSTokenType.LEFT_BRACKET)) {
                    // 数组形式
                    consume(CHTLJSTokenType.LEFT_BRACKET, "期望'['");
                    while (!check(CHTLJSTokenType.RIGHT_BRACKET) && !isAtEnd()) {
                        if (check(CHTLJSTokenType.DOUBLE_LEFT_BRACE)) {
                            delegateCall.addTargetElement(parseEnhancedSelector());
                        }
                        match(CHTLJSTokenType.COMMA);
                    }
                    consume(CHTLJSTokenType.RIGHT_BRACKET, "期望']'");
                } else {
                    // 单个选择器
                    delegateCall.addTargetElement(parseEnhancedSelector());
                }
            } else {
                // 事件处理器
                EventHandlerNode handler = parseEventHandler(key);
                delegateCall.addEventHandler(key, handler);
            }
            
            if (!match(CHTLJSTokenType.COMMA)) {
                break;
            }
        }
        
        consume(CHTLJSTokenType.RIGHT_BRACE, "期望'}'");
        consume(CHTLJSTokenType.RIGHT_PAREN, "期望')'");
        
        return delegateCall;
    }
    
    /**
     * 解析事件处理器
     */
    private EventHandlerNode parseEventHandler(String eventName) {
        EventHandlerNode handler = new EventHandlerNode(eventName);
        
        // 箭头函数
        if (check(CHTLJSTokenType.LEFT_PAREN)) {
            handler.setHandlerType(EventHandlerNode.HandlerType.ARROW_FUNCTION);
            String code = parseArrowFunction();
            handler.setHandlerCode(code);
        }
        // function表达式
        else if (match(CHTLJSTokenType.FUNCTION)) {
            handler.setHandlerType(EventHandlerNode.HandlerType.FUNCTION_EXPRESSION);
            String code = parseFunctionExpression();
            handler.setHandlerCode(code);
        }
        // 函数引用
        else if (check(CHTLJSTokenType.IDENTIFIER)) {
            handler.setHandlerType(EventHandlerNode.HandlerType.FUNCTION_REFERENCE);
            handler.setHandlerCode(advance().getValue());
        }
        
        return handler;
    }
    
    /**
     * 解析箭头函数
     */
    private String parseArrowFunction() {
        StringBuilder code = new StringBuilder();
        
        // 参数列表
        consume(CHTLJSTokenType.LEFT_PAREN, "期望'('");
        while (!check(CHTLJSTokenType.RIGHT_PAREN) && !isAtEnd()) {
            code.append(advance().getValue()).append(" ");
        }
        consume(CHTLJSTokenType.RIGHT_PAREN, "期望')'");
        
        // =>
        code.append(" => ");
        advance(); // 跳过=
        advance(); // 跳过>
        
        // 函数体
        if (check(CHTLJSTokenType.LEFT_BRACE)) {
            code.append(parseBlock());
        } else {
            // 单表达式
            code.append(parseExpression());
        }
        
        return code.toString();
    }
    
    /**
     * 解析function表达式
     */
    private String parseFunctionExpression() {
        StringBuilder code = new StringBuilder("function");
        
        // 参数列表
        consume(CHTLJSTokenType.LEFT_PAREN, "期望'('");
        code.append("(");
        while (!check(CHTLJSTokenType.RIGHT_PAREN) && !isAtEnd()) {
            code.append(advance().getValue()).append(" ");
        }
        consume(CHTLJSTokenType.RIGHT_PAREN, "期望')'");
        code.append(") ");
        
        // 函数体
        code.append(parseBlock());
        
        return code.toString();
    }
    
    /**
     * 解析animate调用
     */
    private AnimateCallNode parseAnimateCall() {
        consume(CHTLJSTokenType.LEFT_PAREN, "期望'('");
        consume(CHTLJSTokenType.LEFT_BRACE, "期望'{'");
        
        AnimateCallNode animateCall = new AnimateCallNode();
        AnimationConfigNode config = new AnimationConfigNode();
        animateCall.setConfig(config);
        
        // 解析动画配置
        while (!check(CHTLJSTokenType.RIGHT_BRACE) && !isAtEnd()) {
            String key = consume(CHTLJSTokenType.IDENTIFIER, "期望配置项").getValue();
            consume(CHTLJSTokenType.COLON, "期望':'");
            
            switch (key) {
                case "duration":
                    config.setDuration(parseNumber());
                    break;
                    
                case "easing":
                    config.setEasing(parseString());
                    break;
                    
                case "begin":
                    parseAnimationState(config.getBeginState());
                    break;
                    
                case "end":
                    parseAnimationState(config.getEndState());
                    break;
                    
                case "when":
                    parseKeyframes(config);
                    break;
                    
                case "loop":
                    config.setLoop(parseNumber());
                    break;
                    
                case "direction":
                    config.setDirection(parseString());
                    break;
                    
                case "delay":
                    config.setDelay(parseNumber());
                    break;
                    
                case "callback":
                    config.setCallback(parseExpression().toString());
                    break;
            }
            
            if (!match(CHTLJSTokenType.COMMA)) {
                break;
            }
        }
        
        consume(CHTLJSTokenType.RIGHT_BRACE, "期望'}'");
        consume(CHTLJSTokenType.RIGHT_PAREN, "期望')'");
        
        return animateCall;
    }
    
    /**
     * 解析动画状态
     */
    private void parseAnimationState(Map<String, String> state) {
        consume(CHTLJSTokenType.LEFT_BRACE, "期望'{'");
        
        while (!check(CHTLJSTokenType.RIGHT_BRACE) && !isAtEnd()) {
            String property = consume(CHTLJSTokenType.IDENTIFIER, "期望CSS属性").getValue();
            consume(CHTLJSTokenType.COLON, "期望':'");
            
            String value = parseValue();
            state.put(property, value);
            
            if (!match(CHTLJSTokenType.COMMA)) {
                break;
            }
        }
        
        consume(CHTLJSTokenType.RIGHT_BRACE, "期望'}'");
    }
    
    /**
     * 解析关键帧
     */
    private void parseKeyframes(AnimationConfigNode config) {
        consume(CHTLJSTokenType.LEFT_BRACKET, "期望'['");
        
        while (!check(CHTLJSTokenType.RIGHT_BRACKET) && !isAtEnd()) {
            consume(CHTLJSTokenType.LEFT_BRACE, "期望'{'");
            
            // 解析at
            consume(CHTLJSTokenType.AT, "期望'at'");
            consume(CHTLJSTokenType.COLON, "期望':'");
            double at = Double.parseDouble(parseNumber().toString());
            
            AnimationKeyframeNode keyframe = new AnimationKeyframeNode(at);
            
            // 跳过分号或逗号
            match(CHTLJSTokenType.SEMICOLON, CHTLJSTokenType.COMMA);
            
            // 解析CSS属性
            while (!check(CHTLJSTokenType.RIGHT_BRACE) && !isAtEnd()) {
                if (check(CHTLJSTokenType.IDENTIFIER)) {
                    String property = advance().getValue();
                    consume(CHTLJSTokenType.COLON, "期望':'");
                    String value = parseValue();
                    keyframe.addCssProperty(property, value);
                    
                    match(CHTLJSTokenType.COMMA, CHTLJSTokenType.SEMICOLON);
                } else {
                    advance();
                }
            }
            
            config.addKeyframe(keyframe);
            consume(CHTLJSTokenType.RIGHT_BRACE, "期望'}'");
            
            if (!match(CHTLJSTokenType.COMMA)) {
                break;
            }
        }
        
        consume(CHTLJSTokenType.RIGHT_BRACKET, "期望']'");
    }
    
    /**
     * 解析数字
     */
    private Integer parseNumber() {
        if (match(CHTLJSTokenType.NUMBER_LITERAL)) {
            return Integer.parseInt(previous().getValue());
        }
        if (match(CHTLJSTokenType.MINUS)) {
            if (match(CHTLJSTokenType.NUMBER_LITERAL)) {
                return -Integer.parseInt(previous().getValue());
            }
        }
        error("期望数字");
        return 0;
    }
    
    /**
     * 解析字符串
     */
    private String parseString() {
        if (match(CHTLJSTokenType.STRING_LITERAL)) {
            return previous().getValue();
        }
        if (match(CHTLJSTokenType.IDENTIFIER)) {
            return previous().getValue();
        }
        error("期望字符串");
        return "";
    }
    
    /**
     * 解析值（字符串或表达式）
     */
    private String parseValue() {
        if (check(CHTLJSTokenType.STRING_LITERAL)) {
            return "'" + advance().getValue() + "'";
        }
        
        // 收集直到逗号或分号或右大括号
        StringBuilder value = new StringBuilder();
        while (!check(CHTLJSTokenType.COMMA) && 
               !check(CHTLJSTokenType.SEMICOLON) && 
               !check(CHTLJSTokenType.RIGHT_BRACE) && 
               !isAtEnd()) {
            value.append(advance().getValue()).append(" ");
        }
        
        return value.toString().trim();
    }
    
    /**
     * 解析代码块
     */
    private String parseBlock() {
        StringBuilder block = new StringBuilder();
        
        consume(CHTLJSTokenType.LEFT_BRACE, "期望'{'");
        block.append("{ ");
        
        int braceCount = 1;
        while (braceCount > 0 && !isAtEnd()) {
            CHTLJSToken token = advance();
            if (token.getType() == CHTLJSTokenType.LEFT_BRACE) {
                braceCount++;
            } else if (token.getType() == CHTLJSTokenType.RIGHT_BRACE) {
                braceCount--;
                if (braceCount == 0) {
                    block.append("}");
                    break;
                }
            }
            block.append(token.getValue()).append(" ");
        }
        
        return block.toString();
    }
    
    /**
     * 解析表达式（简化版）
     */
    private ExpressionNode parseExpression() {
        StringBuilder expr = new StringBuilder();
        
        // 收集直到语句结束
        while (!check(CHTLJSTokenType.SEMICOLON) && 
               !check(CHTLJSTokenType.RIGHT_BRACE) && 
               !check(CHTLJSTokenType.RIGHT_PAREN) &&
               !check(CHTLJSTokenType.COMMA) &&
               !isAtEnd()) {
            
            // 处理嵌套的增强选择器
            if (check(CHTLJSTokenType.DOUBLE_LEFT_BRACE)) {
                EnhancedSelectorNode selector = parseEnhancedSelector();
                expr.append("{{").append(selector.getSelector()).append("}}");
            } else {
                expr.append(advance().getValue()).append(" ");
            }
        }
        
        // 消费分号（如果有）
        match(CHTLJSTokenType.SEMICOLON);
        
        return new ExpressionNode(expr.toString().trim());
    }
    
    // 辅助方法
    
    private boolean match(CHTLJSTokenType... types) {
        for (CHTLJSTokenType type : types) {
            if (check(type)) {
                advance();
                return true;
            }
        }
        return false;
    }
    
    private boolean check(CHTLJSTokenType type) {
        if (isAtEnd()) return false;
        return peek().getType() == type;
    }
    
    private CHTLJSToken advance() {
        if (!isAtEnd()) current++;
        return previous();
    }
    
    private boolean isAtEnd() {
        return peek().getType() == CHTLJSTokenType.EOF;
    }
    
    private CHTLJSToken peek() {
        return tokens.get(current);
    }
    
    private CHTLJSToken previous() {
        return tokens.get(current - 1);
    }
    
    private CHTLJSToken consume(CHTLJSTokenType type, String message) {
        if (check(type)) return advance();
        error(message);
        return new CHTLJSToken(type, "", 0, 0, 0);
    }
    
    private void error(String message) {
        CHTLJSToken token = peek();
        String errorMsg = String.format("[%d:%d] %s", token.getLine(), token.getColumn(), message);
        errors.add(errorMsg);
    }
    
    public List<String> getErrors() {
        return errors;
    }
}
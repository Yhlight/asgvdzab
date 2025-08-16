package com.chtl.parser;

import org.antlr.v4.runtime.Parser;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;


import java.util.*;

/**
 * CHTL JS解析上下文
 * 管理解析过程中的状态，支持无修饰字面量的识别
 */
public class CHTLJSParserContext {
    
    /**
     * 解析状态枚举
     */
    public enum ParseState {
        NORMAL,                    // 正常状态
        IN_SELECTOR,              // 在选择器内部 {{...}}
        IN_LISTEN_CONFIG,         // 在listen配置内部
        IN_DELEGATE_CONFIG,       // 在delegate配置内部
        IN_ANIMATE_CONFIG,        // 在animate配置内部
        IN_ANIMATION_STATE,       // 在动画状态内部(begin/end)
        IN_KEYFRAME,             // 在关键帧内部
        EXPECTING_VALUE          // 期望值（可能是无修饰字面量）
    }
    
    /**
     * 期望值类型枚举
     */
    public enum ExpectedValueType {
        STRING,                   // 字符串
        NUMBER,                   // 数字
        BOOLEAN,                  // 布尔值
        FUNCTION,                 // 函数
        SELECTOR,                 // 选择器
        CSS_VALUE,               // CSS值（可能是无修饰字面量）
        EASING_FUNCTION,         // 缓动函数（无修饰字面量）
        ANIMATION_DIRECTION,     // 动画方向（无修饰字面量）
        ANY                      // 任意值
    }
    
    private Stack<ParseState> stateStack;
    private ParseState currentState;
    private ExpectedValueType expectedValueType;
    private String currentProperty;  // 当前正在解析的属性名
    
    // 定义允许无修饰字面量的属性
    private static final Map<String, Set<String>> UNQUOTED_LITERAL_PROPERTIES;
    private static final Set<String> EASING_FUNCTIONS;
    private static final Set<String> ANIMATION_DIRECTIONS;
    private static final Set<String> CSS_KEYWORDS;
    
    static {
        UNQUOTED_LITERAL_PROPERTIES = new HashMap<>();
        
        // 动画配置中允许无修饰字面量的属性
        Set<String> animateProperties = new HashSet<>();
        animateProperties.add("easing");
        animateProperties.add("direction");
        UNQUOTED_LITERAL_PROPERTIES.put("animate", animateProperties);
        
        // CSS属性中允许无修饰字面量
        Set<String> cssProperties = new HashSet<>();
        cssProperties.add("display");
        cssProperties.add("position");
        cssProperties.add("overflow");
        cssProperties.add("visibility");
        cssProperties.add("float");
        cssProperties.add("clear");
        cssProperties.add("text-align");
        cssProperties.add("vertical-align");
        cssProperties.add("font-weight");
        cssProperties.add("font-style");
        cssProperties.add("text-decoration");
        cssProperties.add("white-space");
        cssProperties.add("word-wrap");
        cssProperties.add("cursor");
        UNQUOTED_LITERAL_PROPERTIES.put("css", cssProperties);
        
        // 缓动函数
        EASING_FUNCTIONS = new HashSet<>();
        EASING_FUNCTIONS.add("linear");
        EASING_FUNCTIONS.add("ease");
        EASING_FUNCTIONS.add("ease-in");
        EASING_FUNCTIONS.add("ease-out");
        EASING_FUNCTIONS.add("ease-in-out");
        EASING_FUNCTIONS.add("step-start");
        EASING_FUNCTIONS.add("step-end");
        
        // 动画方向
        ANIMATION_DIRECTIONS = new HashSet<>();
        ANIMATION_DIRECTIONS.add("normal");
        ANIMATION_DIRECTIONS.add("reverse");
        ANIMATION_DIRECTIONS.add("alternate");
        ANIMATION_DIRECTIONS.add("alternate-reverse");
        
        // CSS关键字
        CSS_KEYWORDS = new HashSet<>();
        CSS_KEYWORDS.add("none");
        CSS_KEYWORDS.add("auto");
        CSS_KEYWORDS.add("inherit");
        CSS_KEYWORDS.add("initial");
        CSS_KEYWORDS.add("unset");
        CSS_KEYWORDS.add("block");
        CSS_KEYWORDS.add("inline");
        CSS_KEYWORDS.add("inline-block");
        CSS_KEYWORDS.add("flex");
        CSS_KEYWORDS.add("grid");
        CSS_KEYWORDS.add("relative");
        CSS_KEYWORDS.add("absolute");
        CSS_KEYWORDS.add("fixed");
        CSS_KEYWORDS.add("sticky");
        CSS_KEYWORDS.add("hidden");
        CSS_KEYWORDS.add("visible");
        CSS_KEYWORDS.add("scroll");
        CSS_KEYWORDS.add("left");
        CSS_KEYWORDS.add("right");
        CSS_KEYWORDS.add("center");
        CSS_KEYWORDS.add("justify");
        CSS_KEYWORDS.add("top");
        CSS_KEYWORDS.add("bottom");
        CSS_KEYWORDS.add("middle");
        CSS_KEYWORDS.add("baseline");
        CSS_KEYWORDS.add("bold");
        CSS_KEYWORDS.add("normal");
        CSS_KEYWORDS.add("italic");
        CSS_KEYWORDS.add("underline");
        CSS_KEYWORDS.add("nowrap");
        CSS_KEYWORDS.add("pointer");
        CSS_KEYWORDS.add("default");
        CSS_KEYWORDS.add("transparent");
    }
    
    public CHTLJSParserContext() {
        this.stateStack = new Stack<>();
        this.currentState = ParseState.NORMAL;
        this.expectedValueType = ExpectedValueType.ANY;
    }
    
    /**
     * 进入新的解析状态
     */
    public void enterState(ParseState newState) {
        stateStack.push(currentState);
        currentState = newState;
    }
    
    /**
     * 退出当前解析状态
     */
    public void exitState() {
        if (!stateStack.isEmpty()) {
            currentState = stateStack.pop();
        }
    }
    
    /**
     * 设置当前属性
     */
    public void setCurrentProperty(String property) {
        this.currentProperty = property;
        updateExpectedValueType();
    }
    
    /**
     * 更新期望值类型
     */
    private void updateExpectedValueType() {
        if (currentProperty == null) {
            expectedValueType = ExpectedValueType.ANY;
            return;
        }
        
        // 根据当前状态和属性确定期望值类型
        switch (currentState) {
            case IN_ANIMATE_CONFIG:
                switch (currentProperty) {
                    case "easing":
                        expectedValueType = ExpectedValueType.EASING_FUNCTION;
                        break;
                    case "direction":
                        expectedValueType = ExpectedValueType.ANIMATION_DIRECTION;
                        break;
                    case "duration":
                    case "delay":
                    case "loop":
                        expectedValueType = ExpectedValueType.NUMBER;
                        break;
                    case "callback":
                        expectedValueType = ExpectedValueType.FUNCTION;
                        break;
                    default:
                        expectedValueType = ExpectedValueType.ANY;
                }
                break;
                
            case IN_ANIMATION_STATE:
            case IN_KEYFRAME:
                // CSS属性值
                expectedValueType = ExpectedValueType.CSS_VALUE;
                break;
                
            case IN_LISTEN_CONFIG:
            case IN_DELEGATE_CONFIG:
                if ("target".equals(currentProperty)) {
                    expectedValueType = ExpectedValueType.SELECTOR;
                } else {
                    expectedValueType = ExpectedValueType.FUNCTION;
                }
                break;
                
            default:
                expectedValueType = ExpectedValueType.ANY;
        }
    }
    
    /**
     * 判断当前是否允许无修饰字面量
     */
    public boolean allowsUnquotedLiteral() {
        switch (expectedValueType) {
            case EASING_FUNCTION:
            case ANIMATION_DIRECTION:
            case CSS_VALUE:
                return true;
            default:
                return false;
        }
    }
    
    /**
     * 验证无修饰字面量是否有效
     */
    public boolean isValidUnquotedLiteral(String value) {
        switch (expectedValueType) {
            case EASING_FUNCTION:
                return EASING_FUNCTIONS.contains(value);
                
            case ANIMATION_DIRECTION:
                return ANIMATION_DIRECTIONS.contains(value);
                
            case CSS_VALUE:
                // CSS值可以是关键字或带单位的数值
                return CSS_KEYWORDS.contains(value) || 
                       isValidCssValue(value);
                
            default:
                return false;
        }
    }
    
    /**
     * 检查是否是有效的CSS值
     */
    private boolean isValidCssValue(String value) {
        // 检查是否是数值+单位
        if (value.matches("^-?\\d+(\\.\\d+)?(px|em|rem|%|vh|vw|pt|cm|mm|in|pc|ex|ch)$")) {
            return true;
        }
        
        // 检查是否是颜色值
        if (value.matches("^#[0-9a-fA-F]{3}([0-9a-fA-F]{3})?$")) {
            return true;
        }
        
        // 检查是否是rgb/rgba/hsl/hsla
        if (value.matches("^(rgb|rgba|hsl|hsla)\\(.+\\)$")) {
            return true;
        }
        
        // 检查是否是calc()等CSS函数
        if (value.matches("^(calc|var|url|linear-gradient|radial-gradient)\\(.+\\)$")) {
            return true;
        }
        
        return false;
    }
    
    /**
     * 获取当前期望的无修饰字面量建议
     */
    public List<String> getUnquotedLiteralSuggestions() {
        List<String> suggestions = new ArrayList<>();
        
        switch (expectedValueType) {
            case EASING_FUNCTION:
                suggestions.addAll(EASING_FUNCTIONS);
                break;
                
            case ANIMATION_DIRECTION:
                suggestions.addAll(ANIMATION_DIRECTIONS);
                break;
                
            case CSS_VALUE:
                if (currentProperty != null) {
                    // 根据具体的CSS属性返回建议
                    suggestions.addAll(getCssPropertySuggestions(currentProperty));
                }
                break;
        }
        
        return suggestions;
    }
    
    /**
     * 获取CSS属性的建议值
     */
    private List<String> getCssPropertySuggestions(String property) {
        List<String> suggestions = new ArrayList<>();
        
        switch (property) {
            case "display":
                suggestions.addAll(Arrays.asList("none", "block", "inline", "inline-block", "flex", "grid"));
                break;
            case "position":
                suggestions.addAll(Arrays.asList("static", "relative", "absolute", "fixed", "sticky"));
                break;
            case "overflow":
                suggestions.addAll(Arrays.asList("visible", "hidden", "scroll", "auto"));
                break;
            case "text-align":
                suggestions.addAll(Arrays.asList("left", "right", "center", "justify"));
                break;
            case "font-weight":
                suggestions.addAll(Arrays.asList("normal", "bold", "100", "200", "300", "400", "500", "600", "700", "800", "900"));
                break;
            case "cursor":
                suggestions.addAll(Arrays.asList("default", "pointer", "move", "text", "wait", "help", "not-allowed"));
                break;
            default:
                suggestions.addAll(CSS_KEYWORDS);
        }
        
        return suggestions;
    }
    
    // Getters
    public ParseState getCurrentState() {
        return currentState;
    }
    
    public ExpectedValueType getExpectedValueType() {
        return expectedValueType;
    }
    
    public String getCurrentProperty() {
        return currentProperty;
    }
}
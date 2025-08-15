package com.chtl.compiler.chtljs.token;

/**
 * CHTL JS Token类型枚举
 * 严格根据CHTL语法文档的CHTL JS部分定义
 */
public enum CHTLJSTokenType {
    // CHTL JS特有标记
    DOUBLE_LEFT_BRACE("{{"),     // {{ 增强选择器开始
    DOUBLE_RIGHT_BRACE("}}"),    // }} 增强选择器结束
    ARROW("->"),                 // -> 箭头操作符（等价于.）
    
    // 选择器类型标记（在{{}}内部使用）
    DOT("."),                    // . 类选择器
    HASH("#"),                   // # ID选择器
    LEFT_BRACKET("["),           // [ 索引访问开始
    RIGHT_BRACKET("]"),          // ] 索引访问结束
    
    // 基础标点符号
    LEFT_BRACE("{"),            // {
    RIGHT_BRACE("}"),           // }
    LEFT_PAREN("("),            // (
    RIGHT_PAREN(")"),           // )
    COMMA(","),                 // ,
    COLON(":"),                 // :
    SEMICOLON(";"),             // ;
    
    // CHTL JS方法
    LISTEN("listen"),           // listen方法
    DELEGATE("delegate"),       // delegate方法
    ANIMATE("animate"),         // animate函数
    
    // 动画相关关键字
    DURATION("duration"),       // 动画持续时间
    EASING("easing"),          // 缓动函数
    BEGIN("begin"),            // 起始状态
    END("end"),                // 终止状态
    WHEN("when"),              // 时间点
    AT("at"),                  // 动画时刻
    LOOP("loop"),              // 循环次数
    DIRECTION("direction"),    // 播放方向
    DELAY("delay"),            // 延迟
    CALLBACK("callback"),      // 回调函数
    TARGET("target"),          // 委托目标
    
    // 字面量
    STRING_LITERAL("STRING"),   // 字符串
    NUMBER_LITERAL("NUMBER"),   // 数字
    IDENTIFIER("IDENTIFIER"),   // 标识符
    UNQUOTED_LITERAL("UNQUOTED"), // 无修饰字面量
    
    // JavaScript关键字（部分）
    FUNCTION("function"),       // function
    CONST("const"),            // const
    LET("let"),                // let
    VAR("var"),                // var
    RETURN("return"),          // return
    IF("if"),                  // if
    ELSE("else"),              // else
    FOR("for"),                // for
    WHILE("while"),            // while
    
    // 操作符
    EQUAL("="),                // = (改名以匹配词法分析器)
    EQUALS("="),               // = (保留兼容性)
    PLUS("+"),                 // +
    MINUS("-"),                // -
    MULTIPLY("*"),             // *
    DIVIDE("/"),               // /
    
    // 布尔和空值
    TRUE("true"),              // true
    FALSE("false"),            // false
    NULL("null"),              // null
    
    // 特殊标记
    EOF("EOF"),                // 文件结束
    WHITESPACE("WHITESPACE"),  // 空白字符
    
    // 注释
    LINE_COMMENT("//"),        // 行注释
    BLOCK_COMMENT("/**/");     // 块注释
    
    private final String symbol;
    
    CHTLJSTokenType(String symbol) {
        this.symbol = symbol;
    }
    
    public String getSymbol() {
        return symbol;
    }
}
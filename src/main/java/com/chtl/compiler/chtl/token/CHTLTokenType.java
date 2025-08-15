package com.chtl.compiler.chtl.token;

/**
 * CHTL Token类型枚举
 * 严格根据CHTL语法文档定义
 */
public enum CHTLTokenType {
    // 基础标点符号
    LEFT_BRACE("{"),              // {
    RIGHT_BRACE("}"),             // }
    COLON(":"),                   // :
    EQUALS("="),                  // = (CE对等式：与:等价)
    SEMICOLON(";"),               // ;
    COMMA(","),                   // ,
    DOT("."),                     // .
    HASH("#"),                    // #
    AMPERSAND("&"),               // &
    LEFT_BRACKET("["),            // [
    RIGHT_BRACKET("]"),           // ]
    LEFT_PAREN("("),              // (
    RIGHT_PAREN(")"),             // )
    SLASH("/"),                   // /
    MINUS("-"),                   // -
    GREATER_THAN(">"),            // >
    LESS_THAN("<"),               // <
    EXCLAMATION("!"),             // !
    AT("@"),                      // @
    
    // 字符串字面量
    STRING_LITERAL("STRING"),     // "..." 或 '...'
    UNQUOTED_LITERAL("UNQUOTED"), // 无引号字面量
    
    // 标识符
    IDENTIFIER("IDENTIFIER"),      // 标识符
    
    // 关键字
    TEXT("text"),                 // text关键字
    STYLE("style"),               // style关键字
    SCRIPT("script"),             // script关键字
    
    // 模板和自定义关键字
    TEMPLATE("[Template]"),       // [Template]
    CUSTOM("[Custom]"),           // [Custom]
    ORIGIN("[Origin]"),           // [Origin]
    IMPORT("[Import]"),           // [Import]
    NAMESPACE("[Namespace]"),     // [Namespace]
    CONFIGURATION("[Configuration]"), // [Configuration]
    
    // @符号关键字
    AT_STYLE("@Style"),           // @Style
    AT_ELEMENT("@Element"),       // @Element
    AT_VAR("@Var"),               // @Var
    AT_HTML("@Html"),             // @Html
    AT_JAVASCRIPT("@JavaScript"), // @JavaScript
    AT_CHTL("@Chtl"),            // @Chtl
    AT_CJMOD("@CJmod"),          // @CJmod
    
    // 特殊关键字
    INHERIT("inherit"),           // inherit
    DELETE("delete"),             // delete
    INSERT("insert"),             // insert
    AFTER("after"),               // after
    BEFORE("before"),             // before
    REPLACE("replace"),           // replace
    AT_TOP("at top"),             // at top
    AT_BOTTOM("at bottom"),       // at bottom
    FROM("from"),                 // from
    AS("as"),                     // as
    EXCEPT("except"),             // except
    TOP("top"),                   // top
    BOTTOM("bottom"),             // bottom
    
    // 注释
    LINE_COMMENT("//"),           // // 注释
    BLOCK_COMMENT("/**/"),        // /* */ 注释
    GENERATOR_COMMENT("--"),      // -- 生成器识别的注释
    
    // 特殊标记
    EOF("EOF"),                   // 文件结束
    
    // 数字
    NUMBER("NUMBER"),             // 数字字面量
    
    // 空白
    WHITESPACE("WHITESPACE");     // 空白字符
    
    private final String symbol;
    
    CHTLTokenType(String symbol) {
        this.symbol = symbol;
    }
    
    public String getSymbol() {
        return symbol;
    }
}
package com.chtl.model;

public enum CodeFragmentType {
    // 模板/HTML 普通文本
    HTML_TEXT,
    // CHTL 语法最小单元（模板/样式/脚本等上下文内）
    CHTL_TOKEN,
    // CHTL JS 语法最小单元（仅脚本上下文）
    CHTL_JS_TOKEN,

    // 样式块（局部/全局）
    LOCAL_STYLE_BLOCK,
    CSS_CODE,

    // 脚本代码
    JS_CODE
}
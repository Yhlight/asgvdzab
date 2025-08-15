package com.chtl.compiler.chtljs.selector;

/**
 * 选择器类型
 */
public enum SelectorType {
    CLASS,      // 类选择器
    ID,         // ID选择器  
    TAG,        // 标签选择器
    ATTRIBUTE,  // 属性选择器
    COMPLEX     // 复杂选择器
}
package com.chtl.ast.chtljs;

import com.chtl.ast.SourceLocation;

/**
 * CHTL JS AST节点接口
 * 严格按照CHTL语法文档的CHTL JS部分定义
 */
public interface CHTLJSASTNode {
    
    /**
     * 获取节点类型
     */
    NodeType getNodeType();
    
    /**
     * 获取源代码位置
     */
    SourceLocation getLocation();
    
    /**
     * 设置源代码位置
     */
    void setLocation(SourceLocation location);
    
    /**
     * 接受访问者
     */
    void accept(CHTLJSASTVisitor visitor);
    
    /**
     * CHTL JS节点类型
     */
    enum NodeType {
        // 增强选择器
        ENHANCED_SELECTOR,    // {{...}}
        
        // 选择器类型
        CLASS_SELECTOR,       // .className
        ID_SELECTOR,          // #idName
        TAG_SELECTOR,         // tagName
        DESCENDANT_SELECTOR,  // .box button
        INDEX_SELECTOR,       // button[0]
        
        // 操作
        ARROW_OPERATION,      // ->
        DOT_OPERATION,        // .
        
        // 方法调用
        LISTEN_CALL,          // listen({...})
        DELEGATE_CALL,        // delegate({...})
        ANIMATE_CALL,         // animate({...})
        
        // 事件处理器
        EVENT_HANDLER,        // click: function
        
        // 动画配置
        ANIMATION_CONFIG,     // animate的配置对象
        ANIMATION_KEYFRAME,   // when数组中的关键帧
        
        // 属性访问
        PROPERTY_ACCESS,      // .style.color
        
        // 表达式
        EXPRESSION,           // 一般表达式
        FUNCTION_EXPRESSION,  // 函数表达式

        
        // 语句
        STATEMENT,            // 语句
        BLOCK_STATEMENT       // 语句块
    }
}
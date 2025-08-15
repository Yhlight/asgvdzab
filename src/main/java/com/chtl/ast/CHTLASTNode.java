package com.chtl.ast;import com.chtl.model.SourceLocation;


import java.util.List;

/**
 * CHTL AST节点基础接口
 * 严格按照CHTL语法文档定义
 */
public interface CHTLASTNode {
    /**
     * 获取节点类型
     */
    NodeType getNodeType();
    
    /**
     * 获取子节点
     */
    List<CHTLASTNode> getChildren();
    
    /**
     * 添加子节点
     */
    void addChild(CHTLASTNode child);
    
    /**
     * 获取父节点
     */
    CHTLASTNode getParent();
    
    /**
     * 设置父节点
     */
    void setParent(CHTLASTNode parent);
    
    /**
     * 获取节点在源代码中的位置
     */
    SourceLocation getLocation();
    
    /**
     * 设置节点在源代码中的位置
     */
    void setLocation(SourceLocation location);
    
    /**
     * 接受访问者
     */
    void accept(CHTLASTVisitor visitor);
    
    /**
     * 节点类型枚举
     */
    enum NodeType {
        // 根节点
        ROOT,
        
        // 基础节点
        ELEMENT,              // 元素节点
        TEXT,                 // 文本节点
        ATTRIBUTE,            // 属性节点
        COMMENT,              // 注释节点
        
        // 样式相关
        STYLE_BLOCK,          // style块
        STYLE_RULE,           // 样式规则
        STYLE_PROPERTY,       // 样式属性
        STYLE_SELECTOR,       // 样式选择器
        
        // 脚本相关
        SCRIPT_BLOCK,         // script块
        
        // 模板相关
        TEMPLATE_DEFINITION,  // 模板定义
        TEMPLATE_USAGE,       // 模板使用
        
        // 自定义相关
        CUSTOM_DEFINITION,    // 自定义定义
        CUSTOM_USAGE,         // 自定义使用
        CUSTOM_OPERATION,     // 自定义操作（delete, insert等）
        SPECIALIZATION,       // 特例化操作
        
        // 原始嵌入
        ORIGIN_BLOCK,         // 原始嵌入块
        
        // 导入
        IMPORT_STATEMENT,     // 导入语句
        
        // 命名空间
        NAMESPACE,            // 命名空间
        
        // 配置
        CONFIGURATION,        // 配置组
        
        // 约束
        CONSTRAINT,           // 约束
        
        // 变量组
        VAR_GROUP_DEFINITION, // 变量组定义
        VAR_GROUP_USAGE,      // 变量组使用
        
        // 继承
        INHERIT_STATEMENT,    // 继承语句
        
        // 字面量
        STRING_LITERAL,       // 字符串字面量
        UNQUOTED_LITERAL,     // 无引号字面量
        NUMBER_LITERAL,       // 数字字面量
        
        // CHTL JS相关
        CHTLJS_SELECTOR,      // CHTL JS选择器
        CHTLJS_METHOD_CALL,   // CHTL JS方法调用
        CHTLJS_ANIMATE,       // CHTL JS动画
        CHTLJS_OBJECT,        // CHTL JS对象
        CHTLJS_ARRAY,         // CHTL JS数组
        CHTLJS_FUNCTION,      // CHTL JS函数

        CHTLJS_IDENTIFIER     // CHTL JS标识符
    }
}
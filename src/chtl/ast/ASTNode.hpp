#pragma once

#include <memory>
#include <vector>
#include <string>

namespace chtl {
namespace ast {

// 前向声明
class ASTVisitor;

/**
 * AST节点类型
 */
enum class ASTNodeType {
    // 根节点
    PROGRAM,
    
    // 基础节点
    ELEMENT,            // HTML元素
    TEXT,               // 文本节点
    ATTRIBUTE,          // 属性
    COMMENT,            // 注释
    
    // 样式相关
    STYLE_BLOCK,        // 局部样式块
    STYLE_RULE,         // 样式规则
    STYLE_SELECTOR,     // 选择器
    STYLE_PROPERTY,     // 样式属性
    
    // 脚本相关
    SCRIPT_BLOCK,       // 局部脚本块
    
    // 模板/自定义
    TEMPLATE_DEF,       // 模板定义
    CUSTOM_DEF,         // 自定义定义
    TEMPLATE_USE,       // 模板使用
    CUSTOM_USE,         // 自定义使用
    
    // 特殊操作
    DELETE_OP,          // delete操作
    INSERT_OP,          // insert操作
    INHERIT_OP,         // inherit操作
    
    // 原始嵌入
    ORIGIN_BLOCK,       // [Origin]块
    
    // 导入
    IMPORT_STMT,        // [Import]语句
    
    // 命名空间
    NAMESPACE_DEF,      // [Namespace]定义
    
    // 配置
    CONFIG_BLOCK,       // [Configuration]块
    
    // 约束
    EXCEPT_CONSTRAINT,  // except约束
    
    // 字面量
    STRING_LITERAL,     // 字符串字面量
    NUMBER_LITERAL,     // 数字字面量
    UNQUOTED_LITERAL,   // 无引号字面量
    IDENTIFIER          // 标识符
};

/**
 * AST基础节点
 */
class ASTNode {
public:
    ASTNode(ASTNodeType type, size_t line = 0, size_t column = 0)
        : type_(type), line_(line), column_(column) {}
    
    virtual ~ASTNode() = default;
    
    // 访问者模式
    virtual void accept(ASTVisitor& visitor) = 0;
    
    // 获取节点类型
    ASTNodeType getType() const { return type_; }
    
    // 位置信息
    size_t getLine() const { return line_; }
    size_t getColumn() const { return column_; }
    
    // 子节点管理
    void addChild(std::shared_ptr<ASTNode> child) {
        children_.push_back(child);
    }
    
    const std::vector<std::shared_ptr<ASTNode>>& getChildren() const {
        return children_;
    }
    
    std::vector<std::shared_ptr<ASTNode>>& getChildren() {
        return children_;
    }
    
protected:
    ASTNodeType type_;
    size_t line_;
    size_t column_;
    std::vector<std::shared_ptr<ASTNode>> children_;
};

// 定义智能指针类型
using ASTNodePtr = std::shared_ptr<ASTNode>;

} // namespace ast
} // namespace chtl
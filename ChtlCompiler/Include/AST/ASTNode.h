#ifndef CHTL_AST_ASTNODE_H
#define CHTL_AST_ASTNODE_H

#include <string>
#include <vector>
#include <memory>
#include "Lexer/Token.h"

namespace Chtl {

// AST节点类型
enum class ASTNodeType {
    // 根节点
    PROGRAM,                    // 程序根节点
    
    // 基本节点
    ELEMENT,                    // 元素节点（div, span等）
    TEXT,                       // 文本节点 text { }
    ATTRIBUTE,                  // 属性节点 id: value;
    COMMENT,                    // 注释节点
    
    // 样式相关
    STYLE_BLOCK,                // style { } 局部样式块
    STYLE_RULE,                 // CSS规则（选择器 + 声明）
    STYLE_PROPERTY,             // CSS属性 width: 100px;
    STYLE_SELECTOR,             // CSS选择器 .box, #id, &:hover
    
    // 脚本相关
    SCRIPT_BLOCK,               // script { } 局部脚本块
    
    // 模板相关
    TEMPLATE_DEFINITION,        // [Template] 定义
    STYLE_TEMPLATE,             // @Style 模板
    ELEMENT_TEMPLATE,           // @Element 模板
    VAR_TEMPLATE,               // @Var 模板
    
    // 自定义相关
    CUSTOM_DEFINITION,          // [Custom] 定义
    CUSTOM_STYLE,               // 自定义样式组
    CUSTOM_ELEMENT,             // 自定义元素
    CUSTOM_VAR,                 // 自定义变量组
    
    // 使用相关
    TEMPLATE_USE,               // 模板使用 @Style Name;
    VAR_USE,                    // 变量使用 ThemeColor(tableColor)
    
    // 继承和操作
    INHERIT_STATEMENT,          // inherit @Style Name;
    DELETE_STATEMENT,           // delete property1, property2;
    INSERT_STATEMENT,           // insert after/before element[index] { }
    
    // 原始嵌入
    ORIGIN_BLOCK,               // [Origin] 块
    
    // 配置相关
    CONFIGURATION_BLOCK,        // [Configuration] 块
    NAME_BLOCK,                 // [Name] 块
    ORIGIN_TYPE_BLOCK,          // [OriginType] 块
    
    // 导入导出
    IMPORT_STATEMENT,           // [Import] 语句
    EXPORT_STATEMENT,           // [Export] 语句
    NAMESPACE_BLOCK,            // [Namespace] 块
    INFO_BLOCK,                 // [Info] 块
    
    // 字面量
    STRING_LITERAL,             // 字符串字面量
    NUMBER_LITERAL,             // 数字字面量
    IDENTIFIER,                 // 标识符
    
    // CHTL JS 特有节点
    SELECTOR_EXPRESSION,        // {{选择器}} 表达式
    ARROW_ACCESS,               // -> 访问
    LISTEN_CALL,                // listen 调用
    DELEGATE_CALL,              // delegate 调用
    ANIMATE_CALL,               // animate 调用
    VIRTUAL_OBJECT,             // 虚对象声明
    VIRTUAL_CALL,               // 虚对象函数调用
    
    // 其他
    EXPRESSION,                 // 通用表达式节点
    STATEMENT_LIST              // 语句列表
};

// AST节点基类
class ASTNode {
public:
    ASTNode(ASTNodeType type, const Token& token);
    virtual ~ASTNode() = default;
    
    // 基本属性
    ASTNodeType getType() const { return type_; }
    const Token& getToken() const { return token_; }
    
    // 位置信息
    size_t getLine() const { return token_.line; }
    size_t getColumn() const { return token_.column; }
    
    // 子节点管理
    void addChild(std::shared_ptr<ASTNode> child);
    const std::vector<std::shared_ptr<ASTNode>>& getChildren() const { return children_; }
    std::shared_ptr<ASTNode> getChild(size_t index) const;
    size_t getChildCount() const { return children_.size(); }
    
    // 访问者模式（后续实现）
    virtual void accept(class ASTVisitor* visitor) = 0;
    
    // 调试输出
    virtual std::string toString() const;
    virtual void dump(int indent = 0) const;

protected:
    ASTNodeType type_;
    Token token_;
    std::vector<std::shared_ptr<ASTNode>> children_;
    
    // 辅助方法
    std::string getIndent(int level) const;
};

// 类型别名
using ASTNodePtr = std::shared_ptr<ASTNode>;

} // namespace Chtl

#endif // CHTL_AST_ASTNODE_H
#ifndef CHTL_COMPILER_CHTL_AST_H
#define CHTL_COMPILER_CHTL_AST_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "common/types.h"

namespace chtl {
namespace compiler {
namespace ast {

// AST节点类型
enum class NodeType {
    DOCUMENT,
    ELEMENT,
    TEXT,
    STYLE,
    SCRIPT,
    TEMPLATE,
    CUSTOM,
    IMPORT,
    NAMESPACE,
    ORIGIN,
    CONFIGURATION,
    ATTRIBUTE,
    COMMENT
};

// 基础AST节点
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual NodeType getType() const = 0;
    virtual SourceLocation getLocation() const { return location_; }
    void setLocation(const SourceLocation& loc) { location_ = loc; }

protected:
    SourceLocation location_;
};

// 文档节点（根节点）
class DocumentNode : public ASTNode {
public:
    NodeType getType() const override { return NodeType::DOCUMENT; }
    
    std::vector<std::shared_ptr<ASTNode>> children;
    std::string namespace_;  // 文档命名空间
};

// 元素节点
class ElementNode : public ASTNode {
public:
    NodeType getType() const override { return NodeType::ELEMENT; }
    
    std::string tagName;
    std::map<std::string, std::string> attributes;
    std::vector<std::shared_ptr<ASTNode>> children;
    
    // 特殊属性
    std::string id;
    std::string className;
    std::vector<std::string> classList;
    
    // 约束
    std::vector<std::string> exceptions;  // except列表
};

// 文本节点
class TextNode : public ASTNode {
public:
    NodeType getType() const override { return NodeType::TEXT; }
    
    std::string content;
    bool isLiteral = false;  // 是否是字面量文本
};

// 样式节点
class StyleNode : public ASTNode {
public:
    NodeType getType() const override { return NodeType::STYLE; }
    
    struct StyleRule {
        std::string selector;
        std::map<std::string, std::string> properties;
        bool isInline = false;  // 是否是内联样式
        bool isAutoClass = false;  // 是否是自动类名
        bool isAutoId = false;  // 是否是自动ID
    };
    
    std::vector<StyleRule> rules;
    bool isLocal = false;  // 是否是局部样式块
    std::string parentElement;  // 父元素名称
    
    // 样式模板引用
    std::vector<std::pair<std::string, std::map<std::string, std::string>>> templateRefs;
};

// 脚本节点
class ScriptNode : public ASTNode {
public:
    NodeType getType() const override { return NodeType::SCRIPT; }
    
    std::string content;
    bool isLocal = false;  // 是否是局部脚本块
    std::string parentElement;  // 父元素名称
};

// 模板节点
class TemplateNode : public ASTNode {
public:
    NodeType getType() const override { return NodeType::TEMPLATE; }
    
    enum class TemplateType {
        STYLE,
        ELEMENT,
        VAR
    };
    
    TemplateType templateType;
    std::string name;
    std::vector<std::shared_ptr<ASTNode>> children;
    std::map<std::string, std::string> parameters;  // 模板参数
    std::vector<std::string> inherits;  // 继承的模板
};

// 自定义节点
class CustomNode : public ASTNode {
public:
    NodeType getType() const override { return NodeType::CUSTOM; }
    
    enum class CustomType {
        STYLE,
        ELEMENT,
        VAR
    };
    
    CustomType customType;
    std::string name;
    std::vector<std::shared_ptr<ASTNode>> children;
    std::map<std::string, std::string> parameters;
    std::vector<std::string> inherits;
    
    // 特例化操作
    std::vector<std::string> deletions;
    std::map<std::string, std::shared_ptr<ASTNode>> insertions;
};

// 导入节点
class ImportNode : public ASTNode {
public:
    NodeType getType() const override { return NodeType::IMPORT; }
    
    enum class ImportType {
        HTML,
        STYLE,
        JAVASCRIPT,
        CHTL,
        CJMOD,
        TEMPLATE,
        CUSTOM
    };
    
    ImportType importType;
    std::string path;
    std::string alias;  // as命名
    std::string target;  // 导入的具体项（如模板名）
};

// 命名空间节点
class NamespaceNode : public ASTNode {
public:
    NodeType getType() const override { return NodeType::NAMESPACE; }
    
    std::string name;
    std::vector<std::shared_ptr<ASTNode>> children;
    std::vector<std::string> exceptions;  // 全局约束
};

// 原始嵌入节点
class OriginNode : public ASTNode {
public:
    NodeType getType() const override { return NodeType::ORIGIN; }
    
    enum class OriginType {
        HTML,
        STYLE,
        JAVASCRIPT
    };
    
    OriginType originType;
    std::string content;
    std::string name;  // 增强原始嵌入的名称
};

// 配置节点
class ConfigurationNode : public ASTNode {
public:
    NodeType getType() const override { return NodeType::CONFIGURATION; }
    
    std::map<std::string, std::string> settings;
    std::map<std::string, std::vector<std::string>> nameGroups;
};

// 属性节点
class AttributeNode : public ASTNode {
public:
    NodeType getType() const override { return NodeType::ATTRIBUTE; }
    
    std::string name;
    std::string value;
    bool isColon = true;  // 使用:还是=
};

// 注释节点
class CommentNode : public ASTNode {
public:
    NodeType getType() const override { return NodeType::COMMENT; }
    
    std::string content;
    bool isMultiLine = false;
    bool isGeneratorComment = false;  // 是否是--注释
};

} // namespace ast
} // namespace compiler
} // namespace chtl

#endif // CHTL_COMPILER_CHTL_AST_H
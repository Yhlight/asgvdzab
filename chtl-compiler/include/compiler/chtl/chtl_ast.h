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

// 节点类型枚举
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

// AST节点基类
class ASTNode {
public:
    NodeType type;
    SourceLocation location;
    
    ASTNode(NodeType t) : type(t) {}
    virtual ~ASTNode();
};

// 前向声明
class AttributeNode;

// 文档节点（根节点）
class DocumentNode : public ASTNode {
public:
    std::vector<std::shared_ptr<ASTNode>> children;
    
    DocumentNode() : ASTNode(NodeType::DOCUMENT) {}
    ~DocumentNode() override;
};

// 元素节点
class ElementNode : public ASTNode {
public:
    std::string tagName;
    std::vector<std::shared_ptr<AttributeNode>> attributes;
    std::vector<std::shared_ptr<ASTNode>> children;
    std::string id;
    std::string className;
    
    ElementNode() : ASTNode(NodeType::ELEMENT) {}
    ~ElementNode() override;
};

// 文本节点
class TextNode : public ASTNode {
public:
    std::string content;
    bool isLiteral;  // 是否是字面量（无需转义）
    
    TextNode() : ASTNode(NodeType::TEXT), isLiteral(false) {}
    ~TextNode() override;
};

// 样式节点
class StyleNode : public ASTNode {
public:
    std::string content;
    bool isLocal;    // 是否是局部样式块
    
    StyleNode() : ASTNode(NodeType::STYLE), isLocal(false) {}
    ~StyleNode() override;
};

// 脚本节点
class ScriptNode : public ASTNode {
public:
    std::string content;
    bool isLocal;    // 是否是局部脚本块
    
    ScriptNode() : ASTNode(NodeType::SCRIPT), isLocal(false) {}
    ~ScriptNode() override;
};

// 模板节点
class TemplateNode : public ASTNode {
public:
    enum class TemplateType {
        STYLE_TEMPLATE,    // [Template] @Style
        ELEMENT_TEMPLATE,  // [Template] @Element
        VAR_TEMPLATE       // [Template] @Var
    };
    
    TemplateType templateType;
    std::string name;
    std::vector<std::shared_ptr<ASTNode>> children;
    
    TemplateNode() : ASTNode(NodeType::TEMPLATE) {}
    ~TemplateNode() override;
};

// 自定义节点
class CustomNode : public ASTNode {
public:
    std::string name;
    std::string baseElement;
    std::vector<std::shared_ptr<ASTNode>> children;
    
    CustomNode() : ASTNode(NodeType::CUSTOM) {}
    ~CustomNode() override;
};

// 导入节点
class ImportNode : public ASTNode {
public:
    std::string path;
    std::string alias;
    std::vector<std::string> exceptions;  // except列表
    
    ImportNode() : ASTNode(NodeType::IMPORT) {}
    ~ImportNode() override;
};

// 命名空间节点
class NamespaceNode : public ASTNode {
public:
    std::string name;
    std::string alias;
    std::vector<std::shared_ptr<ASTNode>> children;
    
    NamespaceNode() : ASTNode(NodeType::NAMESPACE) {}
    ~NamespaceNode() override;
};

// 原始嵌入节点
class OriginNode : public ASTNode {
public:
    enum class OriginType {
        HTML,
        CSS,
        JAVASCRIPT
    };
    
    OriginType originType;
    std::string content;
    
    OriginNode() : ASTNode(NodeType::ORIGIN) {}
    ~OriginNode() override;
};

// 配置节点
class ConfigurationNode : public ASTNode {
public:
    std::string name;
    std::map<std::string, std::string> settings;
    
    ConfigurationNode() : ASTNode(NodeType::CONFIGURATION) {}
    ~ConfigurationNode() override;
};

// 属性节点
class AttributeNode : public ASTNode {
public:
    std::string name;
    std::string value;
    
    AttributeNode() : ASTNode(NodeType::ATTRIBUTE) {}
    ~AttributeNode() override;
};

// 注释节点
class CommentNode : public ASTNode {
public:
    std::string content;
    bool isSemantic;  // 是否是语义注释 (--)
    
    CommentNode() : ASTNode(NodeType::COMMENT), isSemantic(false) {}
    ~CommentNode() override;
};

} // namespace ast
} // namespace compiler
} // namespace chtl

#endif // CHTL_COMPILER_CHTL_AST_H
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <unordered_map>
#include <functional>

namespace chtl {

// AST节点类型枚举 - 严格按照语法文档
enum class ASTNodeType {
    // 基础节点
    ROOT,                   // 根节点
    COMMENT,                // 注释节点
    TEXT_NODE,              // 文本节点
    LITERAL,                // 字面量节点
    
    // 元素节点
    HTML_ELEMENT,           // HTML元素节点
    ATTRIBUTE,              // 属性节点
    ATTRIBUTE_LIST,         // 属性列表节点
    
    // 样式块节点
    STYLE_BLOCK,            // 样式块节点
    CSS_RULE,               // CSS规则节点
    CSS_PROPERTY,           // CSS属性节点
    CSS_VALUE,              // CSS值节点
    CSS_SELECTOR,           // CSS选择器节点
    CONTEXT_REFERENCE,      // 上下文引用节点(&)
    
    // 模板节点
    TEMPLATE_DECLARATION,   // [Template]声明节点
    STYLE_TEMPLATE,         // @Style模板节点
    ELEMENT_TEMPLATE,       // @Element模板节点
    VAR_TEMPLATE,           // @Var模板节点
    TEMPLATE_INHERITANCE,   // 模板继承节点
    TEMPLATE_USAGE,         // 模板使用节点
    
    // 自定义节点
    CUSTOM_DECLARATION,     // [Custom]声明节点
    CUSTOM_STYLE,           // @Style自定义节点
    CUSTOM_ELEMENT,         // @Element自定义节点
    CUSTOM_VAR,             // @Var自定义节点
    CUSTOM_SPECIALIZATION,  // 自定义特例化节点
    DELETE_OPERATION,       // delete操作节点
    INSERT_OPERATION,       // insert操作节点
    REPLACE_OPERATION,      // replace操作节点
    
    // 原始嵌入节点
    ORIGIN_DECLARATION,     // [Origin]声明节点
    ORIGIN_HTML,            // @Html原始嵌入
    ORIGIN_STYLE,           // @Style原始嵌入
    ORIGIN_JAVASCRIPT,      // @JavaScript原始嵌入
    
    // 配置组节点
    CONFIGURATION_DECLARATION, // [Configuration]声明节点
    NAME_DECLARATION,       // [Name]声明节点
    CONFIG_OPTION,          // 配置选项节点
    
    // 导入节点
    IMPORT_DECLARATION,     // [Import]声明节点
    IMPORT_HTML,            // @Html导入
    IMPORT_STYLE,           // @Style导入
    IMPORT_JAVASCRIPT,      // @JavaScript导入
    IMPORT_CHTL,            // @Chtl导入
    IMPORT_CJMOD,           // @CJmod导入
    
    // 命名空间节点
    NAMESPACE_DECLARATION,  // [Namespace]声明节点
    NAMESPACE_BODY,         // 命名空间体节点
    
    // 约束节点
    CONSTRAINT_DECLARATION, // 约束声明节点
    EXCEPT_CLAUSE,          // except子句节点
    
    // 脚本节点
    SCRIPT_BLOCK,           // script块节点
    
    // 模块信息节点
    INFO_DECLARATION,       // [Info]声明节点
    EXPORT_DECLARATION,     // [Export]声明节点
    
    // 其他节点
    UNKNOWN                 // 未知节点类型
};

// AST节点基类
class ASTNode {
public:
    ASTNodeType type;
    std::string node_id;        // 节点唯一标识符
    size_t line;                // 行号
    size_t column;              // 列号
    size_t start_position;      // 开始位置
    size_t end_position;        // 结束位置
    std::string source_file;    // 源文件
    
    // 子节点
    std::vector<std::unique_ptr<ASTNode>> children;
    
    // 父节点引用（弱引用，避免循环引用）
    ASTNode* parent;
    
    // 节点元数据
    std::unordered_map<std::string, std::string> metadata;
    
    ASTNode(ASTNodeType t, size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : type(t), line(l), column(c), start_position(start), end_position(end), parent(nullptr) {}
    
    virtual ~ASTNode() = default;
    
    // 子节点管理
    void addChild(std::unique_ptr<ASTNode> child);
    void removeChild(ASTNode* child);
    ASTNode* getChild(size_t index) const;
    size_t getChildCount() const;
    
    // 父节点管理
    void setParent(ASTNode* p);
    ASTNode* getParent() const;
    
    // 元数据管理
    void setMetadata(const std::string& key, const std::string& value);
    std::string getMetadata(const std::string& key) const;
    bool hasMetadata(const std::string& key) const;
    void removeMetadata(const std::string& key);
    
    // 节点查询
    std::vector<ASTNode*> findNodesByType(ASTNodeType type) const;
    ASTNode* findFirstNodeByType(ASTNodeType type) const;
    ASTNode* findNodeById(const std::string& id) const;
    
    // 节点遍历
    void traverse(std::function<void(ASTNode*)> visitor);
    void traverseConst(std::function<void(const ASTNode*)> visitor) const;
    
    // 获取节点类型名称
    std::string getNodeTypeName() const;
    std::string getNodeTypeName(ASTNodeType type) const;
    
    // 节点验证
    virtual bool isValid() const;
    virtual std::vector<std::string> validate() const;
    
    // 节点序列化
    virtual std::string serialize() const;
    virtual std::string toDebugString() const;
    
    // 节点克隆
    virtual std::unique_ptr<ASTNode> clone() const;
    
    // 节点比较
    virtual bool equals(const ASTNode* other) const;
    
    // 节点统计
    size_t getTotalNodeCount() const;
    size_t getNodeCountByType(ASTNodeType type) const;
    
    // 节点路径
    std::string getNodePath() const;
    std::vector<std::string> getAncestorPath() const;
    
    // 节点深度
    size_t getDepth() const;
    size_t getMaxDepth() const;
    
    // 节点清理
    void clearChildren();
    void clearMetadata();
    
public:
    // 节点ID管理
    void generateNodeId();
    
protected:
    // 内部辅助方法
    std::string getIndentation(size_t depth) const;
};

// AST节点值类型
using ASTNodeValue = std::variant<
    std::string,      // 字符串值
    int,              // 整数值
    double,           // 浮点值
    bool,             // 布尔值
    std::nullptr_t    // 空值
>;

// AST节点访问者模式基类
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    // 访问各种节点类型
    virtual void visit(ASTNode* node) = 0;
    virtual void visitRoot(ASTNode* node) = 0;
    virtual void visitComment(ASTNode* node) = 0;
    virtual void visitTextNode(ASTNode* node) = 0;
    virtual void visitLiteral(ASTNode* node) = 0;
    virtual void visitHTMLElement(ASTNode* node) = 0;
    virtual void visitAttribute(ASTNode* node) = 0;
    virtual void visitStyleBlock(ASTNode* node) = 0;
    virtual void visitTemplateDeclaration(ASTNode* node) = 0;
    virtual void visitCustomDeclaration(ASTNode* node) = 0;
    virtual void visitOriginDeclaration(ASTNode* node) = 0;
    virtual void visitConfigurationDeclaration(ASTNode* node) = 0;
    virtual void visitImportDeclaration(ASTNode* node) = 0;
    virtual void visitNamespaceDeclaration(ASTNode* node) = 0;
    virtual void visitConstraintDeclaration(ASTNode* node) = 0;
    virtual void visitScriptBlock(ASTNode* node) = 0;
    virtual void visitInfoDeclaration(ASTNode* node) = 0;
    virtual void visitExportDeclaration(ASTNode* node) = 0;
};

} // namespace chtl
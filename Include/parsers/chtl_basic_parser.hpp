#pragma once

#include "CHTL/ast_nodes.hpp"
#include "CHTL/lexer.hpp"
#include <string>
#include <vector>
#include <memory>

namespace chtl {

/**
 * CHTL AST节点类型枚举
 */
enum class CHTLASTNodeType {
    // 注释类型
    COMMENT_LINE,           // 单行注释 //
    COMMENT_MULTILINE,      // 多行注释 /* */
    COMMENT_GENERATOR,      // 生成器注释 --
    
    // 基础节点
    TEXT_NODE,              // 文本节点
    ELEMENT,                // HTML元素
    ATTRIBUTE,              // 属性
    
    // CHTL特殊结构
    TEMPLATE_STYLE,         // [Template] @Style
    TEMPLATE_ELEMENT,       // [Template] @Element
    TEMPLATE_VAR,           // [Template] @Var
    CUSTOM_STYLE,           // [Custom] @Style
    CUSTOM_ELEMENT,         // [Custom] @Element
    CUSTOM_VAR,             // [Custom] @Var
    
    // 命名空间和导入
    NAMESPACE,              // [Namespace]
    IMPORT,                 // [Import]
    
    // 样式相关
    STYLE_BLOCK,            // style {}
    STYLE_PROPERTY,         // CSS属性
    SELECTOR,               // CSS选择器
    
    // 配置和约束
    CONFIGURATION,          // [Configuration]
    CONSTRAINT,             // except约束
    
    // 原始嵌入
    ORIGIN_HTML,            // [Origin] @Html
    ORIGIN_STYLE,           // [Origin] @Style
    ORIGIN_JAVASCRIPT,      // [Origin] @JavaScript
    
    // 表达式和操作
    VARIABLE_ACCESS,        // 变量访问
    TEMPLATE_USAGE,         // 模板使用
    INHERITANCE,            // 继承
    SPECIALIZATION,         // 特例化
    
    // 程序结构
    PROGRAM,                // 程序根节点
    BLOCK,                  // 代码块
    LITERAL                 // 字面量
};

/**
 * CHTL AST节点基类
 */
class CHTLASTNode {
public:
    CHTLASTNodeType type;
    std::string value;
    std::vector<std::shared_ptr<CHTLASTNode>> children;
    ast::SourceLocation location;
    
    explicit CHTLASTNode(CHTLASTNodeType t) : type(t) {}
    virtual ~CHTLASTNode() = default;
    
    virtual std::string toString() const;
};

/**
 * 文本节点
 */
class TextNode : public CHTLASTNode {
public:
    std::string content;
    bool isQuoted;
    
    TextNode() : CHTLASTNode(CHTLASTNodeType::TEXT_NODE), isQuoted(false) {}
    explicit TextNode(const std::string& text, bool quoted = false)
        : CHTLASTNode(CHTLASTNodeType::TEXT_NODE), content(text), isQuoted(quoted) {}
};

/**
 * 元素节点
 */
class ElementNode : public CHTLASTNode {
public:
    std::string tagName;
    std::vector<std::pair<std::string, std::string>> attributes;
    std::shared_ptr<CHTLASTNode> styleBlock;
    
    ElementNode() : CHTLASTNode(CHTLASTNodeType::ELEMENT) {}
    explicit ElementNode(const std::string& tag)
        : CHTLASTNode(CHTLASTNodeType::ELEMENT), tagName(tag) {}
};

/**
 * 样式块节点
 */
class StyleBlockNode : public CHTLASTNode {
public:
    std::vector<std::pair<std::string, std::string>> properties;
    std::vector<std::pair<std::string, std::vector<std::pair<std::string, std::string>>>> selectors;
    
    StyleBlockNode() : CHTLASTNode(CHTLASTNodeType::STYLE_BLOCK) {}
};

/**
 * 模板节点基类
 */
class TemplateNode : public CHTLASTNode {
public:
    std::string name;
    std::vector<std::string> parameters;
    std::vector<std::shared_ptr<CHTLASTNode>> inheritances;
    
    explicit TemplateNode(CHTLASTNodeType type) : CHTLASTNode(type) {}
};

/**
 * 解析错误信息
 */
struct ParseError {
    std::string message;
    ast::SourceLocation location;
    int severity;  // 0=error, 1=warning, 2=info
    
    ParseError(const std::string& msg, const ast::SourceLocation& loc, int sev = 0)
        : message(msg), location(loc), severity(sev) {}
};

/**
 * 解析结果
 */
struct ParseResult {
    bool success;
    std::shared_ptr<CHTLASTNode> rootNode;
    std::vector<ParseError> errors;
    std::vector<ParseError> warnings;
    
    ParseResult() : success(false) {}
};

/**
 * CHTL基础解析器
 * 支持完整的CHTL语法解析
 */
class CHTLBasicParser {
public:
    CHTLBasicParser();
    ~CHTLBasicParser();
    
    /**
     * 解析CHTL源代码
     * @param source 源代码字符串
     * @param filename 文件名（用于错误报告）
     * @return 解析结果
     */
    ParseResult parse(const std::string& source, const std::string& filename = "");
    
    /**
     * 解析文件
     * @param filename 文件路径
     * @return 解析结果
     */
    ParseResult parseFile(const std::string& filename);
    
    /**
     * 设置解析选项
     */
    void setStrictMode(bool strict) { strictMode_ = strict; }
    void setValidateHtml(bool validate) { validateHtml_ = validate; }
    void setAllowExperimentalFeatures(bool allow) { allowExperimental_ = allow; }

private:
    std::unique_ptr<ast::Lexer> lexer_;
    bool strictMode_;
    bool validateHtml_;
    bool allowExperimental_;
    
    // 解析状态
    ast::Token currentToken_;
    std::vector<ParseError> errors_;
    std::vector<ParseError> warnings_;
    std::string currentFile_;
    
    // 解析方法
    std::shared_ptr<CHTLASTNode> parseProgram();
    std::shared_ptr<CHTLASTNode> parseStatement();
    
    // 注释解析
    std::shared_ptr<CHTLASTNode> parseComment();
    std::shared_ptr<CHTLASTNode> parseSingleLineComment();
    std::shared_ptr<CHTLASTNode> parseMultiLineComment();
    std::shared_ptr<CHTLASTNode> parseGeneratorComment();
    
    // 文本和元素解析
    std::shared_ptr<CHTLASTNode> parseTextNode();
    std::shared_ptr<CHTLASTNode> parseElement();
    std::shared_ptr<CHTLASTNode> parseAttributes(ElementNode* element);
    
    // 样式解析
    std::shared_ptr<CHTLASTNode> parseStyleBlock();
    std::shared_ptr<CHTLASTNode> parseStyleProperty();
    std::shared_ptr<CHTLASTNode> parseSelector();
    
    // CHTL特殊结构解析
    std::shared_ptr<CHTLASTNode> parseTemplateDefinition();
    std::shared_ptr<CHTLASTNode> parseCustomDefinition();
    std::shared_ptr<CHTLASTNode> parseNamespaceDefinition();
    std::shared_ptr<CHTLASTNode> parseImportStatement();
    std::shared_ptr<CHTLASTNode> parseConfigurationBlock();
    std::shared_ptr<CHTLASTNode> parseOriginBlock();
    
    // 表达式解析
    std::shared_ptr<CHTLASTNode> parseTemplateUsage();
    std::shared_ptr<CHTLASTNode> parseVariableAccess();
    std::shared_ptr<CHTLASTNode> parseInheritance();
    std::shared_ptr<CHTLASTNode> parseSpecialization();
    
    // 工具方法
    void advance();
    bool match(ast::TokenType type);
    bool check(ast::TokenType type);
    ast::Token consume(ast::TokenType type, const std::string& message);
    bool isAtEnd();
    
    void addError(const std::string& message);
    void addWarning(const std::string& message);
    
    bool isValidHtmlTag(const std::string& tagName);
    bool isValidCssProperty(const std::string& property);
    
    std::string parseStringValue();
    std::string parseIdentifier();
    
    // 验证方法
    void validateElement(ElementNode* element);
    void validateStyleProperty(const std::string& property, const std::string& value);
    void validateTemplateDefinition(TemplateNode* templateNode);
};

} // namespace chtl
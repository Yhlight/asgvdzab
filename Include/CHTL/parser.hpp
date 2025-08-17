#pragma once

#include "lexer.hpp"
#include "ast_nodes.hpp"
#include "ast_visitor.hpp"
#include <memory>
#include <stack>

namespace chtl {
namespace ast {

/**
 * 解析器错误信息
 */
struct ParseError {
    std::string message;
    SourceLocation location;
    TokenType expectedType;
    TokenType actualType;
};

/**
 * CHTL语法分析器
 */
class Parser {
public:
    explicit Parser(std::unique_ptr<Lexer> lexer);
    
    /**
     * 解析完整的CHTL程序
     * @return 程序AST根节点
     */
    std::unique_ptr<Program> parseProgram();
    
    /**
     * 解析单个语句
     * @return AST节点
     */
    ASTNodePtr parseStatement();
    
    /**
     * 获取解析错误
     */
    const std::vector<ParseError>& getErrors() const { return errors_; }
    bool hasErrors() const { return !errors_.empty(); }
    
    /**
     * 设置错误恢复模式
     */
    void setErrorRecovery(bool enabled) { errorRecovery_ = enabled; }

private:
    std::unique_ptr<Lexer> lexer_;
    Token currentToken_;
    std::vector<ParseError> errors_;
    bool errorRecovery_;
    
    // 解析上下文
    struct ParseContext {
        enum Type {
            GLOBAL,         // 全局作用域
            NAMESPACE,      // 命名空间内
            TEMPLATE,       // 模板定义内
            CUSTOM,         // 自定义定义内
            ELEMENT,        // HTML元素内
            STYLE_BLOCK     // 样式块内
        };
        
        Type type;
        std::string name;
        int depth;
    };
    
    std::stack<ParseContext> contextStack_;
    
    /**
     * Token操作
     */
    void advance();
    bool match(TokenType type);
    bool check(TokenType type) const;
    Token consume(TokenType type, const std::string& errorMessage);
    Token previous() const;
    bool isAtEnd() const;
    
    /**
     * 错误处理
     */
    void addError(const std::string& message, const SourceLocation& location = {});
    void addError(const std::string& message, TokenType expected, TokenType actual);
    void synchronize();
    void recover();
    
    /**
     * 解析上下文管理
     */
    void pushContext(ParseContext::Type type, const std::string& name = "");
    void popContext();
    ParseContext::Type getCurrentContextType() const;
    
    /**
     * 主要解析方法
     */
    ASTNodePtr parseTopLevelStatement();
    
    // CHTL特殊结构解析
    std::shared_ptr<TemplateStyle> parseTemplateStyle();
    std::shared_ptr<TemplateElement> parseTemplateElement();
    std::shared_ptr<TemplateVar> parseTemplateVar();
    std::shared_ptr<CustomStyle> parseCustomStyle();
    std::shared_ptr<Namespace> parseNamespace();
    std::shared_ptr<Import> parseImport();
    std::shared_ptr<Configuration> parseConfiguration();
    std::shared_ptr<Constraint> parseConstraint();
    
    // HTML元素解析
    std::shared_ptr<Element> parseElement();
    std::shared_ptr<Element> parseElementWithName(const std::string& tagName);
    std::shared_ptr<Attribute> parseAttribute();
    std::shared_ptr<TextNode> parseTextNode();
    
    // 样式解析
    std::shared_ptr<StyleBlock> parseStyleBlock();
    std::shared_ptr<StyleProperty> parseStyleProperty();
    std::shared_ptr<Selector> parseSelector();
    std::shared_ptr<Selector> parseCssSelector();
    
    // 模板使用和继承
    std::shared_ptr<TemplateUsage> parseTemplateUsage();
    std::shared_ptr<TemplateUsage> parseInheritance();
    std::shared_ptr<Specialization> parseSpecialization();
    
    // 表达式解析
    std::shared_ptr<VariableAccess> parseVariableAccess();
    std::shared_ptr<Literal> parseLiteral();
    std::shared_ptr<Identifier> parseIdentifier();
    
    // 注释解析
    std::shared_ptr<Comment> parseComment();
    
    /**
     * 辅助解析方法
     */
    std::string parseStringValue();
    std::string parseUnquotedValue();
    std::string parseIdentifierValue();
    std::vector<std::string> parseParameterList();
    std::unordered_map<std::string, std::string> parseArgumentList();
    
    // 导入路径解析
    Import::ImportType parseImportType();
    std::string parseImportPath();
    
    // 模板参数解析
    std::vector<std::string> parseTemplateParameters();
    std::unordered_map<std::string, std::string> parseTemplateArguments();
    
    // 特例化操作解析
    Specialization::SpecType parseSpecializationType();
    
    /**
     * 验证方法
     */
    bool isValidElementContext() const;
    bool isValidStyleContext() const;
    bool isValidTemplateContext() const;
    bool isValidNamespaceContext() const;
    
    /**
     * 类型检查
     */
    bool isHtmlElement(const std::string& name) const;
    bool isCssProperty(const std::string& name) const;
    bool isValidIdentifier(const std::string& name) const;
    
    /**
     * 同步和恢复
     */
    void synchronizeToNextStatement();
    void synchronizeToClosingBrace();
    void skipToToken(TokenType type);
    
    /**
     * 特殊情况处理
     */
    bool isStartOfStatement() const;
    bool isStartOfExpression() const;
    bool isStartOfElement() const;
    bool isStartOfTemplate() const;
    bool isStartOfCustom() const;
    
    /**
     * 调试辅助
     */
    std::string getTokenSequence(int count = 5) const;
    void logParseState(const std::string& method) const;
};

/**
 * 解析器工厂
 */
class ParserFactory {
public:
    /**
     * 从源代码创建解析器
     */
    static std::unique_ptr<Parser> fromSource(const std::string& source, const std::string& filename = "");
    
    /**
     * 从文件创建解析器
     */
    static std::unique_ptr<Parser> fromFile(const std::string& filename);
    
    /**
     * 创建带有自定义词法分析器的解析器
     */
    static std::unique_ptr<Parser> withLexer(std::unique_ptr<Lexer> lexer);
};

/**
 * 解析器配置
 */
struct ParserConfig {
    bool enableErrorRecovery = true;
    bool strictModeEnabled = false;
    bool allowExperimentalFeatures = false;
    bool validateHtmlTags = true;
    bool validateCssProperties = true;
    int maxErrorCount = 100;
    
    // 兼容性设置
    bool allowDeprecatedSyntax = false;
    bool allowMixedQuoteStyles = true;
    bool allowTrailingSemicolons = true;
};

/**
 * 可配置的解析器
 */
class ConfigurableParser : public Parser {
public:
    explicit ConfigurableParser(std::unique_ptr<Lexer> lexer, const ParserConfig& config = {});
    
    void setConfig(const ParserConfig& config) { config_ = config; }
    const ParserConfig& getConfig() const { return config_; }

private:
    ParserConfig config_;
    
    // 重写基类方法以应用配置
    bool shouldValidateHtmlTag(const std::string& tag) const;
    bool shouldValidateCssProperty(const std::string& property) const;
    bool shouldAllowDeprecatedSyntax() const;
};

} // namespace ast
} // namespace chtl
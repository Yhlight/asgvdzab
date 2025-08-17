#pragma once

#include "Common.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace CHTL {

/**
 * Token类型枚举
 */
enum class TokenType {
    // 文件结束和错误
    END_OF_FILE,
    INVALID,
    UNKNOWN,
    
    // 基础字面量
    IDENTIFIER,         // 标识符
    STRING_LITERAL,     // "字符串"
    SINGLE_STRING,      // '字符串'
    UNQUOTED_LITERAL,   // 无引号字符串
    NUMBER_LITERAL,     // 数字
    BOOLEAN_LITERAL,    // true/false
    
    // CHTL关键字
    TEXT,               // text
    STYLE,              // style
    TEMPLATE,           // Template
    CUSTOM,             // Custom
    NAMESPACE,          // Namespace
    IMPORT,             // Import
    CONFIGURATION,      // Configuration
    ORIGIN,             // Origin
    FROM,               // from
    AS,                 // as
    INHERIT,            // inherit
    EXCEPT,             // except
    DELETE,             // delete
    ADD,                // add
    TRUE,               // true
    FALSE,              // false
    NULL_TOKEN,         // null
    
    // 分隔符和操作符
    LEFT_BRACE,         // {
    RIGHT_BRACE,        // }
    LEFT_BRACKET,       // [
    RIGHT_BRACKET,      // ]
    LEFT_PAREN,         // (
    RIGHT_PAREN,        // )
    SEMICOLON,          // ;
    COLON,              // :
    EQUAL,              // =
    DOT,                // .
    COMMA,              // ,
    AMPERSAND,          // &
    HASH,               // #
    AT,                 // @
    ARROW,              // ->
    DOUBLE_COLON,       // ::
    QUESTION,           // ?
    EXCLAMATION,        // !
    
    // CSS选择器特殊token
    CLASS_SELECTOR,     // .className
    ID_SELECTOR,        // #idName
    PSEUDO_CLASS,       // :hover
    PSEUDO_ELEMENT,     // ::before
    ATTRIBUTE_SELECTOR, // [attr=value]
    
    // 注释
    SINGLE_LINE_COMMENT,    // //
    MULTI_LINE_COMMENT,     // /* */
    GENERATOR_COMMENT,      // --
    
    // 特殊标记
    NEWLINE,            // \n
    WHITESPACE,         // 空白字符
    INDENT,             // 缩进
    DEDENT,             // 取消缩进
    
    // 模板和自定义相关
    TEMPLATE_PARAMETER, // 模板参数
    TEMPLATE_ARGUMENT,  // 模板参数
    VARIABLE_ACCESS,    // 变量访问
    
    // 约束系统
    CONSTRAINT_EXCEPT,  // except约束
    CONSTRAINT_TYPE,    // 类型约束
    
    // 字符串插值
    STRING_INTERPOLATION_START,  // ${
    STRING_INTERPOLATION_END,    // }
    
    // 导入路径
    IMPORT_PATH,        // 导入路径
    MODULE_PATH,        // 模块路径
    
    // 特殊值
    UNDEFINED_VALUE,    // undefined
    DEFAULT_VALUE       // default
};

/**
 * Token结构
 */
struct Token {
    TokenType type;
    std::string value;
    SourceLocation location;
    std::optional<std::string> originalText;  // 原始文本（用于保持格式）
    
    Token() : type(TokenType::INVALID) {}
    
    Token(TokenType t, const std::string& v, const SourceLocation& loc)
        : type(t), value(v), location(loc) {}
    
    Token(TokenType t, const std::string& v, const SourceLocation& loc, const std::string& orig)
        : type(t), value(v), location(loc), originalText(orig) {}
    
    bool isValid() const { 
        return type != TokenType::INVALID && type != TokenType::UNKNOWN; 
    }
    
    bool isLiteral() const;
    bool isKeyword() const;
    bool isOperator() const;
    bool isDelimiter() const;
    bool isComment() const;
    
    std::string toString() const;
    std::string getTypeName() const;
};

/**
 * 词法分析器配置
 */
struct LexerConfig {
    bool skipWhitespace = true;         // 跳过空白字符
    bool skipComments = false;          // 跳过注释
    bool trackIndentation = false;      // 跟踪缩进
    bool allowUnicodeIdentifiers = true; // 允许Unicode标识符
    bool strictStringEscaping = true;   // 严格字符串转义
    bool enableStringInterpolation = true; // 启用字符串插值
    bool preserveOriginalText = false;  // 保存原始文本
    
    // 自定义关键字（用于多语言支持）
    std::unordered_map<std::string, TokenType> customKeywords;
    
    // 允许的标识符字符集
    std::unordered_set<char> additionalIdentifierChars;
};

/**
 * CHTL词法分析器
 */
class Lexer {
public:
    explicit Lexer(const std::string& source, const std::string& fileName = "");
    explicit Lexer(const std::string& source, const LexerConfig& config, const std::string& fileName = "");
    
    /**
     * 获取下一个token
     */
    Token nextToken();
    
    /**
     * 查看下一个token但不消费
     */
    Token peekToken(int offset = 0);
    
    /**
     * 获取当前位置
     */
    SourceLocation getCurrentLocation() const;
    
    /**
     * 检查是否到达文件末尾
     */
    bool isAtEnd() const;
    
    /**
     * 获取所有tokens（用于调试和测试）
     */
    std::vector<Token> tokenizeAll();
    
    /**
     * 配置访问
     */
    void setConfig(const LexerConfig& config) { config_ = config; }
    const LexerConfig& getConfig() const { return config_; }
    
    /**
     * 错误管理
     */
    const std::vector<CompileError>& getErrors() const { return errors_; }
    bool hasErrors() const { return !errors_.empty(); }
    void clearErrors() { errors_.clear(); }
    
    /**
     * 统计信息
     */
    struct Statistics {
        size_t totalTokens = 0;
        size_t totalLines = 0;
        size_t totalCharacters = 0;
        size_t comments = 0;
        size_t literals = 0;
        size_t keywords = 0;
        size_t identifiers = 0;
        double lexingTimeMs = 0.0;
    };
    
    const Statistics& getStatistics() const { return stats_; }

private:
    std::string source_;
    std::string fileName_;
    LexerConfig config_;
    size_t position_;
    size_t line_;
    size_t column_;
    std::vector<CompileError> errors_;
    Statistics stats_;
    
    // Token缓存（用于peekToken）
    std::vector<Token> tokenCache_;
    size_t cachePosition_;
    
    // 关键字映射
    static std::unordered_map<std::string, TokenType> keywords_;
    static std::unordered_set<std::string> htmlTags_;
    static std::unordered_set<std::string> cssProperties_;
    
    // 缩进栈（用于Python风格的缩进处理）
    std::vector<int> indentStack_;
    
    /**
     * 字符操作
     */
    char currentChar() const;
    char peekChar(size_t offset = 1) const;
    void advance();
    bool match(char expected);
    bool match(const std::string& expected);
    
    /**
     * 跳过操作
     */
    void skipWhitespace();
    void skipSingleLineComment();
    void skipMultiLineComment();
    void handleIndentation();
    
    /**
     * Token生成
     */
    Token makeToken(TokenType type, const std::string& value = "");
    Token makeStringLiteral();
    Token makeSingleStringLiteral();
    Token makeUnquotedLiteral();
    Token makeNumberLiteral();
    Token makeIdentifierOrKeyword();
    Token makeGeneratorComment();
    Token makeCssSelector();
    Token makePseudoSelector();
    Token makeAttributeSelector();
    Token makeStringInterpolation();
    
    /**
     * 字符判断
     */
    bool isAlpha(char c) const;
    bool isDigit(char c) const;
    bool isAlphaNumeric(char c) const;
    bool isHexDigit(char c) const;
    bool isOctalDigit(char c) const;
    bool isBinaryDigit(char c) const;
    bool isWhitespace(char c) const;
    bool isNewline(char c) const;
    bool isValidUnquotedChar(char c) const;
    bool isValidIdentifierStart(char c) const;
    bool isValidIdentifierChar(char c) const;
    bool isUnicodeIdentifierStart(char c) const;
    bool isUnicodeIdentifierPart(char c) const;
    
    /**
     * 特殊处理
     */
    bool isHtmlTag(const std::string& name) const;
    bool isCssProperty(const std::string& name) const;
    bool isKeywordInContext(const std::string& name) const;
    
    /**
     * 错误处理
     */
    void addError(CompileError::Type type, const std::string& message, 
                  const std::string& suggestion = "");
    Token makeErrorToken(const std::string& message);
    
    /**
     * 位置管理
     */
    void updatePosition(char c);
    SourceLocation makeLocation() const;
    
    /**
     * 状态管理
     */
    void resetState();
    void updateStatistics();
    
    /**
     * 字符串处理
     */
    std::string processEscapeSequences(const std::string& str);
    char processEscapeSequence(char c);
    std::string processUnicodeEscape(const std::string& sequence);
    
    /**
     * 数字处理
     */
    bool isValidNumber(const std::string& number) const;
    std::string normalizeNumber(const std::string& number) const;
};

/**
 * Token工具函数
 */
namespace TokenUtils {
    std::string getTokenTypeName(TokenType type);
    std::string tokenTypeToString(TokenType type);
    TokenType stringToTokenType(const std::string& str);
    
    bool isKeyword(TokenType type);
    bool isLiteral(TokenType type);
    bool isOperator(TokenType type);
    bool isDelimiter(TokenType type);
    bool isComment(TokenType type);
    bool isSelector(TokenType type);
    
    std::vector<std::string> getCHTLKeywords();
    std::vector<std::string> getHtmlTags();
    std::vector<std::string> getCssProperties();
    
    bool isValidHtmlTag(const std::string& tag);
    bool isValidCssProperty(const std::string& property);
    bool isValidIdentifier(const std::string& identifier);
    
    std::string formatTokenSequence(const std::vector<Token>& tokens);
    std::string highlightTokens(const std::vector<Token>& tokens);
}

/**
 * 词法分析器工厂
 */
class LexerFactory {
public:
    static std::unique_ptr<Lexer> createFromSource(const std::string& source, 
                                                   const std::string& fileName = "");
    static std::unique_ptr<Lexer> createFromFile(const std::string& fileName);
    static std::unique_ptr<Lexer> createWithConfig(const std::string& source, 
                                                   const LexerConfig& config,
                                                   const std::string& fileName = "");
    
    // 预定义配置
    static LexerConfig getDefaultConfig();
    static LexerConfig getStrictConfig();
    static LexerConfig getDebugConfig();
    static LexerConfig getMinimalConfig();
};

} // namespace CHTL
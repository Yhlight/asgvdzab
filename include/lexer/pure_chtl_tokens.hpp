#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "common/types.hpp"

namespace chtl {

/**
 * 纯CHTL Token类型 - 仅供CHTL编译器使用
 * 严格按照CHTL语法文档，不包含CHTL JS、CSS或JavaScript内容
 */
enum class PureCHTLTokenType {
    // 基础字面量
    IDENTIFIER,           // 标识符
    STRING_DOUBLE,        // 双引号字符串 ""
    STRING_SINGLE,        // 单引号字符串 ''
    STRING_UNQUOTED,      // 无修饰字面量（无引号字符串）

    // 分隔符和操作符
    LBRACE,              // {
    RBRACE,              // }
    LBRACKET,            // [
    RBRACKET,            // ]
    LPAREN,              // (
    RPAREN,              // )
    SEMICOLON,           // ;
    COLON_EQUALS,        // : 或 = (CE对等式：完全等价)
    COMMA,               // ,
    DOT,                 // .
    HASH,                // #

    // CHTL特有关键字（不包含JavaScript相关）
    KEYWORD_TEXT,        // text
    KEYWORD_STYLE,       // style  (局部样式块 - 纯CSS内容)
    KEYWORD_SCRIPT,      // script (局部脚本块 - 可能是CHTL JS或普通JS)
    KEYWORD_INHERIT,     // inherit
    KEYWORD_DELETE,      // delete
    KEYWORD_INSERT,      // insert
    KEYWORD_AFTER,       // after
    KEYWORD_BEFORE,      // before
    KEYWORD_REPLACE,     // replace
    KEYWORD_AT_TOP,      // "at top"
    KEYWORD_AT_BOTTOM,   // "at bottom"
    KEYWORD_FROM,        // from
    KEYWORD_AS,          // as
    KEYWORD_EXCEPT,      // except

    // 特殊结构标记
    KEYWORD_TEMPLATE,    // [Template]
    KEYWORD_CUSTOM,      // [Custom]
    KEYWORD_ORIGIN,      // [Origin]
    KEYWORD_IMPORT,      // [Import]
    KEYWORD_NAMESPACE,   // [Namespace]
    KEYWORD_CONFIGURATION, // [Configuration]
    KEYWORD_INFO,        // [Info] (CMOD用)
    KEYWORD_EXPORT,      // [Export] (CMOD用)
    KEYWORD_NAME,        // [Name] (配置组用)

    // CHTL类型前缀
    PREFIX_STYLE,        // @Style
    PREFIX_ELEMENT,      // @Element
    PREFIX_VAR,          // @Var
    PREFIX_HTML,         // @Html
    PREFIX_JAVASCRIPT,   // @JavaScript
    PREFIX_CHTL,         // @Chtl
    PREFIX_CJMOD,        // @CJmod

    // 注释
    COMMENT_LINE,        // //
    COMMENT_MULTILINE,   // /* */
    COMMENT_GENERATOR,   // -- (被生成器识别的注释)

    // 特殊字符
    WHITESPACE,          // 空白字符
    NEWLINE,             // 换行符

    // 结束标记
    EOF_TOKEN,           // 文件结束
    UNKNOWN              // 未知token
};

/**
 * 纯CHTL Token结构
 */
struct PureCHTLToken {
    PureCHTLTokenType type;
    std::string value;        // token的文本值
    Position position;        // 在源码中的位置
    size_t length;           // token长度

    // 语义信息
    struct SemanticInfo {
        bool isKeyword = false;
        bool isLiteral = false;
        bool isOperator = false;
        bool isSeparator = false;
        bool isComment = false;
        bool isWhitespace = false;
        bool isPrefixType = false;     // 是否为类型前缀
        bool isStructuralKeyword = false; // 是否为结构关键字
    } semantic;

    PureCHTLToken() = default;

    PureCHTLToken(PureCHTLTokenType t, const std::string& v, Position pos, size_t len = 0)
        : type(t), value(v), position(pos), length(len == 0 ? v.length() : len) {
        updateSemanticInfo();
    }

    void updateSemanticInfo();

    // 工具方法
    bool isLiteral() const;
    bool isKeyword() const;
    bool isOperator() const;
    bool isSeparator() const;
    bool isComment() const;
    bool isWhitespace() const;
    bool isPrefixType() const;
    bool isStructuralKeyword() const;
    std::string toString() const;
};

/**
 * 纯CHTL关键字映射表
 */
class PureCHTLKeywordMap {
public:
    PureCHTLKeywordMap();

    // 根据配置组更新关键字映射
    void updateFromConfiguration(const std::unordered_map<std::string, std::string>& config);

    // 检查是否为关键字
    bool isKeyword(const std::string& word) const;

    // 获取关键字对应的token类型
    PureCHTLTokenType getKeywordType(const std::string& word) const;

    // 获取所有关键字
    std::vector<std::string> getAllKeywords() const;

    // 检查特定类型的关键字
    bool isTemplateKeyword(const std::string& word) const;
    bool isCustomKeyword(const std::string& word) const;
    bool isOriginKeyword(const std::string& word) const;
    bool isImportKeyword(const std::string& word) const;
    bool isNamespaceKeyword(const std::string& word) const;
    bool isConfigurationKeyword(const std::string& word) const;

private:
    std::unordered_map<std::string, PureCHTLTokenType> keywordMap_;
    void initializeDefaultKeywords();
    
    // 可配置的关键字组
    std::unordered_map<std::string, std::vector<std::string>> configurableKeywords_;
};

/**
 * 纯CHTL前缀映射表
 */
class PureCHTLPrefixMap {
public:
    PureCHTLPrefixMap();

    // 检查是否为有效前缀
    bool isPrefix(const std::string& prefix) const;

    // 获取前缀对应的token类型
    PureCHTLTokenType getPrefixType(const std::string& prefix) const;

    // 根据配置更新前缀映射
    void updateFromConfiguration(const std::unordered_map<std::string, std::string>& config);

private:
    std::unordered_map<std::string, PureCHTLTokenType> prefixMap_;
    void initializeDefaultPrefixes();
};

/**
 * 纯CHTL Token工具类
 */
class PureCHTLTokenUtils {
public:
    // 检查字符类型
    static bool isIdentifierStart(char ch);
    static bool isIdentifierChar(char ch);
    static bool isWhitespace(char ch);
    static bool isNewline(char ch);
    static bool isStringDelimiter(char ch);

    // 字符串处理
    static std::string unescapeString(const std::string& str);
    static std::string escapeString(const std::string& str);

    // Token类型转换
    static std::string tokenTypeToString(PureCHTLTokenType type);
    static bool isLiteralType(PureCHTLTokenType type);
    static bool isKeywordType(PureCHTLTokenType type);
    static bool isOperatorType(PureCHTLTokenType type);
    static bool isSeparatorType(PureCHTLTokenType type);
    static bool isCommentType(PureCHTLTokenType type);
    static bool isWhitespaceType(PureCHTLTokenType type);
    static bool isPrefixType(PureCHTLTokenType type);
    static bool isStructuralKeywordType(PureCHTLTokenType type);

    // 特殊字符序列检查
    static bool startsMultipleCharSequence(char ch);
    static std::string getMultipleCharSequence(const std::string& source, size_t pos);
};

} // namespace chtl
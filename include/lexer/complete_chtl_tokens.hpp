#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "common/types.hpp"

namespace chtl {

/**
 * 完整的CHTL Token类型定义
 * 严格按照CHTL语法文档.md中的语法规范
 */
enum class CompleteCHTLTokenType {
    // ===== 基础字面量 =====
    IDENTIFIER,           // 标识符
    STRING_DOUBLE,        // 双引号字符串 ""
    STRING_SINGLE,        // 单引号字符串 ''
    STRING_UNQUOTED,      // 无修饰字面量（无引号字符串）- CHTL特色
    
    // ===== 分隔符和操作符 =====
    LBRACE,              // {
    RBRACE,              // }
    LBRACKET,            // [
    RBRACKET,            // ]
    LPAREN,              // (
    RPAREN,              // )
    SEMICOLON,           // ;
    COLON,               // :
    EQUALS,              // =
    COLON_EQUALS,        // : 或 = (CE对等式：完全等价)
    COMMA,               // ,
    DOT,                 // .
    SLASH,               // /
    HASH,                // #
    AMPERSAND,           // &
    
    // ===== CHTL核心关键字 =====
    KEYWORD_TEXT,        // text - 文本节点
    KEYWORD_STYLE,       // style - 局部样式块
    KEYWORD_SCRIPT,      // script - 局部脚本块
    
    // ===== 模板和自定义关键字 =====
    KEYWORD_INHERIT,     // inherit - 显性继承
    KEYWORD_DELETE,      // delete - 删除属性/元素/继承
    KEYWORD_INSERT,      // insert - 插入元素
    KEYWORD_AFTER,       // after - 在...之后
    KEYWORD_BEFORE,      // before - 在...之前
    KEYWORD_REPLACE,     // replace - 替换
    KEYWORD_AT_TOP,      // "at top" - 在顶部
    KEYWORD_AT_BOTTOM,   // "at bottom" - 在底部
    
    // ===== 导入和命名空间关键字 =====
    KEYWORD_FROM,        // from - 导入来源
    KEYWORD_AS,          // as - 别名
    KEYWORD_EXCEPT,      // except - 约束关键字
    
    // ===== 特殊结构标记 =====
    KEYWORD_TEMPLATE,    // [Template] - 模板定义
    KEYWORD_CUSTOM,      // [Custom] - 自定义定义
    KEYWORD_ORIGIN,      // [Origin] - 原始嵌入
    KEYWORD_IMPORT,      // [Import] - 导入声明
    KEYWORD_NAMESPACE,   // [Namespace] - 命名空间
    KEYWORD_CONFIGURATION, // [Configuration] - 配置组
    KEYWORD_INFO,        // [Info] - CMOD模块信息
    KEYWORD_EXPORT,      // [Export] - CMOD导出表
    KEYWORD_NAME,        // [Name] - 配置组名称块
    
    // ===== CHTL类型前缀 =====
    PREFIX_STYLE,        // @Style - 样式组类型
    PREFIX_ELEMENT,      // @Element - 元素类型
    PREFIX_VAR,          // @Var - 变量组类型
    PREFIX_HTML,         // @Html - HTML原始嵌入/导入
    PREFIX_JAVASCRIPT,   // @JavaScript - JS原始嵌入/导入
    PREFIX_CHTL,         // @Chtl - CHTL文件导入
    PREFIX_CJMOD,        // @CJmod - CJmod模块导入
    
    // ===== 注释类型 =====
    COMMENT_LINE,        // // - 单行注释（不被生成器识别）
    COMMENT_MULTILINE,   // /* */ - 多行注释（不被生成器识别）
    COMMENT_GENERATOR,   // -- - 生成器注释（被生成器识别）
    
    // ===== HTML元素标签 =====
    // 根据语法文档，CHTL支持所有HTML元素
    HTML_TAG,            // HTML标签名（动态识别）
    
    // ===== CSS相关Token =====
    CSS_PROPERTY,        // CSS属性名
    CSS_VALUE,           // CSS属性值
    CSS_SELECTOR_CLASS,  // .class - 类选择器
    CSS_SELECTOR_ID,     // #id - ID选择器
    CSS_PSEUDO_CLASS,    // :hover等伪类
    CSS_PSEUDO_ELEMENT,  // ::before等伪元素
    
    // ===== CHTL JS相关Token =====
    CHTL_JS_SELECTOR,    // {{选择器}} - 增强选择器
    CHTL_JS_ARROW,       // -> - CHTL JS链式操作符
    CHTL_JS_LISTEN,      // listen - 增强监听器
    CHTL_JS_DELEGATE,    // delegate - 事件委托
    CHTL_JS_ANIMATE,     // animate - 动画函数
    
    // ===== 索引访问 =====
    INDEX_ACCESS,        // [0], [1] - 索引访问语法
    
    // ===== 特殊字符 =====
    WHITESPACE,          // 空白字符
    NEWLINE,             // 换行符
    TAB,                 // 制表符
    
    // ===== 结束和错误标记 =====
    EOF_TOKEN,           // 文件结束
    UNKNOWN,             // 未知token
    ERROR_TOKEN          // 错误token
};

/**
 * 完整的CHTL Token结构
 */
struct CompleteCHTLToken {
    CompleteCHTLTokenType type;
    std::string value;        // token的文本值
    Position position;        // 在源码中的位置
    size_t length;           // token长度
    
    // 详细的语义信息
    struct SemanticInfo {
        bool isKeyword = false;
        bool isLiteral = false;
        bool isOperator = false;
        bool isSeparator = false;
        bool isComment = false;
        bool isWhitespace = false;
        bool isPrefixType = false;        // 是否为类型前缀(@Style等)
        bool isStructuralKeyword = false; // 是否为结构关键字([Template]等)
        bool isHTMLElement = false;       // 是否为HTML元素
        bool isCSSRelated = false;        // 是否为CSS相关
        bool isCHTLJSRelated = false;     // 是否为CHTL JS相关
        bool isConfigurable = false;      // 是否可通过配置组自定义
        
        // 上下文信息
        std::string context;              // 所在上下文（如在style块内、script块内等）
        int nestingLevel = 0;            // 嵌套层级
    } semantic;
    
    CompleteCHTLToken() = default;
    
    CompleteCHTLToken(CompleteCHTLTokenType t, const std::string& v, Position pos, size_t len = 0)
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
    bool isHTMLElement() const;
    bool isCSSRelated() const;
    bool isCHTLJSRelated() const;
    bool isConfigurable() const;
    std::string toString() const;
    std::string getContextualInfo() const;
};

/**
 * CHTL关键字映射表 - 支持配置组完全自定义
 * 严格按照语法文档中的[Configuration][Name]块规范
 */
class CompleteCHTLKeywordMap {
public:
    CompleteCHTLKeywordMap();
    
    // 根据配置组更新关键字映射
    void updateFromConfiguration(const std::unordered_map<std::string, std::string>& config);
    void updateFromNameConfiguration(const std::unordered_map<std::string, std::vector<std::string>>& nameConfig);
    
    // 检查是否为关键字
    bool isKeyword(const std::string& word) const;
    
    // 获取关键字对应的token类型
    CompleteCHTLTokenType getKeywordType(const std::string& word) const;
    
    // 获取所有关键字
    std::vector<std::string> getAllKeywords() const;
    
    // 检查特定类型的关键字
    bool isTemplateKeyword(const std::string& word) const;
    bool isCustomKeyword(const std::string& word) const;
    bool isOriginKeyword(const std::string& word) const;
    bool isImportKeyword(const std::string& word) const;
    bool isNamespaceKeyword(const std::string& word) const;
    bool isConfigurationKeyword(const std::string& word) const;
    
    // 检查可配置的关键字组
    bool isCustomStyleKeyword(const std::string& word) const;
    bool isCustomElementKeyword(const std::string& word) const;
    bool isCustomVarKeyword(const std::string& word) const;
    
    // 获取配置的关键字变体
    std::vector<std::string> getKeywordVariants(const std::string& baseKeyword) const;
    
private:
    std::unordered_map<std::string, CompleteCHTLTokenType> keywordMap_;
    
    // 可配置的关键字组 - 支持多个值（组选项）
    std::unordered_map<std::string, std::vector<std::string>> configurableKeywords_;
    
    // 默认关键字映射
    void initializeDefaultKeywords();
    void initializeConfigurableKeywords();
    
    // 配置组限制
    size_t maxOptionCount_ = 3; // OPTION_COUNT配置
};

/**
 * CHTL前缀映射表
 */
class CompleteCHTLPrefixMap {
public:
    CompleteCHTLPrefixMap();
    
    // 检查是否为有效前缀
    bool isPrefix(const std::string& prefix) const;
    
    // 获取前缀对应的token类型
    CompleteCHTLTokenType getPrefixType(const std::string& prefix) const;
    
    // 根据配置更新前缀映射
    void updateFromConfiguration(const std::unordered_map<std::string, std::string>& config);
    
    // 检查特定前缀类型
    bool isStylePrefix(const std::string& prefix) const;
    bool isElementPrefix(const std::string& prefix) const;
    bool isVarPrefix(const std::string& prefix) const;
    bool isOriginPrefix(const std::string& prefix) const;
    bool isImportPrefix(const std::string& prefix) const;
    
private:
    std::unordered_map<std::string, CompleteCHTLTokenType> prefixMap_;
    void initializeDefaultPrefixes();
};

/**
 * HTML元素识别器
 * 识别所有标准HTML元素
 */
class HTMLElementRecognizer {
public:
    HTMLElementRecognizer();
    
    // 检查是否为有效的HTML元素
    bool isHTMLElement(const std::string& tagName) const;
    
    // 检查元素类型
    bool isBlockElement(const std::string& tagName) const;
    bool isInlineElement(const std::string& tagName) const;
    bool isVoidElement(const std::string& tagName) const; // 单标签元素
    
    // 获取所有HTML元素
    std::vector<std::string> getAllHTMLElements() const;
    
private:
    std::unordered_set<std::string> htmlElements_;
    std::unordered_set<std::string> blockElements_;
    std::unordered_set<std::string> inlineElements_;
    std::unordered_set<std::string> voidElements_;
    
    void initializeHTMLElements();
};

/**
 * CSS属性识别器
 */
class CSSPropertyRecognizer {
public:
    CSSPropertyRecognizer();
    
    // 检查是否为有效的CSS属性
    bool isCSSProperty(const std::string& property) const;
    
    // 获取所有CSS属性
    std::vector<std::string> getAllCSSProperties() const;
    
private:
    std::unordered_set<std::string> cssProperties_;
    void initializeCSSProperties();
};

/**
 * Token工具类 - 扩展版本
 */
class CompleteCHTLTokenUtils {
public:
    // 字符类型检查
    static bool isIdentifierStart(char ch);
    static bool isIdentifierChar(char ch);
    static bool isWhitespace(char ch);
    static bool isNewline(char ch);
    static bool isStringDelimiter(char ch);
    static bool isDigit(char ch);
    static bool isAlpha(char ch);
    static bool isAlphaNumeric(char ch);
    
    // 字符串处理
    static std::string unescapeString(const std::string& str);
    static std::string escapeString(const std::string& str);
    static std::string normalizeUnquotedString(const std::string& str);
    
    // Token类型转换
    static std::string tokenTypeToString(CompleteCHTLTokenType type);
    static bool isLiteralType(CompleteCHTLTokenType type);
    static bool isKeywordType(CompleteCHTLTokenType type);
    static bool isOperatorType(CompleteCHTLTokenType type);
    static bool isSeparatorType(CompleteCHTLTokenType type);
    static bool isCommentType(CompleteCHTLTokenType type);
    static bool isWhitespaceType(CompleteCHTLTokenType type);
    
    // 特殊字符序列检查
    static bool startsMultipleCharSequence(char ch);
    static std::string getMultipleCharSequence(const std::string& source, size_t pos);
    
    // CE对等式处理（: 和 = 完全等价）
    static bool isCEEquivalent(char ch);
    static CompleteCHTLTokenType getCETokenType();
    
    // 上下文相关的token识别
    static bool isValidInContext(CompleteCHTLTokenType tokenType, const std::string& context);
    static std::vector<CompleteCHTLTokenType> getValidTokensInContext(const std::string& context);
    
    // 索引访问解析
    static bool isIndexAccess(const std::string& str);
    static int parseIndexAccess(const std::string& str);
    
    // CHTL JS选择器解析
    static bool isCHTLJSSelector(const std::string& str);
    static std::string parseCHTLJSSelector(const std::string& str);
    
    // 配置组相关
    static bool isConfigurationValue(const std::string& str);
    static std::pair<std::string, std::string> parseConfigurationAssignment(const std::string& str);
};

/**
 * Token验证器
 * 验证token序列的语法正确性
 */
class CompleteCHTLTokenValidator {
public:
    // 验证token序列
    static bool validateTokenSequence(const std::vector<CompleteCHTLToken>& tokens);
    
    // 验证特定结构
    static bool validateTemplateStructure(const std::vector<CompleteCHTLToken>& tokens);
    static bool validateCustomStructure(const std::vector<CompleteCHTLToken>& tokens);
    static bool validateImportStructure(const std::vector<CompleteCHTLToken>& tokens);
    static bool validateNamespaceStructure(const std::vector<CompleteCHTLToken>& tokens);
    static bool validateElementStructure(const std::vector<CompleteCHTLToken>& tokens);
    static bool validateStyleBlockStructure(const std::vector<CompleteCHTLToken>& tokens);
    static bool validateScriptBlockStructure(const std::vector<CompleteCHTLToken>& tokens);
    
    // 验证约束
    static bool validateExceptConstraints(const std::vector<CompleteCHTLToken>& tokens);
    
    // 验证配置组
    static bool validateConfigurationBlock(const std::vector<CompleteCHTLToken>& tokens);
    static bool validateNameBlock(const std::vector<CompleteCHTLToken>& tokens);
    
private:
    static bool isValidTokenTransition(CompleteCHTLTokenType from, CompleteCHTLTokenType to);
    static bool isValidInStructure(CompleteCHTLTokenType tokenType, const std::string& structure);
};

} // namespace chtl
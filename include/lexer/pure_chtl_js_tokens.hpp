#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "common/types.hpp"

namespace chtl {

/**
 * 纯CHTL JS Token类型 - 仅供CHTL JS编译器使用
 * 只包含CHTL JS特有的语法扩展，不包含CHTL、CSS或标准JavaScript内容
 */
enum class PureCHTLJSTokenType {
    // 基础字面量 (标准JavaScript部分)
    IDENTIFIER,           // 标识符
    STRING_DOUBLE,        // 双引号字符串 ""
    STRING_SINGLE,        // 单引号字符串 ''
    NUMBER,               // 数字

    // 分隔符和操作符 (标准JavaScript部分)
    LBRACE,              // {
    RBRACE,              // }
    LBRACKET,            // [
    RBRACKET,            // ]
    LPAREN,              // (
    RPAREN,              // )
    SEMICOLON,           // ;
    COLON,               // :
    COMMA,               // ,
    DOT,                 // .

    // === CHTL JS专有操作符 ===
    ARROW_OP,            // ->  (CHTL JS增强：与.完全等价)

    // === CHTL JS增强选择器 ===
    ENHANCED_SELECTOR_START,  // {{
    ENHANCED_SELECTOR_END,    // }}

    // === CHTL JS专有关键字 ===
    KEYWORD_LISTEN,      // listen (增强监听器)
    KEYWORD_DELEGATE,    // delegate (事件委托)
    KEYWORD_ANIMATE,     // animate (动画)

    // JavaScript标准关键字 (CHTL JS中需要识别的部分)
    KEYWORD_FUNCTION,    // function
    KEYWORD_VAR,         // var
    KEYWORD_LET,         // let
    KEYWORD_CONST,       // const
    KEYWORD_IF,          // if
    KEYWORD_ELSE,        // else
    KEYWORD_FOR,         // for
    KEYWORD_WHILE,       // while
    KEYWORD_RETURN,      // return
    KEYWORD_TRUE,        // true
    KEYWORD_FALSE,       // false
    KEYWORD_NULL,        // null
    KEYWORD_UNDEFINED,   // undefined

    // === CHTL JS动画相关关键字 (animate函数内部) ===
    KEYWORD_DURATION,    // duration
    KEYWORD_EASING,      // easing
    KEYWORD_BEGIN,       // begin
    KEYWORD_WHEN,        // when
    KEYWORD_END,         // end
    KEYWORD_AT,          // at
    KEYWORD_LOOP,        // loop
    KEYWORD_DIRECTION,   // direction
    KEYWORD_DELAY,       // delay
    KEYWORD_CALLBACK,    // callback

    // === CHTL JS事件委托关键字 ===
    KEYWORD_TARGET,      // target

    // 注释
    COMMENT_LINE,        // //
    COMMENT_MULTILINE,   // /* */

    // 特殊字符
    WHITESPACE,          // 空白字符
    NEWLINE,             // 换行符

    // 结束标记
    EOF_TOKEN,           // 文件结束
    UNKNOWN              // 未知token
};

/**
 * 纯CHTL JS Token结构
 */
struct PureCHTLJSToken {
    PureCHTLJSTokenType type;
    std::string value;        // token的文本值
    Position position;        // 在源码中的位置
    size_t length;           // token长度

    // 语义信息
    struct SemanticInfo {
        bool isCHTLJSSpecific = false;   // 是否为CHTL JS特有语法
        bool isEnhancedSelector = false; // 是否为增强选择器部分
        bool isAnimationKeyword = false; // 是否为动画关键字
        bool isDelegateKeyword = false;  // 是否为委托关键字
        bool isKeyword = false;
        bool isLiteral = false;
        bool isOperator = false;
        bool isSeparator = false;
        bool isComment = false;
        bool isWhitespace = false;
    } semantic;

    PureCHTLJSToken() = default;

    PureCHTLJSToken(PureCHTLJSTokenType t, const std::string& v, Position pos, size_t len = 0)
        : type(t), value(v), position(pos), length(len == 0 ? v.length() : len) {
        updateSemanticInfo();
    }

    void updateSemanticInfo();

    // 工具方法
    bool isCHTLJSSpecific() const;
    bool isEnhancedSelector() const;
    bool isAnimationKeyword() const;
    bool isDelegateKeyword() const;
    bool isLiteral() const;
    bool isKeyword() const;
    bool isOperator() const;
    bool isSeparator() const;
    bool isComment() const;
    bool isWhitespace() const;
    std::string toString() const;
};

/**
 * 增强选择器类型 (CHTL JS专有)
 */
enum class EnhancedSelectorType {
    TAG_NAME,           // 标签名 (如button)
    CLASS_NAME,         // 类名 (如.box)
    ID_NAME,            // ID (如#box)
    DESCENDANT,         // 后代选择器 (如.box button)
    INDEXED,            // 索引访问 (如button[0])
    AUTO_DETECT         // 自动检测
};

/**
 * 增强选择器结构 (CHTL JS专有)
 */
struct EnhancedSelector {
    EnhancedSelectorType type;
    std::string selector;           // 选择器内容
    Position position;              // 位置信息

    // 解析后的选择器信息
    struct ParsedInfo {
        std::string tagName;        // 标签名
        std::string className;      // 类名
        std::string idName;         // ID名
        std::vector<std::string> descendantPath; // 后代路径
        int index = -1;             // 索引 (-1表示无索引)
        bool hasIndex = false;      // 是否有索引
    } parsed;

    EnhancedSelector() = default;
    EnhancedSelector(const std::string& sel, Position pos);

    // 解析选择器
    void parse();

    // 生成标准CSS选择器
    std::string toCSSSelector() const;

    // 生成DOM查询代码
    std::string toDOMQuery() const;

    std::string toString() const;
};

/**
 * 纯CHTL JS关键字映射表
 */
class PureCHTLJSKeywordMap {
public:
    PureCHTLJSKeywordMap();

    // 检查是否为关键字
    bool isKeyword(const std::string& word) const;

    // 获取关键字对应的token类型
    PureCHTLJSTokenType getKeywordType(const std::string& word) const;

    // 获取所有关键字
    std::vector<std::string> getAllKeywords() const;

    // 检查特定类型的关键字
    bool isCHTLJSKeyword(const std::string& word) const;
    bool isJavaScriptKeyword(const std::string& word) const;
    bool isAnimateKeyword(const std::string& word) const;
    bool isDelegateKeyword(const std::string& word) const;

private:
    std::unordered_map<std::string, PureCHTLJSTokenType> keywordMap_;
    void initializeDefaultKeywords();
};

/**
 * 纯CHTL JS语法特征检测器
 */
class PureCHTLJSSyntaxDetector {
public:
    // 检测CHTL JS特征
    static bool hasEnhancedSelector(const std::string& content);
    static bool hasArrowOperator(const std::string& content);
    static bool hasListenKeyword(const std::string& content);
    static bool hasDelegateKeyword(const std::string& content);
    static bool hasAnimateKeyword(const std::string& content);

    // 检测是否包含任何CHTL JS特征
    static bool hasAnyCHTLJSFeature(const std::string& content);

    // 提取增强选择器
    static std::vector<EnhancedSelector> extractEnhancedSelectors(const std::string& content);

    // 检查是否为有效的增强选择器内容
    static bool isValidSelectorContent(const std::string& content);
};

/**
 * 纯CHTL JS Token工具类
 */
class PureCHTLJSTokenUtils {
public:
    // 检查字符类型
    static bool isIdentifierStart(char ch);
    static bool isIdentifierChar(char ch);
    static bool isWhitespace(char ch);
    static bool isNewline(char ch);
    static bool isStringDelimiter(char ch);
    static bool isDigit(char ch);

    // 字符串处理
    static std::string unescapeString(const std::string& str);
    static std::string escapeString(const std::string& str);

    // Token类型转换
    static std::string tokenTypeToString(PureCHTLJSTokenType type);
    static bool isLiteralType(PureCHTLJSTokenType type);
    static bool isKeywordType(PureCHTLJSTokenType type);
    static bool isOperatorType(PureCHTLJSTokenType type);
    static bool isSeparatorType(PureCHTLJSTokenType type);
    static bool isCommentType(PureCHTLJSTokenType type);
    static bool isWhitespaceType(PureCHTLJSTokenType type);
    static bool isCHTLJSSpecificType(PureCHTLJSTokenType type);

    // 特殊字符序列检查
    static bool startsMultipleCharSequence(char ch);
    static std::string getMultipleCharSequence(const std::string& source, size_t pos);

    // 增强选择器相关
    static bool isValidSelectorChar(char ch);
    static std::string extractSelectorContent(const std::string& source, size_t start);

    // 数字解析
    static bool isValidNumber(const std::string& str);
    static double parseNumber(const std::string& str);
};

/**
 * CHTL JS动画配置解析器
 */
class CHTLJSAnimationConfigParser {
public:
    struct AnimationConfig {
        int duration = 0;                    // 持续时间(ms)
        std::string easing = "ease";         // 缓动函数
        std::unordered_map<std::string, std::string> begin;  // 起始状态
        std::unordered_map<std::string, std::string> end;    // 结束状态

        struct KeyFrame {
            double at;                       // 时间点 (0.0-1.0)
            std::unordered_map<std::string, std::string> properties; // CSS属性
        };
        std::vector<KeyFrame> when;          // 关键帧

        int loop = 1;                        // 循环次数 (-1为无限)
        std::string direction = "normal";    // 播放方向
        int delay = 0;                       // 延迟(ms)
        std::string callback;                // 回调函数
    };

    static AnimationConfig parse(const std::string& configStr);
    static std::string generateAnimationCode(const AnimationConfig& config);
};

/**
 * CHTL JS事件委托配置解析器
 */
class CHTLJSDelegateConfigParser {
public:
    struct DelegateConfig {
        std::string parentSelector;         // 父元素选择器
        std::vector<std::string> targetSelectors; // 目标子元素选择器
        std::unordered_map<std::string, std::string> events; // 事件映射
    };

    static DelegateConfig parse(const std::string& configStr);
    static std::string generateDelegateCode(const DelegateConfig& config);
};

} // namespace chtl
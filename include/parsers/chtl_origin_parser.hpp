#pragma once

#include "ast/chtl_ast.hpp"
#include "lexer/chtl_lexer.hpp"
#include "common/types.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace chtl {

/**
 * 原始嵌入类型枚举
 */
enum class OriginType {
    HTML_ORIGIN,        // @Html 原始HTML嵌入
    STYLE_ORIGIN,       // @Style 原始CSS嵌入
    JAVASCRIPT_ORIGIN   // @JavaScript 原始JS嵌入
};

/**
 * 原始嵌入模式枚举
 */
enum class OriginMode {
    INLINE_EMBED,       // 内联嵌入模式 [Origin] @Html { ... }
    ENHANCED_DECLARE,   // 增强声明模式 [Origin] @Html name { ... }
    ENHANCED_USAGE      // 增强使用模式 [Origin] @Html name;
};

/**
 * 原始代码块定义
 */
struct OriginBlock {
    OriginType type;            // 原始嵌入类型
    OriginMode mode;            // 嵌入模式
    std::string name;           // 名称 (仅用于增强模式)
    std::string content;        // 原始代码内容
    Position position;          // 位置信息
    bool preserveFormatting;    // 是否保持格式化
    
    OriginBlock() : type(OriginType::HTML_ORIGIN), mode(OriginMode::INLINE_EMBED), preserveFormatting(true) {}
    
    OriginBlock(OriginType t, OriginMode m, const std::string& n = "", const Position& pos = Position{})
        : type(t), mode(m), name(n), position(pos), preserveFormatting(true) {}
};

/**
 * 原始嵌入引用 (用于增强使用)
 */
struct OriginReference {
    OriginType type;            // 原始嵌入类型
    std::string name;           // 引用名称
    Position position;          // 位置信息
    
    OriginReference(OriginType t, const std::string& n, const Position& pos = Position{})
        : type(t), name(n), position(pos) {}
};

/**
 * 原始嵌入注册表
 * 管理所有已声明的增强原始嵌入
 */
class OriginRegistry {
public:
    OriginRegistry();
    ~OriginRegistry();

    /**
     * 注册原始嵌入块
     */
    bool registerOriginBlock(const OriginBlock& block);

    /**
     * 查找原始嵌入块
     */
    const OriginBlock* findOriginBlock(OriginType type, const std::string& name) const;

    /**
     * 获取所有原始嵌入块
     */
    const std::unordered_map<std::string, OriginBlock>& getAllOriginBlocks() const;

    /**
     * 清空注册表
     */
    void clear();

    /**
     * 验证原始嵌入引用
     */
    bool validateOriginReference(const OriginReference& ref) const;

private:
    std::unordered_map<std::string, OriginBlock> originBlocks_;
    
    // 生成原始嵌入键名
    std::string generateOriginKey(OriginType type, const std::string& name) const;
};

/**
 * 原始嵌入解析状态
 */
enum class OriginParseState {
    INITIAL,                    // 初始状态
    READING_ORIGIN_HEADER,      // 读取原始嵌入头 ([Origin] @Html)
    READING_ORIGIN_TYPE,        // 读取原始嵌入类型
    READING_ORIGIN_NAME,        // 读取原始嵌入名称 (可选)
    READING_ORIGIN_CONTENT,     // 读取原始嵌入内容
    READING_USAGE              // 读取原始嵌入使用
};

/**
 * 原始嵌入系统上下文
 */
struct OriginContext {
    OriginType currentType;         // 当前原始嵌入类型
    OriginMode currentMode;         // 当前嵌入模式
    std::string currentName;        // 当前嵌入名称
    int nestingLevel;               // 嵌套层级
    bool inContent;                 // 是否在内容中
    std::string elementContext;     // 元素上下文 (所在的HTML元素)
    
    OriginContext() : currentType(OriginType::HTML_ORIGIN), currentMode(OriginMode::INLINE_EMBED), 
                      nestingLevel(0), inContent(false) {}
};

/**
 * 原始嵌入系统扫描器
 * 负责扫描和保持原始代码的完整性
 */
class OriginScanner {
public:
    OriginScanner();
    ~OriginScanner();

    /**
     * 扫描原始嵌入内容
     * @param content 原始内容
     * @param context 原始嵌入上下文
     * @return 扫描结果 (保持原始格式)
     */
    std::string scanOriginContent(const std::string& content, const OriginContext& context);

    /**
     * 验证原始嵌入语法
     * @param content 原始内容
     * @param type 原始嵌入类型
     * @return 是否为有效语法
     */
    bool validateOriginContent(const std::string& content, OriginType type);

private:
    std::string content_;
    size_t currentPos_;
    int line_;
    int column_;
    OriginContext context_;

    // 扫描方法
    char currentChar() const;
    char peekChar(size_t offset = 1) const;
    void advance();
    bool isEOF() const;
    Position getCurrentPosition() const;
    
    // 特定扫描方法
    std::string scanHtmlContent();
    std::string scanCssContent();
    std::string scanJavaScriptContent();
    
    // 格式保持方法
    std::string preserveWhitespace(const std::string& content);
    std::string preserveLineBreaks(const std::string& content);
    std::string preserveIndentation(const std::string& content);
    
    // 验证方法
    bool isValidHtmlContent(const std::string& content) const;
    bool isValidCssContent(const std::string& content) const;
    bool isValidJavaScriptContent(const std::string& content) const;
    
    // 工具方法
    bool isWhitespace(char c) const;
    void skipWhitespace();
};

/**
 * 原始嵌入系统状态机
 */
class OriginStateMachine {
public:
    OriginStateMachine();
    ~OriginStateMachine();

    /**
     * 重置状态机
     */
    void reset();

    /**
     * 处理token
     * @param token 输入token
     * @param context 原始嵌入上下文
     * @return 是否成功处理
     */
    bool processToken(const Token& token, OriginContext& context);

    /**
     * 获取当前状态
     */
    OriginParseState getCurrentState() const;

    /**
     * 获取收集的原始嵌入块
     */
    const std::vector<OriginBlock>& getOriginBlocks() const;

    /**
     * 获取收集的原始嵌入引用
     */
    const std::vector<OriginReference>& getOriginReferences() const;

private:
    OriginParseState currentState_;
    std::vector<OriginBlock> originBlocks_;
    std::vector<OriginReference> originReferences_;
    
    // 临时状态
    OriginType currentType_;
    OriginMode currentMode_;
    std::string currentName_;
    std::string currentContent_;
    Position currentPosition_;

    // 状态转换方法
    void handleInitialState(const Token& token, OriginContext& context);
    void handleReadingOriginHeaderState(const Token& token, OriginContext& context);
    void handleReadingOriginTypeState(const Token& token, OriginContext& context);
    void handleReadingOriginNameState(const Token& token, OriginContext& context);
    void handleReadingOriginContentState(const Token& token, OriginContext& context);
    void handleReadingUsageState(const Token& token, OriginContext& context);

    // 工具方法
    void addOriginBlock(const OriginBlock& block);
    void addOriginReference(const OriginReference& ref);
    OriginType parseOriginType(const std::string& typeStr);
    bool isValidOriginType(const std::string& typeStr) const;
    bool isValidOriginName(const std::string& name) const;
};

/**
 * CHTL原始嵌入系统解析器
 * 严格按照CHTL语法文档实现原始嵌入解析
 */
class CHTLOriginParser {
public:
    CHTLOriginParser();
    ~CHTLOriginParser();

    /**
     * 解析原始嵌入声明 [Origin] @Type [name] { ... }
     * @param source 源代码
     * @param filename 文件名
     * @return 解析结果
     */
    ParseResult parseOriginDeclaration(const std::string& source, const std::string& filename = "");

    /**
     * 解析原始嵌入使用 [Origin] @Type name;
     * @param usage 原始嵌入使用代码
     * @param context 当前上下文
     * @return 原始嵌入引用
     */
    OriginReference parseOriginUsage(const std::string& usage, const std::string& context = "");

    /**
     * 展开原始嵌入引用
     * @param ref 原始嵌入引用
     * @return 展开后的AST节点
     */
    CHTLASTNodePtr expandOriginReference(const OriginReference& ref);

    /**
     * 设置原始嵌入注册表
     */
    void setOriginRegistry(std::shared_ptr<OriginRegistry> registry);

    /**
     * 获取原始嵌入注册表
     */
    std::shared_ptr<OriginRegistry> getOriginRegistry() const;

    /**
     * 设置严格模式
     */
    void setStrictMode(bool strict);

    /**
     * 设置格式保持模式
     */
    void setPreserveFormatting(bool preserve);

    /**
     * 获取解析错误
     */
    const std::vector<ParseError>& getErrors() const;

    /**
     * 获取解析警告
     */
    const std::vector<ParseWarning>& getWarnings() const;

private:
    std::shared_ptr<OriginRegistry> originRegistry_;
    OriginScanner scanner_;
    OriginStateMachine stateMachine_;
    CHTLLexer lexer_;
    OriginParseState currentState_;
    
    bool strictMode_;
    bool preserveFormatting_;
    std::vector<ParseError> errors_;
    std::vector<ParseWarning> warnings_;

    // 核心解析方法
    OriginBlock parseOriginBlock(const std::vector<Token>& tokens);
    OriginType parseOriginType(const std::string& typeStr);
    std::string parseOriginName(const std::vector<Token>& tokens, size_t& index);
    std::string parseOriginContent(const std::vector<Token>& tokens, size_t& index, OriginType type);
    OriginMode determineOriginMode(const std::vector<Token>& tokens, size_t index);

    // 原始嵌入处理方法
    CHTLASTNodePtr createOriginNode(const OriginBlock& block);
    std::string processHtmlOrigin(const std::string& content);
    std::string processCssOrigin(const std::string& content);
    std::string processJavaScriptOrigin(const std::string& content);

    // 内容验证方法
    bool validateOriginContent(const std::string& content, OriginType type);
    void sanitizeOriginContent(std::string& content, OriginType type);

    // 工具方法
    void reportError(const std::string& message, const Position& position = Position{});
    void reportWarning(const std::string& message, const Position& position = Position{});
    bool isValidOriginName(const std::string& name) const;
    std::string getOriginTypeString(OriginType type) const;
    std::string getOriginModeString(OriginMode mode) const;
};

/**
 * 原始嵌入工具类
 * 提供原始嵌入相关的工具方法
 */
class OriginUtils {
public:
    /**
     * 检查是否为原始嵌入声明
     */
    static bool isOriginDeclaration(const std::string& source);

    /**
     * 检查是否为原始嵌入使用
     */
    static bool isOriginUsage(const std::string& source);

    /**
     * 提取原始嵌入类型字符串
     */
    static std::string extractOriginTypeString(const std::string& source);

    /**
     * 提取原始嵌入名称
     */
    static std::string extractOriginName(const std::string& source);

    /**
     * 验证原始嵌入语法
     */
    static bool validateOriginSyntax(const std::string& source);

    /**
     * 检测原始代码类型
     */
    static OriginType detectOriginType(const std::string& content);

    /**
     * 清理原始代码内容
     */
    static std::string cleanOriginContent(const std::string& content, OriginType type);

    /**
     * 格式化原始代码内容
     */
    static std::string formatOriginContent(const std::string& content, OriginType type);

    /**
     * 验证HTML语法有效性
     */
    static bool isValidHtml(const std::string& content);

    /**
     * 验证CSS语法有效性
     */
    static bool isValidCss(const std::string& content);

    /**
     * 验证JavaScript语法有效性
     */
    static bool isValidJavaScript(const std::string& content);
};

} // namespace chtl
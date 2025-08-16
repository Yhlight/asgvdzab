#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <regex>

namespace chtl {

/**
 * 上下文语言类型
 * Context language types for comment generation
 */
enum class CommentLanguageContext {
    HTML_CONTEXT,          // HTML上下文 - 生成 <!-- comment -->
    CSS_CONTEXT,           // CSS上下文 - 生成 /* comment */
    JAVASCRIPT_CONTEXT,    // JavaScript上下文 - 生成 // comment
    UNKNOWN_CONTEXT        // 未知上下文 - 默认处理
};

/**
 * 注释位置信息
 * Comment position information
 */
struct CommentPosition {
    size_t startPos;
    size_t endPos;
    size_t lineNumber;
    size_t columnNumber;
    std::string originalContent;  // 原始 -- 注释内容
};

/**
 * 生成器注释结果
 * Generator comment result
 */
struct GeneratedComment {
    std::string content;                    // 生成的注释内容
    CommentLanguageContext targetLanguage;  // 目标语言
    CommentPosition position;               // 位置信息
    bool needsEscaping;                    // 是否需要转义
    std::string escapedContent;           // 转义后的内容
};

/**
 * 注释上下文分析器
 * Comment context analyzer - 分析 -- 注释所在的语言环境
 */
class CommentContextAnalyzer {
public:
    CommentContextAnalyzer();
    
    /**
     * 分析注释所在的上下文环境
     * @param sourceCode 完整源代码
     * @param commentPos 注释位置
     * @return 语言上下文类型
     */
    CommentLanguageContext analyzeContext(
        const std::string& sourceCode, 
        const CommentPosition& commentPos
    );
    
    /**
     * 检查注释是否在指定的代码块内
     * @param sourceCode 源代码
     * @param commentPos 注释位置
     * @param blockPattern 代码块模式（如 "style {", "script {", 等）
     * @return 是否在指定块内
     */
    bool isInBlock(
        const std::string& sourceCode,
        const CommentPosition& commentPos,
        const std::string& blockPattern
    );
    
    /**
     * 获取注释周围的上下文信息
     * @param sourceCode 源代码
     * @param commentPos 注释位置
     * @param contextRadius 上下文半径（前后多少字符）
     * @return 上下文字符串
     */
    std::string getContextString(
        const std::string& sourceCode,
        const CommentPosition& commentPos,
        size_t contextRadius = 100
    );

private:
    // 不同语言上下文的正则模式
    std::vector<std::regex> htmlPatterns_;
    std::vector<std::regex> cssPatterns_;
    std::vector<std::regex> jsPatterns_;
    
    void initializePatterns();
    bool matchesPattern(const std::string& context, const std::vector<std::regex>& patterns);
};

/**
 * 注释生成器状态机
 * Comment generator state machine
 */
class CommentGeneratorStateMachine {
public:
    enum class State {
        PARSING_NORMAL,      // 正常解析状态
        IN_CHTL_ELEMENT,     // 在CHTL元素内
        IN_LOCAL_STYLE,      // 在局部样式块内
        IN_LOCAL_SCRIPT,     // 在局部脚本块内
        IN_GLOBAL_STYLE,     // 在全局样式内
        IN_ORIGIN_BLOCK,     // 在原始嵌入块内
        IN_STRING_LITERAL,   // 在字符串字面量内
        IN_REGULAR_COMMENT   // 在常规注释内（// 或 /* */）
    };
    
    CommentGeneratorStateMachine();
    
    /**
     * 处理字符并更新状态
     * @param ch 当前字符
     * @param position 当前位置
     */
    void processCharacter(char ch, size_t position);
    
    /**
     * 获取当前状态
     */
    State getCurrentState() const { return currentState_; }
    
    /**
     * 获取当前语言上下文
     */
    CommentLanguageContext getCurrentLanguageContext() const;
    
    /**
     * 重置状态机
     */
    void reset();
    
    /**
     * 设置源代码（用于更精确的状态判断）
     */
    void setSourceCode(const std::string& sourceCode);

private:
    State currentState_;
    State previousState_;
    std::string sourceCode_;
    size_t currentPosition_;
    
    // 嵌套级别跟踪
    int braceLevel_;
    int blockLevel_;
    
    // 状态转换辅助方法
    void updateStateBasedOnContext(char ch, size_t position);
    bool isEnteringStyleBlock(size_t position);
    bool isEnteringScriptBlock(size_t position);
    bool isLeavingBlock(char ch);
};

/**
 * 生成器注释系统
 * Generator comment system - 主要接口
 */
class GeneratorCommentSystem {
public:
    GeneratorCommentSystem();
    
    /**
     * 扫描源代码中的所有 -- 注释
     * @param sourceCode 源代码
     * @return 注释位置列表
     */
    std::vector<CommentPosition> scanGeneratorComments(const std::string& sourceCode);
    
    /**
     * 生成适当格式的注释
     * @param sourceCode 完整源代码
     * @param commentPositions 注释位置列表
     * @return 生成的注释列表
     */
    std::vector<GeneratedComment> generateComments(
        const std::string& sourceCode,
        const std::vector<CommentPosition>& commentPositions
    );
    
    /**
     * 替换源代码中的 -- 注释为生成的注释
     * @param sourceCode 原始源代码
     * @param generatedComments 生成的注释
     * @return 替换后的源代码
     */
    std::string replaceComments(
        const std::string& sourceCode,
        const std::vector<GeneratedComment>& generatedComments
    );
    
    /**
     * 处理完整的注释转换流程
     * @param sourceCode 源代码
     * @return 处理后的源代码
     */
    std::string processGeneratorComments(const std::string& sourceCode);

private:
    std::unique_ptr<CommentContextAnalyzer> contextAnalyzer_;
    std::unique_ptr<CommentGeneratorStateMachine> stateMachine_;
    
    // 注释生成方法
    std::string generateHtmlComment(const std::string& content);
    std::string generateCssComment(const std::string& content);
    std::string generateJavaScriptComment(const std::string& content, bool useBlockComment = false);
    
    // 内容转义方法
    std::string escapeHtmlComment(const std::string& content);
    std::string escapeCssComment(const std::string& content);
    std::string escapeJavaScriptComment(const std::string& content);
    
    // 正则表达式模式
    std::regex generatorCommentPattern_;
    
    void initializePatterns();
};

} // namespace chtl
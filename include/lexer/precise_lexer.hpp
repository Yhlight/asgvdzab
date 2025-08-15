#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "common/types.hpp"
#include "lexer/chtl_tokens.hpp"
#include "lexer/chtl_js_tokens.hpp"
#include "lexer/lexer_state.hpp"
#include "lexer/lexer_context.hpp"
#include "lexer/global_map.hpp"

namespace chtl {

/**
 * 词法分析结果
 */
struct LexResult {
    bool success;                           // 是否成功
    std::vector<CHTLToken> chtlTokens;      // CHTL Token列表
    std::vector<CHTLJSToken> chtlJSTokens;  // CHTL JS Token列表
    std::vector<std::string> errors;        // 错误列表
    std::vector<std::string> warnings;      // 警告列表
    
    // 统计信息
    struct Statistics {
        size_t totalTokens = 0;
        size_t chtlTokenCount = 0;
        size_t chtlJSTokenCount = 0;
        size_t commentTokens = 0;
        size_t stringTokens = 0;
        size_t keywordTokens = 0;
        size_t identifierTokens = 0;
        double lexingTimeMs = 0.0;
    } stats;
    
    LexResult() : success(false) {}
    
    // 合并Token列表
    std::vector<std::pair<Position, std::string>> getAllTokensWithPositions() const;
    
    // 获取Token数量
    size_t getTotalTokenCount() const;
    
    // 检查是否有错误
    bool hasErrors() const;
    
    // 检查是否有警告
    bool hasWarnings() const;
    
    std::string toString() const;
};

/**
 * 词法分析配置
 */
struct LexerConfig {
    bool enableCHTLJSDetection = true;      // 启用CHTL JS检测
    bool enableCommentTokens = true;        // 启用注释Token
    bool enableWhitespaceTokens = false;    // 启用空白Token
    bool enableStringLiterals = true;       // 启用字符串字面量
    bool enableUnquotedStrings = true;      // 启用无引号字符串
    bool strictMode = true;                 // 严格模式
    bool validateSyntax = true;             // 验证语法
    bool collectStatistics = true;          // 收集统计信息
    
    // 错误处理
    bool stopOnFirstError = false;          // 遇到第一个错误时停止
    size_t maxErrors = 100;                 // 最大错误数量
    
    // 性能优化
    bool enableTokenCaching = true;         // 启用Token缓存
    size_t tokenCacheSize = 1000;          // Token缓存大小
    
    LexerConfig() = default;
};

/**
 * 精确词法分析器
 */
class PreciseLexer {
public:
    explicit PreciseLexer(const LexerConfig& config = LexerConfig{});
    
    // === 主要接口 ===
    
    /**
     * 词法分析字符串
     */
    LexResult lexString(const std::string& source, const std::string& filename = "");
    
    /**
     * 词法分析文件
     */
    LexResult lexFile(const std::string& filename);
    
    /**
     * 增量词法分析（用于编辑器）
     */
    LexResult lexIncremental(const std::string& source, size_t startPos, size_t endPos,
                             const std::string& filename = "");
    
    // === 配置管理 ===
    
    /**
     * 设置配置
     */
    void setConfig(const LexerConfig& config);
    
    /**
     * 获取配置
     */
    const LexerConfig& getConfig() const;
    
    /**
     * 设置全局映射
     */
    void setGlobalMap(std::shared_ptr<GlobalMap> globalMap);
    
    /**
     * 获取全局映射
     */
    std::shared_ptr<GlobalMap> getGlobalMap() const;
    
    // === 状态查询 ===
    
    /**
     * 获取当前位置
     */
    Position getCurrentPosition() const;
    
    /**
     * 获取当前字符
     */
    char getCurrentChar() const;
    
    /**
     * 获取下一个字符
     */
    char peekChar(size_t offset = 1) const;
    
    /**
     * 检查是否到达文件末尾
     */
    bool isAtEnd() const;
    
    // === 调试和诊断 ===
    
    /**
     * 获取状态管理器
     */
    std::shared_ptr<LexerStateManager> getStateManager() const;
    
    /**
     * 获取上下文管理器
     */
    std::shared_ptr<ContextManager> getContextManager() const;
    
    /**
     * 导出调试信息
     */
    std::string exportDebugInfo() const;
    
    /**
     * 重置词法分析器
     */
    void reset();

private:
    LexerConfig config_;                                    // 配置
    std::shared_ptr<GlobalMap> globalMap_;                  // 全局映射
    std::shared_ptr<LexerStateManager> stateManager_;       // 状态管理器
    std::shared_ptr<ContextManager> contextManager_;        // 上下文管理器
    std::shared_ptr<CHTLKeywordMap> chtlKeywordMap_;       // CHTL关键字映射
    std::shared_ptr<CHTLJSKeywordMap> chtlJSKeywordMap_;   // CHTL JS关键字映射
    std::shared_ptr<CHTLPrefixMap> prefixMap_;             // 前缀映射
    
    // 词法分析状态
    std::string source_;                    // 源代码
    std::string filename_;                  // 文件名
    size_t currentPos_;                     // 当前位置
    Position currentPosition_;              // 当前位置信息
    
    // 结果收集
    std::vector<CHTLToken> chtlTokens_;     // CHTL Token列表
    std::vector<CHTLJSToken> chtlJSTokens_; // CHTL JS Token列表
    std::vector<std::string> errors_;       // 错误列表
    std::vector<std::string> warnings_;     // 警告列表
    
    // 私有方法
    
    /**
     * 初始化词法分析器
     */
    void initialize();
    
    /**
     * 开始词法分析
     */
    LexResult performLexing();
    
    /**
     * 跳过空白字符
     */
    void skipWhitespace();
    
    /**
     * 移动到下一个字符
     */
    void advance();
    
    /**
     * 更新位置信息
     */
    void updatePosition();
    
    // === Token识别 ===
    
    /**
     * 识别下一个Token
     */
    bool recognizeNextToken();
    
    /**
     * 识别CHTL Token
     */
    bool recognizeCHTLToken();
    
    /**
     * 识别CHTL JS Token
     */
    bool recognizeCHTLJSToken();
    
    /**
     * 识别关键字
     */
    bool recognizeKeyword();
    
    /**
     * 识别标识符
     */
    bool recognizeIdentifier();
    
    /**
     * 识别字符串字面量
     */
    bool recognizeStringLiteral();
    
    /**
     * 识别无引号字符串
     */
    bool recognizeUnquotedString();
    
    /**
     * 识别数字
     */
    bool recognizeNumber();
    
    /**
     * 识别注释
     */
    bool recognizeComment();
    
    /**
     * 识别操作符
     */
    bool recognizeOperator();
    
    /**
     * 识别分隔符
     */
    bool recognizeSeparator();
    
    /**
     * 识别增强选择器
     */
    bool recognizeEnhancedSelector();
    
    // === 辅助方法 ===
    
    /**
     * 匹配字符串
     */
    bool match(const std::string& str);
    
    /**
     * 匹配字符
     */
    bool match(char ch);
    
    /**
     * 读取直到指定字符
     */
    std::string readUntil(char terminator, bool includeTerminator = false);
    
    /**
     * 读取直到字符串
     */
    std::string readUntil(const std::string& terminator, bool includeTerminator = false);
    
    /**
     * 读取标识符
     */
    std::string readIdentifier();
    
    /**
     * 读取字符串
     */
    std::string readString(char delimiter);
    
    /**
     * 读取数字
     */
    std::string readNumber();
    
    /**
     * 读取注释
     */
    std::string readComment();
    
    // === Token创建 ===
    
    /**
     * 创建CHTL Token
     */
    CHTLToken createCHTLToken(CHTLTokenType type, const std::string& value, Position startPos);
    
    /**
     * 创建CHTL JS Token
     */
    CHTLJSToken createCHTLJSToken(CHTLJSTokenType type, const std::string& value, Position startPos);
    
    /**
     * 添加CHTL Token
     */
    void addCHTLToken(CHTLTokenType type, const std::string& value, Position startPos);
    
    /**
     * 添加CHTL JS Token
     */
    void addCHTLJSToken(CHTLJSTokenType type, const std::string& value, Position startPos);
    
    // === 错误处理 ===
    
    /**
     * 添加错误
     */
    void addError(const std::string& error, Position pos = Position{});
    
    /**
     * 添加警告
     */
    void addWarning(const std::string& warning, Position pos = Position{});
    
    /**
     * 检查是否应该停止
     */
    bool shouldStop() const;
    
    // === 语法验证 ===
    
    /**
     * 验证Token序列
     */
    bool validateTokenSequence();
    
    /**
     * 验证括号平衡
     */
    bool validateBracketBalance();
    
    /**
     * 验证字符串完整性
     */
    bool validateStringIntegrity();
    
    /**
     * 验证注释完整性
     */
    bool validateCommentIntegrity();
    
    // === 统计信息 ===
    
    /**
     * 收集统计信息
     */
    void collectStatistics(LexResult& result);
    
    /**
     * 计算词法分析时间
     */
    double calculateLexingTime();
    
    // === 状态同步 ===
    
    /**
     * 同步状态管理器
     */
    void syncStateManager();
    
    /**
     * 同步上下文管理器
     */
    void syncContextManager();
    
    /**
     * 处理状态转换
     */
    void handleStateTransition(const CHTLToken& token);
    void handleStateTransition(const CHTLJSToken& token);
};

/**
 * 词法分析器工厂
 */
class LexerFactory {
public:
    /**
     * 创建标准词法分析器
     */
    static std::unique_ptr<PreciseLexer> createStandardLexer();
    
    /**
     * 创建严格模式词法分析器
     */
    static std::unique_ptr<PreciseLexer> createStrictLexer();
    
    /**
     * 创建快速词法分析器（性能优化）
     */
    static std::unique_ptr<PreciseLexer> createFastLexer();
    
    /**
     * 创建调试词法分析器
     */
    static std::unique_ptr<PreciseLexer> createDebugLexer();
    
    /**
     * 从配置创建词法分析器
     */
    static std::unique_ptr<PreciseLexer> createFromConfig(const LexerConfig& config);
};

/**
 * 词法分析器工具类
 */
class LexerUtils {
public:
    /**
     * 验证词法分析结果
     */
    static bool validateLexResult(const LexResult& result);
    
    /**
     * 合并词法分析结果
     */
    static LexResult mergeLexResults(const std::vector<LexResult>& results);
    
    /**
     * 格式化词法分析结果
     */
    static std::string formatLexResult(const LexResult& result, bool includeTokens = true);
    
    /**
     * 导出Token列表到CSV
     */
    static std::string exportTokensToCSV(const LexResult& result);
    
    /**
     * 导出Token列表到JSON
     */
    static std::string exportTokensToJSON(const LexResult& result);
    
    /**
     * 比较两个词法分析结果
     */
    static std::string compareLexResults(const LexResult& result1, const LexResult& result2);
    
    /**
     * 获取性能报告
     */
    static std::string getPerformanceReport(const LexResult& result);
};

} // namespace chtl
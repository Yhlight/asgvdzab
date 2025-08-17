#pragma once

#include "CodeFragment.h"
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

namespace CHTL {

/**
 * CHTL统一扫描器 - 精准代码切割器
 * 负责将CHTL源代码精确切分为不同类型的代码片段
 */
class CHTLUnifiedScanner {
public:
    CHTLUnifiedScanner();
    ~CHTLUnifiedScanner() = default;

    /**
     * 扫描CHTL源代码，切分为不同类型的代码片段
     * @param sourceCode CHTL源代码
     * @param filename 文件名（用于错误报告）
     * @return 代码片段列表
     */
    FragmentList scan(const std::string& sourceCode, const std::string& filename = "");

    /**
     * 设置调试模式
     * @param debug 是否启用调试模式
     */
    void setDebugMode(bool debug) { debugMode_ = debug; }

    /**
     * 获取扫描错误信息
     * @return 错误信息列表
     */
    const std::vector<std::string>& getErrors() const { return errors_; }

    /**
     * 清除错误信息
     */
    void clearErrors() { errors_.clear(); }

private:
    // 扫描状态
    enum class ScanState {
        NORMAL,             // 正常状态
        IN_COMMENT,         // 在注释中
        IN_MULTILINE_COMMENT, // 在多行注释中
        IN_STRING,          // 在字符串中
        IN_BLOCK,           // 在代码块中
        IN_STYLE_BLOCK,     // 在样式块中
        IN_SCRIPT_BLOCK,    // 在脚本块中
        IN_ORIGIN_BLOCK     // 在原始嵌入块中
    };

    // 扫描上下文
    struct ScanContext {
        size_t position;        // 当前位置
        size_t line;           // 当前行号
        size_t column;         // 当前列号
        ScanState state;       // 当前状态
        int braceDepth;        // 大括号深度
        std::string currentElement; // 当前元素
        FragmentType expectedType;  // 期望的片段类型
        
        ScanContext() : position(0), line(1), column(1), state(ScanState::NORMAL), 
                       braceDepth(0), expectedType(FragmentType::CHTL) {}
    };

    // HTML元素集合（用于识别CHTL元素）
    std::unordered_set<std::string> htmlElements_;
    
    // CHTL关键字集合
    std::unordered_set<std::string> chtlKeywords_;
    
    // 调试模式
    bool debugMode_;
    
    // 错误信息
    std::vector<std::string> errors_;
    
    // 当前扫描的文件名
    std::string currentFilename_;

    // 初始化HTML元素和关键字
    void initializeKeywords();
    
    // 扫描主循环
    FragmentList scanImpl(const std::string& sourceCode);
    
    // 跳过空白字符和注释
    void skipWhitespaceAndComments(const std::string& source, ScanContext& ctx);
    
    // 识别并扫描代码块
    FragmentPtr scanBlock(const std::string& source, ScanContext& ctx);
    
    // 扫描CHTL元素
    FragmentPtr scanCHTLElement(const std::string& source, ScanContext& ctx);
    
    // 扫描样式块
    FragmentPtr scanStyleBlock(const std::string& source, ScanContext& ctx);
    
    // 扫描脚本块
    FragmentPtr scanScriptBlock(const std::string& source, ScanContext& ctx);
    
    // 扫描原始嵌入块
    FragmentPtr scanOriginBlock(const std::string& source, ScanContext& ctx);
    
    // 扫描模板或自定义块
    FragmentPtr scanTemplateOrCustomBlock(const std::string& source, ScanContext& ctx);
    
    // 识别标识符
    std::string scanIdentifier(const std::string& source, ScanContext& ctx);
    
    // 识别字符串
    std::string scanString(const std::string& source, ScanContext& ctx, char quote);
    
    // 匹配关键字
    bool matchKeyword(const std::string& source, size_t pos, const std::string& keyword);
    
    // 查找匹配的大括号
    size_t findMatchingBrace(const std::string& source, size_t startPos);
    
    // 判断是否为HTML元素
    bool isHTMLElement(const std::string& identifier) const;
    
    // 判断是否为CHTL关键字
    bool isCHTLKeyword(const std::string& identifier) const;
    
    // 分析代码块内容类型
    FragmentType analyzeBlockContentType(const std::string& content, const std::string& context);
    
    // 创建位置信息
    FragmentPosition createPosition(size_t start, size_t end, size_t startLine, 
                                   size_t endLine, size_t startCol, size_t endCol);
    
    // 记录错误
    void recordError(const std::string& message, size_t line, size_t column);
    
    // 调试输出
    void debugOutput(const std::string& message);
    
    // 更新位置信息
    void updatePosition(ScanContext& ctx, char ch);
};

} // namespace CHTL
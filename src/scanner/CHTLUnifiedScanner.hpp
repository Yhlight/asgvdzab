#pragma once

#include <string>
#include <vector>
#include <memory>
#include <stack>
#include "common/CodeFragment.hpp"
#include "common/FragmentType.hpp"

/**
 * CHTLUnifiedScanner - 精准代码切割器
 * 
 * 负责将CHTL源代码切割成不同类型的代码片段：
 * - CHTL片段
 * - CHTL JS片段
 * - CSS片段
 * - JavaScript片段
 * 
 * 使用可变长度切片技术，确保代码片段的完整性
 */
class CHTLUnifiedScanner {
public:
    CHTLUnifiedScanner();
    ~CHTLUnifiedScanner();

    /**
     * 扫描源代码并切割成片段
     * @param source 源代码
     * @return 代码片段列表
     */
    std::vector<CodeFragment> scan(const std::string& source);

private:
    // 扫描状态
    enum class ScanState {
        CHTL,           // CHTL代码
        CHTL_JS,        // CHTL JS代码
        CSS,            // CSS代码
        JAVASCRIPT,     // JavaScript代码
        STRING_SINGLE,  // 单引号字符串
        STRING_DOUBLE,  // 双引号字符串
        COMMENT_LINE,   // 单行注释
        COMMENT_BLOCK   // 多行注释
    };

    // 当前扫描状态
    ScanState currentState_;
    
    // 大括号平衡计数器
    std::stack<char> braceStack_;
    
    // 当前位置
    size_t currentPos_;
    
    // 源代码
    std::string source_;
    
    // 片段列表
    std::vector<CodeFragment> fragments_;

    /**
     * 处理CHTL代码
     */
    void processCHTL();
    
    /**
     * 处理CHTL JS代码
     */
    void processCHTLJS();
    
    /**
     * 处理CSS代码
     */
    void processCSS();
    
    /**
     * 处理JavaScript代码
     */
    void processJavaScript();
    
    /**
     * 检查是否是CHTL关键字
     */
    bool isCHTLKeyword(const std::string& word);
    
    /**
     * 检查是否是CHTL JS选择器
     */
    bool isCHTLJSSelector(size_t pos);
    
    /**
     * 向前查看字符
     */
    char peek(size_t offset = 0);
    
    /**
     * 消费字符
     */
    char consume();
    
    /**
     * 跳过空白字符
     */
    void skipWhitespace();
    
    /**
     * 处理字符串字面量
     */
    void processString(char quote);
    
    /**
     * 处理注释
     */
    void processComment();
    
    /**
     * 添加代码片段
     */
    void addFragment(FragmentType type, const std::string& content, size_t startLine, size_t startCol);
    
    /**
     * 获取当前行号
     */
    size_t getCurrentLine();
    
    /**
     * 获取当前列号
     */
    size_t getCurrentColumn();
    
    /**
     * 检查片段边界
     */
    bool checkFragmentBoundary();
    
    /**
     * 优化片段合并策略
     */
    void optimizeFragments();
};
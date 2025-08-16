#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

namespace chtl {

/**
 * 代码片段类型
 */
enum class CodeFragmentType {
    CHTL_FRAGMENT,      // CHTL片段
    CHTL_JS_FRAGMENT,   // CHTL JS片段
    CSS_FRAGMENT,       // CSS片段
    JS_FRAGMENT         // JS片段
};

/**
 * 代码片段
 * 精准切割后的最小编译单元
 */
struct CodeFragment {
    CodeFragmentType type;          // 片段类型
    std::string content;            // 片段内容
    size_t startPos;               // 起始位置
    size_t endPos;                 // 结束位置
    size_t startLine;              // 起始行号
    size_t endLine;                // 结束行号
    std::string context;           // 上下文信息
    
    // 片段特性
    bool isMinimalUnit;            // 是否为最小单元
    bool isContextDependent;       // 是否依赖上下文
    std::vector<std::string> dependencies;  // 依赖的其他片段
    
    CodeFragment() : type(CodeFragmentType::CHTL_FRAGMENT), startPos(0), endPos(0), 
                     startLine(0), endLine(0), isMinimalUnit(false), isContextDependent(false) {}
};

/**
 * 切片信息
 * 用于可变长度切片处理
 */
struct SliceInfo {
    size_t position;               // 切片位置
    size_t length;                 // 切片长度
    bool needsExpansion;           // 是否需要扩增
    size_t expansionSize;          // 扩增大小
    std::string reason;            // 扩增原因
    
    SliceInfo() : position(0), length(0), needsExpansion(false), expansionSize(0) {}
};

/**
 * CHTL和CHTL JS的最小单元定义
 */
class MinimalUnit {
public:
    /**
     * CHTL最小单元模式
     */
    static const std::vector<std::string>& getChtlMinimalPatterns();
    
    /**
     * CHTL JS最小单元模式
     */
    static const std::vector<std::string>& getChtlJsMinimalPatterns();
    
    /**
     * 检查是否为完整的CHTL单元
     */
    static bool isCompleteChtlUnit(const std::string& fragment);
    
    /**
     * 检查是否为完整的CHTL JS单元
     */
    static bool isCompleteChtlJsUnit(const std::string& fragment);
    
    /**
     * 查找最小单元边界
     */
    static std::vector<size_t> findMinimalUnitBoundaries(const std::string& content, CodeFragmentType type);
};

/**
 * CHTLUnifiedScanner - 精准代码切割器
 * 
 * 核心机制：
 * 1. 基于可变长度切片进行工作
 * 2. 检查切片边界的合理性
 * 3. 根据需要进行片段扩增
 * 4. 按最小单元进行二次精确切割
 * 5. 确保上下文完整性
 */
class CHTLUnifiedScanner {
public:
    CHTLUnifiedScanner();
    
    /**
     * 扫描并切割CHTL源代码为精确片段
     * @param sourceCode 完整的CHTL源代码
     * @return 精确切割的代码片段列表
     */
    std::vector<CodeFragment> scanAndSlice(const std::string& sourceCode);
    
    /**
     * 设置初始切片大小
     */
    void setInitialSliceSize(size_t size) { initialSliceSize_ = size; }
    
    /**
     * 设置最大扩增次数
     */
    void setMaxExpansions(size_t max) { maxExpansions_ = max; }
    
    /**
     * 启用调试模式
     */
    void setDebugMode(bool enabled) { debugMode_ = enabled; }

private:
    size_t initialSliceSize_;      // 初始切片大小
    size_t maxExpansions_;         // 最大扩增次数
    bool debugMode_;               // 调试模式
    
    /**
     * 第一阶段：基于可变长度切片的粗切割
     */
    std::vector<SliceInfo> performInitialSlicing(const std::string& sourceCode);
    
    /**
     * 检查切片边界是否合理
     * @param sourceCode 源代码
     * @param slice 切片信息
     * @return 是否需要扩增
     */
    bool validateSliceBoundary(const std::string& sourceCode, SliceInfo& slice);
    
    /**
     * 扩增切片以确保完整性
     */
    bool expandSlice(const std::string& sourceCode, SliceInfo& slice);
    
    /**
     * 第二阶段：识别片段类型
     */
    CodeFragmentType identifyFragmentType(const std::string& content, const std::string& context = "");
    
    /**
     * 第三阶段：最小单元精确切割
     */
    std::vector<CodeFragment> performMinimalUnitCutting(const std::string& content, CodeFragmentType type, size_t basePos);
    
    /**
     * 检查CHTL/CHTL JS片段的完整性
     */
    bool isChtlFragmentComplete(const std::string& fragment, CodeFragmentType type);
    
    /**
     * 查找可能的片段开始模式
     */
    bool mightStartNewFragment(const std::string& text, size_t position);
    
    /**
     * 合并连续的相同类型片段（基于上下文）
     */
    std::vector<CodeFragment> mergeContextualFragments(const std::vector<CodeFragment>& fragments);
    
    /**
     * 检查片段是否应该与下一个片段合并
     */
    bool shouldMergeWithNext(const CodeFragment& current, const CodeFragment& next);
    
    /**
     * 计算行号
     */
    size_t calculateLineNumber(const std::string& content, size_t position);
    
    /**
     * 调试输出
     */
    void debugLog(const std::string& message);
    
    /**
     * 输出切片状态
     */
    void debugSliceInfo(const SliceInfo& slice, const std::string& content);
};

/**
 * 切片验证器
 * 验证切片的合理性和完整性
 */
class SliceValidator {
public:
    /**
     * 验证CHTL片段完整性
     */
    static bool validateChtlFragment(const std::string& fragment);
    
    /**
     * 验证CHTL JS片段完整性
     */
    static bool validateChtlJsFragment(const std::string& fragment);
    
    /**
     * 检查是否在字符串或注释中间切断
     */
    static bool isInStringOrComment(const std::string& content, size_t position);
    
    /**
     * 检查大括号平衡
     */
    static bool areBracesBalanced(const std::string& fragment);
    
    /**
     * 检查小括号平衡
     */
    static bool areParenthesesBalanced(const std::string& fragment);
};

} // namespace chtl
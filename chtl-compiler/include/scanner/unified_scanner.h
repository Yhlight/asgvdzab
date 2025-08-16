#ifndef CHTL_SCANNER_UNIFIED_SCANNER_H
#define CHTL_SCANNER_UNIFIED_SCANNER_H

#include <string>
#include <vector>
#include <memory>
#include <stack>
#include <deque>
#include "common/types.h"

namespace chtl {
namespace scanner {

// 扫描器状态
enum class ScannerState {
    NORMAL,           // 普通CHTL代码
    IN_STYLE_BLOCK,   // 在style{}块中
    IN_SCRIPT_BLOCK,  // 在script{}块中
    IN_CSS_BLOCK,     // 在全局<style>块中
    IN_JS_BLOCK,      // 在全局<script>块中
    IN_ORIGIN_BLOCK,  // 在[Origin]块中
    IN_STRING,        // 在字符串中
    IN_COMMENT        // 在注释中
};

// 扫描器上下文
struct ScannerContext {
    ScannerState state;
    std::stack<std::string> elementStack;  // 元素栈，跟踪当前元素层级
    std::string currentElement;            // 当前元素名
    size_t braceDepth;                     // 花括号深度
    size_t lineNumber;
    size_t columnNumber;
    std::string currentFile;
    
    ScannerContext() : state(ScannerState::NORMAL), braceDepth(0), 
                      lineNumber(1), columnNumber(1) {}
};

// 切片信息
struct SliceInfo {
    size_t start;
    size_t end;
    CodeSegmentType type;
    bool needsExpansion;  // 是否需要扩展
    
    SliceInfo(size_t s, size_t e, CodeSegmentType t) 
        : start(s), end(e), type(t), needsExpansion(false) {}
};

// CHTL/CHTL JS最小单元类型
enum class TokenUnit {
    IDENTIFIER,          // 标识符
    ENHANCED_SELECTOR,   // {{selector}}
    ARROW_OPERATOR,      // ->
    DOT_OPERATOR,        // .
    KEYWORD,            // 关键字
    BRACE_OPEN,         // {
    BRACE_CLOSE,        // }
    PAREN_OPEN,         // (
    PAREN_CLOSE,        // )
    STRING_LITERAL,     // 字符串
    COMMENT,            // 注释
    WHITESPACE,         // 空白
    OTHER               // 其他
};

// 最小单元信息
struct MinimalUnit {
    TokenUnit type;
    std::string content;
    size_t position;
    size_t length;
};

class UnifiedScanner {
public:
    UnifiedScanner();
    ~UnifiedScanner();
    
    // 扫描CHTL源代码，返回分割后的代码片段
    std::vector<CodeSegment> scan(const std::string& source, 
                                  const std::string& filename = "<input>");
    
    // 设置切片大小（默认1024字节）
    void setSliceSize(size_t size) { defaultSliceSize_ = size; }
    
    // 获取扫描过程中的错误
    const std::vector<std::string>& getErrors() const { return errors_; }
    
    // 获取扫描过程中的警告
    const std::vector<std::string>& getWarnings() const { return warnings_; }
    
    // 清空错误和警告
    void clearDiagnostics() {
        errors_.clear();
        warnings_.clear();
    }

private:
    // 扫描器内部状态
    ScannerContext context_;
    std::vector<std::string> errors_;
    std::vector<std::string> warnings_;
    size_t defaultSliceSize_;
    
    // 第一阶段：初步切片
    std::vector<SliceInfo> performInitialSlicing(const std::string& source);
    
    // 检查切片边界是否合理
    bool isSliceBoundaryValid(const std::string& source, size_t position);
    
    // 扩展切片以包含完整的代码单元
    void expandSlice(const std::string& source, SliceInfo& slice);
    
    // 第二阶段：基于最小单元的精确切割
    std::vector<CodeSegment> performPreciseSlicing(const std::string& source, 
                                                   const std::vector<SliceInfo>& slices);
    
    // 识别最小单元
    std::vector<MinimalUnit> identifyMinimalUnits(const std::string& content, 
                                                  CodeSegmentType type);
    
    // 合并连续的最小单元
    std::vector<CodeSegment> mergeMinimalUnits(const std::vector<MinimalUnit>& units,
                                              const SliceInfo& slice,
                                              const std::string& source);
    
    // 判断是否是CHTL增强语法
    bool isEnhancedSyntax(const std::string& content, size_t position);
    
    // 提取增强选择器 {{...}}
    MinimalUnit extractEnhancedSelector(const std::string& content, size_t& position);
    
    // 提取箭头操作符 ->
    MinimalUnit extractArrowOperator(const std::string& content, size_t& position);
    
    // 判断两个单元是否应该合并
    bool shouldMergeUnits(const MinimalUnit& prev, const MinimalUnit& curr);
    
    // 检测代码片段类型
    CodeSegmentType detectSegmentType(const std::string& content, size_t position);
    
    // 更新位置信息
    void updateLocation(const std::string& content, size_t start, size_t end);
    
    // 错误处理
    void addError(const std::string& message);
    void addWarning(const std::string& message);
};

} // namespace scanner
} // namespace chtl

#endif // CHTL_SCANNER_UNIFIED_SCANNER_H
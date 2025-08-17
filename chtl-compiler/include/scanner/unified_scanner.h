#ifndef CHTL_UNIFIED_SCANNER_H
#define CHTL_UNIFIED_SCANNER_H

#include <string>
#include <vector>
#include <memory>
#include "utils/common.h"

namespace chtl {

// 扫描器配置
struct ScannerConfig {
    size_t initial_slice_size = 256;       // 初始切片大小（调小以便测试）
    size_t min_slice_size = 128;          // 最小切片大小
    size_t max_slice_size = 1048576;      // 最大切片大小（1MB）
    size_t lookahead_size = 256;          // 前瞻大小
    bool enable_intelligent_slicing = true; // 启用智能切片
};

// 扫描器上下文
struct ScanContext {
    std::string content;           // 完整内容
    size_t position = 0;          // 当前位置
    size_t slice_start = 0;       // 当前切片开始位置
    size_t slice_end = 0;         // 当前切片结束位置
    CodeFragmentType current_type = CodeFragmentType::CHTL;
    int line_number = 1;          // 当前行号
    std::vector<CodeFragment> fragments; // 已识别的片段
};

// 切片信息
struct SliceInfo {
    size_t start;
    size_t end;
    CodeFragmentType type;
    bool needs_expansion = false;  // 是否需要扩展
    bool is_complete = true;       // 是否是完整的语法单元
};

// 统一扫描器 - 基于可变长度切片的智能扫描器
class CHTLUnifiedScanner {
public:
    CHTLUnifiedScanner(const ScannerConfig& config = {});
    ~CHTLUnifiedScanner();
    
    // 扫描代码，返回代码片段列表
    std::vector<CodeFragment> scan(const std::string& code);
    
private:
    ScannerConfig config_;
    
    // 主扫描循环
    void scanLoop(ScanContext& ctx);
    
    // 获取下一个切片
    SliceInfo getNextSlice(const ScanContext& ctx);
    
    // 检查切片边界是否合理
    bool isSliceBoundaryValid(const ScanContext& ctx, size_t boundary);
    
    // 扩展切片以包含完整的语法单元
    SliceInfo expandSlice(const ScanContext& ctx, SliceInfo slice);
    
    // 二次切割：将切片细分为最小语法单元
    std::vector<SliceInfo> subdivideSlice(const ScanContext& ctx, const SliceInfo& slice);
    
    // 检测代码片段类型
    CodeFragmentType detectFragmentType(const std::string& content, size_t start, size_t end);
    
    // CHTL特定的边界检测
    bool isCHTLBoundary(const std::string& content, size_t pos);
    
    // CHTL JS特定的边界检测
    bool isCHTLJSBoundary(const std::string& content, size_t pos);
    
    // 检查是否是CHTL JS增强语法
    bool isCHTLJSEnhancedSyntax(const std::string& content, size_t pos);
    
    // 查找CHTL最小单元的结束位置
    size_t findCHTLUnitEnd(const std::string& content, size_t start);
    
    // 查找CHTL JS最小单元的结束位置
    size_t findCHTLJSUnitEnd(const std::string& content, size_t start);
    
    // 合并相邻的相同类型片段
    void mergeAdjacentFragments(std::vector<CodeFragment>& fragments);
    
    // 工具函数
    bool isWhitespace(char c) const;
    bool isIdentifierChar(char c) const;
    bool lookAhead(const std::string& content, size_t pos, const std::string& target) const;
    size_t skipWhitespace(const std::string& content, size_t pos) const;
    
    // 特殊块处理
    size_t scanScriptBlock(const std::string& content, size_t start);
    size_t scanStyleBlock(const std::string& content, size_t start);
    size_t scanOriginBlock(const std::string& content, size_t start);
    
    // 字符串和注释处理
    size_t skipString(const std::string& content, size_t start, char quote);
    size_t skipComment(const std::string& content, size_t start);
    
    // 创建代码片段
    CodeFragment createFragment(const ScanContext& ctx, size_t start, size_t end, 
                               CodeFragmentType type);
};

} // namespace chtl

#endif // CHTL_UNIFIED_SCANNER_H
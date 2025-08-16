#ifndef CHTL_SCANNER_UNIFIED_SCANNER_H
#define CHTL_SCANNER_UNIFIED_SCANNER_H

#include <string>
#include <vector>
#include <memory>
#include <stack>
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

class UnifiedScanner {
public:
    UnifiedScanner();
    ~UnifiedScanner();
    
    // 扫描CHTL源代码，返回分割后的代码片段
    std::vector<CodeSegment> scan(const std::string& source, 
                                  const std::string& filename = "<input>");
    
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
    
    // 当前正在构建的代码片段
    CodeSegment currentSegment_;
    std::string segmentBuffer_;
    
    // 扫描辅助方法
    void processCharacter(char ch, size_t position, std::vector<CodeSegment>& segments);
    void updateLocation(char ch);
    bool isKeyword(const std::string& word) const;
    bool isElementName(const std::string& word) const;
    void handleBraceOpen(std::vector<CodeSegment>& segments);
    void handleBraceClose(std::vector<CodeSegment>& segments);
    void handleStyleBlock(size_t position, std::vector<CodeSegment>& segments);
    void handleScriptBlock(size_t position, std::vector<CodeSegment>& segments);
    void handleOriginBlock(const std::string& originType, std::vector<CodeSegment>& segments);
    void flushCurrentSegment(std::vector<CodeSegment>& segments);
    
    // 判断代码片段类型
    CodeSegmentType detectSegmentType(const std::string& content) const;
    bool isInLocalContext() const;
    
    // 解析辅助方法
    std::string extractWord(const std::string& source, size_t& position) const;
    void skipWhitespace(const std::string& source, size_t& position);
    bool matchKeyword(const std::string& source, size_t position, const std::string& keyword) const;
    
    // 错误处理
    void addError(const std::string& message);
    void addWarning(const std::string& message);
};

} // namespace scanner
} // namespace chtl

#endif // CHTL_SCANNER_UNIFIED_SCANNER_H
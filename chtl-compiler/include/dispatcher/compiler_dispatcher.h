#ifndef COMPILER_DISPATCHER_H
#define COMPILER_DISPATCHER_H

#include <memory>
#include <vector>
#include <functional>
#include "scanner/chtl_unified_scanner.h"

namespace chtl {

// 前向声明各个编译器
class CHTLCompiler;
class CHTLJSCompiler;
class CSSCompiler;
class JavaScriptCompiler;

// 编译结果结构
struct CompileResult {
    bool success;
    std::string output;           // 编译后的输出
    std::string errorMessage;     // 错误信息
    CodeFragmentType fragmentType; // 片段类型
    
    // 额外的编译信息
    struct CompileInfo {
        size_t warningCount = 0;
        size_t errorCount = 0;
        double compileTimeMs = 0.0;
        std::vector<std::string> warnings;
    } info;
    
    CompileResult() : success(false), fragmentType(CodeFragmentType::UNKNOWN) {}
};

// 编译器调度配置
struct DispatcherConfig {
    bool parallelCompilation = true;      // 是否并行编译
    bool stopOnFirstError = false;        // 遇到第一个错误时停止
    bool enableOptimization = true;       // 启用优化
    size_t maxThreads = 0;                // 最大线程数，0表示自动
};

// 编译进度回调
using ProgressCallback = std::function<void(size_t current, size_t total, const std::string& message)>;

// CompilerDispatcher - 编译器调度器
class CompilerDispatcher {
public:
    explicit CompilerDispatcher(const DispatcherConfig& config = DispatcherConfig());
    ~CompilerDispatcher();
    
    // 调度编译多个代码片段
    std::vector<CompileResult> dispatch(const std::vector<CodeFragment>& fragments);
    
    // 设置进度回调
    void setProgressCallback(ProgressCallback callback);
    
    // 获取各个编译器的引用（用于配置）
    CHTLCompiler& getCHTLCompiler();
    CHTLJSCompiler& getCHTLJSCompiler();
    CSSCompiler& getCSSCompiler();
    JavaScriptCompiler& getJavaScriptCompiler();
    
    // 获取编译统计信息
    struct CompileStats {
        size_t totalFragments = 0;
        size_t successCount = 0;
        size_t failureCount = 0;
        double totalCompileTimeMs = 0.0;
        std::unordered_map<CodeFragmentType, size_t> fragmentTypeCounts;
    };
    CompileStats getStats() const;
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace chtl

#endif // COMPILER_DISPATCHER_H
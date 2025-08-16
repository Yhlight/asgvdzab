#pragma once

#include "scanner/CHTLUnifiedScanner.h"
#include <memory>
#include <unordered_map>
#include <functional>
#include <chrono>

namespace chtl {

// 编译结果结构
struct CompilationResult {
    bool success;
    std::string output;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    
    CompilationResult() : success(false) {}
};

// 编译器接口基类
class ICompiler {
public:
    virtual ~ICompiler() = default;
    virtual CompilationResult compile(const CodeFragment& fragment) = 0;
    virtual std::string getName() const = 0;
};

// 编译器调度器类
class CompilerDispatcher {
public:
    CompilerDispatcher();
    ~CompilerDispatcher();
    
    // 注册编译器
    void registerCompiler(FragmentType type, std::unique_ptr<ICompiler> compiler);
    
    // 编译代码片段
    CompilationResult compileFragment(const CodeFragment& fragment);
    
    // 编译所有片段
    std::vector<CompilationResult> compileAllFragments(const std::vector<CodeFragment>& fragments);
    
    // 合并编译结果
    std::string mergeResults(const std::vector<CompilationResult>& results);
    
    // 设置编译选项
    void setCompilationOptions(const std::unordered_map<std::string, std::string>& options);
    
    // 获取编译统计信息
    struct CompilationStats {
        size_t totalFragments;
        size_t successfulCompilations;
        size_t failedCompilations;
        size_t totalErrors;
        size_t totalWarnings;
        std::chrono::milliseconds totalTime;
    };
    
    CompilationStats getCompilationStats() const;
    
    // 获取错误信息
    std::vector<std::string> getErrors() const;
    
    // 清除统计信息
    void clearStats();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 私有方法
    void updateStats(const CompilationResult& result, const std::chrono::milliseconds& time);
    std::string generateHTMLOutput(const std::vector<CompilationResult>& results);
    bool validateCompilationResult(const CompilationResult& result);
};

} // namespace chtl
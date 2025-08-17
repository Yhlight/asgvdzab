#ifndef CHTL_COMPILER_DISPATCHER_H
#define CHTL_COMPILER_DISPATCHER_H

#include "common/types.h"
#include "compilers/base_compiler.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <thread>
#include <future>
#include <mutex>

namespace chtl {

// 编译任务结果
struct CompileTaskResult {
    size_t fragment_index;     // 片段索引
    CompileResult result;      // 编译结果
    CodeFragmentType type;     // 片段类型
};

class CompilerDispatcher {
public:
    CompilerDispatcher();
    ~CompilerDispatcher();
    
    // 调度编译多个代码片段
    std::vector<CompileTaskResult> dispatch(
        const std::vector<CodeFragment>& fragments,
        const CompileOptions& options);
    
    // 设置是否启用并行编译
    void setParallelCompilation(bool enable) { 
        parallel_compilation_ = enable; 
    }
    
    // 设置最大并行线程数
    void setMaxThreads(unsigned int max_threads) {
        max_threads_ = max_threads;
    }
    
    // 注册自定义编译器
    void registerCompiler(CodeFragmentType type, 
                         std::unique_ptr<ICompiler> compiler);
    
    // 设置调试模式
    void setDebugMode(bool debug) { debug_mode_ = debug; }
    
    // 获取编译统计信息
    struct CompileStats {
        size_t total_fragments;
        size_t successful_compiles;
        size_t failed_compiles;
        std::unordered_map<CodeFragmentType, size_t> fragment_counts;
        double total_time_ms;
    };
    
    CompileStats getLastStats() const { return last_stats_; }

private:
    // 编译单个片段
    CompileTaskResult compileFragment(const CodeFragment& fragment,
                                     const CompileOptions& options,
                                     size_t index);
    
    // 并行编译实现
    std::vector<CompileTaskResult> parallelCompile(
        const std::vector<CodeFragment>& fragments,
        const CompileOptions& options);
    
    // 串行编译实现
    std::vector<CompileTaskResult> serialCompile(
        const std::vector<CodeFragment>& fragments,
        const CompileOptions& options);
    
    // 获取或创建编译器
    ICompiler* getCompiler(CodeFragmentType type);
    
private:
    bool debug_mode_;
    bool parallel_compilation_;
    unsigned int max_threads_;
    
    // 编译器映射表
    std::unordered_map<CodeFragmentType, std::unique_ptr<ICompiler>> compilers_;
    
    // 统计信息
    CompileStats last_stats_;
    
    // 互斥锁（用于并行编译）
    mutable std::mutex compiler_mutex_;
};

} // namespace chtl

#endif // CHTL_COMPILER_DISPATCHER_H
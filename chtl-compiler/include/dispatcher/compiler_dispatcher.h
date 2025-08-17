#ifndef CHTL_COMPILER_DISPATCHER_H
#define CHTL_COMPILER_DISPATCHER_H

#include <vector>
#include <memory>
#include <unordered_map>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <optional>
#include <functional>
#include "utils/common.h"
#include "compiler/compiler_interface.h"

namespace chtl {

// 调度配置
struct DispatcherConfig {
    bool enable_parallel_compilation = true;  // 是否启用并行编译
    size_t max_threads = std::thread::hardware_concurrency(); // 最大线程数
    bool enable_cache = true;  // 是否启用编译缓存
    size_t cache_size_mb = 100; // 缓存大小（MB）
};

// 编译任务
struct CompilationTask {
    CodeFragment fragment;
    CompilerOptions options;
    std::promise<CompilationResult> promise;
};

// CompilerDispatcher - 编译器调度器
class CompilerDispatcher {
public:
    explicit CompilerDispatcher(const DispatcherConfig& config = {});
    ~CompilerDispatcher();
    
    // 调度编译任务
    std::vector<CompilationResult> dispatch(const std::vector<CodeFragment>& fragments,
                                           const CompilerOptions& options = {});
    
    // 异步调度
    std::future<std::vector<CompilationResult>> dispatchAsync(
        const std::vector<CodeFragment>& fragments,
        const CompilerOptions& options = {});
    
    // 注册自定义编译器
    void registerCompiler(CodeFragmentType type, std::unique_ptr<ICompiler> compiler);
    
    // 获取编译器信息
    std::vector<std::string> getRegisteredCompilers() const;
    
    // 清理缓存
    void clearCache();
    
    // 设置错误处理回调
    void setErrorHandler(std::function<void(const CompilerError&)> handler) {
        error_handler_ = handler;
    }
    
    // 获取统计信息
    struct Statistics {
        size_t total_compilations;
        size_t cache_hits;
        size_t cache_misses;
        double average_compilation_time_ms;
        std::unordered_map<std::string, size_t> compiler_usage;
    };
    
    Statistics getStatistics() const { return stats_; }
    
private:
    // 编译单个片段
    CompilationResult compileFragment(const CodeFragment& fragment,
                                    const CompilerOptions& options);
    
    // 并行编译
    std::vector<CompilationResult> compileParallel(
        const std::vector<CodeFragment>& fragments,
        const CompilerOptions& options);
    
    // 串行编译
    std::vector<CompilationResult> compileSerial(
        const std::vector<CodeFragment>& fragments,
        const CompilerOptions& options);
    
    // 缓存管理
    std::string getCacheKey(const CodeFragment& fragment, const CompilerOptions& options) const;
    std::optional<CompilationResult> getFromCache(const std::string& key) const;
    void addToCache(const std::string& key, const CompilationResult& result);
    
    // 获取或创建编译器
    ICompiler* getCompiler(CodeFragmentType type);
    
private:
    DispatcherConfig config_;
    std::unordered_map<CodeFragmentType, std::unique_ptr<ICompiler>> compilers_;
    std::unordered_map<std::string, CompilationResult> cache_;
    mutable std::mutex cache_mutex_;
    std::function<void(const CompilerError&)> error_handler_;
    
    // 统计信息
    mutable Statistics stats_;
    mutable std::mutex stats_mutex_;
    
    // 线程池（简化版，实际项目中应使用更完善的线程池）
    std::vector<std::thread> worker_threads_;
    std::queue<CompilationTask> task_queue_;
    std::mutex queue_mutex_;
    std::condition_variable queue_cv_;
    bool stop_workers_ = false;
    
    // 工作线程函数
    void workerThread();
};

} // namespace chtl

#endif // CHTL_COMPILER_DISPATCHER_H
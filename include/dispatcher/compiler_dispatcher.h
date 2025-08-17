#pragma once

#include "common/fragment_types.h"
#include "common/compiler_interface.h"
#include "merger/result_merger.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

namespace chtl {

/**
 * 编译任务类型
 */
enum class CompileTaskType {
    SEQUENTIAL,     // 顺序编译
    PARALLEL,       // 并行编译
    DEPENDENCY      // 依赖编译
};

/**
 * 编译任务
 */
class CompileTask {
public:
    CompileTask(std::unique_ptr<CodeFragment> fragment, 
                CompileTaskType type = CompileTaskType::SEQUENTIAL)
        : fragment_(std::move(fragment)), type_(type), priority_(0) {}
    
    const CodeFragment& getFragment() const { return *fragment_; }
    CompileTaskType getType() const { return type_; }
    int getPriority() const { return priority_; }
    
    void setPriority(int priority) { priority_ = priority; }
    void setType(CompileTaskType type) { type_ = type; }
    
    // 依赖管理
    void addDependency(const std::string& dependency) { dependencies_.push_back(dependency); }
    const std::vector<std::string>& getDependencies() const { return dependencies_; }
    bool hasDependencies() const { return !dependencies_.empty(); }

private:
    std::unique_ptr<CodeFragment> fragment_;
    CompileTaskType type_;
    int priority_;
    std::vector<std::string> dependencies_;
};

/**
 * 编译任务队列
 */
class CompileTaskQueue {
public:
    void addTask(std::unique_ptr<CompileTask> task);
    std::unique_ptr<CompileTask> getNextTask();
    
    bool isEmpty() const { return tasks_.empty(); }
    size_t size() const { return tasks_.size(); }
    
    void clear() { tasks_.clear(); }
    
    // 按优先级排序
    void sortByPriority();
    
    // 按类型分组
    std::vector<std::unique_ptr<CompileTask>> getTasksByType(CompileTaskType type);

private:
    std::vector<std::unique_ptr<CompileTask>> tasks_;
};

/**
 * 编译器调度策略
 */
enum class DispatchStrategy {
    ROUND_ROBIN,    // 轮询
    LOAD_BALANCED,  // 负载均衡
    TYPE_BASED,     // 基于类型
    PRIORITY_BASED  // 基于优先级
};

/**
 * 编译器调度器 - 核心调度组件
 */
class CompilerDispatcher {
public:
    CompilerDispatcher();
    ~CompilerDispatcher();
    
    /**
     * 主要编译接口
     */
    std::unique_ptr<CompileResult> dispatch(const FragmentContainer& fragments);
    std::unique_ptr<CompileResult> dispatch(std::unique_ptr<CodeFragment> fragment);
    
    /**
     * 批量编译
     */
    std::vector<std::unique_ptr<CompileResult>> dispatchBatch(
        const std::vector<std::unique_ptr<CodeFragment>>& fragments);
    
    /**
     * 编译器注册和管理
     */
    void registerCompiler(FragmentType type, std::unique_ptr<ICompiler> compiler);
    void unregisterCompiler(FragmentType type);
    bool hasCompiler(FragmentType type) const;
    ICompiler* getCompiler(FragmentType type) const;
    
    /**
     * 调度策略配置
     */
    void setDispatchStrategy(DispatchStrategy strategy) { dispatch_strategy_ = strategy; }
    DispatchStrategy getDispatchStrategy() const { return dispatch_strategy_; }
    
    /**
     * 并行编译配置
     */
    void setMaxThreads(size_t max_threads) { max_threads_ = max_threads; }
    size_t getMaxThreads() const { return max_threads_; }
    void enableParallelCompilation(bool enable) { parallel_compilation_ = enable; }
    bool isParallelCompilationEnabled() const { return parallel_compilation_; }
    
    /**
     * 编译器配置
     */
    void setCompilerConfig(const CompilerConfig& config) { compiler_config_ = config; }
    const CompilerConfig& getCompilerConfig() const { return compiler_config_; }
    
    /**
     * 结果合并器配置
     */
    void setResultMerger(std::unique_ptr<IResultMerger> merger) { 
        result_merger_ = std::move(merger); 
    }
    
    /**
     * 统计信息
     */
    size_t getTotalCompilations() const { return total_compilations_; }
    size_t getSuccessfulCompilations() const { return successful_compilations_; }
    size_t getFailedCompilations() const { return failed_compilations_; }
    double getAverageCompileTime() const { return average_compile_time_; }
    
    /**
     * 重置统计信息
     */
    void resetStatistics();

private:
    // 编译器映射表
    std::unordered_map<FragmentType, std::unique_ptr<ICompiler>> compilers_;
    
    // 调度配置
    DispatchStrategy dispatch_strategy_;
    size_t max_threads_;
    bool parallel_compilation_;
    
    // 编译器配置
    CompilerConfig compiler_config_;
    
    // 结果合并器
    std::unique_ptr<IResultMerger> result_merger_;
    
    // 任务队列
    std::unique_ptr<CompileTaskQueue> task_queue_;
    
    // 统计信息
    size_t total_compilations_;
    size_t successful_compilations_;
    size_t failed_compilations_;
    double average_compile_time_;
    
    /**
     * 内部调度方法
     */
    std::unique_ptr<CompileResult> dispatchSingle(const CodeFragment& fragment);
    std::vector<std::unique_ptr<CompileResult>> dispatchParallel(
        const std::vector<std::unique_ptr<CodeFragment>>& fragments);
    std::vector<std::unique_ptr<CompileResult>> dispatchSequential(
        const std::vector<std::unique_ptr<CodeFragment>>& fragments);
    
    /**
     * 编译器选择
     */
    ICompiler* selectCompiler(FragmentType type) const;
    
    /**
     * 任务管理
     */
    void createTasks(const FragmentContainer& fragments);
    void processTasks();
    
    /**
     * 依赖解析
     */
    void resolveDependencies();
    bool hasCyclicDependencies() const;
    std::vector<std::unique_ptr<CompileTask>> topologicalSort();
    
    /**
     * 结果处理
     */
    std::unique_ptr<CompileResult> mergeResults(
        const std::vector<std::unique_ptr<CompileResult>>& results);
    
    /**
     * 错误处理
     */
    void handleCompileError(const std::string& error_message, 
                           const SourceLocation& location = SourceLocation{});
    
    /**
     * 性能监控
     */
    void updateStatistics(bool success, double compile_time);
    
    /**
     * 初始化默认编译器
     */
    void initializeDefaultCompilers();
    
    /**
     * 清理资源
     */
    void cleanup();
};

/**
 * 调度器工厂
 */
class DispatcherFactory {
public:
    static std::unique_ptr<CompilerDispatcher> createDispatcher();
    static std::unique_ptr<CompilerDispatcher> createParallelDispatcher(size_t max_threads = 4);
    static std::unique_ptr<CompilerDispatcher> createDebugDispatcher();
    
    /**
     * 预配置的调度器
     */
    static std::unique_ptr<CompilerDispatcher> createProductionDispatcher();
    static std::unique_ptr<CompilerDispatcher> createDevelopmentDispatcher();
    static std::unique_ptr<CompilerDispatcher> createTestDispatcher();
};

/**
 * 调度器配置构建器
 */
class DispatcherConfigBuilder {
public:
    DispatcherConfigBuilder& withStrategy(DispatchStrategy strategy);
    DispatcherConfigBuilder& withMaxThreads(size_t max_threads);
    DispatcherConfigBuilder& withParallelCompilation(bool enable);
    DispatcherConfigBuilder& withCompilerConfig(const CompilerConfig& config);
    DispatcherConfigBuilder& withResultMerger(std::unique_ptr<IResultMerger> merger);
    
    std::unique_ptr<CompilerDispatcher> build();

private:
    DispatchStrategy strategy_ = DispatchStrategy::TYPE_BASED;
    size_t max_threads_ = 4;
    bool parallel_compilation_ = false;
    CompilerConfig compiler_config_;
    std::unique_ptr<IResultMerger> result_merger_;
};

} // namespace chtl
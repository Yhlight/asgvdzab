#pragma once

#include "Common/FragmentTypes.h"
#include "Common/CompilerInterface.h"
#include "Merger/ResultMerger.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

namespace Chtl {

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
        : Fragment_(std::move(fragment)), Type_(type), Priority_(0) {}
    
    const CodeFragment& GetFragment() const { return *Fragment_; }
    CompileTaskType GetType() const { return Type_; }
    int GetPriority() const { return Priority_; }
    
    void SetPriority(int priority) { Priority_ = priority; }
    void SetType(CompileTaskType type) { Type_ = type; }
    
    // 依赖管理
    void AddDependency(const std::string& dependency) { Dependencies_.push_back(dependency); }
    const std::vector<std::string>& GetDependencies() const { return Dependencies_; }
    bool HasDependencies() const { return !Dependencies_.empty(); }

private:
    std::unique_ptr<CodeFragment> Fragment_;
    CompileTaskType Type_;
    int Priority_;
    std::vector<std::string> Dependencies_;
};

/**
 * 编译任务队列
 */
class CompileTaskQueue {
public:
    void AddTask(std::unique_ptr<CompileTask> task);
    std::unique_ptr<CompileTask> GetNextTask();
    
    bool IsEmpty() const { return Tasks_.empty(); }
    size_t Size() const { return Tasks_.size(); }
    
    void Clear() { Tasks_.clear(); }
    
    // 按优先级排序
    void SortByPriority();
    
    // 按类型分组
    std::vector<std::unique_ptr<CompileTask>> GetTasksByType(CompileTaskType type);

private:
    std::vector<std::unique_ptr<CompileTask>> Tasks_;
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
    std::unique_ptr<CompileResult> Dispatch(const FragmentContainer& fragments);
    std::unique_ptr<CompileResult> Dispatch(std::unique_ptr<CodeFragment> fragment);
    
    /**
     * 批量编译
     */
    std::vector<std::unique_ptr<CompileResult>> DispatchBatch(
        const std::vector<std::unique_ptr<CodeFragment>>& fragments);
    
    /**
     * 编译器注册和管理
     */
    void RegisterCompiler(FragmentType type, std::unique_ptr<ICompiler> compiler);
    void UnregisterCompiler(FragmentType type);
    bool HasCompiler(FragmentType type) const;
    ICompiler* GetCompiler(FragmentType type) const;
    
    /**
     * 调度策略配置
     */
    void SetDispatchStrategy(DispatchStrategy strategy) { DispatchStrategy_ = strategy; }
    DispatchStrategy GetDispatchStrategy() const { return DispatchStrategy_; }
    
    /**
     * 并行编译配置
     */
    void SetMaxThreads(size_t maxThreads) { MaxThreads_ = maxThreads; }
    size_t GetMaxThreads() const { return MaxThreads_; }
    void EnableParallelCompilation(bool enable) { ParallelCompilation_ = enable; }
    bool IsParallelCompilationEnabled() const { return ParallelCompilation_; }
    
    /**
     * 编译器配置
     */
    void SetCompilerConfig(const CompilerConfig& config) { CompilerConfig_ = config; }
    const CompilerConfig& GetCompilerConfig() const { return CompilerConfig_; }
    
    /**
     * 结果合并器配置
     */
    void SetResultMerger(std::unique_ptr<IResultMerger> merger) { 
        ResultMerger_ = std::move(merger); 
    }
    
    /**
     * 统计信息
     */
    size_t GetTotalCompilations() const { return TotalCompilations_; }
    size_t GetSuccessfulCompilations() const { return SuccessfulCompilations_; }
    size_t GetFailedCompilations() const { return FailedCompilations_; }
    double GetAverageCompileTime() const { return AverageCompileTime_; }
    
    /**
     * 重置统计信息
     */
    void ResetStatistics();

private:
    // 编译器映射表
    std::unordered_map<FragmentType, std::unique_ptr<ICompiler>> Compilers_;
    
    // 调度配置
    DispatchStrategy DispatchStrategy_;
    size_t MaxThreads_;
    bool ParallelCompilation_;
    
    // 编译器配置
    CompilerConfig CompilerConfig_;
    
    // 结果合并器
    std::unique_ptr<IResultMerger> ResultMerger_;
    
    // 任务队列
    std::unique_ptr<CompileTaskQueue> TaskQueue_;
    
    // 统计信息
    size_t TotalCompilations_;
    size_t SuccessfulCompilations_;
    size_t FailedCompilations_;
    double AverageCompileTime_;
    
    /**
     * 内部调度方法
     */
    std::unique_ptr<CompileResult> DispatchSingle(const CodeFragment& fragment);
    std::vector<std::unique_ptr<CompileResult>> DispatchParallel(
        const std::vector<std::unique_ptr<CodeFragment>>& fragments);
    std::vector<std::unique_ptr<CompileResult>> DispatchSequential(
        const std::vector<std::unique_ptr<CodeFragment>>& fragments);
    
    /**
     * 编译器选择
     */
    ICompiler* SelectCompiler(FragmentType type) const;
    
    /**
     * 任务管理
     */
    void CreateTasks(const FragmentContainer& fragments);
    void ProcessTasks();
    
    /**
     * 依赖解析
     */
    void ResolveDependencies();
    bool HasCyclicDependencies() const;
    std::vector<std::unique_ptr<CompileTask>> TopologicalSort();
    
    /**
     * 结果处理
     */
    std::unique_ptr<CompileResult> MergeResults(
        const std::vector<std::unique_ptr<CompileResult>>& results);
    
    /**
     * 错误处理
     */
    void HandleCompileError(const std::string& errorMessage, 
                           const SourceLocation& location = SourceLocation{});
    
    /**
     * 性能监控
     */
    void UpdateStatistics(bool success, double compileTime);
    
    /**
     * 初始化默认编译器
     */
    void InitializeDefaultCompilers();
    
    /**
     * 清理资源
     */
    void Cleanup();
};

/**
 * 调度器工厂
 */
class DispatcherFactory {
public:
    static std::unique_ptr<CompilerDispatcher> CreateDispatcher();
    static std::unique_ptr<CompilerDispatcher> CreateParallelDispatcher(size_t maxThreads = 4);
    static std::unique_ptr<CompilerDispatcher> CreateDebugDispatcher();
    
    /**
     * 预配置的调度器
     */
    static std::unique_ptr<CompilerDispatcher> CreateProductionDispatcher();
    static std::unique_ptr<CompilerDispatcher> CreateDevelopmentDispatcher();
    static std::unique_ptr<CompilerDispatcher> CreateTestDispatcher();
};

/**
 * 调度器配置构建器
 */
class DispatcherConfigBuilder {
public:
    DispatcherConfigBuilder& WithStrategy(DispatchStrategy strategy);
    DispatcherConfigBuilder& WithMaxThreads(size_t maxThreads);
    DispatcherConfigBuilder& WithParallelCompilation(bool enable);
    DispatcherConfigBuilder& WithCompilerConfig(const CompilerConfig& config);
    DispatcherConfigBuilder& WithResultMerger(std::unique_ptr<IResultMerger> merger);
    
    std::unique_ptr<CompilerDispatcher> Build();

private:
    DispatchStrategy Strategy_ = DispatchStrategy::TYPE_BASED;
    size_t MaxThreads_ = 4;
    bool ParallelCompilation_ = false;
    CompilerConfig CompilerConfig_;
    std::unique_ptr<IResultMerger> ResultMerger_;
};

} // namespace Chtl
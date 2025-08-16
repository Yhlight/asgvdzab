#ifndef CHTL_DISPATCHER_COMPILER_DISPATCHER_H
#define CHTL_DISPATCHER_COMPILER_DISPATCHER_H

#include <vector>
#include <memory>
#include <map>
#include <thread>
#include <future>
#include "common/types.h"
#include "compiler/base_compiler.h"

namespace chtl {
namespace dispatcher {

// 调度策略
enum class DispatchStrategy {
    SEQUENTIAL,     // 顺序执行
    PARALLEL,       // 并行执行
    ADAPTIVE        // 自适应（根据片段数量和类型决定）
};

// 调度结果
struct DispatchResult {
    bool success;
    std::vector<CompilationResult> results;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    double totalTime;  // 总编译时间（秒）
    
    DispatchResult() : success(true), totalTime(0.0) {}
};

class CompilerDispatcher {
public:
    CompilerDispatcher();
    ~CompilerDispatcher();
    
    // 设置调度策略
    void setStrategy(DispatchStrategy strategy) { strategy_ = strategy; }
    
    // 设置最大并行线程数
    void setMaxThreads(size_t maxThreads) { maxThreads_ = maxThreads; }
    
    // 调度编译任务
    DispatchResult dispatch(const std::vector<CodeSegment>& segments,
                           const CompilerOptions& options = CompilerOptions());
    
    // 注册编译器（可选，默认使用工厂创建）
    void registerCompiler(CodeSegmentType type, 
                         std::unique_ptr<compiler::BaseCompiler> compiler);
    
    // 获取统计信息
    struct Statistics {
        std::map<CodeSegmentType, size_t> segmentCounts;
        std::map<CodeSegmentType, double> compileTimes;
        size_t totalSegments;
        double totalTime;
    };
    
    Statistics getStatistics() const { return stats_; }
    
    // 清空统计信息
    void clearStatistics();

private:
    DispatchStrategy strategy_;
    size_t maxThreads_;
    std::map<CodeSegmentType, std::unique_ptr<compiler::BaseCompiler>> compilers_;
    Statistics stats_;
    
    // 内部调度方法
    DispatchResult dispatchSequential(const std::vector<CodeSegment>& segments,
                                    const CompilerOptions& options);
    DispatchResult dispatchParallel(const std::vector<CodeSegment>& segments,
                                   const CompilerOptions& options);
    DispatchResult dispatchAdaptive(const std::vector<CodeSegment>& segments,
                                   const CompilerOptions& options);
    
    // 获取或创建编译器
    compiler::BaseCompiler* getCompiler(CodeSegmentType type);
    
    // 编译单个片段
    CompilationResult compileSegment(const CodeSegment& segment,
                                   const CompilerOptions& options);
    
    // 更新统计信息
    void updateStatistics(CodeSegmentType type, double compileTime);
    
    // 决定是否使用并行策略（自适应模式）
    bool shouldUseParallel(const std::vector<CodeSegment>& segments) const;
};

} // namespace dispatcher
} // namespace chtl

#endif // CHTL_DISPATCHER_COMPILER_DISPATCHER_H
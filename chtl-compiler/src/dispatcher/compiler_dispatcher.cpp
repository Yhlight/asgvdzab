#include "dispatcher/compiler_dispatcher.h"
#include <chrono>
#include <algorithm>
#include <iostream>

namespace chtl {
namespace dispatcher {

CompilerDispatcher::CompilerDispatcher() 
    : strategy_(DispatchStrategy::ADAPTIVE),
      maxThreads_(std::thread::hardware_concurrency()) {
    clearStatistics();
}

CompilerDispatcher::~CompilerDispatcher() = default;

DispatchResult CompilerDispatcher::dispatch(const std::vector<CodeSegment>& segments,
                                          const CompilerOptions& options) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    DispatchResult result;
    
    // 根据策略选择调度方法
    switch (strategy_) {
        case DispatchStrategy::SEQUENTIAL:
            result = dispatchSequential(segments, options);
            break;
        case DispatchStrategy::PARALLEL:
            result = dispatchParallel(segments, options);
            break;
        case DispatchStrategy::ADAPTIVE:
            result = dispatchAdaptive(segments, options);
            break;
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    result.totalTime = duration.count();
    
    stats_.totalTime += result.totalTime;
    
    return result;
}

void CompilerDispatcher::registerCompiler(CodeSegmentType type,
                                        std::unique_ptr<compiler::BaseCompiler> compiler) {
    compilers_[type] = std::move(compiler);
}

void CompilerDispatcher::clearStatistics() {
    stats_.segmentCounts.clear();
    stats_.compileTimes.clear();
    stats_.totalSegments = 0;
    stats_.totalTime = 0.0;
}

DispatchResult CompilerDispatcher::dispatchSequential(const std::vector<CodeSegment>& segments,
                                                    const CompilerOptions& options) {
    DispatchResult result;
    
    for (const auto& segment : segments) {
        auto compileResult = compileSegment(segment, options);
        
        if (!compileResult.success) {
            result.success = false;
        }
        
        // 合并错误和警告
        result.errors.insert(result.errors.end(),
                           compileResult.errors.begin(),
                           compileResult.errors.end());
        result.warnings.insert(result.warnings.end(),
                             compileResult.warnings.begin(),
                             compileResult.warnings.end());
        
        result.results.push_back(std::move(compileResult));
    }
    
    return result;
}

DispatchResult CompilerDispatcher::dispatchParallel(const std::vector<CodeSegment>& segments,
                                                  const CompilerOptions& options) {
    DispatchResult result;
    std::vector<std::future<CompilationResult>> futures;
    
    // 限制并行线程数
    size_t numThreads = std::min(maxThreads_, segments.size());
    
    // 创建异步任务
    for (const auto& segment : segments) {
        futures.push_back(std::async(std::launch::async,
            [this, &segment, &options]() {
                return compileSegment(segment, options);
            }));
    }
    
    // 收集结果
    for (auto& future : futures) {
        auto compileResult = future.get();
        
        if (!compileResult.success) {
            result.success = false;
        }
        
        // 合并错误和警告
        result.errors.insert(result.errors.end(),
                           compileResult.errors.begin(),
                           compileResult.errors.end());
        result.warnings.insert(result.warnings.end(),
                             compileResult.warnings.begin(),
                             compileResult.warnings.end());
        
        result.results.push_back(std::move(compileResult));
    }
    
    return result;
}

DispatchResult CompilerDispatcher::dispatchAdaptive(const std::vector<CodeSegment>& segments,
                                                  const CompilerOptions& options) {
    // 根据片段数量和类型决定使用哪种策略
    if (shouldUseParallel(segments)) {
        return dispatchParallel(segments, options);
    } else {
        return dispatchSequential(segments, options);
    }
}

compiler::BaseCompiler* CompilerDispatcher::getCompiler(CodeSegmentType type) {
    auto it = compilers_.find(type);
    if (it != compilers_.end()) {
        return it->second.get();
    }
    
    // 如果没有注册的编译器，尝试使用工厂创建
    auto compiler = compiler::CompilerFactory::createCompiler(type);
    if (compiler) {
        auto* ptr = compiler.get();
        compilers_[type] = std::move(compiler);
        return ptr;
    }
    
    return nullptr;
}

CompilationResult CompilerDispatcher::compileSegment(const CodeSegment& segment,
                                                   const CompilerOptions& options) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    CompilationResult result;
    
    // 获取对应的编译器
    auto* compiler = getCompiler(segment.type);
    if (!compiler) {
        result.success = false;
        result.errors.push_back("No compiler available for segment type: " + 
                               std::to_string(static_cast<int>(segment.type)));
        return result;
    }
    
    // 编译片段
    result = compiler->compile(segment, options);
    
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    
    // 更新统计信息
    updateStatistics(segment.type, duration.count());
    
    return result;
}

void CompilerDispatcher::updateStatistics(CodeSegmentType type, double compileTime) {
    stats_.segmentCounts[type]++;
    stats_.compileTimes[type] += compileTime;
    stats_.totalSegments++;
}

bool CompilerDispatcher::shouldUseParallel(const std::vector<CodeSegment>& segments) const {
    // 启发式规则：
    // 1. 片段数量大于CPU核心数
    // 2. 包含耗时的编译任务（如CSS和JavaScript）
    
    if (segments.size() >= maxThreads_) {
        return true;
    }
    
    // 检查是否有耗时的编译任务
    int complexSegments = 0;
    for (const auto& segment : segments) {
        if (segment.type == CodeSegmentType::CSS ||
            segment.type == CodeSegmentType::JAVASCRIPT ||
            segment.type == CodeSegmentType::CHTL_JS) {
            complexSegments++;
        }
    }
    
    // 如果复杂片段占比超过50%，使用并行
    return complexSegments > static_cast<int>(segments.size() / 2);
}

} // namespace dispatcher
} // namespace chtl
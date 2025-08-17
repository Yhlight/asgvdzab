#include "dispatcher/dispatcher.h"
#include <chrono>
#include <iostream>
#include <algorithm>
#include <mutex>

namespace chtl {

CompilerDispatcher::CompilerDispatcher()
    : debug_mode_(false),
      parallel_compilation_(true),
      max_threads_(std::thread::hardware_concurrency()) {
    if (max_threads_ == 0) {
        max_threads_ = 4; // 默认4线程
    }
}

CompilerDispatcher::~CompilerDispatcher() {
}

std::vector<CompileTaskResult> CompilerDispatcher::dispatch(
    const std::vector<CodeFragment>& fragments,
    const CompileOptions& options) {
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // 重置统计信息
    last_stats_ = CompileStats();
    last_stats_.total_fragments = fragments.size();
    
    // 统计片段类型
    for (const auto& fragment : fragments) {
        last_stats_.fragment_counts[fragment.type]++;
    }
    
    if (debug_mode_) {
        std::cout << "Dispatching " << fragments.size() << " fragments for compilation\n";
        for (const auto& [type, count] : last_stats_.fragment_counts) {
            std::cout << "  Type " << static_cast<int>(type) << ": " << count << " fragments\n";
        }
    }
    
    // 根据配置选择编译方式
    std::vector<CompileTaskResult> results;
    if (parallel_compilation_ && fragments.size() > 1) {
        results = parallelCompile(fragments, options);
    } else {
        results = serialCompile(fragments, options);
    }
    
    // 统计成功和失败的编译
    for (const auto& result : results) {
        if (result.result.success) {
            last_stats_.successful_compiles++;
        } else {
            last_stats_.failed_compiles++;
        }
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    last_stats_.total_time_ms = std::chrono::duration<double, std::milli>(
        end_time - start_time).count();
    
    if (debug_mode_) {
        std::cout << "Compilation completed in " << last_stats_.total_time_ms << " ms\n";
        std::cout << "Success: " << last_stats_.successful_compiles 
                  << ", Failed: " << last_stats_.failed_compiles << "\n";
    }
    
    return results;
}

void CompilerDispatcher::registerCompiler(CodeFragmentType type, 
                                        std::unique_ptr<ICompiler> compiler) {
    std::lock_guard<std::mutex> lock(compiler_mutex_);
    compilers_[type] = std::move(compiler);
}

CompileTaskResult CompilerDispatcher::compileFragment(
    const CodeFragment& fragment,
    const CompileOptions& options,
    size_t index) {
    
    CompileTaskResult task_result;
    task_result.fragment_index = index;
    task_result.type = fragment.type;
    
    try {
        ICompiler* compiler = getCompiler(fragment.type);
        if (!compiler) {
            task_result.result.success = false;
            task_result.result.error_msg = "No compiler available for fragment type " +
                                          std::to_string(static_cast<int>(fragment.type));
            return task_result;
        }
        
        if (debug_mode_) {
            compiler->setDebugMode(true);
        }
        
        task_result.result = compiler->compile(fragment, options);
        
    } catch (const std::exception& e) {
        task_result.result.success = false;
        task_result.result.error_msg = "Exception during compilation: " + 
                                       std::string(e.what());
    }
    
    return task_result;
}

std::vector<CompileTaskResult> CompilerDispatcher::parallelCompile(
    const std::vector<CodeFragment>& fragments,
    const CompileOptions& options) {
    
    std::vector<std::future<CompileTaskResult>> futures;
    futures.reserve(fragments.size());
    
    // 创建线程池
    size_t thread_count = std::min(static_cast<size_t>(max_threads_), 
                                  fragments.size());
    
    if (debug_mode_) {
        std::cout << "Using " << thread_count << " threads for parallel compilation\n";
    }
    
    // 启动异步编译任务
    for (size_t i = 0; i < fragments.size(); ++i) {
        futures.push_back(
            std::async(std::launch::async,
                      &CompilerDispatcher::compileFragment,
                      this,
                      std::ref(fragments[i]),
                      std::ref(options),
                      i)
        );
    }
    
    // 收集结果
    std::vector<CompileTaskResult> results;
    results.reserve(fragments.size());
    
    for (auto& future : futures) {
        results.push_back(future.get());
    }
    
    // 按原始顺序排序结果
    std::sort(results.begin(), results.end(),
              [](const CompileTaskResult& a, const CompileTaskResult& b) {
                  return a.fragment_index < b.fragment_index;
              });
    
    return results;
}

std::vector<CompileTaskResult> CompilerDispatcher::serialCompile(
    const std::vector<CodeFragment>& fragments,
    const CompileOptions& options) {
    
    std::vector<CompileTaskResult> results;
    results.reserve(fragments.size());
    
    for (size_t i = 0; i < fragments.size(); ++i) {
        results.push_back(compileFragment(fragments[i], options, i));
    }
    
    return results;
}

ICompiler* CompilerDispatcher::getCompiler(CodeFragmentType type) {
    std::lock_guard<std::mutex> lock(compiler_mutex_);
    
    auto it = compilers_.find(type);
    if (it != compilers_.end()) {
        return it->second.get();
    }
    
    // 尝试创建默认编译器
    auto compiler = CompilerFactory::createCompiler(type);
    if (compiler) {
        ICompiler* ptr = compiler.get();
        compilers_[type] = std::move(compiler);
        return ptr;
    }
    
    return nullptr;
}

} // namespace chtl
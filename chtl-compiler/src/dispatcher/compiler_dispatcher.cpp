#include "dispatcher/compiler_dispatcher.h"
#include <chrono>
#include <sstream>
#include <algorithm>

namespace chtl {

CompilerDispatcher::CompilerDispatcher(const DispatcherConfig& config)
    : config_(config), stop_workers_(false) {
    stats_ = {};
    
    // 初始化工作线程池
    if (config_.enable_parallel_compilation) {
        for (size_t i = 0; i < config_.max_threads; ++i) {
            worker_threads_.emplace_back(&CompilerDispatcher::workerThread, this);
        }
    }
}

CompilerDispatcher::~CompilerDispatcher() {
    // 停止工作线程
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        stop_workers_ = true;
    }
    queue_cv_.notify_all();
    
    // 等待所有线程结束
    for (auto& thread : worker_threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

std::vector<CompilationResult> CompilerDispatcher::dispatch(
    const std::vector<CodeFragment>& fragments,
    const CompilerOptions& options) {
    
    if (config_.enable_parallel_compilation && fragments.size() > 1) {
        return compileParallel(fragments, options);
    } else {
        return compileSerial(fragments, options);
    }
}

std::future<std::vector<CompilationResult>> CompilerDispatcher::dispatchAsync(
    const std::vector<CodeFragment>& fragments,
    const CompilerOptions& options) {
    
    return std::async(std::launch::async, 
                     &CompilerDispatcher::dispatch, this, fragments, options);
}

void CompilerDispatcher::registerCompiler(CodeFragmentType type, 
                                         std::unique_ptr<ICompiler> compiler) {
    compilers_[type] = std::move(compiler);
}

std::vector<std::string> CompilerDispatcher::getRegisteredCompilers() const {
    std::vector<std::string> result;
    
    for (const auto& [type, compiler] : compilers_) {
        if (compiler) {
            result.push_back(compiler->getName() + " v" + compiler->getVersion());
        }
    }
    
    return result;
}

void CompilerDispatcher::clearCache() {
    std::lock_guard<std::mutex> lock(cache_mutex_);
    cache_.clear();
}

CompilationResult CompilerDispatcher::compileFragment(const CodeFragment& fragment,
                                                     const CompilerOptions& options) {
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // 检查缓存
    if (config_.enable_cache) {
        std::string cache_key = getCacheKey(fragment, options);
        auto cached_result = getFromCache(cache_key);
        if (cached_result.has_value()) {
            std::lock_guard<std::mutex> lock(stats_mutex_);
            stats_.cache_hits++;
            return cached_result.value();
        } else {
            std::lock_guard<std::mutex> lock(stats_mutex_);
            stats_.cache_misses++;
        }
    }
    
    // 获取编译器
    ICompiler* compiler = getCompiler(fragment.type);
    if (!compiler) {
        CompilationResult result;
        result.success = false;
        result.errors.push_back("No compiler available for fragment type");
        return result;
    }
    
    // 编译
    CompilationResult result = compiler->compile(fragment, options);
    
    // 更新统计信息
    {
        std::lock_guard<std::mutex> lock(stats_mutex_);
        stats_.total_compilations++;
        stats_.compiler_usage[compiler->getName()]++;
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        
        // 更新平均编译时间
        double current_avg = stats_.average_compilation_time_ms;
        size_t n = stats_.total_compilations;
        stats_.average_compilation_time_ms = (current_avg * (n - 1) + duration.count()) / n;
    }
    
    // 添加到缓存
    if (config_.enable_cache && result.success) {
        std::string cache_key = getCacheKey(fragment, options);
        addToCache(cache_key, result);
    }
    
    return result;
}

std::vector<CompilationResult> CompilerDispatcher::compileParallel(
    const std::vector<CodeFragment>& fragments,
    const CompilerOptions& options) {
    
    std::vector<std::future<CompilationResult>> futures;
    
    // 提交任务到线程池
    for (const auto& fragment : fragments) {
        std::promise<CompilationResult> promise;
        futures.push_back(promise.get_future());
        
        CompilationTask task;
        task.fragment = fragment;
        task.options = options;
        task.promise = std::move(promise);
        
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            task_queue_.push(std::move(task));
        }
        queue_cv_.notify_one();
    }
    
    // 收集结果
    std::vector<CompilationResult> results;
    for (auto& future : futures) {
        results.push_back(future.get());
    }
    
    return results;
}

std::vector<CompilationResult> CompilerDispatcher::compileSerial(
    const std::vector<CodeFragment>& fragments,
    const CompilerOptions& options) {
    
    std::vector<CompilationResult> results;
    
    for (const auto& fragment : fragments) {
        results.push_back(compileFragment(fragment, options));
    }
    
    return results;
}

std::string CompilerDispatcher::getCacheKey(const CodeFragment& fragment,
                                           const CompilerOptions& options) const {
    std::stringstream ss;
    ss << static_cast<int>(fragment.type) << "|"
       << fragment.content.length() << "|"
       << std::hash<std::string>{}(fragment.content) << "|"
       << options.debug_mode << "|"
       << options.minimize_output << "|"
       << options.output_format;
    
    return ss.str();
}

std::optional<CompilationResult> CompilerDispatcher::getFromCache(const std::string& key) const {
    std::lock_guard<std::mutex> lock(cache_mutex_);
    
    auto it = cache_.find(key);
    if (it != cache_.end()) {
        return it->second;
    }
    
    return std::nullopt;
}

void CompilerDispatcher::addToCache(const std::string& key, const CompilationResult& result) {
    std::lock_guard<std::mutex> lock(cache_mutex_);
    
    // 简单的缓存大小控制（基于条目数）
    if (cache_.size() >= config_.cache_size_mb * 100) { // 假设每个条目约10KB
        // 清除最旧的条目（这里简化处理，实际应使用LRU等策略）
        cache_.clear();
    }
    
    cache_[key] = result;
}

ICompiler* CompilerDispatcher::getCompiler(CodeFragmentType type) {
    auto it = compilers_.find(type);
    if (it != compilers_.end() && it->second) {
        return it->second.get();
    }
    
    // 如果没有注册的编译器，尝试使用工厂创建
    auto compiler = CompilerFactory::createCompiler(type);
    if (compiler) {
        auto* ptr = compiler.get();
        compilers_[type] = std::move(compiler);
        return ptr;
    }
    
    return nullptr;
}

void CompilerDispatcher::workerThread() {
    while (true) {
        CompilationTask task;
        bool has_task = false;
        
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            queue_cv_.wait(lock, [this] { return stop_workers_ || !task_queue_.empty(); });
            
            if (stop_workers_ && task_queue_.empty()) {
                break;
            }
            
            if (!task_queue_.empty()) {
                task = std::move(task_queue_.front());
                task_queue_.pop();
                has_task = true;
            }
        }
        
        // 执行编译任务
        if (has_task) {
            try {
                CompilationResult result = compileFragment(task.fragment, task.options);
                task.promise.set_value(result);
            } catch (const std::exception& e) {
                CompilationResult error_result;
                error_result.success = false;
                error_result.errors.push_back(std::string("Compilation failed: ") + e.what());
                task.promise.set_value(error_result);
            }
        }
    }
}

} // namespace chtl
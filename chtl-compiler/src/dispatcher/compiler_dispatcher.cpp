#include "dispatcher/compiler_dispatcher.h"
#include "compilers/chtl_compiler.h"
#include "compilers/chtl_js_compiler.h"
#include "compilers/css_compiler.h"
#include "compilers/javascript_compiler.h"
#include <thread>
#include <future>
#include <chrono>
#include <algorithm>

namespace chtl {

// Dispatcher实现类
class CompilerDispatcher::Impl {
public:
    explicit Impl(const DispatcherConfig& config) 
        : config_(config) {
        // 初始化各个编译器
        chtlCompiler_ = std::make_unique<CHTLCompiler>();
        chtlJsCompiler_ = std::make_unique<CHTLJSCompiler>();
        cssCompiler_ = std::make_unique<CSSCompiler>();
        jsCompiler_ = std::make_unique<JavaScriptCompiler>();
        
        // 设置最大线程数
        if (config_.maxThreads == 0) {
            maxThreads_ = std::thread::hardware_concurrency();
            if (maxThreads_ == 0) {
                maxThreads_ = 4; // 默认值
            }
        } else {
            maxThreads_ = config_.maxThreads;
        }
    }
    
    std::vector<CompileResult> dispatch(const std::vector<CodeFragment>& fragments) {
        std::vector<CompileResult> results;
        results.reserve(fragments.size());
        
        // 重置统计信息
        stats_ = {};
        stats_.totalFragments = fragments.size();
        
        auto startTime = std::chrono::high_resolution_clock::now();
        
        if (config_.parallelCompilation && fragments.size() > 1) {
            results = dispatchParallel(fragments);
        } else {
            results = dispatchSequential(fragments);
        }
        
        auto endTime = std::chrono::high_resolution_clock::now();
        stats_.totalCompileTimeMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();
        
        // 更新统计信息
        for (const auto& result : results) {
            if (result.success) {
                stats_.successCount++;
            } else {
                stats_.failureCount++;
            }
            stats_.fragmentTypeCounts[result.fragmentType]++;
        }
        
        return results;
    }
    
    void setProgressCallback(ProgressCallback callback) {
        progressCallback_ = callback;
    }
    
    CHTLCompiler& getCHTLCompiler() { return *chtlCompiler_; }
    CHTLJSCompiler& getCHTLJSCompiler() { return *chtlJsCompiler_; }
    CSSCompiler& getCSSCompiler() { return *cssCompiler_; }
    JavaScriptCompiler& getJavaScriptCompiler() { return *jsCompiler_; }
    
    CompilerDispatcher::CompileStats getStats() const { return stats_; }

private:
    // 顺序编译
    std::vector<CompileResult> dispatchSequential(const std::vector<CodeFragment>& fragments) {
        std::vector<CompileResult> results;
        size_t current = 0;
        
        for (const auto& fragment : fragments) {
            if (progressCallback_) {
                progressCallback_(current, fragments.size(), 
                    "正在编译 " + fragmentTypeToString(fragment.type) + " 片段");
            }
            
            CompileResult result = compileFragment(fragment);
            results.push_back(result);
            
            if (!result.success && config_.stopOnFirstError) {
                break;
            }
            
            current++;
        }
        
        return results;
    }
    
    // 并行编译
    std::vector<CompileResult> dispatchParallel(const std::vector<CodeFragment>& fragments) {
        std::vector<std::future<CompileResult>> futures;
        std::vector<CompileResult> results;
        results.resize(fragments.size());
        
        // 创建线程池任务
        size_t batchSize = (fragments.size() + maxThreads_ - 1) / maxThreads_;
        
        for (size_t i = 0; i < fragments.size(); i += batchSize) {
            size_t end = std::min(i + batchSize, fragments.size());
            
            futures.push_back(std::async(std::launch::async, 
                [this, &fragments, i, end]() {
                    std::vector<CompileResult> batchResults;
                    for (size_t j = i; j < end; j++) {
                        batchResults.push_back(compileFragment(fragments[j]));
                    }
                    return batchResults;
                }
            ));
        }
        
        // 收集结果
        size_t resultIndex = 0;
        for (auto& future : futures) {
            auto batchResults = future.get();
            for (const auto& result : batchResults) {
                results[resultIndex++] = result;
                
                if (progressCallback_) {
                    progressCallback_(resultIndex, fragments.size(), 
                        "已完成 " + std::to_string(resultIndex) + "/" + 
                        std::to_string(fragments.size()) + " 个片段");
                }
            }
        }
        
        return results;
    }
    
    // 编译单个片段
    CompileResult compileFragment(const CodeFragment& fragment) {
        CompileResult result;
        result.fragmentType = fragment.type;
        
        auto startTime = std::chrono::high_resolution_clock::now();
        
        try {
            // 特殊处理不需要编译的片段类型
            if (fragment.type == CodeFragmentType::HTML_ORIGIN || 
                fragment.type == CodeFragmentType::COMMENT) {
                result.success = true;
                result.output = fragment.content;
                auto endTime = std::chrono::high_resolution_clock::now();
                result.info.compileTimeMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();
                return result;
            }
            
            BaseCompiler* compiler = selectCompiler(fragment.type);
            if (!compiler) {
                result.success = false;
                result.errorMessage = "未找到合适的编译器处理 " + 
                    fragmentTypeToString(fragment.type) + " 类型的片段";
                return result;
            }
            
            std::vector<BaseCompiler::CompileError> errors;
            result.success = compiler->compile(fragment.content, result.output, errors);
            
            // 收集错误和警告
            for (const auto& error : errors) {
                if (error.severity == BaseCompiler::CompileError::WARNING) {
                    result.info.warnings.push_back(error.message);
                    result.info.warningCount++;
                } else {
                    if (!result.errorMessage.empty()) {
                        result.errorMessage += "\n";
                    }
                    result.errorMessage += error.message;
                    result.info.errorCount++;
                }
            }
            
        } catch (const std::exception& e) {
            result.success = false;
            result.errorMessage = "编译异常: " + std::string(e.what());
        }
        
        auto endTime = std::chrono::high_resolution_clock::now();
        result.info.compileTimeMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();
        
        return result;
    }
    
    // 选择合适的编译器
    BaseCompiler* selectCompiler(CodeFragmentType type) {
        switch (type) {
            case CodeFragmentType::CHTL:
                return chtlCompiler_.get();
            case CodeFragmentType::CHTL_JS:
                return chtlJsCompiler_.get();
            case CodeFragmentType::CSS:
                return cssCompiler_.get();
            case CodeFragmentType::JAVASCRIPT:
                return jsCompiler_.get();
            case CodeFragmentType::HTML_ORIGIN:
                // HTML原始内容不需要编译，返回nullptr
                return nullptr;
            case CodeFragmentType::COMMENT:
                // 注释不需要编译
                return nullptr;
            default:
                return nullptr;
        }
    }

private:
    DispatcherConfig config_;
    ProgressCallback progressCallback_;
    
    // 各个编译器实例
    std::unique_ptr<CHTLCompiler> chtlCompiler_;
    std::unique_ptr<CHTLJSCompiler> chtlJsCompiler_;
    std::unique_ptr<CSSCompiler> cssCompiler_;
    std::unique_ptr<JavaScriptCompiler> jsCompiler_;
    
    // 统计信息
    CompilerDispatcher::CompileStats stats_;
    
    // 最大线程数
    size_t maxThreads_;
};

// CompilerDispatcher 实现
CompilerDispatcher::CompilerDispatcher(const DispatcherConfig& config)
    : pImpl(std::make_unique<Impl>(config)) {}

CompilerDispatcher::~CompilerDispatcher() = default;

std::vector<CompileResult> CompilerDispatcher::dispatch(const std::vector<CodeFragment>& fragments) {
    return pImpl->dispatch(fragments);
}

void CompilerDispatcher::setProgressCallback(ProgressCallback callback) {
    pImpl->setProgressCallback(callback);
}

CHTLCompiler& CompilerDispatcher::getCHTLCompiler() {
    return pImpl->getCHTLCompiler();
}

CHTLJSCompiler& CompilerDispatcher::getCHTLJSCompiler() {
    return pImpl->getCHTLJSCompiler();
}

CSSCompiler& CompilerDispatcher::getCSSCompiler() {
    return pImpl->getCSSCompiler();
}

JavaScriptCompiler& CompilerDispatcher::getJavaScriptCompiler() {
    return pImpl->getJavaScriptCompiler();
}

CompilerDispatcher::CompileStats CompilerDispatcher::getStats() const {
    return pImpl->getStats();
}

} // namespace chtl
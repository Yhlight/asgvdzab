#include "../../include/Compiler/CompilerDispatcher.h"
#include "../../include/Scanner/UnifiedScanner.h"
#include <chrono>
#include <future>
#include <thread>
#include <iostream>
#include <sstream>

namespace CHTL {
namespace Compiler {

using namespace Core;

CompilerDispatcher::CompilerDispatcher() {
    initializeCompilers();
    initializeDefaultMerger();
    
    // 创建默认扫描器
    Scanner::ScannerConfig config;
    scanner_ = std::make_shared<Scanner::CHTLUnifiedScanner>(config);
}

void CompilerDispatcher::initializeCompilers() {
    // 这里暂时不注册具体的编译器，等待后续实现
    // 编译器将在具体实现时注册
    debugLog("初始化编译器注册表");
}

void CompilerDispatcher::initializeDefaultMerger() {
    resultMerger_ = [](const std::vector<CompileResultPtr>& results, Context& context) -> CompileResultPtr {
        return DefaultResultMerger::merge(results, context);
    };
    debugLog("初始化默认结果合并器");
}

CompileResultPtr CompilerDispatcher::compileSource(const String& source, 
                                                  const String& filename,
                                                  Context& context) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    if (debugMode_) {
        debugLog("开始编译源码: " + filename + " (" + std::to_string(source.size()) + " 字节)");
    }
    
    try {
        // 使用扫描器分析源码
        auto fragments = scanner_->scanSource(source, filename);
        
        if (debugMode_) {
            debugLog("扫描完成，共生成 " + std::to_string(fragments.size()) + " 个片段");
        }
        
        // 编译所有片段
        auto result = compileFragments(fragments, context);
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        if (result) {
            updateStats(result, duration);
        }
        
        return result;
        
    } catch (const std::exception& e) {
        addError("编译源码失败: " + std::string(e.what()), Position{1, 1, 0, filename});
        
        auto errorResult = std::make_shared<CompileResult>();
        errorResult->success = false;
        errorResult->errors = errors_;
        return errorResult;
    }
}

CompileResultPtr CompilerDispatcher::compileFragments(const CodeFragmentList& fragments,
                                                     Context& context) {
    if (fragments.empty()) {
        auto result = std::make_shared<CompileResult>("", true);
        return result;
    }
    
    std::vector<CompileResultPtr> results;
    
    if (pipelineConfig_.parallelCompilation && fragments.size() > 1) {
        results = compileFragmentsParallel(fragments, context);
    } else {
        results = compileFragmentsSequential(fragments, context);
    }
    
    // 合并结果
    if (resultMerger_) {
        return resultMerger_(results, context);
    } else {
        // 默认合并：简单连接
        String combinedOutput;
        bool allSuccess = true;
        ErrorList combinedErrors;
        ErrorList combinedWarnings;
        
        for (const auto& result : results) {
            if (result) {
                combinedOutput += result->output;
                if (!result->success) {
                    allSuccess = false;
                }
                combinedErrors.insert(combinedErrors.end(), 
                                    result->errors.begin(), result->errors.end());
                combinedWarnings.insert(combinedWarnings.end(),
                                      result->warnings.begin(), result->warnings.end());
            }
        }
        
        auto merged = std::make_shared<CompileResult>(combinedOutput, allSuccess);
        merged->errors = combinedErrors;
        merged->warnings = combinedWarnings;
        return merged;
    }
}

CompileResultPtr CompilerDispatcher::compileFragment(const CodeFragmentPtr& fragment,
                                                    Context& context) {
    return compileFragmentInternal(fragment, context);
}

CompileResultPtr CompilerDispatcher::compileFragmentInternal(const CodeFragmentPtr& fragment,
                                                            Context& context) {
    if (!fragment) {
        addError("空片段", Position{});
        auto result = std::make_shared<CompileResult>();
        result->success = false;
        return result;
    }
    
    stats_.totalFragments++;
    
    // 检查缓存
    if (pipelineConfig_.cacheResults) {
        String cacheKey = generateCacheKey(fragment);
        if (isCacheValid(cacheKey)) {
            auto cached = getCachedResult(cacheKey);
            if (cached) {
                if (debugMode_) {
                    debugLog("使用缓存结果: " + cacheKey);
                }
                return cached;
            }
        }
    }
    
    // 查找对应的编译器
    auto compiler = getCompiler(fragment->type);
    if (!compiler) {
        // 如果没有找到编译器，创建一个简单的直通结果
        if (debugMode_) {
            debugLog("未找到编译器，片段类型: " + std::to_string(static_cast<int>(fragment->type)));
        }
        
        auto result = std::make_shared<CompileResult>(fragment->content, true);
        stats_.successfulCompiles++;
        return result;
    }
    
    try {
        // 编译片段
        auto result = compiler->compile(fragment, context);
        
        if (result && result->success) {
            stats_.successfulCompiles++;
        } else {
            stats_.failedCompiles++;
        }
        
        // 缓存结果
        if (pipelineConfig_.cacheResults && result) {
            String cacheKey = generateCacheKey(fragment);
            cacheResult(cacheKey, result);
        }
        
        // 收集编译器错误
        if (result) {
            collectCompilerErrors(result);
        }
        
        return result;
        
    } catch (const std::exception& e) {
        stats_.failedCompiles++;
        addError("编译片段失败: " + std::string(e.what()), 
                Position{fragment->lineNumber, fragment->columnNumber, 
                        fragment->startPos, context.getCurrentFile()});
        
        auto errorResult = std::make_shared<CompileResult>();
        errorResult->success = false;
        return errorResult;
    }
}

std::vector<CompileResultPtr> CompilerDispatcher::compileFragmentsParallel(
    const CodeFragmentList& fragments, Context& context) {
    
    std::vector<std::future<CompileResultPtr>> futures;
    std::vector<CompileResultPtr> results;
    
    // 限制并发数
    size_t maxConcurrency = std::min(pipelineConfig_.maxThreads, fragments.size());
    
    if (debugMode_) {
        debugLog("并行编译 " + std::to_string(fragments.size()) + 
                " 个片段，最大并发数: " + std::to_string(maxConcurrency));
    }
    
    for (size_t i = 0; i < fragments.size(); ++i) {
        // 等待有空闲的线程
        while (futures.size() >= maxConcurrency) {
            for (auto it = futures.begin(); it != futures.end();) {
                if (it->wait_for(std::chrono::milliseconds(1)) == std::future_status::ready) {
                    results.push_back(it->get());
                    it = futures.erase(it);
                } else {
                    ++it;
                }
            }
            if (futures.size() >= maxConcurrency) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
        
        // 启动新的编译任务
        auto future = std::async(std::launch::async, [this, &fragments, i, &context]() {
            // 为每个线程创建独立的上下文副本
            auto contextCopy = context.clone();
            return compileFragmentInternal(fragments[i], *contextCopy);
        });
        
        futures.push_back(std::move(future));
    }
    
    // 等待所有任务完成
    for (auto& future : futures) {
        results.push_back(future.get());
    }
    
    return results;
}

std::vector<CompileResultPtr> CompilerDispatcher::compileFragmentsSequential(
    const CodeFragmentList& fragments, Context& context) {
    
    std::vector<CompileResultPtr> results;
    results.reserve(fragments.size());
    
    if (debugMode_) {
        debugLog("顺序编译 " + std::to_string(fragments.size()) + " 个片段");
    }
    
    for (const auto& fragment : fragments) {
        auto result = compileFragmentInternal(fragment, context);
        results.push_back(result);
    }
    
    return results;
}

// 编译器管理

void CompilerDispatcher::registerCompiler(FragmentType type, CompilerPtr compiler) {
    if (compiler) {
        compilers_[type] = compiler;
        
        if (debugMode_) {
            debugLog("注册编译器: " + compiler->getName() + 
                    " for type " + std::to_string(static_cast<int>(type)));
        }
    }
}

void CompilerDispatcher::unregisterCompiler(FragmentType type) {
    compilers_.erase(type);
}

CompilerPtr CompilerDispatcher::getCompiler(FragmentType type) const {
    auto it = compilers_.find(type);
    return it != compilers_.end() ? it->second : nullptr;
}

bool CompilerDispatcher::hasCompiler(FragmentType type) const {
    return compilers_.find(type) != compilers_.end();
}

void CompilerDispatcher::setScanner(std::shared_ptr<Scanner::CHTLUnifiedScanner> scanner) {
    scanner_ = scanner;
}

std::shared_ptr<Scanner::CHTLUnifiedScanner> CompilerDispatcher::getScanner() const {
    return scanner_;
}

// 配置管理

void CompilerDispatcher::setGlobalOptions(const StringMap& options) {
    globalOptions_ = options;
    
    // 将全局选项传递给所有编译器
    for (auto& [type, compiler] : compilers_) {
        if (compiler) {
            compiler->setOptions(options);
        }
    }
}

void CompilerDispatcher::setCompilerOptions(FragmentType type, const StringMap& options) {
    compilerOptions_[type] = options;
    
    auto compiler = getCompiler(type);
    if (compiler) {
        compiler->setOptions(options);
    }
}

// 统计和错误处理

void CompilerDispatcher::resetStats() {
    stats_ = CompileStats{};
}

void CompilerDispatcher::addError(const String& message, const Position& pos) {
    auto error = std::make_shared<ErrorInfo>();
    error->message = message;
    error->line = pos.line;
    error->column = pos.column;
    error->file = pos.filename;
    errors_.push_back(error);
}

void CompilerDispatcher::addWarning(const String& message, const Position& pos) {
    auto warning = std::make_shared<ErrorInfo>();
    warning->message = message;
    warning->line = pos.line;
    warning->column = pos.column;
    warning->file = pos.filename;
    warnings_.push_back(warning);
}

void CompilerDispatcher::collectCompilerErrors(const CompileResultPtr& result) {
    if (!result) return;
    
    errors_.insert(errors_.end(), result->errors.begin(), result->errors.end());
    warnings_.insert(warnings_.end(), result->warnings.begin(), result->warnings.end());
}

void CompilerDispatcher::updateStats(const CompileResultPtr& result, 
                                    std::chrono::milliseconds duration) {
    stats_.totalTime += duration;
    
    if (result && result->success) {
        // 统计信息已在编译过程中更新
    }
}

// 缓存管理

String CompilerDispatcher::generateCacheKey(const CodeFragmentPtr& fragment) const {
    if (!fragment) return "";
    
    std::stringstream ss;
    ss << static_cast<int>(fragment->type) << ":"
       << std::hash<String>{}(fragment->content) << ":"
       << fragment->startPos << ":" << fragment->endPos;
    
    return ss.str();
}

bool CompilerDispatcher::isCacheValid(const String& key) const {
    return resultCache_.find(key) != resultCache_.end();
}

void CompilerDispatcher::cacheResult(const String& key, CompileResultPtr result) {
    if (result) {
        resultCache_[key] = result;
    }
}

CompileResultPtr CompilerDispatcher::getCachedResult(const String& key) const {
    auto it = resultCache_.find(key);
    return it != resultCache_.end() ? it->second : nullptr;
}

void CompilerDispatcher::clearCache() {
    resultCache_.clear();
}

void CompilerDispatcher::debugLog(const String& message) const {
    if (debugMode_) {
        std::cout << "[Dispatcher] " << message << std::endl;
    }
}

// DefaultResultMerger实现

CompileResultPtr DefaultResultMerger::merge(const std::vector<CompileResultPtr>& results,
                                           Context& context) {
    if (results.empty()) {
        return std::make_shared<CompileResult>("", true);
    }
    
    // 分离不同类型的结果
    std::vector<CompileResultPtr> htmlResults;
    std::vector<CompileResultPtr> cssResults;
    std::vector<CompileResultPtr> jsResults;
    std::vector<CompileResultPtr> otherResults;
    
    bool allSuccess = true;
    ErrorList allErrors;
    ErrorList allWarnings;
    
    for (const auto& result : results) {
        if (!result) continue;
        
        if (!result->success) {
            allSuccess = false;
        }
        
        allErrors.insert(allErrors.end(), result->errors.begin(), result->errors.end());
        allWarnings.insert(allWarnings.end(), result->warnings.begin(), result->warnings.end());
        
        // 根据元数据或内容特征分类结果
        if (result->metadata.count("type")) {
            String type = result->metadata.at("type");
            if (type == "css") {
                cssResults.push_back(result);
            } else if (type == "js" || type == "javascript") {
                jsResults.push_back(result);
            } else {
                htmlResults.push_back(result);
            }
        } else {
            // 默认作为HTML处理
            htmlResults.push_back(result);
        }
    }
    
    // 合并不同类型的内容
    String htmlContent = mergeHTML(htmlResults);
    String cssContent = mergeCSS(cssResults);
    String jsContent = mergeJS(jsResults);
    
    // 构建最终的HTML文档
    std::stringstream finalOutput;
    
    // 如果没有HTML内容，创建基本结构
    if (htmlContent.empty()) {
        finalOutput << "<!DOCTYPE html>\n<html>\n<head>\n";
        
        if (!cssContent.empty()) {
            finalOutput << "<style>\n" << cssContent << "\n</style>\n";
        }
        
        finalOutput << "</head>\n<body>\n";
        
        if (!jsContent.empty()) {
            finalOutput << "<script>\n" << jsContent << "\n</script>\n";
        }
        
        finalOutput << "</body>\n</html>";
    } else {
        // 如果有HTML内容，智能插入CSS和JS
        String output = htmlContent;
        
        if (!cssContent.empty()) {
            String styleTag = "<style>\n" + cssContent + "\n</style>";
            size_t headPos = output.find("</head>");
            if (headPos != String::npos) {
                output.insert(headPos, styleTag + "\n");
            } else {
                // 如果没有head标签，添加到开头
                output = styleTag + "\n" + output;
            }
        }
        
        if (!jsContent.empty()) {
            String scriptTag = "<script>\n" + jsContent + "\n</script>";
            size_t bodyPos = output.find("</body>");
            if (bodyPos != String::npos) {
                output.insert(bodyPos, scriptTag + "\n");
            } else {
                // 如果没有body标签，添加到结尾
                output = output + "\n" + scriptTag;
            }
        }
        
        finalOutput << output;
    }
    
    auto merged = std::make_shared<CompileResult>(finalOutput.str(), allSuccess);
    merged->errors = allErrors;
    merged->warnings = allWarnings;
    
    collectMetadata(results, merged);
    
    return merged;
}

String DefaultResultMerger::mergeHTML(const std::vector<CompileResultPtr>& results) {
    String combined;
    for (const auto& result : results) {
        if (result && result->success) {
            combined += result->output;
        }
    }
    return combined;
}

String DefaultResultMerger::mergeCSS(const std::vector<CompileResultPtr>& results) {
    String combined;
    for (const auto& result : results) {
        if (result && result->success) {
            combined += result->output + "\n";
        }
    }
    return combined;
}

String DefaultResultMerger::mergeJS(const std::vector<CompileResultPtr>& results) {
    String combined;
    for (const auto& result : results) {
        if (result && result->success) {
            combined += result->output + "\n";
        }
    }
    return combined;
}

void DefaultResultMerger::collectMetadata(const std::vector<CompileResultPtr>& results,
                                         CompileResultPtr& merged) {
    for (const auto& result : results) {
        if (!result) continue;
        
        for (const auto& [key, value] : result->metadata) {
            merged->metadata[key] = value;
        }
    }
}

// CompilerFactory实现

std::unique_ptr<CompilerDispatcher> CompilerFactory::createDispatcher() {
    return std::make_unique<CompilerDispatcher>();
}

CompilerPtr CompilerFactory::createCHTLCompiler() {
    // 将在后续实现CHTL编译器时提供
    return nullptr;
}

CompilerPtr CompilerFactory::createCHTLJSCompiler() {
    // 将在后续实现CHTL JS编译器时提供
    return nullptr;
}

CompilerPtr CompilerFactory::createCSSCompiler() {
    // 将在后续实现CSS编译器时提供
    return nullptr;
}

CompilerPtr CompilerFactory::createJavaScriptCompiler() {
    // 将在后续实现JavaScript编译器时提供
    return nullptr;
}

} // namespace Compiler
} // namespace CHTL
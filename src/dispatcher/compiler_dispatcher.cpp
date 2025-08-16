#include "dispatcher/compiler_dispatcher.hpp"
#include "compilers/chtl_compiler.hpp"
#include "compilers/simple_css_compiler.hpp"
#include "compilers/simple_js_compiler.hpp"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <iomanip>

namespace chtl {

CompilerDispatcher::CompilerDispatcher() : debugMode_(false) {
    initializeDefaultCompilers();
}

CompileResult CompilerDispatcher::compile(const std::vector<CodeSegment>& segments, const CompileConfig& config) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    if (debugMode_) {
        logDebug("开始编译 " + std::to_string(segments.size()) + " 个代码片段");
    }
    
    // 预处理代码片段
    auto processedSegments = preprocessSegments(segments);
    
    // 分析依赖关系并排序
    auto sortedSegments = analyzeDependencies(processedSegments);
    
    std::vector<CompileResult> results;
    results.reserve(sortedSegments.size());
    
    // 逐个编译代码片段
    for (const auto& segment : sortedSegments) {
        auto result = compileSegment(segment, config);
        results.push_back(result);
        
        updateStatistics(segment.type, result.success);
        
        if (debugMode_) {
            logDebug("编译片段 " + toString(segment.type) + 
                    (result.success ? " 成功" : " 失败"));
        }
    }
    
    // 后处理并合并结果
    auto finalResult = postprocessResults(results);
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    if (debugMode_) {
        logDebug("编译完成，耗时 " + std::to_string(duration.count()) + " 毫秒");
    }
    
    finalResult.metadata["compile_time_ms"] = std::to_string(duration.count());
    
    return finalResult;
}

CompileResult CompilerDispatcher::compileSegment(const CodeSegment& segment, const CompileConfig& config) {
    auto compiler = getCompiler(segment.type);
    if (!compiler) {
        CompileResult result;
        result.success = false;
        result.errors.push_back("未找到适用于类型 '" + toString(segment.type) + "' 的编译器");
        return result;
    }
    
    try {
        return compiler->compile(segment, config);
    } catch (const std::exception& e) {
        CompileResult result;
        result.success = false;
        result.errors.push_back("编译器异常: " + std::string(e.what()));
        return result;
    }
}

void CompilerDispatcher::registerCompiler(std::unique_ptr<ICompiler> compiler) {
    if (!compiler) {
        return;
    }
    
    auto supportedTypes = compiler->getSupportedTypes();
    std::string compilerName = compiler->getName();
    
    for (auto type : supportedTypes) {
        // 为每个支持的类型创建编译器的副本（除了第一个）
        if (type == supportedTypes[0]) {
            compilers_[type] = std::move(compiler);
        } else {
            // 为其他类型创建新的编译器实例
            if (compilerName == "CHTL Compiler") {
                compilers_[type] = std::make_unique<CHTLCompiler>();
            }
        }
        
        if (debugMode_) {
            logDebug("注册编译器 '" + compilerName + 
                    "' 用于类型 '" + toString(type) + "'");
        }
    }
}

ICompiler* CompilerDispatcher::getCompiler(CodeSegmentType type) {
    auto it = compilers_.find(type);
    if (it != compilers_.end()) {
        return it->second.get();
    }
    return nullptr;
}

void CompilerDispatcher::setGlobalConfig(const CompileConfig& config) {
    globalConfig_ = config;
}

const CompileConfig& CompilerDispatcher::getGlobalConfig() const {
    return globalConfig_;
}

void CompilerDispatcher::setDebugMode(bool enabled) {
    debugMode_ = enabled;
}

std::unordered_map<std::string, size_t> CompilerDispatcher::getCompileStatistics() const {
    return statistics_;
}

void CompilerDispatcher::resetStatistics() {
    statistics_.clear();
}

void CompilerDispatcher::initializeDefaultCompilers() {
    registerCompiler(CompilerFactory::createCHTLCompiler());
    registerCompiler(CompilerFactory::createCHTLJSCompiler());
    registerCompiler(CompilerFactory::createCSSCompiler());
    registerCompiler(CompilerFactory::createJavaScriptCompiler());
}

std::vector<CodeSegment> CompilerDispatcher::preprocessSegments(const std::vector<CodeSegment>& segments) {
    std::vector<CodeSegment> processed;
    processed.reserve(segments.size());
    
    for (const auto& segment : segments) {
        CodeSegment processedSegment = segment;
        
        // 移除多余的空白字符
        std::string& content = processedSegment.content;
        
        // 移除开头和结尾的空白字符
        size_t start = content.find_first_not_of(" \t\n\r");
        if (start != std::string::npos) {
            size_t end = content.find_last_not_of(" \t\n\r");
            content = content.substr(start, end - start + 1);
        } else {
            content.clear();
        }
        
        // 只添加非空的代码片段
        if (!content.empty()) {
            processed.push_back(processedSegment);
        }
    }
    
    return processed;
}

CompileResult CompilerDispatcher::postprocessResults(const std::vector<CompileResult>& results) {
    CompileResult finalResult;
    
    std::string htmlOutput;
    std::string globalCSS;
    std::string globalJS;
    
    // 收集错误和警告
    for (const auto& result : results) {
        if (!result.success) {
            finalResult.success = false;
        }
        
        finalResult.errors.insert(finalResult.errors.end(), 
                                 result.errors.begin(), result.errors.end());
        finalResult.warnings.insert(finalResult.warnings.end(), 
                                   result.warnings.begin(), result.warnings.end());
        
        // 根据类型分类输出
        if (result.metadata.count("type")) {
            std::string type = result.metadata.at("type");
            if (type == "html") {
                htmlOutput += result.output;
            } else if (type == "css") {
                globalCSS += result.output;
            } else if (type == "js") {
                globalJS += result.output;
            }
        } else {
            htmlOutput += result.output;
        }
    }
    
    // 组装最终的HTML输出
    std::string finalHTML = "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"utf-8\">\n";
    
    if (!globalCSS.empty()) {
        finalHTML += "<style>\n" + globalCSS + "\n</style>\n";
    }
    
    finalHTML += "</head>\n<body>\n" + htmlOutput + "\n";
    
    if (!globalJS.empty()) {
        finalHTML += "<script>\n" + globalJS + "\n</script>\n";
    }
    
    finalHTML += "</body>\n</html>";
    
    finalResult.output = finalHTML;
    finalResult.metadata["total_segments"] = std::to_string(results.size());
    
    return finalResult;
}

std::vector<CodeSegment> CompilerDispatcher::analyzeDependencies(const std::vector<CodeSegment>& segments) {
    std::vector<CodeSegment> sorted = segments;
    
    // 简单的依赖排序：配置 -> 导入 -> 模板 -> 自定义 -> 普通代码
    std::stable_sort(sorted.begin(), sorted.end(), [](const CodeSegment& a, const CodeSegment& b) {
        auto getPriority = [](CodeSegmentType type) -> int {
            switch (type) {
                case CodeSegmentType::CHTL_CONFIGURATION: return 1;
                case CodeSegmentType::CHTL_IMPORT: return 2;
                case CodeSegmentType::CHTL_NAMESPACE: return 3;
                case CodeSegmentType::CHTL_TEMPLATE: return 4;
                case CodeSegmentType::CHTL_CUSTOM: return 5;
                case CodeSegmentType::CSS_GLOBAL: return 6;
                case CodeSegmentType::CSS_LOCAL_STYLE: return 7;
                case CodeSegmentType::CHTL_CORE: return 8;
                case CodeSegmentType::JAVASCRIPT_STANDARD: return 9;
                case CodeSegmentType::CHTL_JS_SCRIPT: return 10;
                default: return 100;
            }
        };
        
        return getPriority(a.type) < getPriority(b.type);
    });
    
    return sorted;
}

void CompilerDispatcher::updateStatistics(CodeSegmentType type, bool success) {
    std::string key = toString(type) + (success ? "_success" : "_error");
    statistics_[key]++;
    statistics_["total"]++;
}

void CompilerDispatcher::logDebug(const std::string& message) {
    if (debugMode_) {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::cout << "[DEBUG] " << std::put_time(std::localtime(&time), "%H:%M:%S") 
                  << " " << message << std::endl;
    }
}

// CompilerFactory实现
std::unique_ptr<ICompiler> CompilerFactory::createCHTLCompiler() {
    return std::make_unique<CHTLCompiler>();
}

std::unique_ptr<ICompiler> CompilerFactory::createCHTLJSCompiler() {
    return nullptr; // 将在实现CHTL JS编译器时完成
}

std::unique_ptr<ICompiler> CompilerFactory::createCSSCompiler() {
    return std::make_unique<SimpleCSSCompiler>();
}

std::unique_ptr<ICompiler> CompilerFactory::createJavaScriptCompiler() {
    return std::make_unique<SimpleJSCompiler>();
}

} // namespace chtl
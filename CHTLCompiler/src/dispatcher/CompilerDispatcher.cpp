#include "dispatcher/CompilerDispatcher.h"
#include <chrono>
#include <sstream>
#include <algorithm>
#include <iostream>

namespace chtl {

// 私有实现类
class CompilerDispatcher::Impl {
public:
    std::unordered_map<FragmentType, std::unique_ptr<ICompiler>> compilers;
    std::unordered_map<std::string, std::string> options;
    CompilationStats stats;
    std::vector<std::string> errors;
    
    Impl() {
        stats = CompilationStats{};
    }
    
    ~Impl() = default;
};

CompilerDispatcher::CompilerDispatcher() : pImpl(std::make_unique<Impl>()) {}

CompilerDispatcher::~CompilerDispatcher() = default;

void CompilerDispatcher::registerCompiler(FragmentType type, std::unique_ptr<ICompiler> compiler) {
    if (compiler) {
        pImpl->compilers[type] = std::move(compiler);
    }
}

CompilationResult CompilerDispatcher::compileFragment(const CodeFragment& fragment) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // 查找对应的编译器
    auto it = pImpl->compilers.find(fragment.type);
    if (it == pImpl->compilers.end()) {
        CompilationResult result;
        result.success = false;
        result.errors.push_back("未找到类型 " + std::to_string(static_cast<int>(fragment.type)) + " 的编译器");
        return result;
    }
    
    // 编译片段
    CompilationResult result = it->second->compile(fragment);
    
    // 更新统计信息
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    updateStats(result, duration);
    
    return result;
}

std::vector<CompilationResult> CompilerDispatcher::compileAllFragments(const std::vector<CodeFragment>& fragments) {
    std::vector<CompilationResult> results;
    results.reserve(fragments.size());
    
    for (const auto& fragment : fragments) {
        results.push_back(compileFragment(fragment));
    }
    
    return results;
}

std::string CompilerDispatcher::mergeResults(const std::vector<CompilationResult>& results) {
    return generateHTMLOutput(results);
}

void CompilerDispatcher::setCompilationOptions(const std::unordered_map<std::string, std::string>& options) {
    pImpl->options = options;
}

CompilerDispatcher::CompilationStats CompilerDispatcher::getCompilationStats() const {
    return pImpl->stats;
}

std::vector<std::string> CompilerDispatcher::getErrors() const {
    return pImpl->errors;
}

void CompilerDispatcher::clearStats() {
    pImpl->stats = CompilationStats{};
}

void CompilerDispatcher::updateStats(const CompilationResult& result, const std::chrono::milliseconds& time) {
    pImpl->stats.totalFragments++;
    pImpl->stats.totalTime += time;
    
    if (result.success) {
        pImpl->stats.successfulCompilations++;
    } else {
        pImpl->stats.failedCompilations++;
    }
    
    pImpl->stats.totalErrors += result.errors.size();
    pImpl->stats.totalWarnings += result.warnings.size();
}

std::string CompilerDispatcher::generateHTMLOutput(const std::vector<CompilationResult>& results) {
    std::stringstream html;
    
    // HTML头部
    html << "<!DOCTYPE html>\n";
    html << "<html lang=\"zh-CN\">\n";
    html << "<head>\n";
    html << "    <meta charset=\"UTF-8\">\n";
    html << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    html << "    <title>CHTL编译结果</title>\n";
    
    // 合并CSS样式
    html << "    <style>\n";
    for (const auto& result : results) {
        if (result.success && !result.output.empty()) {
            // 这里需要根据片段类型来提取CSS内容
            // 暂时直接输出
            html << result.output << "\n";
        }
    }
    html << "    </style>\n";
    html << "</head>\n";
    html << "<body>\n";
    
    // 合并HTML内容
    for (const auto& result : results) {
        if (result.success && !result.output.empty()) {
            // 这里需要根据片段类型来提取HTML内容
            // 暂时直接输出
            html << result.output << "\n";
        }
    }
    
    // 合并JavaScript代码
    html << "    <script>\n";
    for (const auto& result : results) {
        if (result.success && !result.output.empty()) {
            // 这里需要根据片段类型来提取JavaScript内容
            // 暂时直接输出
            html << result.output << "\n";
        }
    }
    html << "    </script>\n";
    
    html << "</body>\n";
    html << "</html>";
    
    return html.str();
}

bool CompilerDispatcher::validateCompilationResult(const CompilationResult& result) {
    if (!result.success) {
        return false;
    }
    
    // 检查输出是否为空
    if (result.output.empty()) {
        return false;
    }
    
    // 检查是否有严重错误
    for (const auto& error : result.errors) {
        if (error.find("FATAL") != std::string::npos || 
            error.find("CRITICAL") != std::string::npos) {
            return false;
        }
    }
    
    return true;
}

} // namespace chtl
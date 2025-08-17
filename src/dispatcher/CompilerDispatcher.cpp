#include "CompilerDispatcher.h"
#include "../compilers/chtl/CHTLCompiler.h"
#include "../compilers/chtljs/CHTLJSCompiler.h"
#include "../compilers/css/CSSCompiler.h"
#include "../compilers/js/JSCompiler.h"
#include "../merger/ResultMerger.h"
#include <iostream>
#include <algorithm>

namespace CHTL {

CompilerDispatcher::CompilerDispatcher() : debugMode_(false) {
    initializeCompilers();
}

CompilerDispatcher::~CompilerDispatcher() = default;

void CompilerDispatcher::initializeCompilers() {
    chtlCompiler_ = std::make_unique<CHTLCompiler>();
    chtljsCompiler_ = std::make_unique<CHTLJSCompiler>();
    cssCompiler_ = std::make_unique<CSSCompiler>();
    jsCompiler_ = std::make_unique<JSCompiler>();
    resultMerger_ = std::make_unique<ResultMerger>();
    
    debugOutput("编译器调度器初始化完成");
}

bool CompilerDispatcher::compile(const FragmentList& fragments, const std::string& outputFilename) {
    debugOutput("开始编译，片段数量: " + std::to_string(fragments.size()));
    
    // 清除之前的结果
    clearResults();
    
    // 验证片段依赖关系
    if (!validateFragmentDependencies(fragments)) {
        recordError("片段依赖关系验证失败");
        return false;
    }
    
    // 优化编译顺序
    FragmentList optimizedFragments = optimizeCompilationOrder(fragments);
    
    // 按类型分类片段
    categorizeFragments(optimizedFragments);
    
    debugOutput("片段分类完成 - CHTL: " + std::to_string(chtlFragments_.size()) + 
               ", CHTL JS: " + std::to_string(chtljsFragments_.size()) +
               ", CSS: " + std::to_string(cssFragments_.size()) +
               ", JS: " + std::to_string(jsFragments_.size()) +
               ", HTML: " + std::to_string(htmlFragments_.size()));
    
    bool success = true;
    
    // 按顺序编译各类型片段
    if (!compileCHTLFragments()) success = false;
    if (!compileCHTLJSFragments()) success = false;
    if (!compileCSSFragments()) success = false;
    if (!compileJSFragments()) success = false;
    if (!processHTMLFragments()) success = false;
    
    // 合并编译结果
    if (success && !mergeResults(outputFilename)) {
        success = false;
    }
    
    debugOutput("编译完成，成功: " + std::string(success ? "是" : "否"));
    
    return success;
}

void CompilerDispatcher::categorizeFragments(const FragmentList& fragments) {
    // 清空之前的分类
    chtlFragments_.clear();
    chtljsFragments_.clear();
    cssFragments_.clear();
    jsFragments_.clear();
    htmlFragments_.clear();
    
    for (const auto& fragment : fragments) {
        switch (fragment->type) {
            case FragmentType::CHTL:
                chtlFragments_.push_back(fragment);
                break;
            case FragmentType::CHTL_JS:
                chtljsFragments_.push_back(fragment);
                break;
            case FragmentType::CSS:
                cssFragments_.push_back(fragment);
                break;
            case FragmentType::JAVASCRIPT:
                jsFragments_.push_back(fragment);
                break;
            case FragmentType::HTML:
                htmlFragments_.push_back(fragment);
                break;
            default:
                recordWarning("未知的片段类型: " + fragment->getTypeName());
                break;
        }
    }
}

bool CompilerDispatcher::compileCHTLFragments() {
    if (chtlFragments_.empty()) {
        debugOutput("没有CHTL片段需要编译");
        return true;
    }
    
    debugOutput("开始编译CHTL片段");
    
    chtlCompiler_->setDebugMode(debugMode_);
    chtlCompiler_->setOptions(compilerOptions_);
    
    CompilationResults chtlResults = chtlCompiler_->compileAll(chtlFragments_);
    
    bool success = true;
    for (const auto& result : chtlResults) {
        addResult(result);
        if (!result->success) {
            success = false;
        }
    }
    
    debugOutput("CHTL片段编译完成，成功: " + std::string(success ? "是" : "否"));
    return success;
}

bool CompilerDispatcher::compileCHTLJSFragments() {
    if (chtljsFragments_.empty()) {
        debugOutput("没有CHTL JS片段需要编译");
        return true;
    }
    
    debugOutput("开始编译CHTL JS片段");
    
    chtljsCompiler_->setDebugMode(debugMode_);
    chtljsCompiler_->setOptions(compilerOptions_);
    
    CompilationResults chtljsResults = chtljsCompiler_->compileAll(chtljsFragments_);
    
    bool success = true;
    for (const auto& result : chtljsResults) {
        addResult(result);
        if (!result->success) {
            success = false;
        }
    }
    
    debugOutput("CHTL JS片段编译完成，成功: " + std::string(success ? "是" : "否"));
    return success;
}

bool CompilerDispatcher::compileCSSFragments() {
    if (cssFragments_.empty()) {
        debugOutput("没有CSS片段需要编译");
        return true;
    }
    
    debugOutput("开始编译CSS片段");
    
    cssCompiler_->setDebugMode(debugMode_);
    cssCompiler_->setOptions(compilerOptions_);
    
    CompilationResults cssResults = cssCompiler_->compileAll(cssFragments_);
    
    bool success = true;
    for (const auto& result : cssResults) {
        addResult(result);
        if (!result->success) {
            success = false;
        }
    }
    
    debugOutput("CSS片段编译完成，成功: " + std::string(success ? "是" : "否"));
    return success;
}

bool CompilerDispatcher::compileJSFragments() {
    if (jsFragments_.empty()) {
        debugOutput("没有JavaScript片段需要编译");
        return true;
    }
    
    debugOutput("开始编译JavaScript片段");
    
    jsCompiler_->setDebugMode(debugMode_);
    jsCompiler_->setOptions(compilerOptions_);
    
    CompilationResults jsResults = jsCompiler_->compileAll(jsFragments_);
    
    bool success = true;
    for (const auto& result : jsResults) {
        addResult(result);
        if (!result->success) {
            success = false;
        }
    }
    
    debugOutput("JavaScript片段编译完成，成功: " + std::string(success ? "是" : "否"));
    return success;
}

bool CompilerDispatcher::processHTMLFragments() {
    if (htmlFragments_.empty()) {
        debugOutput("没有HTML片段需要处理");
        return true;
    }
    
    debugOutput("开始处理HTML片段");
    
    // HTML片段（原始嵌入）直接传递，不需要编译
    for (const auto& fragment : htmlFragments_) {
        auto result = std::make_shared<CompilationResult>(true, fragment->content, FragmentType::HTML);
        result->context = fragment->context;
        addResult(result);
    }
    
    debugOutput("HTML片段处理完成");
    return true;
}

bool CompilerDispatcher::mergeResults(const std::string& outputFilename) {
    debugOutput("开始合并编译结果");
    
    resultMerger_->setDebugMode(debugMode_);
    
    finalOutput_ = resultMerger_->merge(results_, outputFilename);
    
    // 检查合并过程中是否有错误
    const auto& mergerErrors = resultMerger_->getErrors();
    const auto& mergerWarnings = resultMerger_->getWarnings();
    
    bool success = mergerErrors.empty();
    
    // 记录合并器的错误和警告
    for (const auto& error : mergerErrors) {
        recordError("合并器错误: " + error);
    }
    
    for (const auto& warning : mergerWarnings) {
        recordWarning("合并器警告: " + warning);
    }
    
    debugOutput("编译结果合并完成，成功: " + std::string(success ? "是" : "否"));
    return success;
}

std::vector<std::string> CompilerDispatcher::getAllErrors() const {
    std::vector<std::string> allErrors;
    
    for (const auto& result : results_) {
        allErrors.insert(allErrors.end(), result->errors.begin(), result->errors.end());
    }
    
    return allErrors;
}

std::vector<std::string> CompilerDispatcher::getAllWarnings() const {
    std::vector<std::string> allWarnings;
    
    for (const auto& result : results_) {
        allWarnings.insert(allWarnings.end(), result->warnings.begin(), result->warnings.end());
    }
    
    return allWarnings;
}

void CompilerDispatcher::setDebugMode(bool debug) {
    debugMode_ = debug;
    
    if (chtlCompiler_) chtlCompiler_->setDebugMode(debug);
    if (chtljsCompiler_) chtljsCompiler_->setDebugMode(debug);
    if (cssCompiler_) cssCompiler_->setDebugMode(debug);
    if (jsCompiler_) jsCompiler_->setDebugMode(debug);
    if (resultMerger_) resultMerger_->setDebugMode(debug);
}

void CompilerDispatcher::clearResults() {
    results_.clear();
    finalOutput_.clear();
    chtlFragments_.clear();
    chtljsFragments_.clear();
    cssFragments_.clear();
    jsFragments_.clear();
    htmlFragments_.clear();
}

void CompilerDispatcher::setCompilerOptions(const std::unordered_map<std::string, std::string>& options) {
    compilerOptions_ = options;
}

void CompilerDispatcher::addResult(CompilationResultPtr result) {
    results_.push_back(result);
}

void CompilerDispatcher::recordError(const std::string& message, const std::string& context) {
    std::string fullMessage = context.empty() ? message : context + ": " + message;
    
    if (debugMode_) {
        std::cerr << "错误: " << fullMessage << std::endl;
    }
    
    // 创建一个错误结果
    auto errorResult = std::make_shared<CompilationResult>();
    errorResult->success = false;
    errorResult->errors.push_back(fullMessage);
    errorResult->context = context;
    
    addResult(errorResult);
}

void CompilerDispatcher::recordWarning(const std::string& message, const std::string& context) {
    std::string fullMessage = context.empty() ? message : context + ": " + message;
    
    if (debugMode_) {
        std::cout << "警告: " << fullMessage << std::endl;
    }
    
    // 如果有当前结果，添加警告到最后一个结果中
    if (!results_.empty()) {
        results_.back()->warnings.push_back(fullMessage);
    }
}

void CompilerDispatcher::debugOutput(const std::string& message) {
    if (debugMode_) {
        std::cout << "[CompilerDispatcher] " << message << std::endl;
    }
}

bool CompilerDispatcher::validateFragmentDependencies(const FragmentList& fragments) {
    // 简单的依赖验证，可以后续扩展
    debugOutput("验证片段依赖关系");
    
    // 检查是否有循环依赖等问题
    // 这里先简单返回true，后续可以实现更复杂的依赖分析
    
    return true;
}

FragmentList CompilerDispatcher::optimizeCompilationOrder(const FragmentList& fragments) {
    // 优化编译顺序，确保依赖关系正确
    debugOutput("优化编译顺序");
    
    FragmentList optimized = fragments;
    
    // 按类型和依赖关系排序
    // CHTL -> CHTL JS -> CSS -> JS -> HTML
    std::stable_sort(optimized.begin(), optimized.end(), 
        [](const FragmentPtr& a, const FragmentPtr& b) {
            // 定义类型优先级
            auto getTypePriority = [](FragmentType type) -> int {
                switch (type) {
                    case FragmentType::CHTL: return 1;
                    case FragmentType::CHTL_JS: return 2;
                    case FragmentType::CSS: return 3;
                    case FragmentType::JAVASCRIPT: return 4;
                    case FragmentType::HTML: return 5;
                    default: return 6;
                }
            };
            
            return getTypePriority(a->type) < getTypePriority(b->type);
        });
    
    return optimized;
}

} // namespace CHTL
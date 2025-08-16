#include "core/compiler_dispatcher.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <regex>

namespace chtl {

// ==================== CompilerDispatcher 实现 ====================

CompilerDispatcher::CompilerDispatcher() 
    : initialized_(false), debugMode_(false) {
    scanner_ = std::make_unique<CHTLUnifiedScanner>();
}

CompilerDispatcher::~CompilerDispatcher() {
    cleanup();
}

bool CompilerDispatcher::initialize() {
    if (initialized_) {
        return true;
    }
    
    debugLog("初始化编译器调度器...");
    
    // 初始化各个编译器
    if (!initializeCompilers()) {
        debugLog("编译器初始化失败");
        return false;
    }
    
    // 验证编译器兼容性
    if (!validateCompilerCompatibility()) {
        debugLog("编译器兼容性验证失败");
        return false;
    }
    
    initialized_ = true;
    debugLog("编译器调度器初始化成功");
    return true;
}

void CompilerDispatcher::cleanup() {
    if (!initialized_) {
        return;
    }
    
    debugLog("清理编译器调度器...");
    
    if (chtlCompiler_) {
        chtlCompiler_->cleanup();
        chtlCompiler_.reset();
    }
    
    if (chtlJsCompiler_) {
        chtlJsCompiler_->cleanup();
        chtlJsCompiler_.reset();
    }
    
    if (cssCompiler_) {
        cssCompiler_->cleanup();
        cssCompiler_.reset();
    }
    
    if (jsCompiler_) {
        jsCompiler_->cleanup();
        jsCompiler_.reset();
    }
    
    initialized_ = false;
    debugLog("编译器调度器清理完成");
}

CompilationResult CompilerDispatcher::compileSource(const std::string& sourceCode) {
    CompilationResult result;
    
    if (!initialized_) {
        result.errors.push_back("编译器调度器未初始化");
        return result;
    }
    
    debugLog("开始编译源代码，长度: " + std::to_string(sourceCode.length()));
    
    try {
        // 第一阶段：使用统一扫描器进行精准切割
        scanner_->setDebugMode(debugMode_);
        auto fragments = scanner_->scanAndSlice(sourceCode);
        
        debugLog("切割完成，共产生 " + std::to_string(fragments.size()) + " 个片段");
        
        // 第二阶段：编译各个片段
        auto fragmentResults = compileFragments(fragments);
        
        // 第三阶段：合并编译结果
        result = mergeResults(fragmentResults);
        
        // 生成编译报告
        if (debugMode_) {
            result.metadata["compilation_report"] = generateCompilationReport(fragmentResults);
        }
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back("编译过程中发生异常: " + std::string(e.what()));
    }
    
    return result;
}

std::vector<FragmentCompilationResult> CompilerDispatcher::compileFragments(const std::vector<CodeFragment>& fragments) {
    std::vector<FragmentCompilationResult> results;
    results.reserve(fragments.size());
    
    for (const auto& fragment : fragments) {
        debugFragmentInfo(fragment);
        
        auto fragmentResult = compileSingleFragment(fragment);
        results.push_back(fragmentResult);
        
        // 处理编译错误
        if (!fragmentResult.result.success) {
            handleCompilationError(fragmentResult.compilerId, fragmentResult.result);
        }
    }
    
    return results;
}

CompilationResult CompilerDispatcher::mergeResults(const std::vector<FragmentCompilationResult>& fragmentResults) {
    CompilationResult mergedResult;
    mergedResult.success = true;
    
    std::ostringstream htmlOutput;
    
    // 收集所有错误和警告
    for (const auto& fragmentResult : fragmentResults) {
        const auto& result = fragmentResult.result;
        
        if (!result.success) {
            mergedResult.success = false;
        }
        
        // 合并错误和警告
        mergedResult.errors.insert(mergedResult.errors.end(), 
                                 result.errors.begin(), result.errors.end());
        mergedResult.warnings.insert(mergedResult.warnings.end(), 
                                    result.warnings.begin(), result.warnings.end());
        
        // 合并元数据
        for (const auto& [key, value] : result.metadata) {
            mergedResult.metadata[fragmentResult.compilerId + "_" + key] = value;
        }
        
        // 合并输出
        if (result.success && !result.output.empty()) {
            htmlOutput << result.output;
        }
    }
    
    // 优化最终HTML输出
    mergedResult.output = optimizeHtmlOutput(htmlOutput.str());
    
    // 添加统计信息
    mergedResult.metadata["total_fragments"] = std::to_string(fragmentResults.size());
    mergedResult.metadata["successful_fragments"] = std::to_string(
        std::count_if(fragmentResults.begin(), fragmentResults.end(),
                     [](const auto& fr) { return fr.result.success; }));
    
    return mergedResult;
}

std::unordered_map<std::string, std::string> CompilerDispatcher::getCompilerStatus() const {
    std::unordered_map<std::string, std::string> status;
    
    status["dispatcher_initialized"] = initialized_ ? "true" : "false";
    status["debug_mode"] = debugMode_ ? "true" : "false";
    
    if (chtlCompiler_) {
        status["chtl_compiler"] = chtlCompiler_->getId() + " v" + chtlCompiler_->getVersion();
    }
    
    if (chtlJsCompiler_) {
        status["chtl_js_compiler"] = chtlJsCompiler_->getId() + " v" + chtlJsCompiler_->getVersion();
    }
    
    if (cssCompiler_) {
        status["css_compiler"] = cssCompiler_->getId() + " v" + cssCompiler_->getVersion();
    }
    
    if (jsCompiler_) {
        status["js_compiler"] = jsCompiler_->getId() + " v" + jsCompiler_->getVersion();
    }
    
    return status;
}

std::shared_ptr<ICompiler> CompilerDispatcher::selectCompiler(const CodeFragment& fragment) {
    switch (fragment.type) {
        case CodeFragmentType::CHTL_FRAGMENT:
            // 进一步分析是否为局部style
            if (analyzeStyleFragment(fragment) == CodeFragmentType::CHTL_FRAGMENT) {
                return chtlCompiler_;
            }
            return chtlCompiler_;
            
        case CodeFragmentType::CHTL_JS_FRAGMENT:
            return chtlJsCompiler_;
            
        case CodeFragmentType::CSS_FRAGMENT:
            return cssCompiler_;
            
        case CodeFragmentType::JS_FRAGMENT:
            return jsCompiler_;
            
        default:
            debugLog("未知片段类型，使用CHTL编译器作为默认");
            return chtlCompiler_;
    }
}

CodeFragmentType CompilerDispatcher::analyzeScriptFragment(const CodeFragment& fragment) {
    const std::string& content = fragment.content;
    
    // 检查是否包含CHTL语法
    std::vector<std::regex> chtlPatterns = {
        std::regex(R"(\{\{[^}]+\}\})"),              // {{variable}}
        std::regex(R"(\w+\s+from\s+[\w./]+)"),       // from语句
        std::regex(R"(\w+\(\w+\s*(?:=\s*[^)]+)?\))"), // 变量组语法
        std::regex(R"(\[Origin\]\s+@\w+)"),          // 原始嵌入
    };
    
    bool hasChtlSyntax = false;
    for (const auto& pattern : chtlPatterns) {
        if (std::regex_search(content, pattern)) {
            hasChtlSyntax = true;
            break;
        }
    }
    
    // 检查是否包含JavaScript语法
    std::vector<std::regex> jsPatterns = {
        std::regex(R"(\b(?:function|var|let|const|if|else|for|while|return|class)\b)"),
        std::regex(R"(\w+\s*=\s*\([^)]*\)\s*=>)"),   // 箭头函数
        std::regex(R"(\w+\.\w+\s*\()"),              // 方法调用
    };
    
    bool hasJsSyntax = false;
    for (const auto& pattern : jsPatterns) {
        if (std::regex_search(content, pattern)) {
            hasJsSyntax = true;
            break;
        }
    }
    
    // 根据语法特征确定类型
    if (hasChtlSyntax && hasJsSyntax) {
        return CodeFragmentType::CHTL_JS_FRAGMENT;
    } else if (hasChtlSyntax) {
        return CodeFragmentType::CHTL_FRAGMENT;
    } else {
        return CodeFragmentType::JS_FRAGMENT;
    }
}

CodeFragmentType CompilerDispatcher::analyzeStyleFragment(const CodeFragment& fragment) {
    const std::string& content = fragment.content;
    
    // 检查是否为局部style（包含CHTL语法）
    std::vector<std::regex> chtlStylePatterns = {
        std::regex(R"(\{\{[^}]+\}\})"),              // {{variable}}
        std::regex(R"(@(?:Var|Style)\s+\w+)"),       // @Var/@Style
        std::regex(R"(inherit\s+@?\w+)"),            // inherit语句
        std::regex(R"(delete\s+@?\w+)"),             // delete语句
    };
    
    for (const auto& pattern : chtlStylePatterns) {
        if (std::regex_search(content, pattern)) {
            return CodeFragmentType::CHTL_FRAGMENT;  // 局部style，使用CHTL编译器
        }
    }
    
    // 否则为全局style，使用CSS编译器
    return CodeFragmentType::CSS_FRAGMENT;
}

FragmentCompilationResult CompilerDispatcher::compileSingleFragment(const CodeFragment& fragment) {
    FragmentCompilationResult result(fragment);
    
    // 选择合适的编译器
    auto compiler = selectCompiler(fragment);
    if (!compiler) {
        result.result.success = false;
        result.result.errors.push_back("无法为片段选择合适的编译器");
        result.compilerId = "unknown";
        return result;
    }
    
    result.compilerId = compiler->getId();
    
    // 根据编译器类型调用特定的编译方法
    try {
        if (fragment.type == CodeFragmentType::CHTL_FRAGMENT) {
            // 检查是否为局部style
            if (fragment.context == "style" || analyzeStyleFragment(fragment) == CodeFragmentType::CHTL_FRAGMENT) {
                auto chtlCompiler = std::dynamic_pointer_cast<ICHTLCompiler>(compiler);
                if (chtlCompiler && chtlCompiler->supportsLocalStyle()) {
                    result.result = chtlCompiler->compileLocalStyle(fragment);
                } else {
                    result.result = compiler->compile(fragment);
                }
            } else {
                result.result = compiler->compile(fragment);
            }
        } else if (fragment.type == CodeFragmentType::CHTL_JS_FRAGMENT) {
            auto chtlJsCompiler = std::dynamic_pointer_cast<ICHTLJSCompiler>(compiler);
            if (chtlJsCompiler) {
                result.result = chtlJsCompiler->compileChtlJsMix(fragment);
            } else {
                result.result = compiler->compile(fragment);
            }
        } else if (fragment.type == CodeFragmentType::CSS_FRAGMENT) {
            auto cssCompiler = std::dynamic_pointer_cast<ICSSCompiler>(compiler);
            if (cssCompiler) {
                result.result = cssCompiler->compileGlobalStyle(fragment);
            } else {
                result.result = compiler->compile(fragment);
            }
        } else if (fragment.type == CodeFragmentType::JS_FRAGMENT) {
            auto jsCompiler = std::dynamic_pointer_cast<IJavaScriptCompiler>(compiler);
            if (jsCompiler) {
                result.result = jsCompiler->compileScript(fragment);
            } else {
                result.result = compiler->compile(fragment);
            }
        } else {
            result.result = compiler->compile(fragment);
        }
    } catch (const std::exception& e) {
        result.result.success = false;
        result.result.errors.push_back("编译器异常: " + std::string(e.what()));
    }
    
    return result;
}

void CompilerDispatcher::debugLog(const std::string& message) {
    if (debugMode_) {
        std::cout << "[CompilerDispatcher] " << message << std::endl;
    }
}

void CompilerDispatcher::debugFragmentInfo(const CodeFragment& fragment) {
    if (debugMode_) {
        std::string typeStr;
        switch (fragment.type) {
            case CodeFragmentType::CHTL_FRAGMENT: typeStr = "CHTL"; break;
            case CodeFragmentType::CHTL_JS_FRAGMENT: typeStr = "CHTL_JS"; break;
            case CodeFragmentType::CSS_FRAGMENT: typeStr = "CSS"; break;
            case CodeFragmentType::JS_FRAGMENT: typeStr = "JS"; break;
        }
        
        std::string preview = fragment.content.substr(0, 50);
        std::replace(preview.begin(), preview.end(), '\n', ' ');
        
        debugLog("编译片段 [" + typeStr + "] " + 
                std::to_string(fragment.startPos) + ":" + std::to_string(fragment.endPos) + 
                " \"" + preview + "...\"");
    }
}

bool CompilerDispatcher::validateCompilerCompatibility() {
    // 验证CHTL编译器
    if (!chtlCompiler_) {
        debugLog("CHTL编译器未找到");
        return false;
    }
    
    // 验证CSS编译器的ANTLR支持
    if (cssCompiler_) {
        auto cssComp = std::dynamic_pointer_cast<ICSSCompiler>(cssCompiler_);
        if (cssComp && !cssComp->supportsANTLR()) {
            debugLog("警告: CSS编译器不支持ANTLR");
        }
    }
    
    // 验证JavaScript编译器的ANTLR支持
    if (jsCompiler_) {
        auto jsComp = std::dynamic_pointer_cast<IJavaScriptCompiler>(jsCompiler_);
        if (jsComp && !jsComp->supportsANTLR()) {
            debugLog("警告: JavaScript编译器不支持ANTLR");
        }
    }
    
    return true;
}

bool CompilerDispatcher::initializeCompilers() {
    // 创建编译器实例
    chtlCompiler_ = CompilerFactory::createCHTLCompiler();
    chtlJsCompiler_ = CompilerFactory::createCHTLJSCompiler();
    cssCompiler_ = CompilerFactory::createCSSCompiler();
    jsCompiler_ = CompilerFactory::createJavaScriptCompiler();
    
    // 初始化各个编译器
    bool allInitialized = true;
    
    if (chtlCompiler_) {
        if (!chtlCompiler_->initialize()) {
            debugLog("CHTL编译器初始化失败");
            allInitialized = false;
        } else {
            debugLog("CHTL编译器初始化成功: " + chtlCompiler_->getId());
        }
    }
    
    if (chtlJsCompiler_) {
        if (!chtlJsCompiler_->initialize()) {
            debugLog("CHTL JS编译器初始化失败");
            allInitialized = false;
        } else {
            debugLog("CHTL JS编译器初始化成功: " + chtlJsCompiler_->getId());
        }
    }
    
    if (cssCompiler_) {
        if (!cssCompiler_->initialize()) {
            debugLog("CSS编译器初始化失败");
            allInitialized = false;
        } else {
            debugLog("CSS编译器初始化成功: " + cssCompiler_->getId());
        }
    }
    
    if (jsCompiler_) {
        if (!jsCompiler_->initialize()) {
            debugLog("JavaScript编译器初始化失败");
            allInitialized = false;
        } else {
            debugLog("JavaScript编译器初始化成功: " + jsCompiler_->getId());
        }
    }
    
    return allInitialized;
}

std::string CompilerDispatcher::generateCompilationReport(const std::vector<FragmentCompilationResult>& results) {
    std::ostringstream report;
    
    report << "=== CHTL编译报告 ===\n";
    report << "总片段数: " << results.size() << "\n";
    
    // 统计各类型片段
    std::unordered_map<std::string, int> typeCount;
    std::unordered_map<std::string, int> successCount;
    
    for (const auto& result : results) {
        std::string type;
        switch (result.fragment.type) {
            case CodeFragmentType::CHTL_FRAGMENT: type = "CHTL"; break;
            case CodeFragmentType::CHTL_JS_FRAGMENT: type = "CHTL_JS"; break;
            case CodeFragmentType::CSS_FRAGMENT: type = "CSS"; break;
            case CodeFragmentType::JS_FRAGMENT: type = "JavaScript"; break;
        }
        
        typeCount[type]++;
        if (result.result.success) {
            successCount[type]++;
        }
    }
    
    report << "\n片段类型统计:\n";
    for (const auto& [type, count] : typeCount) {
        int success = successCount[type];
        report << "  " << type << ": " << success << "/" << count << " 成功\n";
    }
    
    // 编译器使用统计
    std::unordered_map<std::string, int> compilerUsage;
    for (const auto& result : results) {
        compilerUsage[result.compilerId]++;
    }
    
    report << "\n编译器使用统计:\n";
    for (const auto& [compiler, count] : compilerUsage) {
        report << "  " << compiler << ": " << count << " 片段\n";
    }
    
    return report.str();
}

void CompilerDispatcher::handleCompilationError(const std::string& compilerId, const CompilationResult& result) {
    if (debugMode_) {
        debugLog("编译器 " + compilerId + " 编译失败:");
        for (const auto& error : result.errors) {
            debugLog("  错误: " + error);
        }
        for (const auto& warning : result.warnings) {
            debugLog("  警告: " + warning);
        }
    }
}

std::string CompilerDispatcher::optimizeHtmlOutput(const std::string& html) {
    std::string optimized = html;
    
    // 移除多余的空行
    std::regex multipleNewlines(R"(\n\s*\n\s*\n)");
    optimized = std::regex_replace(optimized, multipleNewlines, "\n\n");
    
    // 移除行首尾空白
    std::regex leadingTrailingSpaces(R"([ \t]*\n[ \t]*)");
    optimized = std::regex_replace(optimized, leadingTrailingSpaces, "\n");
    
    return optimized;
}

// ==================== CompilerFactory 实现 ====================

bool CompilerFactory::debugMode_ = false;

// 声明外部工厂函数
extern std::shared_ptr<ICHTLCompiler> createBasicCHTLCompiler();

std::shared_ptr<ICHTLCompiler> CompilerFactory::createCHTLCompiler() {
    if (debugMode_) {
        std::cout << "[CompilerFactory] 创建基础CHTL编译器" << std::endl;
    }
    return createBasicCHTLCompiler();
}

std::shared_ptr<ICHTLJSCompiler> CompilerFactory::createCHTLJSCompiler() {
    // 这里应该返回实际的CHTL JS编译器实现
    if (debugMode_) {
        std::cout << "[CompilerFactory] 创建CHTL JS编译器（待实现）" << std::endl;
    }
    return nullptr;
}

std::shared_ptr<ICSSCompiler> CompilerFactory::createCSSCompiler() {
    // 这里应该返回实际的CSS编译器实现（基于ANTLR）
    if (debugMode_) {
        std::cout << "[CompilerFactory] 创建CSS编译器（待实现）" << std::endl;
    }
    return nullptr;
}

std::shared_ptr<IJavaScriptCompiler> CompilerFactory::createJavaScriptCompiler() {
    // 这里应该返回实际的JavaScript编译器实现（基于ANTLR）
    if (debugMode_) {
        std::cout << "[CompilerFactory] 创建JavaScript编译器（待实现）" << std::endl;
    }
    return nullptr;
}

// ==================== CompilerRegistry 实现 ====================

std::shared_ptr<ICompiler> CompilerRegistry::createCompiler(const std::string& id) {
    // 编译器注册和创建逻辑（待扩展）
    return nullptr;
}

std::vector<std::string> CompilerRegistry::getAvailableCompilers() {
    // 返回可用编译器列表（待扩展）
    return {"CHTL", "CHTL_JS", "CSS_ANTLR", "JavaScript_ANTLR"};
}

// 静态调试函数
static void debugLog(const std::string& message) {
    std::cout << "[CompilerFactory] " << message << std::endl;
}

} // namespace chtl
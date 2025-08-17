#include "../../include/dispatcher/CompilerDispatcher.h"
#include "../../include/common/Utils.h"
#include <iostream>

namespace CHTL {

CompilerDispatcher::CompilerDispatcher() {
    initializeCompilers();
}

CompilerDispatcher::CompilerDispatcher(const Configuration& cfg) : config(cfg) {
    initializeCompilers();
    configureCompilers(cfg);
}

CompilerDispatcher::~CompilerDispatcher() {
    // 智能指针会自动清理
}

void CompilerDispatcher::initializeCompilers() {
    chtl_compiler = std::make_unique<CHTLCompiler>(config);
    chtl_js_compiler = std::make_unique<CHTLJSCompiler>(config);
    css_compiler = std::make_unique<CSSCompiler>(config);
    js_compiler = std::make_unique<JavaScriptCompiler>(config);
}

void CompilerDispatcher::setConfiguration(const Configuration& cfg) {
    config = cfg;
    configureCompilers(cfg);
}

void CompilerDispatcher::configureCompilers(const Configuration& cfg) {
    if (chtl_compiler) {
        chtl_compiler->setConfiguration(cfg);
    }
    if (chtl_js_compiler) {
        chtl_js_compiler->setConfiguration(cfg);
    }
    if (css_compiler) {
        css_compiler->setConfiguration(cfg);
    }
    if (js_compiler) {
        js_compiler->setConfiguration(cfg);
    }
}

CompilerType CompilerDispatcher::getCompilerType(FragmentType fragment_type) const {
    switch (fragment_type) {
        case FragmentType::CHTL:
            return CompilerType::CHTL_COMPILER;
        case FragmentType::CHTL_JS:
            return CompilerType::CHTL_JS_COMPILER;
        case FragmentType::CSS:
            return CompilerType::CSS_COMPILER;
        case FragmentType::JAVASCRIPT:
            return CompilerType::JS_COMPILER;
        default:
            return CompilerType::CHTL_COMPILER; // 默认
    }
}

CompileResultPtr CompilerDispatcher::dispatchToCompiler(FragmentPtr fragment) {
    if (!fragment) {
        auto result = std::make_shared<CompileResult>();
        result->success = false;
        result->errors.push_back("空的代码片段");
        return result;
    }
    
    CompilerType compiler_type = getCompilerType(fragment->type);
    
    if (config.debug_mode) {
        Utils::debugLog("DISPATCHER", "分发片段到编译器: " + 
                       std::to_string(static_cast<int>(compiler_type)));
    }
    
    try {
        switch (compiler_type) {
            case CompilerType::CHTL_COMPILER:
                if (chtl_compiler) {
                    return chtl_compiler->compile(fragment->content);
                }
                break;
                
            case CompilerType::CHTL_JS_COMPILER:
                if (chtl_js_compiler) {
                    return chtl_js_compiler->compile(fragment->content);
                }
                break;
                
            case CompilerType::CSS_COMPILER:
                if (css_compiler) {
                    return css_compiler->compile(fragment->content);
                }
                break;
                
            case CompilerType::JS_COMPILER:
                if (js_compiler) {
                    return js_compiler->compile(fragment->content);
                }
                break;
        }
    } catch (const std::exception& e) {
        auto result = std::make_shared<CompileResult>();
        result->success = false;
        result->errors.push_back("编译器异常: " + std::string(e.what()));
        return result;
    }
    
    // 如果到达这里，说明编译器不可用
    auto result = std::make_shared<CompileResult>();
    result->success = false;
    result->errors.push_back("编译器不可用");
    return result;
}

std::vector<CompileResultPtr> CompilerDispatcher::compileFragments(const std::vector<FragmentPtr>& fragments) {
    std::vector<CompileResultPtr> results;
    compile_results.clear();
    
    if (config.debug_mode) {
        Utils::debugLog("DISPATCHER", "开始编译 " + std::to_string(fragments.size()) + " 个代码片段");
    }
    
    for (const auto& fragment : fragments) {
        CompileResultPtr result = dispatchToCompiler(fragment);
        results.push_back(result);
        compile_results.push_back(result);
        
        if (config.debug_mode) {
            std::string status = result->success ? "成功" : "失败";
            Utils::debugLog("DISPATCHER", "片段编译 " + status);
            if (!result->success && !result->errors.empty()) {
                for (const auto& error : result->errors) {
                    Utils::debugLog("ERROR", error);
                }
            }
        }
    }
    
    return results;
}

CompileResultPtr CompilerDispatcher::compileFragment(FragmentPtr fragment) {
    return dispatchToCompiler(fragment);
}

void CompilerDispatcher::resetCompilers() {
    if (chtl_compiler) {
        chtl_compiler->reset();
    }
    if (chtl_js_compiler) {
        chtl_js_compiler->reset();
    }
    if (css_compiler) {
        css_compiler->reset();
    }
    if (js_compiler) {
        js_compiler->reset();
    }
    
    compile_results.clear();
}

std::vector<std::string> CompilerDispatcher::getAllErrors() const {
    std::vector<std::string> all_errors;
    
    if (chtl_compiler) {
        auto errors = chtl_compiler->getErrors();
        all_errors.insert(all_errors.end(), errors.begin(), errors.end());
    }
    
    if (chtl_js_compiler) {
        auto errors = chtl_js_compiler->getErrors();
        all_errors.insert(all_errors.end(), errors.begin(), errors.end());
    }
    
    if (css_compiler) {
        auto errors = css_compiler->getErrors();
        all_errors.insert(all_errors.end(), errors.begin(), errors.end());
    }
    
    if (js_compiler) {
        auto errors = js_compiler->getErrors();
        all_errors.insert(all_errors.end(), errors.begin(), errors.end());
    }
    
    return all_errors;
}

std::vector<std::string> CompilerDispatcher::getAllWarnings() const {
    std::vector<std::string> all_warnings;
    
    if (chtl_compiler) {
        auto warnings = chtl_compiler->getWarnings();
        all_warnings.insert(all_warnings.end(), warnings.begin(), warnings.end());
    }
    
    if (chtl_js_compiler) {
        auto warnings = chtl_js_compiler->getWarnings();
        all_warnings.insert(all_warnings.end(), warnings.begin(), warnings.end());
    }
    
    if (css_compiler) {
        auto warnings = css_compiler->getWarnings();
        all_warnings.insert(all_warnings.end(), warnings.begin(), warnings.end());
    }
    
    if (js_compiler) {
        auto warnings = js_compiler->getWarnings();
        all_warnings.insert(all_warnings.end(), warnings.begin(), warnings.end());
    }
    
    return all_warnings;
}

size_t CompilerDispatcher::getCompiledFragmentCount() const {
    return compile_results.size();
}

size_t CompilerDispatcher::getSuccessfulCompileCount() const {
    size_t count = 0;
    for (const auto& result : compile_results) {
        if (result && result->success) {
            count++;
        }
    }
    return count;
}

size_t CompilerDispatcher::getFailedCompileCount() const {
    return getCompiledFragmentCount() - getSuccessfulCompileCount();
}

void CompilerDispatcher::dumpCompileResults() const {
    std::cout << "=== 编译结果统计 ===" << std::endl;
    std::cout << "总片段数: " << getCompiledFragmentCount() << std::endl;
    std::cout << "成功编译: " << getSuccessfulCompileCount() << std::endl;
    std::cout << "编译失败: " << getFailedCompileCount() << std::endl;
    
    auto errors = getAllErrors();
    auto warnings = getAllWarnings();
    
    if (!errors.empty()) {
        std::cout << "\n错误信息 (" << errors.size() << "):" << std::endl;
        for (const auto& error : errors) {
            std::cout << "  " << error << std::endl;
        }
    }
    
    if (!warnings.empty()) {
        std::cout << "\n警告信息 (" << warnings.size() << "):" << std::endl;
        for (const auto& warning : warnings) {
            std::cout << "  " << warning << std::endl;
        }
    }
    
    std::cout << "==================" << std::endl;
}

void CompilerDispatcher::setDebugMode(bool enabled) {
    config.debug_mode = enabled;
    
    if (chtl_compiler) {
        chtl_compiler->setDebugMode(enabled);
    }
    if (chtl_js_compiler) {
        chtl_js_compiler->setDebugMode(enabled);
    }
    if (css_compiler) {
        css_compiler->setDebugMode(enabled);
    }
    if (js_compiler) {
        js_compiler->setDebugMode(enabled);
    }
}

} // namespace CHTL
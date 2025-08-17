#include "CSSCompiler.h"
#include <iostream>

namespace CHTL {

CSSCompiler::CSSCompiler() : debugMode_(false) {
    debugOutput("CSS编译器初始化");
}

CompilationResultPtr CSSCompiler::compile(const FragmentPtr& fragment) {
    debugOutput("编译CSS片段: " + fragment->getTypeName());
    
    auto result = std::make_shared<CompilationResult>();
    result->sourceType = FragmentType::CSS;
    result->context = fragment->context;
    
    try {
        // 简单的CSS编译 - 直接传递内容
        std::string cssOutput = compileFragment(fragment->content, fragment->context);
        
        result->success = true;
        result->output = cssOutput;
        
        debugOutput("CSS片段编译成功，输出长度: " + std::to_string(cssOutput.length()));
        
    } catch (const std::exception& e) {
        result->success = false;
        result->errors.push_back("CSS编译错误: " + std::string(e.what()));
        recordError("编译失败: " + std::string(e.what()));
    }
    
    result->errors.insert(result->errors.end(), errors_.begin(), errors_.end());
    result->warnings.insert(result->warnings.end(), warnings_.begin(), warnings_.end());
    
    errors_.clear();
    warnings_.clear();
    
    return result;
}

CompilationResults CSSCompiler::compileAll(const FragmentList& fragments) {
    CompilationResults results;
    
    debugOutput("批量编译 " + std::to_string(fragments.size()) + " 个CSS片段");
    
    for (const auto& fragment : fragments) {
        auto result = compile(fragment);
        results.push_back(result);
    }
    
    return results;
}

void CSSCompiler::setOptions(const std::unordered_map<std::string, std::string>& options) {
    options_ = options;
    debugOutput("设置编译选项，共 " + std::to_string(options.size()) + " 个选项");
}

std::string CSSCompiler::compileFragment(const std::string& content, const std::string& context) {
    debugOutput("编译CSS片段内容，长度: " + std::to_string(content.length()));
    
    // 简化实现：直接返回CSS内容
    // 在完整实现中，这里会使用ANTLR进行CSS解析和优化
    return content;
}

void CSSCompiler::recordError(const std::string& message) {
    errors_.push_back(message);
    if (debugMode_) {
        std::cerr << "[CSSCompiler] 错误: " << message << std::endl;
    }
}

void CSSCompiler::recordWarning(const std::string& message) {
    warnings_.push_back(message);
    if (debugMode_) {
        std::cout << "[CSSCompiler] 警告: " << message << std::endl;
    }
}

void CSSCompiler::debugOutput(const std::string& message) {
    if (debugMode_) {
        std::cout << "[CSSCompiler] " << message << std::endl;
    }
}

} // namespace CHTL
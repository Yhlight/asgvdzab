#include "JSCompiler.h"
#include <iostream>

namespace CHTL {

JSCompiler::JSCompiler() : debugMode_(false) {
    debugOutput("JavaScript编译器初始化");
}

CompilationResultPtr JSCompiler::compile(const FragmentPtr& fragment) {
    debugOutput("编译JavaScript片段: " + fragment->getTypeName());
    
    auto result = std::make_shared<CompilationResult>();
    result->sourceType = FragmentType::JAVASCRIPT;
    result->context = fragment->context;
    
    try {
        // 简单的JavaScript编译 - 直接传递内容
        std::string jsOutput = compileFragment(fragment->content, fragment->context);
        
        result->success = true;
        result->output = jsOutput;
        
        debugOutput("JavaScript片段编译成功，输出长度: " + std::to_string(jsOutput.length()));
        
    } catch (const std::exception& e) {
        result->success = false;
        result->errors.push_back("JavaScript编译错误: " + std::string(e.what()));
        recordError("编译失败: " + std::string(e.what()));
    }
    
    result->errors.insert(result->errors.end(), errors_.begin(), errors_.end());
    result->warnings.insert(result->warnings.end(), warnings_.begin(), warnings_.end());
    
    errors_.clear();
    warnings_.clear();
    
    return result;
}

CompilationResults JSCompiler::compileAll(const FragmentList& fragments) {
    CompilationResults results;
    
    debugOutput("批量编译 " + std::to_string(fragments.size()) + " 个JavaScript片段");
    
    for (const auto& fragment : fragments) {
        auto result = compile(fragment);
        results.push_back(result);
    }
    
    return results;
}

void JSCompiler::setOptions(const std::unordered_map<std::string, std::string>& options) {
    options_ = options;
    debugOutput("设置编译选项，共 " + std::to_string(options.size()) + " 个选项");
}

std::string JSCompiler::compileFragment(const std::string& content, const std::string& context) {
    debugOutput("编译JavaScript片段内容，长度: " + std::to_string(content.length()));
    
    // 简化实现：直接返回JavaScript内容
    // 在完整实现中，这里会使用ANTLR进行JavaScript解析和优化
    return content;
}

void JSCompiler::recordError(const std::string& message) {
    errors_.push_back(message);
    if (debugMode_) {
        std::cerr << "[JSCompiler] 错误: " << message << std::endl;
    }
}

void JSCompiler::recordWarning(const std::string& message) {
    warnings_.push_back(message);
    if (debugMode_) {
        std::cout << "[JSCompiler] 警告: " << message << std::endl;
    }
}

void JSCompiler::debugOutput(const std::string& message) {
    if (debugMode_) {
        std::cout << "[JSCompiler] " << message << std::endl;
    }
}

} // namespace CHTL
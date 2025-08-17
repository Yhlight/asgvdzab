#include "CHTLJSCompiler.h"
#include <iostream>

namespace CHTL {

CHTLJSCompiler::CHTLJSCompiler() : debugMode_(false) {
    debugOutput("CHTL JS编译器初始化");
}

CompilationResultPtr CHTLJSCompiler::compile(const FragmentPtr& fragment) {
    debugOutput("编译CHTL JS片段: " + fragment->getTypeName());
    
    auto result = std::make_shared<CompilationResult>();
    result->sourceType = FragmentType::CHTL_JS;
    result->context = fragment->context;
    
    try {
        // 简单的编译实现 - 将CHTL JS语法转换为JavaScript
        std::string jsOutput = compileFragment(fragment->content, fragment->context);
        
        result->success = true;
        result->output = jsOutput;
        
        debugOutput("CHTL JS片段编译成功，输出长度: " + std::to_string(jsOutput.length()));
        
    } catch (const std::exception& e) {
        result->success = false;
        result->errors.push_back("CHTL JS编译错误: " + std::string(e.what()));
        recordError("编译失败: " + std::string(e.what()));
    }
    
    // 添加收集的错误和警告
    result->errors.insert(result->errors.end(), errors_.begin(), errors_.end());
    result->warnings.insert(result->warnings.end(), warnings_.begin(), warnings_.end());
    
    // 清除临时错误和警告
    errors_.clear();
    warnings_.clear();
    
    return result;
}

CompilationResults CHTLJSCompiler::compileAll(const FragmentList& fragments) {
    CompilationResults results;
    
    debugOutput("批量编译 " + std::to_string(fragments.size()) + " 个CHTL JS片段");
    
    for (const auto& fragment : fragments) {
        auto result = compile(fragment);
        results.push_back(result);
    }
    
    return results;
}

void CHTLJSCompiler::setOptions(const std::unordered_map<std::string, std::string>& options) {
    options_ = options;
    debugOutput("设置编译选项，共 " + std::to_string(options.size()) + " 个选项");
}

std::string CHTLJSCompiler::compileFragment(const std::string& content, const std::string& context) {
    debugOutput("编译CHTL JS片段内容，长度: " + std::to_string(content.length()));
    
    // 简化实现：将CHTL JS语法转换为标准JavaScript
    std::string processedContent = content;
    
    // 处理增强选择器 {{selector}} -> document.querySelector('selector')
    size_t pos = 0;
    while ((pos = processedContent.find("{{", pos)) != std::string::npos) {
        size_t endPos = processedContent.find("}}", pos);
        if (endPos != std::string::npos) {
            std::string selector = processedContent.substr(pos + 2, endPos - pos - 2);
            
            // 去除前后空白
            size_t start = selector.find_first_not_of(" \t\n\r");
            size_t end = selector.find_last_not_of(" \t\n\r");
            if (start != std::string::npos) {
                selector = selector.substr(start, end - start + 1);
            }
            
            std::string replacement;
            if (selector.front() == '.' || selector.front() == '#') {
                // CSS选择器
                replacement = "document.querySelector('" + selector + "')";
            } else {
                // 标签选择器
                replacement = "document.querySelector('" + selector + "')";
            }
            
            processedContent.replace(pos, endPos - pos + 2, replacement);
            pos += replacement.length();
        } else {
            pos++;
        }
    }
    
    // 处理 -> 操作符，替换为 .
    pos = 0;
    while ((pos = processedContent.find("->", pos)) != std::string::npos) {
        processedContent.replace(pos, 2, ".");
        pos += 1;
    }
    
    // 处理listen函数调用
    pos = 0;
    while ((pos = processedContent.find("listen(", pos)) != std::string::npos) {
        recordWarning("listen函数需要更复杂的处理，当前仅做简单替换");
        // 简单替换为addEventListener的形式
        // 这里需要更复杂的解析逻辑
        pos += 7;
    }
    
    return processedContent;
}

void CHTLJSCompiler::recordError(const std::string& message) {
    errors_.push_back(message);
    if (debugMode_) {
        std::cerr << "[CHTLJSCompiler] 错误: " << message << std::endl;
    }
}

void CHTLJSCompiler::recordWarning(const std::string& message) {
    warnings_.push_back(message);
    if (debugMode_) {
        std::cout << "[CHTLJSCompiler] 警告: " << message << std::endl;
    }
}

void CHTLJSCompiler::debugOutput(const std::string& message) {
    if (debugMode_) {
        std::cout << "[CHTLJSCompiler] " << message << std::endl;
    }
}

} // namespace CHTL
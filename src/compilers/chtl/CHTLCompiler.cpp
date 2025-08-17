#include "CHTLCompiler.h"
#include <iostream>
#include <sstream>

namespace CHTL {

CHTLCompiler::CHTLCompiler() : debugMode_(false) {
    debugOutput("CHTL编译器初始化");
}

CompilationResultPtr CHTLCompiler::compile(const FragmentPtr& fragment) {
    debugOutput("编译CHTL片段: " + fragment->getTypeName());
    
    auto result = std::make_shared<CompilationResult>();
    result->sourceType = FragmentType::CHTL;
    result->context = fragment->context;
    
    try {
        // 简单的编译实现 - 将CHTL语法转换为HTML
        std::string htmlOutput = compileFragment(fragment->content, fragment->context);
        
        result->success = true;
        result->output = htmlOutput;
        
        debugOutput("CHTL片段编译成功，输出长度: " + std::to_string(htmlOutput.length()));
        
    } catch (const std::exception& e) {
        result->success = false;
        result->errors.push_back("CHTL编译错误: " + std::string(e.what()));
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

CompilationResults CHTLCompiler::compileAll(const FragmentList& fragments) {
    CompilationResults results;
    
    debugOutput("批量编译 " + std::to_string(fragments.size()) + " 个CHTL片段");
    
    for (const auto& fragment : fragments) {
        auto result = compile(fragment);
        results.push_back(result);
    }
    
    return results;
}

void CHTLCompiler::setOptions(const std::unordered_map<std::string, std::string>& options) {
    options_ = options;
    debugOutput("设置编译选项，共 " + std::to_string(options.size()) + " 个选项");
}

std::string CHTLCompiler::compileFragment(const std::string& content, const std::string& context) {
    // 这是一个简化的实现，将CHTL语法转换为HTML
    // 在完整实现中，这里会使用词法分析器和语法分析器
    
    debugOutput("编译片段内容，长度: " + std::to_string(content.length()));
    
    std::ostringstream html;
    
    // 简单的文本处理 - 查找基本的CHTL结构
    std::string processedContent = content;
    
    // 处理text块
    size_t textPos = 0;
    while ((textPos = processedContent.find("text", textPos)) != std::string::npos) {
        size_t braceStart = processedContent.find("{", textPos);
        if (braceStart != std::string::npos) {
            size_t braceEnd = processedContent.find("}", braceStart);
            if (braceEnd != std::string::npos) {
                std::string textContent = processedContent.substr(braceStart + 1, braceEnd - braceStart - 1);
                
                // 去除前后空白
                size_t start = textContent.find_first_not_of(" \t\n\r");
                size_t end = textContent.find_last_not_of(" \t\n\r");
                if (start != std::string::npos) {
                    textContent = textContent.substr(start, end - start + 1);
                }
                
                // 去除引号
                if (textContent.front() == '"' && textContent.back() == '"') {
                    textContent = textContent.substr(1, textContent.length() - 2);
                }
                
                // 替换text块为实际内容
                std::string replacement = textContent;
                processedContent.replace(textPos, braceEnd - textPos + 1, replacement);
                textPos += replacement.length();
            } else {
                textPos++;
            }
        } else {
            textPos++;
        }
    }
    
    // 处理基本HTML元素
    std::string htmlElements[] = {"div", "span", "p", "h1", "h2", "h3", "body", "html", "head"};
    
    for (const std::string& element : htmlElements) {
        size_t pos = 0;
        while ((pos = processedContent.find(element, pos)) != std::string::npos) {
            // 检查这是否是一个独立的元素名
            if ((pos == 0 || !std::isalnum(processedContent[pos - 1])) &&
                (pos + element.length() >= processedContent.length() || 
                 (!std::isalnum(processedContent[pos + element.length()]) && 
                  processedContent[pos + element.length()] != '_'))) {
                
                size_t braceStart = processedContent.find("{", pos);
                if (braceStart != std::string::npos) {
                    size_t braceEnd = findMatchingBrace(processedContent, braceStart);
                    if (braceEnd != std::string::npos) {
                        std::string elementContent = processedContent.substr(braceStart + 1, braceEnd - braceStart - 1);
                        
                        // 递归处理内容
                        std::string innerHtml = compileFragment(elementContent, element);
                        
                        // 构建HTML标签
                        std::string replacement = "<" + element + ">" + innerHtml + "</" + element + ">";
                        processedContent.replace(pos, braceEnd - pos + 1, replacement);
                        pos += replacement.length();
                    } else {
                        pos++;
                    }
                } else {
                    pos++;
                }
            } else {
                pos++;
            }
        }
    }
    
    return processedContent;
}

size_t CHTLCompiler::findMatchingBrace(const std::string& content, size_t startPos) {
    if (startPos >= content.length() || content[startPos] != '{') {
        return std::string::npos;
    }
    
    int depth = 1;
    size_t pos = startPos + 1;
    
    while (pos < content.length() && depth > 0) {
        if (content[pos] == '{') {
            depth++;
        } else if (content[pos] == '}') {
            depth--;
        }
        pos++;
    }
    
    return (depth == 0) ? pos - 1 : std::string::npos;
}

void CHTLCompiler::recordError(const std::string& message) {
    errors_.push_back(message);
    if (debugMode_) {
        std::cerr << "[CHTLCompiler] 错误: " << message << std::endl;
    }
}

void CHTLCompiler::recordWarning(const std::string& message) {
    warnings_.push_back(message);
    if (debugMode_) {
        std::cout << "[CHTLCompiler] 警告: " << message << std::endl;
    }
}

void CHTLCompiler::debugOutput(const std::string& message) {
    if (debugMode_) {
        std::cout << "[CHTLCompiler] " << message << std::endl;
    }
}

} // namespace CHTL
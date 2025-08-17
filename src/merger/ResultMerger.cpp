#include "ResultMerger.h"
#include <iostream>

namespace CHTL {

ResultMerger::ResultMerger() : debugMode_(false) {
    htmlOutput_ = std::make_unique<HTMLOutput>();
    debugOutput("结果合并器初始化");
}

std::string ResultMerger::merge(const CompilationResults& results, const std::string& outputFilename) {
    debugOutput("开始合并 " + std::to_string(results.size()) + " 个编译结果");
    
    // 清空之前的输出
    htmlOutput_->clear();
    errors_.clear();
    warnings_.clear();
    
    // 设置文档基本信息
    std::string title = "CHTL Generated Page";
    if (!outputFilename.empty()) {
        size_t lastSlash = outputFilename.find_last_of("/\\");
        size_t lastDot = outputFilename.find_last_of(".");
        if (lastDot != std::string::npos && lastDot > lastSlash) {
            title = outputFilename.substr(lastSlash == std::string::npos ? 0 : lastSlash + 1, 
                                        lastDot - (lastSlash == std::string::npos ? 0 : lastSlash + 1));
        }
    }
    htmlOutput_->setDocumentStructure(title);
    
    // 按类型合并结果
    mergeHTMLContent(results);
    mergeCSSContent(results);
    mergeJSContent(results);
    
    // 生成最终HTML
    std::string finalHTML = htmlOutput_->generateHTML();
    
    debugOutput("合并完成，最终HTML长度: " + std::to_string(finalHTML.length()));
    
    return finalHTML;
}

void ResultMerger::mergeHTMLContent(const CompilationResults& results) {
    debugOutput("合并HTML内容");
    
    for (const auto& result : results) {
        if (!result->success) {
            continue; // 跳过失败的结果
        }
        
        if (result->sourceType == FragmentType::CHTL || 
            result->sourceType == FragmentType::HTML) {
            
            if (!result->output.empty()) {
                htmlOutput_->addHTMLContent(result->output);
                debugOutput("添加HTML内容，长度: " + std::to_string(result->output.length()));
            }
        }
    }
}

void ResultMerger::mergeCSSContent(const CompilationResults& results) {
    debugOutput("合并CSS内容");
    
    std::string combinedCSS;
    
    for (const auto& result : results) {
        if (!result->success) {
            continue; // 跳过失败的结果
        }
        
        if (result->sourceType == FragmentType::CSS) {
            if (!result->output.empty()) {
                if (!combinedCSS.empty()) {
                    combinedCSS += "\n\n";
                }
                combinedCSS += "/* " + result->context + " */\n";
                combinedCSS += result->output;
                debugOutput("添加CSS内容，长度: " + std::to_string(result->output.length()));
            }
        }
    }
    
    if (!combinedCSS.empty()) {
        htmlOutput_->addCSS(combinedCSS);
    }
}

void ResultMerger::mergeJSContent(const CompilationResults& results) {
    debugOutput("合并JavaScript内容");
    
    std::string combinedJS;
    
    for (const auto& result : results) {
        if (!result->success) {
            continue; // 跳过失败的结果
        }
        
        if (result->sourceType == FragmentType::CHTL_JS || 
            result->sourceType == FragmentType::JAVASCRIPT) {
            
            if (!result->output.empty()) {
                if (!combinedJS.empty()) {
                    combinedJS += "\n\n";
                }
                combinedJS += "// " + result->context + "\n";
                combinedJS += result->output;
                debugOutput("添加JavaScript内容，长度: " + std::to_string(result->output.length()));
            }
        }
    }
    
    if (!combinedJS.empty()) {
        htmlOutput_->addJavaScript(combinedJS);
    }
}

void ResultMerger::recordError(const std::string& message) {
    errors_.push_back(message);
    if (debugMode_) {
        std::cerr << "[ResultMerger] 错误: " << message << std::endl;
    }
}

void ResultMerger::recordWarning(const std::string& message) {
    warnings_.push_back(message);
    if (debugMode_) {
        std::cout << "[ResultMerger] 警告: " << message << std::endl;
    }
}

void ResultMerger::debugOutput(const std::string& message) {
    if (debugMode_) {
        std::cout << "[ResultMerger] " << message << std::endl;
    }
}

} // namespace CHTL
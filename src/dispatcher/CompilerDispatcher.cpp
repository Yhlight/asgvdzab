#include "CompilerDispatcher.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include "chtl/CHTLCompiler.hpp"
#include "chtljs/CHTLJSCompiler.hpp"
#include "css/CSSCompiler.hpp"
#include "javascript/JavaScriptCompiler.hpp"

namespace fs = std::filesystem;

CompilerDispatcher::CompilerDispatcher(bool debugMode) 
    : debugMode_(debugMode) {
    scanner_ = std::make_unique<CHTLUnifiedScanner>();
    chtlCompiler_ = std::make_unique<CHTLCompiler>();
    chtljsCompiler_ = std::make_unique<CHTLJSCompiler>();
    cssCompiler_ = std::make_unique<CSSCompiler>();
    jsCompiler_ = std::make_unique<JavaScriptCompiler>();
}

CompilerDispatcher::~CompilerDispatcher() {
}

bool CompilerDispatcher::compile(const std::string& inputFile, const std::string& outputDir) {
    try {
        // 读取源文件
        std::string source = readFile(inputFile);
        if (source.empty()) {
            std::cerr << "错误：无法读取文件或文件为空：" << inputFile << std::endl;
            return false;
        }
        
        if (debugMode_) {
            std::cout << "开始扫描文件：" << inputFile << std::endl;
        }
        
        // 使用扫描器切割代码
        auto fragments = scanner_->scan(source);
        
        if (debugMode_) {
            std::cout << "扫描完成，共 " << fragments.size() << " 个片段" << std::endl;
        }
        
        // 处理每个片段
        std::vector<std::string> results;
        for (const auto& fragment : fragments) {
            if (debugMode_) {
                std::cout << "处理片段：类型=" << static_cast<int>(fragment.type) 
                         << "，行=" << fragment.startLine << "-" << fragment.endLine << std::endl;
            }
            
            std::string result = processFragment(fragment);
            results.push_back(result);
        }
        
        // 合并结果
        std::string mergedContent = mergeResults(results);
        
        // 生成HTML
        std::string html = generateHTML(mergedContent);
        
        // 确定输出文件名
        fs::path inputPath(inputFile);
        fs::path outputPath(outputDir);
        outputPath /= inputPath.stem().string() + ".html";
        
        // 写入输出文件
        if (!writeFile(outputPath.string(), html)) {
            std::cerr << "错误：无法写入输出文件：" << outputPath << std::endl;
            return false;
        }
        
        if (debugMode_) {
            std::cout << "编译完成：" << outputPath << std::endl;
        }
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "编译错误：" << e.what() << std::endl;
        return false;
    }
}

std::string CompilerDispatcher::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool CompilerDispatcher::writeFile(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }
    
    file << content;
    return file.good();
}

std::string CompilerDispatcher::processFragment(const CodeFragment& fragment) {
    switch (fragment.type) {
        case FragmentType::CHTL:
            return chtlCompiler_->compile(fragment.content);
            
        case FragmentType::CHTL_JS:
            return chtljsCompiler_->compile(fragment.content);
            
        case FragmentType::CSS:
            return cssCompiler_->compile(fragment.content);
            
        case FragmentType::JAVASCRIPT:
            return jsCompiler_->compile(fragment.content);
            
        default:
            return "";
    }
}

std::string CompilerDispatcher::mergeResults(const std::vector<std::string>& results) {
    std::stringstream merged;
    
    // 收集不同类型的内容
    std::string bodyContent;
    std::string styleContent;
    std::string scriptContent;
    
    for (const auto& result : results) {
        // 简单的内容分类（实际实现会更复杂）
        if (result.find("<style>") != std::string::npos) {
            styleContent += result;
        } else if (result.find("<script>") != std::string::npos) {
            scriptContent += result;
        } else {
            bodyContent += result;
        }
    }
    
    // 组合内容
    if (!styleContent.empty()) {
        merged << styleContent << "\n";
    }
    
    merged << bodyContent;
    
    if (!scriptContent.empty()) {
        merged << "\n" << scriptContent;
    }
    
    return merged.str();
}

std::string CompilerDispatcher::generateHTML(const std::string& content) {
    std::stringstream html;
    
    html << "<!DOCTYPE html>\n";
    html << "<html>\n";
    html << "<head>\n";
    html << "    <meta charset=\"UTF-8\">\n";
    html << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    html << "    <title>CHTL Generated Page</title>\n";
    
    // 提取并放置样式到head中
    size_t styleStart = content.find("<style>");
    size_t styleEnd = content.find("</style>");
    if (styleStart != std::string::npos && styleEnd != std::string::npos) {
        html << "    " << content.substr(styleStart, styleEnd - styleStart + 8) << "\n";
    }
    
    html << "</head>\n";
    html << "<body>\n";
    
    // 添加body内容（排除style和script标签）
    std::string bodyContent = content;
    if (styleStart != std::string::npos && styleEnd != std::string::npos) {
        bodyContent.erase(styleStart, styleEnd - styleStart + 8);
    }
    
    size_t scriptStart = bodyContent.find("<script>");
    size_t scriptEnd = bodyContent.find("</script>");
    std::string scripts;
    if (scriptStart != std::string::npos && scriptEnd != std::string::npos) {
        scripts = bodyContent.substr(scriptStart, scriptEnd - scriptStart + 9);
        bodyContent.erase(scriptStart, scriptEnd - scriptStart + 9);
    }
    
    html << bodyContent;
    
    // 添加脚本到body末尾
    if (!scripts.empty()) {
        html << scripts << "\n";
    }
    
    html << "</body>\n";
    html << "</html>\n";
    
    return html.str();
}
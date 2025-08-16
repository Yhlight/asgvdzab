#include "merger/result_merger.h"
#include "common/utils.h"
#include <sstream>
#include <algorithm>

namespace chtl {
namespace merger {

ResultMerger::ResultMerger() {
}

ResultMerger::~ResultMerger() = default;

std::string ResultMerger::merge(const dispatcher::DispatchResult& dispatchResult,
                               const HTMLOutputOptions& options) {
    errors_.clear();
    warnings_.clear();
    
    if (!dispatchResult.success) {
        errors_.push_back("Compilation failed, cannot merge results");
        return "";
    }
    
    // 生成HTML结构
    std::string html = generateHTMLStructure(dispatchResult.results, options);
    
    // 格式化输出
    if (options.prettyPrint) {
        html = formatHTML(html, true);
    }
    
    return html;
}

std::string ResultMerger::generateHTMLStructure(const std::vector<CompilationResult>& results,
                                              const HTMLOutputOptions& options) {
    std::stringstream html;
    
    // DOCTYPE
    html << options.doctype << "\n";
    
    // HTML开始
    html << "<html>\n";
    
    // HEAD部分
    html << "<head>\n";
    html << "  <meta charset=\"" << options.charset << "\">\n";
    html << "  <title>" << utils::escapeHtml(options.title) << "</title>\n";
    
    // 添加额外的head元素
    for (const auto& element : options.additionalHeadElements) {
        html << "  " << element << "\n";
    }
    
    // 收集并添加全局样式
    std::string globalStyles = collectGlobalStyles(results);
    if (!globalStyles.empty()) {
        html << "  <style>\n";
        html << indent(globalStyles, 2);
        html << "  </style>\n";
    }
    
    html << "</head>\n";
    
    // BODY部分
    html << "<body>\n";
    
    // 收集并添加HTML内容
    std::string bodyContent = collectHTMLBody(results);
    html << indent(bodyContent, 1);
    
    // 收集并添加全局脚本
    std::string globalScripts = collectGlobalScripts(results);
    if (!globalScripts.empty()) {
        html << "  <script>\n";
        html << indent(globalScripts, 2);
        html << "  </script>\n";
    }
    
    html << "</body>\n";
    html << "</html>\n";
    
    return html.str();
}

std::string ResultMerger::collectHTMLBody(const std::vector<CompilationResult>& results) {
    std::stringstream body;
    
    for (const auto& result : results) {
        if (!result.output.empty()) {
            // 只收集HTML内容（非样式和脚本）
            if (result.output.find("<style") == std::string::npos &&
                result.output.find("<script") == std::string::npos) {
                body << result.output;
                // 如果输出不以换行结束，添加换行
                if (!result.output.empty() && result.output.back() != '\n') {
                    body << "\n";
                }
            }
        }
    }
    
    return body.str();
}

std::string ResultMerger::collectGlobalStyles(const std::vector<CompilationResult>& results) {
    std::stringstream styles;
    
    for (const auto& result : results) {
        // 收集全局样式
        if (!result.globalStyles.empty()) {
            styles << result.globalStyles;
            if (result.globalStyles.back() != '\n') {
                styles << "\n";
            }
        }
        
        // 也收集<style>标签中的内容
        if (result.output.find("<style") != std::string::npos) {
            // 提取<style>标签内容
            size_t startPos = result.output.find("<style");
            size_t endPos = result.output.find("</style>");
            if (startPos != std::string::npos && endPos != std::string::npos) {
                size_t contentStart = result.output.find('>', startPos) + 1;
                if (contentStart < endPos) {
                    std::string styleContent = result.output.substr(contentStart, endPos - contentStart);
                    styles << styleContent;
                    if (!styleContent.empty() && styleContent.back() != '\n') {
                        styles << "\n";
                    }
                }
            }
        }
    }
    
    return styles.str();
}

std::string ResultMerger::collectGlobalScripts(const std::vector<CompilationResult>& results) {
    std::stringstream scripts;
    
    for (const auto& result : results) {
        // 收集全局脚本
        if (!result.globalScripts.empty()) {
            scripts << result.globalScripts;
            if (result.globalScripts.back() != '\n') {
                scripts << "\n";
            }
        }
        
        // 也收集<script>标签中的内容
        if (result.output.find("<script") != std::string::npos) {
            // 提取<script>标签内容
            size_t startPos = result.output.find("<script");
            size_t endPos = result.output.find("</script>");
            if (startPos != std::string::npos && endPos != std::string::npos) {
                size_t contentStart = result.output.find('>', startPos) + 1;
                if (contentStart < endPos) {
                    std::string scriptContent = result.output.substr(contentStart, endPos - contentStart);
                    scripts << scriptContent;
                    if (!scriptContent.empty() && scriptContent.back() != '\n') {
                        scripts << "\n";
                    }
                }
            }
        }
    }
    
    return scripts.str();
}

std::string ResultMerger::formatHTML(const std::string& html, bool prettyPrint) {
    if (!prettyPrint) {
        return html;
    }
    
    // 简单的HTML格式化
    // 实际项目中可能需要更复杂的格式化逻辑
    return html;
}

std::string ResultMerger::indent(const std::string& content, int level) {
    if (content.empty()) {
        return content;
    }
    
    std::string indentStr(level * 2, ' ');
    std::stringstream result;
    std::istringstream stream(content);
    std::string line;
    
    while (std::getline(stream, line)) {
        if (!line.empty()) {
            result << indentStr << line;
        }
        result << "\n";
    }
    
    std::string output = result.str();
    // 移除最后的额外换行
    if (!output.empty() && output.back() == '\n') {
        output.pop_back();
    }
    
    return output;
}

} // namespace merger
} // namespace chtl
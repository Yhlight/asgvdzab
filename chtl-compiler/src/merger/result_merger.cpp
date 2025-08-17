#include "merger/result_merger.h"
#include <sstream>
#include <algorithm>
#include <regex>

namespace chtl {

ResultMerger::ResultMerger() = default;
ResultMerger::~ResultMerger() = default;

std::string ResultMerger::merge(const std::vector<CodeFragment>& fragments,
                               const std::vector<CompilationResult>& results,
                               const MergeOptions& options) {
    std::string html_content;
    std::vector<std::string> all_styles;
    std::vector<std::string> all_scripts;
    
    // 根据片段类型处理结果
    for (size_t i = 0; i < results.size(); ++i) {
        const auto& result = results[i];
        const auto& fragment = fragments[i];
        
        if (result.success) {
            switch (fragment.type) {
                case CodeFragmentType::CHTL:
                    // CHTL片段的输出是HTML内容
                    html_content += result.output;
                    
                    // 收集全局样式
                    if (!result.global_styles.empty()) {
                        all_styles.push_back(result.global_styles);
                    }
                    break;
                    
                case CodeFragmentType::CHTL_JS:
                    // CHTL JS片段的输出是JavaScript代码
                    if (!result.output.empty()) {
                        all_scripts.push_back(result.output);
                    }
                    break;
                    
                case CodeFragmentType::CSS:
                    // CSS片段直接添加到样式
                    if (!result.output.empty()) {
                        all_styles.push_back(result.output);
                    }
                    break;
                    
                case CodeFragmentType::JAVASCRIPT:
                    // JavaScript片段直接添加到脚本
                    if (!result.output.empty()) {
                        all_scripts.push_back(result.output);
                    }
                    break;
                    
                case CodeFragmentType::HTML_RAW:
                    // 原始HTML直接添加
                    html_content += result.output;
                    break;
            }
        }
    }
    
    // 构建最终HTML
    std::string final_output;
    
    if (hasHTMLStructure(html_content)) {
        // 已有HTML结构，插入样式和脚本
        final_output = html_content;
        
        if (!all_styles.empty()) {
            final_output = insertStyles(final_output, all_styles, options.inline_styles);
        }
        
        if (!all_scripts.empty()) {
            final_output = insertScripts(final_output, all_scripts, options.inline_scripts);
        }
    } else {
        // 构建完整的HTML文档
        final_output = buildHTMLDocument(html_content, all_styles, all_scripts, options);
    }
    
    // 美化输出（如果需要）
    if (options.prettify) {
        final_output = prettifyHTML(final_output);
    }
    
    return final_output;
}

std::string ResultMerger::buildHTMLDocument(const std::string& content,
                                           const std::vector<std::string>& styles,
                                           const std::vector<std::string>& scripts,
                                           const MergeOptions& options) {
    std::stringstream html;
    
    html << "<!DOCTYPE html>\n";
    html << "<html lang=\"zh-CN\">\n";
    html << "<head>\n";
    html << "  <meta charset=\"UTF-8\">\n";
    html << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    html << "  <title>" << options.title << "</title>\n";
    
    // 添加样式
    if (!styles.empty()) {
        if (options.inline_styles) {
            html << "  <style>\n";
            for (const auto& style : styles) {
                html << style << "\n";
            }
            html << "  </style>\n";
        } else {
            // 外部样式表引用（需要额外的文件写入逻辑）
            html << "  <!-- Styles would be linked here -->\n";
        }
    }
    
    html << "</head>\n";
    html << "<body>\n";
    html << content;
    
    // 添加脚本
    if (!scripts.empty()) {
        if (options.inline_scripts) {
            html << "  <script>\n";
            for (const auto& script : scripts) {
                html << script << "\n";
            }
            html << "  </script>\n";
        } else {
            // 外部脚本引用（需要额外的文件写入逻辑）
            html << "  <!-- Scripts would be linked here -->\n";
        }
    }
    
    html << "</body>\n";
    html << "</html>\n";
    
    return html.str();
}

bool ResultMerger::hasHTMLStructure(const std::string& content) {
    // 检查是否包含基本的HTML结构标签
    return content.find("<html") != std::string::npos ||
           content.find("<!DOCTYPE") != std::string::npos;
}

std::string ResultMerger::insertStyles(const std::string& html,
                                      const std::vector<std::string>& styles,
                                      bool inline_styles) {
    std::string result = html;
    
    // 构建样式块
    std::stringstream style_block;
    if (inline_styles) {
        style_block << "<style>\n";
        for (const auto& style : styles) {
            style_block << style << "\n";
        }
        style_block << "</style>\n";
    }
    
    // 查找</head>标签并插入样式
    size_t head_end = result.find("</head>");
    if (head_end != std::string::npos) {
        result.insert(head_end, style_block.str());
    } else {
        // 如果没有</head>，尝试在<body>之前插入
        size_t body_start = result.find("<body");
        if (body_start != std::string::npos) {
            result.insert(body_start, "<head>\n" + style_block.str() + "</head>\n");
        }
    }
    
    return result;
}

std::string ResultMerger::insertScripts(const std::string& html,
                                       const std::vector<std::string>& scripts,
                                       bool inline_scripts) {
    std::string result = html;
    
    // 构建脚本块
    std::stringstream script_block;
    if (inline_scripts) {
        script_block << "<script>\n";
        for (const auto& script : scripts) {
            script_block << script << "\n";
        }
        script_block << "</script>\n";
    }
    
    // 查找</body>标签并插入脚本
    size_t body_end = result.find("</body>");
    if (body_end != std::string::npos) {
        result.insert(body_end, script_block.str());
    } else {
        // 如果没有</body>，在末尾添加
        result += script_block.str();
    }
    
    return result;
}

std::string ResultMerger::prettifyHTML(const std::string& html) {
    // 简单的HTML美化
    std::stringstream input(html);
    std::stringstream output;
    std::string line;
    int indent_level = 0;
    
    // 定义会增加缩进的标签
    std::vector<std::string> indent_tags = {
        "<html", "<head", "<body", "<div", "<section", "<article",
        "<header", "<footer", "<nav", "<ul", "<ol", "<li",
        "<table", "<tr", "<td", "<th", "<form", "<fieldset"
    };
    
    // 定义自闭合标签
    std::vector<std::string> self_closing_tags = {
        "<meta", "<link", "<img", "<br", "<hr", "<input", "<!DOCTYPE"
    };
    
    while (std::getline(input, line)) {
        // 去除前后空白
        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);
        
        if (line.empty()) continue;
        
        // 检查是否是闭合标签
        bool is_closing = (line.substr(0, 2) == "</");
        bool is_self_closing = false;
        
        // 检查是否是自闭合标签
        for (const auto& tag : self_closing_tags) {
            if (line.find(tag) == 0) {
                is_self_closing = true;
                break;
            }
        }
        
        // 调整缩进
        if (is_closing) {
            indent_level = std::max(0, indent_level - 1);
        }
        
        // 添加缩进
        for (int i = 0; i < indent_level * 2; ++i) {
            output << " ";
        }
        output << line << "\n";
        
        // 检查是否需要增加缩进
        if (!is_closing && !is_self_closing) {
            for (const auto& tag : indent_tags) {
                if (line.find(tag) == 0) {
                    indent_level++;
                    break;
                }
            }
        }
    }
    
    return output.str();
}

} // namespace chtl
#include "merger/merger.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <regex>

namespace chtl {

ResultMerger::ResultMerger() : debug_mode_(false) {
    // 设置默认标题
    doc_template_.head.title = "CHTL Generated Page";
}

ResultMerger::~ResultMerger() {
}

std::string ResultMerger::merge(const std::vector<CompileTaskResult>& results,
                               const CompileOptions& options) {
    // 重置状态
    resetState();
    
    // 分类编译结果
    categorizeResults(results);
    
    // 生成HTML文档
    std::string html = generateHTML();
    
    // 根据选项进行优化
    if (options.minify) {
        html = minifyHTML(html);
    }
    
    // 格式化输出
    if (!options.minify) {
        html = formatDocument(html);
    }
    
    // 更新统计信息
    last_stats_.total_fragments = results.size();
    last_stats_.html_size = mergeHTMLContent().size();
    last_stats_.css_size = mergeCSSContent().size();
    last_stats_.js_size = mergeJSContent().size();
    last_stats_.total_size = html.size();
    
    if (debug_mode_) {
        std::cout << "Merge complete: " << last_stats_.total_fragments << " fragments\n";
        std::cout << "Total size: " << last_stats_.total_size << " bytes\n";
    }
    
    return html;
}

void ResultMerger::categorizeResults(const std::vector<CompileTaskResult>& results) {
    for (const auto& result : results) {
        if (!result.result.success) {
            continue; // 跳过失败的编译结果
        }
        
        const std::string& output = result.result.output;
        
        // 根据片段类型分类
        switch (result.type) {
            case CodeFragmentType::CHTL:
                // CHTL编译结果可能包含HTML、CSS和JS
                // 需要进一步解析
                {
                    // 提取<style>标签
                    std::regex style_regex(R"(<style[^>]*>([\s\S]*?)</style>)");
                    std::smatch style_match;
                    std::string temp = output;
                    while (std::regex_search(temp, style_match, style_regex)) {
                        css_fragments_.push_back(style_match[1].str());
                        temp = style_match.suffix();
                    }
                    
                    // 提取<script>标签
                    std::regex script_regex(R"(<script[^>]*>([\s\S]*?)</script>)");
                    std::smatch script_match;
                    temp = output;
                    while (std::regex_search(temp, script_match, script_regex)) {
                        body_scripts_.push_back(script_match[1].str());
                        temp = script_match.suffix();
                    }
                    
                    // 移除style和script标签后的内容作为HTML
                    std::string html_content = output;
                    html_content = std::regex_replace(html_content, style_regex, "");
                    html_content = std::regex_replace(html_content, script_regex, "");
                    if (!html_content.empty()) {
                        html_fragments_.push_back(html_content);
                    }
                }
                break;
                
            case CodeFragmentType::CHTL_JS:
            case CodeFragmentType::JAVASCRIPT:
                js_fragments_.push_back(output);
                break;
                
            case CodeFragmentType::CSS:
                css_fragments_.push_back(output);
                break;
                
            case CodeFragmentType::HTML_TEXT:
                html_fragments_.push_back(output);
                break;
        }
    }
}

std::string ResultMerger::generateHTML() {
    std::stringstream ss;
    
    // DOCTYPE
    ss << doc_template_.doctype << "\n";
    
    // HTML标签
    ss << "<html " << doc_template_.html_attrs << ">\n";
    
    // Head部分
    ss << "<head>\n";
    
    // 字符集
    ss << "  <meta charset=\"" << doc_template_.head.charset << "\">\n";
    
    // 视口
    if (!doc_template_.head.viewport.empty()) {
        ss << "  <meta name=\"viewport\" content=\"" 
           << doc_template_.head.viewport << "\">\n";
    }
    
    // 标题
    if (!doc_template_.head.title.empty()) {
        ss << "  <title>" << doc_template_.head.title << "</title>\n";
    }
    
    // 其他meta标签
    for (const auto& meta : doc_template_.head.meta_tags) {
        ss << "  " << meta << "\n";
    }
    
    // Link标签
    for (const auto& link : doc_template_.head.link_tags) {
        ss << "  " << link << "\n";
    }
    
    // 合并的CSS
    std::string merged_css = mergeCSSContent();
    if (!merged_css.empty()) {
        ss << "  <style>\n" << merged_css << "  </style>\n";
    }
    
    // Head中的脚本
    for (const auto& script : head_scripts_) {
        ss << "  <script>\n" << script << "  </script>\n";
    }
    
    ss << "</head>\n";
    
    // Body部分
    ss << "<body";
    if (!doc_template_.body.attrs.empty()) {
        ss << " " << doc_template_.body.attrs;
    }
    ss << ">\n";
    
    // 合并的HTML内容
    std::string merged_html = mergeHTMLContent();
    if (!merged_html.empty()) {
        ss << merged_html;
    }
    
    // Body末尾的脚本
    std::string merged_js = mergeJSContent();
    if (!merged_js.empty()) {
        ss << "  <script>\n" << merged_js << "  </script>\n";
    }
    
    // 额外的body脚本
    for (const auto& script : body_scripts_) {
        if (!script.empty()) {
            ss << "  <script>\n" << script << "  </script>\n";
        }
    }
    
    ss << "</body>\n";
    ss << "</html>";
    
    return ss.str();
}

std::string ResultMerger::mergeCSSContent() {
    if (css_fragments_.empty()) {
        return "";
    }
    
    std::stringstream ss;
    
    // 添加注释标记
    if (debug_mode_) {
        ss << "    /* CHTL Merged CSS */\n";
    }
    
    for (size_t i = 0; i < css_fragments_.size(); ++i) {
        if (debug_mode_) {
            ss << "    /* Fragment " << i + 1 << " */\n";
        }
        ss << "    " << css_fragments_[i];
        if (css_fragments_[i].back() != '\n') {
            ss << "\n";
        }
    }
    
    return ss.str();
}

std::string ResultMerger::mergeJSContent() {
    if (js_fragments_.empty()) {
        return "";
    }
    
    std::stringstream ss;
    
    // 添加IIFE包装
    ss << "    (function() {\n";
    ss << "      'use strict';\n";
    
    if (debug_mode_) {
        ss << "      /* CHTL Merged JavaScript */\n";
    }
    
    for (size_t i = 0; i < js_fragments_.size(); ++i) {
        if (debug_mode_) {
            ss << "      /* Fragment " << i + 1 << " */\n";
        }
        
        // 缩进每一行
        std::istringstream iss(js_fragments_[i]);
        std::string line;
        while (std::getline(iss, line)) {
            ss << "      " << line << "\n";
        }
    }
    
    ss << "    })();\n";
    
    return ss.str();
}

std::string ResultMerger::mergeHTMLContent() {
    if (html_fragments_.empty()) {
        return "";
    }
    
    std::stringstream ss;
    
    for (const auto& fragment : html_fragments_) {
        ss << fragment;
        if (!fragment.empty() && fragment.back() != '\n') {
            ss << "\n";
        }
    }
    
    return ss.str();
}

std::string ResultMerger::minifyCSS(const std::string& css) {
    // 简单的CSS压缩
    std::string result = css;
    
    // 移除注释
    result = std::regex_replace(result, std::regex(R"(/\*[\s\S]*?\*/)"), "");
    
    // 移除多余的空白
    result = std::regex_replace(result, std::regex(R"(\s+)"), " ");
    result = std::regex_replace(result, std::regex(R"(\s*:\s*)"), ":");
    result = std::regex_replace(result, std::regex(R"(\s*;\s*)"), ";");
    result = std::regex_replace(result, std::regex(R"(\s*\{\s*)"), "{");
    result = std::regex_replace(result, std::regex(R"(\s*\}\s*)"), "}");
    result = std::regex_replace(result, std::regex(R"(\s*,\s*)"), ",");
    
    return result;
}

std::string ResultMerger::minifyJS(const std::string& js) {
    // 简单的JS压缩（实际应使用专业的压缩工具）
    std::string result = js;
    
    // 移除单行注释（保留包含正则表达式的行）
    result = std::regex_replace(result, std::regex(R"(//[^\n]*\n)"), "\n");
    
    // 移除多行注释
    result = std::regex_replace(result, std::regex(R"(/\*[\s\S]*?\*/)"), "");
    
    // 移除多余的空白
    result = std::regex_replace(result, std::regex(R"(\s+)"), " ");
    
    return result;
}

std::string ResultMerger::minifyHTML(const std::string& html) {
    // 简单的HTML压缩
    std::string result = html;
    
    // 移除HTML注释
    result = std::regex_replace(result, std::regex(R"(<!--[\s\S]*?-->)"), "");
    
    // 移除标签间的多余空白
    result = std::regex_replace(result, std::regex(R"(>\s+<)"), "><");
    
    // 移除多余的空白
    result = std::regex_replace(result, std::regex(R"(\s+)"), " ");
    
    return result;
}

std::string ResultMerger::generateSourceMap() {
    // TODO: 实现source map生成
    return "";
}

std::string ResultMerger::formatDocument(const std::string& html) {
    // 简单的格式化，实际可以使用更复杂的格式化逻辑
    return html;
}

void ResultMerger::resetState() {
    html_fragments_.clear();
    css_fragments_.clear();
    js_fragments_.clear();
    head_scripts_.clear();
    body_scripts_.clear();
    source_map_.clear();
    last_stats_ = MergeStats();
}

} // namespace chtl
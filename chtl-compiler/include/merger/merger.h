#ifndef CHTL_MERGER_H
#define CHTL_MERGER_H

#include "common/types.h"
#include "dispatcher/dispatcher.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace chtl {

// HTML文档结构
struct HTMLDocument {
    std::string doctype = "<!DOCTYPE html>";
    std::string html_attrs = "lang=\"en\"";
    
    // Head部分
    struct Head {
        std::string title;
        std::string charset = "UTF-8";
        std::string viewport = "width=device-width, initial-scale=1.0";
        std::vector<std::string> meta_tags;
        std::vector<std::string> link_tags;
        std::vector<std::string> style_blocks;
        std::vector<std::string> script_blocks;  // head中的脚本
    } head;
    
    // Body部分
    struct Body {
        std::string attrs;
        std::string content;
        std::vector<std::string> script_blocks;  // body末尾的脚本
    } body;
};

class ResultMerger {
public:
    ResultMerger();
    ~ResultMerger();
    
    // 合并编译结果
    std::string merge(const std::vector<CompileTaskResult>& results,
                     const CompileOptions& options);
    
    // 设置HTML模板选项
    void setHTMLTitle(const std::string& title) { doc_template_.head.title = title; }
    void addMetaTag(const std::string& meta) { doc_template_.head.meta_tags.push_back(meta); }
    void addLinkTag(const std::string& link) { doc_template_.head.link_tags.push_back(link); }
    
    // 设置调试模式
    void setDebugMode(bool debug) { debug_mode_ = debug; }
    
    // 获取合并统计
    struct MergeStats {
        size_t total_fragments;
        size_t html_size;
        size_t css_size;
        size_t js_size;
        size_t total_size;
    };
    
    MergeStats getLastStats() const { return last_stats_; }

private:
    // 分类和整理编译结果
    void categorizeResults(const std::vector<CompileTaskResult>& results);
    
    // 生成HTML文档
    std::string generateHTML();
    
    // 合并相同类型的内容
    std::string mergeCSSContent();
    std::string mergeJSContent();
    std::string mergeHTMLContent();
    
    // 优化选项
    std::string minifyCSS(const std::string& css);
    std::string minifyJS(const std::string& js);
    std::string minifyHTML(const std::string& html);
    
    // 生成source map
    std::string generateSourceMap();
    
    // 工具方法
    std::string formatDocument(const std::string& html);
    void resetState();
    
private:
    bool debug_mode_;
    HTMLDocument doc_template_;
    
    // 分类的编译结果
    std::vector<std::string> html_fragments_;
    std::vector<std::string> css_fragments_;
    std::vector<std::string> js_fragments_;
    std::vector<std::string> head_scripts_;
    std::vector<std::string> body_scripts_;
    
    // 源文件映射（用于source map）
    std::unordered_map<std::string, SourceLocation> source_map_;
    
    // 统计信息
    MergeStats last_stats_;
};

} // namespace chtl

#endif // CHTL_MERGER_H
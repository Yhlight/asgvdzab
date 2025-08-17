#pragma once

#include "../common/Types.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

/**
 * 编译结果合并器
 * 负责将各个编译器的编译结果合并成最终的HTML输出
 * 处理HTML结构、CSS样式、JavaScript脚本的整合
 */
class ResultMerger {
private:
    Configuration config;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    bool debug_mode;
    
    // 合并结果存储
    std::string merged_html;
    std::string merged_css;
    std::string merged_js;
    
    // HTML模板
    std::string html_template;
    
    // 内部合并方法
    std::string mergeHTMLContent(const std::vector<CompileResultPtr>& chtl_results);
    std::string mergeCSSContent(const std::vector<CompileResultPtr>& css_results);
    std::string mergeJSContent(const std::vector<CompileResultPtr>& js_results);
    
    // HTML结构处理
    std::string buildHTMLStructure(const std::string& body_content);
    std::string injectCSS(const std::string& html, const std::string& css);
    std::string injectJS(const std::string& html, const std::string& js);
    
    // 代码优化
    std::string optimizeHTML(const std::string& html) const;
    std::string formatHTML(const std::string& html) const;
    
    // 错误处理
    void addError(const std::string& message);
    void addWarning(const std::string& message);
    void collectErrors(const std::vector<CompileResultPtr>& results);
    
public:
    ResultMerger();
    explicit ResultMerger(const Configuration& cfg);
    
    // 设置配置
    void setConfiguration(const Configuration& cfg);
    
    // 设置HTML模板
    void setHTMLTemplate(const std::string& template_html);
    
    // 主要合并方法
    CompileResultPtr mergeResults(const std::vector<CompileResultPtr>& results);
    
    // 分类合并方法
    CompileResultPtr mergeByType(
        const std::vector<CompileResultPtr>& chtl_results,
        const std::vector<CompileResultPtr>& chtl_js_results,
        const std::vector<CompileResultPtr>& css_results,
        const std::vector<CompileResultPtr>& js_results
    );
    
    // 获取合并结果
    std::string getMergedHTML() const { return merged_html; }
    std::string getMergedCSS() const { return merged_css; }
    std::string getMergedJS() const { return merged_js; }
    
    // HTML生成选项
    void setMinifyOutput(bool minify);
    void setFormatOutput(bool format);
    void setIncludeComments(bool include);
    
    // 错误和警告
    std::vector<std::string> getErrors() const { return errors; }
    std::vector<std::string> getWarnings() const { return warnings; }
    void clearErrors() { errors.clear(); }
    void clearWarnings() { warnings.clear(); }
    
    // 重置合并器状态
    void reset();
    
    // 调试方法
    void setDebugMode(bool enabled);
    void dumpMergeResults() const;
};

} // namespace CHTL
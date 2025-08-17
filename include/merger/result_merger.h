#pragma once

#include "common/compiler_interface.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace chtl {

/**
 * HTML文档结构
 */
struct HTMLDocument {
    std::string doctype = "<!DOCTYPE html>";
    std::string html_attributes = "lang=\"zh-CN\"";
    
    // Head部分
    std::string head_meta;
    std::string head_title;
    std::string head_styles;
    std::string head_scripts;
    std::string head_links;
    std::string head_custom;
    
    // Body部分
    std::string body_attributes;
    std::string body_content;
    std::string body_scripts;
    
    // 生成完整HTML
    std::string generateHTML() const;
};

/**
 * 合并配置
 */
struct MergeConfig {
    // HTML配置
    bool minify_html = false;
    bool pretty_print = true;
    std::string indent = "  ";
    
    // CSS配置
    bool inline_css = false;
    bool minify_css = false;
    std::string css_media = "all";
    
    // JavaScript配置
    bool inline_js = false;
    bool minify_js = false;
    bool defer_js = false;
    bool async_js = false;
    
    // 输出配置
    std::string charset = "UTF-8";
    std::string viewport = "width=device-width, initial-scale=1.0";
    bool include_generator = true;
    
    // 模板配置
    std::string template_path;
    std::unordered_map<std::string, std::string> template_variables;
};

/**
 * 结果合并器接口
 */
class IResultMerger {
public:
    virtual ~IResultMerger() = default;
    
    /**
     * 合并编译结果
     */
    virtual std::unique_ptr<CompileResult> merge(
        const std::vector<std::unique_ptr<CompileResult>>& results) = 0;
    
    /**
     * 设置合并配置
     */
    virtual void setConfig(const MergeConfig& config) = 0;
    virtual const MergeConfig& getConfig() const = 0;
    
    /**
     * 获取合并器名称
     */
    virtual std::string getName() const = 0;
    virtual std::string getVersion() const = 0;
};

/**
 * HTML结果合并器
 */
class HTMLResultMerger : public IResultMerger {
public:
    HTMLResultMerger();
    explicit HTMLResultMerger(const MergeConfig& config);
    
    std::unique_ptr<CompileResult> merge(
        const std::vector<std::unique_ptr<CompileResult>>& results) override;
    
    void setConfig(const MergeConfig& config) override { config_ = config; }
    const MergeConfig& getConfig() const override { return config_; }
    
    std::string getName() const override { return "HTML Result Merger"; }
    std::string getVersion() const override { return "1.0.0"; }
    
    // HTML特有功能
    void setTemplate(const std::string& template_content);
    void addTemplateVariable(const std::string& key, const std::string& value);
    
    // 文档结构访问
    HTMLDocument& getDocument() { return document_; }
    const HTMLDocument& getDocument() const { return document_; }

private:
    MergeConfig config_;
    HTMLDocument document_;
    std::string template_content_;
    
    /**
     * 分类处理不同类型的结果
     */
    void processHTMLResults(const std::vector<std::unique_ptr<CompileResult>>& results);
    void processCSSResults(const std::vector<std::unique_ptr<CompileResult>>& results);
    void processJSResults(const std::vector<std::unique_ptr<CompileResult>>& results);
    
    /**
     * 内容处理
     */
    std::string processCSS(const std::string& css_content);
    std::string processJS(const std::string& js_content);
    std::string processHTML(const std::string& html_content);
    
    /**
     * 模板处理
     */
    std::string applyTemplate(const std::string& content);
    std::string replaceTemplateVariables(const std::string& content);
    
    /**
     * 优化处理
     */
    std::string minifyHTML(const std::string& html);
    std::string minifyCSS(const std::string& css);
    std::string minifyJS(const std::string& js);
    std::string prettyPrintHTML(const std::string& html);
    
    /**
     * 工具函数
     */
    std::string escapeHTML(const std::string& text);
    std::string generateMetaTags();
    std::string generateStyleLink(const std::string& href);
    std::string generateScriptTag(const std::string& src, bool defer = false, bool async = false);
    
    /**
     * 内容验证
     */
    bool validateHTML(const std::string& html);
    bool validateCSS(const std::string& css);
    bool validateJS(const std::string& js);
};

/**
 * 流式合并器 - 用于大文件处理
 */
class StreamingResultMerger : public IResultMerger {
public:
    StreamingResultMerger();
    explicit StreamingResultMerger(const MergeConfig& config);
    
    std::unique_ptr<CompileResult> merge(
        const std::vector<std::unique_ptr<CompileResult>>& results) override;
    
    void setConfig(const MergeConfig& config) override { config_ = config; }
    const MergeConfig& getConfig() const override { return config_; }
    
    std::string getName() const override { return "Streaming Result Merger"; }
    std::string getVersion() const override { return "1.0.0"; }
    
    // 流式处理配置
    void setBufferSize(size_t size) { buffer_size_ = size; }
    size_t getBufferSize() const { return buffer_size_; }

private:
    MergeConfig config_;
    size_t buffer_size_;
    
    /**
     * 流式处理方法
     */
    void streamProcess(const std::vector<std::unique_ptr<CompileResult>>& results,
                      std::ostream& output);
    void streamProcessChunk(const std::string& chunk, std::ostream& output);
};

/**
 * 自定义合并器基类
 */
class CustomResultMerger : public IResultMerger {
public:
    CustomResultMerger(const std::string& name, const std::string& version)
        : name_(name), version_(version) {}
    
    std::string getName() const override { return name_; }
    std::string getVersion() const override { return version_; }
    
    void setConfig(const MergeConfig& config) override { config_ = config; }
    const MergeConfig& getConfig() const override { return config_; }

protected:
    std::string name_;
    std::string version_;
    MergeConfig config_;
    
    /**
     * 子类需要实现的方法
     */
    virtual std::string customMerge(
        const std::vector<std::unique_ptr<CompileResult>>& results) = 0;
};

/**
 * 合并器工厂
 */
class MergerFactory {
public:
    static std::unique_ptr<IResultMerger> createHTMLMerger();
    static std::unique_ptr<IResultMerger> createHTMLMerger(const MergeConfig& config);
    static std::unique_ptr<IResultMerger> createStreamingMerger();
    static std::unique_ptr<IResultMerger> createStreamingMerger(const MergeConfig& config);
    
    // 预配置合并器
    static std::unique_ptr<IResultMerger> createProductionMerger();
    static std::unique_ptr<IResultMerger> createDevelopmentMerger();
    static std::unique_ptr<IResultMerger> createDebugMerger();
};

/**
 * 合并器配置构建器
 */
class MergeConfigBuilder {
public:
    MergeConfigBuilder& withMinifyHTML(bool minify);
    MergeConfigBuilder& withPrettyPrint(bool pretty);
    MergeConfigBuilder& withIndent(const std::string& indent);
    
    MergeConfigBuilder& withInlineCSS(bool inline_css);
    MergeConfigBuilder& withMinifyCSS(bool minify);
    MergeConfigBuilder& withCSSMedia(const std::string& media);
    
    MergeConfigBuilder& withInlineJS(bool inline_js);
    MergeConfigBuilder& withMinifyJS(bool minify);
    MergeConfigBuilder& withDeferJS(bool defer);
    MergeConfigBuilder& withAsyncJS(bool async);
    
    MergeConfigBuilder& withCharset(const std::string& charset);
    MergeConfigBuilder& withViewport(const std::string& viewport);
    MergeConfigBuilder& withIncludeGenerator(bool include);
    
    MergeConfigBuilder& withTemplate(const std::string& template_path);
    MergeConfigBuilder& withTemplateVariable(const std::string& key, const std::string& value);
    
    MergeConfig build();

private:
    MergeConfig config_;
};

} // namespace chtl
#pragma once

#include "Common/CompilerInterface.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace Chtl {

/**
 * HTML文档结构
 */
struct HtmlDocument {
    std::string Doctype = "<!DOCTYPE html>";
    std::string HtmlAttributes = "lang=\"zh-CN\"";
    
    // Head部分
    std::string HeadMeta;
    std::string HeadTitle;
    std::string HeadStyles;
    std::string HeadScripts;
    std::string HeadLinks;
    std::string HeadCustom;
    
    // Body部分
    std::string BodyAttributes;
    std::string BodyContent;
    std::string BodyScripts;
    
    // 生成完整HTML
    std::string GenerateHtml() const;
};

/**
 * 合并配置
 */
struct MergeConfig {
    // HTML配置
    bool MinifyHtml = false;
    bool PrettyPrint = true;
    std::string Indent = "  ";
    
    // CSS配置
    bool InlineCss = false;
    bool MinifyCss = false;
    std::string CssMedia = "all";
    
    // JavaScript配置
    bool InlineJs = false;
    bool MinifyJs = false;
    bool DeferJs = false;
    bool AsyncJs = false;
    
    // 输出配置
    std::string Charset = "UTF-8";
    std::string Viewport = "width=device-width, initial-scale=1.0";
    bool IncludeGenerator = true;
    
    // 模板配置
    std::string TemplatePath;
    std::unordered_map<std::string, std::string> TemplateVariables;
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
    virtual std::unique_ptr<CompileResult> Merge(
        const std::vector<std::unique_ptr<CompileResult>>& results) = 0;
    
    /**
     * 设置合并配置
     */
    virtual void SetConfig(const MergeConfig& config) = 0;
    virtual const MergeConfig& GetConfig() const = 0;
    
    /**
     * 获取合并器名称
     */
    virtual std::string GetName() const = 0;
    virtual std::string GetVersion() const = 0;
};

/**
 * HTML结果合并器
 */
class HtmlResultMerger : public IResultMerger {
public:
    HtmlResultMerger();
    explicit HtmlResultMerger(const MergeConfig& config);
    
    std::unique_ptr<CompileResult> Merge(
        const std::vector<std::unique_ptr<CompileResult>>& results) override;
    
    void SetConfig(const MergeConfig& config) override { Config_ = config; }
    const MergeConfig& GetConfig() const override { return Config_; }
    
    std::string GetName() const override { return "HTML Result Merger"; }
    std::string GetVersion() const override { return "1.0.0"; }
    
    // HTML特有功能
    void SetTemplate(const std::string& templateContent);
    void AddTemplateVariable(const std::string& key, const std::string& value);
    
    // 文档结构访问
    HtmlDocument& GetDocument() { return Document_; }
    const HtmlDocument& GetDocument() const { return Document_; }

private:
    MergeConfig Config_;
    HtmlDocument Document_;
    std::string TemplateContent_;
    
    /**
     * 分类处理不同类型的结果
     */
    void ProcessHtmlResults(const std::vector<std::unique_ptr<CompileResult>>& results);
    void ProcessCssResults(const std::vector<std::unique_ptr<CompileResult>>& results);
    void ProcessJsResults(const std::vector<std::unique_ptr<CompileResult>>& results);
    
    /**
     * 内容处理
     */
    std::string ProcessCss(const std::string& cssContent);
    std::string ProcessJs(const std::string& jsContent);
    std::string ProcessHtml(const std::string& htmlContent);
    
    /**
     * 模板处理
     */
    std::string ApplyTemplate(const std::string& content);
    std::string ReplaceTemplateVariables(const std::string& content);
    
    /**
     * 优化处理
     */
    std::string MinifyHtml(const std::string& html);
    std::string MinifyCss(const std::string& css);
    std::string MinifyJs(const std::string& js);
    std::string PrettyPrintHtml(const std::string& html);
    
    /**
     * 工具函数
     */
    std::string EscapeHtml(const std::string& text);
    std::string GenerateMetaTags();
    std::string GenerateStyleLink(const std::string& href);
    std::string GenerateScriptTag(const std::string& src, bool defer = false, bool async = false);
    
    /**
     * 内容验证
     */
    bool ValidateHtml(const std::string& html);
    bool ValidateCss(const std::string& css);
    bool ValidateJs(const std::string& js);
};

/**
 * 流式合并器 - 用于大文件处理
 */
class StreamingResultMerger : public IResultMerger {
public:
    StreamingResultMerger();
    explicit StreamingResultMerger(const MergeConfig& config);
    
    std::unique_ptr<CompileResult> Merge(
        const std::vector<std::unique_ptr<CompileResult>>& results) override;
    
    void SetConfig(const MergeConfig& config) override { Config_ = config; }
    const MergeConfig& GetConfig() const override { return Config_; }
    
    std::string GetName() const override { return "Streaming Result Merger"; }
    std::string GetVersion() const override { return "1.0.0"; }
    
    // 流式处理配置
    void SetBufferSize(size_t size) { BufferSize_ = size; }
    size_t GetBufferSize() const { return BufferSize_; }

private:
    MergeConfig Config_;
    size_t BufferSize_;
    
    /**
     * 流式处理方法
     */
    void StreamProcess(const std::vector<std::unique_ptr<CompileResult>>& results,
                      std::ostream& output);
    void StreamProcessChunk(const std::string& chunk, std::ostream& output);
};

/**
 * 自定义合并器基类
 */
class CustomResultMerger : public IResultMerger {
public:
    CustomResultMerger(const std::string& name, const std::string& version)
        : Name_(name), Version_(version) {}
    
    std::string GetName() const override { return Name_; }
    std::string GetVersion() const override { return Version_; }
    
    void SetConfig(const MergeConfig& config) override { Config_ = config; }
    const MergeConfig& GetConfig() const override { return Config_; }

protected:
    std::string Name_;
    std::string Version_;
    MergeConfig Config_;
    
    /**
     * 子类需要实现的方法
     */
    virtual std::string CustomMerge(
        const std::vector<std::unique_ptr<CompileResult>>& results) = 0;
};

/**
 * 合并器工厂
 */
class MergerFactory {
public:
    static std::unique_ptr<IResultMerger> CreateHtmlMerger();
    static std::unique_ptr<IResultMerger> CreateHtmlMerger(const MergeConfig& config);
    static std::unique_ptr<IResultMerger> CreateStreamingMerger();
    static std::unique_ptr<IResultMerger> CreateStreamingMerger(const MergeConfig& config);
    
    // 预配置合并器
    static std::unique_ptr<IResultMerger> CreateProductionMerger();
    static std::unique_ptr<IResultMerger> CreateDevelopmentMerger();
    static std::unique_ptr<IResultMerger> CreateDebugMerger();
};

/**
 * 合并器配置构建器
 */
class MergeConfigBuilder {
public:
    MergeConfigBuilder& WithMinifyHtml(bool minify);
    MergeConfigBuilder& WithPrettyPrint(bool pretty);
    MergeConfigBuilder& WithIndent(const std::string& indent);
    
    MergeConfigBuilder& WithInlineCss(bool inlineCss);
    MergeConfigBuilder& WithMinifyCss(bool minify);
    MergeConfigBuilder& WithCssMedia(const std::string& media);
    
    MergeConfigBuilder& WithInlineJs(bool inlineJs);
    MergeConfigBuilder& WithMinifyJs(bool minify);
    MergeConfigBuilder& WithDeferJs(bool defer);
    MergeConfigBuilder& WithAsyncJs(bool async);
    
    MergeConfigBuilder& WithCharset(const std::string& charset);
    MergeConfigBuilder& WithViewport(const std::string& viewport);
    MergeConfigBuilder& WithIncludeGenerator(bool include);
    
    MergeConfigBuilder& WithTemplate(const std::string& templatePath);
    MergeConfigBuilder& WithTemplateVariable(const std::string& key, const std::string& value);
    
    MergeConfig Build();

private:
    MergeConfig Config_;
};

} // namespace Chtl
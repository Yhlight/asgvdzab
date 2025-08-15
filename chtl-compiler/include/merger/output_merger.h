#ifndef OUTPUT_MERGER_H
#define OUTPUT_MERGER_H

#include <string>
#include <vector>
#include <memory>
#include "dispatcher/compiler_dispatcher.h"

namespace chtl {

// HTML文档结构
struct HTMLDocument {
    struct Head {
        std::string title;
        std::vector<std::string> metaTags;
        std::vector<std::string> linkTags;
        std::vector<std::string> styleTags;    // 内联样式
        std::vector<std::string> scriptTags;   // 头部脚本
    } head;
    
    struct Body {
        std::string content;                    // 主体HTML内容
        std::vector<std::string> scriptTags;    // 底部脚本
    } body;
    
    std::string doctype = "<!DOCTYPE html>";
    std::string language = "zh-CN";
    std::string charset = "UTF-8";
};

// 合并配置
struct MergerConfig {
    // HTML格式化选项
    bool formatOutput = true;           // 是否格式化输出
    bool minifyHTML = false;           // 是否压缩HTML
    size_t indentSize = 2;             // 缩进大小
    bool useSpaces = true;             // 使用空格（false为tab）
    
    // 资源管理选项
    bool inlineStyles = true;          // 内联样式
    bool inlineScripts = true;         // 内联脚本
    bool combineStyles = true;         // 合并样式
    bool combineScripts = true;        // 合并脚本
    
    // 优化选项
    bool removeComments = false;       // 移除注释
    bool removeEmptyLines = true;      // 移除空行
    bool optimizeWhitespace = false;   // 优化空白字符
};

// OutputMerger - 编译结果合并器
class OutputMerger {
public:
    explicit OutputMerger(const MergerConfig& config = MergerConfig());
    ~OutputMerger();
    
    // 合并编译结果生成HTML
    std::string merge(const std::vector<CompileResult>& results);
    
    // 设置HTML文档信息
    void setDocumentInfo(const HTMLDocument& doc);
    
    // 添加额外的资源
    void addStylesheet(const std::string& href);
    void addScript(const std::string& src, bool defer = false, bool async = false);
    void addMetaTag(const std::string& name, const std::string& content);
    
    // 获取合并统计信息
    struct MergeStats {
        size_t totalFragments = 0;
        size_t mergedStyles = 0;
        size_t mergedScripts = 0;
        size_t htmlFragments = 0;
        size_t outputSize = 0;
    };
    MergeStats getStats() const;
    
    // 验证输出的HTML
    bool validateOutput(const std::string& html, std::vector<std::string>& errors) const;
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace chtl

#endif // OUTPUT_MERGER_H
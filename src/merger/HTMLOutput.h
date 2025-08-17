#pragma once

#include <string>
#include <vector>
#include <sstream>

namespace CHTL {

/**
 * HTML输出构建器
 * 负责构建最终的HTML文档结构
 */
class HTMLOutput {
public:
    HTMLOutput();
    ~HTMLOutput() = default;

    /**
     * 设置HTML文档的基本结构
     * @param title 文档标题
     * @param lang 文档语言
     */
    void setDocumentStructure(const std::string& title = "CHTL Generated Page", 
                             const std::string& lang = "zh-CN");

    /**
     * 添加CSS内容到head部分
     * @param css CSS内容
     * @param isInline 是否为内联样式
     */
    void addCSS(const std::string& css, bool isInline = true);

    /**
     * 添加JavaScript内容
     * @param js JavaScript内容
     * @param isInline 是否为内联脚本
     * @param location 脚本位置 ("head" 或 "body")
     */
    void addJavaScript(const std::string& js, bool isInline = true, 
                      const std::string& location = "body");

    /**
     * 添加HTML内容到body部分
     * @param html HTML内容
     */
    void addHTMLContent(const std::string& html);

    /**
     * 添加meta标签
     * @param name meta名称
     * @param content meta内容
     */
    void addMeta(const std::string& name, const std::string& content);

    /**
     * 生成最终的HTML文档
     * @return 完整的HTML文档字符串
     */
    std::string generateHTML();

    /**
     * 清空所有内容
     */
    void clear();

private:
    std::string documentTitle_;
    std::string documentLang_;
    std::vector<std::string> metaTags_;
    std::vector<std::string> headCSS_;
    std::vector<std::string> headJS_;
    std::vector<std::string> bodyContent_;
    std::vector<std::string> bodyJS_;

    // 辅助方法
    std::string escapeHTML(const std::string& text);
    std::string indentText(const std::string& text, int indentLevel);
};

} // namespace CHTL
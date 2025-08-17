#ifndef CHTL_RESULT_MERGER_H
#define CHTL_RESULT_MERGER_H

#include <string>
#include <vector>
#include "utils/common.h"

namespace chtl {

// 合并选项
struct MergeOptions {
    bool prettify = false;           // 美化输出
    bool inline_styles = false;      // 内联样式
    bool inline_scripts = false;     // 内联脚本
    std::string title = "CHTL Output"; // HTML标题
};

// 结果合并器 - 负责将各个编译器的输出合并成最终的HTML
class ResultMerger {
public:
    ResultMerger();
    ~ResultMerger();
    
    // 合并编译结果
    std::string merge(const std::vector<CodeFragment>& fragments,
                     const std::vector<CompilationResult>& results,
                     const MergeOptions& options = {});
    
private:
    // 构建HTML文档
    std::string buildHTMLDocument(const std::string& content,
                                  const std::vector<std::string>& styles,
                                  const std::vector<std::string>& scripts,
                                  const MergeOptions& options);
    
    // 检查内容是否已包含HTML结构
    bool hasHTMLStructure(const std::string& content);
    
    // 插入样式到HTML头部
    std::string insertStyles(const std::string& html, 
                            const std::vector<std::string>& styles,
                            bool inline_styles);
    
    // 插入脚本到HTML
    std::string insertScripts(const std::string& html,
                             const std::vector<std::string>& scripts,
                             bool inline_scripts);
    
    // 美化HTML输出
    std::string prettifyHTML(const std::string& html);
};

} // namespace chtl

#endif // CHTL_RESULT_MERGER_H
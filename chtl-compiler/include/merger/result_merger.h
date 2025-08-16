#ifndef CHTL_MERGER_RESULT_MERGER_H
#define CHTL_MERGER_RESULT_MERGER_H

#include <string>
#include <vector>
#include "dispatcher/compiler_dispatcher.h"

namespace chtl {
namespace merger {

// HTML输出选项
struct HTMLOutputOptions {
    bool prettyPrint = true;
    bool includeComments = false;
    std::string doctype = "<!DOCTYPE html>";
    std::string charset = "UTF-8";
    std::string title = "CHTL Output";
    std::vector<std::string> additionalHeadElements;
};

class ResultMerger {
public:
    ResultMerger();
    ~ResultMerger();
    
    // 合并编译结果生成最终HTML
    std::string merge(const dispatcher::DispatchResult& dispatchResult,
                     const HTMLOutputOptions& options = HTMLOutputOptions());
    
    // 获取合并过程中的错误
    const std::vector<std::string>& getErrors() const { return errors_; }
    
    // 获取合并过程中的警告
    const std::vector<std::string>& getWarnings() const { return warnings_; }

private:
    std::vector<std::string> errors_;
    std::vector<std::string> warnings_;
    
    // 内部方法
    std::string generateHTMLStructure(const std::vector<CompilationResult>& results,
                                    const HTMLOutputOptions& options);
    
    std::string collectHTMLBody(const std::vector<CompilationResult>& results);
    std::string collectGlobalStyles(const std::vector<CompilationResult>& results);
    std::string collectGlobalScripts(const std::vector<CompilationResult>& results);
    
    std::string formatHTML(const std::string& html, bool prettyPrint);
    std::string indent(const std::string& content, int level);
};

} // namespace merger
} // namespace chtl

#endif // CHTL_MERGER_RESULT_MERGER_H
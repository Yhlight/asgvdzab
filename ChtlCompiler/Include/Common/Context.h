#ifndef CHTL_COMMON_CONTEXT_H
#define CHTL_COMMON_CONTEXT_H

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "Common/GlobalMap.h"
#include "Common/State.h"

namespace Chtl {

// 前向声明
class ImportResolver;

// 源文件信息
struct SourceInfo {
    std::string fileName;
    std::string content;
    std::string directory;     // 文件所在目录
    size_t size;
};

// 编译选项
struct CompileOptions {
    bool debugMode = false;
    bool minify = false;
    bool generateSourceMap = false;
    bool parallelCompile = true;
    std::string outputDir = "./";
    std::unordered_map<std::string, std::string> customOptions;
};

// 编译结果
struct CompileResult {
    bool success = true;
    std::string output;
    std::string errorMsg;
    std::vector<std::string> warnings;
    std::string sourceMap;     // source map（如果生成）
};

// 诊断信息
struct Diagnostic {
    enum Level { INFO, WARNING, ERROR };
    
    Level level;
    std::string message;
    std::string file;
    size_t line;
    size_t column;
    std::string code;          // 相关代码片段
};

// 编译上下文
class Context {
public:
    Context();
    ~Context();
    
    // 源文件管理
    void setSourceInfo(const SourceInfo& info) { sourceInfo_ = info; }
    const SourceInfo& getSourceInfo() const { return sourceInfo_; }
    
    // 编译选项
    void setOptions(const CompileOptions& options) { options_ = options; }
    const CompileOptions& getOptions() const { return options_; }
    CompileOptions& getOptions() { return options_; }
    
    // 全局符号表
    GlobalMap& getGlobalMap() { return *globalMap_; }
    const GlobalMap& getGlobalMap() const { return *globalMap_; }
    
    // 状态管理
    State& getState() { return *state_; }
    const State& getState() const { return *state_; }
    
    // 输出管理
    void appendOutput(const std::string& text) { output_.append(text); }
    void appendHtml(const std::string& html) { htmlOutput_.append(html); }
    void appendCss(const std::string& css) { cssOutput_.append(css); }
    void appendJs(const std::string& js) { jsOutput_.append(js); }
    
    std::string getOutput() const { return output_; }
    std::string getHtmlOutput() const { return htmlOutput_; }
    std::string getCssOutput() const { return cssOutput_; }
    std::string getJsOutput() const { return jsOutput_; }
    
    void clearOutput() {
        output_.clear();
        htmlOutput_.clear();
        cssOutput_.clear();
        jsOutput_.clear();
    }
    
    // 诊断信息
    void addDiagnostic(const Diagnostic& diag);
    void addInfo(const std::string& msg, size_t line = 0, size_t col = 0);
    void addWarning(const std::string& msg, size_t line = 0, size_t col = 0);
    void addError(const std::string& msg, size_t line = 0, size_t col = 0);
    
    const std::vector<Diagnostic>& getDiagnostics() const { return diagnostics_; }
    bool hasErrors() const;
    bool hasWarnings() const;
    
    // 当前位置信息
    void setCurrentPosition(size_t line, size_t column) {
        currentLine_ = line;
        currentColumn_ = column;
    }
    size_t getCurrentLine() const { return currentLine_; }
    size_t getCurrentColumn() const { return currentColumn_; }
    
    // 作用域管理（用于局部变量等）
    void enterScope(const std::string& name = "");
    void exitScope();
    std::string getCurrentScope() const;
    
    // 临时变量管理（编译过程中的临时数据）
    void setTempData(const std::string& key, const std::string& value);
    std::string getTempData(const std::string& key) const;
    bool hasTempData(const std::string& key) const;
    void clearTempData() { tempData_.clear(); }
    
    // 包含路径管理（用于[Import]）
    void addIncludePath(const std::string& path);
    std::vector<std::string> getIncludePaths() const { return includePaths_; }
    std::string resolveFilePath(const std::string& path) const;
    
    // 重置上下文
    void reset();
    
    // 生成编译结果
    CompileResult generateResult();
    
    // 导入解析器
    ImportResolver* getImportResolver() { return importResolver_.get(); }
    const ImportResolver* getImportResolver() const { return importResolver_.get(); }
    
    // 调试输出
    void dump() const;

private:
    // 源文件信息
    SourceInfo sourceInfo_;
    
    // 编译选项
    CompileOptions options_;
    
    // 全局符号表
    std::unique_ptr<GlobalMap> globalMap_;
    
    // 状态管理
    std::unique_ptr<State> state_;
    
    // 导入解析器
    std::unique_ptr<ImportResolver> importResolver_;
    
    // 输出缓冲
    std::string output_;         // 通用输出
    std::string htmlOutput_;     // HTML部分
    std::string cssOutput_;      // CSS部分
    std::string jsOutput_;       // JavaScript部分
    
    // 诊断信息
    std::vector<Diagnostic> diagnostics_;
    
    // 当前位置
    size_t currentLine_;
    size_t currentColumn_;
    
    // 作用域栈
    std::vector<std::string> scopeStack_;
    
    // 临时数据存储
    std::unordered_map<std::string, std::string> tempData_;
    
    // 包含路径
    std::vector<std::string> includePaths_;
};

} // namespace Chtl

#endif // CHTL_COMMON_CONTEXT_H
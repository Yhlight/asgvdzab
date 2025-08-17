#pragma once

#include "../common/Types.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

/**
 * JavaScript编译器 - 基于ANTLR实现
 * 负责编译纯JavaScript代码片段
 * 这是一个接口类，实际的ANTLR实现将在后续集成
 */
class JavaScriptCompiler {
private:
    Configuration config;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    bool debug_mode;
    
    // JS解析相关
    std::string compiled_js;
    std::vector<std::string> js_statements;
    
    // 内部方法
    void addError(const std::string& message);
    void addWarning(const std::string& message);
    
    // JS验证
    bool validateJS(const std::string& js_code) const;
    std::string normalizeJSCode(const std::string& js_code) const;
    
    // JS优化
    std::string optimizeJS(const std::string& js_code) const;
    std::string minifyJS(const std::string& js_code) const;
    
    // JS语法检查
    bool checkSyntax(const std::string& js_code) const;
    std::vector<std::string> findSyntaxErrors(const std::string& js_code) const;
    
public:
    JavaScriptCompiler();
    explicit JavaScriptCompiler(const Configuration& cfg);
    
    // 设置配置
    void setConfiguration(const Configuration& cfg);
    
    // 主要编译方法
    CompileResultPtr compile(const std::string& js_code);
    
    // JS代码管理
    void addJSStatement(const std::string& statement);
    void addGlobalJS(const std::string& js);
    std::string getCompiledJS() const;
    
    // JS合并和优化
    std::string mergeJS(const std::vector<std::string>& js_parts) const;
    std::string optimizeAndMinify(const std::string& js) const;
    
    // 语法验证
    bool validateSyntax(const std::string& js_code) const;
    std::vector<std::string> getSyntaxErrors(const std::string& js_code) const;
    
    // 错误和警告
    std::vector<std::string> getErrors() const { return errors; }
    std::vector<std::string> getWarnings() const { return warnings; }
    void clearErrors() { errors.clear(); }
    void clearWarnings() { warnings.clear(); }
    
    // 重置编译器状态
    void reset();
    
    // 调试方法
    void setDebugMode(bool enabled) { debug_mode = enabled; }
    void dumpJS() const;
};

} // namespace CHTL
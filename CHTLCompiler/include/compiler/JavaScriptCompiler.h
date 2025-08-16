#pragma once

#include "dispatcher/CompilerDispatcher.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace chtl {

// JavaScript编译器类（简化实现，未来可集成ANTLR）
class JavaScriptCompiler : public ICompiler {
public:
    JavaScriptCompiler();
    ~JavaScriptCompiler() override;
    
    // 实现ICompiler接口
    CompilationResult compile(const CodeFragment& fragment) override;
    std::string getName() const override { return "JavaScript编译器"; }
    
    // 设置编译选项
    void setOptions(const std::unordered_map<std::string, std::string>& options);
    
    // 获取编译统计信息
    struct CompileStats {
        size_t totalStatements;
        size_t functions;
        size_t variables;
        size_t classes;
        size_t imports;
        size_t exports;
        size_t expressions;
    };
    
    CompileStats getCompileStats() const;
    
    // 获取错误信息
    std::vector<std::string> getErrors() const;
    
    // 清除错误信息
    void clearErrors();
    
    // 验证JavaScript语法
    bool validateJavaScript(const std::string& js);
    
    // 压缩JavaScript
    std::string minifyJavaScript(const std::string& js);
    
    // 格式化JavaScript
    std::string formatJavaScript(const std::string& js);
    
    // 语法检查
    bool syntaxCheck(const std::string& js);
    
    // 代码优化
    std::string optimizeCode(const std::string& js);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 私有方法
    std::string parseJavaScript(const std::string& source);
    std::string generateJavaScript(const std::string& parsed);
    void addError(const std::string& error);
    
    // JavaScript处理辅助方法
    std::string processFunctions(const std::string& js);
    std::string processVariables(const std::string& js);
    std::string processClasses(const std::string& js);
    std::string processImports(const std::string& js);
    std::string processExports(const std::string& js);
    std::string processExpressions(const std::string& js);
    
    // 验证辅助方法
    bool validateStatement(const std::string& statement);
    bool validateExpression(const std::string& expression);
    bool validateFunction(const std::string& function);
    bool validateClass(const std::string& classDef);
    
    // 优化辅助方法
    std::string removeUnusedVariables(const std::string& js);
    std::string optimizeExpressions(const std::string& js);
    std::string inlineSimpleFunctions(const std::string& js);
};

} // namespace chtl
#pragma once

#include "common/types.hpp"
#include "dispatcher/compiler_dispatcher.hpp"
#include <string>
#include <vector>
#include <memory>

// 标准JavaScript解析器 - 基于正则表达式
// 专注于JavaScript基本语法元素的识别和分析

namespace chtl {

/**
 * JavaScript编译选项
 */
struct JSCompileOptions {
    bool minify = false;                      // 是否压缩JavaScript
    bool validateSyntax = true;               // 是否验证语法
    bool extractFunctions = true;             // 是否提取函数
    bool extractVariables = true;             // 是否提取变量
    bool extractClasses = false;              // 是否提取类
    std::string outputFormat = "js";          // 输出格式：js, json
};

/**
 * JavaScript编译结果
 */
struct JSCompileResult {
    bool success = false;
    std::string javascript;                   // 编译后的JavaScript
    std::string json;                         // JSON格式的结构化信息
    std::vector<std::string> errors;          // 错误信息
    std::vector<std::string> warnings;        // 警告信息
    std::vector<std::string> functions;       // 提取的函数名
    std::vector<std::string> variables;       // 提取的变量名
    std::vector<std::string> classes;         // 提取的类名
    size_t originalSize = 0;                  // 原始大小
    size_t compiledSize = 0;                  // 编译后大小
};

/**
 * 标准JavaScript编译器
 * 
 * 专注于JavaScript基本语法元素的解析和分析，
 * 提供函数、变量、类的识别功能。
 */
class ANTLRJSCompiler : public ICompiler {
public:
    ANTLRJSCompiler();
    virtual ~ANTLRJSCompiler() = default;

    // ICompiler接口实现
    std::string getName() const override { return "StandardJSCompiler (ES6 Basic)"; }
    std::vector<CodeSegmentType> getSupportedTypes() const override;
    CompileResult compile(const CodeSegment& segment, const CompileConfig& config) override;

    // JavaScript特定的编译方法
    JSCompileResult compileJS(const std::string& jsCode, const JSCompileOptions& options = {});
    
    // 验证JavaScript语法
    bool validateSyntax(const std::string& jsCode, std::vector<std::string>& errors);
    
    // JavaScript解析方法
    std::vector<std::string> extractFunctions(const std::string& jsCode);
    std::vector<std::string> extractVariables(const std::string& jsCode);
    std::vector<std::string> extractClasses(const std::string& jsCode);
    
    // JavaScript工具方法
    std::string minifyJS(const std::string& jsCode);
    std::string formatJS(const std::string& jsCode, const std::string& indent = "  ");
    std::string convertToES5(const std::string& jsCode);
    std::string convertToES6(const std::string& jsCode);

private:
    // 标准JavaScript解析（基于正则表达式）
    std::vector<std::string> extractFunctionsRegex(const std::string& jsCode);
    std::vector<std::string> extractVariablesRegex(const std::string& jsCode);
    std::vector<std::string> extractClassesRegex(const std::string& jsCode);
};

/**
 * 标准JavaScript工具类
 */
class JSUtils {
public:
    // 标识符相关
    static bool isValidIdentifier(const std::string& identifier);
    static bool isKeyword(const std::string& word);
    static std::string detectESVersion(const std::string& jsCode);
    
    // 函数相关
    static std::string extractFunctionName(const std::string& funcDecl);
    static std::vector<std::string> extractParameters(const std::string& funcDecl);
    
    // 变量相关
    static std::string extractVariableName(const std::string& varDecl);
    static std::string detectVariableType(const std::string& varDecl);
    
    // 格式化相关
    static std::string removeComments(const std::string& js);
    static std::string removeWhitespace(const std::string& js);
    static std::string addIndentation(const std::string& js, const std::string& indent);
    
    // 分析相关
    static int calculateComplexity(const std::string& jsCode);
    static std::vector<std::string> findDependencies(const std::string& jsCode);
};

} // namespace chtl
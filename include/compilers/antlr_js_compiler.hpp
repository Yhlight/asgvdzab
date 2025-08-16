#pragma once

#include "common/types.hpp"
#include "dispatcher/compiler_dispatcher.hpp"
#include <string>
#include <vector>
#include <memory>

// ANTLR includes
#include "antlr4-runtime.h"
#include "JavaScriptLexer.h"
#include "JavaScriptParser.h"
#include "JavaScriptBaseListener.h"

namespace chtl {

/**
 * JavaScript编译选项
 */
struct JSCompileOptions {
    bool minify = false;                // 是否压缩JS
    bool validateSyntax = true;         // 是否验证语法
    bool extractFunctions = false;      // 是否提取函数信息
    bool extractVariables = false;      // 是否提取变量信息
    bool extractClasses = false;        // 是否提取类信息
    std::string targetVersion = "es2015"; // 目标ES版本
    std::string outputFormat = "js";    // 输出格式：js, json
};

/**
 * JavaScript编译结果
 */
struct JSCompileResult {
    bool success = false;
    std::string javascript;             // 编译后的JS
    std::string json;                   // JSON格式的结构化信息
    std::vector<std::string> errors;    // 错误信息
    std::vector<std::string> warnings;  // 警告信息
    std::vector<std::string> functions; // 提取的函数
    std::vector<std::string> variables; // 提取的变量
    std::vector<std::string> classes;   // 提取的类
    std::vector<std::string> imports;   // 提取的导入
    std::vector<std::string> exports;   // 提取的导出
    size_t originalSize = 0;            // 原始大小
    size_t compiledSize = 0;            // 编译后大小
};

/**
 * JavaScript AST访问器，用于遍历ANTLR解析树
 */
class JSASTVisitor : public JavaScriptBaseListener {
public:
    JSASTVisitor();
    
    // 访问器方法
    void enterProgram(JavaScriptParser::ProgramContext *ctx) override;
    void enterFunctionDeclaration(JavaScriptParser::FunctionDeclarationContext *ctx) override;
    void enterVariableStatement(JavaScriptParser::VariableStatementContext *ctx) override;
    void enterVariableDeclaration(JavaScriptParser::VariableDeclarationContext *ctx) override;
    
    // 获取提取的信息
    const std::vector<std::string>& getFunctions() const { return functions_; }
    const std::vector<std::string>& getVariables() const { return variables_; }
    const std::vector<std::string>& getClasses() const { return classes_; }
    
private:
    std::vector<std::string> functions_;
    std::vector<std::string> variables_;
    std::vector<std::string> classes_;
};

/**
 * 基于ANTLR的JavaScript编译器
 */
class ANTLRJSCompiler : public ICompiler {
public:
    ANTLRJSCompiler();
    virtual ~ANTLRJSCompiler() = default;

    // ICompiler接口实现
    std::string getName() const override { return "ANTLRJSCompiler"; }
    std::vector<CodeSegmentType> getSupportedTypes() const override;
    CompileResult compile(const CodeSegment& segment, const CompileConfig& config) override;

    // JavaScript特定的编译方法
    JSCompileResult compileJS(const std::string& jsCode, const JSCompileOptions& options = {});
    
    // 验证JavaScript语法
    bool validateSyntax(const std::string& jsCode, std::vector<std::string>& errors);
    
    // JavaScript工具方法
    std::vector<std::string> extractFunctions(const std::string& jsCode);
    std::vector<std::string> extractVariables(const std::string& jsCode);
    std::vector<std::string> extractClasses(const std::string& jsCode);
    std::string minifyJS(const std::string& jsCode);
    std::string formatJS(const std::string& jsCode, const std::string& indent = "  ");
    
    // ES版本转换
    std::string convertToES5(const std::string& jsCode);
    std::string convertToES6(const std::string& jsCode);

private:
    // ANTLR解析
    std::unique_ptr<antlr4::tree::ParseTree> parseJS(const std::string& jsCode, 
                                                      std::vector<std::string>& errors);
    
    // 代码生成
    std::string generateJS(antlr4::tree::ParseTree* tree, const JSCompileOptions& options);
    
    // 信息提取
    void extractInfo(antlr4::tree::ParseTree* tree, JSCompileResult& result, 
                     const JSCompileOptions& options);
};

/**
 * JavaScript工具类
 */
class JSUtils {
public:
    // 语法检测
    static bool isValidIdentifier(const std::string& identifier);
    static bool isKeyword(const std::string& word);
    static std::string detectESVersion(const std::string& jsCode);
    
    // 函数相关
    static std::string extractFunctionName(const std::string& functionDecl);
    static std::vector<std::string> extractParameters(const std::string& functionDecl);
    
    // 变量相关
    static std::string extractVariableName(const std::string& varDecl);
    static std::string detectVariableType(const std::string& varDecl);
    
    // 格式化相关
    static std::string removeComments(const std::string& js);
    static std::string removeWhitespace(const std::string& js);
    static std::string addIndentation(const std::string& js, const std::string& indent);
    
    // 代码分析
    static int calculateComplexity(const std::string& jsCode);
    static std::vector<std::string> findDependencies(const std::string& jsCode);
};

} // namespace chtl
#pragma once

#include "core/compiler_dispatcher.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <functional>

// ANTLR相关前向声明
namespace antlr4 {
    class ANTLRInputStream;
    class CommonTokenStream;
    class tree::ParseTree;
}

namespace chtl {

// 前向声明ANTLR生成的类
class JavaScriptLexer;
class JavaScriptParser;

/**
 * JavaScript编译器实现
 * 负责编译标准JavaScript片段
 * 基于ANTLR进行语法解析和AST构建
 */
class JavaScriptCompiler : public IJavaScriptCompiler {
public:
    JavaScriptCompiler();
    virtual ~JavaScriptCompiler();
    
    // 基础编译器接口
    std::string getId() const override;
    std::string getVersion() const override;
    bool initialize() override;
    void cleanup() override;
    CompilationResult compile(const CodeFragment& fragment) override;
    
    // JavaScript编译器特有接口
    bool supportsANTLR() const override;
    bool supportsES6() const override;
    CompilationResult compileScript(const CodeFragment& fragment) override;
    
    // JavaScript特有功能
    void setECMAScriptVersion(const std::string& version); // "ES5", "ES6", "ES2017", "ES2020"等
    void setMinification(bool enabled);
    void setStrictMode(bool enabled);
    void setSourceMap(bool enabled);
    void setModuleSystem(const std::string& system); // "CommonJS", "ES6", "AMD", "UMD"
    void setPolyfills(const std::vector<std::string>& polyfills);
    void setBrowserTargets(const std::vector<std::string>& targets);
    
private:
    bool initialized_;
    std::string ecmaVersion_;
    bool minificationEnabled_;
    bool strictModeEnabled_;
    bool sourceMapEnabled_;
    std::string moduleSystem_;
    std::vector<std::string> polyfills_;
    std::vector<std::string> browserTargets_;
    
    // ANTLR组件
    std::unique_ptr<antlr4::ANTLRInputStream> inputStream_;
    std::unique_ptr<JavaScriptLexer> lexer_;
    std::unique_ptr<antlr4::CommonTokenStream> tokenStream_;
    std::unique_ptr<JavaScriptParser> parser_;
    
    /**
     * 使用ANTLR解析JavaScript
     */
    antlr4::tree::ParseTree* parseJavaScript(const std::string& jsContent);
    
    /**
     * 构建JavaScript AST
     */
    std::unique_ptr<JSASTNode> buildAST(antlr4::tree::ParseTree* parseTree);
    
    /**
     * 编译JavaScript AST为输出
     */
    std::string compileAST(const JSASTNode* ast);
    
    /**
     * 处理变量声明
     */
    std::string processVariableDeclarations(const std::vector<std::string>& declarations);
    
    /**
     * 处理函数定义
     */
    std::string processFunctionDefinitions(const std::vector<std::string>& functions);
    
    /**
     * 处理类定义
     */
    std::string processClassDefinitions(const std::vector<std::string>& classes);
    
    /**
     * 处理模块导入导出
     */
    std::string processModuleStatements(const std::vector<std::string>& imports, 
                                       const std::vector<std::string>& exports);
    
    /**
     * 处理异步函数和Promise
     */
    std::string processAsyncCode(const std::string& asyncCode);
    
    /**
     * 处理箭头函数
     */
    std::string processArrowFunctions(const std::string& code);
    
    /**
     * 处理解构赋值
     */
    std::string processDestructuring(const std::string& code);
    
    /**
     * 处理模板字符串
     */
    std::string processTemplateStrings(const std::string& code);
    
    /**
     * 处理展开操作符
     */
    std::string processSpreadOperator(const std::string& code);
    
    /**
     * 转换ES6+语法到目标版本
     */
    std::string transpileToTarget(const std::string& code);
    
    /**
     * 添加Polyfills
     */
    std::string addPolyfills(const std::string& code);
    
    /**
     * 优化JavaScript输出
     */
    std::string optimizeJavaScript(const std::string& js);
    
    /**
     * 压缩JavaScript
     */
    std::string minifyJavaScript(const std::string& js);
    
    /**
     * 生成Source Map
     */
    std::string generateSourceMap(const std::string& original, const std::string& compiled);
    
    /**
     * 验证JavaScript语法
     */
    bool validateJavaScript(const std::string& js, std::vector<std::string>& errors);
    
    /**
     * 检测语法特性
     */
    std::vector<std::string> detectSyntaxFeatures(const std::string& js);
    
    /**
     * 清理ANTLR资源
     */
    void cleanupANTLRResources();
};

/**
 * JavaScript AST节点基类
 */
class JSASTNode {
public:
    enum Type {
        PROGRAM, STATEMENT, EXPRESSION, DECLARATION, FUNCTION, 
        CLASS, VARIABLE, LITERAL, IDENTIFIER, OPERATOR,
        IMPORT, EXPORT, ASYNC_FUNCTION, ARROW_FUNCTION
    };
    
    virtual ~JSASTNode() = default;
    virtual Type getType() const = 0;
    virtual std::string toString() const = 0;
    virtual std::unique_ptr<JSASTNode> clone() const = 0;
};

/**
 * JavaScript程序节点
 */
class JSProgramNode : public JSASTNode {
public:
    std::vector<std::unique_ptr<JSASTNode>> statements;
    bool isModule;
    bool isStrict;
    
    Type getType() const override { return PROGRAM; }
    std::string toString() const override;
    std::unique_ptr<JSASTNode> clone() const override;
};

/**
 * JavaScript函数节点
 */
class JSFunctionNode : public JSASTNode {
public:
    std::string name;
    std::vector<std::string> parameters;
    std::unique_ptr<JSASTNode> body;
    bool isAsync;
    bool isGenerator;
    bool isArrow;
    
    Type getType() const override { return FUNCTION; }
    std::string toString() const override;
    std::unique_ptr<JSASTNode> clone() const override;
};

/**
 * JavaScript类节点
 */
class JSClassNode : public JSASTNode {
public:
    std::string name;
    std::string superClass;
    std::vector<std::unique_ptr<JSASTNode>> methods;
    std::vector<std::unique_ptr<JSASTNode>> properties;
    
    Type getType() const override { return CLASS; }
    std::string toString() const override;
    std::unique_ptr<JSASTNode> clone() const override;
};

/**
 * JavaScript变量声明节点
 */
class JSVariableNode : public JSASTNode {
public:
    enum VarType { VAR, LET, CONST };
    VarType varType;
    std::string name;
    std::unique_ptr<JSASTNode> initializer;
    
    Type getType() const override { return VARIABLE; }
    std::string toString() const override;
    std::unique_ptr<JSASTNode> clone() const override;
};

/**
 * JavaScript语法分析器
 */
class JSSyntaxAnalyzer {
public:
    struct SyntaxFeatures {
        bool hasES6Classes;
        bool hasArrowFunctions;
        bool hasAsyncAwait;
        bool hasDestructuring;
        bool hasTemplateStrings;
        bool hasSpreadOperator;
        bool hasModules;
        bool hasGenerators;
        std::vector<std::string> detectedFeatures;
    };
    
    /**
     * 分析JavaScript语法特性
     */
    static SyntaxFeatures analyzeSyntaxFeatures(const std::string& code);
    
    /**
     * 检测ES版本兼容性
     */
    static std::string detectRequiredESVersion(const std::string& code);
    
    /**
     * 查找函数定义
     */
    static std::vector<std::string> findFunctionDefinitions(const std::string& code);
    
    /**
     * 查找变量声明
     */
    static std::vector<std::string> findVariableDeclarations(const std::string& code);
    
    /**
     * 查找类定义
     */
    static std::vector<std::string> findClassDefinitions(const std::string& code);
    
    /**
     * 检测循环依赖
     */
    static bool hasCircularDependencies(const std::vector<std::string>& imports);
};

/**
 * JavaScript转译器
 * 负责将高版本JavaScript语法转换为低版本兼容语法
 */
class JSTranspiler {
public:
    /**
     * 转译箭头函数
     */
    static std::string transpileArrowFunctions(const std::string& code);
    
    /**
     * 转译类语法
     */
    static std::string transpileClasses(const std::string& code);
    
    /**
     * 转译解构赋值
     */
    static std::string transpileDestructuring(const std::string& code);
    
    /**
     * 转译模板字符串
     */
    static std::string transpileTemplateStrings(const std::string& code);
    
    /**
     * 转译async/await
     */
    static std::string transpileAsyncAwait(const std::string& code);
    
    /**
     * 转译展开操作符
     */
    static std::string transpileSpreadOperator(const std::string& code);
    
    /**
     * 转译let/const为var
     */
    static std::string transpileBlockScopedVariables(const std::string& code);
    
    /**
     * 转译模块语法
     */
    static std::string transpileModules(const std::string& code, const std::string& targetSystem);
};

/**
 * JavaScript优化器
 */
class JSOptimizer {
public:
    struct OptimizationOptions {
        bool removeComments = true;
        bool removeWhitespace = true;
        bool mangleNames = false;
        bool removeUnusedCode = false;
        bool inlineFunctions = false;
        bool constantFolding = true;
        bool removeConsoleLog = false;
    };
    
    /**
     * 优化JavaScript代码
     */
    static std::string optimize(const std::string& js, const OptimizationOptions& options = {});
    
    /**
     * 移除死代码
     */
    static std::string removeDeadCode(const std::string& js);
    
    /**
     * 内联小函数
     */
    static std::string inlineSmallFunctions(const std::string& js);
    
    /**
     * 常量折叠
     */
    static std::string foldConstants(const std::string& js);
    
    /**
     * 变量名混淆
     */
    static std::string mangleVariableNames(const std::string& js);
    
    /**
     * 移除console.log
     */
    static std::string removeConsoleLogs(const std::string& js);
};

/**
 * ANTLR JavaScript监听器
 */
class JavaScriptListener {
public:
    virtual ~JavaScriptListener() = default;
    
    virtual void enterProgram(JavaScriptParser::ProgramContext* ctx) {}
    virtual void exitProgram(JavaScriptParser::ProgramContext* ctx) {}
    
    virtual void enterFunctionDeclaration(JavaScriptParser::FunctionDeclarationContext* ctx) {}
    virtual void exitFunctionDeclaration(JavaScriptParser::FunctionDeclarationContext* ctx) {}
    
    virtual void enterClassDeclaration(JavaScriptParser::ClassDeclarationContext* ctx) {}
    virtual void exitClassDeclaration(JavaScriptParser::ClassDeclarationContext* ctx) {}
    
    virtual void enterVariableDeclaration(JavaScriptParser::VariableDeclarationContext* ctx) {}
    virtual void exitVariableDeclaration(JavaScriptParser::VariableDeclarationContext* ctx) {}
    
    virtual void enterImportStatement(JavaScriptParser::ImportStatementContext* ctx) {}
    virtual void exitImportStatement(JavaScriptParser::ImportStatementContext* ctx) {}
    
    virtual void enterExportStatement(JavaScriptParser::ExportStatementContext* ctx) {}
    virtual void exitExportStatement(JavaScriptParser::ExportStatementContext* ctx) {}
};

/**
 * JavaScript编译监听器
 */
class JSCompileListener : public JavaScriptListener {
public:
    JSCompileListener();
    
    void enterProgram(JavaScriptParser::ProgramContext* ctx) override;
    void exitProgram(JavaScriptParser::ProgramContext* ctx) override;
    
    void enterFunctionDeclaration(JavaScriptParser::FunctionDeclarationContext* ctx) override;
    void exitFunctionDeclaration(JavaScriptParser::FunctionDeclarationContext* ctx) override;
    
    void enterClassDeclaration(JavaScriptParser::ClassDeclarationContext* ctx) override;
    void exitClassDeclaration(JavaScriptParser::ClassDeclarationContext* ctx) override;
    
    void enterVariableDeclaration(JavaScriptParser::VariableDeclarationContext* ctx) override;
    void exitVariableDeclaration(JavaScriptParser::VariableDeclarationContext* ctx) override;
    
    void enterImportStatement(JavaScriptParser::ImportStatementContext* ctx) override;
    void exitImportStatement(JavaScriptParser::ImportStatementContext* ctx) override;
    
    void enterExportStatement(JavaScriptParser::ExportStatementContext* ctx) override;
    void exitExportStatement(JavaScriptParser::ExportStatementContext* ctx) override;
    
    std::unique_ptr<JSProgramNode> getProgram() { return std::move(program_); }
    
private:
    std::unique_ptr<JSProgramNode> program_;
    std::vector<std::unique_ptr<JSFunctionNode>> functionStack_;
    std::vector<std::unique_ptr<JSClassNode>> classStack_;
    std::vector<std::string> currentImports_;
    std::vector<std::string> currentExports_;
};

/**
 * JavaScript运行时环境
 */
class JSRuntimeEnvironment {
public:
    /**
     * 初始化运行时环境
     */
    static bool initialize();
    
    /**
     * 清理运行时环境
     */
    static void cleanup();
    
    /**
     * 执行JavaScript代码
     */
    static std::string execute(const std::string& code);
    
    /**
     * 设置全局变量
     */
    static void setGlobalVariable(const std::string& name, const std::string& value);
    
    /**
     * 获取全局变量
     */
    static std::string getGlobalVariable(const std::string& name);
    
    /**
     * 加载模块
     */
    static bool loadModule(const std::string& moduleName, const std::string& code);
    
private:
    static std::unordered_map<std::string, std::string> globalVariables_;
    static std::unordered_map<std::string, std::string> loadedModules_;
    static bool initialized_;
};

} // namespace chtl
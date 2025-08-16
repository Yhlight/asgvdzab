#pragma once

#include "core/compiler_dispatcher.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

namespace chtl {

/**
 * CHTL JS编译器实现
 * 负责编译CHTL JS混合片段
 * 使用手写解析器，专门处理CHTL和JavaScript的混合语法
 */
class CHTLJSCompiler : public ICHTLJSCompiler {
public:
    CHTLJSCompiler();
    virtual ~CHTLJSCompiler();
    
    // 基础编译器接口
    std::string getId() const override;
    std::string getVersion() const override;
    bool initialize() override;
    void cleanup() override;
    CompilationResult compile(const CodeFragment& fragment) override;
    
    // CHTL JS编译器特有接口
    bool supportsVariableGroup() const override;
    bool supportsFromClause() const override;
    CompilationResult compileChtlJsMix(const CodeFragment& fragment) override;
    
    // CHTL JS特有功能
    void setTemplateVariables(const std::unordered_map<std::string, std::string>& variables);
    void setCustomVariables(const std::unordered_map<std::string, std::string>& variables);
    void setNamespaces(const std::unordered_map<std::string, std::string>& namespaces);
    void setJavaScriptContext(bool isES6, bool isStrict);
    
private:
    bool initialized_;
    bool es6Support_;
    bool strictMode_;
    std::unordered_map<std::string, std::string> templateVariables_;
    std::unordered_map<std::string, std::string> customVariables_;
    std::unordered_map<std::string, std::string> namespaces_;
    
    /**
     * 编译混合语法片段
     */
    std::string compileMixedSyntax(const std::string& content);
    
    /**
     * 分离CHTL和JavaScript代码
     */
    struct SeparatedCode {
        std::vector<std::pair<std::string, std::string>> segments; // (type, content)
        std::vector<size_t> positions;
    };
    SeparatedCode separateChtlAndJs(const std::string& content);
    
    /**
     * 编译CHTL模板变量
     */
    std::string compileChtlVariables(const std::string& content);
    
    /**
     * 编译CHTL变量组语法
     */
    std::string compileChtlVariableGroups(const std::string& content);
    
    /**
     * 编译CHTL from语句
     */
    std::string compileChtlFromStatements(const std::string& content);
    
    /**
     * 编译原始嵌入
     */
    std::string compileChtlRawEmbedding(const std::string& content);
    
    /**
     * 处理生成器注释
     */
    std::string processChtlGeneratorComments(const std::string& content);
    
    /**
     * 编译JavaScript部分
     */
    std::string compileJavaScriptSegment(const std::string& jsCode);
    
    /**
     * 验证JavaScript语法
     */
    bool validateJavaScriptSyntax(const std::string& jsCode, std::vector<std::string>& errors);
    
    /**
     * 处理JavaScript模板字符串中的CHTL语法
     */
    std::string processTemplateStrings(const std::string& jsCode);
    
    /**
     * 生成JavaScript函数包装
     */
    std::string generateJsFunctionWrapper(const std::string& body);
    
    /**
     * 优化JavaScript输出
     */
    std::string optimizeJavaScriptOutput(const std::string& js);
    
    /**
     * 处理作用域和变量绑定
     */
    std::string processScopeAndBinding(const std::string& content);
    
    /**
     * 生成运行时变量访问代码
     */
    std::string generateRuntimeVariableAccess(const std::string& varName, const std::string& varType);
};

/**
 * CHTL JS语法解析器
 * 专门用于解析CHTL和JavaScript的混合语法
 */
class CHTLJSSyntaxParser {
public:
    struct MixedSegment {
        enum Type { CHTL, JAVASCRIPT, COMMENT, STRING_LITERAL, TEMPLATE_STRING };
        Type type;
        std::string content;
        size_t startPos;
        size_t endPos;
        int nestingLevel;
    };
    
    struct ChtlVariableInJs {
        std::string name;
        std::string type;        // "template", "custom", "variable_group"
        std::string jsContext;   // 所在的JavaScript上下文
        size_t startPos;
        size_t endPos;
        bool isInTemplateString;
    };
    
    struct FromClause {
        std::string namespace_;
        std::vector<std::string> imports;
        std::string asAlias;
        std::vector<std::string> except;
        bool isInJsContext;
    };
    
    /**
     * 解析混合语法片段
     */
    static std::vector<MixedSegment> parseMixedSegments(const std::string& content);
    
    /**
     * 解析JavaScript中的CHTL变量
     */
    static std::vector<ChtlVariableInJs> parseChtlVariablesInJs(const std::string& content);
    
    /**
     * 解析from语句
     */
    static std::vector<FromClause> parseFromClauses(const std::string& content);
    
    /**
     * 检测JavaScript语法特征
     */
    static bool hasJavaScriptSyntax(const std::string& content);
    
    /**
     * 检测CHTL语法特征
     */
    static bool hasChtlSyntax(const std::string& content);
    
    /**
     * 验证混合语法合法性
     */
    static bool validateMixedSyntax(const std::string& content, std::vector<std::string>& errors);
    
    /**
     * 查找JavaScript模板字符串
     */
    static std::vector<std::pair<size_t, size_t>> findTemplateStrings(const std::string& jsCode);
    
    /**
     * 查找JavaScript函数定义
     */
    static std::vector<std::pair<size_t, size_t>> findFunctionDefinitions(const std::string& jsCode);
    
private:
    /**
     * 跳过字符串字面量
     */
    static size_t skipStringLiteral(const std::string& content, size_t pos, char quote);
    
    /**
     * 跳过模板字符串
     */
    static size_t skipTemplateString(const std::string& content, size_t pos);
    
    /**
     * 跳过注释
     */
    static size_t skipComment(const std::string& content, size_t pos);
    
    /**
     * 查找匹配的大括号
     */
    static size_t findMatchingBrace(const std::string& content, size_t pos);
};

/**
 * CHTL JS代码生成器
 * 将解析后的CHTL JS混合语法转换为纯JavaScript输出
 */
class CHTLJSCodeGenerator {
public:
    CHTLJSCodeGenerator();
    
    /**
     * 生成JavaScript代码
     */
    std::string generateJavaScript(const std::vector<CHTLJSSyntaxParser::MixedSegment>& segments);
    
    /**
     * 生成变量访问代码
     */
    std::string generateVariableAccess(const std::string& varName, const std::string& varType);
    
    /**
     * 生成from语句导入代码
     */
    std::string generateFromImport(const CHTLJSSyntaxParser::FromClause& fromClause);
    
    /**
     * 生成运行时辅助函数
     */
    std::string generateRuntimeHelpers();
    
    /**
     * 处理CHTL模板变量替换
     */
    std::string processChtlVariableSubstitution(const std::string& jsCode, 
                                               const std::unordered_map<std::string, std::string>& variables);
    
    /**
     * 生成作用域管理代码
     */
    std::string generateScopeManagement(const std::string& jsCode);
    
    /**
     * 压缩JavaScript输出
     */
    std::string compressJavaScript(const std::string& js);
    
    /**
     * 添加严格模式声明
     */
    std::string addStrictModeIfNeeded(const std::string& js, bool strictMode);
    
private:
    int helperFunctionCounter_;
    std::unordered_map<std::string, std::string> generatedHelpers_;
    
    /**
     * 转义JavaScript字符串
     */
    std::string escapeJavaScriptString(const std::string& str);
    
    /**
     * 生成唯一函数名
     */
    std::string generateUniqueFunctionName(const std::string& baseName);
    
    /**
     * 检查变量名合法性
     */
    bool isValidJavaScriptIdentifier(const std::string& name);
    
    /**
     * 生成变量绑定代码
     */
    std::string generateVariableBinding(const std::string& varName, const std::string& value);
};

/**
 * CHTL JS运行时
 * 提供CHTL JS代码执行时的运行时支持
 */
class CHTLJSRuntime {
public:
    /**
     * 初始化运行时环境
     */
    static bool initializeRuntime();
    
    /**
     * 清理运行时环境
     */
    static void cleanupRuntime();
    
    /**
     * 注册变量
     */
    static void registerVariable(const std::string& name, const std::string& value);
    
    /**
     * 获取变量值
     */
    static std::string getVariable(const std::string& name);
    
    /**
     * 执行CHTL JS代码
     */
    static std::string executeCode(const std::string& jsCode);
    
    /**
     * 加载命名空间
     */
    static bool loadNamespace(const std::string& namespace_, const std::string& path);
    
private:
    static std::unordered_map<std::string, std::string> variables_;
    static std::unordered_map<std::string, std::string> namespaces_;
    static bool initialized_;
};

} // namespace chtl
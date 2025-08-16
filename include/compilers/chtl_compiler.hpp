#pragma once

#include "core/compiler_dispatcher.hpp"
#include <memory>
#include <unordered_map>
#include <string>

namespace chtl {

/**
 * CHTL编译器实现
 * 负责编译CHTL片段（主要是局部style使用）
 * 使用手写解析器，专门处理CHTL语法
 */
class CHTLCompiler : public ICHTLCompiler {
public:
    CHTLCompiler();
    virtual ~CHTLCompiler();
    
    // 基础编译器接口
    std::string getId() const override;
    std::string getVersion() const override;
    bool initialize() override;
    void cleanup() override;
    CompilationResult compile(const CodeFragment& fragment) override;
    
    // CHTL编译器特有接口
    bool supportsLocalStyle() const override;
    CompilationResult compileLocalStyle(const CodeFragment& fragment) override;
    
    // CHTL特有功能
    void setTemplateVariables(const std::unordered_map<std::string, std::string>& variables);
    void setCustomVariables(const std::unordered_map<std::string, std::string>& variables);
    void setStyleGroups(const std::unordered_map<std::string, std::string>& groups);
    
private:
    bool initialized_;
    std::unordered_map<std::string, std::string> templateVariables_;
    std::unordered_map<std::string, std::string> customVariables_;
    std::unordered_map<std::string, std::string> styleGroups_;
    
    /**
     * 编译CHTL模板变量
     */
    std::string compileTemplateVariables(const std::string& content);
    
    /**
     * 编译自定义变量
     */
    std::string compileCustomVariables(const std::string& content);
    
    /**
     * 编译变量组语法
     */
    std::string compileVariableGroups(const std::string& content);
    
    /**
     * 编译样式组
     */
    std::string compileStyleGroups(const std::string& content);
    
    /**
     * 编译继承语句
     */
    std::string compileInheritStatements(const std::string& content);
    
    /**
     * 编译删除语句
     */
    std::string compileDeleteStatements(const std::string& content);
    
    /**
     * 编译from语句
     */
    std::string compileFromStatements(const std::string& content);
    
    /**
     * 处理生成器注释
     */
    std::string processGeneratorComments(const std::string& content);
    
    /**
     * 编译局部样式块
     */
    std::string compileLocalStyleBlock(const std::string& content);
    
    /**
     * 解析变量引用 {{variable}}
     */
    std::string parseVariableReference(const std::string& varRef);
    
    /**
     * 解析HTML变量引用 {{{variable}}}
     */
    std::string parseHtmlVariableReference(const std::string& varRef);
    
    /**
     * 解析无转义变量引用 {{&variable}}
     */
    std::string parseUnescapedVariableReference(const std::string& varRef);
    
    /**
     * 验证CHTL语法
     */
    bool validateChtlSyntax(const std::string& content, std::vector<std::string>& errors);
    
    /**
     * 生成CSS选择器
     */
    std::string generateCssSelector(const std::string& selector);
    
    /**
     * 应用样式继承
     */
    std::string applyStyleInheritance(const std::string& baseStyle, const std::string& inheritedStyle);
    
    /**
     * 处理样式删除
     */
    std::string processStyleDeletion(const std::string& style, const std::string& deleteRule);
    
    /**
     * 优化输出CSS
     */
    std::string optimizeCssOutput(const std::string& css);
};

/**
 * CHTL语法解析器
 * 专门用于解析CHTL特有语法
 */
class CHTLSyntaxParser {
public:
    struct VariableReference {
        std::string name;
        std::string type;        // "template", "custom", "html", "unescaped"
        size_t startPos;
        size_t endPos;
        std::string defaultValue;
    };
    
    struct StyleGroup {
        std::string name;
        std::string baseGroup;   // 继承的基础组
        std::vector<std::string> properties;
        std::vector<std::string> deletedProperties;
        bool isCustom;
    };
    
    struct FromStatement {
        std::string namespace_;
        std::vector<std::string> imports;
        std::string asAlias;
        std::vector<std::string> except;
    };
    
    /**
     * 解析变量引用
     */
    static std::vector<VariableReference> parseVariableReferences(const std::string& content);
    
    /**
     * 解析样式组定义
     */
    static std::vector<StyleGroup> parseStyleGroups(const std::string& content);
    
    /**
     * 解析from语句
     */
    static std::vector<FromStatement> parseFromStatements(const std::string& content);
    
    /**
     * 解析继承语句
     */
    static std::vector<std::pair<std::string, std::string>> parseInheritStatements(const std::string& content);
    
    /**
     * 解析删除语句
     */
    static std::vector<std::pair<std::string, std::string>> parseDeleteStatements(const std::string& content);
    
    /**
     * 验证变量名合法性
     */
    static bool isValidVariableName(const std::string& name);
    
    /**
     * 验证样式组名合法性
     */
    static bool isValidStyleGroupName(const std::string& name);
};

/**
 * CHTL代码生成器
 * 将解析后的CHTL语法转换为HTML/CSS输出
 */
class CHTLCodeGenerator {
public:
    CHTLCodeGenerator();
    
    /**
     * 生成HTML输出
     */
    std::string generateHtml(const std::string& template_, const std::unordered_map<std::string, std::string>& variables);
    
    /**
     * 生成CSS输出
     */
    std::string generateCss(const std::vector<CHTLSyntaxParser::StyleGroup>& styleGroups);
    
    /**
     * 应用变量替换
     */
    std::string applyVariableSubstitution(const std::string& content, const std::unordered_map<std::string, std::string>& variables);
    
    /**
     * 生成唯一CSS类名
     */
    std::string generateUniqueClassName(const std::string& baseName);
    
    /**
     * 压缩CSS输出
     */
    std::string compressCss(const std::string& css);
    
private:
    int classNameCounter_;
    std::unordered_map<std::string, std::string> classNameMapping_;
    
    /**
     * 转义HTML特殊字符
     */
    std::string escapeHtml(const std::string& text);
    
    /**
     * 处理样式组继承
     */
    std::string resolveStyleInheritance(const CHTLSyntaxParser::StyleGroup& group, 
                                      const std::vector<CHTLSyntaxParser::StyleGroup>& allGroups);
};

} // namespace chtl
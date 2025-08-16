#pragma once

#include "core/compiler_dispatcher.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

// ANTLR相关前向声明
namespace antlr4 {
    class ANTLRInputStream;
    class CommonTokenStream;
    class tree::ParseTree;
}

namespace chtl {

// 前向声明ANTLR生成的类
class CSSLexer;
class CSSParser;

/**
 * CSS编译器实现
 * 负责编译CSS片段（全局style使用）
 * 基于ANTLR进行语法解析和AST构建
 */
class CSSCompiler : public ICSSCompiler {
public:
    CSSCompiler();
    virtual ~CSSCompiler();
    
    // 基础编译器接口
    std::string getId() const override;
    std::string getVersion() const override;
    bool initialize() override;
    void cleanup() override;
    CompilationResult compile(const CodeFragment& fragment) override;
    
    // CSS编译器特有接口
    bool supportsANTLR() const override;
    CompilationResult compileGlobalStyle(const CodeFragment& fragment) override;
    
    // CSS特有功能
    void setMinification(bool enabled);
    void setCompatibilityMode(const std::string& mode); // "modern", "legacy", "all"
    void setCSSVariables(const std::unordered_map<std::string, std::string>& variables);
    void setImportResolver(std::function<std::string(const std::string&)> resolver);
    
private:
    bool initialized_;
    bool minificationEnabled_;
    std::string compatibilityMode_;
    std::unordered_map<std::string, std::string> cssVariables_;
    std::function<std::string(const std::string&)> importResolver_;
    
    // ANTLR组件
    std::unique_ptr<antlr4::ANTLRInputStream> inputStream_;
    std::unique_ptr<CSSLexer> lexer_;
    std::unique_ptr<antlr4::CommonTokenStream> tokenStream_;
    std::unique_ptr<CSSParser> parser_;
    
    /**
     * 使用ANTLR解析CSS
     */
    antlr4::tree::ParseTree* parseCSS(const std::string& cssContent);
    
    /**
     * 构建CSS AST
     */
    std::unique_ptr<CSSASTNode> buildAST(antlr4::tree::ParseTree* parseTree);
    
    /**
     * 编译CSS AST为输出
     */
    std::string compileAST(const CSSASTNode* ast);
    
    /**
     * 处理CSS选择器
     */
    std::string processSelectors(const std::vector<std::string>& selectors);
    
    /**
     * 处理CSS属性
     */
    std::string processProperties(const std::vector<std::pair<std::string, std::string>>& properties);
    
    /**
     * 处理CSS @规则
     */
    std::string processAtRules(const std::string& atRule, const std::string& content);
    
    /**
     * 处理CSS媒体查询
     */
    std::string processMediaQueries(const std::string& mediaQuery, const std::string& content);
    
    /**
     * 处理CSS动画和关键帧
     */
    std::string processKeyframes(const std::string& name, const std::string& content);
    
    /**
     * 处理CSS导入
     */
    std::string processImports(const std::string& importPath);
    
    /**
     * 处理CSS变量替换
     */
    std::string processCSSVariables(const std::string& content);
    
    /**
     * 优化CSS输出
     */
    std::string optimizeCSS(const std::string& css);
    
    /**
     * 压缩CSS
     */
    std::string minifyCSS(const std::string& css);
    
    /**
     * 添加浏览器兼容性前缀
     */
    std::string addVendorPrefixes(const std::string& css);
    
    /**
     * 验证CSS语法
     */
    bool validateCSS(const std::string& css, std::vector<std::string>& errors);
    
    /**
     * 清理ANTLR资源
     */
    void cleanupANTLRResources();
};

/**
 * CSS AST节点基类
 */
class CSSASTNode {
public:
    enum Type {
        STYLESHEET, RULE, SELECTOR, PROPERTY, AT_RULE, 
        MEDIA_QUERY, KEYFRAMES, COMMENT, IMPORT
    };
    
    virtual ~CSSASTNode() = default;
    virtual Type getType() const = 0;
    virtual std::string toString() const = 0;
    virtual std::unique_ptr<CSSASTNode> clone() const = 0;
};

/**
 * CSS样式表节点
 */
class CSSStylesheetNode : public CSSASTNode {
public:
    std::vector<std::unique_ptr<CSSASTNode>> rules;
    
    Type getType() const override { return STYLESHEET; }
    std::string toString() const override;
    std::unique_ptr<CSSASTNode> clone() const override;
};

/**
 * CSS规则节点
 */
class CSSRuleNode : public CSSASTNode {
public:
    std::vector<std::string> selectors;
    std::vector<std::pair<std::string, std::string>> properties;
    
    Type getType() const override { return RULE; }
    std::string toString() const override;
    std::unique_ptr<CSSASTNode> clone() const override;
};

/**
 * CSS @规则节点
 */
class CSSAtRuleNode : public CSSASTNode {
public:
    std::string name;           // @media, @keyframes, @import等
    std::string parameters;     // 规则参数
    std::string content;        // 规则内容
    std::vector<std::unique_ptr<CSSASTNode>> nestedRules;
    
    Type getType() const override { return AT_RULE; }
    std::string toString() const override;
    std::unique_ptr<CSSASTNode> clone() const override;
};

/**
 * CSS选择器分析器
 */
class CSSSelectorAnalyzer {
public:
    struct SelectorInfo {
        std::string original;
        std::string normalized;
        int specificity;
        std::vector<std::string> parts;
        bool hasClassSelector;
        bool hasIdSelector;
        bool hasAttributeSelector;
        bool hasPseudoSelector;
    };
    
    /**
     * 分析CSS选择器
     */
    static SelectorInfo analyzeSelector(const std::string& selector);
    
    /**
     * 计算选择器特异性
     */
    static int calculateSpecificity(const std::string& selector);
    
    /**
     * 规范化选择器
     */
    static std::string normalizeSelector(const std::string& selector);
    
    /**
     * 检查选择器冲突
     */
    static bool hasConflict(const SelectorInfo& sel1, const SelectorInfo& sel2);
};

/**
 * CSS属性验证器
 */
class CSSPropertyValidator {
public:
    /**
     * 验证CSS属性
     */
    static bool validateProperty(const std::string& property, const std::string& value);
    
    /**
     * 验证CSS属性名
     */
    static bool isValidPropertyName(const std::string& property);
    
    /**
     * 验证CSS属性值
     */
    static bool isValidPropertyValue(const std::string& property, const std::string& value);
    
    /**
     * 获取属性的可能值
     */
    static std::vector<std::string> getPossibleValues(const std::string& property);
    
    /**
     * 检查属性是否需要浏览器前缀
     */
    static bool needsVendorPrefix(const std::string& property);
    
    /**
     * 生成浏览器前缀版本
     */
    static std::vector<std::string> generateVendorPrefixes(const std::string& property, const std::string& value);
};

/**
 * CSS优化器
 */
class CSSOptimizer {
public:
    struct OptimizationOptions {
        bool removeComments = true;
        bool removeWhitespace = true;
        bool mergeRules = true;
        bool removeUnusedRules = false;
        bool optimizeSelectors = true;
        bool optimizeProperties = true;
        bool compressColors = true;
        bool shorthandProperties = true;
    };
    
    /**
     * 优化CSS代码
     */
    static std::string optimize(const std::string& css, const OptimizationOptions& options = {});
    
    /**
     * 合并相同的CSS规则
     */
    static std::string mergeIdenticalRules(const std::string& css);
    
    /**
     * 移除未使用的CSS规则
     */
    static std::string removeUnusedRules(const std::string& css, const std::vector<std::string>& usedSelectors);
    
    /**
     * 优化CSS选择器
     */
    static std::string optimizeSelectors(const std::string& css);
    
    /**
     * 优化CSS属性
     */
    static std::string optimizeProperties(const std::string& css);
    
    /**
     * 压缩颜色值
     */
    static std::string compressColors(const std::string& css);
    
    /**
     * 使用简写属性
     */
    static std::string useShorthandProperties(const std::string& css);
};

/**
 * ANTLR CSS监听器
 * 用于遍历ANTLR生成的解析树
 */
class CSSListener {
public:
    virtual ~CSSListener() = default;
    
    virtual void enterStylesheet(CSSParser::StylesheetContext* ctx) {}
    virtual void exitStylesheet(CSSParser::StylesheetContext* ctx) {}
    
    virtual void enterRule(CSSParser::RuleContext* ctx) {}
    virtual void exitRule(CSSParser::RuleContext* ctx) {}
    
    virtual void enterSelector(CSSParser::SelectorContext* ctx) {}
    virtual void exitSelector(CSSParser::SelectorContext* ctx) {}
    
    virtual void enterProperty(CSSParser::PropertyContext* ctx) {}
    virtual void exitProperty(CSSParser::PropertyContext* ctx) {}
    
    virtual void enterAtRule(CSSParser::AtRuleContext* ctx) {}
    virtual void exitAtRule(CSSParser::AtRuleContext* ctx) {}
};

/**
 * CSS编译监听器
 * 实现CSSListener以构建AST
 */
class CSSCompileListener : public CSSListener {
public:
    CSSCompileListener();
    
    void enterStylesheet(CSSParser::StylesheetContext* ctx) override;
    void exitStylesheet(CSSParser::StylesheetContext* ctx) override;
    
    void enterRule(CSSParser::RuleContext* ctx) override;
    void exitRule(CSSParser::RuleContext* ctx) override;
    
    void enterSelector(CSSParser::SelectorContext* ctx) override;
    void exitSelector(CSSParser::SelectorContext* ctx) override;
    
    void enterProperty(CSSParser::PropertyContext* ctx) override;
    void exitProperty(CSSParser::PropertyContext* ctx) override;
    
    void enterAtRule(CSSParser::AtRuleContext* ctx) override;
    void exitAtRule(CSSParser::AtRuleContext* ctx) override;
    
    std::unique_ptr<CSSStylesheetNode> getStylesheet() { return std::move(stylesheet_); }
    
private:
    std::unique_ptr<CSSStylesheetNode> stylesheet_;
    std::vector<std::unique_ptr<CSSRuleNode>> ruleStack_;
    std::vector<std::string> currentSelectors_;
    std::vector<std::pair<std::string, std::string>> currentProperties_;
};

} // namespace chtl
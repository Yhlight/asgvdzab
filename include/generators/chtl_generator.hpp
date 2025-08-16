#pragma once

#include "ast/chtl_ast.hpp"
#include "common/types.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <sstream>

namespace chtl {

/**
 * CHTL生成器错误信息
 */
struct CHTLGenerateError {
    std::string message;        // 错误消息
    Position position;          // 错误位置
    std::string context;        // 错误上下文
    std::string suggestion;     // 修复建议
};

/**
 * CHTL生成结果
 */
struct CHTLGenerateResult {
    bool success;                               // 生成是否成功
    std::string html;                          // 生成的HTML
    std::string css;                           // 提取的CSS（全局样式块）
    std::string javascript;                    // 提取的JavaScript
    std::vector<CHTLGenerateError> errors;     // 错误列表
    std::vector<std::string> warnings;         // 警告列表
    
    CHTLGenerateResult() : success(false) {}
};

/**
 * CHTL生成器配置
 */
struct CHTLGeneratorConfig {
    bool enablePrettyPrint;             // 启用格式化输出
    bool enableMinification;            // 启用最小化
    bool enableComments;                // 在输出中保留注释
    bool enableSourceMap;               // 生成源映射
    std::string indentStyle;            // 缩进样式 ("  ", "\t")
    int indentSize;                     // 缩进大小
    bool enableCSSExtraction;           // 是否提取CSS到单独文件
    bool enableJSExtraction;            // 是否提取JS到单独文件
    std::string cssPrefix;              // CSS类名前缀
    
    CHTLGeneratorConfig() 
        : enablePrettyPrint(true)
        , enableMinification(false)
        , enableComments(false)
        , enableSourceMap(false)
        , indentStyle("  ")
        , indentSize(2)
        , enableCSSExtraction(true)
        , enableJSExtraction(true)
        , cssPrefix("chtl-") {}
};

/**
 * 样式信息
 */
struct StyleInfo {
    std::string selector;               // CSS选择器
    std::string properties;             // CSS属性
    bool isInline;                      // 是否为内联样式
    Position sourcePosition;            // 源代码位置
};

/**
 * 脚本信息
 */
struct ScriptInfo {
    std::string content;                // 脚本内容
    bool isInline;                      // 是否为内联脚本
    Position sourcePosition;            // 源代码位置
};

/**
 * CHTL生成器类
 * 严格按照CHTL语法文档.md将AST转换为HTML
 * 负责：
 * - HTML结构生成
 * - CSS样式提取和处理
 * - JavaScript代码提取
 * - 模板展开和继承处理
 * - 自定义特例化处理
 * - 变量替换
 * - 命名空间处理
 */
class CHTLGenerator {
public:
    explicit CHTLGenerator(const CHTLGeneratorConfig& config = CHTLGeneratorConfig());
    ~CHTLGenerator() = default;

    /**
     * 生成HTML
     * @param ast CHTL AST根节点
     * @param filename 文件名（用于错误报告）
     * @return 生成结果
     */
    CHTLGenerateResult generate(CHTLASTNodePtr ast, const std::string& filename = "");

    /**
     * 设置模板库
     * @param templates 外部模板定义
     */
    void setTemplateLibrary(const std::unordered_map<std::string, CHTLASTNodePtr>& templates);

    /**
     * 设置自定义库
     * @param customs 外部自定义定义
     */
    void setCustomLibrary(const std::unordered_map<std::string, CHTLASTNodePtr>& customs);

    /**
     * 设置变量库
     * @param variables 外部变量定义
     */
    void setVariableLibrary(const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& variables);

private:
    CHTLGeneratorConfig config_;
    std::string filename_;
    std::vector<CHTLGenerateError> errors_;
    std::vector<std::string> warnings_;
    
    // 输出流
    std::ostringstream htmlStream_;
    std::ostringstream cssStream_;
    std::ostringstream jsStream_;
    
    // 状态管理
    int indentLevel_;
    std::string currentNamespace_;
    std::string currentElementId_;
    
    // 符号表
    std::unordered_map<std::string, CHTLASTNodePtr> templates_;
    std::unordered_map<std::string, CHTLASTNodePtr> customs_;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> variables_;
    std::unordered_map<std::string, std::string> namespaces_;
    
    // CSS和JS收集
    std::vector<StyleInfo> globalStyles_;
    std::vector<ScriptInfo> globalScripts_;
    int classCounter_;
    int idCounter_;

    // ===== 错误处理 =====
    void reportError(const std::string& message, const Position& pos, const std::string& suggestion = "");
    void reportWarning(const std::string& message, const Position& pos);
    
    // ===== 主要生成方法 =====
    void generateNode(CHTLASTNodePtr node);
    
    // ===== 顶级节点生成 =====
    void generateProgram(CHTLASTNodePtr node);
    void generateTemplateDeclaration(CHTLASTNodePtr node);
    void generateCustomDeclaration(CHTLASTNodePtr node);
    void generateOriginDeclaration(CHTLASTNodePtr node);
    void generateImportDeclaration(CHTLASTNodePtr node);
    void generateNamespaceDeclaration(CHTLASTNodePtr node);
    void generateConfigurationDeclaration(CHTLASTNodePtr node);
    
    // ===== 元素和文本生成 =====
    void generateElement(CHTLASTNodePtr node);
    void generateTextNode(CHTLASTNodePtr node);
    void generateAttribute(CHTLASTNodePtr node);
    
    // ===== 样式生成 =====
    void generateStyleBlock(CHTLASTNodePtr node);
    void generateInlineStyle(CHTLASTNodePtr node);
    void generateClassSelector(CHTLASTNodePtr node);
    void generateIdSelector(CHTLASTNodePtr node);
    void generateContextReference(CHTLASTNodePtr node);
    void generatePseudoSelector(CHTLASTNodePtr node);
    
    // ===== 模板处理 =====
    void processTemplateUsage(CHTLASTNodePtr node);
    void expandTemplate(const std::string& templateName, const std::string& templateType);
    void processInheritance(CHTLASTNodePtr node);
    
    // ===== 自定义处理 =====
    void processCustomUsage(CHTLASTNodePtr node);
    void expandCustom(const std::string& customName, const std::string& customType);
    void processSpecialization(CHTLASTNodePtr node);
    void processDeleteOperation(CHTLASTNodePtr node);
    void processInsertOperation(CHTLASTNodePtr node);
    void processIndexAccess(CHTLASTNodePtr node);
    
    // ===== 变量处理 =====
    void processVarUsage(CHTLASTNodePtr node);
    std::string resolveVariable(const std::string& varGroup, const std::string& property);
    void processVarSpecialization(CHTLASTNodePtr node);
    
    // ===== 原始嵌入处理 =====
    void processOriginHtml(CHTLASTNodePtr node);
    void processOriginStyle(CHTLASTNodePtr node);
    void processOriginJavaScript(CHTLASTNodePtr node);
    
    // ===== 脚本处理 =====
    void generateScriptBlock(CHTLASTNodePtr node);
    
    // ===== 注释处理 =====
    void generateComment(CHTLASTNodePtr node);
    
    // ===== 工具方法 =====
    void writeIndent();
    void increasedIndent();
    void decreaseIndent();
    std::string generateUniqueClassName();
    std::string generateUniqueId();
    std::string escapeHtml(const std::string& text);
    std::string escapeCss(const std::string& text);
    std::string escapeJs(const std::string& text);
    
    // ===== CSS处理 =====
    void addGlobalStyle(const std::string& selector, const std::string& properties, const Position& pos);
    void processLocalStyles(CHTLASTNodePtr elementNode);
    std::string buildCssOutput();
    
    // ===== JavaScript处理 =====
    void addGlobalScript(const std::string& content, const Position& pos);
    std::string buildJsOutput();
    
    // ===== 命名空间处理 =====
    std::string resolveNamespacedName(const std::string& name);
    void enterNamespace(const std::string& name);
    void exitNamespace();
    
    // ===== 验证方法 =====
    bool validateElement(CHTLASTNodePtr node);
    bool validateAttribute(CHTLASTNodePtr node);
    bool validateStyle(CHTLASTNodePtr node);
    
    // ===== 后处理 =====
    std::string postProcessHtml(const std::string& html);
    std::string postProcessCss(const std::string& css);
    std::string postProcessJs(const std::string& js);
    std::string minifyHtml(const std::string& html);
    std::string minifyCss(const std::string& css);
    std::string minifyJs(const std::string& js);
};

} // namespace chtl
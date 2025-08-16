#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "ast/ASTNode.h"
#include "lexer/CHTLLexer.h"
#include "parser/AttributeSystem.h"
#include "statemachine/CHTLStateMachine.h"

namespace chtl {

// 导入类型枚举
enum class ImportType {
    HTML_IMPORT,           // @Html 导入HTML文件
    STYLE_IMPORT,          // @Style 导入CSS文件
    JAVASCRIPT_IMPORT,     // @JavaScript 导入JS文件
    CHTL_IMPORT,           // @Chtl 导入CHTL文件
    CJMOD_IMPORT,          // @CJmod 导入CJmod文件
    CUSTOM_ELEMENT_IMPORT, // [Custom] @Element 导入自定义元素
    CUSTOM_STYLE_IMPORT,   // [Custom] @Style 导入自定义样式组
    CUSTOM_VAR_IMPORT,     // [Custom] @Var 导入自定义变量组
    TEMPLATE_ELEMENT_IMPORT, // [Template] @Element 导入元素模板
    TEMPLATE_STYLE_IMPORT,   // [Template] @Style 导入样式组模板
    TEMPLATE_VAR_IMPORT      // [Template] @Var 导入变量组模板
};

// 导入语法类型枚举
enum class ImportSyntaxType {
    FROM_AS,               // from ... as ...
    FROM_ONLY,             // from ... (无as)
    DIRECT_IMPORT          // 直接导入
};

// 导入信息结构
struct ImportInfo {
    std::string name;                      // 导入名称（as后的命名）
    ImportType type;                       // 导入类型
    std::string file_path;                 // 文件路径
    ImportSyntaxType syntax_type;          // 导入语法类型
    std::string target_name;               // 目标名称（如自定义元素名、样式组名称等）
    size_t line;                           // 行号
    size_t column;                         // 列号
    std::unordered_map<std::string, std::string> metadata; // 元数据
    
    ImportInfo() : type(ImportType::HTML_IMPORT), syntax_type(ImportSyntaxType::DIRECT_IMPORT), line(0), column(0) {}
};

// 模块信息结构
struct ModuleInfo {
    std::string name;                      // 模块名称
    std::string file_path;                 // 模块文件路径
    std::vector<std::string> dependencies; // 依赖列表
    bool is_loaded;                        // 是否已加载
    size_t load_time;                      // 加载时间
    
    ModuleInfo() : is_loaded(false), load_time(0) {}
};

// 导入系统配置
struct ImportSystemConfig {
    bool enable_html_import;               // 启用HTML导入
    bool enable_style_import;              // 启用CSS导入
    bool enable_javascript_import;         // 启用JavaScript导入
    bool enable_chtl_import;               // 启用CHTL导入
    bool enable_cjmod_import;              // 启用CJmod导入
    bool enable_custom_import;             // 启用自定义导入
    bool enable_template_import;           // 启用模板导入
    bool strict_path_validation;           // 严格路径验证
    bool auto_resolve_dependencies;        // 自动解析依赖
    std::string base_import_path;          // 基础导入路径
    size_t max_import_depth;               // 最大导入深度
    bool enable_debug_mode;                // 启用调试模式
    
    ImportSystemConfig() : 
        enable_html_import(true), enable_style_import(true),
        enable_javascript_import(true), enable_chtl_import(true),
        enable_cjmod_import(true), enable_custom_import(true),
        enable_template_import(true), strict_path_validation(false),
        auto_resolve_dependencies(true), max_import_depth(10),
        enable_debug_mode(false) {}
};

// 导入系统解析器类
class ImportSystem {
public:
    ImportSystem(const ImportSystemConfig& config = ImportSystemConfig());
    ~ImportSystem();
    
    // 导入声明解析
    std::shared_ptr<ASTNode> parseImportDeclaration(const std::vector<Token>& tokens, size_t& position);
    ImportInfo parseHtmlImport(const std::vector<Token>& tokens, size_t& position);
    ImportInfo parseStyleImport(const std::vector<Token>& tokens, size_t& position);
    ImportInfo parseJavaScriptImport(const std::vector<Token>& tokens, size_t& position);
    ImportInfo parseChtlImport(const std::vector<Token>& tokens, size_t& position);
    ImportInfo parseCjmodImport(const std::vector<Token>& tokens, size_t& position);
    
    // 自定义导入解析
    ImportInfo parseCustomElementImport(const std::vector<Token>& tokens, size_t& position);
    ImportInfo parseCustomStyleImport(const std::vector<Token>& tokens, size_t& position);
    ImportInfo parseCustomVarImport(const std::vector<Token>& tokens, size_t& position);
    
    // 模板导入解析
    ImportInfo parseTemplateElementImport(const std::vector<Token>& tokens, size_t& position);
    ImportInfo parseTemplateStyleImport(const std::vector<Token>& tokens, size_t& position);
    ImportInfo parseTemplateVarImport(const std::vector<Token>& tokens, size_t& position);
    
    // 导入语法解析
    ImportInfo parseFromAsImport(const std::vector<Token>& tokens, size_t& position);
    ImportInfo parseFromOnlyImport(const std::vector<Token>& tokens, size_t& position);
    ImportInfo parseDirectImport(const std::vector<Token>& tokens, size_t& position);
    
    // 导入管理
    void addImport(const ImportInfo& import_info);
    void removeImport(const std::string& name);
    ImportInfo* getImport(const std::string& name);
    std::vector<ImportInfo> getAllImports() const;
    std::vector<ImportInfo> getImportsByType(ImportType type) const;
    bool hasImport(const std::string& name) const;
    
    // 模块管理
    void addModule(const ModuleInfo& module_info);
    void removeModule(const std::string& name);
    ModuleInfo* getModule(const std::string& name);
    std::vector<ModuleInfo> getAllModules() const;
    bool hasModule(const std::string& name) const;
    
    // 导入验证
    bool validateImport(const ImportInfo& import_info);
    std::vector<std::string> validateImportList(const std::vector<ImportInfo>& imports);
    bool validateImportPath(const std::string& file_path);
    bool validateImportName(const std::string& name);
    
    // 导入处理
    bool processImport(const std::string& import_name);
    bool processImportByType(ImportType type, const std::string& file_path);
    std::string resolveImportPath(const std::string& relative_path);
    
    // 依赖管理
    bool checkCircularDependencies(const std::string& module_name);
    std::vector<std::string> getDependencyChain(const std::string& module_name);
    bool resolveDependencies(const std::string& module_name);
    
    // 配置管理
    void setConfig(const ImportSystemConfig& config);
    ImportSystemConfig getConfig() const;
    
    // 错误处理
    bool hasErrors() const;
    std::vector<std::string> getErrors() const;
    void clearErrors();
    void addError(const std::string& error_message);
    
    // 调试和诊断
    std::string getDebugInfo() const;
    void enableDebugMode(bool enable);
    
    // 状态机管理
    void setStateMachine(std::shared_ptr<CHTLStateMachine> state_machine);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 内部方法
    bool isImportDeclaration(const std::vector<Token>& tokens, size_t position);
    ImportType detectImportType(const std::vector<Token>& tokens, size_t position);
    void skipWhitespace(const std::vector<Token>& tokens, size_t& position);
    bool isEndOfTokens(const std::vector<Token>& tokens, size_t position);
    
    // 导入验证辅助方法
    bool isValidImportName(const std::string& name);
    bool isValidFilePath(const std::string& file_path);
    bool hasValidFileExtension(const std::string& file_path, ImportType type);
    
    // 导入处理辅助方法
    std::string normalizeFilePath(const std::string& file_path);
    std::string extractFileExtension(const std::string& file_path);
    bool fileExists(const std::string& file_path);
    
    // 状态机管理
    std::shared_ptr<CHTLStateMachine> state_machine;
};

} // namespace chtl
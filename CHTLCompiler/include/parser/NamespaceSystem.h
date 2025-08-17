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

// 命名空间冲突类型枚举
enum class NamespaceConflictType {
    NO_CONFLICT,            // 无冲突
    NAME_COLLISION,         // 名称冲突
    IMPORT_COLLISION,       // 导入冲突
    NESTING_CONFLICT        // 嵌套冲突
};

// 命名空间冲突信息结构
struct NamespaceConflict {
    std::string namespace1;                 // 第一个命名空间
    std::string namespace2;                 // 第二个命名空间
    std::string conflicting_item;           // 冲突的项目
    NamespaceConflictType conflict_type;    // 冲突类型
    size_t line;                            // 行号
    size_t column;                          // 列号
    
    NamespaceConflict() : conflict_type(NamespaceConflictType::NO_CONFLICT), line(0), column(0) {}
};

// 命名空间项类型枚举
enum class NamespaceItemType {
    CUSTOM_ELEMENT,         // 自定义元素
    CUSTOM_STYLE,           // 自定义样式组
    CUSTOM_VAR,             // 自定义变量组
    TEMPLATE_ELEMENT,       // 元素模板
    TEMPLATE_STYLE,         // 样式组模板
    TEMPLATE_VAR,           // 变量组模板
    IMPORT_ITEM,            // 导入项
    NAMESPACE_ITEM          // 嵌套命名空间
};

// 命名空间项信息结构
struct NamespaceItem {
    std::string name;                       // 项目名称
    NamespaceItemType type;                 // 项目类型
    std::string value;                      // 项目值
    std::string source_namespace;           // 来源命名空间
    size_t line;                            // 行号
    size_t column;                          // 列号
    std::unordered_map<std::string, std::string> metadata; // 元数据
    
    NamespaceItem() : type(NamespaceItemType::CUSTOM_ELEMENT), line(0), column(0) {}
};

// 命名空间信息结构
struct NamespaceInfo {
    std::string name;                       // 命名空间名称
    std::string file_name;                  // 文件名（默认命名空间）
    std::vector<NamespaceItem> items;       // 命名空间项列表
    std::vector<NamespaceInfo> nested_namespaces; // 嵌套命名空间
    size_t line;                            // 行号
    size_t column;                          // 列号
    bool is_default;                        // 是否为默认命名空间
    
    NamespaceInfo() : line(0), column(0), is_default(false) {}
};

// 命名空间系统配置
struct NamespaceSystemConfig {
    bool enable_namespaces;                 // 启用命名空间
    bool enable_nesting;                    // 启用嵌套命名空间
    bool strict_conflict_detection;         // 严格冲突检测
    bool auto_resolve_conflicts;            // 自动解决冲突
    bool preserve_file_namespace;           // 保留文件命名空间
    size_t max_nesting_depth;               // 最大嵌套深度
    bool enable_debug_mode;                 // 启用调试模式
    
    NamespaceSystemConfig() : 
        enable_namespaces(true), enable_nesting(true),
        strict_conflict_detection(false), auto_resolve_conflicts(true),
        preserve_file_namespace(true), max_nesting_depth(10),
        enable_debug_mode(false) {}
};

// 命名空间系统解析器类
class NamespaceSystem {
public:
    NamespaceSystem(const NamespaceSystemConfig& config = NamespaceSystemConfig());
    ~NamespaceSystem();
    
    // 命名空间声明解析
    std::shared_ptr<ASTNode> parseNamespaceDeclaration(const std::vector<Token>& tokens, size_t& position);
    NamespaceInfo parseNamespace(const std::vector<Token>& tokens, size_t& position);
    std::vector<NamespaceInfo> parseNestedNamespaces(const std::vector<Token>& tokens, size_t& position);
    
    // 命名空间项解析
    NamespaceItem parseCustomElement(const std::vector<Token>& tokens, size_t& position);
    NamespaceItem parseCustomStyle(const std::vector<Token>& tokens, size_t& position);
    NamespaceItem parseCustomVar(const std::vector<Token>& tokens, size_t& position);
    NamespaceItem parseTemplateElement(const std::vector<Token>& tokens, size_t& position);
    NamespaceItem parseTemplateStyle(const std::vector<Token>& tokens, size_t& position);
    NamespaceItem parseTemplateVar(const std::vector<Token>& tokens, size_t& position);
    
    // 命名空间管理
    void addNamespace(const NamespaceInfo& namespace_info);
    void removeNamespace(const std::string& name);
    NamespaceInfo* getNamespace(const std::string& name);
    std::vector<NamespaceInfo> getAllNamespaces() const;
    bool hasNamespace(const std::string& name) const;
    
    // 命名空间项管理
    void addNamespaceItem(const std::string& namespace_name, const NamespaceItem& item);
    void removeNamespaceItem(const std::string& namespace_name, const std::string& item_name);
    NamespaceItem* getNamespaceItem(const std::string& namespace_name, const std::string& item_name);
    std::vector<NamespaceItem> getNamespaceItems(const std::string& namespace_name) const;
    
    // 命名空间合并
    bool mergeNamespaces(const std::string& target_namespace, const std::string& source_namespace);
    bool mergeNamespaceItems(const std::string& target_namespace, const std::vector<NamespaceItem>& items);
    
    // 命名空间冲突检测
    std::vector<NamespaceConflict> detectConflicts(const std::string& namespace_name);
    std::vector<NamespaceConflict> detectAllConflicts();
    bool hasConflicts(const std::string& namespace_name);
    
    // 命名空间验证
    bool validateNamespace(const NamespaceInfo& namespace_info);
    std::vector<std::string> validateNamespaceList(const std::vector<NamespaceInfo>& namespaces);
    bool validateNamespaceName(const std::string& name);
    
    // 命名空间查询
    std::string resolveItemNamespace(const std::string& item_name, const std::string& target_namespace);
    std::vector<std::string> getAvailableNamespaces();
    std::vector<std::string> getNamespaceHierarchy(const std::string& namespace_name);
    
    // 配置管理
    void setConfig(const NamespaceSystemConfig& config);
    NamespaceSystemConfig getConfig() const;
    
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
    bool isNamespaceDeclaration(const std::vector<Token>& tokens, size_t position);
    void skipWhitespace(const std::vector<Token>& tokens, size_t& position);
    bool isEndOfTokens(const std::vector<Token>& tokens, size_t position);
    
    // 命名空间验证辅助方法
    bool isValidNamespaceName(const std::string& name);
    bool isValidNestingDepth(size_t depth);
    bool hasValidNamespaceStructure(const NamespaceInfo& namespace_info);
    
    // 命名空间处理辅助方法
    std::string normalizeNamespaceName(const std::string& name);
    std::string createDefaultNamespaceName(const std::string& file_name);
    bool isReservedNamespaceName(const std::string& name);
    
    // 状态机管理
    std::shared_ptr<CHTLStateMachine> state_machine;
};

} // namespace chtl
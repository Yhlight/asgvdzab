#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include "common/types.hpp"

namespace chtl {

/**
 * 全局符号类型
 */
enum class SymbolType {
    TEMPLATE_STYLE,      // [Template] @Style
    TEMPLATE_ELEMENT,    // [Template] @Element
    TEMPLATE_VAR,        // [Template] @Var
    CUSTOM_STYLE,        // [Custom] @Style
    CUSTOM_ELEMENT,      // [Custom] @Element
    CUSTOM_VAR,          // [Custom] @Var
    ORIGIN_HTML,         // [Origin] @Html
    ORIGIN_STYLE,        // [Origin] @Style
    ORIGIN_JAVASCRIPT,   // [Origin] @JavaScript
    NAMESPACE,           // [Namespace]
    CONFIGURATION,       // [Configuration]
    IMPORT               // [Import]
};

/**
 * 符号定义
 */
struct SymbolDefinition {
    SymbolType type;
    std::string name;               // 符号名称
    std::string namespacePath;      // 命名空间路径 (如 "space.room")
    std::string content;            // 定义内容
    Position position;              // 定义位置
    std::string sourceFile;         // 源文件路径
    
    // 继承信息
    std::vector<std::string> inherits; // 继承的符号名称
    bool isExplicitInherit = false;    // 是否使用显式inherit关键字
    
    // 特例化信息（仅对Custom有效）
    struct Specialization {
        std::vector<std::string> deletedProperties;    // 删除的属性
        std::vector<std::string> deletedInheritances;  // 删除的继承
        std::unordered_map<std::string, std::string> insertions; // 插入操作
        std::unordered_map<std::string, std::string> replacements; // 替换操作
    } specialization;
    
    // 配置信息（仅对Configuration有效）
    std::unordered_map<std::string, std::string> configurations;
    
    SymbolDefinition() = default;
    SymbolDefinition(SymbolType t, const std::string& n, const std::string& c, Position pos)
        : type(t), name(n), content(c), position(pos) {}
    
    // 获取完全限定名称
    std::string getFullyQualifiedName() const;
    
    // 检查是否在指定命名空间中
    bool isInNamespace(const std::string& ns) const;
    
    std::string toString() const;
};

/**
 * 命名空间定义
 */
struct NamespaceDefinition {
    std::string name;                   // 命名空间名称
    std::string parentNamespace;        // 父命名空间
    std::vector<std::string> children;  // 子命名空间
    Position position;                  // 定义位置
    std::string sourceFile;             // 源文件路径
    
    // 约束信息
    std::vector<std::string> constraints; // except约束
    
    // 获取完整命名空间路径
    std::string getFullPath() const;
    
    // 检查是否为嵌套命名空间
    bool isNested() const;
    
    std::string toString() const;
};

/**
 * 导入定义
 */
struct ImportDefinition {
    enum ImportType {
        HTML_FILE,           // [Import] @Html from file
        STYLE_FILE,          // [Import] @Style from file  
        JAVASCRIPT_FILE,     // [Import] @JavaScript from file
        CHTL_FILE,           // [Import] @Chtl from file
        CJMOD_FILE,          // [Import] @CJmod from file
        SPECIFIC_SYMBOL      // [Import] [Custom] @Element Name from file
    };
    
    ImportType type;
    std::string symbolName;         // 符号名称（特定符号导入时）
    std::string filePath;           // 文件路径
    std::string aliasName;          // 别名（as关键字）
    std::string sourceNamespace;    // 源命名空间
    SymbolType symbolType;          // 符号类型（特定符号导入时）
    Position position;              // 导入位置
    std::string sourceFile;         // 源文件路径
    
    ImportDefinition() = default;
    
    // 获取有效名称（别名优先）
    std::string getEffectiveName() const;
    
    std::string toString() const;
};

/**
 * 全局映射系统
 * 管理所有符号定义、命名空间、导入等
 */
class GlobalMap {
public:
    GlobalMap();
    
    // === 符号管理 ===
    
    /**
     * 注册符号定义
     */
    bool registerSymbol(const SymbolDefinition& symbol);
    
    /**
     * 查找符号（支持命名空间查找）
     */
    const SymbolDefinition* findSymbol(const std::string& name, 
                                       const std::string& currentNamespace = "") const;
    
    /**
     * 查找指定类型的符号
     */
    std::vector<const SymbolDefinition*> findSymbolsByType(SymbolType type) const;
    
    /**
     * 删除符号
     */
    bool removeSymbol(const std::string& name, const std::string& namespacePath = "");
    
    /**
     * 获取所有符号
     */
    std::vector<const SymbolDefinition*> getAllSymbols() const;
    
    // === 命名空间管理 ===
    
    /**
     * 注册命名空间
     */
    bool registerNamespace(const NamespaceDefinition& ns);
    
    /**
     * 查找命名空间
     */
    const NamespaceDefinition* findNamespace(const std::string& path) const;
    
    /**
     * 获取当前命名空间的所有符号
     */
    std::vector<const SymbolDefinition*> getNamespaceSymbols(const std::string& namespacePath) const;
    
    /**
     * 检查命名空间约束
     */
    bool checkNamespaceConstraints(const std::string& namespacePath, 
                                   const std::string& symbolName, 
                                   SymbolType symbolType) const;
    
    // === 导入管理 ===
    
    /**
     * 注册导入
     */
    bool registerImport(const ImportDefinition& import);
    
    /**
     * 查找导入
     */
    const ImportDefinition* findImport(const std::string& name) const;
    
    /**
     * 获取所有导入
     */
    std::vector<const ImportDefinition*> getAllImports() const;
    
    /**
     * 解析导入的符号
     */
    bool resolveImports();
    
    // === 继承处理 ===
    
    /**
     * 解析继承关系
     */
    SymbolDefinition resolveInheritance(const SymbolDefinition& symbol) const;
    
    /**
     * 检查循环继承
     */
    bool hasCircularInheritance(const std::string& symbolName, 
                                std::unordered_set<std::string>& visited) const;
    
    // === 配置管理 ===
    
    /**
     * 注册配置
     */
    void registerConfiguration(const std::unordered_map<std::string, std::string>& config);
    
    /**
     * 获取配置值
     */
    std::string getConfigValue(const std::string& key, const std::string& defaultValue = "") const;
    
    /**
     * 获取所有配置
     */
    const std::unordered_map<std::string, std::string>& getAllConfigurations() const;
    
    // === 实用工具 ===
    
    /**
     * 设置当前文件路径
     */
    void setCurrentFile(const std::string& filePath);
    
    /**
     * 获取当前文件路径
     */
    const std::string& getCurrentFile() const;
    
    /**
     * 清空所有数据
     */
    void clear();
    
    /**
     * 获取统计信息
     */
    struct Statistics {
        size_t totalSymbols = 0;
        size_t totalNamespaces = 0;
        size_t totalImports = 0;
        std::unordered_map<SymbolType, size_t> symbolsByType;
    };
    Statistics getStatistics() const;
    
    /**
     * 导出所有定义到字符串（调试用）
     */
    std::string exportToString() const;
    
    /**
     * 验证所有定义的一致性
     */
    std::vector<std::string> validateConsistency() const;

private:
    // 符号存储 - 使用完全限定名称作为键
    std::unordered_map<std::string, std::unique_ptr<SymbolDefinition>> symbols_;
    
    // 命名空间存储
    std::unordered_map<std::string, std::unique_ptr<NamespaceDefinition>> namespaces_;
    
    // 导入存储
    std::unordered_map<std::string, std::unique_ptr<ImportDefinition>> imports_;
    
    // 配置存储
    std::unordered_map<std::string, std::string> configurations_;
    
    // 当前文件路径
    std::string currentFile_;
    
    // 私有方法
    
    /**
     * 构建完全限定名称
     */
    std::string buildFullyQualifiedName(const std::string& name, 
                                        const std::string& namespacePath) const;
    
    /**
     * 解析命名空间路径
     */
    std::vector<std::string> parseNamespacePath(const std::string& path) const;
    
    /**
     * 查找最佳匹配的符号
     */
    const SymbolDefinition* findBestMatch(const std::string& name, 
                                          const std::string& currentNamespace) const;
    
    /**
     * 合并继承的内容
     */
    std::string mergeInheritedContent(const SymbolDefinition& base, 
                                      const SymbolDefinition& derived) const;
    
    /**
     * 应用特例化
     */
    SymbolDefinition applySpecialization(const SymbolDefinition& symbol) const;
    
    /**
     * 验证符号名称的有效性
     */
    bool isValidSymbolName(const std::string& name) const;
    
    /**
     * 验证命名空间名称的有效性
     */
    bool isValidNamespaceName(const std::string& name) const;
};

/**
 * 符号工具类
 */
class SymbolUtils {
public:
    /**
     * 解析符号引用
     * 格式：[Custom] @Element Name 或 @Element Name 或 Name
     */
    static std::pair<SymbolType, std::string> parseSymbolReference(const std::string& reference);
    
    /**
     * 格式化符号引用
     */
    static std::string formatSymbolReference(SymbolType type, const std::string& name);
    
    /**
     * 检查符号类型兼容性
     */
    static bool areTypesCompatible(SymbolType sourceType, SymbolType targetType);
    
    /**
     * 获取符号类型的字符串表示
     */
    static std::string symbolTypeToString(SymbolType type);
    
    /**
     * 从字符串解析符号类型
     */
    static SymbolType symbolTypeFromString(const std::string& str);
    
    /**
     * 验证符号名称格式
     */
    static bool isValidSymbolName(const std::string& name);
    
    /**
     * 提取命名空间和名称
     */
    static std::pair<std::string, std::string> splitQualifiedName(const std::string& qualifiedName);
};

} // namespace chtl
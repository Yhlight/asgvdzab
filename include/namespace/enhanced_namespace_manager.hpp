#pragma once

#include "common/types.hpp"
#include "ast/chtl_ast.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <mutex>

namespace chtl {

/**
 * 命名空间符号类型
 */
enum class NamespaceSymbolType {
    TEMPLATE,       // [Template]
    CUSTOM,         // [Custom] 
    VARIABLE,       // 变量
    CONFIGURATION,  // [Configuration]
    IMPORT,         // [Import]
    NAMESPACE       // [Namespace]
};

/**
 * 符号定义
 */
struct NamespaceSymbolDefinition {
    std::string name;                   // 符号名称
    NamespaceSymbolType type;           // 符号类型
    std::string fullName;               // 完整名称(包含命名空间)
    std::string namespacePath;          // 命名空间路径
    Position position;                  // 定义位置
    std::string sourceFile;             // 源文件
    std::string content;                // 符号内容/定义
    bool isPublic;                      // 是否公开
    bool isExported;                    // 是否导出
    std::unordered_map<std::string, std::string> attributes; // 附加属性
    
    NamespaceSymbolDefinition() 
        : type(NamespaceSymbolType::TEMPLATE), isPublic(true), isExported(false) {}
};

/**
 * 命名空间定义
 */
struct NamespaceDefinition {
    std::string name;                   // 命名空间名称
    std::string fullPath;               // 完整路径
    std::string parentPath;             // 父命名空间路径
    Position position;                  // 定义位置
    std::string sourceFile;             // 源文件
    bool isMerged;                      // 是否为合并的命名空间
    std::vector<std::string> mergedFiles; // 合并的文件列表
    std::unordered_map<std::string, NamespaceSymbolDefinition> symbols; // 符号表
    std::unordered_set<std::string> childNamespaces; // 子命名空间
    
    NamespaceDefinition() : isMerged(false) {}
};

/**
 * 冲突类型
 */
enum class ConflictType {
    NONE,               // 无冲突
    NAME_COLLISION,     // 名称冲突
    TYPE_MISMATCH,      // 类型不匹配
    DUPLICATE_SYMBOL,   // 重复符号
    VISIBILITY_CONFLICT, // 可见性冲突
    EXPORT_CONFLICT     // 导出冲突
};

/**
 * 冲突信息
 */
struct ConflictInfo {
    ConflictType type;                  // 冲突类型
    std::string description;            // 冲突描述
    std::string symbolName;             // 冲突的符号名
    std::string namespacePath;          // 命名空间路径
    Position position1;                 // 第一个定义位置
    Position position2;                 // 第二个定义位置
    std::string sourceFile1;            // 第一个源文件
    std::string sourceFile2;            // 第二个源文件
    std::string suggestion;             // 解决建议
    
    ConflictInfo() : type(ConflictType::NONE) {}
};

/**
 * 命名空间解析结果
 */
struct NamespaceResolutionResult {
    bool success;                       // 是否成功
    std::string errorMessage;           // 错误信息
    std::vector<ConflictInfo> conflicts; // 冲突列表
    std::vector<NamespaceSymbolDefinition> resolvedSymbols; // 解析的符号
    
    NamespaceResolutionResult() : success(false) {}
};

/**
 * 增强的命名空间管理器
 * 实现同名命名空间合并、冲突检测策略
 */
class EnhancedNamespaceManager {
public:
    EnhancedNamespaceManager();
    ~EnhancedNamespaceManager();
    
    // ===== 命名空间管理 =====
    
    /**
     * 声明命名空间
     * @param namespacePath 命名空间路径 (e.g., "A.B.C")
     * @param position 声明位置
     * @param sourceFile 源文件
     * @return 是否成功
     */
    bool declareNamespace(const std::string& namespacePath, const Position& position, const std::string& sourceFile);
    
    /**
     * 进入命名空间
     * @param namespacePath 命名空间路径
     * @return 是否成功
     */
    bool enterNamespace(const std::string& namespacePath);
    
    /**
     * 退出当前命名空间
     * @return 是否成功
     */
    bool exitNamespace();
    
    /**
     * 获取当前命名空间路径
     * @return 当前命名空间路径
     */
    std::string getCurrentNamespacePath() const;
    
    /**
     * 检查命名空间是否存在
     * @param namespacePath 命名空间路径
     * @return 是否存在
     */
    bool namespaceExists(const std::string& namespacePath) const;
    
    /**
     * 获取命名空间定义
     * @param namespacePath 命名空间路径
     * @return 命名空间定义(nullptr如果不存在)
     */
    const NamespaceDefinition* getNamespaceDefinition(const std::string& namespacePath) const;
    
    // ===== 符号管理 =====
    
    /**
     * 声明符号
     * @param symbolName 符号名称
     * @param symbolType 符号类型
     * @param position 定义位置
     * @param sourceFile 源文件
     * @param content 符号内容
     * @param isPublic 是否公开
     * @param isExported 是否导出
     * @return 解析结果
     */
    NamespaceResolutionResult declareSymbol(const std::string& symbolName, 
                                           NamespaceSymbolType symbolType,
                                           const Position& position,
                                           const std::string& sourceFile,
                                           const std::string& content = "",
                                           bool isPublic = true,
                                           bool isExported = false);
    
    /**
     * 查找符号
     * @param symbolName 符号名称
     * @param namespacePath 命名空间路径(空表示当前命名空间)
     * @return 符号定义(nullptr如果未找到)
     */
    const NamespaceSymbolDefinition* findSymbol(const std::string& symbolName, 
                                               const std::string& namespacePath = "") const;
    
    /**
     * 查找符号(完整名称)
     * @param fullName 完整符号名称 (e.g., "A.B.SymbolName")
     * @return 符号定义(nullptr如果未找到)
     */
    const NamespaceSymbolDefinition* findSymbolByFullName(const std::string& fullName) const;
    
    /**
     * 获取当前命名空间的所有符号
     * @param includePrivate 是否包含私有符号
     * @return 符号列表
     */
    std::vector<const NamespaceSymbolDefinition*> getCurrentNamespaceSymbols(bool includePrivate = false) const;
    
    /**
     * 获取指定命名空间的所有符号
     * @param namespacePath 命名空间路径
     * @param includePrivate 是否包含私有符号
     * @return 符号列表
     */
    std::vector<const NamespaceSymbolDefinition*> getNamespaceSymbols(const std::string& namespacePath, 
                                                                     bool includePrivate = false) const;
    
    // ===== 命名空间合并 =====
    
    /**
     * 合并命名空间
     * @param namespacePath 要合并的命名空间路径
     * @param sourceFile 合并来源文件
     * @return 合并结果
     */
    NamespaceResolutionResult mergeNamespace(const std::string& namespacePath, const std::string& sourceFile);
    
    /**
     * 检查命名空间是否可以合并
     * @param namespacePath1 命名空间路径1
     * @param namespacePath2 命名空间路径2
     * @return 冲突列表(空表示可以合并)
     */
    std::vector<ConflictInfo> checkMergeConflicts(const std::string& namespacePath1, 
                                                  const std::string& namespacePath2) const;
    
    // ===== 冲突检测 =====
    
    /**
     * 检测符号冲突
     * @param symbolDef 符号定义
     * @param namespacePath 命名空间路径
     * @return 冲突信息列表
     */
    std::vector<ConflictInfo> detectSymbolConflicts(const NamespaceSymbolDefinition& symbolDef,
                                                    const std::string& namespacePath) const;
    
    /**
     * 检测命名空间冲突
     * @param namespacePath 命名空间路径
     * @param position 位置
     * @param sourceFile 源文件
     * @return 冲突信息列表
     */
    std::vector<ConflictInfo> detectNamespaceConflicts(const std::string& namespacePath,
                                                       const Position& position,
                                                       const std::string& sourceFile) const;
    
    /**
     * 解决冲突
     * @param conflicts 冲突列表
     * @param strategy 解决策略
     * @return 是否成功解决
     */
    bool resolveConflicts(const std::vector<ConflictInfo>& conflicts, const std::string& strategy = "merge");
    
    // ===== 命名空间查询 =====
    
    /**
     * 获取所有命名空间
     * @return 命名空间路径列表
     */
    std::vector<std::string> getAllNamespaces() const;
    
    /**
     * 获取子命名空间
     * @param parentPath 父命名空间路径
     * @return 子命名空间列表
     */
    std::vector<std::string> getChildNamespaces(const std::string& parentPath) const;
    
    /**
     * 获取命名空间层次结构
     * @return 层次结构字符串表示
     */
    std::string getNamespaceHierarchy() const;
    
    /**
     * 搜索符号
     * @param pattern 搜索模式
     * @param symbolType 符号类型过滤(TEMPLATE表示所有类型)
     * @param namespacePath 命名空间过滤(空表示所有命名空间)
     * @return 匹配的符号列表
     */
    std::vector<const NamespaceSymbolDefinition*> searchSymbols(const std::string& pattern,
                                                               NamespaceSymbolType symbolType = NamespaceSymbolType::TEMPLATE,
                                                               const std::string& namespacePath = "") const;
    
    // ===== 导入和导出 =====
    
    /**
     * 导入命名空间
     * @param namespacePath 要导入的命名空间路径
     * @param alias 别名(可选)
     * @param selectiveSymbols 选择性导入的符号(空表示导入所有)
     * @return 导入结果
     */
    NamespaceResolutionResult importNamespace(const std::string& namespacePath,
                                             const std::string& alias = "",
                                             const std::vector<std::string>& selectiveSymbols = {});
    
    /**
     * 导出符号
     * @param symbolName 符号名称
     * @param namespacePath 命名空间路径(空表示当前命名空间)
     * @return 是否成功
     */
    bool exportSymbol(const std::string& symbolName, const std::string& namespacePath = "");
    
    /**
     * 获取导出的符号
     * @param namespacePath 命名空间路径
     * @return 导出符号列表
     */
    std::vector<const NamespaceSymbolDefinition*> getExportedSymbols(const std::string& namespacePath) const;
    
    // ===== 实用工具 =====
    
    /**
     * 解析命名空间路径
     * @param fullPath 完整路径 (e.g., "A.B.C")
     * @return 路径组件列表
     */
    static std::vector<std::string> parseNamespacePath(const std::string& fullPath);
    
    /**
     * 构建命名空间路径
     * @param components 路径组件
     * @return 完整路径
     */
    static std::string buildNamespacePath(const std::vector<std::string>& components);
    
    /**
     * 获取父命名空间路径
     * @param namespacePath 命名空间路径
     * @return 父路径(空表示根命名空间)
     */
    static std::string getParentNamespacePath(const std::string& namespacePath);
    
    /**
     * 获取命名空间名称(不含路径)
     * @param namespacePath 命名空间路径
     * @return 名称
     */
    static std::string getNamespaceName(const std::string& namespacePath);
    
    /**
     * 检查是否为有效的命名空间名称
     * @param name 名称
     * @return 是否有效
     */
    static bool isValidNamespaceName(const std::string& name);
    
    /**
     * 检查是否为有效的符号名称
     * @param name 名称
     * @return 是否有效
     */
    static bool isValidSymbolName(const std::string& name);
    
    // ===== 调试和统计 =====
    
    /**
     * 获取命名空间统计信息
     * @return 统计信息字符串
     */
    std::string getNamespaceStatistics() const;
    
    /**
     * 转储命名空间信息
     * @return 完整命名空间信息
     */
    std::string dumpNamespaces() const;
    
    /**
     * 转储符号表
     * @param namespacePath 命名空间路径(空表示所有)
     * @return 符号表信息
     */
    std::string dumpSymbolTable(const std::string& namespacePath = "") const;
    
    /**
     * 清空所有命名空间和符号
     */
    void clear();

private:
    std::unordered_map<std::string, NamespaceDefinition> namespaces_;   // 命名空间映射
    std::vector<std::string> namespaceStack_;                          // 命名空间栈
    std::unordered_map<std::string, std::string> importAliases_;       // 导入别名映射
    mutable std::mutex mutex_;                                          // 线程安全互斥锁
    
    // 内部工具方法
    std::string getCurrentNamespacePathInternal() const;
    NamespaceDefinition& getOrCreateNamespace(const std::string& namespacePath, 
                                              const Position& position, 
                                              const std::string& sourceFile);
    void createParentNamespaces(const std::string& namespacePath, 
                               const Position& position, 
                               const std::string& sourceFile);
    ConflictInfo createConflictInfo(ConflictType type, const std::string& description,
                                   const NamespaceSymbolDefinition& symbol1,
                                   const NamespaceSymbolDefinition& symbol2) const;
    bool canMergeSymbols(const NamespaceSymbolDefinition& symbol1,
                        const NamespaceSymbolDefinition& symbol2) const;
    std::string generateFullSymbolName(const std::string& symbolName, const std::string& namespacePath) const;
    void buildHierarchyRecursive(const std::string& namespacePath, int depth, std::stringstream& ss) const;
};

} // namespace chtl
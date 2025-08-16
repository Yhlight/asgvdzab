#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

namespace chtl {

/**
 * 命名空间元素类型
 */
enum class NamespaceElementType {
    TEMPLATE_STYLE,     // [Template] @Style
    TEMPLATE_ELEMENT,   // [Template] @Element
    TEMPLATE_VAR,       // [Template] @Var
    CUSTOM_STYLE,       // [Custom] @Style
    CUSTOM_ELEMENT,     // [Custom] @Element
    CUSTOM_VAR,         // [Custom] @Var
    VARIABLE,           // 普通变量
    FUNCTION,           // 函数
    NESTED_NAMESPACE    // 嵌套命名空间
};

/**
 * 命名空间元素
 */
struct NamespaceElement {
    std::string name;                   // 元素名称
    NamespaceElementType type;          // 元素类型
    std::string content;                // 元素内容/定义
    std::string sourceFile;             // 源文件路径
    int sourceLineNumber;               // 源文件行号
    std::unordered_map<std::string, std::string> metadata; // 元数据
    
    NamespaceElement() : type(NamespaceElementType::VARIABLE), sourceLineNumber(-1) {}
};

/**
 * 命名空间冲突信息
 */
struct NamespaceConflict {
    std::string namespaceName;          // 命名空间名称
    std::string elementName;            // 冲突元素名称
    std::vector<NamespaceElement> conflictingElements; // 冲突的元素列表
    std::string conflictType;           // 冲突类型描述
    std::string resolutionSuggestion;   // 解决建议
};

/**
 * 命名空间合并策略
 */
enum class MergeStrategy {
    FIRST_WINS,         // 第一个定义优先
    LAST_WINS,          // 最后一个定义优先
    EXPLICIT_PRIORITY,  // 明确优先级
    FAIL_ON_CONFLICT,   // 冲突时失败
    INTERACTIVE_RESOLVE // 交互式解决
};

/**
 * 命名空间合并配置
 */
struct NamespaceMergeConfig {
    MergeStrategy defaultStrategy;      // 默认合并策略
    std::unordered_map<NamespaceElementType, MergeStrategy> typeStrategies; // 类型特定策略
    std::unordered_map<std::string, int> filePriorities;    // 文件优先级
    bool allowNestedMerging;            // 允许嵌套合并
    bool strictTypeChecking;            // 严格类型检查
    
    NamespaceMergeConfig() 
        : defaultStrategy(MergeStrategy::FAIL_ON_CONFLICT)
        , allowNestedMerging(true)
        , strictTypeChecking(true) {}
};

/**
 * 命名空间定义
 */
class NamespaceDefinition {
public:
    NamespaceDefinition(const std::string& name);
    ~NamespaceDefinition();
    
    /**
     * 获取命名空间名称
     * @return 命名空间名称
     */
    const std::string& getName() const { return name_; }
    
    /**
     * 添加元素
     * @param element 命名空间元素
     * @return 是否成功添加
     */
    bool addElement(const NamespaceElement& element);
    
    /**
     * 移除元素
     * @param elementName 元素名称
     * @param type 元素类型
     * @return 是否成功移除
     */
    bool removeElement(const std::string& elementName, NamespaceElementType type);
    
    /**
     * 获取元素
     * @param elementName 元素名称
     * @param type 元素类型
     * @return 元素指针，未找到返回nullptr
     */
    const NamespaceElement* getElement(const std::string& elementName, NamespaceElementType type) const;
    
    /**
     * 检查元素是否存在
     * @param elementName 元素名称
     * @param type 元素类型
     * @return 是否存在
     */
    bool hasElement(const std::string& elementName, NamespaceElementType type) const;
    
    /**
     * 获取所有元素
     * @return 元素列表
     */
    std::vector<NamespaceElement> getAllElements() const;
    
    /**
     * 获取指定类型的所有元素
     * @param type 元素类型
     * @return 元素列表
     */
    std::vector<NamespaceElement> getElementsByType(NamespaceElementType type) const;
    
    /**
     * 添加嵌套命名空间
     * @param childNamespace 子命名空间
     * @return 是否成功添加
     */
    bool addNestedNamespace(std::shared_ptr<NamespaceDefinition> childNamespace);
    
    /**
     * 获取嵌套命名空间
     * @param name 命名空间名称
     * @return 命名空间指针，未找到返回nullptr
     */
    std::shared_ptr<NamespaceDefinition> getNestedNamespace(const std::string& name) const;
    
    /**
     * 获取所有嵌套命名空间
     * @return 嵌套命名空间列表
     */
    std::vector<std::shared_ptr<NamespaceDefinition>> getNestedNamespaces() const;
    
    /**
     * 检查是否与另一个命名空间冲突
     * @param other 另一个命名空间
     * @return 冲突列表
     */
    std::vector<NamespaceConflict> checkConflicts(const NamespaceDefinition& other) const;
    
    /**
     * 获取元素统计信息
     * @return 统计信息字符串
     */
    std::string getStatistics() const;

private:
    std::string name_;
    std::unordered_map<std::string, std::unordered_map<NamespaceElementType, NamespaceElement>> elements_;
    std::unordered_map<std::string, std::shared_ptr<NamespaceDefinition>> nestedNamespaces_;
    
    std::string generateElementKey(const std::string& name, NamespaceElementType type) const;
};

/**
 * 命名空间合并器
 */
class NamespaceMerger {
public:
    explicit NamespaceMerger(const NamespaceMergeConfig& config = NamespaceMergeConfig());
    ~NamespaceMerger();
    
    /**
     * 设置合并配置
     * @param config 合并配置
     */
    void setMergeConfig(const NamespaceMergeConfig& config);
    
    /**
     * 获取合并配置
     * @return 合并配置
     */
    const NamespaceMergeConfig& getMergeConfig() const;
    
    /**
     * 合并两个命名空间
     * @param target 目标命名空间（会被修改）
     * @param source 源命名空间
     * @return 合并是否成功
     */
    bool mergeNamespaces(NamespaceDefinition& target, const NamespaceDefinition& source);
    
    /**
     * 合并多个同名命名空间
     * @param namespaces 命名空间列表
     * @return 合并后的命名空间，失败返回nullptr
     */
    std::shared_ptr<NamespaceDefinition> mergeMultipleNamespaces(
        const std::vector<std::shared_ptr<NamespaceDefinition>>& namespaces);
    
    /**
     * 检测命名空间冲突
     * @param namespaces 命名空间列表
     * @return 冲突列表
     */
    std::vector<NamespaceConflict> detectConflicts(
        const std::vector<std::shared_ptr<NamespaceDefinition>>& namespaces);
    
    /**
     * 解决命名空间冲突
     * @param conflicts 冲突列表
     * @param resolution 解决方案映射
     * @return 是否成功解决所有冲突
     */
    bool resolveConflicts(const std::vector<NamespaceConflict>& conflicts,
                         const std::unordered_map<std::string, std::string>& resolution);
    
    /**
     * 获取最后的合并统计信息
     * @return 统计信息
     */
    std::string getMergeStatistics() const;
    
    /**
     * 清空合并历史
     */
    void clearMergeHistory();

private:
    NamespaceMergeConfig config_;
    std::vector<NamespaceConflict> lastConflicts_;
    int mergedNamespaces_;
    int resolvedConflicts_;
    int failedMerges_;
    
    /**
     * 合并单个元素
     * @param target 目标命名空间
     * @param element 要合并的元素
     * @return 是否成功合并
     */
    bool mergeElement(NamespaceDefinition& target, const NamespaceElement& element);
    
    /**
     * 解决元素冲突
     * @param existing 现有元素
     * @param newElement 新元素
     * @param strategy 合并策略
     * @return 选择的元素（nullptr表示合并失败）
     */
    std::unique_ptr<NamespaceElement> resolveElementConflict(
        const NamespaceElement& existing, 
        const NamespaceElement& newElement,
        MergeStrategy strategy);
    
    /**
     * 获取元素的合并策略
     * @param type 元素类型
     * @return 合并策略
     */
    MergeStrategy getStrategyForType(NamespaceElementType type) const;
    
    /**
     * 比较元素优先级
     * @param elem1 元素1
     * @param elem2 元素2
     * @return elem1是否优先于elem2
     */
    bool hasHigherPriority(const NamespaceElement& elem1, const NamespaceElement& elem2) const;
    
    /**
     * 检查元素兼容性
     * @param elem1 元素1
     * @param elem2 元素2
     * @return 是否兼容
     */
    bool areElementsCompatible(const NamespaceElement& elem1, const NamespaceElement& elem2) const;
};

/**
 * 命名空间管理器
 * 统一管理所有命名空间的创建、合并和查找
 */
class NamespaceManager {
public:
    NamespaceManager();
    ~NamespaceManager();
    
    /**
     * 注册命名空间
     * @param namespaceDef 命名空间定义
     * @return 是否成功注册
     */
    bool registerNamespace(std::shared_ptr<NamespaceDefinition> namespaceDef);
    
    /**
     * 获取命名空间
     * @param namespacePath 命名空间路径（支持嵌套，如"ns1.ns2"）
     * @return 命名空间指针，未找到返回nullptr
     */
    std::shared_ptr<NamespaceDefinition> getNamespace(const std::string& namespacePath);
    
    /**
     * 创建命名空间
     * @param namespacePath 命名空间路径
     * @return 创建的命名空间
     */
    std::shared_ptr<NamespaceDefinition> createNamespace(const std::string& namespacePath);
    
    /**
     * 删除命名空间
     * @param namespacePath 命名空间路径
     * @return 是否成功删除
     */
    bool removeNamespace(const std::string& namespacePath);
    
    /**
     * 执行命名空间合并
     * @param config 合并配置
     * @return 合并是否成功
     */
    bool performNamespaceMerging(const NamespaceMergeConfig& config = NamespaceMergeConfig());
    
    /**
     * 查找元素
     * @param namespacePath 命名空间路径
     * @param elementName 元素名称
     * @param type 元素类型
     * @return 元素指针，未找到返回nullptr
     */
    const NamespaceElement* findElement(const std::string& namespacePath, 
                                       const std::string& elementName, 
                                       NamespaceElementType type);
    
    /**
     * 获取所有命名空间
     * @return 命名空间列表
     */
    std::vector<std::shared_ptr<NamespaceDefinition>> getAllNamespaces() const;
    
    /**
     * 获取同名命名空间列表
     * @param name 命名空间名称
     * @return 同名命名空间列表
     */
    std::vector<std::shared_ptr<NamespaceDefinition>> getNamespacesByName(const std::string& name) const;
    
    /**
     * 导出命名空间统计
     * @return 统计报告
     */
    std::string exportStatistics() const;
    
    /**
     * 清空所有命名空间
     */
    void clear();

private:
    std::unordered_map<std::string, std::vector<std::shared_ptr<NamespaceDefinition>>> namespacesByName_;
    std::unordered_map<std::string, std::shared_ptr<NamespaceDefinition>> namespacesByPath_;
    NamespaceMerger merger_;
    
    /**
     * 解析命名空间路径
     * @param path 命名空间路径
     * @return 路径组件列表
     */
    std::vector<std::string> parseNamespacePath(const std::string& path) const;
    
    /**
     * 构建命名空间路径
     * @param components 路径组件
     * @return 完整路径
     */
    std::string buildNamespacePath(const std::vector<std::string>& components) const;
    
    /**
     * 更新命名空间索引
     * @param namespaceDef 命名空间定义
     */
    void updateNamespaceIndex(std::shared_ptr<NamespaceDefinition> namespaceDef);
};

/**
 * 命名空间工具函数
 */
namespace NamespaceUtils {
    /**
     * 获取元素类型字符串
     * @param type 元素类型
     * @return 类型字符串
     */
    std::string getElementTypeString(NamespaceElementType type);
    
    /**
     * 解析元素类型字符串
     * @param typeString 类型字符串
     * @return 元素类型
     */
    NamespaceElementType parseElementType(const std::string& typeString);
    
    /**
     * 获取合并策略字符串
     * @param strategy 合并策略
     * @return 策略字符串
     */
    std::string getMergeStrategyString(MergeStrategy strategy);
    
    /**
     * 解析合并策略字符串
     * @param strategyString 策略字符串
     * @return 合并策略
     */
    MergeStrategy parseMergeStrategy(const std::string& strategyString);
    
    /**
     * 生成冲突报告
     * @param conflicts 冲突列表
     * @return 冲突报告字符串
     */
    std::string generateConflictReport(const std::vector<NamespaceConflict>& conflicts);
    
    /**
     * 验证命名空间名称
     * @param name 命名空间名称
     * @return 是否有效
     */
    bool isValidNamespaceName(const std::string& name);
    
    /**
     * 验证元素名称
     * @param name 元素名称
     * @return 是否有效
     */
    bool isValidElementName(const std::string& name);
}

} // namespace chtl
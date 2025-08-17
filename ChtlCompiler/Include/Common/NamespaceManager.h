#ifndef CHTL_COMMON_NAMESPACEMANAGER_H
#define CHTL_COMMON_NAMESPACEMANAGER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "AST/ASTNode.h"
#include "AST/ConfigNodes.h"
#include "Common/GlobalMap.h"

namespace Chtl {

// 命名空间内容
struct NamespaceContent {
    // 模板定义
    std::unordered_map<std::string, StyleGroup> styleTemplates;
    std::unordered_map<std::string, ElementTemplate> elementTemplates;
    std::unordered_map<std::string, VarGroup> varTemplates;
    
    // 自定义定义
    std::unordered_map<std::string, CustomStyle> customStyles;
    std::unordered_map<std::string, CustomElement> customElements;
    std::unordered_map<std::string, VarGroup> customVars;
    
    // 导出项
    std::unordered_set<std::string> exportedItems;
    
    // 源文件信息（用于冲突报告）
    std::string sourceFile;
    size_t startLine;
    size_t endLine;
};

// 冲突信息
struct ConflictInfo {
    std::string itemName;
    std::string itemType;  // "style template", "element template", etc.
    std::string namespace1;
    std::string file1;
    size_t line1;
    std::string namespace2;
    std::string file2;
    size_t line2;
};

class NamespaceManager {
public:
    NamespaceManager();
    ~NamespaceManager();
    
    // 注册命名空间
    void registerNamespace(const std::string& name, const NamespaceContent& content);
    
    // 合并同名命名空间
    bool mergeNamespace(const std::string& name, const NamespaceContent& content);
    
    // 检查冲突
    std::vector<ConflictInfo> checkConflicts(const std::string& name, const NamespaceContent& content);
    
    // 获取命名空间内容
    const NamespaceContent* getNamespace(const std::string& name) const;
    
    // 获取嵌套命名空间内容（如 "space.room"）
    const NamespaceContent* getNestedNamespace(const std::string& path) const;
    
    // 解析命名空间路径（如 "space.room.box" -> ["space", "room", "box"]）
    std::vector<std::string> parseNamespacePath(const std::string& path) const;
    
    // 从AST节点创建命名空间内容
    NamespaceContent createFromASTNode(NamespaceBlockNode* node, const std::string& sourceFile);
    
    // 将命名空间内容添加到全局符号表
    void addToGlobalMap(const std::string& namespaceName, GlobalMap& globalMap);
    
    // 检查项目是否已存在于命名空间中
    bool itemExists(const NamespaceContent& content, const std::string& itemType, const std::string& itemName) const;
    
    // 获取所有命名空间名称
    std::vector<std::string> getAllNamespaces() const;
    
    // 清空所有命名空间
    void clear();
    
    // 调试输出
    void dump() const;
    
private:
    // 命名空间存储：命名空间名 -> 内容列表（同名命名空间可能有多个）
    std::unordered_map<std::string, std::vector<NamespaceContent>> namespaces_;
    
    // 已合并的命名空间
    std::unordered_map<std::string, NamespaceContent> mergedNamespaces_;
    
    // 嵌套命名空间缓存
    mutable std::unordered_map<std::string, NamespaceContent> nestedCache_;
    
    // 内部辅助方法
    bool checkItemConflict(const NamespaceContent& content1, const NamespaceContent& content2,
                          const std::string& itemType, const std::string& itemName,
                          std::vector<ConflictInfo>& conflicts);
    
    void mergeStyleTemplates(std::unordered_map<std::string, StyleGroup>& target,
                            const std::unordered_map<std::string, StyleGroup>& source);
    
    void mergeElementTemplates(std::unordered_map<std::string, ElementTemplate>& target,
                              const std::unordered_map<std::string, ElementTemplate>& source);
    
    void mergeCustomStyles(std::unordered_map<std::string, CustomStyle>& target,
                          const std::unordered_map<std::string, CustomStyle>& source);
    
    void mergeCustomElements(std::unordered_map<std::string, CustomElement>& target,
                            const std::unordered_map<std::string, CustomElement>& source);
    
    void mergeVars(std::unordered_map<std::string, VarGroup>& target,
                  const std::unordered_map<std::string, VarGroup>& source);
    
    NamespaceContent mergeContents(const std::vector<NamespaceContent>& contents);
    
    void buildNestedNamespace(const std::vector<std::string>& path, 
                             NamespaceContent& parent,
                             size_t depth = 0);
};

} // namespace Chtl

#endif // CHTL_COMMON_NAMESPACEMANAGER_H
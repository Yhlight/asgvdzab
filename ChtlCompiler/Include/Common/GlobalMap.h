#ifndef CHTL_COMMON_GLOBALMAP_H
#define CHTL_COMMON_GLOBALMAP_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <optional>

namespace Chtl {

// 样式组定义（用于@Style模板和自定义）
struct StyleGroup {
    std::string name;
    std::unordered_map<std::string, std::string> properties;  // 属性名 -> 值
    std::vector<std::string> inherits;                        // 继承的样式组
    std::vector<std::string> noValueProperties;               // 无值属性（自定义特性）
    bool isTemplate;                                          // 是否是模板
    std::string namespace_;                                   // 所属命名空间
};

// 元素定义（用于@Element模板和自定义）
struct ElementGroup {
    std::string name;
    std::string content;                                      // 元素内容（原始CHTL代码）
    std::vector<std::string> inherits;                        // 继承的元素
    bool isTemplate;                                          // 是否是模板
    std::string namespace_;                                   // 所属命名空间
};

// 变量组定义（用于@Var模板和自定义）
struct VarGroup {
    std::string name;
    std::unordered_map<std::string, std::string> variables;   // 变量名 -> 值
    std::vector<std::string> inherits;                        // 继承的变量组
    bool isTemplate;                                          // 是否是模板
    std::string namespace_;                                   // 所属命名空间
};

// 原始嵌入定义（用于[Origin]）
struct OriginBlock {
    std::string name;                                         // 可选名称
    std::string type;                                         // @Html, @Style, @JavaScript或自定义
    std::string content;                                      // 原始内容
    std::string namespace_;                                   // 所属命名空间
};

// 模块导入信息
struct ModuleImportInfo {
    std::string type;                                         // @Html, @Style, @JavaScript, @Chtl, @CJmod
    std::string path;                                         // 文件路径
    std::string alias;                                        // as别名（可选）
    std::string targetName;                                   // 导入的具体项名称（可选）
    std::string targetType;                                   // [Custom]/@Style等（可选）
};

// 配置信息
struct Configuration {
    // 基本配置
    int indexInitialCount = 0;
    bool disableNameGroup = false;
    bool disableCustomOriginType = false;
    bool debugMode = false;
    
    // Name配置
    std::unordered_map<std::string, std::vector<std::string>> nameGroups;
    int optionCount = 3;
    
    // 自定义Origin类型
    std::unordered_map<std::string, std::string> originTypes;
};

// 全局符号表
class GlobalMap {
public:
    GlobalMap();
    ~GlobalMap();
    
    // 样式组管理
    bool addStyleGroup(const StyleGroup& style);
    std::optional<StyleGroup> getStyleGroup(const std::string& name, 
                                           const std::string& namespace_ = "") const;
    
    // 元素管理
    bool addElementGroup(const ElementGroup& element);
    std::optional<ElementGroup> getElementGroup(const std::string& name,
                                               const std::string& namespace_ = "") const;
    
    // 变量组管理
    bool addVarGroup(const VarGroup& var);
    std::optional<VarGroup> getVarGroup(const std::string& name,
                                        const std::string& namespace_ = "") const;
    
    // 原始嵌入管理
    bool addOriginBlock(const OriginBlock& origin);
    std::optional<OriginBlock> getOriginBlock(const std::string& name,
                                             const std::string& type = "") const;
    
    // 导入管理
    void addImport(const ImportInfo& import);
    std::vector<ImportInfo> getImports() const { return imports_; }
    
    // 命名空间管理
    void enterNamespace(const std::string& name);
    void exitNamespace();
    std::string getCurrentNamespace() const;
    
    // 配置管理
    void setConfiguration(const Configuration& config) { config_ = config; }
    Configuration& getConfiguration() { return config_; }
    const Configuration& getConfiguration() const { return config_; }
    
    // 查找完全限定名
    std::string resolveFullName(const std::string& name, 
                               const std::string& namespace_ = "") const;
    
    // 清空所有符号
    void clear();
    
    // 调试输出
    void dump() const;

private:
    // 内部存储 - 使用完全限定名作为key
    std::unordered_map<std::string, StyleGroup> styleGroups_;
    std::unordered_map<std::string, ElementGroup> elementGroups_;
    std::unordered_map<std::string, VarGroup> varGroups_;
    std::unordered_map<std::string, OriginBlock> originBlocks_;
    
    // 导入列表
    std::vector<ImportInfo> imports_;
    
    // 当前命名空间栈
    std::vector<std::string> namespaceStack_;
    
    // 配置
    Configuration config_;
    
    // 构建完全限定名
    std::string buildFullName(const std::string& name, 
                             const std::string& namespace_) const;
};

} // namespace Chtl

#endif // CHTL_COMMON_GLOBALMAP_H
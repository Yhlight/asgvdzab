#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <filesystem>

namespace chtl {

/**
 * Import类型枚举
 */
enum class ImportType {
    HTML_FILE,           // @Html
    CSS_FILE,            // @Style  
    JAVASCRIPT_FILE,     // @JavaScript
    CHTL_FILE,           // @Chtl
    CJMOD_FILE,          // @CJmod
    CUSTOM_ELEMENT,      // [Custom] @Element
    CUSTOM_STYLE,        // [Custom] @Style
    CUSTOM_VAR,          // [Custom] @Var
    TEMPLATE_ELEMENT,    // [Template] @Element
    TEMPLATE_STYLE,      // [Template] @Style
    TEMPLATE_VAR         // [Template] @Var
};

/**
 * Import项结构
 */
struct ImportItem {
    ImportType type;
    std::string name;           // 导入的对象名称（对于精确导入）
    std::string path;           // 文件路径
    std::string alias;          // 别名（可选）
    std::string namespaceName; // 命名空间名称（可选）
    bool isResolved = false;    // 是否已解析
    std::string content;        // 导入的内容
};

/**
 * CMOD模块结构
 */
struct CMODModule {
    std::string name;
    std::string rootPath;
    std::unordered_map<std::string, std::string> subModules; // 子模块名 -> 路径
    std::unordered_map<std::string, std::string> templates;  // 模板名 -> 内容
    std::unordered_map<std::string, std::string> customs;    // 自定义名 -> 内容
};

/**
 * CJMOD模块结构
 */
struct CJMODModule {
    std::string name;
    std::string rootPath;
    std::vector<std::string> sourceFiles;      // C++源文件
    std::vector<std::string> headerFiles;      // C++头文件
    std::string interfaceFile;                 // 接口描述文件
    void* dynamicLibrary = nullptr;            // 动态库句柄
    bool isLoaded = false;                     // 是否已加载
};

/**
 * 增强Import管理器
 * 负责解析、验证和管理所有类型的导入
 */
class EnhancedImportManager {
public:
    EnhancedImportManager();
    ~EnhancedImportManager();

    /**
     * 解析Import语句
     * @param importStatement Import语句字符串
     * @return 解析是否成功
     */
    bool parseImportStatement(const std::string& importStatement);

    /**
     * 解析和处理Import块
     * @param content 包含Import语句的代码内容
     * @return 处理后的代码（移除Import语句）
     */
    std::string processImports(const std::string& content);

    /**
     * 解析所有导入项
     * @return 解析是否成功
     */
    bool resolveAllImports();

    /**
     * 获取导入的内容
     * @param name 导入项名称或别名
     * @return 导入的内容
     */
    std::string getImportedContent(const std::string& name) const;

    /**
     * 检查是否存在导入
     * @param name 导入项名称或别名
     * @return 是否存在
     */
    bool hasImport(const std::string& name) const;

    /**
     * 获取所有导入项
     * @return 导入项列表
     */
    const std::vector<ImportItem>& getImports() const { return imports_; }

    /**
     * 设置模块搜索路径
     * @param paths 搜索路径列表
     */
    void setModuleSearchPaths(const std::vector<std::string>& paths);

    /**
     * 清理所有导入
     */
    void clearImports();

    /**
     * 获取错误信息
     * @return 错误信息列表
     */
    const std::vector<std::string>& getErrors() const { return errors_; }

private:
    std::vector<ImportItem> imports_;
    std::vector<std::string> moduleSearchPaths_;
    std::unordered_map<std::string, CMODModule> cmodModules_;
    std::unordered_map<std::string, CJMODModule> cjmodModules_;
    std::vector<std::string> errors_;

    /**
     * 解析基本文件导入
     * @param type 导入类型
     * @param path 文件路径
     * @param alias 别名
     * @return 是否成功
     */
    bool parseBasicFileImport(ImportType type, const std::string& path, const std::string& alias = "");

    /**
     * 解析精确对象导入
     * @param category [Custom] 或 [Template]
     * @param type @Element/@Style/@Var
     * @param name 对象名称
     * @param path 文件路径
     * @param alias 别名
     * @return 是否成功
     */
    bool parsePreciseImport(const std::string& category, const std::string& type, 
                           const std::string& name, const std::string& path, const std::string& alias = "");

    /**
     * 解析CMOD模块
     * @param path 模块路径
     * @return 是否成功
     */
    bool parseCMODModule(const std::string& path);

    /**
     * 解析CJMOD模块
     * @param path 模块路径
     * @return 是否成功
     */
    bool parseCJMODModule(const std::string& path);

    /**
     * 解析文件内容
     * @param filePath 文件路径
     * @return 文件内容
     */
    std::string readFile(const std::string& filePath) const;

    /**
     * 解析路径（支持'.'表示'/'）
     * @param path 原始路径
     * @return 规范化路径
     */
    std::string normalizePath(const std::string& path) const;

    /**
     * 查找模块文件
     * @param moduleName 模块名称
     * @return 完整路径，如果未找到返回空字符串
     */
    std::string findModuleFile(const std::string& moduleName) const;

    /**
     * 验证文件存在性
     * @param path 文件路径
     * @return 是否存在
     */
    bool fileExists(const std::string& path) const;

    /**
     * 提取命名空间中的特定对象
     * @param content CHTL文件内容
     * @param objectType 对象类型（Custom/Template）
     * @param subType 子类型（Element/Style/Var）
     * @param objectName 对象名称
     * @return 对象定义内容
     */
    std::string extractNamedObject(const std::string& content, const std::string& objectType,
                                 const std::string& subType, const std::string& objectName) const;

    /**
     * 加载CJMOD动态库
     * @param module CJMOD模块
     * @return 是否成功
     */
    bool loadCJMODLibrary(CJMODModule& module);

    /**
     * 卸载CJMOD动态库
     * @param module CJMOD模块
     */
    void unloadCJMODLibrary(CJMODModule& module);

    /**
     * 添加错误信息
     * @param error 错误描述
     */
    void addError(const std::string& error);
};

/**
 * Import语句解析器
 * 专门用于解析各种复杂的Import语法
 */
class ImportStatementParser {
public:
    /**
     * 解析Import语句
     * @param statement Import语句
     * @return 解析结果
     */
    static ImportItem parseStatement(const std::string& statement);

private:
    /**
     * 解析基本Import语句
     * [Import] @Type from path as alias
     */
    static ImportItem parseBasicImport(const std::string& statement);

    /**
     * 解析精确Import语句
     * [Import] [Category] @Type name from path as alias
     */
    static ImportItem parsePreciseImport(const std::string& statement);

    /**
     * 确定Import类型
     * @param typeString 类型字符串（@Html/@Style等）
     * @return Import类型
     */
    static ImportType determineImportType(const std::string& typeString);

    /**
     * 确定精确Import类型
     * @param category 类别（Custom/Template）
     * @param type 类型（Element/Style/Var）
     * @return Import类型
     */
    static ImportType determinePreciseImportType(const std::string& category, const std::string& type);

    /**
     * 提取路径和别名
     * @param fromClause from子句
     * @return {路径, 别名}
     */
    static std::pair<std::string, std::string> extractPathAndAlias(const std::string& fromClause);
};

} // namespace chtl
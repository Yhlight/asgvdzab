#ifndef CHTL_COMMON_IMPORTRESOLVER_H
#define CHTL_COMMON_IMPORTRESOLVER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <filesystem>
#include "AST/ConfigNodes.h"

namespace Chtl {

// 导入信息结构
struct ImportInfo {
    std::string path;                // 导入路径
    std::string resolvedPath;        // 解析后的完整路径
    ImportStatementNode::ImportType type;  // 导入类型
    std::string alias;               // 别名（as语法）
    std::string targetName;          // 目标名称（用于特定导入）
    bool isCustom;                   // 是否是Custom导入
    bool isTemplate;                 // 是否是Template导入
};

// 模块搜索路径配置
struct ModuleSearchPaths {
    std::string officialModulePath;  // 官方模块路径 (编译器目录/module)
    std::string localModulePath;     // 本地模块路径 (当前目录/module)
    std::string currentPath;         // 当前文件所在目录
    std::string compilerPath;        // 编译器安装路径
};

class ImportResolver {
public:
    ImportResolver();
    ~ImportResolver();
    
    // 设置模块搜索路径
    void setModuleSearchPaths(const ModuleSearchPaths& paths);
    
    // 设置当前编译文件路径
    void setCurrentFilePath(const std::string& filePath);
    
    // 解析导入路径
    std::string resolveImportPath(const ImportInfo& importInfo);
    
    // 检查循环依赖
    bool checkCircularDependency(const std::string& fromPath, const std::string& toPath);
    
    // 记录导入关系
    void recordImport(const std::string& fromPath, const std::string& toPath);
    
    // 检查是否已导入（防止重复导入）
    bool isAlreadyImported(const std::string& fromPath, const std::string& importPath);
    
    // 路径规范化（处理同一路径不同表达方式）
    std::string normalizePath(const std::string& path);
    
    // 解析@Html/@Style/@JavaScript导入
    std::string resolveResourceImport(const std::string& path, ImportStatementNode::ImportType type);
    
    // 解析@Chtl导入
    std::string resolveChtlImport(const std::string& path);
    
    // 解析@CJmod导入
    std::string resolveCJmodImport(const std::string& path);
    
    // 解析通配符导入（如 path/* 或 path/*.chtl）
    std::vector<std::string> resolveWildcardImport(const std::string& pattern);
    
    // 检查是否是官方模块引用（chtl::前缀）
    bool isOfficialModuleReference(const std::string& path);
    
    // 提取官方模块名
    std::string extractOfficialModuleName(const std::string& path);
    
    // 获取文件扩展名
    std::string getFileExtension(const std::string& path);
    
    // 检查文件是否存在
    bool fileExists(const std::string& path);
    
    // 搜索文件（根据类型和名称）
    std::string searchFile(const std::string& name, const std::vector<std::string>& searchPaths, 
                          const std::vector<std::string>& extensions);
    
    // 获取依赖图（用于调试）
    const std::unordered_map<std::string, std::unordered_set<std::string>>& getDependencyGraph() const {
        return dependencyGraph_;
    }
    
    // 清空状态
    void clear();
    
private:
    ModuleSearchPaths searchPaths_;
    std::string currentFilePath_;
    std::string currentDirectory_;
    
    // 依赖图：文件路径 -> 它所依赖的文件集合
    std::unordered_map<std::string, std::unordered_set<std::string>> dependencyGraph_;
    
    // 当前正在处理的导入链（用于检测循环依赖）
    std::unordered_set<std::string> importChain_;
    
    // 已导入的文件集合（每个源文件对应其已导入的文件）
    std::unordered_map<std::string, std::unordered_set<std::string>> importedFiles_;
    
    // 内部辅助方法
    bool hasCircularDependencyDFS(const std::string& current, const std::string& target,
                                  std::unordered_set<std::string>& visited,
                                  std::unordered_set<std::string>& recursionStack);
    
    std::string joinPath(const std::string& base, const std::string& relative);
    bool isAbsolutePath(const std::string& path);
    bool hasFileExtension(const std::string& path);
    std::string removeFileExtension(const std::string& path);
    std::string getDirectory(const std::string& filePath);
    std::string getFileName(const std::string& filePath);
};

} // namespace Chtl

#endif // CHTL_COMMON_IMPORTRESOLVER_H
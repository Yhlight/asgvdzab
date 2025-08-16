#include "import/enhanced_import_manager.hpp"
#include <regex>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <dlfcn.h>  // 用于动态库加载

namespace chtl {

EnhancedImportManager::EnhancedImportManager() {
    // 设置默认模块搜索路径
    moduleSearchPaths_ = {
        "./module",
        "./modules", 
        "module",
        "modules",
        "../module",
        "../modules"
    };
}

EnhancedImportManager::~EnhancedImportManager() {
    // 卸载所有CJMOD动态库
    for (auto& [name, module] : cjmodModules_) {
        if (module.isLoaded) {
            unloadCJMODLibrary(module);
        }
    }
}

bool EnhancedImportManager::parseImportStatement(const std::string& importStatement) {
    try {
        ImportItem item = ImportStatementParser::parseStatement(importStatement);
        if (item.type != ImportType::HTML_FILE || !item.name.empty()) {
            imports_.push_back(item);
            return true;
        }
        return false;
    } catch (const std::exception& e) {
        addError("Import语句解析失败: " + std::string(e.what()));
        return false;
    }
}

std::string EnhancedImportManager::processImports(const std::string& content) {
    std::string result = content;
    std::regex importRegex(R"(\[Import\][^\n]+)");
    std::sregex_iterator iter(content.begin(), content.end(), importRegex);
    std::sregex_iterator end;
    
    std::vector<std::string> importStatements;
    for (; iter != end; ++iter) {
        std::smatch match = *iter;
        importStatements.push_back(match.str());
    }
    
    // 解析每个Import语句
    for (const auto& statement : importStatements) {
        parseImportStatement(statement);
        // 从结果中移除Import语句
        size_t pos = result.find(statement);
        if (pos != std::string::npos) {
            result.erase(pos, statement.length());
        }
    }
    
    return result;
}

bool EnhancedImportManager::resolveAllImports() {
    bool allResolved = true;
    
    for (auto& item : imports_) {
        if (item.isResolved) continue;
        
        try {
            switch (item.type) {
                case ImportType::HTML_FILE:
                case ImportType::CSS_FILE:
                case ImportType::JAVASCRIPT_FILE: {
                    std::string normalizedPath = normalizePath(item.path);
                    if (fileExists(normalizedPath)) {
                        item.content = readFile(normalizedPath);
                        item.isResolved = true;
                    } else {
                        addError("文件不存在: " + normalizedPath);
                        allResolved = false;
                    }
                    break;
                }
                
                case ImportType::CHTL_FILE: {
                    std::string modulePath = findModuleFile(item.path);
                    if (!modulePath.empty()) {
                        if (parseCMODModule(modulePath)) {
                            item.content = readFile(modulePath);
                            item.isResolved = true;
                        } else {
                            allResolved = false;
                        }
                    } else {
                        addError("CHTL模块未找到: " + item.path);
                        allResolved = false;
                    }
                    break;
                }
                
                case ImportType::CJMOD_FILE: {
                    std::string modulePath = findModuleFile(item.path);
                    if (!modulePath.empty()) {
                        if (parseCJMODModule(modulePath)) {
                            item.isResolved = true;
                        } else {
                            allResolved = false;
                        }
                    } else {
                        addError("CJMOD模块未找到: " + item.path);
                        allResolved = false;
                    }
                    break;
                }
                
                case ImportType::CUSTOM_ELEMENT:
                case ImportType::CUSTOM_STYLE:
                case ImportType::CUSTOM_VAR:
                case ImportType::TEMPLATE_ELEMENT:
                case ImportType::TEMPLATE_STYLE:
                case ImportType::TEMPLATE_VAR: {
                    std::string normalizedPath = normalizePath(item.path);
                    if (fileExists(normalizedPath)) {
                        std::string fileContent = readFile(normalizedPath);
                        std::string objectType = (item.type <= ImportType::CUSTOM_VAR) ? "Custom" : "Template";
                        std::string subType;
                        
                        switch (item.type) {
                            case ImportType::CUSTOM_ELEMENT:
                            case ImportType::TEMPLATE_ELEMENT:
                                subType = "Element";
                                break;
                            case ImportType::CUSTOM_STYLE:
                            case ImportType::TEMPLATE_STYLE:
                                subType = "Style";
                                break;
                            case ImportType::CUSTOM_VAR:
                            case ImportType::TEMPLATE_VAR:
                                subType = "Var";
                                break;
                            default:
                                break;
                        }
                        
                        item.content = extractNamedObject(fileContent, objectType, subType, item.name);
                        item.isResolved = !item.content.empty();
                        if (!item.isResolved) {
                            addError("未找到对象: " + objectType + " " + subType + " " + item.name);
                            allResolved = false;
                        }
                    } else {
                        addError("文件不存在: " + normalizedPath);
                        allResolved = false;
                    }
                    break;
                }
            }
        } catch (const std::exception& e) {
            addError("解析导入项失败: " + std::string(e.what()));
            allResolved = false;
        }
    }
    
    return allResolved;
}

std::string EnhancedImportManager::getImportedContent(const std::string& name) const {
    for (const auto& item : imports_) {
        if ((item.alias.empty() ? item.name : item.alias) == name && item.isResolved) {
            return item.content;
        }
    }
    return "";
}

bool EnhancedImportManager::hasImport(const std::string& name) const {
    for (const auto& item : imports_) {
        if ((item.alias.empty() ? item.name : item.alias) == name) {
            return true;
        }
    }
    return false;
}

void EnhancedImportManager::setModuleSearchPaths(const std::vector<std::string>& paths) {
    moduleSearchPaths_ = paths;
}

void EnhancedImportManager::clearImports() {
    imports_.clear();
    cmodModules_.clear();
    for (auto& [name, module] : cjmodModules_) {
        if (module.isLoaded) {
            unloadCJMODLibrary(module);
        }
    }
    cjmodModules_.clear();
    errors_.clear();
}

std::string EnhancedImportManager::readFile(const std::string& filePath) const {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return "";
    }
    
    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

std::string EnhancedImportManager::normalizePath(const std::string& path) const {
    std::string result = path;
    
    // 只有在模块路径中才将'.'替换为'/'
    // 对于文件路径，保持原样
    if (result.find('/') == std::string::npos && result.find('.') != std::string::npos) {
        // 检查是否是文件扩展名
        size_t lastDot = result.find_last_of('.');
        size_t lastSlash = result.find_last_of('/');
        
        // 如果最后一个点在最后一个斜杠之后，可能是扩展名，不替换
        if (lastSlash == std::string::npos || lastDot > lastSlash) {
            // 检查扩展名
            std::string ext = result.substr(lastDot + 1);
            if (ext == "chtl" || ext == "html" || ext == "css" || ext == "js") {
                return result; // 保持原文件路径
            }
        }
        
        // 否则替换为模块路径
        std::replace(result.begin(), result.end(), '.', '/');
    }
    
    return result;
}

std::string EnhancedImportManager::findModuleFile(const std::string& moduleName) const {
    std::string normalizedName = normalizePath(moduleName);
    
    // 在搜索路径中查找模块
    for (const std::string& searchPath : moduleSearchPaths_) {
        std::vector<std::string> possiblePaths = {
            searchPath + "/" + normalizedName + ".chtl",
            searchPath + "/" + normalizedName + "/main.chtl",
            searchPath + "/" + normalizedName + "/" + normalizedName + ".chtl"
        };
        
        for (const std::string& possiblePath : possiblePaths) {
            if (fileExists(possiblePath)) {
                return possiblePath;
            }
        }
    }
    
    return "";
}

bool EnhancedImportManager::fileExists(const std::string& path) const {
    std::ifstream file(path);
    return file.good();
}

std::string EnhancedImportManager::extractNamedObject(const std::string& content, 
                                                     const std::string& objectType,
                                                     const std::string& subType, 
                                                     const std::string& objectName) const {
    // 构建正则表达式匹配特定对象
    std::string pattern = R"(\[)" + objectType + R"(\]\s*@)" + subType + R"(\s+)" + objectName + R"(\s*\{([^}]*)\})";
    std::regex objectRegex(pattern);
    std::smatch match;
    
    if (std::regex_search(content, match, objectRegex)) {
        return match[0].str(); // 返回整个匹配的定义
    }
    
    return "";
}

bool EnhancedImportManager::parseCMODModule(const std::string& path) {
    try {
        std::filesystem::path modulePath(path);
        std::string moduleName = modulePath.stem().string();
        
        CMODModule module;
        module.name = moduleName;
        module.rootPath = modulePath.parent_path().string();
        
        // 解析模块结构
        // 这里可以扩展解析CMOD模块的具体结构
        
        cmodModules_[moduleName] = module;
        return true;
    } catch (const std::exception& e) {
        addError("解析CMOD模块失败: " + std::string(e.what()));
        return false;
    }
}

bool EnhancedImportManager::parseCJMODModule(const std::string& path) {
    try {
        std::filesystem::path modulePath(path);
        std::string moduleName = modulePath.stem().string();
        
        CJMODModule module;
        module.name = moduleName;
        module.rootPath = modulePath.parent_path().string();
        
        // 查找C++源文件和头文件
        std::filesystem::path srcPath = modulePath.parent_path() / "src";
        if (std::filesystem::exists(srcPath)) {
            for (const auto& entry : std::filesystem::directory_iterator(srcPath)) {
                std::string ext = entry.path().extension().string();
                if (ext == ".cpp" || ext == ".cc" || ext == ".cxx") {
                    module.sourceFiles.push_back(entry.path().string());
                } else if (ext == ".h" || ext == ".hpp" || ext == ".hxx") {
                    module.headerFiles.push_back(entry.path().string());
                }
            }
        }
        
        cjmodModules_[moduleName] = module;
        return true;
    } catch (const std::exception& e) {
        addError("解析CJMOD模块失败: " + std::string(e.what()));
        return false;
    }
}

bool EnhancedImportManager::loadCJMODLibrary(CJMODModule& module) {
    // 构建动态库路径
    std::string libPath = module.rootPath + "/lib" + module.name + ".so";
    
    // 加载动态库
    module.dynamicLibrary = dlopen(libPath.c_str(), RTLD_LAZY);
    if (!module.dynamicLibrary) {
        addError("加载CJMOD动态库失败: " + std::string(dlerror()));
        return false;
    }
    
    module.isLoaded = true;
    return true;
}

void EnhancedImportManager::unloadCJMODLibrary(CJMODModule& module) {
    if (module.dynamicLibrary) {
        dlclose(module.dynamicLibrary);
        module.dynamicLibrary = nullptr;
        module.isLoaded = false;
    }
}

void EnhancedImportManager::addError(const std::string& error) {
    errors_.push_back(error);
}

// ImportStatementParser 实现

ImportItem ImportStatementParser::parseStatement(const std::string& statement) {
    ImportItem item;
    
    // 检查是否为精确导入
    if (statement.find("[Custom]") != std::string::npos || 
        statement.find("[Template]") != std::string::npos) {
        return parsePreciseImport(statement);
    } else {
        return parseBasicImport(statement);
    }
}

ImportItem ImportStatementParser::parseBasicImport(const std::string& statement) {
    ImportItem item;
    
    // 解析基本Import: [Import] @Type from path as alias
    std::regex basicRegex(R"(\[Import\]\s*(@\w+)\s+from\s+([^\s]+)(?:\s+as\s+(\w+))?)", std::regex_constants::icase);
    std::smatch match;
    
    if (std::regex_search(statement, match, basicRegex)) {
        std::string typeString = match[1].str();
        item.path = match[2].str();
        if (match[3].matched) {
            item.alias = match[3].str();
        }
        
        item.type = determineImportType(typeString);
        return item;
    }
    
    // 解析异常，返回默认项
    return item;
}

ImportItem ImportStatementParser::parsePreciseImport(const std::string& statement) {
    ImportItem item;
    
    // 解析精确Import: [Import] [Category] @Type name from path as alias
    std::regex preciseRegex(R"(\[Import\]\s*\[(\w+)\]\s*(@\w+)\s+(\w+)\s+from\s+([^\s]+)(?:\s+as\s+(\w+))?)", std::regex_constants::icase);
    std::smatch match;
    
    if (std::regex_search(statement, match, preciseRegex)) {
        std::string category = match[1].str();
        std::string typeString = match[2].str();
        item.name = match[3].str();
        item.path = match[4].str();
        if (match[5].matched) {
            item.alias = match[5].str();
        }
        
        item.type = determinePreciseImportType(category, typeString);
        return item;
    }
    
    return item;
}

ImportType ImportStatementParser::determineImportType(const std::string& typeString) {
    if (typeString == "@Html") return ImportType::HTML_FILE;
    if (typeString == "@Style") return ImportType::CSS_FILE;
    if (typeString == "@JavaScript") return ImportType::JAVASCRIPT_FILE;
    if (typeString == "@Chtl") return ImportType::CHTL_FILE;
    if (typeString == "@CJmod") return ImportType::CJMOD_FILE;
    
    return ImportType::HTML_FILE; // 默认
}

ImportType ImportStatementParser::determinePreciseImportType(const std::string& category, 
                                                           const std::string& type) {
    if (category == "Custom") {
        if (type == "@Element") return ImportType::CUSTOM_ELEMENT;
        if (type == "@Style") return ImportType::CUSTOM_STYLE;
        if (type == "@Var") return ImportType::CUSTOM_VAR;
    } else if (category == "Template") {
        if (type == "@Element") return ImportType::TEMPLATE_ELEMENT;
        if (type == "@Style") return ImportType::TEMPLATE_STYLE;
        if (type == "@Var") return ImportType::TEMPLATE_VAR;
    }
    
    return ImportType::HTML_FILE; // 默认
}

std::pair<std::string, std::string> ImportStatementParser::extractPathAndAlias(const std::string& fromClause) {
    std::regex fromRegex(R"(from\s+([^\s]+)(?:\s+as\s+(\w+))?)", std::regex_constants::icase);
    std::smatch match;
    
    if (std::regex_search(fromClause, match, fromRegex)) {
        std::string path = match[1].str();
        std::string alias = match[2].matched ? match[2].str() : "";
        return {path, alias};
    }
    
    return {"", ""};
}

} // namespace chtl
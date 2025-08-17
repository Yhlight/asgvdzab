#include "module/module_manager.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <algorithm>
#include <fstream>

namespace CHTL {
namespace Module {

// =============================================================================
// ModuleInfo 实现
// =============================================================================

bool ModuleInfo::isVersionCompatible(const std::string& currentVersion) const {
    // 简化的版本兼容性检查（实际应使用语义版本号比较）
    if (minCHTLVersion.empty() && maxCHTLVersion.empty()) {
        return true; // 无版本限制
    }
    
    // 这里应该实现完整的版本号比较逻辑
    // 为简化实现，这里只做基本的字符串比较
    if (!minCHTLVersion.empty() && currentVersion < minCHTLVersion) {
        return false;
    }
    
    if (!maxCHTLVersion.empty() && currentVersion > maxCHTLVersion) {
        return false;
    }
    
    return true;
}

std::unique_ptr<ModuleInfo> ModuleInfo::parseFromChtl(const std::string& chtlContent, ModuleType type) {
    auto info = std::make_unique<ModuleInfo>();
    info->type = type;
    
    // 使用正则表达式解析[Info]块
    std::regex infoBlockRegex(R"(\[Info\]\s*\{([^}]*)\})");
    std::smatch infoMatch;
    
    if (!std::regex_search(chtlContent, infoMatch, infoBlockRegex)) {
        return nullptr; // 未找到Info块
    }
    
    std::string infoContent = infoMatch[1].str();
    
    // 解析各个字段
    std::regex fieldRegex(R"((\w+)\s*=\s*\"([^\"]*)\")");
    std::sregex_iterator iter(infoContent.begin(), infoContent.end(), fieldRegex);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        std::string key = (*iter)[1].str();
        std::string value = (*iter)[2].str();
        
        if (key == "name") {
            info->name = value;
        } else if (key == "version") {
            info->version = value;
        } else if (key == "description") {
            info->description = value;
        } else if (key == "author") {
            info->author = value;
        } else if (key == "license") {
            info->license = value;
        } else if (key == "dependencies") {
            // 解析依赖列表（逗号分隔）
            std::stringstream ss(value);
            std::string dep;
            while (std::getline(ss, dep, ',')) {
                // 移除前后空白
                dep.erase(0, dep.find_first_not_of(" \t"));
                dep.erase(dep.find_last_not_of(" \t") + 1);
                if (!dep.empty()) {
                    info->dependencies.push_back(dep);
                }
            }
        } else if (key == "category") {
            info->category = value;
        } else if (key == "minCHTLVersion") {
            info->minCHTLVersion = value;
        } else if (key == "maxCHTLVersion") {
            info->maxCHTLVersion = value;
        }
    }
    
    return info;
}

// =============================================================================
// ModuleExports 实现
// =============================================================================

void ModuleExports::merge(const ModuleExports& other) {
    styles.insert(other.styles.begin(), other.styles.end());
    elements.insert(other.elements.begin(), other.elements.end());
    variables.insert(other.variables.begin(), other.variables.end());
    functions.insert(other.functions.begin(), other.functions.end());
}

bool ModuleExports::hasStyle(const std::string& name) const {
    return styles.find(name) != styles.end();
}

bool ModuleExports::hasElement(const std::string& name) const {
    return elements.find(name) != elements.end();
}

bool ModuleExports::hasVariable(const std::string& name) const {
    return variables.find(name) != variables.end();
}

bool ModuleExports::hasFunction(const std::string& name) const {
    return functions.find(name) != functions.end();
}

// =============================================================================
// CmodModule 实现
// =============================================================================

CmodModule::CmodModule(std::unique_ptr<ModuleInfo> info, const std::string& modulePath)
    : info_(std::move(info)), modulePath_(modulePath) {
    exports_ = std::make_unique<ModuleExports>();
}

void CmodModule::addSourceFile(const std::string& filename, const std::string& content) {
    sourceFiles_[filename] = content;
}

std::string CmodModule::getSourceFile(const std::string& filename) const {
    auto it = sourceFiles_.find(filename);
    return it != sourceFiles_.end() ? it->second : "";
}

std::vector<std::string> CmodModule::getSourceFileNames() const {
    std::vector<std::string> names;
    for (const auto& [name, content] : sourceFiles_) {
        names.push_back(name);
    }
    return names;
}

bool CmodModule::hasSourceFile(const std::string& filename) const {
    return sourceFiles_.find(filename) != sourceFiles_.end();
}

void CmodModule::addSubModule(const std::string& name, std::unique_ptr<CmodModule> subModule) {
    subModules_[name] = std::move(subModule);
}

const CmodModule* CmodModule::getSubModule(const std::string& name) const {
    auto it = subModules_.find(name);
    return it != subModules_.end() ? it->second.get() : nullptr;
}

CmodModule* CmodModule::getSubModuleMutable(const std::string& name) {
    auto it = subModules_.find(name);
    return it != subModules_.end() ? it->second.get() : nullptr;
}

std::vector<std::string> CmodModule::getSubModuleNames() const {
    std::vector<std::string> names;
    for (const auto& [name, subModule] : subModules_) {
        names.push_back(name);
    }
    return names;
}

bool CmodModule::hasSubModule(const std::string& name) const {
    return subModules_.find(name) != subModules_.end();
}

bool CmodModule::packToFile(const std::string& outputPath) const {
    auto writer = Utils::createZipWriter(outputPath);
    if (!writer) {
        return false;
    }
    
    // 添加info文件
    std::string infoPath = "info/" + info_->name + ".chtl";
    std::string infoContent = generateInfoContent();
    writer->addFile(infoPath, infoContent);
    
    // 添加源文件
    for (const auto& [filename, content] : sourceFiles_) {
        std::string srcPath = "src/" + filename;
        writer->addFile(srcPath, content);
    }
    
    // 递归添加子模块
    for (const auto& [name, subModule] : subModules_) {
        std::string subModulePrefix = "src/" + name + "/";
        
        // 添加子模块的info文件
        std::string subInfoPath = subModulePrefix + "info/" + name + ".chtl";
        std::string subInfoContent = subModule->generateInfoContent();
        writer->addFile(subInfoPath, subInfoContent);
        
        // 添加子模块的源文件
        for (const auto& [subFilename, subContent] : subModule->sourceFiles_) {
            std::string subSrcPath = subModulePrefix + "src/" + subFilename;
            writer->addFile(subSrcPath, subContent);
        }
    }
    
    return writer->write();
}

std::unique_ptr<CmodModule> CmodModule::loadFromFile(const std::string& cmodPath) {
    auto reader = Utils::createZipReader(cmodPath);
    if (!reader || !reader->load()) {
        return nullptr;
    }
    
    // 查找主模块的info文件
    std::vector<std::string> allFiles = reader->listFiles();
    std::string mainInfoFile;
    
    // 查找根级别的info文件
    for (const std::string& filename : allFiles) {
        if (filename.find("info/") == 0 && filename.find(".chtl") != std::string::npos) {
            // 检查是否是根级别的info文件（不在子目录中）
            std::string relativePath = filename.substr(5); // 移除"info/"前缀
            if (relativePath.find('/') == std::string::npos) {
                mainInfoFile = filename;
                break;
            }
        }
    }
    
    if (mainInfoFile.empty()) {
        return nullptr; // 未找到主info文件
    }
    
    // 解析主模块信息
    std::string infoContent = reader->extractTextFile(mainInfoFile);
    auto moduleInfo = ModuleInfo::parseFromChtl(infoContent, ModuleType::CMOD);
    if (!moduleInfo) {
        return nullptr;
    }
    
    auto module = std::make_unique<CmodModule>(std::move(moduleInfo), cmodPath);
    
    // 加载源文件
    for (const std::string& filename : allFiles) {
        if (filename.find("src/") == 0) {
            std::string relativePath = filename.substr(4); // 移除"src/"前缀
            
            // 检查是否是根级别的源文件（不在子模块目录中）
            if (relativePath.find('/') == std::string::npos) {
                std::string content = reader->extractTextFile(filename);
                module->addSourceFile(relativePath, content);
            }
        }
    }
    
    // 解析导出表
    module->parseExports();
    
    return module;
}

std::unique_ptr<CmodModule> CmodModule::buildFromDirectory(const std::string& dirPath) {
    std::filesystem::path dir(dirPath);
    if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir)) {
        return nullptr;
    }
    
    // 查找info文件
    std::filesystem::path infoDir = dir / "info";
    if (!std::filesystem::exists(infoDir)) {
        return nullptr;
    }
    
    std::string infoFile;
    std::string moduleName = dir.filename().string();
    
    // 查找与目录同名的info文件
    std::filesystem::path expectedInfoFile = infoDir / (moduleName + ".chtl");
    if (std::filesystem::exists(expectedInfoFile)) {
        infoFile = expectedInfoFile.string();
    } else {
        // 查找任何.chtl文件
        for (const auto& entry : std::filesystem::directory_iterator(infoDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".chtl") {
                infoFile = entry.path().string();
                break;
            }
        }
    }
    
    if (infoFile.empty()) {
        return nullptr;
    }
    
    // 读取并解析info文件
    std::ifstream infoStream(infoFile);
    if (!infoStream) {
        return nullptr;
    }
    
    std::string infoContent((std::istreambuf_iterator<char>(infoStream)),
                           std::istreambuf_iterator<char>());
    
    auto moduleInfo = ModuleInfo::parseFromChtl(infoContent, ModuleType::CMOD);
    if (!moduleInfo) {
        return nullptr;
    }
    
    auto module = std::make_unique<CmodModule>(std::move(moduleInfo), dirPath);
    
    // 加载源文件
    if (!module->loadDirectoryRecursive(dir, dir)) {
        return nullptr;
    }
    
    // 解析导出表
    module->parseExports();
    
    return module;
}

void CmodModule::parseExports() {
    exports_->styles.clear();
    exports_->elements.clear();
    exports_->variables.clear();
    
    // 解析所有源文件中的导出
    for (const auto& [filename, content] : sourceFiles_) {
        parseExportsFromContent(content);
    }
    
    // 确保子模块也解析了它们的导出
    for (const auto& [name, subModule] : subModules_) {
        subModule->parseExports();
    }
}

bool CmodModule::validate() const {
    // 验证模块信息
    if (info_->name.empty() || info_->version.empty()) {
        return false;
    }
    
    // 验证至少有一个源文件或子模块
    if (sourceFiles_.empty() && subModules_.empty()) {
        return false;
    }
    
    // 递归验证子模块
    for (const auto& [name, subModule] : subModules_) {
        if (!subModule->validate()) {
            return false;
        }
    }
    
    return true;
}

ModuleExports CmodModule::getFullExports() const {
    ModuleExports fullExports = *exports_;
    
    // 合并所有子模块的导出
    for (const auto& [name, subModule] : subModules_) {
        auto subExports = subModule->getFullExports();
        fullExports.merge(subExports);
    }
    
    return fullExports;
}

bool CmodModule::loadDirectoryRecursive(const std::filesystem::path& basePath, const std::filesystem::path& currentPath) {
    std::filesystem::path srcDir = currentPath / "src";
    
    if (std::filesystem::exists(srcDir) && std::filesystem::is_directory(srcDir)) {
        for (const auto& entry : std::filesystem::directory_iterator(srcDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".chtl") {
                // 读取源文件
                std::ifstream file(entry.path());
                if (file) {
                    std::string content((std::istreambuf_iterator<char>(file)),
                                      std::istreambuf_iterator<char>());
                    
                    std::string filename = entry.path().filename().string();
                    addSourceFile(filename, content);
                }
            } else if (entry.is_directory()) {
                // 检查是否是子模块目录
                std::filesystem::path subModuleInfoDir = entry.path() / "info";
                if (std::filesystem::exists(subModuleInfoDir)) {
                    std::string subModuleName = entry.path().filename().string();
                    auto subModule = buildFromDirectory(entry.path().string());
                    if (subModule) {
                        addSubModule(subModuleName, std::move(subModule));
                    }
                }
            }
        }
    }
    
    return true;
}

bool CmodModule::parseInfoFile(const std::string& content) {
    // info已在构造时解析，这里可以进行额外验证
    return !info_->name.empty();
}

std::string CmodModule::generateExportTable() const {
    std::ostringstream oss;
    oss << "[Export]\n{\n";
    
    // 生成Style导出
    if (!exports_->styles.empty()) {
        oss << "    @Style ";
        bool first = true;
        for (const std::string& style : exports_->styles) {
            if (!first) oss << ", ";
            oss << style;
            first = false;
        }
        oss << ";\n";
    }
    
    // 生成Element导出
    if (!exports_->elements.empty()) {
        oss << "    @Element ";
        bool first = true;
        for (const std::string& element : exports_->elements) {
            if (!first) oss << ", ";
            oss << element;
            first = false;
        }
        oss << ";\n";
    }
    
    // 生成Var导出
    if (!exports_->variables.empty()) {
        oss << "    @Var ";
        bool first = true;
        for (const std::string& var : exports_->variables) {
            if (!first) oss << ", ";
            oss << var;
            first = false;
        }
        oss << ";\n";
    }
    
    oss << "}\n";
    return oss.str();
}

std::string CmodModule::generateInfoContent() const {
    std::ostringstream oss;
    oss << "[Info]\n{\n";
    oss << "    name = \"" << info_->name << "\";\n";
    oss << "    version = \"" << info_->version << "\";\n";
    oss << "    description = \"" << info_->description << "\";\n";
    oss << "    author = \"" << info_->author << "\";\n";
    oss << "    license = \"" << info_->license << "\";\n";
    
    // 依赖列表
    if (!info_->dependencies.empty()) {
        oss << "    dependencies = \"";
        for (size_t i = 0; i < info_->dependencies.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << info_->dependencies[i];
        }
        oss << "\";\n";
    }
    
    oss << "    category = \"" << info_->category << "\";\n";
    oss << "    minCHTLVersion = \"" << info_->minCHTLVersion << "\";\n";
    oss << "    maxCHTLVersion = \"" << info_->maxCHTLVersion << "\";\n";
    oss << "}\n\n";
    
    // 生成导出表
    oss << generateExportTable();
    
    return oss.str();
}

void CmodModule::parseExportsFromContent(const std::string& content) {
    // 解析[Custom]和[Template]定义
    std::regex customElementRegex(R"(\[Custom\]\s*@Element\s+(\w+))");
    std::regex templateElementRegex(R"(\[Template\]\s*@Element\s+(\w+))");
    std::regex customStyleRegex(R"(\[Custom\]\s*@Style\s+(\w+))");
    std::regex templateStyleRegex(R"(\[Template\]\s*@Style\s+(\w+))");
    std::regex customVarRegex(R"(\[Custom\]\s*@Var\s+(\w+))");
    std::regex templateVarRegex(R"(\[Template\]\s*@Var\s+(\w+))");
    
    std::smatch match;
    
    // 查找所有匹配
    std::string::const_iterator searchStart(content.cbegin());
    
    // 查找Element定义
    while (std::regex_search(searchStart, content.cend(), match, customElementRegex)) {
        exports_->elements.insert(match[1].str());
        searchStart = match.suffix().first;
    }
    
    searchStart = content.cbegin();
    while (std::regex_search(searchStart, content.cend(), match, templateElementRegex)) {
        exports_->elements.insert(match[1].str());
        searchStart = match.suffix().first;
    }
    
    // 查找Style定义
    searchStart = content.cbegin();
    while (std::regex_search(searchStart, content.cend(), match, customStyleRegex)) {
        exports_->styles.insert(match[1].str());
        searchStart = match.suffix().first;
    }
    
    searchStart = content.cbegin();
    while (std::regex_search(searchStart, content.cend(), match, templateStyleRegex)) {
        exports_->styles.insert(match[1].str());
        searchStart = match.suffix().first;
    }
    
    // 查找Var定义
    searchStart = content.cbegin();
    while (std::regex_search(searchStart, content.cend(), match, customVarRegex)) {
        exports_->variables.insert(match[1].str());
        searchStart = match.suffix().first;
    }
    
    searchStart = content.cbegin();
    while (std::regex_search(searchStart, content.cend(), match, templateVarRegex)) {
        exports_->variables.insert(match[1].str());
        searchStart = match.suffix().first;
    }
}

} // namespace Module
} // namespace CHTL
#include "cjmod/cjmod_manager.hpp"
#include "cmod/cmod_manager.hpp" // 复用SimpleZip类
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <cstring>
#include <chrono>
#include <iostream>
#include <cstdlib>

namespace chtl {

// ========== OfficialModuleResolver 实现 ==========

OfficialModuleResolver::OfficialModuleResolver() 
    : officialModuleRoot_("/usr/local/share/chtl/official") {
}

OfficialModuleResolver::~OfficialModuleResolver() = default;

bool OfficialModuleResolver::isOfficialModule(const std::string& moduleName) {
    return moduleName.length() >= 6 && moduleName.substr(0, 6) == "chtl::";
}

std::string OfficialModuleResolver::extractOfficialModuleName(const std::string& moduleName) {
    if (isOfficialModule(moduleName)) {
        return moduleName.substr(6); // 去除 "chtl::" 前缀
    }
    return moduleName;
}

std::string OfficialModuleResolver::getOfficialModulePath(const std::string& moduleName, const std::string& moduleType) {
    std::string basePath = officialModuleRoot_ + "/" + moduleType + "/" + moduleName;
    
    // 优先查找打包后的模块文件
    std::string packedPath = basePath + "." + moduleType;
    if (std::filesystem::exists(packedPath)) {
        return packedPath;
    }
    
    // 查找源码目录
    if (std::filesystem::exists(basePath) && std::filesystem::is_directory(basePath)) {
        return basePath;
    }
    
    return "";
}

void OfficialModuleResolver::setOfficialModuleRoot(const std::string& path) {
    officialModuleRoot_ = path;
}

const std::string& OfficialModuleResolver::getOfficialModuleRoot() const {
    return officialModuleRoot_;
}

std::vector<std::string> OfficialModuleResolver::listOfficialModules(const std::string& moduleType) const {
    std::vector<std::string> modules;
    
    std::string searchPath = officialModuleRoot_;
    if (!moduleType.empty()) {
        searchPath += "/" + moduleType;
    }
    
    if (!std::filesystem::exists(searchPath)) {
        return modules;
    }
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(searchPath)) {
            if (moduleType.empty()) {
                // 遍历子目录（cmod、cjmod）
                if (entry.is_directory()) {
                    auto subModules = listOfficialModules(entry.path().filename().string());
                    modules.insert(modules.end(), subModules.begin(), subModules.end());
                }
            } else {
                // 列出具体类型的模块
                if (entry.is_directory()) {
                    modules.push_back("chtl::" + entry.path().filename().string());
                } else if (entry.path().extension() == "." + moduleType) {
                    modules.push_back("chtl::" + entry.path().stem().string());
                }
            }
        }
    } catch (...) {
        // 忽略文件系统错误
    }
    
    return modules;
}

// ========== CjmodCompilerInterface 实现 ==========

CjmodCompilerInterface::CjmodCompilerInterface() 
    : compilerPath_("g++") {
    
    // 设置默认编译标志
    compileFlags_ = {
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-O2",
        "-fPIC",
        "-shared"
    };
}

CjmodCompilerInterface::~CjmodCompilerInterface() = default;

bool CjmodCompilerInterface::compileCjmodModule(const std::string& sourceDir, const std::string& outputDir, const CjmodInfo& moduleInfo) {
    // 创建输出目录
    std::filesystem::create_directories(outputDir);
    
    // 生成动态库
    std::string dylibPath = outputDir + "/lib" + moduleInfo.name + ".so";
    return generateDynamicLibrary(sourceDir, dylibPath, moduleInfo);
}

bool CjmodCompilerInterface::generateDynamicLibrary(const std::string& sourceDir, const std::string& outputPath, const CjmodInfo& moduleInfo) {
    // 收集所有C++源文件
    std::vector<std::string> cppFiles;
    
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(sourceDir + "/src")) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                if (ext == ".cpp" || ext == ".cc" || ext == ".cxx") {
                    cppFiles.push_back(entry.path().string());
                }
            }
        }
    } catch (...) {
        return false;
    }
    
    if (cppFiles.empty()) {
        return false;
    }
    
    // 生成编译命令
    std::stringstream cmd;
    cmd << compilerPath_;
    
    // 添加编译标志
    for (const auto& flag : compileFlags_) {
        cmd << " " << flag;
    }
    
    // 添加模块特定的编译标志
    if (!moduleInfo.compilerFlags.empty()) {
        cmd << " " << moduleInfo.compilerFlags;
    }
    
    // 添加包含路径
    for (const auto& includePath : moduleInfo.includePaths) {
        cmd << " -I\"" << includePath << "\"";
    }
    
    // 添加CHTL JS扩展接口头文件路径
    cmd << " -I\"include\"";
    
    // 添加源文件
    for (const auto& cppFile : cppFiles) {
        cmd << " \"" << cppFile << "\"";
    }
    
    // 添加链接库
    for (const auto& library : moduleInfo.libraries) {
        cmd << " -l" << library;
    }
    
    // 输出路径
    cmd << " -o \"" << outputPath << "\"";
    
    return executeBuildCommand(cmd.str());
}

std::vector<std::string> CjmodCompilerInterface::validateCppSyntax(const std::vector<std::string>& cppFiles) {
    std::vector<std::string> errors;
    
    for (const auto& cppFile : cppFiles) {
        std::stringstream cmd;
        cmd << compilerPath_ << " -std=c++17 -fsyntax-only \"" << cppFile << "\" 2>&1";
        
        FILE* pipe = popen(cmd.str().c_str(), "r");
        if (pipe) {
            char buffer[256];
            std::string result;
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                result += buffer;
            }
            pclose(pipe);
            
            if (!result.empty()) {
                errors.push_back(cppFile + ": " + result);
            }
        }
    }
    
    return errors;
}

std::vector<std::string> CjmodCompilerInterface::checkDependencies(const std::vector<std::string>& libraries) {
    std::vector<std::string> missing;
    
    for (const auto& library : libraries) {
        std::stringstream cmd;
        cmd << "pkg-config --exists " << library << " 2>/dev/null";
        
        int result = system(cmd.str().c_str());
        if (result != 0) {
            missing.push_back(library);
        }
    }
    
    return missing;
}

void CjmodCompilerInterface::setCompilerPath(const std::string& compilerPath) {
    compilerPath_ = compilerPath;
}

void CjmodCompilerInterface::setCompileFlags(const std::vector<std::string>& flags) {
    compileFlags_ = flags;
}

std::string CjmodCompilerInterface::generateBuildScript(const std::string& sourceDir, const std::string& outputPath, const CjmodInfo& moduleInfo) {
    std::stringstream script;
    
    script << "#!/bin/bash\n";
    script << "# CJmod构建脚本 - " << moduleInfo.name << "\n\n";
    script << "set -e\n\n";
    
    script << "echo \"构建CJmod模块: " << moduleInfo.name << "\"\n";
    script << "echo \"源目录: " << sourceDir << "\"\n";
    script << "echo \"输出: " << outputPath << "\"\n\n";
    
    // 编译命令
    script << compilerPath_;
    for (const auto& flag : compileFlags_) {
        script << " " << flag;
    }
    script << " -I\"include\"";
    for (const auto& includePath : moduleInfo.includePaths) {
        script << " -I\"" << includePath << "\"";
    }
    script << " " << sourceDir << "/src/*.cpp";
    for (const auto& library : moduleInfo.libraries) {
        script << " -l" << library;
    }
    script << " -o \"" << outputPath << "\"\n\n";
    
    script << "echo \"构建完成\"\n";
    
    return script.str();
}

bool CjmodCompilerInterface::executeBuildCommand(const std::string& command) {
    int result = system(command.c_str());
    return result == 0;
}

// ========== ChtlJsExtensionInterface 实现 ==========

ChtlJsExtensionInterface::ChtlJsExtensionInterface() = default;
ChtlJsExtensionInterface::~ChtlJsExtensionInterface() = default;

// ========== CjmodManager 实现 ==========

CjmodManager::CjmodManager() 
    : verbose_(false),
      compressor_(std::make_unique<SimpleZipCompressor>()),
      decompressor_(std::make_unique<SimpleZipDecompressor>()),
      officialResolver_(std::make_unique<OfficialModuleResolver>()),
      compilerInterface_(std::make_unique<CjmodCompilerInterface>()),
      totalPackedFiles_(0), totalPackedSize_(0),
      totalUnpackedFiles_(0), totalUnpackedSize_(0) {
}

CjmodManager::~CjmodManager() = default;

CjmodPackResult CjmodManager::packModule(const std::string& sourceDir, const std::string& outputPath) {
    CjmodPackResult result;
    
    log("开始打包CJmod模块: " + sourceDir);
    
    // 验证模块结构
    auto validation = validateModuleStructure(sourceDir);
    if (!validation.isValid) {
        result.errorMessage = validation.errorMessage;
        return result;
    }
    
    log("CJmod模块结构验证通过");
    
    compressor_->clear();
    
    // 收集所有文件
    auto files = collectFiles(sourceDir, sourceDir);
    
    log("收集到 " + std::to_string(files.size()) + " 个文件");
    
    for (const auto& filePath : files) {
        std::string relativePath = getRelativePath(filePath, sourceDir);
        
        if (std::filesystem::is_directory(filePath)) {
            compressor_->addDirectory(relativePath);
            log("添加目录: " + relativePath);
        } else {
            std::ifstream file(filePath, std::ios::binary);
            if (!file) {
                result.errorMessage = "无法读取文件: " + filePath;
                return result;
            }
            
            std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)),
                                     std::istreambuf_iterator<char>());
            
            compressor_->addFile(relativePath, data);
            
            // 统计文件类型
            std::string fileType = analyzeFileType(filePath);
            if (fileType == "cpp") {
                result.cppFiles++;
            } else if (fileType == "h") {
                result.headerFiles++;
            } else if (fileType == "chtl") {
                result.infoFiles++;
            }
            
            log("添加文件: " + relativePath + " (" + std::to_string(data.size()) + " bytes, 类型: " + fileType + ")");
            
            result.totalSize += data.size();
            result.totalFiles++;
        }
    }
    
    // 写入ZIP文件
    if (!compressor_->writeToFile(outputPath)) {
        result.errorMessage = "写入CJmod文件失败: " + outputPath;
        return result;
    }
    
    result.success = true;
    result.outputPath = outputPath;
    
    totalPackedFiles_ += result.totalFiles;
    totalPackedSize_ += result.totalSize;
    
    log("CJmod打包完成: " + outputPath);
    log("总文件数: " + std::to_string(result.totalFiles));
    log("C++文件数: " + std::to_string(result.cppFiles));
    log("头文件数: " + std::to_string(result.headerFiles));
    log("信息文件数: " + std::to_string(result.infoFiles));
    log("总大小: " + std::to_string(result.totalSize) + " bytes");
    
    return result;
}

CjmodValidationResult CjmodManager::validateModuleStructure(const std::string& sourceDir) {
    CjmodValidationResult result;
    
    if (!std::filesystem::exists(sourceDir) || !std::filesystem::is_directory(sourceDir)) {
        result.errorMessage = "源目录不存在或不是目录: " + sourceDir;
        return result;
    }
    
    std::string moduleName = std::filesystem::path(sourceDir).filename().string();
    if (!isValidModuleName(moduleName)) {
        result.errorMessage = "无效的模块名称: " + moduleName;
        return result;
    }
    
    // 验证src目录
    std::string srcDir = sourceDir + "/src";
    if (!validateSourceDirectory(srcDir, result)) {
        return result;
    }
    
    // 验证info目录
    std::string infoDir = sourceDir + "/info";
    if (!validateInfoDirectory(infoDir, result)) {
        return result;
    }
    
    result.isValid = true;
    return result;
}

CjmodUnpackResult CjmodManager::unpackModule(const std::string& cjmodPath, const std::string& outputDir) {
    CjmodUnpackResult result;
    
    log("开始解包CJmod模块: " + cjmodPath);
    
    if (!std::filesystem::exists(cjmodPath)) {
        result.errorMessage = "CJmod文件不存在: " + cjmodPath;
        return result;
    }
    
    decompressor_->clear();
    
    if (!decompressor_->loadFromFile(cjmodPath)) {
        result.errorMessage = "加载CJmod文件失败: " + cjmodPath;
        return result;
    }
    
    log("CJmod文件加载成功");
    
    if (!decompressor_->extractAll(outputDir)) {
        result.errorMessage = "解压CJmod文件失败";
        return result;
    }
    
    // 收集解压的文件列表
    for (const auto& entry : decompressor_->getEntries()) {
        if (!entry.isDirectory) {
            result.extractedFiles.push_back(entry.path);
            totalUnpackedFiles_++;
            totalUnpackedSize_ += entry.size;
        }
    }
    
    // 读取模块信息
    std::string infoPath = outputDir + "/info";
    if (std::filesystem::exists(infoPath)) {
        for (const auto& entry : std::filesystem::directory_iterator(infoPath)) {
            if (entry.path().extension() == ".chtl") {
                result.moduleInfo = parseInfoFile(entry.path().string());
                break;
            }
        }
    }
    
    result.success = true;
    result.outputDir = outputDir;
    
    log("CJmod解包完成: " + outputDir);
    log("解压文件数: " + std::to_string(result.extractedFiles.size()));
    
    return result;
}

CjmodInfo CjmodManager::readModuleInfo(const std::string& cjmodPath) {
    CjmodInfo info;
    
    decompressor_->clear();
    
    if (!decompressor_->loadFromFile(cjmodPath)) {
        return info;
    }
    
    // 查找info目录中的.chtl文件
    for (const auto& entry : decompressor_->getEntries()) {
        if (entry.path.find("info/") == 0 && 
            entry.path.find(".chtl") != std::string::npos &&
            !entry.isDirectory) {
            
            std::string content(entry.data.begin(), entry.data.end());
            info = parseInfoContent(content);
            break;
        }
    }
    
    return info;
}

CjmodInfo CjmodManager::parseInfoFile(const std::string& infoFilePath) {
    std::ifstream file(infoFilePath);
    if (!file) {
        return CjmodInfo{};
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    
    return parseInfoContent(content);
}

CjmodInfo CjmodManager::parseInfoContent(const std::string& content) {
    CjmodInfo info;
    
    // 简单的Info文件解析 (严格按照CHTL语法文档)
    std::regex infoBlockRegex("\\[Info\\]\\s*\\{([^}]*)\\}");
    std::smatch infoMatch;
    
    if (!std::regex_search(content, infoMatch, infoBlockRegex)) {
        return info;
    }
    
    std::string infoBlock = infoMatch[1].str();
    
    // 解析各个字段
    std::regex fieldRegex("(\\w+)\\s*[:=]\\s*\"([^\"]*)\"");
    std::sregex_iterator iter(infoBlock.begin(), infoBlock.end(), fieldRegex);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        std::string key = (*iter)[1].str();
        std::string value = (*iter)[2].str();
        
        if (key == "name") {
            info.name = value;
        } else if (key == "version") {
            info.version = value;
        } else if (key == "description") {
            info.description = value;
        } else if (key == "author") {
            info.author = value;
        } else if (key == "license") {
            info.license = value;
        } else if (key == "dependencies") {
            info.dependencies = value;
        } else if (key == "category") {
            info.category = value;
        } else if (key == "minCHTLVersion") {
            info.minCHTLVersion = value;
        } else if (key == "cppStandard") {
            info.cppStandard = value;
        } else if (key == "compilerFlags") {
            info.compilerFlags = value;
        } else if (key == "homepage") {
            info.homepage = value;
        } else if (key == "repository") {
            info.repository = value;
        }
    }
    
    // 解析数组字段
    std::regex arrayFieldRegex("(\\w+)\\s*[:=]\\s*\\[([^\\]]*)\\]");
    std::sregex_iterator arrayIter(infoBlock.begin(), infoBlock.end(), arrayFieldRegex);
    
    for (; arrayIter != end; ++arrayIter) {
        std::string key = (*arrayIter)[1].str();
        std::string arrayContent = (*arrayIter)[2].str();
        
        // 解析数组内容
        std::vector<std::string> items;
        std::regex itemRegex("\"([^\"]*)\"");
        std::sregex_iterator itemIter(arrayContent.begin(), arrayContent.end(), itemRegex);
        
        for (; itemIter != end; ++itemIter) {
            items.push_back((*itemIter)[1].str());
        }
        
        if (key == "includePaths") {
            info.includePaths = items;
        } else if (key == "libraries") {
            info.libraries = items;
        } else if (key == "jsExtensions") {
            info.jsExtensions = items;
        } else if (key == "keywords") {
            info.keywords = items;
        }
    }
    
    return info;
}

std::string CjmodManager::generateInfoFile(const CjmodInfo& info) {
    std::stringstream ss;
    
    ss << "// CJmod模块信息文件\n";
    ss << "// 严格按照CHTL语法文档格式\n\n";
    ss << "[Info]\n{\n";
    ss << "    name = \"" << info.name << "\";\n";
    ss << "    version = \"" << info.version << "\";\n";
    ss << "    description = \"" << info.description << "\";\n";
    ss << "    author = \"" << info.author << "\";\n";
    ss << "    license = \"" << info.license << "\";\n";
    ss << "    dependencies = \"" << info.dependencies << "\";\n";
    ss << "    category = \"" << info.category << "\";\n";
    ss << "    minCHTLVersion = \"" << info.minCHTLVersion << "\";\n";
    ss << "    cppStandard = \"" << info.cppStandard << "\";\n";
    
    if (!info.compilerFlags.empty()) {
        ss << "    compilerFlags = \"" << info.compilerFlags << "\";\n";
    }
    
    if (!info.includePaths.empty()) {
        ss << "    includePaths = [";
        for (size_t i = 0; i < info.includePaths.size(); ++i) {
            if (i > 0) ss << ", ";
            ss << "\"" << info.includePaths[i] << "\"";
        }
        ss << "];\n";
    }
    
    if (!info.libraries.empty()) {
        ss << "    libraries = [";
        for (size_t i = 0; i < info.libraries.size(); ++i) {
            if (i > 0) ss << ", ";
            ss << "\"" << info.libraries[i] << "\"";
        }
        ss << "];\n";
    }
    
    if (!info.jsExtensions.empty()) {
        ss << "    jsExtensions = [";
        for (size_t i = 0; i < info.jsExtensions.size(); ++i) {
            if (i > 0) ss << ", ";
            ss << "\"" << info.jsExtensions[i] << "\"";
        }
        ss << "];\n";
    }
    
    if (!info.homepage.empty()) {
        ss << "    homepage = \"" << info.homepage << "\";\n";
    }
    if (!info.repository.empty()) {
        ss << "    repository = \"" << info.repository << "\";\n";
    }
    
    ss << "}\n";
    
    return ss.str();
}

bool CjmodManager::compileCjmodModule(const std::string& sourceDir, const std::string& outputDir) {
    auto moduleInfo = parseInfoFile(sourceDir + "/info");
    return compilerInterface_->compileCjmodModule(sourceDir, outputDir, moduleInfo);
}

bool CjmodManager::buildDynamicLibrary(const std::string& sourceDir, const std::string& outputPath) {
    auto moduleInfo = parseInfoFile(sourceDir + "/info");
    return compilerInterface_->generateDynamicLibrary(sourceDir, outputPath, moduleInfo);
}

std::string CjmodManager::resolveModuleImport(const std::string& importPath, const std::string& moduleType) {
    // 检查是否为官方模块
    if (OfficialModuleResolver::isOfficialModule(importPath)) {
        std::string moduleName = OfficialModuleResolver::extractOfficialModuleName(importPath);
        return officialResolver_->getOfficialModulePath(moduleName, moduleType);
    }
    
    // 普通模块解析逻辑
    return importPath;
}

OfficialModuleResolver* CjmodManager::getOfficialModuleResolver() {
    return officialResolver_.get();
}

CjmodCompilerInterface* CjmodManager::getCompilerInterface() {
    return compilerInterface_.get();
}

std::vector<std::string> CjmodManager::collectFiles(const std::string& directory, const std::string& baseDir) {
    std::vector<std::string> files;
    
    if (!std::filesystem::exists(directory)) {
        return files;
    }
    
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
        files.push_back(entry.path().string());
    }
    
    return files;
}

std::string CjmodManager::analyzeFileType(const std::string& filePath) {
    std::filesystem::path path(filePath);
    std::string ext = path.extension().string();
    
    if (ext == ".cpp" || ext == ".cc" || ext == ".cxx") {
        return "cpp";
    } else if (ext == ".h" || ext == ".hpp" || ext == ".hxx") {
        return "h";
    } else if (ext == ".chtl") {
        return "chtl";
    } else {
        return "other";
    }
}

bool CjmodManager::isValidModuleName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    // 模块名只能包含字母、数字、下划线和点
    std::regex nameRegex("[a-zA-Z][a-zA-Z0-9_\\.]*[a-zA-Z0-9_]");
    return std::regex_match(name, nameRegex);
}

bool CjmodManager::isValidVersion(const std::string& version) {
    if (version.empty()) {
        return false;
    }
    
    // 简单的版本号验证 (major.minor.patch)
    std::regex versionRegex("\\d+\\.\\d+\\.\\d+");
    return std::regex_match(version, versionRegex);
}

std::string CjmodManager::getRelativePath(const std::string& filePath, const std::string& baseDir) {
    std::filesystem::path file(filePath);
    std::filesystem::path base(baseDir);
    
    try {
        return std::filesystem::relative(file, base).string();
    } catch (...) {
        return filePath;
    }
}

std::string CjmodManager::getPackStatistics() const {
    std::stringstream ss;
    ss << "CJmod打包统计:\n";
    ss << "  总打包文件数: " << totalPackedFiles_ << "\n";
    ss << "  总打包大小: " << totalPackedSize_ << " bytes\n";
    ss << "  总解包文件数: " << totalUnpackedFiles_ << "\n";
    ss << "  总解包大小: " << totalUnpackedSize_ << " bytes\n";
    return ss.str();
}

void CjmodManager::setVerbose(bool verbose) {
    verbose_ = verbose;
}

// 私有方法实现

bool CjmodManager::validateModuleDirectory(const std::string& moduleDir, CjmodValidationResult& result) {
    if (!std::filesystem::exists(moduleDir) || !std::filesystem::is_directory(moduleDir)) {
        result.errorMessage = "模块目录不存在: " + moduleDir;
        return false;
    }
    
    return true;
}

bool CjmodManager::validateSourceDirectory(const std::string& srcDir, CjmodValidationResult& result) {
    if (!std::filesystem::exists(srcDir) || !std::filesystem::is_directory(srcDir)) {
        result.errorMessage = "src目录不存在: " + srcDir;
        result.missingFiles.push_back("src/");
        return false;
    }
    
    // 检查是否有C++文件
    bool hasCppFiles = false;
    bool hasHeaderFiles = false;
    
    for (const auto& entry : std::filesystem::recursive_directory_iterator(srcDir)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext == ".cpp" || ext == ".cc" || ext == ".cxx") {
                hasCppFiles = true;
            } else if (ext == ".h" || ext == ".hpp" || ext == ".hxx") {
                hasHeaderFiles = true;
            }
        }
    }
    
    if (!hasCppFiles) {
        result.warnings.push_back("src目录中没有找到C++源文件");
    }
    
    if (!hasHeaderFiles) {
        result.warnings.push_back("src目录中没有找到头文件");
    }
    
    return true;
}

bool CjmodManager::validateInfoDirectory(const std::string& infoDir, CjmodValidationResult& result) {
    if (!std::filesystem::exists(infoDir) || !std::filesystem::is_directory(infoDir)) {
        result.errorMessage = "info目录不存在: " + infoDir;
        result.missingFiles.push_back("info/");
        return false;
    }
    
    // 检查是否有info文件
    bool hasInfoFile = false;
    for (const auto& entry : std::filesystem::directory_iterator(infoDir)) {
        if (entry.path().extension() == ".chtl") {
            hasInfoFile = true;
            
            // 解析info文件
            result.moduleInfo = parseInfoFile(entry.path().string());
            
            if (result.moduleInfo.name.empty()) {
                result.warnings.push_back("Info文件中缺少模块名称");
            }
            if (result.moduleInfo.version.empty()) {
                result.warnings.push_back("Info文件中缺少版本信息");
            }
            if (result.moduleInfo.cppStandard.empty()) {
                result.warnings.push_back("Info文件中缺少C++标准信息");
            }
            
            break;
        }
    }
    
    if (!hasInfoFile) {
        result.errorMessage = "info目录中没有找到.chtl文件";
        result.missingFiles.push_back("info/*.chtl");
        return false;
    }
    
    return true;
}

std::string CjmodManager::normalizePath(const std::string& path) {
    std::string normalized = path;
    std::replace(normalized.begin(), normalized.end(), '\\', '/');
    return normalized;
}

void CjmodManager::log(const std::string& message) const {
    if (verbose_) {
        std::cout << "[CjmodManager] " << message << std::endl;
    }
}

} // namespace chtl
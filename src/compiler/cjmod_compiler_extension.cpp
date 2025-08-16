#include "compiler/cjmod_compiler_extension.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <regex>

namespace chtl {

// ========== UnifiedModulePrefixManager 实现 ==========

UnifiedModulePrefixManager::UnifiedModulePrefixManager() 
    : resolver_(std::make_unique<OfficialModuleResolver>()) {
}

UnifiedModulePrefixManager::~UnifiedModulePrefixManager() = default;

std::string UnifiedModulePrefixManager::resolveModuleImport(const std::string& importPath, const std::string& moduleType) {
    // 检查是否为官方模块
    if (OfficialModuleResolver::isOfficialModule(importPath)) {
        std::string moduleName = OfficialModuleResolver::extractOfficialModuleName(importPath);
        return resolver_->getOfficialModulePath(moduleName, moduleType);
    }
    
    // 普通模块解析逻辑
    return importPath;
}

bool UnifiedModulePrefixManager::isOfficialModule(const std::string& moduleName) const {
    return OfficialModuleResolver::isOfficialModule(moduleName);
}

std::string UnifiedModulePrefixManager::extractOfficialModuleName(const std::string& moduleName) const {
    return OfficialModuleResolver::extractOfficialModuleName(moduleName);
}

std::vector<std::string> UnifiedModulePrefixManager::listOfficialModules(const std::string& moduleType) const {
    return resolver_->listOfficialModules(moduleType);
}

void UnifiedModulePrefixManager::setOfficialModuleRoot(const std::string& path) {
    resolver_->setOfficialModuleRoot(path);
}

std::string UnifiedModulePrefixManager::getOfficialModuleRoot() const {
    return resolver_->getOfficialModuleRoot();
}

// ========== CjmodCompilerExtension 实现 ==========

CjmodCompilerExtension::CjmodCompilerExtension() 
    : cjmodManager_(std::make_unique<CjmodManager>()),
      prefixManager_(std::make_unique<UnifiedModulePrefixManager>()),
      importManager_(nullptr),
      namespaceManager_(nullptr),
      cacheDirectory_(".chtl_cache_cjmod") {
    
    // 设置默认搜索路径
    moduleSearchPaths_ = {
        "module",           // 当前目录下的module文件夹
        "./module",         // 相对路径
        "../module",        // 上级目录
        "/usr/local/share/chtl/modules",  // 系统级模块路径
        "~/.chtl/modules"   // 用户级模块路径
    };
}

CjmodCompilerExtension::~CjmodCompilerExtension() = default;

CjmodCompilerResult CjmodCompilerExtension::executeCommand(const CjmodCompilerOptions& options) {
    cjmodManager_->setVerbose(options.verbose);
    
    switch (options.command) {
        case CjmodCompilerCommand::PACK:
            return packModule(options);
        case CjmodCompilerCommand::UNPACK:
            return unpackModule(options);
        case CjmodCompilerCommand::VALIDATE:
            return validateModule(options);
        case CjmodCompilerCommand::INFO:
            return showModuleInfo(options);
        case CjmodCompilerCommand::CREATE:
            return createModuleTemplate(options);
        case CjmodCompilerCommand::LIST:
            return listModules(options);
        case CjmodCompilerCommand::COMPILE:
            return compileModule(options);
        case CjmodCompilerCommand::BUILD:
            return buildDynamicLibrary(options);
        default:
            CjmodCompilerResult result;
            result.message = "未知的命令类型";
            return result;
    }
}

CjmodCompilerOptions CjmodCompilerExtension::parseCommandLine(const std::vector<std::string>& args) {
    CjmodCompilerOptions options;
    
    if (args.empty()) {
        return options;
    }
    
    // 解析命令
    std::string commandStr = args[0];
    if (commandStr == "pack") {
        options.command = CjmodCompilerCommand::PACK;
    } else if (commandStr == "unpack") {
        options.command = CjmodCompilerCommand::UNPACK;
    } else if (commandStr == "validate") {
        options.command = CjmodCompilerCommand::VALIDATE;
    } else if (commandStr == "info") {
        options.command = CjmodCompilerCommand::INFO;
    } else if (commandStr == "create") {
        options.command = CjmodCompilerCommand::CREATE;
    } else if (commandStr == "list") {
        options.command = CjmodCompilerCommand::LIST;
    } else if (commandStr == "compile") {
        options.command = CjmodCompilerCommand::COMPILE;
    } else if (commandStr == "build") {
        options.command = CjmodCompilerCommand::BUILD;
    }
    
    // 解析选项
    for (size_t i = 1; i < args.size(); ++i) {
        const std::string& arg = args[i];
        
        if (arg == "-v" || arg == "--verbose") {
            options.verbose = true;
        } else if (arg == "-f" || arg == "--force") {
            options.force = true;
        } else if (arg == "--official") {
            options.useOfficialPrefix = true;
        } else if (arg == "-s" || arg == "--source") {
            if (i + 1 < args.size()) {
                options.sourceDir = args[++i];
            }
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < args.size()) {
                options.outputPath = args[++i];
            }
        } else if (arg == "-n" || arg == "--name") {
            if (i + 1 < args.size()) {
                options.moduleName = args[++i];
            }
        } else if (arg == "--author") {
            if (i + 1 < args.size()) {
                options.moduleAuthor = args[++i];
            }
        } else if (arg == "--description") {
            if (i + 1 < args.size()) {
                options.moduleDescription = args[++i];
            }
        } else if (arg == "--version") {
            if (i + 1 < args.size()) {
                options.moduleVersion = args[++i];
            }
        } else if (arg == "--license") {
            if (i + 1 < args.size()) {
                options.moduleLicense = args[++i];
            }
        } else if (arg == "--category") {
            if (i + 1 < args.size()) {
                options.moduleCategory = args[++i];
            }
        } else if (arg == "--cpp-std") {
            if (i + 1 < args.size()) {
                options.cppStandard = args[++i];
            }
        } else if (arg == "--compiler-flags") {
            if (i + 1 < args.size()) {
                options.compilerFlags = args[++i];
            }
        } else if (arg == "--include") {
            if (i + 1 < args.size()) {
                options.includePaths.push_back(args[++i]);
            }
        } else if (arg == "--lib") {
            if (i + 1 < args.size()) {
                options.libraries.push_back(args[++i]);
            }
        } else if (arg == "--js-ext") {
            if (i + 1 < args.size()) {
                options.jsExtensions.push_back(args[++i]);
            }
        } else {
            // 位置参数
            if (options.sourceDir.empty()) {
                options.sourceDir = arg;
            } else if (options.outputPath.empty()) {
                options.outputPath = arg;
            }
        }
    }
    
    return options;
}

CjmodCompilerResult CjmodCompilerExtension::packModule(const CjmodCompilerOptions& options) {
    CjmodCompilerResult result;
    
    if (options.sourceDir.empty()) {
        result.message = "错误: 缺少源目录参数";
        return result;
    }
    
    std::string outputPath = options.outputPath;
    if (outputPath.empty()) {
        std::string moduleName = std::filesystem::path(options.sourceDir).filename().string();
        outputPath = moduleName + ".cjmod";
    }
    
    // 检查输出文件是否存在
    if (std::filesystem::exists(outputPath) && !options.force) {
        result.message = "错误: 输出文件已存在，使用 --force 强制覆盖";
        return result;
    }
    
    auto packResult = cjmodManager_->packModule(options.sourceDir, outputPath);
    
    result.success = packResult.success;
    if (packResult.success) {
        result.message = "CJmod模块打包成功: " + outputPath;
        result.outputs.push_back("输出文件: " + packResult.outputPath);
        result.outputs.push_back("总文件数: " + std::to_string(packResult.totalFiles));
        result.outputs.push_back("C++文件数: " + std::to_string(packResult.cppFiles));
        result.outputs.push_back("头文件数: " + std::to_string(packResult.headerFiles));
        result.outputs.push_back("信息文件数: " + std::to_string(packResult.infoFiles));
        result.outputs.push_back("总大小: " + std::to_string(packResult.totalSize) + " bytes");
    } else {
        result.message = "CJmod模块打包失败: " + packResult.errorMessage;
    }
    
    return result;
}

CjmodCompilerResult CjmodCompilerExtension::unpackModule(const CjmodCompilerOptions& options) {
    CjmodCompilerResult result;
    
    if (options.sourceDir.empty()) {
        result.message = "错误: 缺少CJmod文件路径参数";
        return result;
    }
    
    std::string outputDir = options.outputPath;
    if (outputDir.empty()) {
        std::string baseName = std::filesystem::path(options.sourceDir).stem().string();
        outputDir = baseName;
    }
    
    // 检查输出目录是否存在
    if (std::filesystem::exists(outputDir) && !options.force) {
        result.message = "错误: 输出目录已存在，使用 --force 强制覆盖";
        return result;
    }
    
    auto unpackResult = cjmodManager_->unpackModule(options.sourceDir, outputDir);
    
    result.success = unpackResult.success;
    if (unpackResult.success) {
        result.message = "CJmod模块解包成功: " + outputDir;
        result.outputs.push_back("输出目录: " + unpackResult.outputDir);
        result.outputs.push_back("解压文件数: " + std::to_string(unpackResult.extractedFiles.size()));
        result.moduleInfo = unpackResult.moduleInfo;
        
        if (!result.moduleInfo.name.empty()) {
            result.outputs.push_back("模块名称: " + result.moduleInfo.name);
            result.outputs.push_back("模块版本: " + result.moduleInfo.version);
            result.outputs.push_back("C++标准: " + result.moduleInfo.cppStandard);
        }
    } else {
        result.message = "CJmod模块解包失败: " + unpackResult.errorMessage;
    }
    
    return result;
}

CjmodCompilerResult CjmodCompilerExtension::validateModule(const CjmodCompilerOptions& options) {
    CjmodCompilerResult result;
    
    if (options.sourceDir.empty()) {
        result.message = "错误: 缺少源目录参数";
        return result;
    }
    
    auto validation = cjmodManager_->validateModuleStructure(options.sourceDir);
    
    result.success = validation.isValid;
    if (validation.isValid) {
        result.message = "CJmod模块结构验证通过";
        result.moduleInfo = validation.moduleInfo;
        
        if (!result.moduleInfo.name.empty()) {
            result.outputs.push_back("模块名称: " + result.moduleInfo.name);
            result.outputs.push_back("模块版本: " + result.moduleInfo.version);
            result.outputs.push_back("模块作者: " + result.moduleInfo.author);
            result.outputs.push_back("模块描述: " + result.moduleInfo.description);
            result.outputs.push_back("C++标准: " + result.moduleInfo.cppStandard);
            
            if (!result.moduleInfo.jsExtensions.empty()) {
                result.outputs.push_back("JS扩展: " + std::to_string(result.moduleInfo.jsExtensions.size()) + " 项");
            }
        }
        
        for (const auto& warning : validation.warnings) {
            result.outputs.push_back("警告: " + warning);
        }
    } else {
        result.message = "CJmod模块结构验证失败: " + validation.errorMessage;
        
        for (const auto& missingFile : validation.missingFiles) {
            result.outputs.push_back("缺失文件: " + missingFile);
        }
        
        for (const auto& warning : validation.warnings) {
            result.outputs.push_back("警告: " + warning);
        }
    }
    
    return result;
}

CjmodCompilerResult CjmodCompilerExtension::showModuleInfo(const CjmodCompilerOptions& options) {
    CjmodCompilerResult result;
    
    if (options.sourceDir.empty()) {
        result.message = "错误: 缺少CJmod文件路径参数";
        return result;
    }
    
    auto moduleInfo = cjmodManager_->readModuleInfo(options.sourceDir);
    
    if (!moduleInfo.name.empty()) {
        result.success = true;
        result.message = "CJmod模块信息";
        result.moduleInfo = moduleInfo;
        
        result.outputs.push_back("模块名称: " + moduleInfo.name);
        result.outputs.push_back("版本: " + moduleInfo.version);
        result.outputs.push_back("作者: " + moduleInfo.author);
        result.outputs.push_back("描述: " + moduleInfo.description);
        result.outputs.push_back("许可证: " + moduleInfo.license);
        result.outputs.push_back("分类: " + moduleInfo.category);
        result.outputs.push_back("最小CHTL版本: " + moduleInfo.minCHTLVersion);
        result.outputs.push_back("C++标准: " + moduleInfo.cppStandard);
        
        if (!moduleInfo.compilerFlags.empty()) {
            result.outputs.push_back("编译器标志: " + moduleInfo.compilerFlags);
        }
        if (!moduleInfo.includePaths.empty()) {
            result.outputs.push_back("包含路径: " + std::to_string(moduleInfo.includePaths.size()) + " 项");
        }
        if (!moduleInfo.libraries.empty()) {
            result.outputs.push_back("链接库: " + std::to_string(moduleInfo.libraries.size()) + " 项");
        }
        if (!moduleInfo.jsExtensions.empty()) {
            result.outputs.push_back("JS扩展: " + std::to_string(moduleInfo.jsExtensions.size()) + " 项");
            for (const auto& ext : moduleInfo.jsExtensions) {
                result.outputs.push_back("  - " + ext);
            }
        }
        if (!moduleInfo.dependencies.empty()) {
            result.outputs.push_back("依赖: " + moduleInfo.dependencies);
        }
        if (!moduleInfo.homepage.empty()) {
            result.outputs.push_back("主页: " + moduleInfo.homepage);
        }
        if (!moduleInfo.repository.empty()) {
            result.outputs.push_back("仓库: " + moduleInfo.repository);
        }
    } else {
        result.message = "无法读取CJmod模块信息，可能不是有效的CJmod文件";
    }
    
    return result;
}

CjmodCompilerResult CjmodCompilerExtension::createModuleTemplate(const CjmodCompilerOptions& options) {
    CjmodCompilerResult result;
    
    if (options.moduleName.empty()) {
        result.message = "错误: 缺少模块名称参数 (--name)";
        return result;
    }
    
    std::string moduleDir = options.outputPath;
    if (moduleDir.empty()) {
        moduleDir = options.moduleName;
    }
    
    // 检查目录是否存在
    if (std::filesystem::exists(moduleDir) && !options.force) {
        result.message = "错误: 目录已存在，使用 --force 强制覆盖";
        return result;
    }
    
    // 创建目录结构
    if (!createDirectoryStructure(moduleDir, options)) {
        result.message = "创建目录结构失败";
        return result;
    }
    
    result.success = true;
    result.message = "CJmod模块模板创建成功: " + moduleDir;
    result.outputs.push_back("创建目录: " + moduleDir);
    result.outputs.push_back("创建目录: " + moduleDir + "/src");
    result.outputs.push_back("创建目录: " + moduleDir + "/info");
    result.outputs.push_back("创建文件: " + moduleDir + "/src/" + options.moduleName + ".cpp");
    result.outputs.push_back("创建文件: " + moduleDir + "/src/" + options.moduleName + ".hpp");
    result.outputs.push_back("创建文件: " + moduleDir + "/info/" + options.moduleName + ".chtl");
    
    if (options.useOfficialPrefix) {
        result.outputs.push_back("✨ 已配置为官方模块前缀: chtl::" + options.moduleName);
    }
    
    return result;
}

CjmodCompilerResult CjmodCompilerExtension::listModules(const CjmodCompilerOptions& options) {
    CjmodCompilerResult result;
    
    std::string searchDir = options.sourceDir;
    if (searchDir.empty()) {
        searchDir = "module";
    }
    
    auto cjmodFiles = findCjmodFiles(searchDir);
    
    // 列出官方模块
    auto officialModules = prefixManager_->listOfficialModules("cjmod");
    
    result.success = true;
    result.message = "找到 " + std::to_string(cjmodFiles.size() + officialModules.size()) + " 个CJmod模块";
    
    if (!officialModules.empty()) {
        result.outputs.push_back("📦 官方模块:");
        for (const auto& officialModule : officialModules) {
            result.outputs.push_back("  " + officialModule);
        }
        result.outputs.push_back("");
    }
    
    if (!cjmodFiles.empty()) {
        result.outputs.push_back("📁 本地模块:");
        for (const auto& cjmodFile : cjmodFiles) {
            auto moduleInfo = cjmodManager_->readModuleInfo(cjmodFile);
            
            if (!moduleInfo.name.empty()) {
                std::string info = moduleInfo.name + " v" + moduleInfo.version;
                if (!moduleInfo.description.empty()) {
                    info += " - " + moduleInfo.description;
                }
                if (!moduleInfo.jsExtensions.empty()) {
                    info += " [" + std::to_string(moduleInfo.jsExtensions.size()) + " JS扩展]";
                }
                info += " (" + cjmodFile + ")";
                result.outputs.push_back("  " + info);
            } else {
                result.outputs.push_back("  无效模块: " + cjmodFile);
            }
        }
    }
    
    return result;
}

CjmodCompilerResult CjmodCompilerExtension::compileModule(const CjmodCompilerOptions& options) {
    CjmodCompilerResult result;
    
    if (options.sourceDir.empty()) {
        result.message = "错误: 缺少源目录参数";
        return result;
    }
    
    std::string outputDir = options.outputPath;
    if (outputDir.empty()) {
        outputDir = options.sourceDir + "_compiled";
    }
    
    bool success = cjmodManager_->compileCjmodModule(options.sourceDir, outputDir);
    
    result.success = success;
    if (success) {
        result.message = "CJmod模块编译成功: " + outputDir;
        result.outputs.push_back("输出目录: " + outputDir);
    } else {
        result.message = "CJmod模块编译失败";
    }
    
    return result;
}

CjmodCompilerResult CjmodCompilerExtension::buildDynamicLibrary(const CjmodCompilerOptions& options) {
    CjmodCompilerResult result;
    
    if (options.sourceDir.empty()) {
        result.message = "错误: 缺少源目录参数";
        return result;
    }
    
    std::string outputPath = options.outputPath;
    if (outputPath.empty()) {
        std::string moduleName = std::filesystem::path(options.sourceDir).filename().string();
        outputPath = "lib" + moduleName + ".so";
    }
    
    bool success = cjmodManager_->buildDynamicLibrary(options.sourceDir, outputPath);
    
    result.success = success;
    if (success) {
        result.message = "CJmod动态库构建成功: " + outputPath;
        result.outputs.push_back("输出文件: " + outputPath);
    } else {
        result.message = "CJmod动态库构建失败";
    }
    
    return result;
}

void CjmodCompilerExtension::registerWithImportManager(EnhancedImportManager* importManager) {
    importManager_ = importManager;
    if (importManager_) {
        importManager_->setOfficialModuleRoot(prefixManager_->getOfficialModuleRoot());
    }
}

void CjmodCompilerExtension::registerWithNamespaceManager(EnhancedNamespaceManager* namespaceManager) {
    namespaceManager_ = namespaceManager;
}

std::string CjmodCompilerExtension::resolveCjmodImport(const std::string& moduleName, const std::vector<std::string>& searchPaths) {
    // 首先尝试官方模块解析
    std::string resolvedPath = prefixManager_->resolveModuleImport(moduleName, "cjmod");
    if (!resolvedPath.empty()) {
        return resolvedPath;
    }
    
    // 组合搜索路径
    std::vector<std::string> allPaths = searchPaths;
    allPaths.insert(allPaths.end(), moduleSearchPaths_.begin(), moduleSearchPaths_.end());
    
    for (const auto& searchPath : allPaths) {
        // 查找.cjmod文件
        std::string cjmodPath = searchPath + "/" + moduleName + ".cjmod";
        if (std::filesystem::exists(cjmodPath)) {
            return cjmodPath;
        }
        
        // 查找目录
        std::string dirPath = searchPath + "/" + moduleName;
        if (std::filesystem::exists(dirPath) && std::filesystem::is_directory(dirPath)) {
            return dirPath;
        }
    }
    
    return "";
}

std::string CjmodCompilerExtension::autoUnpackAndCompile(const std::string& cjmodPath, const std::string& cacheDir) {
    if (!std::filesystem::exists(cjmodPath)) {
        return "";
    }
    
    std::string cacheKey = generateCacheKey(cjmodPath);
    std::string cachedDir = cacheDir + "/" + cacheKey;
    std::string dylibPath = cachedDir + ".so";
    
    // 检查缓存是否有效
    if (isCacheValid(dylibPath, cjmodPath)) {
        return dylibPath;
    }
    
    // 解包模块
    auto unpackResult = cjmodManager_->unpackModule(cjmodPath, cachedDir);
    if (!unpackResult.success) {
        return "";
    }
    
    // 编译动态库
    bool success = cjmodManager_->buildDynamicLibrary(cachedDir, dylibPath);
    if (success) {
        return dylibPath;
    }
    
    return "";
}

UnifiedModulePrefixManager* CjmodCompilerExtension::getModulePrefixManager() {
    return prefixManager_.get();
}

std::string CjmodCompilerExtension::getHelpText() const {
    std::stringstream ss;
    
    ss << "CHTL CJmod 工具 - C++ JS扩展模块打包和管理工具\n\n";
    ss << "用法:\n";
    ss << "  chtl-cjmod <命令> [选项] [参数]\n\n";
    ss << "命令:\n";
    ss << "  pack       打包模块文件夹为.cjmod文件\n";
    ss << "  unpack     解包.cjmod文件到文件夹\n";
    ss << "  validate   验证模块文件夹结构\n";
    ss << "  info       显示模块信息\n";
    ss << "  create     创建模块模板\n";
    ss << "  list       列出可用模块\n";
    ss << "  compile    编译模块\n";
    ss << "  build      构建动态库\n\n";
    ss << "选项:\n";
    ss << "  -s, --source <路径>     源目录或文件路径\n";
    ss << "  -o, --output <路径>     输出路径\n";
    ss << "  -n, --name <名称>       模块名称\n";
    ss << "  -v, --verbose           详细输出\n";
    ss << "  -f, --force             强制覆盖\n";
    ss << "  --official              使用官方模块前缀 chtl::\n";
    ss << "  --author <作者>         模块作者\n";
    ss << "  --description <描述>    模块描述\n";
    ss << "  --version <版本>        模块版本\n";
    ss << "  --license <许可证>      模块许可证\n";
    ss << "  --category <分类>       模块分类\n";
    ss << "  --cpp-std <标准>        C++标准 (如 C++17)\n";
    ss << "  --compiler-flags <标志> 编译器标志\n";
    ss << "  --include <路径>        包含路径 (可多次使用)\n";
    ss << "  --lib <库>              链接库 (可多次使用)\n";
    ss << "  --js-ext <扩展>         JS语法扩展 (可多次使用)\n\n";
    ss << "示例:\n";
    ss << "  chtl-cjmod create --name MyExtension --official    # 创建官方CJmod模块\n";
    ss << "  chtl-cjmod pack MyExtension                        # 打包模块\n";
    ss << "  chtl-cjmod build MyExtension -o libMyExt.so        # 构建动态库\n";
    ss << "  chtl-cjmod unpack MyExtension.cjmod               # 解包模块\n";
    ss << "  chtl-cjmod validate MyExtension                   # 验证模块\n";
    ss << "  chtl-cjmod info MyExtension.cjmod                 # 显示模块信息\n";
    ss << "  chtl-cjmod list                                   # 列出所有模块\n";
    
    return ss.str();
}

std::string CjmodCompilerExtension::getVersionText() const {
    return "CHTL CJmod工具 v1.0.0\n基于CHTL编译器 - 严格遵循CHTL语法文档\nC++ CHTL JS语法扩展模块管理工具";
}

void CjmodCompilerExtension::setModuleSearchPaths(const std::vector<std::string>& paths) {
    moduleSearchPaths_ = paths;
}

const std::vector<std::string>& CjmodCompilerExtension::getModuleSearchPaths() const {
    return moduleSearchPaths_;
}

void CjmodCompilerExtension::setCacheDirectory(const std::string& cacheDir) {
    cacheDirectory_ = cacheDir;
}

bool CjmodCompilerExtension::cleanCache() {
    try {
        if (std::filesystem::exists(cacheDirectory_)) {
            std::filesystem::remove_all(cacheDirectory_);
        }
        return true;
    } catch (...) {
        return false;
    }
}

void CjmodCompilerExtension::printResult(const CjmodCompilerResult& result) const {
    if (result.success) {
        std::cout << "✅ " << result.message << std::endl;
    } else {
        std::cout << "❌ " << result.message << std::endl;
    }
    
    for (const auto& output : result.outputs) {
        std::cout << "   " << output << std::endl;
    }
}

// 私有方法实现

bool CjmodCompilerExtension::createDirectoryStructure(const std::string& moduleDir, const CjmodCompilerOptions& options) {
    try {
        // 创建主目录
        std::filesystem::create_directories(moduleDir);
        std::filesystem::create_directories(moduleDir + "/src");
        std::filesystem::create_directories(moduleDir + "/info");
        
        // 创建C++源文件
        std::string cppContent = createMainCppFile(options);
        std::ofstream cppFile(moduleDir + "/src/" + options.moduleName + ".cpp");
        cppFile << cppContent;
        
        // 创建头文件
        std::string headerContent = createHeaderFile(options);
        std::ofstream headerFile(moduleDir + "/src/" + options.moduleName + ".hpp");
        headerFile << headerContent;
        
        // 创建Info文件
        std::string infoChtlContent = createInfoChtlFile(options);
        std::ofstream infoFile(moduleDir + "/info/" + options.moduleName + ".chtl");
        infoFile << infoChtlContent;
        
        return true;
    } catch (...) {
        return false;
    }
}

std::string CjmodCompilerExtension::createMainCppFile(const CjmodCompilerOptions& options) {
    std::stringstream ss;
    
    ss << "// " << options.moduleName << " - CJmod C++实现文件\n";
    ss << "// 作者: " << (options.moduleAuthor.empty() ? "Unknown" : options.moduleAuthor) << "\n";
    ss << "// 描述: " << (options.moduleDescription.empty() ? "CHTL JS扩展模块" : options.moduleDescription) << "\n\n";
    
    ss << "#include \"" << options.moduleName << ".hpp\"\n";
    ss << "#include <iostream>\n";
    ss << "#include <string>\n";
    ss << "#include <vector>\n\n";
    
    ss << "namespace chtl {\n";
    ss << "namespace cjmod {\n\n";
    
    ss << "// " << options.moduleName << "扩展实现\n";
    ss << "class " << options.moduleName << "Extension : public ChtlJsExtensionInterface {\n";
    ss << "public:\n";
    ss << "    " << options.moduleName << "Extension() = default;\n";
    ss << "    virtual ~" << options.moduleName << "Extension() = default;\n\n";
    
    ss << "    bool initialize(void* context) override {\n";
    ss << "        // 初始化扩展\n";
    ss << "        std::cout << \"初始化 " << options.moduleName << " CJmod扩展\" << std::endl;\n";
    ss << "        return true;\n";
    ss << "    }\n\n";
    
    ss << "    std::string processCustomSyntax(const std::string& syntax, const std::vector<std::string>& parameters) override {\n";
    ss << "        // 处理自定义JS语法\n";
    ss << "        if (syntax == \"example_syntax\") {\n";
    ss << "            return \"console.log('Hello from " << options.moduleName << "!');\";\n";
    ss << "        }\n";
    ss << "        return \"\";\n";
    ss << "    }\n\n";
    
    ss << "    bool registerCustomFunction(const std::string& functionName, void* implementation) override {\n";
    ss << "        // 注册自定义函数\n";
    ss << "        return true;\n";
    ss << "    }\n\n";
    
    ss << "    std::string getExtensionName() const override {\n";
    ss << "        return \"" << options.moduleName << "\";\n";
    ss << "    }\n\n";
    
    ss << "    std::string getExtensionVersion() const override {\n";
    ss << "        return \"" << options.moduleVersion << "\";\n";
    ss << "    }\n\n";
    
    ss << "    void cleanup() override {\n";
    ss << "        // 清理扩展\n";
    ss << "        std::cout << \"清理 " << options.moduleName << " CJmod扩展\" << std::endl;\n";
    ss << "    }\n";
    ss << "};\n\n";
    
    ss << "// 导出函数 - CHTL编译器会调用这些函数\n";
    ss << "extern \"C\" {\n";
    ss << "    ChtlJsExtensionInterface* create_extension() {\n";
    ss << "        return new " << options.moduleName << "Extension();\n";
    ss << "    }\n\n";
    
    ss << "    void destroy_extension(ChtlJsExtensionInterface* extension) {\n";
    ss << "        delete extension;\n";
    ss << "    }\n";
    ss << "}\n\n";
    
    ss << "} // namespace cjmod\n";
    ss << "} // namespace chtl\n";
    
    return ss.str();
}

std::string CjmodCompilerExtension::createHeaderFile(const CjmodCompilerOptions& options) {
    std::stringstream ss;
    
    ss << "// " << options.moduleName << " - CJmod头文件\n";
    ss << "// 作者: " << (options.moduleAuthor.empty() ? "Unknown" : options.moduleAuthor) << "\n";
    ss << "// 描述: " << (options.moduleDescription.empty() ? "CHTL JS扩展模块" : options.moduleDescription) << "\n\n";
    
    std::string guard = options.moduleName;
    std::transform(guard.begin(), guard.end(), guard.begin(), ::toupper);
    guard = "CHTL_CJMOD_" + guard + "_HPP";
    
    ss << "#pragma once\n";
    ss << "#ifndef " << guard << "\n";
    ss << "#define " << guard << "\n\n";
    
    ss << "#include \"cjmod/cjmod_manager.hpp\"\n";
    ss << "#include <string>\n";
    ss << "#include <vector>\n\n";
    
    ss << "namespace chtl {\n";
    ss << "namespace cjmod {\n\n";
    
    ss << "/**\n";
    ss << " * " << options.moduleName << " CJmod扩展\n";
    ss << " * 提供CHTL JS语法扩展功能\n";
    ss << " */\n";
    ss << "class " << options.moduleName << "Extension;\n\n";
    
    if (!options.jsExtensions.empty()) {
        ss << "// 支持的JS语法扩展:\n";
        for (const auto& ext : options.jsExtensions) {
            ss << "// - " << ext << "\n";
        }
        ss << "\n";
    }
    
    ss << "} // namespace cjmod\n";
    ss << "} // namespace chtl\n\n";
    
    ss << "#endif // " << guard << "\n";
    
    return ss.str();
}

std::string CjmodCompilerExtension::createInfoChtlFile(const CjmodCompilerOptions& options) {
    CjmodInfo info;
    info.name = options.moduleName;
    info.version = options.moduleVersion;
    info.description = options.moduleDescription.empty() ? "CHTL JS扩展模块" : options.moduleDescription;
    info.author = options.moduleAuthor.empty() ? "Unknown" : options.moduleAuthor;
    info.license = options.moduleLicense;
    info.category = options.moduleCategory;
    info.minCHTLVersion = "1.0.0";
    info.cppStandard = options.cppStandard;
    info.compilerFlags = options.compilerFlags;
    info.includePaths = options.includePaths;
    info.libraries = options.libraries;
    info.jsExtensions = options.jsExtensions;
    
    return cjmodManager_->generateInfoFile(info);
}

std::vector<std::string> CjmodCompilerExtension::findCjmodFiles(const std::string& directory) {
    std::vector<std::string> cjmodFiles;
    
    if (!std::filesystem::exists(directory)) {
        return cjmodFiles;
    }
    
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
            if (entry.path().extension() == ".cjmod") {
                cjmodFiles.push_back(entry.path().string());
            }
        }
    } catch (...) {
        // 忽略错误
    }
    
    return cjmodFiles;
}

std::string CjmodCompilerExtension::generateCacheKey(const std::string& cjmodPath) {
    // 使用文件名和修改时间生成缓存键
    std::filesystem::path path(cjmodPath);
    std::string baseName = path.stem().string();
    
    try {
        auto lastWriteTime = std::filesystem::last_write_time(cjmodPath);
        auto timeT = std::chrono::duration_cast<std::chrono::seconds>(
            lastWriteTime.time_since_epoch()).count();
        
        return baseName + "_" + std::to_string(timeT);
    } catch (...) {
        return baseName + "_unknown";
    }
}

bool CjmodCompilerExtension::isCacheValid(const std::string& cacheDir, const std::string& cjmodPath) {
    if (!std::filesystem::exists(cacheDir)) {
        return false;
    }
    
    try {
        auto cacheTime = std::filesystem::last_write_time(cacheDir);
        auto cjmodTime = std::filesystem::last_write_time(cjmodPath);
        
        return cacheTime >= cjmodTime;
    } catch (...) {
        return false;
    }
}

void CjmodCompilerExtension::printUsage() const {
    std::cout << getHelpText() << std::endl;
}

// ========== CjmodCommandLineInterface 实现 ==========

CjmodCommandLineInterface::CjmodCommandLineInterface() 
    : extension_(std::make_shared<CjmodCompilerExtension>()) {
}

CjmodCommandLineInterface::~CjmodCommandLineInterface() = default;

int CjmodCommandLineInterface::run(int argc, char* argv[]) {
    printBanner();
    
    if (argc < 2) {
        printUsage();
        return 1;
    }
    
    auto args = parseArguments(argc, argv);
    
    if (args.empty()) {
        printUsage();
        return 1;
    }
    
    // 检查帮助和版本
    if (args[0] == "help" || args[0] == "--help" || args[0] == "-h") {
        std::cout << extension_->getHelpText() << std::endl;
        return 0;
    }
    
    if (args[0] == "version" || args[0] == "--version") {
        std::cout << extension_->getVersionText() << std::endl;
        return 0;
    }
    
    auto options = extension_->parseCommandLine(args);
    auto result = extension_->executeCommand(options);
    
    extension_->printResult(result);
    
    return result.success ? 0 : 1;
}

void CjmodCommandLineInterface::setCompilerExtension(std::shared_ptr<CjmodCompilerExtension> extension) {
    extension_ = extension;
}

void CjmodCommandLineInterface::printBanner() const {
    std::cout << "🚀 CHTL CJmod工具 - C++ JS扩展模块管理" << std::endl;
    std::cout << "严格遵循CHTL语法文档 - 支持官方模块前缀 chtl::" << std::endl;
    std::cout << "========================================" << std::endl;
}

void CjmodCommandLineInterface::printUsage() const {
    std::cout << "\n用法: chtl-cjmod <命令> [选项] [参数]" << std::endl;
    std::cout << "使用 'chtl-cjmod help' 查看详细帮助" << std::endl;
}

std::vector<std::string> CjmodCommandLineInterface::parseArguments(int argc, char* argv[]) {
    std::vector<std::string> args;
    
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    
    return args;
}

} // namespace chtl
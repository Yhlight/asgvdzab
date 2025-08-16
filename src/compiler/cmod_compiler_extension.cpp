#include "compiler/cmod_compiler_extension.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <regex>

namespace chtl {

// ========== CmodCompilerExtension 实现 ==========

CmodCompilerExtension::CmodCompilerExtension() 
    : cmodManager_(std::make_unique<CmodManager>()),
      importManager_(nullptr),
      namespaceManager_(nullptr),
      cacheDirectory_(".chtl_cache") {
    
    // 设置默认搜索路径
    moduleSearchPaths_ = {
        "module",           // 当前目录下的module文件夹
        "./module",         // 相对路径
        "../module",        // 上级目录
        "/usr/local/share/chtl/modules",  // 系统级模块路径
        "~/.chtl/modules"   // 用户级模块路径
    };
}

CmodCompilerExtension::~CmodCompilerExtension() = default;

CmodCompilerResult CmodCompilerExtension::executeCommand(const CmodCompilerOptions& options) {
    cmodManager_->setVerbose(options.verbose);
    
    switch (options.command) {
        case CmodCompilerCommand::PACK:
            return packModule(options);
        case CmodCompilerCommand::UNPACK:
            return unpackModule(options);
        case CmodCompilerCommand::VALIDATE:
            return validateModule(options);
        case CmodCompilerCommand::INFO:
            return showModuleInfo(options);
        case CmodCompilerCommand::CREATE:
            return createModuleTemplate(options);
        case CmodCompilerCommand::LIST:
            return listModules(options);
        default:
            CmodCompilerResult result;
            result.message = "未知的命令类型";
            return result;
    }
}

CmodCompilerOptions CmodCompilerExtension::parseCommandLine(const std::vector<std::string>& args) {
    CmodCompilerOptions options;
    
    if (args.empty()) {
        return options;
    }
    
    // 解析命令
    std::string commandStr = args[0];
    if (commandStr == "pack") {
        options.command = CmodCompilerCommand::PACK;
    } else if (commandStr == "unpack") {
        options.command = CmodCompilerCommand::UNPACK;
    } else if (commandStr == "validate") {
        options.command = CmodCompilerCommand::VALIDATE;
    } else if (commandStr == "info") {
        options.command = CmodCompilerCommand::INFO;
    } else if (commandStr == "create") {
        options.command = CmodCompilerCommand::CREATE;
    } else if (commandStr == "list") {
        options.command = CmodCompilerCommand::LIST;
    }
    
    // 解析选项
    for (size_t i = 1; i < args.size(); ++i) {
        const std::string& arg = args[i];
        
        if (arg == "-v" || arg == "--verbose") {
            options.verbose = true;
        } else if (arg == "-f" || arg == "--force") {
            options.force = true;
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

CmodCompilerResult CmodCompilerExtension::packModule(const CmodCompilerOptions& options) {
    CmodCompilerResult result;
    
    if (options.sourceDir.empty()) {
        result.message = "错误: 缺少源目录参数";
        return result;
    }
    
    std::string outputPath = options.outputPath;
    if (outputPath.empty()) {
        std::string moduleName = std::filesystem::path(options.sourceDir).filename().string();
        outputPath = moduleName + ".cmod";
    }
    
    // 检查输出文件是否存在
    if (std::filesystem::exists(outputPath) && !options.force) {
        result.message = "错误: 输出文件已存在，使用 --force 强制覆盖";
        return result;
    }
    
    auto packResult = cmodManager_->packModule(options.sourceDir, outputPath);
    
    result.success = packResult.success;
    if (packResult.success) {
        result.message = "模块打包成功: " + outputPath;
        result.outputs.push_back("输出文件: " + packResult.outputPath);
        result.outputs.push_back("总文件数: " + std::to_string(packResult.totalFiles));
        result.outputs.push_back("总大小: " + std::to_string(packResult.totalSize) + " bytes");
    } else {
        result.message = "模块打包失败: " + packResult.errorMessage;
    }
    
    return result;
}

CmodCompilerResult CmodCompilerExtension::unpackModule(const CmodCompilerOptions& options) {
    CmodCompilerResult result;
    
    if (options.sourceDir.empty()) {
        result.message = "错误: 缺少Cmod文件路径参数";
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
    
    auto unpackResult = cmodManager_->unpackModule(options.sourceDir, outputDir);
    
    result.success = unpackResult.success;
    if (unpackResult.success) {
        result.message = "模块解包成功: " + outputDir;
        result.outputs.push_back("输出目录: " + unpackResult.outputDir);
        result.outputs.push_back("解压文件数: " + std::to_string(unpackResult.extractedFiles.size()));
        result.moduleInfo = unpackResult.moduleInfo;
        
        if (!result.moduleInfo.name.empty()) {
            result.outputs.push_back("模块名称: " + result.moduleInfo.name);
            result.outputs.push_back("模块版本: " + result.moduleInfo.version);
        }
    } else {
        result.message = "模块解包失败: " + unpackResult.errorMessage;
    }
    
    return result;
}

CmodCompilerResult CmodCompilerExtension::validateModule(const CmodCompilerOptions& options) {
    CmodCompilerResult result;
    
    if (options.sourceDir.empty()) {
        result.message = "错误: 缺少源目录参数";
        return result;
    }
    
    auto validation = cmodManager_->validateModuleStructure(options.sourceDir);
    
    result.success = validation.isValid;
    if (validation.isValid) {
        result.message = "模块结构验证通过";
        result.moduleInfo = validation.moduleInfo;
        
        if (!result.moduleInfo.name.empty()) {
            result.outputs.push_back("模块名称: " + result.moduleInfo.name);
            result.outputs.push_back("模块版本: " + result.moduleInfo.version);
            result.outputs.push_back("模块作者: " + result.moduleInfo.author);
            result.outputs.push_back("模块描述: " + result.moduleInfo.description);
        }
        
        for (const auto& warning : validation.warnings) {
            result.outputs.push_back("警告: " + warning);
        }
    } else {
        result.message = "模块结构验证失败: " + validation.errorMessage;
        
        for (const auto& missingFile : validation.missingFiles) {
            result.outputs.push_back("缺失文件: " + missingFile);
        }
        
        for (const auto& warning : validation.warnings) {
            result.outputs.push_back("警告: " + warning);
        }
    }
    
    return result;
}

CmodCompilerResult CmodCompilerExtension::showModuleInfo(const CmodCompilerOptions& options) {
    CmodCompilerResult result;
    
    if (options.sourceDir.empty()) {
        result.message = "错误: 缺少Cmod文件路径参数";
        return result;
    }
    
    auto moduleInfo = cmodManager_->readModuleInfo(options.sourceDir);
    
    if (!moduleInfo.name.empty()) {
        result.success = true;
        result.message = "模块信息";
        result.moduleInfo = moduleInfo;
        
        result.outputs.push_back("模块名称: " + moduleInfo.name);
        result.outputs.push_back("版本: " + moduleInfo.version);
        result.outputs.push_back("作者: " + moduleInfo.author);
        result.outputs.push_back("描述: " + moduleInfo.description);
        result.outputs.push_back("许可证: " + moduleInfo.license);
        result.outputs.push_back("分类: " + moduleInfo.category);
        result.outputs.push_back("最小CHTL版本: " + moduleInfo.minCHTLVersion);
        
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
        result.message = "无法读取模块信息，可能不是有效的Cmod文件";
    }
    
    return result;
}

CmodCompilerResult CmodCompilerExtension::createModuleTemplate(const CmodCompilerOptions& options) {
    CmodCompilerResult result;
    
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
    result.message = "模块模板创建成功: " + moduleDir;
    result.outputs.push_back("创建目录: " + moduleDir);
    result.outputs.push_back("创建目录: " + moduleDir + "/src");
    result.outputs.push_back("创建目录: " + moduleDir + "/info");
    result.outputs.push_back("创建文件: " + moduleDir + "/src/" + options.moduleName + ".chtl");
    result.outputs.push_back("创建文件: " + moduleDir + "/info/" + options.moduleName + ".chtl");
    
    return result;
}

CmodCompilerResult CmodCompilerExtension::listModules(const CmodCompilerOptions& options) {
    CmodCompilerResult result;
    
    std::string searchDir = options.sourceDir;
    if (searchDir.empty()) {
        searchDir = "module";
    }
    
    auto cmodFiles = findCmodFiles(searchDir);
    
    result.success = true;
    result.message = "找到 " + std::to_string(cmodFiles.size()) + " 个Cmod模块";
    
    for (const auto& cmodFile : cmodFiles) {
        auto moduleInfo = cmodManager_->readModuleInfo(cmodFile);
        
        if (!moduleInfo.name.empty()) {
            std::string info = moduleInfo.name + " v" + moduleInfo.version;
            if (!moduleInfo.description.empty()) {
                info += " - " + moduleInfo.description;
            }
            info += " (" + cmodFile + ")";
            result.outputs.push_back(info);
        } else {
            result.outputs.push_back("无效模块: " + cmodFile);
        }
    }
    
    return result;
}

void CmodCompilerExtension::registerWithImportManager(EnhancedImportManager* importManager) {
    importManager_ = importManager;
}

void CmodCompilerExtension::registerWithNamespaceManager(EnhancedNamespaceManager* namespaceManager) {
    namespaceManager_ = namespaceManager;
}

std::string CmodCompilerExtension::resolveCmodImport(const std::string& moduleName, const std::vector<std::string>& searchPaths) {
    // 组合搜索路径
    std::vector<std::string> allPaths = searchPaths;
    allPaths.insert(allPaths.end(), moduleSearchPaths_.begin(), moduleSearchPaths_.end());
    
    for (const auto& searchPath : allPaths) {
        // 查找.cmod文件
        std::string cmodPath = searchPath + "/" + moduleName + ".cmod";
        if (std::filesystem::exists(cmodPath)) {
            return cmodPath;
        }
        
        // 查找目录
        std::string dirPath = searchPath + "/" + moduleName;
        if (std::filesystem::exists(dirPath) && std::filesystem::is_directory(dirPath)) {
            return dirPath;
        }
    }
    
    return "";
}

std::string CmodCompilerExtension::autoUnpackAndCache(const std::string& cmodPath, const std::string& cacheDir) {
    if (!std::filesystem::exists(cmodPath)) {
        return "";
    }
    
    std::string cacheKey = generateCacheKey(cmodPath);
    std::string cachedDir = cacheDir + "/" + cacheKey;
    
    // 检查缓存是否有效
    if (isCacheValid(cachedDir, cmodPath)) {
        return cachedDir;
    }
    
    // 解包到缓存目录
    auto unpackResult = cmodManager_->unpackModule(cmodPath, cachedDir);
    
    if (unpackResult.success) {
        return cachedDir;
    }
    
    return "";
}

std::string CmodCompilerExtension::getHelpText() const {
    std::stringstream ss;
    
    ss << "CHTL Cmod 工具 - 模块打包和管理工具\n\n";
    ss << "用法:\n";
    ss << "  chtl-cmod <命令> [选项] [参数]\n\n";
    ss << "命令:\n";
    ss << "  pack       打包模块文件夹为.cmod文件\n";
    ss << "  unpack     解包.cmod文件到文件夹\n";
    ss << "  validate   验证模块文件夹结构\n";
    ss << "  info       显示模块信息\n";
    ss << "  create     创建模块模板\n";
    ss << "  list       列出可用模块\n\n";
    ss << "选项:\n";
    ss << "  -s, --source <路径>     源目录或文件路径\n";
    ss << "  -o, --output <路径>     输出路径\n";
    ss << "  -n, --name <名称>       模块名称\n";
    ss << "  -v, --verbose           详细输出\n";
    ss << "  -f, --force             强制覆盖\n";
    ss << "  --author <作者>         模块作者\n";
    ss << "  --description <描述>    模块描述\n";
    ss << "  --version <版本>        模块版本\n";
    ss << "  --license <许可证>      模块许可证\n";
    ss << "  --category <分类>       模块分类\n\n";
    ss << "示例:\n";
    ss << "  chtl-cmod pack MyModule              # 打包MyModule文件夹\n";
    ss << "  chtl-cmod unpack MyModule.cmod      # 解包MyModule.cmod\n";
    ss << "  chtl-cmod create --name MyModule    # 创建模块模板\n";
    ss << "  chtl-cmod validate MyModule         # 验证模块结构\n";
    ss << "  chtl-cmod info MyModule.cmod        # 显示模块信息\n";
    ss << "  chtl-cmod list                       # 列出可用模块\n";
    
    return ss.str();
}

std::string CmodCompilerExtension::getVersionText() const {
    return "CHTL Cmod工具 v1.0.0\n基于CHTL编译器 - 严格遵循CHTL语法文档";
}

void CmodCompilerExtension::setModuleSearchPaths(const std::vector<std::string>& paths) {
    moduleSearchPaths_ = paths;
}

const std::vector<std::string>& CmodCompilerExtension::getModuleSearchPaths() const {
    return moduleSearchPaths_;
}

void CmodCompilerExtension::setCacheDirectory(const std::string& cacheDir) {
    cacheDirectory_ = cacheDir;
}

bool CmodCompilerExtension::cleanCache() {
    try {
        if (std::filesystem::exists(cacheDirectory_)) {
            std::filesystem::remove_all(cacheDirectory_);
        }
        return true;
    } catch (...) {
        return false;
    }
}

// 私有方法实现

bool CmodCompilerExtension::createDirectoryStructure(const std::string& moduleDir, const CmodCompilerOptions& options) {
    try {
        // 创建主目录
        std::filesystem::create_directories(moduleDir);
        std::filesystem::create_directories(moduleDir + "/src");
        std::filesystem::create_directories(moduleDir + "/info");
        
        // 创建主CHTL文件
        std::string mainChtlContent = createMainChtlFile(options);
        std::ofstream mainFile(moduleDir + "/src/" + options.moduleName + ".chtl");
        mainFile << mainChtlContent;
        
        // 创建Info文件
        std::string infoChtlContent = createInfoChtlFile(options);
        std::ofstream infoFile(moduleDir + "/info/" + options.moduleName + ".chtl");
        infoFile << infoChtlContent;
        
        return true;
    } catch (...) {
        return false;
    }
}

std::string CmodCompilerExtension::createMainChtlFile(const CmodCompilerOptions& options) {
    std::stringstream ss;
    
    ss << "// " << options.moduleName << " - 主模块文件\n";
    ss << "// 作者: " << (options.moduleAuthor.empty() ? "Unknown" : options.moduleAuthor) << "\n";
    ss << "// 描述: " << (options.moduleDescription.empty() ? "CHTL模块" : options.moduleDescription) << "\n\n";
    
    ss << "[Namespace] " << options.moduleName << "\n\n";
    
    ss << "// 示例模板定义\n";
    ss << "[Template] Example\n{\n";
    ss << "    div\n    {\n";
    ss << "        id: example;\n";
    ss << "        class: " << options.moduleName << "-example;\n\n";
    ss << "        text\n        {\n";
    ss << "            \"这是 " << options.moduleName << " 模块的示例\"\n";
    ss << "        }\n    }\n}\n\n";
    
    ss << "// 示例自定义元素\n";
    ss << "[Custom] " << options.moduleName << "Button\n{\n";
    ss << "    button\n    {\n";
    ss << "        class: " << options.moduleName << "-button;\n";
    ss << "        \n        style\n        {\n";
    ss << "            background-color: #007bff;\n";
    ss << "            color: white;\n";
    ss << "            border: none;\n";
    ss << "            padding: 10px 20px;\n";
    ss << "            border-radius: 4px;\n";
    ss << "            cursor: pointer;\n        }\n";
    ss << "        \n        text\n        {\n";
    ss << "            \"点击我\"\n";
    ss << "        }\n    }\n}\n";
    
    return ss.str();
}

std::string CmodCompilerExtension::createInfoChtlFile(const CmodCompilerOptions& options) {
    CmodInfo info;
    info.name = options.moduleName;
    info.version = options.moduleVersion;
    info.description = options.moduleDescription.empty() ? "CHTL模块" : options.moduleDescription;
    info.author = options.moduleAuthor.empty() ? "Unknown" : options.moduleAuthor;
    info.license = options.moduleLicense;
    info.category = options.moduleCategory;
    info.minCHTLVersion = "1.0.0";
    
    return cmodManager_->generateInfoFile(info);
}

std::vector<std::string> CmodCompilerExtension::findCmodFiles(const std::string& directory) {
    std::vector<std::string> cmodFiles;
    
    if (!std::filesystem::exists(directory)) {
        return cmodFiles;
    }
    
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
            if (entry.path().extension() == ".cmod") {
                cmodFiles.push_back(entry.path().string());
            }
        }
    } catch (...) {
        // 忽略错误
    }
    
    return cmodFiles;
}

std::string CmodCompilerExtension::generateCacheKey(const std::string& cmodPath) {
    // 使用文件名和修改时间生成缓存键
    std::filesystem::path path(cmodPath);
    std::string baseName = path.stem().string();
    
    try {
        auto lastWriteTime = std::filesystem::last_write_time(cmodPath);
        auto timeT = std::chrono::duration_cast<std::chrono::seconds>(
            lastWriteTime.time_since_epoch()).count();
        
        return baseName + "_" + std::to_string(timeT);
    } catch (...) {
        return baseName + "_unknown";
    }
}

bool CmodCompilerExtension::isCacheValid(const std::string& cacheDir, const std::string& cmodPath) {
    if (!std::filesystem::exists(cacheDir)) {
        return false;
    }
    
    try {
        auto cacheTime = std::filesystem::last_write_time(cacheDir);
        auto cmodTime = std::filesystem::last_write_time(cmodPath);
        
        return cacheTime >= cmodTime;
    } catch (...) {
        return false;
    }
}

void CmodCompilerExtension::printResult(const CmodCompilerResult& result) const {
    if (result.success) {
        std::cout << "✅ " << result.message << std::endl;
    } else {
        std::cout << "❌ " << result.message << std::endl;
    }
    
    for (const auto& output : result.outputs) {
        std::cout << "   " << output << std::endl;
    }
}

void CmodCompilerExtension::printUsage() const {
    std::cout << getHelpText() << std::endl;
}

// ========== CmodCommandLineInterface 实现 ==========

CmodCommandLineInterface::CmodCommandLineInterface() 
    : extension_(std::make_shared<CmodCompilerExtension>()) {
}

CmodCommandLineInterface::~CmodCommandLineInterface() = default;

int CmodCommandLineInterface::run(int argc, char* argv[]) {
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

void CmodCommandLineInterface::setCompilerExtension(std::shared_ptr<CmodCompilerExtension> extension) {
    extension_ = extension;
}

void CmodCommandLineInterface::printBanner() const {
    std::cout << "🎯 CHTL Cmod工具 - 模块打包和管理" << std::endl;
    std::cout << "严格遵循CHTL语法文档 v1.0.0" << std::endl;
    std::cout << "========================================" << std::endl;
}

void CmodCommandLineInterface::printUsage() const {
    std::cout << "\n用法: chtl-cmod <命令> [选项] [参数]" << std::endl;
    std::cout << "使用 'chtl-cmod help' 查看详细帮助" << std::endl;
}

std::vector<std::string> CmodCommandLineInterface::parseArguments(int argc, char* argv[]) {
    std::vector<std::string> args;
    
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    
    return args;
}

} // namespace chtl
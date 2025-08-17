#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include "import/ImportManager.h"

using namespace chtl;

void testImportManagerCreation() {
    std::cout << "=== 测试Import管理器创建 ===" << std::endl;
    
    // 测试默认配置
    ImportManagerConfig default_config;
    auto import_manager1 = std::make_unique<ImportManager>(default_config);
    
    std::cout << "默认配置Import管理器创建成功" << std::endl;
    std::cout << "官方模块路径: " << default_config.official_module_path << std::endl;
    std::cout << "本地模块路径: " << default_config.local_module_path << std::endl;
    std::cout << "当前目录: " << default_config.current_directory << std::endl;
    std::cout << "循环依赖检测: " << (default_config.enable_circular_detection ? "启用" : "禁用") << std::endl;
    std::cout << "重复导入检测: " << (default_config.enable_duplicate_detection ? "启用" : "禁用") << std::endl;
    std::cout << "通配符导入: " << (default_config.enable_wildcard_import ? "启用" : "禁用") << std::endl;
    std::cout << "子模块导入: " << (default_config.enable_submodule_import ? "启用" : "禁用") << std::endl;
    
    // 测试自定义配置
    ImportManagerConfig custom_config;
    custom_config.official_module_path = "official_modules";
    custom_config.local_module_path = "local_modules";
    custom_config.current_directory = "/home/user/project";
    custom_config.enable_circular_detection = false;
    custom_config.enable_duplicate_detection = false;
    custom_config.max_import_depth = 5;
    
    auto import_manager2 = std::make_unique<ImportManager>(custom_config);
    
    std::cout << "自定义配置Import管理器创建成功" << std::endl;
    std::cout << "官方模块路径: " << custom_config.official_module_path << std::endl;
    std::cout << "本地模块路径: " << custom_config.local_module_path << std::endl;
    std::cout << "当前目录: " << custom_config.current_directory << std::endl;
    std::cout << "循环依赖检测: " << (custom_config.enable_circular_detection ? "启用" : "禁用") << std::endl;
    std::cout << "重复导入检测: " << (custom_config.enable_duplicate_detection ? "启用" : "禁用") << std::endl;
    std::cout << "最大导入深度: " << custom_config.max_import_depth << std::endl;
    
    std::cout << std::endl;
}

void testImportTypes() {
    std::cout << "=== 测试导入类型 ===" << std::endl;
    
    ImportManagerConfig config;
    config.enable_debug = true;
    auto import_manager = std::make_unique<ImportManager>(config);
    
    // 测试HTML导入
    ImportInfo html_import(ImportType::HTML, ImportSyntaxType::SIMPLE, "styles.css", "test.chtl", 1, 1);
    bool success = import_manager->addImport(html_import);
    std::cout << "HTML导入添加: " << (success ? "成功" : "失败") << std::endl;
    
    // 测试Style导入
    ImportInfo style_import(ImportType::STYLE, ImportSyntaxType::SIMPLE, "main.css", "test.chtl", 2, 1);
    success = import_manager->addImport(style_import);
    std::cout << "Style导入添加: " << (success ? "成功" : "失败") << std::endl;
    
    // 测试JavaScript导入
    ImportInfo js_import(ImportType::JAVASCRIPT, ImportSyntaxType::SIMPLE, "app.js", "test.chtl", 3, 1);
    success = import_manager->addImport(js_import);
    std::cout << "JavaScript导入添加: " << (success ? "成功" : "失败") << std::endl;
    
    // 测试CHTL导入
    ImportInfo chtl_import(ImportType::CHTL, ImportSyntaxType::SIMPLE, "module.chtl", "test.chtl", 4, 1);
    success = import_manager->addImport(chtl_import);
    std::cout << "CHTL导入添加: " << (success ? "成功" : "失败") << std::endl;
    
    // 测试CJMOD导入
    ImportInfo cjmod_import(ImportType::CJMOD, ImportSyntaxType::SIMPLE, "module.cjmod", "test.chtl", 5, 1);
    success = import_manager->addImport(cjmod_import);
    std::cout << "CJMOD导入添加: " << (success ? "成功" : "失败") << std::endl;
    
    // 测试别名导入
    ImportInfo alias_import(ImportType::HTML, ImportSyntaxType::ALIAS, "styles.css", "test.chtl", 6, 1);
    alias_import.alias = "CustomStyles";
    success = import_manager->addImport(alias_import);
    std::cout << "别名导入添加: " << (success ? "成功" : "失败") << std::endl;
    
    std::cout << "总导入数量: " << import_manager->getImports().size() << std::endl;
    
    std::cout << std::endl;
}

void testImportSyntaxTypes() {
    std::cout << "=== 测试导入语法类型 ===" << std::endl;
    
    ImportManagerConfig config;
    config.enable_debug = true;
    auto import_manager = std::make_unique<ImportManager>(config);
    
    // 测试简单导入
    ImportInfo simple_import(ImportType::CHTL, ImportSyntaxType::SIMPLE, "module", "test.chtl", 1, 1);
    bool success = import_manager->addImport(simple_import);
    std::cout << "简单导入添加: " << (success ? "成功" : "失败") << std::endl;
    
    // 测试别名导入
    ImportInfo alias_import(ImportType::CHTL, ImportSyntaxType::ALIAS, "module", "test.chtl", 2, 1);
    alias_import.alias = "MyModule";
    success = import_manager->addImport(alias_import);
    std::cout << "别名导入添加: " << (success ? "成功" : "失败") << std::endl;
    
    // 测试通配符导入
    ImportInfo wildcard_import(ImportType::CHTL, ImportSyntaxType::WILDCARD, "module.*", "test.chtl", 3, 1);
    success = import_manager->addImport(wildcard_import);
    std::cout << "通配符导入添加: " << (success ? "成功" : "失败") << std::endl;
    
    // 测试子模块导入
    ImportInfo submodule_import(ImportType::CHTL, ImportSyntaxType::SUBMODULE, "module.submodule", "test.chtl", 4, 1);
    success = import_manager->addImport(submodule_import);
    std::cout << "子模块导入添加: " << (success ? "成功" : "失败") << std::endl;
    
    std::cout << std::endl;
}

void testPathResolution() {
    std::cout << "=== 测试路径解析 ===" << std::endl;
    
    ImportManagerConfig config;
    config.enable_debug = true;
    config.current_directory = ".";
    auto import_manager = std::make_unique<ImportManager>(config);
    
    // 测试HTML路径解析
    std::string html_path = import_manager->resolvePath("styles.css", ImportType::HTML);
    std::cout << "HTML路径解析: styles.css -> " << (html_path.empty() ? "未找到" : html_path) << std::endl;
    
    // 测试Style路径解析
    std::string style_path = import_manager->resolvePath("main.css", ImportType::STYLE);
    std::cout << "Style路径解析: main.css -> " << (style_path.empty() ? "未找到" : style_path) << std::endl;
    
    // 测试JavaScript路径解析
    std::string js_path = import_manager->resolvePath("app.js", ImportType::JAVASCRIPT);
    std::cout << "JavaScript路径解析: app.js -> " << (js_path.empty() ? "未找到" : js_path) << std::endl;
    
    // 测试CHTL路径解析
    std::string chtl_path = import_manager->resolvePath("module", ImportType::CHTL);
    std::cout << "CHTL路径解析: module -> " << (chtl_path.empty() ? "未找到" : chtl_path) << std::endl;
    
    // 测试CJMOD路径解析
    std::string cjmod_path = import_manager->resolvePath("module", ImportType::CJMOD);
    std::cout << "CJMOD路径解析: module -> " << (cjmod_path.empty() ? "未找到" : cjmod_path) << std::endl;
    
    std::cout << std::endl;
}

void testDuplicateDetection() {
    std::cout << "=== 测试重复导入检测 ===" << std::endl;
    
    ImportManagerConfig config;
    config.enable_duplicate_detection = true;
    config.enable_debug = true;
    auto import_manager = std::make_unique<ImportManager>(config);
    
    // 添加第一个导入
    ImportInfo import1(ImportType::CHTL, ImportSyntaxType::SIMPLE, "module.chtl", "test.chtl", 1, 1);
    bool success = import_manager->addImport(import1);
    std::cout << "第一个导入添加: " << (success ? "成功" : "失败") << std::endl;
    
    // 添加相同的导入（应该失败）
    ImportInfo import2(ImportType::CHTL, ImportSyntaxType::SIMPLE, "module.chtl", "test.chtl", 2, 1);
    success = import_manager->addImport(import2);
    std::cout << "重复导入添加: " << (success ? "成功" : "失败") << std::endl;
    
    // 检查是否有重复导入
    bool has_duplicates = import_manager->hasDuplicateImports();
    std::cout << "是否有重复导入: " << (has_duplicates ? "是" : "否") << std::endl;
    
    if (has_duplicates) {
        std::vector<std::string> duplicates = import_manager->getDuplicateImports();
        std::cout << "重复导入列表:" << std::endl;
        for (const auto& duplicate : duplicates) {
            std::cout << "  " << duplicate << std::endl;
        }
    }
    
    std::cout << std::endl;
}

void testCircularDependencyDetection() {
    std::cout << "=== 测试循环依赖检测 ===" << std::endl;
    
    ImportManagerConfig config;
    config.enable_circular_detection = true;
    config.enable_debug = true;
    auto import_manager = std::make_unique<ImportManager>(config);
    
    // 添加导入
    ImportInfo import1(ImportType::CHTL, ImportSyntaxType::SIMPLE, "module1.chtl", "main.chtl", 1, 1);
    bool success = import_manager->addImport(import1);
    std::cout << "导入1添加: " << (success ? "成功" : "失败") << std::endl;
    
    ImportInfo import2(ImportType::CHTL, ImportSyntaxType::SIMPLE, "main.chtl", "module1.chtl", 1, 1);
    success = import_manager->addImport(import2);
    std::cout << "导入2添加: " << (success ? "成功" : "失败") << std::endl;
    
    // 检查是否有循环依赖
    bool has_circular = import_manager->hasCircularDependencies();
    std::cout << "是否有循环依赖: " << (has_circular ? "是" : "否") << std::endl;
    
    if (has_circular) {
        std::vector<std::vector<std::string>> circular_deps = import_manager->getCircularDependencies();
        std::cout << "循环依赖列表:" << std::endl;
        for (const auto& cycle : circular_deps) {
            std::cout << "  循环: ";
            for (size_t i = 0; i < cycle.size(); i++) {
                if (i > 0) std::cout << " -> ";
                std::cout << cycle[i];
            }
            std::cout << std::endl;
        }
    }
    
    std::cout << std::endl;
}

void testWildcardImport() {
    std::cout << "=== 测试通配符导入 ===" << std::endl;
    
    ImportManagerConfig config;
    config.enable_wildcard_import = true;
    config.enable_debug = true;
    auto import_manager = std::make_unique<ImportManager>(config);
    
    // 测试通配符路径解析
    std::vector<std::string> wildcard_files = import_manager->resolveWildcardPath("module/*", ImportType::CHTL);
    std::cout << "通配符路径解析: module/* -> " << wildcard_files.size() << " 个文件" << std::endl;
    
    for (size_t i = 0; i < wildcard_files.size(); i++) {
        std::cout << "  [" << i << "]: " << wildcard_files[i] << std::endl;
    }
    
    // 测试特定扩展名通配符
    std::vector<std::string> cmod_files = import_manager->resolveWildcardPath("module/*.cmod", ImportType::CHTL);
    std::cout << "Cmod通配符路径解析: module/*.cmod -> " << cmod_files.size() << " 个文件" << std::endl;
    
    for (size_t i = 0; i < cmod_files.size(); i++) {
        std::cout << "  [" << i << "]: " << cmod_files[i] << std::endl;
    }
    
    std::cout << std::endl;
}

void testSubmoduleImport() {
    std::cout << "=== 测试子模块导入 ===" << std::endl;
    
    ImportManagerConfig config;
    config.enable_submodule_import = true;
    config.enable_debug = true;
    auto import_manager = std::make_unique<ImportManager>(config);
    
    // 测试子模块路径解析
    std::vector<std::string> submodule_files = import_manager->resolveSubmodulePath("Chtholly.Space", ImportType::CHTL);
    std::cout << "子模块路径解析: Chtholly.Space -> " << submodule_files.size() << " 个文件" << std::endl;
    
    for (size_t i = 0; i < submodule_files.size(); i++) {
        std::cout << "  [" << i << "]: " << submodule_files[i] << std::endl;
    }
    
    // 测试另一个子模块
    std::vector<std::string> submodule_files2 = import_manager->resolveSubmodulePath("module.submodule", ImportType::CHTL);
    std::cout << "子模块路径解析: module.submodule -> " << submodule_files2.size() << " 个文件" << std::endl;
    
    for (size_t i = 0; i < submodule_files2.size(); i++) {
        std::cout << "  [" << i << "]: " << submodule_files2[i] << std::endl;
    }
    
    std::cout << std::endl;
}

void testFileSearch() {
    std::cout << "=== 测试文件搜索 ===" << std::endl;
    
    ImportManagerConfig config;
    config.enable_debug = true;
    auto import_manager = std::make_unique<ImportManager>(config);
    
    // 测试在当前目录搜索文件
    std::vector<std::string> html_files = import_manager->findFilesInDirectory(".", ImportType::HTML);
    std::cout << "当前目录HTML文件数量: " << html_files.size() << std::endl;
    
    for (size_t i = 0; i < html_files.size(); i++) {
        std::cout << "  [" << i << "]: " << html_files[i] << std::endl;
    }
    
    // 测试搜索特定文件
    std::string found_file = import_manager->findFile("test", ImportType::CHTL);
    std::cout << "搜索test文件: " << (found_file.empty() ? "未找到" : found_file) << std::endl;
    
    // 测试按模式搜索文件
    std::vector<std::string> pattern_files = import_manager->findFilesByPattern(".", "*.chtl");
    std::cout << "按模式*.chtl搜索文件数量: " << pattern_files.size() << std::endl;
    
    for (size_t i = 0; i < pattern_files.size(); i++) {
        std::cout << "  [" << i << "]: " << pattern_files[i] << std::endl;
    }
    
    std::cout << std::endl;
}

void testModuleManagement() {
    std::cout << "=== 测试模块管理 ===" << std::endl;
    
    ImportManagerConfig config;
    config.enable_debug = true;
    auto import_manager = std::make_unique<ImportManager>(config);
    
    // 测试加载模块
    bool success = import_manager->loadModule("test_module.chtl");
    std::cout << "加载模块test_module.chtl: " << (success ? "成功" : "失败") << std::endl;
    
    // 获取模块列表
    std::vector<ModuleInfo> modules = import_manager->getModules();
    std::cout << "已加载模块数量: " << modules.size() << std::endl;
    
    for (size_t i = 0; i < modules.size(); i++) {
        const auto& module = modules[i];
        std::cout << "  [" << i << "]: " << module.name << " (" << module.path << ") [" << module.type << "]" << std::endl;
    }
    
    // 测试获取特定模块
    ModuleInfo* module = import_manager->getModule("test_module.chtl");
    if (module) {
        std::cout << "找到模块: " << module->name << std::endl;
    } else {
        std::cout << "未找到模块: test_module.chtl" << std::endl;
    }
    
    std::cout << std::endl;
}

void testErrorHandling() {
    std::cout << "=== 测试错误处理 ===" << std::endl;
    
    ImportManagerConfig config;
    config.enable_debug = true;
    auto import_manager = std::make_unique<ImportManager>(config);
    
    // 测试添加错误
    import_manager->addError("测试错误1");
    import_manager->addError("测试错误2");
    
    // 检查错误状态
    bool has_errors = import_manager->hasErrors();
    std::cout << "是否有错误: " << (has_errors ? "是" : "否") << std::endl;
    
    if (has_errors) {
        std::vector<std::string> errors = import_manager->getErrors();
        std::cout << "错误数量: " << errors.size() << std::endl;
        std::cout << "错误列表:" << std::endl;
        for (size_t i = 0; i < errors.size(); i++) {
            std::cout << "  [" << i << "]: " << errors[i] << std::endl;
        }
    }
    
    // 清除错误
    import_manager->clearErrors();
    has_errors = import_manager->hasErrors();
    std::cout << "清除错误后是否有错误: " << (has_errors ? "是" : "否") << std::endl;
    
    std::cout << std::endl;
}

void testDebugInfo() {
    std::cout << "=== 测试调试信息 ===" << std::endl;
    
    ImportManagerConfig config;
    config.enable_debug = true;
    auto import_manager = std::make_unique<ImportManager>(config);
    
    // 添加一些导入
    ImportInfo import1(ImportType::CHTL, ImportSyntaxType::SIMPLE, "module1.chtl", "main.chtl", 1, 1);
    ImportInfo import2(ImportType::HTML, ImportSyntaxType::SIMPLE, "styles.css", "main.chtl", 2, 1);
    ImportInfo import3(ImportType::CJMOD, ImportSyntaxType::SIMPLE, "module1.cjmod", "main.chtl", 3, 1);
    
    import_manager->addImport(import1);
    import_manager->addImport(import2);
    import_manager->addImport(import3);
    
    // 获取调试信息
    std::string debug_info = import_manager->getDebugInfo();
    std::cout << "调试信息:" << std::endl;
    std::cout << debug_info << std::endl;
    
    // 获取导入关系图
    std::string import_graph = import_manager->getImportGraph();
    std::cout << "导入关系图:" << std::endl;
    std::cout << import_graph << std::endl;
    
    std::cout << std::endl;
}

int main() {
    std::cout << "CHTL增强Import功能测试" << std::endl;
    std::cout << "=====================" << std::endl << std::endl;
    
    try {
        testImportManagerCreation();
        testImportTypes();
        testImportSyntaxTypes();
        testPathResolution();
        testDuplicateDetection();
        testCircularDependencyDetection();
        testWildcardImport();
        testSubmoduleImport();
        testFileSearch();
        testModuleManagement();
        testErrorHandling();
        testDebugInfo();
        
        std::cout << "所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include "module/ModuleManager.h"
#include "module/CJmodInterface.h"

using namespace chtl;

void testModuleManagerCreation() {
    std::cout << "=== 测试模块管理器创建 ===" << std::endl;
    
    // 测试默认配置
    ModuleManagerConfig default_config;
    auto module_manager1 = std::make_unique<ModuleManager>(default_config);
    
    std::cout << "默认配置模块管理器创建成功" << std::endl;
    std::cout << "官方模块路径: " << default_config.official_module_path << std::endl;
    std::cout << "本地模块路径: " << default_config.local_module_path << std::endl;
    std::cout << "临时目录: " << default_config.temp_directory << std::endl;
    std::cout << "压缩功能: " << (default_config.enable_compression ? "启用" : "禁用") << std::endl;
    std::cout << "加密功能: " << (default_config.enable_encryption ? "启用" : "禁用") << std::endl;
    std::cout << "验证功能: " << (default_config.enable_validation ? "启用" : "禁用") << std::endl;
    
    // 测试自定义配置
    ModuleManagerConfig custom_config;
    custom_config.official_module_path = "official_modules";
    custom_config.local_module_path = "local_modules";
    custom_config.temp_directory = "temp_modules";
    custom_config.enable_compression = false;
    custom_config.enable_encryption = true;
    custom_config.max_module_size = 50 * 1024 * 1024; // 50MB
    
    auto module_manager2 = std::make_unique<ModuleManager>(custom_config);
    
    std::cout << "自定义配置模块管理器创建成功" << std::endl;
    std::cout << "官方模块路径: " << custom_config.official_module_path << std::endl;
    std::cout << "本地模块路径: " << custom_config.local_module_path << std::endl;
    std::cout << "临时目录: " << custom_config.temp_directory << std::endl;
    std::cout << "最大模块大小: " << custom_config.max_module_size << " 字节" << std::endl;
    
    std::cout << std::endl;
}

void testOfficialModuleSupport() {
    std::cout << "=== 测试官方模块支持 ===" << std::endl;
    
    ModuleManagerConfig config;
    config.enable_debug = true;
    auto module_manager = std::make_unique<ModuleManager>(config);
    
    // 检查默认官方模块
    std::vector<std::string> official_modules = module_manager->getOfficialModuleNames();
    std::cout << "默认官方模块数量: " << official_modules.size() << std::endl;
    
    for (const auto& module_name : official_modules) {
        std::cout << "  官方模块: " << module_name << std::endl;
        bool is_official = module_manager->isOfficialModule(module_name);
        std::cout << "    是否官方: " << (is_official ? "是" : "否") << std::endl;
    }
    
    // 测试注册新的官方模块
    bool success = module_manager->registerOfficialModule("test_official.cmod");
    std::cout << "注册官方模块test_official.cmod: " << (success ? "成功" : "失败") << std::endl;
    
    // 检查更新后的官方模块列表
    official_modules = module_manager->getOfficialModuleNames();
    std::cout << "更新后官方模块数量: " << official_modules.size() << std::endl;
    
    for (const auto& module_name : official_modules) {
        std::cout << "  官方模块: " << module_name << std::endl;
    }
    
    // 测试注销官方模块
    success = module_manager->unregisterOfficialModule("test_official");
    std::cout << "注销官方模块test_official: " << (success ? "成功" : "失败") << std::endl;
    
    std::cout << std::endl;
}

void testModulePacking() {
    std::cout << "=== 测试模块打包 ===" << std::endl;
    
    ModuleManagerConfig config;
    config.enable_debug = true;
    auto module_manager = std::make_unique<ModuleManager>(config);
    
    // 创建测试目录结构
    std::string test_dir = "test_module_source";
    std::string cmod_output = "test_module.cmod";
    std::string cjmod_output = "test_module.cjmod";
    
    // 创建测试目录
    if (!std::filesystem::exists(test_dir)) {
        std::filesystem::create_directories(test_dir);
    }
    
    // 创建测试文件
    std::string test_file = test_dir + "/test.txt";
    std::ofstream file(test_file);
    if (file.is_open()) {
        file << "这是一个测试文件" << std::endl;
        file << "用于测试模块打包功能" << std::endl;
        file.close();
    }
    
    // 创建清单文件
    std::string manifest_file = test_dir + "/manifest.json";
    std::ofstream manifest(manifest_file);
    if (manifest.is_open()) {
        manifest << "{\n";
        manifest << "  \"name\": \"test_module\",\n";
        manifest << "  \"type\": \"cmod\",\n";
        manifest << "  \"version\": \"1.0.0\",\n";
        manifest << "  \"description\": \"测试模块\",\n";
        manifest << "  \"author\": \"测试作者\",\n";
        manifest << "  \"license\": \"MIT\"\n";
        manifest << "}\n";
        manifest.close();
    }
    
    // 测试Cmod打包
    bool success = module_manager->packCmodModule(test_dir, cmod_output);
    std::cout << "Cmod模块打包: " << (success ? "成功" : "失败") << std::endl;
    
    if (success) {
        std::cout << "  输出文件: " << cmod_output << std::endl;
        if (std::filesystem::exists(cmod_output)) {
            size_t file_size = std::filesystem::file_size(cmod_output);
            std::cout << "  文件大小: " << file_size << " 字节" << std::endl;
        }
    }
    
    // 测试CJmod打包
    success = module_manager->packCJmodModule(test_dir, cjmod_output);
    std::cout << "CJmod模块打包: " << (success ? "成功" : "失败") << std::endl;
    
    if (success) {
        std::cout << "  输出文件: " << cjmod_output << std::endl;
        if (std::filesystem::exists(cjmod_output)) {
            size_t file_size = std::filesystem::file_size(cjmod_output);
            std::cout << "  文件大小: " << file_size << " 字节" << std::endl;
        }
    }
    
    std::cout << std::endl;
}

void testModuleUnpacking() {
    std::cout << "=== 测试模块解包 ===" << std::endl;
    
    ModuleManagerConfig config;
    config.enable_debug = true;
    auto module_manager = std::make_unique<ModuleManager>(config);
    
    std::string cmod_file = "test_module.cmod";
    std::string cjmod_file = "test_module.cjmod";
    std::string cmod_output = "unpacked_cmod";
    std::string cjmod_output = "unpacked_cjmod";
    
    // 测试Cmod解包
    if (std::filesystem::exists(cmod_file)) {
        bool success = module_manager->unpackCmodModule(cmod_file, cmod_output);
        std::cout << "Cmod模块解包: " << (success ? "成功" : "失败") << std::endl;
        
        if (success) {
            std::cout << "  输出目录: " << cmod_output << std::endl;
            if (std::filesystem::exists(cmod_output)) {
                std::cout << "  解包后的文件:" << std::endl;
                for (const auto& entry : std::filesystem::directory_iterator(cmod_output)) {
                    std::cout << "    " << entry.path().filename().string() << std::endl;
                }
            }
        }
    } else {
        std::cout << "Cmod文件不存在，跳过解包测试" << std::endl;
    }
    
    // 测试CJmod解包
    if (std::filesystem::exists(cjmod_file)) {
        bool success = module_manager->unpackCJmodModule(cjmod_file, cjmod_output);
        std::cout << "CJmod模块解包: " << (success ? "成功" : "失败") << std::endl;
        
        if (success) {
            std::cout << "  输出目录: " << cjmod_output << std::endl;
            if (std::filesystem::exists(cjmod_output)) {
                std::cout << "  解包后的文件:" << std::endl;
                for (const auto& entry : std::filesystem::directory_iterator(cjmod_output)) {
                    std::cout << "    " << entry.path().filename().string() << std::endl;
                }
            }
        }
    } else {
        std::cout << "CJmod文件不存在，跳过解包测试" << std::endl;
    }
    
    std::cout << std::endl;
}

void testModuleValidation() {
    std::cout << "=== 测试模块验证 ===" << std::endl;
    
    ModuleManagerConfig config;
    config.enable_debug = true;
    auto module_manager = std::make_unique<ModuleManager>(config);
    
    std::string cmod_file = "test_module.cmod";
    std::string cjmod_file = "test_module.cjmod";
    
    // 测试Cmod验证
    if (std::filesystem::exists(cmod_file)) {
        bool is_valid = module_manager->validateModule(cmod_file);
        std::cout << "Cmod模块验证: " << (is_valid ? "通过" : "失败") << std::endl;
    } else {
        std::cout << "Cmod文件不存在，跳过验证测试" << std::endl;
    }
    
    // 测试CJmod验证
    if (std::filesystem::exists(cjmod_file)) {
        bool is_valid = module_manager->validateModule(cjmod_file);
        std::cout << "CJmod模块验证: " << (is_valid ? "通过" : "失败") << std::endl;
    } else {
        std::cout << "CJmod文件不存在，跳过验证测试" << std::endl;
    }
    
    // 测试无效路径验证
    bool is_valid = module_manager->validateModule("nonexistent_file.cmod");
    std::cout << "无效路径验证: " << (is_valid ? "通过" : "失败") << std::endl;
    
    std::cout << std::endl;
}

void testCJmodInterface() {
    std::cout << "=== 测试CJmod接口 ===" << std::endl;
    
    // 测试CJmod函数类型
    std::cout << "CJmod函数类型:" << std::endl;
    std::cout << "  NATIVE_FUNCTION: " << static_cast<int>(CJmodFunctionType::NATIVE_FUNCTION) << std::endl;
    std::cout << "  WRAPPED_FUNCTION: " << static_cast<int>(CJmodFunctionType::WRAPPED_FUNCTION) << std::endl;
    std::cout << "  BUILTIN_FUNCTION: " << static_cast<int>(CJmodFunctionType::BUILTIN_FUNCTION) << std::endl;
    std::cout << "  CUSTOM_FUNCTION: " << static_cast<int>(CJmodFunctionType::CUSTOM_FUNCTION) << std::endl;
    
    // 测试CJmod参数类型
    std::cout << "CJmod参数类型:" << std::endl;
    std::cout << "  STRING: " << static_cast<int>(CJmodParameterType::STRING) << std::endl;
    std::cout << "  NUMBER: " << static_cast<int>(CJmodParameterType::NUMBER) << std::endl;
    std::cout << "  BOOLEAN: " << static_cast<int>(CJmodParameterType::BOOLEAN) << std::endl;
    std::cout << "  OBJECT: " << static_cast<int>(CJmodParameterType::OBJECT) << std::endl;
    std::cout << "  ARRAY: " << static_cast<int>(CJmodParameterType::ARRAY) << std::endl;
    std::cout << "  FUNCTION: " << static_cast<int>(CJmodParameterType::FUNCTION) << std::endl;
    std::cout << "  UNDEFINED: " << static_cast<int>(CJmodParameterType::UNDEFINED) << std::endl;
    std::cout << "  NULL_VALUE: " << static_cast<int>(CJmodParameterType::NULL_VALUE) << std::endl;
    std::cout << "  ANY: " << static_cast<int>(CJmodParameterType::ANY) << std::endl;
    
    // 测试CJmod函数创建
    CJmodFunction test_function("testFunction", "测试函数", CJmodFunctionType::NATIVE_FUNCTION);
    test_function.category = "测试";
    test_function.version = "1.0.0";
    
    // 添加参数
    CJmodParameter param1("param1", CJmodParameterType::STRING, "第一个参数", true);
    CJmodParameter param2("param2", CJmodParameterType::NUMBER, "第二个参数", false);
    param2.default_value = 42;
    
    test_function.parameters.push_back(param1);
    test_function.parameters.push_back(param2);
    
    // 设置返回值
    CJmodReturnValue return_val(CJmodParameterType::BOOLEAN, "返回布尔值");
    return_val.example_value = true;
    test_function.return_value = return_val;
    
    std::cout << "测试函数信息:" << std::endl;
    std::cout << "  名称: " << test_function.name << std::endl;
    std::cout << "  描述: " << test_function.description << std::endl;
    std::cout << "  类型: " << static_cast<int>(test_function.type) << std::endl;
    std::cout << "  分类: " << test_function.category << std::endl;
    std::cout << "  版本: " << test_function.version << std::endl;
    std::cout << "  参数数量: " << test_function.parameters.size() << std::endl;
    std::cout << "  返回值类型: " << static_cast<int>(test_function.return_value.type) << std::endl;
    
    // 测试CJmod类创建
    CJmodClass test_class("TestClass", "测试类");
    test_class.base_class = "BaseClass";
    test_class.category = "测试";
    test_class.version = "1.0.0";
    
    // 添加方法
    test_class.methods.push_back(test_function);
    
    // 添加属性
    CJmodProperty prop1("property1", "第一个属性", CJmodParameterType::STRING);
    prop1.is_readonly = false;
    prop1.is_static = false;
    
    CJmodProperty prop2("property2", "第二个属性", CJmodParameterType::NUMBER);
    prop2.is_readonly = true;
    prop2.is_static = true;
    prop2.default_value = 100;
    
    test_class.properties.push_back(prop1);
    test_class.properties.push_back(prop2);
    
    std::cout << "测试类信息:" << std::endl;
    std::cout << "  名称: " << test_class.name << std::endl;
    std::cout << "  描述: " << test_class.description << std::endl;
    std::cout << "  基类: " << test_class.base_class << std::endl;
    std::cout << "  分类: " << test_class.category << std::endl;
    std::cout << "  版本: " << test_class.version << std::endl;
    std::cout << "  方法数量: " << test_class.methods.size() << std::endl;
    std::cout << "  属性数量: " << test_class.properties.size() << std::endl;
    
    // 测试CJmod模块创建
    CJmodModule test_module("test_module", "测试模块");
    test_module.version = "1.0.0";
    test_module.author = "测试作者";
    test_module.license = "MIT";
    test_module.category = "测试";
    test_module.documentation_url = "https://example.com/docs";
    test_module.repository_url = "https://github.com/example/test_module";
    
    test_module.functions.push_back(test_function);
    test_module.classes.push_back(test_class);
    
    std::cout << "测试模块信息:" << std::endl;
    std::cout << "  名称: " << test_module.name << std::endl;
    std::cout << "  描述: " << test_module.description << std::endl;
    std::cout << "  版本: " << test_module.version << std::endl;
    std::cout << "  作者: " << test_module.author << std::endl;
    std::cout << "  许可证: " << test_module.license << std::endl;
    std::cout << "  分类: " << test_module.category << std::endl;
    std::cout << "  函数数量: " << test_module.functions.size() << std::endl;
    std::cout << "  类数量: " << test_module.classes.size() << std::endl;
    
    std::cout << std::endl;
}

void testModuleLoading() {
    std::cout << "=== 测试模块加载 ===" << std::endl;
    
    ModuleManagerConfig config;
    config.enable_debug = true;
    auto module_manager = std::make_unique<ModuleManager>(config);
    
    std::string cmod_file = "test_module.cmod";
    std::string cjmod_file = "test_module.cjmod";
    
    // 测试Cmod加载
    if (std::filesystem::exists(cmod_file)) {
        bool success = module_manager->loadModule(cmod_file);
        std::cout << "Cmod模块加载: " << (success ? "成功" : "失败") << std::endl;
        
        if (success) {
            std::vector<ModuleInfo> modules = module_manager->getModules();
            std::cout << "  已加载模块数量: " << modules.size() << std::endl;
            
            for (const auto& module : modules) {
                std::cout << "    模块: " << module.name << std::endl;
                std::cout << "      路径: " << module.path << std::endl;
                std::cout << "      类型: " << static_cast<int>(module.type) << std::endl;
                std::cout << "      状态: " << static_cast<int>(module.status) << std::endl;
                std::cout << "      大小: " << module.file_size << " 字节" << std::endl;
                std::cout << "      已加载: " << (module.is_loaded ? "是" : "否") << std::endl;
                std::cout << "      官方模块: " << (module.is_official ? "是" : "否") << std::endl;
            }
        }
    } else {
        std::cout << "Cmod文件不存在，跳过加载测试" << std::endl;
    }
    
    // 测试CJmod加载
    if (std::filesystem::exists(cjmod_file)) {
        bool success = module_manager->loadModule(cjmod_file);
        std::cout << "CJmod模块加载: " << (success ? "成功" : "失败") << std::endl;
    } else {
        std::cout << "CJmod文件不存在，跳过加载测试" << std::endl;
    }
    
    std::cout << std::endl;
}

void testModuleManagement() {
    std::cout << "=== 测试模块管理 ===" << std::endl;
    
    ModuleManagerConfig config;
    config.enable_debug = true;
    auto module_manager = std::make_unique<ModuleManager>(config);
    
    // 获取模块列表
    std::vector<ModuleInfo> all_modules = module_manager->getModules();
    std::cout << "总模块数量: " << all_modules.size() << std::endl;
    
    // 按类型获取模块
    std::vector<ModuleInfo> cmod_modules = module_manager->getModulesByType(ModuleType::CMOD);
    std::cout << "Cmod模块数量: " << cmod_modules.size() << std::endl;
    
    std::vector<ModuleInfo> cjmod_modules = module_manager->getModulesByType(ModuleType::CJMOD);
    std::cout << "CJmod模块数量: " << cjmod_modules.size() << std::endl;
    
    // 获取官方和本地模块
    std::vector<ModuleInfo> official_modules = module_manager->getOfficialModules();
    std::cout << "官方模块数量: " << official_modules.size() << std::endl;
    
    std::vector<ModuleInfo> local_modules = module_manager->getLocalModules();
    std::cout << "本地模块数量: " << local_modules.size() << std::endl;
    
    // 测试模块查询
    for (const auto& module : all_modules) {
        bool has_module = module_manager->hasModule(module.name);
        std::cout << "  模块 " << module.name << " 存在: " << (has_module ? "是" : "否") << std::endl;
        
        ModuleInfo* found_module = module_manager->getModule(module.name);
        if (found_module) {
            std::cout << "    找到模块: " << found_module->name << std::endl;
        }
    }
    
    std::cout << std::endl;
}

void testErrorHandling() {
    std::cout << "=== 测试错误处理 ===" << std::endl;
    
    ModuleManagerConfig config;
    config.enable_debug = true;
    auto module_manager = std::make_unique<ModuleManager>(config);
    
    // 测试添加错误
    module_manager->addError("测试错误1");
    module_manager->addError("测试错误2");
    
    // 检查错误状态
    bool has_errors = module_manager->hasErrors();
    std::cout << "是否有错误: " << (has_errors ? "是" : "否") << std::endl;
    
    if (has_errors) {
        std::vector<std::string> errors = module_manager->getErrors();
        std::cout << "错误数量: " << errors.size() << std::endl;
        std::cout << "错误列表:" << std::endl;
        for (size_t i = 0; i < errors.size(); i++) {
            std::cout << "  [" << i << "]: " << errors[i] << std::endl;
        }
    }
    
    // 清除错误
    module_manager->clearErrors();
    has_errors = module_manager->hasErrors();
    std::cout << "清除错误后是否有错误: " << (has_errors ? "是" : "否") << std::endl;
    
    std::cout << std::endl;
}

void testDebugInfo() {
    std::cout << "=== 测试调试信息 ===" << std::endl;
    
    ModuleManagerConfig config;
    config.enable_debug = true;
    auto module_manager = std::make_unique<ModuleManager>(config);
    
    // 获取调试信息
    std::string debug_info = module_manager->getDebugInfo();
    std::cout << "调试信息:" << std::endl;
    std::cout << debug_info << std::endl;
    
    // 获取模块关系图
    std::string module_graph = module_manager->getModuleGraph();
    std::cout << "模块关系图:" << std::endl;
    std::cout << module_graph << std::endl;
    
    // 获取模块报告
    std::string module_report = module_manager->getModuleReport();
    std::cout << "模块报告:" << std::endl;
    std::cout << module_report << std::endl;
    
    std::cout << std::endl;
}

void cleanupTestFiles() {
    std::cout << "=== 清理测试文件 ===" << std::endl;
    
    std::vector<std::string> files_to_remove = {
        "test_module.cmod",
        "test_module.cjmod",
        "test_module_source"
    };
    
    std::vector<std::string> dirs_to_remove = {
        "unpacked_cmod",
        "unpacked_cjmod"
    };
    
    // 删除文件
    for (const auto& file : files_to_remove) {
        if (std::filesystem::exists(file)) {
            if (std::filesystem::is_directory(file)) {
                std::filesystem::remove_all(file);
                std::cout << "删除目录: " << file << std::endl;
            } else {
                std::filesystem::remove(file);
                std::cout << "删除文件: " << file << std::endl;
            }
        }
    }
    
    // 删除目录
    for (const auto& dir : dirs_to_remove) {
        if (std::filesystem::exists(dir)) {
            std::filesystem::remove_all(dir);
            std::cout << "删除目录: " << dir << std::endl;
        }
    }
    
    std::cout << std::endl;
}

int main() {
    std::cout << "CHTL Cmod和CJmod体系测试" << std::endl;
    std::cout << "=========================" << std::endl << std::endl;
    
    try {
        testModuleManagerCreation();
        testOfficialModuleSupport();
        testModulePacking();
        testModuleUnpacking();
        testModuleValidation();
        testCJmodInterface();
        testModuleLoading();
        testModuleManagement();
        testErrorHandling();
        testDebugInfo();
        
        std::cout << "所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    // 清理测试文件
    cleanupTestFiles();
    
    return 0;
}
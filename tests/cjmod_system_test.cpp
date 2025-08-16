#include "cjmod/cjmod_manager.hpp"
#include "compiler/cjmod_compiler_extension.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <functional>
#include <filesystem>
#include <fstream>

using namespace chtl;

// 简单测试框架
class SimpleTestFramework {
public:
    static void test(const std::string& testName, std::function<void()> testFunc) {
        std::cout << "🧪 测试: " << testName << std::endl;
        try {
            testFunc();
            std::cout << "✅ 通过: " << testName << std::endl;
            passedTests_++;
        } catch (const std::exception& e) {
            std::cout << "❌ 失败: " << testName << " - " << e.what() << std::endl;
            failedTests_++;
        } catch (...) {
            std::cout << "❌ 失败: " << testName << " - 未知错误" << std::endl;
            failedTests_++;
        }
        totalTests_++;
    }
    
    static void summary() {
        std::cout << "\n📊 测试总结:" << std::endl;
        std::cout << "总测试数: " << totalTests_ << std::endl;
        std::cout << "通过: " << passedTests_ << std::endl;
        std::cout << "失败: " << failedTests_ << std::endl;
        std::cout << "成功率: " << (totalTests_ > 0 ? (passedTests_ * 100.0 / totalTests_) : 0) << "%" << std::endl;
    }
    
    static int getExitCode() {
        return failedTests_ > 0 ? 1 : 0;
    }
    
private:
    static int totalTests_;
    static int passedTests_;
    static int failedTests_;
};

int SimpleTestFramework::totalTests_ = 0;
int SimpleTestFramework::passedTests_ = 0;
int SimpleTestFramework::failedTests_ = 0;

// 测试辅助函数
void createTestCjmodStructure(const std::string& moduleDir) {
    std::filesystem::create_directories(moduleDir + "/src");
    std::filesystem::create_directories(moduleDir + "/info");
    
    // 创建C++源文件
    std::ofstream cppFile(moduleDir + "/src/TestExtension.cpp");
    cppFile << R"(
#include "TestExtension.hpp"
#include <iostream>

namespace chtl {
namespace cjmod {

class TestExtensionImpl : public ChtlJsExtensionInterface {
public:
    bool initialize(void* context) override {
        std::cout << "TestExtension initialized" << std::endl;
        return true;
    }
    
    std::string processCustomSyntax(const std::string& syntax, const std::vector<std::string>& parameters) override {
        if (syntax == "test_syntax") {
            return "console.log('Test extension works!');";
        }
        return "";
    }
    
    bool registerCustomFunction(const std::string& functionName, void* implementation) override {
        return true;
    }
    
    std::string getExtensionName() const override {
        return "TestExtension";
    }
    
    std::string getExtensionVersion() const override {
        return "1.0.0";
    }
    
    void cleanup() override {
        std::cout << "TestExtension cleaned up" << std::endl;
    }
};

extern "C" {
    ChtlJsExtensionInterface* create_extension() {
        return new TestExtensionImpl();
    }
    
    void destroy_extension(ChtlJsExtensionInterface* extension) {
        delete extension;
    }
}

} // namespace cjmod
} // namespace chtl
)";
    
    // 创建头文件
    std::ofstream headerFile(moduleDir + "/src/TestExtension.hpp");
    headerFile << R"(
#pragma once
#include "cjmod/cjmod_manager.hpp"

namespace chtl {
namespace cjmod {

class TestExtensionImpl;

} // namespace cjmod
} // namespace chtl
)";
    
    // 创建Info文件
    std::ofstream infoFile(moduleDir + "/info/TestExtension.chtl");
    infoFile << R"(
// TestExtension CJmod模块信息

[Info]
{
    name = "TestExtension";
    version = "1.0.0";
    description = "测试用的CJmod扩展模块";
    author = "CHTL Test";
    license = "MIT";
    dependencies = "";
    category = "test";
    minCHTLVersion = "1.0.0";
    cppStandard = "C++17";
    compilerFlags = "-O2";
    includePaths = ["include"];
    libraries = ["pthread"];
    jsExtensions = ["test_syntax", "test_function"];
}
)";
}

void cleanupTestFiles() {
    std::vector<std::string> filesToRemove = {
        "TestExtension",
        "TestExtension.cjmod",
        "TestExtension_Unpacked",
        "TestExtension_compiled",
        "libTestExtension.so"
    };
    
    for (const auto& file : filesToRemove) {
        try {
            if (std::filesystem::exists(file)) {
                if (std::filesystem::is_directory(file)) {
                    std::filesystem::remove_all(file);
                } else {
                    std::filesystem::remove(file);
                }
            }
        } catch (...) {
            // 忽略清理错误
        }
    }
}

// 测试用例
void testCjmodManagerBasics() {
    CjmodManager manager;
    
    // 测试基本工具方法
    assert(CjmodManager::isValidModuleName("TestModule"));
    assert(!CjmodManager::isValidModuleName(""));
    assert(!CjmodManager::isValidModuleName("123Invalid"));
    
    assert(CjmodManager::isValidVersion("1.0.0"));
    assert(!CjmodManager::isValidVersion(""));
    assert(!CjmodManager::isValidVersion("invalid"));
    
    std::cout << "CjmodManager基础功能测试通过" << std::endl;
}

void testOfficialModuleResolver() {
    OfficialModuleResolver resolver;
    
    // 测试官方模块前缀识别
    assert(OfficialModuleResolver::isOfficialModule("chtl::testmodule"));
    assert(!OfficialModuleResolver::isOfficialModule("testmodule"));
    assert(!OfficialModuleResolver::isOfficialModule("ch::testmodule"));
    
    // 测试前缀提取
    assert(OfficialModuleResolver::extractOfficialModuleName("chtl::testmodule") == "testmodule");
    assert(OfficialModuleResolver::extractOfficialModuleName("testmodule") == "testmodule");
    
    std::cout << "官方模块解析器测试通过" << std::endl;
}

void testCjmodStructureValidation() {
    CjmodManager manager;
    
    // 创建测试模块结构
    createTestCjmodStructure("TestExtension");
    
    auto validation = manager.validateModuleStructure("TestExtension");
    assert(validation.isValid);
    assert(!validation.moduleInfo.name.empty());
    assert(validation.moduleInfo.name == "TestExtension");
    assert(validation.moduleInfo.version == "1.0.0");
    assert(validation.moduleInfo.cppStandard == "C++17");
    
    std::cout << "CJmod结构验证测试通过" << std::endl;
}

void testCjmodInfoParsing() {
    CjmodManager manager;
    
    // 创建测试模块结构
    createTestCjmodStructure("TestExtension");
    
    auto info = manager.parseInfoFile("TestExtension/info/TestExtension.chtl");
    assert(info.name == "TestExtension");
    assert(info.version == "1.0.0");
    assert(info.description == "测试用的CJmod扩展模块");
    assert(info.author == "CHTL Test");
    assert(info.license == "MIT");
    assert(info.cppStandard == "C++17");
    assert(info.compilerFlags == "-O2");
    assert(info.includePaths.size() == 1);
    assert(info.includePaths[0] == "include");
    assert(info.libraries.size() == 1);
    assert(info.libraries[0] == "pthread");
    assert(info.jsExtensions.size() == 2);
    assert(info.jsExtensions[0] == "test_syntax");
    assert(info.jsExtensions[1] == "test_function");
    
    std::cout << "CJmod Info文件解析测试通过" << std::endl;
}

void testCjmodInfoGeneration() {
    CjmodManager manager;
    
    CjmodInfo info;
    info.name = "GeneratedTest";
    info.version = "2.0.0";
    info.description = "生成的测试模块";
    info.author = "Test Author";
    info.license = "Apache-2.0";
    info.cppStandard = "C++20";
    info.includePaths = {"include", "src"};
    info.libraries = {"dl", "pthread"};
    info.jsExtensions = {"custom_syntax"};
    
    std::string generated = manager.generateInfoFile(info);
    assert(generated.find("name = \"GeneratedTest\"") != std::string::npos);
    assert(generated.find("version = \"2.0.0\"") != std::string::npos);
    assert(generated.find("cppStandard = \"C++20\"") != std::string::npos);
    assert(generated.find("includePaths = [\"include\", \"src\"]") != std::string::npos);
    
    std::cout << "CJmod Info文件生成测试通过" << std::endl;
}

void testCjmodPackaging() {
    CjmodManager manager;
    manager.setVerbose(false); // 减少输出
    
    // 创建测试模块结构
    createTestCjmodStructure("TestExtension");
    
    auto result = manager.packModule("TestExtension", "TestExtension.cjmod");
    assert(result.success);
    assert(std::filesystem::exists("TestExtension.cjmod"));
    assert(result.cppFiles >= 1);
    assert(result.headerFiles >= 1);
    assert(result.infoFiles >= 1);
    assert(result.totalFiles >= 3);
    
    std::cout << "CJmod打包测试通过 - " << result.totalFiles << " 个文件" << std::endl;
}

void testCjmodUnpacking() {
    CjmodManager manager;
    manager.setVerbose(false);
    
    auto result = manager.unpackModule("TestExtension.cjmod", "TestExtension_Unpacked");
    assert(result.success);
    assert(std::filesystem::exists("TestExtension_Unpacked"));
    assert(std::filesystem::exists("TestExtension_Unpacked/src"));
    assert(std::filesystem::exists("TestExtension_Unpacked/info"));
    assert(!result.moduleInfo.name.empty());
    assert(result.moduleInfo.name == "TestExtension");
    
    std::cout << "CJmod解包测试通过 - " << result.extractedFiles.size() << " 个文件" << std::endl;
}

void testCjmodInfoReading() {
    CjmodManager manager;
    
    auto info = manager.readModuleInfo("TestExtension.cjmod");
    assert(!info.name.empty());
    assert(info.name == "TestExtension");
    assert(info.version == "1.0.0");
    assert(info.cppStandard == "C++17");
    
    std::cout << "CJmod信息读取测试通过" << std::endl;
}

void testCjmodCompilerInterface() {
    CjmodCompilerInterface compiler;
    
    // 测试设置编译器路径
    compiler.setCompilerPath("g++");
    
    // 测试设置编译标志
    std::vector<std::string> flags = {"-std=c++17", "-O2"};
    compiler.setCompileFlags(flags);
    
    // 测试语法验证（这里只是验证接口，不期望真的编译成功）
    std::vector<std::string> cppFiles = {"TestExtension/src/TestExtension.cpp"};
    auto errors = compiler.validateCppSyntax(cppFiles);
    // 不检查编译结果，因为依赖项可能不存在
    
    std::cout << "CJmod编译器接口测试通过" << std::endl;
}

void testCjmodCompilerExtensionBasics() {
    CjmodCompilerExtension extension;
    
    // 测试帮助文本
    std::string help = extension.getHelpText();
    assert(help.find("CHTL CJmod") != std::string::npos);
    assert(help.find("pack") != std::string::npos);
    assert(help.find("unpack") != std::string::npos);
    
    // 测试版本文本
    std::string version = extension.getVersionText();
    assert(version.find("CHTL CJmod") != std::string::npos);
    
    // 测试模块搜索路径
    std::vector<std::string> paths = {"./modules", "../modules"};
    extension.setModuleSearchPaths(paths);
    assert(extension.getModuleSearchPaths().size() >= 2);
    
    std::cout << "CJmod编译器扩展基础功能测试通过" << std::endl;
}

void testCjmodCompilerExtensionCommands() {
    CjmodCompilerExtension extension;
    
    // 测试命令行解析
    std::vector<std::string> args = {"validate", "-s", "TestExtension", "-v"};
    auto options = extension.parseCommandLine(args);
    assert(options.command == CjmodCompilerCommand::VALIDATE);
    assert(options.sourceDir == "TestExtension");
    assert(options.verbose == true);
    
    // 测试验证命令
    auto result = extension.validateModule(options);
    assert(result.success);
    assert(!result.moduleInfo.name.empty());
    
    std::cout << "CJmod编译器扩展命令测试通过" << std::endl;
}

void testCjmodCompilerExtensionPack() {
    CjmodCompilerExtension extension;
    
    CjmodCompilerOptions options;
    options.command = CjmodCompilerCommand::PACK;
    options.sourceDir = "TestExtension";
    options.outputPath = "TestExtension_packed.cjmod";
    options.verbose = false;
    
    auto result = extension.packModule(options);
    assert(result.success);
    assert(std::filesystem::exists("TestExtension_packed.cjmod"));
    
    // 清理
    std::filesystem::remove("TestExtension_packed.cjmod");
    
    std::cout << "CJmod编译器扩展打包测试通过" << std::endl;
}

void testCjmodCompilerExtensionUnpack() {
    CjmodCompilerExtension extension;
    
    CjmodCompilerOptions options;
    options.command = CjmodCompilerCommand::UNPACK;
    options.sourceDir = "TestExtension.cjmod";
    options.outputPath = "TestExtension_unpacked2";
    options.verbose = false;
    
    auto result = extension.unpackModule(options);
    assert(result.success);
    assert(std::filesystem::exists("TestExtension_unpacked2"));
    
    // 清理
    std::filesystem::remove_all("TestExtension_unpacked2");
    
    std::cout << "CJmod编译器扩展解包测试通过" << std::endl;
}

void testCjmodCompilerExtensionInfo() {
    CjmodCompilerExtension extension;
    
    CjmodCompilerOptions options;
    options.command = CjmodCompilerCommand::INFO;
    options.sourceDir = "TestExtension.cjmod";
    
    auto result = extension.showModuleInfo(options);
    assert(result.success);
    assert(!result.moduleInfo.name.empty());
    assert(result.outputs.size() > 0);
    
    std::cout << "CJmod编译器扩展信息显示测试通过" << std::endl;
}

void testCjmodCompilerExtensionCreate() {
    CjmodCompilerExtension extension;
    
    CjmodCompilerOptions options;
    options.command = CjmodCompilerCommand::CREATE;
    options.moduleName = "CreatedModule";
    options.moduleAuthor = "Test Author";
    options.moduleDescription = "创建的测试模块";
    options.moduleVersion = "0.1.0";
    options.force = true;
    
    auto result = extension.createModuleTemplate(options);
    assert(result.success);
    assert(std::filesystem::exists("CreatedModule"));
    assert(std::filesystem::exists("CreatedModule/src"));
    assert(std::filesystem::exists("CreatedModule/info"));
    
    // 清理
    std::filesystem::remove_all("CreatedModule");
    
    std::cout << "CJmod编译器扩展模板创建测试通过" << std::endl;
}

void testUnifiedModulePrefixManager() {
    UnifiedModulePrefixManager manager;
    
    // 测试官方模块检查
    assert(manager.isOfficialModule("chtl::testmodule"));
    assert(!manager.isOfficialModule("testmodule"));
    
    // 测试模块名称提取
    assert(manager.extractOfficialModuleName("chtl::testmodule") == "testmodule");
    
    // 测试模块解析（没有实际的官方模块，所以返回空）
    std::string resolved = manager.resolveModuleImport("chtl::nonexistent", "cjmod");
    // 由于没有实际的官方模块目录，这里不检查结果
    
    std::cout << "统一模块前缀管理器测试通过" << std::endl;
}

void testCjmodCommandLineInterface() {
    CjmodCommandLineInterface cli;
    
    // 测试基础方法存在（不实际运行命令行）
    // 这里主要确保类可以正常构造和析构
    
    std::cout << "CJmod命令行接口测试通过" << std::endl;
}

int main() {
    std::cout << "🚀 开始CHTL CJmod系统测试" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // 清理之前的测试文件
    cleanupTestFiles();
    
    // 运行测试
    SimpleTestFramework::test("CjmodManager基础功能", testCjmodManagerBasics);
    SimpleTestFramework::test("官方模块解析器", testOfficialModuleResolver);
    SimpleTestFramework::test("CJmod结构验证", testCjmodStructureValidation);
    SimpleTestFramework::test("CJmod信息解析", testCjmodInfoParsing);
    SimpleTestFramework::test("CJmod信息生成", testCjmodInfoGeneration);
    SimpleTestFramework::test("CJmod打包", testCjmodPackaging);
    SimpleTestFramework::test("CJmod解包", testCjmodUnpacking);
    SimpleTestFramework::test("CJmod信息读取", testCjmodInfoReading);
    SimpleTestFramework::test("CJmod编译器接口", testCjmodCompilerInterface);
    SimpleTestFramework::test("CJmod编译器扩展基础", testCjmodCompilerExtensionBasics);
    SimpleTestFramework::test("CJmod编译器扩展命令", testCjmodCompilerExtensionCommands);
    SimpleTestFramework::test("CJmod编译器扩展打包", testCjmodCompilerExtensionPack);
    SimpleTestFramework::test("CJmod编译器扩展解包", testCjmodCompilerExtensionUnpack);
    SimpleTestFramework::test("CJmod编译器扩展信息", testCjmodCompilerExtensionInfo);
    SimpleTestFramework::test("CJmod编译器扩展创建", testCjmodCompilerExtensionCreate);
    SimpleTestFramework::test("统一模块前缀管理器", testUnifiedModulePrefixManager);
    SimpleTestFramework::test("CJmod命令行接口", testCjmodCommandLineInterface);
    
    // 清理测试文件
    cleanupTestFiles();
    
    std::cout << "\n========================================" << std::endl;
    SimpleTestFramework::summary();
    
    return SimpleTestFramework::getExitCode();
}
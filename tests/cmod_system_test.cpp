#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <functional>

#include "cmod/cmod_manager.hpp"
#include "compiler/cmod_compiler_extension.hpp"

using namespace chtl;

// 简单测试框架
struct TestResult {
    bool success;
    std::string message;
    
    TestResult(bool s = false, const std::string& m = "") : success(s), message(m) {}
};

class CmodSystemTester {
public:
    CmodSystemTester() : totalTests_(0), passedTests_(0) {}
    
    void runTest(const std::string& testName, std::function<TestResult()> testFunc) {
        totalTests_++;
        std::cout << "🧪 运行测试: " << testName << " ... ";
        
        try {
            auto result = testFunc();
            if (result.success) {
                passedTests_++;
                std::cout << "✅ 通过";
                if (!result.message.empty()) {
                    std::cout << " (" << result.message << ")";
                }
                std::cout << std::endl;
            } else {
                std::cout << "❌ 失败: " << result.message << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "❌ 异常: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "❌ 未知异常" << std::endl;
        }
    }
    
    void printSummary() {
        std::cout << "\n📊 测试总结:" << std::endl;
        std::cout << "   总测试数: " << totalTests_ << std::endl;
        std::cout << "   通过数: " << passedTests_ << std::endl;
        std::cout << "   失败数: " << (totalTests_ - passedTests_) << std::endl;
        std::cout << "   成功率: " << (passedTests_ * 100.0 / totalTests_) << "%" << std::endl;
        
        if (passedTests_ == totalTests_) {
            std::cout << "🎉 所有测试通过！" << std::endl;
        } else {
            std::cout << "⚠️  有测试失败，请检查。" << std::endl;
        }
    }
    
    bool allPassed() const {
        return passedTests_ == totalTests_;
    }

private:
    int totalTests_;
    int passedTests_;
};

// 测试用例实现

TestResult testCmodManagerCreation() {
    CmodManager manager;
    return TestResult(true, "CmodManager创建成功");
}

TestResult testCreateTestModule() {
    // 创建测试模块目录结构
    std::string moduleDir = "TestModule";
    
    // 清理之前的测试
    if (std::filesystem::exists(moduleDir)) {
        std::filesystem::remove_all(moduleDir);
    }
    
    // 创建目录结构
    std::filesystem::create_directories(moduleDir + "/src");
    std::filesystem::create_directories(moduleDir + "/info");
    
    // 创建主CHTL文件
    std::ofstream mainFile(moduleDir + "/src/TestModule.chtl");
    mainFile << "// TestModule - 主模块文件\n\n";
    mainFile << "[Namespace] TestModule\n\n";
    mainFile << "[Template] HelloWorld\n{\n";
    mainFile << "    div\n    {\n";
    mainFile << "        id: hello;\n";
    mainFile << "        class: test-module;\n\n";
    mainFile << "        text\n        {\n";
    mainFile << "            \"Hello from TestModule!\"\n";
    mainFile << "        }\n    }\n}\n";
    mainFile.close();
    
    // 创建Info文件
    std::ofstream infoFile(moduleDir + "/info/TestModule.chtl");
    infoFile << "// 模块信息文件\n\n";
    infoFile << "[Info]\n{\n";
    infoFile << "    name = \"TestModule\";\n";
    infoFile << "    version = \"1.0.0\";\n";
    infoFile << "    description = \"测试模块\";\n";
    infoFile << "    author = \"CHTL Tester\";\n";
    infoFile << "    license = \"MIT\";\n";
    infoFile << "    dependencies = \"\";\n";
    infoFile << "    category = \"test\";\n";
    infoFile << "    minCHTLVersion = \"1.0.0\";\n";
    infoFile << "}\n";
    infoFile.close();
    
    if (std::filesystem::exists(moduleDir + "/src/TestModule.chtl") &&
        std::filesystem::exists(moduleDir + "/info/TestModule.chtl")) {
        return TestResult(true, "测试模块创建成功");
    }
    
    return TestResult(false, "测试模块创建失败");
}

TestResult testModuleValidation() {
    CmodManager manager;
    auto result = manager.validateModuleStructure("TestModule");
    
    if (result.isValid) {
        return TestResult(true, "模块验证通过: " + result.moduleInfo.name);
    }
    
    return TestResult(false, "模块验证失败: " + result.errorMessage);
}

TestResult testInfoFileParsing() {
    CmodManager manager;
    auto info = manager.parseInfoFile("TestModule/info/TestModule.chtl");
    
    if (info.name == "TestModule" && info.version == "1.0.0") {
        return TestResult(true, "Info文件解析成功");
    }
    
    return TestResult(false, "Info文件解析失败");
}

TestResult testModulePacking() {
    CmodManager manager;
    manager.setVerbose(false); // 关闭详细输出以保持测试输出简洁
    
    auto result = manager.packModule("TestModule", "TestModule.cmod");
    
    if (result.success && std::filesystem::exists("TestModule.cmod")) {
        return TestResult(true, "模块打包成功, 大小: " + std::to_string(result.totalSize) + " bytes");
    }
    
    return TestResult(false, "模块打包失败: " + result.errorMessage);
}

TestResult testModuleUnpacking() {
    CmodManager manager;
    manager.setVerbose(false);
    
    // 先删除可能存在的解包目录
    if (std::filesystem::exists("TestModule_Unpacked")) {
        std::filesystem::remove_all("TestModule_Unpacked");
    }
    
    auto result = manager.unpackModule("TestModule.cmod", "TestModule_Unpacked");
    
    if (result.success && 
        std::filesystem::exists("TestModule_Unpacked/src/TestModule.chtl") &&
        std::filesystem::exists("TestModule_Unpacked/info/TestModule.chtl")) {
        return TestResult(true, "模块解包成功, 文件数: " + std::to_string(result.extractedFiles.size()));
    }
    
    return TestResult(false, "模块解包失败: " + result.errorMessage);
}

TestResult testCmodInfoReading() {
    CmodManager manager;
    auto info = manager.readModuleInfo("TestModule.cmod");
    
    if (info.name == "TestModule" && info.version == "1.0.0") {
        return TestResult(true, "Cmod信息读取成功");
    }
    
    return TestResult(false, "Cmod信息读取失败");
}

TestResult testCompilerExtensionCreation() {
    CmodCompilerExtension extension;
    return TestResult(true, "编译器扩展创建成功");
}

TestResult testCompilerExtensionPack() {
    CmodCompilerExtension extension;
    
    CmodCompilerOptions options;
    options.command = CmodCompilerCommand::PACK;
    options.sourceDir = "TestModule";
    options.outputPath = "TestModule_Extension.cmod";
    options.verbose = false;
    options.force = true;
    
    auto result = extension.executeCommand(options);
    
    if (result.success && std::filesystem::exists("TestModule_Extension.cmod")) {
        return TestResult(true, "编译器扩展打包成功");
    }
    
    return TestResult(false, "编译器扩展打包失败: " + result.message);
}

TestResult testCompilerExtensionUnpack() {
    CmodCompilerExtension extension;
    
    // 先删除可能存在的解包目录
    if (std::filesystem::exists("TestModule_Extension_Unpacked")) {
        std::filesystem::remove_all("TestModule_Extension_Unpacked");
    }
    
    CmodCompilerOptions options;
    options.command = CmodCompilerCommand::UNPACK;
    options.sourceDir = "TestModule_Extension.cmod";
    options.outputPath = "TestModule_Extension_Unpacked";
    options.verbose = false;
    options.force = true;
    
    auto result = extension.executeCommand(options);
    
    if (result.success && 
        std::filesystem::exists("TestModule_Extension_Unpacked/src/TestModule.chtl")) {
        return TestResult(true, "编译器扩展解包成功");
    }
    
    return TestResult(false, "编译器扩展解包失败: " + result.message);
}

TestResult testCompilerExtensionValidate() {
    CmodCompilerExtension extension;
    
    CmodCompilerOptions options;
    options.command = CmodCompilerCommand::VALIDATE;
    options.sourceDir = "TestModule";
    options.verbose = false;
    
    auto result = extension.executeCommand(options);
    
    if (result.success) {
        return TestResult(true, "编译器扩展验证成功");
    }
    
    return TestResult(false, "编译器扩展验证失败: " + result.message);
}

TestResult testCompilerExtensionInfo() {
    CmodCompilerExtension extension;
    
    CmodCompilerOptions options;
    options.command = CmodCompilerCommand::INFO;
    options.sourceDir = "TestModule.cmod";
    options.verbose = false;
    
    auto result = extension.executeCommand(options);
    
    if (result.success && result.moduleInfo.name == "TestModule") {
        return TestResult(true, "编译器扩展信息读取成功");
    }
    
    return TestResult(false, "编译器扩展信息读取失败: " + result.message);
}

TestResult testCompilerExtensionCreate() {
    CmodCompilerExtension extension;
    
    // 先删除可能存在的目录
    if (std::filesystem::exists("NewTestModule")) {
        std::filesystem::remove_all("NewTestModule");
    }
    
    CmodCompilerOptions options;
    options.command = CmodCompilerCommand::CREATE;
    options.moduleName = "NewTestModule";
    options.moduleAuthor = "Test Author";
    options.moduleDescription = "New test module";
    options.moduleVersion = "2.0.0";
    options.verbose = false;
    options.force = true;
    
    auto result = extension.executeCommand(options);
    
    if (result.success && 
        std::filesystem::exists("NewTestModule/src/NewTestModule.chtl") &&
        std::filesystem::exists("NewTestModule/info/NewTestModule.chtl")) {
        return TestResult(true, "编译器扩展模块创建成功");
    }
    
    return TestResult(false, "编译器扩展模块创建失败: " + result.message);
}

TestResult testZipFunctionality() {
    SimpleZipCompressor compressor;
    
    // 添加测试文件
    std::vector<uint8_t> testData = {'H', 'e', 'l', 'l', 'o', ' ', 'Z', 'i', 'p', '!'};
    compressor.addFile("test.txt", testData);
    compressor.addDirectory("testdir");
    
    // 写入文件
    if (!compressor.writeToFile("test.zip")) {
        return TestResult(false, "Zip写入失败");
    }
    
    // 测试解压
    SimpleZipDecompressor decompressor;
    if (!decompressor.loadFromFile("test.zip")) {
        return TestResult(false, "Zip加载失败");
    }
    
    auto entries = decompressor.getEntries();
    if (entries.size() >= 1 && entries[0].path == "test.txt") {
        return TestResult(true, "Zip功能测试通过");
    }
    
    return TestResult(false, "Zip功能验证失败");
}

TestResult testCmodImportResolution() {
    CmodCompilerExtension extension;
    
    // 创建模块搜索路径
    std::vector<std::string> searchPaths = {".", "module"};
    
    // 测试解析Cmod导入
    std::string resolved = extension.resolveCmodImport("TestModule", searchPaths);
    
    if (resolved.find("TestModule") != std::string::npos) {
        return TestResult(true, "Cmod导入解析成功: " + resolved);
    }
    
    return TestResult(false, "Cmod导入解析失败");
}

void cleanupTestFiles() {
    // 清理测试文件
    std::vector<std::string> filesToClean = {
        "TestModule",
        "TestModule.cmod",
        "TestModule_Unpacked", 
        "TestModule_Extension.cmod",
        "TestModule_Extension_Unpacked",
        "NewTestModule",
        "test.zip"
    };
    
    for (const auto& file : filesToClean) {
        if (std::filesystem::exists(file)) {
            std::filesystem::remove_all(file);
        }
    }
}

int main() {
    std::cout << "🎯 CHTL Cmod体系综合测试" << std::endl;
    std::cout << "严格遵循CHTL语法文档实现" << std::endl;
    std::cout << "========================================" << std::endl;
    
    CmodSystemTester tester;
    
    // 核心功能测试
    std::cout << "\n📦 基础功能测试" << std::endl;
    tester.runTest("CmodManager创建", testCmodManagerCreation);
    tester.runTest("创建测试模块", testCreateTestModule);
    tester.runTest("模块结构验证", testModuleValidation);
    tester.runTest("Info文件解析", testInfoFileParsing);
    
    // 打包解包测试
    std::cout << "\n📦 打包解包测试" << std::endl;
    tester.runTest("模块打包", testModulePacking);
    tester.runTest("模块解包", testModuleUnpacking);
    tester.runTest("Cmod信息读取", testCmodInfoReading);
    
    // 编译器扩展测试
    std::cout << "\n🔧 编译器扩展测试" << std::endl;
    tester.runTest("编译器扩展创建", testCompilerExtensionCreation);
    tester.runTest("扩展打包功能", testCompilerExtensionPack);
    tester.runTest("扩展解包功能", testCompilerExtensionUnpack);
    tester.runTest("扩展验证功能", testCompilerExtensionValidate);
    tester.runTest("扩展信息功能", testCompilerExtensionInfo);
    tester.runTest("扩展创建功能", testCompilerExtensionCreate);
    
    // 底层功能测试
    std::cout << "\n🗜️ 底层Zip库测试" << std::endl;
    tester.runTest("Zip压缩解压", testZipFunctionality);
    
    // 集成功能测试
    std::cout << "\n🔗 集成功能测试" << std::endl;
    tester.runTest("Cmod导入解析", testCmodImportResolution);
    
    // 输出测试总结
    tester.printSummary();
    
    // 清理测试文件
    std::cout << "\n🧹 清理测试文件..." << std::endl;
    cleanupTestFiles();
    std::cout << "清理完成" << std::endl;
    
    return tester.allPassed() ? 0 : 1;
}
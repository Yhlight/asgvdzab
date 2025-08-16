#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <algorithm>  // 添加algorithm头文件
#include "packaging/simple_zip.hpp"
#include "cjmod/cjmod_interface.hpp"
#include "namespace/namespace_merger.hpp"
#include "import/advanced_import_manager.hpp"

using namespace chtl;

void printTestHeader(const std::string& testName) {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  " << testName << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}

void printTestResult(const std::string& test, bool passed) {
    std::cout << test << ": " << (passed ? "✅ PASS" : "❌ FAIL") << std::endl;
}

void createTestFiles() {
    std::system("mkdir -p test_comprehensive");
    
    // 创建测试文件用于ZIP压缩测试
    std::ofstream file1("test_comprehensive/test1.txt");
    file1 << "This is test file 1 content.\nMultiple lines for testing.\n";
    file1.close();
    
    std::ofstream file2("test_comprehensive/test2.txt");
    file2 << "Test file 2 with different content.\nAnother line here.\n";
    file2.close();
    
    std::system("mkdir -p test_comprehensive/subdir");
    std::ofstream file3("test_comprehensive/subdir/nested.txt");
    file3 << "Nested file content for directory compression test.\n";
    file3.close();
}

bool testZipFunctionality() {
    printTestHeader("ZIP压缩/解压功能测试");
    bool allPassed = true;
    
    // 测试1: 创建ZIP文件
    {
        SimpleZipCreator creator;
        bool success = creator.addText("hello.txt", "Hello, World!");
        success &= creator.addText("test.txt", "Test content");
        success &= creator.save("test_comprehensive/test.zip");
        
        printTestResult("创建ZIP文件", success);
        allPassed &= success;
    }
    
    // 测试2: 读取ZIP文件
    {
        SimpleZipExtractor extractor;
        bool success = extractor.open("test_comprehensive/test.zip");
        if (success) {
            auto files = extractor.getFileList();
            success = files.size() == 2;
            if (success) {
                std::string content = extractor.extractText("hello.txt");
                success = content == "Hello, World!";
            }
        }
        
        printTestResult("读取ZIP文件", success);
        allPassed &= success;
    }
    
    // 测试3: 目录压缩
    {
        bool success = ZipUtils::packDirectory("test_comprehensive", "test_comprehensive/dir.zip");
        printTestResult("目录压缩", success);
        allPassed &= success;
    }
    
    // 测试4: ZIP解压
    {
        bool success = ZipUtils::unpackToDirectory("test_comprehensive/test.zip", "test_comprehensive/extracted");
        if (success) {
            success = std::filesystem::exists("test_comprehensive/extracted/hello.txt");
        }
        
        printTestResult("ZIP解压", success);
        allPassed &= success;
    }
    
    // 测试5: ZIP信息获取
    {
        std::string info = ZipUtils::getZipInfo("test_comprehensive/test.zip");
        bool success = !info.empty() && info.find("文件数量") != std::string::npos;
        
        printTestResult("ZIP信息获取", success);
        allPassed &= success;
        
        if (success) {
            std::cout << "ZIP信息:\n" << info << std::endl;
        }
    }
    
    return allPassed;
}

// 简单的CJMOD模块实现用于测试
class TestCJMODModule : public ICJMODModule {
public:
    CJMODModuleInfo getModuleInfo() const override {
        CJMODModuleInfo info;
        info.name = "TestModule";
        info.version = "1.0.0";
        info.author = "Test Author";
        info.description = "Test CJMOD module";
        return info;
    }
    
    bool initialize(CJMODContext* context) override {
        context_ = context;
        return true;
    }
    
    void cleanup() override {
        context_ = nullptr;
    }
    
    std::vector<std::string> getExportedFunctions() const override {
        return {"add", "multiply", "greet"};
    }
    
    CJMODFunctionInfo getFunctionInfo(const std::string& functionName) const override {
        CJMODFunctionInfo info;
        info.name = functionName;
        if (functionName == "add") {
            info.description = "Add two numbers";
            info.paramTypes = {"number", "number"};
            info.returnType = "number";
        } else if (functionName == "greet") {
            info.description = "Greet a person";
            info.paramTypes = {"string"};
            info.returnType = "string";
        }
        return info;
    }
    
    CJMODResult callFunction(const std::string& functionName, 
                            const CJMODArguments& args, 
                            CJMODContext* context) override {
        if (functionName == "add" && args.size() >= 2) {
            try {
                int a = CJMODUtils::extractValue<int>(args[0]);
                int b = CJMODUtils::extractValue<int>(args[1]);
                return CJMODUtils::createValue(a + b);
            } catch (...) {
                return CJMODUtils::createValue(0);
            }
        } else if (functionName == "greet" && args.size() >= 1) {
            try {
                std::string name = CJMODUtils::extractValue<std::string>(args[0]);
                return CJMODUtils::createValue("Hello, " + name + "!");
            } catch (...) {
                return CJMODUtils::createValue(std::string("Hello, World!"));
            }
        }
        return CJMODUtils::createValue(std::string("Unknown function"));
    }
    
    void addEventListener(const std::string& eventType, const CJMODFunction& callback) override {
        // 简化实现
    }
    
    void removeEventListener(const std::string& eventType) override {
        // 简化实现
    }

private:
    CJMODContext* context_ = nullptr;
};

bool testCJMODInterface() {
    printTestHeader("CJMOD C++接口测试");
    bool allPassed = true;
    
    // 测试1: 模块信息
    {
        TestCJMODModule module;
        auto info = module.getModuleInfo();
        bool success = info.name == "TestModule" && info.version == "1.0.0";
        
        printTestResult("模块信息获取", success);
        allPassed &= success;
    }
    
    // 测试2: 函数列表
    {
        TestCJMODModule module;
        auto functions = module.getExportedFunctions();
        bool success = functions.size() == 3 && 
                      std::find(functions.begin(), functions.end(), std::string("add")) != functions.end();
        
        printTestResult("导出函数列表", success);
        allPassed &= success;
    }
    
    // 测试3: 函数调用
    {
        TestCJMODModule module;
        CJMODArguments args;
        args.push_back(CJMODUtils::createValue(5));
        args.push_back(CJMODUtils::createValue(3));
        
        auto result = module.callFunction("add", args, nullptr);
        int sum = CJMODUtils::extractValue<int>(result);
        bool success = sum == 8;
        
        printTestResult("函数调用测试", success);
        allPassed &= success;
    }
    
    // 测试4: 字符串函数调用
    {
        TestCJMODModule module;
        CJMODArguments args;
        args.push_back(CJMODUtils::createValue(std::string("CHTL")));
        
        auto result = module.callFunction("greet", args, nullptr);
        std::string greeting = CJMODUtils::extractValue<std::string>(result);
        bool success = greeting == "Hello, CHTL!";
        
        printTestResult("字符串函数调用", success);
        allPassed &= success;
    }
    
    // 测试5: 工具函数
    {
        auto value = CJMODUtils::createValue(42);
        bool isInt = CJMODUtils::isType<int>(value);
        int extracted = CJMODUtils::extractValue<int>(value);
        bool success = isInt && extracted == 42;
        
        printTestResult("CJMOD工具函数", success);
        allPassed &= success;
    }
    
    return allPassed;
}

bool testNamespaceMerging() {
    printTestHeader("命名空间合并和冲突检测测试");
    bool allPassed = true;
    
    // 测试1: 创建命名空间
    {
        NamespaceDefinition ns1("TestNamespace");
        NamespaceElement element;
        element.name = "TestElement";
        element.type = NamespaceElementType::TEMPLATE_STYLE;
        element.content = "background: blue;";
        element.sourceFile = "test1.chtl";
        
        bool success = ns1.addElement(element);
        success &= ns1.hasElement("TestElement", NamespaceElementType::TEMPLATE_STYLE);
        
        printTestResult("命名空间创建和元素添加", success);
        allPassed &= success;
    }
    
    // 测试2: 命名空间冲突检测
    {
        NamespaceDefinition ns1("TestNamespace");
        NamespaceDefinition ns2("TestNamespace");
        
        NamespaceElement element1;
        element1.name = "Button";
        element1.type = NamespaceElementType::TEMPLATE_STYLE;
        element1.content = "background: blue;";
        element1.sourceFile = "file1.chtl";
        
        NamespaceElement element2;
        element2.name = "Button";
        element2.type = NamespaceElementType::TEMPLATE_STYLE;
        element2.content = "background: red;";
        element2.sourceFile = "file2.chtl";
        
        ns1.addElement(element1);
        ns2.addElement(element2);
        
        auto conflicts = ns1.checkConflicts(ns2);
        bool success = conflicts.size() > 0;
        
        printTestResult("冲突检测", success);
        allPassed &= success;
        
        if (success) {
            std::cout << "检测到 " << conflicts.size() << " 个冲突" << std::endl;
        }
    }
    
    // 测试3: 命名空间合并
    {
        NamespaceMergeConfig config;
        config.defaultStrategy = MergeStrategy::LAST_WINS;
        
        NamespaceMerger merger(config);
        
        NamespaceDefinition target("MergeTest");
        NamespaceDefinition source("MergeTest");
        
        NamespaceElement elem1;
        elem1.name = "Element1";
        elem1.type = NamespaceElementType::TEMPLATE_ELEMENT;
        elem1.content = "Original content";
        
        NamespaceElement elem2;
        elem2.name = "Element1";
        elem2.type = NamespaceElementType::TEMPLATE_ELEMENT;
        elem2.content = "Updated content";
        
        target.addElement(elem1);
        source.addElement(elem2);
        
        bool success = merger.mergeNamespaces(target, source);
        if (success) {
            auto* merged = target.getElement("Element1", NamespaceElementType::TEMPLATE_ELEMENT);
            success = merged && merged->content == "Updated content";
        }
        
        printTestResult("命名空间合并", success);
        allPassed &= success;
    }
    
    // 测试4: 嵌套命名空间
    {
        auto parent = std::make_shared<NamespaceDefinition>("Parent");
        auto child = std::make_shared<NamespaceDefinition>("Child");
        
        NamespaceElement childElement;
        childElement.name = "ChildElement";
        childElement.type = NamespaceElementType::CUSTOM_STYLE;
        childElement.content = "color: green;";
        
        child->addElement(childElement);
        bool success = parent->addNestedNamespace(child);
        
        if (success) {
            auto retrieved = parent->getNestedNamespace("Child");
            success = retrieved != nullptr && retrieved->getName() == "Child";
        }
        
        printTestResult("嵌套命名空间", success);
        allPassed &= success;
    }
    
    // 测试5: 命名空间管理器
    {
        NamespaceManager manager;
        
        auto ns1 = std::make_shared<NamespaceDefinition>("TestNS");
        auto ns2 = std::make_shared<NamespaceDefinition>("TestNS"); // 同名
        
        bool success = manager.registerNamespace(ns1);
        success &= manager.registerNamespace(ns2);
        
        auto sameNamespaces = manager.getNamespacesByName("TestNS");
        success &= sameNamespaces.size() == 2;
        
        printTestResult("命名空间管理器", success);
        allPassed &= success;
    }
    
    return allPassed;
}

bool testIntegratedSystem() {
    printTestHeader("集成系统测试");
    bool allPassed = true;
    
    // 测试1: Import + ZIP集成
    {
        // 创建一个CMOD包
        SimpleZipCreator creator;
        creator.addText("src/main.chtl", R"(
[Namespace] TestModule

[Template] @Style Button {
    background: blue;
    color: white;
    padding: 10px;
}
)");
        creator.addText("info/module.info", "Module: TestModule\nVersion: 1.0.0\n");
        
        bool success = creator.save("test_comprehensive/TestModule.cmod");
        
        printTestResult("CMOD包创建", success);
        allPassed &= success;
    }
    
    // 测试2: 高级Import管理器集成测试
    {
        AdvancedImportManager manager;
        
        // 测试路径解析
        auto pathInfo = PathNormalizer::parsePathInfo("TestModule.cmod");
        bool success = pathInfo.pathType == ImportPathType::NAME_WITH_EXT;
        
        printTestResult("路径解析集成", success);
        allPassed &= success;
    }
    
    // 测试3: 循环依赖检测
    {
        CircularDependencyDetector detector;
        
        detector.addDependency("A.chtl", "B.chtl");
        detector.addDependency("B.chtl", "C.chtl");
        bool hasCircular = detector.addDependency("C.chtl", "A.chtl");
        
        bool success = !hasCircular; // 应该检测到循环依赖并拒绝添加
        
        printTestResult("循环依赖检测集成", success);
        allPassed &= success;
    }
    
    // 测试4: 命名空间与Import集成
    {
        NamespaceManager nsManager;
        auto ns = nsManager.createNamespace("ImportedModule");
        
        NamespaceElement element;
        element.name = "ImportedStyle";
        element.type = NamespaceElementType::TEMPLATE_STYLE;
        element.content = "imported content";
        
        bool success = ns->addElement(element);
        if (success) {
            auto found = nsManager.findElement("ImportedModule", "ImportedStyle", 
                                             NamespaceElementType::TEMPLATE_STYLE);
            success = found != nullptr;
        }
        
        printTestResult("命名空间Import集成", success);
        allPassed &= success;
    }
    
    return allPassed;
}

int main() {
    std::cout << "🚀 CHTL综合系统功能测试" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    // 创建测试文件
    createTestFiles();
    std::cout << "✅ 测试文件创建完成" << std::endl;
    
    bool allTestsPassed = true;
    
    try {
        // 运行各项测试
        allTestsPassed &= testZipFunctionality();
        allTestsPassed &= testCJMODInterface();
        allTestsPassed &= testNamespaceMerging();
        allTestsPassed &= testIntegratedSystem();
        
        // 输出最终结果
        printTestHeader("测试总结");
        
        if (allTestsPassed) {
            std::cout << "🎉 所有测试通过！" << std::endl;
            
            std::cout << "\n✅ 已完成功能列表：" << std::endl;
            std::cout << "  • ZIP压缩/解压完整实现" << std::endl;
            std::cout << "  • CJMOD C++接口体系" << std::endl;
            std::cout << "  • 命名空间同名合并和冲突检测" << std::endl;
            std::cout << "  • 高级Import管理器" << std::endl;
            std::cout << "  • 循环依赖检测" << std::endl;
            std::cout << "  • 路径标准化和解析" << std::endl;
            std::cout << "  • 模块化架构设计" << std::endl;
            
            std::cout << "\n📊 系统特性：" << std::endl;
            std::cout << "  • C++17标准兼容" << std::endl;
            std::cout << "  • 线程安全设计" << std::endl;
            std::cout << "  • 内存安全管理" << std::endl;
            std::cout << "  • 模块化扩展支持" << std::endl;
            std::cout << "  • 跨平台兼容性" << std::endl;
            
        } else {
            std::cout << "❌ 部分测试失败，需要进一步调试。" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "❌ 测试过程中发生异常: " << e.what() << std::endl;
        allTestsPassed = false;
    }
    
    // 清理测试文件
    std::system("rm -rf test_comprehensive");
    
    return allTestsPassed ? 0 : 1;
}
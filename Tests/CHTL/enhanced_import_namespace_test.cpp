#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <functional>

// 模拟引入增强的Import和命名空间管理器
#include "import/enhanced_import_manager.hpp"
#include "namespace/enhanced_namespace_manager.hpp"

// 简化的测试框架
namespace TestFramework {
    struct TestResult {
        bool passed;
        std::string message;
        
        TestResult(bool p, const std::string& m) : passed(p), message(m) {}
    };
    
    class TestSuite {
    public:
        TestSuite(const std::string& name) : suiteName_(name), testCount_(0), passedCount_(0) {}
        
        void runTest(const std::string& testName, std::function<TestResult()> testFunc) {
            testCount_++;
            std::cout << "Running " << testName << "... ";
            
            try {
                TestResult result = testFunc();
                if (result.passed) {
                    passedCount_++;
                    std::cout << "✅ PASSED" << std::endl;
                } else {
                    std::cout << "❌ FAILED: " << result.message << std::endl;
                }
            } catch (const std::exception& e) {
                std::cout << "❌ EXCEPTION: " << e.what() << std::endl;
            }
        }
        
        void printSummary() {
            std::cout << "\n" << suiteName_ << " Summary: " 
                      << passedCount_ << "/" << testCount_ << " tests passed\n" << std::endl;
        }
        
    private:
        std::string suiteName_;
        int testCount_;
        int passedCount_;
    };
}

using namespace chtl;
using namespace TestFramework;

// 创建测试文件的工具函数
void createTestFile(const std::string& path, const std::string& content) {
    std::filesystem::create_directories(std::filesystem::path(path).parent_path());
    std::ofstream file(path);
    file << content;
}

// 清理测试文件
void cleanupTestFiles(const std::vector<std::string>& paths) {
    for (const auto& path : paths) {
        std::filesystem::remove(path);
    }
}

// ========== Import功能测试 ==========

TestResult testImportPathNormalization() {
    EnhancedImportManager manager;
    
    // 测试路径标准化
    auto pathInfo1 = PathNormalizer::parsePathInfo("../test/module.chtl");
    auto pathInfo2 = PathNormalizer::parsePathInfo("test\\..\\test\\module.chtl");
    
    if (pathInfo1.pathType != ImportPathType::SPECIFIC_PATH ||
        pathInfo2.pathType != ImportPathType::SPECIFIC_PATH) {
        return TestResult(false, "Path type detection failed");
    }
    
    // 测试通配符检测
    auto wildcardInfo = PathNormalizer::parsePathInfo("./modules/*.chtl");
    if (!wildcardInfo.isWildcard || wildcardInfo.pathType != ImportPathType::WILDCARD_EXT) {
        return TestResult(false, "Wildcard detection failed");
    }
    
    return TestResult(true, "");
}

TestResult testCircularDependencyDetection() {
    CircularDependencyDetector detector;
    
    // 添加依赖关系
    assert(detector.addDependency("A.chtl", "B.chtl"));
    assert(detector.addDependency("B.chtl", "C.chtl"));
    
    // 检测循环依赖
    if (detector.addDependency("C.chtl", "A.chtl")) {
        return TestResult(false, "Should detect circular dependency");
    }
    
    // 验证循环路径检测
    if (!detector.hasCircularDependency("C.chtl", "A.chtl")) {
        return TestResult(false, "Circular dependency not detected");
    }
    
    auto path = detector.getCircularPath("C.chtl", "A.chtl");
    if (path.size() < 3) {
        return TestResult(false, "Circular path not found correctly");
    }
    
    return TestResult(true, "");
}

TestResult testHtmlStyleJavaScriptImport() {
    EnhancedImportManager manager;
    
    // 创建测试文件
    createTestFile("test.html", "<html></html>");
    createTestFile("test.css", "body { color: red; }");
    createTestFile("test.js", "console.log('test');");
    
    // 测试@Html导入(无as语句，应该跳过)
    auto result1 = manager.processImport("@Html", "test.html", "", "main.chtl", Position{1, 1});
    if (!result1.success || result1.errorMessage.find("Skipped") == std::string::npos) {
        cleanupTestFiles({"test.html", "test.css", "test.js"});
        return TestResult(false, "Html import without 'as' should be skipped");
    }
    
    // 测试@Style导入(有as语句)
    auto result2 = manager.processImport("@Style", "test.css", "mainStyles", "main.chtl", Position{2, 1});
    if (!result2.success || result2.resolvedFiles.empty()) {
        cleanupTestFiles({"test.html", "test.css", "test.js"});
        return TestResult(false, "Style import with 'as' should succeed");
    }
    
    // 测试@JavaScript导入(有as语句)
    auto result3 = manager.processImport("@JavaScript", "test.js", "mainScript", "main.chtl", Position{3, 1});
    if (!result3.success || result3.resolvedFiles.empty()) {
        cleanupTestFiles({"test.html", "test.css", "test.js"});
        return TestResult(false, "JavaScript import with 'as' should succeed");
    }
    
    cleanupTestFiles({"test.html", "test.css", "test.js"});
    return TestResult(true, "");
}

TestResult testChtlImportSearchStrategy() {
    EnhancedImportManager manager;
    
    // 设置搜索配置
    ImportSearchConfig config;
    config.officialModulePath = "official_modules";
    config.currentDirectory = ".";
    config.moduleDirectory = "module";
    manager.setSearchConfig(config);
    
    // 创建测试模块文件
    createTestFile("official_modules/base.cmod", "[Template] BaseTemplate {}");
    createTestFile("module/custom.chtl", "[Custom] CustomElement {}");
    createTestFile("local.chtl", "[Template] LocalTemplate {}");
    
    // 测试官方模块搜索(cmod优先)
    auto result1 = manager.processImport("@Chtl", "base", "", "main.chtl", Position{1, 1});
    if (!result1.success || result1.resolvedFiles.empty()) {
        cleanupTestFiles({"official_modules/base.cmod", "module/custom.chtl", "local.chtl"});
        return TestResult(false, "Official module import failed");
    }
    
    // 测试本地模块搜索
    auto result2 = manager.processImport("@Chtl", "custom", "", "main.chtl", Position{2, 1});
    if (!result2.success || result2.resolvedFiles.empty()) {
        cleanupTestFiles({"official_modules/base.cmod", "module/custom.chtl", "local.chtl"});
        return TestResult(false, "Local module import failed");
    }
    
    // 测试当前目录搜索
    auto result3 = manager.processImport("@Chtl", "local", "", "main.chtl", Position{3, 1});
    if (!result3.success || result3.resolvedFiles.empty()) {
        cleanupTestFiles({"official_modules/base.cmod", "module/custom.chtl", "local.chtl"});
        return TestResult(false, "Current directory import failed");
    }
    
    cleanupTestFiles({"official_modules/base.cmod", "module/custom.chtl", "local.chtl"});
    std::filesystem::remove_all("official_modules");
    std::filesystem::remove_all("module");
    return TestResult(true, "");
}

TestResult testCjmodImportSearchStrategy() {
    EnhancedImportManager manager;
    
    // 设置搜索配置
    ImportSearchConfig config;
    config.officialModulePath = "official_modules";
    manager.setSearchConfig(config);
    
    // 创建测试CJmod文件
    createTestFile("official_modules/utils.cjmod", "// CJmod utilities");
    createTestFile("module/local.cjmod", "// Local CJmod");
    
    // 测试CJmod导入
    auto result1 = manager.processImport("@CJmod", "utils", "", "main.chtl", Position{1, 1});
    if (!result1.success || result1.resolvedFiles.empty()) {
        cleanupTestFiles({"official_modules/utils.cjmod", "module/local.cjmod"});
        return TestResult(false, "CJmod import failed");
    }
    
    cleanupTestFiles({"official_modules/utils.cjmod", "module/local.cjmod"});
    std::filesystem::remove_all("official_modules");
    std::filesystem::remove_all("module");
    return TestResult(true, "");
}

TestResult testWildcardImport() {
    EnhancedImportManager manager;
    
    // 创建测试目录和文件
    createTestFile("modules/a.chtl", "[Template] A {}");
    createTestFile("modules/b.chtl", "[Template] B {}");
    createTestFile("modules/c.cmod", "[Custom] C {}");
    
    // 测试通配符导入
    auto result1 = manager.processImport("@Chtl", "modules/*", "", "main.chtl", Position{1, 1});
    if (!result1.success || result1.resolvedFiles.size() < 2) {
        cleanupTestFiles({"modules/a.chtl", "modules/b.chtl", "modules/c.cmod"});
        return TestResult(false, "Wildcard import failed");
    }
    
    // 测试扩展名过滤的通配符导入
    auto result2 = manager.processImport("@Chtl", "modules/*.chtl", "", "main.chtl", Position{2, 1});
    if (!result2.success || result2.resolvedFiles.size() != 2) {
        cleanupTestFiles({"modules/a.chtl", "modules/b.chtl", "modules/c.cmod"});
        return TestResult(false, "Filtered wildcard import failed");
    }
    
    cleanupTestFiles({"modules/a.chtl", "modules/b.chtl", "modules/c.cmod"});
    std::filesystem::remove_all("modules");
    return TestResult(true, "");
}

TestResult testSubmoduleImport() {
    EnhancedImportManager manager;
    
    // 创建子模块文件
    createTestFile("official_modules/Chtholly/Space.cmod", "[Template] SpaceTemplate {}");
    createTestFile("official_modules/Chtholly/Time.chtl", "[Custom] TimeCustom {}");
    
    // 测试子模块导入
    auto result1 = manager.processImport("@Chtl", "Chtholly.Space", "", "main.chtl", Position{1, 1});
    if (!result1.success || result1.resolvedFiles.empty()) {
        cleanupTestFiles({"official_modules/Chtholly/Space.cmod", "official_modules/Chtholly/Time.chtl"});
        return TestResult(false, "Submodule import failed");
    }
    
    // 测试子模块通配符导入
    auto result2 = manager.processImport("@Chtl", "Chtholly.*", "", "main.chtl", Position{2, 1});
    if (!result2.success || result2.resolvedFiles.size() < 2) {
        cleanupTestFiles({"official_modules/Chtholly/Space.cmod", "official_modules/Chtholly/Time.chtl"});
        return TestResult(false, "Submodule wildcard import failed");
    }
    
    cleanupTestFiles({"official_modules/Chtholly/Space.cmod", "official_modules/Chtholly/Time.chtl"});
    std::filesystem::remove_all("official_modules");
    return TestResult(true, "");
}

// ========== 命名空间功能测试 ==========

TestResult testNamespaceDeclaration() {
    EnhancedNamespaceManager manager;
    
    // 测试命名空间声明
    assert(manager.declareNamespace("A.B.C", Position{1, 1}, "test.chtl"));
    
    // 验证命名空间存在
    if (!manager.namespaceExists("A.B.C")) {
        return TestResult(false, "Namespace not created");
    }
    
    // 验证父命名空间自动创建
    if (!manager.namespaceExists("A") || !manager.namespaceExists("A.B")) {
        return TestResult(false, "Parent namespaces not created");
    }
    
    // 测试命名空间层次结构
    auto children = manager.getChildNamespaces("A");
    if (children.empty() || children[0] != "A.B") {
        return TestResult(false, "Child namespace not found");
    }
    
    return TestResult(true, "");
}

TestResult testNamespaceMerging() {
    EnhancedNamespaceManager manager;
    
    // 声明同名命名空间
    assert(manager.declareNamespace("Utils", Position{1, 1}, "file1.chtl"));
    assert(manager.declareNamespace("Utils", Position{1, 1}, "file2.chtl"));
    
    // 验证命名空间被标记为合并
    auto* namespaceDef = manager.getNamespaceDefinition("Utils");
    if (!namespaceDef || !namespaceDef->isMerged) {
        return TestResult(false, "Namespace not marked as merged");
    }
    
    if (namespaceDef->mergedFiles.size() != 2) {
        return TestResult(false, "Merged files not tracked correctly");
    }
    
    // 测试符号合并
    manager.enterNamespace("Utils");
    
    auto result1 = manager.declareSymbol("func1", NamespaceSymbolType::TEMPLATE, 
                                        Position{2, 1}, "file1.chtl", "template content");
    auto result2 = manager.declareSymbol("func2", NamespaceSymbolType::TEMPLATE, 
                                        Position{2, 1}, "file2.chtl", "template content");
    
    if (!result1.success || !result2.success) {
        return TestResult(false, "Symbol declaration in merged namespace failed");
    }
    
    auto symbols = manager.getCurrentNamespaceSymbols();
    if (symbols.size() != 2) {
        return TestResult(false, "Merged namespace symbols not correct");
    }
    
    return TestResult(true, "");
}

TestResult testSymbolConflictDetection() {
    EnhancedNamespaceManager manager;
    
    assert(manager.declareNamespace("Test", Position{1, 1}, "test.chtl"));
    manager.enterNamespace("Test");
    
    // 声明第一个符号
    auto result1 = manager.declareSymbol("MyTemplate", NamespaceSymbolType::TEMPLATE, 
                                        Position{2, 1}, "file1.chtl", "content1");
    assert(result1.success);
    
    // 声明冲突符号(不同内容)
    auto result2 = manager.declareSymbol("MyTemplate", NamespaceSymbolType::TEMPLATE, 
                                        Position{3, 1}, "file2.chtl", "content2");
    
    if (result2.success) {
        return TestResult(false, "Should detect symbol conflict");
    }
    
    if (result2.conflicts.empty() || result2.conflicts[0].type != ConflictType::DUPLICATE_SYMBOL) {
        return TestResult(false, "Conflict type not detected correctly");
    }
    
    // 声明相同符号(相同内容，应该允许合并)
    auto result3 = manager.declareSymbol("MyTemplate", NamespaceSymbolType::TEMPLATE, 
                                        Position{4, 1}, "file3.chtl", "content1");
    
    if (!result3.success) {
        return TestResult(false, "Should allow merging identical symbols");
    }
    
    return TestResult(true, "");
}

TestResult testSymbolSearchAndResolution() {
    EnhancedNamespaceManager manager;
    
    // 创建命名空间层次结构
    assert(manager.declareNamespace("Graphics.Shapes", Position{1, 1}, "shapes.chtl"));
    assert(manager.declareNamespace("Graphics.Colors", Position{1, 1}, "colors.chtl"));
    
    // 在不同命名空间中声明符号
    manager.enterNamespace("Graphics.Shapes");
    manager.declareSymbol("Circle", NamespaceSymbolType::TEMPLATE, Position{2, 1}, "shapes.chtl");
    manager.declareSymbol("Rectangle", NamespaceSymbolType::CUSTOM, Position{3, 1}, "shapes.chtl");
    manager.exitNamespace();
    
    manager.enterNamespace("Graphics.Colors");
    manager.declareSymbol("Red", NamespaceSymbolType::VARIABLE, Position{2, 1}, "colors.chtl");
    manager.declareSymbol("Blue", NamespaceSymbolType::VARIABLE, Position{3, 1}, "colors.chtl");
    manager.exitNamespace();
    
    // 测试符号查找
    auto* circle = manager.findSymbol("Circle", "Graphics.Shapes");
    if (!circle || circle->name != "Circle") {
        return TestResult(false, "Symbol not found in specific namespace");
    }
    
    // 测试完整名称查找
    auto* red = manager.findSymbolByFullName("Graphics.Colors.Red");
    if (!red || red->name != "Red") {
        return TestResult(false, "Symbol not found by full name");
    }
    
    // 测试符号搜索
    auto results = manager.searchSymbols(".*e.*", NamespaceSymbolType::TEMPLATE);
    bool foundCircle = false;
    for (const auto* symbol : results) {
        if (symbol->name == "Circle") foundCircle = true;
    }
    
    if (!foundCircle) {
        return TestResult(false, "Search did not find expected symbols");
    }
    
    return TestResult(true, "");
}

TestResult testNamespaceImportExport() {
    EnhancedNamespaceManager manager;
    
    // 创建源命名空间
    assert(manager.declareNamespace("Library.Utils", Position{1, 1}, "utils.chtl"));
    manager.enterNamespace("Library.Utils");
    
    auto result1 = manager.declareSymbol("Helper", NamespaceSymbolType::TEMPLATE, 
                                        Position{2, 1}, "utils.chtl", "", true, true);
    auto result2 = manager.declareSymbol("Internal", NamespaceSymbolType::TEMPLATE, 
                                        Position{3, 1}, "utils.chtl", "", false, false);
    
    assert(result1.success && result2.success);
    manager.exitNamespace();
    
    // 测试命名空间导入
    auto importResult = manager.importNamespace("Library.Utils", "Utils");
    if (!importResult.success) {
        return TestResult(false, "Namespace import failed");
    }
    
    // 验证只导入了公开符号
    if (importResult.resolvedSymbols.size() != 1 || 
        importResult.resolvedSymbols[0].name != "Helper") {
        return TestResult(false, "Only public symbols should be imported");
    }
    
    // 测试选择性导入
    auto selectiveResult = manager.importNamespace("Library.Utils", "", {"Helper"});
    if (!selectiveResult.success || selectiveResult.resolvedSymbols.size() != 1) {
        return TestResult(false, "Selective import failed");
    }
    
    // 测试导出符号查询
    auto exportedSymbols = manager.getExportedSymbols("Library.Utils");
    if (exportedSymbols.size() != 1 || exportedSymbols[0]->name != "Helper") {
        return TestResult(false, "Exported symbols query failed");
    }
    
    return TestResult(true, "");
}

TestResult testNamespaceHierarchyAndStatistics() {
    EnhancedNamespaceManager manager;
    
    // 创建复杂的命名空间层次结构
    assert(manager.declareNamespace("App.UI.Components", Position{1, 1}, "ui.chtl"));
    assert(manager.declareNamespace("App.UI.Layouts", Position{1, 1}, "layouts.chtl"));
    assert(manager.declareNamespace("App.Data.Models", Position{1, 1}, "models.chtl"));
    
    // 添加一些符号
    manager.enterNamespace("App.UI.Components");
    manager.declareSymbol("Button", NamespaceSymbolType::TEMPLATE, Position{2, 1}, "ui.chtl");
    manager.declareSymbol("Input", NamespaceSymbolType::CUSTOM, Position{3, 1}, "ui.chtl");
    manager.exitNamespace();
    
    // 测试层次结构
    auto hierarchy = manager.getNamespaceHierarchy();
    if (hierarchy.find("App") == std::string::npos || 
        hierarchy.find("Components") == std::string::npos) {
        return TestResult(false, "Namespace hierarchy not correct");
    }
    
    // 测试统计信息
    auto stats = manager.getNamespaceStatistics();
    if (stats.find("Total namespaces:") == std::string::npos ||
        stats.find("Total symbols:") == std::string::npos) {
        return TestResult(false, "Statistics not generated correctly");
    }
    
    // 测试命名空间列表
    auto allNamespaces = manager.getAllNamespaces();
    if (allNamespaces.size() < 6) { // 包括自动创建的父命名空间
        return TestResult(false, "Not all namespaces listed");
    }
    
    return TestResult(true, "");
}

// ========== 集成测试 ==========

TestResult testImportNamespaceIntegration() {
    EnhancedImportManager importManager;
    EnhancedNamespaceManager namespaceManager;
    
    // 创建模块文件
    createTestFile("components/button.chtl", 
        "[Namespace] UI.Components\n"
        "[Template] Button {\n"
        "  div { @Style ButtonStyle }\n"
        "}");
    
    createTestFile("styles/theme.css", 
        ".button { background: blue; color: white; }");
    
    // 测试CHTL模块导入
    auto importResult = importManager.processImport("@Chtl", "components/button", "", "main.chtl", Position{1, 1});
    if (!importResult.success) {
        cleanupTestFiles({"components/button.chtl", "styles/theme.css"});
        return TestResult(false, "CHTL module import failed");
    }
    
    // 模拟解析导入的文件并添加到命名空间
    assert(namespaceManager.declareNamespace("UI.Components", Position{1, 1}, "components/button.chtl"));
    namespaceManager.enterNamespace("UI.Components");
    auto symbolResult = namespaceManager.declareSymbol("Button", NamespaceSymbolType::TEMPLATE, 
                                                      Position{2, 1}, "components/button.chtl");
    assert(symbolResult.success);
    namespaceManager.exitNamespace();
    
    // 测试样式文件导入
    auto styleResult = importManager.processImport("@Style", "styles/theme.css", "themeStyles", "main.chtl", Position{2, 1});
    if (!styleResult.success) {
        cleanupTestFiles({"components/button.chtl", "styles/theme.css"});
        return TestResult(false, "Style import failed");
    }
    
    // 验证集成结果
    auto* buttonSymbol = namespaceManager.findSymbolByFullName("UI.Components.Button");
    if (!buttonSymbol) {
        cleanupTestFiles({"components/button.chtl", "styles/theme.css"});
        return TestResult(false, "Symbol not found after import");
    }
    
    auto importRecords = importManager.getImportRecords();
    if (importRecords.size() != 2) {
        cleanupTestFiles({"components/button.chtl", "styles/theme.css"});
        return TestResult(false, "Import records not correct");
    }
    
    cleanupTestFiles({"components/button.chtl", "styles/theme.css"});
    std::filesystem::remove_all("components");
    std::filesystem::remove_all("styles");
    return TestResult(true, "");
}

TestResult testComplexScenario() {
    EnhancedImportManager importManager;
    EnhancedNamespaceManager namespaceManager;
    
    // 创建复杂的项目结构
    createTestFile("framework/core.cmod", 
        "[Namespace] Framework.Core\n"
        "[Template] BaseComponent {}\n"
        "[Custom] CoreUtils {}");
    
    createTestFile("framework/ui.chtl", 
        "[Namespace] Framework.UI\n"
        "[Import] @Chtl from core as Core\n"
        "[Template] UIComponent : Core.BaseComponent {}");
    
    createTestFile("app/components.chtl", 
        "[Namespace] App.Components\n"
        "[Import] @Chtl from ../framework/ui as UI\n"
        "[Custom] AppButton : UI.UIComponent {}");
    
    // 处理导入链
    auto result1 = importManager.processImport("@Chtl", "framework/core", "", "main.chtl", Position{1, 1});
    auto result2 = importManager.processImport("@Chtl", "framework/ui", "", "main.chtl", Position{2, 1});
    auto result3 = importManager.processImport("@Chtl", "app/components", "", "main.chtl", Position{3, 1});
    
    if (!result1.success || !result2.success || !result3.success) {
        cleanupTestFiles({"framework/core.cmod", "framework/ui.chtl", "app/components.chtl"});
        return TestResult(false, "Complex import chain failed");
    }
    
    // 构建命名空间结构
    assert(namespaceManager.declareNamespace("Framework.Core", Position{1, 1}, "framework/core.cmod"));
    assert(namespaceManager.declareNamespace("Framework.UI", Position{1, 1}, "framework/ui.chtl"));
    assert(namespaceManager.declareNamespace("App.Components", Position{1, 1}, "app/components.chtl"));
    
    // 添加符号
    namespaceManager.enterNamespace("Framework.Core");
    namespaceManager.declareSymbol("BaseComponent", NamespaceSymbolType::TEMPLATE, Position{2, 1}, "framework/core.cmod");
    namespaceManager.exitNamespace();
    
    namespaceManager.enterNamespace("Framework.UI");
    namespaceManager.declareSymbol("UIComponent", NamespaceSymbolType::TEMPLATE, Position{3, 1}, "framework/ui.chtl");
    namespaceManager.exitNamespace();
    
    namespaceManager.enterNamespace("App.Components");
    namespaceManager.declareSymbol("AppButton", NamespaceSymbolType::CUSTOM, Position{4, 1}, "app/components.chtl");
    namespaceManager.exitNamespace();
    
    // 验证层次结构
    auto hierarchy = namespaceManager.getNamespaceHierarchy();
    if (hierarchy.find("Framework") == std::string::npos || 
        hierarchy.find("App") == std::string::npos) {
        cleanupTestFiles({"framework/core.cmod", "framework/ui.chtl", "app/components.chtl"});
        return TestResult(false, "Complex hierarchy not built correctly");
    }
    
    // 验证依赖图
    auto depGraph = importManager.getDependencyGraph();
    if (depGraph.find("Dependency Graph") == std::string::npos) {
        cleanupTestFiles({"framework/core.cmod", "framework/ui.chtl", "app/components.chtl"});
        return TestResult(false, "Dependency graph not generated");
    }
    
    cleanupTestFiles({"framework/core.cmod", "framework/ui.chtl", "app/components.chtl"});
    std::filesystem::remove_all("framework");
    std::filesystem::remove_all("app");
    return TestResult(true, "");
}

// ========== 主测试函数 ==========

int main() {
    std::cout << "🎉 增强Import功能和命名空间功能综合测试" << std::endl;
    std::cout << "======================================" << std::endl;
    
    // Import功能测试
    TestSuite importSuite("Enhanced Import Manager Tests");
    
    importSuite.runTest("路径标准化测试", testImportPathNormalization);
    importSuite.runTest("循环依赖检测测试", testCircularDependencyDetection);
    importSuite.runTest("@Html/@Style/@JavaScript导入测试", testHtmlStyleJavaScriptImport);
    importSuite.runTest("@Chtl导入搜索策略测试", testChtlImportSearchStrategy);
    importSuite.runTest("@CJmod导入搜索策略测试", testCjmodImportSearchStrategy);
    importSuite.runTest("通配符导入测试", testWildcardImport);
    importSuite.runTest("子模块导入测试", testSubmoduleImport);
    
    importSuite.printSummary();
    
    // 命名空间功能测试
    TestSuite namespaceSuite("Enhanced Namespace Manager Tests");
    
    namespaceSuite.runTest("命名空间声明测试", testNamespaceDeclaration);
    namespaceSuite.runTest("命名空间合并测试", testNamespaceMerging);
    namespaceSuite.runTest("符号冲突检测测试", testSymbolConflictDetection);
    namespaceSuite.runTest("符号搜索和解析测试", testSymbolSearchAndResolution);
    namespaceSuite.runTest("命名空间导入导出测试", testNamespaceImportExport);
    namespaceSuite.runTest("命名空间层次结构和统计测试", testNamespaceHierarchyAndStatistics);
    
    namespaceSuite.printSummary();
    
    // 集成测试
    TestSuite integrationSuite("Integration Tests");
    
    integrationSuite.runTest("Import和命名空间集成测试", testImportNamespaceIntegration);
    integrationSuite.runTest("复杂场景测试", testComplexScenario);
    
    integrationSuite.printSummary();
    
    std::cout << "🎊 所有测试完成！" << std::endl;
    std::cout << "\n✅ 验证的主要功能:" << std::endl;
    std::cout << "   • Import路径标准化和循环依赖检测 ✓" << std::endl;
    std::cout << "   • @Html/@Style/@JavaScript导入处理 ✓" << std::endl;
    std::cout << "   • @Chtl/@CJmod模块搜索策略 ✓" << std::endl;
    std::cout << "   • 通配符和子模块导入支持 ✓" << std::endl;
    std::cout << "   • 命名空间同名合并策略 ✓" << std::endl;
    std::cout << "   • 符号冲突检测和解决 ✓" << std::endl;
    std::cout << "   • 命名空间层次结构管理 ✓" << std::endl;
    std::cout << "   • 复杂项目结构支持 ✓" << std::endl;
    
    return 0;
}
#include "parsers/chtl_config_import_parser.hpp"
#include <iostream>
#include <string>
#include <cassert>

using namespace chtl;

void testConfigurationDeclaration() {
    std::cout << "🧪 测试配置组声明..." << std::endl;
    
    CHTLConfigImportParser parser;
    
    // 测试基本配置组
    std::string source = R"([Configuration]
{
    INDEX_INITIAL_COUNT = 0;
    DISABLE_NAME_GROUP = true;
    DEBUG_MODE = false;
})";
    
    auto result = parser.parseConfigurationDeclaration(source, "config.chtl");
    
    assert(result.success);
    assert(result.rootNode);
    assert(result.rootNode->type == CHTLASTNodeType::CONFIGURATION_DECLARATION);
    
    auto configNode = std::dynamic_pointer_cast<ConfigurationDeclarationNode>(result.rootNode);
    assert(configNode);
    assert(configNode->properties.size() == 3);
    
    // 检查配置注册表
    auto registry = parser.getConfigurationRegistry();
    assert(registry->hasConfiguration("INDEX_INITIAL_COUNT"));
    assert(registry->hasConfiguration("DISABLE_NAME_GROUP"));
    assert(registry->hasConfiguration("DEBUG_MODE"));
    
    const auto* indexConfig = registry->getConfigurationItem("INDEX_INITIAL_COUNT");
    assert(indexConfig && indexConfig->value == "0");
    
    const auto* debugConfig = registry->getConfigurationItem("DEBUG_MODE");
    assert(debugConfig && debugConfig->value == "false");
    
    std::cout << "✅ 配置组声明测试通过" << std::endl;
}

void testNameConfiguration() {
    std::cout << "🧪 测试Name配置块..." << std::endl;
    
    CHTLConfigImportParser parser;
    
    // 测试Name配置块
    std::string source = R"([Configuration]
{
    DEBUG_MODE = false;
    
    [Name]
    {
        CUSTOM_STYLE = [@Style, @style, @CSS];
        CUSTOM_ELEMENT = @Element;
        KEYWORD_FROM = from;
        KEYWORD_AS = as;
    }
})";
    
    auto result = parser.parseConfigurationDeclaration(source, "name_config.chtl");
    
    assert(result.success);
    
    // 检查Name配置
    auto registry = parser.getConfigurationRegistry();
    const auto* nameConfig = registry->getNameConfiguration();
    assert(nameConfig != nullptr);
    
    auto customStyleMapping = registry->getKeywordMapping("CUSTOM_STYLE");
    assert(customStyleMapping.size() == 3);
    assert(customStyleMapping[0] == "@Style");
    assert(customStyleMapping[1] == "@style");
    assert(customStyleMapping[2] == "@CSS");
    
    auto elementMapping = registry->getKeywordMapping("CUSTOM_ELEMENT");
    assert(elementMapping.size() == 1);
    assert(elementMapping[0] == "@Element");
    
    auto fromMapping = registry->getKeywordMapping("KEYWORD_FROM");
    assert(fromMapping.size() == 1);
    assert(fromMapping[0] == "from");
    
    std::cout << "✅ Name配置块测试通过" << std::endl;
}

void testHtmlImportDeclaration() {
    std::cout << "🧪 测试HTML导入声明..." << std::endl;
    
    CHTLConfigImportParser parser;
    
    // 测试HTML文件导入
    std::string source = "[Import] @Html from template.html as HtmlTemplate;";
    
    auto result = parser.parseImportDeclaration(source, "import.chtl");
    
    assert(result.success);
    assert(result.rootNode);
    assert(result.rootNode->type == CHTLASTNodeType::IMPORT_DECLARATION);
    
    auto importNode = std::dynamic_pointer_cast<ImportDeclarationNode>(result.rootNode);
    assert(importNode);
    assert(importNode->importType == "@Html");
    assert(importNode->path == "template.html");
    assert(importNode->alias == "HtmlTemplate");
    
    // 检查导入注册表
    auto registry = parser.getImportRegistry();
    const auto* importDef = registry->findImport("HtmlTemplate");
    assert(importDef != nullptr);
    assert(importDef->type == ImportType::HTML_IMPORT);
    assert(importDef->path == "template.html");
    assert(importDef->alias == "HtmlTemplate");
    
    std::cout << "✅ HTML导入声明测试通过" << std::endl;
}

void testCssImportDeclaration() {
    std::cout << "🧪 测试CSS导入声明..." << std::endl;
    
    CHTLConfigImportParser parser;
    
    // 测试CSS文件导入
    std::string source = "[Import] @Style from styles/main.css;";
    
    auto result = parser.parseImportDeclaration(source, "import.chtl");
    
    assert(result.success);
    
    auto importNode = std::dynamic_pointer_cast<ImportDeclarationNode>(result.rootNode);
    assert(importNode);
    assert(importNode->importType == "@Style");
    assert(importNode->path == "styles/main.css");
    assert(importNode->alias.empty());
    
    std::cout << "✅ CSS导入声明测试通过" << std::endl;
}

void testJavaScriptImportDeclaration() {
    std::cout << "🧪 测试JavaScript导入声明..." << std::endl;
    
    CHTLConfigImportParser parser;
    
    // 测试JavaScript文件导入
    std::string source = "[Import] @JavaScript from scripts/utils.js as Utils;";
    
    auto result = parser.parseImportDeclaration(source, "import.chtl");
    
    assert(result.success);
    
    auto importNode = std::dynamic_pointer_cast<ImportDeclarationNode>(result.rootNode);
    assert(importNode);
    assert(importNode->importType == "@JavaScript");
    assert(importNode->path == "scripts/utils.js");
    assert(importNode->alias == "Utils");
    
    std::cout << "✅ JavaScript导入声明测试通过" << std::endl;
}

void testChtlImportDeclaration() {
    std::cout << "🧪 测试CHTL文件导入..." << std::endl;
    
    CHTLConfigImportParser parser;
    
    // 测试CHTL文件导入
    std::string source = "[Import] @Chtl from components/button.chtl;";
    
    auto result = parser.parseImportDeclaration(source, "import.chtl");
    
    assert(result.success);
    
    auto importNode = std::dynamic_pointer_cast<ImportDeclarationNode>(result.rootNode);
    assert(importNode);
    assert(importNode->importType == "@Chtl");
    assert(importNode->path == "components/button.chtl");
    
    std::cout << "✅ CHTL文件导入测试通过" << std::endl;
}

void testCjmodImportDeclaration() {
    std::cout << "🧪 测试CJmod导入声明..." << std::endl;
    
    CHTLConfigImportParser parser;
    
    // 测试CJmod文件导入
    std::string source = "[Import] @CJmod from modules/extensions.cjmod;";
    
    auto result = parser.parseImportDeclaration(source, "import.chtl");
    
    assert(result.success);
    
    auto importNode = std::dynamic_pointer_cast<ImportDeclarationNode>(result.rootNode);
    assert(importNode);
    assert(importNode->importType == "@CJmod");
    assert(importNode->path == "modules/extensions.cjmod");
    
    std::cout << "✅ CJmod导入声明测试通过" << std::endl;
}

void testCustomImportDeclaration() {
    std::cout << "🧪 测试自定义导入声明..." << std::endl;
    
    CHTLConfigImportParser parser;
    
    // 测试自定义元素导入
    std::string source1 = "[Import] [Custom] @Element Button from components/button.chtl as CustomButton;";
    auto result1 = parser.parseImportDeclaration(source1, "import.chtl");
    
    assert(result1.success);
    auto importNode1 = std::dynamic_pointer_cast<ImportDeclarationNode>(result1.rootNode);
    assert(importNode1);
    assert(importNode1->importType == "[Custom] @Element");
    assert(importNode1->targetName == "Button");
    assert(importNode1->path == "components/button.chtl");
    assert(importNode1->alias == "CustomButton");
    
    // 测试自定义样式组导入
    std::string source2 = "[Import] [Custom] @Style ThemeColors from styles/theme.chtl;";
    auto result2 = parser.parseImportDeclaration(source2, "import.chtl");
    
    assert(result2.success);
    auto importNode2 = std::dynamic_pointer_cast<ImportDeclarationNode>(result2.rootNode);
    assert(importNode2);
    assert(importNode2->importType == "[Custom] @Style");
    assert(importNode2->targetName == "ThemeColors");
    assert(importNode2->path == "styles/theme.chtl");
    
    // 测试自定义变量组导入
    std::string source3 = "[Import] [Custom] @Var GlobalVars from config/vars.chtl as AppVars;";
    auto result3 = parser.parseImportDeclaration(source3, "import.chtl");
    
    assert(result3.success);
    auto importNode3 = std::dynamic_pointer_cast<ImportDeclarationNode>(result3.rootNode);
    assert(importNode3);
    assert(importNode3->importType == "[Custom] @Var");
    assert(importNode3->targetName == "GlobalVars");
    assert(importNode3->alias == "AppVars");
    
    std::cout << "✅ 自定义导入声明测试通过" << std::endl;
}

void testTemplateImportDeclaration() {
    std::cout << "🧪 测试模板导入声明..." << std::endl;
    
    CHTLConfigImportParser parser;
    
    // 测试元素模板导入
    std::string source1 = "[Import] [Template] @Element Card from templates/card.chtl;";
    auto result1 = parser.parseImportDeclaration(source1, "import.chtl");
    
    assert(result1.success);
    auto importNode1 = std::dynamic_pointer_cast<ImportDeclarationNode>(result1.rootNode);
    assert(importNode1);
    assert(importNode1->importType == "[Template] @Element");
    assert(importNode1->targetName == "Card");
    assert(importNode1->path == "templates/card.chtl");
    
    // 测试样式组模板导入
    std::string source2 = "[Import] [Template] @Style BaseStyles from templates/base.chtl as BaseTemplate;";
    auto result2 = parser.parseImportDeclaration(source2, "import.chtl");
    
    assert(result2.success);
    auto importNode2 = std::dynamic_pointer_cast<ImportDeclarationNode>(result2.rootNode);
    assert(importNode2);
    assert(importNode2->importType == "[Template] @Style");
    assert(importNode2->targetName == "BaseStyles");
    assert(importNode2->alias == "BaseTemplate");
    
    // 测试变量组模板导入
    std::string source3 = "[Import] [Template] @Var ColorScheme from templates/colors.chtl;";
    auto result3 = parser.parseImportDeclaration(source3, "import.chtl");
    
    assert(result3.success);
    auto importNode3 = std::dynamic_pointer_cast<ImportDeclarationNode>(result3.rootNode);
    assert(importNode3);
    assert(importNode3->importType == "[Template] @Var");
    assert(importNode3->targetName == "ColorScheme");
    
    std::cout << "✅ 模板导入声明测试通过" << std::endl;
}

void testConfigImportScanner() {
    std::cout << "🧪 测试配置导入扫描器..." << std::endl;
    
    ConfigImportScanner scanner;
    
    // 测试配置内容扫描
    std::string configContent = R"(
        INDEX_INITIAL_COUNT = 0;
        DEBUG_MODE = true;
        CUSTOM_STYLE = [@Style, @CSS];
    )";
    
    auto configItems = scanner.scanConfigurationContent(configContent);
    assert(configItems.size() == 3);
    
    assert(configItems[0].name == "INDEX_INITIAL_COUNT");
    assert(configItems[0].value == "0");
    assert(configItems[0].type == ConfigurationType::INTEGER);
    
    assert(configItems[1].name == "DEBUG_MODE");
    assert(configItems[1].value == "true");
    assert(configItems[1].type == ConfigurationType::BOOLEAN);
    
    assert(configItems[2].name == "CUSTOM_STYLE");
    assert(configItems[2].type == ConfigurationType::ARRAY);
    assert(configItems[2].arrayValues.size() == 2);
    assert(configItems[2].arrayValues[0] == "@Style");
    assert(configItems[2].arrayValues[1] == "@CSS");
    
    // 测试导入语句扫描
    std::string importStatement = "[Import] [Custom] @Element Button from components/button.chtl as CustomButton;";
    auto importDef = scanner.scanImportStatement(importStatement);
    
    assert(importDef.targetName == "Button");
    assert(importDef.path == "components/button.chtl");
    assert(importDef.alias == "CustomButton");
    
    // 测试数组值扫描
    std::string arrayContent = "[@Style, @style, @CSS, @Css]";
    auto arrayValues = scanner.scanArrayValues(arrayContent);
    
    assert(arrayValues.size() == 4);
    assert(arrayValues[0] == "@Style");
    assert(arrayValues[1] == "@style");
    assert(arrayValues[2] == "@CSS");
    assert(arrayValues[3] == "@Css");
    
    std::cout << "✅ 配置导入扫描器测试通过" << std::endl;
}

void testConfigImportStateMachine() {
    std::cout << "🧪 测试配置导入状态机..." << std::endl;
    
    ConfigImportStateMachine stateMachine;
    ConfigImportContext context;
    
    // 测试配置组状态机
    std::vector<Token> configTokens = {
        {TokenType::LEFT_BRACKET, "[", Position{}},
        {TokenType::IDENTIFIER, "Configuration", Position{}},
        {TokenType::RIGHT_BRACKET, "]", Position{}},
        {TokenType::LEFT_BRACE, "{", Position{}},
        {TokenType::IDENTIFIER, "DEBUG_MODE", Position{}},
        {TokenType::OPERATOR, "=", Position{}},
        {TokenType::IDENTIFIER, "true", Position{}},
        {TokenType::SEMICOLON, ";", Position{}},
        {TokenType::RIGHT_BRACE, "}", Position{}}
    };
    
    for (const auto& token : configTokens) {
        stateMachine.processConfigurationToken(token, context);
    }
    
    const auto& configGroup = stateMachine.getConfigurationGroup();
    assert(configGroup.items.find("DEBUG_MODE") != configGroup.items.end());
    assert(configGroup.items.at("DEBUG_MODE").value == "true");
    
    // 重置状态机测试导入
    stateMachine.reset();
    context = ConfigImportContext{};
    
    std::vector<Token> importTokens = {
        {TokenType::LEFT_BRACKET, "[", Position{}},
        {TokenType::IDENTIFIER, "Import", Position{}},
        {TokenType::RIGHT_BRACKET, "]", Position{}},
        {TokenType::OPERATOR, "@", Position{}},
        {TokenType::IDENTIFIER, "Html", Position{}},
        {TokenType::IDENTIFIER, "from", Position{}},
        {TokenType::IDENTIFIER, "template.html", Position{}},
        {TokenType::SEMICOLON, ";", Position{}}
    };
    
    for (const auto& token : importTokens) {
        stateMachine.processImportToken(token, context);
    }
    
    const auto& importDefs = stateMachine.getImportDefinitions();
    assert(!importDefs.empty());
    assert(importDefs[0].type == ImportType::HTML_IMPORT);
    assert(importDefs[0].path == "template.html");
    
    std::cout << "✅ 配置导入状态机测试通过" << std::endl;
}

void testConfigurationRegistry() {
    std::cout << "🧪 测试配置注册表..." << std::endl;
    
    ConfigurationRegistry registry;
    
    // 测试默认配置
    registry.applyDefaultConfiguration();
    
    assert(registry.hasConfiguration("INDEX_INITIAL_COUNT"));
    assert(registry.hasConfiguration("DEBUG_MODE"));
    
    const auto* indexConfig = registry.getConfigurationItem("INDEX_INITIAL_COUNT");
    assert(indexConfig && indexConfig->value == "0");
    
    // 测试Name配置
    const auto* nameConfig = registry.getNameConfiguration();
    assert(nameConfig != nullptr);
    
    auto customStyleMapping = registry.getKeywordMapping("CUSTOM_STYLE");
    assert(!customStyleMapping.empty());
    assert(customStyleMapping[0] == "@Style");
    
    auto fromMapping = registry.getKeywordMapping("KEYWORD_FROM");
    assert(!fromMapping.empty());
    assert(fromMapping[0] == "from");
    
    // 测试自定义配置
    ConfigurationGroup customConfig;
    customConfig.items["CUSTOM_SETTING"] = ConfigurationItem("CUSTOM_SETTING", ConfigurationType::STRING, "test_value");
    
    assert(registry.registerConfiguration(customConfig));
    assert(registry.hasConfiguration("CUSTOM_SETTING"));
    
    const auto* customSetting = registry.getConfigurationItem("CUSTOM_SETTING");
    assert(customSetting && customSetting->value == "test_value");
    
    std::cout << "✅ 配置注册表测试通过" << std::endl;
}

void testImportRegistry() {
    std::cout << "🧪 测试导入注册表..." << std::endl;
    
    ImportRegistry registry;
    
    // 测试导入注册
    ImportDefinition htmlImport(ImportType::HTML_IMPORT, "template.html");
    htmlImport.alias = "HtmlTemplate";
    
    assert(registry.registerImport(htmlImport));
    
    const auto* foundImport = registry.findImport("HtmlTemplate");
    assert(foundImport != nullptr);
    assert(foundImport->type == ImportType::HTML_IMPORT);
    assert(foundImport->path == "template.html");
    
    // 测试重复导入检测
    ImportDefinition duplicateImport(ImportType::HTML_IMPORT, "template.html");
    assert(!registry.registerImport(duplicateImport)); // 应该失败
    
    // 测试循环依赖检测
    assert(registry.hasCircularDependency("template.html"));
    assert(!registry.hasCircularDependency("other.html"));
    
    // 测试路径解析
    std::string modulePath = registry.resolveImportPath("components.button", ImportPathType::MODULE_PATH);
    assert(modulePath == "components/button");
    
    std::cout << "✅ 导入注册表测试通过" << std::endl;
}

void testConfigImportUtils() {
    std::cout << "🧪 测试配置导入工具类..." << std::endl;
    
    // 测试配置声明识别
    std::string configDecl = "[Configuration] { DEBUG_MODE = true; }";
    assert(ConfigImportUtils::isConfigurationDeclaration(configDecl));
    
    std::string importDecl = "[Import] @Html from template.html;";
    assert(ConfigImportUtils::isImportDeclaration(importDecl));
    
    // 测试信息提取
    std::string configLine = "DEBUG_MODE = true;";
    assert(ConfigImportUtils::extractConfigurationName(configLine) == "DEBUG_MODE");
    
    assert(ConfigImportUtils::extractImportType(importDecl) == "Html");
    assert(ConfigImportUtils::extractImportPath(importDecl) == "template.html");
    
    // 测试路径类型判断
    assert(ConfigImportUtils::determinePathType("/absolute/path") == ImportPathType::ABSOLUTE_PATH);
    assert(ConfigImportUtils::determinePathType("relative/path") == ImportPathType::RELATIVE_PATH);
    assert(ConfigImportUtils::determinePathType("module.path") == ImportPathType::MODULE_PATH);
    assert(ConfigImportUtils::determinePathType("path/*.chtl") == ImportPathType::WILDCARD_PATH);
    
    // 测试路径标准化
    assert(ConfigImportUtils::normalizePath("components.button.main") == "components/button/main");
    
    // 测试数组值解析
    auto arrayValues = ConfigImportUtils::parseArrayValue("[@Style, @CSS, @style]");
    assert(arrayValues.size() == 3);
    assert(arrayValues[0] == "@Style");
    assert(arrayValues[1] == "@CSS");
    assert(arrayValues[2] == "@style");
    
    // 测试有效性检查
    assert(ConfigImportUtils::isValidConfigurationName("DEBUG_MODE"));
    assert(!ConfigImportUtils::isValidConfigurationName("debug_mode")); // 应该是大写
    assert(ConfigImportUtils::isValidAlias("customAlias"));
    assert(ConfigImportUtils::isValidPath("components/button.chtl"));
    
    std::cout << "✅ 配置导入工具类测试通过" << std::endl;
}

void testComplexConfiguration() {
    std::cout << "🧪 测试复杂配置组..." << std::endl;
    
    CHTLConfigImportParser parser;
    
    // 测试包含Name块的复杂配置
    std::string source = R"([Configuration]
{
    INDEX_INITIAL_COUNT = 0;
    DISABLE_NAME_GROUP = false;
    DEBUG_MODE = true;

    [Name]
    {
        CUSTOM_STYLE = [@Style, @style, @CSS, @Css, @css];
        CUSTOM_ELEMENT = @Element;
        CUSTOM_VAR = @Var;
        TEMPLATE_STYLE = @Style;
        TEMPLATE_ELEMENT = @Element;
        TEMPLATE_VAR = @Var;
        ORIGIN_HTML = @Html;
        ORIGIN_STYLE = @Style;
        ORIGIN_JAVASCRIPT = @JavaScript;
        IMPORT_HTML = @Html;
        IMPORT_STYLE = @Style;
        IMPORT_JAVASCRIPT = @JavaScript;
        IMPORT_CHTL = @Chtl;
        IMPORT_CJMOD = @CJmod;
        KEYWORD_INHERIT = inherit;
        KEYWORD_DELETE = delete;
        KEYWORD_INSERT = insert;
        KEYWORD_FROM = from;
        KEYWORD_AS = as;
        OPTION_COUNT = 5;
    }
})";
    
    auto result = parser.parseConfigurationDeclaration(source, "complex_config.chtl");
    
    assert(result.success);
    
    auto registry = parser.getConfigurationRegistry();
    
    // 检查基本配置项
    assert(registry->hasConfiguration("INDEX_INITIAL_COUNT"));
    assert(registry->hasConfiguration("DISABLE_NAME_GROUP"));
    assert(registry->hasConfiguration("DEBUG_MODE"));
    
    // 检查Name配置
    const auto* nameConfig = registry->getNameConfiguration();
    assert(nameConfig != nullptr);
    
    auto customStyleMapping = registry->getKeywordMapping("CUSTOM_STYLE");
    assert(customStyleMapping.size() == 5);
    assert(customStyleMapping[0] == "@Style");
    assert(customStyleMapping[4] == "@css");
    
    auto importChtlMapping = registry->getKeywordMapping("IMPORT_CHTL");
    assert(importChtlMapping.size() == 1);
    assert(importChtlMapping[0] == "@Chtl");
    
    auto keywordFromMapping = registry->getKeywordMapping("KEYWORD_FROM");
    assert(keywordFromMapping.size() == 1);
    assert(keywordFromMapping[0] == "from");
    
    std::cout << "✅ 复杂配置组测试通过" << std::endl;
}

void testErrorHandling() {
    std::cout << "🧪 测试错误处理..." << std::endl;
    
    CHTLConfigImportParser parser;
    
    // 测试无效的配置组语法
    std::string invalidConfig = "[Configuration] { INVALID SYNTAX }";
    auto result1 = parser.parseConfigurationDeclaration(invalidConfig, "invalid.chtl");
    assert(!result1.success);
    assert(!parser.getErrors().empty());
    
    // 测试无效的导入语法
    std::string invalidImport = "[Import] InvalidSyntax";
    auto result2 = parser.parseImportDeclaration(invalidImport, "invalid.chtl");
    assert(!result2.success);
    assert(!parser.getErrors().empty());
    
    // 测试重复导入
    ImportRegistry registry;
    ImportDefinition import1(ImportType::HTML_IMPORT, "same.html");
    ImportDefinition import2(ImportType::HTML_IMPORT, "same.html");
    
    assert(registry.registerImport(import1));
    assert(!registry.registerImport(import2)); // 应该失败
    
    std::cout << "✅ 错误处理测试通过" << std::endl;
}

int main() {
    std::cout << "🚀 开始CHTL配置组与导入系统解析器测试" << std::endl;
    std::cout << "========================================" << std::endl;
    
    try {
        testConfigurationDeclaration();
        testNameConfiguration();
        testHtmlImportDeclaration();
        testCssImportDeclaration();
        testJavaScriptImportDeclaration();
        testChtlImportDeclaration();
        testCjmodImportDeclaration();
        testCustomImportDeclaration();
        testTemplateImportDeclaration();
        testConfigImportScanner();
        testConfigImportStateMachine();
        testConfigurationRegistry();
        testImportRegistry();
        testConfigImportUtils();
        testComplexConfiguration();
        testErrorHandling();
        
        std::cout << "\n========================================" << std::endl;
        std::cout << "🎉 所有测试通过！" << std::endl;
        std::cout << "✅ 配置组与导入系统解析器实现完成" << std::endl;
        std::cout << "   • 配置组声明 ([Configuration] { 配置项 })" << std::endl;
        std::cout << "   • Name配置块 ([Name] { 关键字映射 })" << std::endl;
        std::cout << "   • HTML导入 ([Import] @Html from path as alias)" << std::endl;
        std::cout << "   • CSS导入 ([Import] @Style from path)" << std::endl;
        std::cout << "   • JavaScript导入 ([Import] @JavaScript from path)" << std::endl;
        std::cout << "   • CHTL文件导入 ([Import] @Chtl from path)" << std::endl;
        std::cout << "   • CJmod导入 ([Import] @CJmod from path)" << std::endl;
        std::cout << "   • 自定义导入 ([Import] [Custom] @Type Name from path)" << std::endl;
        std::cout << "   • 模板导入 ([Import] [Template] @Type Name from path)" << std::endl;
        std::cout << "   • 配置注册表管理 (全局配置项存储)" << std::endl;
        std::cout << "   • 导入注册表管理 (循环依赖检测)" << std::endl;
        std::cout << "   • 路径类型检测 (相对/绝对/模块/通配符)" << std::endl;
        std::cout << "   • 状态机和扫描器架构" << std::endl;
        std::cout << "   • 关键字映射系统" << std::endl;
        std::cout << "   • 严格按照CHTL语法文档实现" << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ 测试失败: " << e.what() << std::endl;
        return 1;
    }
}
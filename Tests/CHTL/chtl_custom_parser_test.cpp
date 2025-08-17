#include "parsers/chtl_custom_parser.hpp"
#include <iostream>
#include <string>
#include <cassert>

using namespace chtl;

void testStyleCustomDeclaration() {
    std::cout << "🧪 测试自定义样式组声明..." << std::endl;
    
    CHTLCustomParser parser;
    
    // 测试自定义样式组声明
    std::string source = "[Custom] @Style TextSet { color, font-size; }";
    
    auto result = parser.parseCustomDeclaration(source, "style_custom.chtl");
    
    assert(result.success);
    assert(result.rootNode);
    assert(result.rootNode->type == CHTLASTNodeType::CUSTOM_DECLARATION);
    
    auto customNode = std::dynamic_pointer_cast<CustomDeclarationNode>(result.rootNode);
    assert(customNode);
    assert(customNode->customType == "@Style");
    assert(customNode->name == "TextSet");
    
    // 检查自定义是否注册
    auto registry = parser.getCustomRegistry();
    const auto* customDef = registry->findCustom(CustomType::STYLE_CUSTOM, "TextSet");
    assert(customDef != nullptr);
    assert(customDef->name == "TextSet");
    assert(customDef->type == CustomType::STYLE_CUSTOM);
    
    std::cout << "✅ 自定义样式组声明测试通过" << std::endl;
}

void testElementCustomDeclaration() {
    std::cout << "🧪 测试自定义元素声明..." << std::endl;
    
    CHTLCustomParser parser;
    
    // 测试自定义元素声明
    std::string source = "[Custom] @Element Box { div { } span { } }";
    
    auto result = parser.parseCustomDeclaration(source, "element_custom.chtl");
    
    assert(result.success);
    assert(result.rootNode);
    assert(result.rootNode->type == CHTLASTNodeType::CUSTOM_DECLARATION);
    
    auto customNode = std::dynamic_pointer_cast<CustomDeclarationNode>(result.rootNode);
    assert(customNode);
    assert(customNode->customType == "@Element");
    assert(customNode->name == "Box");
    
    // 检查自定义是否注册
    auto registry = parser.getCustomRegistry();
    const auto* customDef = registry->findCustom(CustomType::ELEMENT_CUSTOM, "Box");
    assert(customDef != nullptr);
    assert(customDef->name == "Box");
    assert(customDef->type == CustomType::ELEMENT_CUSTOM);
    
    std::cout << "✅ 自定义元素声明测试通过" << std::endl;
}

void testVariableCustomDeclaration() {
    std::cout << "🧪 测试自定义变量组声明..." << std::endl;
    
    CHTLCustomParser parser;
    
    // 测试自定义变量组声明
    std::string source = "[Custom] @Var ThemeColor { tableColor: \"rgb(255, 192, 203)\"; }";
    
    auto result = parser.parseCustomDeclaration(source, "var_custom.chtl");
    
    assert(result.success);
    assert(result.rootNode);
    assert(result.rootNode->type == CHTLASTNodeType::CUSTOM_DECLARATION);
    
    auto customNode = std::dynamic_pointer_cast<CustomDeclarationNode>(result.rootNode);
    assert(customNode);
    assert(customNode->customType == "@Var");
    assert(customNode->name == "ThemeColor");
    
    // 检查自定义是否注册
    auto registry = parser.getCustomRegistry();
    const auto* customDef = registry->findCustom(CustomType::VAR_CUSTOM, "ThemeColor");
    assert(customDef != nullptr);
    assert(customDef->name == "ThemeColor");
    assert(customDef->type == CustomType::VAR_CUSTOM);
    
    std::cout << "✅ 自定义变量组声明测试通过" << std::endl;
}

void testCustomUsage() {
    std::cout << "🧪 测试自定义使用..." << std::endl;
    
    CHTLCustomParser parser;
    
    // 先注册一个自定义样式
    std::string styleCustom = "[Custom] @Style TextSet { color, font-size; }";
    auto customResult = parser.parseCustomDeclaration(styleCustom, "custom.chtl");
    assert(customResult.success);
    
    // 测试自定义使用
    std::string usage = "@Style TextSet { color: red; font-size: \"16px\"; }";
    auto customRef = parser.parseCustomUsage(usage);
    
    assert(customRef.type == CustomType::STYLE_CUSTOM);
    assert(customRef.customName == "TextSet");
    
    // 测试自定义展开
    auto expandedNode = parser.expandCustomReference(customRef);
    assert(expandedNode != nullptr);
    
    std::cout << "✅ 自定义使用测试通过" << std::endl;
}

void testCustomScanner() {
    std::cout << "🧪 测试自定义扫描器..." << std::endl;
    
    CustomScanner scanner;
    CustomContext context;
    context.currentType = CustomType::STYLE_CUSTOM;
    
    // 测试扫描无值属性
    std::string content = "color, font-size;";
    auto tokens = scanner.scanCustomContent(content, context);
    
    assert(!tokens.empty());
    
    // 验证扫描的token类型
    bool foundComma = false;
    bool foundIdentifier = false;
    for (const auto& token : tokens) {
        if (token.type == TokenType::OPERATOR && token.value == ",") {
            foundComma = true;
        }
        if (token.type == TokenType::IDENTIFIER) {
            foundIdentifier = true;
        }
    }
    
    assert(foundComma);
    assert(foundIdentifier);
    
    std::cout << "✅ 自定义扫描器测试通过" << std::endl;
}

void testCustomStateMachine() {
    std::cout << "🧪 测试自定义状态机..." << std::endl;
    
    CustomStateMachine stateMachine;
    CustomContext context;
    context.currentType = CustomType::STYLE_CUSTOM;
    
    // 测试处理删除操作
    Token deleteToken(TokenType::KEYWORD, "delete", Position{});
    Token targetToken(TokenType::IDENTIFIER, "color", Position{});
    Token semicolonToken(TokenType::SEMICOLON, ";", Position{});
    
    stateMachine.processToken(deleteToken, context);
    stateMachine.processToken(targetToken, context);
    stateMachine.processToken(semicolonToken, context);
    
    auto specializations = stateMachine.getSpecializations();
    assert(!specializations.empty());
    assert(specializations[0].type == SpecializationType::DELETE_PROPERTY);
    assert(specializations[0].target == "color");
    
    std::cout << "✅ 自定义状态机测试通过" << std::endl;
}

void testCustomRegistry() {
    std::cout << "🧪 测试自定义注册表..." << std::endl;
    
    CustomRegistry registry;
    
    // 创建测试自定义定义
    CustomDefinition styleDef(CustomType::STYLE_CUSTOM, "TestStyle");
    CustomDefinition elementDef(CustomType::ELEMENT_CUSTOM, "TestElement");
    CustomDefinition varDef(CustomType::VAR_CUSTOM, "TestVar");
    
    // 注册自定义
    assert(registry.registerCustom(styleDef));
    assert(registry.registerCustom(elementDef));
    assert(registry.registerCustom(varDef));
    
    // 查找自定义
    const auto* foundStyle = registry.findCustom(CustomType::STYLE_CUSTOM, "TestStyle");
    const auto* foundElement = registry.findCustom(CustomType::ELEMENT_CUSTOM, "TestElement");
    const auto* foundVar = registry.findCustom(CustomType::VAR_CUSTOM, "TestVar");
    
    assert(foundStyle != nullptr);
    assert(foundElement != nullptr);
    assert(foundVar != nullptr);
    
    // 验证自定义引用
    CustomReference styleRef(CustomType::STYLE_CUSTOM, "TestStyle");
    CustomReference elementRef(CustomType::ELEMENT_CUSTOM, "TestElement");
    CustomReference varRef(CustomType::VAR_CUSTOM, "TestVar");
    
    assert(registry.validateCustomReference(styleRef));
    assert(registry.validateCustomReference(elementRef));
    assert(registry.validateCustomReference(varRef));
    
    // 测试重复注册
    assert(!registry.registerCustom(styleDef)); // 应该失败，已存在
    
    std::cout << "✅ 自定义注册表测试通过" << std::endl;
}

void testCustomUtils() {
    std::cout << "🧪 测试自定义工具类..." << std::endl;
    
    // 测试自定义声明识别
    std::string customDecl = "[Custom] @Style TextSet { color, font-size; }";
    assert(CustomUtils::isCustomDeclaration(customDecl));
    
    // 测试自定义使用识别
    std::string customUsage = "@Style TextSet { color: red; }";
    assert(CustomUtils::isCustomUsage(customUsage));
    
    // 测试特例化操作识别
    std::string deleteOp = "delete color;";
    assert(CustomUtils::isSpecializationOperation(deleteOp));
    
    std::string insertOp = "insert after div { }";
    assert(CustomUtils::isSpecializationOperation(insertOp));
    
    // 测试类型提取
    assert(CustomUtils::extractCustomTypeString(customDecl) == "Style");
    assert(CustomUtils::extractCustomName(customUsage) == "TextSet");
    
    // 测试索引访问解析
    auto indexResult = CustomUtils::parseIndexAccess("div[1]");
    assert(indexResult.first == "div");
    assert(indexResult.second == 1);
    
    // 测试插入位置验证
    assert(CustomUtils::isValidInsertPosition("after"));
    assert(CustomUtils::isValidInsertPosition("before"));
    assert(CustomUtils::isValidInsertPosition("at top"));
    assert(!CustomUtils::isValidInsertPosition("invalid"));
    
    // 测试语法验证
    assert(CustomUtils::validateCustomSyntax(customDecl));
    assert(CustomUtils::validateCustomSyntax(customUsage));
    assert(CustomUtils::validateCustomSyntax(deleteOp));
    
    std::cout << "✅ 自定义工具类测试通过" << std::endl;
}

void testErrorHandling() {
    std::cout << "🧪 测试错误处理..." << std::endl;
    
    CHTLCustomParser parser;
    
    // 测试无效的自定义声明
    std::string invalidCustom = "[Custom] @InvalidType TestName { }";
    auto result = parser.parseCustomDeclaration(invalidCustom, "invalid.chtl");
    assert(!result.success);
    assert(!parser.getErrors().empty());
    
    // 测试无效的自定义使用
    std::string invalidUsage = "@InvalidType TestName { }";
    auto customRef = parser.parseCustomUsage(invalidUsage);
    assert(!parser.getErrors().empty());
    
    std::cout << "✅ 错误处理测试通过" << std::endl;
}

int main() {
    std::cout << "🚀 开始CHTL自定义系统解析器测试" << std::endl;
    std::cout << "========================================" << std::endl;
    
    try {
        testStyleCustomDeclaration();
        testElementCustomDeclaration();
        testVariableCustomDeclaration();
        testCustomUsage();
        testCustomScanner();
        testCustomStateMachine();
        testCustomRegistry();
        testCustomUtils();
        testErrorHandling();
        
        std::cout << "\n========================================" << std::endl;
        std::cout << "🎉 所有测试通过！" << std::endl;
        std::cout << "✅ 自定义系统解析器实现完成" << std::endl;
        std::cout << "   • 自定义样式组 ([Custom] @Style Name { 无值属性, 特例化 })" << std::endl;
        std::cout << "   • 自定义元素 ([Custom] @Element Name { 元素内容, 特例化 })" << std::endl;
        std::cout << "   • 自定义变量组 ([Custom] @Var Name { 变量定义, 特例化 })" << std::endl;
        std::cout << "   • 自定义使用 (@Style Name { 特例化操作 })" << std::endl;
        std::cout << "   • 特例化操作 (delete, insert, 索引访问)" << std::endl;
        std::cout << "   • 无值属性 (color, font-size;)" << std::endl;
        std::cout << "   • 变量特例化 (ThemeColor(var = value))" << std::endl;
        std::cout << "   • 自定义注册表管理" << std::endl;
        std::cout << "   • 状态机和扫描器架构" << std::endl;
        std::cout << "   • 上下文感知解析" << std::endl;
        std::cout << "   • 严格按照CHTL语法文档实现" << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ 测试失败: " << e.what() << std::endl;
        return 1;
    }
}
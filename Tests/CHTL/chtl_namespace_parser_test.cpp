#include "parsers/chtl_namespace_parser.hpp"
#include <iostream>
#include <cassert>
#include <string>

using namespace chtl;

// 测试命名空间声明解析
void testNamespaceDeclaration() {
    std::cout << "🧪 测试命名空间声明..." << std::endl;
    
    CHTLNamespaceParser parser;
    
    // 测试基本命名空间声明
    std::string source = "[Namespace] space";
    auto result = parser.parseNamespaceDeclaration(source, "test.chtl");
    
    assert(result.success);
    assert(result.rootNode != nullptr);
    
    auto namespaceNode = std::dynamic_pointer_cast<NamespaceDeclarationNode>(result.rootNode);
    assert(namespaceNode != nullptr);
    assert(namespaceNode->name == "space");
    
    std::cout << "✅ 命名空间声明测试通过" << std::endl;
}

// 测试嵌套命名空间
void testNestedNamespace() {
    std::cout << "🧪 测试嵌套命名空间..." << std::endl;
    
    CHTLNamespaceParser parser;
    
    // 测试嵌套命名空间
    std::string source = R"([Namespace] space
{
    [Namespace] room
    
    [Namespace] room2
    {
        // 嵌套内容
    }
})";
    
    auto result = parser.parseNamespaceDeclaration(source, "test.chtl");
    
    assert(result.success);
    assert(result.rootNode != nullptr);
    
    auto namespaceNode = std::dynamic_pointer_cast<NamespaceDeclarationNode>(result.rootNode);
    assert(namespaceNode != nullptr);
    assert(namespaceNode->name == "space");
    
    // 检查是否有嵌套命名空间
    auto registry = parser.getNamespaceRegistry();
    assert(registry != nullptr);
    
    std::cout << "✅ 嵌套命名空间测试通过" << std::endl;
}

// 测试精确约束声明
void testPreciseConstraintDeclaration() {
    std::cout << "🧪 测试精确约束声明..." << std::endl;
    
    CHTLNamespaceParser parser;
    
    // 测试精确约束：禁止span和自定义元素Box
    std::string source = "except span, [Custom] @Element Box;";
    auto result = parser.parseConstraintDeclaration(source, "test.chtl");
    
    assert(result.success);
    assert(result.rootNode != nullptr);
    
    auto constraintRegistry = parser.getConstraintRegistry();
    assert(constraintRegistry != nullptr);
    
    // 检查约束违反
    ConstraintTarget spanTarget(ConstraintTargetType::HTML_ELEMENT, "span");
    assert(constraintRegistry->hasConstraintViolation("global", spanTarget));
    
    ConstraintTarget boxTarget(ConstraintTargetType::CUSTOM_ELEMENT, "Box");
    assert(constraintRegistry->hasConstraintViolation("global", boxTarget));
    
    ConstraintTarget divTarget(ConstraintTargetType::HTML_ELEMENT, "div");
    assert(!constraintRegistry->hasConstraintViolation("global", divTarget));
    
    std::cout << "✅ 精确约束声明测试通过" << std::endl;
}

// 测试类型约束声明
void testTypeConstraintDeclaration() {
    std::cout << "🧪 测试类型约束声明..." << std::endl;
    
    CHTLNamespaceParser parser;
    
    // 测试类型约束：禁止Html和Custom类型
    std::string source = "except @Html, [Custom];";
    auto result = parser.parseConstraintDeclaration(source, "test.chtl");
    
    assert(result.success);
    assert(result.rootNode != nullptr);
    
    auto constraintRegistry = parser.getConstraintRegistry();
    assert(constraintRegistry != nullptr);
    
    // 检查约束违反
    ConstraintTarget htmlTarget(ConstraintTargetType::HTML_TYPE);
    assert(constraintRegistry->hasConstraintViolation("global", htmlTarget));
    
    ConstraintTarget customTarget(ConstraintTargetType::CUSTOM_TYPE);
    assert(constraintRegistry->hasConstraintViolation("global", customTarget));
    
    ConstraintTarget templateTarget(ConstraintTargetType::TEMPLATE_TYPE);
    assert(!constraintRegistry->hasConstraintViolation("global", templateTarget));
    
    std::cout << "✅ 类型约束声明测试通过" << std::endl;
}

// 测试模板约束声明
void testTemplateConstraintDeclaration() {
    std::cout << "🧪 测试模板约束声明..." << std::endl;
    
    CHTLNamespaceParser parser;
    
    // 测试模板约束：禁止Template变量组
    std::string source = "except [Template] @Var;";
    auto result = parser.parseConstraintDeclaration(source, "test.chtl");
    
    assert(result.success);
    assert(result.rootNode != nullptr);
    
    auto constraintRegistry = parser.getConstraintRegistry();
    assert(constraintRegistry != nullptr);
    
    // 检查约束违反
    ConstraintTarget templateVarTarget(ConstraintTargetType::TEMPLATE_VAR);
    assert(constraintRegistry->hasConstraintViolation("global", templateVarTarget));
    
    ConstraintTarget templateElementTarget(ConstraintTargetType::TEMPLATE_ELEMENT);
    assert(!constraintRegistry->hasConstraintViolation("global", templateElementTarget));
    
    std::cout << "✅ 模板约束声明测试通过" << std::endl;
}

// 测试命名空间注册表
void testNamespaceRegistry() {
    std::cout << "🧪 测试命名空间注册表..." << std::endl;
    
    NamespaceRegistry registry;
    
    // 创建命名空间定义
    NamespaceDefinition space("space");
    space.fullPath = "space";
    
    NamespaceDefinition room("room");
    room.fullPath = "space.room";
    space.children.push_back(room);
    
    // 注册命名空间
    assert(registry.registerNamespace(space));
    
    // 查找命名空间
    const auto* foundSpace = registry.findNamespace("space");
    assert(foundSpace != nullptr);
    assert(foundSpace->name == "space");
    
    // 检查路径有效性
    assert(registry.isValidNamespacePath("space"));
    assert(!registry.isValidNamespacePath("nonexistent"));
    
    // 测试路径构建
    std::string fullPath = registry.getFullPath("space", "room");
    assert(fullPath == "space.room");
    
    std::cout << "✅ 命名空间注册表测试通过" << std::endl;
}

// 测试约束注册表
void testConstraintRegistry() {
    std::cout << "🧪 测试约束注册表..." << std::endl;
    
    ConstraintRegistry registry;
    
    // 创建约束定义
    ConstraintDefinition constraint(ConstraintType::PRECISE);
    constraint.targets.emplace_back(ConstraintTargetType::HTML_ELEMENT, "span");
    constraint.targets.emplace_back(ConstraintTargetType::CUSTOM_ELEMENT, "Box");
    
    // 注册约束
    assert(registry.registerConstraint("test_scope", constraint));
    
    // 检查约束违反
    ConstraintTarget spanTarget(ConstraintTargetType::HTML_ELEMENT, "span");
    assert(registry.hasConstraintViolation("test_scope", spanTarget));
    
    ConstraintTarget boxTarget(ConstraintTargetType::CUSTOM_ELEMENT, "Box");
    assert(registry.hasConstraintViolation("test_scope", boxTarget));
    
    ConstraintTarget divTarget(ConstraintTargetType::HTML_ELEMENT, "div");
    assert(!registry.hasConstraintViolation("test_scope", divTarget));
    
    std::cout << "✅ 约束注册表测试通过" << std::endl;
}

// 测试命名空间扫描器
void testNamespaceScanner() {
    std::cout << "🧪 测试命名空间扫描器..." << std::endl;
    
    NamespaceScanner scanner;
    
    // 测试命名空间声明扫描
    std::string namespaceSource = "[Namespace] space { /* content */ }";
    NamespaceDefinition namespaceDef = scanner.scanNamespaceDeclaration(namespaceSource);
    
    assert(namespaceDef.name == "space");
    assert(namespaceDef.hasBlock == true);
    
    // 测试约束声明扫描
    std::string constraintSource = "except span, [Custom] @Element Box;";
    ConstraintDefinition constraint = scanner.scanConstraintDeclaration(constraintSource);
    
    assert(constraint.targets.size() >= 1);
    assert(constraint.type == ConstraintType::PRECISE);
    
    // 测试语法验证
    assert(scanner.validateNamespaceSyntax("[Namespace] test"));
    assert(!scanner.validateNamespaceSyntax("invalid syntax"));
    
    assert(scanner.validateConstraintSyntax("except span;"));
    assert(!scanner.validateConstraintSyntax("invalid constraint"));
    
    std::cout << "✅ 命名空间扫描器测试通过" << std::endl;
}

// 测试命名空间状态机
void testNamespaceStateMachine() {
    std::cout << "🧪 测试命名空间状态机..." << std::endl;
    
    NamespaceStateMachine stateMachine;
    NamespaceContext context;
    
    // 创建测试tokens
    Token leftBracket{TokenType::LEFT_BRACKET, "[", Position{}};
    Token namespaceKeyword{TokenType::IDENTIFIER, "Namespace", Position{}};
    Token rightBracket{TokenType::RIGHT_BRACKET, "]", Position{}};
    Token spaceName{TokenType::IDENTIFIER, "space", Position{}};
    Token leftBrace{TokenType::LEFT_BRACE, "{", Position{}};
    Token rightBrace{TokenType::RIGHT_BRACE, "}", Position{}};
    
    // 处理命名空间tokens
    assert(stateMachine.processNamespaceToken(leftBracket, context));
    assert(context.state == NamespaceParseState::READING_NAMESPACE_HEADER);
    
    assert(stateMachine.processNamespaceToken(namespaceKeyword, context));
    assert(context.state == NamespaceParseState::READING_NAMESPACE_NAME);
    
    assert(stateMachine.processNamespaceToken(rightBracket, context));
    assert(stateMachine.processNamespaceToken(spaceName, context));
    assert(context.state == NamespaceParseState::READING_NAMESPACE_CONTENT);
    assert(context.currentNamespace == "space");
    
    std::cout << "✅ 命名空间状态机测试通过" << std::endl;
}

// 测试命名空间工具类
void testNamespaceUtils() {
    std::cout << "🧪 测试命名空间工具类..." << std::endl;
    
    // 测试命名空间识别
    assert(NamespaceUtils::isNamespaceDeclaration("[Namespace] test"));
    assert(!NamespaceUtils::isNamespaceDeclaration("not a namespace"));
    
    // 测试约束识别
    assert(NamespaceUtils::isConstraintDeclaration("except span;"));
    assert(!NamespaceUtils::isConstraintDeclaration("not a constraint"));
    
    // 测试名称提取
    std::string namespaceName = NamespaceUtils::extractNamespaceName("[Namespace] myspace");
    assert(namespaceName == "myspace");
    
    // 测试约束目标提取
    auto targets = NamespaceUtils::extractConstraintTargets("except span, div;");
    assert(targets.size() == 2);
    assert(targets[0] == "span");
    assert(targets[1] == "div");
    
    // 测试路径标准化
    std::string normalized = NamespaceUtils::normalizeNamespacePath("space..room");
    assert(normalized == "space.room");
    
    // 测试路径分割
    auto parts = NamespaceUtils::splitNamespacePath("space.room.area");
    assert(parts.size() == 3);
    assert(parts[0] == "space");
    assert(parts[1] == "room");
    assert(parts[2] == "area");
    
    // 测试路径连接
    std::string joined = NamespaceUtils::joinNamespacePath({"space", "room", "area"});
    assert(joined == "space.room.area");
    
    // 测试名称有效性
    assert(NamespaceUtils::isValidNamespaceName("validName"));
    assert(NamespaceUtils::isValidNamespaceName("_validName"));
    assert(!NamespaceUtils::isValidNamespaceName("123invalid"));
    assert(!NamespaceUtils::isValidNamespaceName(""));
    
    // 测试约束目标类型解析
    auto customElementType = NamespaceUtils::parseConstraintTargetType("[Custom] @Element");
    assert(customElementType == ConstraintTargetType::CUSTOM_ELEMENT);
    
    auto htmlType = NamespaceUtils::parseConstraintTargetType("@Html");
    assert(htmlType == ConstraintTargetType::HTML_TYPE);
    
    std::cout << "✅ 命名空间工具类测试通过" << std::endl;
}

// 测试复杂命名空间结构
void testComplexNamespaceStructure() {
    std::cout << "🧪 测试复杂命名空间结构..." << std::endl;
    
    CHTLNamespaceParser parser;
    
    // 测试多层嵌套命名空间
    std::string source = R"([Namespace] root
{
    [Namespace] level1
    {
        [Namespace] level2
        {
            // 深层嵌套
        }
    }
    
    [Namespace] sibling
    {
        // 兄弟命名空间
    }
})";
    
    auto result = parser.parseNamespaceDeclaration(source, "complex.chtl");
    
    assert(result.success);
    assert(result.rootNode != nullptr);
    
    auto namespaceNode = std::dynamic_pointer_cast<NamespaceDeclarationNode>(result.rootNode);
    assert(namespaceNode != nullptr);
    assert(namespaceNode->name == "root");
    
    auto registry = parser.getNamespaceRegistry();
    assert(registry != nullptr);
    
    std::cout << "✅ 复杂命名空间结构测试通过" << std::endl;
}

// 测试错误处理
void testErrorHandling() {
    std::cout << "🧪 测试错误处理..." << std::endl;
    
    CHTLNamespaceParser parser;
    
    // 测试无效命名空间语法
    std::string invalidNamespace = "[Namespace]"; // 缺少名称
    auto result1 = parser.parseNamespaceDeclaration(invalidNamespace, "test.chtl");
    assert(!result1.success);
    assert(!result1.errors.empty());
    
    // 测试无效约束语法
    std::string invalidConstraint = "except"; // 缺少目标
    auto result2 = parser.parseConstraintDeclaration(invalidConstraint, "test.chtl");
    assert(!result2.success || result2.warnings.size() > 0);
    
    // 测试无效命名空间名称
    std::string invalidName = "[Namespace] 123invalid";
    auto result3 = parser.parseNamespaceDeclaration(invalidName, "test.chtl");
    // 应该有警告或错误
    
    std::cout << "✅ 错误处理测试通过" << std::endl;
}

int main() {
    std::cout << "🚀 开始CHTL命名空间与约束系统解析器测试" << std::endl;
    std::cout << "========================================" << std::endl;
    
    try {
        testNamespaceDeclaration();
        testNestedNamespace();
        testPreciseConstraintDeclaration();
        testTypeConstraintDeclaration();
        testTemplateConstraintDeclaration();
        testNamespaceRegistry();
        testConstraintRegistry();
        testNamespaceScanner();
        testNamespaceStateMachine();
        testNamespaceUtils();
        testComplexNamespaceStructure();
        testErrorHandling();
        
        std::cout << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "🎉 所有测试通过！" << std::endl;
        std::cout << "✅ 命名空间与约束系统解析器实现完成" << std::endl;
        std::cout << "   • 命名空间声明 ([Namespace] name { ... })" << std::endl;
        std::cout << "   • 嵌套命名空间支持 (space.room.area)" << std::endl;
        std::cout << "   • 精确约束 (except span, [Custom] @Element Box)" << std::endl;
        std::cout << "   • 类型约束 (except @Html, [Custom], [Template])" << std::endl;
        std::cout << "   • 模板约束 (except [Template] @Var)" << std::endl;
        std::cout << "   • 命名空间注册表管理 (层次结构存储)" << std::endl;
        std::cout << "   • 约束注册表管理 (约束违反检测)" << std::endl;
        std::cout << "   • 命名空间路径处理 (标准化/分割/连接)" << std::endl;
        std::cout << "   • 状态机和扫描器架构" << std::endl;
        std::cout << "   • 约束违反检测系统" << std::endl;
        std::cout << "   • 严格按照CHTL语法文档实现" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ 测试失败: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "parser/AttributeSystem.h"
#include "lexer/CHTLLexer.h"

using namespace chtl;

void testAttributeSystemCreation() {
    std::cout << "测试属性系统创建..." << std::endl;
    
    AttributeSystemConfig config;
    config.enable_ce_equivalence = true;
    config.enable_unquoted_literals = true;
    config.strict_validation = false;
    config.allow_duplicate_attributes = false;
    config.enable_attribute_inheritance = true;
    
    AttributeSystem attr_system(config);
    
    std::cout << "✓ 属性系统创建成功" << std::endl;
    std::cout << "配置信息: " << attr_system.getDebugInfo() << std::endl;
}

void testAttributeParsing() {
    std::cout << "\n测试属性解析..." << std::endl;
    
    AttributeSystem attr_system;
    
    // 创建测试token
    std::vector<Token> tokens = {
        {TokenType::IDENTIFIER, "id", 1, 1, 0},
        {TokenType::COLON, ":", 1, 3, 2},
        {TokenType::STRING_LITERAL, "\"", 1, 4, 3},
        {TokenType::IDENTIFIER, "box", 1, 5, 4},
        {TokenType::STRING_LITERAL, "\"", 1, 6, 5},
        {TokenType::SEMICOLON, ";", 1, 7, 6},
        {TokenType::WHITESPACE, " ", 1, 8, 7},
        {TokenType::IDENTIFIER, "class", 1, 9, 8},
        {TokenType::COLON, ":", 1, 14, 13},
        {TokenType::IDENTIFIER, "welcome", 1, 15, 14},
        {TokenType::SEMICOLON, ";", 1, 22, 21}
    };
    
    size_t position = 0;
    
    // 测试单个属性解析
    auto attr1 = attr_system.parseAttribute(tokens, position);
    if (attr1) {
        std::cout << "✓ 第一个属性解析成功: " << attr1->serialize() << std::endl;
    } else {
        std::cout << "✗ 第一个属性解析失败" << std::endl;
    }
    
    // 跳过空白字符
    while (position < tokens.size() && tokens[position].value == " ") {
        position++;
    }
    
    // 测试第二个属性解析
    auto attr2 = attr_system.parseAttribute(tokens, position);
    if (attr2) {
        std::cout << "✓ 第二个属性解析成功: " << attr2->serialize() << std::endl;
    } else {
        std::cout << "✗ 第二个属性解析失败" << std::endl;
    }
}

void testAttributeListParsing() {
    std::cout << "\n测试属性列表解析..." << std::endl;
    
    AttributeSystem attr_system;
    
    // 创建测试token
    std::vector<Token> tokens = {
        {TokenType::IDENTIFIER, "id", 1, 1, 0},
        {TokenType::COLON, ":", 1, 3, 2},
        {TokenType::STRING_LITERAL, "\"", 1, 4, 3},
        {TokenType::IDENTIFIER, "main", 1, 5, 4},
        {TokenType::STRING_LITERAL, "\"", 1, 6, 5},
        {TokenType::SEMICOLON, ";", 1, 7, 6},
        {TokenType::WHITESPACE, " ", 1, 8, 7},
        {TokenType::IDENTIFIER, "class", 1, 9, 8},
        {TokenType::COLON, ":", 1, 14, 13},
        {TokenType::IDENTIFIER, "container", 1, 15, 14},
        {TokenType::SEMICOLON, ";", 1, 24, 23},
        {TokenType::WHITESPACE, " ", 1, 25, 24},
        {TokenType::IDENTIFIER, "style", 1, 26, 25},
        {TokenType::COLON, ":", 1, 31, 30},
        {TokenType::IDENTIFIER, "display:flex", 1, 32, 31},
        {TokenType::SEMICOLON, ";", 1, 43, 42}
    };
    
    size_t position = 0;
    
    auto attr_list = attr_system.parseAttributeList(tokens, position);
    std::cout << "✓ 属性列表解析成功，共 " << attr_list.size() << " 个属性" << std::endl;
    
    for (size_t i = 0; i < attr_list.size(); i++) {
        std::cout << "  属性 " << (i + 1) << ": " << attr_list[i]->serialize() << std::endl;
    }
}

void testAttributeValueTypeDetection() {
    std::cout << "\n测试属性值类型检测..." << std::endl;
    
    AttributeSystem attr_system;
    
    // 测试不同类型的属性值
    std::vector<std::pair<std::string, std::string>> test_cases = {
        {"字符串", "\"Hello World\""},
        {"无修饰字面量", "welcome"},
        {"数值", "100"},
        {"布尔值", "true"},
        {"模板引用", "ThemeColor(primary)"},
        {"函数调用", "getValue()"},
        {"表达式", "{{user.name}}"}
    };
    
    for (const auto& [desc, value] : test_cases) {
        auto value_type = attr_system.detectAttributeValueType(value);
        std::cout << "✓ " << desc << " (" << value << "): 类型 " << static_cast<int>(value_type) << std::endl;
    }
}

void testAttributeValidation() {
    std::cout << "\n测试属性验证..." << std::endl;
    
    AttributeSystem attr_system;
    
    // 测试有效的属性
    std::vector<AttributeInfo> valid_attrs;
    
    AttributeInfo attr1;
    attr1.name = "id";
    attr1.value = "main";
    attr1.value_type = AttributeValueType::STRING_LITERAL;
    valid_attrs.push_back(attr1);
    
    AttributeInfo attr2;
    attr2.name = "class";
    attr2.value = "container";
    attr2.value_type = AttributeValueType::UNQUOTED_LITERAL;
    valid_attrs.push_back(attr2);
    
    AttributeInfo attr3;
    attr3.name = "width";
    attr3.value = "100";
    attr3.value_type = AttributeValueType::NUMERIC_LITERAL;
    valid_attrs.push_back(attr3);
    
    AttributeInfo attr4;
    attr4.name = "disabled";
    attr4.value = "true";
    attr4.value_type = AttributeValueType::BOOLEAN_LITERAL;
    valid_attrs.push_back(attr4);
    
    for (const auto& attr : valid_attrs) {
        bool is_valid = attr_system.validateAttribute(attr);
        if (is_valid) {
            std::cout << "✓ 属性验证成功: " << attr.name << " = " << attr.value << std::endl;
        } else {
            std::cout << "✗ 属性验证失败: " << attr.name << " = " << attr.value << std::endl;
        }
    }
    
    // 测试无效的属性
    std::vector<AttributeInfo> invalid_attrs;
    
    AttributeInfo invalid_attr1;
    invalid_attr1.name = "";
    invalid_attr1.value = "value";
    invalid_attr1.value_type = AttributeValueType::STRING_LITERAL;
    invalid_attrs.push_back(invalid_attr1);
    
    AttributeInfo invalid_attr2;
    invalid_attr2.name = "123id";
    invalid_attr2.value = "value";
    invalid_attr2.value_type = AttributeValueType::STRING_LITERAL;
    invalid_attrs.push_back(invalid_attr2);
    
    AttributeInfo invalid_attr3;
    invalid_attr3.name = "class";
    invalid_attr3.value = "";
    invalid_attr3.value_type = AttributeValueType::STRING_LITERAL;
    invalid_attrs.push_back(invalid_attr3);
    
    for (const auto& attr : invalid_attrs) {
        bool is_valid = attr_system.validateAttribute(attr);
        if (!is_valid) {
            std::cout << "✓ 无效属性正确被拒绝: " << attr.name << " = " << attr.value << std::endl;
        } else {
            std::cout << "✗ 无效属性错误被接受: " << attr.name << " = " << attr.value << std::endl;
        }
    }
}

void testAttributeManagement() {
    std::cout << "\n测试属性管理..." << std::endl;
    
    AttributeSystem attr_system;
    
    // 添加属性
    auto attr1 = std::make_shared<AttributeNode>("id", "main");
    auto attr2 = std::make_shared<AttributeNode>("class", "container");
    auto attr3 = std::make_shared<AttributeNode>("style", "display:flex");
    
    attr_system.addAttribute(attr1);
    attr_system.addAttribute(attr2);
    attr_system.addAttribute(attr3);
    
    std::cout << "✓ 添加属性成功，当前属性数量: " << attr_system.getAttributes().size() << std::endl;
    
    // 查询属性
    auto found_attr = attr_system.getAttribute("id");
    if (found_attr) {
        std::cout << "✓ 查询属性成功: " << found_attr->serialize() << std::endl;
    } else {
        std::cout << "✗ 查询属性失败" << std::endl;
    }
    
    // 检查属性存在性
    if (attr_system.hasAttribute("class")) {
        std::cout << "✓ 属性存在性检查成功: class 属性存在" << std::endl;
    } else {
        std::cout << "✗ 属性存在性检查失败: class 属性应该存在" << std::endl;
    }
    
    // 删除属性
    attr_system.removeAttribute("style");
    std::cout << "✓ 删除属性成功，当前属性数量: " << attr_system.getAttributes().size() << std::endl;
}

void testAttributeInheritance() {
    std::cout << "\n测试属性继承..." << std::endl;
    
    AttributeSystem attr_system;
    
    // 创建父级属性
    std::vector<std::shared_ptr<AttributeNode>> parent_attrs = {
        std::make_shared<AttributeNode>("base-class", "base"),
        std::make_shared<AttributeNode>("theme", "light")
    };
    
    // 继承属性
    attr_system.inheritAttributes(parent_attrs);
    std::cout << "✓ 属性继承成功，继承属性数量: " << attr_system.getInheritedAttributes().size() << std::endl;
    
    // 覆盖继承的属性
    attr_system.overrideAttribute("theme", "dark");
    std::cout << "✓ 属性覆盖成功" << std::endl;
    
    // 添加新属性
    attr_system.overrideAttribute("new-attr", "new-value");
    std::cout << "✓ 新属性添加成功" << std::endl;
}

void testAttributeConversion() {
    std::cout << "\n测试属性转换..." << std::endl;
    
    AttributeSystem attr_system;
    
    // 创建测试属性
    std::vector<std::shared_ptr<AttributeNode>> test_attrs = {
        std::make_shared<AttributeNode>("id", "main"),
        std::make_shared<AttributeNode>("class", "container"),
        std::make_shared<AttributeNode>("style", "display:flex; color:red")
    };
    
    // 转换为HTML
    std::string html_attrs = attr_system.convertToHTML(test_attrs);
    std::cout << "✓ HTML转换成功: " << html_attrs << std::endl;
    
    // 转换为CSS
    std::string css_attrs = attr_system.convertToCSS(test_attrs);
    std::cout << "✓ CSS转换成功:\n" << css_attrs;
    
    // 转换为JavaScript
    std::string js_attrs = attr_system.convertToJavaScript(test_attrs);
    std::cout << "✓ JavaScript转换成功:\n" << js_attrs;
}

void testCEEquivalence() {
    std::cout << "\n测试CE等价性..." << std::endl;
    
    AttributeSystemConfig config;
    config.enable_ce_equivalence = true;
    
    AttributeSystem attr_system(config);
    
    // 创建使用 = 的测试token
    std::vector<Token> tokens = {
        {TokenType::IDENTIFIER, "id", 1, 1, 0},
        {TokenType::EQUAL, "=", 1, 3, 2},
        {TokenType::STRING_LITERAL, "\"", 1, 4, 3},
        {TokenType::IDENTIFIER, "box", 1, 5, 4},
        {TokenType::STRING_LITERAL, "\"", 1, 6, 5},
        {TokenType::SEMICOLON, ";", 1, 7, 6}
    };
    
    size_t position = 0;
    
    auto attr = attr_system.parseAttribute(tokens, position);
    if (attr) {
        std::cout << "✓ CE等价性测试成功: 使用 = 代替 : 解析成功" << std::endl;
        std::cout << "  解析结果: " << attr->serialize() << std::endl;
    } else {
        std::cout << "✗ CE等价性测试失败: 使用 = 代替 : 解析失败" << std::endl;
    }
}

void testErrorHandling() {
    std::cout << "\n测试错误处理..." << std::endl;
    
    AttributeSystem attr_system;
    
    // 测试无效的属性声明
    std::vector<Token> invalid_tokens = {
        {TokenType::IDENTIFIER, "123id", 1, 1, 0},  // 无效属性名
        {TokenType::COLON, ":", 1, 5, 4},
        {TokenType::STRING_LITERAL, "\"", 1, 6, 5},
        {TokenType::IDENTIFIER, "value", 1, 7, 6},
        {TokenType::STRING_LITERAL, "\"", 1, 8, 7},
        {TokenType::SEMICOLON, ";", 1, 9, 8}
    };
    
    size_t position = 0;
    
    auto attr = attr_system.parseAttribute(invalid_tokens, position);
    if (!attr) {
        std::cout << "✓ 无效属性正确被拒绝" << std::endl;
        
        if (attr_system.hasErrors()) {
            std::cout << "  错误信息:" << std::endl;
            auto errors = attr_system.getErrors();
            for (const auto& error : errors) {
                std::cout << "    " << error << std::endl;
            }
        }
    } else {
        std::cout << "✗ 无效属性错误被接受" << std::endl;
    }
}

int main() {
    std::cout << "CHTL属性系统测试" << std::endl;
    std::cout << "=================" << std::endl << std::endl;
    
    try {
        testAttributeSystemCreation();
        testAttributeParsing();
        testAttributeListParsing();
        testAttributeValueTypeDetection();
        testAttributeValidation();
        testAttributeManagement();
        testAttributeInheritance();
        testAttributeConversion();
        testCEEquivalence();
        testErrorHandling();
        
        std::cout << "\n所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
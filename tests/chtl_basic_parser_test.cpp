#include "parsers/chtl_basic_parser.hpp"
#include <iostream>
#include <string>
#include <cassert>

using namespace chtl;

void testCommentParsing() {
    std::cout << "🧪 测试注释解析..." << std::endl;
    
    CHTLBasicParser parser;
    
    // 测试单行注释
    std::string source1 = "// 这是单行注释\n";
    auto result1 = parser.parse(source1, "test1.chtl");
    assert(result1.success);
    assert(result1.rootNode);
    assert(result1.rootNode->children.size() == 1);
    assert(result1.rootNode->children[0]->type == CHTLASTNodeType::COMMENT_LINE);
    
    // 测试多行注释
    std::string source2 = "/* 这是多行注释 */\n";
    auto result2 = parser.parse(source2, "test2.chtl");
    assert(result2.success);
    assert(result2.rootNode->children.size() == 1);
    assert(result2.rootNode->children[0]->type == CHTLASTNodeType::COMMENT_MULTILINE);
    
    // 测试生成器注释
    std::string source3 = "-- 这是生成器注释\n";
    auto result3 = parser.parse(source3, "test3.chtl");
    assert(result3.success);
    assert(result3.rootNode->children.size() == 1);
    assert(result3.rootNode->children[0]->type == CHTLASTNodeType::COMMENT_GENERATOR);
    
    std::cout << "✅ 注释解析测试通过" << std::endl;
}

void testTextNodeParsing() {
    std::cout << "🧪 测试文本节点解析..." << std::endl;
    
    CHTLBasicParser parser;
    
    // 测试双引号文本
    std::string source1 = "text{\"这是一段文本\"}";
    auto result1 = parser.parse(source1, "test1.chtl");
    assert(result1.success);
    assert(result1.rootNode->children.size() == 1);
    assert(result1.rootNode->children[0]->type == CHTLASTNodeType::TEXT_NODE);
    
    auto textNode1 = std::dynamic_pointer_cast<TextNode>(result1.rootNode->children[0]);
    assert(textNode1);
    assert(textNode1->content == "这是一段文本");
    assert(textNode1->literalType == LiteralType::DOUBLE_QUOTED);
    
    // 测试单引号文本
    std::string source2 = "text{'这是一段文本'}";
    auto result2 = parser.parse(source2, "test2.chtl");
    assert(result2.success);
    auto textNode2 = std::dynamic_pointer_cast<TextNode>(result2.rootNode->children[0]);
    assert(textNode2->literalType == LiteralType::SINGLE_QUOTED);
    
    // 测试无修饰文本
    std::string source3 = "text{这是一段文本}";
    auto result3 = parser.parse(source3, "test3.chtl");
    assert(result3.success);
    auto textNode3 = std::dynamic_pointer_cast<TextNode>(result3.rootNode->children[0]);
    assert(textNode3->literalType == LiteralType::UNQUOTED);
    
    std::cout << "✅ 文本节点解析测试通过" << std::endl;
}

void testElementNodeParsing() {
    std::cout << "🧪 测试元素节点解析..." << std::endl;
    
    CHTLBasicParser parser;
    
    // 测试简单div元素
    std::string source1 = "div{}";
    auto result1 = parser.parse(source1, "test1.chtl");
    assert(result1.success);
    assert(result1.rootNode->children.size() == 1);
    assert(result1.rootNode->children[0]->type == CHTLASTNodeType::ELEMENT_NODE);
    
    auto elementNode1 = std::dynamic_pointer_cast<ElementNode>(result1.rootNode->children[0]);
    assert(elementNode1);
    assert(elementNode1->tagName == "div");
    
    // 测试简单嵌套元素
    std::string source2 = "html{head{}body{}}";
    auto result2 = parser.parse(source2, "test2.chtl");
    assert(result2.success);
    assert(result2.rootNode->children.size() == 1);
    
    auto htmlNode = std::dynamic_pointer_cast<ElementNode>(result2.rootNode->children[0]);
    assert(htmlNode->tagName == "html");
    assert(htmlNode->children.size() == 2); // head和body
    
    std::cout << "✅ 元素节点解析测试通过" << std::endl;
}

void testAttributeParsing() {
    std::cout << "🧪 测试属性解析..." << std::endl;
    
    CHTLBasicParser parser;
    
    // 测试CE对等式 (: 和 = 等价)
    std::string source1 = "div{id:box;class=welcome;data-value:\"test\";}";
    auto result1 = parser.parse(source1, "test1.chtl");
    assert(result1.success);
    
    auto divNode = std::dynamic_pointer_cast<ElementNode>(result1.rootNode->children[0]);
    assert(divNode);
    assert(divNode->children.size() == 3); // 3个属性
    
    // 检查第一个属性 (id: box;)
    assert(divNode->children[0]->type == CHTLASTNodeType::ATTRIBUTE_NODE);
    auto attr1 = std::dynamic_pointer_cast<AttributeNode>(divNode->children[0]);
    assert(attr1->name == "id");
    assert(attr1->value == "box");
    assert(attr1->valueType == LiteralType::UNQUOTED);
    
    // 检查第二个属性 (class = welcome;)
    auto attr2 = std::dynamic_pointer_cast<AttributeNode>(divNode->children[1]);
    assert(attr2->name == "class");
    assert(attr2->value == "welcome");
    assert(attr2->valueType == LiteralType::UNQUOTED);
    
    // 检查第三个属性 (data-value: "test";)
    auto attr3 = std::dynamic_pointer_cast<AttributeNode>(divNode->children[2]);
    assert(attr3->name == "data-value");
    assert(attr3->value == "test");
    assert(attr3->valueType == LiteralType::DOUBLE_QUOTED);
    
    std::cout << "✅ 属性解析测试通过" << std::endl;
}

void testLiteralTypes() {
    std::cout << "🧪 测试字面量类型..." << std::endl;
    
    CHTLBasicParser parser;
    
    // 测试各种字面量类型
    std::string source = "div{attr1:unquoted;attr2:\"double-quoted\";attr3:'single-quoted';attr4:123;}";
    auto result = parser.parse(source, "test.chtl");
    assert(result.success);
    
    auto divNode = std::dynamic_pointer_cast<ElementNode>(result.rootNode->children[0]);
    assert(divNode->children.size() == 4);
    
    // 检查无修饰字面量
    auto attr1 = std::dynamic_pointer_cast<AttributeNode>(divNode->children[0]);
    assert(attr1->valueType == LiteralType::UNQUOTED);
    assert(attr1->value == "unquoted");
    
    // 检查双引号字面量
    auto attr2 = std::dynamic_pointer_cast<AttributeNode>(divNode->children[1]);
    assert(attr2->valueType == LiteralType::DOUBLE_QUOTED);
    assert(attr2->value == "double-quoted");
    
    // 检查单引号字面量
    auto attr3 = std::dynamic_pointer_cast<AttributeNode>(divNode->children[2]);
    assert(attr3->valueType == LiteralType::SINGLE_QUOTED);
    assert(attr3->value == "single-quoted");
    
    // 检查数字字面量
    auto attr4 = std::dynamic_pointer_cast<AttributeNode>(divNode->children[3]);
    assert(attr4->valueType == LiteralType::UNQUOTED);
    assert(attr4->value == "123");
    
    std::cout << "✅ 字面量类型测试通过" << std::endl;
}

void testComplexStructure() {
    std::cout << "🧪 测试复杂结构解析..." << std::endl;
    
    CHTLBasicParser parser;
    
    // 测试简单的复杂结构
    std::string source = "//comment\nhtml{head{title:\"test\";}body{div{id:container;}}}";
    auto result = parser.parse(source, "complex.chtl");
    assert(result.success);
    
    // 检查根节点有2个子节点：注释 + html元素
    assert(result.rootNode->children.size() == 2);
    assert(result.rootNode->children[0]->type == CHTLASTNodeType::COMMENT_LINE);
    assert(result.rootNode->children[1]->type == CHTLASTNodeType::ELEMENT_NODE);
    
    std::cout << "✅ 复杂结构解析测试通过" << std::endl;
}

void testErrorHandling() {
    std::cout << "🧪 测试错误处理..." << std::endl;
    
    CHTLBasicParser parser;
    
    // 测试语法错误
    std::string source1 = "div{id:;}";
    auto result1 = parser.parse(source1, "error1.chtl");
    assert(!result1.success);
    assert(!result1.errors.empty());
    
    // 测试未知元素警告
    std::string source2 = "unknowntag{}";
    auto result2 = parser.parse(source2, "error2.chtl");
    // 应该有警告但解析成功
    assert(!result2.warnings.empty());
    
    std::cout << "✅ 错误处理测试通过" << std::endl;
}

int main() {
    std::cout << "🚀 开始CHTL基础语法解析器测试" << std::endl;
    std::cout << "========================================" << std::endl;
    
    try {
        testCommentParsing();
        testTextNodeParsing();
        testElementNodeParsing();
        testAttributeParsing();
        testLiteralTypes();
        testComplexStructure();
        testErrorHandling();
        
        std::cout << "\n========================================" << std::endl;
        std::cout << "🎉 所有测试通过！" << std::endl;
        std::cout << "✅ 基础语法解析器实现完成" << std::endl;
        std::cout << "   • 注释解析 (//、/**/、--)" << std::endl;
        std::cout << "   • 文本节点解析 (text { })" << std::endl;
        std::cout << "   • 字面量解析 (双引号、单引号、无修饰)" << std::endl;
        std::cout << "   • CE对等式支持 (: 和 = 等价)" << std::endl;
        std::cout << "   • 元素节点解析 (HTML标签)" << std::endl;
        std::cout << "   • 属性解析 (name: value;)" << std::endl;
        std::cout << "   • 严格HTML5标准验证" << std::endl;
        std::cout << "   • 错误处理和恢复" << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ 测试失败: " << e.what() << std::endl;
        return 1;
    }
}
#include "parsers/chtl_template_parser.hpp"
#include "parsers/chtl_basic_parser.hpp"
#include <iostream>

using namespace chtl;

int main() {
    std::cout << "🔍 调试CHTL模板解析器" << std::endl;
    
    CHTLTemplateParser parser;
    
    // 测试元素模板声明
    std::string source = "[Template] @Element Box { div { style { width: \"200px\"; height: \"200px\"; } } }";
    std::cout << "源代码: '" << source << "'" << std::endl;
    
    auto result = parser.parseTemplateDeclaration(source, "style_template.chtl");
    
    std::cout << "解析成功: " << (result.success ? "是" : "否") << std::endl;
    std::cout << "错误数量: " << result.errors.size() << std::endl;
    std::cout << "警告数量: " << result.warnings.size() << std::endl;
    
    if (!result.errors.empty()) {
        std::cout << "错误信息:" << std::endl;
        for (const auto& error : result.errors) {
            std::cout << "  - " << error.message << " (行:" << error.position.line << ", 列:" << error.position.column << ")" << std::endl;
        }
    }
    
    if (!result.warnings.empty()) {
        std::cout << "警告信息:" << std::endl;
        for (const auto& warning : result.warnings) {
            std::cout << "  - " << warning.message << " (行:" << warning.position.line << ", 列:" << warning.position.column << ")" << std::endl;
        }
    }
    
    if (result.rootNode) {
        std::cout << "根节点类型: " << static_cast<int>(result.rootNode->type) << std::endl;
        std::cout << "子节点数量: " << result.rootNode->children.size() << std::endl;
    } else {
        std::cout << "根节点为空!" << std::endl;
    }
    
    // 测试基础解析器直接解析元素内容
    std::cout << "\n--- 测试基础解析器直接解析元素内容 ---" << std::endl;
    CHTLBasicParser basicParser;
    std::string elementContent = "div { text { \"Hello World\" } }";
    std::cout << "元素内容: '" << elementContent << "'" << std::endl;
    
    auto basicResult = basicParser.parse(elementContent, "element_test.chtl");
    std::cout << "基础解析成功: " << (basicResult.success ? "是" : "否") << std::endl;
    std::cout << "基础解析错误数量: " << basicResult.errors.size() << std::endl;
    
    if (!basicResult.errors.empty()) {
        std::cout << "基础解析错误信息:" << std::endl;
        for (const auto& error : basicResult.errors) {
            std::cout << "  - " << error.message << std::endl;
        }
    }
    
    return 0;
}
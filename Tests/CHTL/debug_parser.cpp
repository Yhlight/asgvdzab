#include "parsers/chtl_basic_parser.hpp"
#include <iostream>

using namespace chtl;

int main() {
    std::cout << "🔍 调试CHTL基础语法解析器" << std::endl;
    
    // 测试未知元素警告
    CHTLBasicParser parser;
    std::string source = "unknowntag{}";
    std::cout << "源代码: '" << source << "'" << std::endl;
    
    auto result = parser.parse(source, "debug.chtl");
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
        
        for (size_t i = 0; i < result.rootNode->children.size(); ++i) {
            const auto& child = result.rootNode->children[i];
            std::cout << "  子节点 " << i << ": 类型=" << static_cast<int>(child->type) << std::endl;
        }
    } else {
        std::cout << "根节点为空!" << std::endl;
    }
    
    return 0;
}
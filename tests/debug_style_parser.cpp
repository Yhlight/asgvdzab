#include "parsers/chtl_basic_parser.hpp"
#include <iostream>

using namespace chtl;

int main() {
    std::cout << "🔍 调试CHTL样式解析器 - 复杂样式块" << std::endl;
    
    CHTLBasicParser parser;
    
    // 测试复杂样式块 (简化格式)
    std::string source = "div{style{width:100px;height:200px;}}";
    
    std::cout << "源代码: '" << source << "'" << std::endl;
    
    auto result = parser.parse(source, "complex_test.chtl");
    
    std::cout << "解析成功: " << (result.success ? "是" : "否") << std::endl;
    std::cout << "错误数量: " << result.errors.size() << std::endl;
    std::cout << "警告数量: " << result.warnings.size() << std::endl;
    
    if (!result.errors.empty()) {
        std::cout << "错误信息:" << std::endl;
        for (const auto& error : result.errors) {
            std::cout << "  - " << error.message << " (行:" << error.position.line << ", 列:" << error.position.column << ")" << std::endl;
        }
    }
    
    if (result.rootNode) {
        std::cout << "根节点类型: " << static_cast<int>(result.rootNode->type) << std::endl;
        std::cout << "子节点数量: " << result.rootNode->children.size() << std::endl;
        
        for (size_t i = 0; i < result.rootNode->children.size(); ++i) {
            const auto& child = result.rootNode->children[i];
            std::cout << "  子节点 " << i << ": 类型=" << static_cast<int>(child->type) << std::endl;
            
            if (child->type == CHTLASTNodeType::ELEMENT_NODE) {
                std::cout << "    元素子节点数量: " << child->children.size() << std::endl;
                for (size_t j = 0; j < child->children.size(); ++j) {
                    std::cout << "      子节点 " << j << ": 类型=" << static_cast<int>(child->children[j]->type) << std::endl;
                }
            }
        }
    } else {
        std::cout << "根节点为空!" << std::endl;
    }
    
    return 0;
}
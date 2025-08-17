#include "parsers/chtl_config_import_parser.hpp"
#include <iostream>
#include <string>

using namespace chtl;

int main() {
    std::cout << "🔍 调试自定义样式导入声明" << std::endl;
    
    CHTLConfigImportParser parser;
    
    // 测试自定义样式组导入
    std::string source = "[Import] [Custom] @Style ThemeColors from styles/theme.chtl;";
    
    std::cout << "解析源代码:\n" << source << std::endl;
    
    auto result = parser.parseImportDeclaration(source, "import.chtl");
    
    std::cout << "\n解析结果: " << (result.success ? "成功" : "失败") << std::endl;
    
    if (result.success && result.rootNode) {
        auto importNode = std::dynamic_pointer_cast<ImportDeclarationNode>(result.rootNode);
        if (importNode) {
            std::cout << "✅ ImportDeclarationNode created" << std::endl;
            std::cout << "  importType: '" << importNode->importType << "'" << std::endl;
            std::cout << "  targetName: '" << importNode->targetName << "'" << std::endl;
            std::cout << "  path: '" << importNode->path << "'" << std::endl;
            std::cout << "  alias: '" << importNode->alias << "'" << std::endl;
            
            std::cout << "期望importType: '[Custom] @Style'" << std::endl;
            std::cout << "匹配: " << (importNode->importType == "[Custom] @Style" ? "✅" : "❌") << std::endl;
        }
    } else {
        std::cout << "❌ 解析失败" << std::endl;
        if (!result.success) {
            for (const auto& error : result.errors) {
                std::cout << "  错误: " << error.message << std::endl;
            }
        }
    }
    
    return 0;
}
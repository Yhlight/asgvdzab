#include "core/chtl_unified_scanner.hpp"
#include "core/compiler_dispatcher.hpp"
#include <iostream>
#include <vector>
#include <string>

using namespace chtl;

struct CHTLSyntaxTest {
    std::string name;
    std::string chtlCode;
    std::string expectedKeyword;
};

// 基于CHTL语法文档的语法测试用例
std::vector<CHTLSyntaxTest> getCHTLSyntaxTests() {
    return {
        // 1. 注释测试 (CHTL语法文档：注释部分)
        {
            "生成器注释",
            "-- 这是会被生成器识别的注释",
            "<!--"
        },
        {
            "普通注释",
            "// 这是普通注释",
            "" // 普通注释不输出
        },
        
        // 2. 文本节点测试
        {
            "文本节点（带引号）",
            "text { \"这是一段文本\" }",
            "这是一段文本"
        },
        {
            "文本节点（无引号字面量）",
            "text { 这是一段文本 }",
            "这是一段文本"
        },
        
        // 3. 元素节点测试
        {
            "基本HTML元素",
            "div { }",
            "<div></div>"
        },
        {
            "嵌套HTML元素",
            "div { span { } }",
            "<div><span></span></div>"
        },
        
        // 4. 属性测试
        {
            "冒号属性语法",
            "id: box;",
            "id=\"box\""
        },
        {
            "等号属性语法（CE对等式）",
            "class = welcome;",
            "class=\"welcome\""
        },
        
        // 5. 模板定义测试 (CHTL语法文档：模板部分)
        {
            "样式组模板定义",
            "[Template] @Style DefaultText { color: black; }",
            "Template registered: DefaultText"
        },
        {
            "元素模板定义",
            "[Template] @Element Box { div { } }",
            "Template registered: Box"
        },
        {
            "变量组模板定义",
            "[Template] @Var ThemeColor { tableColor: rgb(255, 192, 203); }",
            "Template registered: ThemeColor"
        },
        
        // 6. 自定义定义测试 (CHTL语法文档：自定义部分)
        {
            "自定义样式组",
            "[Custom] @Style TextSet { color, font-size; }",
            "Custom registered: TextSet"
        },
        {
            "自定义元素",
            "[Custom] @Element Box { div { } span { } }",
            "Custom registered: Box"
        },
        
        // 7. 原始嵌入测试 (CHTL语法文档：原始嵌入部分)
        {
            "HTML原始嵌入",
            "[Origin] @Html { <p>原始HTML</p> }",
            "<p>原始HTML</p>"
        },
        {
            "带名原始嵌入",
            "[Origin] @Html box { <div>内容</div> }",
            "<div>内容</div>"
        },
        
        // 8. 配置组测试 (CHTL语法文档：配置组部分)
        {
            "配置块",
            "[Configuration] { DEBUG_MODE = false; }",
            "Configuration block processed"
        },
        
        // 9. 导入测试 (CHTL语法文档：导入部分)
        {
            "导入HTML文件",
            "[Import] @Html from style.html",
            "Import processed: HTML file"
        },
        {
            "导入CHTL文件",
            "[Import] @Chtl from components.chtl",
            "Import processed: CHTL file"
        },
        
        // 10. 命名空间测试 (CHTL语法文档：命名空间部分)
        {
            "命名空间定义",
            "[Namespace] myspace { div { } }",
            "<div></div>"
        },
        
        // 11. 约束测试 (CHTL语法文档：约束部分)
        {
            "精确约束",
            "except span, [Custom] @Element Box;",
            "Constraint applied: constraint rules"
        },
        {
            "类型约束",
            "except @Html;",
            "Constraint applied: constraint rules"
        },
        
        // 12. 变量组使用测试
        {
            "变量组基本使用",
            "ThemeColor(tableColor)",
            "Variable group ThemeColor with params: tableColor"
        },
        {
            "变量组特例化使用",
            "ThemeColor(tableColor = rgb(145, 155, 200))",
            "Variable group ThemeColor with params: tableColor = rgb(145, 155, 200)"
        },
        
        // 13. 模板使用测试
        {
            "样式组模板使用",
            "@Style DefaultText;",
            "Template Style DefaultText expanded"
        },
        {
            "元素模板使用",
            "@Element Box;",
            "Template Element Box expanded"
        },
    };
}

void testCHTLSyntax() {
    std::cout << "\n🧪 CHTL语法完整性测试" << std::endl;
    std::cout << "基于CHTL语法文档的严格测试" << std::endl;
    std::cout << "========================================" << std::endl;
    
    auto tests = getCHTLSyntaxTests();
    int passedTests = 0;
    int totalTests = tests.size();
    
    CompilerFactory::setDebugMode(false); // 关闭调试输出，便于查看测试结果
    auto compiler = CompilerFactory::createCHTLCompiler();
    
    if (!compiler) {
        std::cerr << "❌ 无法创建CHTL编译器" << std::endl;
        return;
    }
    
    if (!compiler->initialize()) {
        std::cerr << "❌ CHTL编译器初始化失败" << std::endl;
        return;
    }
    
    for (const auto& test : tests) {
        std::cout << "\n🔍 测试: " << test.name << std::endl;
        std::cout << "   代码: " << test.chtlCode << std::endl;
        
        CodeFragment fragment;
        fragment.type = CodeFragmentType::CHTL_FRAGMENT;
        fragment.content = test.chtlCode;
        
        auto result = compiler->compile(fragment);
        
        std::cout << "   输出: " << result.output << std::endl;
        
        if (result.success && result.output.find(test.expectedKeyword) != std::string::npos) {
            std::cout << "   ✅ 通过" << std::endl;
            passedTests++;
        } else {
            std::cout << "   ❌ 失败";
            if (!result.success) {
                std::cout << " (编译失败)";
                for (const auto& error : result.errors) {
                    std::cout << " " << error;
                }
            } else {
                std::cout << " (输出不匹配，期望包含: " << test.expectedKeyword << ")";
            }
            std::cout << std::endl;
        }
    }
    
    std::cout << "\n📊 测试结果总结" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "总测试数: " << totalTests << std::endl;
    std::cout << "通过数: " << passedTests << std::endl;
    std::cout << "失败数: " << (totalTests - passedTests) << std::endl;
    std::cout << "通过率: " << (passedTests * 100 / totalTests) << "%" << std::endl;
    
    if (passedTests == totalTests) {
        std::cout << "🎉 所有CHTL语法测试通过！编译器完全支持CHTL语法文档" << std::endl;
    } else {
        std::cout << "⚠️  部分测试失败，需要进一步完善编译器" << std::endl;
    }
}

int main() {
    std::cout << "🚀 CHTL编译器语法完整性测试" << std::endl;
    std::cout << "严格基于CHTL语法文档进行测试" << std::endl;
    
    try {
        testCHTLSyntax();
    } catch (const std::exception& e) {
        std::cerr << "❌ 测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
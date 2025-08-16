#include "constraints/chtl_global_style_constraint.hpp"
#include "constraints/chtl_js_constraint.hpp"
#include <iostream>

using namespace chtl;

void testGlobalStyleConstraint() {
    std::cout << "=== CHTL全局样式块约束器测试 ===" << std::endl;
    
    CHtlGlobalStyleConstraint constraint;
    
    // 测试允许的全局样式语法
    std::string validGlobalStyle = R"(
        /* 允许的语法 */
        .main-container {
            color: red;
            background: ThemeColor(primaryBg);
        }
        
        #header {
            font-size: 24px;
        }
        
        @Style DefaultButton;
        inherit @Style BaseTheme;
        delete border, margin;
        delete @Style OldTheme;
        
        -- 这是生成器注释
        
        [Origin] @Style {
            /* 原始CSS嵌入 */
            body { margin: 0; }
        }
        
        ThemeColor from ui.theme;
        ButtonStyle from components.buttons;
        
        ui.theme.PrimaryColor;
    )";
    
    auto result = constraint.validateGlobalStyleBlock(validGlobalStyle);
    
    std::cout << "有效语法测试: " << (result.isValid ? "✓ 通过" : "✗ 失败") << std::endl;
    if (!result.isValid) {
        for (const auto& violation : result.violations) {
            std::cout << "  错误: " << violation.message << " (" << violation.element << ")" << std::endl;
        }
    }
    
    std::cout << "使用的模板变量: " << result.usedTemplateVariables.size() << std::endl;
    std::cout << "使用的样式组: " << result.usedStyleGroups.size() << std::endl;
    std::cout << "继承关系: " << result.inheritances.size() << std::endl;
    
    // 测试禁止的语法
    std::string invalidGlobalStyle = R"(
        /* 禁止的语法 */
        .button {
            color: red;
        }
        
        .button:hover {  /* 伪类应该在局部样式使用 */
            background: blue;
        }
        
        &:focus {  /* 局部样式特性 */
            border: 1px solid red;
        }
        
        @media (max-width: 768px) {  /* 媒体查询 */
            .container { width: 100%; }
        }
        
        style {  /* 嵌套样式块 */
            color: green;
        }
    )";
    
    auto invalidResult = constraint.validateGlobalStyleBlock(invalidGlobalStyle);
    
    std::cout << "\n无效语法测试: " << (invalidResult.isValid ? "✗ 失败(应该检测到错误)" : "✓ 通过(正确检测到错误)") << std::endl;
    if (!invalidResult.isValid) {
        std::cout << "检测到的错误:" << std::endl;
        for (const auto& violation : invalidResult.violations) {
            std::cout << "  - " << violation.message << std::endl;
        }
    }
}

void testCHtlJSConstraint() {
    std::cout << "\n=== CHTL JavaScript块约束器测试 ===" << std::endl;
    
    CHtlJSConstraint constraint;
    
    // 测试允许的JavaScript语法
    std::string validJS = R"(
        // 允许的语法
        function initApp() {
            const theme = ThemeColor(primaryColor);
            const button = chtl.getElementById('main-button');
            
            button.addEventListener('click', function() {
                console.log('Button clicked');
            });
            
            // 使用命名空间变量
            const config = ConfigVar from ui.settings;
            const primaryButton = ui.components.PrimaryButton;
        }
        
        // 生成器注释
        -- 这是生成器注释
        
        // 原始JavaScript嵌入
        [Origin] @JavaScript {
            // 原始JavaScript代码
            window.onload = function() {
                console.log('Page loaded');
            };
        }
        
        // CHTL DOM访问
        const element = chtl.getElementByClass('container');
    )";
    
    auto result = constraint.validateCHtlJSBlock(validJS);
    
    std::cout << "有效JavaScript语法测试: " << (result.isValid ? "✓ 通过" : "✗ 失败") << std::endl;
    if (!result.isValid) {
        for (const auto& violation : result.violations) {
            std::cout << "  错误: " << violation.message << std::endl;
        }
    }
    
    std::cout << "使用的模板变量: " << result.usedTemplateVariables.size() << std::endl;
    std::cout << "访问的DOM元素: " << result.accessedDOMElements.size() << std::endl;
    
    // 测试禁止的JavaScript语法
    std::string invalidJS = R"(
        // 禁止的语法
        function dangerous() {
            eval('alert("XSS")');  // 禁止eval
            
            const div = document.createElement('div');
            div.innerHTML = userInput;  // 危险的innerHTML
            
            // 禁止在JS中使用CHTL元素定义语法
            @Element Button {
                div {
                    text { "Click me" }
                }
            }
            
            style {
                color: red;
            }
            
            [Template] @Style MyStyle {
                background: blue;
            }
        }
    )";
    
    auto invalidResult = constraint.validateCHtlJSBlock(invalidJS);
    
    std::cout << "\n无效JavaScript语法测试: " << (invalidResult.isValid ? "✗ 失败(应该检测到错误)" : "✓ 通过(正确检测到错误)") << std::endl;
    if (!invalidResult.isValid) {
        std::cout << "检测到的错误:" << std::endl;
        for (const auto& violation : invalidResult.violations) {
            std::cout << "  - " << violation.message << std::endl;
        }
    }
}

void testSpecificConstraints() {
    std::cout << "\n=== 特定约束测试 ===" << std::endl;
    
    CHtlGlobalStyleConstraint styleConstraint;
    CHtlJSConstraint jsConstraint;
    
    // 测试变量引用验证
    std::cout << "\n变量引用验证:" << std::endl;
    std::cout << "模板变量 'ThemeColor(primary)': " 
              << (styleConstraint.validateTemplateVariableReference("ThemeColor(primary)") ? "✓" : "✗") << std::endl;
    std::cout << "自定义变量 '[Custom] @Var MyVar(value)': " 
              << (styleConstraint.validateCustomVariableReference("[Custom] @Var MyVar(value)") ? "✓" : "✗") << std::endl;
    
    // 测试样式组引用验证
    std::cout << "\n样式组引用验证:" << std::endl;
    std::cout << "模板样式组 '@Style DefaultButton': " 
              << (styleConstraint.validateStyleGroupReference("@Style DefaultButton") ? "✓" : "✗") << std::endl;
    std::cout << "自定义样式组 '[Custom] @Style MyButton': " 
              << (styleConstraint.validateStyleGroupReference("[Custom] @Style MyButton") ? "✓" : "✗") << std::endl;
    
    // 测试delete和inherit语句
    std::cout << "\ndelete/inherit语句验证:" << std::endl;
    std::cout << "delete属性 'delete color, margin': " 
              << (styleConstraint.validateDeleteStatement("delete color, margin") ? "✓" : "✗") << std::endl;
    std::cout << "delete继承 'delete @Style OldTheme': " 
              << (styleConstraint.validateDeleteStatement("delete @Style OldTheme") ? "✓" : "✗") << std::endl;
    std::cout << "继承语句 'inherit @Style BaseTheme': " 
              << (styleConstraint.validateInheritStatement("inherit @Style BaseTheme") ? "✓" : "✗") << std::endl;
    
    // 测试from子句
    std::cout << "\nfrom子句验证:" << std::endl;
    std::cout << "命名空间引用 'ButtonStyle from ui.components': " 
              << (styleConstraint.validateFromClause("ButtonStyle from ui.components") ? "✓" : "✗") << std::endl;
    
    // 测试全缀名
    std::cout << "\n全缀名验证:" << std::endl;
    std::cout << "全缀名 'ui.theme.PrimaryColor': " 
              << (styleConstraint.validateFullQualifiedName("ui.theme.PrimaryColor") ? "✓" : "✗") << std::endl;
    
    // 测试JavaScript约束
    std::cout << "\nJavaScript约束验证:" << std::endl;
    std::cout << "CHTL DOM访问 'chtl.getElementById(\"test\")': " 
              << (jsConstraint.validateCHtlDOMAccess("chtl.getElementById(\"test\")") ? "✓" : "✗") << std::endl;
    
    // 测试禁止的API检测
    auto forbiddenAPIs = jsConstraint.checkForbiddenAPIs("eval('test'); innerHTML = 'content';");
    std::cout << "检测到的禁止API数量: " << forbiddenAPIs.size() << std::endl;
    for (const auto& api : forbiddenAPIs) {
        std::cout << "  - " << api << std::endl;
    }
}

int main() {
    try {
        std::cout << "🚀 CHTL语法约束器测试" << std::endl;
        std::cout << "验证CHTL全局样式块和JavaScript块的语法边界\n" << std::endl;
        
        testGlobalStyleConstraint();
        testCHtlJSConstraint();
        testSpecificConstraints();
        
        std::cout << "\n🎉 约束器测试完成!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "测试异常: " << e.what() << std::endl;
        return 1;
    }
}
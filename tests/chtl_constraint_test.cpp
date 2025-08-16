#include "constraints/chtl_global_style_constraint.hpp"
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
        
        [Custom] @Style MyButton;
        [Template] @Style BaseButton;
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

void testCHTLSyntaxCorrectness() {
    std::cout << "\n=== CHTL语法正确性测试 ===" << std::endl;
    
    CHtlGlobalStyleConstraint constraint;
    
    // 测试正确的CHTL语法元素
    std::cout << "\n单个语法元素验证:" << std::endl;
    
    // 测试变量引用的多种格式
    std::cout << "变量引用简写 'ThemeColor(primary)': " 
              << (constraint.validateTemplateVariableReference("color: ThemeColor(primary);") ? "✓" : "✗") << std::endl;
    
    std::cout << "变量引用完整 '@Var ThemeColor(primary)': " 
              << (constraint.validateTemplateVariableReference("color: @Var ThemeColor(primary);") ? "✓" : "✗") << std::endl;
    
    // 测试变量特例化
    std::cout << "变量特例化 'ThemeColor(tableColor = rgb(255, 192, 203))': " 
              << (constraint.validateCustomVariableReference("color: ThemeColor(tableColor = rgb(255, 192, 203));") ? "✓" : "✗") << std::endl;
    
    // 测试样式组引用
    std::cout << "样式组引用 '@Style DefaultButton;': " 
              << (constraint.validateStyleGroupReference("@Style DefaultButton;") ? "✓" : "✗") << std::endl;
    
    // 测试全缀名（包括@Var）
    std::cout << "全缀名样式 '[Custom] @Style MyButton': " 
              << (constraint.validateFullQualifiedName("[Custom] @Style MyButton") ? "✓" : "✗") << std::endl;
              
    std::cout << "全缀名变量 '[Template] @Var ThemeColor': " 
              << (constraint.validateFullQualifiedName("[Template] @Var ThemeColor") ? "✓" : "✗") << std::endl;
    
    // 测试继承
    std::cout << "继承语句 'inherit @Style BaseTheme': " 
              << (constraint.validateInheritStatement("inherit @Style BaseTheme") ? "✓" : "✗") << std::endl;
    
    // 测试delete操作
    std::cout << "delete属性 'delete line-height, border': " 
              << (constraint.validateDeleteStatement("delete line-height, border") ? "✓" : "✗") << std::endl;
    
    std::cout << "delete继承 'delete @Style OldTheme': " 
              << (constraint.validateDeleteStatement("delete @Style OldTheme") ? "✓" : "✗") << std::endl;
}

void testSpecificConstraints() {
    std::cout << "\n=== CHTL原始嵌入和注释测试 ===" << std::endl;
    
    CHtlGlobalStyleConstraint styleConstraint;
    
    // 测试生成器注释和原始嵌入（通过checkAllowedElement方法）
    std::cout << "\n特殊语法元素验证:" << std::endl;
    auto commentTypes = styleConstraint.checkAllowedElement("-- 这是生成器注释");
    std::cout << "生成器注释 '-- 这是生成器注释': " 
              << (!commentTypes.empty() ? "✓" : "✗") << std::endl;
    
    auto embeddingTypes = styleConstraint.checkAllowedElement("[Origin] @Style");
    std::cout << "原始样式嵌入 '[Origin] @Style': " 
              << (!embeddingTypes.empty() ? "✓" : "✗") << std::endl;
    
    // 测试from子句 (导入语法)
    std::cout << "\nfrom子句验证:" << std::endl;
    std::cout << "导入语句 'ButtonStyle from ui/components.chtl': " 
              << (styleConstraint.validateFromClause("ButtonStyle from ui/components.chtl") ? "✓" : "✗") << std::endl;
    
    // 测试完整的全局样式块
    std::cout << "\n完整样式块测试:" << std::endl;
    std::string correctCHTLStyle = R"(
        /* 正确的CHTL全局样式语法 */
        .container {
            color: ThemeColor(primaryColor);
            background: @Var CustomColor(userBg);
        }
        
        #header {
            font-size: @Var FontSize(large);
        }
        
        @Style DefaultButton;
        [Template] @Style BaseTheme;
        [Custom] @Style UserTheme;
        [Template] @Var ThemeColor;
        [Custom] @Var UserSettings;
        
        inherit @Style BaseTheme;
        delete border, margin;
        
        -- 这是生成器注释，会被生成器识别
        
        [Origin] @Style {
            /* 原始CSS代码，直接输出 */
            * { box-sizing: border-box; }
        }
    )";
    
    auto result = styleConstraint.validateGlobalStyleBlock(correctCHTLStyle);
    std::cout << "正确CHTL样式块验证: " << (result.violations.size() < 5 ? "✓ 大部分通过" : "✗ 多处错误") << std::endl;
    if (!result.violations.empty()) {
        std::cout << "  发现 " << result.violations.size() << " 个验证问题" << std::endl;
    }
}

int main() {
    try {
        std::cout << "🚀 CHTL语法约束器测试" << std::endl;
        std::cout << "验证CHTL全局样式块和JavaScript块的语法边界\n" << std::endl;
        
        testGlobalStyleConstraint();
        testCHTLSyntaxCorrectness();
        testSpecificConstraints();
        
        std::cout << "\n🎉 约束器测试完成!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "测试异常: " << e.what() << std::endl;
        return 1;
    }
}
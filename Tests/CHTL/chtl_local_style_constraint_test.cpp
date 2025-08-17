#include "constraints/chtl_local_style_constraint.hpp"
#include <iostream>

using namespace chtl;

void testValidLocalStyleSyntax() {
    std::cout << "=== CHTL局部样式块允许语法测试 ===" << std::endl;
    
    CHtlLocalStyleConstraint constraint;
    
    // Test valid local style syntax (same as global style)
    std::string validLocalStyle = R"(
        /* 允许的CHTL语法 - 与全局样式块相同 */
        
        // 模板变量
        color: ThemeColor(primary);
        background: @Var ColorPalette(secondary);
        
        // 自定义变量特例化
        border-color: ThemeColor(tableColor = rgb(255, 192, 203));
        
        // 样式组引用
        @Style DefaultButton;
        [Custom] @Style MyCustomButton;
        
        // 继承
        inherit @Style BaseTheme;
        
        // delete操作
        delete line-height, border;
        delete @Style OldTheme;
        
        // 生成器注释
        -- 局部样式配置信息
        
        // 原始嵌入
        [Origin] @Style {
            .local-dynamic { color: blue; }
        }
        
        // from命名空间
        ButtonStyle from ui/components.chtl;
        
        // 全缀名
        [Template] @Var ThemeColor;
        [Custom] @Style LocalButton;
        
        // 基本CSS语法
        .local-button {
            padding: 10px;
            margin: 5px;
        }
        
        #local-container {
            width: 100%;
            height: auto;
        }
        
        font-size: 14px;
        background-color: white;
    )";
    
    auto result = constraint.validateCHtlLocalStyleBlock(validLocalStyle);
    
    std::cout << "有效局部样式语法测试: " << (result.isValid ? "✓ 通过" : "✗ 失败") << std::endl;
    if (!result.isValid) {
        for (const auto& violation : result.violations) {
            std::cout << "  错误: " << violation.message << " (" << violation.element << ")" << std::endl;
        }
    }
    
    std::cout << "使用的模板变量: " << result.templateVariables.size() << std::endl;
    std::cout << "使用的自定义变量: " << result.customVariables.size() << std::endl;
    std::cout << "使用的样式组: " << result.styleGroups.size() << std::endl;
    std::cout << "继承关系: " << result.inheritances.size() << std::endl;
    std::cout << "生成器注释: " << result.foundGeneratorComments.size() << std::endl;
    std::cout << "原始嵌入: " << result.foundRawEmbeddings.size() << std::endl;
    std::cout << "from子句: " << result.fromClauses.size() << std::endl;
    std::cout << "全缀名: " << result.fullQualifiedNames.size() << std::endl;
    std::cout << "CSS语句: " << result.cssStatements << std::endl;
}

void testLocalStyleCHTLSyntaxCorrectness() {
    std::cout << "\n=== CHTL局部样式语法正确性测试 ===" << std::endl;
    
    CHtlLocalStyleConstraint constraint;
    
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

void testLocalStyleSpecificConstraints() {
    std::cout << "\n=== 局部样式特定约束测试 ===" << std::endl;
    
    CHtlLocalStyleConstraint constraint;
    
    // 测试生成器注释验证
    std::cout << "\n生成器注释验证:" << std::endl;
    std::cout << "有效注释 '-- 局部样式配置': " 
              << (constraint.validateGeneratorComment("-- 局部样式配置") ? "✓" : "✗") << std::endl;
    std::cout << "无效注释 '<!-- HTML注释 -->': " 
              << (constraint.validateGeneratorComment("<!-- HTML注释 -->") ? "✗" : "✓") << std::endl;
    
    // 测试原始嵌入验证 - 官方支持3种类型
    std::cout << "\n原始嵌入验证:" << std::endl;
    std::cout << "HTML嵌入 '[Origin] @Html {': " 
              << (constraint.validateRawEmbedding("[Origin] @Html {") ? "✓" : "✗") << std::endl;
    std::cout << "CSS嵌入 '[Origin] @Style {': " 
              << (constraint.validateRawEmbedding("[Origin] @Style {") ? "✓" : "✗") << std::endl;
    std::cout << "JS嵌入 '[Origin] @JavaScript {': " 
              << (constraint.validateRawEmbedding("[Origin] @JavaScript {") ? "✓" : "✗") << std::endl;
    std::cout << "无效嵌入 '[Origin] @Custom {': " 
              << (constraint.validateRawEmbedding("[Origin] @Custom {") ? "✗" : "✓") << std::endl;
    
    // 测试from子句验证
    std::cout << "\nfrom子句验证:" << std::endl;
    std::cout << "导入语句 'ButtonStyle from ui/components.chtl': " 
              << (constraint.validateFromClause("ButtonStyle from ui/components.chtl") ? "✓" : "✗") << std::endl;
    std::cout << "无效导入 'ButtonStyle import ui/components': " 
              << (constraint.validateFromClause("ButtonStyle import ui/components") ? "✗" : "✓") << std::endl;
}

void testCompleteLocalStyleBlock() {
    std::cout << "\n=== 完整局部样式块测试 ===" << std::endl;
    
    CHtlLocalStyleConstraint constraint;
    
    // 测试完整的局部样式块
    std::string completeLocalStyleBlock = R"(
        /* 完整的CHTL局部样式块示例 */
        
        -- 局部样式块配置
        -- 支持所有CHTL样式语法
        
        // 变量系统
        color: ThemeColor(primary);
        background: @Var ColorPalette(secondary);
        border-color: CustomColor(highlight = #ff6b6b);
        
        // 样式组系统
        @Style ButtonBase;
        [Custom] @Style LocalButton;
        inherit @Style Theme;
        
        // 操作系统
        delete margin, padding;
        delete @Style OldStyle;
        
        // 命名空间导入
        IconStyle from icons/basic.chtl;
        
        // 全缀名引用
        [Template] @Var FontSize;
        [Custom] @Style CustomButton;
        
        // 原始嵌入
        [Origin] @Style {
            .local-override {
                transition: all 0.3s ease;
            }
        }
        
        // 基本CSS
        .local-button {
            padding: 8px 16px;
            border-radius: 4px;
            cursor: pointer;
        }
        
        &:hover {
            opacity: 0.8;
        }
        
        font-family: Arial, sans-serif;
        line-height: 1.5;
    )";
    
    auto result = constraint.validateCHtlLocalStyleBlock(completeLocalStyleBlock);
    
    std::cout << "完整局部样式块验证: " << (result.isValid ? "✓ 通过" : "✗ 有问题") << std::endl;
    if (!result.isValid) {
        std::cout << "发现 " << result.violations.size() << " 个验证问题:" << std::endl;
        for (const auto& violation : result.violations) {
            std::cout << "  - " << violation.message << " (" << violation.element << ")" << std::endl;
        }
    }
    
    std::cout << "\n统计信息:" << std::endl;
    std::cout << "  模板变量: " << result.templateVariables.size() << " 个" << std::endl;
    std::cout << "  自定义变量: " << result.customVariables.size() << " 个" << std::endl;
    std::cout << "  样式组: " << result.styleGroups.size() << " 个" << std::endl;
    std::cout << "  继承关系: " << result.inheritances.size() << " 个" << std::endl;
    std::cout << "  生成器注释: " << result.foundGeneratorComments.size() << " 个" << std::endl;
    std::cout << "  原始嵌入: " << result.foundRawEmbeddings.size() << " 个" << std::endl;
    std::cout << "  from子句: " << result.fromClauses.size() << " 个" << std::endl;
    std::cout << "  全缀名: " << result.fullQualifiedNames.size() << " 个" << std::endl;
    std::cout << "  CSS语句: " << result.cssStatements << " 行" << std::endl;
}

int main() {
    try {
        std::cout << "🚀 CHTL局部样式块约束器测试" << std::endl;
        std::cout << "验证CHTL局部样式块(style { ... })的语法约束：与全局样式块约束相同\n" << std::endl;
        
        testValidLocalStyleSyntax();
        testLocalStyleCHTLSyntaxCorrectness();
        testLocalStyleSpecificConstraints();
        testCompleteLocalStyleBlock();
        
        std::cout << "\n🎉 局部样式约束器测试完成!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "测试异常: " << e.what() << std::endl;
        return 1;
    }
}
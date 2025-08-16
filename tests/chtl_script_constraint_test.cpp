#include "constraints/chtl_script_constraint.hpp"
#include <iostream>

using namespace chtl;

void testValidScriptSyntax() {
    std::cout << "=== CHTL Script块允许语法测试 ===" << std::endl;
    
    CHtlScriptConstraint constraint;
    
    // 测试允许的Script语法
    std::string validScript = R"(
        // ✅ 允许的语法
        
        // 纯JavaScript代码
        function initApp() {
            console.log('Application starting...');
            const data = fetchData();
            return data;
        }
        
        // 生成器注释
        -- 这是生成器注释，会被生成器识别
        
        // 原始HTML嵌入
        [Origin] @Html {
            <div id="dynamic-content"></div>
        }
        
        // 原始CSS嵌入
        [Origin] @Style {
            .dynamic-style { color: blue; }
        }
        
        // 原始JavaScript嵌入
        [Origin] @JavaScript {
            window.customConfig = { version: '1.0' };
        }
        
        // 标准JavaScript代码继续
        var config = {
            theme: 'dark',
            version: '1.0'
        };
        
        // 更多纯JavaScript
        const appState = {
            initialized: false,
            data: null
        };
        
        document.addEventListener('DOMContentLoaded', function() {
            initApp();
        });
    )";
    
    auto result = constraint.validateCHtlScriptBlock(validScript);
    
    std::cout << "有效Script语法测试: " << (result.isValid ? "✓ 通过" : "✗ 失败") << std::endl;
    if (!result.isValid) {
        for (const auto& violation : result.violations) {
            std::cout << "  错误: " << violation.message << std::endl;
        }
    }
    
    std::cout << "发现的生成器注释: " << result.foundGeneratorComments.size() << std::endl;
    std::cout << "发现的原始嵌入: " << result.foundRawEmbeddings.size() << std::endl;
    std::cout << "纯JavaScript行数: " << result.pureJavaScriptLines << std::endl;
}

void testForbiddenScriptSyntax() {
    std::cout << "\n=== CHTL Script块禁止语法测试 ===" << std::endl;
    
    CHtlScriptConstraint constraint;
    
    // 测试禁止的Script语法
    std::string forbiddenScript = R"(
        // ❌ 禁止的CHTL语法
        
        function badFunction() {
            // 禁止：模板变量
            const color = ThemeColor(primaryColor);
            
            // 禁止：@Var变量引用
            const size = @Var FontSize(large);
            
            // 禁止：变量特例化
            const customColor = ThemeColor(tableColor = rgb(255, 0, 0));
            
            // 禁止：命名空间from子句
            const button = ButtonStyle from ui.components;
            
            // 禁止：样式组引用
            @Style DefaultButton;
            
            // 禁止：元素定义
            @Element CustomBox;
            
            // 禁止：全缀名
            [Custom] @Style MyButton;
            [Template] @Var ThemeColor;
            
            // 禁止：样式块
            style {
                color: red;
            }
            
            // 禁止：文本块
            text {
                "Some text"
            }
            
            // 禁止：继承
            inherit @Style BaseTheme;
            
            // 禁止：delete操作
            delete color, background;
            delete @Style OldTheme;
        }
    )";
    
    auto result = constraint.validateCHtlScriptBlock(forbiddenScript);
    
    std::cout << "禁止Script语法测试: " << (result.isValid ? "✗ 失败(应该检测到错误)" : "✓ 通过(正确检测到错误)") << std::endl;
    if (!result.isValid) {
        std::cout << "检测到的CHTL语法违规:" << std::endl;
        for (const auto& violation : result.violations) {
            std::cout << "  - " << violation.message << std::endl;
        }
    }
}

void testSpecificScriptConstraints() {
    std::cout << "\n=== 特定Script约束测试 ===" << std::endl;
    
    CHtlScriptConstraint constraint;
    
    // 测试生成器注释验证
    std::cout << "\n生成器注释验证:" << std::endl;
    std::cout << "有效注释 '-- 配置信息': " 
              << (constraint.validateGeneratorComment("-- 配置信息") ? "✓" : "✗") << std::endl;
    std::cout << "无效注释 '<!-- HTML注释 -->': " 
              << (constraint.validateGeneratorComment("<!-- HTML注释 -->") ? "✗" : "✓") << std::endl;
    
    // 测试原始嵌入验证 - 类型标识无用，任何类型都允许
    std::cout << "\n原始嵌入验证:" << std::endl;
    std::cout << "HTML嵌入 '[Origin] @Html {': " 
              << (constraint.validateRawEmbedding("[Origin] @Html {") ? "✓" : "✗") << std::endl;
    std::cout << "CSS嵌入 '[Origin] @Style {': " 
              << (constraint.validateRawEmbedding("[Origin] @Style {") ? "✓" : "✗") << std::endl;
    std::cout << "JS嵌入 '[Origin] @JavaScript {': " 
              << (constraint.validateRawEmbedding("[Origin] @JavaScript {") ? "✓" : "✗") << std::endl;
    std::cout << "无效嵌入 '[Origin] @Chtl {': " 
              << (constraint.validateRawEmbedding("[Origin] @Chtl {") ? "✗" : "✓") << std::endl;
    std::cout << "无效嵌入 '[Origin] @CustomType {': " 
              << (constraint.validateRawEmbedding("[Origin] @CustomType {") ? "✗" : "✓") << std::endl;
    
    // 测试禁止的CHTL语法检测
    std::cout << "\n禁止语法检测:" << std::endl;
    
    std::string testVarScript = "const color = ThemeColor(primary);";
    auto varResult = constraint.checkForbiddenCHtlSyntax(testVarScript);
    std::cout << "变量引用检测: " << (!varResult.empty() ? "✓ 检测到" : "✗ 未检测到") << std::endl;
    
    std::string testStyleScript = "@Style DefaultButton;";
    auto styleResult = constraint.checkForbiddenCHtlSyntax(testStyleScript);
    std::cout << "样式组检测: " << (!styleResult.empty() ? "✓ 检测到" : "✗ 未检测到") << std::endl;
    
    std::string testFromScript = "const btn = ButtonStyle from ui.components;";
    auto fromResult = constraint.checkForbiddenCHtlSyntax(testFromScript);
    std::cout << "From子句检测: " << (!fromResult.empty() ? "✓ 检测到" : "✗ 未检测到") << std::endl;
}

void testMixedScriptContent() {
    std::cout << "\n=== 混合Script内容测试 ===" << std::endl;
    
    CHtlScriptConstraint constraint;
    
    // 测试混合的Script内容
    std::string mixedScript = R"(
        // 部分允许，部分禁止的混合内容
        
        // ✅ 允许：纯JavaScript
        function validFunction() {
            console.log('This is valid JavaScript');
            return { status: 'ok' };
        }
        
        // ✅ 允许：生成器注释
        -- Script配置注释
        
        // ✅ 允许：原始嵌入
        [Origin] @Html {
            <div class="embedded"></div>
        }
        
        // ❌ 禁止：变量引用
        const invalidColor = ThemeColor(primary);
        
        // ✅ 允许：纯JavaScript
        document.getElementById('test').addEventListener('click', function() {
            validFunction();
        });
        
        // ❌ 禁止：样式组
        @Style InvalidButton;
    )";
    
    auto result = constraint.validateCHtlScriptBlock(mixedScript);
    
    std::cout << "混合内容测试: " << (result.isValid ? "部分通过" : "检测到违规") << std::endl;
    std::cout << "  生成器注释: " << result.foundGeneratorComments.size() << " 个" << std::endl;
    std::cout << "  原始嵌入: " << result.foundRawEmbeddings.size() << " 个" << std::endl;
    std::cout << "  纯JavaScript行: " << result.pureJavaScriptLines << " 行" << std::endl;
    std::cout << "  违规项: " << result.violations.size() << " 个" << std::endl;
    
    if (!result.violations.empty()) {
        std::cout << "  违规详情:" << std::endl;
        for (const auto& violation : result.violations) {
            std::cout << "    - " << violation.message << std::endl;
        }
    }
}

int main() {
    try {
        std::cout << "🚀 CHTL Script块约束器测试" << std::endl;
        std::cout << "验证CHTL Script块的语法约束：禁止CHTL语法，允许--注释和原始嵌入\n" << std::endl;
        
        testValidScriptSyntax();
        testForbiddenScriptSyntax();
        testSpecificScriptConstraints();
        testMixedScriptContent();
        
        std::cout << "\n🎉 Script约束器测试完成!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "测试异常: " << e.what() << std::endl;
        return 1;
    }
}
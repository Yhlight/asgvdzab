#include "constraints/chtl_local_script_constraint.hpp"
#include "constraints/chtl_global_style_constraint.hpp"
#include "constraints/chtl_local_style_constraint.hpp"
#include "constraints/chtl_script_constraint.hpp"
#include <iostream>

using namespace chtl;

void testEnhancedRawEmbedding() {
    std::cout << "=== 增强原始嵌入功能测试 ===" << std::endl;
    
    CHtlLocalScriptConstraint scriptConstraint;
    CHtlGlobalStyleConstraint styleConstraint;
    
    // 测试基本类型
    std::cout << "\n基本类型原始嵌入验证:" << std::endl;
    std::cout << "HTML基本 '[Origin] @Html {': " 
              << (scriptConstraint.validateRawEmbedding("[Origin] @Html {") ? "✓" : "✗") << std::endl;
    std::cout << "CSS基本 '[Origin] @Style {': " 
              << (scriptConstraint.validateRawEmbedding("[Origin] @Style {") ? "✓" : "✗") << std::endl;
    std::cout << "JS基本 '[Origin] @JavaScript {': " 
              << (scriptConstraint.validateRawEmbedding("[Origin] @JavaScript {") ? "✓" : "✗") << std::endl;
    
    // 测试带名原始嵌入
    std::cout << "\n带名原始嵌入验证:" << std::endl;
    std::cout << "HTML带名 '[Origin] @Html box {': " 
              << (scriptConstraint.validateRawEmbedding("[Origin] @Html box {") ? "✓" : "✗") << std::endl;
    std::cout << "CSS带名 '[Origin] @Style header {': " 
              << (scriptConstraint.validateRawEmbedding("[Origin] @Style header {") ? "✓" : "✗") << std::endl;
    std::cout << "JS带名 '[Origin] @JavaScript utils {': " 
              << (scriptConstraint.validateRawEmbedding("[Origin] @JavaScript utils {") ? "✓" : "✗") << std::endl;
    
    // 测试自定义类型
    std::cout << "\n自定义类型原始嵌入验证:" << std::endl;
    std::cout << "Vue类型 '[Origin] @Vue {': " 
              << (scriptConstraint.validateRawEmbedding("[Origin] @Vue {") ? "✓" : "✗") << std::endl;
    std::cout << "Markdown类型 '[Origin] @Markdown {': " 
              << (scriptConstraint.validateRawEmbedding("[Origin] @Markdown {") ? "✓" : "✗") << std::endl;
    std::cout << "React类型 '[Origin] @React {': " 
              << (scriptConstraint.validateRawEmbedding("[Origin] @React {") ? "✓" : "✗") << std::endl;
    
    // 测试自定义类型带名
    std::cout << "\n自定义类型带名原始嵌入验证:" << std::endl;
    std::cout << "Vue带名 '[Origin] @Vue component {': " 
              << (scriptConstraint.validateRawEmbedding("[Origin] @Vue component {") ? "✓" : "✗") << std::endl;
    std::cout << "Markdown带名 '[Origin] @Markdown docs {': " 
              << (scriptConstraint.validateRawEmbedding("[Origin] @Markdown docs {") ? "✓" : "✗") << std::endl;
    std::cout << "React带名 '[Origin] @React widgets {': " 
              << (scriptConstraint.validateRawEmbedding("[Origin] @React widgets {") ? "✓" : "✗") << std::endl;
    
    // 测试无效格式
    std::cout << "\n无效格式验证:" << std::endl;
    std::cout << "缺少@前缀 '[Origin] Html {': " 
              << (scriptConstraint.validateRawEmbedding("[Origin] Html {") ? "✗" : "✓") << std::endl;
    std::cout << "无效类型名 '[Origin] @123 {': " 
              << (scriptConstraint.validateRawEmbedding("[Origin] @123 {") ? "✗" : "✓") << std::endl;
    std::cout << "缺少Origin '[Origin @Html {': " 
              << (scriptConstraint.validateRawEmbedding("[Origin @Html {") ? "✗" : "✓") << std::endl;
}

void testEnhancedRawEmbeddingInBlocks() {
    std::cout << "\n=== 增强原始嵌入在约束块中的应用测试 ===" << std::endl;
    
    CHtlLocalScriptConstraint scriptConstraint;
    
    // 测试局部script中使用增强原始嵌入
    std::string scriptWithEnhancedRawEmbedding = R"(
        // 基本类型原始嵌入
        [Origin] @Html {
            <div>Basic HTML</div>
        }
        
        [Origin] @Style {
            .basic-style { color: red; }
        }
        
        [Origin] @JavaScript {
            console.log('Basic JS');
        }
        
        // 带名原始嵌入
        [Origin] @Html headerSection {
            <header>Named HTML block</header>
        }
        
        [Origin] @Style navStyles {
            .nav { background: blue; }
        }
        
        [Origin] @JavaScript utilities {
            function util() { return true; }
        }
        
        // 自定义类型原始嵌入
        [Origin] @Vue {
            <template>
                <div>Vue component</div>
            </template>
        }
        
        [Origin] @React {
            const Component = () => <div>React component</div>;
        }
        
        [Origin] @Markdown {
            # Markdown Content
            This is a markdown block.
        }
        
        // 自定义类型带名原始嵌入
        [Origin] @Vue userCard {
            <template>
                <div class="user-card">{{ user.name }}</div>
            </template>
        }
        
        [Origin] @React dashboard {
            const Dashboard = () => (
                <div>React Dashboard</div>
            );
        }
        
        [Origin] @Svelte widget {
            <script>
                let count = 0;
            </script>
            <div>{count}</div>
        }
        
        // 原始嵌入引用
        [Origin] @Html headerSection;
        [Origin] @Vue userCard;
        [Origin] @React dashboard;
        
        // 正常的CHTL语法应该继续工作
        const themeColor = ThemeColor(primary);
        ConfigData from config/settings.chtl;
        
        -- 生成器注释：增强原始嵌入测试
        
        // JavaScript模板语法应该被保护
        const userName = {{user.name}};
        const content = {{{htmlContent}}};
    )";
    
    auto result = scriptConstraint.validateCHtlLocalScriptBlock(scriptWithEnhancedRawEmbedding);
    
    std::cout << "增强原始嵌入块测试: " << (result.isValid ? "✓ 通过" : "✗ 失败") << std::endl;
    if (!result.isValid) {
        for (const auto& violation : result.violations) {
            std::cout << "  错误: " << violation.message << " (" << violation.element << ")" << std::endl;
        }
    }
    
    std::cout << "\n统计信息:" << std::endl;
    std::cout << "  原始嵌入: " << result.foundRawEmbeddings.size() << " 个" << std::endl;
    std::cout << "  模板变量: " << result.templateVariables.size() << " 个" << std::endl;
    std::cout << "  from子句: " << result.fromClauses.size() << " 个" << std::endl;
    std::cout << "  生成器注释: " << result.foundGeneratorComments.size() << " 个" << std::endl;
    std::cout << "  JavaScript模板语法: " << result.jsTemplateSyntax.size() << " 个" << std::endl;
    
    std::cout << "\n发现的原始嵌入类型:" << std::endl;
    for (const auto& embedding : result.foundRawEmbeddings) {
        std::cout << "  - " << embedding << std::endl;
    }
}

void testRawEmbeddingDetection() {
    std::cout << "\n=== 原始嵌入检测测试 ===" << std::endl;
    
    CHtlLocalScriptConstraint constraint;
    
    // 测试各种原始嵌入检测
    std::vector<std::string> testCases = {
        "[Origin] @Html",
        "[Origin] @Style",
        "[Origin] @JavaScript", 
        "[Origin] @Vue",
        "[Origin] @React",
        "[Origin] @Markdown",
        "[Origin] @Html box",
        "[Origin] @Style header",
        "[Origin] @JavaScript utils",
        "[Origin] @Vue component",
        "[Origin] @React dashboard",
        "[Origin] @Svelte widget",
        "[Origin] @CustomType myName",
        "[Origin] @A1B2C3 test123",
        // 无效格式
        "[Origin] Html",           // 缺少@
        "[Origin] @123",           // 数字开头
        "Origin @Html",            // 缺少方括号
        "[Origin] @",              // 缺少类型名
        "[Origin] @Html-box",      // 无效名称格式
    };
    
    std::cout << "\n原始嵌入检测结果:" << std::endl;
    for (const auto& testCase : testCases) {
        auto allowedTypes = constraint.checkAllowedElement(testCase);
        bool isRawEmbedding = false;
        for (auto type : allowedTypes) {
            if (type == LocalScriptAllowedElement::RAW_EMBEDDING) {
                isRawEmbedding = true;
                break;
            }
        }
        std::cout << "'" << testCase << "': " << (isRawEmbedding ? "✓ 识别" : "✗ 未识别") << std::endl;
    }
}

int main() {
    try {
        std::cout << "🚀 CHTL增强原始嵌入功能测试" << std::endl;
        std::cout << "验证基本类型、自定义类型、带名原始嵌入的支持\n" << std::endl;
        
        testEnhancedRawEmbedding();
        testEnhancedRawEmbeddingInBlocks();
        testRawEmbeddingDetection();
        
        std::cout << "\n🎉 增强原始嵌入测试完成!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "测试异常: " << e.what() << std::endl;
        return 1;
    }
}
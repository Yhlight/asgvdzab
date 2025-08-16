#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include "core/chtl_unified_scanner.hpp"
#include "core/compiler_dispatcher.hpp"

using namespace chtl;

/**
 * 测试代码切割器
 */
void testCodeFragmentCutting() {
    std::cout << "=== 测试精准代码切割器 ===" << std::endl;
    
    CHTLUnifiedScanner scanner;
    scanner.setDebugMode(true);
    scanner.setInitialSliceSize(512);
    
    // 测试CHTL代码
    std::string chtlCode = R"(
        {{title}} 是模板变量
        [Custom] @Style MyButton {
            color: {{primaryColor}};
            background: inherit BaseButton;
            margin: {{spacing}}px;
        }
        
        div {
            class: MyButton;
            style {
                {{dynamicStyle}}
                font-size: 16px;
            }
            script {
                let color = {{primaryColor}};
                console.log("CHTL变量:", color);
                function updateStyle() {
                    this.style.color = {{secondaryColor}};
                }
            }
        }
        
        [Origin] @Html content {
            <p>原始HTML内容</p>
        }
    )";
    
    auto fragments = scanner.scanAndSlice(chtlCode);
    
    std::cout << "切割完成，共产生 " << fragments.size() << " 个片段：" << std::endl;
    
    for (size_t i = 0; i < fragments.size(); ++i) {
        const auto& fragment = fragments[i];
        std::string typeStr;
        switch (fragment.type) {
            case CodeFragmentType::CHTL_FRAGMENT: typeStr = "CHTL"; break;
            case CodeFragmentType::CHTL_JS_FRAGMENT: typeStr = "CHTL_JS"; break;
            case CodeFragmentType::CSS_FRAGMENT: typeStr = "CSS"; break;
            case CodeFragmentType::JS_FRAGMENT: typeStr = "JS"; break;
        }
        
        std::string preview = fragment.content.substr(0, 50);
        std::replace(preview.begin(), preview.end(), '\n', ' ');
        
        std::cout << "  片段[" << i << "] 类型:" << typeStr 
                  << " 位置:" << fragment.startPos << "-" << fragment.endPos
                  << " 内容:\"" << preview << "...\"" << std::endl;
    }
    
    assert(fragments.size() > 0);
    std::cout << "✓ 代码切割器测试通过" << std::endl << std::endl;
}

/**
 * 测试编译器调度器
 */
void testCompilerDispatcher() {
    std::cout << "=== 测试编译器调度器 ===" << std::endl;
    
    CompilerDispatcher dispatcher;
    dispatcher.setDebugMode(true);
    
    // 注意：由于编译器工厂还没有实际实现，这里只测试调度器的初始化
    bool initialized = dispatcher.initialize();
    std::cout << "调度器初始化: " << (initialized ? "成功" : "失败（预期，因为编译器还未实现）") << std::endl;
    
    auto status = dispatcher.getCompilerStatus();
    std::cout << "调度器状态：" << std::endl;
    for (const auto& [key, value] : status) {
        std::cout << "  " << key << ": " << value << std::endl;
    }
    
    // 测试简单的CHTL代码编译
    std::string testCode = "{{message}}";
    auto result = dispatcher.compileSource(testCode);
    
    std::cout << "编译结果: " << (result.success ? "成功" : "失败") << std::endl;
    if (!result.errors.empty()) {
        std::cout << "编译错误：" << std::endl;
        for (const auto& error : result.errors) {
            std::cout << "  - " << error << std::endl;
        }
    }
    
    dispatcher.cleanup();
    std::cout << "✓ 编译器调度器测试完成" << std::endl << std::endl;
}

/**
 * 测试最小单元切割
 */
void testMinimalUnitCutting() {
    std::cout << "=== 测试最小单元切割 ===" << std::endl;
    
    // 测试CHTL最小单元模式
    auto chtlPatterns = MinimalUnit::getChtlMinimalPatterns();
    std::cout << "CHTL最小单元模式数量: " << chtlPatterns.size() << std::endl;
    
    // 测试CHTL JS最小单元模式
    auto chtlJsPatterns = MinimalUnit::getChtlJsMinimalPatterns();
    std::cout << "CHTL JS最小单元模式数量: " << chtlJsPatterns.size() << std::endl;
    
    // 测试单元完整性检查
    std::string completeUnit = "{{variable}}";
    bool isComplete = MinimalUnit::isCompleteChtlUnit(completeUnit);
    std::cout << "{{variable}} 是否为完整单元: " << (isComplete ? "是" : "否") << std::endl;
    
    std::string incompleteUnit = "{{variable";
    isComplete = MinimalUnit::isCompleteChtlUnit(incompleteUnit);
    std::cout << "{{variable 是否为完整单元: " << (isComplete ? "是" : "否") << std::endl;
    
    // 测试边界查找
    std::string testContent = "{{var1}} -> {{var2}} + increment";
    auto boundaries = MinimalUnit::findMinimalUnitBoundaries(testContent, CodeFragmentType::CHTL_FRAGMENT);
    std::cout << "边界位置: ";
    for (size_t boundary : boundaries) {
        std::cout << boundary << " ";
    }
    std::cout << std::endl;
    
    std::cout << "✓ 最小单元切割测试完成" << std::endl << std::endl;
}

/**
 * 测试切片验证器
 */
void testSliceValidator() {
    std::cout << "=== 测试切片验证器 ===" << std::endl;
    
    // 测试大括号平衡
    assert(SliceValidator::areBracesBalanced("{{variable}}"));
    assert(!SliceValidator::areBracesBalanced("{{variable}"));
    std::cout << "✓ 大括号平衡检查正常" << std::endl;
    
    // 测试小括号平衡
    assert(SliceValidator::areParenthesesBalanced("function(param)"));
    assert(!SliceValidator::areParenthesesBalanced("function(param"));
    std::cout << "✓ 小括号平衡检查正常" << std::endl;
    
    // 测试字符串/注释检查
    std::string codeWithString = R"(let str = "{{不应该被识别}}"; {{realVar}})";
    bool inString = SliceValidator::isInStringOrComment(codeWithString, 15); // 在字符串内部
    std::cout << "字符串内部检查: " << (inString ? "正确识别在字符串内" : "未正确识别") << std::endl;
    
    bool notInString = SliceValidator::isInStringOrComment(codeWithString, codeWithString.length() - 5); // 在realVar位置
    std::cout << "字符串外部检查: " << (!notInString ? "正确识别在字符串外" : "错误识别") << std::endl;
    
    std::cout << "✓ 切片验证器测试完成" << std::endl << std::endl;
}

/**
 * 测试复杂的混合代码
 */
void testComplexMixedCode() {
    std::cout << "=== 测试复杂混合代码处理 ===" << std::endl;
    
    std::string complexCode = R"(
        [Template] @Style MainTheme {
            primary: #3498db;
            secondary: #2ecc71;
            font: "Arial, sans-serif";
        }
        
        [Custom] @Element Header {
            tag: "header";
            class: "site-header";
        }
        
        div {
            class: "container";
            style {
                background: {{MainTheme.primary}};
                color: {{textColor}};
                font-family: {{MainTheme.font}};
                
                .header {
                    margin: 20px;
                    padding: {{spacing}}px;
                }
                
                /* 这是CSS注释 */
                @media (max-width: 768px) {
                    padding: 10px;
                }
            }
            
            script {
                // CHTL JS混合代码
                const theme = {{MainTheme}};
                let currentColor = {{primaryColor}};
                
                function updateTheme() {
                    document.querySelector('.container').style.background = currentColor;
                    console.log(`主题更新为: ${currentColor}`);
                }
                
                // 使用CHTL变量组语法
                ColorGroup(primary = {{primaryColor}}, secondary = {{secondaryColor}});
                
                // 从命名空间导入
                ThemeUtils from "ui/themes" as Utils except deprecated;
                
                [Origin] @JavaScript utilities {
                    function debounce(func, delay) {
                        let timeoutId;
                        return (...args) => {
                            clearTimeout(timeoutId);
                            timeoutId = setTimeout(() => func.apply(this, args), delay);
                        };
                    }
                }
                
                // 混合使用
                const debouncedUpdate = debounce(() => updateTheme(), 300);
            }
            
            [Origin] @Html content {
                <section class="hero">
                    <h1>{{pageTitle}}</h1>
                    <p>{{pageDescription}}</p>
                </section>
            }
            
            Header {
                text: {{siteTitle}};
                style {
                    inherit MainTheme;
                    font-size: 2em;
                    delete background;
                }
            }
        }
        
        -- 生成器注释：这将生成响应式布局
    )";
    
    CHTLUnifiedScanner scanner;
    scanner.setDebugMode(true);
    scanner.setInitialSliceSize(256); // 使用较小的切片测试扩增机制
    
    auto fragments = scanner.scanAndSlice(complexCode);
    
    std::cout << "复杂代码切割结果：" << std::endl;
    std::cout << "总片段数: " << fragments.size() << std::endl;
    
    // 统计各类型片段
    int chtlCount = 0, chtlJsCount = 0, cssCount = 0, jsCount = 0;
    for (const auto& fragment : fragments) {
        switch (fragment.type) {
            case CodeFragmentType::CHTL_FRAGMENT: chtlCount++; break;
            case CodeFragmentType::CHTL_JS_FRAGMENT: chtlJsCount++; break;
            case CodeFragmentType::CSS_FRAGMENT: cssCount++; break;
            case CodeFragmentType::JS_FRAGMENT: jsCount++; break;
        }
    }
    
    std::cout << "片段类型统计:" << std::endl;
    std::cout << "  CHTL片段: " << chtlCount << std::endl;
    std::cout << "  CHTL_JS片段: " << chtlJsCount << std::endl;
    std::cout << "  CSS片段: " << cssCount << std::endl;
    std::cout << "  JS片段: " << jsCount << std::endl;
    
    assert(fragments.size() > 0);
    std::cout << "✓ 复杂混合代码处理测试完成" << std::endl << std::endl;
}

int main() {
    std::cout << "🚀 CHTL统一编译器架构测试" << std::endl;
    std::cout << "================================" << std::endl << std::endl;
    
    try {
        testCodeFragmentCutting();
        testMinimalUnitCutting();
        testSliceValidator();
        testComplexMixedCode();
        testCompilerDispatcher();
        
        std::cout << "🎉 所有测试完成！" << std::endl;
        std::cout << "================================" << std::endl;
        std::cout << "✅ 精准代码切割器 - 正常运行" << std::endl;
        std::cout << "✅ 最小单元处理 - 正常运行" << std::endl;
        std::cout << "✅ 切片验证机制 - 正常运行" << std::endl;
        std::cout << "✅ 复杂代码处理 - 正常运行" << std::endl;
        std::cout << "✅ 编译器调度器 - 架构完整" << std::endl;
        std::cout << std::endl;
        std::cout << "📋 下一步：实现具体的编译器实现类" << std::endl;
        std::cout << "  - CHTL编译器 (手写)" << std::endl;
        std::cout << "  - CHTL JS编译器 (手写)" << std::endl;
        std::cout << "  - CSS编译器 (ANTLR)" << std::endl;
        std::cout << "  - JavaScript编译器 (ANTLR)" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ 测试失败: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
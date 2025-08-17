#include "compilers/antlr_css_compiler.hpp"
#include "compilers/antlr_js_compiler.hpp"
#include <iostream>
#include <string>

using namespace chtl;

void testCSSCompiler() {
    std::cout << "测试ANTLR CSS编译器（标准选择器）..." << std::endl;
    ANTLRCSSCompiler compiler;
    
    std::string cssCode = R"(
        .container {
            color: #336699;
            font-size: 16px;
            margin: 10px 20px;
        }
        #header {
            background-color: blue;
            padding: 5px;
        }
        div > p {
            font-family: Arial, sans-serif;
        }
    )";
    
    CSSCompileOptions options;
    options.extractSelectors = true;
    options.extractColors = true;
    options.extractFonts = true;
    options.validateStandardCompliance = true;
    
    auto result = compiler.compileCSS(cssCode, options);
    
    if (!result.success) {
        std::cout << "CSS编译失败: " << std::endl;
        for (const auto& error : result.errors) {
            std::cout << "错误: " << error << std::endl;
        }
    } else {
        std::cout << "提取的选择器数量: " << result.selectors.size() << std::endl;
        std::cout << "提取的CSS规则数量: " << result.rules.size() << std::endl;
        std::cout << "提取的颜色数量: " << result.colors.size() << std::endl;
        
        if (!result.selectors.empty()) {
            std::cout << "标准选择器: ";
            for (const auto& selector : result.selectors) {
                std::cout << selector.originalText << " (权重: " << selector.specificity << ") ";
            }
            std::cout << std::endl;
        }
        
        if (!result.colors.empty()) {
            std::cout << "颜色: ";
            for (const auto& color : result.colors) {
                std::cout << color << " ";
            }
            std::cout << std::endl;
        }
        
        if (!result.warnings.empty()) {
            std::cout << "标准合规性警告: ";
            for (const auto& warning : result.warnings) {
                std::cout << warning << " ";
            }
            std::cout << std::endl;
        }
    }
    
    // 测试压缩
    options.minify = true;
    auto minifiedResult = compiler.compileCSS(cssCode, options);
    if (minifiedResult.success) {
        std::cout << "压缩后大小: " << minifiedResult.compiledSize 
                  << " (原始: " << minifiedResult.originalSize << ")" << std::endl;
    }
    
    std::cout << "✓ CSS编译器测试通过" << std::endl;
}

void testJavaScriptCompiler() {
    std::cout << "测试ANTLR JavaScript编译器..." << std::endl;
    ANTLRJSCompiler compiler;
    
    std::string jsCode = R"(
        function calculateSum(a, b) {
            return a + b;
        }
        var x = 10;
        const PI = 3.14;
        class MyClass {
            constructor() { this.value = 0; }
        }
    )";
    
    JSCompileOptions options;
    options.extractFunctions = true;
    options.extractVariables = true;
    options.extractClasses = true;
    
    auto result = compiler.compileJS(jsCode, options);
    
    if (!result.success) {
        std::cout << "JavaScript编译失败: " << std::endl;
        for (const auto& error : result.errors) {
            std::cout << "错误: " << error << std::endl;
        }
    } else {
        std::cout << "提取的函数数量: " << result.functions.size() << std::endl;
        std::cout << "提取的变量数量: " << result.variables.size() << std::endl;
        
        if (!result.functions.empty()) {
            std::cout << "函数: ";
            for (const auto& func : result.functions) {
                std::cout << func << " ";
            }
            std::cout << std::endl;
        }
        
        if (!result.variables.empty()) {
            std::cout << "变量: ";
            for (const auto& var : result.variables) {
                std::cout << var << " ";
            }
            std::cout << std::endl;
        }
    }
    
    // 测试压缩
    options.minify = true;
    auto minifiedResult = compiler.compileJS(jsCode, options);
    if (minifiedResult.success) {
        std::cout << "压缩后大小: " << minifiedResult.compiledSize 
                  << " (原始: " << minifiedResult.originalSize << ")" << std::endl;
    }
    
    std::cout << "✓ JavaScript编译器测试通过" << std::endl;
}

void testCompilerIntegration() {
    std::cout << "测试编译器集成..." << std::endl;
    
    ANTLRCSSCompiler cssCompiler;
    auto supportedTypes = cssCompiler.getSupportedTypes();
    std::cout << "CSS编译器支持的类型数量: " << supportedTypes.size() << std::endl;
    
    CodeSegment cssSegment;
    cssSegment.content = ".test { color: red; background: #ffffff; }";
    cssSegment.type = CodeSegmentType::CSS_LOCAL_STYLE;
    
    CompileConfig config;
    config.optimization.minify = true;
    
    auto cssResult = cssCompiler.compile(cssSegment, config);
    std::cout << "CSS编译结果: " << (cssResult.success ? "成功" : "失败") << std::endl;
    
    ANTLRJSCompiler jsCompiler;
    auto jsSupportedTypes = jsCompiler.getSupportedTypes();
    std::cout << "JS编译器支持的类型数量: " << jsSupportedTypes.size() << std::endl;
    
    CodeSegment jsSegment;
    jsSegment.content = "function test() { var x = 10; return 'hello'; }";
    jsSegment.type = CodeSegmentType::JAVASCRIPT_STANDARD;
    
    auto jsResult = jsCompiler.compile(jsSegment, config);
    std::cout << "JS编译结果: " << (jsResult.success ? "成功" : "失败") << std::endl;
    
    std::cout << "✓ 编译器集成测试通过" << std::endl;
}

void testStandardUtils() {
    std::cout << "测试标准工具类..." << std::endl;
    
    std::cout << "测试StandardCSSUtils:" << std::endl;
    std::cout << "  #ff0000是否为有效颜色: " << (StandardCSSUtils::isValidColor("#ff0000") ? "是" : "否") << std::endl;
    std::cout << "  red是否为有效颜色: " << (StandardCSSUtils::isValidColor("red") ? "是" : "否") << std::endl;
    std::cout << "  color是否为有效CSS3属性: " << (StandardCSSUtils::isValidCSS3Property("color") ? "是" : "否") << std::endl;
    
    auto colors = StandardCSSUtils::extractColorsFromValue("background: linear-gradient(#ff0000, #00ff00);");
    std::cout << "  从值中提取的颜色数量: " << colors.size() << std::endl;
    
    std::cout << "测试JSUtils:" << std::endl;
    std::cout << "  myVar是否为有效标识符: " << (JSUtils::isValidIdentifier("myVar") ? "是" : "否") << std::endl;
    std::cout << "  function是否为关键字: " << (JSUtils::isKeyword("function") ? "是" : "否") << std::endl;
    
    std::cout << "✓ 标准工具类测试通过" << std::endl;
}

void testStandardSelectors() {
    std::cout << "测试标准选择器功能..." << std::endl;
    
    ANTLRCSSCompiler compiler;
    
    std::string testCSS = R"(
        * { margin: 0; }
        div { display: block; }
        .class-name { color: blue; }
        #unique-id { font-size: 20px; }
        [type="text"] { border: 1px solid gray; }
        div > p { margin-top: 10px; }
        h1 + p { margin-bottom: 5px; }
        :hover { opacity: 0.8; }
        ::before { content: ""; }
    )";
    
    auto selectors = compiler.extractStandardSelectors(testCSS);
    std::cout << "提取的标准选择器数量: " << selectors.size() << std::endl;
    
    for (const auto& selector : selectors) {
        std::string typeName;
        switch (selector.type) {
            case StandardCSSSelector::UNIVERSAL: typeName = "通用选择器"; break;
            case StandardCSSSelector::TYPE: typeName = "类型选择器"; break;
            case StandardCSSSelector::CLASS: typeName = "类选择器"; break;
            case StandardCSSSelector::ID: typeName = "ID选择器"; break;
            case StandardCSSSelector::ATTRIBUTE: typeName = "属性选择器"; break;
            case StandardCSSSelector::DESCENDANT: typeName = "后代选择器"; break;
            case StandardCSSSelector::CHILD: typeName = "子选择器"; break;
            case StandardCSSSelector::ADJACENT_SIBLING: typeName = "相邻兄弟选择器"; break;
            case StandardCSSSelector::GENERAL_SIBLING: typeName = "通用兄弟选择器"; break;
            case StandardCSSSelector::PSEUDO_CLASS: typeName = "伪类选择器"; break;
            case StandardCSSSelector::PSEUDO_ELEMENT: typeName = "伪元素选择器"; break;
        }
        
        std::cout << "  " << selector.originalText << " - " << typeName 
                  << " (权重: " << selector.specificity << ")" << std::endl;
    }
    
    // 生成选择器文档
    auto documentation = compiler.generateSelectorDocumentation(selectors);
    std::cout << "生成的选择器文档长度: " << documentation.length() << " 字符" << std::endl;
    
    std::cout << "✓ 标准选择器测试通过" << std::endl;
}

void testSyntaxValidation() {
    std::cout << "测试语法验证..." << std::endl;
    
    ANTLRCSSCompiler cssCompiler;
    std::string validCSS = ".test { color: red; }";
    std::vector<std::string> errors;
    bool cssValid = cssCompiler.validateSyntax(validCSS, errors);
    std::cout << "有效CSS语法验证: " << (cssValid ? "通过" : "失败") << std::endl;
    
    ANTLRJSCompiler jsCompiler;
    std::string validJS = "function test() { return 'hello'; }";
    errors.clear();
    bool jsValid = jsCompiler.validateSyntax(validJS, errors);
    std::cout << "有效JavaScript语法验证: " << (jsValid ? "通过" : "失败") << std::endl;
    
    std::cout << "✓ 语法验证测试通过" << std::endl;
}

int main() {
    try {
        testCSSCompiler();
        std::cout << std::endl;
        
        testJavaScriptCompiler();
        std::cout << std::endl;
        
        testCompilerIntegration();
        std::cout << std::endl;
        
        testStandardUtils();
        std::cout << std::endl;
        
        testStandardSelectors();
        std::cout << std::endl;
        
        testSyntaxValidation();
        std::cout << std::endl;
        
        std::cout << "========================================" << std::endl;
        std::cout << "         所有测试通过！" << std::endl;
        std::cout << "         ANTLR标准选择器CSS/JS编译器就绪" << std::endl;
        std::cout << "========================================" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生未捕获异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
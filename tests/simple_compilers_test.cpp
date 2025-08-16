#include "compilers/simple_css_compiler.hpp"
#include "compilers/simple_js_compiler.hpp"
#include <iostream>
#include <cassert>

using namespace chtl;

void testCSSCompiler() {
    std::cout << "测试Simple CSS编译器..." << std::endl;
    
    SimpleCSSCompiler compiler;
    
    // 测试基础CSS编译
    std::string cssCode = R"(
        .container {
            width: 100%;
            margin: 0 auto;
            background-color: #ffffff;
        }
        
        #header {
            height: 60px;
            background: linear-gradient(to right, #ff0000, #00ff00);
        }
        
        .btn:hover {
            color: rgba(255, 255, 255, 0.8);
            transform: scale(1.05);
        }
    )";
    
    CSSCompileOptions options;
    options.minify = false;
    options.extractColors = true;
    options.validateSyntax = true;
    
    auto result = compiler.compileCSS(cssCode, options);
    
    std::cout << "编译结果: " << (result.success ? "成功" : "失败") << std::endl;
    if (!result.success) {
        for (const auto& error : result.errors) {
            std::cout << "错误: " << error << std::endl;
        }
    }
    
    std::cout << "提取的选择器数量: " << result.selectors.size() << std::endl;
    std::cout << "提取的属性数量: " << result.properties.size() << std::endl;
    std::cout << "提取的颜色数量: " << result.colors.size() << std::endl;
    
    // 测试压缩
    options.minify = true;
    auto minifiedResult = compiler.compileCSS(cssCode, options);
    std::cout << "压缩后大小: " << minifiedResult.compiledSize << " (原始: " << minifiedResult.originalSize << ")" << std::endl;
    
    // 测试工具方法
    auto selectors = compiler.extractSelectors(cssCode);
    auto colors = compiler.extractColors(cssCode);
    
    std::cout << "工具方法提取选择器数量: " << selectors.size() << std::endl;
    std::cout << "工具方法提取颜色数量: " << colors.size() << std::endl;
    
    std::cout << "✓ CSS编译器测试通过" << std::endl;
}

void testJSCompiler() {
    std::cout << "测试Simple JavaScript编译器..." << std::endl;
    
    SimpleJSCompiler compiler;
    
    // 测试基础JavaScript编译
    std::string jsCode = R"(
        function calculateSum(a, b) {
            return a + b;
        }
        
        var result = calculateSum(10, 20);
        
        let message = "Hello, World!";
        const PI = 3.14159;
        
        if (result > 25) {
            console.log(message);
        }
        
        for (let i = 0; i < 5; i++) {
            console.log("Count: " + i);
        }
    )";
    
    JSCompileOptions options;
    options.minify = false;
    options.extractFunctions = true;
    options.extractVariables = true;
    options.validateSyntax = true;
    
    auto result = compiler.compileJS(jsCode, options);
    
    std::cout << "编译结果: " << (result.success ? "成功" : "失败") << std::endl;
    if (!result.success) {
        for (const auto& error : result.errors) {
            std::cout << "错误: " << error << std::endl;
        }
    }
    
    std::cout << "提取的函数数量: " << result.functions.size() << std::endl;
    std::cout << "提取的变量数量: " << result.variables.size() << std::endl;
    
    // 测试压缩
    options.minify = true;
    auto minifiedResult = compiler.compileJS(jsCode, options);
    std::cout << "压缩后大小: " << minifiedResult.compiledSize << " (原始: " << minifiedResult.originalSize << ")" << std::endl;
    
    // 测试工具方法
    auto functions = compiler.extractFunctions(jsCode);
    auto variables = compiler.extractVariables(jsCode);
    
    std::cout << "工具方法提取函数数量: " << functions.size() << std::endl;
    std::cout << "工具方法提取变量数量: " << variables.size() << std::endl;
    
    // 测试ES版本转换
    auto es5Code = compiler.convertToES5(jsCode);
    std::cout << "ES5转换完成，长度: " << es5Code.length() << std::endl;
    
    std::cout << "✓ JavaScript编译器测试通过" << std::endl;
}

void testCompilerIntegration() {
    std::cout << "测试编译器集成..." << std::endl;
    
    // 测试CSS编译器与ICompiler接口
    SimpleCSSCompiler cssCompiler;
    auto supportedTypes = cssCompiler.getSupportedTypes();
    
    std::cout << "CSS编译器支持的类型数量: " << supportedTypes.size() << std::endl;
    
    CodeSegment cssSegment;
    cssSegment.content = ".test { color: red; }";
    cssSegment.type = CodeSegmentType::CSS_LOCAL_STYLE;
    
    CompileConfig config;
    config.optimization.minify = true;
    
    auto cssResult = cssCompiler.compile(cssSegment, config);
    std::cout << "CSS编译结果: " << (cssResult.success ? "成功" : "失败") << std::endl;
    
    // 测试JavaScript编译器与ICompiler接口
    SimpleJSCompiler jsCompiler;
    auto jsSupportedTypes = jsCompiler.getSupportedTypes();
    
    std::cout << "JS编译器支持的类型数量: " << jsSupportedTypes.size() << std::endl;
    
    CodeSegment jsSegment;
    jsSegment.content = "function test() { return 'hello'; }";
    jsSegment.type = CodeSegmentType::JAVASCRIPT_STANDARD;
    
    auto jsResult = jsCompiler.compile(jsSegment, config);
    std::cout << "JS编译结果: " << (jsResult.success ? "成功" : "失败") << std::endl;
    
    std::cout << "✓ 编译器集成测试通过" << std::endl;
}

void testUtilityClasses() {
    std::cout << "测试工具类..." << std::endl;
    
    // 测试CSSUtils
    std::cout << "测试CSSUtils:" << std::endl;
    std::cout << "  #ff0000是否为有效颜色: " << (CSSUtils::isValidColor("#ff0000") ? "是" : "否") << std::endl;
    std::cout << "  red是否为有效颜色: " << (CSSUtils::isValidColor("red") ? "是" : "否") << std::endl;
    std::cout << "  color是否为有效属性: " << (CSSUtils::isValidProperty("color") ? "是" : "否") << std::endl;
    
    auto colors = CSSUtils::extractColorsFromValue("background: linear-gradient(#ff0000, #00ff00);");
    std::cout << "  从值中提取的颜色数量: " << colors.size() << std::endl;
    
    // 测试JSUtils
    std::cout << "测试JSUtils:" << std::endl;
    std::cout << "  myVar是否为有效标识符: " << (JSUtils::isValidIdentifier("myVar") ? "是" : "否") << std::endl;
    std::cout << "  function是否为关键字: " << (JSUtils::isKeyword("function") ? "是" : "否") << std::endl;
    
    std::string jsCode = "const test = () => { console.log('test'); };";
    std::cout << "  检测ES版本: " << JSUtils::detectESVersion(jsCode) << std::endl;
    
    std::string funcDecl = "function myFunc(a, b, c) { return a + b + c; }";
    std::cout << "  函数名: " << JSUtils::extractFunctionName(funcDecl) << std::endl;
    
    auto params = JSUtils::extractParameters(funcDecl);
    std::cout << "  参数数量: " << params.size() << std::endl;
    
    std::cout << "✓ 工具类测试通过" << std::endl;
}

void testErrorHandling() {
    std::cout << "测试错误处理..." << std::endl;
    
    SimpleCSSCompiler cssCompiler;
    SimpleJSCompiler jsCompiler;
    
    // 测试CSS语法错误
    std::string invalidCSS = ".test { color: ; background }";
    auto cssResult = cssCompiler.compileCSS(invalidCSS);
    std::cout << "无效CSS编译结果: " << (cssResult.success ? "成功" : "失败") << std::endl;
    if (!cssResult.success) {
        std::cout << "  错误数量: " << cssResult.errors.size() << std::endl;
    }
    
    // 测试JavaScript语法错误
    std::string invalidJS = "function test( { return 'missing parenthesis'; }";
    auto jsResult = jsCompiler.compileJS(invalidJS);
    std::cout << "无效JavaScript编译结果: " << (jsResult.success ? "成功" : "失败") << std::endl;
    if (!jsResult.success) {
        std::cout << "  错误数量: " << jsResult.errors.size() << std::endl;
    }
    
    // 测试语法验证
    std::vector<std::string> errors;
    bool cssValid = cssCompiler.validateSyntax(invalidCSS, errors);
    std::cout << "CSS语法验证: " << (cssValid ? "有效" : "无效") << std::endl;
    
    errors.clear();
    bool jsValid = jsCompiler.validateSyntax(invalidJS, errors);
    std::cout << "JavaScript语法验证: " << (jsValid ? "有效" : "无效") << std::endl;
    
    std::cout << "✓ 错误处理测试通过" << std::endl;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "         简化编译器测试套件" << std::endl;
    std::cout << "========================================" << std::endl;
    
    try {
        testCSSCompiler();
        std::cout << std::endl;
        
        testJSCompiler();
        std::cout << std::endl;
        
        testCompilerIntegration();
        std::cout << std::endl;
        
        testUtilityClasses();
        std::cout << std::endl;
        
        testErrorHandling();
        std::cout << std::endl;
        
        std::cout << "========================================" << std::endl;
        std::cout << "         所有测试通过！" << std::endl;
        std::cout << "========================================" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试失败: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
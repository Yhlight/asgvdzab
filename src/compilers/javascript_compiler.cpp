#include "compilers/javascript_compiler.hpp"
#include <sstream>
#include <algorithm>
#include <regex>
#include <cctype>

namespace chtl {

JavaScriptCompiler::JavaScriptCompiler() 
    : parserInitialized_(false), debugMode_(false), enableCHTLJSExtensions_(true) {
    
    // 初始化支持的JavaScript特性
    supportedJSFeatures_ = {
        "es6", "es7", "es8", "es9", "es10", "es11", "es12",
        "modules", "async-await", "arrow-functions", "destructuring",
        "spread-operator", "template-literals", "classes", "generators"
    };
}

CompileResult JavaScriptCompiler::compile(const CodeSegment& segment, const CompileConfig& config) {
    CompileResult result;
    result.success = false;
    result.compilerName = getName();
    result.compilerVersion = getVersion();
    
    // 检查是否支持该类型
    if (!supportsType(segment.type)) {
        result.errors.push_back("JavaScript编译器不支持代码片段类型: " + toString(segment.type));
        return result;
    }
    
    try {
        // 验证JavaScript语法
        if (!validateJavaScriptSyntax(segment.content)) {
            result.errors.insert(result.errors.end(), errors_.begin(), errors_.end());
            return result;
        }
        
        // 处理CHTL JS扩展语法（如果启用）
        std::string processedJS = segment.content;
        if (enableCHTLJSExtensions_) {
            processedJS = processCHTLJSExtensions(processedJS);
        }
        
        // 处理ES6+特性
        processedJS = processES6Features(processedJS);
        
        // 处理模块导入导出
        processedJS = processModules(processedJS);
        
        // 处理异步代码
        processedJS = processAsyncCode(processedJS);
        
        // 优化JavaScript代码
        processedJS = optimizeJavaScript(processedJS, config);
        
        // 生成JavaScript包装器
        processedJS = generateJavaScriptWrapper(processedJS, config);
        
        // 根据配置进行压缩或格式化
        if (config.minify) {
            processedJS = minifyJavaScript(processedJS);
        } else if (config.format) {
            processedJS = formatJavaScript(processedJS);
        }
        
        // 添加JavaScript polyfill（如果需要）
        if (config.addPolyfills) {
            processedJS = addJavaScriptPolyfills(processedJS);
        }
        
        // 检查兼容性
        auto compatibilityIssues = checkJavaScriptCompatibility(processedJS);
        result.warnings.insert(result.warnings.end(), compatibilityIssues.begin(), compatibilityIssues.end());
        
        // 生成统计信息
        auto stats = generateJavaScriptStatistics(processedJS);
        result.metadata["js_statements"] = std::to_string(stats["statements"]);
        result.metadata["js_functions"] = std::to_string(stats["functions"]);
        result.metadata["js_variables"] = std::to_string(stats["variables"]);
        result.metadata["js_size"] = std::to_string(processedJS.length());
        
        // 设置输出类型
        result.metadata["type"] = "js";
        result.output = processedJS;
        result.success = true;
        
    } catch (const std::exception& e) {
        result.errors.push_back("JavaScript编译过程中发生异常: " + std::string(e.what()));
    }
    
    return result;
}

std::string JavaScriptCompiler::getName() const {
    return "CHTL JavaScript Compiler";
}

std::string JavaScriptCompiler::getVersion() const {
    return "1.0.0";
}

bool JavaScriptCompiler::supportsType(CodeSegmentType type) const {
    return type == CodeSegmentType::JAVASCRIPT_STANDARD || 
           type == CodeSegmentType::CHTL_JS_SCRIPT;
}

std::vector<CodeSegmentType> JavaScriptCompiler::getSupportedTypes() const {
    return {CodeSegmentType::JAVASCRIPT_STANDARD, CodeSegmentType::CHTL_JS_SCRIPT};
}

std::shared_ptr<void> JavaScriptCompiler::parseJavaScript(const std::string& jsCode) {
    // 这里可以实现真正的JavaScript解析器
    // 目前返回一个空的智能指针作为占位符
    return std::shared_ptr<void>();
}

bool JavaScriptCompiler::validateJavaScriptSyntax(const std::string& jsCode) {
    errors_.clear();
    warnings_.clear();
    
    // 基本语法检查
    int braceCount = 0;
    int parenCount = 0;
    int bracketCount = 0;
    bool inString = false;
    bool inTemplate = false;
    char stringChar = 0;
    
    for (size_t i = 0; i < jsCode.length(); i++) {
        char c = jsCode[i];
        char prev = (i > 0) ? jsCode[i-1] : 0;
        char next = (i < jsCode.length() - 1) ? jsCode[i+1] : 0;
        
        // 处理字符串
        if ((c == '"' || c == '\'') && prev != '\\') {
            if (!inString && !inTemplate) {
                inString = true;
                stringChar = c;
            } else if (c == stringChar) {
                inString = false;
                stringChar = 0;
            }
        }
        
        // 处理模板字符串
        if (c == '`' && prev != '\\') {
            if (!inTemplate && !inString) {
                inTemplate = true;
            } else if (inTemplate) {
                inTemplate = false;
            }
        }
        
        if (inString || inTemplate) continue;
        
        // 检查大括号平衡
        if (c == '{') braceCount++;
        if (c == '}') braceCount--;
        
        // 检查小括号平衡
        if (c == '(') parenCount++;
        if (c == ')') parenCount--;
        
        // 检查方括号平衡
        if (c == '[') bracketCount++;
        if (c == ']') bracketCount--;
        
        // 检查各种括号是否小于0
        if (braceCount < 0) {
            errors_.push_back("JavaScript语法错误：意外的大括号 '}' 在第 " + std::to_string(i + 1) + " 个字符");
            return false;
        }
        
        if (parenCount < 0) {
            errors_.push_back("JavaScript语法错误：意外的小括号 ')' 在第 " + std::to_string(i + 1) + " 个字符");
            return false;
        }
        
        if (bracketCount < 0) {
            errors_.push_back("JavaScript语法错误：意外的方括号 ']' 在第 " + std::to_string(i + 1) + " 个字符");
            return false;
        }
    }
    
    // 检查各种括号是否平衡
    if (braceCount != 0) {
        errors_.push_back("JavaScript语法错误：大括号不平衡，缺少 " + std::to_string(braceCount) + " 个 '}'");
        return false;
    }
    
    if (parenCount != 0) {
        errors_.push_back("JavaScript语法错误：小括号不平衡，缺少 " + std::to_string(parenCount) + " 个 ')'");
        return false;
    }
    
    if (bracketCount != 0) {
        errors_.push_back("JavaScript语法错误：方括号不平衡，缺少 " + std::to_string(bracketCount) + " 个 ']'");
        return false;
    }
    
    // 检查基本的JavaScript语法规则
    std::regex functionPattern(R"(function\s+\w*\s*\([^)]*\)\s*\{)");
    std::regex arrowFunctionPattern(R"(\w*\s*=>\s*\{?)");
    std::regex variablePattern(R"(let|const|var)\s+\w+)");
    
    // 这里可以添加更多的语法检查
    
    return errors_.empty();
}

std::string JavaScriptCompiler::optimizeJavaScript(const std::string& jsCode, const CompileConfig& config) {
    std::string optimized = jsCode;
    
    // 移除注释（如果配置要求）
    if (config.removeComments) {
        // 移除单行注释
        std::regex singleLineCommentPattern(R"(\/\/.*$)");
        optimized = std::regex_replace(optimized, singleLineCommentPattern, "");
        
        // 移除多行注释
        std::regex multiLineCommentPattern(R"(\/\*[^*]*\*+(?:[^/*][^*]*\*+)*\/)");
        optimized = std::regex_replace(optimized, multiLineCommentPattern, "");
    }
    
    // 移除不必要的空白
    if (config.minify) {
        std::regex whitespacePattern(R"(\s+)");
        optimized = std::regex_replace(optimized, whitespacePattern, " ");
    }
    
    return optimized;
}

std::string JavaScriptCompiler::processES6Features(const std::string& jsCode) {
    std::string processed = jsCode;
    
    // 处理箭头函数
    std::regex arrowFunctionPattern(R"((\w+)\s*=>\s*\{)");
    processed = std::regex_replace(processed, arrowFunctionPattern, "function($1) {");
    
    // 处理模板字符串
    std::regex templateStringPattern(R"(`([^`]*)`)");
    processed = std::regex_replace(processed, templateStringPattern, "\"$1\"");
    
    // 这里可以添加更多的ES6特性处理
    
    return processed;
}

std::string JavaScriptCompiler::processModules(const std::string& jsCode) {
    std::string processed = jsCode;
    
    // 处理ES6模块导入
    std::regex importPattern(R"(import\s+(\{[^}]*\})\s+from\s+['"]([^'"]+)['"])");
    processed = std::regex_replace(processed, importPattern, "// ES6 import: $1 from '$2'");
    
    // 处理ES6模块导出
    std::regex exportPattern(R"(export\s+(default\s+)?(?:function|class|const|let|var))");
    processed = std::regex_replace(processed, exportPattern, "// ES6 export: $1");
    
    return processed;
}

std::string JavaScriptCompiler::processAsyncCode(const std::string& jsCode) {
    std::string processed = jsCode;
    
    // 处理async/await语法
    std::regex asyncFunctionPattern(R"(async\s+function\s+(\w+))");
    processed = std::regex_replace(processed, asyncFunctionPattern, "function $1");
    
    std::regex awaitPattern(R"(await\s+(\w+))");
    processed = std::regex_replace(processed, awaitPattern, "$1");
    
    return processed;
}

std::string JavaScriptCompiler::generateJavaScriptWrapper(const std::string& jsCode, const CompileConfig& config) {
    std::string wrapped = jsCode;
    
    // 如果需要，可以添加IIFE包装器
    if (config.wrapInIIFE) {
        wrapped = "(function() {\n" + wrapped + "\n})();";
    }
    
    // 如果需要，可以添加严格模式
    if (config.useStrictMode) {
        wrapped = "'use strict';\n" + wrapped;
    }
    
    return wrapped;
}

std::string JavaScriptCompiler::minifyJavaScript(const std::string& jsCode) {
    std::string minified = jsCode;
    
    // 移除注释
    std::regex singleLineCommentPattern(R"(\/\/.*$)");
    minified = std::regex_replace(minified, singleLineCommentPattern, "");
    
    std::regex multiLineCommentPattern(R"(\/\*[^*]*\*+(?:[^/*][^*]*\*+)*\/)");
    minified = std::regex_replace(minified, multiLineCommentPattern, "");
    
    // 移除不必要的空白
    std::regex whitespacePattern(R"(\s+)");
    minified = std::regex_replace(minified, whitespacePattern, " ");
    
    // 移除行尾空白
    std::regex trailingWhitespacePattern(R"(\s+$)");
    minified = std::regex_replace(minified, trailingWhitespacePattern, "");
    
    // 移除分号后的空白
    std::regex semicolonWhitespacePattern(R"(\s*;\s*)");
    minified = std::regex_replace(minified, semicolonWhitespacePattern, ";");
    
    return minified;
}

std::string JavaScriptCompiler::formatJavaScript(const std::string& jsCode) {
    std::string formatted = jsCode;
    
    // 基本的格式化
    std::regex functionPattern(R"(function\s+(\w+)\s*\(([^)]*)\)\s*\{)");
    formatted = std::regex_replace(formatted, functionPattern, "function $1($2) {\n  ");
    
    // 格式化对象字面量
    std::regex objectPattern(R"(\{([^}]+)\})");
    formatted = std::regex_replace(formatted, objectPattern, "{\n  $1\n}");
    
    return formatted;
}

std::string JavaScriptCompiler::addJavaScriptPolyfills(const std::string& jsCode) {
    std::string polyfilled = jsCode;
    
    // 添加Array.prototype.includes polyfill
    if (jsCode.find("includes(") != std::string::npos) {
        polyfilled = "// Array.prototype.includes polyfill\n" + polyfilled;
    }
    
    // 添加Promise polyfill
    if (jsCode.find("new Promise") != std::string::npos) {
        polyfilled = "// Promise polyfill\n" + polyfilled;
    }
    
    return polyfilled;
}

std::vector<std::string> JavaScriptCompiler::checkJavaScriptCompatibility(const std::string& jsCode) {
    std::vector<std::string> issues;
    
    // 检查ES6特性支持
    if (jsCode.find("const ") != std::string::npos || jsCode.find("let ") != std::string::npos) {
        issues.push_back("const/let 在某些旧版浏览器中可能不被支持");
    }
    
    if (jsCode.find("=>") != std::string::npos) {
        issues.push_back("箭头函数在某些旧版浏览器中可能不被支持");
    }
    
    if (jsCode.find("`") != std::string::npos) {
        issues.push_back("模板字符串在某些旧版浏览器中可能不被支持");
    }
    
    if (jsCode.find("async") != std::string::npos || jsCode.find("await") != std::string::npos) {
        issues.push_back("async/await 在某些旧版浏览器中可能不被支持");
    }
    
    return issues;
}

std::unordered_map<std::string, size_t> JavaScriptCompiler::generateJavaScriptStatistics(const std::string& jsCode) {
    std::unordered_map<std::string, size_t> stats;
    
    // 统计语句数量
    std::regex statementPattern(R"([^;{}]+[;{}])");
    std::sregex_iterator statementBegin(jsCode.begin(), jsCode.end(), statementPattern);
    std::sregex_iterator statementEnd;
    stats["statements"] = std::distance(statementBegin, statementEnd);
    
    // 统计函数数量
    std::regex functionPattern(R"(function\s+\w+\s*\(|=>\s*\{?)");
    std::sregex_iterator functionBegin(jsCode.begin(), jsCode.end(), functionPattern);
    std::sregex_iterator functionEnd;
    stats["functions"] = std::distance(functionBegin, functionEnd);
    
    // 统计变量数量
    std::regex variablePattern(R"((let|const|var)\s+\w+)");
    std::sregex_iterator variableBegin(jsCode.begin(), jsCode.end(), variablePattern);
    std::sregex_iterator variableEnd;
    stats["variables"] = std::distance(variableBegin, variableEnd);
    
    // 统计文件大小
    stats["size"] = jsCode.length();
    
    return stats;
}

std::string JavaScriptCompiler::processCHTLJSExtensions(const std::string& jsCode) {
    std::string processed = jsCode;
    
    // 处理增强选择器语法 {{}}
    processed = processEnhancedSelectors(processed);
    
    // 处理箭头操作符 ->
    processed = processArrowOperators(processed);
    
    // 处理增强监听器语法 listen()
    processed = processEnhancedListeners(processed);
    
    // 处理事件委托语法 delegate()
    processed = processEventDelegation(processed);
    
    // 处理动画语法 animate()
    processed = processAnimationSyntax(processed);
    
    return processed;
}

std::string JavaScriptCompiler::processEnhancedSelectors(const std::string& jsCode) {
    std::string processed = jsCode;
    
    // 处理 {{}} 语法，将其转换为标准的JavaScript选择器
    std::regex enhancedSelectorPattern(R"(\{\{([^}]+)\}\})");
    processed = std::regex_replace(processed, enhancedSelectorPattern, "document.querySelector('$1')");
    
    return processed;
}

std::string JavaScriptCompiler::processArrowOperators(const std::string& jsCode) {
    std::string processed = jsCode;
    
    // 处理 -> 操作符，将其转换为标准的JavaScript语法
    std::regex arrowOperatorPattern(R"((\w+)\s*->\s*(\w+))");
    processed = std::regex_replace(processed, arrowOperatorPattern, "$1.$2");
    
    return processed;
}

std::string JavaScriptCompiler::processEnhancedListeners(const std::string& jsCode) {
    std::string processed = jsCode;
    
    // 处理 listen() 语法，将其转换为标准的addEventListener
    std::regex listenPattern(R"(listen\s*\(\s*['"]([^'"]+)['"]\s*,\s*function\s*\([^)]*\)\s*\{([^}]*)\}\s*\))");
    processed = std::regex_replace(processed, listenPattern, "addEventListener('$1', function(event) { $2 })");
    
    return processed;
}

std::string JavaScriptCompiler::processEventDelegation(const std::string& jsCode) {
    std::string processed = jsCode;
    
    // 处理 delegate() 语法，将其转换为标准的事件委托实现
    std::regex delegatePattern(R"(delegate\s*\(\s*['"]([^'"]+)['"]\s*,\s*['"]([^'"]+)['"]\s*,\s*function\s*\([^)]*\)\s*\{([^}]*)\}\s*\))");
    processed = std::regex_replace(processed, delegatePattern, 
        "addEventListener('$1', function(event) { if (event.target.matches('$2')) { $3 } })");
    
    return processed;
}

std::string JavaScriptCompiler::processAnimationSyntax(const std::string& jsCode) {
    std::string processed = jsCode;
    
    // 处理 animate() 语法，将其转换为标准的CSS动画或Web Animations API
    std::regex animatePattern(R"(animate\s*\(\s*['"]([^'"]+)['"]\s*,\s*\{([^}]*)\}\s*\))");
    processed = std::regex_replace(processed, animatePattern, 
        "element.animate([{ $2 }], { duration: 1000, easing: 'ease-in-out' })");
    
    return processed;
}

} // namespace chtl
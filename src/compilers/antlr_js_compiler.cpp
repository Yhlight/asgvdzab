#include "compilers/antlr_js_compiler.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <regex>

using namespace chtl;

// ANTLRJSCompiler实现
ANTLRJSCompiler::ANTLRJSCompiler() {
}

std::vector<CodeSegmentType> ANTLRJSCompiler::getSupportedTypes() const {
    return {
        CodeSegmentType::JAVASCRIPT_STANDARD,
        CodeSegmentType::CHTL_JS_SCRIPT
    };
}

CompileResult ANTLRJSCompiler::compile(const CodeSegment& segment, const CompileConfig& config) {
    JSCompileOptions jsOptions;
    jsOptions.minify = config.optimization.minify;
    jsOptions.validateSyntax = true;
    jsOptions.extractFunctions = true;
    jsOptions.extractVariables = true;
    jsOptions.extractClasses = true;
    
    auto jsResult = compileJS(segment.content, jsOptions);
    
    CompileResult result;
    result.success = jsResult.success;
    result.output = jsResult.javascript;
    result.errors = jsResult.errors;
    result.warnings = jsResult.warnings;
    
    return result;
}

JSCompileResult ANTLRJSCompiler::compileJS(const std::string& jsCode, const JSCompileOptions& options) {
    JSCompileResult result;
    result.originalSize = jsCode.length();
    
    try {
        std::cout << "开始标准JavaScript编译..." << std::endl;
        
        // 提取函数
        if (options.extractFunctions) {
            result.functions = extractFunctionsRegex(jsCode);
            std::cout << "提取的函数数量: " << result.functions.size() << std::endl;
        }
        
        // 提取变量
        if (options.extractVariables) {
            result.variables = extractVariablesRegex(jsCode);
            std::cout << "提取的变量数量: " << result.variables.size() << std::endl;
        }
        
        // 提取类
        if (options.extractClasses) {
            result.classes = extractClassesRegex(jsCode);
            std::cout << "提取的类数量: " << result.classes.size() << std::endl;
        }
        
        // 生成JavaScript
        result.javascript = jsCode; // 基础版本直接返回原始代码
        if (options.minify) {
            result.javascript = JSUtils::removeWhitespace(result.javascript);
        }
        
        result.compiledSize = result.javascript.length();
        result.success = true;
        
        std::cout << "标准JavaScript编译完成" << std::endl;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back(std::string("JavaScript compilation error: ") + e.what());
    }
    
    return result;
}

std::vector<std::string> ANTLRJSCompiler::extractFunctionsRegex(const std::string& jsCode) {
    std::vector<std::string> functions;
    
    // 匹配function声明
    std::regex functionRegex(R"(function\s+([a-zA-Z_$][a-zA-Z0-9_$]*)\s*\()");
    std::sregex_iterator iter(jsCode.begin(), jsCode.end(), functionRegex);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        std::string functionName = iter->str(1);
        if (!functionName.empty()) {
            functions.push_back(functionName);
        }
    }
    
    // 匹配箭头函数赋值
    std::regex arrowRegex(R"((const|let|var)\s+([a-zA-Z_$][a-zA-Z0-9_$]*)\s*=\s*\([^)]*\)\s*=>)");
    iter = std::sregex_iterator(jsCode.begin(), jsCode.end(), arrowRegex);
    
    for (; iter != end; ++iter) {
        std::string functionName = iter->str(2);
        if (!functionName.empty()) {
            functions.push_back(functionName);
        }
    }
    
    return functions;
}

std::vector<std::string> ANTLRJSCompiler::extractVariablesRegex(const std::string& jsCode) {
    std::vector<std::string> variables;
    
    // 匹配var/let/const声明
    std::regex varRegex(R"((var|let|const)\s+([a-zA-Z_$][a-zA-Z0-9_$]*))");
    std::sregex_iterator iter(jsCode.begin(), jsCode.end(), varRegex);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        std::string varName = iter->str(2);
        if (!varName.empty()) {
            variables.push_back(varName);
        }
    }
    
    return variables;
}

std::vector<std::string> ANTLRJSCompiler::extractClassesRegex(const std::string& jsCode) {
    std::vector<std::string> classes;
    
    // 匹配class声明
    std::regex classRegex(R"(class\s+([a-zA-Z_$][a-zA-Z0-9_$]*))");
    std::sregex_iterator iter(jsCode.begin(), jsCode.end(), classRegex);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        std::string className = iter->str(1);
        if (!className.empty()) {
            classes.push_back(className);
        }
    }
    
    return classes;
}

bool ANTLRJSCompiler::validateSyntax(const std::string& jsCode, std::vector<std::string>& errors) {
    // 基本的JavaScript语法验证
    try {
        // 检查基本的语法结构
        std::regex invalidRegex(R"(\}\s*\{)"); // 简单的语法错误检测
        if (std::regex_search(jsCode, invalidRegex)) {
            errors.push_back("Potential syntax error: unexpected token sequence");
            return false;
        }
        return true;
    } catch (const std::exception& e) {
        errors.push_back(e.what());
        return false;
    }
}

std::vector<std::string> ANTLRJSCompiler::extractFunctions(const std::string& jsCode) {
    return extractFunctionsRegex(jsCode);
}

std::vector<std::string> ANTLRJSCompiler::extractVariables(const std::string& jsCode) {
    return extractVariablesRegex(jsCode);
}

std::vector<std::string> ANTLRJSCompiler::extractClasses(const std::string& jsCode) {
    return extractClassesRegex(jsCode);
}

std::string ANTLRJSCompiler::minifyJS(const std::string& jsCode) {
    JSCompileOptions options;
    options.minify = true;
    auto result = compileJS(jsCode, options);
    return result.success ? result.javascript : jsCode;
}

std::string ANTLRJSCompiler::formatJS(const std::string& jsCode, const std::string& indent) {
    JSCompileOptions options;
    options.minify = false;
    auto result = compileJS(jsCode, options);
    return result.success ? result.javascript : jsCode;
}

std::string ANTLRJSCompiler::convertToES5(const std::string& jsCode) {
    // 简化实现：基本的ES6到ES5转换
    std::string result = jsCode;
    
    // 将const和let转换为var
    result = std::regex_replace(result, std::regex(R"(\b(const|let)\b)"), "var");
    
    return result;
}

std::string ANTLRJSCompiler::convertToES6(const std::string& jsCode) {
    // 简化实现：基本的ES5到ES6转换
    return jsCode; // 大部分ES5代码在ES6中都是有效的
}

// JSUtils实现
bool JSUtils::isValidIdentifier(const std::string& identifier) {
    if (identifier.empty()) return false;
    
    // JavaScript标识符规则：以字母、_或$开头，后面可以跟字母、数字、_或$
    std::regex identifierRegex(R"(^[a-zA-Z_$][a-zA-Z0-9_$]*$)");
    return std::regex_match(identifier, identifierRegex);
}

bool JSUtils::isKeyword(const std::string& word) {
    static const std::vector<std::string> keywords = {
        "break", "case", "catch", "class", "const", "continue", "debugger", "default",
        "delete", "do", "else", "export", "extends", "finally", "for", "function",
        "if", "import", "in", "instanceof", "let", "new", "return", "super",
        "switch", "this", "throw", "try", "typeof", "var", "void", "while", "with", "yield"
    };
    
    return std::find(keywords.begin(), keywords.end(), word) != keywords.end();
}

std::string JSUtils::detectESVersion(const std::string& jsCode) {
    // 检测ES版本特性
    if (jsCode.find("class ") != std::string::npos ||
        jsCode.find("const ") != std::string::npos ||
        jsCode.find("let ") != std::string::npos ||
        jsCode.find("=>") != std::string::npos) {
        return "ES6+";
    }
    
    return "ES5";
}

std::string JSUtils::extractFunctionName(const std::string& funcDecl) {
    std::regex functionRegex(R"(function\s+([a-zA-Z_$][a-zA-Z0-9_$]*))");
    std::smatch match;
    
    if (std::regex_search(funcDecl, match, functionRegex)) {
        return match.str(1);
    }
    
    return "";
}

std::vector<std::string> JSUtils::extractParameters(const std::string& funcDecl) {
    std::vector<std::string> parameters;
    
    std::regex paramRegex(R"(\(([^)]*)\))");
    std::smatch match;
    
    if (std::regex_search(funcDecl, match, paramRegex)) {
        std::string paramList = match.str(1);
        
        // 分割参数
        std::regex commaRegex(R"(\s*,\s*)");
        std::sregex_token_iterator iter(paramList.begin(), paramList.end(), commaRegex, -1);
        std::sregex_token_iterator end;
        
        for (; iter != end; ++iter) {
            std::string param = iter->str();
            param = std::regex_replace(param, std::regex(R"(^\s+|\s+$)"), ""); // trim
            if (!param.empty()) {
                parameters.push_back(param);
            }
        }
    }
    
    return parameters;
}

std::string JSUtils::extractVariableName(const std::string& varDecl) {
    std::regex varRegex(R"((var|let|const)\s+([a-zA-Z_$][a-zA-Z0-9_$]*))");
    std::smatch match;
    
    if (std::regex_search(varDecl, match, varRegex)) {
        return match.str(2);
    }
    
    return "";
}

std::string JSUtils::detectVariableType(const std::string& varDecl) {
    if (varDecl.find("const ") != std::string::npos) return "const";
    if (varDecl.find("let ") != std::string::npos) return "let";
    if (varDecl.find("var ") != std::string::npos) return "var";
    return "unknown";
}

std::string JSUtils::removeComments(const std::string& js) {
    std::string result = js;
    
    // 移除单行注释
    result = std::regex_replace(result, std::regex(R"(//.*$)"), "", std::regex_constants::match_default);
    
    // 移除多行注释
    result = std::regex_replace(result, std::regex(R"(/\*.*?\*/)"), "");
    
    return result;
}

std::string JSUtils::removeWhitespace(const std::string& js) {
    return std::regex_replace(js, std::regex(R"(\s+)"), " ");
}

std::string JSUtils::addIndentation(const std::string& js, const std::string& indent) {
    std::string result = js;
    result = std::regex_replace(result, std::regex(R"(})"), "}\n");
    result = std::regex_replace(result, std::regex(R"({)"), "{\n" + indent);
    return result;
}

int JSUtils::calculateComplexity(const std::string& jsCode) {
    int complexity = 1; // 基础复杂度
    
    // 统计控制流关键字
    std::vector<std::string> complexityKeywords = {"if", "else", "for", "while", "switch", "case", "catch"};
    
    for (const auto& keyword : complexityKeywords) {
        std::regex keywordRegex("\\b" + keyword + "\\b");
        std::sregex_iterator iter(jsCode.begin(), jsCode.end(), keywordRegex);
        std::sregex_iterator end;
        complexity += std::distance(iter, end);
    }
    
    return complexity;
}

std::vector<std::string> JSUtils::findDependencies(const std::string& jsCode) {
    std::vector<std::string> dependencies;
    
    // 查找require语句
    std::regex requireRegex(R"(require\s*\(\s*['"]([^'"]+)['"]\s*\))");
    std::sregex_iterator iter(jsCode.begin(), jsCode.end(), requireRegex);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        dependencies.push_back(iter->str(1));
    }
    
    // 查找import语句
    std::regex importRegex(R"(import\s+.*?from\s+['"]([^'"]+)['"])");
    iter = std::sregex_iterator(jsCode.begin(), jsCode.end(), importRegex);
    
    for (; iter != end; ++iter) {
        dependencies.push_back(iter->str(1));
    }
    
    return dependencies;
}
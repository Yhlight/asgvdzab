#include "compilers/simple_js_compiler.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace chtl;

SimpleJSCompiler::SimpleJSCompiler() {
}

std::vector<CodeSegmentType> SimpleJSCompiler::getSupportedTypes() const {
    return {
        CodeSegmentType::JAVASCRIPT_STANDARD,
        CodeSegmentType::JAVASCRIPT_ORIGIN,
        CodeSegmentType::CHTL_JS_SCRIPT
    };
}

CompileResult SimpleJSCompiler::compile(const CodeSegment& segment, const CompileConfig& config) {
    JSCompileOptions jsOptions;
    jsOptions.minify = config.optimization.minify;
    jsOptions.validateSyntax = true;
    jsOptions.extractFunctions = true;
    jsOptions.extractVariables = true;
    
    auto jsResult = compileJS(segment.content, jsOptions);
    
    CompileResult result;
    result.success = jsResult.success;
    result.output = jsResult.javascript;
    result.errors = jsResult.errors;
    result.warnings = jsResult.warnings;
    
    return result;
}

JSCompileResult SimpleJSCompiler::compileJS(const std::string& jsCode, const JSCompileOptions& options) {
    JSCompileResult result;
    result.originalSize = jsCode.length();
    
    try {
        // 基础语法验证
        if (options.validateSyntax) {
            if (!basicSyntaxCheck(jsCode, result.errors)) {
                result.success = false;
                return result;
            }
        }
        
        // 提取信息
        extractInfo(jsCode, result, options);
        
        // 处理代码（美化或压缩）
        result.javascript = processCode(jsCode, options.minify);
        result.compiledSize = result.javascript.length();
        result.success = true;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back(std::string("JavaScript compilation error: ") + e.what());
    }
    
    return result;
}

void SimpleJSCompiler::extractInfo(const std::string& jsCode, JSCompileResult& result, const JSCompileOptions& options) {
    if (options.extractFunctions) {
        // 提取函数声明
        std::regex funcRegex(R"(function\s+([a-zA-Z_$][a-zA-Z0-9_$]*)\s*\()");
        std::sregex_iterator begin(jsCode.begin(), jsCode.end(), funcRegex);
        std::sregex_iterator end;
        
        for (std::sregex_iterator i = begin; i != end; ++i) {
            result.functions.push_back(i->str(1));
        }
        
        // 提取箭头函数
        std::regex arrowFuncRegex(R"((?:const|let|var)\s+([a-zA-Z_$][a-zA-Z0-9_$]*)\s*=.*?=>)");
        begin = std::sregex_iterator(jsCode.begin(), jsCode.end(), arrowFuncRegex);
        
        for (std::sregex_iterator i = begin; i != end; ++i) {
            result.functions.push_back(i->str(1));
        }
    }
    
    if (options.extractVariables) {
        // 提取变量声明
        std::regex varRegex(R"((?:var|let|const)\s+([a-zA-Z_$][a-zA-Z0-9_$]*))");
        std::sregex_iterator begin(jsCode.begin(), jsCode.end(), varRegex);
        std::sregex_iterator end;
        
        for (std::sregex_iterator i = begin; i != end; ++i) {
            result.variables.push_back(i->str(1));
        }
    }
    
    if (options.extractClasses) {
        // 提取类声明
        std::regex classRegex(R"(class\s+([a-zA-Z_$][a-zA-Z0-9_$]*))");
        std::sregex_iterator begin(jsCode.begin(), jsCode.end(), classRegex);
        std::sregex_iterator end;
        
        for (std::sregex_iterator i = begin; i != end; ++i) {
            result.classes.push_back(i->str(1));
        }
    }
}

std::string SimpleJSCompiler::processCode(const std::string& jsCode, bool minify) {
    std::string processed = jsCode;
    
    if (minify) {
        // 移除注释
        processed = JSUtils::removeComments(processed);
        
        // 移除多余空白
        processed = JSUtils::removeWhitespace(processed);
        
        // 简单压缩
        processed = std::regex_replace(processed, std::regex(R"(\s*{\s*)"), "{");
        processed = std::regex_replace(processed, std::regex(R"(\s*}\s*)"), "}");
        processed = std::regex_replace(processed, std::regex(R"(\s*;\s*)"), ";");
        processed = std::regex_replace(processed, std::regex(R"(\s*,\s*)"), ",");
        processed = std::regex_replace(processed, std::regex(R"(\s*\(\s*)"), "(");
        processed = std::regex_replace(processed, std::regex(R"(\s*\)\s*)"), ")");
    }
    
    return processed;
}

bool SimpleJSCompiler::basicSyntaxCheck(const std::string& jsCode, std::vector<std::string>& errors) {
    // 简单的括号匹配检查
    int braceCount = 0;
    int parenCount = 0;
    int bracketCount = 0;
    
    bool inString = false;
    char stringChar = '\0';
    
    for (size_t i = 0; i < jsCode.length(); ++i) {
        char c = jsCode[i];
        
        // 处理字符串
        if (!inString && (c == '"' || c == '\'' || c == '`')) {
            inString = true;
            stringChar = c;
            continue;
        } else if (inString && c == stringChar && (i == 0 || jsCode[i-1] != '\\')) {
            inString = false;
            continue;
        }
        
        if (inString) continue;
        
        // 检查括号匹配
        switch (c) {
            case '{': braceCount++; break;
            case '}': braceCount--; break;
            case '(': parenCount++; break;
            case ')': parenCount--; break;
            case '[': bracketCount++; break;
            case ']': bracketCount--; break;
        }
        
        // 检查是否有不匹配的括号
        if (braceCount < 0) {
            errors.push_back("Unmatched closing brace '}'");
            return false;
        }
        if (parenCount < 0) {
            errors.push_back("Unmatched closing parenthesis ')'");
            return false;
        }
        if (bracketCount < 0) {
            errors.push_back("Unmatched closing bracket ']'");
            return false;
        }
    }
    
    // 检查最终是否匹配
    if (braceCount != 0) {
        errors.push_back("Unmatched braces");
        return false;
    }
    if (parenCount != 0) {
        errors.push_back("Unmatched parentheses");
        return false;
    }
    if (bracketCount != 0) {
        errors.push_back("Unmatched brackets");
        return false;
    }
    
    return true;
}

bool SimpleJSCompiler::validateSyntax(const std::string& jsCode, std::vector<std::string>& errors) {
    return basicSyntaxCheck(jsCode, errors);
}

std::vector<std::string> SimpleJSCompiler::extractFunctions(const std::string& jsCode) {
    JSCompileOptions options;
    options.extractFunctions = true;
    auto result = compileJS(jsCode, options);
    return result.functions;
}

std::vector<std::string> SimpleJSCompiler::extractVariables(const std::string& jsCode) {
    JSCompileOptions options;
    options.extractVariables = true;
    auto result = compileJS(jsCode, options);
    return result.variables;
}

std::vector<std::string> SimpleJSCompiler::extractClasses(const std::string& jsCode) {
    JSCompileOptions options;
    options.extractClasses = true;
    auto result = compileJS(jsCode, options);
    return result.classes;
}

std::string SimpleJSCompiler::minifyJS(const std::string& jsCode) {
    JSCompileOptions options;
    options.minify = true;
    auto result = compileJS(jsCode, options);
    return result.success ? result.javascript : jsCode;
}

std::string SimpleJSCompiler::formatJS(const std::string& jsCode, const std::string& indent) {
    JSCompileOptions options;
    options.minify = false;
    auto result = compileJS(jsCode, options);
    return result.success ? result.javascript : jsCode;
}

std::string SimpleJSCompiler::convertToES5(const std::string& jsCode) {
    std::string es5Code = jsCode;
    
    // 转换箭头函数
    es5Code = std::regex_replace(es5Code, std::regex(R"(\(\s*([^)]*)\s*\)\s*=>\s*)"), "function($1)");
    
    // 转换const/let为var
    es5Code = std::regex_replace(es5Code, std::regex(R"(\bconst\b)"), "var");
    es5Code = std::regex_replace(es5Code, std::regex(R"(\blet\b)"), "var");
    
    return es5Code;
}

std::string SimpleJSCompiler::convertToES6(const std::string& jsCode) {
    // 简单的ES5到ES6转换
    return jsCode;
}

// JSUtils实现

bool JSUtils::isValidIdentifier(const std::string& identifier) {
    if (identifier.empty()) return false;
    
    // 检查第一个字符
    if (!std::isalpha(identifier[0]) && identifier[0] != '_' && identifier[0] != '$') {
        return false;
    }
    
    // 检查其余字符
    for (size_t i = 1; i < identifier.length(); ++i) {
        if (!std::isalnum(identifier[i]) && identifier[i] != '_' && identifier[i] != '$') {
            return false;
        }
    }
    
    return !isKeyword(identifier);
}

bool JSUtils::isKeyword(const std::string& word) {
    static const std::vector<std::string> keywords = {
        "abstract", "boolean", "break", "byte", "case", "catch", "char", "class",
        "const", "continue", "debugger", "default", "delete", "do", "double",
        "else", "enum", "export", "extends", "false", "final", "finally",
        "float", "for", "function", "goto", "if", "implements", "import",
        "in", "instanceof", "int", "interface", "let", "long", "native",
        "new", "null", "package", "private", "protected", "public", "return",
        "short", "static", "super", "switch", "synchronized", "this", "throw",
        "throws", "transient", "true", "try", "typeof", "var", "void",
        "volatile", "while", "with", "yield"
    };
    
    return std::find(keywords.begin(), keywords.end(), word) != keywords.end();
}

std::string JSUtils::detectESVersion(const std::string& jsCode) {
    if (jsCode.find("=>") != std::string::npos ||
        jsCode.find("const ") != std::string::npos ||
        jsCode.find("let ") != std::string::npos ||
        jsCode.find("class ") != std::string::npos) {
        return "es2015";
    }
    
    if (jsCode.find("async ") != std::string::npos ||
        jsCode.find("await ") != std::string::npos) {
        return "es2017";
    }
    
    return "es5";
}

std::string JSUtils::extractFunctionName(const std::string& functionDecl) {
    std::regex funcRegex(R"(function\s+([a-zA-Z_$][a-zA-Z0-9_$]*))");
    std::smatch match;
    
    if (std::regex_search(functionDecl, match, funcRegex)) {
        return match[1].str();
    }
    
    return "";
}

std::vector<std::string> JSUtils::extractParameters(const std::string& functionDecl) {
    std::vector<std::string> params;
    std::regex paramRegex(R"(\(([^)]*)\))");
    std::smatch match;
    
    if (std::regex_search(functionDecl, match, paramRegex)) {
        std::string paramStr = match[1].str();
        std::stringstream ss(paramStr);
        std::string param;
        
        while (std::getline(ss, param, ',')) {
            // 去除空白
            param.erase(0, param.find_first_not_of(" \t"));
            param.erase(param.find_last_not_of(" \t") + 1);
            if (!param.empty()) {
                params.push_back(param);
            }
        }
    }
    
    return params;
}

std::string JSUtils::extractVariableName(const std::string& varDecl) {
    std::regex varRegex(R"((?:var|let|const)\s+([a-zA-Z_$][a-zA-Z0-9_$]*))");
    std::smatch match;
    
    if (std::regex_search(varDecl, match, varRegex)) {
        return match[1].str();
    }
    
    return "";
}

std::string JSUtils::detectVariableType(const std::string& varDecl) {
    if (varDecl.find("const ") == 0) return "const";
    if (varDecl.find("let ") == 0) return "let";
    if (varDecl.find("var ") == 0) return "var";
    return "unknown";
}

std::string JSUtils::removeComments(const std::string& js) {
    std::string result = js;
    
    // 移除单行注释
    result = std::regex_replace(result, std::regex(R"(//.*$)"), "", std::regex_constants::match_any);
    
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
    int complexity = 1;
    
    std::vector<std::string> controlFlow = {"if", "else", "while", "for", "switch", "case", "catch"};
    
    for (const auto& keyword : controlFlow) {
        std::regex keywordRegex("\\b" + keyword + "\\b");
        std::sregex_iterator begin(jsCode.begin(), jsCode.end(), keywordRegex);
        std::sregex_iterator end;
        complexity += std::distance(begin, end);
    }
    
    return complexity;
}

std::vector<std::string> JSUtils::findDependencies(const std::string& jsCode) {
    std::vector<std::string> deps;
    
    // 查找require语句
    std::regex requireRegex(R"(require\s*\(\s*['"]([^'"]+)['"]\s*\))");
    std::sregex_iterator begin(jsCode.begin(), jsCode.end(), requireRegex);
    std::sregex_iterator end;
    
    for (std::sregex_iterator i = begin; i != end; ++i) {
        deps.push_back(i->str(1));
    }
    
    // 查找import语句
    std::regex importRegex(R"(import\s+.*\s+from\s+['"]([^'"]+)['"])");
    begin = std::sregex_iterator(jsCode.begin(), jsCode.end(), importRegex);
    
    for (std::sregex_iterator i = begin; i != end; ++i) {
        deps.push_back(i->str(1));
    }
    
    return deps;
}
#include "compiler/JavaScriptCompiler.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <regex>
#include <unordered_set>

namespace chtl {

// 私有实现类
class JavaScriptCompiler::Impl {
public:
    std::unordered_map<std::string, std::string> options;
    CompileStats stats;
    std::vector<std::string> errors;
    
    // JavaScript关键字和保留字
    std::unordered_set<std::string> keywords;
    std::unordered_set<std::string> reservedWords;
    
    Impl() {
        stats = CompileStats{};
        
        // 初始化JavaScript关键字
        keywords = {
            "break", "case", "catch", "class", "const", "continue", "debugger", "default",
            "delete", "do", "else", "export", "extends", "finally", "for", "function",
            "if", "import", "in", "instanceof", "let", "new", "return", "super",
            "switch", "this", "throw", "try", "typeof", "var", "void", "while",
            "with", "yield", "enum", "await", "implements", "interface", "package",
            "private", "protected", "public", "static", "true", "false", "null", "undefined"
        };
        
        // 初始化保留字
        reservedWords = {
            "abstract", "boolean", "byte", "char", "double", "final", "float", "goto",
            "int", "long", "native", "short", "synchronized", "throws", "transient", "volatile"
        };
    }
    
    ~Impl() = default;
};

JavaScriptCompiler::JavaScriptCompiler() : pImpl(std::make_unique<Impl>()) {}

JavaScriptCompiler::~JavaScriptCompiler() = default;

CompilationResult JavaScriptCompiler::compile(const CodeFragment& fragment) {
    CompilationResult result;
    
    try {
        // 验证JavaScript语法
        if (!validateJavaScript(fragment.content)) {
            result.success = false;
            result.errors = pImpl->errors;
            return result;
        }
        
        // 解析JavaScript
        std::string parsed = parseJavaScript(fragment.content);
        
        // 生成JavaScript
        std::string generated = generateJavaScript(parsed);
        
        result.success = true;
        result.output = generated;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back("编译异常: " + std::string(e.what()));
    }
    
    return result;
}

void JavaScriptCompiler::setOptions(const std::unordered_map<std::string, std::string>& options) {
    pImpl->options = options;
}

JavaScriptCompiler::CompileStats JavaScriptCompiler::getCompileStats() const {
    return pImpl->stats;
}

std::vector<std::string> JavaScriptCompiler::getErrors() const {
    return pImpl->errors;
}

void JavaScriptCompiler::clearErrors() {
    pImpl->errors.clear();
}

bool JavaScriptCompiler::validateJavaScript(const std::string& js) {
    pImpl->errors.clear();
    
    // 基本的JavaScript语法验证
    std::istringstream stream(js);
    std::string line;
    size_t lineNumber = 0;
    
    while (std::getline(stream, line)) {
        lineNumber++;
        
        // 跳过空行和注释
        if (line.empty() || line.find("//") == 0 || line.find("/*") == 0) {
            continue;
        }
        
        // 检查语句
        if (!validateStatement(line)) {
            addError("第" + std::to_string(lineNumber) + "行: 无效的语句: " + line);
        }
    }
    
    return pImpl->errors.empty();
}

std::string JavaScriptCompiler::minifyJavaScript(const std::string& js) {
    std::string minified = js;
    
    // 移除注释
    std::regex singleLineComment(R"(//.*$)");
    std::regex multiLineComment(R"(/\*[\s\S]*?\*/)");
    
    minified = std::regex_replace(minified, singleLineComment, "");
    minified = std::regex_replace(minified, multiLineComment, "");
    
    // 移除多余的空白字符
    std::regex whitespaceRegex(R"(\s+)");
    minified = std::regex_replace(minified, whitespaceRegex, " ");
    
    // 移除行首行尾空白
    minified = std::regex_replace(minified, std::regex(R"(^\s+|\s+$)"), "");
    
    // 移除分号后的空白
    minified = std::regex_replace(minified, std::regex(R"(\s*;\s*)"), ";");
    
    // 移除逗号后的空白
    minified = std::regex_replace(minified, std::regex(R"(\s*,\s*)"), ",");
    
    // 移除操作符周围的空白
    minified = std::regex_replace(minified, std::regex(R"(\s*([+\-*/=<>!&|])\s*)"), "$1");
    
    return minified;
}

std::string JavaScriptCompiler::formatJavaScript(const std::string& js) {
    std::string formatted = js;
    std::string result;
    
    // 基本的格式化
    std::istringstream stream(formatted);
    std::string line;
    int indentLevel = 0;
    
    while (std::getline(stream, line)) {
        // 跳过空行和注释
        if (line.empty() || line.find("//") == 0 || line.find("/*") == 0) {
            continue;
        }
        
        // 减少缩进级别
        if (line.find('}') != std::string::npos) {
            indentLevel = std::max(0, indentLevel - 1);
        }
        
        // 添加缩进
        if (!line.empty()) {
            result += std::string(indentLevel * 4, ' ') + line + "\n";
        }
        
        // 增加缩进级别
        if (line.find('{') != std::string::npos) {
            indentLevel++;
        }
    }
    
    return result;
}

bool JavaScriptCompiler::syntaxCheck(const std::string& js) {
    // 基本的语法检查
    int braceCount = 0;
    int parenCount = 0;
    int bracketCount = 0;
    
    for (char c : js) {
        switch (c) {
            case '{': braceCount++; break;
            case '}': braceCount--; break;
            case '(': parenCount++; break;
            case ')': parenCount--; break;
            case '[': bracketCount++; break;
            case ']': bracketCount--; break;
        }
        
        if (braceCount < 0 || parenCount < 0 || bracketCount < 0) {
            return false;
        }
    }
    
    return braceCount == 0 && parenCount == 0 && bracketCount == 0;
}

std::string JavaScriptCompiler::optimizeCode(const std::string& js) {
    std::string optimized = js;
    
    // 移除未使用的变量
    optimized = removeUnusedVariables(optimized);
    
    // 优化表达式
    optimized = optimizeExpressions(optimized);
    
    // 内联简单函数
    optimized = inlineSimpleFunctions(optimized);
    
    return optimized;
}

std::string JavaScriptCompiler::parseJavaScript(const std::string& source) {
    // 简化实现，直接返回源内容
    // 在实际实现中，这里应该使用ANTLR4进行解析
    return source;
}

std::string JavaScriptCompiler::generateJavaScript(const std::string& parsed) {
    // 处理各种JavaScript结构
    std::string result = parsed;
    
    result = processFunctions(result);
    result = processVariables(result);
    result = processClasses(result);
    result = processImports(result);
    result = processExports(result);
    result = processExpressions(result);
    
    return result;
}

std::string JavaScriptCompiler::processFunctions(const std::string& js) {
    // 处理函数
    std::string result = js;
    
    // 统计函数数量
    std::regex functionRegex(R"(function\s+\w+\s*\(|=>\s*\{|\w+\s*\([^)]*\)\s*=>)");
    auto begin = std::sregex_iterator(result.begin(), result.end(), functionRegex);
    auto end = std::sregex_iterator();
    
    pImpl->stats.functions = std::distance(begin, end);
    
    return result;
}

std::string JavaScriptCompiler::processVariables(const std::string& js) {
    // 处理变量
    std::string result = js;
    
    // 统计变量数量
    std::regex variableRegex(R"((?:var|let|const)\s+\w+)");
    auto begin = std::sregex_iterator(result.begin(), result.end(), variableRegex);
    auto end = std::sregex_iterator();
    
    pImpl->stats.variables = std::distance(begin, end);
    
    return result;
}

std::string JavaScriptCompiler::processClasses(const std::string& js) {
    // 处理类
    std::string result = js;
    
    // 统计类数量
    std::regex classRegex(R"(class\s+\w+)");
    auto begin = std::sregex_iterator(result.begin(), result.end(), classRegex);
    auto end = std::sregex_iterator();
    
    pImpl->stats.classes = std::distance(begin, end);
    
    return result;
}

std::string JavaScriptCompiler::processImports(const std::string& js) {
    // 处理导入
    std::string result = js;
    
    // 统计导入数量
    std::regex importRegex(R"(import\s+.*?from)");
    auto begin = std::sregex_iterator(result.begin(), result.end(), importRegex);
    auto end = std::sregex_iterator();
    
    pImpl->stats.imports = std::distance(begin, end);
    
    return result;
}

std::string JavaScriptCompiler::processExports(const std::string& js) {
    // 处理导出
    std::string result = js;
    
    // 统计导出数量
    std::regex exportRegex(R"(export\s+)");
    auto begin = std::sregex_iterator(result.begin(), result.end(), exportRegex);
    auto end = std::sregex_iterator();
    
    pImpl->stats.exports = std::distance(begin, end);
    
    return result;
}

std::string JavaScriptCompiler::processExpressions(const std::string& js) {
    // 处理表达式
    std::string result = js;
    
    // 统计表达式数量（简化实现）
    pImpl->stats.expressions = 0;
    
    // 统计语句数量
    std::regex statementRegex(R"([^;{}]+[;{}])");
    auto begin = std::sregex_iterator(result.begin(), result.end(), statementRegex);
    auto end = std::sregex_iterator();
    
    pImpl->stats.totalStatements = std::distance(begin, end);
    
    return result;
}

bool JavaScriptCompiler::validateStatement(const std::string& statement) {
    // 基本的语句验证
    std::string trimmed = statement;
    
    // 移除空白字符
    trimmed.erase(std::remove_if(trimmed.begin(), trimmed.end(), ::isspace), trimmed.end());
    
    if (trimmed.empty()) {
        return true; // 空行是有效的
    }
    
    // 检查是否是有效的JavaScript语句
    // 这里可以添加更复杂的验证逻辑
    
    return true;
}

bool JavaScriptCompiler::validateExpression(const std::string& expression) {
    // 基本的表达式验证
    std::string trimmed = expression;
    
    // 移除空白字符
    trimmed.erase(std::remove_if(trimmed.begin(), trimmed.end(), ::isspace), trimmed.end());
    
    if (trimmed.empty()) {
        return false;
    }
    
    // 检查是否是有效的JavaScript表达式
    // 这里可以添加更复杂的验证逻辑
    
    return true;
}

bool JavaScriptCompiler::validateFunction(const std::string& function) {
    // 基本的函数验证
    std::string trimmed = function;
    
    // 移除空白字符
    trimmed.erase(std::remove_if(trimmed.begin(), trimmed.end(), ::isspace), trimmed.end());
    
    if (trimmed.empty()) {
        return false;
    }
    
    // 检查是否是有效的JavaScript函数
    // 这里可以添加更复杂的验证逻辑
    
    return true;
}

bool JavaScriptCompiler::validateClass(const std::string& classDef) {
    // 基本的类验证
    std::string trimmed = classDef;
    
    // 移除空白字符
    trimmed.erase(std::remove_if(trimmed.begin(), trimmed.end(), ::isspace), trimmed.end());
    
    if (trimmed.empty()) {
        return false;
    }
    
    // 检查是否是有效的JavaScript类
    // 这里可以添加更复杂的验证逻辑
    
    return true;
}

std::string JavaScriptCompiler::removeUnusedVariables(const std::string& js) {
    // 简化实现，直接返回原内容
    // 在实际实现中，这里应该进行静态分析来移除未使用的变量
    return js;
}

std::string JavaScriptCompiler::optimizeExpressions(const std::string& js) {
    // 简化实现，直接返回原内容
    // 在实际实现中，这里应该进行表达式优化
    return js;
}

std::string JavaScriptCompiler::inlineSimpleFunctions(const std::string& js) {
    // 简化实现，直接返回原内容
    // 在实际实现中，这里应该进行函数内联优化
    return js;
}

void JavaScriptCompiler::addError(const std::string& error) {
    pImpl->errors.push_back(error);
}

} // namespace chtl
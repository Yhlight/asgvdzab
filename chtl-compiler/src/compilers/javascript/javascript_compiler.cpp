#include "compilers/javascript_compiler.h"
#include <sstream>
#include <regex>
#include <algorithm>
#include <set>

namespace chtl {

// JavaScript编译器实现类
class JavaScriptCompiler::Impl {
public:
    Impl() {
        initializeReservedWords();
    }
    
    bool compile(const std::string& input, 
                std::string& output,
                std::vector<BaseCompiler::CompileError>& errors) {
        errors_ = &errors;
        reset();
        
        try {
            // 预处理JavaScript
            std::string processed = preprocessJS(input);
            
            // 验证JavaScript
            if (!validateJS(processed)) {
                return false;
            }
            
            // 转换JavaScript（如果需要）
            output = transformJS(processed);
            
            // 压缩输出（如果需要）
            if (config_.minifyOutput) {
                output = minifyJS(output);
            }
            
            // 生成source map（如果需要）
            if (config_.generateSourceMap) {
                // 简化实现，实际需要更复杂的source map生成
                generateSourceMap(input, output);
            }
            
            return errors.empty();
            
        } catch (const std::exception& e) {
            addError("JavaScript编译错误: " + std::string(e.what()), 0, 0);
            return false;
        }
    }
    
    void setOptions(const BaseCompiler::CompileOptions& options) {
        options_ = options;
    }
    
    bool validate(const std::string& input) const {
        // 基本验证：检查括号匹配
        int braceCount = 0;
        int parenCount = 0;
        int bracketCount = 0;
        bool inString = false;
        bool inComment = false;
        char stringChar = 0;
        
        for (size_t i = 0; i < input.length(); ++i) {
            char c = input[i];
            char next = (i + 1 < input.length()) ? input[i + 1] : '\0';
            
            // 处理注释
            if (!inString && !inComment && c == '/' && next == '/') {
                // 单行注释
                while (i < input.length() && input[i] != '\n') i++;
                continue;
            }
            
            if (!inString && !inComment && c == '/' && next == '*') {
                // 多行注释
                inComment = true;
                i++;
                continue;
            }
            
            if (inComment && c == '*' && next == '/') {
                inComment = false;
                i++;
                continue;
            }
            
            if (inComment) continue;
            
            // 处理字符串
            if (!inString && (c == '"' || c == '\'' || c == '`')) {
                inString = true;
                stringChar = c;
                continue;
            }
            
            if (inString && c == stringChar && (i == 0 || input[i-1] != '\\')) {
                inString = false;
                continue;
            }
            
            if (inString) continue;
            
            // 检查括号
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
    
    void setJSConfig(const JSConfig& config) {
        config_ = config;
    }
    
    JSFeatures getUsedFeatures() const {
        return features_;
    }

private:
    // 初始化保留字
    void initializeReservedWords() {
        reservedWords_ = {
            // ES5关键字
            "break", "case", "catch", "continue", "debugger", "default",
            "delete", "do", "else", "finally", "for", "function", "if",
            "in", "instanceof", "new", "return", "switch", "this", "throw",
            "try", "typeof", "var", "void", "while", "with",
            
            // ES6+关键字
            "class", "const", "let", "static", "extends", "super",
            "import", "export", "async", "await", "yield",
            
            // 保留字
            "enum", "implements", "interface", "package", "private",
            "protected", "public"
        };
    }
    
    // 重置状态
    void reset() {
        features_ = JSFeatures();
        currentLine_ = 1;
        currentColumn_ = 1;
    }
    
    // 预处理JavaScript
    std::string preprocessJS(const std::string& input) {
        std::string result = input;
        
        // 添加严格模式（如果配置）
        if (config_.strictMode && result.find("'use strict'") == std::string::npos &&
            result.find("\"use strict\"") == std::string::npos) {
            result = "'use strict';\n\n" + result;
        }
        
        return result;
    }
    
    // 验证JavaScript
    bool validateJS(const std::string& js) {
        // 检测使用的特性
        detectFeatures(js);
        
        // 基本语法检查
        if (!checkBasicSyntax(js)) {
            return false;
        }
        
        // 检查ECMAScript版本兼容性
        if (!checkECMACompatibility(js)) {
            return false;
        }
        
        return true;
    }
    
    // 检查基本语法
    bool checkBasicSyntax(const std::string& js) {
        // 简化的语法检查
        // 实际ANTLR版本会进行完整的语法分析
        
        // 检查未闭合的字符串
        std::regex unclosedStringRegex(R"((['"`])(?:[^\\]|\\.)*?$)");
        if (std::regex_search(js, unclosedStringRegex)) {
            addError("未闭合的字符串", 0, 0);
            return false;
        }
        
        // 检查无效的标识符
        std::regex invalidIdentifierRegex(R"(\b\d+[a-zA-Z_]\w*\b)");
        if (std::regex_search(js, invalidIdentifierRegex)) {
            addError("无效的标识符（不能以数字开头）", 0, 0);
            return false;
        }
        
        return true;
    }
    
    // 检查ECMAScript兼容性
    bool checkECMACompatibility(const std::string& js) {
        // 根据配置的ECMAScript版本检查特性
        
        if (config_.version < JSConfig::ES6) {
            // 检查ES6特性
            if (std::regex_search(js, std::regex(R"(\b(let|const|class|arrow|`)\b)"))) {
                addError("使用了ES6+特性，但目标版本为ES5", 0, 0);
                return false;
            }
        }
        
        return true;
    }
    
    // 转换JavaScript
    std::string transformJS(const std::string& js) {
        std::string result = js;
        
        // 如果需要转译到ES5
        if (config_.transpileToES5) {
            result = transpileToES5(result);
        }
        
        return result;
    }
    
    // 转译到ES5
    std::string transpileToES5(const std::string& js) {
        std::string result = js;
        
        // 简化的转译实现
        // 实际应该使用完整的AST转换
        
        // 转换let/const为var
        result = std::regex_replace(result, std::regex(R"(\b(let|const)\b)"), "var");
        
        // 转换箭头函数（简化版本）
        std::regex arrowFuncRegex(R"(\(([^)]*)\)\s*=>\s*\{)");
        result = std::regex_replace(result, arrowFuncRegex, "function($1) {");
        
        // 转换模板字符串（简化版本）
        std::regex templateStringRegex(R"(`([^`]*)\$\{([^}]+)\}([^`]*)`)");
        result = std::regex_replace(result, templateStringRegex, "'$1' + ($2) + '$3'");
        
        return result;
    }
    
    // 压缩JavaScript
    std::string minifyJS(const std::string& js) {
        std::string minified;
        bool inString = false;
        bool inComment = false;
        bool inRegex = false;
        char stringChar = 0;
        bool lastWasSpace = false;
        
        for (size_t i = 0; i < js.length(); ++i) {
            char c = js[i];
            char next = (i + 1 < js.length()) ? js[i + 1] : '\0';
            
            // 处理注释
            if (!inString && !inComment && c == '/' && next == '/') {
                // 跳过单行注释
                while (i < js.length() && js[i] != '\n') i++;
                continue;
            }
            
            if (!inString && !inComment && c == '/' && next == '*') {
                // 跳过多行注释
                inComment = true;
                i++; // 跳过'*'
                continue;
            }
            
            if (inComment && c == '*' && next == '/') {
                inComment = false;
                i++; // 跳过'/'
                continue;
            }
            
            if (inComment) continue;
            
            // 处理字符串
            if (!inString && (c == '"' || c == '\'' || c == '`')) {
                inString = true;
                stringChar = c;
                minified += c;
                lastWasSpace = false;
                continue;
            }
            
            if (inString && c == stringChar && (i == 0 || js[i-1] != '\\')) {
                inString = false;
                minified += c;
                lastWasSpace = false;
                continue;
            }
            
            if (inString) {
                minified += c;
                lastWasSpace = false;
                continue;
            }
            
            // 处理正则表达式（简化）
            if (!inString && c == '/' && i > 0 && isRegexContext(js, i)) {
                inRegex = true;
                minified += c;
                continue;
            }
            
            if (inRegex && c == '/' && (i == 0 || js[i-1] != '\\')) {
                inRegex = false;
                minified += c;
                // 处理正则标志
                while (i + 1 < js.length() && std::isalpha(js[i + 1])) {
                    minified += js[++i];
                }
                continue;
            }
            
            if (inRegex) {
                minified += c;
                continue;
            }
            
            // 移除多余的空白
            if (std::isspace(c)) {
                if (!lastWasSpace && !minified.empty() && 
                    needsSpaceAfter(minified.back()) && 
                    i + 1 < js.length() && needsSpaceBefore(js[i + 1])) {
                    minified += ' ';
                    lastWasSpace = true;
                }
            } else {
                minified += c;
                lastWasSpace = false;
            }
        }
        
        return minified;
    }
    
    // 检测使用的特性
    void detectFeatures(const std::string& js) {
        // 检测async/await
        if (std::regex_search(js, std::regex(R"(\basync\s+function\b|\basync\s*\(|\bawait\b)"))) {
            features_.usesAsync = true;
        }
        
        // 检测生成器
        if (std::regex_search(js, std::regex(R"(\bfunction\s*\*|\byield\b)"))) {
            features_.usesGenerators = true;
        }
        
        // 检测类
        if (std::regex_search(js, std::regex(R"(\bclass\s+\w+)"))) {
            features_.usesClasses = true;
        }
        
        // 检测箭头函数
        if (std::regex_search(js, std::regex(R"(=>\s*[{(])"))) {
            features_.usesArrowFunctions = true;
        }
        
        // 检测解构赋值
        if (std::regex_search(js, std::regex(R"(\b(const|let|var)\s*\{|\b(const|let|var)\s*\[)"))) {
            features_.usesDestructuring = true;
        }
        
        // 检测模块
        if (std::regex_search(js, std::regex(R"(\b(import|export)\s+)"))) {
            features_.usesModules = true;
        }
        
        // 检测Promise
        if (std::regex_search(js, std::regex(R"(\bnew\s+Promise\b|\bPromise\s*\.)"))) {
            features_.usesPromises = true;
        }
        
        // 检测全局变量（简化版本）
        // 实际应该通过AST分析
        std::regex globalVarRegex(R"(\b(?:var|let|const)\s+(\w+)\s*[=;])");
        std::smatch match;
        std::string remaining = js;
        
        while (std::regex_search(remaining, match, globalVarRegex)) {
            if (isGlobalScope(js, match.position())) {
                features_.globalVariables.push_back(match[1].str());
            }
            remaining = match.suffix();
        }
    }
    
    // 生成source map
    void generateSourceMap(const std::string& original, const std::string& compiled) {
        // 简化的source map生成
        // 实际实现需要更复杂的映射逻辑
        sourceMap_ = {
            {"version", "3"},
            {"sources", {"input.js"}},
            {"mappings", "AAAA"}  // 简化的映射
        };
    }
    
    // 工具函数
    bool isRegexContext(const std::string& js, size_t pos) const {
        // 简化的正则表达式上下文检测
        // 检查前面是否是 = ( [ , ; : ! & | ? + - * / % ^ ~ 
        if (pos == 0) return true;
        
        size_t i = pos - 1;
        while (i > 0 && std::isspace(js[i])) i--;
        
        char c = js[i];
        return c == '=' || c == '(' || c == '[' || c == ',' || 
               c == ';' || c == ':' || c == '!' || c == '&' || 
               c == '|' || c == '?' || c == '+' || c == '-' || 
               c == '*' || c == '/' || c == '%' || c == '^' || 
               c == '~';
    }
    
    bool needsSpaceAfter(char c) const {
        return std::isalnum(c) || c == '_' || c == '$' || c == ')' || c == ']' || c == '}';
    }
    
    bool needsSpaceBefore(char c) const {
        return std::isalnum(c) || c == '_' || c == '$' || c == '(' || c == '[' || c == '{';
    }
    
    bool isGlobalScope(const std::string& js, size_t pos) const {
        // 简化的全局作用域检测
        // 实际应该通过AST分析
        int braceCount = 0;
        for (size_t i = 0; i < pos && i < js.length(); ++i) {
            if (js[i] == '{') braceCount++;
            else if (js[i] == '}') braceCount--;
        }
        return braceCount == 0;
    }
    
    void addError(const std::string& message, size_t line, size_t column) {
        BaseCompiler::CompileError error;
        error.severity = BaseCompiler::CompileError::ERROR;
        error.message = message;
        error.line = line;
        error.column = column;
        if (errors_) {
            errors_->push_back(error);
        }
    }

private:
    BaseCompiler::CompileOptions options_;
    JSConfig config_;
    
    std::vector<BaseCompiler::CompileError>* errors_;
    JSFeatures features_;
    
    std::set<std::string> reservedWords_;
    std::map<std::string, std::string> sourceMap_;
    
    size_t currentLine_;
    size_t currentColumn_;
};

// JavaScriptCompiler 实现
JavaScriptCompiler::JavaScriptCompiler()
    : pImpl(std::make_unique<Impl>()) {}

JavaScriptCompiler::~JavaScriptCompiler() = default;

bool JavaScriptCompiler::compile(const std::string& input, 
                               std::string& output,
                               std::vector<CompileError>& errors) {
    return pImpl->compile(input, output, errors);
}

void JavaScriptCompiler::setOptions(const CompileOptions& options) {
    pImpl->setOptions(options);
}

bool JavaScriptCompiler::validate(const std::string& input) const {
    return pImpl->validate(input);
}

void JavaScriptCompiler::setJSConfig(const JSConfig& config) {
    pImpl->setJSConfig(config);
}

JavaScriptCompiler::JSFeatures JavaScriptCompiler::getUsedFeatures() const {
    return pImpl->getUsedFeatures();
}

} // namespace chtl
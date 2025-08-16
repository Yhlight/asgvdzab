#include "compilers/js_compiler.hpp"
#include "dispatcher/compiler_dispatcher.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <regex>
#include <iostream>

namespace chtl {

// JSCompiler::Impl
struct JSCompiler::Impl {
    std::shared_ptr<DependencyManager> dependencyManager;
    std::unique_ptr<NodeJSProcessManager> nodeManager;
    bool initialized = false;

    Impl(std::shared_ptr<DependencyManager> depMgr) 
        : dependencyManager(depMgr) {
        // 尝试找到Node.js
        std::string nodePath = "node";
        if (depMgr && depMgr->isDependencyAvailable("node")) {
            nodePath = depMgr->getDependencyPath("node") + "/bin/node";
        }
        nodeManager = std::make_unique<NodeJSProcessManager>(nodePath);
    }
};

// JSCompiler实现
JSCompiler::JSCompiler(std::shared_ptr<DependencyManager> dependencyManager)
    : pImpl_(std::make_unique<Impl>(dependencyManager)) {
}

JSCompiler::~JSCompiler() = default;

JSCompileResult JSCompiler::compile(const std::string& source, const JSCompileOptions& options) {
    if (!pImpl_->initialized) {
        // 在未初始化时使用回退模式
        std::string processedJS = source;
        if (options.minify) {
            processedJS = JSUtils::minifyJS(processedJS, options.preserveComments);
        }
        
        JSCompileResult result;
        result.success = true;
        result.javascript = processedJS;
        result.originalSize = source.length();
        result.compiledSize = processedJS.length();
        result.targetVersion = options.targetVersion;
        return result;
    }

    // 检测语法类型
    std::string syntaxType = detectSyntaxType(source);
    
    JSCompileResult result;
    result.originalSize = source.length();
    
    // 根据语法类型选择编译方式
    if (syntaxType == "ts" || syntaxType == "tsx") {
        result = compileWithTypeScript(source, options);
    } else {
        result = compileWithBabel(source, options);
    }

    // 后处理：压缩
    if (result.success && options.minify) {
        result = minifyWithTerser(result.javascript, options);
    }

    result.targetVersion = options.targetVersion;
    return result;
}

JSCompileResult JSCompiler::compileFile(const std::string& inputFile, 
                                       const std::string& outputFile,
                                       const JSCompileOptions& options) {
    // 读取输入文件
    std::ifstream file(inputFile);
    if (!file) {
        return {false, "", "", "Cannot read input file: " + inputFile, {}, 0, 0, ""};
    }

    std::string source((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    file.close();

    // 编译
    auto result = compile(source, options);
    
    if (result.success && !outputFile.empty()) {
        // 写入输出文件
        std::ofstream outFile(outputFile);
        if (outFile) {
            outFile << result.javascript;
            outFile.close();
        } else {
            result.success = false;
            result.errorMessage = "Cannot write output file: " + outputFile;
        }
    }

    return result;
}

JSCompileResult JSCompiler::compileTypeScript(const std::string& source, const JSCompileOptions& options) {
    return compileWithTypeScript(source, options);
}

std::string JSCompiler::detectSyntaxType(const std::string& source) {
    // 检测TypeScript特征
    if (source.find(": ") != std::string::npos ||  // 类型注解
        source.find("interface ") != std::string::npos ||
        source.find("type ") != std::string::npos ||
        source.find("enum ") != std::string::npos) {
        
        // 检测是否是TSX
        if (source.find("<") != std::string::npos && source.find("/>") != std::string::npos) {
            return "tsx";
        }
        return "ts";
    }
    
    // 检测JSX特征
    if (source.find("<") != std::string::npos && 
        (source.find("/>") != std::string::npos || source.find("</") != std::string::npos)) {
        return "jsx";
    }

    return "js";
}

JSLintResult JSCompiler::validateSyntax(const std::string& source) {
    JSLintResult result;
    result.isValid = true;
    
    // 简单的语法检查
    int braceCount = 0;
    int parenCount = 0;
    int bracketCount = 0;
    bool inString = false;
    char stringChar = '\0';
    
    for (size_t i = 0; i < source.length(); i++) {
        char c = source[i];
        
        if (!inString) {
            if (c == '"' || c == '\'' || c == '`') {
                inString = true;
                stringChar = c;
            } else if (c == '{') {
                braceCount++;
            } else if (c == '}') {
                braceCount--;
                if (braceCount < 0) {
                    result.isValid = false;
                    result.errors.push_back("Unmatched closing brace at position " + std::to_string(i));
                }
            } else if (c == '(') {
                parenCount++;
            } else if (c == ')') {
                parenCount--;
                if (parenCount < 0) {
                    result.isValid = false;
                    result.errors.push_back("Unmatched closing parenthesis at position " + std::to_string(i));
                }
            } else if (c == '[') {
                bracketCount++;
            } else if (c == ']') {
                bracketCount--;
                if (bracketCount < 0) {
                    result.isValid = false;
                    result.errors.push_back("Unmatched closing bracket at position " + std::to_string(i));
                }
            }
        } else {
            if (c == stringChar && (i == 0 || source[i-1] != '\\')) {
                inString = false;
            }
        }
    }
    
    if (braceCount != 0) {
        result.isValid = false;
        result.errors.push_back("Unmatched braces");
    }
    if (parenCount != 0) {
        result.isValid = false;
        result.errors.push_back("Unmatched parentheses");
    }
    if (bracketCount != 0) {
        result.isValid = false;
        result.errors.push_back("Unmatched brackets");
    }
    
    return result;
}

std::vector<JSProcessType> JSCompiler::getSupportedProcessTypes() const {
    return {
        JSProcessType::PARSE_ONLY,
        JSProcessType::MINIFY,
        JSProcessType::TRANSPILE_ES5,
        JSProcessType::TRANSPILE_ES6,
        JSProcessType::TYPESCRIPT_COMPILE,
        JSProcessType::BABEL_TRANSFORM
    };
}

std::future<bool> JSCompiler::initialize(ProgressCallback callback) {
    return std::async(std::launch::async, [this, callback]() -> bool {
        if (callback) {
            callback("js-compiler", 0.0, "Initializing JS compiler");
        }

        // 确保依赖可用
        if (!ensureDependenciesAvailable()) {
            if (callback) {
                callback("js-compiler", 100.0, "Failed to ensure dependencies");
            }
            return false;
        }

        if (callback) {
            callback("js-compiler", 50.0, "Dependencies ready");
        }

        // 检查Node.js
        if (!pImpl_->nodeManager->isNodeJSAvailable()) {
            if (callback) {
                callback("js-compiler", 100.0, "Node.js not available");
            }
            return false;
        }

        pImpl_->initialized = true;
        
        if (callback) {
            callback("js-compiler", 100.0, "JS compiler initialized");
        }
        
        return true;
    });
}

JSCompileResult JSCompiler::bundle(const std::vector<std::string>& entryFiles,
                                  const std::string& outputFile,
                                  const JSCompileOptions& options) {
    return bundleWithWebpack(entryFiles, outputFile, options);
}

std::vector<CodeSegmentType> JSCompiler::getSupportedTypes() const {
    return {
        CodeSegmentType::JAVASCRIPT_STANDARD,
        CodeSegmentType::JAVASCRIPT_ORIGIN
    };
}

CompileResult JSCompiler::compile(const CodeSegment& segment, const CompileConfig& config) {
    JSCompileOptions options;
    options.minify = true;  // 默认压缩
    options.targetVersion = "es2020";

    auto result = compile(segment.content, options);
    
    CompileResult compileResult;
    compileResult.success = result.success;
    compileResult.output = result.javascript;
    if (!result.errorMessage.empty()) {
        compileResult.errors.push_back(result.errorMessage);
    }
    compileResult.warnings = result.warnings;
    
    return compileResult;
}

JSCompileResult JSCompiler::compileWithBabel(const std::string& source, const JSCompileOptions& options) {
    // 如果Babel不可用，回退到基本处理
    if (!pImpl_->dependencyManager->isDependencyAvailable("babel-core")) {
        JSCompileResult result;
        result.success = true;
        result.javascript = source;
        result.originalSize = source.length();
        result.compiledSize = source.length();
        result.targetVersion = options.targetVersion;
        return result;
    }

    // 这里应该调用Babel进行转译
    // 简化实现：直接返回源代码
    JSCompileResult result;
    result.success = true;
    result.javascript = source;
    result.originalSize = source.length();
    result.compiledSize = source.length();
    result.targetVersion = options.targetVersion;
    
    return result;
}

JSCompileResult JSCompiler::compileWithTypeScript(const std::string& source, const JSCompileOptions& options) {
    if (!pImpl_->dependencyManager->isDependencyAvailable("typescript")) {
        // 回退到简化的TypeScript编译
        std::string result = source;
        
        // 移除接口定义
        size_t interfacePos = result.find("interface ");
        while (interfacePos != std::string::npos) {
            size_t braceStart = result.find('{', interfacePos);
            if (braceStart != std::string::npos) {
                int braceCount = 1;
                size_t pos = braceStart + 1;
                while (pos < result.length() && braceCount > 0) {
                    if (result[pos] == '{') braceCount++;
                    else if (result[pos] == '}') braceCount--;
                    pos++;
                }
                result.erase(interfacePos, pos - interfacePos);
            } else {
                break;
            }
            interfacePos = result.find("interface ", interfacePos);
        }
        
        JSCompileResult compileResult;
        compileResult.success = true;
        compileResult.javascript = result;
        compileResult.originalSize = source.length();
        compileResult.compiledSize = result.length();
        compileResult.targetVersion = options.targetVersion;
        
        return compileResult;
    }

    // 这里应该调用TypeScript编译器
    // 简化实现：移除类型注解
    std::string result = source;
    
    // 简单地移除一些TypeScript语法（避免复杂正则表达式）
    // 移除接口定义
    size_t interfacePos = result.find("interface ");
    while (interfacePos != std::string::npos) {
        size_t braceStart = result.find('{', interfacePos);
        if (braceStart != std::string::npos) {
            int braceCount = 1;
            size_t pos = braceStart + 1;
            while (pos < result.length() && braceCount > 0) {
                if (result[pos] == '{') braceCount++;
                else if (result[pos] == '}') braceCount--;
                pos++;
            }
            result.erase(interfacePos, pos - interfacePos);
        } else {
            break;
        }
        interfacePos = result.find("interface ", interfacePos);
    }
    
    JSCompileResult compileResult;
    compileResult.success = true;
    compileResult.javascript = result;
    compileResult.originalSize = source.length();
    compileResult.compiledSize = result.length();
    compileResult.targetVersion = options.targetVersion;
    
    return compileResult;
}

JSCompileResult JSCompiler::minifyWithTerser(const std::string& source, const JSCompileOptions& options) {
    if (!pImpl_->dependencyManager->isDependencyAvailable("terser")) {
        // 回退到基本压缩
        std::string minified = JSUtils::minifyJS(source, options.preserveComments);
        
        JSCompileResult result;
        result.success = true;
        result.javascript = minified;
        result.originalSize = source.length();
        result.compiledSize = minified.length();
        result.targetVersion = options.targetVersion;
        
        return result;
    }

    // 这里应该调用Terser进行压缩
    // 简化实现：使用内置压缩
    std::string minified = JSUtils::minifyJS(source, options.preserveComments);
    
    JSCompileResult result;
    result.success = true;
    result.javascript = minified;
    result.originalSize = source.length();
    result.compiledSize = minified.length();
    result.targetVersion = options.targetVersion;
    
    return result;
}

JSCompileResult JSCompiler::bundleWithWebpack(const std::vector<std::string>& entryFiles,
                                             const std::string& outputFile,
                                             const JSCompileOptions& options) {
    // Webpack打包实现
    return {false, "", "", "Webpack bundling not implemented yet", {}, 0, 0, ""};
}

std::string JSCompiler::generateCompilerScript(JSProcessType type, const JSCompileOptions& options) {
    std::ostringstream script;
    
    switch (type) {
        case JSProcessType::BABEL_TRANSFORM:
            script << "const babel = require('@babel/core');\n";
            script << "const result = babel.transformSync(process.argv[2], {\n";
            script << "  presets: [";
            for (size_t i = 0; i < options.presets.size(); i++) {
                if (i > 0) script << ", ";
                script << "'" << options.presets[i] << "'";
            }
            script << "],\n";
            script << "  plugins: [";
            for (size_t i = 0; i < options.plugins.size(); i++) {
                if (i > 0) script << ", ";
                script << "'" << options.plugins[i] << "'";
            }
            script << "]\n";
            script << "});\n";
            script << "console.log(result.code);\n";
            break;
            
        case JSProcessType::TYPESCRIPT_COMPILE:
            script << "const ts = require('typescript');\n";
            script << "const result = ts.transpile(process.argv[2], {\n";
            script << "  target: ts.ScriptTarget.ES2020,\n";
            script << "  module: ts.ModuleKind.CommonJS\n";
            script << "});\n";
            script << "console.log(result);\n";
            break;
            
        default:
            script << "console.log(process.argv[2]);\n";
            break;
    }
    
    return script.str();
}

bool JSCompiler::ensureDependenciesAvailable() {
    if (!pImpl_->dependencyManager) {
        return false;
    }

    // 检查Node.js
    if (!pImpl_->dependencyManager->isDependencyAvailable("node")) {
        return false;
    }

    // 其他依赖是可选的
    return true;
}

// JSUtils实现
std::string JSUtils::minifyJS(const std::string& js, bool preserveComments) {
    std::string result = js;
    
    // 简化实现，避免正则表达式问题
    // 移除多余的空白
    std::string temp;
    bool inString = false;
    char stringChar = '\0';
    bool lastWasSpace = false;
    
    for (size_t i = 0; i < result.length(); i++) {
        char c = result[i];
        
        if (!inString && (c == '"' || c == '\'' || c == '`')) {
            inString = true;
            stringChar = c;
            temp += c;
            lastWasSpace = false;
        } else if (inString && c == stringChar && (i == 0 || result[i-1] != '\\')) {
            inString = false;
            temp += c;
            lastWasSpace = false;
        } else if (!inString && std::isspace(c)) {
            if (!lastWasSpace && !temp.empty() && 
                temp.back() != '{' && temp.back() != '}' && 
                temp.back() != ';' && temp.back() != ',' && 
                temp.back() != '(' && temp.back() != ')') {
                temp += ' ';
                lastWasSpace = true;
            }
        } else {
            temp += c;
            lastWasSpace = false;
        }
    }
    
    // 移除不必要的空格
    result = temp;
    temp.clear();
    
    for (size_t i = 0; i < result.length(); i++) {
        char c = result[i];
        if (c == ' ') {
            if (i > 0 && i < result.length() - 1) {
                char prev = result[i-1];
                char next = result[i+1];
                if (prev == '{' || prev == '}' || prev == ';' || prev == ',' || 
                    prev == '(' || prev == ')' ||
                    next == '{' || next == '}' || next == ';' || next == ',' ||
                    next == '(' || next == ')') {
                    continue; // 跳过这个空格
                }
            }
        }
        temp += c;
    }
    
    return temp;
}

std::string JSUtils::beautifyJS(const std::string& js, const std::string& indent) {
    // 简化的JavaScript美化
    std::string result = js;
    int indentLevel = 0;
    std::ostringstream formatted;
    
    for (size_t i = 0; i < result.length(); i++) {
        char c = result[i];
        
        if (c == '{') {
            formatted << c << '\n';
            indentLevel++;
            for (int j = 0; j < indentLevel; j++) {
                formatted << indent;
            }
        } else if (c == '}') {
            formatted << '\n';
            indentLevel--;
            for (int j = 0; j < indentLevel; j++) {
                formatted << indent;
            }
            formatted << c << '\n';
            if (indentLevel > 0) {
                for (int j = 0; j < indentLevel; j++) {
                    formatted << indent;
                }
            }
        } else if (c == ';') {
            formatted << c << '\n';
            for (int j = 0; j < indentLevel; j++) {
                formatted << indent;
            }
        } else {
            formatted << c;
        }
    }
    
    return formatted.str();
}

std::vector<std::string> JSUtils::extractFunctions(const std::string& js) {
    std::vector<std::string> functions;
    size_t pos = 0;
    
    while ((pos = js.find("function ", pos)) != std::string::npos) {
        pos += 9; // "function ".length()
        
        // 跳过空白
        while (pos < js.length() && std::isspace(js[pos])) pos++;
        
        // 提取函数名
        size_t nameStart = pos;
        while (pos < js.length() && (std::isalnum(js[pos]) || js[pos] == '_')) pos++;
        
        if (pos > nameStart) {
            functions.push_back(js.substr(nameStart, pos - nameStart));
        }
    }
    
    return functions;
}

std::vector<std::string> JSUtils::extractVariables(const std::string& js) {
    std::vector<std::string> variables;
    std::vector<std::string> keywords = {"var ", "let ", "const "};
    
    for (const auto& keyword : keywords) {
        size_t pos = 0;
        while ((pos = js.find(keyword, pos)) != std::string::npos) {
            pos += keyword.length();
            
            // 跳过空白
            while (pos < js.length() && std::isspace(js[pos])) pos++;
            
            // 提取变量名
            size_t nameStart = pos;
            while (pos < js.length() && (std::isalnum(js[pos]) || js[pos] == '_')) pos++;
            
            if (pos > nameStart) {
                variables.push_back(js.substr(nameStart, pos - nameStart));
            }
        }
    }
    
    return variables;
}

bool JSUtils::isValidSyntax(const std::string& js) {
    // 简单的语法检查
    int braceCount = 0;
    int parenCount = 0;
    bool inString = false;
    char stringChar = '\0';
    
    for (char c : js) {
        if (!inString) {
            if (c == '"' || c == '\'' || c == '`') {
                inString = true;
                stringChar = c;
            } else if (c == '{') {
                braceCount++;
            } else if (c == '}') {
                braceCount--;
                if (braceCount < 0) return false;
            } else if (c == '(') {
                parenCount++;
            } else if (c == ')') {
                parenCount--;
                if (parenCount < 0) return false;
            }
        } else {
            if (c == stringChar) {
                inString = false;
            }
        }
    }
    
    return braceCount == 0 && parenCount == 0;
}

std::string JSUtils::detectESVersion(const std::string& js) {
    // 检测ES特征
    if (js.find("class ") != std::string::npos ||
        js.find("const ") != std::string::npos ||
        js.find("let ") != std::string::npos ||
        js.find("=>") != std::string::npos) {
        
        if (js.find("async ") != std::string::npos ||
            js.find("await ") != std::string::npos) {
            return "es2017";
        }
        
        return "es6";
    }
    
    return "es5";
}

std::vector<std::string> JSUtils::extractDependencies(const std::string& js) {
    std::vector<std::string> dependencies;
    
    // 提取require语句
    std::regex requireRegex(R"(require\s*\(\s*['"`]([^'"`]+)['"`]\s*\))");
    std::sregex_iterator iter(js.begin(), js.end(), requireRegex);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        dependencies.push_back(iter->str(1));
    }
    
    // 提取import语句
    std::regex importRegex(R"(import\s+.*?\s+from\s+['"`]([^'"`]+)['"`])");
    iter = std::sregex_iterator(js.begin(), js.end(), importRegex);
    
    for (; iter != end; ++iter) {
        dependencies.push_back(iter->str(1));
    }
    
    return dependencies;
}

std::string JSUtils::convertModuleSystem(const std::string& js, 
                                        const std::string& fromSystem,
                                        const std::string& toSystem) {
    // 简化的模块系统转换
    std::string result = js;
    
    if (fromSystem == "commonjs" && toSystem == "es6") {
        // 转换require为import
        result = std::regex_replace(result, 
            std::regex(R"(const\s+(\w+)\s*=\s*require\s*\(\s*['"`]([^'"`]+)['"`]\s*\))"),
            "import $1 from '$2'");
        
        // 转换module.exports为export default
        result = std::regex_replace(result, 
            std::regex(R"(module\.exports\s*=\s*)"),
            "export default ");
    }
    
    return result;
}

std::string JSUtils::addStrictMode(const std::string& js) {
    if (js.find("'use strict'") == std::string::npos &&
        js.find("\"use strict\"") == std::string::npos) {
        return "'use strict';\n" + js;
    }
    return js;
}

std::string JSUtils::removeDebugStatements(const std::string& js) {
    std::string result = js;
    
    // 移除console.log语句
    result = std::regex_replace(result, std::regex(R"(console\.log\s*\([^)]*\)\s*;?)"), "");
    
    // 移除debugger语句
    result = std::regex_replace(result, std::regex(R"(debugger\s*;?)"), "");
    
    return result;
}

// 配置生成器实现（简化版本）
std::string WebpackConfigGenerator::generateBasicConfig(const std::vector<std::string>& entryFiles,
                                                       const std::string& outputFile,
                                                       const JSCompileOptions& options) {
    std::ostringstream config;
    
    config << "{\n";
    config << "  \"entry\": [";
    for (size_t i = 0; i < entryFiles.size(); i++) {
        if (i > 0) config << ", ";
        config << "\"" << entryFiles[i] << "\"";
    }
    config << "],\n";
    config << "  \"output\": {\n";
    config << "    \"filename\": \"" << outputFile << "\"\n";
    config << "  },\n";
    config << "  \"mode\": \"" << (options.minify ? "production" : "development") << "\"\n";
    config << "}\n";
    
    return config.str();
}

bool WebpackConfigGenerator::generateConfigFile(const std::string& configPath,
                                               const std::vector<std::string>& entryFiles,
                                               const std::string& outputFile,
                                               const JSCompileOptions& options) {
    std::string config = generateBasicConfig(entryFiles, outputFile, options);
    
    std::ofstream file(configPath);
    if (file) {
        file << config;
        return true;
    }
    
    return false;
}

std::string BabelConfigGenerator::generateConfig(const JSCompileOptions& options) {
    std::ostringstream config;
    
    config << "{\n";
    config << "  \"presets\": [";
    for (size_t i = 0; i < options.presets.size(); i++) {
        if (i > 0) config << ", ";
        config << "\"" << options.presets[i] << "\"";
    }
    config << "],\n";
    config << "  \"plugins\": [";
    for (size_t i = 0; i < options.plugins.size(); i++) {
        if (i > 0) config << ", ";
        config << "\"" << options.plugins[i] << "\"";
    }
    config << "]\n";
    config << "}\n";
    
    return config.str();
}

bool BabelConfigGenerator::generateConfigFile(const std::string& configPath, const JSCompileOptions& options) {
    std::string config = generateConfig(options);
    
    std::ofstream file(configPath);
    if (file) {
        file << config;
        return true;
    }
    
    return false;
}

std::vector<std::string> BabelConfigGenerator::getRecommendedPresets(const std::string& targetVersion) {
    if (targetVersion == "es5") {
        return {"@babel/preset-env"};
    } else if (targetVersion == "es6" || targetVersion == "es2015") {
        return {"@babel/preset-env"};
    }
    
    return {"@babel/preset-env"};
}

} // namespace chtl
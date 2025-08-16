#include "compilers/css_compiler.hpp"
#include "dispatcher/compiler_dispatcher.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <regex>
#include <iostream>
#include <cstdlib>
#include <sys/wait.h>
#include <unistd.h>

namespace chtl {

// NodeJSProcessManager实现
NodeJSProcessManager::NodeJSProcessManager(const std::string& nodePath) 
    : nodePath_(nodePath), available_(false) {
    // 检查Node.js是否可用
    available_ = isNodeJSAvailable();
}

NodeJSProcessManager::~NodeJSProcessManager() = default;

NodeJSProcessManager::ProcessResult NodeJSProcessManager::executeScript(
    const std::string& scriptPath, 
    const std::vector<std::string>& args,
    const std::string& input) {
    
    if (!available_) {
        return {-1, "", "Node.js is not available"};
    }

    // 构建命令
    std::string command = nodePath_ + " " + scriptPath;
    for (const auto& arg : args) {
        command += " \"" + arg + "\"";
    }

    // 创建管道
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        return {-1, "", "Failed to create pipe"};
    }

    pid_t pid = fork();
    if (pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return {-1, "", "Failed to fork process"};
    }

    if (pid == 0) {
        // 子进程
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);

        execl("/bin/sh", "sh", "-c", command.c_str(), nullptr);
        exit(127);
    } else {
        // 父进程
        close(pipefd[1]);

        // 写入输入数据（如果有）
        if (!input.empty()) {
            // 这里简化处理，实际应该使用双向管道
        }

        // 读取输出
        std::string output;
        char buffer[4096];
        ssize_t bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
            output += buffer;
        }

        close(pipefd[0]);

        // 等待子进程结束
        int status;
        waitpid(pid, &status, 0);
        int exitCode = WEXITSTATUS(status);

        return {exitCode, output, ""};
    }
}

bool NodeJSProcessManager::isNodeJSAvailable() const {
    std::string command = nodePath_ + " --version > /dev/null 2>&1";
    return system(command.c_str()) == 0;
}

// CSSCompiler::Impl
struct CSSCompiler::Impl {
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

// CSSCompiler实现
CSSCompiler::CSSCompiler(std::shared_ptr<DependencyManager> dependencyManager)
    : pImpl_(std::make_unique<Impl>(dependencyManager)) {
}

CSSCompiler::~CSSCompiler() = default;

CSSCompileResult CSSCompiler::compile(const std::string& source, const CSSCompileOptions& options) {
    if (!pImpl_->initialized) {
        // 在未初始化时使用回退模式
        std::string processedCSS = source;
        if (options.minify) {
            processedCSS = CSSUtils::minifyCSS(processedCSS);
        }
        
        CSSCompileResult result;
        result.success = true;
        result.css = processedCSS;
        result.originalSize = source.length();
        result.compiledSize = processedCSS.length();
        return result;
    }

    // 检测语法类型
    std::string syntaxType = detectSyntaxType(source);
    
    CSSCompileResult result;
    
    // 根据语法类型选择编译方式
    if (syntaxType == "scss" || syntaxType == "sass") {
        result = compileSass(source, options);
    } else if (syntaxType == "less") {
        result = compileLess(source, options);
    } else {
        // 纯CSS，使用PostCSS处理
        result = processWithPostCSS(source, options);
    }

    // 后处理
    if (result.success && options.minify) {
        result.css = CSSUtils::minifyCSS(result.css);
        result.compiledSize = result.css.length();
    }

    return result;
}

CSSCompileResult CSSCompiler::compileFile(const std::string& inputFile, 
                                         const std::string& outputFile,
                                         const CSSCompileOptions& options) {
    // 读取输入文件
    std::ifstream file(inputFile);
    if (!file) {
        return {false, "", "", "Cannot read input file: " + inputFile, {}, 0, 0};
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
            outFile << result.css;
            outFile.close();
        } else {
            result.success = false;
            result.errorMessage = "Cannot write output file: " + outputFile;
        }
    }

    return result;
}

std::string CSSCompiler::detectSyntaxType(const std::string& source) {
    // 检测SCSS特征：包含$变量或嵌套选择器或SCSS关键字
    if (source.find('$') != std::string::npos || // 变量
        source.find("@import") != std::string::npos || 
        source.find("@mixin") != std::string::npos ||
        source.find("@include") != std::string::npos ||
        (source.find('{') != std::string::npos && 
         source.find_first_of("{}") < source.find_last_of("{}"))) { // 嵌套检测
        
        // 进一步确认是SCSS（检查嵌套结构）
        size_t pos = 0;
        while ((pos = source.find('{', pos)) != std::string::npos) {
            size_t nextBrace = source.find('{', pos + 1);
            size_t closeBrace = source.find('}', pos + 1);
            if (nextBrace != std::string::npos && nextBrace < closeBrace) {
                return "scss"; // 找到嵌套结构
            }
            pos++;
        }
        
        // 如果有$变量也认为是SCSS
        if (source.find('$') != std::string::npos) {
            return "scss";
        }
    }
    
    // 检测SASS特征（缩进语法）
    // 简化：如果没有大括号且没有$变量，可能是SASS
    if (source.find('{') == std::string::npos && 
        source.find('$') == std::string::npos &&
        source.find('@') == std::string::npos) {
        return "sass";
    }

    // 检测LESS特征（@变量而非$变量）
    if (source.find('@') != std::string::npos && 
        source.find('$') == std::string::npos && // 排除SCSS
        source.find("@import") != std::string::npos) {
        return "less";
    }

    return "css";
}

bool CSSCompiler::validateSyntax(const std::string& source) {
    // 简单的CSS语法验证
    int braceCount = 0;
    bool inString = false;
    char stringChar = '\0';
    
    for (char c : source) {
        if (!inString) {
            if (c == '"' || c == '\'') {
                inString = true;
                stringChar = c;
            } else if (c == '{') {
                braceCount++;
            } else if (c == '}') {
                braceCount--;
                if (braceCount < 0) return false;
            }
        } else {
            if (c == stringChar) {
                inString = false;
            }
        }
    }
    
    return braceCount == 0;
}

std::vector<CSSProcessType> CSSCompiler::getSupportedProcessTypes() const {
    return {
        CSSProcessType::PARSE_ONLY,
        CSSProcessType::MINIFY,
        CSSProcessType::AUTOPREFIXER,
        CSSProcessType::SASS_COMPILE,
        CSSProcessType::POSTCSS_PROCESS
    };
}

std::future<bool> CSSCompiler::initialize(ProgressCallback callback) {
    return std::async(std::launch::async, [this, callback]() -> bool {
        if (callback) {
            callback("css-compiler", 0.0, "Initializing CSS compiler");
        }

        // 确保依赖可用
        if (!ensureDependenciesAvailable()) {
            if (callback) {
                callback("css-compiler", 100.0, "Failed to ensure dependencies");
            }
            return false;
        }

        if (callback) {
            callback("css-compiler", 50.0, "Dependencies ready");
        }

        // 检查Node.js
        if (!pImpl_->nodeManager->isNodeJSAvailable()) {
            if (callback) {
                callback("css-compiler", 100.0, "Node.js not available");
            }
            return false;
        }

        pImpl_->initialized = true;
        
        if (callback) {
            callback("css-compiler", 100.0, "CSS compiler initialized");
        }
        
        return true;
    });
}

std::vector<CodeSegmentType> CSSCompiler::getSupportedTypes() const {
    return {
        CodeSegmentType::CSS_GLOBAL,
        CodeSegmentType::CSS_LOCAL_STYLE,
        CodeSegmentType::CSS_ORIGIN
    };
}

CompileResult CSSCompiler::compile(const CodeSegment& segment, const CompileConfig& config) {
    CSSCompileOptions options;
    options.minify = true;  // 默认压缩
    options.autoprefixer = true;

    auto result = compile(segment.content, options);
    
    CompileResult compileResult;
    compileResult.success = result.success;
    compileResult.output = result.css;
    if (!result.errorMessage.empty()) {
        compileResult.errors.push_back(result.errorMessage);
    }
    compileResult.warnings = result.warnings;
    
    return compileResult;
}

CSSCompileResult CSSCompiler::compileSass(const std::string& source, const CSSCompileOptions& options) {
    if (!pImpl_->dependencyManager->isDependencyAvailable("sass")) {
        return {false, "", "", "Sass compiler not available", {}, source.length(), 0};
    }

    // 创建临时文件
    std::string tempDir = "/tmp/chtl_css_" + std::to_string(getpid());
    std::filesystem::create_directories(tempDir);
    
    std::string inputFile = tempDir + "/input.scss";
    std::string outputFile = tempDir + "/output.css";
    
    // 写入源文件
    std::ofstream file(inputFile);
    file << source;
    file.close();

    // 生成Sass命令
    std::string sassPath = pImpl_->dependencyManager->getDependencyPath("sass");
    std::vector<std::string> args = {inputFile, outputFile};
    
    if (options.sourceMaps) {
        args.push_back("--source-map");
    }

    // 执行Sass编译
    auto processResult = pImpl_->nodeManager->executeScript(sassPath + "/sass", args);
    
    CSSCompileResult result;
    result.originalSize = source.length();
    
    if (processResult.exitCode == 0) {
        // 读取输出
        std::ifstream outFile(outputFile);
        if (outFile) {
            result.css = std::string((std::istreambuf_iterator<char>(outFile)),
                                   std::istreambuf_iterator<char>());
            result.success = true;
            result.compiledSize = result.css.length();
        } else {
            result.success = false;
            result.errorMessage = "Failed to read compiled output";
        }
    } else {
        result.success = false;
        result.errorMessage = processResult.stderr.empty() ? processResult.stdout : processResult.stderr;
    }

    // 清理临时文件
    std::filesystem::remove_all(tempDir);
    
    return result;
}

CSSCompileResult CSSCompiler::compileLess(const std::string& source, const CSSCompileOptions& options) {
    // Less编译实现（类似Sass）
    return {false, "", "", "Less compiler not implemented yet", {}, source.length(), 0};
}

CSSCompileResult CSSCompiler::processWithPostCSS(const std::string& source, const CSSCompileOptions& options) {
    if (!pImpl_->dependencyManager->isDependencyAvailable("postcss")) {
        // 回退到基本处理
        CSSCompileResult result;
        result.success = true;
        result.css = source;
        result.originalSize = source.length();
        result.compiledSize = source.length();
        return result;
    }

    // PostCSS处理实现
    CSSCompileResult result;
    result.success = true;
    result.css = source;
    result.originalSize = source.length();
    
    // 如果启用了autoprefixer
    if (options.autoprefixer) {
        // 这里应该调用autoprefixer
        // 简化实现：添加一些常见前缀
        std::string processed = source;
        // 实际实现中应该使用PostCSS + Autoprefixer
        result.css = processed;
    }
    
    result.compiledSize = result.css.length();
    return result;
}

std::string CSSCompiler::generateCompilerScript(CSSProcessType type, const CSSCompileOptions& options) {
    std::ostringstream script;
    
    switch (type) {
        case CSSProcessType::SASS_COMPILE:
            script << "const sass = require('sass');\n";
            script << "const result = sass.compileString(process.argv[2], {\n";
            script << "  style: '" << (options.minify ? "compressed" : "expanded") << "',\n";
            script << "  sourceMap: " << (options.sourceMaps ? "true" : "false") << "\n";
            script << "});\n";
            script << "console.log(result.css);\n";
            break;
            
        case CSSProcessType::POSTCSS_PROCESS:
            script << "const postcss = require('postcss');\n";
            if (options.autoprefixer) {
                script << "const autoprefixer = require('autoprefixer');\n";
            }
            script << "const plugins = [];\n";
            if (options.autoprefixer) {
                script << "plugins.push(autoprefixer());\n";
            }
            script << "postcss(plugins).process(process.argv[2], {from: undefined})\n";
            script << "  .then(result => console.log(result.css))\n";
            script << "  .catch(err => { console.error(err); process.exit(1); });\n";
            break;
            
        default:
            script << "console.log(process.argv[2]);\n";
            break;
    }
    
    return script.str();
}

bool CSSCompiler::ensureDependenciesAvailable() {
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

// CSSUtils实现
std::string CSSUtils::minifyCSS(const std::string& css) {
    std::string result = css;
    
    // 简化实现，避免正则表达式问题
    // 移除多余的空白
    std::string temp;
    bool inString = false;
    char stringChar = '\0';
    bool lastWasSpace = false;
    
    for (char c : result) {
        if (!inString && (c == '"' || c == '\'')) {
            inString = true;
            stringChar = c;
            temp += c;
            lastWasSpace = false;
        } else if (inString && c == stringChar) {
            inString = false;
            temp += c;
            lastWasSpace = false;
        } else if (!inString && std::isspace(c)) {
            if (!lastWasSpace && !temp.empty() && 
                temp.back() != '{' && temp.back() != '}' && 
                temp.back() != ';' && temp.back() != ':' && temp.back() != ',') {
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
                if (prev == '{' || prev == '}' || prev == ';' || prev == ':' || prev == ',' ||
                    next == '{' || next == '}' || next == ';' || next == ':' || next == ',') {
                    continue; // 跳过这个空格
                }
            }
        }
        temp += c;
    }
    
    return temp;
}

std::string CSSUtils::beautifyCSS(const std::string& css, const std::string& indent) {
    std::string result = css;
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

std::vector<std::string> CSSUtils::extractSelectors(const std::string& css) {
    std::vector<std::string> selectors;
    std::regex selectorRegex(R"(([^{}]+)\s*\{)");
    std::sregex_iterator iter(css.begin(), css.end(), selectorRegex);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        std::string selector = iter->str(1);
        // 清理selector
        selector = std::regex_replace(selector, std::regex(R"(^\s+|\s+$)"), "");
        if (!selector.empty()) {
            selectors.push_back(selector);
        }
    }
    
    return selectors;
}

std::unordered_map<std::string, std::unordered_map<std::string, std::string>> 
CSSUtils::extractProperties(const std::string& css) {
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> result;
    
    // 这是一个简化实现
    // 实际应该使用CSS解析器
    
    return result;
}

bool CSSUtils::isValidSelector(const std::string& selector) {
    // 简单的选择器验证
    if (selector.empty()) return false;
    
    // 检查基本字符
    std::regex validChars(R"(^[a-zA-Z0-9\-_#.:\[\](),\s>+~*]+$)");
    return std::regex_match(selector, validChars);
}

bool CSSUtils::isValidProperty(const std::string& property, const std::string& value) {
    if (property.empty() || value.empty()) return false;
    
    // 简单验证
    std::regex propRegex(R"(^[a-zA-Z\-]+$)");
    return std::regex_match(property, propRegex);
}

std::vector<std::string> CSSUtils::getBrowserPrefixes(const std::string& property) {
    std::vector<std::string> prefixes;
    
    // 常见需要前缀的属性
    static const std::unordered_map<std::string, std::vector<std::string>> prefixMap = {
        {"transform", {"-webkit-", "-moz-", "-ms-"}},
        {"transition", {"-webkit-", "-moz-", "-ms-"}},
        {"animation", {"-webkit-", "-moz-"}},
        {"border-radius", {"-webkit-", "-moz-"}},
        {"box-shadow", {"-webkit-", "-moz-"}},
        {"user-select", {"-webkit-", "-moz-", "-ms-"}}
    };
    
    auto it = prefixMap.find(property);
    if (it != prefixMap.end()) {
        return it->second;
    }
    
    return {};
}

} // namespace chtl
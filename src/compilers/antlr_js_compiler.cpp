#include "compilers/antlr_js_compiler.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <antlr4-runtime.h>

using namespace antlr4;
using namespace chtl;

// JSASTVisitor实现

JSASTVisitor::JSASTVisitor(const JSCompileOptions& options) 
    : options_(options) {
    result_.success = true;
}

antlrcpp::Any JSASTVisitor::visitProgram(JavaScriptParser::ProgramContext *ctx) {
    if (!ctx) return nullptr;
    
    std::ostringstream js;
    
    if (ctx->sourceElements()) {
        for (auto element : ctx->sourceElements()->sourceElement()) {
            if (element->functionDeclaration()) {
                if (options_.extractFunctions) {
                    extractFunctionInfo(element->functionDeclaration());
                }
                std::string func = formatFunction(element->functionDeclaration());
                if (!func.empty()) {
                    js << func;
                    if (!options_.minify) js << "\n\n";
                }
            } else if (element->statement()) {
                std::string stmt = formatStatement(element);
                if (!stmt.empty()) {
                    js << stmt;
                    if (!options_.minify) js << "\n";
                }
            }
        }
    }
    
    result_.javascript = js.str();
    result_.originalSize = result_.javascript.length();
    
    if (options_.minify) {
        result_.javascript = minifyJS(result_.javascript);
    }
    
    result_.compiledSize = result_.javascript.length();
    return result_.javascript;
}

antlrcpp::Any JSASTVisitor::visitFunctionDeclaration(JavaScriptParser::FunctionDeclarationContext *ctx) {
    if (options_.extractFunctions) {
        extractFunctionInfo(ctx);
    }
    return formatFunction(ctx);
}

antlrcpp::Any JSASTVisitor::visitVariableStatement(JavaScriptParser::VariableStatementContext *ctx) {
    if (options_.extractVariables) {
        extractVariableInfo(ctx);
    }
    return ctx->getText();
}

antlrcpp::Any JSASTVisitor::visitExpressionStatement(JavaScriptParser::ExpressionStatementContext *ctx) {
    if (!ctx) return std::string("");
    return ctx->getText();
}

antlrcpp::Any JSASTVisitor::visitBlock(JavaScriptParser::BlockContext *ctx) {
    if (!ctx) return std::string("");
    
    std::ostringstream block;
    block << "{";
    if (!options_.minify) block << "\n";
    
    if (ctx->statementList()) {
        for (auto stmt : ctx->statementList()->statement()) {
            std::string statement = stmt->getText();
            if (!statement.empty()) {
                if (!options_.minify) block << "  ";
                block << statement;
                if (!options_.minify) block << "\n";
            }
        }
    }
    
    block << "}";
    return block.str();
}

void JSASTVisitor::extractFunctionInfo(JavaScriptParser::FunctionDeclarationContext *ctx) {
    if (!ctx) return;
    
    std::string functionName;
    if (ctx->Identifier()) {
        functionName = ctx->Identifier()->getText();
        result_.functions.push_back(functionName);
    }
}

void JSASTVisitor::extractVariableInfo(JavaScriptParser::VariableStatementContext *ctx) {
    if (!ctx) return;
    
    if (ctx->variableDeclarationList()) {
        for (auto varDecl : ctx->variableDeclarationList()->variableDeclaration()) {
            if (varDecl->Identifier()) {
                std::string varName = varDecl->Identifier()->getText();
                result_.variables.push_back(varName);
            }
        }
    }
}

std::string JSASTVisitor::minifyJS(const std::string& js) {
    std::string minified = js;
    
    // 简单的压缩：移除多余空白和换行
    minified = std::regex_replace(minified, std::regex(R"(\s+)"), " ");
    minified = std::regex_replace(minified, std::regex(R"(\s*{\s*)"), "{");
    minified = std::regex_replace(minified, std::regex(R"(\s*}\s*)"), "}");
    minified = std::regex_replace(minified, std::regex(R"(\s*;\s*)"), ";");
    minified = std::regex_replace(minified, std::regex(R"(\s*,\s*)"), ",");
    minified = std::regex_replace(minified, std::regex(R"(\s*\(\s*)"), "(");
    minified = std::regex_replace(minified, std::regex(R"(\s*\)\s*)"), ")");
    
    return minified;
}

std::string JSASTVisitor::formatFunction(JavaScriptParser::FunctionDeclarationContext *ctx) {
    if (!ctx) return "";
    
    std::ostringstream func;
    func << "function ";
    
    if (ctx->Identifier()) {
        func << ctx->Identifier()->getText();
    }
    
    func << "(";
    if (ctx->formalParameterList()) {
        bool first = true;
        for (auto param : ctx->formalParameterList()->Identifier()) {
            if (!first) func << ", ";
            func << param->getText();
            first = false;
        }
    }
    func << ") ";
    
    if (ctx->functionBody()) {
        func << "{";
        if (!options_.minify) func << "\n";
        
        // 简化：直接输出函数体文本
        std::string bodyText = ctx->functionBody()->getText();
        if (!bodyText.empty()) {
            if (!options_.minify) {
                // 添加缩进
                bodyText = std::regex_replace(bodyText, std::regex("\n"), "\n  ");
                func << "  " << bodyText;
                func << "\n";
            } else {
                func << bodyText;
            }
        }
        
        func << "}";
    }
    
    return func.str();
}

std::string JSASTVisitor::formatStatement(JavaScriptParser::SourceElementContext *ctx) {
    if (!ctx || !ctx->statement()) return "";
    return ctx->statement()->getText();
}

// ANTLRJSCompiler实现

ANTLRJSCompiler::ANTLRJSCompiler() {
}

std::vector<CodeSegmentType> ANTLRJSCompiler::getSupportedTypes() const {
    return {
        CodeSegmentType::JAVASCRIPT_STANDARD,
        CodeSegmentType::JAVASCRIPT_ORIGIN,
        CodeSegmentType::CHTL_JS_SCRIPT
    };
}

CompileResult ANTLRJSCompiler::compile(const CodeSegment& segment, const CompileConfig& config) {
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

JSCompileResult ANTLRJSCompiler::compileJS(const std::string& jsCode, const JSCompileOptions& options) {
    JSCompileResult result;
    
    try {
        // 创建输入流
        ANTLRInputStream input(jsCode);
        
        // 创建词法分析器
        JavaScriptLexer lexer(&input);
        CommonTokenStream tokens(&lexer);
        
        // 创建解析器
        JavaScriptParser parser(&tokens);
        
        // 禁用默认错误输出
        parser.removeErrorListeners();
        
        // 解析
        JavaScriptParser::ProgramContext* tree = parser.program();
        
        if (tree) {
            // 使用访问器遍历AST
            JSASTVisitor visitor(options);
            visitor.visitProgram(tree);
            result = visitor.getResult();
        } else {
            result.success = false;
            result.errors.push_back("Failed to parse JavaScript: Invalid syntax");
        }
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back(std::string("JavaScript compilation error: ") + e.what());
    }
    
    return result;
}

bool ANTLRJSCompiler::validateSyntax(const std::string& jsCode, std::vector<std::string>& errors) {
    try {
        ANTLRInputStream input(jsCode);
        JavaScriptLexer lexer(&input);
        CommonTokenStream tokens(&lexer);
        JavaScriptParser parser(&tokens);
        
        // 禁用默认错误输出
        parser.removeErrorListeners();
        
        JavaScriptParser::ProgramContext* tree = parser.program();
        return tree != nullptr;
        
    } catch (const std::exception& e) {
        errors.push_back(std::string("Syntax validation error: ") + e.what());
        return false;
    }
}

std::vector<std::string> ANTLRJSCompiler::extractFunctions(const std::string& jsCode) {
    JSCompileOptions options;
    options.extractFunctions = true;
    options.extractVariables = false;
    
    auto result = compileJS(jsCode, options);
    return result.functions;
}

std::vector<std::string> ANTLRJSCompiler::extractVariables(const std::string& jsCode) {
    JSCompileOptions options;
    options.extractFunctions = false;
    options.extractVariables = true;
    
    auto result = compileJS(jsCode, options);
    return result.variables;
}

std::vector<std::string> ANTLRJSCompiler::extractClasses(const std::string& jsCode) {
    JSCompileOptions options;
    options.extractClasses = true;
    
    auto result = compileJS(jsCode, options);
    return result.classes;
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
    // 简单的ES6到ES5转换（这里只是示例）
    std::string es5Code = jsCode;
    
    // 转换箭头函数
    es5Code = std::regex_replace(es5Code, std::regex(R"(\(\s*([^)]*)\s*\)\s*=>\s*)"), "function($1)");
    
    // 转换const/let为var
    es5Code = std::regex_replace(es5Code, std::regex(R"(\bconst\b)"), "var");
    es5Code = std::regex_replace(es5Code, std::regex(R"(\blet\b)"), "var");
    
    return es5Code;
}

std::string ANTLRJSCompiler::convertToES6(const std::string& jsCode) {
    // 简单的ES5到ES6转换（这里只是示例）
    std::string es6Code = jsCode;
    
    // 可以添加ES6特性转换逻辑
    
    return es6Code;
}

std::unique_ptr<JavaScriptParser> ANTLRJSCompiler::createParser(const std::string& input, std::vector<std::string>& errors) {
    // 这个方法可以在未来扩展以提供更复杂的解析器配置
    return nullptr;
}

void ANTLRJSCompiler::handleParseErrors(antlr4::Parser* parser, std::vector<std::string>& errors) {
    // 这个方法可以在未来扩展以提供更好的错误处理
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
    // 简单的ES版本检测
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
    int complexity = 1; // 基础复杂度
    
    // 计算控制流语句
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
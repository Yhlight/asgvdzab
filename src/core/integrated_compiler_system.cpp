#include "core/integrated_compiler_system.hpp"
#include <iostream>
#include <chrono>
#include <algorithm>
#include <sstream>

namespace chtl {

// ==================== CHTLLocalStateMachine 实现 ====================

CHTLLocalStateMachine::CHTLLocalStateMachine() : currentState_(CHTLState::IDLE) {
    contextStack_.push_back("global");
}

bool CHTLLocalStateMachine::transitionTo(CHTLState newState, const std::string& context) {
    if (!isValidTransition(currentState_, newState)) {
        reportError("无效的状态转换: " + std::to_string(static_cast<int>(currentState_)) + 
                   " -> " + std::to_string(static_cast<int>(newState)));
        currentState_ = CHTLState::ERROR_STATE;
        return false;
    }
    
    if (!context.empty()) {
        pushContext(context);
    }
    
    currentState_ = newState;
    return true;
}

void CHTLLocalStateMachine::pushContext(const std::string& context) {
    contextStack_.push_back(context);
}

void CHTLLocalStateMachine::popContext() {
    if (contextStack_.size() > 1) {
        contextStack_.pop_back();
    }
}

std::string CHTLLocalStateMachine::getCurrentContext() const {
    return contextStack_.empty() ? "unknown" : contextStack_.back();
}

void CHTLLocalStateMachine::reportError(const std::string& error) {
    errors_.push_back("[CHTL:" + getCurrentContext() + "] " + error);
}

bool CHTLLocalStateMachine::isValidTransition(CHTLState from, CHTLState to) const {
    if (to == CHTLState::ERROR_STATE) return true; // 总是可以转到错误状态
    if (from == CHTLState::ERROR_STATE) return to == CHTLState::IDLE; // 从错误状态只能回到空闲
    
    // 定义有效的状态转换
    switch (from) {
        case CHTLState::IDLE:
            return to == CHTLState::PARSING_TEMPLATE_VAR || 
                   to == CHTLState::PARSING_CUSTOM_VAR ||
                   to == CHTLState::PARSING_STYLE_GROUP ||
                   to == CHTLState::PARSING_LOCAL_STYLE;
        case CHTLState::PARSING_TEMPLATE_VAR:
        case CHTLState::PARSING_CUSTOM_VAR:
        case CHTLState::PARSING_STYLE_GROUP:
            return to == CHTLState::PARSING_INHERITANCE || 
                   to == CHTLState::GENERATING_OUTPUT ||
                   to == CHTLState::IDLE;
        case CHTLState::PARSING_INHERITANCE:
            return to == CHTLState::GENERATING_OUTPUT || to == CHTLState::IDLE;
        case CHTLState::PARSING_LOCAL_STYLE:
            return to == CHTLState::GENERATING_OUTPUT || to == CHTLState::IDLE;
        case CHTLState::GENERATING_OUTPUT:
            return to == CHTLState::IDLE;
        default:
            return false;
    }
}

// ==================== CHTLJSLocalStateMachine 实现 ====================

CHTLJSLocalStateMachine::CHTLJSLocalStateMachine() : currentState_(CHTLJSState::IDLE) {
    contextStack_.push_back("global");
}

bool CHTLJSLocalStateMachine::transitionTo(CHTLJSState newState, const std::string& context) {
    if (!isValidTransition(currentState_, newState)) {
        reportError("无效的状态转换: " + std::to_string(static_cast<int>(currentState_)) + 
                   " -> " + std::to_string(static_cast<int>(newState)));
        currentState_ = CHTLJSState::ERROR_STATE;
        return false;
    }
    
    if (!context.empty()) {
        pushContext(context);
    }
    
    currentState_ = newState;
    return true;
}

void CHTLJSLocalStateMachine::pushContext(const std::string& context) {
    contextStack_.push_back(context);
}

void CHTLJSLocalStateMachine::popContext() {
    if (contextStack_.size() > 1) {
        contextStack_.pop_back();
    }
}

std::string CHTLJSLocalStateMachine::getCurrentContext() const {
    return contextStack_.empty() ? "unknown" : contextStack_.back();
}

void CHTLJSLocalStateMachine::reportError(const std::string& error) {
    errors_.push_back("[CHTL-JS:" + getCurrentContext() + "] " + error);
}

bool CHTLJSLocalStateMachine::isValidTransition(CHTLJSState from, CHTLJSState to) const {
    if (to == CHTLJSState::ERROR_STATE) return true;
    if (from == CHTLJSState::ERROR_STATE) return to == CHTLJSState::IDLE;
    
    switch (from) {
        case CHTLJSState::IDLE:
            return to == CHTLJSState::PARSING_MIXED_SYNTAX;
        case CHTLJSState::PARSING_MIXED_SYNTAX:
            return to == CHTLJSState::SEPARATING_CHTL_JS;
        case CHTLJSState::SEPARATING_CHTL_JS:
            return to == CHTLJSState::PARSING_VARIABLE_GROUP ||
                   to == CHTLJSState::PARSING_FROM_CLAUSE ||
                   to == CHTLJSState::PARSING_JS_TEMPLATE;
        case CHTLJSState::PARSING_VARIABLE_GROUP:
        case CHTLJSState::PARSING_FROM_CLAUSE:
        case CHTLJSState::PARSING_JS_TEMPLATE:
            return to == CHTLJSState::GENERATING_OUTPUT || to == CHTLJSState::IDLE;
        case CHTLJSState::GENERATING_OUTPUT:
            return to == CHTLJSState::IDLE;
        default:
            return false;
    }
}

// ==================== CHTLTestFramework 实现 ====================

CHTLTestFramework::CHTLTestFramework() {
}

void CHTLTestFramework::addTestCase(const TestCase& testCase) {
    testCases_.push_back(testCase);
}

void CHTLTestFramework::loadTestCases(const std::string& filePath) {
    // TODO: 从文件加载测试用例
    // 暂时添加一些默认测试用例
    addTestCase({
        "basic_variable",
        "{{title}}",
        "<span>{{title}}</span>",
        true,
        {},
        "测试基本变量引用"
    });
    
    addTestCase({
        "style_group",
        "[Custom] @Style Button { color: red; }",
        ".Button { color: red; }",
        true,
        {},
        "测试自定义样式组"
    });
}

CHTLTestFramework::TestResult CHTLTestFramework::runSingleTest(const TestCase& testCase) {
    TestResult result;
    auto startTime = std::chrono::high_resolution_clock::now();
    
    try {
        // 创建临时的集成系统来执行测试
        auto system = CompilerSystemFactory::createTestingSystem();
        if (!system->initialize()) {
            result.passed = false;
            result.failureReason = "系统初始化失败";
            return result;
        }
        
        auto compilationResult = system->compile(testCase.input);
        result.actualOutput = compilationResult.output;
        
        // 收集错误
        for (const auto& error : compilationResult.errors) {
            result.actualErrors.push_back(error);
        }
        
        // 检查结果
        if (testCase.shouldPass) {
            result.passed = compilationResult.success && 
                           compareOutput(testCase.expectedOutput, result.actualOutput);
            if (!result.passed && compilationResult.success) {
                result.failureReason = "输出不匹配预期";
            } else if (!compilationResult.success) {
                result.failureReason = "编译失败";
            }
        } else {
            result.passed = !compilationResult.success;
            if (result.passed) {
                result.passed = compareErrors(testCase.expectedErrors, result.actualErrors);
            }
            if (!result.passed) {
                result.failureReason = "错误不匹配预期";
            }
        }
        
    } catch (const std::exception& e) {
        result.passed = false;
        result.failureReason = "测试执行异常: " + std::string(e.what());
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    result.executionTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    
    return result;
}

std::vector<CHTLTestFramework::TestResult> CHTLTestFramework::runAllTests() {
    std::vector<TestResult> results;
    
    for (const auto& testCase : testCases_) {
        std::cout << "运行测试: " << testCase.name << std::endl;
        auto result = runSingleTest(testCase);
        results.push_back(result);
        
        if (result.passed) {
            std::cout << "  ✓ 通过 (" << result.executionTime << "ms)" << std::endl;
        } else {
            std::cout << "  ✗ 失败: " << result.failureReason << std::endl;
        }
    }
    
    return results;
}

CHTLTestFramework::TestSummary CHTLTestFramework::generateSummary(const std::vector<TestResult>& results) {
    TestSummary summary;
    summary.totalTests = results.size();
    summary.passedTests = 0;
    summary.failedTests = 0;
    summary.totalTime = 0.0;
    
    for (size_t i = 0; i < results.size(); ++i) {
        const auto& result = results[i];
        summary.totalTime += result.executionTime;
        
        if (result.passed) {
            summary.passedTests++;
        } else {
            summary.failedTests++;
            if (i < testCases_.size()) {
                summary.failedTestNames.push_back(testCases_[i].name);
            }
        }
    }
    
    return summary;
}

void CHTLTestFramework::printTestReport(const TestSummary& summary) {
    std::cout << "\n=== CHTL测试报告 ===" << std::endl;
    std::cout << "总测试数: " << summary.totalTests << std::endl;
    std::cout << "通过: " << summary.passedTests << std::endl;
    std::cout << "失败: " << summary.failedTests << std::endl;
    std::cout << "总时间: " << summary.totalTime << "ms" << std::endl;
    
    if (!summary.failedTestNames.empty()) {
        std::cout << "\n失败的测试:" << std::endl;
        for (const auto& name : summary.failedTestNames) {
            std::cout << "  - " << name << std::endl;
        }
    }
}

bool CHTLTestFramework::compareOutput(const std::string& expected, const std::string& actual) {
    // 简单的字符串比较，可以扩展为更复杂的比较逻辑
    return expected == actual;
}

bool CHTLTestFramework::compareErrors(const std::vector<std::string>& expected, const std::vector<std::string>& actual) {
    if (expected.size() != actual.size()) return false;
    
    for (const auto& expectedError : expected) {
        bool found = false;
        for (const auto& actualError : actual) {
            if (actualError.find(expectedError) != std::string::npos) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    
    return true;
}

// ==================== CHTLErrorHandler 实现 ====================

CHTLErrorHandler::CHTLErrorHandler() {
}

void CHTLErrorHandler::reportError(ErrorType type, const std::string& message, 
                                  const std::string& location, int line, int column) {
    CHTLError error;
    error.type = type;
    error.message = message;
    error.location = location;
    error.line = line;
    error.column = column;
    error.suggestion = generateSuggestion(error);
    error.code = "CHTL" + std::to_string(static_cast<int>(type)) + 
                std::to_string(errors_.size() + 1);
    
    errors_.push_back(error);
}

void CHTLErrorHandler::reportSyntaxError(const std::string& message, int line, int column) {
    reportError(ErrorType::SYNTAX_ERROR, message, "", line, column);
}

void CHTLErrorHandler::reportConstraintError(const std::string& message, const std::string& context) {
    reportError(ErrorType::CONSTRAINT_ERROR, message, context);
}

std::vector<CHTLErrorHandler::CHTLError> CHTLErrorHandler::getErrorsByType(ErrorType type) const {
    std::vector<CHTLError> filtered;
    for (const auto& error : errors_) {
        if (error.type == type) {
            filtered.push_back(error);
        }
    }
    return filtered;
}

bool CHTLErrorHandler::hasCriticalErrors() const {
    for (const auto& error : errors_) {
        if (isCritical(error.type)) {
            return true;
        }
    }
    return false;
}

std::string CHTLErrorHandler::generateErrorReport() const {
    std::ostringstream report;
    report << "=== CHTL错误报告 ===\n";
    report << "总错误数: " << errors_.size() << "\n\n";
    
    for (const auto& error : errors_) {
        report << "[" << error.code << "] " << getErrorTypeString(error.type) << ": " << error.message << "\n";
        if (!error.location.empty()) {
            report << "  位置: " << error.location;
        }
        if (error.line >= 0) {
            report << " (行: " << error.line;
            if (error.column >= 0) {
                report << ", 列: " << error.column;
            }
            report << ")";
        }
        report << "\n";
        if (!error.suggestion.empty()) {
            report << "  建议: " << error.suggestion << "\n";
        }
        report << "\n";
    }
    
    return report.str();
}

std::vector<std::string> CHTLErrorHandler::generateFixSuggestions() const {
    std::vector<std::string> suggestions;
    for (const auto& error : errors_) {
        if (!error.suggestion.empty()) {
            suggestions.push_back(error.suggestion);
        }
    }
    return suggestions;
}

std::string CHTLErrorHandler::getErrorTypeString(ErrorType type) const {
    switch (type) {
        case ErrorType::SYNTAX_ERROR: return "语法错误";
        case ErrorType::SEMANTIC_ERROR: return "语义错误";
        case ErrorType::CONSTRAINT_ERROR: return "约束错误";
        case ErrorType::RUNTIME_ERROR: return "运行时错误";
        case ErrorType::COMPILATION_ERROR: return "编译错误";
        default: return "未知错误";
    }
}

std::string CHTLErrorHandler::generateSuggestion(const CHTLError& error) const {
    // 根据错误类型和消息生成修复建议
    switch (error.type) {
        case ErrorType::SYNTAX_ERROR:
            if (error.message.find("{{") != std::string::npos) {
                return "检查变量引用语法，确保使用正确的双大括号格式";
            }
            break;
        case ErrorType::CONSTRAINT_ERROR:
            if (error.message.find("不允许") != std::string::npos) {
                return "检查语法是否在正确的上下文中使用";
            }
            break;
        default:
            break;
    }
    return "请检查相关语法和文档";
}

bool CHTLErrorHandler::isCritical(ErrorType type) const {
    return type == ErrorType::SYNTAX_ERROR || type == ErrorType::COMPILATION_ERROR;
}

// ==================== CHTLJSTestFramework 实现 ====================

CHTLJSTestFramework::CHTLJSTestFramework() {
}

void CHTLJSTestFramework::addTestCase(const JSTestCase& testCase) {
    testCases_.push_back(testCase);
}

void CHTLJSTestFramework::loadTestCases(const std::string& filePath) {
    // TODO: 从文件加载测试用例
    // 暂时添加一些默认测试用例
    addTestCase({
        "basic_chtl_js_mix",
        "{{color}} = '#ff0000'; console.log({{color}});",
        "const color = '#ff0000'; console.log(color);",
        true,
        {},
        "测试基本CHTL JS混合语法"
    });
    
    addTestCase({
        "variable_group",
        "ColorGroup(primary = {{mainColor}}, secondary = {{secondaryColor}});",
        "ColorGroup({primary: mainColor, secondary: secondaryColor});",
        true,
        {},
        "测试变量组语法"
    });
}

CHTLJSTestFramework::JSTestResult CHTLJSTestFramework::runSingleTest(const JSTestCase& testCase) {
    JSTestResult result;
    auto startTime = std::chrono::high_resolution_clock::now();
    
    try {
        // 创建临时的集成系统来执行测试
        auto system = CompilerSystemFactory::createTestingSystem();
        if (!system->initialize()) {
            result.passed = false;
            result.failureReason = "系统初始化失败";
            return result;
        }
        
        auto compilationResult = system->compile(testCase.chtlJsInput);
        result.actualJsOutput = compilationResult.output;
        
        // 收集错误
        for (const auto& error : compilationResult.errors) {
            result.actualErrors.push_back(error);
        }
        
        // 检查结果
        if (testCase.shouldPass) {
            result.passed = compilationResult.success && 
                           compareJsOutput(testCase.expectedJsOutput, result.actualJsOutput);
            if (!result.passed && compilationResult.success) {
                result.failureReason = "JS输出不匹配预期";
            } else if (!compilationResult.success) {
                result.failureReason = "编译失败";
            }
        } else {
            result.passed = !compilationResult.success;
            if (result.passed) {
                bool errorsMatch = true;
                for (const auto& expectedError : testCase.expectedErrors) {
                    bool found = false;
                    for (const auto& actualError : result.actualErrors) {
                        if (actualError.find(expectedError) != std::string::npos) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        errorsMatch = false;
                        break;
                    }
                }
                result.passed = errorsMatch;
            }
            if (!result.passed) {
                result.failureReason = "错误不匹配预期";
            }
        }
        
    } catch (const std::exception& e) {
        result.passed = false;
        result.failureReason = "测试执行异常: " + std::string(e.what());
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    result.executionTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    
    return result;
}

std::vector<CHTLJSTestFramework::JSTestResult> CHTLJSTestFramework::runAllTests() {
    std::vector<JSTestResult> results;
    
    for (const auto& testCase : testCases_) {
        std::cout << "运行CHTL JS测试: " << testCase.name << std::endl;
        auto result = runSingleTest(testCase);
        results.push_back(result);
        
        if (result.passed) {
            std::cout << "  ✓ 通过 (" << result.executionTime << "ms)" << std::endl;
        } else {
            std::cout << "  ✗ 失败: " << result.failureReason << std::endl;
        }
    }
    
    return results;
}

CHTLJSTestFramework::JSTestSummary CHTLJSTestFramework::generateSummary(const std::vector<JSTestResult>& results) {
    JSTestSummary summary;
    summary.totalTests = results.size();
    summary.passedTests = 0;
    summary.failedTests = 0;
    summary.totalTime = 0.0;
    
    for (size_t i = 0; i < results.size(); ++i) {
        const auto& result = results[i];
        summary.totalTime += result.executionTime;
        
        if (result.passed) {
            summary.passedTests++;
        } else {
            summary.failedTests++;
            if (i < testCases_.size()) {
                summary.failedTestNames.push_back(testCases_[i].name);
            }
        }
    }
    
    return summary;
}

void CHTLJSTestFramework::printTestReport(const JSTestSummary& summary) {
    std::cout << "\n=== CHTL JS测试报告 ===" << std::endl;
    std::cout << "总测试数: " << summary.totalTests << std::endl;
    std::cout << "通过: " << summary.passedTests << std::endl;
    std::cout << "失败: " << summary.failedTests << std::endl;
    std::cout << "总时间: " << summary.totalTime << "ms" << std::endl;
    
    if (!summary.failedTestNames.empty()) {
        std::cout << "\n失败的测试:" << std::endl;
        for (const auto& name : summary.failedTestNames) {
            std::cout << "  - " << name << std::endl;
        }
    }
}

bool CHTLJSTestFramework::compareJsOutput(const std::string& expected, const std::string& actual) {
    // 简单的字符串比较，可以扩展为更复杂的比较逻辑
    return expected == actual;
}

bool CHTLJSTestFramework::validateJsSyntax(const std::string& jsCode) {
    // 简单的JS语法验证
    // 检查基本的大括号匹配
    int braceCount = 0;
    int parenCount = 0;
    
    for (char c : jsCode) {
        if (c == '{') braceCount++;
        else if (c == '}') braceCount--;
        else if (c == '(') parenCount++;
        else if (c == ')') parenCount--;
    }
    
    return braceCount == 0 && parenCount == 0;
}

// ==================== CHTLJSErrorHandler 实现 ====================

CHTLJSErrorHandler::CHTLJSErrorHandler() {
}

void CHTLJSErrorHandler::reportError(JSErrorType type, const std::string& message, 
                                    const std::string& location, int line, int column) {
    CHTLJSError error;
    error.type = type;
    error.message = message;
    error.location = location;
    error.line = line;
    error.column = column;
    error.suggestion = generateSuggestion(error);
    error.code = "CHTLJS" + std::to_string(static_cast<int>(type)) + 
                std::to_string(errors_.size() + 1);
    error.isChtlRelated = (type == JSErrorType::CHTL_SYNTAX_ERROR || 
                          type == JSErrorType::MIXING_ERROR || 
                          type == JSErrorType::VARIABLE_ERROR);
    
    errors_.push_back(error);
}

void CHTLJSErrorHandler::reportMixingError(const std::string& message, const std::string& context) {
    reportError(JSErrorType::MIXING_ERROR, message, context);
}

void CHTLJSErrorHandler::reportVariableError(const std::string& varName, const std::string& issue) {
    std::string message = "变量 '" + varName + "' " + issue;
    reportError(JSErrorType::VARIABLE_ERROR, message);
}

std::vector<CHTLJSErrorHandler::CHTLJSError> CHTLJSErrorHandler::getChtlErrors() const {
    std::vector<CHTLJSError> chtlErrors;
    for (const auto& error : errors_) {
        if (error.isChtlRelated) {
            chtlErrors.push_back(error);
        }
    }
    return chtlErrors;
}

std::vector<CHTLJSErrorHandler::CHTLJSError> CHTLJSErrorHandler::getJsErrors() const {
    std::vector<CHTLJSError> jsErrors;
    for (const auto& error : errors_) {
        if (!error.isChtlRelated) {
            jsErrors.push_back(error);
        }
    }
    return jsErrors;
}

std::string CHTLJSErrorHandler::generateErrorReport() const {
    std::ostringstream report;
    report << "=== CHTL JS错误报告 ===\n";
    report << "总错误数: " << errors_.size() << "\n\n";
    
    for (const auto& error : errors_) {
        report << "[" << error.code << "] " << getErrorTypeString(error.type) << ": " << error.message << "\n";
        if (!error.location.empty()) {
            report << "  位置: " << error.location;
        }
        if (error.line >= 0) {
            report << " (行: " << error.line;
            if (error.column >= 0) {
                report << ", 列: " << error.column;
            }
            report << ")";
        }
        report << "\n";
        if (!error.suggestion.empty()) {
            report << "  建议: " << error.suggestion << "\n";
        }
        report << "  类型: " << (error.isChtlRelated ? "CHTL相关" : "JavaScript相关") << "\n";
        report << "\n";
    }
    
    return report.str();
}

std::vector<std::string> CHTLJSErrorHandler::generateFixSuggestions() const {
    std::vector<std::string> suggestions;
    for (const auto& error : errors_) {
        if (!error.suggestion.empty()) {
            suggestions.push_back(error.suggestion);
        }
    }
    return suggestions;
}

std::string CHTLJSErrorHandler::getErrorTypeString(JSErrorType type) const {
    switch (type) {
        case JSErrorType::CHTL_SYNTAX_ERROR: return "CHTL语法错误";
        case JSErrorType::JS_SYNTAX_ERROR: return "JavaScript语法错误";
        case JSErrorType::MIXING_ERROR: return "混合语法错误";
        case JSErrorType::VARIABLE_ERROR: return "变量错误";
        case JSErrorType::COMPILATION_ERROR: return "编译错误";
        default: return "未知错误";
    }
}

std::string CHTLJSErrorHandler::generateSuggestion(const CHTLJSError& error) const {
    switch (error.type) {
        case JSErrorType::CHTL_SYNTAX_ERROR:
            if (error.message.find("{{") != std::string::npos) {
                return "检查CHTL变量引用语法，确保使用正确的双大括号格式";
            }
            break;
        case JSErrorType::JS_SYNTAX_ERROR:
            if (error.message.find("语法") != std::string::npos) {
                return "检查JavaScript语法是否正确";
            }
            break;
        case JSErrorType::MIXING_ERROR:
            return "检查CHTL和JavaScript的混合使用是否正确";
        case JSErrorType::VARIABLE_ERROR:
            return "检查变量定义和使用是否一致";
        default:
            break;
    }
    return "请检查相关语法和文档";
}

// ==================== IntegratedCompilerSystem 实现 ====================

IntegratedCompilerSystem::IntegratedCompilerSystem() 
    : initialized_(false), debugMode_(false), pipelineLogging_(false) {
}

IntegratedCompilerSystem::~IntegratedCompilerSystem() {
    cleanup();
}

bool IntegratedCompilerSystem::initialize() {
    if (initialized_) {
        return true;
    }
    
    try {
        initializeComponents();
        setupComponentConnections();
        initialized_ = true;
        
        if (debugMode_) {
            std::cout << "[IntegratedCompilerSystem] 系统初始化成功" << std::endl;
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[IntegratedCompilerSystem] 初始化失败: " << e.what() << std::endl;
        return false;
    }
}

void IntegratedCompilerSystem::cleanup() {
    if (!initialized_) return;
    
    if (scanner_) scanner_.reset();
    if (dispatcher_) dispatcher_.reset();
    if (constraintManager_) constraintManager_.reset();
    if (preValidator_) preValidator_.reset();
    
    chtlStateMachine_.reset();
    chtlJSStateMachine_.reset();
    
    chtlTestFramework_.reset();
    chtlJSTestFramework_.reset();
    chtlErrorHandler_.reset();
    chtlJSErrorHandler_.reset();
    
    initialized_ = false;
}

CompilationResult IntegratedCompilerSystem::compile(const std::string& sourceCode) {
    return compileWithConfig(sourceCode, nullptr);
}

CompilationResult IntegratedCompilerSystem::compileWithConfig(const std::string& sourceCode, 
                                                             std::shared_ptr<CHtlConfiguration> config) {
    if (!initialized_) {
        CompilationResult result;
        result.success = false;
        result.errors.push_back("编译系统未初始化");
        return result;
    }
    
    CompilationPipeline pipeline;
    
    // 执行编译流水线
    if (!executeSourceInput(pipeline, sourceCode)) {
        return pipeline.finalResult;
    }
    
    if (config && !executeConfigurationLoad(pipeline, config)) {
        return pipeline.finalResult;
    }
    
    if (!executePipeline(pipeline)) {
        return pipeline.finalResult;
    }
    
    return pipeline.finalResult;
}

bool IntegratedCompilerSystem::executePipeline(CompilationPipeline& pipeline) {
    std::vector<std::function<bool()>> stages = {
        [&]() { return executeUnifiedScanning(pipeline); },
        [&]() { return executePreciseSlicing(pipeline); },
        [&]() { return executeConstraintValidation(pipeline); },
        [&]() { return executeCompilerRouting(pipeline); },
        [&]() { return executeLocalCompilation(pipeline); },
        [&]() { return executeResultMerging(pipeline); },
        [&]() { return executeOutputGeneration(pipeline); }
    };
    
    for (auto& stage : stages) {
        if (!stage()) {
            pipeline.finalResult.success = false;
            return false;
        }
    }
    
    pipeline.finalResult.success = true;
    return true;
}

bool IntegratedCompilerSystem::executeSourceInput(CompilationPipeline& pipeline, const std::string& sourceCode) {
    pipeline.currentStage = PipelineStage::SOURCE_INPUT;
    pipeline.sourceCode = sourceCode;
    pipeline.metadata["source_length"] = std::to_string(sourceCode.length());
    
    logPipelineStage(pipeline, "源码输入完成");
    return true;
}

bool IntegratedCompilerSystem::executeConfigurationLoad(CompilationPipeline& pipeline, std::shared_ptr<CHtlConfiguration> config) {
    pipeline.currentStage = PipelineStage::CONFIGURATION_LOAD;
    pipeline.configuration = config;
    
    if (config) {
        // 应用配置到各个组件
        CHtlConfigurationManager::applyConfigurationFromChtlFile(pipeline.sourceCode);
    }
    
    logPipelineStage(pipeline, "配置加载完成");
    return true;
}

bool IntegratedCompilerSystem::executeUnifiedScanning(CompilationPipeline& pipeline) {
    pipeline.currentStage = PipelineStage::UNIFIED_SCANNING;
    
    if (!scanner_) {
        handlePipelineError(pipeline, "统一扫描器未初始化");
        return false;
    }
    
    // 这里先返回空的片段列表，等待实际的扫描器实现
    pipeline.codeFragments.clear();
    
    logPipelineStage(pipeline, "统一扫描完成");
    return true;
}

bool IntegratedCompilerSystem::executePreciseSlicing(CompilationPipeline& pipeline) {
    pipeline.currentStage = PipelineStage::PRECISE_SLICING;
    
    if (!scanner_) {
        handlePipelineError(pipeline, "精确切片器未初始化");
        return false;
    }
    
    try {
        // 使用统一扫描器进行精确切片
        auto fragments = scanner_->scanAndSlice(pipeline.sourceCode);
        pipeline.codeFragments = fragments;
        
        pipeline.metadata["fragment_count"] = std::to_string(fragments.size());
        
    } catch (const std::exception& e) {
        handlePipelineError(pipeline, "精确切片失败: " + std::string(e.what()));
        return false;
    }
    
    logPipelineStage(pipeline, "精确切片完成，片段数: " + std::to_string(pipeline.codeFragments.size()));
    return true;
}

bool IntegratedCompilerSystem::executeConstraintValidation(CompilationPipeline& pipeline) {
    pipeline.currentStage = PipelineStage::CONSTRAINT_VALIDATION;
    
    if (!preValidator_) {
        handlePipelineError(pipeline, "约束验证器未初始化");
        return false;
    }
    
    try {
        auto preValidationResult = preValidator_->validateBeforeCompilation(pipeline.codeFragments);
        pipeline.preValidationResult = preValidationResult;
        
        if (!preValidationResult.canProceedToCompilation) {
            for (const auto& error : preValidationResult.blockerErrors) {
                pipeline.stageErrors.push_back("约束验证失败: " + error);
            }
            handlePipelineError(pipeline, "约束验证失败，存在阻止编译的错误");
            return false;
        }
        
    } catch (const std::exception& e) {
        handlePipelineError(pipeline, "约束验证异常: " + std::string(e.what()));
        return false;
    }
    
    logPipelineStage(pipeline, "约束验证完成");
    return true;
}

bool IntegratedCompilerSystem::executeCompilerRouting(CompilationPipeline& pipeline) {
    pipeline.currentStage = PipelineStage::COMPILER_ROUTING;
    
    if (!dispatcher_) {
        handlePipelineError(pipeline, "编译器调度器未初始化");
        return false;
    }
    
    // 编译器路由逻辑已经集成在调度器中
    logPipelineStage(pipeline, "编译器路由完成");
    return true;
}

bool IntegratedCompilerSystem::executeLocalCompilation(CompilationPipeline& pipeline) {
    pipeline.currentStage = PipelineStage::LOCAL_COMPILATION;
    
    if (!dispatcher_) {
        handlePipelineError(pipeline, "编译器调度器未初始化");
        return false;
    }
    
    try {
        // 重置局部状态机
        if (chtlStateMachine_) chtlStateMachine_->clearErrors();
        if (chtlJSStateMachine_) chtlJSStateMachine_->clearErrors();
        
        // 编译各个片段
        auto fragmentResults = dispatcher_->compileFragments(pipeline.codeFragments);
        pipeline.fragmentResults = fragmentResults;
        
        // 收集局部状态机的错误
        if (chtlStateMachine_) {
            for (const auto& error : chtlStateMachine_->getErrors()) {
                pipeline.stageErrors.push_back(error);
            }
        }
        
        if (chtlJSStateMachine_) {
            for (const auto& error : chtlJSStateMachine_->getErrors()) {
                pipeline.stageErrors.push_back(error);
            }
        }
        
    } catch (const std::exception& e) {
        handlePipelineError(pipeline, "局部编译异常: " + std::string(e.what()));
        return false;
    }
    
    logPipelineStage(pipeline, "局部编译完成");
    return true;
}

bool IntegratedCompilerSystem::executeResultMerging(CompilationPipeline& pipeline) {
    pipeline.currentStage = PipelineStage::RESULT_MERGING;
    
    if (!dispatcher_) {
        handlePipelineError(pipeline, "编译器调度器未初始化");
        return false;
    }
    
    try {
        auto mergedResult = dispatcher_->mergeResults(pipeline.fragmentResults);
        pipeline.finalResult = mergedResult;
        
    } catch (const std::exception& e) {
        handlePipelineError(pipeline, "结果合并异常: " + std::string(e.what()));
        return false;
    }
    
    logPipelineStage(pipeline, "结果合并完成");
    return true;
}

bool IntegratedCompilerSystem::executeOutputGeneration(CompilationPipeline& pipeline) {
    pipeline.currentStage = PipelineStage::OUTPUT_GENERATION;
    
    // 输出生成已经在结果合并阶段完成
    // 这里可以添加额外的后处理逻辑
    
    logPipelineStage(pipeline, "输出生成完成");
    return true;
}

void IntegratedCompilerSystem::initializeComponents() {
    // 启用调试模式以便查看编译器创建过程
    CompilerFactory::setDebugMode(true);
    
    // 创建核心组件
    scanner_ = std::make_shared<CHTLUnifiedScanner>();
    dispatcher_ = std::make_shared<CompilerDispatcher>();
    constraintManager_ = std::make_shared<ConstraintValidationManager>();
    preValidator_ = std::make_shared<PreCompilationValidator>();
    
    // 创建局部状态机
    chtlStateMachine_ = std::make_unique<CHTLLocalStateMachine>();
    chtlJSStateMachine_ = std::make_unique<CHTLJSLocalStateMachine>();
    
    // 创建测试框架
    chtlTestFramework_ = std::make_shared<CHTLTestFramework>();
    chtlJSTestFramework_ = std::make_shared<CHTLJSTestFramework>();
    
    // 创建错误处理框架
    chtlErrorHandler_ = std::make_shared<CHTLErrorHandler>();
    chtlJSErrorHandler_ = std::make_shared<CHTLJSErrorHandler>();
    
    // 初始化约束管理器
    constraintManager_->initializeDefaultValidators();
    preValidator_->setValidationManager(constraintManager_);
}

void IntegratedCompilerSystem::setupComponentConnections() {
    // 初始化所有组件
    if (dispatcher_ && !dispatcher_->initialize()) {
        throw std::runtime_error("编译器调度器初始化失败");
    }
    
    // 设置调试模式
    if (scanner_) scanner_->setDebugMode(debugMode_);
    if (dispatcher_) dispatcher_->setDebugMode(debugMode_);
    if (constraintManager_) constraintManager_->setDebugMode(debugMode_);
    
    // 设置组件间的连接
    // 这里可以添加更多的连接逻辑
}

void IntegratedCompilerSystem::setDebugMode(bool enabled) {
    debugMode_ = enabled;
    
    if (scanner_) scanner_->setDebugMode(enabled);
    if (dispatcher_) dispatcher_->setDebugMode(enabled);
    if (constraintManager_) constraintManager_->setDebugMode(enabled);
}

void IntegratedCompilerSystem::enablePipelineLogging(bool enabled) {
    pipelineLogging_ = enabled;
}

std::string IntegratedCompilerSystem::getPipelineStatus(const CompilationPipeline& pipeline) {
    std::ostringstream status;
    status << "Pipeline Stage: " << static_cast<int>(pipeline.currentStage) << "\n";
    status << "Fragments: " << pipeline.codeFragments.size() << "\n";
    status << "Errors: " << pipeline.stageErrors.size() << "\n";
    status << "Warnings: " << pipeline.stageWarnings.size() << "\n";
    return status.str();
}

std::unordered_map<std::string, std::string> IntegratedCompilerSystem::getSystemStatus() {
    std::unordered_map<std::string, std::string> status;
    
    status["initialized"] = initialized_ ? "true" : "false";
    status["debug_mode"] = debugMode_ ? "true" : "false";
    status["pipeline_logging"] = pipelineLogging_ ? "true" : "false";
    
    if (scanner_) status["scanner"] = "initialized";
    if (dispatcher_) status["dispatcher"] = "initialized";
    if (constraintManager_) status["constraint_manager"] = "initialized";
    
    return status;
}

void IntegratedCompilerSystem::logPipelineStage(const CompilationPipeline& pipeline, const std::string& message) {
    if (pipelineLogging_ || debugMode_) {
        std::cout << "[Pipeline:" << static_cast<int>(pipeline.currentStage) << "] " << message << std::endl;
    }
}

void IntegratedCompilerSystem::handlePipelineError(CompilationPipeline& pipeline, const std::string& error) {
    pipeline.stageErrors.push_back(error);
    pipeline.finalResult.success = false;
    pipeline.finalResult.errors.push_back(error);
    
    if (debugMode_) {
        std::cerr << "[Pipeline Error] " << error << std::endl;
    }
}

bool IntegratedCompilerSystem::validatePipelineStage(const CompilationPipeline& pipeline, PipelineStage expectedStage) {
    return pipeline.currentStage == expectedStage;
}

// ==================== CompilerSystemFactory 实现 ====================

std::unique_ptr<IntegratedCompilerSystem> CompilerSystemFactory::createDefaultSystem() {
    auto system = std::make_unique<IntegratedCompilerSystem>();
    configureDefaultComponents(system.get());
    return system;
}

std::unique_ptr<IntegratedCompilerSystem> CompilerSystemFactory::createSystemWithConfig(
    std::shared_ptr<CHtlConfiguration> config) {
    auto system = createDefaultSystem();
    // 应用特定配置
    return system;
}

std::unique_ptr<IntegratedCompilerSystem> CompilerSystemFactory::createTestingSystem() {
    auto system = std::make_unique<IntegratedCompilerSystem>();
    configureTestingComponents(system.get());
    return system;
}

void CompilerSystemFactory::configureDefaultComponents(IntegratedCompilerSystem* system) {
    if (system) {
        system->setDebugMode(false);
        system->enablePipelineLogging(false);
    }
}

void CompilerSystemFactory::configureTestingComponents(IntegratedCompilerSystem* system) {
    if (system) {
        system->setDebugMode(true);
        system->enablePipelineLogging(true);
        
        // 加载测试用例
        auto chtlFramework = system->getCHTLTestFramework();
        if (chtlFramework) {
            chtlFramework->loadTestCases("");
        }
    }
}

} // namespace chtl
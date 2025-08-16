#pragma once

#include "core/chtl_unified_scanner.hpp"
#include "core/compiler_dispatcher.hpp"
#include "core/constraint_validator.hpp"
#include "core/chtl_configuration.hpp"
#include "constraints/raw_embedding_parser.hpp"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace chtl {

// 前向声明
class CHTLLocalStateMachine;
class CHTLJSLocalStateMachine;
class CHTLTestFramework;
class CHTLJSTestFramework;
class CHTLErrorHandler;
class CHTLJSErrorHandler;

/**
 * 编译流水线阶段
 */
enum class PipelineStage {
    SOURCE_INPUT,           // 源码输入
    CONFIGURATION_LOAD,     // 配置加载
    UNIFIED_SCANNING,       // 统一扫描
    PRECISE_SLICING,        // 精确切片
    CONSTRAINT_VALIDATION,  // 约束验证
    COMPILER_ROUTING,       // 编译器路由
    LOCAL_COMPILATION,      // 局部编译
    RESULT_MERGING,         // 结果合并
    OUTPUT_GENERATION       // 输出生成
};

/**
 * 编译管道数据
 */
struct CompilationPipeline {
    // 输入数据
    std::string sourceCode;
    std::shared_ptr<CHtlConfiguration> configuration;
    
    // 扫描和切片结果
    std::vector<CodeFragment> codeFragments;
    
    // 验证结果
    std::vector<ConstraintValidationResult> validationResults;
    PreCompilationValidator::PreValidationResult preValidationResult;
    
    // 编译结果
    std::vector<FragmentCompilationResult> fragmentResults;
    CompilationResult finalResult;
    
    // 元数据和状态
    PipelineStage currentStage;
    std::unordered_map<std::string, std::string> metadata;
    std::vector<std::string> stageErrors;
    std::vector<std::string> stageWarnings;
    
    CompilationPipeline() : currentStage(PipelineStage::SOURCE_INPUT) {}
};

/**
 * CHTL局部状态机和上下文
 */
class CHTLLocalStateMachine {
public:
    enum class CHTLState {
        IDLE,
        PARSING_TEMPLATE_VAR,
        PARSING_CUSTOM_VAR,
        PARSING_STYLE_GROUP,
        PARSING_INHERITANCE,
        PARSING_LOCAL_STYLE,
        GENERATING_OUTPUT,
        ERROR_STATE
    };
    
    CHTLLocalStateMachine();
    
    // 状态控制
    bool transitionTo(CHTLState newState, const std::string& context = "");
    CHTLState getCurrentState() const { return currentState_; }
    
    // 上下文管理
    void pushContext(const std::string& context);
    void popContext();
    std::string getCurrentContext() const;
    
    // 错误处理
    void reportError(const std::string& error);
    std::vector<std::string> getErrors() const { return errors_; }
    void clearErrors() { errors_.clear(); }
    
private:
    CHTLState currentState_;
    std::vector<std::string> contextStack_;
    std::vector<std::string> errors_;
    
    bool isValidTransition(CHTLState from, CHTLState to) const;
};

/**
 * CHTL JS局部状态机和上下文
 */
class CHTLJSLocalStateMachine {
public:
    enum class CHTLJSState {
        IDLE,
        PARSING_MIXED_SYNTAX,
        SEPARATING_CHTL_JS,
        PARSING_VARIABLE_GROUP,
        PARSING_FROM_CLAUSE,
        PARSING_JS_TEMPLATE,
        GENERATING_OUTPUT,
        ERROR_STATE
    };
    
    CHTLJSLocalStateMachine();
    
    // 状态控制
    bool transitionTo(CHTLJSState newState, const std::string& context = "");
    CHTLJSState getCurrentState() const { return currentState_; }
    
    // 上下文管理
    void pushContext(const std::string& context);
    void popContext();
    std::string getCurrentContext() const;
    
    // 错误处理
    void reportError(const std::string& error);
    std::vector<std::string> getErrors() const { return errors_; }
    void clearErrors() { errors_.clear(); }
    
private:
    CHTLJSState currentState_;
    std::vector<std::string> contextStack_;
    std::vector<std::string> errors_;
    
    bool isValidTransition(CHTLJSState from, CHTLJSState to) const;
};

/**
 * CHTL测试框架
 */
class CHTLTestFramework {
public:
    struct TestCase {
        std::string name;
        std::string input;
        std::string expectedOutput;
        bool shouldPass;
        std::vector<std::string> expectedErrors;
        std::string description;
    };
    
    struct TestResult {
        bool passed;
        std::string actualOutput;
        std::vector<std::string> actualErrors;
        double executionTime;
        std::string failureReason;
    };
    
    CHTLTestFramework();
    
    // 测试用例管理
    void addTestCase(const TestCase& testCase);
    void loadTestCases(const std::string& filePath);
    
    // 测试执行
    TestResult runSingleTest(const TestCase& testCase);
    std::vector<TestResult> runAllTests();
    
    // 测试报告
    struct TestSummary {
        int totalTests;
        int passedTests;
        int failedTests;
        double totalTime;
        std::vector<std::string> failedTestNames;
    };
    
    TestSummary generateSummary(const std::vector<TestResult>& results);
    void printTestReport(const TestSummary& summary);
    
private:
    std::vector<TestCase> testCases_;
    
    bool compareOutput(const std::string& expected, const std::string& actual);
    bool compareErrors(const std::vector<std::string>& expected, const std::vector<std::string>& actual);
};

/**
 * CHTL JS测试框架
 */
class CHTLJSTestFramework {
public:
    struct JSTestCase {
        std::string name;
        std::string chtlJsInput;
        std::string expectedJsOutput;
        bool shouldPass;
        std::vector<std::string> expectedErrors;
        std::string description;
    };
    
    struct JSTestResult {
        bool passed;
        std::string actualJsOutput;
        std::vector<std::string> actualErrors;
        double executionTime;
        std::string failureReason;
    };
    
    CHTLJSTestFramework();
    
    // 测试用例管理
    void addTestCase(const JSTestCase& testCase);
    void loadTestCases(const std::string& filePath);
    
    // 测试执行
    JSTestResult runSingleTest(const JSTestCase& testCase);
    std::vector<JSTestResult> runAllTests();
    
    // 测试报告
    struct JSTestSummary {
        int totalTests;
        int passedTests;
        int failedTests;
        double totalTime;
        std::vector<std::string> failedTestNames;
    };
    
    JSTestSummary generateSummary(const std::vector<JSTestResult>& results);
    void printTestReport(const JSTestSummary& summary);
    
private:
    std::vector<JSTestCase> testCases_;
    
    bool compareJsOutput(const std::string& expected, const std::string& actual);
    bool validateJsSyntax(const std::string& jsCode);
};

/**
 * CHTL错误处理框架
 */
class CHTLErrorHandler {
public:
    enum class ErrorType {
        SYNTAX_ERROR,
        SEMANTIC_ERROR,
        CONSTRAINT_ERROR,
        RUNTIME_ERROR,
        COMPILATION_ERROR
    };
    
    struct CHTLError {
        ErrorType type;
        std::string message;
        std::string location;
        int line;
        int column;
        std::string suggestion;
        std::string code;  // 错误代码
    };
    
    CHTLErrorHandler();
    
    // 错误报告
    void reportError(ErrorType type, const std::string& message, 
                    const std::string& location = "", int line = -1, int column = -1);
    void reportSyntaxError(const std::string& message, int line, int column);
    void reportConstraintError(const std::string& message, const std::string& context);
    
    // 错误查询
    std::vector<CHTLError> getAllErrors() const { return errors_; }
    std::vector<CHTLError> getErrorsByType(ErrorType type) const;
    bool hasErrors() const { return !errors_.empty(); }
    bool hasCriticalErrors() const;
    
    // 错误处理
    void clearErrors() { errors_.clear(); }
    std::string generateErrorReport() const;
    std::vector<std::string> generateFixSuggestions() const;
    
private:
    std::vector<CHTLError> errors_;
    
    std::string getErrorTypeString(ErrorType type) const;
    std::string generateSuggestion(const CHTLError& error) const;
    bool isCritical(ErrorType type) const;
};

/**
 * CHTL JS错误处理框架
 */
class CHTLJSErrorHandler {
public:
    enum class JSErrorType {
        CHTL_SYNTAX_ERROR,
        JS_SYNTAX_ERROR,
        MIXING_ERROR,
        VARIABLE_ERROR,
        COMPILATION_ERROR
    };
    
    struct CHTLJSError {
        JSErrorType type;
        std::string message;
        std::string location;
        int line;
        int column;
        std::string suggestion;
        std::string code;
        bool isChtlRelated;  // 是否与CHTL语法相关
    };
    
    CHTLJSErrorHandler();
    
    // 错误报告
    void reportError(JSErrorType type, const std::string& message, 
                    const std::string& location = "", int line = -1, int column = -1);
    void reportMixingError(const std::string& message, const std::string& context);
    void reportVariableError(const std::string& varName, const std::string& issue);
    
    // 错误查询
    std::vector<CHTLJSError> getAllErrors() const { return errors_; }
    std::vector<CHTLJSError> getChtlErrors() const;
    std::vector<CHTLJSError> getJsErrors() const;
    bool hasErrors() const { return !errors_.empty(); }
    
    // 错误处理
    void clearErrors() { errors_.clear(); }
    std::string generateErrorReport() const;
    std::vector<std::string> generateFixSuggestions() const;
    
private:
    std::vector<CHTLJSError> errors_;
    
    std::string getErrorTypeString(JSErrorType type) const;
    std::string generateSuggestion(const CHTLJSError& error) const;
};

/**
 * 集成编译器系统
 * 将所有组件组合成完整的编译系统
 */
class IntegratedCompilerSystem {
public:
    IntegratedCompilerSystem();
    ~IntegratedCompilerSystem();
    
    // 系统初始化
    bool initialize();
    void cleanup();
    
    // 主编译接口
    CompilationResult compile(const std::string& sourceCode);
    CompilationResult compileWithConfig(const std::string& sourceCode, 
                                       std::shared_ptr<CHtlConfiguration> config);
    
    // 流水线执行
    bool executePipeline(CompilationPipeline& pipeline);
    
    // 各阶段执行方法
    bool executeSourceInput(CompilationPipeline& pipeline, const std::string& sourceCode);
    bool executeConfigurationLoad(CompilationPipeline& pipeline, std::shared_ptr<CHtlConfiguration> config);
    bool executeUnifiedScanning(CompilationPipeline& pipeline);
    bool executePreciseSlicing(CompilationPipeline& pipeline);
    bool executeConstraintValidation(CompilationPipeline& pipeline);
    bool executeCompilerRouting(CompilationPipeline& pipeline);
    bool executeLocalCompilation(CompilationPipeline& pipeline);
    bool executeResultMerging(CompilationPipeline& pipeline);
    bool executeOutputGeneration(CompilationPipeline& pipeline);
    
    // 组件访问接口
    std::shared_ptr<CHTLUnifiedScanner> getScanner() { return scanner_; }
    std::shared_ptr<CompilerDispatcher> getDispatcher() { return dispatcher_; }
    std::shared_ptr<ConstraintValidationManager> getConstraintManager() { return constraintManager_; }
    
    // 测试和错误框架访问
    std::shared_ptr<CHTLTestFramework> getCHTLTestFramework() { return chtlTestFramework_; }
    std::shared_ptr<CHTLJSTestFramework> getCHTLJSTestFramework() { return chtlJSTestFramework_; }
    std::shared_ptr<CHTLErrorHandler> getCHTLErrorHandler() { return chtlErrorHandler_; }
    std::shared_ptr<CHTLJSErrorHandler> getCHTLJSErrorHandler() { return chtlJSErrorHandler_; }
    
    // 调试和监控
    void setDebugMode(bool enabled);
    void enablePipelineLogging(bool enabled);
    std::string getPipelineStatus(const CompilationPipeline& pipeline);
    
    // 系统状态
    bool isInitialized() const { return initialized_; }
    std::unordered_map<std::string, std::string> getSystemStatus();
    
private:
    bool initialized_;
    bool debugMode_;
    bool pipelineLogging_;
    
    // 核心组件
    std::shared_ptr<CHTLUnifiedScanner> scanner_;
    std::shared_ptr<CompilerDispatcher> dispatcher_;
    std::shared_ptr<ConstraintValidationManager> constraintManager_;
    std::shared_ptr<PreCompilationValidator> preValidator_;
    
    // 局部状态机
    std::unique_ptr<CHTLLocalStateMachine> chtlStateMachine_;
    std::unique_ptr<CHTLJSLocalStateMachine> chtlJSStateMachine_;
    
    // 测试框架
    std::shared_ptr<CHTLTestFramework> chtlTestFramework_;
    std::shared_ptr<CHTLJSTestFramework> chtlJSTestFramework_;
    
    // 错误处理框架
    std::shared_ptr<CHTLErrorHandler> chtlErrorHandler_;
    std::shared_ptr<CHTLJSErrorHandler> chtlJSErrorHandler_;
    
    // 私有方法
    void initializeComponents();
    void setupComponentConnections();
    bool validatePipelineStage(const CompilationPipeline& pipeline, PipelineStage expectedStage);
    void logPipelineStage(const CompilationPipeline& pipeline, const std::string& message);
    void handlePipelineError(CompilationPipeline& pipeline, const std::string& error);
};

/**
 * 系统工厂
 * 创建和配置完整的编译系统
 */
class CompilerSystemFactory {
public:
    static std::unique_ptr<IntegratedCompilerSystem> createDefaultSystem();
    static std::unique_ptr<IntegratedCompilerSystem> createSystemWithConfig(
        std::shared_ptr<CHtlConfiguration> config);
    static std::unique_ptr<IntegratedCompilerSystem> createTestingSystem();
    
private:
    static void configureDefaultComponents(IntegratedCompilerSystem* system);
    static void configureTestingComponents(IntegratedCompilerSystem* system);
};

} // namespace chtl
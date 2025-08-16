#pragma once

#include "core/chtl_unified_scanner.hpp"
#include "core/compiler_dispatcher.hpp"
#include "core/chtl_configuration.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <functional>
#include <chrono>

namespace chtl {

/**
 * 编译阶段枚举
 */
enum class CompilationPhase {
    INITIALIZATION,     // 初始化阶段
    CONFIGURATION,      // 配置加载阶段
    SCANNING,          // 扫描阶段
    SLICING,           // 切片阶段
    ROUTING,           // 路由阶段
    COMPILATION,       // 编译阶段
    MERGING,           // 合并阶段
    OPTIMIZATION,      // 优化阶段
    FINALIZATION,      // 完成阶段
    ERROR_HANDLING     // 错误处理阶段
};

/**
 * 编译状态
 */
enum class CompilationState {
    IDLE,              // 空闲状态
    PROCESSING,        // 处理中
    WAITING,           // 等待状态
    SUCCESS,           // 成功状态
    FAILED,            // 失败状态
    CANCELLED          // 取消状态
};

/**
 * 语法上下文类型
 */
enum class SyntaxContext {
    GLOBAL_SCOPE,      // 全局作用域
    LOCAL_STYLE,       // 局部样式上下文
    LOCAL_SCRIPT,      // 局部脚本上下文
    TEMPLATE_CONTEXT,  // 模板上下文
    CUSTOM_CONTEXT,    // 自定义上下文
    ORIGIN_CONTEXT,    // 原始嵌入上下文
    NAMESPACE_CONTEXT  // 命名空间上下文
};

/**
 * 编译上下文
 * 管理整个编译过程的状态、配置和数据流
 */
class CompilationContext {
public:
    CompilationContext();
    ~CompilationContext();
    
    // 阶段管理
    CompilationPhase getCurrentPhase() const { return currentPhase_; }
    CompilationState getCurrentState() const { return currentState_; }
    void setPhase(CompilationPhase phase);
    void setState(CompilationState state);
    
    // 配置管理
    void setConfiguration(std::shared_ptr<CHtlConfiguration> config);
    std::shared_ptr<CHtlConfiguration> getConfiguration() const { return configuration_; }
    
    // 源代码管理
    void setSourceCode(const std::string& source);
    const std::string& getSourceCode() const { return sourceCode_; }
    
    // 片段管理
    void setCodeFragments(const std::vector<CodeFragment>& fragments);
    const std::vector<CodeFragment>& getCodeFragments() const { return codeFragments_; }
    void addCodeFragment(const CodeFragment& fragment);
    
    // 编译结果管理
    void addFragmentResult(const FragmentCompilationResult& result);
    const std::vector<FragmentCompilationResult>& getFragmentResults() const { return fragmentResults_; }
    
    // 最终结果管理
    void setFinalResult(const CompilationResult& result);
    const CompilationResult& getFinalResult() const { return finalResult_; }
    
    // 错误和警告管理
    void addError(const std::string& error, const std::string& phase = "");
    void addWarning(const std::string& warning, const std::string& phase = "");
    const std::vector<std::string>& getErrors() const { return errors_; }
    const std::vector<std::string>& getWarnings() const { return warnings_; }
    
    // 元数据管理
    void setMetadata(const std::string& key, const std::string& value);
    std::string getMetadata(const std::string& key) const;
    const std::unordered_map<std::string, std::string>& getAllMetadata() const { return metadata_; }
    
    // 语法上下文管理
    void pushSyntaxContext(SyntaxContext context);
    void popSyntaxContext();
    SyntaxContext getCurrentSyntaxContext() const;
    const std::vector<SyntaxContext>& getSyntaxContextStack() const { return syntaxContextStack_; }
    
    // 性能监控
    void startTimer(const std::string& name);
    void stopTimer(const std::string& name);
    double getElapsedTime(const std::string& name) const;
    
    // 调试支持
    void setDebugMode(bool enabled) { debugMode_ = enabled; }
    bool isDebugMode() const { return debugMode_; }
    void debugLog(const std::string& message, const std::string& component = "");
    
    // 清理方法
    void clear();
    
private:
    CompilationPhase currentPhase_;
    CompilationState currentState_;
    
    std::shared_ptr<CHtlConfiguration> configuration_;
    std::string sourceCode_;
    std::vector<CodeFragment> codeFragments_;
    std::vector<FragmentCompilationResult> fragmentResults_;
    CompilationResult finalResult_;
    
    std::vector<std::string> errors_;
    std::vector<std::string> warnings_;
    std::unordered_map<std::string, std::string> metadata_;
    
    std::vector<SyntaxContext> syntaxContextStack_;
    
    std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> timers_;
    std::unordered_map<std::string, double> elapsedTimes_;
    
    bool debugMode_;
    
    void logPhaseChange(CompilationPhase oldPhase, CompilationPhase newPhase);
    void logStateChange(CompilationState oldState, CompilationState newState);
};

/**
 * 编译状态机
 * 控制编译流程的状态转换和阶段管理
 */
class CompilationStateMachine {
public:
    using StateHandler = std::function<bool(CompilationContext&)>;
    using PhaseTransition = std::function<bool(CompilationContext&, CompilationPhase, CompilationPhase)>;
    
    CompilationStateMachine();
    
    // 状态处理器注册
    void registerPhaseHandler(CompilationPhase phase, StateHandler handler);
    void registerTransitionValidator(CompilationPhase from, CompilationPhase to, PhaseTransition validator);
    
    // 状态机执行
    bool executePhase(CompilationContext& context, CompilationPhase phase);
    bool transitionTo(CompilationContext& context, CompilationPhase targetPhase);
    bool runFullCompilation(CompilationContext& context);
    
    // 错误恢复
    bool handleError(CompilationContext& context, const std::string& error);
    void setErrorRecoveryHandler(CompilationPhase phase, StateHandler handler);
    
private:
    std::unordered_map<CompilationPhase, StateHandler> phaseHandlers_;
    std::unordered_map<std::pair<CompilationPhase, CompilationPhase>, PhaseTransition, 
                      std::hash<std::pair<int, int>>> transitionValidators_;
    std::unordered_map<CompilationPhase, StateHandler> errorRecoveryHandlers_;
    
    bool isValidTransition(CompilationPhase from, CompilationPhase to) const;
    CompilationPhase getNextPhase(CompilationPhase current) const;
};

/**
 * 语法上下文管理器
 * 管理不同语法环境下的解析规则和约束
 */
class SyntaxContextManager {
public:
    SyntaxContextManager();
    
    // 上下文切换
    void enterContext(SyntaxContext context, CompilationContext& compilationContext);
    void exitContext(CompilationContext& compilationContext);
    
    // 约束验证
    bool validateSyntax(const std::string& code, SyntaxContext context, std::vector<std::string>& errors);
    bool isAllowedInContext(const std::string& syntaxElement, SyntaxContext context);
    
    // 上下文特定的解析规则
    std::vector<std::string> getApplicableRules(SyntaxContext context);
    bool shouldApplyRule(const std::string& rule, SyntaxContext context);
    
    // 上下文继承
    void setContextInheritance(SyntaxContext child, SyntaxContext parent);
    std::vector<SyntaxContext> getInheritanceChain(SyntaxContext context);
    
private:
    std::unordered_map<SyntaxContext, std::vector<std::string>> contextRules_;
    std::unordered_map<SyntaxContext, std::unordered_set<std::string>> allowedElements_;
    std::unordered_map<SyntaxContext, SyntaxContext> contextInheritance_;
    
    void initializeDefaultRules();
    void initializeAllowedElements();
    bool inheritsRule(SyntaxContext context, const std::string& rule);
};

/**
 * 组件协调器
 * 协调扫描器、切片器、调度器之间的交互
 */
class ComponentCoordinator {
public:
    ComponentCoordinator();
    
    // 组件注册
    void setUnifiedScanner(std::shared_ptr<CHTLUnifiedScanner> scanner);
    void setCompilerDispatcher(std::shared_ptr<CompilerDispatcher> dispatcher);
    void setStateMachine(std::shared_ptr<CompilationStateMachine> stateMachine);
    void setContextManager(std::shared_ptr<SyntaxContextManager> contextManager);
    
    // 编译流程协调
    CompilationResult coordinateCompilation(const std::string& sourceCode, 
                                          std::shared_ptr<CHtlConfiguration> config = nullptr);
    
    // 阶段执行协调
    bool executeScanning(CompilationContext& context);
    bool executeSlicing(CompilationContext& context);
    bool executeRouting(CompilationContext& context);
    bool executeCompilation(CompilationContext& context);
    bool executeMerging(CompilationContext& context);
    
    // 错误处理协调
    bool handleCompilationError(CompilationContext& context, const std::string& component, 
                               const std::string& error);
    
    // 调试和监控
    void setDebugMode(bool enabled);
    std::unordered_map<std::string, std::string> getSystemStatus();
    
private:
    std::shared_ptr<CHTLUnifiedScanner> scanner_;
    std::shared_ptr<CompilerDispatcher> dispatcher_;
    std::shared_ptr<CompilationStateMachine> stateMachine_;
    std::shared_ptr<SyntaxContextManager> contextManager_;
    
    bool debugMode_;
    
    void logCoordinationStep(const std::string& step, const CompilationContext& context);
    bool validateComponentStates();
};

/**
 * 集成控制器
 * 整个编译系统的主控制器，管理所有组件的生命周期和交互
 */
class IntegrationController {
public:
    IntegrationController();
    ~IntegrationController();
    
    // 系统初始化
    bool initialize();
    void cleanup();
    
    // 编译接口
    CompilationResult compile(const std::string& sourceCode);
    CompilationResult compileFile(const std::string& filePath);
    CompilationResult compileWithConfig(const std::string& sourceCode, 
                                       const std::string& configPath);
    
    // 配置管理
    void setGlobalConfiguration(std::shared_ptr<CHtlConfiguration> config);
    void loadConfigurationFile(const std::string& configPath);
    
    // 系统控制
    void setDebugMode(bool enabled);
    void enablePerformanceMonitoring(bool enabled);
    
    // 状态查询
    bool isInitialized() const { return initialized_; }
    std::unordered_map<std::string, std::string> getSystemStatus();
    std::vector<std::string> getLastErrors();
    std::vector<std::string> getLastWarnings();
    
    // 高级功能
    void setCompilerIsolationMode(bool enabled);
    void setParallelCompilation(bool enabled);
    void setMemoryOptimization(bool enabled);
    
private:
    bool initialized_;
    bool debugMode_;
    bool performanceMonitoring_;
    bool compilerIsolation_;
    bool parallelCompilation_;
    bool memoryOptimization_;
    
    std::shared_ptr<CHtlConfiguration> globalConfig_;
    std::unique_ptr<ComponentCoordinator> coordinator_;
    std::unique_ptr<CompilationContext> lastContext_;
    
    void initializeComponents();
    void setupDefaultHandlers();
    bool validateSystemIntegrity();
    void logSystemEvent(const std::string& event);
};

} // namespace chtl
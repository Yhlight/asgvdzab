#pragma once

#include "state/raii_state_machine.hpp"
#include "state/context_manager.hpp"
#include "state/ast_state_tracker.hpp"
#include "parsers/chtl_parser.hpp"
#include "generators/chtl_generator.hpp"
#include "compilers/chtl_compiler.hpp"
#include "common/types.hpp"
#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <chrono>

namespace chtl {

/**
 * 编译器状态集成协调器
 * 协调RAII状态机与编译器系统的交互
 */
class CompilerStateIntegrator {
public:
    explicit CompilerStateIntegrator(RAIIStateMachine& stateMachine, 
                                   ContextManager& contextManager,
                                   ASTStateTracker& astTracker);
    ~CompilerStateIntegrator();

    // ===== 编译器集成 =====
    
    /**
     * 集成CHTL解析器
     * @param parser 解析器实例
     * @return 是否集成成功
     */
    bool integrateCHTLParser(CHTLParser& parser);
    
    /**
     * 集成CHTL生成器
     * @param generator 生成器实例
     * @return 是否集成成功
     */
    bool integrateCHTLGenerator(CHTLGenerator& generator);
    
    /**
     * 集成CHTL编译器
     * @param compiler 编译器实例
     * @return 是否集成成功
     */
    bool integrateCHTLCompiler(CHTLCompiler& compiler);
    
    /**
     * 移除编译器集成
     * @param compiler 编译器实例
     */
    void removeIntegration(void* compiler);

    // ===== 编译事件处理 =====
    
    /**
     * 处理解析开始事件
     * @param source 源代码
     * @param filename 文件名
     */
    void onParseStart(const std::string& source, const std::string& filename);
    
    /**
     * 处理解析结束事件
     * @param result 解析结果
     */
    void onParseEnd(const CHTLParseResult& result);
    
    /**
     * 处理AST节点创建事件
     * @param nodePtr 节点指针
     * @param nodeType 节点类型
     * @param position 源码位置
     */
    void onASTNodeCreated(const void* nodePtr, const std::string& nodeType, const Position& position);
    
    /**
     * 处理AST节点处理事件
     * @param nodePtr 节点指针
     * @param phase 处理阶段
     */
    void onASTNodeProcessed(const void* nodePtr, const std::string& phase);
    
    /**
     * 处理生成开始事件
     * @param ast AST根节点
     */
    void onGenerateStart(CHTLASTNodePtr ast);
    
    /**
     * 处理生成结束事件
     * @param result 生成结果
     */
    void onGenerateEnd(const CHTLGenerateResult& result);
    
    /**
     * 处理编译错误事件
     * @param error 错误信息
     * @param position 错误位置
     */
    void onCompileError(const std::string& error, const Position& position);

    // ===== 状态同步 =====
    
    /**
     * 同步解析状态
     * @param phase 解析阶段
     * @param nodePtr 当前节点
     */
    void syncParseState(const std::string& phase, const void* nodePtr = nullptr);
    
    /**
     * 同步生成状态
     * @param phase 生成阶段
     * @param nodePtr 当前节点
     */
    void syncGenerateState(const std::string& phase, const void* nodePtr = nullptr);
    
    /**
     * 重置编译状态
     */
    void resetCompileStates();

    // ===== 编译阶段映射 =====
    
    /**
     * 将编译阶段映射到语法状态
     * @param phase 编译阶段
     * @return 对应的语法状态
     */
    static CHTLGrammarState mapCompilePhaseToGrammarState(const std::string& phase);
    
    /**
     * 将编译阶段映射到上下文类型
     * @param phase 编译阶段
     * @return 对应的上下文类型
     */
    static CHTLContextType mapCompilePhaseToContextType(const std::string& phase);
    
    /**
     * 将AST节点类型映射到上下文类型
     * @param nodeType AST节点类型
     * @return 对应的上下文类型
     */
    static CHTLContextType mapASTNodeTypeToContextType(const std::string& nodeType);

    // ===== 配置和控制 =====
    
    /**
     * 启用自动状态跟踪
     * @param enabled 是否启用
     */
    void setAutoTrackingEnabled(bool enabled);
    
    /**
     * 获取自动跟踪状态
     * @return 是否启用自动跟踪
     */
    bool isAutoTrackingEnabled() const;
    
    /**
     * 设置性能监控模式
     * @param enabled 是否启用
     */
    void setPerformanceMonitoring(bool enabled);

    // ===== 统计和监控 =====
    
    /**
     * 获取编译统计信息
     * @return 统计信息映射
     */
    std::unordered_map<std::string, size_t> getCompileStatistics() const;
    
    /**
     * 获取性能指标
     * @return 性能指标映射
     */
    std::unordered_map<std::string, double> getPerformanceMetrics() const;
    
    /**
     * 重置统计信息
     */
    void resetStatistics();

private:
    // 内部状态管理
    void updateCompilePhase(const std::string& phase, const void* nodePtr);
    void trackASTNodeLifecycle(const void* nodePtr, const std::string& event);
    void recordPerformanceMetric(const std::string& metric, double value);
    
    // 日志和诊断
    void logCompileEvent(const std::string& event);
    void logPerformanceData();

private:
    RAIIStateMachine& stateMachine_;
    ContextManager& contextManager_;
    ASTStateTracker& astTracker_;
    
    // 配置选项
    bool autoTrackingEnabled_;
    bool performanceMonitoring_;
    
    // 统计信息
    std::unordered_map<std::string, size_t> compileStats_;
    std::unordered_map<std::string, double> performanceMetrics_;
    
    // 状态跟踪
    std::string currentPhase_;
    std::unordered_map<const void*, std::string> nodePhases_;
    std::vector<void*> integratedCompilers_;
    
    // 时间测量
    std::chrono::high_resolution_clock::time_point startTime_;
    std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> phaseStartTimes_;
};

/**
 * 增强的CHTL解析器包装器
 * 在原有解析器基础上添加状态管理功能
 */
class EnhancedCHTLParser {
public:
    explicit EnhancedCHTLParser(RAIIStateMachine& stateMachine, 
                               ContextManager& contextManager,
                               ASTStateTracker& astTracker,
                               const CHTLParserConfig& config = CHTLParserConfig());
    ~EnhancedCHTLParser();

    // ===== 解析接口 =====
    
    /**
     * 解析CHTL源代码
     * @param source 源代码
     * @param filename 文件名
     * @return 解析结果
     */
    CHTLParseResult parse(const std::string& source, const std::string& filename = "");
    
    /**
     * 解析Token序列
     * @param tokens Token序列
     * @param filename 文件名
     * @return 解析结果
     */
    CHTLParseResult parseTokens(const std::vector<PureCHTLToken>& tokens, const std::string& filename = "");

    // ===== 状态查询 =====
    
    /**
     * 获取当前解析状态
     * @return 当前解析状态
     */
    CHTLGrammarState getCurrentParseState() const;
    
    /**
     * 获取当前解析上下文
     * @return 当前上下文类型
     */
    CHTLContextType getCurrentParseContext() const;
    
    /**
     * 获取解析统计信息
     * @return 统计信息字符串
     */
    std::string getParseStatistics() const;

    // ===== 配置 =====
    
    /**
     * 更新解析器配置
     * @param config 新配置
     */
    void updateConfiguration(const CHTLParserConfig& config);
    
    /**
     * 重置解析器状态
     */
    void reset();

private:
    std::unique_ptr<CHTLParser> parser_;
    std::unique_ptr<CompilerStateIntegrator> integrator_;
    RAIIStateMachine& stateMachine_;
    ContextManager& contextManager_;
    ASTStateTracker& astTracker_;
};

/**
 * 增强的CHTL生成器包装器
 * 在原有生成器基础上添加状态管理功能
 */
class EnhancedCHTLGenerator {
public:
    explicit EnhancedCHTLGenerator(RAIIStateMachine& stateMachine, 
                                  ContextManager& contextManager,
                                  ASTStateTracker& astTracker,
                                  const CHTLGeneratorConfig& config = CHTLGeneratorConfig());
    ~EnhancedCHTLGenerator();

    // ===== 生成接口 =====
    
    /**
     * 生成HTML
     * @param ast AST根节点
     * @param filename 文件名
     * @return 生成结果
     */
    CHTLGenerateResult generate(CHTLASTNodePtr ast, const std::string& filename = "");

    // ===== 库管理 =====
    
    /**
     * 设置模板库
     * @param templates 模板库
     */
    void setTemplateLibrary(const std::unordered_map<std::string, CHTLASTNodePtr>& templates);
    
    /**
     * 设置自定义库
     * @param customs 自定义库
     */
    void setCustomLibrary(const std::unordered_map<std::string, CHTLASTNodePtr>& customs);

    // ===== 状态查询 =====
    
    /**
     * 获取当前生成状态
     * @return 当前生成状态
     */
    CHTLGrammarState getCurrentGenerateState() const;
    
    /**
     * 获取生成统计信息
     * @return 统计信息字符串
     */
    std::string getGenerateStatistics() const;

    // ===== 配置 =====
    
    /**
     * 更新生成器配置
     * @param config 新配置
     */
    void updateConfiguration(const CHTLGeneratorConfig& config);
    
    /**
     * 重置生成器状态
     */
    void reset();

private:
    std::unique_ptr<CHTLGenerator> generator_;
    std::unique_ptr<CompilerStateIntegrator> integrator_;
    RAIIStateMachine& stateMachine_;
    ContextManager& contextManager_;
    ASTStateTracker& astTracker_;
};

/**
 * 增强的CHTL编译器包装器
 * 在原有编译器基础上添加完整的状态管理功能
 */
class EnhancedCHTLCompiler {
public:
    explicit EnhancedCHTLCompiler(RAIIStateMachine& stateMachine, 
                                 ContextManager& contextManager,
                                 ASTStateTracker& astTracker);
    ~EnhancedCHTLCompiler();

    // ===== 编译接口 =====
    
    /**
     * 编译代码段
     * @param slice 代码段
     * @return 编译结果
     */
    std::string compile(const CodeSlice& slice);
    
    /**
     * 获取支持的代码段类型
     * @return 支持的类型列表
     */
    std::vector<CodeSegmentType> getSupportedTypes() const;

    // ===== 状态查询 =====
    
    /**
     * 获取当前编译状态
     * @return 当前编译状态
     */
    CHTLGrammarState getCurrentCompileState() const;
    
    /**
     * 获取编译统计信息
     * @return 统计信息字符串
     */
    std::string getCompileStatistics() const;

    // ===== 配置 =====
    
    /**
     * 重置编译器状态
     */
    void reset();

private:
    std::unique_ptr<CHTLCompiler> compiler_;
    std::unique_ptr<CompilerStateIntegrator> integrator_;
    RAIIStateMachine& stateMachine_;
    ContextManager& contextManager_;
    ASTStateTracker& astTracker_;
};

/**
 * 编译器状态监听器
 * 监听编译过程中的状态变化
 */
class CompilerStateListener : public IStateChangeListener, 
                             public IContextChangeListener, 
                             public IASTStateChangeListener {
public:
    explicit CompilerStateListener();
    ~CompilerStateListener() override;

    // ===== IStateChangeListener接口 =====
    void onStateEnter(CHTLGrammarState state, const std::string& context) override;
    void onStateExit(CHTLGrammarState state, const std::string& context) override;
    void onStateTransition(CHTLGrammarState from, CHTLGrammarState to, const std::string& reason) override;
    void onStateError(CHTLGrammarState currentState, const std::string& error) override;

    // ===== IContextChangeListener接口 =====
    void onContextEnter(CHTLContextType type, const std::string& identifier) override;
    void onContextExit(CHTLContextType type, const std::string& identifier) override;
    void onContextProperty(CHTLContextType type, const std::string& key, const std::string& value) override;
    void onContextError(CHTLContextType type, const std::string& error) override;

    // ===== IASTStateChangeListener接口 =====
    void onNodeStateChanged(const void* nodePtr, ASTNodeState oldState, ASTNodeState newState) override;
    void onNodeFlagChanged(const void* nodePtr, ASTNodeFlag flag, bool added) override;
    void onNodeError(const void* nodePtr, const std::string& error) override;
    void onNodeStatsUpdated(const void* nodePtr, const ASTNodeStats& stats) override;

    // ===== 事件统计 =====
    
    /**
     * 获取事件统计信息
     * @return 事件统计映射
     */
    std::unordered_map<std::string, size_t> getEventStatistics() const;
    
    /**
     * 获取错误统计信息
     * @return 错误统计映射
     */
    std::unordered_map<std::string, size_t> getErrorStatistics() const;
    
    /**
     * 重置统计信息
     */
    void resetStatistics();

private:
    std::unordered_map<std::string, size_t> eventCounts_;
    std::unordered_map<std::string, size_t> errorCounts_;
    mutable std::mutex statsMutex_;
    
    void incrementEventCount(const std::string& event);
    void incrementErrorCount(const std::string& error);
};

/**
 * 编译器状态集成工厂
 */
class CompilerStateIntegrationFactory {
public:
    /**
     * 创建完整的状态管理编译器系统
     */
    struct StateManagedCompilerSystem {
        std::unique_ptr<RAIIStateMachine> stateMachine;
        std::unique_ptr<ContextManager> contextManager;
        std::unique_ptr<ASTStateTracker> astTracker;
        std::unique_ptr<EnhancedCHTLParser> parser;
        std::unique_ptr<EnhancedCHTLGenerator> generator;
        std::unique_ptr<EnhancedCHTLCompiler> compiler;
        std::shared_ptr<CompilerStateListener> listener;
    };
    
    /**
     * 创建CHTL状态管理编译器系统
     * @return 完整系统实例
     */
    static StateManagedCompilerSystem createCHTLSystem();
    
    /**
     * 创建CHTL JS状态管理编译器系统
     * @return 完整系统实例
     */
    static StateManagedCompilerSystem createCHTLJSSystem();
    
    /**
     * 创建通用状态管理编译器系统
     * @return 完整系统实例
     */
    static StateManagedCompilerSystem createGenericSystem();
    
    /**
     * 创建完整的端到端系统
     * 包含扫描器、编译器和所有状态管理组件
     * @return 完整端到端系统
     */
    static struct CompleteStateManagedSystem {
        // 扫描器系统
        std::unique_ptr<RAIIStateMachine> scannerStateMachine;
        std::unique_ptr<ContextManager> scannerContextManager;
        std::unique_ptr<ASTStateTracker> scannerAstTracker;
        
        // 编译器系统
        std::unique_ptr<RAIIStateMachine> compilerStateMachine;
        std::unique_ptr<ContextManager> compilerContextManager;
        std::unique_ptr<ASTStateTracker> compilerAstTracker;
        
        // 增强组件
        std::unique_ptr<EnhancedCHTLParser> parser;
        std::unique_ptr<EnhancedCHTLGenerator> generator;
        std::unique_ptr<EnhancedCHTLCompiler> compiler;
        
        // 监听器
        std::shared_ptr<CompilerStateListener> listener;
    } createCompleteSystem();
};

// ===== 便利宏定义 =====

/**
 * 编译器状态同步宏
 */
#define CHTL_SYNC_COMPILE_STATE(integrator, phase, nodePtr) \
    (integrator).syncParseState(phase, nodePtr)

/**
 * 编译器错误报告宏
 */
#define CHTL_REPORT_COMPILE_ERROR(integrator, error, position) \
    (integrator).onCompileError(error, position)

/**
 * AST节点生命周期跟踪宏
 */
#define CHTL_TRACK_AST_NODE(integrator, nodePtr, nodeType, position) \
    (integrator).onASTNodeCreated(nodePtr, nodeType, position)

/**
 * 编译阶段跟踪宏
 */
#define CHTL_TRACK_COMPILE_PHASE(integrator, phase) \
    do { \
        (integrator).syncParseState(phase); \
        /* 编译逻辑 */ \
    } while(0)

} // namespace chtl
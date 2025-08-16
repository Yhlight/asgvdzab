#pragma once

#include "state/raii_state_machine.hpp"
#include "state/context_manager.hpp"
#include "state/ast_state_tracker.hpp"
#include "scanner/unified_scanner.hpp"
#include "scanner/slice_scanner.hpp"
#include "common/types.hpp"
#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <mutex>

namespace chtl {

/**
 * 扫描器状态集成协调器
 * 协调RAII状态机与现有扫描器系统的交互
 */
class ScannerStateIntegrator {
public:
    explicit ScannerStateIntegrator(RAIIStateMachine& stateMachine, 
                                   ContextManager& contextManager,
                                   ASTStateTracker& astTracker);
    ~ScannerStateIntegrator();

    // ===== 扫描器集成 =====
    
    /**
     * 集成CHTLUnifiedScanner
     * @param scanner 统一扫描器实例
     * @return 是否集成成功
     */
    bool integrateUnifiedScanner(CHTLUnifiedScanner& scanner);
    
    /**
     * 集成SliceScanner
     * @param scanner 切片扫描器实例
     * @return 是否集成成功
     */
    bool integrateSliceScanner(SliceScanner& scanner);
    
    /**
     * 移除扫描器集成
     * @param scanner 扫描器实例
     */
    void removeIntegration(void* scanner);

    // ===== 扫描事件处理 =====
    
    /**
     * 处理代码段开始事件
     * @param slice 代码段
     */
    void onSegmentStart(const CodeSlice& slice);
    
    /**
     * 处理代码段结束事件
     * @param slice 代码段
     */
    void onSegmentEnd(const CodeSlice& slice);
    
    /**
     * 处理Token扫描事件
     * @param token Token信息
     * @param slice 所属代码段
     */
    void onTokenScanned(const std::string& token, const CodeSlice& slice);
    
    /**
     * 处理扫描错误事件
     * @param error 错误信息
     * @param position 错误位置
     */
    void onScanError(const std::string& error, const Position& position);

    // ===== 状态同步 =====
    
    /**
     * 同步扫描器状态到状态机
     * @param segmentType 代码段类型
     * @param position 当前位置
     */
    void syncScannerState(CodeSegmentType segmentType, const Position& position);
    
    /**
     * 更新上下文信息
     * @param slice 代码段
     */
    void updateContext(const CodeSlice& slice);
    
    /**
     * 重置所有状态
     */
    void resetStates();

    // ===== 代码段类型映射 =====
    
    /**
     * 将CodeSegmentType映射到CHTLGrammarState
     * @param segmentType 代码段类型
     * @return 对应的语法状态
     */
    static CHTLGrammarState mapSegmentTypeToGrammarState(CodeSegmentType segmentType);
    
    /**
     * 将CodeSegmentType映射到CHTLContextType
     * @param segmentType 代码段类型
     * @return 对应的上下文类型
     */
    static CHTLContextType mapSegmentTypeToContextType(CodeSegmentType segmentType);

    // ===== 配置和控制 =====
    
    /**
     * 启用自动状态同步
     * @param enabled 是否启用
     */
    void setAutoSyncEnabled(bool enabled);
    
    /**
     * 获取自动同步状态
     * @return 是否启用自动同步
     */
    bool isAutoSyncEnabled() const;
    
    /**
     * 设置详细日志模式
     * @param enabled 是否启用
     */
    void setVerboseLogging(bool enabled);

    // ===== 统计和监控 =====
    
    /**
     * 获取处理的代码段数量
     * @return 代码段数量
     */
    size_t getProcessedSegmentCount() const;
    
    /**
     * 获取处理的Token数量
     * @return Token数量
     */
    size_t getProcessedTokenCount() const;
    
    /**
     * 获取错误数量
     * @return 错误数量
     */
    size_t getErrorCount() const;
    
    /**
     * 重置统计信息
     */
    void resetStatistics();

private:
    // 内部状态转换逻辑
    void handleSegmentTransition(CodeSegmentType fromType, CodeSegmentType toType, const Position& position);
    void updateGrammarState(CodeSegmentType segmentType, const Position& position);
    void updateContextState(CodeSegmentType segmentType, const std::string& identifier, const Position& position);
    
    // 日志记录
    void logStateTransition(const std::string& message);
    void logError(const std::string& error, const Position& position);

private:
    RAIIStateMachine& stateMachine_;
    ContextManager& contextManager_;
    ASTStateTracker& astTracker_;
    
    // 配置选项
    bool autoSyncEnabled_;
    bool verboseLogging_;
    
    // 统计信息
    size_t processedSegmentCount_;
    size_t processedTokenCount_;
    size_t errorCount_;
    
    // 状态跟踪
    CodeSegmentType currentSegmentType_;
    Position currentPosition_;
    std::vector<void*> integratedScanners_;
};

/**
 * 增强的CHTLUnifiedScanner包装器
 * 在原有扫描器基础上添加状态管理功能
 */
class EnhancedUnifiedScanner {
public:
    explicit EnhancedUnifiedScanner(RAIIStateMachine& stateMachine, 
                                   ContextManager& contextManager,
                                   ASTStateTracker& astTracker);
    ~EnhancedUnifiedScanner();

    // ===== 扫描接口 =====
    
    /**
     * 扫描源代码并生成代码段
     * @param source 源代码
     * @param filename 文件名
     * @return 代码段列表
     */
    std::vector<CodeSlice> scan(const std::string& source, const std::string& filename = "");
    
    /**
     * 增量扫描
     * @param source 源代码
     * @param startPos 开始位置
     * @param endPos 结束位置
     * @return 代码段列表
     */
    std::vector<CodeSlice> incrementalScan(const std::string& source, 
                                          const Position& startPos, 
                                          const Position& endPos);

    // ===== 状态查询 =====
    
    /**
     * 获取当前语法状态
     * @return 当前语法状态
     */
    CHTLGrammarState getCurrentGrammarState() const;
    
    /**
     * 获取当前上下文类型
     * @return 当前上下文类型
     */
    CHTLContextType getCurrentContextType() const;
    
    /**
     * 获取扫描统计信息
     * @return 统计信息字符串
     */
    std::string getStatistics() const;

    // ===== 配置 =====
    
    /**
     * 设置扫描配置
     * @param config 配置参数
     */
    void setConfiguration(const std::unordered_map<std::string, std::string>& config);
    
    /**
     * 重置扫描器状态
     */
    void reset();

private:
    std::unique_ptr<CHTLUnifiedScanner> scanner_;
    std::unique_ptr<ScannerStateIntegrator> integrator_;
    RAIIStateMachine& stateMachine_;
    ContextManager& contextManager_;
    ASTStateTracker& astTracker_;
};

/**
 * 增强的SliceScanner包装器
 * 在原有切片扫描器基础上添加状态管理功能
 */
class EnhancedSliceScanner {
public:
    explicit EnhancedSliceScanner(RAIIStateMachine& stateMachine, 
                                 ContextManager& contextManager,
                                 ASTStateTracker& astTracker);
    ~EnhancedSliceScanner();

    // ===== 切片扫描接口 =====
    
    /**
     * 扫描代码段
     * @param slice 输入代码段
     * @return 细化的代码段列表
     */
    std::vector<CodeSlice> scanSlice(const CodeSlice& slice);
    
    /**
     * 分析代码段类型
     * @param content 代码内容
     * @param position 位置信息
     * @return 代码段类型
     */
    CodeSegmentType analyzeSegmentType(const std::string& content, const Position& position);

    // ===== 状态管理 =====
    
    /**
     * 同步切片状态
     * @param slice 代码段
     */
    void syncSliceState(const CodeSlice& slice);
    
    /**
     * 获取切片统计信息
     * @return 统计信息
     */
    std::string getSliceStatistics() const;

private:
    std::unique_ptr<SliceScanner> scanner_;
    std::unique_ptr<ScannerStateIntegrator> integrator_;
    RAIIStateMachine& stateMachine_;
    ContextManager& contextManager_;
    ASTStateTracker& astTracker_;
};

/**
 * 扫描器状态监听器
 * 监听扫描过程中的状态变化
 */
class ScannerStateListener : public IStateChangeListener, public IContextChangeListener {
public:
    explicit ScannerStateListener();
    ~ScannerStateListener() override;

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

    // ===== 日志和统计 =====
    
    /**
     * 获取事件日志
     * @return 事件日志列表
     */
    const std::vector<std::string>& getEventLog() const;
    
    /**
     * 清空事件日志
     */
    void clearEventLog();
    
    /**
     * 设置日志级别
     * @param level 日志级别 (0=ERROR, 1=WARN, 2=INFO, 3=DEBUG)
     */
    void setLogLevel(int level);

private:
    std::vector<std::string> eventLog_;
    int logLevel_;
    mutable std::mutex logMutex_;
    
    void logEvent(const std::string& event, int level = 2);
};

/**
 * 扫描器状态集成工厂
 */
class ScannerStateIntegrationFactory {
public:
    /**
     * 创建完整的状态管理扫描器系统
     * @return 包含所有组件的系统
     */
    struct StateManagedScannerSystem {
        std::unique_ptr<RAIIStateMachine> stateMachine;
        std::unique_ptr<ContextManager> contextManager;
        std::unique_ptr<ASTStateTracker> astTracker;
        std::unique_ptr<EnhancedUnifiedScanner> unifiedScanner;
        std::unique_ptr<EnhancedSliceScanner> sliceScanner;
        std::shared_ptr<ScannerStateListener> listener;
    };
    
    /**
     * 创建CHTL状态管理扫描器系统
     * @return 完整系统实例
     */
    static StateManagedScannerSystem createCHTLSystem();
    
    /**
     * 创建CHTL JS状态管理扫描器系统
     * @return 完整系统实例
     */
    static StateManagedScannerSystem createCHTLJSSystem();
    
    /**
     * 创建通用状态管理扫描器系统
     * @return 完整系统实例
     */
    static StateManagedScannerSystem createGenericSystem();
};

// ===== 便利宏定义 =====

/**
 * 扫描器状态同步宏
 */
#define CHTL_SYNC_SCANNER_STATE(integrator, segmentType, position) \
    (integrator).syncScannerState(segmentType, position)

/**
 * 扫描器错误报告宏
 */
#define CHTL_REPORT_SCAN_ERROR(integrator, error, position) \
    (integrator).onScanError(error, position)

/**
 * 代码段处理宏
 */
#define CHTL_PROCESS_SEGMENT(integrator, slice) \
    do { \
        (integrator).onSegmentStart(slice); \
        /* 处理逻辑 */ \
        (integrator).onSegmentEnd(slice); \
    } while(0)

} // namespace chtl
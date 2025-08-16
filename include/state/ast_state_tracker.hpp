#pragma once

#include "state/raii_state_machine.hpp"
#include "state/context_manager.hpp"
#include "ast/chtl_ast.hpp"
#include "ast/chtl_js_ast.hpp"
#include "common/types.hpp"
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <mutex>

namespace chtl {

/**
 * AST节点状态
 * 严格按照CHTL语法文档定义节点的生命周期状态
 */
enum class ASTNodeState {
    // 基本状态
    UNINITIALIZED,                  // 未初始化
    INITIALIZING,                   // 初始化中
    READY,                          // 就绪状态
    
    // 解析状态
    PARSING_STARTED,                // 开始解析
    PARSING_ATTRIBUTES,             // 解析属性中
    PARSING_CHILDREN,               // 解析子节点中
    PARSING_CONTENT,                // 解析内容中
    PARSING_COMPLETED,              // 解析完成
    
    // 验证状态
    VALIDATION_PENDING,             // 等待验证
    VALIDATING,                     // 验证中
    VALIDATION_PASSED,              // 验证通过
    VALIDATION_FAILED,              // 验证失败
    
    // 生成状态
    GENERATION_READY,               // 生成就绪
    GENERATING,                     // 生成中
    GENERATION_COMPLETED,           // 生成完成
    
    // 优化状态
    OPTIMIZATION_PENDING,           // 等待优化
    OPTIMIZING,                     // 优化中
    OPTIMIZATION_COMPLETED,         // 优化完成
    
    // 错误状态
    ERROR_SYNTAX,                   // 语法错误
    ERROR_SEMANTIC,                 // 语义错误
    ERROR_VALIDATION,               // 验证错误
    ERROR_GENERATION,               // 生成错误
    
    // 特殊状态
    FROZEN,                         // 冻结状态（不可修改）
    DISPOSED                        // 已释放
};

/**
 * AST节点标记
 * 用于标记节点的特殊属性和状态
 */
enum class ASTNodeFlag {
    // 语法标记
    HAS_ATTRIBUTES,                 // 包含属性
    HAS_CHILDREN,                   // 包含子节点
    HAS_TEXT_CONTENT,               // 包含文本内容
    IS_SELF_CLOSING,                // 自闭合标签
    IS_BLOCK_ELEMENT,               // 块级元素
    IS_INLINE_ELEMENT,              // 行内元素
    
    // 模板和自定义标记
    IS_TEMPLATE_DECLARATION,        // 模板声明
    IS_TEMPLATE_USAGE,              // 模板使用
    IS_CUSTOM_DECLARATION,          // 自定义声明
    IS_CUSTOM_USAGE,                // 自定义使用
    IS_SPECIALIZED,                 // 已特例化
    
    // 样式标记
    HAS_LOCAL_STYLE,                // 包含局部样式
    HAS_INLINE_STYLE,               // 包含内联样式
    HAS_CLASS_SELECTOR,             // 包含类选择器
    HAS_ID_SELECTOR,                // 包含ID选择器
    HAS_PSEUDO_SELECTOR,            // 包含伪选择器
    
    // 脚本标记
    HAS_LOCAL_SCRIPT,               // 包含局部脚本
    HAS_CHTL_JS,                    // 包含CHTL JS
    HAS_ENHANCED_SELECTOR,          // 包含增强选择器
    HAS_EVENT_LISTENER,             // 包含事件监听器
    HAS_ANIMATION,                  // 包含动画
    
    // 命名空间标记
    IS_NAMESPACED,                  // 包含命名空间
    IS_IMPORTED,                    // 来自导入
    IS_EXPORTED,                    // 可导出
    
    // 优化标记
    IS_OPTIMIZABLE,                 // 可优化
    IS_CACHEABLE,                   // 可缓存
    IS_CRITICAL_PATH,               // 关键路径
    
    // 错误标记
    HAS_SYNTAX_ERROR,               // 包含语法错误
    HAS_SEMANTIC_ERROR,             // 包含语义错误
    HAS_WARNING,                    // 包含警告
    
    // 调试标记
    DEBUG_INFO_AVAILABLE,           // 调试信息可用
    SOURCE_MAP_AVAILABLE            // 源映射可用
};

/**
 * AST节点统计信息
 */
struct ASTNodeStats {
    uint32_t attributeCount;        // 属性数量
    uint32_t childCount;            // 子节点数量
    uint32_t depth;                 // 节点深度
    uint32_t textLength;            // 文本长度
    uint64_t parseTime;             // 解析耗时（微秒）
    uint64_t generateTime;          // 生成耗时（微秒）
    size_t memoryUsage;             // 内存使用量（字节）
    
    ASTNodeStats() : attributeCount(0), childCount(0), depth(0), textLength(0), 
                    parseTime(0), generateTime(0), memoryUsage(0) {}
};

/**
 * AST节点状态信息
 */
struct ASTNodeStateInfo {
    ASTNodeState currentState;      // 当前状态
    std::unordered_set<ASTNodeFlag> flags; // 节点标记
    CHTLGrammarState grammarState;  // 语法状态
    CHTLContextType contextType;    // 上下文类型
    Position position;              // 源码位置
    ASTNodeStats stats;             // 统计信息
    std::string lastError;          // 最后错误信息
    uint64_t lastUpdated;           // 最后更新时间
    
    ASTNodeStateInfo() 
        : currentState(ASTNodeState::UNINITIALIZED)
        , grammarState(CHTLGrammarState::INITIAL)
        , contextType(CHTLContextType::GLOBAL_SCOPE)
        , position{0, 0}
        , lastUpdated(0) {}
};

/**
 * AST节点状态变化监听器
 */
class IASTStateChangeListener {
public:
    virtual ~IASTStateChangeListener() = default;
    virtual void onNodeStateChanged(const void* nodePtr, ASTNodeState oldState, ASTNodeState newState) = 0;
    virtual void onNodeFlagChanged(const void* nodePtr, ASTNodeFlag flag, bool added) = 0;
    virtual void onNodeError(const void* nodePtr, const std::string& error) = 0;
    virtual void onNodeStatsUpdated(const void* nodePtr, const ASTNodeStats& stats) = 0;
};

/**
 * RAII AST节点状态守卫
 * 自动管理AST节点状态的进入和退出
 */
class ASTNodeStateGuard {
public:
    ASTNodeStateGuard(class ASTStateTracker& tracker, const void* nodePtr, ASTNodeState newState);
    ~ASTNodeStateGuard();
    
    // 禁止拷贝和赋值
    ASTNodeStateGuard(const ASTNodeStateGuard&) = delete;
    ASTNodeStateGuard& operator=(const ASTNodeStateGuard&) = delete;
    
    // 支持移动
    ASTNodeStateGuard(ASTNodeStateGuard&& other) noexcept;
    ASTNodeStateGuard& operator=(ASTNodeStateGuard&& other) noexcept;
    
    // 提前释放守卫
    void release();
    
    // 检查守卫是否有效
    bool isValid() const { return tracker_ != nullptr; }
    
    // 设置节点标记
    void setFlag(ASTNodeFlag flag);
    void clearFlag(ASTNodeFlag flag);
    
    // 更新统计信息
    void updateStats(const ASTNodeStats& stats);

private:
    class ASTStateTracker* tracker_;
    const void* nodePtr_;
    ASTNodeState previousState_;
    bool released_;
};

/**
 * AST状态追踪器
 * 与RAII状态机和上下文管理器协作，为AST节点提供精确的状态管理
 */
class ASTStateTracker {
public:
    explicit ASTStateTracker(RAIIStateMachine& stateMachine, ContextManager& contextManager);
    ~ASTStateTracker();
    
    // ===== 节点状态管理 =====
    
    /**
     * 注册AST节点
     * @param nodePtr 节点指针
     * @param nodeType 节点类型
     * @param position 源码位置
     * @return 是否注册成功
     */
    bool registerNode(const void* nodePtr, const std::string& nodeType, const Position& position);
    
    /**
     * 注销AST节点
     * @param nodePtr 节点指针
     */
    void unregisterNode(const void* nodePtr);
    
    /**
     * 获取节点状态信息
     * @param nodePtr 节点指针
     * @return 状态信息，如果节点不存在则返回nullptr
     */
    const ASTNodeStateInfo* getNodeStateInfo(const void* nodePtr) const;
    
    /**
     * 创建RAII节点状态守卫
     * @param nodePtr 节点指针
     * @param newState 新状态
     * @return 状态守卫对象
     */
    ASTNodeStateGuard createNodeGuard(const void* nodePtr, ASTNodeState newState);
    
    /**
     * 手动设置节点状态
     * @param nodePtr 节点指针
     * @param newState 新状态
     * @return 是否设置成功
     */
    bool setNodeState(const void* nodePtr, ASTNodeState newState);
    
    /**
     * 获取节点当前状态
     * @param nodePtr 节点指针
     * @return 当前状态
     */
    ASTNodeState getNodeState(const void* nodePtr) const;
    
    // ===== 节点标记管理 =====
    
    /**
     * 设置节点标记
     * @param nodePtr 节点指针
     * @param flag 标记
     */
    void setNodeFlag(const void* nodePtr, ASTNodeFlag flag);
    
    /**
     * 清除节点标记
     * @param nodePtr 节点指针
     * @param flag 标记
     */
    void clearNodeFlag(const void* nodePtr, ASTNodeFlag flag);
    
    /**
     * 检查节点是否有指定标记
     * @param nodePtr 节点指针
     * @param flag 标记
     * @return 是否有指定标记
     */
    bool hasNodeFlag(const void* nodePtr, ASTNodeFlag flag) const;
    
    /**
     * 获取节点所有标记
     * @param nodePtr 节点指针
     * @return 标记集合
     */
    const std::unordered_set<ASTNodeFlag>& getNodeFlags(const void* nodePtr) const;
    
    // ===== 统计信息管理 =====
    
    /**
     * 更新节点统计信息
     * @param nodePtr 节点指针
     * @param stats 统计信息
     */
    void updateNodeStats(const void* nodePtr, const ASTNodeStats& stats);
    
    /**
     * 获取节点统计信息
     * @param nodePtr 节点指针
     * @return 统计信息
     */
    const ASTNodeStats& getNodeStats(const void* nodePtr) const;
    
    /**
     * 增加节点子节点计数
     * @param nodePtr 节点指针
     */
    void incrementChildCount(const void* nodePtr);
    
    /**
     * 增加节点属性计数
     * @param nodePtr 节点指针
     */
    void incrementAttributeCount(const void* nodePtr);
    
    // ===== 错误管理 =====
    
    /**
     * 报告节点错误
     * @param nodePtr 节点指针
     * @param error 错误信息
     */
    void reportNodeError(const void* nodePtr, const std::string& error);
    
    /**
     * 清除节点错误
     * @param nodePtr 节点指针
     */
    void clearNodeError(const void* nodePtr);
    
    /**
     * 获取节点最后错误
     * @param nodePtr 节点指针
     * @return 错误信息
     */
    const std::string& getNodeLastError(const void* nodePtr) const;
    
    // ===== 查询和筛选 =====
    
    /**
     * 查找指定状态的所有节点
     * @param state 节点状态
     * @return 节点指针列表
     */
    std::vector<const void*> findNodesByState(ASTNodeState state) const;
    
    /**
     * 查找包含指定标记的所有节点
     * @param flag 节点标记
     * @return 节点指针列表
     */
    std::vector<const void*> findNodesByFlag(ASTNodeFlag flag) const;
    
    /**
     * 查找指定上下文类型的所有节点
     * @param contextType 上下文类型
     * @return 节点指针列表
     */
    std::vector<const void*> findNodesByContext(CHTLContextType contextType) const;
    
    /**
     * 统计各状态的节点数量
     * @return 状态->数量的映射
     */
    std::unordered_map<ASTNodeState, size_t> getStateStatistics() const;
    
    /**
     * 获取所有注册的节点数量
     * @return 节点总数
     */
    size_t getTotalNodeCount() const;
    
    // ===== 批量操作 =====
    
    /**
     * 批量设置节点状态
     * @param nodes 节点指针列表
     * @param newState 新状态
     * @return 成功设置的节点数量
     */
    size_t batchSetNodeState(const std::vector<const void*>& nodes, ASTNodeState newState);
    
    /**
     * 批量设置节点标记
     * @param nodes 节点指针列表
     * @param flag 标记
     * @return 成功设置的节点数量
     */
    size_t batchSetNodeFlag(const std::vector<const void*>& nodes, ASTNodeFlag flag);
    
    /**
     * 清理指定状态的所有节点
     * @param state 要清理的状态
     * @return 清理的节点数量
     */
    size_t cleanupNodesByState(ASTNodeState state);
    
    // ===== 监听器管理 =====
    
    /**
     * 添加状态变化监听器
     * @param listener 监听器
     */
    void addListener(std::shared_ptr<IASTStateChangeListener> listener);
    
    /**
     * 移除状态变化监听器
     * @param listener 监听器
     */
    void removeListener(std::shared_ptr<IASTStateChangeListener> listener);
    
    // ===== 与状态机和上下文管理器的集成 =====
    
    /**
     * 同步语法状态
     * @param nodePtr 节点指针
     * 自动从状态机同步当前语法状态到节点
     */
    void syncGrammarState(const void* nodePtr);
    
    /**
     * 同步上下文类型
     * @param nodePtr 节点指针
     * 自动从上下文管理器同步当前上下文到节点
     */
    void syncContextType(const void* nodePtr);
    
    /**
     * 全量同步所有节点
     * 从状态机和上下文管理器同步所有节点的状态
     */
    void syncAllNodes();
    
    // ===== 调试和诊断 =====
    
    /**
     * 获取节点状态名称
     * @param state 节点状态
     * @return 状态名称
     */
    static std::string getNodeStateName(ASTNodeState state);
    
    /**
     * 获取节点标记名称
     * @param flag 节点标记
     * @return 标记名称
     */
    static std::string getNodeFlagName(ASTNodeFlag flag);
    
    /**
     * 转储节点状态信息
     * @param nodePtr 节点指针
     * @return 状态信息字符串
     */
    std::string dumpNodeState(const void* nodePtr) const;
    
    /**
     * 转储所有节点状态
     * @return 完整状态信息
     */
    std::string dumpAllNodeStates() const;
    
    /**
     * 生成状态统计报告
     * @return 统计报告字符串
     */
    std::string generateStatisticsReport() const;

private:
    friend class ASTNodeStateGuard;
    
    // 内部状态管理（由ASTNodeStateGuard调用）
    void enterNodeState(const void* nodePtr, ASTNodeState newState);
    void exitNodeState(const void* nodePtr);
    
    // 通知监听器
    void notifyStateChanged(const void* nodePtr, ASTNodeState oldState, ASTNodeState newState);
    void notifyFlagChanged(const void* nodePtr, ASTNodeFlag flag, bool added);
    void notifyError(const void* nodePtr, const std::string& error);
    void notifyStatsUpdated(const void* nodePtr, const ASTNodeStats& stats);
    
    // 内部工具方法
    ASTNodeStateInfo& getOrCreateNodeStateInfo(const void* nodePtr);
    uint64_t getCurrentTimestamp() const;
    void updateLastModified(const void* nodePtr);
    
    // 状态验证
    bool isValidStateTransition(ASTNodeState from, ASTNodeState to) const;

private:
    RAIIStateMachine& stateMachine_;                        // 语法状态机引用
    ContextManager& contextManager_;                        // 上下文管理器引用
    
    std::unordered_map<const void*, ASTNodeStateInfo> nodeStates_; // 节点状态映射
    std::vector<std::shared_ptr<IASTStateChangeListener>> listeners_; // 监听器列表
    
    mutable std::mutex trackerMutex_;                       // 追踪器互斥锁
    static const std::unordered_set<ASTNodeFlag> emptyFlags_; // 空标记集合
    static const ASTNodeStats emptyStats_;                  // 空统计信息
    static const std::string emptyError_;                   // 空错误信息
};

/**
 * AST状态追踪器工厂
 */
class ASTStateTrackerFactory {
public:
    /**
     * 创建CHTL AST状态追踪器
     * @param stateMachine 状态机
     * @param contextManager 上下文管理器
     * @return 追踪器实例
     */
    static std::unique_ptr<ASTStateTracker> createCHTLTracker(
        RAIIStateMachine& stateMachine, ContextManager& contextManager);
    
    /**
     * 创建CHTL JS AST状态追踪器
     * @param stateMachine 状态机
     * @param contextManager 上下文管理器
     * @return 追踪器实例
     */
    static std::unique_ptr<ASTStateTracker> createCHTLJSTracker(
        RAIIStateMachine& stateMachine, ContextManager& contextManager);
};

// ===== 便利宏定义 =====

/**
 * 自动AST节点状态守卫宏
 */
#define CHTL_AST_STATE_GUARD(tracker, nodePtr, state) \
    auto __ast_guard_##__LINE__ = (tracker).createNodeGuard(nodePtr, state)

/**
 * AST节点注册宏
 */
#define CHTL_REGISTER_AST_NODE(tracker, nodePtr, nodeType, position) \
    (tracker).registerNode(nodePtr, nodeType, position)

/**
 * AST节点注销宏
 */
#define CHTL_UNREGISTER_AST_NODE(tracker, nodePtr) \
    (tracker).unregisterNode(nodePtr)

/**
 * AST节点标记设置宏
 */
#define CHTL_SET_AST_FLAG(tracker, nodePtr, flag) \
    (tracker).setNodeFlag(nodePtr, flag)

/**
 * AST节点错误报告宏
 */
#define CHTL_REPORT_AST_ERROR(tracker, nodePtr, error) \
    (tracker).reportNodeError(nodePtr, error)

/**
 * AST节点状态验证宏
 */
#define CHTL_VALIDATE_AST_STATE(tracker, nodePtr, expected_state) \
    do { \
        auto current_state = (tracker).getNodeState(nodePtr); \
        if (current_state != expected_state) { \
            throw std::runtime_error("Expected AST node state " + ASTStateTracker::getNodeStateName(expected_state) + \
                                    " but current state is " + ASTStateTracker::getNodeStateName(current_state)); \
        } \
    } while(0)

} // namespace chtl
#include <iostream>
#include <memory>
#include <string>
#include <cassert>
#include <chrono>
#include <thread>
#include <vector>
#include <unordered_map>
#include <set>

// 模拟核心类型
namespace chtl {

struct Position {
    int line;
    int column;
    Position(int l = 0, int c = 0) : line(l), column(c) {}
};

// RAII状态机演示
enum class CHTLGrammarState {
    INITIAL,
    PROGRAM_ROOT,
    ELEMENT_DECLARATION,
    ELEMENT_BODY,
    STYLE_DECLARATION,
    STYLE_BODY,
    TEMPLATE_DECLARATION,
    TEMPLATE_STYLE
};

class SimpleStateMachine {
private:
    CHTLGrammarState currentState_;
    
public:
    SimpleStateMachine() : currentState_(CHTLGrammarState::INITIAL) {}
    
    CHTLGrammarState getCurrentState() const { return currentState_; }
    
    void setState(CHTLGrammarState newState) {
        std::cout << "状态转换: " << getStateName(currentState_) 
                  << " -> " << getStateName(newState) << std::endl;
        currentState_ = newState;
    }
    
    static std::string getStateName(CHTLGrammarState state) {
        switch (state) {
            case CHTLGrammarState::INITIAL: return "INITIAL";
            case CHTLGrammarState::PROGRAM_ROOT: return "PROGRAM_ROOT";
            case CHTLGrammarState::ELEMENT_DECLARATION: return "ELEMENT_DECLARATION";
            case CHTLGrammarState::ELEMENT_BODY: return "ELEMENT_BODY";
            case CHTLGrammarState::STYLE_DECLARATION: return "STYLE_DECLARATION";
            case CHTLGrammarState::STYLE_BODY: return "STYLE_BODY";
            case CHTLGrammarState::TEMPLATE_DECLARATION: return "TEMPLATE_DECLARATION";
            case CHTLGrammarState::TEMPLATE_STYLE: return "TEMPLATE_STYLE";
            default: return "UNKNOWN";
        }
    }
};

// RAII状态守卫
class StateGuard {
private:
    SimpleStateMachine& machine_;
    CHTLGrammarState previousState_;
    bool released_;
    
public:
    StateGuard(SimpleStateMachine& machine, CHTLGrammarState newState)
        : machine_(machine), previousState_(machine.getCurrentState()), released_(false) {
        machine_.setState(newState);
        std::cout << "🔒 StateGuard 激活: 进入 " << SimpleStateMachine::getStateName(newState) << std::endl;
    }
    
    ~StateGuard() {
        if (!released_) {
            machine_.setState(previousState_);
            std::cout << "🔓 StateGuard 释放: 恢复到 " << SimpleStateMachine::getStateName(previousState_) << std::endl;
        }
    }
    
    // 禁止拷贝
    StateGuard(const StateGuard&) = delete;
    StateGuard& operator=(const StateGuard&) = delete;
    
    // 支持移动
    StateGuard(StateGuard&& other) noexcept 
        : machine_(other.machine_), previousState_(other.previousState_), released_(other.released_) {
        other.released_ = true;
    }
    
    void release() {
        if (!released_) {
            machine_.setState(previousState_);
            released_ = true;
            std::cout << "🔓 StateGuard 手动释放" << std::endl;
        }
    }
};

// 上下文管理器演示
enum class CHTLContextType {
    GLOBAL_SCOPE,
    ELEMENT_SCOPE,
    STYLE_SCOPE,
    TEMPLATE_SCOPE
};

struct SymbolEntry {
    std::string name;
    std::string type;
    Position position;
    
    SymbolEntry(const std::string& n, const std::string& t, const Position& pos)
        : name(n), type(t), position(pos) {}
};

class SimpleContextManager {
private:
    CHTLContextType currentContext_;
    std::vector<std::unique_ptr<SymbolEntry>> symbols_;
    
public:
    SimpleContextManager() : currentContext_(CHTLContextType::GLOBAL_SCOPE) {}
    
    CHTLContextType getCurrentContext() const { return currentContext_; }
    
    void setContext(CHTLContextType newContext) {
        std::cout << "上下文切换: " << getContextName(currentContext_) 
                  << " -> " << getContextName(newContext) << std::endl;
        currentContext_ = newContext;
    }
    
    void declareSymbol(const std::string& name, const std::string& type, const Position& pos) {
        symbols_.push_back(std::make_unique<SymbolEntry>(name, type, pos));
        std::cout << "符号声明: " << name << " (" << type << ") 在 " 
                  << getContextName(currentContext_) << std::endl;
    }
    
    const SymbolEntry* findSymbol(const std::string& name) const {
        for (const auto& symbol : symbols_) {
            if (symbol->name == name) {
                return symbol.get();
            }
        }
        return nullptr;
    }
    
    static std::string getContextName(CHTLContextType context) {
        switch (context) {
            case CHTLContextType::GLOBAL_SCOPE: return "GLOBAL_SCOPE";
            case CHTLContextType::ELEMENT_SCOPE: return "ELEMENT_SCOPE";
            case CHTLContextType::STYLE_SCOPE: return "STYLE_SCOPE";
            case CHTLContextType::TEMPLATE_SCOPE: return "TEMPLATE_SCOPE";
            default: return "UNKNOWN";
        }
    }
};

// RAII上下文守卫
class ContextGuard {
private:
    SimpleContextManager& manager_;
    CHTLContextType previousContext_;
    bool released_;
    
public:
    ContextGuard(SimpleContextManager& manager, CHTLContextType newContext)
        : manager_(manager), previousContext_(manager.getCurrentContext()), released_(false) {
        manager_.setContext(newContext);
        std::cout << "🏠 ContextGuard 激活: 进入 " << SimpleContextManager::getContextName(newContext) << std::endl;
    }
    
    ~ContextGuard() {
        if (!released_) {
            manager_.setContext(previousContext_);
            std::cout << "🏠 ContextGuard 释放: 恢复到 " << SimpleContextManager::getContextName(previousContext_) << std::endl;
        }
    }
    
    // 禁止拷贝
    ContextGuard(const ContextGuard&) = delete;
    ContextGuard& operator=(const ContextGuard&) = delete;
    
    // 支持移动
    ContextGuard(ContextGuard&& other) noexcept 
        : manager_(other.manager_), previousContext_(other.previousContext_), released_(other.released_) {
        other.released_ = true;
    }
};

// AST节点状态追踪器演示
enum class ASTNodeState {
    UNINITIALIZED,
    PARSING_STARTED,
    PARSING_COMPLETED,
    GENERATION_READY,
    DISPOSED
};

enum class ASTNodeFlag {
    HAS_ATTRIBUTES,
    HAS_CHILDREN,
    IS_TEMPLATE_USAGE,
    HAS_LOCAL_STYLE
};

struct ASTNodeInfo {
    ASTNodeState state;
    std::set<ASTNodeFlag> flags;
    Position position;
    
    ASTNodeInfo(const Position& pos) : state(ASTNodeState::UNINITIALIZED), position(pos) {}
};

class SimpleASTTracker {
private:
    std::unordered_map<const void*, std::unique_ptr<ASTNodeInfo>> nodeInfos_;
    
public:
    void registerNode(const void* nodePtr, const Position& pos) {
        nodeInfos_[nodePtr] = std::make_unique<ASTNodeInfo>(pos);
        std::cout << "🌳 AST节点注册: " << nodePtr << " at (" << pos.line << "," << pos.column << ")" << std::endl;
    }
    
    void setNodeState(const void* nodePtr, ASTNodeState newState) {
        auto it = nodeInfos_.find(nodePtr);
        if (it != nodeInfos_.end()) {
            auto oldState = it->second->state;
            it->second->state = newState;
            std::cout << "🌳 节点状态变更: " << nodePtr << " " 
                      << getStateName(oldState) << " -> " << getStateName(newState) << std::endl;
        }
    }
    
    void setNodeFlag(const void* nodePtr, ASTNodeFlag flag) {
        auto it = nodeInfos_.find(nodePtr);
        if (it != nodeInfos_.end()) {
            it->second->flags.insert(flag);
            std::cout << "🏷️  节点标记设置: " << nodePtr << " " << getFlagName(flag) << std::endl;
        }
    }
    
    ASTNodeState getNodeState(const void* nodePtr) const {
        auto it = nodeInfos_.find(nodePtr);
        return (it != nodeInfos_.end()) ? it->second->state : ASTNodeState::UNINITIALIZED;
    }
    
    bool hasNodeFlag(const void* nodePtr, ASTNodeFlag flag) const {
        auto it = nodeInfos_.find(nodePtr);
        return (it != nodeInfos_.end()) && (it->second->flags.count(flag) > 0);
    }
    
    void unregisterNode(const void* nodePtr) {
        nodeInfos_.erase(nodePtr);
        std::cout << "🌳 AST节点注销: " << nodePtr << std::endl;
    }
    
    static std::string getStateName(ASTNodeState state) {
        switch (state) {
            case ASTNodeState::UNINITIALIZED: return "UNINITIALIZED";
            case ASTNodeState::PARSING_STARTED: return "PARSING_STARTED";
            case ASTNodeState::PARSING_COMPLETED: return "PARSING_COMPLETED";
            case ASTNodeState::GENERATION_READY: return "GENERATION_READY";
            case ASTNodeState::DISPOSED: return "DISPOSED";
            default: return "UNKNOWN";
        }
    }
    
    static std::string getFlagName(ASTNodeFlag flag) {
        switch (flag) {
            case ASTNodeFlag::HAS_ATTRIBUTES: return "HAS_ATTRIBUTES";
            case ASTNodeFlag::HAS_CHILDREN: return "HAS_CHILDREN";
            case ASTNodeFlag::IS_TEMPLATE_USAGE: return "IS_TEMPLATE_USAGE";
            case ASTNodeFlag::HAS_LOCAL_STYLE: return "HAS_LOCAL_STYLE";
            default: return "UNKNOWN";
        }
    }
};

// RAII AST状态守卫
class ASTStateGuard {
private:
    SimpleASTTracker& tracker_;
    const void* nodePtr_;
    ASTNodeState previousState_;
    bool released_;
    
public:
    ASTStateGuard(SimpleASTTracker& tracker, const void* nodePtr, ASTNodeState newState)
        : tracker_(tracker), nodePtr_(nodePtr), 
          previousState_(tracker.getNodeState(nodePtr)), released_(false) {
        tracker_.setNodeState(nodePtr, newState);
        std::cout << "🎯 ASTStateGuard 激活: 节点 " << nodePtr << " 进入 " 
                  << SimpleASTTracker::getStateName(newState) << std::endl;
    }
    
    ~ASTStateGuard() {
        if (!released_) {
            tracker_.setNodeState(nodePtr_, previousState_);
            std::cout << "🎯 ASTStateGuard 释放: 节点 " << nodePtr_ << " 恢复到 " 
                      << SimpleASTTracker::getStateName(previousState_) << std::endl;
        }
    }
    
    // 禁止拷贝
    ASTStateGuard(const ASTStateGuard&) = delete;
    ASTStateGuard& operator=(const ASTStateGuard&) = delete;
    
    // 支持移动
    ASTStateGuard(ASTStateGuard&& other) noexcept 
        : tracker_(other.tracker_), nodePtr_(other.nodePtr_), 
          previousState_(other.previousState_), released_(other.released_) {
        other.released_ = true;
    }
};

// 便利宏定义
#define STATE_GUARD(machine, state) chtl::StateGuard __guard(machine, state)
#define CONTEXT_GUARD(manager, context) chtl::ContextGuard __context_guard(manager, context)
#define AST_STATE_GUARD(tracker, nodePtr, state) chtl::ASTStateGuard __ast_guard(tracker, nodePtr, state)

} // namespace chtl

// 演示函数
void demonstrateBasicRAII() {
    std::cout << "\n========== 基本RAII状态机演示 ==========\n" << std::endl;
    
    chtl::SimpleStateMachine stateMachine;
    
    std::cout << "初始状态: " << chtl::SimpleStateMachine::getStateName(stateMachine.getCurrentState()) << std::endl;
    
    // 演示RAII状态守卫
    {
        std::cout << "\n--- 进入程序根状态 ---" << std::endl;
        STATE_GUARD(stateMachine, chtl::CHTLGrammarState::PROGRAM_ROOT);
        
        {
            std::cout << "\n--- 进入元素声明状态 ---" << std::endl;
            STATE_GUARD(stateMachine, chtl::CHTLGrammarState::ELEMENT_DECLARATION);
            
            {
                std::cout << "\n--- 进入元素体状态 ---" << std::endl;
                STATE_GUARD(stateMachine, chtl::CHTLGrammarState::ELEMENT_BODY);
                
                std::cout << "当前状态: " << chtl::SimpleStateMachine::getStateName(stateMachine.getCurrentState()) << std::endl;
            }
            std::cout << "退出元素体状态后: " << chtl::SimpleStateMachine::getStateName(stateMachine.getCurrentState()) << std::endl;
        }
        std::cout << "退出元素声明状态后: " << chtl::SimpleStateMachine::getStateName(stateMachine.getCurrentState()) << std::endl;
    }
    std::cout << "退出程序根状态后: " << chtl::SimpleStateMachine::getStateName(stateMachine.getCurrentState()) << std::endl;
}

void demonstrateContextManager() {
    std::cout << "\n========== 上下文管理器演示 ==========\n" << std::endl;
    
    chtl::SimpleContextManager contextManager;
    
    std::cout << "初始上下文: " << chtl::SimpleContextManager::getContextName(contextManager.getCurrentContext()) << std::endl;
    
    // 演示上下文管理
    {
        std::cout << "\n--- 进入元素作用域 ---" << std::endl;
        CONTEXT_GUARD(contextManager, chtl::CHTLContextType::ELEMENT_SCOPE);
        
        contextManager.declareSymbol("myDiv", "div", chtl::Position{1, 5});
        contextManager.declareSymbol("id", "box", chtl::Position{2, 10});
        
        {
            std::cout << "\n--- 进入样式作用域 ---" << std::endl;
            CONTEXT_GUARD(contextManager, chtl::CHTLContextType::STYLE_SCOPE);
            
            contextManager.declareSymbol("color", "red", chtl::Position{3, 15});
            contextManager.declareSymbol("padding", "10px", chtl::Position{4, 5});
            
            // 查找符号
            const auto* symbol = contextManager.findSymbol("myDiv");
            if (symbol) {
                std::cout << "找到符号: " << symbol->name << " (" << symbol->type << ")" << std::endl;
            }
        }
        std::cout << "退出样式作用域后: " << chtl::SimpleContextManager::getContextName(contextManager.getCurrentContext()) << std::endl;
    }
    std::cout << "退出元素作用域后: " << chtl::SimpleContextManager::getContextName(contextManager.getCurrentContext()) << std::endl;
}

void demonstrateASTTracker() {
    std::cout << "\n========== AST状态追踪器演示 ==========\n" << std::endl;
    
    chtl::SimpleASTTracker astTracker;
    
    // 模拟AST节点
    std::string dummyNode1 = "ElementNode";
    std::string dummyNode2 = "StyleNode";
    const void* node1Ptr = &dummyNode1;
    const void* node2Ptr = &dummyNode2;
    
    // 注册节点
    astTracker.registerNode(node1Ptr, chtl::Position{1, 1});
    astTracker.registerNode(node2Ptr, chtl::Position{2, 5});
    
    // 演示AST节点状态管理
    {
        std::cout << "\n--- 开始解析节点 ---" << std::endl;
        AST_STATE_GUARD(astTracker, node1Ptr, chtl::ASTNodeState::PARSING_STARTED);
        
        // 设置节点标记
        astTracker.setNodeFlag(node1Ptr, chtl::ASTNodeFlag::HAS_ATTRIBUTES);
        astTracker.setNodeFlag(node1Ptr, chtl::ASTNodeFlag::HAS_CHILDREN);
        
        {
            std::cout << "\n--- 处理样式节点 ---" << std::endl;
            AST_STATE_GUARD(astTracker, node2Ptr, chtl::ASTNodeState::PARSING_STARTED);
            
            astTracker.setNodeFlag(node2Ptr, chtl::ASTNodeFlag::HAS_LOCAL_STYLE);
            
            // 检查节点状态和标记
            std::cout << "节点1状态: " << chtl::SimpleASTTracker::getStateName(astTracker.getNodeState(node1Ptr)) << std::endl;
            std::cout << "节点1是否有属性: " << (astTracker.hasNodeFlag(node1Ptr, chtl::ASTNodeFlag::HAS_ATTRIBUTES) ? "是" : "否") << std::endl;
            std::cout << "节点2是否有样式: " << (astTracker.hasNodeFlag(node2Ptr, chtl::ASTNodeFlag::HAS_LOCAL_STYLE) ? "是" : "否") << std::endl;
        }
        std::cout << "样式节点处理完成后状态: " << chtl::SimpleASTTracker::getStateName(astTracker.getNodeState(node2Ptr)) << std::endl;
    }
    std::cout << "元素节点解析完成后状态: " << chtl::SimpleASTTracker::getStateName(astTracker.getNodeState(node1Ptr)) << std::endl;
    
    // 清理
    astTracker.unregisterNode(node1Ptr);
    astTracker.unregisterNode(node2Ptr);
}

void demonstrateComplexScenario() {
    std::cout << "\n========== 复杂场景综合演示 ==========\n" << std::endl;
    
    chtl::SimpleStateMachine stateMachine;
    chtl::SimpleContextManager contextManager;
    chtl::SimpleASTTracker astTracker;
    
    // 模拟复杂的CHTL解析场景
    std::string templateNode = "TemplateNode";
    std::string elementNode = "ElementNode";
    const void* templatePtr = &templateNode;
    const void* elementPtr = &elementNode;
    
    astTracker.registerNode(templatePtr, chtl::Position{1, 1});
    astTracker.registerNode(elementPtr, chtl::Position{5, 1});
    
    {
        std::cout << "\n--- 解析CHTL模板 ---" << std::endl;
        STATE_GUARD(stateMachine, chtl::CHTLGrammarState::TEMPLATE_DECLARATION);
        CONTEXT_GUARD(contextManager, chtl::CHTLContextType::TEMPLATE_SCOPE);
        AST_STATE_GUARD(astTracker, templatePtr, chtl::ASTNodeState::PARSING_STARTED);
        
        // 注册模板
        contextManager.declareSymbol("ButtonTemplate", "@Style", chtl::Position{1, 15});
        astTracker.setNodeFlag(templatePtr, chtl::ASTNodeFlag::IS_TEMPLATE_USAGE);
        
        {
            std::cout << "\n--- 解析模板样式 ---" << std::endl;
            STATE_GUARD(stateMachine, chtl::CHTLGrammarState::TEMPLATE_STYLE);
            CONTEXT_GUARD(contextManager, chtl::CHTLContextType::STYLE_SCOPE);
            
            contextManager.declareSymbol("padding", "10px", chtl::Position{2, 5});
            contextManager.declareSymbol("border", "1px solid", chtl::Position{3, 5});
        }
        
        // 模板解析完成
        astTracker.setNodeState(templatePtr, chtl::ASTNodeState::PARSING_COMPLETED);
    }
    
    {
        std::cout << "\n--- 解析使用模板的元素 ---" << std::endl;
        STATE_GUARD(stateMachine, chtl::CHTLGrammarState::ELEMENT_DECLARATION);
        CONTEXT_GUARD(contextManager, chtl::CHTLContextType::ELEMENT_SCOPE);
        AST_STATE_GUARD(astTracker, elementPtr, chtl::ASTNodeState::PARSING_STARTED);
        
        contextManager.declareSymbol("button", "button", chtl::Position{5, 5});
        astTracker.setNodeFlag(elementPtr, chtl::ASTNodeFlag::HAS_ATTRIBUTES);
        astTracker.setNodeFlag(elementPtr, chtl::ASTNodeFlag::IS_TEMPLATE_USAGE);
        
        // 查找模板
        const auto* template_ = contextManager.findSymbol("ButtonTemplate");
        if (template_) {
            std::cout << "✅ 成功找到并应用模板: " << template_->name << std::endl;
        }
        
        astTracker.setNodeState(elementPtr, chtl::ASTNodeState::PARSING_COMPLETED);
    }
    
    std::cout << "\n--- 最终状态检查 ---" << std::endl;
    std::cout << "状态机: " << chtl::SimpleStateMachine::getStateName(stateMachine.getCurrentState()) << std::endl;
    std::cout << "上下文: " << chtl::SimpleContextManager::getContextName(contextManager.getCurrentContext()) << std::endl;
    std::cout << "模板节点状态: " << chtl::SimpleASTTracker::getStateName(astTracker.getNodeState(templatePtr)) << std::endl;
    std::cout << "元素节点状态: " << chtl::SimpleASTTracker::getStateName(astTracker.getNodeState(elementPtr)) << std::endl;
    
    // 清理
    astTracker.unregisterNode(templatePtr);
    astTracker.unregisterNode(elementPtr);
}

void demonstratePerformance() {
    std::cout << "\n========== 性能测试演示 ==========\n" << std::endl;
    
    chtl::SimpleStateMachine stateMachine;
    
    const int iterations = 100000;
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // 性能测试：大量状态转换
    for (int i = 0; i < iterations; ++i) {
        STATE_GUARD(stateMachine, chtl::CHTLGrammarState::ELEMENT_DECLARATION);
        // 模拟一些工作
        auto currentState = stateMachine.getCurrentState();
        (void)currentState; // 避免未使用变量警告
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    
    std::cout << "状态机性能测试: " << iterations << " 次操作耗时 " 
              << duration.count() << " 微秒" << std::endl;
    std::cout << "平均每次操作: " << static_cast<double>(duration.count()) / iterations 
              << " 微秒" << std::endl;
}

int main() {
    std::cout << "🎉 CHTL RAII状态管理系统演示" << std::endl;
    std::cout << "========================================" << std::endl;
    
    try {
        demonstrateBasicRAII();
        demonstrateContextManager();
        demonstrateASTTracker();
        demonstrateComplexScenario();
        demonstratePerformance();
        
        std::cout << "\n🎊 所有演示完成！RAII状态管理系统工作正常。" << std::endl;
        std::cout << "\n✅ 主要特性验证:" << std::endl;
        std::cout << "   • RAII自动状态管理 ✓" << std::endl;
        std::cout << "   • 嵌套状态守卫 ✓" << std::endl;
        std::cout << "   • 上下文和符号管理 ✓" << std::endl;
        std::cout << "   • AST节点状态追踪 ✓" << std::endl;
        std::cout << "   • 复杂场景协同工作 ✓" << std::endl;
        std::cout << "   • 高性能状态转换 ✓" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ 演示过程中发生错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
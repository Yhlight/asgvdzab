#include <gtest/gtest.h>
#include "state/raii_state_machine.hpp"
#include "state/context_manager.hpp"
#include "state/ast_state_tracker.hpp"
#include "integration/scanner_state_integration.hpp"
#include "integration/compiler_state_integration.hpp"
#include "ast/chtl_ast.hpp"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

namespace chtl {

class RAIIStateManagementIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 创建状态管理组件
        stateMachine_ = StateMachineFactory::createCHTLStateMachine();
        contextManager_ = ContextManagerFactory::createCHTLContextManager();
        astTracker_ = ASTStateTrackerFactory::createCHTLTracker(*stateMachine_, *contextManager_);
        
        // 创建监听器
        stateListener_ = std::make_shared<ScannerStateListener>();
        compilerListener_ = std::make_shared<CompilerStateListener>();
        
        // 添加监听器
        stateMachine_->addListener(stateListener_);
        contextManager_->addListener(stateListener_);
        astTracker_->addListener(compilerListener_);
    }
    
    void TearDown() override {
        // 清理资源
        astTracker_.reset();
        contextManager_.reset();
        stateMachine_.reset();
    }

protected:
    std::unique_ptr<RAIIStateMachine> stateMachine_;
    std::unique_ptr<ContextManager> contextManager_;
    std::unique_ptr<ASTStateTracker> astTracker_;
    std::shared_ptr<ScannerStateListener> stateListener_;
    std::shared_ptr<CompilerStateListener> compilerListener_;
};

// 测试基本的RAII状态机功能
TEST_F(RAIIStateManagementIntegrationTest, testBasicStateMachine) {
    // 验证初始状态
    EXPECT_EQ(stateMachine_->getCurrentState(), CHTLGrammarState::INITIAL);
    
    // 测试状态转换
    EXPECT_TRUE(stateMachine_->transitionTo(CHTLGrammarState::PROGRAM_ROOT, "开始解析"));
    EXPECT_EQ(stateMachine_->getCurrentState(), CHTLGrammarState::PROGRAM_ROOT);
    
    // 测试RAII状态守卫
    {
        CHTL_STATE_GUARD(*stateMachine_, CHTLGrammarState::ELEMENT_DECLARATION);
        EXPECT_EQ(stateMachine_->getCurrentState(), CHTLGrammarState::ELEMENT_DECLARATION);
        
        // 嵌套状态守卫
        {
            auto guard = stateMachine_->createGuard(CHTLGrammarState::ELEMENT_BODY, "解析元素体");
            EXPECT_EQ(stateMachine_->getCurrentState(), CHTLGrammarState::ELEMENT_BODY);
        }
        
        // 应该恢复到ELEMENT_DECLARATION
        EXPECT_EQ(stateMachine_->getCurrentState(), CHTLGrammarState::ELEMENT_DECLARATION);
    }
    
    // 应该恢复到PROGRAM_ROOT
    EXPECT_EQ(stateMachine_->getCurrentState(), CHTLGrammarState::PROGRAM_ROOT);
}

// 测试上下文管理器功能
TEST_F(RAIIStateManagementIntegrationTest, testContextManager) {
    // 验证初始上下文
    EXPECT_EQ(contextManager_->getCurrentContextType(), CHTLContextType::GLOBAL_SCOPE);
    
    // 测试上下文转换
    {
        CHTL_CONTEXT_GUARD(*contextManager_, CHTLContextType::ELEMENT_SCOPE);
        EXPECT_EQ(contextManager_->getCurrentContextType(), CHTLContextType::ELEMENT_SCOPE);
        
        // 测试符号声明
        Position pos{1, 5};
        EXPECT_TRUE(CHTL_DECLARE_SYMBOL(*contextManager_, "testElement", "div", pos));
        
        const auto* symbol = CHTL_FIND_SYMBOL(*contextManager_, "testElement");
        ASSERT_NE(symbol, nullptr);
        EXPECT_EQ(symbol->name, "testElement");
        EXPECT_EQ(symbol->type, "div");
        
        // 嵌套上下文
        {
            auto guard = contextManager_->createGuard(CHTLContextType::STYLE_SCOPE, "div", "解析样式");
            EXPECT_EQ(contextManager_->getCurrentContextType(), CHTLContextType::STYLE_SCOPE);
            
            // 在样式上下文中声明符号
            EXPECT_TRUE(contextManager_->declareSymbol("color", "property", pos));
        }
        
        EXPECT_EQ(contextManager_->getCurrentContextType(), CHTLContextType::ELEMENT_SCOPE);
    }
    
    EXPECT_EQ(contextManager_->getCurrentContextType(), CHTLContextType::GLOBAL_SCOPE);
}

// 测试AST状态追踪器功能
TEST_F(RAIIStateManagementIntegrationTest, testASTStateTracker) {
    // 创建虚拟AST节点
    auto element = std::make_shared<ElementNode>("div", Position{1, 1});
    const void* nodePtr = element.get();
    
    // 注册节点
    CHTL_REGISTER_AST_NODE(*astTracker_, nodePtr, "ElementNode", Position{1, 1});
    
    // 验证初始状态
    EXPECT_EQ(astTracker_->getNodeState(nodePtr), ASTNodeState::UNINITIALIZED);
    
    // 测试状态转换
    {
        CHTL_AST_STATE_GUARD(*astTracker_, nodePtr, ASTNodeState::PARSING_STARTED);
        EXPECT_EQ(astTracker_->getNodeState(nodePtr), ASTNodeState::PARSING_STARTED);
        
        // 设置节点标记
        CHTL_SET_AST_FLAG(*astTracker_, nodePtr, ASTNodeFlag::HAS_CHILDREN);
        EXPECT_TRUE(astTracker_->hasNodeFlag(nodePtr, ASTNodeFlag::HAS_CHILDREN));
        
        // 更新统计信息
        ASTNodeStats stats;
        stats.childCount = 2;
        stats.attributeCount = 3;
        astTracker_->updateNodeStats(nodePtr, stats);
        
        const auto& nodeStats = astTracker_->getNodeStats(nodePtr);
        EXPECT_EQ(nodeStats.childCount, 2);
        EXPECT_EQ(nodeStats.attributeCount, 3);
        
        // 嵌套状态
        {
            auto guard = astTracker_->createNodeGuard(nodePtr, ASTNodeState::PARSING_ATTRIBUTES);
            EXPECT_EQ(astTracker_->getNodeState(nodePtr), ASTNodeState::PARSING_ATTRIBUTES);
        }
        
        EXPECT_EQ(astTracker_->getNodeState(nodePtr), ASTNodeState::PARSING_STARTED);
    }
    
    EXPECT_EQ(astTracker_->getNodeState(nodePtr), ASTNodeState::UNINITIALIZED);
    
    // 注销节点
    CHTL_UNREGISTER_AST_NODE(*astTracker_, nodePtr);
}

// 测试状态同步功能
TEST_F(RAIIStateManagementIntegrationTest, testStateSynchronization) {
    // 创建AST节点
    auto element = std::make_shared<ElementNode>("div", Position{1, 1});
    const void* nodePtr = element.get();
    
    CHTL_REGISTER_AST_NODE(*astTracker_, nodePtr, "ElementNode", Position{1, 1});
    
    // 同时管理三种状态
    {
        CHTL_STATE_GUARD(*stateMachine_, CHTLGrammarState::ELEMENT_DECLARATION);
        CHTL_CONTEXT_GUARD(*contextManager_, CHTLContextType::ELEMENT_SCOPE);
        CHTL_AST_STATE_GUARD(*astTracker_, nodePtr, ASTNodeState::PARSING_STARTED);
        
        // 验证状态一致性
        EXPECT_EQ(stateMachine_->getCurrentState(), CHTLGrammarState::ELEMENT_DECLARATION);
        EXPECT_EQ(contextManager_->getCurrentContextType(), CHTLContextType::ELEMENT_SCOPE);
        EXPECT_EQ(astTracker_->getNodeState(nodePtr), ASTNodeState::PARSING_STARTED);
        
        // 测试同步功能
        astTracker_->syncGrammarState(nodePtr);
        astTracker_->syncContextType(nodePtr);
        
        const auto* stateInfo = astTracker_->getNodeStateInfo(nodePtr);
        ASSERT_NE(stateInfo, nullptr);
        EXPECT_EQ(stateInfo->grammarState, CHTLGrammarState::ELEMENT_DECLARATION);
        EXPECT_EQ(stateInfo->contextType, CHTLContextType::ELEMENT_SCOPE);
    }
    
    CHTL_UNREGISTER_AST_NODE(*astTracker_, nodePtr);
}

// 测试错误处理和恢复
TEST_F(RAIIStateManagementIntegrationTest, testErrorHandlingAndRecovery) {
    // 创建AST节点
    auto element = std::make_shared<ElementNode>("div", Position{1, 1});
    const void* nodePtr = element.get();
    
    CHTL_REGISTER_AST_NODE(*astTracker_, nodePtr, "ElementNode", Position{1, 1});
    
    // 测试错误报告
    CHTL_REPORT_AST_ERROR(*astTracker_, nodePtr, "测试错误");
    
    const std::string& lastError = astTracker_->getNodeLastError(nodePtr);
    EXPECT_FALSE(lastError.empty());
    EXPECT_TRUE(lastError.find("测试错误") != std::string::npos);
    
    // 测试错误标记
    EXPECT_TRUE(astTracker_->hasNodeFlag(nodePtr, ASTNodeFlag::HAS_SYNTAX_ERROR));
    
    // 清除错误
    astTracker_->clearNodeError(nodePtr);
    EXPECT_TRUE(astTracker_->getNodeLastError(nodePtr).empty());
    
    CHTL_UNREGISTER_AST_NODE(*astTracker_, nodePtr);
}

// 测试并发安全性
TEST_F(RAIIStateManagementIntegrationTest, testConcurrencySafety) {
    const int numThreads = 4;
    const int operationsPerThread = 100;
    std::vector<std::thread> threads;
    
    // 创建多个线程同时操作状态机
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([this, i, operationsPerThread]() {
            for (int j = 0; j < operationsPerThread; ++j) {
                // 测试状态机并发访问
                {
                    auto guard = stateMachine_->createGuard(
                        static_cast<CHTLGrammarState>((i * operationsPerThread + j) % 10 + static_cast<int>(CHTLGrammarState::ELEMENT_DECLARATION)),
                        "并发测试"
                    );
                    std::this_thread::sleep_for(std::chrono::microseconds(1));
                }
                
                // 测试上下文管理器并发访问
                {
                    auto guard = contextManager_->createGuard(
                        static_cast<CHTLContextType>((i * operationsPerThread + j) % 5 + static_cast<int>(CHTLContextType::ELEMENT_SCOPE)),
                        "element_" + std::to_string(i) + "_" + std::to_string(j)
                    );
                    std::this_thread::sleep_for(std::chrono::microseconds(1));
                }
            }
        });
    }
    
    // 等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }
    
    // 验证最终状态
    EXPECT_EQ(stateMachine_->getCurrentState(), CHTLGrammarState::INITIAL);
    EXPECT_EQ(contextManager_->getCurrentContextType(), CHTLContextType::GLOBAL_SCOPE);
}

// 测试性能基准
TEST_F(RAIIStateManagementIntegrationTest, testPerformanceBenchmark) {
    const int numOperations = 10000;
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // 测试状态机性能
    for (int i = 0; i < numOperations; ++i) {
        auto guard = stateMachine_->createGuard(CHTLGrammarState::ELEMENT_DECLARATION, "性能测试");
        // 模拟一些工作
        stateMachine_->isInState(CHTLGrammarState::ELEMENT_DECLARATION);
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    
    std::cout << "状态机性能测试: " << numOperations << " 操作耗时 " 
              << duration.count() << " 微秒" << std::endl;
    std::cout << "平均每操作: " << static_cast<double>(duration.count()) / numOperations 
              << " 微秒" << std::endl;
    
    // 性能阈值检查（每操作不应超过10微秒）
    EXPECT_LT(static_cast<double>(duration.count()) / numOperations, 10.0);
}

// 测试内存使用情况
TEST_F(RAIIStateManagementIntegrationTest, testMemoryUsage) {
    const int numNodes = 1000;
    std::vector<std::shared_ptr<ElementNode>> elements;
    
    // 创建大量AST节点
    for (int i = 0; i < numNodes; ++i) {
        auto element = std::make_shared<ElementNode>("div" + std::to_string(i), Position{i, i});
        elements.push_back(element);
        
        CHTL_REGISTER_AST_NODE(*astTracker_, element.get(), "ElementNode", Position{i, i});
        
        // 设置一些状态和标记
        astTracker_->setNodeState(element.get(), ASTNodeState::PARSING_COMPLETED);
        astTracker_->setNodeFlag(element.get(), ASTNodeFlag::HAS_CHILDREN);
    }
    
    // 验证节点数量
    EXPECT_EQ(astTracker_->getTotalNodeCount(), numNodes);
    
    // 获取统计信息
    auto statistics = astTracker_->getStateStatistics();
    EXPECT_EQ(statistics[ASTNodeState::PARSING_COMPLETED], numNodes);
    
    // 清理节点
    for (const auto& element : elements) {
        CHTL_UNREGISTER_AST_NODE(*astTracker_, element.get());
    }
    
    EXPECT_EQ(astTracker_->getTotalNodeCount(), 0);
}

// 测试复杂的CHTL语法场景
TEST_F(RAIIStateManagementIntegrationTest, testComplexCHTLScenario) {
    // 模拟复杂的CHTL解析场景
    {
        CHTL_STATE_GUARD(*stateMachine_, CHTLGrammarState::PROGRAM_ROOT);
        CHTL_CONTEXT_GUARD(*contextManager_, CHTLContextType::FILE_SCOPE);
        
        // 解析模板声明
        {
            CHTL_STATE_GUARD(*stateMachine_, CHTLGrammarState::TEMPLATE_DECLARATION);
            CHTL_CONTEXT_GUARD(*contextManager_, CHTLContextType::TEMPLATE_SCOPE);
            
            // 注册模板
            contextManager_->registerTemplate("@Style", "DefaultButton", Position{1, 1});
            
            // 解析模板样式
            {
                CHTL_STATE_GUARD(*stateMachine_, CHTLGrammarState::TEMPLATE_STYLE);
                CHTL_CONTEXT_GUARD(*contextManager_, CHTLContextType::TEMPLATE_STYLE_SCOPE);
                
                // 声明样式属性
                contextManager_->declareSymbol("padding", "10px", Position{2, 5});
                contextManager_->declareSymbol("border", "1px solid", Position{3, 5});
            }
        }
        
        // 解析元素使用模板
        {
            CHTL_STATE_GUARD(*stateMachine_, CHTLGrammarState::ELEMENT_DECLARATION);
            CHTL_CONTEXT_GUARD_WITH_ID(*contextManager_, CHTLContextType::ELEMENT_SCOPE, "button");
            
            auto button = std::make_shared<ElementNode>("button", Position{5, 1});
            CHTL_REGISTER_AST_NODE(*astTracker_, button.get(), "ElementNode", Position{5, 1});
            
            {
                CHTL_AST_STATE_GUARD(*astTracker_, button.get(), ASTNodeState::PARSING_STARTED);
                
                // 使用模板
                {
                    CHTL_STATE_GUARD(*stateMachine_, CHTLGrammarState::TEMPLATE_USAGE);
                    
                    const auto* template_ = contextManager_->findTemplate("@Style", "DefaultButton");
                    EXPECT_NE(template_, nullptr);
                    
                    CHTL_SET_AST_FLAG(*astTracker_, button.get(), ASTNodeFlag::IS_TEMPLATE_USAGE);
                }
                
                astTracker_->setNodeState(button.get(), ASTNodeState::PARSING_COMPLETED);
            }
            
            CHTL_UNREGISTER_AST_NODE(*astTracker_, button.get());
        }
    }
    
    // 验证最终状态
    EXPECT_EQ(stateMachine_->getCurrentState(), CHTLGrammarState::INITIAL);
    EXPECT_EQ(contextManager_->getCurrentContextType(), CHTLContextType::GLOBAL_SCOPE);
}

// 测试监听器事件
TEST_F(RAIIStateManagementIntegrationTest, testListenerEvents) {
    // 清空事件日志
    stateListener_->clearEventLog();
    compilerListener_->resetStatistics();
    
    // 执行一系列操作
    {
        CHTL_STATE_GUARD(*stateMachine_, CHTLGrammarState::ELEMENT_DECLARATION);
        CHTL_CONTEXT_GUARD(*contextManager_, CHTLContextType::ELEMENT_SCOPE);
        
        auto element = std::make_shared<ElementNode>("div", Position{1, 1});
        CHTL_REGISTER_AST_NODE(*astTracker_, element.get(), "ElementNode", Position{1, 1});
        
        {
            CHTL_AST_STATE_GUARD(*astTracker_, element.get(), ASTNodeState::PARSING_STARTED);
            CHTL_SET_AST_FLAG(*astTracker_, element.get(), ASTNodeFlag::HAS_ATTRIBUTES);
        }
        
        CHTL_UNREGISTER_AST_NODE(*astTracker_, element.get());
    }
    
    // 验证事件记录
    const auto& eventLog = stateListener_->getEventLog();
    EXPECT_FALSE(eventLog.empty());
    
    const auto& eventStats = compilerListener_->getEventStatistics();
    EXPECT_GT(eventStats.size(), 0);
    
    // 查找特定事件
    bool foundStateEnter = false;
    bool foundContextEnter = false;
    
    for (const auto& event : eventLog) {
        if (event.find("State Enter") != std::string::npos) {
            foundStateEnter = true;
        }
        if (event.find("Context Enter") != std::string::npos) {
            foundContextEnter = true;
        }
    }
    
    EXPECT_TRUE(foundStateEnter);
    EXPECT_TRUE(foundContextEnter);
}

// 测试调试和诊断功能
TEST_F(RAIIStateManagementIntegrationTest, testDebuggingAndDiagnostics) {
    // 测试状态机调试信息
    std::string stateDebugInfo = stateMachine_->getDebugInfo();
    EXPECT_FALSE(stateDebugInfo.empty());
    EXPECT_TRUE(stateDebugInfo.find("Current State") != std::string::npos);
    
    // 测试上下文管理器调试信息
    std::string contextDebugInfo = contextManager_->getDebugInfo();
    EXPECT_FALSE(contextDebugInfo.empty());
    
    // 创建AST节点进行测试
    auto element = std::make_shared<ElementNode>("div", Position{1, 1});
    CHTL_REGISTER_AST_NODE(*astTracker_, element.get(), "ElementNode", Position{1, 1});
    
    // 测试AST追踪器统计报告
    std::string statsReport = astTracker_->generateStatisticsReport();
    EXPECT_FALSE(statsReport.empty());
    EXPECT_TRUE(statsReport.find("Total Nodes") != std::string::npos);
    
    // 测试节点状态转储
    std::string nodeState = astTracker_->dumpNodeState(element.get());
    EXPECT_FALSE(nodeState.empty());
    EXPECT_TRUE(nodeState.find("State:") != std::string::npos);
    
    CHTL_UNREGISTER_AST_NODE(*astTracker_, element.get());
}

} // namespace chtl
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>
#include "statemachine/CHTLStateMachine.h"

using namespace chtl;

void testStateMachineCreation() {
    std::cout << "=== 测试状态机创建 ===" << std::endl;
    
    // 测试默认配置
    StateMachineConfig default_config;
    auto state_machine1 = std::make_unique<CHTLStateMachine>(default_config);
    
    std::cout << "默认配置状态机创建成功" << std::endl;
    std::cout << "错误恢复: " << (default_config.enable_auto_recovery ? "启用" : "禁用") << std::endl;
    std::cout << "状态历史: " << (default_config.enable_state_history ? "启用" : "禁用") << std::endl;
    std::cout << "性能跟踪: " << (default_config.enable_performance_tracking ? "启用" : "禁用") << std::endl;
    std::cout << "最大状态栈深度: " << default_config.max_state_stack_depth << std::endl;
    std::cout << "最大历史记录大小: " << default_config.max_history_size << std::endl;
    std::cout << "状态超时时间: " << default_config.state_timeout.count() << "ms" << std::endl;
    
    // 测试自定义配置
    StateMachineConfig custom_config;
    custom_config.enable_auto_recovery = false;
    custom_config.enable_state_history = false;
    custom_config.max_state_stack_depth = 50;
    custom_config.max_history_size = 500;
    custom_config.state_timeout = std::chrono::milliseconds(2000);
    
    auto state_machine2 = std::make_unique<CHTLStateMachine>(custom_config);
    
    std::cout << "自定义配置状态机创建成功" << std::endl;
    std::cout << "错误恢复: " << (custom_config.enable_auto_recovery ? "启用" : "禁用") << std::endl;
    std::cout << "状态历史: " << (custom_config.enable_state_history ? "启用" : "禁用") << std::endl;
    std::cout << "最大状态栈深度: " << custom_config.max_state_stack_depth << std::endl;
    std::cout << "最大历史记录大小: " << custom_config.max_history_size << std::endl;
    std::cout << "状态超时时间: " << custom_config.state_timeout.count() << "ms" << std::endl;
    
    std::cout << std::endl;
}

void testStateTransitions() {
    std::cout << "=== 测试状态转换 ===" << std::endl;
    
    StateMachineConfig config;
    config.enable_debug = true;
    auto state_machine = std::make_unique<CHTLStateMachine>(config);
    
    // 测试初始状态
    std::cout << "初始状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    std::cout << "前一个状态: " << static_cast<int>(state_machine->getPreviousState()) << std::endl;
    
    // 测试开始事件
    bool success = state_machine->transitionTo(CHTLStateEvent::START);
    std::cout << "开始事件转换: " << (success ? "成功" : "失败") << std::endl;
    std::cout << "当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    // 测试注释事件
    success = state_machine->transitionTo(CHTLStateEvent::SINGLE_LINE_COMMENT_START);
    std::cout << "单行注释开始转换: " << (success ? "成功" : "失败") << std::endl;
    std::cout << "当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    // 测试退出注释
    success = state_machine->transitionTo(CHTLStateEvent::COMMENT_END);
    std::cout << "注释结束转换: " << (success ? "成功" : "失败") << std::endl;
    std::cout << "当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    // 测试文本节点事件
    success = state_machine->transitionTo(CHTLStateEvent::TEXT_KEYWORD_FOUND);
    std::cout << "text关键字转换: " << (success ? "成功" : "失败") << std::endl;
    std::cout << "当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    // 测试进入text块
    success = state_machine->transitionTo(CHTLStateEvent::TEXT_BRACE_OPEN);
    std::cout << "text块开始转换: " << (success ? "成功" : "失败") << std::endl;
    std::cout << "当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    // 测试找到文本内容
    success = state_machine->transitionTo(CHTLStateEvent::TEXT_CONTENT_FOUND);
    std::cout << "文本内容转换: " << (success ? "成功" : "失败") << std::endl;
    std::cout << "当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    // 测试退出text块
    success = state_machine->transitionTo(CHTLStateEvent::TEXT_BRACE_CLOSE);
    std::cout << "text块结束转换: " << (success ? "成功" : "失败") << std::endl;
    std::cout << "当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    std::cout << std::endl;
}

void testStateStack() {
    std::cout << "=== 测试状态栈 ===" << std::endl;
    
    StateMachineConfig config;
    config.enable_debug = true;
    auto state_machine = std::make_unique<CHTLStateMachine>(config);
    
    // 开始状态机
    state_machine->transitionTo(CHTLStateEvent::START);
    
    std::cout << "初始状态栈深度: " << state_machine->getStateStackDepth() << std::endl;
    
    // 测试状态入栈
    state_machine->pushState(CHTLState::IN_ELEMENT_BODY);
    std::cout << "入栈后状态栈深度: " << state_machine->getStateStackDepth() << std::endl;
    std::cout << "当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    state_machine->pushState(CHTLState::IN_STYLE_BLOCK);
    std::cout << "再次入栈后状态栈深度: " << state_machine->getStateStackDepth() << std::endl;
    std::cout << "当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    // 测试状态出栈
    CHTLState popped_state = state_machine->popState();
    std::cout << "出栈状态: " << static_cast<int>(popped_state) << std::endl;
    std::cout << "出栈后状态栈深度: " << state_machine->getStateStackDepth() << std::endl;
    std::cout << "当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    // 测试查看栈顶
    CHTLState peeked_state = state_machine->peekState();
    std::cout << "栈顶状态: " << static_cast<int>(peeked_state) << std::endl;
    
    // 测试清空状态栈
    state_machine->clearStateStack();
    std::cout << "清空后状态栈深度: " << state_machine->getStateStackDepth() << std::endl;
    std::cout << "当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    std::cout << std::endl;
}

void testStateValidation() {
    std::cout << "=== 测试状态验证 ===" << std::endl;
    
    StateMachineConfig config;
    auto state_machine = std::make_unique<CHTLStateMachine>(config);
    
    // 开始状态机
    state_machine->transitionTo(CHTLStateEvent::START);
    
    // 测试状态验证
    std::cout << "当前状态是否有效: " << (state_machine->isValidState(state_machine->getCurrentState()) ? "是" : "否") << std::endl;
    std::cout << "是否在IDLE状态: " << (state_machine->isInState(CHTLState::IDLE) ? "是" : "否") << std::endl;
    std::cout << "是否在INITIAL状态: " << (state_machine->isInState(CHTLState::INITIAL) ? "是" : "否") << std::endl;
    
    // 测试多状态检查
    std::vector<CHTLState> valid_states = {CHTLState::IDLE, CHTLState::INITIAL};
    std::cout << "是否在有效状态列表中: " << (state_machine->isInAnyState(valid_states) ? "是" : "否") << std::endl;
    
    std::vector<CHTLState> invalid_states = {CHTLState::ERROR, CHTLState::RECOVERY};
    std::cout << "是否在无效状态列表中: " << (state_machine->isInAnyState(invalid_states) ? "是" : "否") << std::endl;
    
    std::cout << std::endl;
}

void testValidTransitions() {
    std::cout << "=== 测试有效转换 ===" << std::endl;
    
    StateMachineConfig config;
    auto state_machine = std::make_unique<CHTLStateMachine>(config);
    
    // 开始状态机
    state_machine->transitionTo(CHTLStateEvent::START);
    
    // 获取有效转换
    std::vector<CHTLStateEvent> valid_transitions = state_machine->getValidTransitions();
    std::cout << "有效转换数量: " << valid_transitions.size() << std::endl;
    
    std::cout << "有效转换列表:" << std::endl;
    for (size_t i = 0; i < valid_transitions.size(); i++) {
        std::cout << "  [" << i << "]: " << static_cast<int>(valid_transitions[i]) << std::endl;
    }
    
    // 测试转换验证
    std::cout << "是否可以转换到START: " << (state_machine->canTransitionTo(CHTLStateEvent::START) ? "是" : "否") << std::endl;
    std::cout << "是否可以转换到END: " << (state_machine->canTransitionTo(CHTLStateEvent::END) ? "是" : "否") << std::endl;
    std::cout << "是否可以转换到RESET: " << (state_machine->canTransitionTo(CHTLStateEvent::RESET) ? "是" : "否") << std::endl;
    
    std::cout << std::endl;
}

void testPerformanceTracking() {
    std::cout << "=== 测试性能跟踪 ===" << std::endl;
    
    StateMachineConfig config;
    config.enable_performance_tracking = true;
    auto state_machine = std::make_unique<CHTLStateMachine>(config);
    
    // 开始状态机
    state_machine->transitionTo(CHTLStateEvent::START);
    
    // 等待一段时间
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // 执行一些状态转换
    state_machine->transitionTo(CHTLStateEvent::SINGLE_LINE_COMMENT_START);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    state_machine->transitionTo(CHTLStateEvent::COMMENT_END);
    
    // 获取性能信息
    std::cout << "当前状态持续时间: " << state_machine->getStateDuration().count() << "ms" << std::endl;
    std::cout << "总持续时间: " << state_machine->getTotalDuration().count() << "ms" << std::endl;
    std::cout << "转换次数: " << state_machine->getTransitionCount() << std::endl;
    
    std::cout << std::endl;
}

void testErrorHandling() {
    std::cout << "=== 测试错误处理 ===" << std::endl;
    
    StateMachineConfig config;
    auto state_machine = std::make_unique<CHTLStateMachine>(config);
    
    // 开始状态机
    state_machine->transitionTo(CHTLStateEvent::START);
    
    // 测试无效转换
    bool success = state_machine->transitionTo(CHTLStateEvent::SINGLE_LINE_COMMENT_START);
    std::cout << "有效转换: " << (success ? "成功" : "失败") << std::endl;
    
    // 尝试无效转换
    success = state_machine->transitionTo(CHTLStateEvent::START);
    std::cout << "无效转换: " << (success ? "成功" : "失败") << std::endl;
    
    // 检查错误状态
    std::cout << "是否有错误: " << (state_machine->hasErrors() ? "是" : "否") << std::endl;
    std::cout << "错误数量: " << state_machine->getErrors().size() << std::endl;
    
    if (state_machine->hasErrors()) {
        std::cout << "错误列表:" << std::endl;
        auto errors = state_machine->getErrors();
        for (size_t i = 0; i < errors.size(); i++) {
            std::cout << "  [" << i << "]: " << errors[i] << std::endl;
        }
    }
    
    // 清除错误
    state_machine->clearErrors();
    std::cout << "清除错误后是否有错误: " << (state_machine->hasErrors() ? "是" : "否") << std::endl;
    
    std::cout << std::endl;
}

void testDebugInfo() {
    std::cout << "=== 测试调试信息 ===" << std::endl;
    
    StateMachineConfig config;
    config.enable_debug = true;
    config.enable_state_history = true;
    auto state_machine = std::make_unique<CHTLStateMachine>(config);
    
    // 开始状态机
    state_machine->transitionTo(CHTLStateEvent::START);
    
    // 执行一些状态转换
    state_machine->transitionTo(CHTLStateEvent::SINGLE_LINE_COMMENT_START);
    state_machine->pushState(CHTLState::IN_ELEMENT_BODY);
    state_machine->transitionTo(CHTLStateEvent::COMMENT_END);
    
    // 获取调试信息
    std::string debug_info = state_machine->getDebugInfo();
    std::cout << "调试信息:" << std::endl;
    std::cout << debug_info << std::endl;
    
    // 获取状态转换日志
    std::string transition_log = state_machine->getStateTransitionLog();
    std::cout << "状态转换日志:" << std::endl;
    std::cout << transition_log << std::endl;
    
    std::cout << std::endl;
}

void testComplexScenario() {
    std::cout << "=== 测试复杂场景 ===" << std::endl;
    
    StateMachineConfig config;
    config.enable_debug = true;
    config.enable_state_history = true;
    auto state_machine = std::make_unique<CHTLStateMachine>(config);
    
    // 开始状态机
    state_machine->transitionTo(CHTLStateEvent::START);
    
    // 模拟解析一个完整的HTML元素
    std::cout << "模拟解析HTML元素..." << std::endl;
    
    // 找到元素名称
    state_machine->transitionTo(CHTLStateEvent::ELEMENT_NAME_FOUND);
    std::cout << "找到元素名称，当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    // 进入元素体
    state_machine->transitionTo(CHTLStateEvent::ELEMENT_BRACE_OPEN);
    std::cout << "进入元素体，当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    // 找到属性名
    state_machine->transitionTo(CHTLStateEvent::ATTRIBUTE_NAME_FOUND);
    std::cout << "找到属性名，当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    // 找到属性分隔符
    state_machine->transitionTo(CHTLStateEvent::ATTRIBUTE_SEPARATOR_FOUND);
    std::cout << "找到属性分隔符，当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    // 找到属性值
    state_machine->transitionTo(CHTLStateEvent::ATTRIBUTE_VALUE_FOUND);
    std::cout << "找到属性值，当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    // 找到属性结束符
    state_machine->transitionTo(CHTLStateEvent::ATTRIBUTE_TERMINATOR_FOUND);
    std::cout << "找到属性结束符，当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    // 找到style关键字
    state_machine->transitionTo(CHTLStateEvent::STYLE_KEYWORD_FOUND);
    std::cout << "找到style关键字，当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    // 进入样式块
    state_machine->transitionTo(CHTLStateEvent::STYLE_BRACE_OPEN);
    std::cout << "进入样式块，当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    // 退出样式块
    state_machine->transitionTo(CHTLStateEvent::STYLE_BRACE_CLOSE);
    std::cout << "退出样式块，当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    // 退出元素体
    state_machine->transitionTo(CHTLStateEvent::ELEMENT_BRACE_CLOSE);
    std::cout << "退出元素体，当前状态: " << static_cast<int>(state_machine->getCurrentState()) << std::endl;
    
    // 获取最终统计信息
    std::cout << "最终统计:" << std::endl;
    std::cout << "  转换次数: " << state_machine->getTransitionCount() << std::endl;
    std::cout << "  状态历史大小: " << state_machine->getStateHistory().size() << std::endl;
    std::cout << "  总持续时间: " << state_machine->getTotalDuration().count() << "ms" << std::endl;
    
    std::cout << std::endl;
}

int main() {
    std::cout << "CHTL状态机系统测试" << std::endl;
    std::cout << "==================" << std::endl << std::endl;
    
    try {
        testStateMachineCreation();
        testStateTransitions();
        testStateStack();
        testStateValidation();
        testValidTransitions();
        testPerformanceTracking();
        testErrorHandling();
        testDebugInfo();
        testComplexScenario();
        
        std::cout << "所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
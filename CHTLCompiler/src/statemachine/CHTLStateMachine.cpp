#include "statemachine/CHTLStateMachine.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <chrono>

namespace chtl {

// 实现类
class CHTLStateMachine::Impl {
public:
    StateMachineConfig config;
    CHTLState current_state;
    CHTLState previous_state;
    std::vector<CHTLState> state_history;
    std::stack<CHTLState> state_stack;
    std::vector<StateTransition> state_transitions;
    std::vector<std::string> errors;
    std::vector<std::pair<CHTLState, CHTLStateEvent>> transition_log;
    
    // 性能跟踪
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point state_start_time;
    std::chrono::steady_clock::time_point last_transition_time;
    size_t transition_count;
    bool debug_mode;
    
    Impl(const StateMachineConfig& cfg) : 
        config(cfg), current_state(CHTLState::INITIAL), previous_state(CHTLState::INITIAL),
        transition_count(0), debug_mode(false) {
        start_time = std::chrono::steady_clock::now();
        state_start_time = start_time;
        last_transition_time = start_time;
        initializeStateTransitions();
    }
    
    ~Impl() = default;
    
    // 状态转换验证
    bool canTransitionTo(CHTLStateEvent event) const;
    CHTLState getNextState(CHTLStateEvent event) const;
    void recordStateTransition(CHTLState from, CHTLStateEvent event, CHTLState to);
    void updatePerformanceMetrics();
    void handleStateTimeout();
    void attemptAutoRecovery();
};

// CHTLStateMachine实现
CHTLStateMachine::CHTLStateMachine(const StateMachineConfig& config) 
    : pImpl(std::make_unique<Impl>(config)) {}

CHTLStateMachine::~CHTLStateMachine() = default;

CHTLState CHTLStateMachine::getCurrentState() const {
    return pImpl->current_state;
}

CHTLState CHTLStateMachine::getPreviousState() const {
    return pImpl->previous_state;
}

std::vector<CHTLState> CHTLStateMachine::getStateHistory() const {
    return pImpl->state_history;
}

size_t CHTLStateMachine::getStateStackDepth() const {
    return pImpl->state_stack.size();
}

bool CHTLStateMachine::transitionTo(CHTLStateEvent event) {
    if (!canTransitionTo(event)) {
        addError("无效的状态转换: " + std::to_string(static_cast<int>(event)));
        return false;
    }
    
    CHTLState next_state = pImpl->getNextState(event);
    if (next_state == CHTLState::ERROR) {
        addError("状态转换失败，进入错误状态");
        return false;
    }
    
    // 记录状态转换
    pImpl->recordStateTransition(pImpl->current_state, event, next_state);
    
    // 更新状态
    pImpl->previous_state = pImpl->current_state;
    pImpl->current_state = next_state;
    pImpl->state_start_time = std::chrono::steady_clock::now();
    pImpl->transition_count++;
    
    if (pImpl->debug_mode) {
        std::cout << "状态转换: " << static_cast<int>(pImpl->previous_state) 
                  << " -> " << static_cast<int>(pImpl->current_state) 
                  << " (事件: " << static_cast<int>(event) << ")" << std::endl;
    }
    
    return true;
}

bool CHTLStateMachine::canTransitionTo(CHTLStateEvent event) const {
    return pImpl->canTransitionTo(event);
}

std::vector<CHTLStateEvent> CHTLStateMachine::getValidTransitions() const {
    std::vector<CHTLStateEvent> valid_events;
    for (const auto& transition : pImpl->state_transitions) {
        if (transition.from_state == pImpl->current_state && transition.is_valid) {
            valid_events.push_back(transition.event);
        }
    }
    return valid_events;
}

void CHTLStateMachine::pushState(CHTLState state) {
    if (pImpl->state_stack.size() >= pImpl->config.max_state_stack_depth) {
        addError("状态栈深度超出限制");
        return;
    }
    
    pImpl->state_stack.push(pImpl->current_state);
    pImpl->current_state = state;
    pImpl->state_start_time = std::chrono::steady_clock::now();
    
    if (pImpl->debug_mode) {
        std::cout << "状态入栈: " << static_cast<int>(state) << std::endl;
    }
}

CHTLState CHTLStateMachine::popState() {
    if (pImpl->state_stack.empty()) {
        addError("状态栈为空，无法弹出");
        return CHTLState::ERROR;
    }
    
    CHTLState popped_state = pImpl->current_state;
    pImpl->current_state = pImpl->state_stack.top();
    pImpl->state_stack.pop();
    pImpl->state_start_time = std::chrono::steady_clock::now();
    
    if (pImpl->debug_mode) {
        std::cout << "状态出栈: " << static_cast<int>(popped_state) 
                  << " -> " << static_cast<int>(pImpl->current_state) << std::endl;
    }
    
    return popped_state;
}

CHTLState CHTLStateMachine::peekState() const {
    if (pImpl->state_stack.empty()) {
        return CHTLState::ERROR;
    }
    return pImpl->state_stack.top();
}

void CHTLStateMachine::clearStateStack() {
    while (!pImpl->state_stack.empty()) {
        pImpl->state_stack.pop();
    }
    pImpl->current_state = CHTLState::INITIAL;
    pImpl->state_start_time = std::chrono::steady_clock::now();
    
    if (pImpl->debug_mode) {
        std::cout << "状态栈已清空" << std::endl;
    }
}

bool CHTLStateMachine::isValidState(CHTLState state) const {
    return static_cast<int>(state) >= 0 && 
           static_cast<int>(state) < static_cast<int>(CHTLState::ERROR);
}

bool CHTLStateMachine::isInState(CHTLState state) const {
    return pImpl->current_state == state;
}

bool CHTLStateMachine::isInAnyState(const std::vector<CHTLState>& states) const {
    return std::find(states.begin(), states.end(), pImpl->current_state) != states.end();
}

std::chrono::milliseconds CHTLStateMachine::getStateDuration() const {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - pImpl->state_start_time);
}

std::chrono::milliseconds CHTLStateMachine::getTotalDuration() const {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - pImpl->start_time);
}

size_t CHTLStateMachine::getTransitionCount() const {
    return pImpl->transition_count;
}

bool CHTLStateMachine::hasErrors() const {
    return !pImpl->errors.empty();
}

std::vector<std::string> CHTLStateMachine::getErrors() const {
    return pImpl->errors;
}

void CHTLStateMachine::clearErrors() {
    pImpl->errors.clear();
}

void CHTLStateMachine::addError(const std::string& error_message) {
    pImpl->errors.push_back(error_message);
    
    if (pImpl->debug_mode) {
        std::cerr << "状态机错误: " << error_message << std::endl;
    }
}

void CHTLStateMachine::setConfig(const StateMachineConfig& config) {
    pImpl->config = config;
}

StateMachineConfig CHTLStateMachine::getConfig() const {
    return pImpl->config;
}

std::string CHTLStateMachine::getDebugInfo() const {
    std::ostringstream oss;
    oss << "CHTL状态机调试信息:\n";
    oss << "当前状态: " << static_cast<int>(pImpl->current_state) << "\n";
    oss << "前一个状态: " << static_cast<int>(pImpl->previous_state) << "\n";
    oss << "状态栈深度: " << pImpl->state_stack.size() << "\n";
    oss << "状态历史大小: " << pImpl->state_history.size() << "\n";
    oss << "转换次数: " << pImpl->transition_count << "\n";
    oss << "当前状态持续时间: " << getStateDuration().count() << "ms\n";
    oss << "总持续时间: " << getTotalDuration().count() << "ms\n";
    oss << "错误数量: " << pImpl->errors.size() << "\n";
    oss << "调试模式: " << (pImpl->debug_mode ? "启用" : "禁用") << "\n";
    
    if (!pImpl->state_stack.empty()) {
        oss << "状态栈内容:\n";
        std::stack<CHTLState> temp_stack = pImpl->state_stack;
        std::vector<CHTLState> stack_content;
        while (!temp_stack.empty()) {
            stack_content.push_back(temp_stack.top());
            temp_stack.pop();
        }
        for (size_t i = 0; i < stack_content.size(); i++) {
            oss << "  [" << i << "]: " << static_cast<int>(stack_content[i]) << "\n";
        }
    }
    
    return oss.str();
}

std::string CHTLStateMachine::getStateTransitionLog() const {
    std::ostringstream oss;
    oss << "状态转换日志:\n";
    for (size_t i = 0; i < pImpl->transition_log.size(); i++) {
        const auto& log_entry = pImpl->transition_log[i];
        oss << "[" << i << "] " << static_cast<int>(log_entry.first) 
            << " -> " << static_cast<int>(log_entry.second) << "\n";
    }
    return oss.str();
}

void CHTLStateMachine::enableDebugMode(bool enable) {
    pImpl->debug_mode = enable;
}

// Impl类方法实现
void CHTLStateMachine::Impl::initializeStateTransitions() {
    // 基础状态转换
    state_transitions.emplace_back(CHTLState::INITIAL, CHTLStateEvent::START, CHTLState::IDLE, "开始");
    state_transitions.emplace_back(CHTLState::IDLE, CHTLStateEvent::END, CHTLState::INITIAL, "结束");
    state_transitions.emplace_back(CHTLState::IDLE, CHTLStateEvent::RESET, CHTLState::INITIAL, "重置");
    
    // 注释状态转换
    state_transitions.emplace_back(CHTLState::IDLE, CHTLStateEvent::SINGLE_LINE_COMMENT_START, CHTLState::IN_SINGLE_LINE_COMMENT, "进入单行注释");
    state_transitions.emplace_back(CHTLState::IDLE, CHTLStateEvent::MULTI_LINE_COMMENT_START, CHTLState::IN_MULTI_LINE_COMMENT, "进入多行注释");
    state_transitions.emplace_back(CHTLState::IDLE, CHTLStateEvent::GENERATOR_COMMENT_START, CHTLState::IN_GENERATOR_COMMENT, "进入生成器注释");
    state_transitions.emplace_back(CHTLState::IN_SINGLE_LINE_COMMENT, CHTLStateEvent::COMMENT_END, CHTLState::IDLE, "退出单行注释");
    state_transitions.emplace_back(CHTLState::IN_MULTI_LINE_COMMENT, CHTLStateEvent::COMMENT_END, CHTLState::IDLE, "退出多行注释");
    state_transitions.emplace_back(CHTLState::IN_GENERATOR_COMMENT, CHTLStateEvent::COMMENT_END, CHTLState::IDLE, "退出生成器注释");
    
    // 文本节点状态转换
    state_transitions.emplace_back(CHTLState::IDLE, CHTLStateEvent::TEXT_KEYWORD_FOUND, CHTLState::EXPECTING_TEXT_KEYWORD, "找到text关键字");
    state_transitions.emplace_back(CHTLState::EXPECTING_TEXT_KEYWORD, CHTLStateEvent::TEXT_BRACE_OPEN, CHTLState::IN_TEXT_BLOCK, "进入text块");
    state_transitions.emplace_back(CHTLState::IN_TEXT_BLOCK, CHTLStateEvent::TEXT_CONTENT_FOUND, CHTLState::EXPECTING_TEXT_CONTENT, "找到文本内容");
    state_transitions.emplace_back(CHTLState::EXPECTING_TEXT_CONTENT, CHTLStateEvent::TEXT_BRACE_CLOSE, CHTLState::EXPECTING_TEXT_BRACE_CLOSE, "期望text块结束");
    state_transitions.emplace_back(CHTLState::EXPECTING_TEXT_BRACE_CLOSE, CHTLStateEvent::TEXT_BRACE_CLOSE, CHTLState::IDLE, "退出text块");
    
    // 字面量状态转换
    state_transitions.emplace_back(CHTLState::IDLE, CHTLStateEvent::LITERAL_START, CHTLState::EXPECTING_LITERAL_VALUE, "开始字面量");
    state_transitions.emplace_back(CHTLState::EXPECTING_LITERAL_VALUE, CHTLStateEvent::QUOTE_FOUND, CHTLState::IN_QUOTED_STRING, "进入引号字符串");
    state_transitions.emplace_back(CHTLState::EXPECTING_LITERAL_VALUE, CHTLStateEvent::LITERAL_END, CHTLState::IN_UNQUOTED_STRING, "进入无引号字符串");
    state_transitions.emplace_back(CHTLState::IN_QUOTED_STRING, CHTLStateEvent::LITERAL_END, CHTLState::IDLE, "退出引号字符串");
    state_transitions.emplace_back(CHTLState::IN_UNQUOTED_STRING, CHTLStateEvent::LITERAL_END, CHTLState::IDLE, "退出无引号字符串");
    
    // 元素状态转换
    state_transitions.emplace_back(CHTLState::IDLE, CHTLStateEvent::ELEMENT_NAME_FOUND, CHTLState::EXPECTING_ELEMENT_NAME, "找到元素名称");
    state_transitions.emplace_back(CHTLState::EXPECTING_ELEMENT_NAME, CHTLStateEvent::ELEMENT_BRACE_OPEN, CHTLState::IN_ELEMENT_BODY, "进入元素体");
    state_transitions.emplace_back(CHTLState::IN_ELEMENT_BODY, CHTLStateEvent::ELEMENT_BRACE_CLOSE, CHTLState::IDLE, "退出元素体");
    state_transitions.emplace_back(CHTLState::EXPECTING_ELEMENT_NAME, CHTLStateEvent::ELEMENT_SELF_CLOSING, CHTLState::IDLE, "元素自闭合");
    
    // 属性状态转换
    state_transitions.emplace_back(CHTLState::IN_ELEMENT_BODY, CHTLStateEvent::ATTRIBUTE_NAME_FOUND, CHTLState::EXPECTING_ATTRIBUTE_NAME, "找到属性名");
    state_transitions.emplace_back(CHTLState::EXPECTING_ATTRIBUTE_NAME, CHTLStateEvent::ATTRIBUTE_SEPARATOR_FOUND, CHTLState::EXPECTING_ATTRIBUTE_SEPARATOR, "找到属性分隔符");
    state_transitions.emplace_back(CHTLState::EXPECTING_ATTRIBUTE_SEPARATOR, CHTLStateEvent::ATTRIBUTE_VALUE_FOUND, CHTLState::EXPECTING_ATTRIBUTE_VALUE, "找到属性值");
    state_transitions.emplace_back(CHTLState::EXPECTING_ATTRIBUTE_VALUE, CHTLStateEvent::ATTRIBUTE_TERMINATOR_FOUND, CHTLState::EXPECTING_ATTRIBUTE_TERMINATOR, "找到属性结束符");
    state_transitions.emplace_back(CHTLState::EXPECTING_ATTRIBUTE_TERMINATOR, CHTLStateEvent::ATTRIBUTE_TERMINATOR_FOUND, CHTLState::IN_ELEMENT_BODY, "属性解析完成");
    
    // 样式块状态转换
    state_transitions.emplace_back(CHTLState::IN_ELEMENT_BODY, CHTLStateEvent::STYLE_KEYWORD_FOUND, CHTLState::EXPECTING_STYLE_KEYWORD, "找到style关键字");
    state_transitions.emplace_back(CHTLState::EXPECTING_STYLE_KEYWORD, CHTLStateEvent::STYLE_BRACE_OPEN, CHTLState::IN_STYLE_BLOCK, "进入样式块");
    state_transitions.emplace_back(CHTLState::IN_STYLE_BLOCK, CHTLStateEvent::STYLE_BRACE_CLOSE, CHTLState::IN_ELEMENT_BODY, "退出样式块");
    state_transitions.emplace_back(CHTLState::IN_STYLE_BLOCK, CHTLStateEvent::CSS_SELECTOR_FOUND, CHTLState::EXPECTING_CSS_SELECTOR, "找到CSS选择器");
    state_transitions.emplace_back(CHTLState::EXPECTING_CSS_SELECTOR, CHTLStateEvent::CSS_PROPERTY_FOUND, CHTLState::EXPECTING_CSS_PROPERTY, "找到CSS属性");
    state_transitions.emplace_back(CHTLState::EXPECTING_CSS_PROPERTY, CHTLStateEvent::CSS_VALUE_FOUND, CHTLState::EXPECTING_CSS_VALUE, "找到CSS值");
    state_transitions.emplace_back(CHTLState::EXPECTING_CSS_VALUE, CHTLStateEvent::CSS_TERMINATOR_FOUND, CHTLState::EXPECTING_CSS_TERMINATOR, "找到CSS结束符");
    state_transitions.emplace_back(CHTLState::EXPECTING_CSS_TERMINATOR, CHTLStateEvent::CSS_TERMINATOR_FOUND, CHTLState::IN_STYLE_BLOCK, "CSS规则完成");
    
    // 模板状态转换
    state_transitions.emplace_back(CHTLState::IDLE, CHTLStateEvent::TEMPLATE_KEYWORD_FOUND, CHTLState::EXPECTING_TEMPLATE_KEYWORD, "找到[Template]关键字");
    state_transitions.emplace_back(CHTLState::EXPECTING_TEMPLATE_KEYWORD, CHTLStateEvent::TEMPLATE_TYPE_FOUND, CHTLState::EXPECTING_TEMPLATE_TYPE, "找到模板类型");
    state_transitions.emplace_back(CHTLState::EXPECTING_TEMPLATE_TYPE, CHTLStateEvent::TEMPLATE_NAME_FOUND, CHTLState::EXPECTING_TEMPLATE_NAME, "找到模板名称");
    state_transitions.emplace_back(CHTLState::EXPECTING_TEMPLATE_NAME, CHTLStateEvent::TEMPLATE_BRACE_OPEN, CHTLState::IN_TEMPLATE_BODY, "进入模板体");
    state_transitions.emplace_back(CHTLState::IN_TEMPLATE_BODY, CHTLStateEvent::TEMPLATE_BRACE_CLOSE, CHTLState::IDLE, "退出模板体");
    
    // 自定义状态转换
    state_transitions.emplace_back(CHTLState::IDLE, CHTLStateEvent::CUSTOM_KEYWORD_FOUND, CHTLState::EXPECTING_CUSTOM_KEYWORD, "找到[Custom]关键字");
    state_transitions.emplace_back(CHTLState::EXPECTING_CUSTOM_KEYWORD, CHTLStateEvent::CUSTOM_TYPE_FOUND, CHTLState::EXPECTING_CUSTOM_TYPE, "找到自定义类型");
    state_transitions.emplace_back(CHTLState::EXPECTING_CUSTOM_TYPE, CHTLStateEvent::CUSTOM_NAME_FOUND, CHTLState::EXPECTING_CUSTOM_NAME, "找到自定义名称");
    state_transitions.emplace_back(CHTLState::EXPECTING_CUSTOM_NAME, CHTLStateEvent::CUSTOM_BRACE_OPEN, CHTLState::IN_CUSTOM_BODY, "进入自定义体");
    state_transitions.emplace_back(CHTLState::IN_CUSTOM_BODY, CHTLStateEvent::CUSTOM_BRACE_CLOSE, CHTLState::IDLE, "退出自定义体");
    
    // 原始嵌入状态转换
    state_transitions.emplace_back(CHTLState::IDLE, CHTLStateEvent::ORIGIN_KEYWORD_FOUND, CHTLState::EXPECTING_ORIGIN_KEYWORD, "找到[Origin]关键字");
    state_transitions.emplace_back(CHTLState::EXPECTING_ORIGIN_KEYWORD, CHTLStateEvent::ORIGIN_TYPE_FOUND, CHTLState::EXPECTING_ORIGIN_TYPE, "找到原始嵌入类型");
    state_transitions.emplace_back(CHTLState::EXPECTING_ORIGIN_TYPE, CHTLStateEvent::ORIGIN_CONTENT_START, CHTLState::IN_ORIGIN_CONTENT, "进入原始嵌入内容");
    state_transitions.emplace_back(CHTLState::IN_ORIGIN_CONTENT, CHTLStateEvent::END, CHTLState::IDLE, "退出原始嵌入内容");
    
    // 导入状态转换
    state_transitions.emplace_back(CHTLState::IDLE, CHTLStateEvent::IMPORT_KEYWORD_FOUND, CHTLState::EXPECTING_IMPORT_KEYWORD, "找到[Import]关键字");
    state_transitions.emplace_back(CHTLState::EXPECTING_IMPORT_KEYWORD, CHTLStateEvent::IMPORT_TYPE_FOUND, CHTLState::EXPECTING_IMPORT_TYPE, "找到导入类型");
    state_transitions.emplace_back(CHTLState::EXPECTING_IMPORT_TYPE, CHTLStateEvent::IMPORT_PATH_FOUND, CHTLState::EXPECTING_IMPORT_PATH, "找到导入路径");
    state_transitions.emplace_back(CHTLState::EXPECTING_IMPORT_PATH, CHTLStateEvent::IMPORT_NAME_FOUND, CHTLState::EXPECTING_IMPORT_NAME, "找到导入名称");
    state_transitions.emplace_back(CHTLState::EXPECTING_IMPORT_NAME, CHTLStateEvent::END, CHTLState::IDLE, "导入声明完成");
    
    // 命名空间状态转换
    state_transitions.emplace_back(CHTLState::IDLE, CHTLStateEvent::NAMESPACE_KEYWORD_FOUND, CHTLState::EXPECTING_NAMESPACE_KEYWORD, "找到[Namespace]关键字");
    state_transitions.emplace_back(CHTLState::EXPECTING_NAMESPACE_KEYWORD, CHTLStateEvent::NAMESPACE_NAME_FOUND, CHTLState::EXPECTING_NAMESPACE_NAME, "找到命名空间名称");
    state_transitions.emplace_back(CHTLState::EXPECTING_NAMESPACE_NAME, CHTLStateEvent::NAMESPACE_BRACE_OPEN, CHTLState::IN_NAMESPACE_BODY, "进入命名空间体");
    state_transitions.emplace_back(CHTLState::IN_NAMESPACE_BODY, CHTLStateEvent::NAMESPACE_BRACE_CLOSE, CHTLState::IDLE, "退出命名空间体");
    
    // 约束状态转换
    state_transitions.emplace_back(CHTLState::IN_ELEMENT_BODY, CHTLStateEvent::CONSTRAINT_KEYWORD_FOUND, CHTLState::EXPECTING_CONSTRAINT_KEYWORD, "找到except关键字");
    state_transitions.emplace_back(CHTLState::EXPECTING_CONSTRAINT_KEYWORD, CHTLStateEvent::CONSTRAINT_TARGET_FOUND, CHTLState::EXPECTING_CONSTRAINT_TARGET, "找到约束目标");
    state_transitions.emplace_back(CHTLState::EXPECTING_CONSTRAINT_TARGET, CHTLStateEvent::CONSTRAINT_TYPE_FOUND, CHTLState::EXPECTING_CONSTRAINT_TYPE, "找到约束类型");
    state_transitions.emplace_back(CHTLState::EXPECTING_CONSTRAINT_TYPE, CHTLStateEvent::END, CHTLState::IN_ELEMENT_BODY, "约束声明完成");
    
    // 脚本状态转换
    state_transitions.emplace_back(CHTLState::IN_ELEMENT_BODY, CHTLStateEvent::SCRIPT_KEYWORD_FOUND, CHTLState::EXPECTING_SCRIPT_KEYWORD, "找到script关键字");
    state_transitions.emplace_back(CHTLState::EXPECTING_SCRIPT_KEYWORD, CHTLStateEvent::SCRIPT_BRACE_OPEN, CHTLState::IN_SCRIPT_BLOCK, "进入脚本块");
    state_transitions.emplace_back(CHTLState::IN_SCRIPT_BLOCK, CHTLStateEvent::SCRIPT_BRACE_CLOSE, CHTLState::IN_ELEMENT_BODY, "退出脚本块");
    
    // 错误恢复转换
    state_transitions.emplace_back(CHTLState::ERROR, CHTLStateEvent::RECOVERY_ATTEMPTED, CHTLState::RECOVERY, "尝试恢复");
    state_transitions.emplace_back(CHTLState::RECOVERY, CHTLStateEvent::RECOVERY_SUCCEEDED, CHTLState::IDLE, "恢复成功");
    state_transitions.emplace_back(CHTLState::RECOVERY, CHTLStateEvent::RECOVERY_FAILED, CHTLState::ERROR, "恢复失败");
    
    // 从任何状态都可以进入错误状态
    for (int i = 0; i < static_cast<int>(CHTLState::ERROR); i++) {
        CHTLState state = static_cast<CHTLState>(i);
        state_transitions.emplace_back(state, CHTLStateEvent::ERROR_OCCURRED, CHTLState::ERROR, "发生错误");
    }
}

bool CHTLStateMachine::Impl::canTransitionTo(CHTLStateEvent event) const {
    for (const auto& transition : state_transitions) {
        if (transition.from_state == current_state && transition.event == event && transition.is_valid) {
            return true;
        }
    }
    return false;
}

CHTLState CHTLStateMachine::Impl::getNextState(CHTLStateEvent event) const {
    for (const auto& transition : state_transitions) {
        if (transition.from_state == current_state && transition.event == event && transition.is_valid) {
            return transition.to_state;
        }
    }
    return CHTLState::ERROR;
}

void CHTLStateMachine::Impl::recordStateTransition(CHTLState from, CHTLStateEvent event, CHTLState to) {
    // 记录到历史
    if (config.enable_state_history && state_history.size() < config.max_history_size) {
        state_history.push_back(from);
    }
    
    // 记录到日志
    transition_log.emplace_back(from, event);
    
    // 更新性能指标
    updatePerformanceMetrics();
    
    // 检查状态超时
    handleStateTimeout();
}

void CHTLStateMachine::Impl::updatePerformanceMetrics() {
    last_transition_time = std::chrono::steady_clock::now();
}

void CHTLStateMachine::Impl::handleStateTimeout() {
    if (config.state_timeout.count() > 0) {
        auto current_time = std::chrono::steady_clock::now();
        auto state_duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - state_start_time);
        
        if (state_duration > config.state_timeout) {
            // 状态超时，尝试自动恢复
            if (config.enable_auto_recovery) {
                attemptAutoRecovery();
            }
        }
    }
}

void CHTLStateMachine::Impl::attemptAutoRecovery() {
    // 简单的自动恢复策略：回到上一个有效状态
    if (!state_stack.empty()) {
        current_state = state_stack.top();
        state_stack.pop();
        state_start_time = std::chrono::steady_clock::now();
        
        if (debug_mode) {
            std::cout << "自动恢复: 回到状态 " << static_cast<int>(current_state) << std::endl;
        }
    } else {
        // 如果栈为空，回到初始状态
        current_state = CHTLState::INITIAL;
        state_start_time = std::chrono::steady_clock::now();
        
        if (debug_mode) {
            std::cout << "自动恢复: 回到初始状态" << std::endl;
        }
    }
}

} // namespace chtl
#include "core/compilation_context.hpp"
#include "constraints/chtl_global_style_constraint.hpp"
#include "constraints/chtl_local_style_constraint.hpp"
#include "constraints/chtl_local_script_constraint.hpp"
#include "constraints/chtl_script_constraint.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

namespace chtl {

// ==================== CompilationContext 实现 ====================

CompilationContext::CompilationContext() 
    : currentPhase_(CompilationPhase::INITIALIZATION)
    , currentState_(CompilationState::IDLE)
    , debugMode_(false) {
    syntaxContextStack_.push_back(SyntaxContext::GLOBAL_SCOPE);
}

CompilationContext::~CompilationContext() {
    clear();
}

void CompilationContext::setPhase(CompilationPhase phase) {
    CompilationPhase oldPhase = currentPhase_;
    currentPhase_ = phase;
    logPhaseChange(oldPhase, phase);
}

void CompilationContext::setState(CompilationState state) {
    CompilationState oldState = currentState_;
    currentState_ = state;
    logStateChange(oldState, state);
}

void CompilationContext::setConfiguration(std::shared_ptr<CHtlConfiguration> config) {
    configuration_ = config;
    if (config && debugMode_) {
        debugLog("配置已加载", "CompilationContext");
    }
}

void CompilationContext::setSourceCode(const std::string& source) {
    sourceCode_ = source;
    setMetadata("source_length", std::to_string(source.length()));
    setMetadata("source_lines", std::to_string(std::count(source.begin(), source.end(), '\n') + 1));
}

void CompilationContext::setCodeFragments(const std::vector<CodeFragment>& fragments) {
    codeFragments_ = fragments;
    setMetadata("fragment_count", std::to_string(fragments.size()));
    
    // 统计各类型片段
    int chtlCount = 0, chtlJsCount = 0, cssCount = 0, jsCount = 0;
    for (const auto& fragment : fragments) {
        switch (fragment.type) {
            case CodeFragmentType::CHTL_FRAGMENT: chtlCount++; break;
            case CodeFragmentType::CHTL_JS_FRAGMENT: chtlJsCount++; break;
            case CodeFragmentType::CSS_FRAGMENT: cssCount++; break;
            case CodeFragmentType::JS_FRAGMENT: jsCount++; break;
        }
    }
    
    setMetadata("chtl_fragments", std::to_string(chtlCount));
    setMetadata("chtl_js_fragments", std::to_string(chtlJsCount));
    setMetadata("css_fragments", std::to_string(cssCount));
    setMetadata("js_fragments", std::to_string(jsCount));
}

void CompilationContext::addCodeFragment(const CodeFragment& fragment) {
    codeFragments_.push_back(fragment);
    setMetadata("fragment_count", std::to_string(codeFragments_.size()));
}

void CompilationContext::addFragmentResult(const FragmentCompilationResult& result) {
    fragmentResults_.push_back(result);
    
    if (!result.result.success) {
        for (const auto& error : result.result.errors) {
            addError(error, "Fragment-" + result.compilerId);
        }
    }
    
    for (const auto& warning : result.result.warnings) {
        addWarning(warning, "Fragment-" + result.compilerId);
    }
}

void CompilationContext::setFinalResult(const CompilationResult& result) {
    finalResult_ = result;
    setState(result.success ? CompilationState::SUCCESS : CompilationState::FAILED);
}

void CompilationContext::addError(const std::string& error, const std::string& phase) {
    std::string fullError = phase.empty() ? error : "[" + phase + "] " + error;
    errors_.push_back(fullError);
    
    if (debugMode_) {
        debugLog("错误: " + fullError, "CompilationContext");
    }
}

void CompilationContext::addWarning(const std::string& warning, const std::string& phase) {
    std::string fullWarning = phase.empty() ? warning : "[" + phase + "] " + warning;
    warnings_.push_back(fullWarning);
    
    if (debugMode_) {
        debugLog("警告: " + fullWarning, "CompilationContext");
    }
}

void CompilationContext::setMetadata(const std::string& key, const std::string& value) {
    metadata_[key] = value;
}

std::string CompilationContext::getMetadata(const std::string& key) const {
    auto it = metadata_.find(key);
    return it != metadata_.end() ? it->second : "";
}

void CompilationContext::pushSyntaxContext(SyntaxContext context) {
    syntaxContextStack_.push_back(context);
    if (debugMode_) {
        debugLog("进入语法上下文: " + std::to_string(static_cast<int>(context)), "SyntaxContext");
    }
}

void CompilationContext::popSyntaxContext() {
    if (syntaxContextStack_.size() > 1) {
        SyntaxContext oldContext = syntaxContextStack_.back();
        syntaxContextStack_.pop_back();
        if (debugMode_) {
            debugLog("退出语法上下文: " + std::to_string(static_cast<int>(oldContext)), "SyntaxContext");
        }
    }
}

SyntaxContext CompilationContext::getCurrentSyntaxContext() const {
    return syntaxContextStack_.empty() ? SyntaxContext::GLOBAL_SCOPE : syntaxContextStack_.back();
}

void CompilationContext::startTimer(const std::string& name) {
    timers_[name] = std::chrono::high_resolution_clock::now();
}

void CompilationContext::stopTimer(const std::string& name) {
    auto it = timers_.find(name);
    if (it != timers_.end()) {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - it->second);
        elapsedTimes_[name] = duration.count() / 1000.0; // 转换为毫秒
        timers_.erase(it);
    }
}

double CompilationContext::getElapsedTime(const std::string& name) const {
    auto it = elapsedTimes_.find(name);
    return it != elapsedTimes_.end() ? it->second : 0.0;
}

void CompilationContext::debugLog(const std::string& message, const std::string& component) {
    if (debugMode_) {
        std::string prefix = component.empty() ? "[DEBUG] " : "[DEBUG:" + component + "] ";
        std::cout << prefix << message << std::endl;
    }
}

void CompilationContext::clear() {
    sourceCode_.clear();
    codeFragments_.clear();
    fragmentResults_.clear();
    errors_.clear();
    warnings_.clear();
    metadata_.clear();
    syntaxContextStack_.clear();
    syntaxContextStack_.push_back(SyntaxContext::GLOBAL_SCOPE);
    timers_.clear();
    elapsedTimes_.clear();
    
    currentPhase_ = CompilationPhase::INITIALIZATION;
    currentState_ = CompilationState::IDLE;
}

void CompilationContext::logPhaseChange(CompilationPhase oldPhase, CompilationPhase newPhase) {
    if (debugMode_) {
        debugLog("阶段转换: " + std::to_string(static_cast<int>(oldPhase)) + 
                " -> " + std::to_string(static_cast<int>(newPhase)), "PhaseTransition");
    }
}

void CompilationContext::logStateChange(CompilationState oldState, CompilationState newState) {
    if (debugMode_) {
        debugLog("状态转换: " + std::to_string(static_cast<int>(oldState)) + 
                " -> " + std::to_string(static_cast<int>(newState)), "StateTransition");
    }
}

// ==================== CompilationStateMachine 实现 ====================

CompilationStateMachine::CompilationStateMachine() {
    // 注册默认的阶段处理器
    registerPhaseHandler(CompilationPhase::INITIALIZATION, [](CompilationContext& context) {
        context.setState(CompilationState::PROCESSING);
        context.startTimer("initialization");
        context.debugLog("初始化阶段开始", "StateMachine");
        context.stopTimer("initialization");
        return true;
    });
    
    registerPhaseHandler(CompilationPhase::CONFIGURATION, [](CompilationContext& context) {
        context.setState(CompilationState::PROCESSING);
        context.startTimer("configuration");
        context.debugLog("配置加载阶段", "StateMachine");
        
        // 如果有配置，应用它
        if (context.getConfiguration()) {
            context.debugLog("应用CHTL配置", "StateMachine");
        }
        
        context.stopTimer("configuration");
        return true;
    });
}

void CompilationStateMachine::registerPhaseHandler(CompilationPhase phase, StateHandler handler) {
    phaseHandlers_[phase] = handler;
}

void CompilationStateMachine::registerTransitionValidator(CompilationPhase from, CompilationPhase to, PhaseTransition validator) {
    transitionValidators_[{from, to}] = validator;
}

bool CompilationStateMachine::executePhase(CompilationContext& context, CompilationPhase phase) {
    context.setPhase(phase);
    
    auto it = phaseHandlers_.find(phase);
    if (it != phaseHandlers_.end()) {
        try {
            bool success = it->second(context);
            if (success) {
                context.setState(CompilationState::SUCCESS);
            } else {
                context.setState(CompilationState::FAILED);
                context.addError("阶段处理失败: " + std::to_string(static_cast<int>(phase)));
            }
            return success;
        } catch (const std::exception& e) {
            context.setState(CompilationState::FAILED);
            context.addError("阶段处理异常: " + std::string(e.what()));
            return false;
        }
    } else {
        context.addWarning("未找到阶段处理器: " + std::to_string(static_cast<int>(phase)));
        return true; // 默认成功
    }
}

bool CompilationStateMachine::transitionTo(CompilationContext& context, CompilationPhase targetPhase) {
    CompilationPhase currentPhase = context.getCurrentPhase();
    
    if (!isValidTransition(currentPhase, targetPhase)) {
        context.addError("无效的阶段转换: " + std::to_string(static_cast<int>(currentPhase)) + 
                        " -> " + std::to_string(static_cast<int>(targetPhase)));
        return false;
    }
    
    // 检查转换验证器
    auto validator = transitionValidators_.find({currentPhase, targetPhase});
    if (validator != transitionValidators_.end()) {
        if (!validator->second(context, currentPhase, targetPhase)) {
            context.addError("阶段转换验证失败");
            return false;
        }
    }
    
    return executePhase(context, targetPhase);
}

bool CompilationStateMachine::runFullCompilation(CompilationContext& context) {
    std::vector<CompilationPhase> phases = {
        CompilationPhase::INITIALIZATION,
        CompilationPhase::CONFIGURATION,
        CompilationPhase::SCANNING,
        CompilationPhase::SLICING,
        CompilationPhase::ROUTING,
        CompilationPhase::COMPILATION,
        CompilationPhase::MERGING,
        CompilationPhase::OPTIMIZATION,
        CompilationPhase::FINALIZATION
    };
    
    for (CompilationPhase phase : phases) {
        if (!transitionTo(context, phase)) {
            // 尝试错误恢复
            if (!handleError(context, "阶段执行失败: " + std::to_string(static_cast<int>(phase)))) {
                context.setPhase(CompilationPhase::ERROR_HANDLING);
                return false;
            }
        }
    }
    
    return true;
}

bool CompilationStateMachine::handleError(CompilationContext& context, const std::string& error) {
    context.addError(error);
    
    CompilationPhase currentPhase = context.getCurrentPhase();
    auto recovery = errorRecoveryHandlers_.find(currentPhase);
    
    if (recovery != errorRecoveryHandlers_.end()) {
        context.debugLog("尝试错误恢复", "StateMachine");
        return recovery->second(context);
    }
    
    return false; // 无法恢复
}

void CompilationStateMachine::setErrorRecoveryHandler(CompilationPhase phase, StateHandler handler) {
    errorRecoveryHandlers_[phase] = handler;
}

bool CompilationStateMachine::isValidTransition(CompilationPhase from, CompilationPhase to) const {
    // 基本的转换规则
    int fromInt = static_cast<int>(from);
    int toInt = static_cast<int>(to);
    
    // 可以向前推进或跳转到错误处理
    return toInt > fromInt || to == CompilationPhase::ERROR_HANDLING;
}

CompilationPhase CompilationStateMachine::getNextPhase(CompilationPhase current) const {
    int currentInt = static_cast<int>(current);
    int nextInt = currentInt + 1;
    
    if (nextInt <= static_cast<int>(CompilationPhase::FINALIZATION)) {
        return static_cast<CompilationPhase>(nextInt);
    }
    
    return CompilationPhase::FINALIZATION;
}

// ==================== SyntaxContextManager 实现 ====================

SyntaxContextManager::SyntaxContextManager() {
    initializeDefaultRules();
    initializeAllowedElements();
}

void SyntaxContextManager::enterContext(SyntaxContext context, CompilationContext& compilationContext) {
    compilationContext.pushSyntaxContext(context);
}

void SyntaxContextManager::exitContext(CompilationContext& compilationContext) {
    compilationContext.popSyntaxContext();
}

bool SyntaxContextManager::validateSyntax(const std::string& code, SyntaxContext context, std::vector<std::string>& errors) {
    bool isValid = true;
    
    // 根据上下文选择适当的约束器
    switch (context) {
        case SyntaxContext::GLOBAL_SCOPE: {
            CHtlGlobalStyleConstraint constraint;
            auto statements = constraint.parseGlobalStyleStatements(code);
            for (const auto& statement : statements) {
                if (!constraint.checkAllowedElement(statement)) {
                    errors.push_back("全局作用域中不允许的语法: " + statement);
                    isValid = false;
                }
            }
            break;
        }
        case SyntaxContext::LOCAL_STYLE: {
            CHtlLocalStyleConstraint constraint;
            auto statements = constraint.parseLocalStyleStatements(code);
            for (const auto& statement : statements) {
                if (!constraint.checkAllowedElement(statement)) {
                    errors.push_back("局部样式中不允许的语法: " + statement);
                    isValid = false;
                }
            }
            break;
        }
        case SyntaxContext::LOCAL_SCRIPT: {
            CHtlLocalScriptConstraint constraint;
            auto statements = constraint.parseLocalScriptStatements(code);
            for (const auto& statement : statements) {
                if (!constraint.checkAllowedElement(statement)) {
                    errors.push_back("局部脚本中不允许的语法: " + statement);
                    isValid = false;
                }
            }
            break;
        }
        default:
            // 其他上下文暂时允许所有语法
            break;
    }
    
    return isValid;
}

bool SyntaxContextManager::isAllowedInContext(const std::string& syntaxElement, SyntaxContext context) {
    auto it = allowedElements_.find(context);
    if (it != allowedElements_.end()) {
        return it->second.count(syntaxElement) > 0;
    }
    
    // 检查继承
    return inheritsRule(context, syntaxElement);
}

std::vector<std::string> SyntaxContextManager::getApplicableRules(SyntaxContext context) {
    auto it = contextRules_.find(context);
    if (it != contextRules_.end()) {
        return it->second;
    }
    return {};
}

bool SyntaxContextManager::shouldApplyRule(const std::string& rule, SyntaxContext context) {
    auto rules = getApplicableRules(context);
    return std::find(rules.begin(), rules.end(), rule) != rules.end();
}

void SyntaxContextManager::setContextInheritance(SyntaxContext child, SyntaxContext parent) {
    contextInheritance_[child] = parent;
}

std::vector<SyntaxContext> SyntaxContextManager::getInheritanceChain(SyntaxContext context) {
    std::vector<SyntaxContext> chain;
    SyntaxContext current = context;
    
    while (current != SyntaxContext::GLOBAL_SCOPE) {
        chain.push_back(current);
        auto it = contextInheritance_.find(current);
        if (it != contextInheritance_.end()) {
            current = it->second;
        } else {
            break;
        }
    }
    
    chain.push_back(SyntaxContext::GLOBAL_SCOPE);
    return chain;
}

void SyntaxContextManager::initializeDefaultRules() {
    // 全局作用域规则
    contextRules_[SyntaxContext::GLOBAL_SCOPE] = {
        "template_variables", "custom_variables", "style_groups", 
        "elements", "imports", "namespaces", "raw_embeddings"
    };
    
    // 局部样式规则
    contextRules_[SyntaxContext::LOCAL_STYLE] = {
        "template_variables", "custom_variables", "style_groups",
        "inheritance", "deletion", "raw_embeddings"
    };
    
    // 局部脚本规则
    contextRules_[SyntaxContext::LOCAL_SCRIPT] = {
        "template_variables", "variable_groups", "from_clauses", 
        "raw_embeddings", "generator_comments", "javascript_syntax"
    };
}

void SyntaxContextManager::initializeAllowedElements() {
    // 这里可以根据具体需求初始化允许的语法元素
    // 暂时留空，由具体的约束器来处理
}

bool SyntaxContextManager::inheritsRule(SyntaxContext context, const std::string& rule) {
    auto inheritanceChain = getInheritanceChain(context);
    
    for (SyntaxContext ancestorContext : inheritanceChain) {
        auto rules = getApplicableRules(ancestorContext);
        if (std::find(rules.begin(), rules.end(), rule) != rules.end()) {
            return true;
        }
    }
    
    return false;
}

} // namespace chtl
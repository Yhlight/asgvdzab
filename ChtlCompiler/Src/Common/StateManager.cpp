#include "Common/StateManager.h"
#include <algorithm>

namespace Chtl {

// StateGuard 实现
StateGuard::StateGuard(State* state, CompilerState newState) 
    : state_(state), released_(false) {
    if (state_) {
        state_->pushState(newState);
    }
}

StateGuard::~StateGuard() {
    if (!released_ && state_) {
        state_->popState();
    }
}

StateGuard::StateGuard(StateGuard&& other) noexcept 
    : state_(other.state_), released_(other.released_) {
    other.state_ = nullptr;
    other.released_ = true;
}

StateGuard& StateGuard::operator=(StateGuard&& other) noexcept {
    if (this != &other) {
        if (!released_ && state_) {
            state_->popState();
        }
        state_ = other.state_;
        released_ = other.released_;
        other.state_ = nullptr;
        other.released_ = true;
    }
    return *this;
}

void StateGuard::release() {
    released_ = true;
}

// ElementGuard 实现
ElementGuard::ElementGuard(State* state, const ElementState& element)
    : state_(state), released_(false) {
    if (state_) {
        state_->pushElement(element);
    }
}

ElementGuard::~ElementGuard() {
    if (!released_ && state_) {
        state_->popElement();
    }
}

ElementGuard::ElementGuard(ElementGuard&& other) noexcept
    : state_(other.state_), released_(other.released_) {
    other.state_ = nullptr;
    other.released_ = true;
}

ElementGuard& ElementGuard::operator=(ElementGuard&& other) noexcept {
    if (this != &other) {
        if (!released_ && state_) {
            state_->popElement();
        }
        state_ = other.state_;
        released_ = other.released_;
        other.state_ = nullptr;
        other.released_ = true;
    }
    return *this;
}

ElementState& ElementGuard::getElement() {
    return state_->getCurrentElement();
}

const ElementState& ElementGuard::getElement() const {
    return state_->getCurrentElement();
}

void ElementGuard::release() {
    released_ = true;
}

// ScopeGuard 实现
ScopeGuard::ScopeGuard(Context* context, const std::string& scopeName)
    : context_(context), released_(false) {
    if (context_) {
        context_->enterScope(scopeName);
    }
}

ScopeGuard::~ScopeGuard() {
    if (!released_ && context_) {
        context_->exitScope();
    }
}

ScopeGuard::ScopeGuard(ScopeGuard&& other) noexcept
    : context_(other.context_), released_(other.released_) {
    other.context_ = nullptr;
    other.released_ = true;
}

ScopeGuard& ScopeGuard::operator=(ScopeGuard&& other) noexcept {
    if (this != &other) {
        if (!released_ && context_) {
            context_->exitScope();
        }
        context_ = other.context_;
        released_ = other.released_;
        other.context_ = nullptr;
        other.released_ = true;
    }
    return *this;
}

void ScopeGuard::release() {
    released_ = true;
}

// StateManager 实现
StateManager::StateManager(Context* context)
    : context_(context), state_(&context->getState()) {
}

StateManager::~StateManager() {
}

StateGuard StateManager::enterState(CompilerState state) {
    if (!canEnterState(state)) {
        context_->addError("Invalid state transition to " + 
                          std::to_string(static_cast<int>(state)));
    }
    return StateGuard(state_, state);
}

ElementGuard StateManager::enterElement(const std::string& tagName) {
    ElementState element;
    element.tagName = tagName;
    element.nestLevel = state_->hasCurrentElement() ? 
                       state_->getCurrentElement().nestLevel + 1 : 0;
    return ElementGuard(state_, element);
}

ElementGuard StateManager::enterElement(const ElementState& element) {
    return ElementGuard(state_, element);
}

ScopeGuard StateManager::enterScope(const std::string& scopeName) {
    return ScopeGuard(context_, scopeName);
}

bool StateManager::isInElement() const {
    return state_->hasCurrentElement();
}

bool StateManager::isInStyleBlock() const {
    return state_->isInState(CompilerState::STYLE_BLOCK) ||
           state_->isInState(CompilerState::LOCAL_STYLE_BLOCK);
}

bool StateManager::isInScriptBlock() const {
    return state_->isInState(CompilerState::SCRIPT_BLOCK) ||
           state_->isInState(CompilerState::LOCAL_SCRIPT_BLOCK);
}

bool StateManager::isInTemplate() const {
    return state_->isInState(CompilerState::TEMPLATE_DEFINITION);
}

bool StateManager::isInCustom() const {
    return state_->isInState(CompilerState::CUSTOM_DEFINITION);
}

bool StateManager::isInLocalContext() const {
    return state_->isInState(CompilerState::LOCAL_STYLE_BLOCK) ||
           state_->isInState(CompilerState::LOCAL_SCRIPT_BLOCK);
}

std::string StateManager::getCurrentElementTag() const {
    if (state_->hasCurrentElement()) {
        return state_->getCurrentElement().tagName;
    }
    return "";
}

std::string StateManager::getCurrentElementId() const {
    if (state_->hasCurrentElement()) {
        return state_->getCurrentElement().id;
    }
    return "";
}

std::vector<std::string> StateManager::getCurrentElementClasses() const {
    if (state_->hasCurrentElement()) {
        return state_->getCurrentElement().classes;
    }
    return {};
}

bool StateManager::canEnterState(CompilerState state) const {
    CompilerState current = state_->getCurrentState();
    return validateStateTransition(current, state);
}

bool StateManager::validateStateTransition(CompilerState from, CompilerState to) const {
    // 根据CHTL语法规则验证状态转换
    switch (from) {
        case CompilerState::INITIAL:
            // 初始状态可以进入任何顶层状态
            return to == CompilerState::ELEMENT ||
                   to == CompilerState::TEMPLATE_DEFINITION ||
                   to == CompilerState::CUSTOM_DEFINITION ||
                   to == CompilerState::ORIGIN_BLOCK ||
                   to == CompilerState::CONFIGURATION ||
                   to == CompilerState::NAMESPACE_BLOCK ||
                   to == CompilerState::IMPORT_STATEMENT;
                   
        case CompilerState::ELEMENT:
            // 元素内可以有子元素、文本、样式、脚本等
            return to == CompilerState::ELEMENT ||
                   to == CompilerState::TEXT_BLOCK ||
                   to == CompilerState::LOCAL_STYLE_BLOCK ||
                   to == CompilerState::LOCAL_SCRIPT_BLOCK ||
                   to == CompilerState::ATTRIBUTE;
                   
        case CompilerState::LOCAL_STYLE_BLOCK:
            // 局部样式块内可以有CSS规则
            return to == CompilerState::CSS_RULE ||
                   to == CompilerState::STYLE_PROPERTY;
                   
        case CompilerState::TEMPLATE_DEFINITION:
            // 模板定义内部的状态
            return to == CompilerState::ELEMENT ||
                   to == CompilerState::STYLE_PROPERTY ||
                   to == CompilerState::ATTRIBUTE;
                   
        default:
            // 其他状态转换需要具体分析
            return isValidTransition(from, to);
    }
}

void StateManager::recoverToStableState() {
    // 恢复到稳定状态
    while (state_->getCurrentState() != CompilerState::INITIAL) {
        CompilerState current = state_->getCurrentState();
        if (current == CompilerState::ELEMENT ||
            current == CompilerState::TEMPLATE_DEFINITION ||
            current == CompilerState::CUSTOM_DEFINITION) {
            // 这些是相对稳定的状态
            break;
        }
        state_->popState();
    }
    
    // 清理错误状态
    state_->clearError();
    state_->resetBraceCount();
}

StateManager::StateSnapshot StateManager::captureSnapshot() const {
    StateSnapshot snapshot;
    // 这里需要访问State的内部栈，可能需要扩展State类的接口
    snapshot.cssState = state_->getCssState();
    snapshot.currentScope = context_->getCurrentScope();
    snapshot.braceCount = state_->getBraceCount();
    return snapshot;
}

void StateManager::restoreSnapshot(const StateSnapshot& snapshot) {
    // 恢复快照状态
    state_->setCssState(snapshot.cssState);
    state_->resetBraceCount();
    for (int i = 0; i < snapshot.braceCount; ++i) {
        state_->enterBrace();
    }
}

bool StateManager::isValidTransition(CompilerState from, CompilerState to) const {
    // 扩展的状态转换验证逻辑
    // 基于CHTL语法文档的规则
    
    // 任何状态都可以进入错误状态
    if (to == CompilerState::ERROR) {
        return true;
    }
    
    // 从错误状态只能回到初始状态
    if (from == CompilerState::ERROR) {
        return to == CompilerState::INITIAL;
    }
    
    // 其他特定规则...
    return false;
}

CompilerState StateManager::findStableState() const {
    CompilerState current = state_->getCurrentState();
    
    // 定义稳定状态列表
    const std::vector<CompilerState> stableStates = {
        CompilerState::INITIAL,
        CompilerState::ELEMENT,
        CompilerState::TEMPLATE_DEFINITION,
        CompilerState::CUSTOM_DEFINITION
    };
    
    // 如果当前已经是稳定状态
    if (std::find(stableStates.begin(), stableStates.end(), current) != stableStates.end()) {
        return current;
    }
    
    // 否则返回初始状态
    return CompilerState::INITIAL;
}

// ParseContextHelper 实现
ParseContextHelper::ParseContextHelper(StateManager* stateManager, Context* context)
    : stateManager_(stateManager), context_(context) {
}

StateGuard ParseContextHelper::enterTextBlock() {
    return stateManager_->enterState(CompilerState::TEXT_BLOCK);
}

StateGuard ParseContextHelper::enterStyleBlock(bool isLocal) {
    CompilerState state = isLocal ? 
        CompilerState::LOCAL_STYLE_BLOCK : 
        CompilerState::STYLE_BLOCK;
    
    auto guard = stateManager_->enterState(state);
    
    if (isLocal && stateManager_->isInElement()) {
        // 标记元素有局部样式
        auto& element = context_->getState().getCurrentElement();
        element.hasLocalStyle = true;
    }
    
    return guard;
}

StateGuard ParseContextHelper::enterScriptBlock(bool isLocal) {
    CompilerState state = isLocal ? 
        CompilerState::LOCAL_SCRIPT_BLOCK : 
        CompilerState::SCRIPT_BLOCK;
    
    auto guard = stateManager_->enterState(state);
    
    if (isLocal && stateManager_->isInElement()) {
        // 标记元素有局部脚本
        auto& element = context_->getState().getCurrentElement();
        element.hasLocalScript = true;
    }
    
    return guard;
}

StateGuard ParseContextHelper::enterTemplate(const std::string& type) {
    auto guard = stateManager_->enterState(CompilerState::TEMPLATE_DEFINITION);
    context_->setTempData("template_type", type);
    return guard;
}

StateGuard ParseContextHelper::enterCustom(const std::string& type) {
    auto guard = stateManager_->enterState(CompilerState::CUSTOM_DEFINITION);
    context_->setTempData("custom_type", type);
    return guard;
}

StateGuard ParseContextHelper::enterOriginBlock(const std::string& type) {
    auto guard = stateManager_->enterState(CompilerState::ORIGIN_BLOCK);
    context_->setTempData("origin_type", type);
    return guard;
}

void ParseContextHelper::beginCSSRule(const std::string& selector) {
    auto& cssState = context_->getState().getCssState();
    cssState.currentSelector = selector;
    cssState.inRule = true;
}

void ParseContextHelper::endCSSRule() {
    auto& cssState = context_->getState().getCssState();
    cssState.currentSelector.clear();
    cssState.inRule = false;
}

void ParseContextHelper::setAutoClassName(const std::string& className) {
    auto& cssState = context_->getState().getCssState();
    cssState.autoClassName = className;
}

std::string ParseContextHelper::generateAutoClassIfNeeded() {
    if (stateManager_->isInLocalContext() && stateManager_->isInElement()) {
        auto& state = context_->getState();
        std::string autoClass = state.generateAutoClassName();
        state.addGeneratedClass(autoClass);
        
        // 添加到当前元素的类列表
        if (state.hasCurrentElement()) {
            auto& element = state.getCurrentElement();
            element.classes.push_back(autoClass);
        }
        
        return autoClass;
    }
    return "";
}

void ParseContextHelper::markChtlJsSyntax() {
    context_->setTempData("has_chtl_js", "true");
}

bool ParseContextHelper::hasChtlJsSyntax() const {
    return context_->hasTempData("has_chtl_js");
}

void ParseContextHelper::registerTemplate(const std::string& type, const std::string& name) {
    // 根据类型注册到全局符号表
    auto& globalMap = context_->getGlobalMap();
    
    if (type == "@Style") {
        StyleGroup style;
        style.name = name;
        style.isTemplate = true;
        globalMap.addStyleGroup(style);
    } else if (type == "@Element") {
        ElementGroup element;
        element.name = name;
        element.isTemplate = true;
        globalMap.addElementGroup(element);
    } else if (type == "@Var") {
        VarGroup var;
        var.name = name;
        var.isTemplate = true;
        globalMap.addVarGroup(var);
    }
}

void ParseContextHelper::registerCustom(const std::string& type, const std::string& name) {
    // 类似registerTemplate，但标记为自定义
    auto& globalMap = context_->getGlobalMap();
    
    if (type == "@Style") {
        StyleGroup style;
        style.name = name;
        style.isCustom = true;
        globalMap.addStyleGroup(style);
    } else if (type == "@Element") {
        ElementGroup element;
        element.name = name;
        element.isCustom = true;
        globalMap.addElementGroup(element);
    } else if (type == "@Var") {
        VarGroup var;
        var.name = name;
        var.isCustom = true;
        globalMap.addVarGroup(var);
    }
}

bool ParseContextHelper::isInheritanceContext() const {
    return context_->hasTempData("in_inheritance");
}

void ParseContextHelper::reportError(const std::string& message, const Token& token) {
    context_->addError(message, token.line, token.column);
}

void ParseContextHelper::reportWarning(const std::string& message, const Token& token) {
    context_->addWarning(message, token.line, token.column);
}

void ParseContextHelper::updatePosition(const Token& token) {
    context_->setCurrentPosition(token.line, token.column);
}

// GenerateContextHelper 实现
GenerateContextHelper::GenerateContextHelper(Context* context)
    : context_(context), currentTarget_(OutputTarget::HTML) {
}

void GenerateContextHelper::setOutputTarget(OutputTarget target) {
    currentTarget_ = target;
}

GenerateContextHelper::OutputTarget GenerateContextHelper::getCurrentTarget() const {
    return currentTarget_;
}

bool GenerateContextHelper::shouldGenerateAutoClass() const {
    const auto& state = context_->getState();
    return state.hasCurrentElement() && 
           state.getCurrentElement().hasLocalStyle &&
           !state.getCssState().autoClassName.empty();
}

std::string GenerateContextHelper::getCurrentAutoClass() const {
    return context_->getState().getCssState().autoClassName;
}

void GenerateContextHelper::processLocalStyleContext(const std::string& selector) {
    if (selector.find("&") != std::string::npos) {
        // 需要替换上下文符号
        std::string autoClass = getCurrentAutoClass();
        if (!autoClass.empty()) {
            // 执行替换逻辑
            context_->setTempData("processed_selector", selector);
        }
    }
}

bool GenerateContextHelper::isTemplateExpansion() const {
    return !templateExpansionStack_.empty();
}

void GenerateContextHelper::beginTemplateExpansion(const std::string& templateName) {
    templateExpansionStack_.push(templateName);
}

void GenerateContextHelper::endTemplateExpansion() {
    if (!templateExpansionStack_.empty()) {
        templateExpansionStack_.pop();
    }
}

std::string GenerateContextHelper::resolveVariable(const std::string& groupName, 
                                                  const std::string& varName) {
    auto varGroup = context_->getGlobalMap().getVarGroup(groupName);
    if (varGroup.has_value()) {
        auto it = varGroup->variables.find(varName);
        if (it != varGroup->variables.end()) {
            return it->second;
        }
    }
    return "";
}

bool GenerateContextHelper::hasVariable(const std::string& groupName, 
                                       const std::string& varName) const {
    auto varGroup = context_->getGlobalMap().getVarGroup(groupName);
    if (varGroup.has_value()) {
        return varGroup->variables.find(varName) != varGroup->variables.end();
    }
    return false;
}

GenerateContextHelper::OutputTarget GenerateContextHelper::determineOriginTarget(
    const std::string& originType) const {
    
    if (originType == "@Html") {
        return OutputTarget::HTML;
    } else if (originType == "@Style") {
        return OutputTarget::CSS;
    } else if (originType == "@JavaScript") {
        return OutputTarget::JAVASCRIPT;
    } else {
        // 查找自定义原始类型映射
        const auto& config = context_->getGlobalMap().getConfiguration();
        auto it = config.originTypes.find(originType);
        if (it != config.originTypes.end()) {
            return determineOriginTarget(it->second);
        }
    }
    
    return OutputTarget::HTML; // 默认
}

} // namespace Chtl
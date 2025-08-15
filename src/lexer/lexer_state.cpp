#include "lexer/lexer_state.hpp"
#include <algorithm>
#include <sstream>

namespace chtl {

// LexerStateManager 实现
LexerStateManager::LexerStateManager() {
    // 初始化状态栈，推入初始状态
    stateStack_.push(LexerContext(LexerStateType::INITIAL));
}

void LexerStateManager::pushState(LexerStateType state, const std::string& blockType, 
                                 const std::string& blockName) {
    LexerContext context(state, blockType, blockName);
    stateStack_.push(context);
}

bool LexerStateManager::popState() {
    if (stateStack_.size() <= 1) {
        return false; // 不能弹出初始状态
    }
    stateStack_.pop();
    return true;
}

LexerStateType LexerStateManager::getCurrentState() const {
    if (stateStack_.empty()) {
        return LexerStateType::ERROR_STATE;
    }
    return stateStack_.top().state;
}

const LexerContext& LexerStateManager::getCurrentContext() const {
    static LexerContext errorContext(LexerStateType::ERROR_STATE);
    if (stateStack_.empty()) {
        return errorContext;
    }
    return stateStack_.top();
}

LexerContext& LexerStateManager::getCurrentContext() {
    static LexerContext errorContext(LexerStateType::ERROR_STATE);
    if (stateStack_.empty()) {
        return errorContext;
    }
    return const_cast<LexerContext&>(stateStack_.top());
}

bool LexerStateManager::isInState(LexerStateType state) const {
    return getCurrentState() == state;
}

bool LexerStateManager::isInAnyState(const std::vector<LexerStateType>& states) const {
    LexerStateType current = getCurrentState();
    return std::find(states.begin(), states.end(), current) != states.end();
}

size_t LexerStateManager::getStackDepth() const {
    return stateStack_.size();
}

// 括号管理
void LexerStateManager::incrementBraceDepth() {
    if (!stateStack_.empty()) {
        const_cast<LexerContext&>(stateStack_.top()).braceDepth++;
    }
}

void LexerStateManager::decrementBraceDepth() {
    if (!stateStack_.empty() && stateStack_.top().braceDepth > 0) {
        const_cast<LexerContext&>(stateStack_.top()).braceDepth--;
    }
}

void LexerStateManager::incrementBracketDepth() {
    if (!stateStack_.empty()) {
        const_cast<LexerContext&>(stateStack_.top()).bracketDepth++;
    }
}

void LexerStateManager::decrementBracketDepth() {
    if (!stateStack_.empty() && stateStack_.top().bracketDepth > 0) {
        const_cast<LexerContext&>(stateStack_.top()).bracketDepth--;
    }
}

void LexerStateManager::incrementParenDepth() {
    if (!stateStack_.empty()) {
        const_cast<LexerContext&>(stateStack_.top()).parenDepth++;
    }
}

void LexerStateManager::decrementParenDepth() {
    if (!stateStack_.empty() && stateStack_.top().parenDepth > 0) {
        const_cast<LexerContext&>(stateStack_.top()).parenDepth--;
    }
}

size_t LexerStateManager::getBraceDepth() const {
    return stateStack_.empty() ? 0 : stateStack_.top().braceDepth;
}

size_t LexerStateManager::getBracketDepth() const {
    return stateStack_.empty() ? 0 : stateStack_.top().bracketDepth;
}

size_t LexerStateManager::getParenDepth() const {
    return stateStack_.empty() ? 0 : stateStack_.top().parenDepth;
}

// 状态查询
bool LexerStateManager::isInString() const {
    return StateUtils::isStringState(getCurrentState());
}

bool LexerStateManager::isInComment() const {
    return StateUtils::isCommentState(getCurrentState());
}

bool LexerStateManager::isInCHTLBlock() const {
    return isInState(LexerStateType::IN_CHTL_BLOCK) ||
           isInState(LexerStateType::IN_TEXT_BLOCK) ||
           isInState(LexerStateType::IN_ELEMENT_BODY) ||
           isInState(LexerStateType::IN_ATTRIBUTE_LIST);
}

bool LexerStateManager::isInCHTLJSBlock() const {
    return isInState(LexerStateType::IN_CHTL_JS_BLOCK) ||
           isInState(LexerStateType::IN_SCRIPT_BLOCK) ||
           isInState(LexerStateType::IN_ENHANCED_SELECTOR);
}

bool LexerStateManager::isInCSSBlock() const {
    return isInState(LexerStateType::IN_CSS_BLOCK) ||
           isInState(LexerStateType::IN_STYLE_BLOCK);
}

bool LexerStateManager::isInJavaScriptBlock() const {
    return isInState(LexerStateType::IN_JAVASCRIPT_BLOCK);
}

bool LexerStateManager::isInSpecialBlock() const {
    return isInState(LexerStateType::IN_TEMPLATE_BLOCK) ||
           isInState(LexerStateType::IN_CUSTOM_BLOCK) ||
           isInState(LexerStateType::IN_ORIGIN_BLOCK) ||
           isInState(LexerStateType::IN_IMPORT_BLOCK) ||
           isInState(LexerStateType::IN_NAMESPACE_BLOCK) ||
           isInState(LexerStateType::IN_CONFIGURATION_BLOCK);
}

bool LexerStateManager::isExpectingValue() const {
    return stateStack_.empty() ? false : stateStack_.top().expectingValue;
}

void LexerStateManager::setExpectingValue(bool expecting) {
    if (!stateStack_.empty()) {
        const_cast<LexerContext&>(stateStack_.top()).expectingValue = expecting;
    }
}

bool LexerStateManager::isInSpecialization() const {
    return stateStack_.empty() ? false : stateStack_.top().inSpecialization;
}

void LexerStateManager::setInSpecialization(bool inSpec) {
    if (!stateStack_.empty()) {
        const_cast<LexerContext&>(stateStack_.top()).inSpecialization = inSpec;
    }
}

// 状态转换
bool LexerStateManager::transitionOnToken(const CHTLToken& token) {
    LexerStateType currentState = getCurrentState();
    
    switch (token.type) {
        case CHTLTokenType::KEYWORD_TEMPLATE:
            if (currentState == LexerStateType::INITIAL) {
                pushState(LexerStateType::IN_TEMPLATE_BLOCK);
                return true;
            }
            break;
            
        case CHTLTokenType::KEYWORD_CUSTOM:
            if (currentState == LexerStateType::INITIAL) {
                pushState(LexerStateType::IN_CUSTOM_BLOCK);
                return true;
            }
            break;
            
        case CHTLTokenType::KEYWORD_ORIGIN:
            if (currentState == LexerStateType::INITIAL) {
                pushState(LexerStateType::IN_ORIGIN_BLOCK);
                return true;
            }
            break;
            
        case CHTLTokenType::KEYWORD_IMPORT:
            if (currentState == LexerStateType::INITIAL) {
                pushState(LexerStateType::IN_IMPORT_BLOCK);
                return true;
            }
            break;
            
        case CHTLTokenType::KEYWORD_NAMESPACE:
            if (currentState == LexerStateType::INITIAL) {
                pushState(LexerStateType::IN_NAMESPACE_BLOCK);
                return true;
            }
            break;
            
        case CHTLTokenType::KEYWORD_CONFIGURATION:
            if (currentState == LexerStateType::INITIAL) {
                pushState(LexerStateType::IN_CONFIGURATION_BLOCK);
                return true;
            }
            break;
            
        case CHTLTokenType::KEYWORD_TEXT:
            if (isInCHTLBlock()) {
                pushState(LexerStateType::IN_TEXT_BLOCK);
                return true;
            }
            break;
            
        case CHTLTokenType::KEYWORD_STYLE:
            if (isInCHTLBlock()) {
                pushState(LexerStateType::IN_STYLE_BLOCK);
                return true;
            }
            break;
            
        case CHTLTokenType::KEYWORD_SCRIPT:
            if (isInCHTLBlock()) {
                pushState(LexerStateType::IN_SCRIPT_BLOCK);
                return true;
            }
            break;
            
        case CHTLTokenType::LBRACE:
            incrementBraceDepth();
            if (currentState == LexerStateType::INITIAL) {
                pushState(LexerStateType::IN_CHTL_BLOCK);
                return true;
            }
            break;
            
        case CHTLTokenType::RBRACE:
            decrementBraceDepth();
            if (getBraceDepth() == 0 && StateUtils::isBlockState(currentState)) {
                popState();
                return true;
            }
            break;
            
        case CHTLTokenType::COLON:
        case CHTLTokenType::EQUALS:
            setExpectingValue(true);
            break;
            
        case CHTLTokenType::SEMICOLON:
            setExpectingValue(false);
            break;
            
        default:
            break;
    }
    
    return false;
}

bool LexerStateManager::transitionOnCHTLJSToken(const CHTLJSToken& token) {
    LexerStateType currentState = getCurrentState();
    
    switch (token.type) {
        case CHTLJSTokenType::ENHANCED_SELECTOR_START:
            if (isInCHTLJSBlock()) {
                pushState(LexerStateType::IN_ENHANCED_SELECTOR);
                return true;
            }
            break;
            
        case CHTLJSTokenType::ENHANCED_SELECTOR_END:
            if (currentState == LexerStateType::IN_ENHANCED_SELECTOR) {
                popState();
                return true;
            }
            break;
            
        case CHTLJSTokenType::LBRACE:
            incrementBraceDepth();
            break;
            
        case CHTLJSTokenType::RBRACE:
            decrementBraceDepth();
            if (getBraceDepth() == 0 && StateUtils::isBlockState(currentState)) {
                popState();
                return true;
            }
            break;
            
        default:
            break;
    }
    
    return false;
}

bool LexerStateManager::transitionOnCharacter(char ch, Position pos) {
    LexerStateType currentState = getCurrentState();
    
    switch (ch) {
        case '"':
            if (currentState == LexerStateType::IN_STRING_DOUBLE) {
                popState();
                return true;
            } else if (!isInString() && !isInComment()) {
                pushState(LexerStateType::IN_STRING_DOUBLE);
                return true;
            }
            break;
            
        case '\'':
            if (currentState == LexerStateType::IN_STRING_SINGLE) {
                popState();
                return true;
            } else if (!isInString() && !isInComment()) {
                pushState(LexerStateType::IN_STRING_SINGLE);
                return true;
            }
            break;
            
        case '/':
            if (!isInString() && !isInComment()) {
                // 可能是注释开始，需要下一个字符确认
                return true;
            }
            break;
            
        case '\n':
            if (currentState == LexerStateType::IN_COMMENT_LINE) {
                popState();
                return true;
            }
            break;
            
        default:
            break;
    }
    
    return false;
}

// 错误处理
void LexerStateManager::setErrorState(const std::string& error, Position pos) {
    errorMessage_ = error;
    errorPosition_ = pos;
    pushState(LexerStateType::ERROR_STATE);
}

bool LexerStateManager::isInErrorState() const {
    return getCurrentState() == LexerStateType::ERROR_STATE;
}

const std::string& LexerStateManager::getErrorMessage() const {
    return errorMessage_;
}

Position LexerStateManager::getErrorPosition() const {
    return errorPosition_;
}

void LexerStateManager::clearError() {
    errorMessage_.clear();
    if (isInErrorState()) {
        popState();
    }
}

// 调试和诊断
std::string LexerStateManager::getStackTrace() const {
    std::ostringstream oss;
    std::stack<LexerContext> temp = stateStack_;
    std::vector<LexerContext> contexts;
    
    while (!temp.empty()) {
        contexts.push_back(temp.top());
        temp.pop();
    }
    
    for (int i = contexts.size() - 1; i >= 0; --i) {
        oss << stateToString(contexts[i].state);
        if (!contexts[i].blockType.empty()) {
            oss << "(" << contexts[i].blockType;
            if (!contexts[i].blockName.empty()) {
                oss << " " << contexts[i].blockName;
            }
            oss << ")";
        }
        if (i > 0) oss << " -> ";
    }
    
    return oss.str();
}

std::string LexerStateManager::getCurrentStateInfo() const {
    const LexerContext& context = getCurrentContext();
    std::ostringstream oss;
    
    oss << "State: " << stateToString(context.state) << "\n";
    oss << "Block Type: " << context.blockType << "\n";
    oss << "Block Name: " << context.blockName << "\n";
    oss << "Brace Depth: " << context.braceDepth << "\n";
    oss << "Bracket Depth: " << context.bracketDepth << "\n";
    oss << "Paren Depth: " << context.parenDepth << "\n";
    oss << "Expecting Value: " << (context.expectingValue ? "true" : "false") << "\n";
    oss << "In Specialization: " << (context.inSpecialization ? "true" : "false") << "\n";
    
    return oss.str();
}

void LexerStateManager::reset() {
    // 清空状态栈
    while (!stateStack_.empty()) {
        stateStack_.pop();
    }
    
    // 重新添加初始状态
    stateStack_.push(LexerContext(LexerStateType::INITIAL));
    
    // 清除错误信息
    errorMessage_.clear();
}

bool LexerStateManager::validateState() const {
    if (stateStack_.empty()) {
        return false;
    }
    
    // 检查括号平衡
    const LexerContext& context = getCurrentContext();
    if (context.braceDepth < 0 || context.bracketDepth < 0 || context.parenDepth < 0) {
        return false;
    }
    
    // 检查状态转换的有效性
    if (stateStack_.size() > 1) {
        std::stack<LexerContext> temp = stateStack_;
        temp.pop(); // 移除当前状态
        LexerStateType previousState = temp.top().state;
        LexerStateType currentState = getCurrentState();
        
        if (!isValidTransition(previousState, currentState)) {
            return false;
        }
    }
    
    return true;
}

// 私有方法实现
LexerStateType LexerStateManager::determineBlockState(const std::string& blockType) const {
    if (blockType == "@Style") {
        return LexerStateType::IN_CSS_BLOCK;
    } else if (blockType == "@JavaScript") {
        return LexerStateType::IN_JAVASCRIPT_BLOCK;
    } else if (blockType == "@Element") {
        return LexerStateType::IN_CHTL_BLOCK;
    }
    
    return LexerStateType::IN_CHTL_BLOCK; // 默认
}

bool LexerStateManager::isValidTransition(LexerStateType from, LexerStateType to) const {
    // 定义有效的状态转换
    switch (from) {
        case LexerStateType::INITIAL:
            return to == LexerStateType::IN_CHTL_BLOCK ||
                   to == LexerStateType::IN_TEMPLATE_BLOCK ||
                   to == LexerStateType::IN_CUSTOM_BLOCK ||
                   to == LexerStateType::IN_ORIGIN_BLOCK ||
                   to == LexerStateType::IN_IMPORT_BLOCK ||
                   to == LexerStateType::IN_NAMESPACE_BLOCK ||
                   to == LexerStateType::IN_CONFIGURATION_BLOCK;
                   
        case LexerStateType::IN_CHTL_BLOCK:
            return to == LexerStateType::IN_TEXT_BLOCK ||
                   to == LexerStateType::IN_STYLE_BLOCK ||
                   to == LexerStateType::IN_SCRIPT_BLOCK ||
                   to == LexerStateType::IN_ELEMENT_BODY ||
                   to == LexerStateType::IN_ATTRIBUTE_LIST;
                   
        case LexerStateType::IN_SCRIPT_BLOCK:
            return to == LexerStateType::IN_CHTL_JS_BLOCK ||
                   to == LexerStateType::IN_ENHANCED_SELECTOR;
                   
        default:
            return true; // 大多数转换都是有效的
    }
}

void LexerStateManager::handleSpecialKeyword(const std::string& keyword) {
    if (keyword == "delete") {
        setInSpecialization(true);
    } else if (keyword == "insert") {
        setInSpecialization(true);
    } else if (keyword == "replace") {
        setInSpecialization(true);
    }
}

std::string LexerStateManager::stateToString(LexerStateType state) const {
    switch (state) {
        case LexerStateType::INITIAL: return "INITIAL";
        case LexerStateType::IN_CHTL_BLOCK: return "IN_CHTL_BLOCK";
        case LexerStateType::IN_CHTL_JS_BLOCK: return "IN_CHTL_JS_BLOCK";
        case LexerStateType::IN_CSS_BLOCK: return "IN_CSS_BLOCK";
        case LexerStateType::IN_JAVASCRIPT_BLOCK: return "IN_JAVASCRIPT_BLOCK";
        case LexerStateType::IN_TEXT_BLOCK: return "IN_TEXT_BLOCK";
        case LexerStateType::IN_STYLE_BLOCK: return "IN_STYLE_BLOCK";
        case LexerStateType::IN_SCRIPT_BLOCK: return "IN_SCRIPT_BLOCK";
        case LexerStateType::IN_TEMPLATE_BLOCK: return "IN_TEMPLATE_BLOCK";
        case LexerStateType::IN_CUSTOM_BLOCK: return "IN_CUSTOM_BLOCK";
        case LexerStateType::IN_ORIGIN_BLOCK: return "IN_ORIGIN_BLOCK";
        case LexerStateType::IN_IMPORT_BLOCK: return "IN_IMPORT_BLOCK";
        case LexerStateType::IN_NAMESPACE_BLOCK: return "IN_NAMESPACE_BLOCK";
        case LexerStateType::IN_CONFIGURATION_BLOCK: return "IN_CONFIGURATION_BLOCK";
        case LexerStateType::IN_STRING_DOUBLE: return "IN_STRING_DOUBLE";
        case LexerStateType::IN_STRING_SINGLE: return "IN_STRING_SINGLE";
        case LexerStateType::IN_STRING_UNQUOTED: return "IN_STRING_UNQUOTED";
        case LexerStateType::IN_COMMENT_LINE: return "IN_COMMENT_LINE";
        case LexerStateType::IN_COMMENT_MULTILINE: return "IN_COMMENT_MULTILINE";
        case LexerStateType::IN_COMMENT_GENERATOR: return "IN_COMMENT_GENERATOR";
        case LexerStateType::IN_ENHANCED_SELECTOR: return "IN_ENHANCED_SELECTOR";
        case LexerStateType::IN_ATTRIBUTE_LIST: return "IN_ATTRIBUTE_LIST";
        case LexerStateType::IN_ELEMENT_BODY: return "IN_ELEMENT_BODY";
        case LexerStateType::ERROR_STATE: return "ERROR_STATE";
        default: return "UNKNOWN";
    }
}

// StateUtils 实现
bool StateUtils::isBlockState(LexerStateType state) {
    switch (state) {
        case LexerStateType::IN_CHTL_BLOCK:
        case LexerStateType::IN_CHTL_JS_BLOCK:
        case LexerStateType::IN_CSS_BLOCK:
        case LexerStateType::IN_JAVASCRIPT_BLOCK:
        case LexerStateType::IN_TEXT_BLOCK:
        case LexerStateType::IN_STYLE_BLOCK:
        case LexerStateType::IN_SCRIPT_BLOCK:
        case LexerStateType::IN_TEMPLATE_BLOCK:
        case LexerStateType::IN_CUSTOM_BLOCK:
        case LexerStateType::IN_ORIGIN_BLOCK:
        case LexerStateType::IN_IMPORT_BLOCK:
        case LexerStateType::IN_NAMESPACE_BLOCK:
        case LexerStateType::IN_CONFIGURATION_BLOCK:
        case LexerStateType::IN_ELEMENT_BODY:
            return true;
        default:
            return false;
    }
}

bool StateUtils::isStringState(LexerStateType state) {
    return state == LexerStateType::IN_STRING_DOUBLE ||
           state == LexerStateType::IN_STRING_SINGLE ||
           state == LexerStateType::IN_STRING_UNQUOTED;
}

bool StateUtils::isCommentState(LexerStateType state) {
    return state == LexerStateType::IN_COMMENT_LINE ||
           state == LexerStateType::IN_COMMENT_MULTILINE ||
           state == LexerStateType::IN_COMMENT_GENERATOR;
}

int StateUtils::getStatePriority(LexerStateType state) {
    switch (state) {
        case LexerStateType::ERROR_STATE: return 100;
        case LexerStateType::IN_STRING_DOUBLE:
        case LexerStateType::IN_STRING_SINGLE:
        case LexerStateType::IN_STRING_UNQUOTED: return 90;
        case LexerStateType::IN_COMMENT_LINE:
        case LexerStateType::IN_COMMENT_MULTILINE:
        case LexerStateType::IN_COMMENT_GENERATOR: return 80;
        case LexerStateType::IN_ENHANCED_SELECTOR: return 70;
        default: return 50;
    }
}

bool StateUtils::allowsNesting(LexerStateType state) {
    switch (state) {
        case LexerStateType::IN_STRING_DOUBLE:
        case LexerStateType::IN_STRING_SINGLE:
        case LexerStateType::IN_STRING_UNQUOTED:
        case LexerStateType::IN_COMMENT_LINE:
        case LexerStateType::IN_COMMENT_MULTILINE:
        case LexerStateType::IN_COMMENT_GENERATOR:
        case LexerStateType::ERROR_STATE:
            return false;
        default:
            return true;
    }
}

CodeSegmentType StateUtils::getCodeSegmentType(LexerStateType state) {
    switch (state) {
        case LexerStateType::IN_CHTL_BLOCK:
        case LexerStateType::IN_TEXT_BLOCK:
        case LexerStateType::IN_ELEMENT_BODY:
        case LexerStateType::IN_ATTRIBUTE_LIST:
            return CodeSegmentType::CHTL;
        case LexerStateType::IN_CHTL_JS_BLOCK:
        case LexerStateType::IN_SCRIPT_BLOCK:
        case LexerStateType::IN_ENHANCED_SELECTOR:
            return CodeSegmentType::CHTL_JS;
        case LexerStateType::IN_CSS_BLOCK:
        case LexerStateType::IN_STYLE_BLOCK:
            return CodeSegmentType::CSS;
        case LexerStateType::IN_JAVASCRIPT_BLOCK:
            return CodeSegmentType::JAVASCRIPT;
        case LexerStateType::IN_TEMPLATE_BLOCK:
            return CodeSegmentType::TEMPLATE;
        case LexerStateType::IN_CUSTOM_BLOCK:
            return CodeSegmentType::CUSTOM;
        case LexerStateType::IN_ORIGIN_BLOCK:
            return CodeSegmentType::ORIGIN;
        case LexerStateType::IN_IMPORT_BLOCK:
            return CodeSegmentType::IMPORT;
        case LexerStateType::IN_NAMESPACE_BLOCK:
            return CodeSegmentType::NAMESPACE;
        case LexerStateType::IN_CONFIGURATION_BLOCK:
            return CodeSegmentType::CONFIGURATION;
        default:
            return CodeSegmentType::UNKNOWN;
    }
}

LexerStateType StateUtils::getStateFromCodeSegmentType(CodeSegmentType type) {
    switch (type) {
        case CodeSegmentType::CHTL:
            return LexerStateType::IN_CHTL_BLOCK;
        case CodeSegmentType::CHTL_JS:
            return LexerStateType::IN_CHTL_JS_BLOCK;
        case CodeSegmentType::CSS:
            return LexerStateType::IN_CSS_BLOCK;
        case CodeSegmentType::JAVASCRIPT:
            return LexerStateType::IN_JAVASCRIPT_BLOCK;
        case CodeSegmentType::TEMPLATE:
            return LexerStateType::IN_TEMPLATE_BLOCK;
        case CodeSegmentType::CUSTOM:
            return LexerStateType::IN_CUSTOM_BLOCK;
        case CodeSegmentType::ORIGIN:
            return LexerStateType::IN_ORIGIN_BLOCK;
        case CodeSegmentType::IMPORT:
            return LexerStateType::IN_IMPORT_BLOCK;
        case CodeSegmentType::NAMESPACE:
            return LexerStateType::IN_NAMESPACE_BLOCK;
        case CodeSegmentType::CONFIGURATION:
            return LexerStateType::IN_CONFIGURATION_BLOCK;
        default:
            return LexerStateType::INITIAL;
    }
}

// StateListenerManager 实现
void StateListenerManager::addListener(std::shared_ptr<IStateListener> listener) {
    listeners_.push_back(listener);
}

void StateListenerManager::removeListener(std::shared_ptr<IStateListener> listener) {
    listeners_.erase(
        std::remove(listeners_.begin(), listeners_.end(), listener),
        listeners_.end()
    );
}

void StateListenerManager::notifyStateEntered(LexerStateType state, const LexerContext& context) {
    for (auto& listener : listeners_) {
        if (listener) {
            listener->onStateEntered(state, context);
        }
    }
}

void StateListenerManager::notifyStateExited(LexerStateType state, const LexerContext& context) {
    for (auto& listener : listeners_) {
        if (listener) {
            listener->onStateExited(state, context);
        }
    }
}

void StateListenerManager::notifyStateTransition(LexerStateType from, LexerStateType to, const LexerContext& context) {
    for (auto& listener : listeners_) {
        if (listener) {
            listener->onStateTransition(from, to, context);
        }
    }
}

void StateListenerManager::notifyError(const std::string& error, Position pos) {
    for (auto& listener : listeners_) {
        if (listener) {
            listener->onError(error, pos);
        }
    }
}

} // namespace chtl
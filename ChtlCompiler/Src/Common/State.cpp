#include "Common/State.h"
#include <iostream>
#include <sstream>

namespace Chtl {

State::State() 
    : braceCount_(0),
      autoClassCounter_(0),
      stringDelimiter_('\0'),
      commentType_(CommentType::NONE) {
    // 初始状态栈
    stateStack_.push(CompilerState::INITIAL);
}

State::~State() {
}

void State::pushState(CompilerState state) {
    stateStack_.push(state);
}

void State::popState() {
    if (stateStack_.size() > 1) { // 保留至少一个状态
        stateStack_.pop();
    }
}

CompilerState State::getCurrentState() const {
    return stateStack_.empty() ? CompilerState::INITIAL : stateStack_.top();
}

bool State::isInState(CompilerState state) const {
    std::stack<CompilerState> temp = stateStack_;
    while (!temp.empty()) {
        if (temp.top() == state) {
            return true;
        }
        temp.pop();
    }
    return false;
}

void State::pushElement(const ElementState& element) {
    elementStack_.push(element);
}

void State::popElement() {
    if (!elementStack_.empty()) {
        elementStack_.pop();
    }
}

ElementState& State::getCurrentElement() {
    static ElementState dummy{};
    return elementStack_.empty() ? dummy : elementStack_.top();
}

const ElementState& State::getCurrentElement() const {
    static const ElementState dummy{};
    return elementStack_.empty() ? dummy : elementStack_.top();
}

bool State::hasCurrentElement() const {
    return !elementStack_.empty();
}

void State::pushParen(char paren) {
    if (paren == '(' || paren == '[' || paren == '{') {
        parenStack_.push(paren);
    }
}

bool State::popParen(char paren) {
    if (parenStack_.empty()) {
        return false;
    }
    
    char expected = '\0';
    switch (paren) {
        case ')': expected = '('; break;
        case ']': expected = '['; break;
        case '}': expected = '{'; break;
        default: return false;
    }
    
    if (parenStack_.top() == expected) {
        parenStack_.pop();
        return true;
    }
    
    return false;
}

void State::setError(const std::string& message) {
    errorMessage_ = message;
    pushState(CompilerState::ERROR);
}

std::string State::generateAutoClassName() {
    std::stringstream ss;
    ss << "chtl-auto-" << autoClassCounter_++;
    std::string className = ss.str();
    generatedClasses_.insert(className);
    return className;
}

void State::addGeneratedClass(const std::string& className) {
    generatedClasses_.insert(className);
}

bool State::isGeneratedClass(const std::string& className) const {
    return generatedClasses_.find(className) != generatedClasses_.end();
}

void State::reset() {
    // 清空所有栈
    while (!stateStack_.empty()) {
        stateStack_.pop();
    }
    stateStack_.push(CompilerState::INITIAL);
    
    while (!elementStack_.empty()) {
        elementStack_.pop();
    }
    
    while (!parenStack_.empty()) {
        parenStack_.pop();
    }
    
    // 重置其他状态
    cssState_ = CssState();
    braceCount_ = 0;
    autoClassCounter_ = 0;
    stringDelimiter_ = '\0';
    commentType_ = CommentType::NONE;
    generatedClasses_.clear();
    errorMessage_.clear();
}

void State::dump() const {
    std::cout << "=== State Dump ===\n";
    
    // 输出状态栈
    std::cout << "State Stack:\n";
    std::stack<CompilerState> tempStates = stateStack_;
    std::vector<CompilerState> states;
    while (!tempStates.empty()) {
        states.push_back(tempStates.top());
        tempStates.pop();
    }
    
    for (auto it = states.rbegin(); it != states.rend(); ++it) {
        std::cout << "  ";
        switch (*it) {
            case CompilerState::INITIAL: std::cout << "INITIAL"; break;
            case CompilerState::IN_ELEMENT: std::cout << "IN_ELEMENT"; break;
            case CompilerState::IN_ATTRIBUTE: std::cout << "IN_ATTRIBUTE"; break;
            case CompilerState::IN_TEXT: std::cout << "IN_TEXT"; break;
            case CompilerState::IN_STYLE: std::cout << "IN_STYLE"; break;
            case CompilerState::IN_SCRIPT: std::cout << "IN_SCRIPT"; break;
            case CompilerState::IN_TEMPLATE: std::cout << "IN_TEMPLATE"; break;
            case CompilerState::IN_CUSTOM: std::cout << "IN_CUSTOM"; break;
            case CompilerState::IN_ORIGIN: std::cout << "IN_ORIGIN"; break;
            case CompilerState::IN_NAMESPACE: std::cout << "IN_NAMESPACE"; break;
            case CompilerState::IN_CONFIGURATION: std::cout << "IN_CONFIGURATION"; break;
            case CompilerState::IN_IMPORT: std::cout << "IN_IMPORT"; break;
            case CompilerState::IN_STRING: std::cout << "IN_STRING"; break;
            case CompilerState::IN_COMMENT: std::cout << "IN_COMMENT"; break;
            case CompilerState::IN_CSS_SELECTOR: std::cout << "IN_CSS_SELECTOR"; break;
            case CompilerState::IN_CSS_RULE: std::cout << "IN_CSS_RULE"; break;
            case CompilerState::IN_JS_SELECTOR: std::cout << "IN_JS_SELECTOR"; break;
            case CompilerState::ERROR: std::cout << "ERROR"; break;
        }
        std::cout << "\n";
    }
    
    // 输出元素栈
    std::cout << "\nElement Stack (" << elementStack_.size() << " elements):\n";
    std::stack<ElementState> tempElements = elementStack_;
    while (!tempElements.empty()) {
        const ElementState& elem = tempElements.top();
        std::cout << "  " << elem.tagName;
        if (!elem.id.empty()) std::cout << "#" << elem.id;
        if (!elem.className.empty()) std::cout << "." << elem.className;
        std::cout << " (level: " << elem.nestLevel << ")\n";
        tempElements.pop();
    }
    
    // 输出其他信息
    std::cout << "\nBrace Count: " << braceCount_ << "\n";
    std::cout << "Auto Class Counter: " << autoClassCounter_ << "\n";
    std::cout << "Generated Classes: " << generatedClasses_.size() << "\n";
    
    if (isInString()) {
        std::cout << "In String: " << stringDelimiter_ << "\n";
    }
    
    if (commentType_ != CommentType::NONE) {
        std::cout << "In Comment: ";
        switch (commentType_) {
            case CommentType::SINGLE: std::cout << "//"; break;
            case CommentType::MULTI: std::cout << "/* */"; break;
            case CommentType::GENERATOR: std::cout << "--"; break;
            default: break;
        }
        std::cout << "\n";
    }
    
    if (hasError()) {
        std::cout << "Error: " << errorMessage_ << "\n";
    }
    
    std::cout << "==================\n";
}

} // namespace Chtl
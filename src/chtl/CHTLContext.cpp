#include "CHTLContext.hpp"
#include <sstream>

namespace chtl {

CHTLContext::CHTLContext()
    : currentLine_(1), currentColumn_(1), uniqueIdCounter_(0),
      indentLevel_(0), styleBlockDepth_(0), scriptBlockDepth_(0),
      debugMode_(false) {
    globalMap_ = std::make_unique<CHTLGlobalMap>();
    stateManager_ = std::make_unique<CHTLStateManager>();
}

CHTLContext::~CHTLContext() = default;

void CHTLContext::setCurrentFile(const std::string& filePath) {
    currentFile_ = filePath;
}

void CHTLContext::setCurrentPosition(size_t line, size_t column) {
    currentLine_ = line;
    currentColumn_ = column;
}

void CHTLContext::addError(const std::string& message) {
    addError(message, currentLine_, currentColumn_);
}

void CHTLContext::addWarning(const std::string& message) {
    addWarning(message, currentLine_, currentColumn_);
}

void CHTLContext::addError(const std::string& message, size_t line, size_t column) {
    std::stringstream ss;
    ss << currentFile_ << ":" << line << ":" << column << ": 错误: " << message;
    errors_.push_back(ss.str());
}

void CHTLContext::addWarning(const std::string& message, size_t line, size_t column) {
    std::stringstream ss;
    ss << currentFile_ << ":" << line << ":" << column << ": 警告: " << message;
    warnings_.push_back(ss.str());
}

std::string CHTLContext::generateUniqueId(const std::string& prefix) {
    std::stringstream ss;
    ss << prefix << "_" << uniqueIdCounter_++;
    return ss.str();
}

std::string CHTLContext::getIndentString() const {
    return std::string(indentLevel_ * 4, ' ');  // 4个空格作为一级缩进
}

void CHTLContext::enterStyleBlock() {
    styleBlockDepth_++;
    stateManager_->pushLexerState(LexerState::IN_STYLE_BLOCK);
}

void CHTLContext::exitStyleBlock() {
    if (styleBlockDepth_ > 0) {
        styleBlockDepth_--;
        stateManager_->popLexerState();
    }
}

void CHTLContext::enterScriptBlock() {
    scriptBlockDepth_++;
    stateManager_->pushLexerState(LexerState::IN_SCRIPT_BLOCK);
}

void CHTLContext::exitScriptBlock() {
    if (scriptBlockDepth_ > 0) {
        scriptBlockDepth_--;
        stateManager_->popLexerState();
    }
}

void CHTLContext::pushElement(const std::string& elementName) {
    elementStack_.push(elementName);
    globalMap_->enterScope(ScopeType::ELEMENT, elementName);
}

void CHTLContext::popElement() {
    if (!elementStack_.empty()) {
        elementStack_.pop();
        globalMap_->exitScope();
    }
}

const std::string& CHTLContext::getCurrentElement() const {
    static const std::string empty = "";
    return elementStack_.empty() ? empty : elementStack_.top();
}

void CHTLContext::addGeneratedClass(const std::string& className) {
    generatedClasses_.push_back(className);
}

void CHTLContext::reset() {
    currentFile_.clear();
    currentLine_ = 1;
    currentColumn_ = 1;
    errors_.clear();
    warnings_.clear();
    globalMap_->clear();
    stateManager_->reset();
    uniqueIdCounter_ = 0;
    indentLevel_ = 0;
    styleBlockDepth_ = 0;
    scriptBlockDepth_ = 0;
    while (!elementStack_.empty()) {
        elementStack_.pop();
    }
    generatedClasses_.clear();
}

} // namespace chtl
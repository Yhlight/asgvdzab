#include "CHTLState.hpp"

namespace chtl {

CHTLStateManager::CHTLStateManager() 
    : braceDepth_(0), parenDepth_(0), bracketDepth_(0) {
    lexerStateStack_.push(LexerState::NORMAL);
    parserStateStack_.push(ParserState::TOP_LEVEL);
}

void CHTLStateManager::pushLexerState(LexerState state) {
    lexerStateStack_.push(state);
}

void CHTLStateManager::popLexerState() {
    if (lexerStateStack_.size() > 1) {
        lexerStateStack_.pop();
    }
}

LexerState CHTLStateManager::getCurrentLexerState() const {
    return lexerStateStack_.empty() ? LexerState::NORMAL : lexerStateStack_.top();
}

bool CHTLStateManager::isInLexerState(LexerState state) const {
    return getCurrentLexerState() == state;
}

void CHTLStateManager::pushParserState(ParserState state) {
    parserStateStack_.push(state);
}

void CHTLStateManager::popParserState() {
    if (parserStateStack_.size() > 1) {
        parserStateStack_.pop();
    }
}

ParserState CHTLStateManager::getCurrentParserState() const {
    return parserStateStack_.empty() ? ParserState::TOP_LEVEL : parserStateStack_.top();
}

bool CHTLStateManager::isInParserState(ParserState state) const {
    return getCurrentParserState() == state;
}

bool CHTLStateManager::isInString() const {
    LexerState state = getCurrentLexerState();
    return state == LexerState::IN_STRING_SINGLE || 
           state == LexerState::IN_STRING_DOUBLE;
}

bool CHTLStateManager::isInComment() const {
    LexerState state = getCurrentLexerState();
    return state == LexerState::IN_COMMENT_LINE || 
           state == LexerState::IN_COMMENT_BLOCK ||
           state == LexerState::IN_COMMENT_HTML;
}

bool CHTLStateManager::isInBlock() const {
    LexerState state = getCurrentLexerState();
    return state == LexerState::IN_STYLE_BLOCK || 
           state == LexerState::IN_SCRIPT_BLOCK;
}

void CHTLStateManager::reset() {
    // 清空栈
    while (!lexerStateStack_.empty()) {
        lexerStateStack_.pop();
    }
    while (!parserStateStack_.empty()) {
        parserStateStack_.pop();
    }
    
    // 重置为初始状态
    lexerStateStack_.push(LexerState::NORMAL);
    parserStateStack_.push(ParserState::TOP_LEVEL);
    
    // 重置计数器
    braceDepth_ = 0;
    parenDepth_ = 0;
    bracketDepth_ = 0;
}

} // namespace chtl
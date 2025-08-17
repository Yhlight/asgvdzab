#pragma once

#include "Common/FragmentTypes.h"
#include "Scanner/Token.h"
#include <string>
#include <stack>
#include <vector>

namespace Chtl {

/**
 * 词法分析器状态枚举
 */
enum class LexerState {
    NORMAL,                    // 正常状态
    IN_TEXT_BLOCK,            // 在text{}块中
    IN_STYLE_BLOCK,           // 在style{}块中
    IN_SCRIPT_BLOCK,          // 在script{}块中
    IN_TEMPLATE_BLOCK,        // 在[Template]块中
    IN_CUSTOM_BLOCK,          // 在[Custom]块中
    IN_ORIGIN_BLOCK,          // 在[Origin]块中
    IN_CONFIGURATION_BLOCK,   // 在[Configuration]块中
    IN_NAMESPACE_BLOCK,       // 在[Namespace]块中
    IN_IMPORT_BLOCK,          // 在[Import]块中
    IN_STRING_DOUBLE,         // 在双引号字符串中
    IN_STRING_SINGLE,         // 在单引号字符串中
    IN_LINE_COMMENT,          // 在//注释中
    IN_BLOCK_COMMENT,         // 在/**/注释中
    IN_GENERATOR_COMMENT,     // 在--注释中
    IN_ENHANCED_SELECTOR,     // 在{{}}增强选择器中
    IN_ATTRIBUTE_VALUE        // 在属性值中
};

/**
 * 括号类型
 */
enum class BraceType {
    CURLY,      // {}
    SQUARE,     // []
    ROUND       // ()
};

/**
 * 括号信息
 */
struct BraceInfo {
    BraceType Type;
    size_t Position;
    size_t Line;
    size_t Column;
    LexerState StateWhenOpened;
};

/**
 * 词法分析器状态管理器
 */
class LexerStateManager {
public:
    LexerStateManager();
    ~LexerStateManager() = default;
    
    // === 状态管理 ===
    LexerState GetCurrentState() const { return CurrentState_; }
    void SetState(LexerState state) { CurrentState_ = state; }
    
    void PushState(LexerState state);
    LexerState PopState();
    bool HasPreviousState() const { return !StateStack_.empty(); }
    
    // === 括号匹配 ===
    void PushBrace(BraceType type, size_t position, size_t line, size_t column);
    bool PopBrace(BraceType type);
    bool IsMatchingBrace(char openBrace, char closeBrace) const;
    size_t GetBraceDepth() const { return BraceStack_.size(); }
    bool IsInBraces() const { return !BraceStack_.empty(); }
    
    // === 块状态检测 ===
    bool IsInTextBlock() const { return CurrentState_ == LexerState::IN_TEXT_BLOCK; }
    bool IsInStyleBlock() const { return CurrentState_ == LexerState::IN_STYLE_BLOCK; }
    bool IsInScriptBlock() const { return CurrentState_ == LexerState::IN_SCRIPT_BLOCK; }
    bool IsInTemplateBlock() const { return CurrentState_ == LexerState::IN_TEMPLATE_BLOCK; }
    bool IsInCustomBlock() const { return CurrentState_ == LexerState::IN_CUSTOM_BLOCK; }
    bool IsInOriginBlock() const { return CurrentState_ == LexerState::IN_ORIGIN_BLOCK; }
    bool IsInConfigurationBlock() const { return CurrentState_ == LexerState::IN_CONFIGURATION_BLOCK; }
    bool IsInNamespaceBlock() const { return CurrentState_ == LexerState::IN_NAMESPACE_BLOCK; }
    bool IsInImportBlock() const { return CurrentState_ == LexerState::IN_IMPORT_BLOCK; }
    
    // === 字符串状态检测 ===
    bool IsInString() const { 
        return CurrentState_ == LexerState::IN_STRING_DOUBLE || 
               CurrentState_ == LexerState::IN_STRING_SINGLE; 
    }
    bool IsInDoubleQuotedString() const { return CurrentState_ == LexerState::IN_STRING_DOUBLE; }
    bool IsInSingleQuotedString() const { return CurrentState_ == LexerState::IN_STRING_SINGLE; }
    
    // === 注释状态检测 ===
    bool IsInComment() const {
        return CurrentState_ == LexerState::IN_LINE_COMMENT ||
               CurrentState_ == LexerState::IN_BLOCK_COMMENT ||
               CurrentState_ == LexerState::IN_GENERATOR_COMMENT;
    }
    bool IsInLineComment() const { return CurrentState_ == LexerState::IN_LINE_COMMENT; }
    bool IsInBlockComment() const { return CurrentState_ == LexerState::IN_BLOCK_COMMENT; }
    bool IsInGeneratorComment() const { return CurrentState_ == LexerState::IN_GENERATOR_COMMENT; }
    
    // === 特殊状态检测 ===
    bool IsInEnhancedSelector() const { return CurrentState_ == LexerState::IN_ENHANCED_SELECTOR; }
    bool IsInAttributeValue() const { return CurrentState_ == LexerState::IN_ATTRIBUTE_VALUE; }
    
    // === 状态转换 ===
    void EnterTextBlock() { PushState(LexerState::IN_TEXT_BLOCK); }
    void EnterStyleBlock() { PushState(LexerState::IN_STYLE_BLOCK); }
    void EnterScriptBlock() { PushState(LexerState::IN_SCRIPT_BLOCK); }
    void EnterTemplateBlock() { PushState(LexerState::IN_TEMPLATE_BLOCK); }
    void EnterCustomBlock() { PushState(LexerState::IN_CUSTOM_BLOCK); }
    void EnterOriginBlock() { PushState(LexerState::IN_ORIGIN_BLOCK); }
    void EnterConfigurationBlock() { PushState(LexerState::IN_CONFIGURATION_BLOCK); }
    void EnterNamespaceBlock() { PushState(LexerState::IN_NAMESPACE_BLOCK); }
    void EnterImportBlock() { PushState(LexerState::IN_IMPORT_BLOCK); }
    
    void EnterDoubleQuotedString() { PushState(LexerState::IN_STRING_DOUBLE); }
    void EnterSingleQuotedString() { PushState(LexerState::IN_STRING_SINGLE); }
    void EnterLineComment() { PushState(LexerState::IN_LINE_COMMENT); }
    void EnterBlockComment() { PushState(LexerState::IN_BLOCK_COMMENT); }
    void EnterGeneratorComment() { PushState(LexerState::IN_GENERATOR_COMMENT); }
    void EnterEnhancedSelector() { PushState(LexerState::IN_ENHANCED_SELECTOR); }
    void EnterAttributeValue() { PushState(LexerState::IN_ATTRIBUTE_VALUE); }
    
    void ExitCurrentState() { PopState(); }
    
    // === 工具方法 ===
    void Reset();
    std::string GetStateDescription() const;
    std::string GetDebugInfo() const;
    
    // === 错误检测 ===
    bool HasUnmatchedBraces() const { return !BraceStack_.empty(); }
    std::vector<BraceInfo> GetUnmatchedBraces() const { return BraceStack_; }

private:
    LexerState CurrentState_;
    std::stack<LexerState> StateStack_;
    std::vector<BraceInfo> BraceStack_;
    
    // 工具方法
    std::string StateToString(LexerState state) const;
    BraceType CharToBraceType(char brace) const;
    char BraceTypeToChar(BraceType type, bool isOpen) const;
};

} // namespace Chtl
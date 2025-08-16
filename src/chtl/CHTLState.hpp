#pragma once

#include <stack>
#include <string>

namespace chtl {

/**
 * 词法分析器状态
 */
enum class LexerState {
    NORMAL,             // 正常状态
    IN_STRING_SINGLE,   // 单引号字符串中
    IN_STRING_DOUBLE,   // 双引号字符串中
    IN_COMMENT_LINE,    // 单行注释中
    IN_COMMENT_BLOCK,   // 多行注释中
    IN_COMMENT_HTML,    // HTML注释中
    IN_BRACKET,         // 在[]中
    IN_AT_DIRECTIVE,    // 在@指令中
    IN_TEMPLATE,        // 在Template中
    IN_CUSTOM,          // 在Custom中
    IN_STYLE_BLOCK,     // 在style块中
    IN_SCRIPT_BLOCK     // 在script块中
};

/**
 * 解析器状态
 */
enum class ParserState {
    TOP_LEVEL,          // 顶层
    IN_ELEMENT,         // 在元素中
    IN_ATTRIBUTE,       // 在属性中
    IN_TEXT,            // 在文本中
    IN_STYLE,           // 在样式块中
    IN_SCRIPT,          // 在脚本块中
    IN_TEMPLATE_DEF,    // 在模板定义中
    IN_CUSTOM_DEF,      // 在自定义定义中
    IN_NAMESPACE_DEF,   // 在命名空间定义中
    IN_CONFIG,          // 在配置块中
    IN_IMPORT,          // 在导入语句中
    IN_ORIGIN           // 在原始嵌入中
};

/**
 * 状态管理器
 */
class CHTLStateManager {
public:
    CHTLStateManager();
    
    // Lexer状态管理
    void pushLexerState(LexerState state);
    void popLexerState();
    LexerState getCurrentLexerState() const;
    bool isInLexerState(LexerState state) const;
    
    // Parser状态管理
    void pushParserState(ParserState state);
    void popParserState();
    ParserState getCurrentParserState() const;
    bool isInParserState(ParserState state) const;
    
    // 大括号计数
    void enterBrace() { braceDepth_++; }
    void exitBrace() { if (braceDepth_ > 0) braceDepth_--; }
    int getBraceDepth() const { return braceDepth_; }
    
    // 括号计数
    void enterParen() { parenDepth_++; }
    void exitParen() { if (parenDepth_ > 0) parenDepth_--; }
    int getParenDepth() const { return parenDepth_; }
    
    // 方括号计数
    void enterBracket() { bracketDepth_++; }
    void exitBracket() { if (bracketDepth_ > 0) bracketDepth_--; }
    int getBracketDepth() const { return bracketDepth_; }
    
    // 特殊状态检查
    bool isInString() const;
    bool isInComment() const;
    bool isInBlock() const;
    
    // 重置
    void reset();
    
private:
    std::stack<LexerState> lexerStateStack_;
    std::stack<ParserState> parserStateStack_;
    int braceDepth_;
    int parenDepth_;
    int bracketDepth_;
};

} // namespace chtl
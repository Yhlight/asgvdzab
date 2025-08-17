#ifndef CHTL_COMMON_STATE_H
#define CHTL_COMMON_STATE_H

#include <string>
#include <stack>
#include <vector>
#include <unordered_set>

namespace Chtl {

// 编译器状态枚举
enum class CompilerState {
    INITIAL,                    // 初始状态
    IN_ELEMENT,                 // 在元素内部
    IN_ATTRIBUTE,               // 在属性定义中
    IN_TEXT,                    // 在text块内
    IN_STYLE,                   // 在style块内（局部）
    IN_SCRIPT,                  // 在script块内（局部）
    IN_TEMPLATE,                // 在[Template]定义内
    IN_CUSTOM,                  // 在[Custom]定义内
    IN_ORIGIN,                  // 在[Origin]块内
    IN_NAMESPACE,               // 在[Namespace]内
    IN_CONFIGURATION,           // 在[Configuration]内
    IN_IMPORT,                  // 在[Import]语句中
    IN_STRING,                  // 在字符串内
    IN_COMMENT,                 // 在注释内
    IN_CSS_SELECTOR,            // 在CSS选择器内（style块中）
    IN_CSS_RULE,                // 在CSS规则内
    IN_JS_SELECTOR,             // 在CHTL JS {{}}选择器内
    ATTRIBUTE,                  // 属性状态（向后兼容）
    CSS_RULE,                   // CSS规则（向后兼容）
    STYLE_PROPERTY,             // 样式属性（向后兼容）
    ERROR                       // 错误状态
};

// 元素状态（记录当前处理的元素信息）
struct ElementState {
    std::string tagName;               // 标签名
    std::string id;                    // id属性
    std::string className;             // class属性
    std::vector<std::string> classes;  // 所有类名
    bool hasLocalStyle = false;        // 是否有局部样式
    bool hasLocalScript = false;       // 是否有局部脚本
    int nestLevel = 0;                 // 嵌套层级
    
    ElementState() = default;
};

// CSS状态（处理style块时的状态）
struct CssState {
    std::string currentSelector;       // 当前选择器
    bool inRule = false;               // 是否在规则内
    bool isInlineStyle = false;        // 是否是内联样式
    std::string autoClassName;         // 自动生成的类名
    
    CssState() = default;
};

// 状态管理器
class State {
public:
    State();
    ~State();
    
    // 主状态管理
    void pushState(CompilerState state);
    void popState();
    CompilerState getCurrentState() const;
    bool isInState(CompilerState state) const;
    
    // 元素状态管理
    void pushElement(const ElementState& element);
    void popElement();
    ElementState& getCurrentElement();
    const ElementState& getCurrentElement() const;
    bool hasCurrentElement() const;
    
    // CSS状态管理
    void setCssState(const CssState& css) { cssState_ = css; }
    CssState& getCssState() { return cssState_; }
    const CssState& getCssState() const { return cssState_; }
    
    // 花括号计数
    void enterBrace() { braceCount_++; }
    void exitBrace() { if (braceCount_ > 0) braceCount_--; }
    int getBraceCount() const { return braceCount_; }
    void resetBraceCount() { braceCount_ = 0; }
    
    // 括号平衡检查
    void pushParen(char paren);
    bool popParen(char paren);
    bool isBalanced() const { return parenStack_.empty(); }
    
    // 字符串定界符
    void setStringDelimiter(char delimiter) { stringDelimiter_ = delimiter; }
    char getStringDelimiter() const { return stringDelimiter_; }
    bool isInString() const { return stringDelimiter_ != '\0'; }
    
    // 注释类型
    enum class CommentType { NONE, SINGLE, MULTI, GENERATOR };
    void setCommentType(CommentType type) { commentType_ = type; }
    CommentType getCommentType() const { return commentType_; }
    
    // 错误状态
    void setError(const std::string& message);
    bool hasError() const { return !errorMessage_.empty(); }
    std::string getError() const { return errorMessage_; }
    void clearError() { errorMessage_.clear(); }
    
    // 自动类名生成
    std::string generateAutoClassName();
    void addGeneratedClass(const std::string& className);
    bool isGeneratedClass(const std::string& className) const;
    
    // 重置所有状态
    void reset();
    
    // 调试输出
    void dump() const;

private:
    // 状态栈
    std::stack<CompilerState> stateStack_;
    
    // 元素栈
    std::stack<ElementState> elementStack_;
    
    // CSS状态
    CssState cssState_;
    
    // 计数器
    int braceCount_;
    int autoClassCounter_;
    
    // 括号栈（用于平衡检查）
    std::stack<char> parenStack_;
    
    // 字符串定界符
    char stringDelimiter_;
    
    // 注释类型
    CommentType commentType_;
    
    // 生成的类名集合
    std::unordered_set<std::string> generatedClasses_;
    
    // 错误信息
    std::string errorMessage_;
};

} // namespace Chtl

#endif // CHTL_COMMON_STATE_H
#pragma once

#include "ASTNode.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace chtl {

// CHTL JS AST节点类型枚举 - 严格按照语法文档
enum class CHTLJSASTNodeType {
    // 基础节点
    CHTLJS_ROOT,                // CHTL JS根节点
    CHTLJS_COMMENT,             // CHTL JS注释节点
    
    // 局部script节点
    SCRIPT_BLOCK,               // script块节点
    
    // 增强选择器节点
    ENHANCED_SELECTOR,          // {{CSS选择器}}节点
    CSS_SELECTOR_CONTENT,       // CSS选择器内容节点
    ARRAY_ACCESS,               // [0] 数组访问节点
    
    // 链式访问节点
    CHAIN_ACCESS,               // -> 链式访问节点
    DOT_ACCESS,                 // . 链式访问节点
    METHOD_CALL,                // 方法调用节点
    
    // 增强监听器节点
    LISTEN_DECLARATION,         // listen声明节点
    EVENT_HANDLER,              // 事件处理器节点
    EVENT_TYPE,                 // 事件类型节点
    
    // 事件委托节点
    DELEGATE_DECLARATION,       // delegate声明节点
    DELEGATE_TARGET,            // 委托目标节点
    DELEGATE_EVENT,             // 委托事件节点
    
    // 动画节点
    ANIMATE_DECLARATION,        // animate声明节点
    ANIMATION_DURATION,         // 动画持续时间节点
    ANIMATION_EASING,           // 动画缓动函数节点
    ANIMATION_BEGIN,            // 动画开始状态节点
    ANIMATION_WHEN,             // 动画中间状态节点
    ANIMATION_END,              // 动画结束状态节点
    ANIMATION_LOOP,             // 动画循环次数节点
    ANIMATION_DIRECTION,        // 动画方向节点
    ANIMATION_DELAY,            // 动画延迟节点
    ANIMATION_CALLBACK,         // 动画回调函数节点
    ANIMATION_KEYFRAME,         // 动画关键帧节点
    ANIMATION_TIME,             // 动画时间点节点
    
    // JavaScript语法节点（基础支持）
    FUNCTION_DECLARATION,       // 函数声明节点
    FUNCTION_PARAMETER,         // 函数参数节点
    FUNCTION_BODY,              // 函数体节点
    VARIABLE_DECLARATION,       // 变量声明节点
    VARIABLE_INITIALIZER,       // 变量初始化器节点
    EXPRESSION_STATEMENT,       // 表达式语句节点
    IF_STATEMENT,               // if语句节点
    ELSE_CLAUSE,                // else子句节点
    FOR_STATEMENT,              // for语句节点
    WHILE_STATEMENT,            // while语句节点
    RETURN_STATEMENT,           // return语句节点
    FUNCTION_CALL,              // 函数调用节点
    ARGUMENT_LIST,              // 参数列表节点
    OBJECT_LITERAL,             // 对象字面量节点
    PROPERTY_ASSIGNMENT,        // 属性赋值节点
    ARRAY_LITERAL,              // 数组字面量节点
    ARRAY_ELEMENT,              // 数组元素节点
    BINARY_EXPRESSION,          // 二元表达式节点
    UNARY_EXPRESSION,           // 一元表达式节点
    IDENTIFIER,                 // 标识符节点
    LITERAL_VALUE,              // 字面量值节点
    
    // 其他节点
    CHTLJS_UNKNOWN              // 未知节点类型
};

// CHTL JS AST节点基类
class CHTLJSASTNode : public ASTNode {
public:
    CHTLJSASTNodeType chtljs_type;
    
    CHTLJSASTNode(CHTLJSASTNodeType t, size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : ASTNode(ASTNodeType::SCRIPT_BLOCK, l, c, start, end), chtljs_type(t) {}
    
    virtual ~CHTLJSASTNode() = default;
    
    // 重写基类方法
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
    
    // CHTL JS特定方法
    virtual bool isCHTLJSSpecific() const;
    virtual std::string getCHTLJSNodeTypeName() const;
    
protected:
    std::string getCHTLJSNodeTypeName(CHTLJSASTNodeType type) const;
};

// 增强选择器节点
class EnhancedSelectorNode : public CHTLJSASTNode {
public:
    std::string selector_content;   // CSS选择器内容
    std::string array_index;        // 数组索引（如果有）
    bool has_array_access;           // 是否有数组访问
    
    EnhancedSelectorNode(const std::string& content, const std::string& index = "", 
                        size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::ENHANCED_SELECTOR, l, c, start, end),
          selector_content(content), array_index(index), has_array_access(!index.empty()) {}
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 链式访问节点
class ChainAccessNode : public CHTLJSASTNode {
public:
    std::string access_type;        // 访问类型（-> 或 .）
    std::string method_name;        // 方法名
    std::vector<std::unique_ptr<ASTNode>> arguments; // 方法参数
    
    ChainAccessNode(const std::string& type, const std::string& method, 
                   size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::CHAIN_ACCESS, l, c, start, end),
          access_type(type), method_name(method) {}
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 增强监听器节点
class ListenDeclarationNode : public CHTLJSASTNode {
public:
    std::string target_selector;    // 目标选择器
    std::unordered_map<std::string, std::unique_ptr<ASTNode>> event_handlers; // 事件处理器映射
    
    ListenDeclarationNode(const std::string& selector, size_t l = 0, size_t c = 0, 
                         size_t start = 0, size_t end = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::LISTEN_DECLARATION, l, c, start, end),
          target_selector(selector) {}
    
    void addEventHandler(const std::string& event_type, std::unique_ptr<ASTNode> handler);
    ASTNode* getEventHandler(const std::string& event_type) const;
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 事件委托节点
class DelegateDeclarationNode : public CHTLJSASTNode {
public:
    std::string parent_selector;    // 父元素选择器
    std::vector<std::unique_ptr<ASTNode>> target_selectors; // 目标选择器列表
    std::unordered_map<std::string, std::unique_ptr<ASTNode>> event_handlers; // 事件处理器映射
    
    DelegateDeclarationNode(const std::string& parent, size_t l = 0, size_t c = 0, 
                           size_t start = 0, size_t end = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::DELEGATE_DECLARATION, l, c, start, end),
          parent_selector(parent) {}
    
    void addTargetSelector(std::unique_ptr<ASTNode> selector);
    void addEventHandler(const std::string& event_type, std::unique_ptr<ASTNode> handler);
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 动画声明节点
class AnimateDeclarationNode : public CHTLJSASTNode {
public:
    std::unique_ptr<ASTNode> duration;      // 动画持续时间
    std::unique_ptr<ASTNode> easing;        // 动画缓动函数
    std::unique_ptr<ASTNode> begin_state;   // 开始状态
    std::vector<std::unique_ptr<ASTNode>> when_states; // 中间状态
    std::unique_ptr<ASTNode> end_state;     // 结束状态
    std::unique_ptr<ASTNode> loop_count;    // 循环次数
    std::unique_ptr<ASTNode> direction;     // 播放方向
    std::unique_ptr<ASTNode> delay;         // 延迟时间
    std::unique_ptr<ASTNode> callback;      // 回调函数
    
    AnimateDeclarationNode(size_t l = 0, size_t c = 0, size_t start = 0, size_t end = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::ANIMATE_DECLARATION, l, c, start, end) {}
    
    void setDuration(std::unique_ptr<ASTNode> dur);
    void setEasing(std::unique_ptr<ASTNode> ease);
    void setBeginState(std::unique_ptr<ASTNode> begin);
    void addWhenState(std::unique_ptr<ASTNode> when);
    void setEndState(std::unique_ptr<ASTNode> end);
    void setLoopCount(std::unique_ptr<ASTNode> loop);
    void setDirection(std::unique_ptr<ASTNode> dir);
    void setDelay(std::unique_ptr<ASTNode> del);
    void setCallback(std::unique_ptr<ASTNode> cb);
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// 动画关键帧节点
class AnimationKeyframeNode : public CHTLJSASTNode {
public:
    std::string time_point;         // 时间点（如0.4, 0.8）
    std::vector<std::unique_ptr<ASTNode>> css_properties; // CSS属性列表
    
    AnimationKeyframeNode(const std::string& time, size_t l = 0, size_t c = 0, 
                         size_t start = 0, size_t end = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::ANIMATION_KEYFRAME, l, c, start, end),
          time_point(time) {}
    
    void addCSSProperty(std::unique_ptr<ASTNode> property);
    
    virtual bool isValid() const override;
    virtual std::vector<std::string> validate() const override;
    virtual std::string serialize() const override;
    virtual std::string toDebugString() const override;
    virtual std::unique_ptr<ASTNode> clone() const override;
};

// CHTL JS AST访问者基类
class CHTLJSASTVisitor : public ASTVisitor {
public:
    virtual ~CHTLJSASTVisitor() = default;
    
    // 重写基类方法
    virtual void visit(ASTNode* node) override;
    
    // CHTL JS特定节点访问方法
    virtual void visitEnhancedSelector(EnhancedSelectorNode* node) = 0;
    virtual void visitChainAccess(ChainAccessNode* node) = 0;
    virtual void visitListenDeclaration(ListenDeclarationNode* node) = 0;
    virtual void visitDelegateDeclaration(DelegateDeclarationNode* node) = 0;
    virtual void visitAnimateDeclaration(AnimateDeclarationNode* node) = 0;
    virtual void visitAnimationKeyframe(AnimationKeyframeNode* node) = 0;
    
    // 其他CHTL JS节点访问方法
    virtual void visitScriptBlock(ASTNode* node) override;
    virtual void visitCHTLJSRoot(ASTNode* node) = 0;
    virtual void visitCHTLJSComment(ASTNode* node) = 0;
};

} // namespace chtl
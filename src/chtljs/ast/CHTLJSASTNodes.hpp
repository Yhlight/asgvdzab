#pragma once

#include "chtl/ast/ASTNode.hpp"
#include <unordered_map>

namespace chtl {
namespace js {
namespace ast {

using chtl::ast::ASTNode;
using chtl::ast::ASTNodePtr;

/**
 * CHTL JS特有的AST节点类型
 */
enum class CHTLJSNodeType {
    // 增强选择器
    SELECTOR_EXPR,       // {{selector}}
    
    // 增强操作符
    ARROW_ACCESS,        // ->
    
    // 增强监听器
    LISTEN_EXPR,         // ->listen({...})
    
    // 事件委托
    DELEGATE_EXPR,       // ->delegate({...})
    
    // 动画
    ANIMATE_EXPR,        // animate({...})
    
    // 对象字面量
    OBJECT_LITERAL,      // {...}
    
    // 属性
    PROPERTY,            // key: value
    
    // 函数表达式
    FUNCTION_EXPR,       // function() {...}
    
    // 标识符引用
    IDENTIFIER_REF,      // 已存在的函数名等
    
    // 方法调用
    METHOD_CALL          // method(args)
};

/**
 * CHTL JS基础节点
 */
class CHTLJSNode : public ASTNode {
public:
    CHTLJSNode(CHTLJSNodeType jsType, size_t line = 0, size_t column = 0)
        : ASTNode(static_cast<chtl::ast::ASTNodeType>(-1), line, column), jsType_(jsType) {}
    
    CHTLJSNodeType getJSType() const { return jsType_; }
    
protected:
    CHTLJSNodeType jsType_;
};

/**
 * 增强选择器节点 {{selector}}
 */
class SelectorExprNode : public CHTLJSNode {
public:
    enum SelectorType {
        TAG_OR_ID_OR_CLASS,  // {{box}} - 先tag，然后id，最后class
        CLASS_SELECTOR,      // {{.box}}
        ID_SELECTOR,         // {{#box}}
        TAG_SELECTOR,        // {{button}}
        COMPLEX_SELECTOR,    // {{.box button}}
        INDEXED_SELECTOR     // {{button[0]}}
    };
    
    SelectorExprNode(const std::string& selector, SelectorType type, size_t line = 0, size_t column = 0)
        : CHTLJSNode(CHTLJSNodeType::SELECTOR_EXPR, line, column), 
          selector_(selector), selectorType_(type), index_(-1) {}
    
    void accept(chtl::ast::ASTVisitor& visitor) override;
    
    const std::string& getSelector() const { return selector_; }
    SelectorType getSelectorType() const { return selectorType_; }
    void setIndex(int index) { index_ = index; }
    int getIndex() const { return index_; }
    
private:
    std::string selector_;
    SelectorType selectorType_;
    int index_;  // 对于索引选择器
};

/**
 * 箭头访问节点 ->
 */
class ArrowAccessNode : public CHTLJSNode {
public:
    ArrowAccessNode(ASTNodePtr left, ASTNodePtr right, size_t line = 0, size_t column = 0)
        : CHTLJSNode(CHTLJSNodeType::ARROW_ACCESS, line, column), 
          left_(left), right_(right) {}
    
    void accept(chtl::ast::ASTVisitor& visitor) override;
    
    ASTNodePtr getLeft() const { return left_; }
    ASTNodePtr getRight() const { return right_; }
    
private:
    ASTNodePtr left_;
    ASTNodePtr right_;
};

/**
 * 增强监听器节点 ->listen({...})
 */
class ListenExprNode : public CHTLJSNode {
public:
    ListenExprNode(size_t line = 0, size_t column = 0)
        : CHTLJSNode(CHTLJSNodeType::LISTEN_EXPR, line, column) {}
    
    void accept(chtl::ast::ASTVisitor& visitor) override;
    
    void addEventHandler(const std::string& event, ASTNodePtr handler) {
        eventHandlers_[event] = handler;
    }
    
    const std::unordered_map<std::string, ASTNodePtr>& getEventHandlers() const {
        return eventHandlers_;
    }
    
private:
    std::unordered_map<std::string, ASTNodePtr> eventHandlers_;
};

/**
 * 事件委托节点 ->delegate({...})
 */
class DelegateExprNode : public CHTLJSNode {
public:
    DelegateExprNode(size_t line = 0, size_t column = 0)
        : CHTLJSNode(CHTLJSNodeType::DELEGATE_EXPR, line, column) {}
    
    void accept(chtl::ast::ASTVisitor& visitor) override;
    
    void setTarget(ASTNodePtr target) { target_ = target; }
    ASTNodePtr getTarget() const { return target_; }
    
    void addTargets(const std::vector<ASTNodePtr>& targets) { 
        targets_ = targets; 
    }
    const std::vector<ASTNodePtr>& getTargets() const { return targets_; }
    
    void addEventHandler(const std::string& event, ASTNodePtr handler) {
        eventHandlers_[event] = handler;
    }
    
    const std::unordered_map<std::string, ASTNodePtr>& getEventHandlers() const {
        return eventHandlers_;
    }
    
private:
    ASTNodePtr target_;  // 单个目标
    std::vector<ASTNodePtr> targets_;  // 多个目标
    std::unordered_map<std::string, ASTNodePtr> eventHandlers_;
};

/**
 * 动画节点 animate({...})
 */
class AnimateExprNode : public CHTLJSNode {
public:
    AnimateExprNode(size_t line = 0, size_t column = 0)
        : CHTLJSNode(CHTLJSNodeType::ANIMATE_EXPR, line, column) {}
    
    void accept(chtl::ast::ASTVisitor& visitor) override;
    
    // 动画配置
    void setDuration(int duration) { duration_ = duration; }
    int getDuration() const { return duration_; }
    
    void setEasing(const std::string& easing) { easing_ = easing; }
    const std::string& getEasing() const { return easing_; }
    
    void setBegin(ASTNodePtr begin) { begin_ = begin; }
    ASTNodePtr getBegin() const { return begin_; }
    
    void setEnd(ASTNodePtr end) { end_ = end; }
    ASTNodePtr getEnd() const { return end_; }
    
    void addWhenFrame(double at, ASTNodePtr properties) {
        whenFrames_.push_back({at, properties});
    }
    const std::vector<std::pair<double, ASTNodePtr>>& getWhenFrames() const {
        return whenFrames_;
    }
    
    void setLoop(int loop) { loop_ = loop; }
    int getLoop() const { return loop_; }
    
    void setDirection(const std::string& direction) { direction_ = direction; }
    const std::string& getDirection() const { return direction_; }
    
    void setDelay(int delay) { delay_ = delay; }
    int getDelay() const { return delay_; }
    
    void setCallback(ASTNodePtr callback) { callback_ = callback; }
    ASTNodePtr getCallback() const { return callback_; }
    
private:
    int duration_ = 0;
    std::string easing_;
    ASTNodePtr begin_;
    ASTNodePtr end_;
    std::vector<std::pair<double, ASTNodePtr>> whenFrames_;
    int loop_ = 1;
    std::string direction_;
    int delay_ = 0;
    ASTNodePtr callback_;
};

/**
 * 对象字面量节点 {...}
 */
class ObjectLiteralNode : public CHTLJSNode {
public:
    ObjectLiteralNode(size_t line = 0, size_t column = 0)
        : CHTLJSNode(CHTLJSNodeType::OBJECT_LITERAL, line, column) {}
    
    void accept(chtl::ast::ASTVisitor& visitor) override;
    
    void addProperty(const std::string& key, ASTNodePtr value) {
        properties_.push_back({key, value});
    }
    
    const std::vector<std::pair<std::string, ASTNodePtr>>& getProperties() const {
        return properties_;
    }
    
private:
    std::vector<std::pair<std::string, ASTNodePtr>> properties_;
};

/**
 * 属性节点 key: value
 */
class PropertyNode : public CHTLJSNode {
public:
    PropertyNode(const std::string& key, ASTNodePtr value, size_t line = 0, size_t column = 0)
        : CHTLJSNode(CHTLJSNodeType::PROPERTY, line, column), 
          key_(key), value_(value) {}
    
    void accept(chtl::ast::ASTVisitor& visitor) override;
    
    const std::string& getKey() const { return key_; }
    ASTNodePtr getValue() const { return value_; }
    
private:
    std::string key_;
    ASTNodePtr value_;
};

/**
 * 函数表达式节点 function() {...} 或 () => {...}
 */
class FunctionExprNode : public CHTLJSNode {
public:
    FunctionExprNode(size_t line = 0, size_t column = 0)
        : CHTLJSNode(CHTLJSNodeType::FUNCTION_EXPR, line, column) {}
    
    void accept(chtl::ast::ASTVisitor& visitor) override;
    
    void addParameter(const std::string& param) {
        parameters_.push_back(param);
    }
    
    const std::vector<std::string>& getParameters() const {
        return parameters_;
    }
    
    void setBody(const std::string& body) { body_ = body; }
    const std::string& getBody() const { return body_; }
    
private:
    std::vector<std::string> parameters_;
    std::string body_;  // 暂时存储为字符串，后续可以解析为语句
};

/**
 * 标识符引用节点（引用已存在的函数等）
 */
class IdentifierRefNode : public CHTLJSNode {
public:
    IdentifierRefNode(const std::string& name, size_t line = 0, size_t column = 0)
        : CHTLJSNode(CHTLJSNodeType::IDENTIFIER_REF, line, column), name_(name) {}
    
    void accept(chtl::ast::ASTVisitor& visitor) override;
    
    const std::string& getName() const { return name_; }
    
private:
    std::string name_;
};

/**
 * 方法调用节点
 */
class MethodCallNode : public CHTLJSNode {
public:
    MethodCallNode(const std::string& method, size_t line = 0, size_t column = 0)
        : CHTLJSNode(CHTLJSNodeType::METHOD_CALL, line, column), method_(method) {}
    
    void accept(chtl::ast::ASTVisitor& visitor) override;
    
    const std::string& getMethod() const { return method_; }
    
    void addArgument(ASTNodePtr arg) {
        arguments_.push_back(arg);
    }
    
    const std::vector<ASTNodePtr>& getArguments() const {
        return arguments_;
    }
    
private:
    std::string method_;
    std::vector<ASTNodePtr> arguments_;
};

} // namespace ast
} // namespace js
} // namespace chtl
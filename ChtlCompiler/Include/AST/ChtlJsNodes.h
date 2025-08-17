#ifndef CHTL_AST_CHTLJSNODES_H
#define CHTL_AST_CHTLJSNODES_H

#include "AST/ASTNode.h"
#include <vector>
#include <unordered_map>

namespace Chtl {

// {{选择器}} 表达式
class SelectorExpressionNode : public ASTNode {
public:
    enum SelectorType {
        TAG,            // {{div}} - 标签选择器
        CLASS,          // {{.box}} - 类选择器
        ID,             // {{#id}} - ID选择器
        COMPLEX,        // {{.box button}} - 复合选择器
        INDEX           // {{button[0]}} - 带索引的选择器
    };
    
    SelectorExpressionNode(const Token& token);
    
    SelectorType getSelectorType() const { return selectorType_; }
    void setSelectorType(SelectorType type) { selectorType_ = type; }
    
    const std::string& getSelector() const { return selector_; }
    void setSelector(const std::string& selector) { selector_ = selector; }
    
    // 索引访问（如果有）
    bool hasIndex() const { return index_ >= 0; }
    int getIndex() const { return index_; }
    void setIndex(int index) { index_ = index; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    SelectorType selectorType_;
    std::string selector_;
    int index_ = -1;  // -1 表示没有索引
};

// -> 箭头访问表达式
class ArrowAccessNode : public ASTNode {
public:
    ArrowAccessNode(const Token& token);
    
    // 左侧表达式
    void setLeft(ASTNodePtr left) { left_ = left; }
    ASTNodePtr getLeft() const { return left_; }
    
    // 右侧成员名或方法调用
    void setMember(const std::string& member) { member_ = member; }
    const std::string& getMember() const { return member_; }
    
    // 是否是方法调用
    bool isMethodCall() const { return isMethodCall_; }
    void setIsMethodCall(bool value) { isMethodCall_ = value; }
    
    // 方法参数（如果是方法调用）
    void addArgument(ASTNodePtr arg) { arguments_.push_back(arg); }
    const std::vector<ASTNodePtr>& getArguments() const { return arguments_; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    ASTNodePtr left_;
    std::string member_;
    bool isMethodCall_ = false;
    std::vector<ASTNodePtr> arguments_;
};

// listen 调用节点
class ListenCallNode : public ASTNode {
public:
    ListenCallNode(const Token& token);
    
    // 目标对象（通常是选择器表达式）
    void setTarget(ASTNodePtr target) { target_ = target; }
    ASTNodePtr getTarget() const { return target_; }
    
    // 事件配置对象
    struct EventConfig {
        std::string eventName;     // 事件名称
        ASTNodePtr handler;        // 处理函数
        bool useCapture = false;   // 是否使用捕获
        bool once = false;         // 是否只执行一次
        bool passive = false;      // 是否为被动监听器
    };
    
    void addEventConfig(const EventConfig& config) { eventConfigs_.push_back(config); }
    const std::vector<EventConfig>& getEventConfigs() const { return eventConfigs_; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    ASTNodePtr target_;
    std::vector<EventConfig> eventConfigs_;
};

// delegate 事件委托调用节点
class DelegateCallNode : public ASTNode {
public:
    DelegateCallNode(const Token& token);
    
    // 父元素（委托目标）
    void setParent(ASTNodePtr parent) { parent_ = parent; }
    ASTNodePtr getParent() const { return parent_; }
    
    // 委托配置
    struct DelegateConfig {
        std::vector<ASTNodePtr> targets;  // 要委托的子元素选择器
        std::string eventName;            // 事件名称
        ASTNodePtr handler;               // 处理函数
    };
    
    void setDelegateConfig(const DelegateConfig& config) { config_ = config; }
    const DelegateConfig& getDelegateConfig() const { return config_; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    ASTNodePtr parent_;
    DelegateConfig config_;
};

// animate 动画调用节点
class AnimateCallNode : public ASTNode {
public:
    AnimateCallNode(const Token& token);
    
    // 动画配置
    struct AnimateConfig {
        std::vector<ASTNodePtr> targets;    // 目标元素
        int duration = 1000;                // 持续时间（毫秒）
        std::string easing = "linear";      // 缓动函数
        std::unordered_map<std::string, std::pair<std::string, std::string>> properties; // 属性 -> (from, to)
        ASTNodePtr onComplete;              // 完成回调
        ASTNodePtr onUpdate;                // 更新回调
    };
    
    void setConfig(const AnimateConfig& config) { config_ = config; }
    const AnimateConfig& getConfig() const { return config_; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    AnimateConfig config_;
};

// CHTL JS 表达式节点（通用）
class ChtlJsExpressionNode : public ASTNode {
public:
    ChtlJsExpressionNode(const Token& token);
    
    // 原始JavaScript代码（已转换的）
    const std::string& getJavaScriptCode() const { return jsCode_; }
    void setJavaScriptCode(const std::string& code) { jsCode_ = code; }
    
    // 是否包含CHTL JS特殊语法
    bool hasSpecialSyntax() const { return hasSpecialSyntax_; }
    void setHasSpecialSyntax(bool value) { hasSpecialSyntax_ = value; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    std::string jsCode_;
    bool hasSpecialSyntax_ = false;
};

// 虚对象声明节点
class VirtualObjectNode : public ASTNode {
public:
    VirtualObjectNode(const Token& token);
    
    // 虚对象名称
    const std::string& getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    
    // 初始化表达式（通常是listen、iNeverAway等）
    ASTNodePtr getInitExpression() const { return initExpression_; }
    void setInitExpression(ASTNodePtr expr) { initExpression_ = expr; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    std::string name_;
    ASTNodePtr initExpression_;
};



// 虚对象函数调用节点
class VirtualCallNode : public ASTNode {
public:
    VirtualCallNode(const Token& token);
    
    // 虚对象名称
    const std::string& getObjectName() const { return objectName_; }
    void setObjectName(const std::string& name) { objectName_ = name; }
    
    // 函数名
    const std::string& getFunctionName() const { return functionName_; }
    void setFunctionName(const std::string& name) { functionName_ = name; }
    
    // 参数列表
    void addArgument(ASTNodePtr arg) { arguments_.push_back(arg); }
    const std::vector<ASTNodePtr>& getArguments() const { return arguments_; }
    
    void accept(class ASTVisitor* visitor) override;

private:
    std::string objectName_;
    std::string functionName_;
    std::vector<ASTNodePtr> arguments_;
};

} // namespace Chtl

#endif // CHTL_AST_CHTLJSNODES_H
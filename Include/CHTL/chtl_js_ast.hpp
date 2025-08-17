#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "common/types.hpp"

namespace chtl {

// 前向声明
class CHTLJSASTNode;
using CHTLJSASTNodePtr = std::shared_ptr<CHTLJSASTNode>;

/**
 * CHTL JS AST节点类型枚举
 * 严格按照CHTL语法文档.md中的CHTL JS部分定义
 */
enum class CHTLJSASTNodeType {
    // 根节点
    SCRIPT_PROGRAM,             // script { } 脚本程序

    // CHTL JS特有节点
    ENHANCED_SELECTOR,          // {{selector}} 增强选择器
    ARROW_OPERATOR,             // -> 箭头操作符
    
    // CHTL JS功能节点
    LISTEN_CALL,                // listen({ ... }) 增强监听器调用
    DELEGATE_CALL,              // delegate({ ... }) 事件委托调用
    ANIMATE_CALL,               // animate({ ... }) 动画调用

    // 监听器相关
    LISTEN_CONFIG,              // listen配置对象
    EVENT_HANDLER,              // 事件处理器 (click: function)

    // 委托相关
    DELEGATE_CONFIG,            // delegate配置对象
    DELEGATE_TARGET,            // target: {{selector}} 委托目标

    // 动画相关
    ANIMATE_CONFIG,             // animate配置对象
    ANIMATE_DURATION,           // duration: 100
    ANIMATE_EASING,             // easing: ease-in-out
    ANIMATE_BEGIN,              // begin: { ... } 起始状态
    ANIMATE_END,                // end: { ... } 结束状态
    ANIMATE_WHEN,               // when: [...] 关键帧数组
    ANIMATE_KEYFRAME,           // 关键帧对象 { at: 0.4, ... }
    ANIMATE_AT,                 // at: 0.4 时间点
    ANIMATE_LOOP,               // loop: -1 循环次数
    ANIMATE_DIRECTION,          // direction: normal 播放方向
    ANIMATE_DELAY,              // delay: 100 延迟
    ANIMATE_CALLBACK,           // callback: function 回调

    // 增强选择器类型
    SELECTOR_TAG,               // {{button}} 标签选择器
    SELECTOR_CLASS,             // {{.box}} 类选择器
    SELECTOR_ID,                // {{#box}} ID选择器
    SELECTOR_DESCENDANT,        // {{.box button}} 后代选择器
    SELECTOR_INDEXED,           // {{button[0]}} 索引选择器

    // JavaScript标准节点 (CHTL JS中需要识别的部分)
    FUNCTION_DECLARATION,       // function() { }
    FUNCTION_EXPRESSION,        // function() { }
    ARROW_FUNCTION,             // () => { }
    VARIABLE_DECLARATION,       // var/let/const
    IDENTIFIER,                 // 标识符
    LITERAL,                    // 字面量
    PROPERTY,                   // 对象属性
    OBJECT_EXPRESSION,          // { ... } 对象表达式
    ARRAY_EXPRESSION,           // [ ... ] 数组表达式
    MEMBER_EXPRESSION,          // obj.prop 成员访问
    CALL_EXPRESSION,            // func() 函数调用
    ASSIGNMENT_EXPRESSION,      // = 赋值表达式

    // 控制流
    IF_STATEMENT,               // if
    FOR_STATEMENT,              // for
    WHILE_STATEMENT,            // while
    RETURN_STATEMENT,           // return

    // 注释
    COMMENT_LINE,               // //
    COMMENT_MULTILINE,          // /* */

    // 其他
    BLOCK_STATEMENT,            // { }
    EXPRESSION_STATEMENT,       // 表达式语句
    UNKNOWN                     // 未知节点
};

/**
 * 增强选择器类型 (严格按照文档)
 */
enum class EnhancedSelectorType {
    TAG_NAME,                   // {{button}} 标签名
    CLASS_NAME,                 // {{.box}} 类名
    ID_NAME,                    // {{#box}} ID名
    DESCENDANT,                 // {{.box button}} 后代选择器
    INDEXED,                    // {{button[0]}} 索引访问
    AUTO_DETECT                 // 自动检测
};

/**
 * CHTL JS AST基础节点类
 */
class CHTLJSASTNode {
public:
    CHTLJSASTNodeType type;
    Position position;
    std::vector<CHTLJSASTNodePtr> children;
    CHTLJSASTNode* parent = nullptr;

    CHTLJSASTNode(CHTLJSASTNodeType t, Position pos = Position{})
        : type(t), position(pos) {}

    virtual ~CHTLJSASTNode() = default;

    // 添加子节点
    void addChild(CHTLJSASTNodePtr child);
    
    // 获取特定类型的子节点
    std::vector<CHTLJSASTNodePtr> getChildrenByType(CHTLJSASTNodeType targetType) const;
    
    // 查找特定类型的第一个子节点
    CHTLJSASTNodePtr findChild(CHTLJSASTNodeType targetType) const;

    // 节点类型检查
    bool isCHTLJSSpecific() const;
    bool isEnhancedSelector() const;
    bool isListenNode() const;
    bool isDelegateNode() const;
    bool isAnimateNode() const;

    // 虚函数供子类重写
    virtual std::string toString() const;
    virtual CHTLJSASTNodePtr clone() const;
};

/**
 * 脚本程序根节点
 * 对应: script { ... }
 */
class ScriptProgramNode : public CHTLJSASTNode {
public:
    ScriptProgramNode(Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::SCRIPT_PROGRAM, pos) {}

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * 增强选择器节点
 * 对应: {{selector}} (严格按照文档中的种类)
 */
class EnhancedSelectorNode : public CHTLJSASTNode {
public:
    std::string selectorText;           // 选择器文本内容
    EnhancedSelectorType selectorType;  // 选择器类型
    
    // 解析后的信息
    struct ParsedInfo {
        std::string tagName;            // 标签名 (button)
        std::string className;          // 类名 (box)
        std::string idName;             // ID名 (box)
        std::vector<std::string> descendantPath; // 后代路径 [.box, button]
        int index = -1;                 // 索引 (0, 1, 2...)
        bool hasIndex = false;          // 是否有索引
    } parsed;

    EnhancedSelectorNode(const std::string& selector, Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::ENHANCED_SELECTOR, pos), 
          selectorText(selector), selectorType(EnhancedSelectorType::AUTO_DETECT) {
        parseSelector();
    }

    // 解析选择器内容
    void parseSelector();

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * 箭头操作符节点
 * 对应: -> (与.完全等价，用于明确使用CHTL JS语法)
 */
class ArrowOperatorNode : public CHTLJSASTNode {
public:
    CHTLJSASTNodePtr left;      // 左操作数
    CHTLJSASTNodePtr right;     // 右操作数

    ArrowOperatorNode(Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::ARROW_OPERATOR, pos) {}

    void setLeft(CHTLJSASTNodePtr leftNode) { left = leftNode; }
    void setRight(CHTLJSASTNodePtr rightNode) { right = rightNode; }

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * 增强监听器调用节点
 * 对应: {{selector}}->listen({ click: function, ... })
 */
class ListenCallNode : public CHTLJSASTNode {
public:
    CHTLJSASTNodePtr target;        // 目标选择器
    CHTLJSASTNodePtr config;        // 配置对象

    ListenCallNode(Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::LISTEN_CALL, pos) {}

    void setTarget(CHTLJSASTNodePtr targetNode) { target = targetNode; }
    void setConfig(CHTLJSASTNodePtr configNode) { config = configNode; }

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * 监听器配置节点
 * 对应: { click: function, mouseenter: handler, ... }
 */
class ListenConfigNode : public CHTLJSASTNode {
public:
    std::vector<CHTLJSASTNodePtr> eventHandlers;

    ListenConfigNode(Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::LISTEN_CONFIG, pos) {}

    void addEventHandler(CHTLJSASTNodePtr handler) {
        eventHandlers.push_back(handler);
    }

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * 事件处理器节点
 * 对应: click: () => { }, mouseenter: function() { }
 */
class EventHandlerNode : public CHTLJSASTNode {
public:
    std::string eventType;          // click, mouseenter, etc.
    CHTLJSASTNodePtr handler;       // 处理函数

    EventHandlerNode(const std::string& event, Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::EVENT_HANDLER, pos), eventType(event) {}

    void setHandler(CHTLJSASTNodePtr handlerNode) { handler = handlerNode; }

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * 事件委托调用节点
 * 对应: {{parent}}->delegate({ target: {{child}}, click: function, ... })
 */
class DelegateCallNode : public CHTLJSASTNode {
public:
    CHTLJSASTNodePtr parent;        // 父元素选择器
    CHTLJSASTNodePtr config;        // 委托配置对象

    DelegateCallNode(Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::DELEGATE_CALL, pos) {}

    void setParent(CHTLJSASTNodePtr parentNode) { parent = parentNode; }
    void setConfig(CHTLJSASTNodePtr configNode) { config = configNode; }

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * 委托配置节点
 * 对应: { target: {{selector}}, click: function, ... }
 */
class DelegateConfigNode : public CHTLJSASTNode {
public:
    CHTLJSASTNodePtr target;                        // target属性
    std::vector<CHTLJSASTNodePtr> eventHandlers;   // 事件处理器

    DelegateConfigNode(Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::DELEGATE_CONFIG, pos) {}

    void setTarget(CHTLJSASTNodePtr targetNode) { target = targetNode; }
    void addEventHandler(CHTLJSASTNodePtr handler) {
        eventHandlers.push_back(handler);
    }

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * 动画调用节点
 * 对应: animate({ duration: 100, easing: ease-in-out, ... })
 */
class AnimateCallNode : public CHTLJSASTNode {
public:
    CHTLJSASTNodePtr config;        // 动画配置对象

    AnimateCallNode(Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::ANIMATE_CALL, pos) {}

    void setConfig(CHTLJSASTNodePtr configNode) { config = configNode; }

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * 动画配置节点
 * 对应: { duration: 100, easing: ease-in-out, begin: {}, end: {}, when: [], ... }
 */
class AnimateConfigNode : public CHTLJSASTNode {
public:
    // 动画属性
    int duration = 0;                   // 持续时间(ms)
    std::string easing = "ease";        // 缓动函数
    CHTLJSASTNodePtr begin = nullptr;   // 起始状态
    CHTLJSASTNodePtr end = nullptr;     // 结束状态
    CHTLJSASTNodePtr when = nullptr;    // 关键帧数组
    int loop = 1;                       // 循环次数
    std::string direction = "normal";   // 播放方向
    int delay = 0;                      // 延迟(ms)
    CHTLJSASTNodePtr callback = nullptr; // 回调函数

    AnimateConfigNode(Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::ANIMATE_CONFIG, pos) {}

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * 动画关键帧节点
 * 对应: { at: 0.4, opacity: 0.5, transform: 'scale(1.1)' }
 */
class AnimateKeyframeNode : public CHTLJSASTNode {
public:
    double at;                          // 时间点 (0.0-1.0)
    std::unordered_map<std::string, std::string> properties; // CSS属性

    AnimateKeyframeNode(double atTime, Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::ANIMATE_KEYFRAME, pos), at(atTime) {}

    void addProperty(const std::string& prop, const std::string& value) {
        properties[prop] = value;
    }

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * 函数声明节点
 * 对应: function name() { } 或 function() { }
 */
class FunctionDeclarationNode : public CHTLJSASTNode {
public:
    std::string name;               // 函数名 (可为空表示匿名函数)
    std::vector<std::string> parameters; // 参数列表
    CHTLJSASTNodePtr body;          // 函数体

    FunctionDeclarationNode(const std::string& funcName = "", Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::FUNCTION_DECLARATION, pos), name(funcName) {}

    void addParameter(const std::string& param) { parameters.push_back(param); }
    void setBody(CHTLJSASTNodePtr bodyNode) { body = bodyNode; }

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * 箭头函数节点
 * 对应: () => { }, (param) => expr
 */
class ArrowFunctionNode : public CHTLJSASTNode {
public:
    std::vector<std::string> parameters; // 参数列表
    CHTLJSASTNodePtr body;          // 函数体或表达式

    ArrowFunctionNode(Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::ARROW_FUNCTION, pos) {}

    void addParameter(const std::string& param) { parameters.push_back(param); }
    void setBody(CHTLJSASTNodePtr bodyNode) { body = bodyNode; }

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * CHTL JS 标识符节点
 */
class CHTLJSIdentifierNode : public CHTLJSASTNode {
public:
    std::string name;

    CHTLJSIdentifierNode(const std::string& identifier, Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::IDENTIFIER, pos), name(identifier) {}

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * 字面量节点
 */
class LiteralNode : public CHTLJSASTNode {
public:
    std::string value;
    std::string type;               // "string", "number", "boolean", "null"

    LiteralNode(const std::string& val, const std::string& literalType, Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::LITERAL, pos), value(val), type(literalType) {}

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * 对象表达式节点
 * 对应: { prop: value, ... }
 */
class ObjectExpressionNode : public CHTLJSASTNode {
public:
    std::vector<CHTLJSASTNodePtr> properties;

    ObjectExpressionNode(Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::OBJECT_EXPRESSION, pos) {}

    void addProperty(CHTLJSASTNodePtr prop) { properties.push_back(prop); }

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * 属性节点
 * 对应: key: value
 */
class PropertyNode : public CHTLJSASTNode {
public:
    CHTLJSASTNodePtr key;           // 属性键
    CHTLJSASTNodePtr value;         // 属性值

    PropertyNode(Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::PROPERTY, pos) {}

    void setKey(CHTLJSASTNodePtr keyNode) { key = keyNode; }
    void setValue(CHTLJSASTNodePtr valueNode) { value = valueNode; }

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * 函数调用节点
 * 对应: func(args...)
 */
class CallExpressionNode : public CHTLJSASTNode {
public:
    CHTLJSASTNodePtr callee;        // 被调用的函数
    std::vector<CHTLJSASTNodePtr> arguments; // 参数列表

    CallExpressionNode(Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::CALL_EXPRESSION, pos) {}

    void setCallee(CHTLJSASTNodePtr calleeNode) { callee = calleeNode; }
    void addArgument(CHTLJSASTNodePtr arg) { arguments.push_back(arg); }

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * 成员访问节点
 * 对应: object.property 或 object[property]
 */
class MemberExpressionNode : public CHTLJSASTNode {
public:
    CHTLJSASTNodePtr object;        // 对象
    CHTLJSASTNodePtr property;      // 属性
    bool computed = false;          // 是否为计算属性 [property]

    MemberExpressionNode(Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::MEMBER_EXPRESSION, pos) {}

    void setObject(CHTLJSASTNodePtr objNode) { object = objNode; }
    void setProperty(CHTLJSASTNodePtr propNode) { property = propNode; }

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * 块语句节点
 * 对应: { ... }
 */
class BlockStatementNode : public CHTLJSASTNode {
public:
    std::vector<CHTLJSASTNodePtr> statements;

    BlockStatementNode(Position pos = Position{})
        : CHTLJSASTNode(CHTLJSASTNodeType::BLOCK_STATEMENT, pos) {}

    void addStatement(CHTLJSASTNodePtr stmt) { statements.push_back(stmt); }

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

/**
 * CHTL JS 注释节点
 */
class CHTLJSCommentNode : public CHTLJSASTNode {
public:
    std::string content;
    bool isMultiline;

    CHTLJSCommentNode(const std::string& text, bool multiline = false, Position pos = Position{})
        : CHTLJSASTNode(multiline ? CHTLJSASTNodeType::COMMENT_MULTILINE : CHTLJSASTNodeType::COMMENT_LINE, pos),
          content(text), isMultiline(multiline) {}

    std::string toString() const override;
    CHTLJSASTNodePtr clone() const override;
};

} // namespace chtl
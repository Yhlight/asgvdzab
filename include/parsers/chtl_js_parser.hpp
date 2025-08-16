#pragma once

#include "ast/chtl_ast.hpp"
#include "lexer/chtl_lexer.hpp"
#include "common/types.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <regex>

namespace chtl {

/**
 * CHTL JS增强选择器类型
 */
enum class CHTLJSSelectorType {
    TAG,            // {{button}} - HTML标签选择器
    CLASS,          // {{.box}} - 类选择器
    ID,             // {{#box}} - ID选择器
    DESCENDANT,     // {{.box button}} - 后代选择器
    INDEXED,        // {{button[0]}} - 索引选择器
    AUTO            // {{box}} - 自动判断（先tag后class/id）
};

/**
 * CHTL JS增强选择器定义
 */
struct CHTLJSSelector {
    CHTLJSSelectorType type;        // 选择器类型
    std::string selector;           // 选择器字符串
    std::string parentSelector;     // 父选择器（用于后代选择器）
    int index;                      // 索引（用于索引选择器）
    Position position;              // 位置信息
    
    CHTLJSSelector() : type(CHTLJSSelectorType::AUTO), index(-1) {}
    
    CHTLJSSelector(CHTLJSSelectorType selectorType, const std::string& sel, 
                   const Position& pos = Position{})
        : type(selectorType), selector(sel), index(-1), position(pos) {}
};

/**
 * CHTL JS方法类型
 */
enum class CHTLJSMethodType {
    STANDARD,       // 标准JS方法 (.textContent, .style.color等)
    LISTEN,         // listen方法 (->listen)
    DELEGATE,       // delegate方法 (->delegate)
    ANIMATE         // animate函数
};

/**
 * CHTL JS监听器定义
 */
struct CHTLJSListener {
    std::string eventType;          // 事件类型 (click, mouseenter等)
    std::string handlerCode;        // 处理函数代码
    Position position;              // 位置信息
    
    CHTLJSListener() = default;
    
    CHTLJSListener(const std::string& event, const std::string& handler, 
                   const Position& pos = Position{})
        : eventType(event), handlerCode(handler), position(pos) {}
};

/**
 * CHTL JS事件委托定义
 */
struct CHTLJSDelegate {
    std::vector<CHTLJSSelector> targets;    // 目标选择器列表
    std::vector<CHTLJSListener> listeners;  // 事件监听器列表
    Position position;                      // 位置信息
    
    CHTLJSDelegate() = default;
    
    CHTLJSDelegate(const Position& pos) : position(pos) {}
};

/**
 * CHTL JS动画关键帧
 */
struct CHTLJSAnimationKeyframe {
    double at;                              // 动画时间点 (0.0-1.0)
    std::unordered_map<std::string, std::string> properties; // CSS属性
    Position position;                      // 位置信息
    
    CHTLJSAnimationKeyframe() : at(0.0) {}
    
    CHTLJSAnimationKeyframe(double time, const Position& pos = Position{})
        : at(time), position(pos) {}
};

/**
 * CHTL JS动画定义
 */
struct CHTLJSAnimation {
    int duration;                           // 持续时间 (ms)
    std::string easing;                     // 缓动函数
    std::unordered_map<std::string, std::string> beginState;  // 起始状态
    std::unordered_map<std::string, std::string> endState;    // 结束状态
    std::vector<CHTLJSAnimationKeyframe> keyframes;          // 关键帧
    int loop;                               // 循环次数 (-1为无限)
    std::string direction;                  // 播放方向
    int delay;                              // 延迟 (ms)
    std::string callback;                   // 回调函数
    Position position;                      // 位置信息
    
    CHTLJSAnimation() : duration(0), loop(1), delay(0) {}
    
    CHTLJSAnimation(const Position& pos) : duration(0), loop(1), delay(0), position(pos) {}
};

/**
 * CHTL JS脚本块定义
 */
struct CHTLJSScriptBlock {
    std::vector<CHTLJSSelector> selectors;      // 增强选择器列表
    std::vector<CHTLJSListener> listeners;      // 监听器列表
    std::vector<CHTLJSDelegate> delegates;      // 事件委托列表
    std::vector<CHTLJSAnimation> animations;    // 动画列表
    std::string rawCode;                        // 原始代码
    Position position;                          // 位置信息
    
    CHTLJSScriptBlock() = default;
    
    CHTLJSScriptBlock(const Position& pos) : position(pos) {}
};

/**
 * CHTL JS解析状态
 */
enum class CHTLJSParseState {
    INITIAL,                    // 初始状态
    READING_SCRIPT_HEADER,      // 读取script头
    READING_SCRIPT_CONTENT,     // 读取script内容
    READING_SELECTOR,           // 读取增强选择器
    READING_METHOD_CALL,        // 读取方法调用
    READING_LISTEN_PARAMS,      // 读取listen参数
    READING_DELEGATE_PARAMS,    // 读取delegate参数
    READING_ANIMATE_PARAMS      // 读取animate参数
};

/**
 * CHTL JS解析上下文
 */
struct CHTLJSContext {
    CHTLJSParseState state;             // 解析状态
    int nestingLevel;                   // 嵌套层级
    bool inScriptBlock;                 // 是否在script块中
    std::string currentSelector;        // 当前选择器
    CHTLJSMethodType currentMethod;     // 当前方法类型
    std::vector<std::string> tokenBuffer; // Token缓冲区
    
    CHTLJSContext() 
        : state(CHTLJSParseState::INITIAL)
        , nestingLevel(0)
        , inScriptBlock(false)
        , currentMethod(CHTLJSMethodType::STANDARD) {}
};

/**
 * CHTL JS选择器注册表
 * 管理所有增强选择器的使用和定义
 */
class CHTLJSSelectorRegistry {
public:
    CHTLJSSelectorRegistry();
    ~CHTLJSSelectorRegistry();

    /**
     * 注册选择器
     */
    bool registerSelector(const CHTLJSSelector& selector);

    /**
     * 查找选择器
     */
    const CHTLJSSelector* findSelector(const std::string& selectorStr) const;

    /**
     * 获取所有选择器
     */
    const std::vector<CHTLJSSelector>& getAllSelectors() const;

    /**
     * 检查选择器是否有效
     */
    bool isValidSelector(const std::string& selectorStr) const;

    /**
     * 清空注册表
     */
    void clear();

private:
    std::vector<CHTLJSSelector> selectors_;
    std::unordered_map<std::string, size_t> selectorIndexMap_;
};

/**
 * CHTL JS事件委托注册表
 * 管理事件委托的父元素，避免重复绑定
 */
class CHTLJSEventDelegateRegistry {
public:
    CHTLJSEventDelegateRegistry();
    ~CHTLJSEventDelegateRegistry();

    /**
     * 注册事件委托
     */
    bool registerDelegate(const std::string& parentSelector, const CHTLJSDelegate& delegate);

    /**
     * 获取父元素的委托
     */
    std::vector<CHTLJSDelegate> getDelegates(const std::string& parentSelector) const;

    /**
     * 合并相同父元素的委托
     */
    void mergeDelegates(const std::string& parentSelector);

    /**
     * 清空注册表
     */
    void clear();

private:
    std::unordered_map<std::string, std::vector<CHTLJSDelegate>> delegateMap_;
};

/**
 * CHTL JS扫描器
 * 专门扫描和识别CHTL JS语法
 */
class CHTLJSScanner {
public:
    CHTLJSScanner();
    ~CHTLJSScanner();

    /**
     * 扫描脚本块
     */
    CHTLJSScriptBlock scanScriptBlock(const std::string& content);

    /**
     * 扫描增强选择器
     */
    std::vector<CHTLJSSelector> scanSelectors(const std::string& content);

    /**
     * 扫描listen调用
     */
    std::vector<CHTLJSListener> scanListeners(const std::string& content);

    /**
     * 扫描delegate调用
     */
    std::vector<CHTLJSDelegate> scanDelegates(const std::string& content);

    /**
     * 扫描animate调用
     */
    std::vector<CHTLJSAnimation> scanAnimations(const std::string& content);

    /**
     * 验证CHTL JS语法
     */
    bool validateCHTLJSSyntax(const std::string& content);

private:
    // 选择器解析
    CHTLJSSelector parseSelector(const std::string& selectorStr);
    CHTLJSSelectorType determineSelectorType(const std::string& selectorStr);
    
    // 监听器解析
    CHTLJSListener parseListener(const std::string& eventType, const std::string& handlerCode);
    
    // 委托解析
    CHTLJSDelegate parseDelegate(const std::string& delegateCode);
    std::vector<CHTLJSSelector> parseTargetSelectors(const std::string& targetsStr);
    
    // 动画解析
    CHTLJSAnimation parseAnimation(const std::string& animationCode);
    CHTLJSAnimationKeyframe parseKeyframe(const std::string& keyframeCode);
    
    // 工具方法
    std::string trimWhitespace(const std::string& str);
    std::vector<std::string> splitByComma(const std::string& str);
    bool isValidSelectorName(const std::string& name) const;
    bool isValidEventType(const std::string& eventType);
    std::unordered_map<std::string, std::string> parseObjectProperties(const std::string& objStr);
};

/**
 * CHTL JS状态机
 * 处理复杂的CHTL JS语法解析
 */
class CHTLJSStateMachine {
public:
    CHTLJSStateMachine();
    ~CHTLJSStateMachine();

    /**
     * 重置状态机
     */
    void reset();

    /**
     * 处理脚本token
     */
    bool processScriptToken(const Token& token, CHTLJSContext& context);

    /**
     * 获取脚本块结果
     */
    const std::vector<CHTLJSScriptBlock>& getScriptBlocks() const;

private:
    std::vector<CHTLJSScriptBlock> scriptBlocks_;
    
    // 临时状态
    CHTLJSScriptBlock currentScriptBlock_;
    std::string currentContent_;

    // 状态处理
    void handleInitialState(const Token& token, CHTLJSContext& context);
    void handleScriptHeaderState(const Token& token, CHTLJSContext& context);
    void handleScriptContentState(const Token& token, CHTLJSContext& context);
    void handleSelectorState(const Token& token, CHTLJSContext& context);
    void handleMethodCallState(const Token& token, CHTLJSContext& context);

    // 工具方法
    void addScriptBlock(const CHTLJSScriptBlock& scriptBlock);
    bool isValidCHTLJSToken(const Token& token);
    CHTLJSSelectorType detectSelectorType(const std::string& content);
};

/**
 * CHTL JS解析器
 * 严格按照CHTL语法文档实现CHTL JS功能
 */
class CHTLJSParser {
public:
    CHTLJSParser();
    ~CHTLJSParser();

    /**
     * 解析脚本块 script { ... }
     */
    ParseResult parseScriptBlock(const std::string& source, const std::string& filename = "");

    /**
     * 设置选择器注册表
     */
    void setSelectorRegistry(std::shared_ptr<CHTLJSSelectorRegistry> registry);

    /**
     * 获取选择器注册表
     */
    std::shared_ptr<CHTLJSSelectorRegistry> getSelectorRegistry() const;

    /**
     * 设置事件委托注册表
     */
    void setEventDelegateRegistry(std::shared_ptr<CHTLJSEventDelegateRegistry> registry);

    /**
     * 获取事件委托注册表
     */
    std::shared_ptr<CHTLJSEventDelegateRegistry> getEventDelegateRegistry() const;

    /**
     * 设置严格模式
     */
    void setStrictMode(bool strict);

    /**
     * 获取解析错误
     */
    const std::vector<ParseError>& getErrors() const;

    /**
     * 获取解析警告
     */
    const std::vector<ParseWarning>& getWarnings() const;

private:
    std::shared_ptr<CHTLJSSelectorRegistry> selectorRegistry_;
    std::shared_ptr<CHTLJSEventDelegateRegistry> eventDelegateRegistry_;
    CHTLJSScanner scanner_;
    CHTLJSStateMachine stateMachine_;
    CHTLLexer lexer_;
    
    bool strictMode_;
    std::vector<ParseError> errors_;
    std::vector<ParseWarning> warnings_;

    // 核心解析方法
    CHTLJSScriptBlock parseScriptBlockDefinition(const std::vector<Token>& tokens);
    
    // 选择器解析
    std::vector<CHTLJSSelector> parseSelectors(const std::string& content);
    CHTLJSSelector parseSelector(const std::string& selectorStr);
    
    // 监听器解析
    std::vector<CHTLJSListener> parseListeners(const std::string& content);
    CHTLJSListener parseListener(const std::string& listenerCode);
    
    // 委托解析
    std::vector<CHTLJSDelegate> parseDelegates(const std::string& content);
    CHTLJSDelegate parseDelegate(const std::string& delegateCode);
    
    // 动画解析
    std::vector<CHTLJSAnimation> parseAnimations(const std::string& content);
    CHTLJSAnimation parseAnimation(const std::string& animationCode);

    // AST节点创建
    CHTLASTNodePtr createScriptBlockNode(const CHTLJSScriptBlock& scriptBlock);

    // 工具方法
    void reportError(const std::string& message, const Position& position = Position{});
    void reportWarning(const std::string& message, const Position& position = Position{});
    bool isValidSelectorSyntax(const std::string& selectorStr) const;
    bool isValidMethodCall(const std::string& methodCall) const;
    std::string extractSelectorFromBraces(const std::string& content) const;
    std::string getMethodTypeString(CHTLJSMethodType type) const;
    std::string getSelectorTypeString(CHTLJSSelectorType type) const;
};

/**
 * CHTL JS工具类
 */
class CHTLJSUtils {
public:
    /**
     * 检查是否为脚本块声明
     */
    static bool isScriptBlockDeclaration(const std::string& source);

    /**
     * 检查是否包含增强选择器
     */
    static bool hasEnhancedSelectors(const std::string& source);

    /**
     * 检查是否包含CHTL JS箭头操作符
     */
    static bool hasCHTLJSArrow(const std::string& source);

    /**
     * 检查是否包含listen调用
     */
    static bool hasListenCall(const std::string& source);

    /**
     * 检查是否包含delegate调用
     */
    static bool hasDelegateCall(const std::string& source);

    /**
     * 检查是否包含animate调用
     */
    static bool hasAnimateCall(const std::string& source);

    /**
     * 提取增强选择器
     */
    static std::vector<std::string> extractEnhancedSelectors(const std::string& source);

    /**
     * 验证选择器语法
     */
    static bool validateSelectorSyntax(const std::string& selector);

    /**
     * 验证事件类型
     */
    static bool validateEventType(const std::string& eventType);

    /**
     * 标准化选择器
     */
    static std::string normalizeSelector(const std::string& selector);

    /**
     * 检查选择器类型
     */
    static CHTLJSSelectorType detectSelectorType(const std::string& selector);

    /**
     * 格式化选择器
     */
    static std::string formatSelector(const CHTLJSSelector& selector);

    /**
     * 格式化监听器
     */
    static std::string formatListener(const CHTLJSListener& listener);

    /**
     * 格式化委托
     */
    static std::string formatDelegate(const CHTLJSDelegate& delegate);

    /**
     * 格式化动画
     */
    static std::string formatAnimation(const CHTLJSAnimation& animation);
};

} // namespace chtl
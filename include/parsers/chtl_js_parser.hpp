#pragma once

#include "ast/chtl_js_ast.hpp"
#include "lexer/pure_chtl_js_tokens.hpp"
#include "common/types.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace chtl {

/**
 * CHTL JS解析器错误信息
 */
struct CHTLJSParseError {
    std::string message;        // 错误消息
    Position position;          // 错误位置
    std::string context;        // 错误上下文
    std::string suggestion;     // 修复建议
};

/**
 * CHTL JS解析结果
 */
struct CHTLJSParseResult {
    bool success;                               // 解析是否成功
    CHTLJSASTNodePtr ast;                      // 解析得到的AST
    std::vector<CHTLJSParseError> errors;      // 错误列表
    std::vector<std::string> warnings;         // 警告列表
    
    CHTLJSParseResult() : success(false) {}
};

/**
 * CHTL JS解析器配置
 */
struct CHTLJSParserConfig {
    bool enableStrictMode;              // 严格模式，不允许语法扩展
    bool enableWarnings;                // 启用警告
    bool enableCommentsInAST;           // 是否在AST中保留注释
    bool enableArrowOperator;           // 启用箭头操作符 ->
    
    CHTLJSParserConfig() 
        : enableStrictMode(true)
        , enableWarnings(true) 
        , enableCommentsInAST(false)
        , enableArrowOperator(true) {}
};

/**
 * CHTL JS解析器类
 * 严格按照CHTL语法文档.md中CHTL JS部分实现
 * 支持完整的CHTL JS语法，包括：
 * - 增强选择器 ({{selector}})
 * - 箭头操作符 (->)
 * - 增强监听器 (listen)
 * - 事件委托 (delegate)
 * - 动画系统 (animate)
 * - JavaScript标准语法
 * 
 * 注意：CHTL JS不是JS的超集，只支持特定的扩展语法
 */
class CHTLJSParser {
public:
    explicit CHTLJSParser(const CHTLJSParserConfig& config = CHTLJSParserConfig());
    ~CHTLJSParser() = default;

    /**
     * 解析CHTL JS源代码
     * @param source CHTL JS源代码
     * @param filename 文件名（用于错误报告）
     * @return 解析结果
     */
    CHTLJSParseResult parse(const std::string& source, const std::string& filename = "");

    /**
     * 解析Token序列
     * @param tokens Token序列
     * @param filename 文件名
     * @return 解析结果
     */
    CHTLJSParseResult parseTokens(const std::vector<PureCHTLJSToken>& tokens, const std::string& filename = "");

private:
    CHTLJSParserConfig config_;
    std::vector<PureCHTLJSToken> tokens_;
    size_t currentTokenIndex_;
    std::string filename_;
    std::vector<CHTLJSParseError> errors_;
    std::vector<std::string> warnings_;

    // ===== Token管理 =====
    const PureCHTLJSToken& currentToken() const;
    const PureCHTLJSToken& peekToken(size_t offset = 1) const;
    void advance();
    bool isAtEnd() const;
    bool match(PureCHTLJSTokenType type);
    bool check(PureCHTLJSTokenType type) const;
    
    // ===== 错误处理 =====
    void reportError(const std::string& message, const std::string& suggestion = "");
    void reportWarning(const std::string& message);
    void synchronize(); // 错误恢复
    
    // ===== 主要解析方法 =====
    CHTLJSASTNodePtr parseProgram();
    CHTLJSASTNodePtr parseStatement();
    CHTLJSASTNodePtr parseExpression();
    
    // ===== CHTL JS特有语法解析 =====
    CHTLJSASTNodePtr parseEnhancedSelector();       // {{selector}}
    CHTLJSASTNodePtr parseArrowOperator();          // ->
    CHTLJSASTNodePtr parseListenCall();             // listen({...})
    CHTLJSASTNodePtr parseDelegateCall();           // delegate({...})
    CHTLJSASTNodePtr parseAnimateCall();            // animate({...})
    
    // ===== 增强选择器解析 =====
    CHTLJSASTNodePtr parseClassSelector();          // .className
    CHTLJSASTNodePtr parseIdSelector();             // #idName
    CHTLJSASTNodePtr parseTagSelector();            // tagName
    CHTLJSASTNodePtr parseDescendantSelector();     // .parent child
    CHTLJSASTNodePtr parseIndexedSelector();        // selector[index]
    
    // ===== 监听器配置解析 =====
    CHTLJSASTNodePtr parseListenConfig();           // listen配置对象
    CHTLJSASTNodePtr parseEventHandler();           // 事件处理函数
    
    // ===== 委托配置解析 =====
    CHTLJSASTNodePtr parseDelegateConfig();         // delegate配置对象
    CHTLJSASTNodePtr parseDelegateTarget();         // target选择器
    
    // ===== 动画配置解析 =====
    CHTLJSASTNodePtr parseAnimateConfig();          // animate配置对象
    CHTLJSASTNodePtr parseAnimateKeyframe();        // 动画关键帧
    CHTLJSASTNodePtr parseAnimateTimeline();        // when数组
    
    // ===== JavaScript标准语法解析 =====
    CHTLJSASTNodePtr parseFunctionDeclaration();    // function声明
    CHTLJSASTNodePtr parseArrowFunction();          // 箭头函数
    CHTLJSASTNodePtr parseCallExpression();         // 函数调用
    CHTLJSASTNodePtr parseMemberExpression();       // 成员访问
    CHTLJSASTNodePtr parseObjectExpression();       // 对象字面量
    CHTLJSASTNodePtr parseArrayExpression();        // 数组字面量
    CHTLJSASTNodePtr parseBlockStatement();         // 代码块
    
    // ===== 字面量解析 =====
    CHTLJSASTNodePtr parseStringLiteral();          // 字符串字面量
    CHTLJSASTNodePtr parseNumberLiteral();          // 数字字面量
    CHTLJSASTNodePtr parseBooleanLiteral();         // 布尔字面量
    CHTLJSASTNodePtr parseIdentifier();             // 标识符
    
    // ===== 工具方法 =====
    std::string parseIdentifierString();            // 解析标识符字符串
    std::string parseStringLiteralValue();          // 解析字符串值
    std::string parseSelector();                    // 解析CSS选择器
    EnhancedSelectorType determineSelectorType(const std::string& selector); // 确定选择器类型
    
    // ===== 验证方法 =====
    bool isValidSelector(const std::string& selector) const;
    bool isValidEventType(const std::string& eventType) const;
    bool isValidCSSProperty(const std::string& property) const;
    bool isValidEasingFunction(const std::string& easing) const;
    
    // ===== 优先级和关联性 =====
    int getOperatorPrecedence(PureCHTLJSTokenType type) const;
    bool isRightAssociative(PureCHTLJSTokenType type) const;
    
    // ===== 位置信息 =====
    Position getCurrentPosition() const;
    void savePosition(CHTLJSASTNodePtr node) const;
    
    // ===== CHTL JS特征检测 =====
    bool isEnhancedSelector() const;                // 检测 {{...}}
    bool isArrowOperator() const;                   // 检测 ->
    bool isListenCall() const;                      // 检测 listen(
    bool isDelegateCall() const;                    // 检测 delegate(
    bool isAnimateCall() const;                     // 检测 animate(
};

} // namespace chtl
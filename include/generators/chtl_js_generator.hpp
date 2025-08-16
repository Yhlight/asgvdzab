#pragma once

#include "ast/chtl_js_ast.hpp"
#include "common/types.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <sstream>

namespace chtl {

/**
 * CHTL JS生成器错误信息
 */
struct CHTLJSGenerateError {
    std::string message;        // 错误消息
    Position position;          // 错误位置
    std::string context;        // 错误上下文
    std::string suggestion;     // 修复建议
};

/**
 * CHTL JS生成结果
 */
struct CHTLJSGenerateResult {
    bool success;                                   // 生成是否成功
    std::string javascript;                        // 生成的JavaScript代码
    std::string delegateRegistry;                  // 事件委托注册表代码
    std::string animationHelpers;                  // 动画助手函数
    std::vector<CHTLJSGenerateError> errors;       // 错误列表
    std::vector<std::string> warnings;             // 警告列表
    
    CHTLJSGenerateResult() : success(false) {}
};

/**
 * CHTL JS生成器配置
 */
struct CHTLJSGeneratorConfig {
    bool enablePrettyPrint;             // 启用格式化输出
    bool enableMinification;            // 启用最小化
    bool enableComments;                // 在输出中保留注释
    bool enableSourceMap;               // 生成源映射
    std::string indentStyle;            // 缩进样式 ("  ", "\t")
    int indentSize;                     // 缩进大小
    bool enableArrowFunctions;          // 优先使用箭头函数
    bool enableES6Modules;              // 生成ES6模块
    bool enableStrictMode;              // 启用严格模式
    std::string targetVersion;          // 目标JS版本 ("es5", "es6", "es2017")
    
    CHTLJSGeneratorConfig() 
        : enablePrettyPrint(true)
        , enableMinification(false)
        , enableComments(false)
        , enableSourceMap(false)
        , indentStyle("  ")
        , indentSize(2)
        , enableArrowFunctions(true)
        , enableES6Modules(false)
        , enableStrictMode(true)
        , targetVersion("es6") {}
};

/**
 * 事件委托信息
 */
struct DelegateInfo {
    std::string parentSelector;        // 父元素选择器
    std::vector<std::string> targetSelectors; // 目标选择器列表
    std::unordered_map<std::string, std::string> eventHandlers; // 事件处理函数
    Position sourcePosition;           // 源代码位置
};

/**
 * 动画信息
 */
struct AnimationInfo {
    std::string name;                  // 动画名称
    int duration;                      // 持续时间
    std::string easing;                // 缓动函数
    std::vector<std::pair<float, std::string>> keyframes; // 关键帧
    Position sourcePosition;           // 源代码位置
};

/**
 * CHTL JS生成器类
 * 严格按照CHTL语法文档.md中CHTL JS部分将AST转换为JavaScript
 * 负责：
 * - 增强选择器转换为DOM查询
 * - 箭头操作符处理
 * - listen方法展开为事件监听器
 * - delegate方法展开为事件委托
 * - animate方法展开为动画代码
 * - JavaScript标准语法生成
 */
class CHTLJSGenerator {
public:
    explicit CHTLJSGenerator(const CHTLJSGeneratorConfig& config = CHTLJSGeneratorConfig());
    ~CHTLJSGenerator() = default;

    /**
     * 生成JavaScript
     * @param ast CHTL JS AST根节点
     * @param filename 文件名（用于错误报告）
     * @return 生成结果
     */
    CHTLJSGenerateResult generate(CHTLJSASTNodePtr ast, const std::string& filename = "");

private:
    CHTLJSGeneratorConfig config_;
    std::string filename_;
    std::vector<CHTLJSGenerateError> errors_;
    std::vector<std::string> warnings_;
    
    // 输出流
    std::ostringstream jsStream_;
    std::ostringstream delegateStream_;
    std::ostringstream animationStream_;
    
    // 状态管理
    int indentLevel_;
    bool inArrowFunction_;
    bool inObjectLiteral_;
    
    // 委托和动画管理
    std::vector<DelegateInfo> delegates_;
    std::vector<AnimationInfo> animations_;
    int delegateCounter_;
    int animationCounter_;

    // ===== 错误处理 =====
    void reportError(const std::string& message, const Position& pos, const std::string& suggestion = "");
    void reportWarning(const std::string& message, const Position& pos);
    
    // ===== 主要生成方法 =====
    void generateNode(CHTLJSASTNodePtr node);
    void generateStatement(CHTLJSASTNodePtr node);
    void generateExpression(CHTLJSASTNodePtr node);
    
    // ===== CHTL JS特有语法生成 =====
    void generateEnhancedSelector(CHTLJSASTNodePtr node);
    void generateArrowOperator(CHTLJSASTNodePtr node);
    void generateListenCall(CHTLJSASTNodePtr node);
    void generateDelegateCall(CHTLJSASTNodePtr node);
    void generateAnimateCall(CHTLJSASTNodePtr node);
    
    // ===== 选择器转换 =====
    std::string convertSelector(const std::string& selector, EnhancedSelectorType type);
    std::string convertClassSelector(const std::string& selector);
    std::string convertIdSelector(const std::string& selector);
    std::string convertTagSelector(const std::string& selector);
    std::string convertDescendantSelector(const std::string& selector);
    std::string convertIndexedSelector(const std::string& selector, int index);
    
    // ===== 监听器生成 =====
    void generateListenConfig(CHTLJSASTNodePtr node);
    void generateEventHandler(CHTLJSASTNodePtr node);
    std::string generateEventBinding(const std::string& eventType, const std::string& handler);
    
    // ===== 委托生成 =====
    void generateDelegateConfig(CHTLJSASTNodePtr node);
    void generateDelegateTarget(CHTLJSASTNodePtr node);
    std::string generateDelegateRegistry();
    std::string generateDelegateHandler(const DelegateInfo& delegate);
    
    // ===== 动画生成 =====
    void generateAnimateConfig(CHTLJSASTNodePtr node);
    void generateAnimateKeyframe(CHTLJSASTNodePtr node);
    void generateAnimateTimeline(CHTLJSASTNodePtr node);
    std::string generateAnimationHelpers();
    std::string generateAnimationFunction(const AnimationInfo& animation);
    std::string generateKeyframeCSS(const AnimationInfo& animation);
    
    // ===== JavaScript标准语法生成 =====
    void generateFunctionDeclaration(CHTLJSASTNodePtr node);
    void generateArrowFunction(CHTLJSASTNodePtr node);
    void generateCallExpression(CHTLJSASTNodePtr node);
    void generateMemberExpression(CHTLJSASTNodePtr node);
    void generateObjectExpression(CHTLJSASTNodePtr node);
    void generateArrayExpression(CHTLJSASTNodePtr node);
    void generateBlockStatement(CHTLJSASTNodePtr node);
    void generateProperty(CHTLJSASTNodePtr node);
    
    // ===== 字面量生成 =====
    void generateStringLiteral(CHTLJSASTNodePtr node);
    void generateNumberLiteral(CHTLJSASTNodePtr node);
    void generateBooleanLiteral(CHTLJSASTNodePtr node);
    void generateIdentifier(CHTLJSASTNodePtr node);
    
    // ===== 注释生成 =====
    void generateComment(CHTLJSASTNodePtr node);
    
    // ===== 工具方法 =====
    void writeIndent();
    void increaseIndent();
    void decreaseIndent();
    void writeLine(const std::string& line);
    void write(const std::string& text);
    std::string escapeString(const std::string& str);
    std::string escapeRegex(const std::string& regex);
    
    // ===== 选择器工具 =====
    bool isSimpleSelector(const std::string& selector);
    bool needsQuerySelectorAll(const std::string& selector);
    std::string optimizeSelector(const std::string& selector);
    
    // ===== 事件工具 =====
    bool isValidEventType(const std::string& eventType);
    std::string normalizeEventType(const std::string& eventType);
    
    // ===== 动画工具 =====
    std::string convertEasingFunction(const std::string& easing);
    std::string generateUniqueAnimationName();
    std::string formatDuration(int milliseconds);
    
    // ===== CSS属性工具 =====
    std::string convertCSSProperty(const std::string& property);
    std::string formatCSSValue(const std::string& property, const std::string& value);
    
    // ===== 优化工具 =====
    std::string optimizeGeneratedCode(const std::string& code);
    std::string removeUnnecessarySpaces(const std::string& code);
    std::string combineSelectors(const std::vector<std::string>& selectors);
    
    // ===== 验证方法 =====
    bool validateSelector(const std::string& selector);
    bool validateEventType(const std::string& eventType);
    bool validateCSSProperty(const std::string& property);
    bool validateAnimationConfig(CHTLJSASTNodePtr node);
    
    // ===== 后处理 =====
    std::string postProcessJS(const std::string& js);
    std::string minifyJS(const std::string& js);
    std::string addModuleWrapper(const std::string& js);
    std::string addStrictModeDirective(const std::string& js);
    
    // ===== 模板生成 =====
    std::string generateDOMReadyWrapper();
    std::string generateEventDelegateTemplate();
    std::string generateAnimationFrameTemplate();
    std::string generateSelectorCacheTemplate();
};

} // namespace chtl
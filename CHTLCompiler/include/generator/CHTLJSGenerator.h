#pragma once

#include "ast/CHTLJSASTNode.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

namespace chtl {

// CHTL JS生成器配置
struct CHTLJSGeneratorConfig {
    bool enable_formatting;         // 是否启用代码格式化
    bool enable_minification;       // 是否启用代码压缩
    bool preserve_comments;         // 是否保留注释
    bool enable_debug;              // 是否启用调试模式
    std::string indent_style;       // 缩进样式 (space, tab)
    size_t indent_size;             // 缩进大小
    bool enable_enhanced_selectors; // 是否启用增强选择器
    bool enable_chain_access;       // 是否启用链式访问
    bool enable_listen;             // 是否启用增强监听器
    bool enable_delegate;           // 是否启用事件委托
    bool enable_animate;            // 是否启用动画
    bool enable_polyfills;          // 是否启用polyfill
    bool enable_source_maps;        // 是否启用源码映射
    std::string target_es_version;  // 目标ES版本 (ES5, ES6, ES2020等)
    
    CHTLJSGeneratorConfig() : 
        enable_formatting(true), enable_minification(false), preserve_comments(true),
        enable_debug(false), indent_style("space"), indent_size(2),
        enable_enhanced_selectors(true), enable_chain_access(true), enable_listen(true),
        enable_delegate(true), enable_animate(true), enable_polyfills(false),
        enable_source_maps(false), target_es_version("ES6") {}
};

// CHTL JS生成器统计信息
struct CHTLJSGeneratorStats {
    size_t total_nodes_processed;
    size_t enhanced_selectors_generated;
    size_t chain_accesses_generated;
    size_t listen_declarations_generated;
    size_t delegate_declarations_generated;
    size_t animate_declarations_generated;
    size_t function_declarations_generated;
    size_t variable_declarations_generated;
    size_t generation_time_ms;
    size_t output_size_bytes;
};

// CHTL JS生成器类
class CHTLJSGenerator {
public:
    CHTLJSGenerator(const CHTLJSGeneratorConfig& config = CHTLJSGeneratorConfig());
    ~CHTLJSGenerator();
    
    // 生成方法
    std::string generateJavaScript(const ASTNode* root);
    std::string generateEnhancedSelectors(const ASTNode* root);
    std::string generateChainAccesses(const ASTNode* root);
    std::string generateListenDeclarations(const ASTNode* root);
    std::string generateDelegateDeclarations(const ASTNode* root);
    std::string generateAnimateDeclarations(const ASTNode* root);
    
    // 文件生成方法
    bool generateJavaScriptFile(const ASTNode* root, const std::string& file_path);
    bool generateSourceMapFile(const ASTNode* root, const std::string& file_path);
    
    // 配置管理
    void setConfig(const CHTLJSGeneratorConfig& config);
    CHTLJSGeneratorConfig getConfig() const;
    
    // 统计信息
    CHTLJSGeneratorStats getStats() const;
    void resetStats();
    
    // 调试和诊断
    std::string getDebugInfo() const;
    void enableDebugMode(bool enable);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 核心生成方法
    std::string generateFromNode(const ASTNode* node);
    std::string generateScriptBlock(const ASTNode* script_block);
    
    // CHTL JS特定语法生成
    std::string generateEnhancedSelector(const EnhancedSelectorNode* selector);
    std::string generateChainAccess(const ChainAccessNode* chain_access);
    std::string generateListenDeclaration(const ListenDeclarationNode* listen_decl);
    std::string generateDelegateDeclaration(const DelegateDeclarationNode* delegate_decl);
    std::string generateAnimateDeclaration(const AnimateDeclarationNode* animate_decl);
    std::string generateAnimationKeyframe(const AnimationKeyframeNode* keyframe);
    
    // JavaScript语法生成
    std::string generateFunctionDeclaration(const ASTNode* function_decl);
    std::string generateFunctionParameter(const ASTNode* parameter);
    std::string generateFunctionBody(const ASTNode* function_body);
    std::string generateVariableDeclaration(const ASTNode* variable_decl);
    std::string generateExpressionStatement(const ASTNode* expression_stmt);
    std::string generateIfStatement(const ASTNode* if_stmt);
    std::string generateForStatement(const ASTNode* for_stmt);
    std::string generateWhileStatement(const ASTNode* while_stmt);
    std::string generateReturnStatement(const ASTNode* return_stmt);
    std::string generateFunctionCall(const ASTNode* function_call);
    std::string generateArgumentList(const ASTNode* argument_list);
    std::string generateObjectLiteral(const ASTNode* object_literal);
    std::string generatePropertyAssignment(const ASTNode* property_assignment);
    std::string generateArrayLiteral(const ASTNode* array_literal);
    std::string generateBinaryExpression(const ASTNode* binary_expr);
    std::string generateUnaryExpression(const ASTNode* unary_expr);
    std::string generateIdentifier(const ASTNode* identifier);
    std::string generateLiteralValue(const ASTNode* literal_value);
    
    // 增强选择器转换
    std::string convertEnhancedSelectorToDOM(const std::string& selector);
    std::string processArrayAccess(const std::string& selector, const std::string& index);
    std::string validateCSSSelector(const std::string& selector);
    
    // 链式访问转换
    std::string convertChainAccessToMethodCall(const std::string& access_type, 
                                              const std::string& method_name,
                                              const std::vector<std::unique_ptr<ASTNode>>& arguments);
    std::string generateMethodArguments(const std::vector<std::unique_ptr<ASTNode>>& arguments);
    
    // 事件监听器生成
    std::string convertListenToAddEventListener(const std::string& target_selector,
                                               const std::unordered_map<std::string, std::unique_ptr<ASTNode>>& event_handlers);
    std::string generateEventHandler(const std::string& event_type, const ASTNode* handler);
    
    // 事件委托生成
    std::string convertDelegateToEventDelegation(const std::string& parent_selector,
                                                const std::vector<std::unique_ptr<ASTNode>>& target_selectors,
                                                const std::unordered_map<std::string, std::unique_ptr<ASTNode>>& event_handlers);
    std::string generateDelegationLogic(const std::string& parent_selector,
                                       const std::vector<std::unique_ptr<ASTNode>>& target_selectors);
    
    // 动画生成
    std::string convertAnimateToRequestAnimationFrame(const AnimateDeclarationNode* animate_decl);
    std::string generateAnimationLoop(const AnimateDeclarationNode* animate_decl);
    std::string generateAnimationKeyframes(const std::vector<std::unique_ptr<ASTNode>>& when_states);
    std::string generateAnimationTiming(const AnimateDeclarationNode* animate_decl);
    std::string generateAnimationEasing(const std::string& easing_function);
    
    // 辅助生成方法
    std::string generateComment(const ASTNode* comment);
    std::string generateCSSSelector(const std::string& selector);
    std::string generateArrayAccess(const std::string& index);
    std::string generateEventType(const std::string& event_type);
    std::string generateAnimationProperty(const std::string& property);
    std::string generateAnimationValue(const std::string& value);
    std::string generateAnimationTime(const std::string& time);
    
    // 格式化方法
    std::string formatJavaScript(const std::string& js);
    std::string addIndentation(const std::string& code, size_t level);
    std::string normalizeWhitespace(const std::string& code);
    
    // 压缩方法
    std::string minifyJavaScript(const std::string& js);
    std::string removeComments(const std::string& js);
    std::string removeWhitespace(const std::string& js);
    
    // 验证方法
    bool validateJavaScriptOutput(const std::string& js) const;
    bool validateEnhancedSelector(const std::string& selector) const;
    bool validateChainAccess(const std::string& chain_access) const;
    bool validateEventHandler(const std::string& handler) const;
    bool validateAnimationDeclaration(const std::string& animation) const;
    
    // 工具方法
    std::string escapeJavaScript(const std::string& text);
    std::string generateUniqueId(const std::string& prefix);
    std::string sanitizeIdentifier(const std::string& identifier);
    std::string convertToValidJS(const std::string& text);
    
    // 依赖管理
    void collectDependencies(const ASTNode* node);
    std::vector<std::string> getDependencies() const;
    void resolveDependencies();
    
    // 错误处理
    void addGenerationError(const std::string& error_message);
    bool hasGenerationErrors() const;
    std::vector<std::string> getGenerationErrors() const;
    void clearGenerationErrors();
    
    // 源码映射生成
    std::string generateSourceMap(const ASTNode* root, const std::string& output_file);
    std::string generateSourceMapContent(const std::string& source_file, 
                                       const std::string& output_file,
                                       const std::vector<std::pair<size_t, size_t>>& mappings);
};

} // namespace chtl
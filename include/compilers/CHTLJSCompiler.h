#pragma once

#include "../common/Types.h"
#include "../common/Token.h"
#include "../common/AST.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace CHTL {

/**
 * CHTL JS编译器 - 手写实现
 * 负责编译CHTL JS代码片段，处理script相关的内容
 * 处理：增强选择器、增强监听器、事件委托、动画、虚对象等CHTL JS特有语法
 */
class CHTLJSCompiler {
private:
    Configuration config;
    std::vector<TokenPtr> tokens;
    size_t current_token_index;
    
    // 解析状态
    ASTNodePtr current_ast;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    
    // 虚对象管理
    std::map<std::string, ASTNodePtr> virtual_objects;
    std::map<std::string, std::vector<std::string>> virtual_object_functions;
    
    // 全局函数注册表(用于虚对象函数管理)
    std::map<std::string, std::string> global_function_registry;
    int function_counter;
    
    // 事件委托注册表
    std::map<std::string, std::vector<std::string>> event_delegation_registry;
    
    // CSS选择器到类名/ID的映射(与CHTL编译器共享)
    std::map<std::string, std::string> selector_mapping;
    
    // Token操作方法
    TokenPtr currentToken() const;
    TokenPtr peekToken(size_t offset = 1) const;
    TokenPtr advance();
    bool match(TokenType type);
    bool check(TokenType type) const;
    bool isAtEnd() const;
    
    // 解析方法
    ASTNodePtr parseProgram();
    ASTNodePtr parseStatement();
    ASTNodePtr parseExpression();
    ASTNodePtr parseVirObject();
    ASTNodePtr parseEnhancedSelector();
    ASTNodePtr parseListenBlock();
    ASTNodePtr parseDelegateBlock();
    ASTNodePtr parseAnimateBlock();
    
    // 增强选择器解析
    std::string parseSelector();
    std::string parseSelectorWithIndex();
    bool isValidSelector(const std::string& selector) const;
    
    // 虚对象处理
    void registerVirtualObject(const std::string& name, ASTNodePtr node);
    std::string generateVirtualFunction(const std::string& vir_name, const std::string& func_name);
    std::string resolveVirtualCall(const std::string& vir_name, const std::string& func_name);
    
    // 事件委托处理
    void registerEventDelegation(const std::string& parent_selector, const std::string& child_selector);
    std::string generateDelegationCode(ASTNodePtr delegate_node);
    
    // 动画处理
    std::string generateAnimationCode(ASTNodePtr animate_node);
    std::string parseAnimationKeyframes(ASTNodePtr when_block);
    
    // 代码生成
    std::string generateJS(ASTNodePtr node);
    std::string generateEnhancedSelectorCode(const std::string& selector);
    std::string generateListenCode(ASTNodePtr listen_node);
    std::string generateINeverAwayCode(ASTNodePtr vir_node);
    std::string generateSelectorCode(const std::string& selector);
    
    // CHTL JS特有的处理方法
    std::string processEnhancedSelectors(const std::string& js_code);
    std::string processArrowOperators(const std::string& js_code);
    void parseVirtualObjectFunctions(const std::string& vir_name, const std::string& params);
    void parseListenParameters(ASTNodePtr listen_node, const std::string& params);
    void parseINeverAwayParameters(ASTNodePtr vir_node, const std::string& params);
    
    // 错误处理
    void addError(const std::string& message);
    void addWarning(const std::string& message);
    std::string formatError(const std::string& message, TokenPtr token = nullptr);
    
public:
    CHTLJSCompiler();
    explicit CHTLJSCompiler(const Configuration& cfg);
    
    // 设置配置
    void setConfiguration(const Configuration& cfg);
    
    // 主要编译方法
    CompileResultPtr compile(const std::string& chtl_js_code);
    CompileResultPtr compile(const std::vector<TokenPtr>& token_stream);
    
    // 解析方法
    ASTNodePtr parse(const std::vector<TokenPtr>& token_stream);
    
    // 代码生成方法
    std::string generateCode(ASTNodePtr ast);
    
    // 选择器映射管理(与CHTL编译器协作)
    void setSelectorMapping(const std::map<std::string, std::string>& mapping);
    std::map<std::string, std::string> getSelectorMapping() const { return selector_mapping; }
    
    // 虚对象管理
    std::vector<std::string> getVirtualObjectNames() const;
    std::string getVirtualObjectFunction(const std::string& vir_name, const std::string& func_name) const;
    
    // 全局函数管理
    std::vector<std::string> getGeneratedGlobalFunctions() const;
    
    // 事件委托管理
    std::string getEventDelegationCode() const;
    
    // 错误和警告
    std::vector<std::string> getErrors() const { return errors; }
    std::vector<std::string> getWarnings() const { return warnings; }
    void clearErrors() { errors.clear(); }
    void clearWarnings() { warnings.clear(); }
    
    // 重置编译器状态
    void reset();
    
    // 调试方法
    void dumpAST(ASTNodePtr node = nullptr) const;
    void setDebugMode(bool enabled);
};

} // namespace CHTL
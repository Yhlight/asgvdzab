#pragma once

#include "dispatcher/CompilerDispatcher.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace chtl {

// CHTL JS AST节点类型
enum class CHTLJSNodeType {
    ROOT,
    SCRIPT_BLOCK,
    LISTEN_CALL,
    DELEGATE_CALL,
    ANIMATE_CALL,
    ENHANCED_SELECTOR,
    FUNCTION_CALL,
    VARIABLE_DECLARATION,
    EXPRESSION
};

// CHTL JS AST节点基类
class CHTLJSNode {
public:
    CHTLJSNodeType type;
    std::string name;
    std::string value;
    std::vector<std::unique_ptr<CHTLJSNode>> children;
    std::unordered_map<std::string, std::string> attributes;
    
    CHTLJSNode(CHTLJSNodeType t, const std::string& n = "") 
        : type(t), name(n) {}
    
    virtual ~CHTLJSNode() = default;
    
    void addChild(std::unique_ptr<CHTLJSNode> child) {
        if (child) {
            children.push_back(std::move(child));
        }
    }
    
    void addAttribute(const std::string& key, const std::string& value) {
        attributes[key] = value;
    }
};

// CHTL JS编译器类
class CHTLJSCompiler : public ICompiler {
public:
    CHTLJSCompiler();
    ~CHTLJSCompiler() override;
    
    // 实现ICompiler接口
    CompilationResult compile(const CodeFragment& fragment) override;
    std::string getName() const override { return "CHTL JS编译器"; }
    
    // 设置编译选项
    void setOptions(const std::unordered_map<std::string, std::string>& options);
    
    // 获取编译统计信息
    struct CompileStats {
        size_t totalNodes;
        size_t scriptBlocks;
        size_t listenCalls;
        size_t delegateCalls;
        size_t animateCalls;
        size_t enhancedSelectors;
        size_t functionCalls;
    };
    
    CompileStats getCompileStats() const;
    
    // 获取错误信息
    std::vector<std::string> getErrors() const;
    
    // 清除错误信息
    void clearErrors();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 私有方法
    std::unique_ptr<CHTLJSNode> parseCHTLJS(const std::string& source);
    std::string generateJavaScript(const CHTLJSNode* node);
    
    // 解析方法
    std::unique_ptr<CHTLJSNode> parseScriptBlock(const std::string& source, size_t& pos);
    std::unique_ptr<CHTLJSNode> parseListenCall(const std::string& source, size_t& pos);
    std::unique_ptr<CHTLJSNode> parseDelegateCall(const std::string& source, size_t& pos);
    std::unique_ptr<CHTLJSNode> parseAnimateCall(const std::string& source, size_t& pos);
    std::unique_ptr<CHTLJSNode> parseEnhancedSelector(const std::string& source, size_t& pos);
    std::unique_ptr<CHTLJSNode> parseFunctionCall(const std::string& source, size_t& pos);
    
    // 辅助方法
    void skipWhitespace(const std::string& source, size_t& pos);
    std::string extractIdentifier(const std::string& source, size_t& pos);
    std::string extractString(const std::string& source, size_t& pos);
    std::string extractEnhancedSelector(const std::string& source, size_t& pos);
    bool isKeyword(const std::string& word);
    void addError(const std::string& error);
    
    // 代码生成辅助方法
    std::string generateListenCode(const CHTLJSNode* node);
    std::string generateDelegateCode(const CHTLJSNode* node);
    std::string generateAnimateCode(const CHTLJSNode* node);
    std::string generateEnhancedSelectorCode(const CHTLJSNode* node);
};

} // namespace chtl
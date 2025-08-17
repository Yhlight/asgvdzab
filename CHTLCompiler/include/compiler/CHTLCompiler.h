#pragma once

#include "dispatcher/CompilerDispatcher.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace chtl {

// CHTL AST节点类型
enum class CHTLNodeType {
    ROOT,
    ELEMENT,
    TEXT,
    ATTRIBUTE,
    STYLE_BLOCK,
    TEMPLATE,
    CUSTOM,
    ORIGIN,
    IMPORT,
    NAMESPACE,
    CONFIGURATION
};

// CHTL AST节点基类
class CHTLASTNode {
public:
    CHTLNodeType type;
    std::string name;
    std::string value;
    std::vector<std::unique_ptr<CHTLASTNode>> children;
    std::unordered_map<std::string, std::string> attributes;
    
    CHTLASTNode(CHTLNodeType t, const std::string& n = "") 
        : type(t), name(n) {}
    
    virtual ~CHTLASTNode() = default;
    
    void addChild(std::unique_ptr<CHTLASTNode> child) {
        if (child) {
            children.push_back(std::move(child));
        }
    }
    
    void addAttribute(const std::string& key, const std::string& value) {
        attributes[key] = value;
    }
};

// CHTL编译器类
class CHTLCompiler : public ICompiler {
public:
    CHTLCompiler();
    ~CHTLCompiler() override;
    
    // 实现ICompiler接口
    CompilationResult compile(const CodeFragment& fragment) override;
    std::string getName() const override { return "CHTL编译器"; }
    
    // 设置编译选项
    void setOptions(const std::unordered_map<std::string, std::string>& options);
    
    // 获取编译统计信息
    struct CompileStats {
        size_t totalNodes;
        size_t elements;
        size_t textNodes;
        size_t styleBlocks;
        size_t templates;
        size_t customBlocks;
        size_t originBlocks;
        size_t imports;
        size_t namespaces;
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
    std::unique_ptr<CHTLASTNode> parseCHTL(const std::string& source);
    std::string generateHTML(const CHTLASTNode* node);
    std::string generateCSS(const CHTLASTNode* node);
    std::string generateJavaScript(const CHTLASTNode* node);
    
    // 解析方法
    std::unique_ptr<CHTLASTNode> parseElement(const std::string& source, size_t& pos);
    std::unique_ptr<CHTLASTNode> parseStyleBlock(const std::string& source, size_t& pos);
    std::unique_ptr<CHTLASTNode> parseTemplate(const std::string& source, size_t& pos);
    std::unique_ptr<CHTLASTNode> parseCustom(const std::string& source, size_t& pos);
    std::unique_ptr<CHTLASTNode> parseOrigin(const std::string& source, size_t& pos);
    std::unique_ptr<CHTLASTNode> parseImport(const std::string& source, size_t& pos);
    std::unique_ptr<CHTLASTNode> parseNamespace(const std::string& source, size_t& pos);
    
    // 辅助方法
    void skipWhitespace(const std::string& source, size_t& pos);
    std::string extractIdentifier(const std::string& source, size_t& pos);
    std::string extractString(const std::string& source, size_t& pos);
    bool isKeyword(const std::string& word);
    void addError(const std::string& error);
};

} // namespace chtl
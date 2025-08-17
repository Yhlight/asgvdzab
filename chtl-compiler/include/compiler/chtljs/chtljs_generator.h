#ifndef CHTLJS_GENERATOR_H
#define CHTLJS_GENERATOR_H

#include <string>
#include <sstream>
#include <memory>
#include <unordered_map>
#include <vector>
#include "compiler/chtljs/chtljs_ast.h"

namespace chtl {

// CHTL JS代码生成器
class CHTLJSGenerator {
public:
    CHTLJSGenerator();
    ~CHTLJSGenerator() = default;
    
    // 生成JavaScript代码
    std::string generate(const CHTLJSASTNode* ast);
    
    // 获取生成的辅助函数
    std::string getHelperFunctions() const;
    
    // 获取vir对象的全局函数声明
    std::string getVirFunctions() const;
    
private:
    std::stringstream output_;
    int indent_level_;
    
    // 记录需要的辅助函数
    bool needs_query_helper_;
    bool needs_listen_helper_;
    bool needs_delegate_helper_;
    bool needs_animate_helper_;
    
    // vir对象映射
    std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> vir_objects_;
    
    // 缩进管理
    void indent();
    void dedent();
    std::string getIndent() const;
    
    // 节点访问
    void visit(const CHTLJSASTNode* node);
    void visitRoot(const CHTLJSRootNode* node);
    void visitEnhancedSelector(const EnhancedSelectorNode* node);
    void visitArrowAccess(const ArrowAccessNode* node);
    void visitListenFunction(const ListenFunctionNode* node);
    void visitDelegateFunction(const DelegateFunctionNode* node);
    void visitAnimateFunction(const AnimateFunctionNode* node);
    void visitVirDeclaration(const VirDeclarationNode* node);
    void visitVirAccess(const VirAccessNode* node);
    void visitIdentifier(const CHTLJSIdentifierNode* node);
    void visitStringLiteral(const CHTLJSStringLiteralNode* node);
    void visitNumberLiteral(const CHTLJSNumberLiteralNode* node);
    void visitObjectLiteral(const ObjectLiteralNode* node);
    void visitArrayLiteral(const ArrayLiteralNode* node);
    void visitFunctionExpression(const FunctionExpressionNode* node);
    void visitMemberAccess(const MemberAccessNode* node);
    void visitFunctionCall(const FunctionCallNode* node);
    void visitJSCodeBlock(const JSCodeBlockNode* node);
    
    // 辅助方法
    std::string escapeString(const std::string& str);
    std::string generateSelector(const std::string& selector);
    
    // 生成辅助函数
    std::string generateQueryHelper() const;
    std::string generateListenHelper() const;
    std::string generateDelegateHelper() const;
    std::string generateAnimateHelper() const;
};

} // namespace chtl

#endif // CHTLJS_GENERATOR_H
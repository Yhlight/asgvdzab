#pragma once

#include "ASTVisitor.hpp"
#include <iostream>
#include <string>

namespace chtl {
namespace ast {

/**
 * AST打印器
 * 用于调试和可视化AST结构
 */
class ASTPrinter : public DefaultASTVisitor {
public:
    ASTPrinter(std::ostream& out = std::cout) : out_(out), indent_(0) {}
    
    // 覆盖需要打印的节点访问方法
    void visitProgramNode(ProgramNode& node) override;
    void visitElementNode(ElementNode& node) override;
    void visitTextNode(TextNode& node) override;
    void visitAttributeNode(AttributeNode& node) override;
    void visitCommentNode(CommentNode& node) override;
    void visitStyleBlockNode(StyleBlockNode& node) override;
    void visitStyleRuleNode(StyleRuleNode& node) override;
    void visitStyleSelectorNode(StyleSelectorNode& node) override;
    void visitStylePropertyNode(StylePropertyNode& node) override;
    void visitScriptBlockNode(ScriptBlockNode& node) override;
    void visitStringLiteralNode(StringLiteralNode& node) override;
    void visitNumberLiteralNode(NumberLiteralNode& node) override;
    void visitUnquotedLiteralNode(UnquotedLiteralNode& node) override;
    void visitIdentifierNode(IdentifierNode& node) override;
    
    // CHTL JS节点
    void visitSelectorExprNode(chtl::js::ast::SelectorExprNode& node) override;
    void visitArrowAccessNode(chtl::js::ast::ArrowAccessNode& node) override;
    void visitListenExprNode(chtl::js::ast::ListenExprNode& node) override;
    void visitAnimateExprNode(chtl::js::ast::AnimateExprNode& node) override;
    void visitObjectLiteralNode(chtl::js::ast::ObjectLiteralNode& node) override;
    void visitFunctionExprNode(chtl::js::ast::FunctionExprNode& node) override;
    
private:
    std::ostream& out_;
    int indent_;
    
    // 打印缩进
    void printIndent() {
        for (int i = 0; i < indent_; ++i) {
            out_ << "  ";
        }
    }
    
    // 打印节点信息
    void printNode(const std::string& type, const std::string& value = "") {
        printIndent();
        out_ << type;
        if (!value.empty()) {
            out_ << ": " << value;
        }
        out_ << "\n";
    }
    
    // RAII缩进管理
    class IndentGuard {
    public:
        IndentGuard(ASTPrinter& printer) : printer_(printer) {
            printer_.indent_++;
        }
        ~IndentGuard() {
            printer_.indent_--;
        }
    private:
        ASTPrinter& printer_;
    };
};

} // namespace ast
} // namespace chtl
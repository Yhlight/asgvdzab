#include "ASTPrinter.hpp"
#include "CHTLASTNodes.hpp"
#include "chtljs/ast/CHTLJSASTNodes.hpp"

namespace chtl {
namespace ast {

void ASTPrinter::visitProgramNode(ProgramNode& node) {
    printNode("Program");
    IndentGuard guard(*this);
    DefaultASTVisitor::visitProgramNode(node);
}

void ASTPrinter::visitElementNode(ElementNode& node) {
    printNode("Element", node.getTagName());
    IndentGuard guard(*this);
    
    // 打印属性
    for (const auto& [name, value] : node.getAttributes()) {
        printIndent();
        out_ << "@" << name << " = " << value << "\n";
    }
    
    DefaultASTVisitor::visitElementNode(node);
}

void ASTPrinter::visitTextNode(TextNode& node) {
    printNode("Text");
    IndentGuard guard(*this);
    DefaultASTVisitor::visitTextNode(node);
}

void ASTPrinter::visitAttributeNode(AttributeNode& node) {
    printNode("Attribute", node.getName());
    IndentGuard guard(*this);
    DefaultASTVisitor::visitAttributeNode(node);
}

void ASTPrinter::visitCommentNode(CommentNode& node) {
    std::string type;
    switch (node.getCommentType()) {
        case CommentNode::LINE_COMMENT: type = "//"; break;
        case CommentNode::BLOCK_COMMENT: type = "/**/"; break;
        case CommentNode::HTML_COMMENT: type = "--"; break;
    }
    printNode("Comment[" + type + "]", node.getContent());
}

void ASTPrinter::visitStyleBlockNode(StyleBlockNode& node) {
    printNode("StyleBlock");
    IndentGuard guard(*this);
    DefaultASTVisitor::visitStyleBlockNode(node);
}

void ASTPrinter::visitStyleRuleNode(StyleRuleNode& node) {
    printNode("StyleRule");
    IndentGuard guard(*this);
    DefaultASTVisitor::visitStyleRuleNode(node);
}

void ASTPrinter::visitStyleSelectorNode(StyleSelectorNode& node) {
    std::string type;
    switch (node.getSelectorType()) {
        case StyleSelectorNode::CLASS_SELECTOR: type = "class"; break;
        case StyleSelectorNode::ID_SELECTOR: type = "id"; break;
        case StyleSelectorNode::ELEMENT_SELECTOR: type = "element"; break;
        case StyleSelectorNode::AMPERSAND_SELECTOR: type = "&"; break;
        case StyleSelectorNode::PSEUDO_CLASS: type = "pseudo-class"; break;
        case StyleSelectorNode::PSEUDO_ELEMENT: type = "pseudo-element"; break;
        case StyleSelectorNode::COMPLEX_SELECTOR: type = "complex"; break;
    }
    printNode("StyleSelector[" + type + "]", node.getValue());
}

void ASTPrinter::visitStylePropertyNode(StylePropertyNode& node) {
    printNode("StyleProperty", node.getProperty());
    IndentGuard guard(*this);
    DefaultASTVisitor::visitStylePropertyNode(node);
}

void ASTPrinter::visitScriptBlockNode(ScriptBlockNode& node) {
    printNode("ScriptBlock");
    IndentGuard guard(*this);
    DefaultASTVisitor::visitScriptBlockNode(node);
}

void ASTPrinter::visitStringLiteralNode(StringLiteralNode& node) {
    printNode("StringLiteral", "\"" + node.getValue() + "\"");
}

void ASTPrinter::visitNumberLiteralNode(NumberLiteralNode& node) {
    printNode("NumberLiteral", node.getValue());
}

void ASTPrinter::visitUnquotedLiteralNode(UnquotedLiteralNode& node) {
    printNode("UnquotedLiteral", node.getValue());
}

void ASTPrinter::visitIdentifierNode(IdentifierNode& node) {
    printNode("Identifier", node.getName());
}

// CHTL JS节点
void ASTPrinter::visitSelectorExprNode(js::ast::SelectorExprNode& node) {
    std::string type;
    switch (node.getSelectorType()) {
        case js::ast::SelectorExprNode::TAG_OR_ID_OR_CLASS: type = "auto"; break;
        case js::ast::SelectorExprNode::CLASS_SELECTOR: type = "class"; break;
        case js::ast::SelectorExprNode::ID_SELECTOR: type = "id"; break;
        case js::ast::SelectorExprNode::TAG_SELECTOR: type = "tag"; break;
        case js::ast::SelectorExprNode::COMPLEX_SELECTOR: type = "complex"; break;
        case js::ast::SelectorExprNode::INDEXED_SELECTOR: type = "indexed"; break;
    }
    
    std::string selector = "{{" + node.getSelector() + "}}";
    if (node.getIndex() >= 0) {
        selector += "[" + std::to_string(node.getIndex()) + "]";
    }
    
    printNode("SelectorExpr[" + type + "]", selector);
    IndentGuard guard(*this);
    DefaultASTVisitor::visitSelectorExprNode(node);
}

void ASTPrinter::visitArrowAccessNode(js::ast::ArrowAccessNode& node) {
    printNode("ArrowAccess", "->");
    IndentGuard guard(*this);
    DefaultASTVisitor::visitArrowAccessNode(node);
}

void ASTPrinter::visitListenExprNode(js::ast::ListenExprNode& node) {
    printNode("ListenExpr");
    IndentGuard guard(*this);
    
    for (const auto& [event, handler] : node.getEventHandlers()) {
        printNode("Event", event);
        IndentGuard eventGuard(*this);
        if (handler) {
            handler->accept(*this);
        }
    }
}

void ASTPrinter::visitAnimateExprNode(js::ast::AnimateExprNode& node) {
    printNode("AnimateExpr");
    IndentGuard guard(*this);
    
    if (node.getDuration() > 0) {
        printNode("Duration", std::to_string(node.getDuration()) + "ms");
    }
    if (!node.getEasing().empty()) {
        printNode("Easing", node.getEasing());
    }
    
    DefaultASTVisitor::visitAnimateExprNode(node);
}

void ASTPrinter::visitObjectLiteralNode(js::ast::ObjectLiteralNode& node) {
    printNode("ObjectLiteral");
    IndentGuard guard(*this);
    DefaultASTVisitor::visitObjectLiteralNode(node);
}

void ASTPrinter::visitFunctionExprNode(js::ast::FunctionExprNode& node) {
    std::string params = "(";
    for (size_t i = 0; i < node.getParameters().size(); ++i) {
        if (i > 0) params += ", ";
        params += node.getParameters()[i];
    }
    params += ")";
    
    printNode("FunctionExpr", params);
    IndentGuard guard(*this);
    printNode("Body", node.getBody());
}

} // namespace ast
} // namespace chtl
#include "compiler/chtljs/chtljs_generator.h"
#include <algorithm>

namespace chtl {

CHTLJSGenerator::CHTLJSGenerator()
    : indent_level_(0), needs_query_helper_(false),
      needs_listen_helper_(false), needs_delegate_helper_(false),
      needs_animate_helper_(false) {}

std::string CHTLJSGenerator::generate(const CHTLJSASTNode* ast) {
    output_.str("");
    output_.clear();
    indent_level_ = 0;
    
    // 重置辅助函数标记
    needs_query_helper_ = false;
    needs_listen_helper_ = false;
    needs_delegate_helper_ = false;
    needs_animate_helper_ = false;
    vir_objects_.clear();
    
    visit(ast);
    
    return output_.str();
}

std::string CHTLJSGenerator::getHelperFunctions() const {
    std::stringstream helpers;
    
    if (needs_query_helper_) {
        helpers << generateQueryHelper() << "\n\n";
    }
    
    if (needs_listen_helper_) {
        helpers << generateListenHelper() << "\n\n";
    }
    
    if (needs_delegate_helper_) {
        helpers << generateDelegateHelper() << "\n\n";
    }
    
    if (needs_animate_helper_) {
        helpers << generateAnimateHelper() << "\n\n";
    }
    
    return helpers.str();
}

std::string CHTLJSGenerator::getVirFunctions() const {
    std::stringstream funcs;
    
    for (const auto& [vir_name, methods] : vir_objects_) {
        for (const auto& [method_name, method_body] : methods) {
            funcs << "function " << vir_name << "_" << method_name 
                 << method_body << "\n\n";
        }
    }
    
    return funcs.str();
}

void CHTLJSGenerator::indent() {
    indent_level_++;
}

void CHTLJSGenerator::dedent() {
    if (indent_level_ > 0) {
        indent_level_--;
    }
}

std::string CHTLJSGenerator::getIndent() const {
    return std::string(indent_level_ * 2, ' ');
}

void CHTLJSGenerator::visit(const CHTLJSASTNode* node) {
    if (!node) return;
    
    switch (node->getType()) {
        case CHTLJSASTNodeType::ROOT:
            visitRoot(static_cast<const CHTLJSRootNode*>(node));
            break;
        case CHTLJSASTNodeType::ENHANCED_SELECTOR:
            visitEnhancedSelector(static_cast<const EnhancedSelectorNode*>(node));
            break;
        case CHTLJSASTNodeType::ARROW_ACCESS:
            visitArrowAccess(static_cast<const ArrowAccessNode*>(node));
            break;
        case CHTLJSASTNodeType::LISTEN_FUNCTION:
            visitListenFunction(static_cast<const ListenFunctionNode*>(node));
            break;
        case CHTLJSASTNodeType::DELEGATE_FUNCTION:
            visitDelegateFunction(static_cast<const DelegateFunctionNode*>(node));
            break;
        case CHTLJSASTNodeType::ANIMATE_FUNCTION:
            visitAnimateFunction(static_cast<const AnimateFunctionNode*>(node));
            break;
        case CHTLJSASTNodeType::VIR_DECLARATION:
            visitVirDeclaration(static_cast<const VirDeclarationNode*>(node));
            break;
        case CHTLJSASTNodeType::VIR_ACCESS:
            visitVirAccess(static_cast<const VirAccessNode*>(node));
            break;
        case CHTLJSASTNodeType::IDENTIFIER:
            visitIdentifier(static_cast<const CHTLJSIdentifierNode*>(node));
            break;
        case CHTLJSASTNodeType::STRING_LITERAL:
            visitStringLiteral(static_cast<const CHTLJSStringLiteralNode*>(node));
            break;
        case CHTLJSASTNodeType::NUMBER_LITERAL:
            visitNumberLiteral(static_cast<const CHTLJSNumberLiteralNode*>(node));
            break;
        case CHTLJSASTNodeType::OBJECT_LITERAL:
            visitObjectLiteral(static_cast<const ObjectLiteralNode*>(node));
            break;
        case CHTLJSASTNodeType::ARRAY_LITERAL:
            visitArrayLiteral(static_cast<const ArrayLiteralNode*>(node));
            break;
        case CHTLJSASTNodeType::FUNCTION_EXPRESSION:
            visitFunctionExpression(static_cast<const FunctionExpressionNode*>(node));
            break;
        case CHTLJSASTNodeType::MEMBER_ACCESS:
            visitMemberAccess(static_cast<const MemberAccessNode*>(node));
            break;
        case CHTLJSASTNodeType::FUNCTION_CALL:
            visitFunctionCall(static_cast<const FunctionCallNode*>(node));
            break;
        case CHTLJSASTNodeType::JS_CODE_BLOCK:
            visitJSCodeBlock(static_cast<const JSCodeBlockNode*>(node));
            break;
        default:
            // 其他节点类型
            for (const auto& child : node->getChildren()) {
                visit(child.get());
            }
            break;
    }
}

void CHTLJSGenerator::visitRoot(const CHTLJSRootNode* node) {
    for (const auto& child : node->getChildren()) {
        visit(child.get());
        output_ << ";\n";
    }
}

void CHTLJSGenerator::visitEnhancedSelector(const EnhancedSelectorNode* node) {
    needs_query_helper_ = true;
    output_ << "$$(\"" << escapeString(node->getSelector()) << "\")";
}

void CHTLJSGenerator::visitArrowAccess(const ArrowAccessNode* node) {
    visit(node->getLeft());
    output_ << ".";
    visit(node->getRight());
}

void CHTLJSGenerator::visitListenFunction(const ListenFunctionNode* node) {
    needs_listen_helper_ = true;
    output_ << "$$listen(";
    visit(node->getSelector());
    output_ << ", ";
    visit(node->getEvent());
    output_ << ", ";
    visit(node->getHandler());
    output_ << ")";
}

void CHTLJSGenerator::visitDelegateFunction(const DelegateFunctionNode* node) {
    needs_delegate_helper_ = true;
    output_ << "$$delegate(";
    visit(node->getParent());
    output_ << ", ";
    visit(node->getChild());
    output_ << ", ";
    visit(node->getEvent());
    output_ << ", ";
    visit(node->getHandler());
    output_ << ")";
}

void CHTLJSGenerator::visitAnimateFunction(const AnimateFunctionNode* node) {
    needs_animate_helper_ = true;
    output_ << "$$animate(";
    visit(node->getSelector());
    output_ << ", ";
    visit(node->getProperties());
    output_ << ", ";
    visit(node->getDuration());
    
    if (node->getEasing()) {
        output_ << ", ";
        visit(node->getEasing());
    }
    
    if (node->getCallback()) {
        output_ << ", ";
        visit(node->getCallback());
    }
    
    output_ << ")";
}

void CHTLJSGenerator::visitVirDeclaration(const VirDeclarationNode* node) {
    const std::string& vir_name = node->getName();
    const auto* object = static_cast<const ObjectLiteralNode*>(node->getObject());
    
    // 收集vir对象的方法
    for (const auto& [prop_name, prop_value] : object->getProperties()) {
        if (prop_value->getType() == CHTLJSASTNodeType::FUNCTION_EXPRESSION) {
            auto* func = static_cast<const FunctionExpressionNode*>(prop_value.get());
            
            // 生成函数签名
            std::stringstream func_sig;
            func_sig << "(";
            const auto& params = func->getParameters();
            for (size_t i = 0; i < params.size(); ++i) {
                if (i > 0) func_sig << ", ";
                func_sig << params[i];
            }
            func_sig << ") {\n";
            
            // 函数体
            if (func->getBody()) {
                func_sig << func->getBody()->getContent();
            }
            
            func_sig << "\n}";
            
            vir_objects_[vir_name].push_back({prop_name, func_sig.str()});
        }
    }
    
    // vir声明本身不生成输出
}

void CHTLJSGenerator::visitVirAccess(const VirAccessNode* node) {
    output_ << node->getVirName() << "_" << node->getMethodName();
}

void CHTLJSGenerator::visitIdentifier(const CHTLJSIdentifierNode* node) {
    output_ << node->getName();
}

void CHTLJSGenerator::visitStringLiteral(const CHTLJSStringLiteralNode* node) {
    output_ << "\"" << escapeString(node->getValue()) << "\"";
}

void CHTLJSGenerator::visitNumberLiteral(const CHTLJSNumberLiteralNode* node) {
    output_ << node->getValue();
}

void CHTLJSGenerator::visitObjectLiteral(const ObjectLiteralNode* node) {
    output_ << "{";
    
    const auto& props = node->getProperties();
    for (size_t i = 0; i < props.size(); ++i) {
        if (i > 0) output_ << ", ";
        output_ << "\"" << escapeString(props[i].first) << "\": ";
        visit(props[i].second.get());
    }
    
    output_ << "}";
}

void CHTLJSGenerator::visitArrayLiteral(const ArrayLiteralNode* node) {
    output_ << "[";
    
    const auto& elements = node->getElements();
    for (size_t i = 0; i < elements.size(); ++i) {
        if (i > 0) output_ << ", ";
        visit(elements[i].get());
    }
    
    output_ << "]";
}

void CHTLJSGenerator::visitFunctionExpression(const FunctionExpressionNode* node) {
    output_ << "function";
    
    if (!node->getName().empty()) {
        output_ << " " << node->getName();
    }
    
    output_ << "(";
    const auto& params = node->getParameters();
    for (size_t i = 0; i < params.size(); ++i) {
        if (i > 0) output_ << ", ";
        output_ << params[i];
    }
    output_ << ") {\n";
    
    indent();
    if (node->getBody()) {
        output_ << getIndent() << node->getBody()->getContent();
    }
    dedent();
    
    output_ << "\n" << getIndent() << "}";
}

void CHTLJSGenerator::visitMemberAccess(const MemberAccessNode* node) {
    visit(node->getObject());
    output_ << "." << node->getMember();
}

void CHTLJSGenerator::visitFunctionCall(const FunctionCallNode* node) {
    visit(node->getFunction());
    output_ << "(";
    
    const auto& args = node->getArguments();
    for (size_t i = 0; i < args.size(); ++i) {
        if (i > 0) output_ << ", ";
        visit(args[i].get());
    }
    
    output_ << ")";
}

void CHTLJSGenerator::visitJSCodeBlock(const JSCodeBlockNode* node) {
    output_ << node->getContent();
}

std::string CHTLJSGenerator::escapeString(const std::string& str) {
    std::string result;
    for (char c : str) {
        switch (c) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default: result += c; break;
        }
    }
    return result;
}

std::string CHTLJSGenerator::generateSelector(const std::string& selector) {
    // 对选择器进行必要的转换
    return selector;
}

std::string CHTLJSGenerator::generateQueryHelper() const {
    return R"(function $$(selector) {
  if (typeof selector === 'string') {
    const elements = document.querySelectorAll(selector);
    return Array.from(elements);
  }
  return selector;
})";
}

std::string CHTLJSGenerator::generateListenHelper() const {
    return R"(function $$listen(selector, event, handler) {
  const elements = $$(selector);
  elements.forEach(function(element) {
    element.addEventListener(event, handler);
  });
  return elements;
})";
}

std::string CHTLJSGenerator::generateDelegateHelper() const {
    return R"(function $$delegate(parent, childSelector, event, handler) {
  const parentElements = $$(parent);
  parentElements.forEach(function(parentElement) {
    parentElement.addEventListener(event, function(e) {
      const target = e.target.closest(childSelector);
      if (target && parentElement.contains(target)) {
        handler.call(target, e);
      }
    });
  });
  return parentElements;
})";
}

std::string CHTLJSGenerator::generateAnimateHelper() const {
    return R"(function $$animate(selector, properties, duration, easing, callback) {
  const elements = $$(selector);
  const startTime = performance.now();
  const durationMs = parseInt(duration) || 1000;
  
  function ease(t) {
    // 简单的缓动函数
    if (easing === 'ease-in') return t * t;
    if (easing === 'ease-out') return t * (2 - t);
    if (easing === 'ease-in-out') return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t;
    return t; // linear
  }
  
  function animate(currentTime) {
    const elapsed = currentTime - startTime;
    const progress = Math.min(elapsed / durationMs, 1);
    const easedProgress = ease(progress);
    
    elements.forEach(function(element) {
      for (const [prop, value] of Object.entries(properties)) {
        if (typeof value === 'number') {
          element.style[prop] = value + 'px';
        } else {
          element.style[prop] = value;
        }
      }
    });
    
    if (progress < 1) {
      requestAnimationFrame(animate);
    } else if (callback) {
      callback();
    }
  }
  
  requestAnimationFrame(animate);
  return elements;
})";
}

} // namespace chtl
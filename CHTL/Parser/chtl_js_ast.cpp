#include "CHTL/ASTNodes.hpp"
#include <sstream>
#include <algorithm>

namespace chtl {

// CHTLJSASTNode 基础实现
void CHTLJSASTNode::addChild(CHTLJSASTNodePtr child) {
    if (child) {
        child->parent = this;
        children.push_back(child);
    }
}

std::vector<CHTLJSASTNodePtr> CHTLJSASTNode::getChildrenByType(CHTLJSASTNodeType targetType) const {
    std::vector<CHTLJSASTNodePtr> result;
    for (const auto& child : children) {
        if (child && child->type == targetType) {
            result.push_back(child);
        }
    }
    return result;
}

CHTLJSASTNodePtr CHTLJSASTNode::findChild(CHTLJSASTNodeType targetType) const {
    for (const auto& child : children) {
        if (child && child->type == targetType) {
            return child;
        }
    }
    return nullptr;
}

bool CHTLJSASTNode::isCHTLJSSpecific() const {
    return type == CHTLJSASTNodeType::ENHANCED_SELECTOR ||
           type == CHTLJSASTNodeType::ARROW_OPERATOR ||
           type == CHTLJSASTNodeType::LISTEN_CALL ||
           type == CHTLJSASTNodeType::DELEGATE_CALL ||
           type == CHTLJSASTNodeType::ANIMATE_CALL;
}

bool CHTLJSASTNode::isEnhancedSelector() const {
    return type == CHTLJSASTNodeType::ENHANCED_SELECTOR;
}

bool CHTLJSASTNode::isListenNode() const {
    return type == CHTLJSASTNodeType::LISTEN_CALL ||
           type == CHTLJSASTNodeType::LISTEN_CONFIG ||
           type == CHTLJSASTNodeType::EVENT_HANDLER;
}

bool CHTLJSASTNode::isDelegateNode() const {
    return type == CHTLJSASTNodeType::DELEGATE_CALL ||
           type == CHTLJSASTNodeType::DELEGATE_CONFIG ||
           type == CHTLJSASTNodeType::DELEGATE_TARGET;
}

bool CHTLJSASTNode::isAnimateNode() const {
    return type == CHTLJSASTNodeType::ANIMATE_CALL ||
           type == CHTLJSASTNodeType::ANIMATE_CONFIG ||
           type == CHTLJSASTNodeType::ANIMATE_KEYFRAME;
}

std::string CHTLJSASTNode::toString() const {
    return "CHTLJSASTNode(type: " + std::to_string(static_cast<int>(type)) + ")";
}

CHTLJSASTNodePtr CHTLJSASTNode::clone() const {
    return std::make_shared<CHTLJSASTNode>(type, position);
}

// ScriptProgramNode 实现
std::string ScriptProgramNode::toString() const {
    return "ScriptProgramNode(children: " + std::to_string(children.size()) + ")";
}

CHTLJSASTNodePtr ScriptProgramNode::clone() const {
    auto cloned = std::make_shared<ScriptProgramNode>(position);
    for (const auto& child : children) {
        cloned->addChild(child->clone());
    }
    return cloned;
}

// EnhancedSelectorNode 实现
void EnhancedSelectorNode::parseSelector() {
    std::string trimmed = selectorText;
    
    // 移除前后空白
    trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));
    trimmed.erase(trimmed.find_last_not_of(" \t\n\r") + 1);
    
    if (trimmed.empty()) {
        selectorType = EnhancedSelectorType::AUTO_DETECT;
        return;
    }
    
    // 检查索引访问 [n]
    size_t bracketPos = trimmed.find('[');
    if (bracketPos != std::string::npos) {
        size_t closeBracket = trimmed.find(']', bracketPos);
        if (closeBracket != std::string::npos) {
            parsed.hasIndex = true;
            std::string indexStr = trimmed.substr(bracketPos + 1, closeBracket - bracketPos - 1);
            parsed.index = std::stoi(indexStr);
            trimmed = trimmed.substr(0, bracketPos); // 移除索引部分
            selectorType = EnhancedSelectorType::INDEXED;
        }
    }
    
    // 检查后代选择器（包含空格）
    size_t spacePos = trimmed.find(' ');
    if (spacePos != std::string::npos) {
        if (selectorType != EnhancedSelectorType::INDEXED) {
            selectorType = EnhancedSelectorType::DESCENDANT;
        }
        // 解析后代路径
        std::stringstream ss(trimmed);
        std::string part;
        while (ss >> part) {
            parsed.descendantPath.push_back(part);
        }
    } else {
        // 单一选择器
        if (trimmed[0] == '.') {
            if (selectorType != EnhancedSelectorType::INDEXED) {
                selectorType = EnhancedSelectorType::CLASS_NAME;
            }
            parsed.className = trimmed.substr(1);
        } else if (trimmed[0] == '#') {
            if (selectorType != EnhancedSelectorType::INDEXED) {
                selectorType = EnhancedSelectorType::ID_NAME;
            }
            parsed.idName = trimmed.substr(1);
        } else {
            if (selectorType != EnhancedSelectorType::INDEXED) {
                selectorType = EnhancedSelectorType::TAG_NAME;
            }
            parsed.tagName = trimmed;
        }
    }
}

std::string EnhancedSelectorNode::toString() const {
    std::string typeStr;
    switch (selectorType) {
        case EnhancedSelectorType::TAG_NAME: typeStr = "TAG"; break;
        case EnhancedSelectorType::CLASS_NAME: typeStr = "CLASS"; break;
        case EnhancedSelectorType::ID_NAME: typeStr = "ID"; break;
        case EnhancedSelectorType::DESCENDANT: typeStr = "DESCENDANT"; break;
        case EnhancedSelectorType::INDEXED: typeStr = "INDEXED"; break;
        case EnhancedSelectorType::AUTO_DETECT: typeStr = "AUTO"; break;
    }
    
    std::string result = "EnhancedSelectorNode({{" + selectorText + "}}, type: " + typeStr;
    if (parsed.hasIndex) {
        result += ", index: " + std::to_string(parsed.index);
    }
    result += ")";
    return result;
}

CHTLJSASTNodePtr EnhancedSelectorNode::clone() const {
    return std::make_shared<EnhancedSelectorNode>(selectorText, position);
}

// ArrowOperatorNode 实现
std::string ArrowOperatorNode::toString() const {
    return "ArrowOperatorNode(->)";
}

CHTLJSASTNodePtr ArrowOperatorNode::clone() const {
    auto cloned = std::make_shared<ArrowOperatorNode>(position);
    if (left) cloned->setLeft(left->clone());
    if (right) cloned->setRight(right->clone());
    return cloned;
}

// ListenCallNode 实现
std::string ListenCallNode::toString() const {
    return "ListenCallNode(listen call)";
}

CHTLJSASTNodePtr ListenCallNode::clone() const {
    auto cloned = std::make_shared<ListenCallNode>(position);
    if (target) cloned->setTarget(target->clone());
    if (config) cloned->setConfig(config->clone());
    return cloned;
}

// ListenConfigNode 实现
std::string ListenConfigNode::toString() const {
    return "ListenConfigNode(handlers: " + std::to_string(eventHandlers.size()) + ")";
}

CHTLJSASTNodePtr ListenConfigNode::clone() const {
    auto cloned = std::make_shared<ListenConfigNode>(position);
    for (const auto& handler : eventHandlers) {
        cloned->addEventHandler(handler->clone());
    }
    return cloned;
}

// EventHandlerNode 实现
std::string EventHandlerNode::toString() const {
    return "EventHandlerNode(" + eventType + ": handler)";
}

CHTLJSASTNodePtr EventHandlerNode::clone() const {
    auto cloned = std::make_shared<EventHandlerNode>(eventType, position);
    if (handler) cloned->setHandler(handler->clone());
    return cloned;
}

// DelegateCallNode 实现
std::string DelegateCallNode::toString() const {
    return "DelegateCallNode(delegate call)";
}

CHTLJSASTNodePtr DelegateCallNode::clone() const {
    auto cloned = std::make_shared<DelegateCallNode>(position);
    if (parent) cloned->setParent(parent->clone());
    if (config) cloned->setConfig(config->clone());
    return cloned;
}

// DelegateConfigNode 实现
std::string DelegateConfigNode::toString() const {
    return "DelegateConfigNode(handlers: " + std::to_string(eventHandlers.size()) + ")";
}

CHTLJSASTNodePtr DelegateConfigNode::clone() const {
    auto cloned = std::make_shared<DelegateConfigNode>(position);
    if (target) cloned->setTarget(target->clone());
    for (const auto& handler : eventHandlers) {
        cloned->addEventHandler(handler->clone());
    }
    return cloned;
}

// AnimateCallNode 实现
std::string AnimateCallNode::toString() const {
    return "AnimateCallNode(animate call)";
}

CHTLJSASTNodePtr AnimateCallNode::clone() const {
    auto cloned = std::make_shared<AnimateCallNode>(position);
    if (config) cloned->setConfig(config->clone());
    return cloned;
}

// AnimateConfigNode 实现
std::string AnimateConfigNode::toString() const {
    return "AnimateConfigNode(duration: " + std::to_string(duration) + "ms, easing: " + easing + ")";
}

CHTLJSASTNodePtr AnimateConfigNode::clone() const {
    auto cloned = std::make_shared<AnimateConfigNode>(position);
    cloned->duration = duration;
    cloned->easing = easing;
    cloned->loop = loop;
    cloned->direction = direction;
    cloned->delay = delay;
    if (begin) cloned->begin = begin->clone();
    if (end) cloned->end = end->clone();
    if (when) cloned->when = when->clone();
    if (callback) cloned->callback = callback->clone();
    return cloned;
}

// AnimateKeyframeNode 实现
std::string AnimateKeyframeNode::toString() const {
    return "AnimateKeyframeNode(at: " + std::to_string(at) + ", properties: " + std::to_string(properties.size()) + ")";
}

CHTLJSASTNodePtr AnimateKeyframeNode::clone() const {
    auto cloned = std::make_shared<AnimateKeyframeNode>(at, position);
    cloned->properties = properties;
    return cloned;
}

// FunctionDeclarationNode 实现
std::string FunctionDeclarationNode::toString() const {
    std::string result = "FunctionDeclarationNode(";
    if (!name.empty()) {
        result += "name: " + name + ", ";
    }
    result += "params: " + std::to_string(parameters.size()) + ")";
    return result;
}

CHTLJSASTNodePtr FunctionDeclarationNode::clone() const {
    auto cloned = std::make_shared<FunctionDeclarationNode>(name, position);
    cloned->parameters = parameters;
    if (body) cloned->setBody(body->clone());
    return cloned;
}

// ArrowFunctionNode 实现
std::string ArrowFunctionNode::toString() const {
    return "ArrowFunctionNode(params: " + std::to_string(parameters.size()) + ")";
}

CHTLJSASTNodePtr ArrowFunctionNode::clone() const {
    auto cloned = std::make_shared<ArrowFunctionNode>(position);
    cloned->parameters = parameters;
    if (body) cloned->setBody(body->clone());
    return cloned;
}

// CHTLJSIdentifierNode 实现
std::string CHTLJSIdentifierNode::toString() const {
    return "CHTLJSIdentifierNode(" + name + ")";
}

CHTLJSASTNodePtr CHTLJSIdentifierNode::clone() const {
    return std::make_shared<CHTLJSIdentifierNode>(name, position);
}

// LiteralNode 实现
std::string LiteralNode::toString() const {
    return "LiteralNode(" + value + ", type: " + type + ")";
}

CHTLJSASTNodePtr LiteralNode::clone() const {
    return std::make_shared<LiteralNode>(value, type, position);
}

// ObjectExpressionNode 实现
std::string ObjectExpressionNode::toString() const {
    return "ObjectExpressionNode(properties: " + std::to_string(properties.size()) + ")";
}

CHTLJSASTNodePtr ObjectExpressionNode::clone() const {
    auto cloned = std::make_shared<ObjectExpressionNode>(position);
    for (const auto& prop : properties) {
        cloned->addProperty(prop->clone());
    }
    return cloned;
}

// PropertyNode 实现
std::string PropertyNode::toString() const {
    return "PropertyNode(key: value)";
}

CHTLJSASTNodePtr PropertyNode::clone() const {
    auto cloned = std::make_shared<PropertyNode>(position);
    if (key) cloned->setKey(key->clone());
    if (value) cloned->setValue(value->clone());
    return cloned;
}

// CallExpressionNode 实现
std::string CallExpressionNode::toString() const {
    return "CallExpressionNode(args: " + std::to_string(arguments.size()) + ")";
}

CHTLJSASTNodePtr CallExpressionNode::clone() const {
    auto cloned = std::make_shared<CallExpressionNode>(position);
    if (callee) cloned->setCallee(callee->clone());
    for (const auto& arg : arguments) {
        cloned->addArgument(arg->clone());
    }
    return cloned;
}

// MemberExpressionNode 实现
std::string MemberExpressionNode::toString() const {
    std::string accessType = computed ? "computed" : "dot";
    return "MemberExpressionNode(" + accessType + " access)";
}

CHTLJSASTNodePtr MemberExpressionNode::clone() const {
    auto cloned = std::make_shared<MemberExpressionNode>(position);
    cloned->computed = computed;
    if (object) cloned->setObject(object->clone());
    if (property) cloned->setProperty(property->clone());
    return cloned;
}

// BlockStatementNode 实现
std::string BlockStatementNode::toString() const {
    return "BlockStatementNode(statements: " + std::to_string(statements.size()) + ")";
}

CHTLJSASTNodePtr BlockStatementNode::clone() const {
    auto cloned = std::make_shared<BlockStatementNode>(position);
    for (const auto& stmt : statements) {
        cloned->addStatement(stmt->clone());
    }
    return cloned;
}

// CHTLJSCommentNode 实现
std::string CHTLJSCommentNode::toString() const {
    std::string typeStr = isMultiline ? "/* */" : "//";
    return "CHTLJSCommentNode(" + typeStr + " " + content + ")";
}

CHTLJSASTNodePtr CHTLJSCommentNode::clone() const {
    return std::make_shared<CHTLJSCommentNode>(content, isMultiline, position);
}

} // namespace chtl
#include "compiler/CHTLJSCompiler.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <regex>
#include <unordered_set>

namespace chtl {

// 私有实现类
class CHTLJSCompiler::Impl {
public:
    std::unordered_map<std::string, std::string> options;
    CompileStats stats;
    std::vector<std::string> errors;
    
    // 关键字集合
    std::unordered_set<std::string> keywords;
    
    Impl() {
        stats = CompileStats{};
        
        // 初始化关键字集合
        keywords = {
            "script", "listen", "delegate", "animate", "function", "var", "let", "const",
            "if", "else", "for", "while", "switch", "case", "break", "continue", "return",
            "new", "class", "extends", "super", "this", "console", "document", "window",
            "addEventListener", "querySelector", "getElementById", "textContent", "innerHTML"
        };
    }
    
    ~Impl() = default;
};

CHTLJSCompiler::CHTLJSCompiler() : pImpl(std::make_unique<Impl>()) {}

CHTLJSCompiler::~CHTLJSCompiler() = default;

CompilationResult CHTLJSCompiler::compile(const CodeFragment& fragment) {
    CompilationResult result;
    
    try {
        // 解析CHTL JS代码
        auto ast = parseCHTLJS(fragment.content);
        if (!ast) {
            result.success = false;
            result.errors.push_back("解析CHTL JS代码失败");
            return result;
        }
        
        // 生成JavaScript代码
        std::string js = generateJavaScript(ast.get());
        
        result.success = true;
        result.output = js;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back("编译异常: " + std::string(e.what()));
    }
    
    return result;
}

void CHTLJSCompiler::setOptions(const std::unordered_map<std::string, std::string>& options) {
    pImpl->options = options;
}

CHTLJSCompiler::CompileStats CHTLJSCompiler::getCompileStats() const {
    return pImpl->stats;
}

std::vector<std::string> CHTLJSCompiler::getErrors() const {
    return pImpl->errors;
}

void CHTLJSCompiler::clearErrors() {
    pImpl->errors.clear();
}

std::unique_ptr<CHTLJSNode> CHTLJSCompiler::parseCHTLJS(const std::string& source) {
    auto root = std::make_unique<CHTLJSNode>(CHTLJSNodeType::ROOT, "root");
    size_t pos = 0;
    
    // 跳过开头的空白字符
    skipWhitespace(source, pos);
    
    while (pos < source.length()) {
        // 解析script块
        if (source.substr(pos, 6) == "script") {
            auto node = parseScriptBlock(source, pos);
            if (node) root->addChild(std::move(node));
        } else if (source.substr(pos, 6) == "listen") {
            auto node = parseListenCall(source, pos);
            if (node) root->addChild(std::move(node));
        } else if (source.substr(pos, 8) == "delegate") {
            auto node = parseDelegateCall(source, pos);
            if (node) root->addChild(std::move(node));
        } else if (source.substr(pos, 7) == "animate") {
            auto node = parseAnimateCall(source, pos);
            if (node) root->addChild(std::move(node));
        } else if (source[pos] == '{' && pos > 0 && source[pos-1] == '}') {
            // 增强选择器
            auto node = parseEnhancedSelector(source, pos);
            if (node) root->addChild(std::move(node));
        } else {
            pos++;
        }
        
        skipWhitespace(source, pos);
    }
    
    return root;
}

std::unique_ptr<CHTLJSNode> CHTLJSCompiler::parseScriptBlock(const std::string& source, size_t& pos) {
    if (source.substr(pos, 6) != "script") {
        return nullptr;
    }
    
    pos += 6; // 跳过"script"
    skipWhitespace(source, pos);
    
    auto scriptNode = std::make_unique<CHTLJSNode>(CHTLJSNodeType::SCRIPT_BLOCK, "script");
    pImpl->stats.scriptBlocks++;
    
    if (pos < source.length() && source[pos] == '{') {
        pos++; // 跳过{
        
        std::string jsContent;
        int braceCount = 1;
        
        while (pos < source.length() && braceCount > 0) {
            if (source[pos] == '{') braceCount++;
            else if (source[pos] == '}') braceCount--;
            
            if (braceCount > 0) {
                jsContent += source[pos];
            }
            pos++;
        }
        
        scriptNode->value = jsContent;
    }
    
    return scriptNode;
}

std::unique_ptr<CHTLJSNode> CHTLJSCompiler::parseListenCall(const std::string& source, size_t& pos) {
    if (source.substr(pos, 6) != "listen") {
        return nullptr;
    }
    
    pos += 6; // 跳过"listen"
    skipWhitespace(source, pos);
    
    auto listenNode = std::make_unique<CHTLJSNode>(CHTLJSNodeType::LISTEN_CALL, "listen");
    pImpl->stats.listenCalls++;
    
    if (pos < source.length() && source[pos] == '(') {
        pos++; // 跳过(
        
        std::string params;
        int parenCount = 1;
        
        while (pos < source.length() && parenCount > 0) {
            if (source[pos] == '(') parenCount++;
            else if (source[pos] == ')') parenCount--;
            
            if (parenCount > 0) {
                params += source[pos];
            }
            pos++;
        }
        
        listenNode->value = params;
    }
    
    return listenNode;
}

std::unique_ptr<CHTLJSNode> CHTLJSCompiler::parseDelegateCall(const std::string& source, size_t& pos) {
    if (source.substr(pos, 8) != "delegate") {
        return nullptr;
    }
    
    pos += 8; // 跳过"delegate"
    skipWhitespace(source, pos);
    
    auto delegateNode = std::make_unique<CHTLJSNode>(CHTLJSNodeType::DELEGATE_CALL, "delegate");
    pImpl->stats.delegateCalls++;
    
    if (pos < source.length() && source[pos] == '(') {
        pos++; // 跳过(
        
        std::string params;
        int parenCount = 1;
        
        while (pos < source.length() && parenCount > 0) {
            if (source[pos] == '(') parenCount++;
            else if (source[pos] == ')') parenCount--;
            
            if (parenCount > 0) {
                params += source[pos];
            }
            pos++;
        }
        
        delegateNode->value = params;
    }
    
    return delegateNode;
}

std::unique_ptr<CHTLJSNode> CHTLJSCompiler::parseAnimateCall(const std::string& source, size_t& pos) {
    if (source.substr(pos, 7) != "animate") {
        return nullptr;
    }
    
    pos += 7; // 跳过"animate"
    skipWhitespace(source, pos);
    
    auto animateNode = std::make_unique<CHTLJSNode>(CHTLJSNodeType::ANIMATE_CALL, "animate");
    pImpl->stats.animateCalls++;
    
    if (pos < source.length() && source[pos] == '(') {
        pos++; // 跳过(
        
        std::string params;
        int parenCount = 1;
        
        while (pos < source.length() && parenCount > 0) {
            if (source[pos] == '(') parenCount++;
            else if (source[pos] == ')') parenCount--;
            
            if (parenCount > 0) {
                params += source[pos];
            }
            pos++;
        }
        
        animateNode->value = params;
    }
    
    return animateNode;
}

std::unique_ptr<CHTLJSNode> CHTLJSCompiler::parseEnhancedSelector(const std::string& source, size_t& pos) {
    if (pos >= source.length() || source[pos] != '{') {
        return nullptr;
    }
    
    pos++; // 跳过{
    
    auto selectorNode = std::make_unique<CHTLJSNode>(CHTLJSNodeType::ENHANCED_SELECTOR, "selector");
    pImpl->stats.enhancedSelectors++;
    
    std::string selectorContent;
    int braceCount = 1;
    
    while (pos < source.length() && braceCount > 0) {
        if (source[pos] == '{') braceCount++;
        else if (source[pos] == '}') braceCount--;
        
        if (braceCount > 0) {
            selectorContent += source[pos];
        }
        pos++;
    }
    
    selectorNode->value = selectorContent;
    
    return selectorNode;
}

std::unique_ptr<CHTLJSNode> CHTLJSCompiler::parseFunctionCall(const std::string& source, size_t& pos) {
    // 简化实现，跳过函数调用
    while (pos < source.length() && source[pos] != ';' && source[pos] != '\n') {
        pos++;
    }
    
    pImpl->stats.functionCalls++;
    return std::make_unique<CHTLJSNode>(CHTLJSNodeType::FUNCTION_CALL, "function_call");
}

std::string CHTLJSCompiler::generateJavaScript(const CHTLJSNode* node) {
    if (!node) return "";
    
    std::stringstream js;
    
    switch (node->type) {
        case CHTLJSNodeType::SCRIPT_BLOCK:
            js << node->value;
            break;
            
        case CHTLJSNodeType::LISTEN_CALL:
            js << generateListenCode(node);
            break;
            
        case CHTLJSNodeType::DELEGATE_CALL:
            js << generateDelegateCode(node);
            break;
            
        case CHTLJSNodeType::ANIMATE_CALL:
            js << generateAnimateCode(node);
            break;
            
        case CHTLJSNodeType::ENHANCED_SELECTOR:
            js << generateEnhancedSelectorCode(node);
            break;
            
        default:
            // 递归处理子节点
            for (const auto& child : node->children) {
                js << generateJavaScript(child.get());
            }
            break;
    }
    
    return js.str();
}

std::string CHTLJSCompiler::generateListenCode(const CHTLJSNode* node) {
    std::stringstream js;
    
    // 解析参数并生成标准的事件监听器代码
    std::string params = node->value;
    
    // 简化实现，直接返回参数内容
    js << "// listen call: " << params << "\n";
    js << "// 这里应该解析参数并生成标准的事件监听器代码\n";
    
    return js.str();
}

std::string CHTLJSCompiler::generateDelegateCode(const CHTLJSNode* node) {
    std::stringstream js;
    
    // 解析参数并生成事件委托代码
    std::string params = node->value;
    
    // 简化实现，直接返回参数内容
    js << "// delegate call: " << params << "\n";
    js << "// 这里应该解析参数并生成事件委托代码\n";
    
    return js.str();
}

std::string CHTLJSCompiler::generateAnimateCode(const CHTLJSNode* node) {
    std::stringstream js;
    
    // 解析参数并生成动画代码
    std::string params = node->value;
    
    // 简化实现，直接返回参数内容
    js << "// animate call: " << params << "\n";
    js << "// 这里应该解析参数并生成动画代码\n";
    
    return js.str();
}

std::string CHTLJSCompiler::generateEnhancedSelectorCode(const CHTLJSNode* node) {
    std::stringstream js;
    
    // 解析增强选择器并生成标准的选择器代码
    std::string selector = node->value;
    
    // 简化实现，直接返回选择器内容
    js << "// enhanced selector: " << selector << "\n";
    js << "// 这里应该解析选择器并生成标准的DOM选择器代码\n";
    
    return js.str();
}

void CHTLJSCompiler::skipWhitespace(const std::string& source, size_t& pos) {
    while (pos < source.length() && std::isspace(source[pos])) {
        pos++;
    }
}

std::string CHTLJSCompiler::extractIdentifier(const std::string& source, size_t& pos) {
    std::string identifier;
    while (pos < source.length() && (std::isalnum(source[pos]) || source[pos] == '_')) {
        identifier += source[pos];
        pos++;
    }
    return identifier;
}

std::string CHTLJSCompiler::extractString(const std::string& source, size_t& pos) {
    std::string str;
    
    if (pos < source.length() && (source[pos] == '"' || source[pos] == '\'')) {
        char quote = source[pos];
        pos++; // 跳过引号
        
        while (pos < source.length() && source[pos] != quote) {
            str += source[pos];
            pos++;
        }
        
        if (pos < source.length() && source[pos] == quote) {
            pos++; // 跳过结束引号
        }
    }
    
    return str;
}

std::string CHTLJSCompiler::extractEnhancedSelector(const std::string& source, size_t& pos) {
    std::string selector;
    
    if (pos < source.length() && source[pos] == '{') {
        pos++; // 跳过{
        
        int braceCount = 1;
        while (pos < source.length() && braceCount > 0) {
            if (source[pos] == '{') braceCount++;
            else if (source[pos] == '}') braceCount--;
            
            if (braceCount > 0) {
                selector += source[pos];
            }
            pos++;
        }
    }
    
    return selector;
}

bool CHTLJSCompiler::isKeyword(const std::string& word) {
    return pImpl->keywords.find(word) != pImpl->keywords.end();
}

void CHTLJSCompiler::addError(const std::string& error) {
    pImpl->errors.push_back(error);
}

} // namespace chtl
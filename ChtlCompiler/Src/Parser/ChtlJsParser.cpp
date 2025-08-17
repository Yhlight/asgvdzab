#include "Parser/ChtlJsParser.h"
#include <regex>
#include <sstream>
#include <algorithm>

namespace Chtl {

ChtlJsParser::ChtlJsParser() : currentPos_(0) {
}

ChtlJsParser::~ChtlJsParser() {
}

bool ChtlJsParser::hasChtlJsSyntax(const std::string& jsCode) const {
    // 检查是否包含CHTL JS特殊语法
    return jsCode.find("{{") != std::string::npos ||
           jsCode.find("->") != std::string::npos ||
           jsCode.find("listen") != std::string::npos ||
           jsCode.find("delegate") != std::string::npos ||
           jsCode.find("animate") != std::string::npos;
}

std::string ChtlJsParser::transform(const std::string& jsCode) {
    if (!hasChtlJsSyntax(jsCode)) {
        return jsCode; // 没有特殊语法，直接返回
    }
    
    std::string result = jsCode;
    
    // 1. 转换 {{selector}} 语法
    std::regex selectorRegex(R"(\{\{([^}]+)\}\})");
    std::smatch match;
    std::string temp = result;
    
    while (std::regex_search(temp, match, selectorRegex)) {
        std::string selector = trim(match[1].str());
        std::string replacement = generateQuerySelector(selector);
        
        result = result.substr(0, match.position()) + 
                replacement + 
                result.substr(match.position() + match.length());
        
        temp = result.substr(match.position() + replacement.length());
    }
    
    // 2. 转换 -> 为 .
    result = replaceAll(result, "->", ".");
    
    // 3. 转换 listen 调用
    std::regex listenRegex(R"((\w+)\.listen\s*\()");
    result = std::regex_replace(result, listenRegex, "$1.addEventListener(");
    
    // 4. 转换 delegate 调用
    if (result.find("delegate") != std::string::npos) {
        // 生成委托辅助函数
        std::string delegateHelper = R"(
// CHTL JS delegate helper
function _chtlDelegate(parent, config) {
    const targets = Array.isArray(config.target) ? config.target : [config.target];
    const handler = config.handler;
    const event = config.event || 'click';
    
    parent.addEventListener(event, function(e) {
        targets.forEach(target => {
            const selector = typeof target === 'string' ? target : target.selector;
            if (e.target.matches(selector)) {
                handler.call(e.target, e);
            }
        });
    });
}
)";
        
        // 转换delegate调用
        std::regex delegateRegex(R"((\w+)\.delegate\s*\(([^)]+)\))");
        result = std::regex_replace(result, delegateRegex, "_chtlDelegate($1, $2)");
        
        // 在开头添加辅助函数
        result = delegateHelper + "\n" + result;
    }
    
    // 5. 转换 animate 调用
    if (result.find("animate") != std::string::npos) {
        // 生成动画辅助函数
        std::string animateHelper = R"(
// CHTL JS animate helper
function _chtlAnimate(config) {
    const targets = Array.isArray(config.target) ? config.target : [config.target];
    const duration = config.duration || 1000;
    const easing = config.easing || 'linear';
    const properties = config.properties || {};
    const onComplete = config.onComplete;
    const onUpdate = config.onUpdate;
    
    const startTime = performance.now();
    
    function easingFunctions(t) {
        switch(easing) {
            case 'linear': return t;
            case 'easeIn': return t * t;
            case 'easeOut': return t * (2 - t);
            case 'easeInOut': return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t;
            default: return t;
        }
    }
    
    function animate(currentTime) {
        const elapsed = currentTime - startTime;
        const progress = Math.min(elapsed / duration, 1);
        const easedProgress = easingFunctions(progress);
        
        targets.forEach(target => {
            Object.keys(properties).forEach(prop => {
                const [from, to] = properties[prop];
                const current = from + (to - from) * easedProgress;
                target.style[prop] = current + 'px';
            });
        });
        
        if (onUpdate) onUpdate(progress);
        
        if (progress < 1) {
            requestAnimationFrame(animate);
        } else if (onComplete) {
            onComplete();
        }
    }
    
    requestAnimationFrame(animate);
    return { stop: () => { /* TODO: implement stop */ } };
}
)";
        
        // 转换animate调用
        std::regex animateRegex(R"(animate\s*\(([^)]+)\))");
        result = std::regex_replace(result, animateRegex, "_chtlAnimate($1)");
        
        // 在开头添加辅助函数
        result = animateHelper + "\n" + result;
    }
    
    return result;
}

std::string ChtlJsParser::generateQuerySelector(const std::string& selector, bool multiple) {
    std::string trimmedSelector = trim(selector);
    std::string actualSelector;
    int index = -1;
    
    // 检查是否有索引访问 [n]
    std::regex indexRegex(R"((.+)\[(\d+)\]$)");
    std::smatch match;
    if (std::regex_match(trimmedSelector, match, indexRegex)) {
        actualSelector = trim(match[1].str());
        index = std::stoi(match[2].str());
    } else {
        actualSelector = trimmedSelector;
    }
    
    // 确定选择器类型
    std::string queryMethod;
    std::string querySelectorString;
    
    if (actualSelector.size() > 0 && actualSelector[0] == '.') {
        // 类选择器
        querySelectorString = actualSelector;
        queryMethod = multiple ? "document.querySelectorAll" : "document.querySelector";
    } else if (actualSelector.size() > 0 && actualSelector[0] == '#') {
        // ID选择器
        querySelectorString = actualSelector;
        queryMethod = "document.querySelector";
    } else if (actualSelector.find(" ") != std::string::npos || 
               actualSelector.find(".") != std::string::npos ||
               actualSelector.find(">") != std::string::npos) {
        // 复合选择器
        querySelectorString = actualSelector;
        queryMethod = multiple ? "document.querySelectorAll" : "document.querySelector";
    } else {
        // 标签选择器 - 需要智能判断
        // 首先尝试作为标签，如果没找到再尝试ID和类
        std::string jsCode = "(function() {\n";
        jsCode += "  let el = document.querySelector('" + actualSelector + "');\n";
        jsCode += "  if (!el) el = document.getElementById('" + actualSelector + "');\n";
        jsCode += "  if (!el) el = document.querySelector('." + actualSelector + "');\n";
        
        if (index >= 0) {
            jsCode += "  if (!el) {\n";
            jsCode += "    const els = document.querySelectorAll('" + actualSelector + "');\n";
            jsCode += "    el = els[" + std::to_string(index) + "];\n";
            jsCode += "  }\n";
        }
        
        jsCode += "  return el;\n";
        jsCode += "})()";
        
        return jsCode;
    }
    
    // 生成查询代码
    if (index >= 0) {
        return queryMethod + "('" + querySelectorString + "')[" + std::to_string(index) + "]";
    } else {
        return queryMethod + "('" + querySelectorString + "')";
    }
}

SelectorExpressionNode::SelectorType ChtlJsParser::determineSelectorType(const std::string& selector) {
    std::string trimmed = trim(selector);
    
    if (trimmed.find("[") != std::string::npos && trimmed.find("]") != std::string::npos) {
        return SelectorExpressionNode::INDEX;
    } else if (trimmed.size() > 0 && trimmed[0] == '.') {
        return SelectorExpressionNode::CLASS;
    } else if (trimmed.size() > 0 && trimmed[0] == '#') {
        return SelectorExpressionNode::ID;
    } else if (trimmed.find(" ") != std::string::npos || 
               trimmed.find(">") != std::string::npos) {
        return SelectorExpressionNode::COMPLEX;
    } else {
        return SelectorExpressionNode::TAG;
    }
}

std::string ChtlJsParser::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

bool ChtlJsParser::startsWith(const std::string& str, const std::string& prefix) {
    return str.size() >= prefix.size() && 
           str.compare(0, prefix.size(), prefix) == 0;
}

std::string ChtlJsParser::replaceAll(const std::string& str, 
                                    const std::string& from, 
                                    const std::string& to) {
    std::string result = str;
    size_t start_pos = 0;
    
    while ((start_pos = result.find(from, start_pos)) != std::string::npos) {
        result.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    
    return result;
}

// 解析方法的简单实现（主要用于AST构建，实际转换在transform中完成）
std::vector<ASTNodePtr> ChtlJsParser::parse(const std::string& jsCode) {
    std::vector<ASTNodePtr> nodes;
    
    // 这里只是简单标记包含CHTL JS语法的节点
    // 实际的详细解析可以根据需要扩展
    if (hasChtlJsSyntax(jsCode)) {
        Token dummyToken(TokenType::SCRIPT, jsCode, "", 1, 1, 0);
        auto jsNode = std::make_shared<ChtlJsExpressionNode>(dummyToken);
        jsNode->setJavaScriptCode(transform(jsCode));
        jsNode->setHasSpecialSyntax(true);
        nodes.push_back(jsNode);
    }
    
    return nodes;
}

ASTNodePtr ChtlJsParser::parseSelectorExpression(const std::string& selector, size_t pos) {
    Token token(TokenType::DOUBLE_LBRACE, selector, "", 1, 1, pos);
    auto node = std::make_shared<SelectorExpressionNode>(token);
    
    node->setSelector(selector);
    node->setSelectorType(determineSelectorType(selector));
    
    // 检查索引
    std::regex indexRegex(R"((.+)\[(\d+)\]$)");
    std::smatch match;
    if (std::regex_match(selector, match, indexRegex)) {
        node->setIndex(std::stoi(match[2].str()));
    }
    
    return node;
}

} // namespace Chtl
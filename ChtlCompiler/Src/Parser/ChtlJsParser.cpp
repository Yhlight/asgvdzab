#include "Parser/ChtlJsParser.h"
#include "Parser/ChtlJsFunctionManager.h"
#include <sstream>
#include <regex>
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
           jsCode.find("listen(") != std::string::npos ||
           jsCode.find("delegate(") != std::string::npos ||
           jsCode.find("animate(") != std::string::npos ||
           jsCode.find("vir ") != std::string::npos;
}

std::vector<ASTNodePtr> ChtlJsParser::parse(const std::string& jsCode) {
    std::vector<ASTNodePtr> nodes;
    currentPos_ = 0;
    
    // 逐行解析
    std::istringstream stream(jsCode);
    std::string line;
    size_t lineStart = 0;
    
    while (std::getline(stream, line)) {
        // 解析每一行中的CHTL JS语法
        size_t pos = 0;
        while (pos < line.length()) {
            // 检查虚对象声明
            if (line.substr(pos).find("vir ") == 0) {
                auto node = parseVirtualObject(line, pos);
                if (node) nodes.push_back(node);
            }
            // 检查选择器表达式
            else if (line.substr(pos).find("{{") == 0) {
                auto node = parseSelectorExpression(line, pos);
                if (node) nodes.push_back(node);
            }
            // 检查箭头访问
            else if (line.find("->", pos) != std::string::npos) {
                auto node = parseArrowAccess(line, pos);
                if (node) nodes.push_back(node);
            }

            else {
                pos++;
            }
        }
        lineStart += line.length() + 1; // +1 for newline
    }
    
    return nodes;
}

std::string ChtlJsParser::transform(const std::string& jsCode) {
    std::string result = jsCode;
    
    // 转换{{选择器}}语法
    std::regex selectorRegex(R"(\{\{([^}]+)\}\})");
    std::smatch match;
    std::string temp = result;
    
    while (std::regex_search(temp, match, selectorRegex)) {
        std::string selector = trim(match[1].str());
        std::string replacement = generateQuerySelector(selector);
        
        size_t pos = result.find(match[0].str());
        if (pos != std::string::npos) {
            result.replace(pos, match[0].length(), replacement);
        }
        
        temp = match.suffix().str();
    }
    
    // 转换箭头语法 -> 为 .
    size_t pos = 0;
    while ((pos = result.find("->", pos)) != std::string::npos) {
        // 检查是否是虚对象调用
        size_t nameStart = pos;
        while (nameStart > 0 && (std::isalnum(result[nameStart-1]) || result[nameStart-1] == '_')) {
            nameStart--;
        }
        std::string objName = result.substr(nameStart, pos - nameStart);
        
        auto& manager = ChtlJsFunctionRegistry::getInstance().getManager();
        if (manager.hasVirtualObject(objName)) {
            // 虚对象调用，需要转换为全局函数调用
            size_t funcEnd = result.find('(', pos + 2);
            if (funcEnd != std::string::npos) {
                std::string funcCall = result.substr(pos + 2, funcEnd - pos - 2);
                
                std::string funcName = funcCall;
                auto funcInfo = manager.findFunction(objName, funcName);
                if (funcInfo) {
                    // 替换为生成的全局函数名
                    result = result.substr(0, nameStart) + 
                            funcInfo->generatedName + 
                            result.substr(funcEnd);
                    pos = nameStart + funcInfo->generatedName.length();
                    continue;
                }
            }
        }
        
        // 普通箭头访问，替换为点
        result[pos] = '.';
        result.erase(pos + 1, 1);
        pos++;
    }
    
    return result;
}

ASTNodePtr ChtlJsParser::parseSelectorExpression(const std::string& code, size_t pos) {
    if (code.substr(pos, 2) != "{{") {
        return nullptr;
    }
    
    size_t start = pos + 2;
    size_t end = code.find("}}", start);
    if (end == std::string::npos) {
        errors_.push_back("Unclosed selector expression");
        return nullptr;
    }
    
    std::string selector = trim(code.substr(start, end - start));
    
    Token token(TokenType::DOUBLE_LBRACE, "{{", "", 1, pos, pos);
    auto node = std::make_shared<SelectorExpressionNode>(token);
    
    // 判断选择器类型并解析
    if (selector.find('[') != std::string::npos && selector.find(']') != std::string::npos) {
        // 索引选择器
        size_t bracketPos = selector.find('[');
        std::string baseSelector = selector.substr(0, bracketPos);
        std::string indexStr = selector.substr(bracketPos + 1, selector.find(']') - bracketPos - 1);
        
        node->setSelector(baseSelector);
        node->setSelectorType(SelectorExpressionNode::INDEX);
        node->setIndex(std::stoi(indexStr));
    } else {
        node->setSelector(selector);
        node->setSelectorType(determineSelectorType(selector));
    }
    
    currentPos_ = end + 2;
    return node;
}

ASTNodePtr ChtlJsParser::parseVirtualObject(const std::string& code, size_t pos) {
    // 格式: vir name = expression
    if (code.substr(pos, 4) != "vir ") {
        return nullptr;
    }
    
    pos += 4;
    while (pos < code.length() && std::isspace(code[pos])) pos++;
    
    // 提取虚对象名称
    size_t nameStart = pos;
    while (pos < code.length() && (std::isalnum(code[pos]) || code[pos] == '_')) {
        pos++;
    }
    std::string name = code.substr(nameStart, pos - nameStart);
    
    // 跳过空格和等号
    while (pos < code.length() && std::isspace(code[pos])) pos++;
    if (pos >= code.length() || code[pos] != '=') {
        errors_.push_back("Expected '=' after virtual object name");
        return nullptr;
    }
    pos++;
    while (pos < code.length() && std::isspace(code[pos])) pos++;
    
    Token token(TokenType::VIR, "vir", "", 1, nameStart, nameStart);
    auto node = std::make_shared<VirtualObjectNode>(token);
    node->setName(name);
    
    // 解析初始化表达式 - 虚对象只是语法糖，需要分析右侧表达式来确定函数映射
    // 简化处理：暂时只支持 listen、delegate、animate
    if (code.find("listen(", pos) == pos) {
        auto listenNode = parseListenCall(code, pos);
        node->setInitExpression(listenNode);
        
        // 注册虚对象 - 用于后续查找
        auto& manager = ChtlJsFunctionRegistry::getInstance().getManager();
        manager.registerVirtualObject(name, "listen");
        
        // TODO: 解析listen的参数对象，提取函数键名
        // 暂时使用简化处理，实际应该解析JavaScript对象字面量
    }
    else if (code.find("delegate(", pos) == pos) {
        auto delegateNode = parseDelegateCall(code, pos);
        node->setInitExpression(delegateNode);
        
        auto& manager = ChtlJsFunctionRegistry::getInstance().getManager();
        manager.registerVirtualObject(name, "delegate");
        
        // TODO: 解析delegate的参数对象
    }
    else if (code.find("animate(", pos) == pos) {
        auto animateNode = parseAnimateCall(code, pos);
        node->setInitExpression(animateNode);
        
        auto& manager = ChtlJsFunctionRegistry::getInstance().getManager();
        manager.registerVirtualObject(name, "animate");
        
        // TODO: 解析animate的参数对象
    }

    
    currentPos_ = pos;
    return node;
}



ASTNodePtr ChtlJsParser::parseArrowAccess(const std::string& code, size_t pos) {
    size_t arrowPos = code.find("->", pos);
    if (arrowPos == std::string::npos) {
        return nullptr;
    }
    
    // 找到箭头前的表达式
    size_t exprStart = arrowPos;
    while (exprStart > 0 && (std::isalnum(code[exprStart-1]) || 
           code[exprStart-1] == '_' || code[exprStart-1] == '}' || 
           code[exprStart-1] == ')' || code[exprStart-1] == ']')) {
        exprStart--;
    }
    
    Token token(TokenType::ARROW, "->", "", 1, arrowPos, arrowPos);
    auto node = std::make_shared<ArrowAccessNode>(token);
    
    // 解析右侧成员
    size_t memberStart = arrowPos + 2;
    size_t memberEnd = memberStart;
    while (memberEnd < code.length() && (std::isalnum(code[memberEnd]) || code[memberEnd] == '_')) {
        memberEnd++;
    }
    
    std::string member = code.substr(memberStart, memberEnd - memberStart);
    node->setMember(member);
    
    // 检查是否是方法调用
    if (memberEnd < code.length() && code[memberEnd] == '(') {
        node->setIsMethodCall(true);
        // TODO: 解析参数
    }
    
    currentPos_ = memberEnd;
    return node;
}

ASTNodePtr ChtlJsParser::parseListenCall(const std::string& code, size_t pos) {
    if (code.substr(pos, 7) != "listen(") {
        return nullptr;
    }
    
    Token token(TokenType::IDENTIFIER, "listen", "", 1, pos, pos);
    auto node = std::make_shared<ListenCallNode>(token);
    
    // TODO: 完整解析listen参数
    
    return node;
}

ASTNodePtr ChtlJsParser::parseDelegateCall(const std::string& code, size_t pos) {
    if (code.substr(pos, 9) != "delegate(") {
        return nullptr;
    }
    
    Token token(TokenType::IDENTIFIER, "delegate", "", 1, pos, pos);
    auto node = std::make_shared<DelegateCallNode>(token);
    
    // TODO: 完整解析delegate参数
    
    return node;
}

ASTNodePtr ChtlJsParser::parseAnimateCall(const std::string& code, size_t pos) {
    if (code.substr(pos, 8) != "animate(") {
        return nullptr;
    }
    
    Token token(TokenType::IDENTIFIER, "animate", "", 1, pos, pos);
    auto node = std::make_shared<AnimateCallNode>(token);
    
    // TODO: 完整解析animate参数
    
    return node;
}

ASTNodePtr ChtlJsParser::parseVirtualCall(const std::string& code, size_t pos) {
    // 格式: obj->func<state>(args)
    size_t arrowPos = code.find("->", pos);
    if (arrowPos == std::string::npos) {
        return nullptr;
    }
    
    // 提取对象名
    size_t objStart = arrowPos;
    while (objStart > 0 && (std::isalnum(code[objStart-1]) || code[objStart-1] == '_')) {
        objStart--;
    }
    std::string objName = code.substr(objStart, arrowPos - objStart);
    
    // 检查是否是虚对象
    auto& manager = ChtlJsFunctionRegistry::getInstance().getManager();
    if (!manager.hasVirtualObject(objName)) {
        return nullptr;
    }
    
    Token token(TokenType::ARROW, "->", "", 1, arrowPos, arrowPos);
    auto node = std::make_shared<VirtualCallNode>(token);
    node->setObjectName(objName);
    
    // 解析函数名
    size_t funcStart = arrowPos + 2;
    size_t funcEnd = funcStart;
    while (funcEnd < code.length() && (std::isalnum(code[funcEnd]) || code[funcEnd] == '_')) {
        funcEnd++;
    }
    
    std::string funcName = code.substr(funcStart, funcEnd - funcStart);
    node->setFunctionName(funcName);
    
    currentPos_ = funcEnd;
    return node;
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

std::string ChtlJsParser::generateQuerySelector(const std::string& selector, bool multiple) {
    std::string actualSelector = trim(selector);
    
    // 处理索引访问
    size_t bracketPos = actualSelector.find('[');
    std::string index;
    if (bracketPos != std::string::npos) {
        size_t bracketEnd = actualSelector.find(']', bracketPos);
        if (bracketEnd != std::string::npos) {
            index = actualSelector.substr(bracketPos + 1, bracketEnd - bracketPos - 1);
            actualSelector = actualSelector.substr(0, bracketPos);
        }
    }
    
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
               actualSelector.find(">") != std::string::npos) {
        // 复合选择器
        querySelectorString = actualSelector;
        queryMethod = multiple ? "document.querySelectorAll" : "document.querySelector";
    } else {
        // 标签选择器
        // 先查找类名或ID为此名称的元素
        querySelectorString = "." + actualSelector + ", #" + actualSelector + ", " + actualSelector;
        queryMethod = multiple ? "document.querySelectorAll" : "document.querySelector";
    }
    
    std::string result = queryMethod + "('" + querySelectorString + "')";
    
    // 如果有索引
    if (!index.empty() && multiple) {
        result = "(" + result + ")[" + index + "]";
    }
    
    return result;
}

std::string ChtlJsParser::generateListenCode(const ListenCallNode* node) {
    std::stringstream ss;
    
    // 获取目标元素
    if (auto selector = std::dynamic_pointer_cast<SelectorExpressionNode>(node->getTarget())) {
        std::string target = generateQuerySelector(selector->getSelector());
        
        // 为每个事件生成addEventListener
        for (const auto& config : node->getEventConfigs()) {
            ss << target << ".addEventListener('" << config.eventName << "', ";
            // TODO: 生成处理函数
            ss << ");\n";
        }
    }
    
    return ss.str();
}

std::string ChtlJsParser::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

} // namespace Chtl
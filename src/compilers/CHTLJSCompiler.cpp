#include "../../include/compilers/CHTLJSCompiler.h"
#include "../../include/scanner/CHTLUnifiedScanner.h"
#include "../../include/common/Utils.h"
#include <iostream>
#include <sstream>
#include <regex>

namespace CHTL {

CHTLJSCompiler::CHTLJSCompiler()
    : current_token_index(0), function_counter(0) {
}

CHTLJSCompiler::CHTLJSCompiler(const Configuration& cfg)
    : config(cfg), current_token_index(0), function_counter(0) {
}

void CHTLJSCompiler::setConfiguration(const Configuration& cfg) {
    config = cfg;
}

CompileResultPtr CHTLJSCompiler::compile(const std::string& chtl_js_code) {
    // 使用扫描器进行词法分析
    CHTLUnifiedScanner scanner(chtl_js_code);
    scanner.setConfiguration(config);
    
    auto token_stream = scanner.tokenize();
    return compile(token_stream);
}

CompileResultPtr CHTLJSCompiler::compile(const std::vector<TokenPtr>& token_stream) {
    auto result = std::make_shared<CompileResult>();
    
    try {
        // 解析Token流生成AST
        ASTNodePtr ast = parse(token_stream);
        
        if (!ast) {
            result->success = false;
            result->errors.push_back("解析失败：无法生成AST");
            return result;
        }
        
        // 生成代码
        std::string generated_code = generateCode(ast);
        
        result->success = errors.empty();
        result->output = generated_code;
        result->errors = errors;
        result->warnings = warnings;
        
    } catch (const std::exception& e) {
        result->success = false;
        result->errors.push_back("编译异常: " + std::string(e.what()));
    }
    
    return result;
}

ASTNodePtr CHTLJSCompiler::parse(const std::vector<TokenPtr>& token_stream) {
    tokens = token_stream;
    current_token_index = 0;
    errors.clear();
    warnings.clear();
    
    return parseProgram();
}

TokenPtr CHTLJSCompiler::currentToken() const {
    if (current_token_index >= tokens.size()) {
        return nullptr;
    }
    return tokens[current_token_index];
}

TokenPtr CHTLJSCompiler::peekToken(size_t offset) const {
    size_t index = current_token_index + offset;
    if (index >= tokens.size()) {
        return nullptr;
    }
    return tokens[index];
}

TokenPtr CHTLJSCompiler::advance() {
    if (current_token_index < tokens.size()) {
        return tokens[current_token_index++];
    }
    return nullptr;
}

bool CHTLJSCompiler::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool CHTLJSCompiler::check(TokenType type) const {
    if (isAtEnd()) return false;
    return currentToken()->type == type;
}

bool CHTLJSCompiler::isAtEnd() const {
    return current_token_index >= tokens.size() || 
           (currentToken() && currentToken()->type == TokenType::EOF_TOKEN);
}

ASTNodePtr CHTLJSCompiler::parseProgram() {
    auto root = std::make_shared<ASTNode>(ASTNodeType::ROOT, "chtl_js_program");
    current_ast = root;
    
    while (!isAtEnd()) {
        // 跳过无关的token
        if (currentToken()->type == TokenType::NEWLINE || 
            currentToken()->type == TokenType::WHITESPACE) {
            advance();
            continue;
        }
        
        ASTNodePtr node = parseStatement();
        if (node) {
            root->addChild(node);
        }
    }
    
    return root;
}

ASTNodePtr CHTLJSCompiler::parseStatement() {
    // 解析CHTL JS语句
    if (check(TokenType::KEYWORD_VIR)) {
        return parseVirObject();
    } else if (currentToken() && currentToken()->value.find("{{") != std::string::npos) {
        return parseEnhancedSelector();
    } else if (check(TokenType::IDENTIFIER)) {
        std::string identifier = currentToken()->value;
        
        // 检查是否为特殊CHTL JS函数
        if (identifier == "listen") {
            return parseListenBlock();
        } else if (identifier == "delegate") {
            return parseDelegateBlock();
        } else if (identifier == "animate") {
            return parseAnimateBlock();
        } else {
            // 普通JS语句 - 作为表达式处理
            return parseExpression();
        }
    } else {
        // 其他JS语句
        return parseExpression();
    }
}

ASTNodePtr CHTLJSCompiler::parseExpression() {
    // 简单的表达式解析 - 收集到下一个分号或语句结束
    auto expr_node = std::make_shared<ASTNode>(ASTNodeType::JS_EXPRESSION, "expression");
    
    std::string expression_content;
    int paren_count = 0;
    int brace_count = 0;
    
    while (!isAtEnd()) {
        TokenPtr token = currentToken();
        
        if (token->type == TokenType::LEFT_PAREN) paren_count++;
        else if (token->type == TokenType::RIGHT_PAREN) paren_count--;
        else if (token->type == TokenType::LEFT_BRACE) brace_count++;
        else if (token->type == TokenType::RIGHT_BRACE) brace_count--;
        
        // 检查是否到达语句结束
        if ((token->type == TokenType::SEMICOLON || 
             token->type == TokenType::RIGHT_BRACE) && 
            paren_count == 0 && brace_count == 0) {
            break;
        }
        
        expression_content += token->value;
        advance();
    }
    
    expr_node->value = expression_content;
    match(TokenType::SEMICOLON); // 可选的分号
    
    return expr_node;
}

ASTNodePtr CHTLJSCompiler::parseVirObject() {
    if (!match(TokenType::KEYWORD_VIR)) {
        addError("期望 'vir' 关键字");
        return nullptr;
    }
    
    if (!check(TokenType::IDENTIFIER)) {
        addError("期望虚对象名称");
        return nullptr;
    }
    
    std::string vir_name = currentToken()->value;
    advance();
    
    if (!match(TokenType::EQUALS)) {
        addError("期望 '='");
        return nullptr;
    }
    
    auto vir_node = std::make_shared<ASTNode>(ASTNodeType::VIR_OBJECT, vir_name);
    
    // 解析虚对象赋值 - 通常是函数调用
    if (check(TokenType::IDENTIFIER)) {
        std::string function_name = currentToken()->value;
        advance();
        
        if (match(TokenType::LEFT_PAREN)) {
            // 解析函数参数
            std::string params_content;
            int paren_count = 1;
            
            while (paren_count > 0 && !isAtEnd()) {
                if (check(TokenType::LEFT_PAREN)) paren_count++;
                else if (check(TokenType::RIGHT_PAREN)) paren_count--;
                
                if (paren_count > 0) {
                    params_content += currentToken()->value;
                }
                advance();
            }
            
            vir_node->value = function_name;
            vir_node->addAttribute("params", params_content);
            
            // 如果是listen函数，需要特殊处理
            if (function_name == "listen") {
                parseListenParameters(vir_node, params_content);
            } else if (function_name == "iNeverAway") {
                parseINeverAwayParameters(vir_node, params_content);
            }
        }
    }
    
    match(TokenType::SEMICOLON);
    
    // 注册虚对象
    registerVirtualObject(vir_name, vir_node);
    
    return vir_node;
}

ASTNodePtr CHTLJSCompiler::parseEnhancedSelector() {
    // 解析增强选择器 {{selector}}
    std::string selector_content;
    
    // 查找完整的选择器内容
    while (!isAtEnd()) {
        std::string token_value = currentToken()->value;
        selector_content += token_value;
        
        if (token_value.find("}}") != std::string::npos) {
            advance();
            break;
        }
        advance();
    }
    
    auto selector_node = std::make_shared<ASTNode>(ASTNodeType::ENHANCED_SELECTOR, "enhanced_selector");
    selector_node->value = selector_content;
    
    return selector_node;
}

ASTNodePtr CHTLJSCompiler::parseListenBlock() {
    if (!check(TokenType::IDENTIFIER) || currentToken()->value != "listen") {
        return nullptr;
    }
    
    advance(); // 'listen'
    
    auto listen_node = std::make_shared<ASTNode>(ASTNodeType::LISTEN_BLOCK, "listen");
    
    if (!match(TokenType::LEFT_PAREN)) {
        addError("期望 '('");
        return listen_node;
    }
    
    // 解析listen参数
    std::string params_content;
    int paren_count = 1;
    
    while (paren_count > 0 && !isAtEnd()) {
        if (check(TokenType::LEFT_PAREN)) paren_count++;
        else if (check(TokenType::RIGHT_PAREN)) paren_count--;
        
        if (paren_count > 0) {
            params_content += currentToken()->value;
        }
        advance();
    }
    
    listen_node->value = params_content;
    return listen_node;
}

ASTNodePtr CHTLJSCompiler::parseDelegateBlock() {
    if (!check(TokenType::IDENTIFIER) || currentToken()->value != "delegate") {
        return nullptr;
    }
    
    advance(); // 'delegate'
    
    auto delegate_node = std::make_shared<ASTNode>(ASTNodeType::DELEGATE_BLOCK, "delegate");
    
    if (!match(TokenType::LEFT_PAREN)) {
        addError("期望 '('");
        return delegate_node;
    }
    
    // 解析delegate参数
    std::string params_content;
    int paren_count = 1;
    
    while (paren_count > 0 && !isAtEnd()) {
        if (check(TokenType::LEFT_PAREN)) paren_count++;
        else if (check(TokenType::RIGHT_PAREN)) paren_count--;
        
        if (paren_count > 0) {
            params_content += currentToken()->value;
        }
        advance();
    }
    
    delegate_node->value = params_content;
    return delegate_node;
}

ASTNodePtr CHTLJSCompiler::parseAnimateBlock() {
    if (!check(TokenType::IDENTIFIER) || currentToken()->value != "animate") {
        return nullptr;
    }
    
    advance(); // 'animate'
    
    auto animate_node = std::make_shared<ASTNode>(ASTNodeType::ANIMATE_BLOCK, "animate");
    
    if (!match(TokenType::LEFT_PAREN)) {
        addError("期望 '('");
        return animate_node;
    }
    
    // 解析animate参数
    std::string params_content;
    int paren_count = 1;
    
    while (paren_count > 0 && !isAtEnd()) {
        if (check(TokenType::LEFT_PAREN)) paren_count++;
        else if (check(TokenType::RIGHT_PAREN)) paren_count--;
        
        if (paren_count > 0) {
            params_content += currentToken()->value;
        }
        advance();
    }
    
    animate_node->value = params_content;
    return animate_node;
}

std::string CHTLJSCompiler::parseSelector() {
    // 解析增强选择器内容
    std::string selector;
    
    // 从{{开始解析
    if (currentToken() && currentToken()->value.find("{{") != std::string::npos) {
        std::string token_value = currentToken()->value;
        
        // 提取{{}}之间的内容
        size_t start = token_value.find("{{");
        size_t end = token_value.find("}}");
        
        if (start != std::string::npos && end != std::string::npos && end > start) {
            selector = token_value.substr(start + 2, end - start - 2);
        }
    }
    
    return Utils::trim(selector);
}

std::string CHTLJSCompiler::parseSelectorWithIndex() {
    std::string selector = parseSelector();
    
    // 检查是否有索引访问 [n]
    std::regex index_regex(R"(\[(\d+)\])");
    std::smatch match;
    
    if (std::regex_search(selector, match, index_regex)) {
        // 有索引访问
        return selector;
    }
    
    return selector;
}

bool CHTLJSCompiler::isValidSelector(const std::string& selector) const {
    if (selector.empty()) return false;
    
    // 简单的选择器验证
    // 支持: .class, #id, element, .class element, element[index]
    return true; // 暂时简化
}

void CHTLJSCompiler::registerVirtualObject(const std::string& name, ASTNodePtr node) {
    virtual_objects[name] = node;
    
    // 解析虚对象中的函数
    if (node && node->hasAttribute("params")) {
        std::string params = node->getAttribute("params");
        parseVirtualObjectFunctions(name, params);
    }
}

void CHTLJSCompiler::parseVirtualObjectFunctions(const std::string& vir_name, const std::string& params) {
    // 解析listen或iNeverAway参数中的函数定义
    // 这里简化实现，实际需要更复杂的解析
    
    std::vector<std::string> functions;
    
    // 查找函数定义模式
    std::regex func_regex(R"((\w+)\s*:\s*function|\w+\s*:\s*\(\s*\)\s*=>|\w+\s*:\s*\w+)");
    std::sregex_iterator iter(params.begin(), params.end(), func_regex);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        std::smatch match = *iter;
        std::string func_name = match[1].str();
        if (!func_name.empty()) {
            functions.push_back(func_name);
        }
    }
    
    virtual_object_functions[vir_name] = functions;
}

void CHTLJSCompiler::parseListenParameters(ASTNodePtr listen_node, const std::string& params) {
    // 解析listen函数的参数，提取事件类型和处理函数
    listen_node->addAttribute("listen_params", params);
}

void CHTLJSCompiler::parseINeverAwayParameters(ASTNodePtr vir_node, const std::string& params) {
    // 解析iNeverAway函数的参数，提取函数定义
    vir_node->addAttribute("ineveraway_params", params);
}

std::string CHTLJSCompiler::generateVirtualFunction(const std::string& vir_name, const std::string& func_name) {
    std::string global_func_name = "chtl_vir_" + vir_name + "_" + func_name + "_" + std::to_string(++function_counter);
    global_function_registry[vir_name + "::" + func_name] = global_func_name;
    return global_func_name;
}

std::string CHTLJSCompiler::resolveVirtualCall(const std::string& vir_name, const std::string& func_name) {
    std::string key = vir_name + "::" + func_name;
    auto it = global_function_registry.find(key);
    
    if (it != global_function_registry.end()) {
        return it->second;
    }
    
    // 如果没有找到，生成一个新的
    return generateVirtualFunction(vir_name, func_name);
}

void CHTLJSCompiler::registerEventDelegation(const std::string& parent_selector, const std::string& child_selector) {
    event_delegation_registry[parent_selector].push_back(child_selector);
}

std::string CHTLJSCompiler::generateDelegationCode(ASTNodePtr delegate_node) {
    if (!delegate_node || delegate_node->type != ASTNodeType::DELEGATE_BLOCK) {
        return "";
    }
    
    std::stringstream js;
    std::string params = delegate_node->value;
    
    // 解析delegate参数并生成事件委托代码
    js << "// 事件委托代码\n";
    js << "(function() {\n";
    js << "  // 解析delegate参数: " << params << "\n";
    js << "  // TODO: 实现完整的事件委托逻辑\n";
    js << "})();\n";
    
    return js.str();
}

std::string CHTLJSCompiler::generateAnimationCode(ASTNodePtr animate_node) {
    if (!animate_node || animate_node->type != ASTNodeType::ANIMATE_BLOCK) {
        return "";
    }
    
    std::stringstream js;
    std::string params = animate_node->value;
    
    // 解析animate参数并生成动画代码
    js << "// 动画代码\n";
    js << "(function() {\n";
    js << "  // 解析animate参数: " << params << "\n";
    js << "  // TODO: 实现完整的动画逻辑\n";
    js << "})();\n";
    
    return js.str();
}

std::string CHTLJSCompiler::parseAnimationKeyframes(ASTNodePtr when_block) {
    // 解析动画关键帧
    return "/* 关键帧解析 */";
}

std::string CHTLJSCompiler::generateCode(ASTNodePtr ast) {
    if (!ast) return "";
    
    return generateJS(ast);
}

std::string CHTLJSCompiler::generateJS(ASTNodePtr node) {
    if (!node) return "";
    
    std::stringstream js;
    
    switch (node->type) {
        case ASTNodeType::ROOT: {
            // 生成所有子节点的JS代码
            for (const auto& child : node->children) {
                js << generateJS(child);
            }
            break;
        }
        
        case ASTNodeType::VIR_OBJECT: {
            // 生成虚对象相关的JS代码
            std::string vir_name = node->name;
            std::string function_name = node->value;
            
            if (function_name == "listen") {
                js << generateListenCode(node);
            } else if (function_name == "iNeverAway") {
                js << generateINeverAwayCode(node);
            }
            break;
        }
        
        case ASTNodeType::ENHANCED_SELECTOR: {
            js << generateEnhancedSelectorCode(node->value);
            break;
        }
        
        case ASTNodeType::LISTEN_BLOCK: {
            js << generateListenCode(node);
            break;
        }
        
        case ASTNodeType::DELEGATE_BLOCK: {
            js << generateDelegationCode(node);
            break;
        }
        
        case ASTNodeType::ANIMATE_BLOCK: {
            js << generateAnimationCode(node);
            break;
        }
        
        case ASTNodeType::JS_EXPRESSION:
        case ASTNodeType::JS_STATEMENT: {
            // 处理增强选择器和箭头操作符
            std::string js_code = node->value;
            js_code = processEnhancedSelectors(js_code);
            js_code = processArrowOperators(js_code);
            js << js_code;
            if (!js_code.empty() && js_code.back() != ';') {
                js << ";";
            }
            js << "\n";
            break;
        }
        
        default:
            // 递归处理子节点
            for (const auto& child : node->children) {
                js << generateJS(child);
            }
            break;
    }
    
    return js.str();
}

std::string CHTLJSCompiler::generateEnhancedSelectorCode(const std::string& selector) {
    std::string cleaned_selector = parseSelector();
    
    if (cleaned_selector.empty()) {
        return "null";
    }
    
    std::stringstream js;
    
    // 检查是否有索引访问
    std::regex index_regex(R"((.+)\[(\d+)\])");
    std::smatch match;
    
    if (std::regex_match(cleaned_selector, match, index_regex)) {
        // 有索引访问
        std::string base_selector = match[1].str();
        std::string index = match[2].str();
        
        js << "document.querySelectorAll('" << base_selector << "')[" << index << "]";
    } else {
        // 普通选择器
        if (cleaned_selector.find(' ') != std::string::npos) {
            // 复合选择器，返回所有匹配元素
            js << "document.querySelectorAll('" << cleaned_selector << "')";
        } else {
            // 简单选择器
            if (cleaned_selector[0] == '.') {
                // 类选择器
                js << "document.getElementsByClassName('" << cleaned_selector.substr(1) << "')[0]";
            } else if (cleaned_selector[0] == '#') {
                // ID选择器
                js << "document.getElementById('" << cleaned_selector.substr(1) << "')";
            } else {
                // 元素选择器
                js << "document.getElementsByTagName('" << cleaned_selector << "')[0]";
            }
        }
    }
    
    return js.str();
}

std::string CHTLJSCompiler::generateListenCode(ASTNodePtr listen_node) {
    if (!listen_node) return "";
    
    std::stringstream js;
    std::string params = listen_node->hasAttribute("params") ? 
                        listen_node->getAttribute("params") : listen_node->value;
    
    // 生成事件监听代码
    js << "// 增强监听器代码\n";
    js << "(function() {\n";
    js << "  const events = " << params << ";\n";
    js << "  for (const [eventType, handler] of Object.entries(events)) {\n";
    js << "    if (typeof handler === 'function') {\n";
    js << "      element.addEventListener(eventType, handler);\n";
    js << "    }\n";
    js << "  }\n";
    js << "})();\n";
    
    return js.str();
}

std::string CHTLJSCompiler::generateINeverAwayCode(ASTNodePtr vir_node) {
    if (!vir_node) return "";
    
    std::stringstream js;
    std::string vir_name = vir_node->name;
    std::string params = vir_node->hasAttribute("params") ? 
                        vir_node->getAttribute("params") : vir_node->value;
    
    // 生成iNeverAway相关的全局函数
    js << "// iNeverAway虚对象: " << vir_name << "\n";
    
    // 解析参数中的函数定义并生成全局函数
    auto functions = virtual_object_functions[vir_name];
    for (const auto& func_name : functions) {
        std::string global_func_name = generateVirtualFunction(vir_name, func_name);
        js << "function " << global_func_name << "() {\n";
        js << "  // TODO: 从参数中提取函数体\n";
        js << "}\n";
    }
    
    return js.str();
}

std::string CHTLJSCompiler::processEnhancedSelectors(const std::string& js_code) {
    std::string result = js_code;
    
    // 查找并替换所有的{{selector}}
    std::regex selector_regex(R"(\{\{([^}]+)\}\})");
    std::sregex_iterator iter(result.begin(), result.end(), selector_regex);
    std::sregex_iterator end;
    
    // 收集所有匹配项并倒序替换（避免位置偏移）
    std::vector<std::pair<std::string, std::string>> replacements;
    
    for (; iter != end; ++iter) {
        std::smatch match = *iter;
        std::string full_match = match[0].str();
        std::string selector_content = match[1].str();
        
        // 生成对应的DOM查询代码
        std::string replacement = generateSelectorCode(selector_content);
        replacements.push_back({full_match, replacement});
    }
    
    // 执行替换
    for (const auto& replacement : replacements) {
        result = Utils::replace(result, replacement.first, replacement.second);
    }
    
    return result;
}

std::string CHTLJSCompiler::processArrowOperators(const std::string& js_code) {
    // 将->替换为.
    return Utils::replace(js_code, "->", ".");
}

std::string CHTLJSCompiler::generateSelectorCode(const std::string& selector) {
    std::string cleaned_selector = Utils::trim(selector);
    
    if (cleaned_selector.empty()) {
        return "null";
    }
    
    std::stringstream js;
    
    // 检查是否有索引访问
    std::regex index_regex(R"((.+)\[(\d+)\])");
    std::smatch match;
    
    if (std::regex_match(cleaned_selector, match, index_regex)) {
        // 有索引访问
        std::string base_selector = Utils::trim(match[1].str());
        std::string index = match[2].str();
        
        js << "document.querySelectorAll('" << base_selector << "')[" << index << "]";
    } else {
        // 普通选择器
        if (cleaned_selector.find(' ') != std::string::npos) {
            // 复合选择器，返回所有匹配元素
            js << "document.querySelectorAll('" << cleaned_selector << "')";
        } else {
            // 简单选择器
            if (cleaned_selector[0] == '.') {
                // 类选择器
                js << "document.getElementsByClassName('" << cleaned_selector.substr(1) << "')[0]";
            } else if (cleaned_selector[0] == '#') {
                // ID选择器
                js << "document.getElementById('" << cleaned_selector.substr(1) << "')";
            } else {
                // 元素选择器或变量
                // 检查是否在选择器映射中
                auto it = selector_mapping.find(cleaned_selector);
                if (it != selector_mapping.end()) {
                    js << "document.getElementsByClassName('" << it->second << "')[0]";
                } else {
                    js << "document.getElementsByTagName('" << cleaned_selector << "')[0]";
                }
            }
        }
    }
    
    return js.str();
}

void CHTLJSCompiler::setSelectorMapping(const std::map<std::string, std::string>& mapping) {
    selector_mapping = mapping;
}

std::vector<std::string> CHTLJSCompiler::getVirtualObjectNames() const {
    std::vector<std::string> names;
    for (const auto& pair : virtual_objects) {
        names.push_back(pair.first);
    }
    return names;
}

std::string CHTLJSCompiler::getVirtualObjectFunction(const std::string& vir_name, const std::string& func_name) const {
    std::string key = vir_name + "::" + func_name;
    auto it = global_function_registry.find(key);
    
    if (it != global_function_registry.end()) {
        return it->second;
    }
    
    return "";
}

std::vector<std::string> CHTLJSCompiler::getGeneratedGlobalFunctions() const {
    std::vector<std::string> functions;
    for (const auto& pair : global_function_registry) {
        functions.push_back(pair.second);
    }
    return functions;
}

std::string CHTLJSCompiler::getEventDelegationCode() const {
    std::stringstream js;
    
    js << "// 事件委托管理器\n";
    js << "(function() {\n";
    js << "  const delegationRegistry = {};\n";
    
    for (const auto& delegation : event_delegation_registry) {
        const std::string& parent = delegation.first;
        const auto& children = delegation.second;
        
        js << "  // 父选择器: " << parent << "\n";
        for (const auto& child : children) {
            js << "  // 子选择器: " << child << "\n";
        }
    }
    
    js << "})();\n";
    
    return js.str();
}

void CHTLJSCompiler::addError(const std::string& message) {
    std::string formatted_error = formatError(message, currentToken());
    errors.push_back(formatted_error);
    
    if (config.debug_mode) {
        Utils::debugLog("CHTL_JS_COMPILER_ERROR", formatted_error);
    }
}

void CHTLJSCompiler::addWarning(const std::string& message) {
    std::string formatted_warning = "警告: " + message;
    warnings.push_back(formatted_warning);
    
    if (config.debug_mode) {
        Utils::debugLog("CHTL_JS_COMPILER_WARNING", formatted_warning);
    }
}

std::string CHTLJSCompiler::formatError(const std::string& message, TokenPtr token) {
    if (token) {
        return Utils::formatError(message, token->line, token->column);
    }
    return "错误: " + message;
}

void CHTLJSCompiler::reset() {
    current_token_index = 0;
    current_ast = nullptr;
    errors.clear();
    warnings.clear();
    virtual_objects.clear();
    virtual_object_functions.clear();
    global_function_registry.clear();
    event_delegation_registry.clear();
    function_counter = 0;
}

void CHTLJSCompiler::dumpAST(ASTNodePtr node) const {
    if (!node) {
        node = current_ast;
    }
    
    if (node) {
        std::cout << "=== CHTL JS AST Dump ===" << std::endl;
        std::cout << node->toString() << std::endl;
        std::cout << "========================" << std::endl;
    }
}

void CHTLJSCompiler::setDebugMode(bool enabled) {
    config.debug_mode = enabled;
}

} // namespace CHTL
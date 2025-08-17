#include "compiler/chtljs/chtljs_parser.h"
#include <sstream>
#include <iostream>

namespace chtl {

CHTLJSParser::CHTLJSParser(CHTLJSLexer& lexer)
    : lexer_(lexer) {
    advance(); // 读取第一个token
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parse() {
    auto root = std::make_unique<CHTLJSRootNode>();
    
    while (!isAtEnd()) {
        try {
            // 跳过注释
            while (current_.type == CHTLJSTokenType::COMMENT) {
                advance();
            }
            
            if (isAtEnd()) break;
            
            auto stmt = parseStatement();
            if (stmt) {
                root->addChild(std::move(stmt));
            }
        } catch (const std::exception& e) {
            error(e.what());
            synchronize();
        }
    }
    
    return root;
}

void CHTLJSParser::advance() {
    previous_ = current_;
    current_ = lexer_.nextToken();
}

bool CHTLJSParser::check(CHTLJSTokenType type) {
    return current_.type == type;
}

bool CHTLJSParser::match(CHTLJSTokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool CHTLJSParser::match(const std::string& value) {
    if (current_.value == value) {
        advance();
        return true;
    }
    return false;
}

CHTLJSToken CHTLJSParser::consume(CHTLJSTokenType type, const std::string& message) {
    if (check(type)) {
        CHTLJSToken token = current_;
        advance();
        return token;
    }
    
    error(message);
    throw std::runtime_error(message);
}

CHTLJSToken CHTLJSParser::consume(const std::string& value, const std::string& message) {
    if (current_.value == value) {
        CHTLJSToken token = current_;
        advance();
        return token;
    }
    
    error(message);
    throw std::runtime_error(message);
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parseStatement() {
    // 检查CHTL JS特殊语法
    if (isEnhancedSelector()) {
        return parseExpression();
    }
    
    // vir声明
    if (check(CHTLJSTokenType::VIR)) {
        return parseVirDeclaration();
    }
    
    // 特殊函数
    if (isSpecialFunction()) {
        return parseExpression();
    }
    
    // 其他表达式
    return parseExpression();
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parseExpression() {
    auto expr = parsePrimary();
    
    // 处理后续操作
    while (true) {
        if (check(CHTLJSTokenType::ARROW)) {
            expr = parseArrowAccess(std::move(expr));
        } else if (check(CHTLJSTokenType::DOT)) {
            expr = parseMemberAccess(std::move(expr));
        } else if (check(CHTLJSTokenType::LPAREN)) {
            expr = parseFunctionCall(std::move(expr));
        } else {
            break;
        }
    }
    
    return expr;
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parseEnhancedSelector() {
    auto token = consume(CHTLJSTokenType::ENHANCED_SELECTOR, "Expected enhanced selector");
    return std::make_unique<EnhancedSelectorNode>(token.value, token.line, token.column);
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parseArrowAccess(std::unique_ptr<CHTLJSASTNode> left) {
    consume(CHTLJSTokenType::ARROW, "Expected '->'");
    
    auto right = parsePrimary();
    
    auto arrow = std::make_unique<ArrowAccessNode>(current_.line, current_.column);
    arrow->setLeft(std::move(left));
    arrow->setRight(std::move(right));
    
    return arrow;
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parseListenFunction() {
    consume(CHTLJSTokenType::LISTEN, "Expected 'listen'");
    consume(CHTLJSTokenType::LPAREN, "Expected '(' after 'listen'");
    
    // 解析选择器
    auto selector = parseExpression();
    consume(CHTLJSTokenType::COMMA, "Expected ',' after selector");
    
    // 解析事件
    auto event = parseStringLiteral();
    consume(CHTLJSTokenType::COMMA, "Expected ',' after event");
    
    // 解析处理函数
    auto handler = parseFunctionExpression();
    
    consume(CHTLJSTokenType::RPAREN, "Expected ')' after listen arguments");
    
    auto listen = std::make_unique<ListenFunctionNode>(current_.line, current_.column);
    listen->setSelector(std::move(selector));
    listen->setEvent(std::move(event));
    listen->setHandler(std::move(handler));
    
    return listen;
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parseDelegateFunction() {
    consume(CHTLJSTokenType::DELEGATE, "Expected 'delegate'");
    consume(CHTLJSTokenType::LPAREN, "Expected '(' after 'delegate'");
    
    // 解析父选择器
    auto parent = parseExpression();
    consume(CHTLJSTokenType::COMMA, "Expected ',' after parent selector");
    
    // 解析子选择器
    auto child = parseStringLiteral();
    consume(CHTLJSTokenType::COMMA, "Expected ',' after child selector");
    
    // 解析事件
    auto event = parseStringLiteral();
    consume(CHTLJSTokenType::COMMA, "Expected ',' after event");
    
    // 解析处理函数
    auto handler = parseFunctionExpression();
    
    consume(CHTLJSTokenType::RPAREN, "Expected ')' after delegate arguments");
    
    auto delegate = std::make_unique<DelegateFunctionNode>(current_.line, current_.column);
    delegate->setParent(std::move(parent));
    delegate->setChild(std::move(child));
    delegate->setEvent(std::move(event));
    delegate->setHandler(std::move(handler));
    
    return delegate;
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parseAnimateFunction() {
    consume(CHTLJSTokenType::ANIMATE, "Expected 'animate'");
    consume(CHTLJSTokenType::LPAREN, "Expected '(' after 'animate'");
    
    // 解析选择器
    auto selector = parseExpression();
    consume(CHTLJSTokenType::COMMA, "Expected ',' after selector");
    
    // 解析动画属性
    auto properties = parseObjectLiteral();
    consume(CHTLJSTokenType::COMMA, "Expected ',' after properties");
    
    // 解析持续时间
    auto duration = parseNumberLiteral();
    
    // 可选的缓动函数
    std::unique_ptr<CHTLJSASTNode> easing;
    if (match(CHTLJSTokenType::COMMA)) {
        easing = parseStringLiteral();
    }
    
    // 可选的回调函数
    std::unique_ptr<CHTLJSASTNode> callback;
    if (match(CHTLJSTokenType::COMMA)) {
        callback = parseFunctionExpression();
    }
    
    consume(CHTLJSTokenType::RPAREN, "Expected ')' after animate arguments");
    
    auto animate = std::make_unique<AnimateFunctionNode>(current_.line, current_.column);
    animate->setSelector(std::move(selector));
    animate->setProperties(std::move(properties));
    animate->setDuration(std::move(duration));
    if (easing) animate->setEasing(std::move(easing));
    if (callback) animate->setCallback(std::move(callback));
    
    return animate;
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parseVirDeclaration() {
    consume(CHTLJSTokenType::VIR, "Expected 'vir'");
    
    auto name = consume(CHTLJSTokenType::IDENTIFIER, "Expected identifier after 'vir'");
    
    consume(CHTLJSTokenType::EQUALS, "Expected '=' after vir name");
    
    auto object = parseObjectLiteral();
    
    auto vir = std::make_unique<VirDeclarationNode>(name.value, current_.line, current_.column);
    vir->setObject(std::move(object));
    
    return vir;
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parseVirAccess() {
    auto name = consume(CHTLJSTokenType::IDENTIFIER, "Expected vir name");
    consume(CHTLJSTokenType::DOT, "Expected '.' after vir name");
    auto method = consume(CHTLJSTokenType::IDENTIFIER, "Expected method name");
    
    auto access = std::make_unique<VirAccessNode>(name.value, method.value, 
                                                  current_.line, current_.column);
    
    // 如果后面是函数调用
    if (check(CHTLJSTokenType::LPAREN)) {
        return parseFunctionCall(std::move(access));
    }
    
    return access;
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parsePrimary() {
    // 增强选择器
    if (isEnhancedSelector()) {
        return parseEnhancedSelector();
    }
    
    // 特殊函数
    if (check(CHTLJSTokenType::LISTEN)) {
        return parseListenFunction();
    }
    if (check(CHTLJSTokenType::DELEGATE)) {
        return parseDelegateFunction();
    }
    if (check(CHTLJSTokenType::ANIMATE)) {
        return parseAnimateFunction();
    }
    
    // 标识符（可能是vir访问）
    if (check(CHTLJSTokenType::IDENTIFIER)) {
        // 检查是否是vir访问模式
        if (lexer_.peekToken().type == CHTLJSTokenType::DOT) {
            // 可能是vir访问
            return parseVirAccess();
        }
        return parseIdentifier();
    }
    
    // 字面量
    if (check(CHTLJSTokenType::STRING_LITERAL)) {
        return parseStringLiteral();
    }
    if (check(CHTLJSTokenType::NUMBER)) {
        return parseNumberLiteral();
    }
    
    // 对象字面量
    if (check(CHTLJSTokenType::LBRACE)) {
        return parseObjectLiteral();
    }
    
    // 数组字面量
    if (check(CHTLJSTokenType::LBRACKET)) {
        return parseArrayLiteral();
    }
    
    // 函数表达式
    if (match("function")) {
        return parseFunctionExpression();
    }
    
    // 括号表达式
    if (match(CHTLJSTokenType::LPAREN)) {
        auto expr = parseExpression();
        consume(CHTLJSTokenType::RPAREN, "Expected ')' after expression");
        return expr;
    }
    
    error("Unexpected token: " + current_.value);
    advance();
    return nullptr;
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parseIdentifier() {
    auto token = consume(CHTLJSTokenType::IDENTIFIER, "Expected identifier");
    return std::make_unique<CHTLJSIdentifierNode>(token.value, token.line, token.column);
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parseStringLiteral() {
    auto token = consume(CHTLJSTokenType::STRING_LITERAL, "Expected string literal");
    return std::make_unique<CHTLJSStringLiteralNode>(token.value, token.line, token.column);
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parseNumberLiteral() {
    auto token = consume(CHTLJSTokenType::NUMBER, "Expected number");
    return std::make_unique<CHTLJSNumberLiteralNode>(token.value, token.line, token.column);
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parseObjectLiteral() {
    consume(CHTLJSTokenType::LBRACE, "Expected '{'");
    
    auto object = std::make_unique<ObjectLiteralNode>(current_.line, current_.column);
    
    while (!check(CHTLJSTokenType::RBRACE) && !isAtEnd()) {
        // 属性名
        std::string key;
        if (check(CHTLJSTokenType::STRING_LITERAL)) {
            key = consume(CHTLJSTokenType::STRING_LITERAL, "Expected property name").value;
        } else {
            key = consume(CHTLJSTokenType::IDENTIFIER, "Expected property name").value;
        }
        
        consume(CHTLJSTokenType::COLON, "Expected ':' after property name");
        
        // 属性值
        auto value = parseExpression();
        
        object->addProperty(key, std::move(value));
        
        if (!match(CHTLJSTokenType::COMMA)) {
            break;
        }
    }
    
    consume(CHTLJSTokenType::RBRACE, "Expected '}'");
    
    return object;
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parseArrayLiteral() {
    consume(CHTLJSTokenType::LBRACKET, "Expected '['");
    
    auto array = std::make_unique<ArrayLiteralNode>(current_.line, current_.column);
    
    while (!check(CHTLJSTokenType::RBRACKET) && !isAtEnd()) {
        array->addElement(parseExpression());
        
        if (!match(CHTLJSTokenType::COMMA)) {
            break;
        }
    }
    
    consume(CHTLJSTokenType::RBRACKET, "Expected ']'");
    
    return array;
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parseFunctionExpression() {
    consume("function", "Expected 'function'");
    
    auto func = std::make_unique<FunctionExpressionNode>(current_.line, current_.column);
    
    // 可选的函数名
    if (check(CHTLJSTokenType::IDENTIFIER)) {
        func->setName(consume(CHTLJSTokenType::IDENTIFIER, "Expected function name").value);
    }
    
    // 参数列表
    consume(CHTLJSTokenType::LPAREN, "Expected '(' after function");
    
    while (!check(CHTLJSTokenType::RPAREN) && !isAtEnd()) {
        func->addParameter(consume(CHTLJSTokenType::IDENTIFIER, "Expected parameter name").value);
        
        if (!match(CHTLJSTokenType::COMMA)) {
            break;
        }
    }
    
    consume(CHTLJSTokenType::RPAREN, "Expected ')' after parameters");
    
    // 函数体
    consume(CHTLJSTokenType::LBRACE, "Expected '{' before function body");
    
    // 简化：将函数体作为原始JavaScript代码
    std::string body;
    int brace_count = 1;
    
    while (brace_count > 0 && !isAtEnd()) {
        if (check(CHTLJSTokenType::LBRACE)) {
            brace_count++;
        } else if (check(CHTLJSTokenType::RBRACE)) {
            brace_count--;
            if (brace_count == 0) {
                advance();
                break;
            }
        }
        
        body += current_.value + " ";
        advance();
    }
    
    auto body_node = std::make_unique<JSCodeBlockNode>(body, current_.line, current_.column);
    func->setBody(std::move(body_node));
    
    return func;
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parseMemberAccess(std::unique_ptr<CHTLJSASTNode> object) {
    consume(CHTLJSTokenType::DOT, "Expected '.'");
    
    auto member = consume(CHTLJSTokenType::IDENTIFIER, "Expected member name");
    
    auto access = std::make_unique<MemberAccessNode>(current_.line, current_.column);
    access->setObject(std::move(object));
    access->setMember(member.value);
    
    return access;
}

std::unique_ptr<CHTLJSASTNode> CHTLJSParser::parseFunctionCall(std::unique_ptr<CHTLJSASTNode> function) {
    consume(CHTLJSTokenType::LPAREN, "Expected '('");
    
    auto call = std::make_unique<FunctionCallNode>(current_.line, current_.column);
    call->setFunction(std::move(function));
    
    // 解析参数
    while (!check(CHTLJSTokenType::RPAREN) && !isAtEnd()) {
        call->addArgument(parseExpression());
        
        if (!match(CHTLJSTokenType::COMMA)) {
            break;
        }
    }
    
    consume(CHTLJSTokenType::RPAREN, "Expected ')' after arguments");
    
    return call;
}

bool CHTLJSParser::isAtEnd() {
    return current_.type == CHTLJSTokenType::EOF_TOKEN;
}

bool CHTLJSParser::isSpecialFunction() {
    return current_.type == CHTLJSTokenType::LISTEN ||
           current_.type == CHTLJSTokenType::DELEGATE ||
           current_.type == CHTLJSTokenType::ANIMATE;
}

bool CHTLJSParser::isEnhancedSelector() {
    return current_.type == CHTLJSTokenType::ENHANCED_SELECTOR;
}

void CHTLJSParser::error(const std::string& message) {
    std::stringstream ss;
    ss << "Line " << current_.line << ", column " << current_.column << ": " << message;
    errors_.push_back(ss.str());
}

void CHTLJSParser::warning(const std::string& message) {
    std::stringstream ss;
    ss << "Line " << current_.line << ", column " << current_.column << ": " << message;
    warnings_.push_back(ss.str());
}

void CHTLJSParser::synchronize() {
    advance();
    
    while (!isAtEnd()) {
        if (previous_.type == CHTLJSTokenType::SEMICOLON) return;
        
        advance();
    }
}

} // namespace chtl
#include "compiler/chtljs/chtljs_compiler.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>
#include <iostream>

namespace chtl {

// CHTL JS特殊函数
const std::unordered_set<std::string> CHTLJSCompiler::chtljs_functions_ = {
    "listen", "delegate", "animate"
};

// ==================== Lexer 实现 ====================

CHTLJSCompiler::Lexer::Lexer(const std::string& input, size_t start_line)
    : input_(input), position_(0), line_(start_line), column_(1) {
}

CHTLJSCompiler::Lexer::Token CHTLJSCompiler::Lexer::nextToken() {
    if (peeked_token_.has_value()) {
        Token token = peeked_token_.value();
        peeked_token_.reset();
        return token;
    }
    
    skipWhitespace();
    
    if (isAtEnd()) {
        return Token{EOF_TOKEN, "", line_, column_};
    }
    
    size_t start_line = line_;
    size_t start_column = column_;
    char c = peek();
    
    // 增强选择器
    if (c == '{' && peek(1) == '{') {
        return scanSelector();
    }
    
    // 箭头操作符
    if (c == '-' && peek(1) == '>') {
        advance();
        advance();
        return Token{ARROW, "=>", start_line, start_column};
    }
    
    // 胖箭头
    if (c == '=' && peek(1) == '>') {
        advance();
        advance();
        return Token{ARROW, "=>", start_line, start_column};
    }
    
    // 比较操作符
    if (c == '=' && peek(1) == '=') {
        advance();
        advance();
        return Token{EQ, "==", start_line, start_column};
    }
    
    if (c == '!' && peek(1) == '=') {
        advance();
        advance();
        return Token{NE, "!=", start_line, start_column};
    }
    
    if (c == '<' && peek(1) == '=') {
        advance();
        advance();
        return Token{LE, "<=", start_line, start_column};
    }
    
    if (c == '>' && peek(1) == '=') {
        advance();
        advance();
        return Token{GE, ">=", start_line, start_column};
    }
    
    // 逻辑操作符
    if (c == '&' && peek(1) == '&') {
        advance();
        advance();
        return Token{AND, "&&", start_line, start_column};
    }
    
    if (c == '|' && peek(1) == '|') {
        advance();
        advance();
        return Token{OR, "||", start_line, start_column};
    }
    
    // 单字符操作符
    switch (c) {
        case '{': advance(); return Token{LBRACE, "{", start_line, start_column};
        case '}': advance(); return Token{RBRACE, "}", start_line, start_column};
        case '(': advance(); return Token{LPAREN, "(", start_line, start_column};
        case ')': advance(); return Token{RPAREN, ")", start_line, start_column};
        case '[': advance(); return Token{LBRACKET, "[", start_line, start_column};
        case ']': advance(); return Token{RBRACKET, "]", start_line, start_column};
        case '.': advance(); return Token{DOT, ".", start_line, start_column};
        case ',': advance(); return Token{COMMA, ",", start_line, start_column};
        case ';': advance(); return Token{SEMICOLON, ";", start_line, start_column};
        case ':': advance(); return Token{COLON, ":", start_line, start_column};
        case '=': advance(); return Token{EQUALS, "=", start_line, start_column};
        case '+': advance(); return Token{PLUS, "+", start_line, start_column};
        case '-': advance(); return Token{MINUS, "-", start_line, start_column};
        case '*': advance(); return Token{STAR, "*", start_line, start_column};
        case '/': advance(); return Token{SLASH, "/", start_line, start_column};
        case '<': advance(); return Token{LT, "<", start_line, start_column};
        case '>': advance(); return Token{GT, ">", start_line, start_column};
        case '!': advance(); return Token{NOT, "!", start_line, start_column};
    }
    
    // 字符串
    if (c == '"' || c == '\'' || c == '`') {
        return scanString();
    }
    
    // 数字
    if (std::isdigit(c)) {
        return scanNumber();
    }
    
    // 标识符或关键字
    if (std::isalpha(c) || c == '_' || c == '$') {
        return scanIdentifier();
    }
    
    // 未知字符
    advance();
    return Token{ERROR, std::string(1, c), start_line, start_column};
}

CHTLJSCompiler::Lexer::Token CHTLJSCompiler::Lexer::peekToken() {
    if (!peeked_token_.has_value()) {
        peeked_token_ = nextToken();
    }
    return peeked_token_.value();
}

void CHTLJSCompiler::Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r') {
            advance();
        } else if (c == '\n') {
            line_++;
            column_ = 1;
            position_++;
        } else if (c == '/' && peek(1) == '/') {
            // 单行注释
            advance();
            advance();
            while (!isAtEnd() && peek() != '\n') {
                advance();
            }
        } else if (c == '/' && peek(1) == '*') {
            // 多行注释
            advance();
            advance();
            while (!isAtEnd()) {
                if (peek() == '*' && peek(1) == '/') {
                    advance();
                    advance();
                    break;
                }
                if (peek() == '\n') {
                    line_++;
                    column_ = 1;
                    position_++;
                } else {
                    advance();
                }
            }
        } else {
            break;
        }
    }
}

CHTLJSCompiler::Lexer::Token CHTLJSCompiler::Lexer::scanIdentifier() {
    size_t start_line = line_;
    size_t start_column = column_;
    std::string value;
    
    while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_' || peek() == '$')) {
        value += peek();
        advance();
    }
    
    // 检查关键字
    TokenType type = IDENTIFIER;
    if (value == "vir") type = VIR;
    else if (value == "const") type = CONST;
    else if (value == "let") type = LET;
    else if (value == "var") type = VAR;
    else if (value == "function") type = FUNCTION;
    else if (value == "return") type = RETURN;
    else if (value == "if") type = IF;
    else if (value == "else") type = ELSE;
    else if (value == "for") type = FOR;
    else if (value == "while") type = WHILE;
    else if (value == "listen") type = LISTEN;
    else if (value == "delegate") type = DELEGATE;
    else if (value == "animate") type = ANIMATE;
    
    return Token{type, value, start_line, start_column};
}

CHTLJSCompiler::Lexer::Token CHTLJSCompiler::Lexer::scanString() {
    size_t start_line = line_;
    size_t start_column = column_;
    char delimiter = peek();
    std::string value;
    value += delimiter;
    advance();
    
    bool escaped = false;
    while (!isAtEnd()) {
        char c = peek();
        
        if (escaped) {
            value += c;
            escaped = false;
            advance();
            continue;
        }
        
        if (c == '\\') {
            escaped = true;
            value += c;
            advance();
            continue;
        }
        
        if (c == delimiter) {
            value += c;
            advance();
            break;
        }
        
        if (c == '\n') {
            line_++;
            column_ = 1;
            position_++;
        } else {
            advance();
        }
        
        value += c;
    }
    
    return Token{STRING, value, start_line, start_column};
}

CHTLJSCompiler::Lexer::Token CHTLJSCompiler::Lexer::scanNumber() {
    size_t start_line = line_;
    size_t start_column = column_;
    std::string value;
    
    while (!isAtEnd() && std::isdigit(peek())) {
        value += peek();
        advance();
    }
    
    if (peek() == '.' && std::isdigit(peek(1))) {
        value += peek();
        advance();
        while (!isAtEnd() && std::isdigit(peek())) {
            value += peek();
            advance();
        }
    }
    
    return Token{NUMBER, value, start_line, start_column};
}

CHTLJSCompiler::Lexer::Token CHTLJSCompiler::Lexer::scanSelector() {
    size_t start_line = line_;
    size_t start_column = column_;
    std::string value;
    
    advance(); // {{
    advance();
    
    while (!isAtEnd()) {
        if (peek() == '}' && peek(1) == '}') {
            advance(); // }}
            advance();
            break;
        }
        
        char c = peek();
        advance();
        
        if (c == '\n') {
            line_++;
            column_ = 1;
        }
        
        value += c;
    }
    
    return Token{SELECTOR_START, value, start_line, start_column};
}

char CHTLJSCompiler::Lexer::peek(size_t offset) const {
    size_t pos = position_ + offset;
    if (pos >= input_.length() || pos < 0) {
        return '\0';
    }
    return input_[pos];
}

void CHTLJSCompiler::Lexer::advance() {
    if (!isAtEnd()) {
        column_++;
        position_++;
    }
}

// ==================== Parser 实现 ====================

CHTLJSCompiler::Parser::Parser(Lexer& lexer, CompilationContext& context)
    : lexer_(lexer), context_(context) {
}

std::unique_ptr<CHTLJSCompiler::ASTNode> CHTLJSCompiler::Parser::parse() {
    return parseProgram();
}

std::unique_ptr<CHTLJSCompiler::ASTNode> CHTLJSCompiler::Parser::parseProgram() {
    auto program = std::make_unique<ASTNode>(ASTNodeType::PROGRAM);
    
    while (!lexer_.isAtEnd()) {
        auto stmt = parseStatement();
        if (stmt) {
            program->children.push_back(std::move(stmt));
        }
    }
    
    return program;
}

std::unique_ptr<CHTLJSCompiler::ASTNode> CHTLJSCompiler::Parser::parseStatement() {
    Lexer::Token token = lexer_.peekToken();
    
    if (token.type == Lexer::VIR) {
        return parseVirDeclaration();
    }
    
    // 处理变量声明
    if (token.type == Lexer::VAR || token.type == Lexer::LET || token.type == Lexer::CONST ||
        (token.type == Lexer::IDENTIFIER && (token.value == "var" || token.value == "let" || token.value == "const"))) {
        return parseVariableDeclaration();
    }
    
    // 其他语句类型暂时作为表达式语句处理
    return parseExpressionStatement();
}

std::unique_ptr<CHTLJSCompiler::ASTNode> CHTLJSCompiler::Parser::parseVirDeclaration() {
    auto vir_node = std::make_unique<ASTNode>(ASTNodeType::VIR_DECLARATION);
    
    expect(Lexer::VIR); // 消费 'vir'
    
    // 获取虚对象名称
    Lexer::Token name_token = lexer_.nextToken();
    if (name_token.type != Lexer::IDENTIFIER) {
        reportError("Expected identifier after 'vir'");
        return nullptr;
    }
    vir_node->value = name_token.value;
    
    // 期待 =
    if (!expect(Lexer::EQUALS)) {
        return nullptr;
    }
    
    // 解析右侧表达式
    auto expr = parseExpression();
    if (expr) {
        vir_node->children.push_back(std::move(expr));
    }
    
    // 可选的分号
    match(Lexer::SEMICOLON);
    
    return vir_node;
}

std::unique_ptr<CHTLJSCompiler::ASTNode> CHTLJSCompiler::Parser::parseVariableDeclaration() {
    auto var_node = std::make_unique<ASTNode>(ASTNodeType::VARIABLE_DECLARATION);
    
    // 获取声明类型 (var/let/const)
    Lexer::Token decl_type = lexer_.nextToken();
    
    // 确保获取到正确的关键字值
    if (decl_type.type == Lexer::VAR || decl_type.value == "var") {
        var_node->value = "var";
    } else if (decl_type.type == Lexer::LET || decl_type.value == "let") {
        var_node->value = "let";
    } else if (decl_type.type == Lexer::CONST || decl_type.value == "const") {
        var_node->value = "const";
    } else {
        var_node->value = decl_type.value;
    }
    
    // 获取变量名
    Lexer::Token name_token = lexer_.nextToken();
    if (name_token.type != Lexer::IDENTIFIER) {
        reportError("Expected identifier after '" + var_node->value + "'");
        return nullptr;
    }
    
    auto name_node = std::make_unique<ASTNode>(ASTNodeType::IDENTIFIER);
    name_node->value = name_token.value;
    var_node->children.push_back(std::move(name_node));
    
    // 检查是否有初始化
    if (match(Lexer::EQUALS)) {
        auto expr = parseExpression();
        if (expr) {
            var_node->children.push_back(std::move(expr));
        }
    }
    
    // 可选的分号
    match(Lexer::SEMICOLON);
    
    return var_node;
}

std::unique_ptr<CHTLJSCompiler::ASTNode> CHTLJSCompiler::Parser::parseExpressionStatement() {
    auto stmt = std::make_unique<ASTNode>(ASTNodeType::EXPRESSION_STATEMENT);
    
    // 检查是否是空语句（只有分号）
    if (lexer_.peekToken().type == Lexer::SEMICOLON) {
        lexer_.nextToken(); // 消费分号
        return stmt;
    }
    
    auto expr = parseExpression();
    if (expr) {
        stmt->children.push_back(std::move(expr));
    }
    
    // 可选的分号
    match(Lexer::SEMICOLON);
    
    return stmt;
}

std::unique_ptr<CHTLJSCompiler::ASTNode> CHTLJSCompiler::Parser::parseExpression() {
    return parsePostfixExpression();
}

std::unique_ptr<CHTLJSCompiler::ASTNode> CHTLJSCompiler::Parser::parsePrimaryExpression() {
    Lexer::Token token = lexer_.peekToken();
    
    // 增强选择器
    if (token.type == Lexer::SELECTOR_START) {
        return parseSelectorExpression();
    }
    
    // 标识符
    if (token.type == Lexer::IDENTIFIER) {
        lexer_.nextToken();
        auto node = std::make_unique<ASTNode>(ASTNodeType::IDENTIFIER);
        node->value = token.value;
        return node;
    }
    
    // 字符串
    if (token.type == Lexer::STRING) {
        lexer_.nextToken();
        auto node = std::make_unique<ASTNode>(ASTNodeType::STRING);
        node->value = token.value;
        return node;
    }
    
    // 数字
    if (token.type == Lexer::NUMBER) {
        lexer_.nextToken();
        auto node = std::make_unique<ASTNode>(ASTNodeType::NUMBER);
        node->value = token.value;
        return node;
    }
    
    // 对象字面量
    if (token.type == Lexer::LBRACE) {
        return parseObjectLiteral();
    }
    
    // 函数表达式
    if (token.type == Lexer::FUNCTION || token.value == "function") {
        return parseFunctionExpression();
    }
    
    // 括号表达式或箭头函数
    if (token.type == Lexer::LPAREN) {
        lexer_.nextToken(); // (
        
        // 检查是否是箭头函数 () =>
        if (lexer_.peekToken().type == Lexer::RPAREN) {
            Lexer::Token next = lexer_.nextToken(); // )
            if (lexer_.peekToken().type == Lexer::ARROW) {
                lexer_.nextToken(); // =>
                return parseArrowFunction();
            } else {
                // 空括号表达式
                return std::make_unique<ASTNode>(ASTNodeType::IDENTIFIER);
            }
        }
        
        auto expr = parseExpression();
        expect(Lexer::RPAREN); // )
        return expr;
    }
    
    reportError("Unexpected token in expression");
    lexer_.nextToken(); // 跳过错误token
    return nullptr;
}

std::unique_ptr<CHTLJSCompiler::ASTNode> CHTLJSCompiler::Parser::parsePostfixExpression() {
    auto expr = parsePrimaryExpression();
    
    while (expr) {
        Lexer::Token token = lexer_.peekToken();
        
        if (token.type == Lexer::DOT || token.type == Lexer::ARROW) {
            expr = parseMemberExpression(std::move(expr));
        } else if (token.type == Lexer::LPAREN) {
            expr = parseCallExpression(std::move(expr));
        } else if (token.type == Lexer::LBRACKET) {
            // 数组访问
            lexer_.nextToken(); // [
            auto index = parseExpression();
            expect(Lexer::RBRACKET); // ]
            
            auto access = std::make_unique<ASTNode>(ASTNodeType::MEMBER_ACCESS);
            access->children.push_back(std::move(expr));
            access->children.push_back(std::move(index));
            expr = std::move(access);
        } else {
            break;
        }
    }
    
    return expr;
}

std::unique_ptr<CHTLJSCompiler::ASTNode> CHTLJSCompiler::Parser::parseMemberExpression(
    std::unique_ptr<ASTNode> object) {
    
    auto member = std::make_unique<ASTNode>(ASTNodeType::MEMBER_ACCESS);
    member->children.push_back(std::move(object));
    
    Lexer::Token op = lexer_.nextToken(); // . 或 ->
    member->value = op.type == Lexer::ARROW ? "->" : ".";
    
    Lexer::Token prop = lexer_.nextToken();
    if (prop.type != Lexer::IDENTIFIER) {
        reportError("Expected property name after member operator");
        return member;
    }
    
    auto prop_node = std::make_unique<ASTNode>(ASTNodeType::IDENTIFIER);
    prop_node->value = prop.value;
    member->children.push_back(std::move(prop_node));
    
    return member;
}

std::unique_ptr<CHTLJSCompiler::ASTNode> CHTLJSCompiler::Parser::parseCallExpression(
    std::unique_ptr<ASTNode> callee) {
    
    auto call = std::make_unique<ASTNode>(ASTNodeType::FUNCTION_CALL);
    call->children.push_back(std::move(callee));
    
    expect(Lexer::LPAREN); // (
    
    // 解析参数
    while (lexer_.peekToken().type != Lexer::RPAREN && !lexer_.isAtEnd()) {
        auto arg = parseExpression();
        if (arg) {
            call->children.push_back(std::move(arg));
        }
        
        if (lexer_.peekToken().type == Lexer::COMMA) {
            lexer_.nextToken(); // ,
        } else {
            break;
        }
    }
    
    expect(Lexer::RPAREN); // )
    
    return call;
}

std::unique_ptr<CHTLJSCompiler::ASTNode> CHTLJSCompiler::Parser::parseSelectorExpression() {
    auto selector = std::make_unique<ASTNode>(ASTNodeType::SELECTOR_EXPR);
    
    Lexer::Token token = lexer_.nextToken();
    std::string selector_text = token.value;
    
    // 去除首尾空格
    selector_text.erase(0, selector_text.find_first_not_of(" \t\n\r"));
    selector_text.erase(selector_text.find_last_not_of(" \t\n\r") + 1);
    
    selector->value = selector_text;
    
    // 判断选择器类型
    if (!selector_text.empty()) {
        if (selector_text[0] == '.') {
            selector->selector_type = "class";
        } else if (selector_text[0] == '#') {
            selector->selector_type = "id";
        } else if (selector_text.find(' ') != std::string::npos || 
                   selector_text.find('[') != std::string::npos) {
            selector->selector_type = "complex";
        } else {
            selector->selector_type = "tag";
        }
    } else {
        selector->selector_type = "tag";
    }
    
    return selector;
}

std::unique_ptr<CHTLJSCompiler::ASTNode> CHTLJSCompiler::Parser::parseObjectLiteral() {
    auto obj = std::make_unique<ASTNode>(ASTNodeType::OBJECT_LITERAL);
    
    expect(Lexer::LBRACE); // {
    
    while (lexer_.peekToken().type != Lexer::RBRACE && !lexer_.isAtEnd()) {
        // 属性名
        Lexer::Token key = lexer_.nextToken();
        if (key.type != Lexer::IDENTIFIER && key.type != Lexer::STRING) {
            reportError("Expected property name");
            synchronize();
            continue;
        }
        
        expect(Lexer::COLON); // :
        
        // 属性值
        auto value = parseExpression();
        
        // 创建属性节点
        auto prop = std::make_unique<ASTNode>(ASTNodeType::IDENTIFIER);
        prop->value = key.value;
        if (value) {
            prop->children.push_back(std::move(value));
        }
        obj->children.push_back(std::move(prop));
        
        if (lexer_.peekToken().type == Lexer::COMMA) {
            lexer_.nextToken(); // ,
        } else {
            break;
        }
    }
    
    expect(Lexer::RBRACE); // }
    
    return obj;
}

std::unique_ptr<CHTLJSCompiler::ASTNode> CHTLJSCompiler::Parser::parseFunctionExpression() {
    auto func = std::make_unique<ASTNode>(ASTNodeType::FUNCTION_EXPR);
    
    lexer_.nextToken(); // function
    
    // 可选的函数名
    if (lexer_.peekToken().type == Lexer::IDENTIFIER) {
        func->value = lexer_.nextToken().value;
    }
    
    expect(Lexer::LPAREN); // (
    
    // 参数列表（暂时简化处理）
    while (lexer_.peekToken().type != Lexer::RPAREN && !lexer_.isAtEnd()) {
        if (lexer_.peekToken().type == Lexer::IDENTIFIER) {
            lexer_.nextToken();
        }
        if (lexer_.peekToken().type == Lexer::COMMA) {
            lexer_.nextToken();
        }
    }
    
    expect(Lexer::RPAREN); // )
    expect(Lexer::LBRACE); // {
    
    // 函数体（暂时简化处理，作为原始文本）
    int brace_depth = 1;
    std::string body;
    
    while (!lexer_.isAtEnd() && brace_depth > 0) {
        Lexer::Token token = lexer_.nextToken();
        if (token.type == Lexer::LBRACE) {
            brace_depth++;
        } else if (token.type == Lexer::RBRACE) {
            brace_depth--;
            if (brace_depth == 0) break;
        }
        body += token.value + " ";
    }
    
    auto body_node = std::make_unique<ASTNode>(ASTNodeType::BLOCK);
    body_node->value = body;
    func->children.push_back(std::move(body_node));
    
    return func;
}

std::unique_ptr<CHTLJSCompiler::ASTNode> CHTLJSCompiler::Parser::parseArrowFunction() {
    auto func = std::make_unique<ASTNode>(ASTNodeType::FUNCTION_EXPR);
    
    // 箭头函数体
    if (lexer_.peekToken().type == Lexer::LBRACE) {
        lexer_.nextToken(); // {
        
        int brace_depth = 1;
        std::string body;
        
        while (!lexer_.isAtEnd() && brace_depth > 0) {
            Lexer::Token token = lexer_.nextToken();
            if (token.type == Lexer::LBRACE) {
                brace_depth++;
            } else if (token.type == Lexer::RBRACE) {
                brace_depth--;
                if (brace_depth == 0) break;
            }
            body += token.value + " ";
        }
        
        auto body_node = std::make_unique<ASTNode>(ASTNodeType::BLOCK);
        body_node->value = body;
        func->children.push_back(std::move(body_node));
    } else {
        // 单表达式箭头函数
        auto expr = parseExpression();
        if (expr) {
            auto body_node = std::make_unique<ASTNode>(ASTNodeType::BLOCK);
            body_node->value = "return ";
            body_node->children.push_back(std::move(expr));
            func->children.push_back(std::move(body_node));
        }
    }
    
    return func;
}

bool CHTLJSCompiler::Parser::match(Lexer::TokenType type) {
    if (lexer_.peekToken().type == type) {
        lexer_.nextToken();
        return true;
    }
    return false;
}

bool CHTLJSCompiler::Parser::expect(Lexer::TokenType type) {
    if (match(type)) {
        return true;
    }
    reportError("Unexpected token");
    return false;
}

void CHTLJSCompiler::Parser::synchronize() {
    while (!lexer_.isAtEnd()) {
        if (lexer_.peekToken().type == Lexer::SEMICOLON) {
            lexer_.nextToken();
            return;
        }
        
        Lexer::Token token = lexer_.peekToken();
        if (token.type == Lexer::VIR || token.type == Lexer::CONST ||
            token.type == Lexer::LET || token.type == Lexer::VAR ||
            token.type == Lexer::FUNCTION || token.type == Lexer::IF ||
            token.type == Lexer::FOR || token.type == Lexer::WHILE) {
            return;
        }
        
        lexer_.nextToken();
    }
}

void CHTLJSCompiler::Parser::reportError(const std::string& message) {
    CompilerError error;
    error.severity = CompilerError::ERROR;
    error.message = message;
    error.location.line = lexer_.peekToken().line;
    error.location.column = lexer_.peekToken().column;
    context_.errors.push_back(error);
}

// ==================== CodeGenerator 实现 ====================

CHTLJSCompiler::CodeGenerator::CodeGenerator(CompilationContext& context, 
                                           const CompilerOptions& options)
    : context_(context), options_(options), indent_level_(0) {
}

std::string CHTLJSCompiler::CodeGenerator::generate(const std::unique_ptr<ASTNode>& ast) {
    output_.clear();
    
    // 生成主代码
    generateNode(ast);
    
    // 生成辅助函数
    if (!context_.helper_functions.empty()) {
        emitLine("\n// CHTL JS Helper Functions");
        for (const auto& helper : context_.helper_functions) {
            emitLine(helper);
        }
    }
    
    return output_;
}

void CHTLJSCompiler::CodeGenerator::generateNode(const std::unique_ptr<ASTNode>& node) {
    if (!node) return;
    
    switch (node->type) {
        case ASTNodeType::PROGRAM:
            for (const auto& child : node->children) {
                generateNode(child);
            }
            break;
            
        case ASTNodeType::VIR_DECLARATION:
            generateVirDeclaration(node);
            break;
            
        case ASTNodeType::EXPRESSION_STATEMENT:
            if (!node->children.empty()) {
                generateNode(node->children[0]);
                emit(";");
                emitLine("");
            }
            break;
            
        case ASTNodeType::VARIABLE_DECLARATION:
            emit(node->value + " "); // var/let/const
            if (!node->children.empty()) {
                emit(node->children[0]->value); // 变量名
                if (node->children.size() > 1) {
                    emit(" = ");
                    generateNode(node->children[1]); // 初始值
                }
            }
            emit(";");
            emitLine("");
            break;
            
        case ASTNodeType::SELECTOR_EXPR:
            generateSelectorExpression(node);
            break;
            
        case ASTNodeType::MEMBER_ACCESS:
            generateMemberAccess(node);
            break;
            
        case ASTNodeType::FUNCTION_CALL:
            generateFunctionCall(node);
            break;
            
        case ASTNodeType::IDENTIFIER:
            emit(node->value);
            break;
            
        case ASTNodeType::STRING:
            emit(node->value);
            break;
            
        case ASTNodeType::NUMBER:
            emit(node->value);
            break;
            
        case ASTNodeType::OBJECT_LITERAL:
            emit("{");
            for (size_t i = 0; i < node->children.size(); ++i) {
                if (i > 0) emit(", ");
                const auto& prop = node->children[i];
                emit(prop->value + ": ");
                if (!prop->children.empty()) {
                    generateNode(prop->children[0]);
                }
            }
            emit("}");
            break;
            
        case ASTNodeType::FUNCTION_EXPR:
            emit("function");
            if (!node->value.empty()) {
                emit(" " + node->value);
            }
            emit("() {");
            if (!node->children.empty()) {
                emit(node->children[0]->value);
            }
            emit("}");
            break;
            
        default:
            emit("/* TODO: " + std::to_string(static_cast<int>(node->type)) + " */");
            break;
    }
}

void CHTLJSCompiler::CodeGenerator::generateSelectorExpression(const std::unique_ptr<ASTNode>& node) {
    std::string selector = node->value;
    std::string type = node->selector_type.value_or("tag");
    
    emit(convertSelectorToJS(selector, type));
}

std::string CHTLJSCompiler::CodeGenerator::convertSelectorToJS(const std::string& selector, 
                                                               const std::string& type) {
    // 处理数组索引
    std::regex index_regex(R"(\[(\d+)\]$)");
    std::smatch match;
    std::string base_selector = selector;
    std::string index_part;
    
    if (std::regex_search(selector, match, index_regex)) {
        base_selector = selector.substr(0, match.position());
        index_part = "[" + match[1].str() + "]";
    }
    
    std::string js_code;
    
    if (type == "class") {
        js_code = "document.querySelectorAll('" + base_selector + "')";
    } else if (type == "id") {
        js_code = "document.querySelector('" + base_selector + "')";
        if (!index_part.empty()) {
            // ID选择器不应该有索引
            index_part.clear();
        }
    } else if (type == "complex") {
        js_code = "document.querySelectorAll('" + base_selector + "')";
    } else { // tag
        // 特殊处理：如果只是标签名，先尝试当前元素的类名
        if (!context_.current_element_class.empty()) {
            js_code = "(document.querySelector('." + context_.current_element_class + 
                     "') || document.querySelector('#" + base_selector + 
                     "') || document.querySelector('." + base_selector + 
                     "') || document.querySelectorAll('" + base_selector + "'))";
        } else {
            js_code = "(document.querySelector('#" + base_selector + 
                     "') || document.querySelector('." + base_selector + 
                     "') || document.querySelectorAll('" + base_selector + "'))";
        }
    }
    
    if (!index_part.empty()) {
        js_code += index_part;
    }
    
    return js_code;
}

void CHTLJSCompiler::CodeGenerator::generateVirDeclaration(const std::unique_ptr<ASTNode>& node) {
    std::string vir_name = node->value;
    
    // 创建虚对象
    VirObject vir_obj;
    vir_obj.name = vir_name;
    
    // 分析右侧的表达式，提取方法
    if (!node->children.empty()) {
        const auto& expr = node->children[0];
        
        // 如果是函数调用
        if (expr->type == ASTNodeType::FUNCTION_CALL) {
            const auto& callee = expr->children[0];
            
            // 如果是listen调用
            if (callee->type == ASTNodeType::IDENTIFIER && callee->value == "listen") {
                generateListenCall(expr);
                
                // 提取listen对象中的方法
                if (expr->children.size() > 1 && 
                    expr->children[1]->type == ASTNodeType::OBJECT_LITERAL) {
                    
                    for (const auto& prop : expr->children[1]->children) {
                        std::string method_name = prop->value;
                        std::string func_name = "_chtljs_" + vir_name + "_" + method_name;
                        vir_obj.methods[method_name] = func_name;
                        
                        // 生成全局函数
                        std::string func_code = "function " + func_name + "() { ";
                        if (!prop->children.empty() && 
                            prop->children[0]->type == ASTNodeType::FUNCTION_EXPR) {
                            func_code += "return (";
                            generateNode(prop->children[0]);
                            func_code += output_.substr(output_.find_last_of("function"));
                            output_.erase(output_.find_last_of("function"));
                            func_code += ")();";
                        }
                        func_code += " }";
                        context_.helper_functions.push_back(func_code);
                    }
                }
                
                emitLine("");
            } else {
                // 其他情况，正常生成
                generateNode(expr);
                emitLine(";");
            }
        } else {
            // 其他表达式
            generateNode(expr);
            emitLine(";");
        }
    }
    
    // 注册虚对象
    context_.vir_objects[vir_name] = vir_obj;
}

void CHTLJSCompiler::CodeGenerator::generateMemberAccess(const std::unique_ptr<ASTNode>& node) {
    if (node->children.size() < 2) return;
    
    const auto& object = node->children[0];
    const auto& property = node->children[1];
    
    // 检查是否是虚对象的方法访问
    if (object->type == ASTNodeType::IDENTIFIER) {
        auto it = context_.vir_objects.find(object->value);
        if (it != context_.vir_objects.end() && property->type == ASTNodeType::IDENTIFIER) {
            auto method_it = it->second.methods.find(property->value);
            if (method_it != it->second.methods.end()) {
                // 替换为全局函数名
                emit(method_it->second);
                return;
            }
        }
    }
    
    // 正常的成员访问
    generateNode(object);
    emit(node->value == "->" ? "." : node->value); // -> 转换为 .
    generateNode(property);
}

void CHTLJSCompiler::CodeGenerator::generateFunctionCall(const std::unique_ptr<ASTNode>& node) {
    if (node->children.empty()) return;
    
    const auto& callee = node->children[0];
    
    // 检查是否是CHTL JS特殊函数
    if (callee->type == ASTNodeType::IDENTIFIER) {
        if (callee->value == "listen") {
            generateListenCall(node);
            return;
        } else if (callee->value == "delegate") {
            generateDelegateCall(node);
            return;
        } else if (callee->value == "animate") {
            generateAnimateCall(node);
            return;
        }
    }
    
    // 普通函数调用
    generateNode(callee);
    emit("(");
    for (size_t i = 1; i < node->children.size(); ++i) {
        if (i > 1) emit(", ");
        generateNode(node->children[i]);
    }
    emit(")");
}

void CHTLJSCompiler::CodeGenerator::generateListenCall(const std::unique_ptr<ASTNode>& node) {
    // 生成辅助函数（如果还没有生成）
    static bool helper_generated = false;
    if (!helper_generated) {
        context_.helper_functions.push_back(generateListenHelper());
        helper_generated = true;
    }
    
    emit("_chtljs_listen");
    emit("(");
    for (size_t i = 1; i < node->children.size(); ++i) {
        if (i > 1) emit(", ");
        generateNode(node->children[i]);
    }
    emit(")");
}

void CHTLJSCompiler::CodeGenerator::generateDelegateCall(const std::unique_ptr<ASTNode>& node) {
    // 生成辅助函数（如果还没有生成）
    static bool helper_generated = false;
    if (!helper_generated) {
        context_.helper_functions.push_back(generateDelegateHelper());
        helper_generated = true;
    }
    
    emit("_chtljs_delegate");
    emit("(");
    for (size_t i = 1; i < node->children.size(); ++i) {
        if (i > 1) emit(", ");
        generateNode(node->children[i]);
    }
    emit(")");
}

void CHTLJSCompiler::CodeGenerator::generateAnimateCall(const std::unique_ptr<ASTNode>& node) {
    // 生成辅助函数（如果还没有生成）
    static bool helper_generated = false;
    if (!helper_generated) {
        context_.helper_functions.push_back(generateAnimateHelper());
        helper_generated = true;
    }
    
    emit("_chtljs_animate");
    emit("(");
    for (size_t i = 1; i < node->children.size(); ++i) {
        if (i > 1) emit(", ");
        generateNode(node->children[i]);
    }
    emit(")");
}

std::string CHTLJSCompiler::CodeGenerator::generateListenHelper() {
    return R"(
function _chtljs_listen(eventMap) {
    var element = this;
    if (!element || !element.addEventListener) {
        console.error('listen() called on non-element');
        return;
    }
    
    for (var event in eventMap) {
        if (eventMap.hasOwnProperty(event)) {
            element.addEventListener(event, eventMap[event]);
        }
    }
    return element;
})";
}

std::string CHTLJSCompiler::CodeGenerator::generateDelegateHelper() {
    return R"(
var _chtljs_delegateRegistry = {};

function _chtljs_delegate(options) {
    var parent = this;
    if (!parent || !parent.addEventListener) {
        console.error('delegate() called on non-element');
        return;
    }
    
    var targets = options.target;
    if (!Array.isArray(targets)) {
        targets = [targets];
    }
    
    var parentKey = parent.tagName + '#' + parent.id + '.' + parent.className;
    
    if (!_chtljs_delegateRegistry[parentKey]) {
        _chtljs_delegateRegistry[parentKey] = {};
        
        parent.addEventListener('click', function(e) {
            _chtljs_handleDelegatedEvent(e, 'click', parent);
        }, true);
        
        parent.addEventListener('mouseenter', function(e) {
            _chtljs_handleDelegatedEvent(e, 'mouseenter', parent);
        }, true);
        
        parent.addEventListener('mouseleave', function(e) {
            _chtljs_handleDelegatedEvent(e, 'mouseleave', parent);
        }, true);
    }
    
    targets.forEach(function(target) {
        for (var event in options) {
            if (event !== 'target' && options.hasOwnProperty(event)) {
                if (!_chtljs_delegateRegistry[parentKey][event]) {
                    _chtljs_delegateRegistry[parentKey][event] = {};
                }
                _chtljs_delegateRegistry[parentKey][event][target] = options[event];
            }
        }
    });
}

function _chtljs_handleDelegatedEvent(e, eventType, parent) {
    var parentKey = parent.tagName + '#' + parent.id + '.' + parent.className;
    var handlers = _chtljs_delegateRegistry[parentKey][eventType];
    
    if (!handlers) return;
    
    for (var selector in handlers) {
        var elements = parent.querySelectorAll(selector);
        for (var i = 0; i < elements.length; i++) {
            if (elements[i] === e.target || elements[i].contains(e.target)) {
                handlers[selector].call(elements[i], e);
                break;
            }
        }
    }
})";
}

std::string CHTLJSCompiler::CodeGenerator::generateAnimateHelper() {
    return R"(
function _chtljs_animate(options) {
    var targets = options.target;
    if (!Array.isArray(targets)) {
        targets = [targets];
    }
    
    var duration = options.duration || 1000;
    var easing = options.easing || 'linear';
    var begin = options.begin || {};
    var end = options.end || {};
    var when = options.when || [];
    var loop = options.loop || 1;
    var direction = options.direction || 'normal';
    var delay = options.delay || 0;
    var callback = options.callback;
    
    var startTime = null;
    var currentLoop = 0;
    
    function animate(timestamp) {
        if (!startTime) startTime = timestamp + delay;
        if (timestamp < startTime) {
            requestAnimationFrame(animate);
            return;
        }
        
        var progress = (timestamp - startTime) / duration;
        if (progress > 1) progress = 1;
        
        var easedProgress = applyEasing(progress, easing);
        
        targets.forEach(function(target) {
            // Apply begin styles
            for (var prop in begin) {
                if (begin.hasOwnProperty(prop)) {
                    target.style[prop] = begin[prop];
                }
            }
            
            // Apply when keyframes
            when.forEach(function(keyframe) {
                if (easedProgress >= keyframe.at) {
                    for (var prop in keyframe) {
                        if (prop !== 'at' && keyframe.hasOwnProperty(prop)) {
                            target.style[prop] = keyframe[prop];
                        }
                    }
                }
            });
            
            // Apply end styles
            if (progress === 1) {
                for (var prop in end) {
                    if (end.hasOwnProperty(prop)) {
                        target.style[prop] = end[prop];
                    }
                }
            }
        });
        
        if (progress < 1) {
            requestAnimationFrame(animate);
        } else {
            currentLoop++;
            if (loop === -1 || currentLoop < loop) {
                startTime = null;
                requestAnimationFrame(animate);
            } else if (callback) {
                callback();
            }
        }
    }
    
    function applyEasing(t, type) {
        switch (type) {
            case 'ease-in': return t * t;
            case 'ease-out': return t * (2 - t);
            case 'ease-in-out': return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t;
            default: return t;
        }
    }
    
    requestAnimationFrame(animate);
    
    return {
        stop: function() { /* TODO */ },
        pause: function() { /* TODO */ },
        resume: function() { /* TODO */ }
    };
})";
}

void CHTLJSCompiler::CodeGenerator::emit(const std::string& code) {
    output_ += code;
}

void CHTLJSCompiler::CodeGenerator::emitLine(const std::string& code) {
    for (size_t i = 0; i < indent_level_; ++i) {
        output_ += "  ";
    }
    output_ += code + "\n";
}

void CHTLJSCompiler::CodeGenerator::indent() {
    indent_level_++;
}

void CHTLJSCompiler::CodeGenerator::dedent() {
    if (indent_level_ > 0) {
        indent_level_--;
    }
}

std::string CHTLJSCompiler::CodeGenerator::generateUniqueId() {
    return "_chtljs_" + std::to_string(context_.unique_id++);
}

// ==================== CHTLJSCompiler 主类实现 ====================

CHTLJSCompiler::CHTLJSCompiler() = default;
CHTLJSCompiler::~CHTLJSCompiler() = default;

CompilationResult CHTLJSCompiler::compile(const CodeFragment& fragment, 
                                         const CompilerOptions& options) {
    CompilationResult result;
    result.success = true;
    
    try {
        CompilationContext context;
        
        // 调试：输出所有token
        if (options.debug_mode) {
            Lexer lexer(fragment.content);
            std::cout << "=== CHTL JS Lexer Tokens ===\n";
            while (!lexer.isAtEnd()) {
                Lexer::Token token = lexer.nextToken();
                std::cout << "Token: type=" << static_cast<int>(token.type) 
                         << ", value='" << token.value << "'\n";
                if (token.type == Lexer::EOF_TOKEN) break;
            }
            std::cout << "=== End Tokens ===\n";
        }
        
        // 解析输入
        auto ast = parseInput(fragment.content, context);
        
        if (!context.errors.empty()) {
            result.success = false;
            for (const auto& error : context.errors) {
                result.errors.push_back(error.message);
            }
            return result;
        }
        
        // 生成输出
        result.output = generateOutput(ast, context, options);
        
        // 添加到全局脚本
        result.global_scripts = "<script>\n" + result.output + "</script>\n";
        
        // 添加警告
        for (const auto& warning : context.warnings) {
            result.warnings.push_back(warning.message);
        }
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back(std::string("Compilation failed: ") + e.what());
    }
    
    return result;
}

std::unique_ptr<CHTLJSCompiler::ASTNode> CHTLJSCompiler::parseInput(
    const std::string& input, CompilationContext& context) {
    Lexer lexer(input);
    Parser parser(lexer, context);
    return parser.parse();
}

std::string CHTLJSCompiler::generateOutput(const std::unique_ptr<ASTNode>& ast, 
                                          CompilationContext& context,
                                          const CompilerOptions& options) {
    CodeGenerator generator(context, options);
    return generator.generate(ast);
}

} // namespace chtl
#include "compiler/chtl/chtl_compiler.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <unordered_set>

namespace chtl {

// 使用新的Token系统定义的关键字和HTML标签

// ==================== Lexer 实现 ====================

CHTLCompiler::Lexer::Lexer(const std::string& input, size_t start_line)
    : input_(input), position_(0), line_(start_line), column_(1) {
}

Token CHTLCompiler::Lexer::nextToken() {
    if (peeked_token_.has_value()) {
        Token token = peeked_token_.value();
        peeked_token_.reset();
        return token;
    }
    
    skipWhitespace();
    
    if (isAtEnd()) {
        return Token{TokenType::EOF_TOKEN, "", line_, column_, position_, position_};
    }
    
    size_t start_line = line_;
    size_t start_column = column_;
    
    char c = peek();
    
    // 单字符标记
    size_t start_pos = position_;
    switch (c) {
        case '{': advance(); return Token{TokenType::LBRACE, "{", start_line, start_column, start_pos, position_};
        case '}': advance(); return Token{TokenType::RBRACE, "}", start_line, start_column, start_pos, position_};
        case '(': advance(); return Token{TokenType::LPAREN, "(", start_line, start_column, start_pos, position_};
        case ')': advance(); return Token{TokenType::RPAREN, ")", start_line, start_column, start_pos, position_};
        case '[': advance(); return Token{TokenType::LBRACKET, "[", start_line, start_column, start_pos, position_};
        case ']': advance(); return Token{TokenType::RBRACKET, "]", start_line, start_column, start_pos, position_};
        case ':': advance(); return Token{TokenType::COLON, ":", start_line, start_column, start_pos, position_};
        case ';': advance(); return Token{TokenType::SEMICOLON, ";", start_line, start_column, start_pos, position_};
        case ',': advance(); return Token{TokenType::COMMA, ",", start_line, start_column, start_pos, position_};
        case '.': advance(); return Token{TokenType::DOT, ".", start_line, start_column, start_pos, position_};
        case '@': advance(); return Token{TokenType::AT, "@", start_line, start_column, start_pos, position_};
        case '&': advance(); return Token{TokenType::AMPERSAND, "&", start_line, start_column, start_pos, position_};
        case '=': advance(); return Token{TokenType::EQUALS, "=", start_line, start_column, start_pos, position_};

    }
    
    // 多字符标记
    if (c == '-' && peek(1) == '>') {
        advance();
        advance();
        return Token{TokenType::ARROW, "->", start_line, start_column, start_pos, position_};
    }
    
    // 注释
    if (c == '/' && peek(1) == '/') {
        skipComment();
        return nextToken();
    }
    
    if (c == '/' && peek(1) == '*') {
        skipComment();
        return nextToken();
    }
    
    // 字符串
    if (c == '"' || c == '\'') {
        return scanString();
    }
    
    // 数字
    if (std::isdigit(c)) {
        return scanNumber();
    }
    
    // 标识符或关键字
    if (std::isalpha(c) || c == '_') {
        return scanIdentifier();
    }
    
    // 未知字符
    advance();
    return Token{TokenType::IDENTIFIER, std::string(1, c), start_line, start_column, start_pos, position_};
}

Token CHTLCompiler::Lexer::peekToken() {
    if (!peeked_token_.has_value()) {
        peeked_token_ = nextToken();
    }
    return peeked_token_.value();
}

void CHTLCompiler::Lexer::consumeToken() {
    nextToken();
}

void CHTLCompiler::Lexer::skipWhitespace() {
    while (!isAtEnd() && std::isspace(peek())) {
        if (peek() == '\n') {
            line_++;
            column_ = 1;
        } else {
            column_++;
        }
        position_++;
    }
}

void CHTLCompiler::Lexer::skipComment() {
    if (peek() == '/' && peek(1) == '/') {
        // 单行注释
        position_ += 2;
        column_ += 2;
        while (!isAtEnd() && peek() != '\n') {
            advance();
        }
    } else if (peek() == '/' && peek(1) == '*') {
        // 多行注释
        position_ += 2;
        column_ += 2;
        while (!isAtEnd()) {
            if (peek() == '*' && peek(1) == '/') {
                position_ += 2;
                column_ += 2;
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
    }
}

Token CHTLCompiler::Lexer::scanIdentifier() {
    size_t start_line = line_;
    size_t start_column = column_;
    size_t start_pos = position_;
    std::string value;
    
    // 首字符必须是字母或下划线
    if (!std::isalpha(peek()) && peek() != '_') {
        return Token{TokenType::IDENTIFIER, "", start_line, start_column, start_pos, position_};
    }
    
    while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_' || peek() == '-')) {
        value += peek();
        advance();
    }
    
    // 检查是否是关键字
    if (getCHTLKeywords().find(value) != getCHTLKeywords().end()) {
        return Token{TokenType::KEYWORD, value, start_line, start_column, start_pos, position_};
    }
    
    return Token{TokenType::IDENTIFIER, value, start_line, start_column, start_pos, position_};
}

Token CHTLCompiler::Lexer::scanString() {
    size_t start_line = line_;
    size_t start_column = column_;
    size_t start_pos = position_;
    char delimiter = peek();
    std::string value;
    value += delimiter;
    advance(); // 跳过开始引号
    
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
    
    return Token{TokenType::STRING_LITERAL, value, start_line, start_column, start_pos, position_};
}

Token CHTLCompiler::Lexer::scanNumber() {
    size_t start_line = line_;
    size_t start_column = column_;
    size_t start_pos = position_;
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
    
    return Token{TokenType::NUMBER, value, start_line, start_column, start_pos, position_};
}

char CHTLCompiler::Lexer::peek(size_t offset) const {
    size_t pos = position_ + offset;
    if (pos >= input_.length()) {
        return '\0';
    }
    return input_[pos];
}

void CHTLCompiler::Lexer::advance() {
    if (!isAtEnd()) {
        column_++;
        position_++;
    }
}

// ==================== Parser 实现 ====================

CHTLCompiler::Parser::Parser(Lexer& lexer, CompilationContext& context)
    : lexer_(lexer), context_(context) {
}

std::unique_ptr<ASTNode> CHTLCompiler::Parser::parse() {
    return parseRoot();
}

std::unique_ptr<ASTNode> CHTLCompiler::Parser::parseRoot() {
    auto root = std::make_unique<ASTNode>(ASTNodeType::ROOT);
    
    while (!lexer_.isAtEnd()) {
        Token token = lexer_.peekToken();
        
        if (token.type == TokenType::EOF_TOKEN) {
            break;
        }
        
        // 处理顶级元素
        if (token.type == TokenType::LBRACKET) {
            lexer_.consumeToken();
            Token next = lexer_.peekToken();
            
            if (next.type == TokenType::IDENTIFIER) {
                if (next.value == "Template") {
                    root->children.push_back(parseTemplate());
                } else if (next.value == "Custom") {
                    root->children.push_back(parseCustom());
                } else if (next.value == "Import") {
                    root->children.push_back(parseImport());
                } else if (next.value == "Origin") {
                    root->children.push_back(parseOrigin());
                } else {
                    reportError("Unknown directive: " + next.value);
                    synchronize();
                }
            }
        } else if (token.type == TokenType::IDENTIFIER && getHTMLTags().find(token.value) != getHTMLTags().end()) {
            root->children.push_back(parseElement());
        } else if (token.type == TokenType::KEYWORD && token.value == "style") {
            // 全局style块
            root->children.push_back(parseStyle());
        } else {
            reportError("Unexpected token at top level: " + token.value);
            lexer_.consumeToken();
        }
    }
    
    return root;
}

std::unique_ptr<ASTNode> CHTLCompiler::Parser::parseElement() {
    auto element = std::make_unique<ElementNode>();
    
    Token tag = lexer_.nextToken();
    element->tag_name = tag.value;
    element->name = tag.value;
    element->location.start_line = tag.line;
    element->location.start_column = tag.column;
    
    // 期待 {
    if (!expect(TokenType::LBRACE)) {
        return element;
    }
    
    // 解析元素内容
    parseElementContent(element.get());
    
    return element;
}

void CHTLCompiler::Parser::parseElementContent(ElementNode* element) {
    while (!lexer_.isAtEnd()) {
        Token token = lexer_.peekToken();
        
        if (token.type == TokenType::RBRACE) {
            lexer_.consumeToken();
            break;
        }
        
        if (token.type == TokenType::IDENTIFIER) {
            // 查看下一个标记来决定是属性还是子元素
            std::string identifier = token.value;
            lexer_.consumeToken(); // 消费标识符
            
            Token peek = lexer_.peekToken();
            if (peek.type == TokenType::COLON || peek.type == TokenType::EQUALS) {
                // 是属性
                lexer_.consumeToken(); // 消费 : 或 =
                Token value = lexer_.nextToken();
                
                // 处理属性值
                std::string attr_value;
                if (value.type == TokenType::STRING_LITERAL && value.value.length() >= 2) {
                    // 去除字符串引号
                    attr_value = value.value.substr(1, value.value.length() - 2);
                } else {
                    // 无引号字面量（标识符、数字等）
                    attr_value = value.value;
                }
                
                element->attributes[identifier] = attr_value;
                
                // 可选的分号
                if (lexer_.peekToken().type == TokenType::SEMICOLON) {
                    lexer_.consumeToken();
                }
            } else if (peek.type == TokenType::LBRACE) {
                // 是子元素
                if (getHTMLTags().find(identifier) != getHTMLTags().end()) {
                    // 递归解析子元素
                    auto child = std::make_unique<ElementNode>();
                    child->tag_name = identifier;
                    child->name = identifier;
                    child->location.start_line = token.line;
                    child->location.start_column = token.column;
                    
                    lexer_.consumeToken(); // 消费 {
                    
                    // 解析子元素内容（递归调用相同逻辑）
                    parseElementContent(child.get());
                    
                    element->children.push_back(std::move(child));
                } else {
                    reportError("Unknown element: " + identifier);
                    // 跳过这个块
                    lexer_.consumeToken(); // 消费 {
                    int depth = 1;
                    while (depth > 0 && !lexer_.isAtEnd()) {
                        Token t = lexer_.nextToken();
                        if (t.type == TokenType::LBRACE) depth++;
                        else if (t.type == TokenType::RBRACE) depth--;
                    }
                }
            } else {
                // 可能是一个没有花括号的HTML标签名
                reportError("Unexpected token after identifier: " + identifier);
            }
        } else if (token.type == TokenType::KEYWORD) {
            if (token.value == "style") {
                element->has_local_style = true;
                element->children.push_back(parseStyle());
            } else if (token.value == "script") {
                element->has_local_script = true;
                element->children.push_back(parseScript());
            } else if (token.value == "text") {
                element->children.push_back(parseText());
            } else {
                reportError("Unexpected keyword in element: " + token.value);
                lexer_.consumeToken();
            }
        } else {
            reportError("Unexpected token in element");
            lexer_.consumeToken();
        }
    }
}

std::unique_ptr<ASTNode> CHTLCompiler::Parser::parseText() {
    auto text = std::make_unique<ASTNode>(ASTNodeType::TEXT);
    
    lexer_.consumeToken(); // 消费 'text'
    
    if (!expect(TokenType::LBRACE)) {
        return text;
    }
    
    // 收集文本内容直到 }
    std::string content;
    int brace_depth = 1;
    
    while (!lexer_.isAtEnd() && brace_depth > 0) {
        Token token = lexer_.nextToken();
        
        if (token.type == TokenType::LBRACE) {
            brace_depth++;
            content += token.value;
        } else if (token.type == TokenType::RBRACE) {
            brace_depth--;
            if (brace_depth > 0) {
                content += token.value;
            }
        } else {
            content += token.value;
            if (token.type != TokenType::STRING_LITERAL && !content.empty() && content.back() != ' ') {
                content += " ";
            }
        }
    }
    
    // 去除首尾空白
    size_t start = content.find_first_not_of(" \t\n\r");
    size_t end = content.find_last_not_of(" \t\n\r");
    if (start != std::string::npos && end != std::string::npos) {
        text->value = content.substr(start, end - start + 1);
    }
    
    return text;
}

std::unique_ptr<ASTNode> CHTLCompiler::Parser::parseStyle() {
    auto style = std::make_unique<ASTNode>(ASTNodeType::STYLE);
    
    lexer_.consumeToken(); // 消费 'style'
    
    if (!expect(TokenType::LBRACE)) {
        return style;
    }
    
    // 收集样式内容直到 }
    std::string content;
    int brace_depth = 1;
    
    while (!lexer_.isAtEnd() && brace_depth > 0) {
        Token token = lexer_.nextToken();
        
        if (token.type == TokenType::LBRACE) {
            brace_depth++;
        } else if (token.type == TokenType::RBRACE) {
            brace_depth--;
            if (brace_depth == 0) {
                break;
            }
        }
        
        // 根据token类型决定是否添加空格
        if (token.type == TokenType::SEMICOLON || token.type == TokenType::COLON || 
            token.type == TokenType::COMMA || token.type == TokenType::LBRACE || token.type == TokenType::RBRACE) {
            content += token.value;
        } else if (token.type == TokenType::STRING_LITERAL) {
            content += token.value;
        } else {
            // 其他token前面可能需要空格
            if (!content.empty() && content.back() != ' ' && content.back() != '{' && 
                content.back() != ';' && content.back() != ':') {
                content += " ";
            }
            content += token.value;
        }
    }
    
    style->value = content;
    return style;
}

std::unique_ptr<ASTNode> CHTLCompiler::Parser::parseScript() {
    auto script = std::make_unique<ASTNode>(ASTNodeType::SCRIPT);
    
    lexer_.consumeToken(); // 消费 'script'
    
    // script块会被扫描器分割成单独的CHTL JS片段
    // 所以这里我们不需要解析内容，只需要标记这是一个script节点
    return script;
}

bool CHTLCompiler::Parser::expect(TokenType type) {
    Token token = lexer_.peekToken();
    if (token.type != type) {
        reportError("Expected token type but found: " + token.value);
        return false;
    }
    lexer_.consumeToken();
    return true;
}

bool CHTLCompiler::Parser::match(TokenType type) {
    if (lexer_.peekToken().type == type) {
        lexer_.consumeToken();
        return true;
    }
    return false;
}

std::unique_ptr<ASTNode> CHTLCompiler::Parser::parseTemplate() {
    auto tmpl = std::make_unique<TemplateNode>();
    
    lexer_.consumeToken(); // 消费 'Template'
    expect(TokenType::RBRACKET); // ]
    
    // 读取模板类型 @Style, @Element, @Var
    expect(TokenType::AT);
    Token type_token = lexer_.nextToken();
    
    if (type_token.value == "Style") {
        tmpl->template_type = TemplateNode::STYLE;
    } else if (type_token.value == "Element") {
        tmpl->template_type = TemplateNode::ELEMENT;
    } else if (type_token.value == "Var") {
        tmpl->template_type = TemplateNode::VAR;
    } else {
        reportError("Invalid template type: " + type_token.value);
    }
    
    // 读取模板名称
    Token name_token = lexer_.nextToken();
    tmpl->template_name = name_token.value;
    
    // TODO: 实现模板内容解析
    expect(TokenType::LBRACE);
    // 暂时跳过内容
    int brace_depth = 1;
    while (brace_depth > 0 && !lexer_.isAtEnd()) {
        Token t = lexer_.nextToken();
        if (t.type == TokenType::LBRACE) brace_depth++;
        else if (t.type == TokenType::RBRACE) brace_depth--;
    }
    
    return tmpl;
}

std::unique_ptr<ASTNode> CHTLCompiler::Parser::parseCustom() {
    auto custom = std::make_unique<ASTNode>(ASTNodeType::CUSTOM_DEF);
    
    lexer_.consumeToken(); // 消费 'Custom'
    expect(TokenType::RBRACKET); // ]
    
    // TODO: 实现自定义内容解析
    return custom;
}

std::unique_ptr<ASTNode> CHTLCompiler::Parser::parseImport() {
    auto import = std::make_unique<ASTNode>(ASTNodeType::IMPORT);
    
    lexer_.consumeToken(); // 消费 'Import'
    expect(TokenType::RBRACKET); // ]
    
    // TODO: 实现导入解析
    return import;
}

std::unique_ptr<ASTNode> CHTLCompiler::Parser::parseOrigin() {
    auto origin = std::make_unique<ASTNode>(ASTNodeType::ORIGIN_BLOCK);
    
    lexer_.consumeToken(); // 消费 'Origin'
    expect(TokenType::RBRACKET); // ]
    
    // TODO: 实现原始块解析
    return origin;
}

void CHTLCompiler::Parser::synchronize() {
    // 错误恢复：跳到下一个顶级结构
    while (!lexer_.isAtEnd()) {
        Token t = lexer_.peekToken();
        if (t.type == TokenType::RBRACE || 
            t.type == TokenType::LBRACKET ||
            (t.type == TokenType::IDENTIFIER && getHTMLTags().find(t.value) != getHTMLTags().end())) {
            break;
        }
        lexer_.consumeToken();
    }
}

void CHTLCompiler::Parser::reportError(const std::string& message) {
    CompilerError error;
    error.severity = CompilerError::ERROR;
    error.message = message;
    error.line = lexer_.peekToken().line;
    error.column = lexer_.peekToken().column;
    context_.errors.push_back(error);
}

// ==================== CodeGenerator 实现 ====================

CHTLCompiler::CodeGenerator::CodeGenerator(CompilationContext& context, const CompilerOptions& options)
    : context_(context), options_(options), indent_level_(0) {
}

std::string CHTLCompiler::CodeGenerator::generate(const std::unique_ptr<ASTNode>& ast) {
    output_.clear();
    generateNode(ast);
    return output_;
}

void CHTLCompiler::CodeGenerator::generateNode(const std::unique_ptr<ASTNode>& node) {
    if (!node) return;
    
    switch (node->type) {
        case ASTNodeType::ROOT:
            for (const auto& child : node->children) {
                generateNode(child);
            }
            break;
            
        case ASTNodeType::ELEMENT:
            generateElement(static_cast<const ElementNode*>(node.get()));
            break;
            
        case ASTNodeType::TEXT:
            generateText(node.get());
            break;
            
        case ASTNodeType::STYLE:
            // 样式节点暂时跳过，在生成元素时处理
            break;
            
        case ASTNodeType::SCRIPT:
            // script节点会在元素生成时处理，这里跳过
            break;
            
        default:
            break;
    }
}

void CHTLCompiler::CodeGenerator::generateElement(const ElementNode* element) {
    // 生成开始标签（带缩进）
    std::string tag_line = "<" + element->tag_name;
    
    // 准备属性，包括可能的自动生成类名
    std::unordered_map<std::string, std::string> final_attrs = element->attributes;
    
    // 如果有局部样式，生成类名
    if (element->has_local_style) {
        std::string class_name = generateClassName();
        const_cast<ElementNode*>(element)->generated_class_name = class_name;
        
        // 合并到现有的class属性
        if (final_attrs.find("class") != final_attrs.end()) {
            final_attrs["class"] = class_name + " " + final_attrs["class"];
        } else {
            final_attrs["class"] = class_name;
        }
    }
    
    // 生成所有属性
    for (const auto& [key, value] : final_attrs) {
        tag_line += " " + key + "=\"" + escapeHtml(value) + "\"";
    }
    
    // 自闭合标签
    static const std::unordered_set<std::string> VOID_TAGS = {
        "br", "hr", "img", "input", "meta", "link"
    };
    
    if (VOID_TAGS.find(element->tag_name) != VOID_TAGS.end()) {
        emitLine(tag_line + " />");
        return;
    }
    
    tag_line += ">";
    emitLine(tag_line);
    indent();
    
    // 处理局部样式
    if (element->has_local_style && !element->generated_class_name.empty()) {
        for (const auto& child : element->children) {
            if (child->type == ASTNodeType::STYLE) {
                // 处理局部样式内容
                std::string style_content = child->value;
                
                // 简单处理：如果样式内容包含选择器（如.box），需要特殊处理
                // 否则作为内联样式的内容，用生成的类名包装
                if (style_content.find('.') != std::string::npos || 
                    style_content.find('#') != std::string::npos ||
                    style_content.find('&') != std::string::npos) {
                    // 包含选择器的样式，需要更复杂的处理
                    // 暂时简单处理
                    context_.global_styles.push_back(style_content);
                } else {
                    // 纯属性，作为生成类的样式
                    context_.global_styles.push_back("." + element->generated_class_name + " {\n" + style_content + "\n}");
                }
            }
        }
    }
    
    // 生成子元素（跳过style和script节点）
    for (const auto& child : element->children) {
        if (child->type != ASTNodeType::STYLE && child->type != ASTNodeType::SCRIPT) {
            generateNode(child);
        }
    }
    
    dedent();
    emitLine("</" + element->tag_name + ">");
}

void CHTLCompiler::CodeGenerator::generateText(const ASTNode* text) {
    if (!text->value.empty()) {
        emitLine(escapeHtml(text->value));
    }
}

void CHTLCompiler::CodeGenerator::generateAttributes(const std::unordered_map<std::string, std::string>& attrs) {
    for (const auto& [key, value] : attrs) {
        emit(" " + key + "=\"" + escapeHtml(value) + "\"");
    }
}

void CHTLCompiler::CodeGenerator::indent() {
    indent_level_++;
}

void CHTLCompiler::CodeGenerator::dedent() {
    if (indent_level_ > 0) {
        indent_level_--;
    }
}

void CHTLCompiler::CodeGenerator::emit(const std::string& code) {
    output_ += code;
}

void CHTLCompiler::CodeGenerator::emitLine(const std::string& code) {
    if (code.empty()) {
        output_ += "\n";
        return;
    }
    
    // 添加缩进
    for (size_t i = 0; i < indent_level_; ++i) {
        output_ += "  ";
    }
    output_ += code + "\n";
}

std::string CHTLCompiler::CodeGenerator::generateClassName() {
    std::stringstream ss;
    ss << "chtl_" << std::hex << context_.class_counter++;
    return ss.str();
}

std::string CHTLCompiler::CodeGenerator::escapeHtml(const std::string& text) {
    std::string result;
    for (char c : text) {
        switch (c) {
            case '&': result += "&amp;"; break;
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '"': result += "&quot;"; break;
            case '\'': result += "&#39;"; break;
            default: result += c; break;
        }
    }
    return result;
}

// ==================== CHTLCompiler 主类实现 ====================

CHTLCompiler::CHTLCompiler() = default;
CHTLCompiler::~CHTLCompiler() = default;

CompilationResult CHTLCompiler::compile(const CodeFragment& fragment, const CompilerOptions& options) {
    CompilationResult result;
    result.success = true;
    
    try {
        CompilationContext context;
        
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
        
        // 收集全局样式
        if (!context.global_styles.empty()) {
            result.global_styles = "<style>\n";
            for (const auto& style : context.global_styles) {
                result.global_styles += style + "\n";
            }
            result.global_styles += "</style>\n";
        }
        
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

std::unique_ptr<ASTNode> CHTLCompiler::parseInput(const std::string& input, CompilationContext& context) {
    Lexer lexer(input);
    Parser parser(lexer, context);
    return parser.parse();
}

std::string CHTLCompiler::generateOutput(const std::unique_ptr<ASTNode>& ast, 
                                        CompilationContext& context,
                                        const CompilerOptions& options) {
    CodeGenerator generator(context, options);
    return generator.generate(ast);
}

} // namespace chtl
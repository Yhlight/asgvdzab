#include "compiler/chtl/chtl_parser.h"
#include "common/utils.h"
#include <sstream>

namespace chtl {
namespace compiler {

CHTLParser::CHTLParser() : lexer_(nullptr) {
}

CHTLParser::~CHTLParser() = default;

std::shared_ptr<ast::DocumentNode> CHTLParser::parse(CHTLLexer& lexer, CHTLContext& context) {
    lexer_ = &lexer;
    errors_.clear();
    warnings_.clear();
    
    auto document = std::make_shared<ast::DocumentNode>();
    document->location = SourceLocation();
    
    // 获取第一个token
    currentToken_ = lexer_->nextToken();
    
    // 解析顶层元素
    while (!isAtEnd()) {
        auto node = parseTopLevel();
        if (node) {
            document->children.push_back(node);
        }
    }
    
    return document;
}

std::shared_ptr<ast::ASTNode> CHTLParser::parseTopLevel() {
    skipWhitespaceAndComments();
    
    // 检查各种顶层结构
    if (check(TokenType::LEFT_BRACKET)) {
        // [Template], [Custom], [Import], [Namespace], [Origin], [Configuration]
        if (matchKeyword("Template")) {
            return parseTemplate();
        } else if (matchKeyword("Custom")) {
            return parseCustom();
        } else if (matchKeyword("Import")) {
            return parseImport();
        } else if (matchKeyword("Namespace")) {
            return parseNamespace();
        } else if (matchKeyword("Origin")) {
            return parseOrigin();
        } else if (matchKeyword("Configuration")) {
            return parseConfiguration();
        }
    }
    
    // 检查关键字
    if (check(TokenType::KEYWORD_TEXT)) {
        return parseText();
    }
    if (check(TokenType::KEYWORD_STYLE)) {
        return parseStyle();
    }
    if (check(TokenType::KEYWORD_SCRIPT)) {
        return parseScript();
    }
    
    // 检查元素（IDENTIFIER但不是关键字）
    if (check(TokenType::IDENTIFIER)) {
        return parseElement();
    }
    
    // 错误：无法识别的顶层结构
    error("Unexpected token at top level");
    advance();
    return nullptr;
}

std::shared_ptr<ast::ElementNode> CHTLParser::parseElement() {
    auto element = std::make_shared<ast::ElementNode>();
    element->location = currentToken_.location;
    
    // 解析元素名（当前token应该是IDENTIFIER）
    if (check(TokenType::IDENTIFIER)) {
        element->tagName = advance().value;
    } else {
        error("Expected element name");
        return element;
    }
    
    skipWhitespaceAndComments();
    
    // 解析属性（在花括号之前）
    while (!check(TokenType::LEFT_BRACE) && !isAtEnd()) {
        if (check(TokenType::IDENTIFIER)) {
            auto attr = parseAttribute();
            if (attr) {
                element->attributes.push_back(attr);
                
                // 特殊处理id和class
                if (attr->name == "id") {
                    element->id = attr->value;
                } else if (attr->name == "class") {
                    element->className = attr->value;
                }
            }
        } else {
            break;
        }
        skipWhitespaceAndComments();
    }
    
    // 解析元素体
    consume(TokenType::LEFT_BRACE, "Expected '{' after element name");
    
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        skipWhitespaceAndComments();
        
        if (check(TokenType::KEYWORD_TEXT)) {
            advance(); // consume 'text'
            element->children.push_back(parseText());
        } else if (check(TokenType::KEYWORD_STYLE)) {
            advance(); // consume 'style'
            element->children.push_back(parseStyle());
        } else if (check(TokenType::KEYWORD_SCRIPT)) {
            advance(); // consume 'script'
            element->children.push_back(parseScript());
        } else if (check(TokenType::IDENTIFIER)) {
            // 嵌套元素
            element->children.push_back(parseElement());
        } else if (!check(TokenType::RIGHT_BRACE)) {
            error("Unexpected token in element body");
            advance();
        }
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' to close element");
    
    return element;
}

std::shared_ptr<ast::TextNode> CHTLParser::parseText() {
    auto text = std::make_shared<ast::TextNode>();
    text->location = currentToken_.location;
    
    skipWhitespaceAndComments();
    consume(TokenType::LEFT_BRACE, "Expected '{' after 'text'");
    skipWhitespaceAndComments();
    
    // 解析文本内容
    if (check(TokenType::STRING_LITERAL)) {
        text->content = advance().value;
        text->isLiteral = false;
    } else if (check(TokenType::UNQUOTED_LITERAL) || check(TokenType::IDENTIFIER)) {
        text->content = advance().value;
        text->isLiteral = true;
    } else {
        error("Expected text content");
    }
    
    skipWhitespaceAndComments();
    consume(TokenType::RIGHT_BRACE, "Expected '}' to close text block");
    
    return text;
}

std::shared_ptr<ast::StyleNode> CHTLParser::parseStyle() {
    auto style = std::make_shared<ast::StyleNode>();
    style->location = currentToken_.location;
    
    // 检查是否是局部样式（在元素内部）
    // TODO: 实现上下文检查
    style->isLocal = false;
    
    skipWhitespaceAndComments();
    consume(TokenType::LEFT_BRACE, "Expected '{' after 'style'");
    
    // 收集样式内容直到遇到 }
    std::stringstream content;
    int braceDepth = 1;
    
    while (braceDepth > 0 && !isAtEnd()) {
        Token token = advance();
        if (token.type == TokenType::LEFT_BRACE) {
            braceDepth++;
        } else if (token.type == TokenType::RIGHT_BRACE) {
            braceDepth--;
            if (braceDepth > 0) {
                content << token.value;
            }
        } else {
            content << token.value;
            // 添加空格保持格式
            if (token.type != TokenType::WHITESPACE && !isAtEnd()) {
                Token next = peek();
                if (next.type != TokenType::RIGHT_BRACE && 
                    next.type != TokenType::LEFT_BRACE &&
                    next.type != TokenType::SEMICOLON &&
                    next.type != TokenType::COLON) {
                    content << " ";
                }
            }
        }
    }
    
    style->content = content.str();
    return style;
}

std::shared_ptr<ast::ScriptNode> CHTLParser::parseScript() {
    auto script = std::make_shared<ast::ScriptNode>();
    script->location = currentToken_.location;
    
    // 检查是否是局部脚本（在元素内部）
    // TODO: 实现上下文检查
    script->isLocal = false;
    
    skipWhitespaceAndComments();
    consume(TokenType::LEFT_BRACE, "Expected '{' after 'script'");
    
    // 收集脚本内容直到遇到 }
    std::stringstream content;
    int braceDepth = 1;
    
    while (braceDepth > 0 && !isAtEnd()) {
        Token token = advance();
        if (token.type == TokenType::LEFT_BRACE) {
            braceDepth++;
        } else if (token.type == TokenType::RIGHT_BRACE) {
            braceDepth--;
            if (braceDepth > 0) {
                content << token.value;
            }
        } else {
            content << token.value;
            // 添加空格保持格式
            if (token.type != TokenType::WHITESPACE && !isAtEnd()) {
                Token next = peek();
                if (next.type != TokenType::RIGHT_BRACE && 
                    next.type != TokenType::LEFT_BRACE &&
                    next.type != TokenType::SEMICOLON &&
                    next.type != TokenType::COLON) {
                    content << " ";
                }
            }
        }
    }
    
    script->content = content.str();
    return script;
}

// 辅助方法实现
void CHTLParser::skipWhitespaceAndComments() {
    while (match(TokenType::WHITESPACE) || 
           match(TokenType::SINGLE_LINE_COMMENT) ||
           match(TokenType::MULTI_LINE_COMMENT)) {
        // 跳过空白和注释
    }
}

bool CHTLParser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

Token CHTLParser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    
    error(message);
    return currentToken_;
}

Token CHTLParser::advance() {
    if (!isAtEnd()) {
        previousToken_ = currentToken_;
        currentToken_ = lexer_->nextToken();
    }
    return previousToken_;
}

Token CHTLParser::peek() const {
    return currentToken_;
}

Token CHTLParser::previous() const {
    return previousToken_;
}

bool CHTLParser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return currentToken_.type == type;
}

bool CHTLParser::checkKeyword(const std::string& keyword) const {
    return check(TokenType::IDENTIFIER) && currentToken_.value == keyword;
}

bool CHTLParser::matchKeyword(const std::string& keyword) {
    if (match(TokenType::LEFT_BRACKET)) {
        if (checkKeyword(keyword)) {
            advance(); // consume keyword
            if (match(TokenType::RIGHT_BRACKET)) {
                return true;
            }
        }
    }
    return false;
}

bool CHTLParser::isAtEnd() const {
    return currentToken_.type == TokenType::EOF_TOKEN;
}

void CHTLParser::error(const std::string& message) {
    errors_.push_back(utils::formatError(
        currentToken_.location.filename,
        currentToken_.location.line,
        currentToken_.location.column,
        message
    ));
    synchronize();
}

void CHTLParser::warning(const std::string& message) {
    warnings_.push_back(utils::formatWarning(
        currentToken_.location.filename,
        currentToken_.location.line,
        currentToken_.location.column,
        message
    ));
}

void CHTLParser::synchronize() {
    // 错误恢复：跳到下一个语句边界
    advance();
    
    while (!isAtEnd()) {
        if (currentToken_.type == TokenType::SEMICOLON) {
            advance();
            return;
        }
        
        if (currentToken_.type == TokenType::RIGHT_BRACE) {
            return;
        }
        
        advance();
    }
}

// 以下是占位实现，需要完善
std::shared_ptr<ast::TemplateNode> CHTLParser::parseTemplate() {
    return std::make_shared<ast::TemplateNode>();
}

std::shared_ptr<ast::CustomNode> CHTLParser::parseCustom() {
    return std::make_shared<ast::CustomNode>();
}

std::shared_ptr<ast::ImportNode> CHTLParser::parseImport() {
    return std::make_shared<ast::ImportNode>();
}

std::shared_ptr<ast::NamespaceNode> CHTLParser::parseNamespace() {
    return std::make_shared<ast::NamespaceNode>();
}

std::shared_ptr<ast::OriginNode> CHTLParser::parseOrigin() {
    return std::make_shared<ast::OriginNode>();
}

std::shared_ptr<ast::ConfigurationNode> CHTLParser::parseConfiguration() {
    // TODO: 实现配置解析
    return std::make_shared<ast::ConfigurationNode>();
}

std::vector<std::shared_ptr<ast::AttributeNode>> CHTLParser::parseAttributes() {
    std::vector<std::shared_ptr<ast::AttributeNode>> attributes;
    
    while (!check(TokenType::LEFT_BRACE) && !isAtEnd()) {
        skipWhitespaceAndComments();
        
        if (check(TokenType::IDENTIFIER)) {
            auto attr = parseAttribute();
            if (attr) {
                attributes.push_back(attr);
            }
        } else {
            break;
        }
        
        skipWhitespaceAndComments();
        
        // 属性之间可能有逗号分隔
        match(TokenType::COMMA);
    }
    
    return attributes;
}

std::shared_ptr<ast::AttributeNode> CHTLParser::parseAttribute() {
    auto attr = std::make_shared<ast::AttributeNode>();
    attr->location = currentToken_.location;
    
    // 属性名
    attr->name = consume(TokenType::IDENTIFIER, "Expected attribute name").value;
    
    skipWhitespaceAndComments();
    
    // 属性值（可选）
    if (match(TokenType::COLON) || match(TokenType::EQUALS)) {
        skipWhitespaceAndComments();
        
        if (check(TokenType::STRING_LITERAL)) {
            attr->value = advance().value;
        } else if (check(TokenType::UNQUOTED_LITERAL) || check(TokenType::IDENTIFIER)) {
            attr->value = advance().value;
        } else {
            error("Expected attribute value");
        }
    }
    
    return attr;
}

} // namespace compiler
} // namespace chtl
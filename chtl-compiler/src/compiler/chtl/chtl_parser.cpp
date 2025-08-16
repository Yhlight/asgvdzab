#include "compiler/chtl/chtl_parser.h"
#include "common/utils.h"
#include <sstream>

namespace chtl {
namespace compiler {

CHTLParser::CHTLParser() : lexer_(nullptr) {
}

CHTLParser::~CHTLParser() = default;

std::shared_ptr<ast::DocumentNode> CHTLParser::parse(CHTLLexer& lexer) {
    lexer_ = &lexer;
    errors_.clear();
    warnings_.clear();
    
    auto document = std::make_shared<ast::DocumentNode>();
    document->setLocation(lexer_->getCurrentLocation());
    
    // 获取第一个token
    advance();
    
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
    // 跳过空白和注释
    while (match(TokenType::WHITESPACE) || match(TokenType::COMMENT)) {
        // 继续
    }
    
    if (isAtEnd()) {
        return nullptr;
    }
    
    // 检查特殊标记
    if (check(TokenType::LEFT_BRACKET)) {
        // [Template], [Custom], [Import], [Namespace], [Origin], [Configuration]
        advance(); // [
        if (check(TokenType::IDENTIFIER)) {
            std::string keyword = currentToken_.value;
            if (keyword == "Template") {
                advance(); // Template
                consume(TokenType::RIGHT_BRACKET, "Expected ']' after 'Template'");
                return parseTemplate();
            } else if (keyword == "Custom") {
                advance(); // Custom
                consume(TokenType::RIGHT_BRACKET, "Expected ']' after 'Custom'");
                return parseCustom();
            } else if (keyword == "Import") {
                advance(); // Import
                consume(TokenType::RIGHT_BRACKET, "Expected ']' after 'Import'");
                return parseImport();
            } else if (keyword == "Namespace") {
                advance(); // Namespace
                consume(TokenType::RIGHT_BRACKET, "Expected ']' after 'Namespace'");
                return parseNamespace();
            } else if (keyword == "Origin") {
                advance(); // Origin
                consume(TokenType::RIGHT_BRACKET, "Expected ']' after 'Origin'");
                return parseOrigin();
            } else if (keyword == "Configuration") {
                advance(); // Configuration
                consume(TokenType::RIGHT_BRACKET, "Expected ']' after 'Configuration'");
                return parseConfiguration();
            }
        }
        error("Unknown bracket expression");
        return nullptr;
    }
    
    // 检查元素
    if (check(TokenType::IDENTIFIER)) {
        std::string identifier = currentToken_.value;
        
        // 检查是否是关键字
        if (identifier == "text") {
            return parseText();
        } else if (identifier == "style") {
            return parseStyle();
        } else if (identifier == "script") {
            return parseScript();
        } else {
            // 假设是HTML元素
            return parseElement();
        }
    }
    
    error("Unexpected token at top level");
    advance(); // 跳过错误的token
    return nullptr;
}

std::shared_ptr<ast::ElementNode> CHTLParser::parseElement() {
    auto element = std::make_shared<ast::ElementNode>();
    element->setLocation(currentToken_.location);
    
    // 元素名
    element->tagName = currentToken_.value;
    advance();
    
    // 解析属性（在{之前）
    while (!check(TokenType::LEFT_BRACE) && !isAtEnd()) {
        if (check(TokenType::IDENTIFIER)) {
            auto attr = parseAttribute();
            element->attributes[attr.first] = attr.second;
            
            // 特殊处理id和class
            if (attr.first == "id") {
                element->id = attr.second;
            } else if (attr.first == "class") {
                element->className = attr.second;
                element->classList = utils::split(attr.second, ' ');
            }
        } else {
            advance(); // 跳过其他token
        }
    }
    
    // 解析子元素
    if (consume(TokenType::LEFT_BRACE, "Expected '{' after element name")) {
        while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
            auto child = parseTopLevel();
            if (child) {
                element->children.push_back(child);
            }
        }
        consume(TokenType::RIGHT_BRACE, "Expected '}' to close element");
    }
    
    return element;
}

std::shared_ptr<ast::TextNode> CHTLParser::parseText() {
    auto text = std::make_shared<ast::TextNode>();
    text->setLocation(currentToken_.location);
    
    advance(); // text
    consume(TokenType::LEFT_BRACE, "Expected '{' after 'text'");
    
    // 解析文本内容
    if (check(TokenType::STRING_LITERAL)) {
        text->content = currentToken_.value;
        advance();
    } else if (check(TokenType::IDENTIFIER)) {
        // 无引号字面量
        text->content = currentToken_.value;
        text->isLiteral = true;
        advance();
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' to close text block");
    
    return text;
}

std::shared_ptr<ast::StyleNode> CHTLParser::parseStyle() {
    auto style = std::make_shared<ast::StyleNode>();
    style->setLocation(currentToken_.location);
    
    advance(); // style
    consume(TokenType::LEFT_BRACE, "Expected '{' after 'style'");
    
    // TODO: 实现样式规则解析
    // 这里需要解析CSS语法
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' to close style block");
    
    return style;
}

std::shared_ptr<ast::ScriptNode> CHTLParser::parseScript() {
    auto script = std::make_shared<ast::ScriptNode>();
    script->setLocation(currentToken_.location);
    
    advance(); // script
    consume(TokenType::LEFT_BRACE, "Expected '{' after 'script'");
    
    // TODO: 收集脚本内容直到}
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' to close script block");
    
    return script;
}

std::pair<std::string, std::string> CHTLParser::parseAttribute() {
    std::string name = currentToken_.value;
    advance();
    
    // CE对等式：: 和 = 是等价的
    if (!match(TokenType::COLON) && !match(TokenType::EQUALS)) {
        error("Expected ':' or '=' after attribute name");
        return {name, ""};
    }
    
    std::string value;
    if (check(TokenType::STRING_LITERAL)) {
        value = currentToken_.value;
        advance();
    } else if (check(TokenType::IDENTIFIER)) {
        // 无引号字面量
        value = currentToken_.value;
        advance();
    }
    
    // 分号是可选的
    match(TokenType::SEMICOLON);
    
    return {name, value};
}

// 辅助方法实现
bool CHTLParser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool CHTLParser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        advance();
        return true;
    }
    
    error(message);
    return false;
}

void CHTLParser::advance() {
    if (!isAtEnd()) {
        currentToken_ = lexer_->nextToken();
    }
}

Token CHTLParser::peek() {
    return lexer_->peekToken();
}

bool CHTLParser::check(TokenType type) {
    if (isAtEnd()) return false;
    return currentToken_.type == type;
}

bool CHTLParser::checkKeyword(const std::string& keyword) {
    return check(TokenType::IDENTIFIER) && currentToken_.value == keyword;
}

bool CHTLParser::isAtEnd() {
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
    return std::make_shared<ast::ConfigurationNode>();
}

} // namespace compiler
} // namespace chtl
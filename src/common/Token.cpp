#include "../../include/common/Token.h"

namespace CHTL {

std::string Token::getTypeString() const {
    switch (type) {
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::STRING_LITERAL: return "STRING_LITERAL";
        case TokenType::NUMBER_LITERAL: return "NUMBER_LITERAL";
        case TokenType::UNQUOTED_LITERAL: return "UNQUOTED_LITERAL";
        
        case TokenType::COLON: return "COLON";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::EQUALS: return "EQUALS";
        case TokenType::COMMA: return "COMMA";
        case TokenType::DOT: return "DOT";
        case TokenType::HASH: return "HASH";
        case TokenType::AMPERSAND: return "AMPERSAND";
        case TokenType::ARROW: return "ARROW";
        
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACKET: return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
        
        case TokenType::KEYWORD_TEXT: return "KEYWORD_TEXT";
        case TokenType::KEYWORD_STYLE: return "KEYWORD_STYLE";
        case TokenType::KEYWORD_SCRIPT: return "KEYWORD_SCRIPT";
        case TokenType::KEYWORD_INHERIT: return "KEYWORD_INHERIT";
        case TokenType::KEYWORD_DELETE: return "KEYWORD_DELETE";
        case TokenType::KEYWORD_INSERT: return "KEYWORD_INSERT";
        case TokenType::KEYWORD_AFTER: return "KEYWORD_AFTER";
        case TokenType::KEYWORD_BEFORE: return "KEYWORD_BEFORE";
        case TokenType::KEYWORD_REPLACE: return "KEYWORD_REPLACE";
        case TokenType::KEYWORD_AT_TOP: return "KEYWORD_AT_TOP";
        case TokenType::KEYWORD_AT_BOTTOM: return "KEYWORD_AT_BOTTOM";
        case TokenType::KEYWORD_FROM: return "KEYWORD_FROM";
        case TokenType::KEYWORD_AS: return "KEYWORD_AS";
        case TokenType::KEYWORD_EXCEPT: return "KEYWORD_EXCEPT";
        case TokenType::KEYWORD_VIR: return "KEYWORD_VIR";
        
        case TokenType::TEMPLATE: return "TEMPLATE";
        case TokenType::CUSTOM: return "CUSTOM";
        case TokenType::ORIGIN: return "ORIGIN";
        case TokenType::IMPORT: return "IMPORT";
        case TokenType::NAMESPACE: return "NAMESPACE";
        case TokenType::CONFIGURATION: return "CONFIGURATION";
        case TokenType::INFO: return "INFO";
        case TokenType::EXPORT: return "EXPORT";
        
        case TokenType::AT_STYLE: return "AT_STYLE";
        case TokenType::AT_ELEMENT: return "AT_ELEMENT";
        case TokenType::AT_VAR: return "AT_VAR";
        case TokenType::AT_HTML: return "AT_HTML";
        case TokenType::AT_JAVASCRIPT: return "AT_JAVASCRIPT";
        case TokenType::AT_CHTL: return "AT_CHTL";
        case TokenType::AT_CJMOD: return "AT_CJMOD";
        
        case TokenType::COMMENT_LINE: return "COMMENT_LINE";
        case TokenType::COMMENT_BLOCK: return "COMMENT_BLOCK";
        case TokenType::COMMENT_GENERATOR: return "COMMENT_GENERATOR";
        case TokenType::EOF_TOKEN: return "EOF_TOKEN";
        case TokenType::NEWLINE: return "NEWLINE";
        case TokenType::WHITESPACE: return "WHITESPACE";
        
        case TokenType::INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

bool Token::isKeyword() const {
    return type >= TokenType::KEYWORD_TEXT && type <= TokenType::KEYWORD_VIR;
}

bool Token::isOperator() const {
    return type >= TokenType::COLON && type <= TokenType::ARROW;
}

bool Token::isLiteral() const {
    return type == TokenType::STRING_LITERAL || 
           type == TokenType::NUMBER_LITERAL || 
           type == TokenType::UNQUOTED_LITERAL;
}

bool Token::isBlockIdentifier() const {
    return type >= TokenType::TEMPLATE && type <= TokenType::EXPORT;
}

bool Token::isTypeIdentifier() const {
    return type >= TokenType::AT_STYLE && type <= TokenType::AT_CJMOD;
}

} // namespace CHTL
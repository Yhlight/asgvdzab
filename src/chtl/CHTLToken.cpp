#include "CHTLToken.hpp"

namespace chtl {

const char* tokenTypeName(CHTLTokenType type) {
    switch (type) {
        // 结构符号
        case CHTLTokenType::LEFT_BRACE: return "LEFT_BRACE";
        case CHTLTokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case CHTLTokenType::LEFT_BRACKET: return "LEFT_BRACKET";
        case CHTLTokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
        case CHTLTokenType::LEFT_PAREN: return "LEFT_PAREN";
        case CHTLTokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case CHTLTokenType::SEMICOLON: return "SEMICOLON";
        case CHTLTokenType::COMMA: return "COMMA";
        case CHTLTokenType::COLON: return "COLON";
        case CHTLTokenType::EQUALS: return "EQUALS";
        case CHTLTokenType::DOT: return "DOT";
        case CHTLTokenType::AT: return "AT";
        case CHTLTokenType::AMPERSAND: return "AMPERSAND";
        
        // 注释
        case CHTLTokenType::COMMENT_LINE: return "COMMENT_LINE";
        case CHTLTokenType::COMMENT_BLOCK: return "COMMENT_BLOCK";
        case CHTLTokenType::COMMENT_HTML: return "COMMENT_HTML";
        
        // 字面量
        case CHTLTokenType::STRING_LITERAL: return "STRING_LITERAL";
        case CHTLTokenType::UNQUOTED_LITERAL: return "UNQUOTED_LITERAL";
        case CHTLTokenType::NUMBER_LITERAL: return "NUMBER_LITERAL";
        
        // 关键字
        case CHTLTokenType::TEXT: return "TEXT";
        case CHTLTokenType::STYLE: return "STYLE";
        case CHTLTokenType::SCRIPT: return "SCRIPT";
        case CHTLTokenType::TEMPLATE: return "TEMPLATE";
        case CHTLTokenType::CUSTOM: return "CUSTOM";
        case CHTLTokenType::ORIGIN: return "ORIGIN";
        case CHTLTokenType::IMPORT: return "IMPORT";
        case CHTLTokenType::NAMESPACE: return "NAMESPACE";
        case CHTLTokenType::CONFIGURATION: return "CONFIGURATION";
        case CHTLTokenType::NAME: return "NAME";
        case CHTLTokenType::INFO: return "INFO";
        case CHTLTokenType::EXPORT: return "EXPORT";
        
        // 模板/自定义修饰符
        case CHTLTokenType::AT_STYLE: return "AT_STYLE";
        case CHTLTokenType::AT_ELEMENT: return "AT_ELEMENT";
        case CHTLTokenType::AT_VAR: return "AT_VAR";
        case CHTLTokenType::AT_HTML: return "AT_HTML";
        case CHTLTokenType::AT_JAVASCRIPT: return "AT_JAVASCRIPT";
        case CHTLTokenType::AT_CHTL: return "AT_CHTL";
        case CHTLTokenType::AT_CJMOD: return "AT_CJMOD";
        
        // 操作关键字
        case CHTLTokenType::INHERIT: return "INHERIT";
        case CHTLTokenType::DELETE: return "DELETE";
        case CHTLTokenType::INSERT: return "INSERT";
        case CHTLTokenType::AFTER: return "AFTER";
        case CHTLTokenType::BEFORE: return "BEFORE";
        case CHTLTokenType::REPLACE: return "REPLACE";
        case CHTLTokenType::AT_TOP: return "AT_TOP";
        case CHTLTokenType::AT_BOTTOM: return "AT_BOTTOM";
        case CHTLTokenType::FROM: return "FROM";
        case CHTLTokenType::AS: return "AS";
        case CHTLTokenType::EXCEPT: return "EXCEPT";
        
        // 其他
        case CHTLTokenType::IDENTIFIER: return "IDENTIFIER";
        case CHTLTokenType::EOF_TOKEN: return "EOF";
        case CHTLTokenType::ERROR: return "ERROR";
        
        default: return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& os, const CHTLToken& token) {
    os << tokenTypeName(token.type) << "[" << token.value << "]"
       << " at " << token.line << ":" << token.column;
    return os;
}

} // namespace chtl
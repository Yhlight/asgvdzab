#include "lexer/CHTLToken.h"
#include <sstream>

namespace chtl {

std::string Token::toString() const {
    std::ostringstream oss;
    oss << "Token{type=" << static_cast<int>(type) 
        << ", value='" << value << "'"
        << ", line=" << line 
        << ", column=" << column 
        << ", position=" << position << "}";
    return oss.str();
}

std::string CHTLJSToken::toString() const {
    std::ostringstream oss;
    oss << "CHTLJSToken{type=" << static_cast<int>(type) 
        << ", value='" << value << "'"
        << ", line=" << line 
        << ", column=" << column 
        << ", position=" << position << "}";
    return oss.str();
}

} // namespace chtl
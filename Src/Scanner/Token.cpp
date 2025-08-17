#include "Scanner/Token.h"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace Chtl {

// === Token类实现 ===

bool Token::IsKeyword() const {
    return Type_ >= TokenType::TEXT && Type_ <= TokenType::ANIMATE;
}

bool Token::IsOperator() const {
    return Type_ == TokenType::COLON || Type_ == TokenType::ASSIGN || 
           Type_ == TokenType::DOT || Type_ == TokenType::ARROW_OPERATOR;
}

bool Token::IsLiteral() const {
    return Type_ == TokenType::UNQUOTED_STRING || 
           Type_ == TokenType::DOUBLE_QUOTED_STRING || 
           Type_ == TokenType::SINGLE_QUOTED_STRING;
}

bool Token::IsDelimiter() const {
    return Type_ >= TokenType::LBRACE && Type_ <= TokenType::DOT;
}

bool Token::IsComment() const {
    return Type_ >= TokenType::LINE_COMMENT && Type_ <= TokenType::GENERATOR_COMMENT;
}

bool Token::IsChtlKeyword() const {
    return Type_ == TokenType::TEXT || Type_ == TokenType::STYLE || Type_ == TokenType::SCRIPT;
}

bool Token::IsChtlJsKeyword() const {
    return Type_ >= TokenType::VIR && Type_ <= TokenType::ANIMATE;
}

bool Token::IsTemplateKeyword() const {
    return (Type_ >= TokenType::TEMPLATE_BRACKET && Type_ <= TokenType::EXPORT_BRACKET) ||
           (Type_ >= TokenType::AT_STYLE && Type_ <= TokenType::AT_CJMOD);
}

bool Token::IsHtmlElement() const {
    return Type_ == TokenType::HTML_ELEMENT;
}

bool Token::IsEnhancedSelector() const {
    return Type_ == TokenType::ENHANCED_SELECTOR;
}

std::string Token::GetStringValue() const {
    if (std::holds_alternative<std::string>(Value_)) {
        return std::get<std::string>(Value_);
    }
    return Text_;
}

int Token::GetIntValue() const {
    if (std::holds_alternative<int>(Value_)) {
        return std::get<int>(Value_);
    }
    return 0;
}

double Token::GetDoubleValue() const {
    if (std::holds_alternative<double>(Value_)) {
        return std::get<double>(Value_);
    }
    return 0.0;
}

bool Token::GetBoolValue() const {
    if (std::holds_alternative<bool>(Value_)) {
        return std::get<bool>(Value_);
    }
    return false;
}

std::string Token::ToString() const {
    std::ostringstream oss;
    oss << TokenUtils::TokenTypeToString(Type_) << "('" << Text_ << "')";
    if (Location_.FileName.empty()) {
        oss << " at " << Location_.Line << ":" << Location_.Column;
    } else {
        oss << " at " << Location_.FileName << ":" << Location_.Line << ":" << Location_.Column;
    }
    return oss.str();
}

bool Token::Equals(const Token& other) const {
    return Type_ == other.Type_ && Text_ == other.Text_;
}

// === TokenStream类实现 ===

const Token TokenStream::EofToken_{TokenType::EOF_TOKEN, ""};

const Token& TokenStream::Current() const {
    if (CurrentIndex_ >= Tokens_.size()) {
        return EofToken_;
    }
    return Tokens_[CurrentIndex_];
}

const Token& TokenStream::Peek(size_t offset) const {
    size_t targetIndex = CurrentIndex_ + offset;
    if (targetIndex >= Tokens_.size()) {
        return EofToken_;
    }
    return Tokens_[targetIndex];
}

const Token& TokenStream::Previous() const {
    if (CurrentIndex_ == 0) {
        return EofToken_;
    }
    return Tokens_[CurrentIndex_ - 1];
}

Token& TokenStream::Advance() {
    if (CurrentIndex_ < Tokens_.size()) {
        ++CurrentIndex_;
    }
    return const_cast<Token&>(Current());
}

bool TokenStream::Match(TokenType type) {
    if (Check(type)) {
        Advance();
        return true;
    }
    return false;
}

bool TokenStream::Match(const std::vector<TokenType>& types) {
    for (auto type : types) {
        if (Check(type)) {
            Advance();
            return true;
        }
    }
    return false;
}

bool TokenStream::Check(TokenType type) const {
    if (IsAtEnd()) return false;
    return Current().GetType() == type;
}

Token TokenStream::Consume(TokenType type, const std::string& errorMessage) {
    if (Check(type)) {
        return Advance();
    }
    
    std::string msg = errorMessage.empty() ? 
        "Expected " + TokenUtils::TokenTypeToString(type) : errorMessage;
    throw std::runtime_error(msg + " at " + Current().ToString());
}

void TokenStream::SkipWhitespace() {
    while (!IsAtEnd() && Current().GetType() == TokenType::WHITESPACE) {
        Advance();
    }
}

void TokenStream::SkipComments() {
    while (!IsAtEnd() && Current().IsComment()) {
        Advance();
    }
}

void TokenStream::SkipWhitespaceAndComments() {
    while (!IsAtEnd() && 
           (Current().GetType() == TokenType::WHITESPACE || Current().IsComment())) {
        Advance();
    }
}

std::string TokenStream::ToString() const {
    std::ostringstream oss;
    oss << "TokenStream[" << Tokens_.size() << " tokens, current=" << CurrentIndex_ << "]";
    return oss.str();
}

void TokenStream::PrintTokens() const {
    for (size_t i = 0; i < Tokens_.size(); ++i) {
        std::cout << "[" << i << "] " << Tokens_[i].ToString() << std::endl;
    }
}

// === ChtlKeywords类实现 ===

std::unordered_set<std::string> ChtlKeywords::HtmlElements_;
std::unordered_set<std::string> ChtlKeywords::ChtlCoreKeywords_;
std::unordered_set<std::string> ChtlKeywords::TemplateKeywords_;
std::unordered_set<std::string> ChtlKeywords::ControlKeywords_;
std::unordered_set<std::string> ChtlKeywords::ChtlJsKeywords_;
bool ChtlKeywords::Initialized_ = false;

void ChtlKeywords::InitializeKeywords() {
    if (Initialized_) return;
    
    // HTML元素标签 - 按照HTML5标准
    HtmlElements_ = {
        // 文档结构
        "html", "head", "body", "title", "meta", "link", "style", "script",
        
        // 文档分区
        "header", "nav", "main", "section", "article", "aside", "footer",
        "h1", "h2", "h3", "h4", "h5", "h6", "hgroup",
        
        // 文本内容
        "div", "p", "hr", "pre", "blockquote", "ol", "ul", "li", "dl", "dt", "dd",
        "figure", "figcaption", "address",
        
        // 内联文本语义
        "a", "em", "strong", "small", "s", "cite", "q", "dfn", "abbr", "data",
        "time", "code", "var", "samp", "kbd", "sub", "sup", "i", "b", "u",
        "mark", "ruby", "rt", "rp", "bdi", "bdo", "span", "br", "wbr",
        
        // 图片和多媒体
        "img", "iframe", "embed", "object", "param", "video", "audio", "source",
        "track", "canvas", "map", "area", "svg", "math",
        
        // 表格
        "table", "caption", "colgroup", "col", "tbody", "thead", "tfoot",
        "tr", "td", "th",
        
        // 表单
        "form", "fieldset", "legend", "label", "input", "button", "select",
        "datalist", "optgroup", "option", "textarea", "keygen", "output",
        "progress", "meter",
        
        // 交互元素
        "details", "summary", "dialog", "menu", "menuitem",
        
        // Web组件
        "template", "slot"
    };
    
    // CHTL核心关键字
    ChtlCoreKeywords_ = {
        "text", "style", "script"
    };
    
    // 模板系统关键字
    TemplateKeywords_ = {
        "[Template]", "[Custom]", "[Origin]", "[Configuration]", 
        "[Namespace]", "[Import]", "[Info]", "[Export]",
        "@Style", "@Element", "@Var", "@Html", "@JavaScript", "@Chtl", "@CJmod"
    };
    
    // 控制关键字
    ControlKeywords_ = {
        "inherit", "delete", "insert", "after", "before", "replace",
        "at top", "at bottom", "from", "as", "except"
    };
    
    // CHTL JS关键字
    ChtlJsKeywords_ = {
        "vir", "listen", "delegate", "animate"
    };
    
    Initialized_ = true;
}

const std::unordered_set<std::string>& ChtlKeywords::GetHtmlElements() {
    InitializeKeywords();
    return HtmlElements_;
}

const std::unordered_set<std::string>& ChtlKeywords::GetChtlCoreKeywords() {
    InitializeKeywords();
    return ChtlCoreKeywords_;
}

const std::unordered_set<std::string>& ChtlKeywords::GetTemplateKeywords() {
    InitializeKeywords();
    return TemplateKeywords_;
}

const std::unordered_set<std::string>& ChtlKeywords::GetControlKeywords() {
    InitializeKeywords();
    return ControlKeywords_;
}

const std::unordered_set<std::string>& ChtlKeywords::GetChtlJsKeywords() {
    InitializeKeywords();
    return ChtlJsKeywords_;
}

bool ChtlKeywords::IsHtmlElement(const std::string& text) {
    InitializeKeywords();
    return HtmlElements_.find(text) != HtmlElements_.end();
}

bool ChtlKeywords::IsChtlCoreKeyword(const std::string& text) {
    InitializeKeywords();
    return ChtlCoreKeywords_.find(text) != ChtlCoreKeywords_.end();
}

bool ChtlKeywords::IsTemplateKeyword(const std::string& text) {
    InitializeKeywords();
    return TemplateKeywords_.find(text) != TemplateKeywords_.end();
}

bool ChtlKeywords::IsControlKeyword(const std::string& text) {
    InitializeKeywords();
    return ControlKeywords_.find(text) != ControlKeywords_.end();
}

bool ChtlKeywords::IsChtlJsKeyword(const std::string& text) {
    InitializeKeywords();
    return ChtlJsKeywords_.find(text) != ChtlJsKeywords_.end();
}

bool ChtlKeywords::IsEnhancedSelector(const std::string& text) {
    return text.length() >= 4 && text.substr(0, 2) == "{{" && 
           text.substr(text.length() - 2) == "}}";
}

TokenType ChtlKeywords::GetKeywordType(const std::string& text) {
    InitializeKeywords();
    
    // CHTL核心关键字
    if (text == "text") return TokenType::TEXT;
    if (text == "style") return TokenType::STYLE;
    if (text == "script") return TokenType::SCRIPT;
    
    // 模板系统
    if (text == "[Template]") return TokenType::TEMPLATE_BRACKET;
    if (text == "[Custom]") return TokenType::CUSTOM_BRACKET;
    if (text == "[Origin]") return TokenType::ORIGIN_BRACKET;
    if (text == "[Configuration]") return TokenType::CONFIGURATION_BRACKET;
    if (text == "[Namespace]") return TokenType::NAMESPACE_BRACKET;
    if (text == "[Import]") return TokenType::IMPORT_BRACKET;
    if (text == "[Info]") return TokenType::INFO_BRACKET;
    if (text == "[Export]") return TokenType::EXPORT_BRACKET;
    
    if (text == "@Style") return TokenType::AT_STYLE;
    if (text == "@Element") return TokenType::AT_ELEMENT;
    if (text == "@Var") return TokenType::AT_VAR;
    if (text == "@Html") return TokenType::AT_HTML;
    if (text == "@JavaScript") return TokenType::AT_JAVASCRIPT;
    if (text == "@Chtl") return TokenType::AT_CHTL;
    if (text == "@CJmod") return TokenType::AT_CJMOD;
    
    // 控制关键字
    if (text == "inherit") return TokenType::INHERIT;
    if (text == "delete") return TokenType::DELETE;
    if (text == "insert") return TokenType::INSERT;
    if (text == "after") return TokenType::AFTER;
    if (text == "before") return TokenType::BEFORE;
    if (text == "replace") return TokenType::REPLACE;
    if (text == "at top") return TokenType::AT_TOP;
    if (text == "at bottom") return TokenType::AT_BOTTOM;
    if (text == "from") return TokenType::FROM;
    if (text == "as") return TokenType::AS;
    if (text == "except") return TokenType::EXCEPT;
    
    // CHTL JS关键字
    if (text == "vir") return TokenType::VIR;
    if (text == "listen") return TokenType::LISTEN;
    if (text == "delegate") return TokenType::DELEGATE;
    if (text == "animate") return TokenType::ANIMATE;
    
    // HTML元素
    if (IsHtmlElement(text)) return TokenType::HTML_ELEMENT;
    
    return TokenType::IDENTIFIER;
}

// === TokenUtils类实现 ===

std::string TokenUtils::TokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::UNQUOTED_STRING: return "UNQUOTED_STRING";
        case TokenType::DOUBLE_QUOTED_STRING: return "DOUBLE_QUOTED_STRING";
        case TokenType::SINGLE_QUOTED_STRING: return "SINGLE_QUOTED_STRING";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::LBRACKET: return "LBRACKET";
        case TokenType::RBRACKET: return "RBRACKET";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COLON: return "COLON";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::COMMA: return "COMMA";
        case TokenType::DOT: return "DOT";
        case TokenType::TEXT: return "TEXT";
        case TokenType::STYLE: return "STYLE";
        case TokenType::SCRIPT: return "SCRIPT";
        case TokenType::HTML_ELEMENT: return "HTML_ELEMENT";
        case TokenType::LINE_COMMENT: return "LINE_COMMENT";
        case TokenType::BLOCK_COMMENT: return "BLOCK_COMMENT";
        case TokenType::GENERATOR_COMMENT: return "GENERATOR_COMMENT";
        
        // 模板系统Token
        case TokenType::TEMPLATE_BRACKET: return "TEMPLATE_BRACKET";
        case TokenType::CUSTOM_BRACKET: return "CUSTOM_BRACKET";
        case TokenType::ORIGIN_BRACKET: return "ORIGIN_BRACKET";
        case TokenType::CONFIGURATION_BRACKET: return "CONFIGURATION_BRACKET";
        case TokenType::NAMESPACE_BRACKET: return "NAMESPACE_BRACKET";
        case TokenType::IMPORT_BRACKET: return "IMPORT_BRACKET";
        case TokenType::INFO_BRACKET: return "INFO_BRACKET";
        case TokenType::EXPORT_BRACKET: return "EXPORT_BRACKET";
        
        case TokenType::AT_STYLE: return "AT_STYLE";
        case TokenType::AT_ELEMENT: return "AT_ELEMENT";
        case TokenType::AT_VAR: return "AT_VAR";
        case TokenType::AT_HTML: return "AT_HTML";
        case TokenType::AT_JAVASCRIPT: return "AT_JAVASCRIPT";
        case TokenType::AT_CHTL: return "AT_CHTL";
        case TokenType::AT_CJMOD: return "AT_CJMOD";
        
        // 控制关键字
        case TokenType::INHERIT: return "INHERIT";
        case TokenType::DELETE: return "DELETE";
        case TokenType::INSERT: return "INSERT";
        case TokenType::AFTER: return "AFTER";
        case TokenType::BEFORE: return "BEFORE";
        case TokenType::REPLACE: return "REPLACE";
        case TokenType::AT_TOP: return "AT_TOP";
        case TokenType::AT_BOTTOM: return "AT_BOTTOM";
        case TokenType::FROM: return "FROM";
        case TokenType::AS: return "AS";
        case TokenType::EXCEPT: return "EXCEPT";
        
        // CHTL JS Token
        case TokenType::ENHANCED_SELECTOR: return "ENHANCED_SELECTOR";
        case TokenType::ARROW_OPERATOR: return "ARROW_OPERATOR";
        case TokenType::VIR: return "VIR";
        case TokenType::LISTEN: return "LISTEN";
        case TokenType::DELEGATE: return "DELEGATE";
        case TokenType::ANIMATE: return "ANIMATE";
        
        case TokenType::AMPERSAND: return "AMPERSAND";
        case TokenType::NEWLINE: return "NEWLINE";
        case TokenType::WHITESPACE: return "WHITESPACE";
        case TokenType::EOF_TOKEN: return "EOF_TOKEN";
        case TokenType::UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN_TOKEN_TYPE";
    }
}

TokenType TokenUtils::StringToTokenType(const std::string& str) {
    return ChtlKeywords::GetKeywordType(str);
}

bool TokenUtils::IsValidIdentifier(const std::string& text) {
    if (text.empty()) return false;
    
    // 第一个字符必须是字母或下划线
    if (!std::isalpha(text[0]) && text[0] != '_') return false;
    
    // 其余字符必须是字母、数字或下划线
    for (size_t i = 1; i < text.length(); ++i) {
        if (!std::isalnum(text[i]) && text[i] != '_' && text[i] != '-') {
            return false;
        }
    }
    
    return true;
}

bool TokenUtils::IsValidUnquotedString(const std::string& text) {
    if (text.empty()) return false;
    
    // 无修饰字面量可以包含字母、数字、连字符、下划线等
    for (char ch : text) {
        if (!std::isalnum(ch) && ch != '-' && ch != '_' && ch != '.' && ch != '#') {
            return false;
        }
    }
    
    return true;
}

bool TokenUtils::IsEnhancedSelector(const std::string& text) {
    return ChtlKeywords::IsEnhancedSelector(text);
}

std::string TokenUtils::ExtractSelectorContent(const std::string& text) {
    if (!IsEnhancedSelector(text)) return "";
    return text.substr(2, text.length() - 4);  // 去掉 {{ 和 }}
}

bool TokenUtils::ValidateEnhancedSelector(const std::string& selector) {
    std::string content = ExtractSelectorContent(selector);
    if (content.empty()) return false;
    
    // 验证增强选择器语法 - 根据文档支持的格式
    // 支持: tag, .class, #id, .class tag, tag[index]
    
    // 简单验证：不能包含某些特殊字符
    for (char ch : content) {
        if (ch == '{' || ch == '}' || ch == '<' || ch == '>') {
            return false;
        }
    }
    
    return true;
}

std::string TokenUtils::ProcessUnquotedString(const std::string& text) {
    // 无修饰字面量按原样返回
    return text;
}

std::string TokenUtils::ProcessQuotedString(const std::string& text, char quote) {
    if (text.length() < 2) return text;
    if (text.front() != quote || text.back() != quote) return text;
    
    // 去掉引号并处理转义字符
    std::string result = text.substr(1, text.length() - 2);
    
    // 简单的转义处理
    std::string processed;
    for (size_t i = 0; i < result.length(); ++i) {
        if (result[i] == '\\' && i + 1 < result.length()) {
            switch (result[i + 1]) {
                case 'n': processed += '\n'; break;
                case 't': processed += '\t'; break;
                case 'r': processed += '\r'; break;
                case '\\': processed += '\\'; break;
                case '"': processed += '"'; break;
                case '\'': processed += '\''; break;
                default: processed += result[i + 1]; break;
            }
            ++i;  // 跳过转义字符
        } else {
            processed += result[i];
        }
    }
    
    return processed;
}

bool TokenUtils::IsColonEqualOperator(TokenType type) {
    // CE对等式：在CHTL中，':'与'='完全等价
    return type == TokenType::COLON || type == TokenType::ASSIGN;
}

} // namespace Chtl
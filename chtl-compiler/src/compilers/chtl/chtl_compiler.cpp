#include "compilers/chtl_compiler.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <cctype>
#include <regex>

namespace chtl {

CHTLCompiler::CHTLCompiler() 
    : debug_mode_(false),
      current_pos_(0),
      current_line_(1),
      current_column_(1),
      token_index_(0),
      class_counter_(0) {
}

CHTLCompiler::~CHTLCompiler() {
}

CompileResult CHTLCompiler::compile(const CodeFragment& fragment,
                                   const CompileOptions& options) {
    CompileResult result;
    
    if (!validate(fragment)) {
        result.success = false;
        result.error_msg = "Invalid fragment type for CHTL compiler";
        return result;
    }
    
    try {
        // 重置状态
        source_code_ = fragment.content;
        current_pos_ = 0;
        current_line_ = 1;
        current_column_ = 1;
        output_.clear();
        global_styles_.clear();
        global_scripts_.clear();
        style_rules_.clear();
        generated_classes_.clear();
        class_counter_ = 0;
        
        // 词法分析
        auto tokens = tokenize(fragment.content);
        
        if (debug_mode_) {
            std::cout << "Tokenized " << tokens.size() << " tokens\n";
        }
        
        // 语法分析和代码生成
        result.output = parseAndGenerate(tokens);
        result.success = true;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.error_msg = e.what();
    }
    
    return result;
}

std::vector<Token> CHTLCompiler::tokenize(const std::string& source) {
    std::vector<Token> tokens;
    source_code_ = source;
    current_pos_ = 0;
    current_line_ = 1;
    current_column_ = 1;
    
    while (current_pos_ < source_code_.length()) {
        Token token = getNextToken();
        if (token.type != TokenType::WHITESPACE && 
            token.type != TokenType::COMMENT) {
            tokens.push_back(token);
        }
        
        if (token.type == TokenType::EOF_TOKEN) {
            break;
        }
    }
    
    return tokens;
}

Token CHTLCompiler::getNextToken() {
    skipWhitespace();
    
    if (current_pos_ >= source_code_.length()) {
        return Token(TokenType::EOF_TOKEN, "", current_line_, current_column_);
    }
    
    char ch = source_code_[current_pos_];
    size_t start_line = current_line_;
    size_t start_column = current_column_;
    
    // 注释
    if (ch == '/' && current_pos_ + 1 < source_code_.length()) {
        if (source_code_[current_pos_ + 1] == '/' || 
            source_code_[current_pos_ + 1] == '*') {
            return readComment();
        }
    }
    
    // 字符串
    if (ch == '"' || ch == '\'') {
        return readString();
    }
    
    // 数字
    if (std::isdigit(ch)) {
        return readNumber();
    }
    
    // 标识符或关键字
    if (std::isalpha(ch) || ch == '_' || ch == '@' || ch == '[') {
        return readIdentifier();
    }
    
    // 单字符操作符
    Token token;
    token.line = start_line;
    token.column = start_column;
    
    switch (ch) {
        case '{':
            token.type = TokenType::LBRACE;
            token.value = "{";
            break;
        case '}':
            token.type = TokenType::RBRACE;
            token.value = "}";
            break;
        case '(':
            token.type = TokenType::LPAREN;
            token.value = "(";
            break;
        case ')':
            token.type = TokenType::RPAREN;
            token.value = ")";
            break;
        case '[':
            token.type = TokenType::LBRACKET;
            token.value = "[";
            break;
        case ']':
            token.type = TokenType::RBRACKET;
            token.value = "]";
            break;
        case ';':
            token.type = TokenType::SEMICOLON;
            token.value = ";";
            break;
        case ':':
            token.type = TokenType::COLON;
            token.value = ":";
            break;
        case '=':
            token.type = TokenType::EQUALS;
            token.value = "=";
            break;
        case ',':
            token.type = TokenType::COMMA;
            token.value = ",";
            break;
        case '.':
            token.type = TokenType::DOT;
            token.value = ".";
            break;
        case '&':
            token.type = TokenType::AMPERSAND;
            token.value = "&";
            break;
        default:
            // 无引号字面量
            return readIdentifier();
    }
    
    current_pos_++;
    current_column_++;
    return token;
}

void CHTLCompiler::skipWhitespace() {
    while (current_pos_ < source_code_.length() && 
           std::isspace(source_code_[current_pos_])) {
        if (source_code_[current_pos_] == '\n') {
            current_line_++;
            current_column_ = 1;
        } else {
            current_column_++;
        }
        current_pos_++;
    }
}

Token CHTLCompiler::readIdentifier() {
    size_t start_pos = current_pos_;
    size_t start_line = current_line_;
    size_t start_column = current_column_;
    
    // 处理[Template], [Custom]等
    if (source_code_[current_pos_] == '[') {
        while (current_pos_ < source_code_.length() && 
               source_code_[current_pos_] != ']') {
            current_pos_++;
            current_column_++;
        }
        if (current_pos_ < source_code_.length()) {
            current_pos_++; // 跳过]
            current_column_++;
        }
    }
    // 处理@Style, @Element等
    else if (source_code_[current_pos_] == '@') {
        current_pos_++;
        current_column_++;
        while (current_pos_ < source_code_.length() && 
               (std::isalnum(source_code_[current_pos_]) || 
                source_code_[current_pos_] == '_')) {
            current_pos_++;
            current_column_++;
        }
    }
    // 普通标识符
    else {
        while (current_pos_ < source_code_.length() && 
               (std::isalnum(source_code_[current_pos_]) || 
                source_code_[current_pos_] == '_' ||
                source_code_[current_pos_] == '-')) {
            current_pos_++;
            current_column_++;
        }
    }
    
    std::string value = source_code_.substr(start_pos, current_pos_ - start_pos);
    Token token(TokenType::IDENTIFIER, value, start_line, start_column);
    
    // 检查关键字
    if (value == "text") token.type = TokenType::TEXT;
    else if (value == "style") token.type = TokenType::STYLE;
    else if (value == "script") token.type = TokenType::SCRIPT;
    else if (value == "[Template]") token.type = TokenType::TEMPLATE;
    else if (value == "[Custom]") token.type = TokenType::CUSTOM;
    else if (value == "[Origin]") token.type = TokenType::ORIGIN;
    else if (value == "[Import]") token.type = TokenType::IMPORT;
    else if (value == "[Namespace]") token.type = TokenType::NAMESPACE;
    else if (value == "[Configuration]") token.type = TokenType::CONFIGURATION;
    else if (value == "@Style") token.type = TokenType::AT_STYLE;
    else if (value == "@Element") token.type = TokenType::AT_ELEMENT;
    else if (value == "@Var") token.type = TokenType::AT_VAR;
    
    return token;
}

Token CHTLCompiler::readString() {
    size_t start_line = current_line_;
    size_t start_column = current_column_;
    char delimiter = source_code_[current_pos_];
    current_pos_++; // 跳过开始引号
    current_column_++;
    
    std::string value;
    while (current_pos_ < source_code_.length() && 
           source_code_[current_pos_] != delimiter) {
        if (source_code_[current_pos_] == '\\' && 
            current_pos_ + 1 < source_code_.length()) {
            // 转义字符
            current_pos_++;
            current_column_++;
        }
        value += source_code_[current_pos_];
        if (source_code_[current_pos_] == '\n') {
            current_line_++;
            current_column_ = 1;
        } else {
            current_column_++;
        }
        current_pos_++;
    }
    
    if (current_pos_ < source_code_.length()) {
        current_pos_++; // 跳过结束引号
        current_column_++;
    }
    
    return Token(TokenType::STRING_LITERAL, value, start_line, start_column);
}

Token CHTLCompiler::readNumber() {
    size_t start_pos = current_pos_;
    size_t start_line = current_line_;
    size_t start_column = current_column_;
    
    while (current_pos_ < source_code_.length() && 
           (std::isdigit(source_code_[current_pos_]) || 
            source_code_[current_pos_] == '.')) {
        current_pos_++;
        current_column_++;
    }
    
    std::string value = source_code_.substr(start_pos, current_pos_ - start_pos);
    return Token(TokenType::NUMBER, value, start_line, start_column);
}

Token CHTLCompiler::readComment() {
    size_t start_line = current_line_;
    size_t start_column = current_column_;
    std::string value;
    
    if (source_code_[current_pos_ + 1] == '/') {
        // 单行注释
        current_pos_ += 2;
        current_column_ += 2;
        while (current_pos_ < source_code_.length() && 
               source_code_[current_pos_] != '\n') {
            value += source_code_[current_pos_];
            current_pos_++;
            current_column_++;
        }
    } else if (source_code_[current_pos_ + 1] == '*') {
        // 多行注释
        current_pos_ += 2;
        current_column_ += 2;
        while (current_pos_ + 1 < source_code_.length() && 
               !(source_code_[current_pos_] == '*' && 
                 source_code_[current_pos_ + 1] == '/')) {
            value += source_code_[current_pos_];
            if (source_code_[current_pos_] == '\n') {
                current_line_++;
                current_column_ = 1;
            } else {
                current_column_++;
            }
            current_pos_++;
        }
        current_pos_ += 2; // 跳过*/
        current_column_ += 2;
    }
    
    return Token(TokenType::COMMENT, value, start_line, start_column);
}

std::string CHTLCompiler::parseAndGenerate(const std::vector<Token>& tokens) {
    tokens_ = tokens;
    token_index_ = 0;
    if (!tokens_.empty()) {
        current_token_ = tokens_[0];
    }
    
    // 主解析循环
    while (token_index_ < tokens_.size() && 
           current_token_.type != TokenType::EOF_TOKEN) {
        parseElement();
    }
    
    // 组合最终输出
    std::string final_output;
    
    // 添加全局样式
    if (!global_styles_.empty()) {
        final_output += "<style>\n" + global_styles_ + "</style>\n";
    }
    
    // 添加主体内容
    final_output += output_;
    
    // 添加全局脚本
    if (!global_scripts_.empty()) {
        final_output += "<script>\n" + global_scripts_ + "</script>\n";
    }
    
    return final_output;
}

void CHTLCompiler::parseElement() {
    if (current_token_.type == TokenType::IDENTIFIER) {
        // HTML元素
        std::string tag_name = current_token_.value;
        element_stack_.push(tag_name);
        
        output_ += "<" + tag_name;
        
        consumeToken(TokenType::IDENTIFIER);
        
        // 检查是否有属性或内容
        if (current_token_.type == TokenType::LBRACE) {
            consumeToken(TokenType::LBRACE);
            
            // 处理属性和子元素
            while (current_token_.type != TokenType::RBRACE) {
                if (current_token_.type == TokenType::IDENTIFIER) {
                    // 检查是否是属性或子元素
                    std::string next_value = current_token_.value;
                    size_t save_index = token_index_;
                    
                    consumeToken(TokenType::IDENTIFIER);
                    
                    if (current_token_.type == TokenType::COLON || 
                        current_token_.type == TokenType::EQUALS) {
                        // 是属性
                        token_index_ = save_index;
                        current_token_ = tokens_[token_index_];
                        parseAttributes();
                    } else if (current_token_.type == TokenType::LBRACE) {
                        // 是子元素
                        token_index_ = save_index;
                        current_token_ = tokens_[token_index_];
                        output_ += ">";
                        parseElement();
                    }
                } else if (current_token_.type == TokenType::TEXT) {
                    output_ += ">";
                    parseText();
                } else if (current_token_.type == TokenType::STYLE) {
                    parseStyle();
                } else if (current_token_.type == TokenType::SCRIPT) {
                    parseScript();
                } else {
                    // 跳过不认识的token
                    token_index_++;
                    if (token_index_ < tokens_.size()) {
                        current_token_ = tokens_[token_index_];
                    }
                }
            }
            
            consumeToken(TokenType::RBRACE);
        }
        
        // 关闭标签
        if (output_.back() != '>') {
            output_ += ">";
        }
        output_ += "</" + tag_name + ">";
        
        element_stack_.pop();
        
    } else if (current_token_.type == TokenType::TEMPLATE) {
        parseTemplate();
    } else if (current_token_.type == TokenType::CUSTOM) {
        parseCustom();
    } else {
        // 跳过不认识的token
        token_index_++;
        if (token_index_ < tokens_.size()) {
            current_token_ = tokens_[token_index_];
        }
    }
}

void CHTLCompiler::parseAttributes() {
    while (current_token_.type == TokenType::IDENTIFIER) {
        std::string attr_name = current_token_.value;
        consumeToken(TokenType::IDENTIFIER);
        
        if (current_token_.type == TokenType::COLON || 
            current_token_.type == TokenType::EQUALS) {
            consumeToken(current_token_.type);
            
            std::string attr_value;
            if (current_token_.type == TokenType::STRING_LITERAL) {
                attr_value = current_token_.value;
                consumeToken(TokenType::STRING_LITERAL);
            } else if (current_token_.type == TokenType::IDENTIFIER ||
                      current_token_.type == TokenType::NUMBER) {
                attr_value = current_token_.value;
                consumeToken(current_token_.type);
            }
            
            output_ += " " + attr_name + "=\"" + escapeHtml(attr_value) + "\"";
            
            if (current_token_.type == TokenType::SEMICOLON) {
                consumeToken(TokenType::SEMICOLON);
            }
        }
    }
}

void CHTLCompiler::parseStyle() {
    consumeToken(TokenType::STYLE);
    consumeToken(TokenType::LBRACE);
    
    std::string style_content;
    int brace_count = 1;
    
    // 收集style块内容
    while (brace_count > 0 && token_index_ < tokens_.size()) {
        if (current_token_.type == TokenType::LBRACE) {
            brace_count++;
        } else if (current_token_.type == TokenType::RBRACE) {
            brace_count--;
            if (brace_count == 0) break;
        }
        
        style_content += current_token_.value + " ";
        token_index_++;
        if (token_index_ < tokens_.size()) {
            current_token_ = tokens_[token_index_];
        }
    }
    
    consumeToken(TokenType::RBRACE);
    
    // 处理局部样式
    processLocalStyle(style_content);
}

void CHTLCompiler::processLocalStyle(const std::string& style_content) {
    // 简化的样式处理
    // 实际实现需要更复杂的CSS解析
    
    // 检查是否包含类选择器
    if (style_content.find(".") != std::string::npos) {
        // 自动生成类名并添加到当前元素
        std::string class_name = generateUniqueClassName();
        
        // 将类选择器样式添加到全局样式
        std::string processed_style = style_content;
        size_t pos = processed_style.find(".");
        if (pos != std::string::npos) {
            processed_style.insert(pos + 1, class_name + " ");
        }
        
        global_styles_ += processed_style + "\n";
        
        // 添加class属性到当前元素
        // 这里简化处理，实际需要在适当位置插入
        current_element_class_ = class_name;
    } else {
        // 内联样式
        output_ += " style=\"" + style_content + "\"";
    }
}

std::string CHTLCompiler::generateUniqueClassName() {
    std::string class_name = "chtl-auto-" + std::to_string(class_counter_++);
    generated_classes_.insert(class_name);
    return class_name;
}

void CHTLCompiler::parseScript() {
    consumeToken(TokenType::SCRIPT);
    consumeToken(TokenType::LBRACE);
    
    std::string script_content;
    int brace_count = 1;
    
    // 收集script块内容
    while (brace_count > 0 && token_index_ < tokens_.size()) {
        if (current_token_.type == TokenType::LBRACE) {
            brace_count++;
        } else if (current_token_.type == TokenType::RBRACE) {
            brace_count--;
            if (brace_count == 0) break;
        }
        
        script_content += current_token_.value + " ";
        token_index_++;
        if (token_index_ < tokens_.size()) {
            current_token_ = tokens_[token_index_];
        }
    }
    
    consumeToken(TokenType::RBRACE);
    
    // 将脚本添加到全局脚本
    global_scripts_ += script_content + "\n";
}

void CHTLCompiler::parseText() {
    consumeToken(TokenType::TEXT);
    consumeToken(TokenType::LBRACE);
    
    std::string text_content;
    
    // 收集文本内容
    while (current_token_.type != TokenType::RBRACE) {
        if (current_token_.type == TokenType::STRING_LITERAL) {
            text_content += current_token_.value;
        } else if (current_token_.type == TokenType::IDENTIFIER) {
            text_content += current_token_.value;
        }
        
        token_index_++;
        if (token_index_ < tokens_.size()) {
            current_token_ = tokens_[token_index_];
        }
    }
    
    consumeToken(TokenType::RBRACE);
    
    output_ += escapeHtml(text_content);
}

void CHTLCompiler::parseTemplate() {
    // 简化的模板处理
    consumeToken(TokenType::TEMPLATE);
    
    // 跳过模板定义
    int brace_count = 0;
    while (token_index_ < tokens_.size()) {
        if (current_token_.type == TokenType::LBRACE) {
            brace_count++;
        } else if (current_token_.type == TokenType::RBRACE) {
            brace_count--;
            if (brace_count == 0) {
                token_index_++;
                if (token_index_ < tokens_.size()) {
                    current_token_ = tokens_[token_index_];
                }
                break;
            }
        }
        token_index_++;
        if (token_index_ < tokens_.size()) {
            current_token_ = tokens_[token_index_];
        }
    }
}

void CHTLCompiler::parseCustom() {
    // 简化的自定义处理
    consumeToken(TokenType::CUSTOM);
    
    // 跳过自定义定义
    int brace_count = 0;
    while (token_index_ < tokens_.size()) {
        if (current_token_.type == TokenType::LBRACE) {
            brace_count++;
        } else if (current_token_.type == TokenType::RBRACE) {
            brace_count--;
            if (brace_count == 0) {
                token_index_++;
                if (token_index_ < tokens_.size()) {
                    current_token_ = tokens_[token_index_];
                }
                break;
            }
        }
        token_index_++;
        if (token_index_ < tokens_.size()) {
            current_token_ = tokens_[token_index_];
        }
    }
}

void CHTLCompiler::parseOrigin() {
    // Origin块处理会在Scanner中完成，这里不需要处理
}

bool CHTLCompiler::expectToken(TokenType type) {
    return current_token_.type == type;
}

bool CHTLCompiler::matchToken(TokenType type) {
    if (current_token_.type == type) {
        token_index_++;
        if (token_index_ < tokens_.size()) {
            current_token_ = tokens_[token_index_];
        }
        return true;
    }
    return false;
}

void CHTLCompiler::consumeToken(TokenType type) {
    if (current_token_.type != type) {
        throw std::runtime_error("Expected token type " + 
                               std::to_string(static_cast<int>(type)) + 
                               " but got " + 
                               std::to_string(static_cast<int>(current_token_.type)));
    }
    token_index_++;
    if (token_index_ < tokens_.size()) {
        current_token_ = tokens_[token_index_];
    }
}

std::string CHTLCompiler::escapeHtml(const std::string& str) {
    std::string result;
    for (char ch : str) {
        switch (ch) {
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '&': result += "&amp;"; break;
            case '"': result += "&quot;"; break;
            case '\'': result += "&#39;"; break;
            default: result += ch; break;
        }
    }
    return result;
}

} // namespace chtl
#include "compilers/chtl_compiler.h"
#include <sstream>
#include <stack>
#include <regex>
#include <algorithm>
#include <cctype>
#include <unordered_set>

namespace chtl {

// CHTL词法单元类型
enum class TokenType {
    // 字面量
    IDENTIFIER,
    STRING_LITERAL,
    NUMBER_LITERAL,
    
    // 关键字
    KEYWORD_TEXT,
    KEYWORD_STYLE,
    KEYWORD_SCRIPT,
    KEYWORD_TEMPLATE,
    KEYWORD_CUSTOM,
    KEYWORD_ORIGIN,
    KEYWORD_CONFIGURATION,
    KEYWORD_IMPORT,
    KEYWORD_NAMESPACE,
    KEYWORD_INHERIT,
    KEYWORD_DELETE,
    KEYWORD_INSERT,
    KEYWORD_EXCEPT,
    
    // 特殊标记
    AT_STYLE,
    AT_ELEMENT,
    AT_VAR,
    
    // 符号
    LBRACE,         // {
    RBRACE,         // }
    LBRACKET,       // [
    RBRACKET,       // ]
    LPAREN,         // (
    RPAREN,         // )
    SEMICOLON,      // ;
    COLON,          // :
    COMMA,          // ,
    DOT,            // .
    EQUALS,         // =
    AMPERSAND,      // &
    
    // 其他
    COMMENT,
    WHITESPACE,
    NEWLINE,
    EOF_TOKEN,
    UNKNOWN
};

// 词法单元
struct Token {
    TokenType type;
    std::string value;
    size_t line;
    size_t column;
    
    Token(TokenType t = TokenType::UNKNOWN, const std::string& v = "", 
          size_t l = 0, size_t c = 0)
        : type(t), value(v), line(l), column(c) {}
};

// CHTL解析器状态
struct ParserState {
    // 当前元素上下文
    struct ElementContext {
        std::string name;
        std::string id;
        std::vector<std::string> classes;
        bool hasLocalStyle = false;
        bool hasLocalScript = false;
    };
    
    std::stack<ElementContext> elementStack;
    
    // 模板定义
    std::unordered_map<std::string, CHTLCompiler::TemplateInfo> templates;
    
    // 生成的全局样式
    std::stringstream globalStyles;
    
    // 自动生成的类名/ID计数
    int autoClassCounter = 0;
    int autoIdCounter = 0;
    
    // 配置
    CHTLCompiler::CHTLConfig config;
};

// CHTL编译器实现类
class CHTLCompiler::Impl {
public:
    Impl() {
        initializeKeywords();
    }
    
    bool compile(const std::string& input, 
                std::string& output,
                std::vector<BaseCompiler::CompileError>& errors) {
        // 重置状态
        reset();
        errors_ = &errors;
        
        try {
            // 词法分析
            std::vector<Token> tokens = tokenize(input);
            
            // 语法分析和代码生成
            output = parse(tokens);
            
            // 添加生成的全局样式到输出
            if (!state_.globalStyles.str().empty()) {
                output = "<style>\n" + state_.globalStyles.str() + "</style>\n" + output;
            }
            
            return errors.empty();
            
        } catch (const std::exception& e) {
            addError("编译错误: " + std::string(e.what()), currentLine_, currentColumn_);
            return false;
        }
    }
    
    void setOptions(const BaseCompiler::CompileOptions& options) {
        options_ = options;
    }
    
    bool validate(const std::string& input) const {
        // 简单验证：检查大括号是否匹配
        int braceCount = 0;
        for (char c : input) {
            if (c == '{') braceCount++;
            else if (c == '}') braceCount--;
            if (braceCount < 0) return false;
        }
        return braceCount == 0;
    }
    
    void setCHTLConfig(const CHTLConfig& config) {
        state_.config = config;
    }
    
    std::string getGeneratedGlobalStyles() const {
        return state_.globalStyles.str();
    }
    
    std::vector<CHTLCompiler::TemplateInfo> getTemplates() const {
        std::vector<CHTLCompiler::TemplateInfo> result;
        for (const auto& [name, info] : state_.templates) {
            result.push_back(info);
        }
        return result;
    }

private:
    // 初始化关键字
    void initializeKeywords() {
        keywords_ = {
            {"text", TokenType::KEYWORD_TEXT},
            {"style", TokenType::KEYWORD_STYLE},
            {"script", TokenType::KEYWORD_SCRIPT},
            {"[Template]", TokenType::KEYWORD_TEMPLATE},
            {"[Custom]", TokenType::KEYWORD_CUSTOM},
            {"[Origin]", TokenType::KEYWORD_ORIGIN},
            {"[Configuration]", TokenType::KEYWORD_CONFIGURATION},
            {"[Import]", TokenType::KEYWORD_IMPORT},
            {"[Namespace]", TokenType::KEYWORD_NAMESPACE},
            {"inherit", TokenType::KEYWORD_INHERIT},
            {"delete", TokenType::KEYWORD_DELETE},
            {"insert", TokenType::KEYWORD_INSERT},
            {"except", TokenType::KEYWORD_EXCEPT},
            {"@Style", TokenType::AT_STYLE},
            {"@Element", TokenType::AT_ELEMENT},
            {"@Var", TokenType::AT_VAR}
        };
    }
    
    // 重置编译器状态
    void reset() {
        state_ = ParserState();
        state_.config = chtlConfig_;
        currentLine_ = 1;
        currentColumn_ = 1;
        pos_ = 0;
    }
    
    // 词法分析
    std::vector<Token> tokenize(const std::string& input) {
        std::vector<Token> tokens;
        input_ = input;
        pos_ = 0;
        currentLine_ = 1;
        currentColumn_ = 1;
        
        while (pos_ < input_.length()) {
            Token token = nextToken();
            if (token.type != TokenType::WHITESPACE && 
                token.type != TokenType::COMMENT) {
                tokens.push_back(token);
            }
        }
        
        tokens.push_back(Token(TokenType::EOF_TOKEN, "", currentLine_, currentColumn_));
        return tokens;
    }
    
    // 获取下一个词法单元
    Token nextToken() {
        skipWhitespace();
        
        if (pos_ >= input_.length()) {
            return Token(TokenType::EOF_TOKEN, "", currentLine_, currentColumn_);
        }
        
        size_t startLine = currentLine_;
        size_t startColumn = currentColumn_;
        char ch = peek();
        
        // 注释
        if (ch == '/' && peek(1) == '/') {
            return scanComment();
        }
        
        // 字符串字面量
        if (ch == '"' || ch == '\'') {
            return scanString();
        }
        
        // 符号
        switch (ch) {
            case '{': advance(); return Token(TokenType::LBRACE, "{", startLine, startColumn);
            case '}': advance(); return Token(TokenType::RBRACE, "}", startLine, startColumn);
            case '[': advance(); return Token(TokenType::LBRACKET, "[", startLine, startColumn);
            case ']': advance(); return Token(TokenType::RBRACKET, "]", startLine, startColumn);
            case '(': advance(); return Token(TokenType::LPAREN, "(", startLine, startColumn);
            case ')': advance(); return Token(TokenType::RPAREN, ")", startLine, startColumn);
            case ';': advance(); return Token(TokenType::SEMICOLON, ";", startLine, startColumn);
            case ':': advance(); return Token(TokenType::COLON, ":", startLine, startColumn);
            case ',': advance(); return Token(TokenType::COMMA, ",", startLine, startColumn);
            case '.': advance(); return Token(TokenType::DOT, ".", startLine, startColumn);
            case '=': advance(); return Token(TokenType::EQUALS, "=", startLine, startColumn);
            case '&': advance(); return Token(TokenType::AMPERSAND, "&", startLine, startColumn);
        }
        
        // 标识符或关键字
        if (std::isalpha(ch) || ch == '_' || ch == '@' || ch == '[') {
            return scanIdentifierOrKeyword();
        }
        
        // 数字
        if (std::isdigit(ch)) {
            return scanNumber();
        }
        
        // 未知字符
        advance();
        return Token(TokenType::UNKNOWN, std::string(1, ch), startLine, startColumn);
    }
    
    // 语法分析
    std::string parse(const std::vector<Token>& tokens) {
        tokens_ = tokens;
        tokenPos_ = 0;
        
        std::stringstream output;
        
        while (!isAtEnd()) {
            parseTopLevel(output);
        }
        
        return output.str();
    }
    
    // 解析顶层元素
    void parseTopLevel(std::stringstream& output) {
        Token current = peekToken();
        
        switch (current.type) {
            case TokenType::KEYWORD_TEMPLATE:
                parseTemplate();
                break;
                
            case TokenType::KEYWORD_CUSTOM:
                parseCustom();
                break;
                
            case TokenType::KEYWORD_CONFIGURATION:
                parseConfiguration();
                break;
                
            case TokenType::IDENTIFIER:
                parseElement(output);
                break;
                
            case TokenType::KEYWORD_TEXT:
                parseTextNode(output);
                break;
                
            default:
                advanceToken();
                break;
        }
    }
    
    // 解析元素
    void parseElement(std::stringstream& output) {
        Token elementName = consume(TokenType::IDENTIFIER, "期望元素名称");
        
        ParserState::ElementContext context;
        context.name = elementName.value;
        state_.elementStack.push(context);
        
        output << "<" << elementName.value;
        
        consume(TokenType::LBRACE, "期望 '{'");
        
        // 解析属性和子元素
        while (!check(TokenType::RBRACE) && !isAtEnd()) {
            if (check(TokenType::IDENTIFIER)) {
                Token next = peekToken(1);
                if (next.type == TokenType::COLON || next.type == TokenType::EQUALS) {
                    // 属性
                    parseAttribute(output);
                } else if (next.type == TokenType::LBRACE) {
                    // 子元素
                    output << ">";
                    parseElement(output);
                    continue;
                } else {
                    // 可能是标签名
                    output << ">";
                    parseElement(output);
                    continue;
                }
            } else if (check(TokenType::KEYWORD_STYLE)) {
                // 局部样式块
                output << ">";
                parseLocalStyle(output);
            } else if (check(TokenType::KEYWORD_SCRIPT)) {
                // 局部脚本块
                output << ">";
                parseLocalScript(output);
            } else if (check(TokenType::KEYWORD_TEXT)) {
                // 文本节点
                output << ">";
                parseTextNode(output);
            } else {
                advanceToken();
            }
        }
        
        consume(TokenType::RBRACE, "期望 '}'");
        
        // 如果元素有内容，添加闭合标签
        if (output.str().back() == '>') {
            output << "</" << elementName.value << ">";
        } else {
            output << ">";
            output << "</" << elementName.value << ">";
        }
        
        state_.elementStack.pop();
    }
    
    // 解析属性
    void parseAttribute(std::stringstream& output) {
        Token attrName = consume(TokenType::IDENTIFIER, "期望属性名");
        
        // 消费 : 或 =
        if (check(TokenType::COLON)) {
            advance();
        } else if (check(TokenType::EQUALS)) {
            advance();
        }
        
        // 属性值
        std::string value;
        if (check(TokenType::STRING_LITERAL)) {
            value = consume(TokenType::STRING_LITERAL, "").value;
        } else if (check(TokenType::IDENTIFIER) || check(TokenType::NUMBER_LITERAL)) {
            value = advanceToken().value;
        }
        
        // 特殊处理 class 和 id
        if (attrName.value == "class" && !state_.elementStack.empty()) {
            state_.elementStack.top().classes.push_back(value);
        } else if (attrName.value == "id" && !state_.elementStack.empty()) {
            state_.elementStack.top().id = value;
        }
        
        output << " " << attrName.value << "=\"" << value << "\"";
        
        consume(TokenType::SEMICOLON, "期望 ';'");
    }
    
    // 解析局部样式块
    void parseLocalStyle(std::stringstream& output) {
        consume(TokenType::KEYWORD_STYLE, "期望 'style'");
        consume(TokenType::LBRACE, "期望 '{'");
        
        if (!state_.elementStack.empty()) {
            state_.elementStack.top().hasLocalStyle = true;
        }
        
        std::stringstream inlineStyles;
        std::stringstream globalStyles;
        
        while (!check(TokenType::RBRACE) && !isAtEnd()) {
            if (check(TokenType::DOT)) {
                // 类选择器 - 自动添加类名
                parseAutoClass(globalStyles);
            } else if (check(TokenType::IDENTIFIER)) {
                // 检查是否是CSS属性（后面跟着:）
                Token next = peekToken(1);
                if (next.type == TokenType::COLON) {
                    // CSS属性
                    parseCSSProperty(inlineStyles);
                } else {
                    advanceToken();
                }
            } else if (check(TokenType::AMPERSAND)) {
                // & 选择器
                parseAmpersandSelector(globalStyles);
            } else {
                advanceToken();
            }
        }
        
        consume(TokenType::RBRACE, "期望 '}'");
        
        // 添加内联样式
        if (!inlineStyles.str().empty()) {
            output << " style=\"" << inlineStyles.str() << "\"";
        }
        
        // 添加到全局样式
        if (!globalStyles.str().empty()) {
            state_.globalStyles << globalStyles.str() << "\n";
        }
    }
    
    // 解析CSS属性
    void parseCSSProperty(std::stringstream& output) {
        Token property = consume(TokenType::IDENTIFIER, "期望CSS属性名");
        consume(TokenType::COLON, "期望 ':'");
        
        std::string value;
        while (!check(TokenType::SEMICOLON) && !isAtEnd()) {
            value += advanceToken().value + " ";
        }
        
        // 移除尾部空格
        if (!value.empty() && value.back() == ' ') {
            value.pop_back();
        }
        
        consume(TokenType::SEMICOLON, "期望 ';'");
        
        output << property.value << ": " << value << "; ";
    }
    
    // 解析自动类选择器
    void parseAutoClass(std::stringstream& output) {
        consume(TokenType::DOT, "期望 '.'");
        Token className = consume(TokenType::IDENTIFIER, "期望类名");
        
        // 自动添加类名到当前元素
        if (!state_.elementStack.empty()) {
            state_.elementStack.top().classes.push_back(className.value);
        }
        
        // 生成CSS规则
        output << "." << className.value << " ";
        consume(TokenType::LBRACE, "期望 '{'");
        
        output << "{\n";
        while (!check(TokenType::RBRACE) && !isAtEnd()) {
            parseCSSProperty(output);
            output << "\n";
        }
        consume(TokenType::RBRACE, "期望 '}'");
        output << "}\n";
    }
    
    // 解析文本节点
    void parseTextNode(std::stringstream& output) {
        consume(TokenType::KEYWORD_TEXT, "期望 'text'");
        consume(TokenType::LBRACE, "期望 '{'");
        
        if (check(TokenType::STRING_LITERAL)) {
            output << consume(TokenType::STRING_LITERAL, "").value;
        } else {
            // 无引号文本
            while (!check(TokenType::RBRACE) && !isAtEnd()) {
                output << advanceToken().value << " ";
            }
        }
        
        consume(TokenType::RBRACE, "期望 '}'");
    }
    
    // 解析模板
    void parseTemplate() {
        consume(TokenType::KEYWORD_TEMPLATE, "期望 '[Template]'");
        
        // 模板类型
        Token templateType = advanceToken();
        std::string type = templateType.value;
        
        // 模板名称
        Token templateName = consume(TokenType::IDENTIFIER, "期望模板名称");
        
        CHTLCompiler::TemplateInfo info;
        info.name = templateName.value;
        info.type = type;
        
        consume(TokenType::LBRACE, "期望 '{'");
        
        // 收集模板内容
        std::stringstream content;
        int braceCount = 1;
        while (braceCount > 0 && !isAtEnd()) {
            Token t = advanceToken();
            if (t.type == TokenType::LBRACE) braceCount++;
            else if (t.type == TokenType::RBRACE) braceCount--;
            if (braceCount > 0) {
                content << t.value << " ";
            }
        }
        
        info.content = content.str();
        state_.templates[info.name] = info;
    }
    
    // 辅助函数
    char peek(size_t offset = 0) const {
        size_t index = pos_ + offset;
        return (index < input_.length()) ? input_[index] : '\0';
    }
    
    void advance() {
        if (pos_ < input_.length()) {
            if (input_[pos_] == '\n') {
                currentLine_++;
                currentColumn_ = 1;
            } else {
                currentColumn_++;
            }
            pos_++;
        }
    }
    
    void skipWhitespace() {
        while (pos_ < input_.length() && std::isspace(input_[pos_])) {
            advance();
        }
    }
    
    // Token辅助函数
    Token peekToken(size_t offset = 0) const {
        size_t index = tokenPos_ + offset;
        return (index < tokens_.size()) ? tokens_[index] : Token(TokenType::EOF_TOKEN);
    }
    
    Token advanceToken() {
        if (!isAtEnd()) tokenPos_++;
        return previous();
    }
    
    Token previous() const {
        return tokens_[tokenPos_ - 1];
    }
    
    bool check(TokenType type) const {
        if (isAtEnd()) return false;
        return peekToken().type == type;
    }
    
    bool isAtEnd() const {
        return peekToken().type == TokenType::EOF_TOKEN;
    }
    
    Token consume(TokenType type, const std::string& message) {
        if (check(type)) return advanceToken();
        
        addError(message, peekToken().line, peekToken().column);
        throw std::runtime_error(message);
    }
    
    // 错误处理
    void addError(const std::string& message, size_t line, size_t column) {
        BaseCompiler::CompileError error;
        error.severity = BaseCompiler::CompileError::ERROR;
        error.message = message;
        error.line = line;
        error.column = column;
        if (errors_) {
            errors_->push_back(error);
        }
    }
    
    // 扫描函数（简化版本）
    Token scanComment() {
        size_t startLine = currentLine_;
        size_t startColumn = currentColumn_;
        std::string value = "//";
        advance(); advance();
        
        while (peek() != '\n' && pos_ < input_.length()) {
            value += peek();
            advance();
        }
        
        return Token(TokenType::COMMENT, value, startLine, startColumn);
    }
    
    Token scanString() {
        size_t startLine = currentLine_;
        size_t startColumn = currentColumn_;
        char quote = peek();
        std::string value;
        
        advance(); // 跳过引号
        
        while (peek() != quote && pos_ < input_.length()) {
            if (peek() == '\\') {
                advance();
                if (pos_ < input_.length()) {
                    value += peek();
                    advance();
                }
            } else {
                value += peek();
                advance();
            }
        }
        
        if (peek() == quote) {
            advance(); // 跳过结束引号
        }
        
        return Token(TokenType::STRING_LITERAL, value, startLine, startColumn);
    }
    
    Token scanIdentifierOrKeyword() {
        size_t startLine = currentLine_;
        size_t startColumn = currentColumn_;
        std::string value;
        
        // 特殊处理 [ 开头的关键字
        if (peek() == '[') {
            while (pos_ < input_.length() && peek() != ']') {
                value += peek();
                advance();
            }
            if (peek() == ']') {
                value += peek();
                advance();
            }
        } else {
            while (pos_ < input_.length() && 
                   (std::isalnum(peek()) || peek() == '_' || peek() == '@' || peek() == '-')) {
                value += peek();
                advance();
            }
        }
        
        // 检查是否是关键字
        auto it = keywords_.find(value);
        if (it != keywords_.end()) {
            return Token(it->second, value, startLine, startColumn);
        }
        
        return Token(TokenType::IDENTIFIER, value, startLine, startColumn);
    }
    
    Token scanNumber() {
        size_t startLine = currentLine_;
        size_t startColumn = currentColumn_;
        std::string value;
        
        while (pos_ < input_.length() && std::isdigit(peek())) {
            value += peek();
            advance();
        }
        
        if (peek() == '.' && std::isdigit(peek(1))) {
            value += peek();
            advance();
            while (pos_ < input_.length() && std::isdigit(peek())) {
                value += peek();
                advance();
            }
        }
        
        return Token(TokenType::NUMBER_LITERAL, value, startLine, startColumn);
    }
    
    void parseCustom() {
        // 简化实现
        consume(TokenType::KEYWORD_CUSTOM, "");
        // ... 更多实现
    }
    
    void parseConfiguration() {
        // 简化实现
        consume(TokenType::KEYWORD_CONFIGURATION, "");
        // ... 更多实现
    }
    
    void parseLocalScript(std::stringstream& output) {
        // 简化实现 - 实际应该将script内容传递给CHTL JS编译器
        consume(TokenType::KEYWORD_SCRIPT, "");
        consume(TokenType::LBRACE, "");
        
        output << "<script>";
        int braceCount = 1;
        while (braceCount > 0 && !isAtEnd()) {
            Token t = advanceToken();
            if (t.type == TokenType::LBRACE) braceCount++;
            else if (t.type == TokenType::RBRACE) braceCount--;
            if (braceCount > 0) {
                output << t.value << " ";
            }
        }
        output << "</script>";
    }
    
    void parseAmpersandSelector(std::stringstream& output) {
        // 简化实现 - & 选择器处理
        consume(TokenType::AMPERSAND, "");
        // ... 更多实现
    }

private:
    BaseCompiler::CompileOptions options_;
    CHTLCompiler::CHTLConfig chtlConfig_;
    ParserState state_;
    
    // 词法分析状态
    std::string input_;
    size_t pos_;
    size_t currentLine_;
    size_t currentColumn_;
    
    // 语法分析状态
    std::vector<Token> tokens_;
    size_t tokenPos_;
    
    // 错误收集
    std::vector<BaseCompiler::CompileError>* errors_;
    
    // 关键字表
    std::unordered_map<std::string, TokenType> keywords_;
};

// CHTLCompiler 实现
CHTLCompiler::CHTLCompiler() 
    : pImpl(std::make_unique<Impl>()) {}

CHTLCompiler::~CHTLCompiler() = default;

bool CHTLCompiler::compile(const std::string& input, 
                          std::string& output,
                          std::vector<CompileError>& errors) {
    return pImpl->compile(input, output, errors);
}

void CHTLCompiler::setOptions(const CompileOptions& options) {
    pImpl->setOptions(options);
}

bool CHTLCompiler::validate(const std::string& input) const {
    return pImpl->validate(input);
}

void CHTLCompiler::setCHTLConfig(const CHTLConfig& config) {
    pImpl->setCHTLConfig(config);
}

std::string CHTLCompiler::getGeneratedGlobalStyles() const {
    return pImpl->getGeneratedGlobalStyles();
}

std::vector<CHTLCompiler::TemplateInfo> CHTLCompiler::getTemplates() const {
    return pImpl->getTemplates();
}

} // namespace chtl
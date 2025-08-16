#include "parser/CHTLParser.h"
#include <chrono>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>

namespace chtl {

// 实现类
class CHTLParser::Impl {
public:
    ParserConfig config;
    ParserState current_state;
    std::vector<ParserError> errors;
    std::vector<std::string> context_stack;
    std::vector<Token> tokens;
    size_t current_token_index;
    size_t nesting_depth;
    size_t error_count;
    size_t recovery_count;
    std::chrono::steady_clock::time_point parse_start_time;
    GlobalMap global_map;
    
    // 解析统计
    size_t total_tokens;
    size_t total_nodes;
    
    Impl(const ParserConfig& cfg) : 
        config(cfg), current_state(ParserState::INITIAL), current_token_index(0),
        nesting_depth(0), error_count(0), recovery_count(0), total_tokens(0), total_nodes(0) {}
    
    ~Impl() = default;
    
    // 核心解析方法
    std::unique_ptr<ASTNode> parseTokens();
    std::unique_ptr<ASTNode> parseRoot();
    std::unique_ptr<ASTNode> parseElement();
    std::unique_ptr<ASTNode> parseAttribute();
    std::unique_ptr<ASTNode> parseStyleBlock();
    std::unique_ptr<ASTNode> parseTemplateDeclaration();
    std::unique_ptr<ASTNode> parseCustomDeclaration();
    std::unique_ptr<ASTNode> parseOriginDeclaration();
    std::unique_ptr<ASTNode> parseImportDeclaration();
    std::unique_ptr<ASTNode> parseNamespaceDeclaration();
    std::unique_ptr<ASTNode> parseConstraintDeclaration();
    std::unique_ptr<ASTNode> parseScriptBlock();
    
    // 辅助解析方法
    std::unique_ptr<ASTNode> parseComment();
    std::unique_ptr<ASTNode> parseTextNode();
    std::unique_ptr<ASTNode> parseLiteral();
    std::unique_ptr<ASTNode> parseCSSRule();
    std::unique_ptr<ASTNode> parseCSSSelector();
    std::unique_ptr<ASTNode> parseCSSProperty();
    std::unique_ptr<ASTNode> parseTemplateUsage();
    std::unique_ptr<ASTNode> parseCustomSpecialization();
    
    // 错误恢复方法
    bool tryToRecover();
    void skipToNextSafePoint();
    void skipToNextBrace();
    void skipToNextSemicolon();
    
    // 辅助方法
    bool isElementStart(const Token& token) const;
    bool isAttributeStart(const Token& token) const;
    bool isStyleBlockStart(const Token& token) const;
    bool isTemplateStart(const Token& token) const;
    bool isCustomStart(const Token& token) const;
    bool isOriginStart(const Token& token) const;
    bool isImportStart(const Token& token) const;
    bool isNamespaceStart(const Token& token) const;
    bool isConstraintStart(const Token& token) const;
    bool isScriptStart(const Token& token) const;
    
    // 上下文管理
    void pushContext(const std::string& context_name);
    void popContext();
    std::string getCurrentContext() const;
    bool isInContext(const std::string& context_name) const;
    
    // 符号表管理
    void addSymbol(const SymbolInfo& symbol);
    SymbolInfo* findSymbol(const std::string& name, const std::string& namespace_path = "");
    
    // 验证方法
    bool validateElement(const std::unique_ptr<ASTNode>& element) const;
    bool validateAttribute(const std::unique_ptr<ASTNode>& attribute) const;
    bool validateStyleBlock(const std::unique_ptr<ASTNode>& style_block) const;
    bool validateTemplateDeclaration(const std::unique_ptr<ASTNode>& template_decl) const;
    bool validateCustomDeclaration(const std::unique_ptr<ASTNode>& custom_decl) const;
    
    // 工具方法
    Token getCurrentToken() const;
    Token getNextToken();
    bool hasMoreTokens() const;
    void advanceToken();
    bool matchToken(TokenType expected_type);
    bool matchToken(const std::string& expected_value);
    void consumeToken(TokenType expected_type);
    void consumeToken(const std::string& expected_value);
    std::string getTokenValue(const Token& token) const;
    bool isEndOfFile() const;
    bool isEndOfLine() const;
    void skipWhitespace();
    void skipComments();
};

// CHTLParser实现
CHTLParser::CHTLParser(const ParserConfig& config) : pImpl(std::make_unique<Impl>(config)) {}

CHTLParser::~CHTLParser() = default;

std::unique_ptr<ASTNode> CHTLParser::parse(const std::vector<Token>& tokens) {
    pImpl->tokens = tokens;
    pImpl->current_token_index = 0;
    pImpl->total_tokens = tokens.size();
    pImpl->parse_start_time = std::chrono::steady_clock::now();
    
    try {
        auto result = pImpl->parseTokens();
        pImpl->total_nodes = result ? result->getTotalNodeCount() : 0;
        return result;
    } catch (const std::exception& e) {
        addError(ParserErrorType::SYNTAX_ERROR, "解析异常: " + std::string(e.what()), 0, 0, 0, false);
        return nullptr;
    }
}

std::unique_ptr<ASTNode> CHTLParser::parseFile(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        addError(ParserErrorType::LEXICAL_ERROR, "无法打开文件: " + file_path, 0, 0, 0, false);
        return nullptr;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return parseString(buffer.str());
}

std::unique_ptr<ASTNode> CHTLParser::parseString(const std::string& source_code) {
    // 这里应该调用词法分析器来生成tokens
    // 暂时返回nullptr，等待词法分析器集成
    addError(ParserErrorType::LEXICAL_ERROR, "词法分析器尚未集成", 0, 0, 0, false);
    return nullptr;
}

ParserState CHTLParser::getCurrentState() const {
    return pImpl->current_state;
}

void CHTLParser::setState(ParserState new_state) {
    pImpl->current_state = new_state;
}

bool CHTLParser::isInState(ParserState state) const {
    return pImpl->current_state == state;
}

bool CHTLParser::hasErrors() const {
    return !pImpl->errors.empty();
}

std::vector<ParserError> CHTLParser::getErrors() const {
    return pImpl->errors;
}

void CHTLParser::clearErrors() {
    pImpl->errors.clear();
    pImpl->error_count = 0;
}

void CHTLParser::addError(ParserErrorType type, const std::string& message, 
                          size_t line, size_t column, size_t position, bool recoverable) {
    if (pImpl->error_count < pImpl->config.max_error_count) {
        pImpl->errors.emplace_back(type, message, line, column, position, "", recoverable);
        pImpl->error_count++;
    }
}

CHTLParser::ParseStats CHTLParser::getStats() const {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - pImpl->parse_start_time);
    
    return ParseStats{
        pImpl->total_tokens,
        pImpl->total_nodes,
        pImpl->error_count,
        pImpl->recovery_count,
        static_cast<size_t>(duration.count())
    };
}

void CHTLParser::setConfig(const ParserConfig& config) {
    pImpl->config = config;
}

ParserConfig CHTLParser::getConfig() const {
    return pImpl->config;
}

std::string CHTLParser::getDebugInfo() const {
    std::ostringstream oss;
    oss << "CHTL解析器调试信息:\n";
    oss << "当前状态: " << static_cast<int>(pImpl->current_state) << "\n";
    oss << "当前上下文: " << pImpl->getCurrentContext() << "\n";
    oss << "嵌套深度: " << pImpl->nesting_depth << "\n";
    oss << "当前Token索引: " << pImpl->current_token_index << "\n";
    oss << "总Token数: " << pImpl->total_tokens << "\n";
    oss << "错误数量: " << pImpl->error_count << "\n";
    oss << "恢复次数: " << pImpl->recovery_count << "\n";
    return oss.str();
}

void CHTLParser::enableDebugMode(bool enable) {
    pImpl->config.enable_debug = enable;
}

// Impl类方法实现
std::unique_ptr<ASTNode> CHTLParser::Impl::parseTokens() {
    if (tokens.empty()) {
        return nullptr;
    }
    
    return parseRoot();
}

std::unique_ptr<ASTNode> CHTLParser::Impl::parseRoot() {
    auto root = std::make_unique<ASTNode>(ASTNodeType::ROOT, 1, 1, 0, 0);
    root->generateNodeId();
    
    while (hasMoreTokens()) {
        auto token = getCurrentToken();
        
        if (isElementStart(token)) {
            auto element = parseElement();
            if (element) {
                root->addChild(std::move(element));
            }
        } else if (isTemplateStart(token)) {
            auto template_decl = parseTemplateDeclaration();
            if (template_decl) {
                root->addChild(std::move(template_decl));
            }
        } else if (isCustomStart(token)) {
            auto custom_decl = parseCustomDeclaration();
            if (custom_decl) {
                root->addChild(std::move(custom_decl));
            }
        } else if (isOriginStart(token)) {
            auto origin_decl = parseOriginDeclaration();
            if (origin_decl) {
                root->addChild(std::move(origin_decl));
            }
        } else if (isImportStart(token)) {
            auto import_decl = parseImportDeclaration();
            if (import_decl) {
                root->addChild(std::move(import_decl));
            }
        } else if (isNamespaceStart(token)) {
            auto namespace_decl = parseNamespaceDeclaration();
            if (namespace_decl) {
                root->addChild(std::move(namespace_decl));
            }
        } else if (isConstraintStart(token)) {
            auto constraint_decl = parseConstraintDeclaration();
            if (constraint_decl) {
                root->addChild(std::move(constraint_decl));
            }
        } else if (token.type == TokenType::COMMENT) {
            auto comment = parseComment();
            if (comment) {
                root->addChild(std::move(comment));
            }
        } else {
            // 未知token，尝试错误恢复
            if (!tryToRecover()) {
                break;
            }
        }
    }
    
    return root;
}

std::unique_ptr<ASTNode> CHTLParser::Impl::parseElement() {
    auto token = getCurrentToken();
    if (!isElementStart(token)) {
        return nullptr;
    }
    
    auto element = std::make_unique<HTMLElementNode>(getTokenValue(token), false, 
                                                    token.line, token.column, token.position, 0);
    element->generateNodeId();
    
    advanceToken();
    
    // 解析属性
    while (hasMoreTokens() && isAttributeStart(getCurrentToken())) {
        auto attribute = parseAttribute();
        if (attribute) {
            element->addAttribute(std::move(attribute));
        }
    }
    
    // 检查是否有子元素或自闭合
    if (hasMoreTokens()) {
        auto next_token = getCurrentToken();
        if (next_token.type == TokenType::LEFT_BRACE) {
            // 有子元素
            consumeToken(TokenType::LEFT_BRACE);
            nesting_depth++;
            
            while (hasMoreTokens() && getCurrentToken().type != TokenType::RIGHT_BRACE) {
                if (isElementStart(getCurrentToken())) {
                    auto child = parseElement();
                    if (child) {
                        element->addChild(std::move(child));
                    }
                } else if (isStyleBlockStart(getCurrentToken())) {
                    auto style_block = parseStyleBlock();
                    if (style_block) {
                        element->addChild(std::move(style_block));
                    }
                } else if (isScriptStart(getCurrentToken())) {
                    auto script_block = parseScriptBlock();
                    if (script_block) {
                        element->addChild(std::move(script_block));
                    }
                } else if (getCurrentToken().type == TokenType::COMMENT) {
                    auto comment = parseComment();
                    if (comment) {
                        element->addChild(std::move(comment));
                    }
                } else {
                    // 尝试解析文本节点
                    auto text_node = parseTextNode();
                    if (text_node) {
                        element->addChild(std::move(text_node));
                    } else {
                        // 错误恢复
                        if (!tryToRecover()) {
                            break;
                        }
                    }
                }
            }
            
            if (hasMoreTokens() && getCurrentToken().type == TokenType::RIGHT_BRACE) {
                consumeToken(TokenType::RIGHT_BRACE);
                nesting_depth--;
            }
        }
    }
    
    return element;
}

std::unique_ptr<ASTNode> CHTLParser::Impl::parseAttribute() {
    auto token = getCurrentToken();
    if (!isAttributeStart(token)) {
        return nullptr;
    }
    
    std::string attr_name = getTokenValue(token);
    advanceToken();
    
    // 检查分隔符（: 或 =）
    std::string separator = ":";
    if (hasMoreTokens()) {
        auto sep_token = getCurrentToken();
        if (sep_token.type == TokenType::COLON || sep_token.type == TokenType::EQUAL) {
            separator = getTokenValue(sep_token);
            advanceToken();
        }
    }
    
    // 解析属性值
    std::unique_ptr<ASTNode> attr_value = nullptr;
    if (hasMoreTokens()) {
        attr_value = parseLiteral();
    }
    
    // 检查分号
    if (hasMoreTokens() && getCurrentToken().type == TokenType::SEMICOLON) {
        advanceToken();
    }
    
    auto attribute = std::make_unique<AttributeNode>(attr_name, std::move(attr_value), separator,
                                                    token.line, token.column, token.position, 0);
    attribute->generateNodeId();
    
    return attribute;
}

std::unique_ptr<ASTNode> CHTLParser::Impl::parseStyleBlock() {
    auto token = getCurrentToken();
    if (!isStyleBlockStart(token)) {
        return nullptr;
    }
    
    auto style_block = std::make_unique<StyleBlockNode>(token.line, token.column, token.position, 0);
    style_block->generateNodeId();
    
    advanceToken();
    
    // 解析样式块内容
    if (hasMoreTokens() && getCurrentToken().type == TokenType::LEFT_BRACE) {
        consumeToken(TokenType::LEFT_BRACE);
        nesting_depth++;
        
        while (hasMoreTokens() && getCurrentToken().type != TokenType::RIGHT_BRACE) {
            if (getCurrentToken().type == TokenType::CSS_SELECTOR) {
                auto css_rule = parseCSSRule();
                if (css_rule) {
                    style_block->addCSSRule(std::move(css_rule));
                }
            } else if (getCurrentToken().type == TokenType::CSS_PROPERTY) {
                // 内联样式
                auto css_prop = parseCSSProperty();
                if (css_prop) {
                    style_block->addInlineStyle(std::move(css_prop));
                }
            } else {
                // 错误恢复
                if (!tryToRecover()) {
                    break;
                }
            }
        }
        
        if (hasMoreTokens() && getCurrentToken().type == TokenType::RIGHT_BRACE) {
            consumeToken(TokenType::RIGHT_BRACE);
            nesting_depth--;
        }
    }
    
    return style_block;
}

// 其他解析方法的实现...
std::unique_ptr<ASTNode> CHTLParser::Impl::parseTemplateDeclaration() {
    // 模板声明解析实现
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::Impl::parseCustomDeclaration() {
    // 自定义声明解析实现
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::Impl::parseOriginDeclaration() {
    // 原始嵌入声明解析实现
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::Impl::parseImportDeclaration() {
    // 导入声明解析实现
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::Impl::parseNamespaceDeclaration() {
    // 命名空间声明解析实现
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::Impl::parseConstraintDeclaration() {
    // 约束声明解析实现
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::Impl::parseScriptBlock() {
    // 脚本块解析实现
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::Impl::parseComment() {
    // 注释解析实现
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::Impl::parseTextNode() {
    // 文本节点解析实现
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::Impl::parseLiteral() {
    // 字面量解析实现
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::Impl::parseCSSRule() {
    // CSS规则解析实现
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::Impl::parseCSSSelector() {
    // CSS选择器解析实现
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::Impl::parseCSSProperty() {
    // CSS属性解析实现
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::Impl::parseTemplateUsage() {
    // 模板使用解析实现
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::Impl::parseCustomSpecialization() {
    // 自定义特例化解析实现
    return nullptr;
}

// 错误恢复方法
bool CHTLParser::Impl::tryToRecover() {
    if (!config.enable_error_recovery) {
        return false;
    }
    
    recovery_count++;
    skipToNextSafePoint();
    return hasMoreTokens();
}

void CHTLParser::Impl::skipToNextSafePoint() {
    while (hasMoreTokens()) {
        auto token = getCurrentToken();
        if (token.type == TokenType::RIGHT_BRACE || 
            token.type == TokenType::SEMICOLON ||
            isElementStart(token) ||
            isTemplateStart(token) ||
            isCustomStart(token)) {
            break;
        }
        advanceToken();
    }
}

void CHTLParser::Impl::skipToNextBrace() {
    while (hasMoreTokens() && getCurrentToken().type != TokenType::RIGHT_BRACE) {
        advanceToken();
    }
}

void CHTLParser::Impl::skipToNextSemicolon() {
    while (hasMoreTokens() && getCurrentToken().type != TokenType::SEMICOLON) {
        advanceToken();
    }
}

// 辅助方法实现
bool CHTLParser::Impl::isElementStart(const Token& token) const {
    return token.type == TokenType::IDENTIFIER && 
           !isTemplateStart(token) && 
           !isCustomStart(token) && 
           !isOriginStart(token) && 
           !isImportStart(token) && 
           !isNamespaceStart(token) && 
           !isConstraintStart(token);
}

bool CHTLParser::Impl::isAttributeStart(const Token& token) const {
    return token.type == TokenType::IDENTIFIER;
}

bool CHTLParser::Impl::isStyleBlockStart(const Token& token) const {
    return token.type == TokenType::STYLE_KEYWORD;
}

bool CHTLParser::Impl::isTemplateStart(const Token& token) const {
    return token.type == TokenType::TEMPLATE_KEYWORD;
}

bool CHTLParser::Impl::isCustomStart(const Token& token) const {
    return token.type == TokenType::CUSTOM_KEYWORD;
}

bool CHTLParser::Impl::isOriginStart(const Token& token) const {
    return token.type == TokenType::ORIGIN_KEYWORD;
}

bool CHTLParser::Impl::isImportStart(const Token& token) const {
    return token.type == TokenType::IMPORT_KEYWORD;
}

bool CHTLParser::Impl::isNamespaceStart(const Token& token) const {
    return token.type == TokenType::NAMESPACE_KEYWORD;
}

bool CHTLParser::Impl::isConstraintStart(const Token& token) const {
    return token.type == TokenType::EXCEPT_KEYWORD;
}

bool CHTLParser::Impl::isScriptStart(const Token& token) const {
    return token.type == TokenType::SCRIPT_KEYWORD;
}

// 上下文管理
void CHTLParser::Impl::pushContext(const std::string& context_name) {
    context_stack.push_back(context_name);
}

void CHTLParser::Impl::popContext() {
    if (!context_stack.empty()) {
        context_stack.pop_back();
    }
}

std::string CHTLParser::Impl::getCurrentContext() const {
    if (context_stack.empty()) {
        return "global";
    }
    return context_stack.back();
}

bool CHTLParser::Impl::isInContext(const std::string& context_name) const {
    return std::find(context_stack.begin(), context_stack.end(), context_name) != context_stack.end();
}

// 符号表管理
void CHTLParser::Impl::addSymbol(const SymbolInfo& symbol) {
    global_map.addSymbol(symbol);
}

SymbolInfo* CHTLParser::Impl::findSymbol(const std::string& name, const std::string& namespace_path) {
    return global_map.findSymbol(name, namespace_path);
}

// 验证方法
bool CHTLParser::Impl::validateElement(const std::unique_ptr<ASTNode>& element) const {
    return element && element->type == ASTNodeType::HTML_ELEMENT;
}

bool CHTLParser::Impl::validateAttribute(const std::unique_ptr<ASTNode>& attribute) const {
    return attribute && attribute->type == ASTNodeType::ATTRIBUTE;
}

bool CHTLParser::Impl::validateStyleBlock(const std::unique_ptr<ASTNode>& style_block) const {
    return style_block && style_block->type == ASTNodeType::STYLE_BLOCK;
}

bool CHTLParser::Impl::validateTemplateDeclaration(const std::unique_ptr<ASTNode>& template_decl) const {
    return template_decl && template_decl->type == ASTNodeType::TEMPLATE_DECLARATION;
}

bool CHTLParser::Impl::validateCustomDeclaration(const std::unique_ptr<ASTNode>& custom_decl) const {
    return custom_decl && custom_decl->type == ASTNodeType::CUSTOM_DECLARATION;
}

// 工具方法
Token CHTLParser::Impl::getCurrentToken() const {
    if (current_token_index < tokens.size()) {
        return tokens[current_token_index];
    }
    return Token{TokenType::UNKNOWN, "", 0, 0, 0};
}

Token CHTLParser::Impl::getNextToken() {
    if (current_token_index + 1 < tokens.size()) {
        return tokens[current_token_index + 1];
    }
    return Token{TokenType::UNKNOWN, "", 0, 0, 0};
}

bool CHTLParser::Impl::hasMoreTokens() const {
    return current_token_index < tokens.size();
}

void CHTLParser::Impl::advanceToken() {
    if (current_token_index < tokens.size()) {
        current_token_index++;
    }
}

bool CHTLParser::Impl::matchToken(TokenType expected_type) {
    return hasMoreTokens() && getCurrentToken().type == expected_type;
}

bool CHTLParser::Impl::matchToken(const std::string& expected_value) {
    return hasMoreTokens() && getTokenValue(getCurrentToken()) == expected_value;
}

void CHTLParser::Impl::consumeToken(TokenType expected_type) {
    if (matchToken(expected_type)) {
        advanceToken();
    }
}

void CHTLParser::Impl::consumeToken(const std::string& expected_value) {
    if (matchToken(expected_value)) {
        advanceToken();
    }
}

std::string CHTLParser::Impl::getTokenValue(const Token& token) const {
    return token.value;
}

bool CHTLParser::Impl::isEndOfFile() const {
    return current_token_index >= tokens.size();
}

bool CHTLParser::Impl::isEndOfLine() const {
    return hasMoreTokens() && getCurrentToken().type == TokenType::NEWLINE;
}

void CHTLParser::Impl::skipWhitespace() {
    while (hasMoreTokens() && getCurrentToken().type == TokenType::WHITESPACE) {
        advanceToken();
    }
}

void CHTLParser::Impl::skipComments() {
    while (hasMoreTokens() && getCurrentToken().type == TokenType::COMMENT) {
        advanceToken();
    }
}

} // namespace chtl
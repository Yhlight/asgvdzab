#include "parser/BasicSyntaxParser.h"
#include "ast/CommentNode.h"
#include "ast/TextNode.h"
#include "ast/LiteralNode.h"
#include "ast/HTMLElementNode.h"
#include "ast/AttributeNode.h"
#include "ast/DocumentNode.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cctype>

namespace chtl {

// 实现类
class BasicSyntaxParser::Impl {
public:
    BasicSyntaxParserConfig config;
    std::vector<std::string> errors;
    std::vector<std::string> validation_errors;
    bool debug_mode;
    
    Impl(const BasicSyntaxParserConfig& cfg) : config(cfg), debug_mode(false) {}
    
    ~Impl() = default;
    
    // 内部方法实现
    bool isCommentToken(const Token& token);
    bool isTextNodeStart(const std::vector<Token>& tokens, size_t position);
    bool isHTMLElementStart(const std::vector<Token>& tokens, size_t position);
    bool isAttributeDeclaration(const std::vector<Token>& tokens, size_t position);
    std::string extractCommentContent(const std::vector<Token>& tokens, size_t& position);
    std::string extractTextContent(const std::vector<Token>& tokens, size_t& position);
    std::string extractLiteralContent(const std::vector<Token>& tokens, size_t& position);
    std::string extractElementName(const std::vector<Token>& tokens, size_t& position);
    std::pair<std::string, std::string> extractAttribute(const std::vector<Token>& tokens, size_t& position);
    void skipWhitespace(const std::vector<Token>& tokens, size_t& position);
    bool isEndOfTokens(const std::vector<Token>& tokens, size_t position);
    void addValidationError(const std::string& error_message);
    
    // 辅助方法
    bool isValidHTMLElementName(const std::string& name);
    bool isValidAttributeName(const std::string& name);
    std::string normalizeAttributeValue(const std::string& value);
    void logDebug(const std::string& message);
};

// BasicSyntaxParser实现
BasicSyntaxParser::BasicSyntaxParser(const BasicSyntaxParserConfig& config) 
    : pImpl(std::make_unique<Impl>(config)) {}

BasicSyntaxParser::~BasicSyntaxParser() = default;

std::shared_ptr<ASTNode> BasicSyntaxParser::parseComment(const std::vector<Token>& tokens, size_t& position) {
    if (position >= tokens.size()) {
        addError("位置超出token范围");
        return nullptr;
    }
    
    const Token& token = tokens[position];
    
    if (!pImpl->isCommentToken(token)) {
        addError("不是注释token: " + token.value);
        return nullptr;
    }
    
    std::string comment_content = pImpl->extractCommentContent(tokens, position);
    
    // 根据注释类型创建相应的AST节点
    ASTNodeType node_type;
    if (token.value == "//") {
        node_type = ASTNodeType::COMMENT_SINGLE_LINE;
    } else if (token.value == "/*") {
        node_type = ASTNodeType::COMMENT_MULTI_LINE;
    } else if (token.value == "--") {
        node_type = ASTNodeType::COMMENT_GENERATOR;
    } else {
        addError("未知的注释类型: " + token.value);
        return nullptr;
    }
    
    auto comment_node = std::make_shared<CommentNode>(comment_content, node_type);
    comment_node->line = token.line;
    comment_node->column = token.column;
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("解析注释: " + comment_content);
    }
    
    return comment_node;
}

std::shared_ptr<ASTNode> BasicSyntaxParser::parseTextNode(const std::vector<Token>& tokens, size_t& position) {
    if (!pImpl->isTextNodeStart(tokens, position)) {
        addError("不是文本节点开始");
        return nullptr;
    }
    
    // 跳过 "text" 关键字
    position++;
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查是否有左大括号
    if (position >= tokens.size() || tokens[position].value != "{") {
        addError("文本节点缺少左大括号");
        return nullptr;
    }
    position++;
    
    // 提取文本内容
    std::string text_content = pImpl->extractTextContent(tokens, position);
    
    // 检查是否有右大括号
    if (position >= tokens.size() || tokens[position].value != "}") {
        addError("文本节点缺少右大括号");
        return nullptr;
    }
    position++;
    
    auto text_node = std::make_shared<TextNode>(text_content);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("解析文本节点: " + text_content);
    }
    
    return text_node;
}

std::shared_ptr<ASTNode> BasicSyntaxParser::parseLiteral(const std::vector<Token>& tokens, size_t& position) {
    if (position >= tokens.size()) {
        addError("位置超出token范围");
        return nullptr;
    }
    
    const Token& token = tokens[position];
    std::string literal_content = pImpl->extractLiteralContent(tokens, position);
    
    // 根据字面量类型创建相应的AST节点
    ASTNodeType node_type;
    if (token.value == "\"") {
        node_type = ASTNodeType::LITERAL_DOUBLE_QUOTE;
    } else if (token.value == "'") {
        node_type = ASTNodeType::LITERAL_SINGLE_QUOTE;
    } else {
        // 无修饰字面量
        if (!pImpl->config.enable_unquoted_literals) {
            addError("未启用无修饰字面量");
            return nullptr;
        }
        node_type = ASTNodeType::LITERAL_UNQUOTED;
    }
    
    auto literal_node = std::make_shared<LiteralNode>(literal_content, node_type);
    literal_node->line = token.line;
    literal_node->column = token.column;
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("解析字面量: " + literal_content);
    }
    
    return literal_node;
}

std::shared_ptr<ASTNode> BasicSyntaxParser::parseHTMLElement(const std::vector<Token>& tokens, size_t& position) {
    if (!pImpl->isHTMLElementStart(tokens, position)) {
        addError("不是HTML元素开始");
        return nullptr;
    }
    
    std::string element_name = pImpl->extractElementName(tokens, position);
    
    if (!pImpl->isValidHTMLElementName(element_name)) {
        addError("无效的HTML元素名: " + element_name);
        return nullptr;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查是否有左大括号
    if (position >= tokens.size() || tokens[position].value != "{") {
        addError("HTML元素缺少左大括号");
        return nullptr;
    }
    position++;
    
    auto element_node = std::make_shared<HTMLElementNode>(element_name);
    
    // 解析元素内容（属性、子元素等）
    while (position < tokens.size() && tokens[position].value != "}") {
        pImpl->skipWhitespace(tokens, position);
        
        if (pImpl->isAttributeDeclaration(tokens, position)) {
            auto attr_node = parseAttribute(tokens, position);
            if (attr_node) {
                element_node->addChild(attr_node);
            }
        } else if (pImpl->isHTMLElementStart(tokens, position)) {
            auto child_element = parseHTMLElement(tokens, position);
            if (child_element) {
                element_node->addChild(child_element);
            }
        } else if (pImpl->isTextNodeStart(tokens, position)) {
            auto text_node = parseTextNode(tokens, position);
            if (text_node) {
                element_node->addChild(text_node);
            }
        } else {
            addError("无法识别的元素内容: " + tokens[position].value);
            position++;
        }
    }
    
    // 检查是否有右大括号
    if (position >= tokens.size() || tokens[position].value != "}") {
        addError("HTML元素缺少右大括号");
        return nullptr;
    }
    position++;
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("解析HTML元素: " + element_name);
    }
    
    return element_node;
}

std::shared_ptr<ASTNode> BasicSyntaxParser::parseAttribute(const std::vector<Token>& tokens, size_t& position) {
    if (!pImpl->isAttributeDeclaration(tokens, position)) {
        addError("不是属性声明");
        return nullptr;
    }
    
    auto [attr_name, attr_value] = pImpl->extractAttribute(tokens, position);
    
    if (attr_name.empty()) {
        addError("属性名不能为空");
        return nullptr;
    }
    
    if (!pImpl->isValidAttributeName(attr_name)) {
        addError("无效的属性名: " + attr_name);
        return nullptr;
    }
    
    // 检查是否有分号
    if (position >= tokens.size() || tokens[position].value != ";") {
        addError("属性声明缺少分号");
        return nullptr;
    }
    position++;
    
    auto attr_node = std::make_shared<AttributeNode>(attr_name, attr_value);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("解析属性: " + attr_name + " = " + attr_value);
    }
    
    return attr_node;
}

std::vector<std::shared_ptr<ASTNode>> BasicSyntaxParser::parseBasicSyntax(const std::vector<Token>& tokens) {
    std::vector<std::shared_ptr<ASTNode>> result;
    size_t position = 0;
    
    while (position < tokens.size()) {
        pImpl->skipWhitespace(tokens, position);
        
        if (pImpl->isEndOfTokens(tokens, position)) {
            break;
        }
        
        std::shared_ptr<ASTNode> node = nullptr;
        
        if (pImpl->isCommentToken(tokens[position])) {
            node = parseComment(tokens, position);
        } else if (pImpl->isTextNodeStart(tokens, position)) {
            node = parseTextNode(tokens, position);
        } else if (pImpl->isHTMLElementStart(tokens, position)) {
            node = parseHTMLElement(tokens, position);
        } else {
            addError("无法识别的语法: " + tokens[position].value);
            position++;
            continue;
        }
        
        if (node) {
            result.push_back(node);
        }
    }
    
    return result;
}

std::shared_ptr<ASTNode> BasicSyntaxParser::parseDocument(const std::vector<Token>& tokens) {
    auto nodes = parseBasicSyntax(tokens);
    
    if (nodes.empty()) {
        addError("文档为空");
        return nullptr;
    }
    
    auto document_node = std::make_shared<DocumentNode>();
    
    for (const auto& node : nodes) {
        document_node->addChild(node);
    }
    
    return document_node;
}

bool BasicSyntaxParser::validateBasicSyntax(const std::vector<Token>& tokens) {
    pImpl->validation_errors.clear();
    
    try {
        size_t position = 0;
        
        while (position < tokens.size()) {
            pImpl->skipWhitespace(tokens, position);
            
            if (pImpl->isEndOfTokens(tokens, position)) {
                break;
            }
            
            if (pImpl->isCommentToken(tokens[position])) {
                // 验证注释
                std::string comment_content = pImpl->extractCommentContent(tokens, position);
                if (comment_content.length() > pImpl->config.max_comment_length) {
                    pImpl->addValidationError("注释长度超过限制: " + std::to_string(comment_content.length()));
                }
            } else if (pImpl->isTextNodeStart(tokens, position)) {
                // 验证文本节点
                position++; // 跳过 "text"
                pImpl->skipWhitespace(tokens, position);
                
                if (position >= tokens.size() || tokens[position].value != "{") {
                    pImpl->addValidationError("文本节点缺少左大括号");
                    return false;
                }
                position++;
                
                std::string text_content = pImpl->extractTextContent(tokens, position);
                if (text_content.length() > pImpl->config.max_text_length) {
                    pImpl->addValidationError("文本长度超过限制: " + std::to_string(text_content.length()));
                }
                
                if (position >= tokens.size() || tokens[position].value != "}") {
                    pImpl->addValidationError("文本节点缺少右大括号");
                    return false;
                }
                position++;
            } else if (pImpl->isHTMLElementStart(tokens, position)) {
                // 验证HTML元素
                std::string element_name = pImpl->extractElementName(tokens, position);
                if (!pImpl->isValidHTMLElementName(element_name)) {
                    pImpl->addValidationError("无效的HTML元素名: " + element_name);
                    return false;
                }
                
                pImpl->skipWhitespace(tokens, position);
                
                if (position >= tokens.size() || tokens[position].value != "{") {
                    pImpl->addValidationError("HTML元素缺少左大括号");
                    return false;
                }
                position++;
                
                // 递归验证元素内容
                int brace_count = 1;
                while (position < tokens.size() && brace_count > 0) {
                    if (tokens[position].value == "{") {
                        brace_count++;
                    } else if (tokens[position].value == "}") {
                        brace_count--;
                    }
                    position++;
                }
                
                if (brace_count > 0) {
                    pImpl->addValidationError("HTML元素缺少右大括号");
                    return false;
                }
            } else {
                pImpl->addValidationError("无法识别的语法: " + tokens[position].value);
                return false;
            }
        }
        
        return pImpl->validation_errors.empty();
        
    } catch (const std::exception& e) {
        pImpl->addValidationError("验证过程中发生异常: " + std::string(e.what()));
        return false;
    }
}

std::vector<std::string> BasicSyntaxParser::getValidationErrors() const {
    return pImpl->validation_errors;
}

bool BasicSyntaxParser::hasValidationErrors() const {
    return !pImpl->validation_errors.empty();
}

void BasicSyntaxParser::setConfig(const BasicSyntaxParserConfig& config) {
    pImpl->config = config;
}

BasicSyntaxParserConfig BasicSyntaxParser::getConfig() const {
    return pImpl->config;
}

bool BasicSyntaxParser::hasErrors() const {
    return !pImpl->errors.empty();
}

std::vector<std::string> BasicSyntaxParser::getErrors() const {
    return pImpl->errors;
}

void BasicSyntaxParser::clearErrors() {
    pImpl->errors.clear();
}

void BasicSyntaxParser::addError(const std::string& error_message) {
    pImpl->errors.push_back(error_message);
    
    if (pImpl->debug_mode) {
        std::cerr << "基础语法解析器错误: " << error_message << std::endl;
    }
}

std::string BasicSyntaxParser::getDebugInfo() const {
    std::ostringstream oss;
    oss << "基础语法解析器调试信息:\n";
    oss << "错误数量: " << pImpl->errors.size() << "\n";
    oss << "验证错误数量: " << pImpl->validation_errors.size() << "\n";
    oss << "CE等价性: " << (pImpl->config.enable_ce_equivalence ? "启用" : "禁用") << "\n";
    oss << "无修饰字面量: " << (pImpl->config.enable_unquoted_literals ? "启用" : "禁用") << "\n";
    oss << "生成器注释: " << (pImpl->config.enable_generator_comments ? "启用" : "禁用") << "\n";
    oss << "严格模式: " << (pImpl->config.strict_mode ? "启用" : "禁用") << "\n";
    return oss.str();
}

void BasicSyntaxParser::enableDebugMode(bool enable) {
    pImpl->debug_mode = enable;
}

// Impl类方法实现
bool BasicSyntaxParser::Impl::isCommentToken(const Token& token) {
    return token.value == "//" || token.value == "/*" || token.value == "--";
}

bool BasicSyntaxParser::Impl::isTextNodeStart(const std::vector<Token>& tokens, size_t position) {
    return position < tokens.size() && tokens[position].value == "text";
}

bool BasicSyntaxParser::Impl::isHTMLElementStart(const std::vector<Token>& tokens, size_t position) {
    if (position >= tokens.size()) {
        return false;
    }
    
    const Token& token = tokens[position];
    
    // 检查是否是有效的HTML元素名
    if (token.value.empty()) {
        return false;
    }
    
    // HTML元素名必须以字母开头
    if (!std::isalpha(token.value[0])) {
        return false;
    }
    
    // 检查是否是HTML标准元素名（简化版本）
    static const std::vector<std::string> html_elements = {
        "html", "head", "body", "div", "span", "p", "h1", "h2", "h3", "h4", "h5", "h6",
        "ul", "ol", "li", "a", "img", "table", "tr", "td", "th", "form", "input", "button",
        "textarea", "select", "option", "label", "fieldset", "legend", "section", "article",
        "aside", "nav", "header", "footer", "main", "figure", "figcaption", "blockquote",
        "code", "pre", "em", "strong", "small", "mark", "del", "ins", "sub", "sup"
    };
    
    std::string lower_name = token.value;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);
    
    return std::find(html_elements.begin(), html_elements.end(), lower_name) != html_elements.end();
}

bool BasicSyntaxParser::Impl::isAttributeDeclaration(const std::vector<Token>& tokens, size_t position) {
    if (position + 2 >= tokens.size()) {
        return false;
    }
    
    const Token& attr_name = tokens[position];
    const Token& separator = tokens[position + 1];
    const Token& attr_value = tokens[position + 2];
    
    // 检查格式：属性名 : "属性值"
    return isValidAttributeName(attr_name.value) && 
           (separator.value == ":" || (config.enable_ce_equivalence && separator.value == "=")) &&
           (attr_value.value == "\"" || attr_value.value == "'" || 
            (config.enable_unquoted_literals && !attr_value.value.empty()));
}

std::string BasicSyntaxParser::Impl::extractCommentContent(const std::vector<Token>& tokens, size_t& position) {
    std::string content;
    const Token& start_token = tokens[position];
    
    if (start_token.value == "//") {
        // 单行注释：// 到行尾
        position++;
        while (position < tokens.size() && tokens[position].value != "\n") {
            content += tokens[position].value;
            position++;
        }
    } else if (start_token.value == "/*") {
        // 多行注释：/* 到 */
        position++;
        while (position < tokens.size() && tokens[position].value != "*/") {
            content += tokens[position].value;
            position++;
        }
        if (position < tokens.size()) {
            position++; // 跳过 */
        }
    } else if (start_token.value == "--") {
        // 生成器注释：-- 到行尾
        position++;
        while (position < tokens.size() && tokens[position].value != "\n") {
            content += tokens[position].value;
            position++;
        }
    }
    
    return content;
}

std::string BasicSyntaxParser::Impl::extractTextContent(const std::vector<Token>& tokens, size_t& position) {
    std::string content;
    
    while (position < tokens.size() && tokens[position].value != "}") {
        content += tokens[position].value;
        position++;
    }
    
    return content;
}

std::string BasicSyntaxParser::Impl::extractLiteralContent(const std::vector<Token>& tokens, size_t& position) {
    std::string content;
    const Token& start_token = tokens[position];
    
    if (start_token.value == "\"" || start_token.value == "'") {
        // 引号字符串
        char quote_char = start_token.value[0];
        position++;
        
        while (position < tokens.size() && tokens[position].value != std::string(1, quote_char)) {
            content += tokens[position].value;
            position++;
        }
        
        if (position < tokens.size()) {
            position++; // 跳过结束引号
        }
    } else {
        // 无修饰字面量
        content = start_token.value;
        position++;
    }
    
    return content;
}

std::string BasicSyntaxParser::Impl::extractElementName(const std::vector<Token>& tokens, size_t& position) {
    if (position < tokens.size()) {
        return tokens[position].value;
    }
    return "";
}

std::pair<std::string, std::string> BasicSyntaxParser::Impl::extractAttribute(const std::vector<Token>& tokens, size_t& position) {
    std::string attr_name = tokens[position].value;
    position++;
    
    // 跳过分隔符（: 或 =）
    position++;
    
    std::string attr_value;
    if (position < tokens.size()) {
        if (tokens[position].value == "\"" || tokens[position].value == "'") {
            // 引号字符串
            char quote_char = tokens[position].value[0];
            position++;
            
            while (position < tokens.size() && tokens[position].value != std::string(1, quote_char)) {
                attr_value += tokens[position].value;
                position++;
            }
            
            if (position < tokens.size()) {
                position++; // 跳过结束引号
            }
        } else {
            // 无修饰字面量
            attr_value = tokens[position].value;
            position++;
        }
    }
    
    return {attr_name, attr_value};
}

void BasicSyntaxParser::Impl::skipWhitespace(const std::vector<Token>& tokens, size_t& position) {
    while (position < tokens.size() && 
           (tokens[position].value == " " || tokens[position].value == "\t" || 
            tokens[position].value == "\n" || tokens[position].value == "\r")) {
        position++;
    }
}

bool BasicSyntaxParser::Impl::isEndOfTokens(const std::vector<Token>& tokens, size_t position) {
    return position >= tokens.size();
}

void BasicSyntaxParser::Impl::addValidationError(const std::string& error_message) {
    validation_errors.push_back(error_message);
}

bool BasicSyntaxParser::Impl::isValidHTMLElementName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    // HTML元素名必须以字母开头
    if (!std::isalpha(name[0])) {
        return false;
    }
    
    // 检查是否只包含字母、数字、连字符
    for (char c : name) {
        if (!std::isalnum(c) && c != '-') {
            return false;
        }
    }
    
    return true;
}

bool BasicSyntaxParser::Impl::isValidAttributeName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    // 属性名必须以字母开头
    if (!std::isalpha(name[0])) {
        return false;
    }
    
    // 检查是否只包含字母、数字、连字符、下划线
    for (char c : name) {
        if (!std::isalnum(c) && c != '-' && c != '_') {
            return false;
        }
    }
    
    return true;
}

std::string BasicSyntaxParser::Impl::normalizeAttributeValue(const std::string& value) {
    // 移除首尾空白字符
    std::string result = value;
    result.erase(0, result.find_first_not_of(" \t\n\r"));
    result.erase(result.find_last_not_of(" \t\n\r") + 1);
    return result;
}

void BasicSyntaxParser::Impl::logDebug(const std::string& message) {
    if (debug_mode) {
        std::cout << "[DEBUG] " << message << std::endl;
    }
}

} // namespace chtl
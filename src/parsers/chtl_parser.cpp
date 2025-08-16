#include "parsers/chtl_parser.hpp"
#include "lexer/pure_chtl_tokens.hpp"
#include <stdexcept>
#include <algorithm>

namespace chtl {

// 构造函数
CHTLParser::CHTLParser(const CHTLParserConfig& config) 
    : config_(config), currentTokenIndex_(0) {}

// 解析源代码
CHTLParseResult CHTLParser::parse(const std::string& source, const std::string& filename) {
    filename_ = filename;
    errors_.clear();
    warnings_.clear();
    
    // 使用纯CHTL词法分析器进行tokenization
    PureCHTLTokenUtils tokenUtils;
    tokens_ = tokenUtils.tokenize(source);
    
    if (tokens_.empty()) {
        CHTLParseResult result;
        result.success = false;
        reportError("源代码为空或无法识别的token");
        result.errors = errors_;
        return result;
    }
    
    currentTokenIndex_ = 0;
    return parseTokens(tokens_, filename);
}

// 解析Token序列
CHTLParseResult CHTLParser::parseTokens(const std::vector<PureCHTLToken>& tokens, const std::string& filename) {
    tokens_ = tokens;
    filename_ = filename;
    currentTokenIndex_ = 0;
    errors_.clear();
    warnings_.clear();
    
    CHTLParseResult result;
    
    try {
        result.ast = parseProgram();
        result.success = errors_.empty();
        result.errors = errors_;
        result.warnings = warnings_;
    } catch (const std::exception& e) {
        reportError("解析过程中发生异常: " + std::string(e.what()));
        result.success = false;
        result.errors = errors_;
        result.warnings = warnings_;
    }
    
    return result;
}

// ===== Token管理 =====

const PureCHTLToken& CHTLParser::currentToken() const {
    if (currentTokenIndex_ >= tokens_.size()) {
        static PureCHTLToken eofToken{PureCHTLTokenType::EOF_TOKEN, "", Position{0, 0}};
        return eofToken;
    }
    return tokens_[currentTokenIndex_];
}

const PureCHTLToken& CHTLParser::peekToken(size_t offset) const {
    size_t index = currentTokenIndex_ + offset;
    if (index >= tokens_.size()) {
        static PureCHTLToken eofToken{PureCHTLTokenType::EOF_TOKEN, "", Position{0, 0}};
        return eofToken;
    }
    return tokens_[index];
}

void CHTLParser::advance() {
    if (currentTokenIndex_ < tokens_.size()) {
        currentTokenIndex_++;
    }
}

bool CHTLParser::isAtEnd() const {
    return currentTokenIndex_ >= tokens_.size() || 
           currentToken().type == PureCHTLTokenType::EOF_TOKEN;
}

bool CHTLParser::match(PureCHTLTokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool CHTLParser::check(PureCHTLTokenType type) const {
    if (isAtEnd()) return false;
    return currentToken().type == type;
}

// ===== 错误处理 =====

void CHTLParser::reportError(const std::string& message, const std::string& suggestion) {
    CHTLParseError error;
    error.message = message;
    error.position = getCurrentPosition();
    error.context = currentToken().value;
    error.suggestion = suggestion;
    errors_.push_back(error);
}

void CHTLParser::reportWarning(const std::string& message) {
    if (config_.enableWarnings) {
        warnings_.push_back(message + " at " + std::to_string(getCurrentPosition().line) + 
                          ":" + std::to_string(getCurrentPosition().column));
    }
}

void CHTLParser::synchronize() {
    advance();
    
    while (!isAtEnd()) {
        // 寻找下一个语句的开始
        if (check(PureCHTLTokenType::LEFT_BRACE) ||
            check(PureCHTLTokenType::IDENTIFIER) ||
            check(PureCHTLTokenType::KEYWORD_TEXT) ||
            check(PureCHTLTokenType::BRACKET_TEMPLATE) ||
            check(PureCHTLTokenType::BRACKET_CUSTOM) ||
            check(PureCHTLTokenType::BRACKET_ORIGIN) ||
            check(PureCHTLTokenType::BRACKET_IMPORT) ||
            check(PureCHTLTokenType::BRACKET_NAMESPACE) ||
            check(PureCHTLTokenType::BRACKET_CONFIGURATION)) {
            return;
        }
        advance();
    }
}

// ===== 主要解析方法 =====

CHTLASTNodePtr CHTLParser::parseProgram() {
    auto program = std::make_shared<ProgramNode>(getCurrentPosition());
    
    while (!isAtEnd()) {
        // 跳过注释（如果配置允许保留注释则解析）
        if (check(PureCHTLTokenType::COMMENT_LINE) || 
            check(PureCHTLTokenType::COMMENT_MULTILINE) || 
            check(PureCHTLTokenType::COMMENT_GENERATOR)) {
            if (config_.enableCommentsInAST) {
                auto comment = parseComment();
                if (comment) {
                    program->addChild(comment);
                }
            } else {
                advance(); // 跳过注释
            }
            continue;
        }
        
        auto declaration = parseTopLevelDeclaration();
        if (declaration) {
            program->addChild(declaration);
        } else {
            synchronize(); // 错误恢复
        }
    }
    
    return program;
}

CHTLASTNodePtr CHTLParser::parseTopLevelDeclaration() {
    // 解析顶级声明
    if (check(PureCHTLTokenType::BRACKET_TEMPLATE)) {
        return parseTemplateDeclaration();
    } else if (check(PureCHTLTokenType::BRACKET_CUSTOM)) {
        return parseCustomDeclaration();
    } else if (check(PureCHTLTokenType::BRACKET_ORIGIN)) {
        return parseOriginDeclaration();
    } else if (check(PureCHTLTokenType::BRACKET_IMPORT)) {
        return parseImportDeclaration();
    } else if (check(PureCHTLTokenType::BRACKET_NAMESPACE)) {
        return parseNamespaceDeclaration();
    } else if (check(PureCHTLTokenType::BRACKET_CONFIGURATION)) {
        return parseConfigurationDeclaration();
    } else if (check(PureCHTLTokenType::KEYWORD_TEXT)) {
        return parseTextNode();
    } else if (check(PureCHTLTokenType::IDENTIFIER)) {
        // 可能是HTML元素
        return parseElement();
    } else {
        reportError("期待顶级声明，但得到了'" + currentToken().value + "'", 
                   "尝试使用 [Template], [Custom], [Origin], [Import], [Namespace], [Configuration], text, 或HTML元素名");
        return nullptr;
    }
}

// ===== 模板解析 =====

CHTLASTNodePtr CHTLParser::parseTemplateDeclaration() {
    if (!match(PureCHTLTokenType::BRACKET_TEMPLATE)) {
        reportError("期待 '[Template]'");
        return nullptr;
    }
    
    // 检查模板类型
    if (check(PureCHTLTokenType::PREFIX_STYLE)) {
        return parseTemplateStyle();
    } else if (check(PureCHTLTokenType::PREFIX_ELEMENT)) {
        return parseTemplateElement();
    } else if (check(PureCHTLTokenType::PREFIX_VAR)) {
        return parseTemplateVar();
    } else {
        reportError("期待模板类型 @Style, @Element, 或 @Var");
        return nullptr;
    }
}

CHTLASTNodePtr CHTLParser::parseTemplateStyle() {
    if (!match(PureCHTLTokenType::PREFIX_STYLE)) {
        reportError("期待 '@Style'");
        return nullptr;
    }
    
    std::string templateName = parseIdentifierString();
    if (templateName.empty()) {
        reportError("期待模板名称");
        return nullptr;
    }
    
    auto templateNode = std::make_shared<TemplateDeclarationNode>("@Style", templateName, getCurrentPosition());
    
    if (!match(PureCHTLTokenType::LEFT_BRACE)) {
        reportError("期待 '{'", "模板定义需要用花括号包围");
        return nullptr;
    }
    
    // 解析样式属性
    while (!check(PureCHTLTokenType::RIGHT_BRACE) && !isAtEnd()) {
        if (check(PureCHTLTokenType::IDENTIFIER)) {
            auto property = parseInlineStyle();
            if (property) {
                templateNode->addChild(property);
            }
        } else if (check(PureCHTLTokenType::PREFIX_STYLE)) {
            // 继承其他样式模板
            auto inherit = parseInheritStatement();
            if (inherit) {
                templateNode->addChild(inherit);
            }
        } else {
            reportError("期待CSS属性或继承语句");
            advance();
        }
    }
    
    if (!match(PureCHTLTokenType::RIGHT_BRACE)) {
        reportError("期待 '}'");
        return nullptr;
    }
    
    return templateNode;
}

CHTLASTNodePtr CHTLParser::parseTemplateElement() {
    if (!match(PureCHTLTokenType::PREFIX_ELEMENT)) {
        reportError("期待 '@Element'");
        return nullptr;
    }
    
    std::string templateName = parseIdentifierString();
    if (templateName.empty()) {
        reportError("期待模板名称");
        return nullptr;
    }
    
    auto templateNode = std::make_shared<TemplateDeclarationNode>("@Element", templateName, getCurrentPosition());
    
    if (!match(PureCHTLTokenType::LEFT_BRACE)) {
        reportError("期待 '{'");
        return nullptr;
    }
    
    // 解析元素内容
    while (!check(PureCHTLTokenType::RIGHT_BRACE) && !isAtEnd()) {
        auto element = parseTopLevelDeclaration();
        if (element) {
            templateNode->addChild(element);
        } else {
            advance();
        }
    }
    
    if (!match(PureCHTLTokenType::RIGHT_BRACE)) {
        reportError("期待 '}'");
        return nullptr;
    }
    
    return templateNode;
}

CHTLASTNodePtr CHTLParser::parseTemplateVar() {
    if (!match(PureCHTLTokenType::PREFIX_VAR)) {
        reportError("期待 '@Var'");
        return nullptr;
    }
    
    std::string templateName = parseIdentifierString();
    if (templateName.empty()) {
        reportError("期待变量组名称");
        return nullptr;
    }
    
    auto templateNode = std::make_shared<TemplateDeclarationNode>("@Var", templateName, getCurrentPosition());
    
    if (!match(PureCHTLTokenType::LEFT_BRACE)) {
        reportError("期待 '{'");
        return nullptr;
    }
    
    // 解析变量定义
    while (!check(PureCHTLTokenType::RIGHT_BRACE) && !isAtEnd()) {
        if (check(PureCHTLTokenType::IDENTIFIER)) {
            std::string varName = parseIdentifierString();
            
            if (!match(PureCHTLTokenType::COLON_EQUALS)) {
                reportError("期待 ':' 或 '=' (CE对等式)", "在CHTL中 ':' 和 '=' 完全等价");
                continue;
            }
            
            std::string varValue = parseStringLiteralValue();
            
            auto varProperty = std::make_shared<VarGroupNode>(varName, varValue, getCurrentPosition());
            templateNode->addChild(varProperty);
            
            match(PureCHTLTokenType::SEMICOLON); // 可选的分号
        } else {
            reportError("期待变量名");
            advance();
        }
    }
    
    if (!match(PureCHTLTokenType::RIGHT_BRACE)) {
        reportError("期待 '}'");
        return nullptr;
    }
    
    return templateNode;
}

// ===== 元素和文本解析 =====

CHTLASTNodePtr CHTLParser::parseElement() {
    std::string elementName = parseIdentifierString();
    if (elementName.empty()) {
        reportError("期待元素名");
        return nullptr;
    }
    
    if (!isValidElementName(elementName)) {
        reportWarning("'" + elementName + "' 可能不是有效的HTML元素名");
    }
    
    auto element = std::make_shared<ElementNode>(elementName, getCurrentPosition());
    
    if (!match(PureCHTLTokenType::LEFT_BRACE)) {
        reportError("期待 '{'", "元素内容需要用花括号包围");
        return nullptr;
    }
    
    // 解析元素内容
    while (!check(PureCHTLTokenType::RIGHT_BRACE) && !isAtEnd()) {
        if (check(PureCHTLTokenType::IDENTIFIER)) {
            // 可能是属性或子元素
            const std::string& nextValue = currentToken().value;
            const PureCHTLToken& followingToken = peekToken(1);
            
            if (followingToken.type == PureCHTLTokenType::COLON_EQUALS) {
                // 这是属性
                auto attr = parseAttribute();
                if (attr) {
                    element->addAttribute(attr);
                }
            } else {
                // 这是子元素
                auto child = parseElement();
                if (child) {
                    element->addChild(child);
                }
            }
        } else if (check(PureCHTLTokenType::KEYWORD_TEXT)) {
            auto textNode = parseTextNode();
            if (textNode) {
                element->addChild(textNode);
            }
        } else if (check(PureCHTLTokenType::KEYWORD_STYLE)) {
            auto styleBlock = parseStyleBlock();
            if (styleBlock) {
                element->setStyleBlock(styleBlock);
            }
        } else if (check(PureCHTLTokenType::KEYWORD_SCRIPT)) {
            auto scriptBlock = parseScriptBlock();
            if (scriptBlock) {
                element->setScriptBlock(scriptBlock);
            }
        } else if (check(PureCHTLTokenType::PREFIX_ELEMENT)) {
            // 模板使用
            auto templateUsage = parseTemplateUsage();
            if (templateUsage) {
                element->addChild(templateUsage);
            }
        } else {
            reportError("在元素 '" + elementName + "' 中期待属性、子元素、text、style或script");
            advance();
        }
    }
    
    if (!match(PureCHTLTokenType::RIGHT_BRACE)) {
        reportError("期待 '}'");
        return nullptr;
    }
    
    return element;
}

CHTLASTNodePtr CHTLParser::parseTextNode() {
    if (!match(PureCHTLTokenType::KEYWORD_TEXT)) {
        reportError("期待 'text'");
        return nullptr;
    }
    
    if (!match(PureCHTLTokenType::LEFT_BRACE)) {
        reportError("期待 '{'", "文本内容需要用花括号包围");
        return nullptr;
    }
    
    std::string content = parseStringLiteralValue();
    
    if (!match(PureCHTLTokenType::RIGHT_BRACE)) {
        reportError("期待 '}'");
        return nullptr;
    }
    
    // 确定字面量类型
    LiteralType literalType = LiteralType::UNQUOTED;
    if (!content.empty()) {
        if (content.front() == '"' && content.back() == '"') {
            literalType = LiteralType::DOUBLE_QUOTED;
            content = content.substr(1, content.length() - 2); // 移除引号
        } else if (content.front() == '\'' && content.back() == '\'') {
            literalType = LiteralType::SINGLE_QUOTED;
            content = content.substr(1, content.length() - 2); // 移除引号
        }
    }
    
    auto textNode = std::make_shared<TextNode>(content, literalType, getCurrentPosition());
    return textNode;
}

CHTLASTNodePtr CHTLParser::parseAttribute() {
    std::string attrName = parseIdentifierString();
    if (attrName.empty()) {
        reportError("期待属性名");
        return nullptr;
    }
    
    if (!match(PureCHTLTokenType::COLON_EQUALS)) {
        reportError("期待 ':' 或 '=' (CE对等式)", "在CHTL中 ':' 和 '=' 完全等价");
        return nullptr;
    }
    
    std::string attrValue = parseStringLiteralValue();
    
    match(PureCHTLTokenType::SEMICOLON); // 可选的分号
    
    auto attribute = std::make_shared<AttributeNode>(attrName, attrValue, getCurrentPosition());
    return attribute;
}

// ===== 工具方法 =====

std::string CHTLParser::parseIdentifierString() {
    if (check(PureCHTLTokenType::IDENTIFIER)) {
        std::string value = currentToken().value;
        advance();
        return value;
    }
    return "";
}

std::string CHTLParser::parseStringLiteralValue() {
    if (check(PureCHTLTokenType::STRING_LITERAL)) {
        std::string value = currentToken().value;
        advance();
        return value;
    } else if (check(PureCHTLTokenType::IDENTIFIER)) {
        // 无修饰字面量
        std::string value = currentToken().value;
        advance();
        return value;
    }
    return "";
}

Position CHTLParser::getCurrentPosition() const {
    return currentToken().position;
}

void CHTLParser::savePosition(CHTLASTNodePtr node) const {
    if (node) {
        node->position = getCurrentPosition();
    }
}

// ===== 验证方法 =====

bool CHTLParser::isValidElementName(const std::string& name) const {
    if (name.empty()) return false;
    
    // HTML5元素名验证：只能包含字母、数字、连字符，必须以字母开头
    if (!std::isalpha(name[0])) return false;
    
    for (char c : name) {
        if (!std::isalnum(c) && c != '-') {
            return false;
        }
    }
    
    return true;
}

bool CHTLParser::isValidAttributeName(const std::string& name) const {
    if (name.empty()) return false;
    
    // HTML属性名验证
    for (char c : name) {
        if (!std::isalnum(c) && c != '-' && c != '_') {
            return false;
        }
    }
    
    return true;
}

bool CHTLParser::isValidClassName(const std::string& name) const {
    if (name.empty()) return false;
    
    // CSS类名验证
    for (char c : name) {
        if (!std::isalnum(c) && c != '-' && c != '_') {
            return false;
        }
    }
    
    return true;
}

bool CHTLParser::isValidIdName(const std::string& name) const {
    return isValidClassName(name); // ID和类名规则相同
}

bool CHTLParser::isValidNamespace(const std::string& name) const {
    return isValidClassName(name); // 命名空间规则与类名相同
}

// 其他方法的实现待完成...
// 由于篇幅限制，这里只实现了核心的解析功能

} // namespace chtl
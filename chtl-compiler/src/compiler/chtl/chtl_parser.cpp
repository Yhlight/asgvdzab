#include "compiler/chtl/chtl_parser.h"
#include <sstream>
#include <iostream>

namespace chtl {

CHTLParser::CHTLParser(CHTLLexer& lexer, GlobalMap& global_map)
    : lexer_(lexer), global_map_(global_map), state_(global_map) {
    advance(); // 读取第一个token
}

std::unique_ptr<ASTNode> CHTLParser::parse() {
    auto root = std::make_unique<RootNode>();
    
    while (!isAtEnd()) {
        try {
            auto stmt = parseTopLevel();
            if (stmt) {
                root->addChild(std::move(stmt));
            }
        } catch (const std::exception& e) {
            error(e.what());
            synchronize();
        }
    }
    
    // 将错误和警告传递给state
    for (const auto& err : errors_) {
        state_.addError(err);
    }
    for (const auto& warn : warnings_) {
        state_.addWarning(warn);
    }
    
    return root;
}

void CHTLParser::advance() {
    previous_ = current_;
    current_ = lexer_.nextToken();
    
    // 跳过注释（但保留生成器注释）
    while (current_.type == TokenType::COMMENT_SINGLE || 
           current_.type == TokenType::COMMENT_MULTI) {
        current_ = lexer_.nextToken();
    }
}

bool CHTLParser::check(TokenType type) {
    return current_.type == type;
}

bool CHTLParser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool CHTLParser::match(const std::string& value) {
    if (current_.value == value) {
        advance();
        return true;
    }
    return false;
}

Token CHTLParser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        Token token = current_;
        advance();
        return token;
    }
    
    error(message);
    throw std::runtime_error(message);
}

Token CHTLParser::consume(const std::string& value, const std::string& message) {
    if (current_.value == value) {
        Token token = current_;
        advance();
        return token;
    }
    
    error(message);
    throw std::runtime_error(message);
}

std::unique_ptr<ASTNode> CHTLParser::parseTopLevel() {
    // 处理生成器注释
    if (current_.type == TokenType::COMMENT_GENERATOR) {
        // 生成器注释需要特殊处理
        std::string comment_value = current_.value;
        advance();
        // 暂时返回空，后续可以创建专门的注释节点
        return nullptr;
    }
    
    // 特殊块
    if (isSpecialBlockStart()) {
        return parseSpecialBlock();
    }
    
    // 元素
    if (isElementStart()) {
        return parseElement();
    }
    
    // 其他语句
    return parseStatement();
}

std::unique_ptr<ASTNode> CHTLParser::parseStatement() {
    // 模板使用或原始嵌入使用
    if (current_.type == TokenType::AT) {
        // 查看@后面的标识符
        Token next = lexer_.peekToken();
        if (next.type == TokenType::IDENTIFIER) {
            if (next.value == "Element" || next.value == "Style" || next.value == "Var") {
                return parseTemplateUsage();
            } else if (next.value == "Html" || next.value == "JavaScript" || next.value == "Css") {
                // 这是原始嵌入使用
                advance(); // 跳过 @
                std::string type_name = consume(TokenType::IDENTIFIER, "Expected origin type").value;
                std::string name = consume(TokenType::IDENTIFIER, "Expected origin name").value;
                match(TokenType::SEMICOLON);
                
                auto origin = std::make_unique<OriginBlockNode>("@" + type_name, name, current_.line, current_.column);
                origin->setUsageMode(true);
                return origin;
            }
        }
    }
    
    // 自定义使用
    if (isCustomUsage()) {
        return parseCustomUsage();
    }
    
    // 特殊操作
    if (match("delete")) {
        return parseDeleteOperation();
    }
    
    if (match("insert")) {
        return parseInsertOperation();
    }
    
    if (match("inherit")) {
        return parseInheritOperation();
    }
    
    if (match("except")) {
        return parseExceptClause();
    }
    
    // 文本节点
    if (match("text")) {
        return parseTextNode();
    }
    
    // 元素
    if (isElementStart()) {
        return parseElement();
    }
    
    error("Unexpected token: " + current_.value);
    advance();
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::parseSpecialBlock() {
    if (match("[Configuration]")) {
        return parseConfiguration();
    }
    
    if (match("[Namespace]")) {
        return parseNamespace();
    }
    
    if (match("[Template]")) {
        return parseTemplate();
    }
    
    if (match("[Custom]")) {
        return parseCustom();
    }
    
    if (match("[Origin]")) {
        return parseOrigin();
    }
    
    if (match("[Import]")) {
        return parseImport();
    }
    
    error("Unknown special block: " + current_.value);
    advance();
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::parseConfiguration() {
    auto config = std::make_unique<ConfigurationBlockNode>(current_.line, current_.column);
    
    consume(TokenType::LBRACE, "Expected '{' after [Configuration]");
    
    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        if (match("[Name]")) {
            // 解析Name配置块
            consume(TokenType::LBRACE, "Expected '{' after [Name]");
            while (!check(TokenType::RBRACE) && !isAtEnd()) {
                std::string key = consume(TokenType::IDENTIFIER, "Expected configuration key").value;
                consume(TokenType::EQUALS, "Expected '=' after key");
                // TODO: 解析配置值
                advance(); // 临时跳过值
                match(TokenType::SEMICOLON);
            }
            consume(TokenType::RBRACE, "Expected '}'");
        } else if (match("[OriginType]")) {
            // 解析OriginType配置块
            consume(TokenType::LBRACE, "Expected '{' after [OriginType]");
            while (!check(TokenType::RBRACE) && !isAtEnd()) {
                advance(); // 临时跳过
            }
            consume(TokenType::RBRACE, "Expected '}'");
        } else {
            // 普通配置项
            std::string key = consume(TokenType::IDENTIFIER, "Expected configuration key").value;
            consume(TokenType::EQUALS, "Expected '=' after key");
            std::string value = parseLiteralValue();
            config->addConfig(key, value);
            match(TokenType::SEMICOLON);
        }
    }
    
    consume(TokenType::RBRACE, "Expected '}' after configuration");
    
    // 应用配置到GlobalMap
    auto& options = global_map_.getConfig();
    for (const auto& [key, value] : config->getConfigs()) {
        if (key == "INDEX_INITIAL_COUNT") {
            options.index_initial_count = std::stoi(value);
        } else if (key == "DISABLE_NAME_GROUP") {
            options.disable_name_group = (value == "true");
        } else if (key == "DISABLE_CUSTOM_ORIGIN_TYPE") {
            options.disable_custom_origin_type = (value == "true");
        } else if (key == "DEBUG_MODE") {
            options.debug_mode = (value == "true");
        }
    }
    
    return config;
}

std::unique_ptr<ASTNode> CHTLParser::parseNamespace() {
    std::string name = consume(TokenType::IDENTIFIER, "Expected namespace name").value;
    auto ns = std::make_unique<NamespaceBlockNode>(name, current_.line, current_.column);
    
    global_map_.enterNamespace(name);
    
    if (match(TokenType::LBRACE)) {
        while (!check(TokenType::RBRACE) && !isAtEnd()) {
            auto stmt = parseTopLevel();
            if (stmt) {
                ns->addChild(std::move(stmt));
            }
        }
        consume(TokenType::RBRACE, "Expected '}' after namespace");
    }
    
    global_map_.exitNamespace();
    
    return ns;
}

std::unique_ptr<ASTNode> CHTLParser::parseTemplate() {
    // 解析模板类型 @Style, @Element, @Var
    consume(TokenType::AT, "Expected '@' after [Template]");
    std::string type_name = consume(TokenType::IDENTIFIER, "Expected template type").value;
    
    TemplateType type;
    if (type_name == "Style") {
        type = TemplateType::STYLE;
    } else if (type_name == "Element") {
        type = TemplateType::ELEMENT;
    } else if (type_name == "Var") {
        type = TemplateType::VAR;
    } else {
        error("Unknown template type: " + type_name);
        return nullptr;
    }
    
    std::string name = consume(TokenType::IDENTIFIER, "Expected template name").value;
    auto tmpl = std::make_unique<TemplateDefinitionNode>(type, name, current_.line, current_.column);
    
    consume(TokenType::LBRACE, "Expected '{' after template name");
    
    // 解析模板内容
    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        if (type == TemplateType::STYLE) {
            // 样式模板内容
            auto prop = parseStyleProperty();
            if (prop) {
                tmpl->addChild(std::move(prop));
            }
        } else if (type == TemplateType::ELEMENT) {
            // 元素模板内容
            auto stmt = parseStatement();
            if (stmt) {
                tmpl->addChild(std::move(stmt));
            }
        } else if (type == TemplateType::VAR) {
            // 变量模板内容
            std::string var_name = consume(TokenType::IDENTIFIER, "Expected variable name").value;
            matchColonOrEquals();
            std::string value = parseLiteralValue();
            match(TokenType::SEMICOLON);
            
            // 创建变量属性节点
            auto var_prop = std::make_unique<VarPropertyNode>(var_name, value, current_.line, current_.column);
            tmpl->addChild(std::move(var_prop));
        }
    }
    
    consume(TokenType::RBRACE, "Expected '}' after template");
    
    // 创建模板定义的深拷贝用于GlobalMap
    auto def = std::make_unique<TemplateDefinition>();
    def->type = type;
    def->name = name;
    def->namespace_path = global_map_.getCurrentNamespace();
    
    // 创建内容的深拷贝
    auto content_copy = std::make_unique<TemplateDefinitionNode>(type, name, tmpl->getLine(), tmpl->getColumn());
    for (const auto& child : tmpl->getChildren()) {
        // 这里应该实现深拷贝，暂时只是移动子节点的指针
        // 实际使用时会从GlobalMap中获取完整定义
        content_copy->addChild(child->clone());
    }
    def->content = std::move(content_copy);
    
    global_map_.addTemplate(name, std::move(def));
    
    return tmpl;
}

std::unique_ptr<ASTNode> CHTLParser::parseElement() {
    std::string tag_name = consume(TokenType::IDENTIFIER, "Expected element name").value;
    auto element = std::make_unique<ElementNode>(tag_name, current_.line, current_.column);
    
    state_.pushElementContext(tag_name);
    state_.pushLocalStyleContext();
    
    consume(TokenType::LBRACE, "Expected '{' after element name");
    
    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        // 属性
        if (isAttributeStart()) {
            auto attr = parseAttribute();
            if (attr) {
                auto* attr_node = static_cast<AttributeNode*>(attr.get());
                element->addAttribute(attr_node->getName(), attr_node->getValue());
            }
        }
        // 局部样式块
        else if (match("style")) {
            element->has_local_style = true;
            auto style = parseStyleBlock();
            if (style) {
                element->addChild(std::move(style));
            }
        }
        // 局部脚本块
        else if (match("script")) {
            element->has_local_script = true;
            auto script = parseScriptBlock();
            if (script) {
                element->addChild(std::move(script));
            }
        }
        // 子元素或语句
        else {
            auto stmt = parseStatement();
            if (stmt) {
                element->addChild(std::move(stmt));
            }
        }
    }
    
    consume(TokenType::RBRACE, "Expected '}' after element content");
    
    // 如果有局部样式，生成类名
    if (element->has_local_style) {
        element->generated_class_name = state_.generateClassName();
        // 不要在这里添加class属性，让生成器处理
    }
    
    state_.popLocalStyleContext();
    state_.popElementContext();
    
    return element;
}

std::unique_ptr<ASTNode> CHTLParser::parseTextNode() {
    consume(TokenType::LBRACE, "Expected '{' after 'text'");
    
    std::string content;
    if (current_.type == TokenType::STRING_LITERAL) {
        content = current_.value;
        advance();
    } else {
        // 无引号字面量
        content = parseUnquotedLiteral();
    }
    
    consume(TokenType::RBRACE, "Expected '}' after text content");
    
    return std::make_unique<TextNode>(content, current_.line, current_.column);
}

std::unique_ptr<ASTNode> CHTLParser::parseAttribute() {
    std::string name = consume(TokenType::IDENTIFIER, "Expected attribute name").value;
    matchColonOrEquals(); // CE对等式
    std::string value = parseLiteralValue();
    match(TokenType::SEMICOLON);
    
    return std::make_unique<AttributeNode>(name, value, current_.line, current_.column);
}

std::unique_ptr<ASTNode> CHTLParser::parseStyleBlock() {
    auto style_block = std::make_unique<StyleBlockNode>(current_.line, current_.column);
    
    consume(TokenType::LBRACE, "Expected '{' after 'style'");
    
    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        // 检查是否是模板使用
        if (current_.type == TokenType::AT) {
            // 处理 @Style TemplateName;
            auto usage = parseTemplateUsage();
            if (usage) {
                style_block->addChild(std::move(usage));
            }
        }
        // 检查是否是选择器（.class, #id, &:hover, 元素选择器等）
        else if (current_.type == TokenType::DOT || 
            (current_.type == TokenType::IDENTIFIER && current_.value[0] == '#') ||
            current_.type == TokenType::AMPERSAND ||
            (current_.type == TokenType::IDENTIFIER && lexer_.peekToken().type == TokenType::LBRACE)) {
            
            auto rule = parseStyleRule();
            if (rule) {
                style_block->addChild(std::move(rule));
            }
        } else {
            // 内联样式属性
            auto prop = parseStyleProperty();
            if (prop) {
                style_block->addChild(std::move(prop));
                
                // 添加到局部样式上下文
                // 内联样式已经作为节点添加，状态处理在生成器中
            }
        }
    }
    
    consume(TokenType::RBRACE, "Expected '}' after style block");
    
    return style_block;
}

std::unique_ptr<ASTNode> CHTLParser::parseStyleProperty() {
    std::string property = consume(TokenType::IDENTIFIER, "Expected style property").value;
    matchColonOrEquals();
    
    std::string value = parseLiteralValue();
    
    match(TokenType::SEMICOLON);
    
    return std::make_unique<StylePropertyNode>(property, value, current_.line, current_.column);
}

bool CHTLParser::isAtEnd() {
    return current_.type == TokenType::EOF_TOKEN;
}

bool CHTLParser::isElementStart() {
    return current_.type == TokenType::IDENTIFIER && 
           getHTMLTags().count(current_.value) > 0;
}

bool CHTLParser::isAttributeStart() {
    // 属性以标识符开始，后面跟着:或=
    if (current_.type != TokenType::IDENTIFIER) return false;
    
    Token next = lexer_.peekToken();
    return next.type == TokenType::COLON || next.type == TokenType::EQUALS;
}

bool CHTLParser::isSpecialBlockStart() {
    return current_.isSpecialBlock();
}

bool CHTLParser::isTemplateUsage() {
    return current_.type == TokenType::AT &&
           lexer_.peekToken().type == TokenType::IDENTIFIER;
}

bool CHTLParser::matchColonOrEquals() {
    if (match(TokenType::COLON) || match(TokenType::EQUALS)) {
        return true;
    }
    error("Expected ':' or '='");
    return false;
}

std::string CHTLParser::parseLiteralValue() {
    if (current_.type == TokenType::STRING_LITERAL) {
        std::string value = current_.value;
        advance();
        return value;
    }
    
    return parseUnquotedLiteral();
}

std::string CHTLParser::parseUnquotedLiteral() {
    std::string value;
    
    while (!check(TokenType::SEMICOLON) && 
           !check(TokenType::RBRACE) && 
           !check(TokenType::COMMA) &&
           !isAtEnd()) {
        if (!value.empty() && current_.value.size() > 0 && isWhitespace(current_.value[0])) {
            value += " ";
            advance();
        } else {
            value += current_.value;
            advance();
        }
    }
    
    return value;
}

void CHTLParser::error(const std::string& message) {
    std::stringstream ss;
    ss << "Line " << current_.line << ", column " << current_.column << ": " << message;
    errors_.push_back(ss.str());
}

void CHTLParser::warning(const std::string& message) {
    std::stringstream ss;
    ss << "Line " << current_.line << ", column " << current_.column << ": " << message;
    warnings_.push_back(ss.str());
}

void CHTLParser::synchronize() {
    advance();
    
    while (!isAtEnd()) {
        if (previous_.type == TokenType::SEMICOLON) return;
        if (previous_.type == TokenType::RBRACE) return;
        
        if (isSpecialBlockStart() || isElementStart()) return;
        
        advance();
    }
}

bool CHTLParser::isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

// 其他方法的简单实现（需要后续完善）
std::unique_ptr<ASTNode> CHTLParser::parseCustom() {
    // TODO: 实现自定义解析
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::parseOrigin() {
    // 解析原始类型 @Html, @Style, @JavaScript或自定义
    consume(TokenType::AT, "Expected '@' after [Origin]");
    std::string origin_type = "@" + consume(TokenType::IDENTIFIER, "Expected origin type").value;
    
    // 解析原始块名称
    std::string name = consume(TokenType::IDENTIFIER, "Expected origin block name").value;
    
    auto origin = std::make_unique<OriginBlockNode>(origin_type, name, current_.line, current_.column);
    
    // 如果是声明式使用（以分号结尾）
    if (match(TokenType::SEMICOLON)) {
        origin->setUsageMode(true);
        return origin;
    }
    
    // 否则是定义，需要解析内容
    consume(TokenType::LBRACE, "Expected '{' after origin name");
    
    // 对于原始内容，我们需要读取原始文本而不是通过词法分析器
    // 保存当前位置
    size_t start_pos = lexer_.getPosition();
    
    // 跳过空白
    while (!isAtEnd() && isWhitespace(lexer_.peekChar())) {
        lexer_.skipChar();
    }
    
    // 读取原始内容直到找到行首的 }
    std::string content;
    int brace_depth = 0;
    bool at_line_start = true;
    
    while (!lexer_.isAtEnd()) {
        char c = lexer_.peekChar();
        
        if (at_line_start && c == '}' && brace_depth == 0) {
            // 找到了结束的 }
            lexer_.skipChar(); // 跳过 }
            // 重新同步词法分析器
            advance();
            break;
        }
        
        if (c == '{') {
            brace_depth++;
        } else if (c == '}') {
            brace_depth--;
        }
        
        content += c;
        lexer_.skipChar();
        
        if (c == '\n') {
            at_line_start = true;
        } else if (!isWhitespace(c)) {
            at_line_start = false;
        }
    }
    
    origin->setContent(content);
    
    // 添加到GlobalMap
    auto origin_def = std::make_unique<OriginDefinition>();
    origin_def->type = origin_type;
    origin_def->name = name;
    origin_def->content = content;
    global_map_.addOrigin(name + "_" + origin_type, std::move(origin_def));
    
    return origin;
}

std::unique_ptr<ASTNode> CHTLParser::parseImport() {
    // TODO: 实现导入解析
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::parseScriptBlock() {
    // TODO: 实现脚本块解析
    consume(TokenType::LBRACE, "Expected '{' after 'script'");
    std::string content;
    // 临时实现：读取到}为止
    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        content += current_.value + " ";
        advance();
    }
    consume(TokenType::RBRACE, "Expected '}'");
    return std::make_unique<ScriptBlockNode>(content, current_.line, current_.column);
}

std::unique_ptr<ASTNode> CHTLParser::parseTemplateUsage() {
    // 解析模板使用 @Element, @Style, @Var
    advance(); // 跳过 @
    std::string type_name = consume(TokenType::IDENTIFIER, "Expected template type after @").value;
    
    TemplateType type;
    if (type_name == "Element") {
        type = TemplateType::ELEMENT;
    } else if (type_name == "Style") {
        type = TemplateType::STYLE;
    } else if (type_name == "Var") {
        type = TemplateType::VAR;
    } else {
        error("Unknown template usage type: @" + type_name);
        return nullptr;
    }
    
    // 获取模板名称
    std::string template_name = consume(TokenType::IDENTIFIER, "Expected template name").value;
    
    auto usage = std::make_unique<TemplateUsageNode>(type, template_name, current_.line, current_.column);
    
    // 对于变量组模板，可能有参数
    if (type == TemplateType::VAR && match(TokenType::DOT)) {
        // 处理 @Var TemplateName.propertyName 形式
        std::string property_name = consume(TokenType::IDENTIFIER, "Expected property name").value;
        usage->addParameter("property", property_name);
    } else if (type == TemplateType::VAR && match(TokenType::LPAREN)) {
        // 处理 @Var TemplateName(param1: value1, param2: value2) 形式
        while (!check(TokenType::RPAREN) && !isAtEnd()) {
            std::string param_name = consume(TokenType::IDENTIFIER, "Expected parameter name").value;
            matchColonOrEquals();
            std::string param_value = parseLiteralValue();
            usage->addParameter(param_name, param_value);
            
            if (!match(TokenType::COMMA)) {
                break;
            }
        }
        consume(TokenType::RPAREN, "Expected ')' after parameters");
    }
    
    // 消费结束分号
    match(TokenType::SEMICOLON);
    
    return usage;
}

std::unique_ptr<ASTNode> CHTLParser::parseCustomUsage() {
    // TODO: 实现自定义使用解析
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::parseDeleteOperation() {
    // TODO: 实现删除操作解析
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::parseInsertOperation() {
    // TODO: 实现插入操作解析
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::parseInheritOperation() {
    // TODO: 实现继承操作解析
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::parseExceptClause() {
    // TODO: 实现约束解析
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::parseStyleRule() {
    // 解析选择器
    std::string selector;
    
    // 处理不同类型的选择器
    if (match(TokenType::DOT)) {
        // 类选择器 .class
        selector = ".";
        selector += consume(TokenType::IDENTIFIER, "Expected class name after '.'").value;
    } else if (match(TokenType::AMPERSAND)) {
        // & 符号（引用父选择器）
        selector = "&";
        
        // 可能跟着伪类或伪元素
        if (match(TokenType::COLON)) {
            selector += ":";
            selector += consume(TokenType::IDENTIFIER, "Expected pseudo-class/element after ':'").value;
            
            // 处理双冒号伪元素
            if (match(TokenType::COLON)) {
                selector = "&::";
                selector += previous_.value;
            }
        }
    } else if (current_.type == TokenType::IDENTIFIER && current_.value[0] == '#') {
        // ID选择器 #id
        selector = current_.value;
        advance();
    } else {
        // 其他选择器（元素选择器等）
        selector = consume(TokenType::IDENTIFIER, "Expected selector").value;
    }
    
    auto rule = std::make_unique<StyleRuleNode>(selector, current_.line, current_.column);
    
    consume(TokenType::LBRACE, "Expected '{' after selector");
    
    // 解析样式属性
    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        auto prop = parseStyleProperty();
        if (prop) {
            rule->addChild(std::move(prop));
        }
    }
    
    consume(TokenType::RBRACE, "Expected '}' after style rule");
    
    return rule;
}

std::unique_ptr<ASTNode> CHTLParser::parseExpression() {
    // TODO: 实现表达式解析
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::parseLiteral() {
    // TODO: 实现字面量解析
    return nullptr;
}

std::unique_ptr<ASTNode> CHTLParser::parseVariableAccess() {
    // TODO: 实现变量访问解析
    return nullptr;
}

bool CHTLParser::isCustomUsage() {
    // TODO: 实现自定义使用判断
    return false;
}

} // namespace chtl
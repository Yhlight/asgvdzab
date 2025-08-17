#include "../../include/compilers/CHTLCompiler.h"
#include "../../include/scanner/CHTLUnifiedScanner.h"
#include "../../include/common/Utils.h"
#include <iostream>
#include <sstream>

namespace CHTL {

CHTLCompiler::CHTLCompiler()
    : current_token_index(0), class_counter(0), id_counter(0) {
}

CHTLCompiler::CHTLCompiler(const Configuration& cfg)
    : config(cfg), current_token_index(0), class_counter(0), id_counter(0) {
}

void CHTLCompiler::setConfiguration(const Configuration& cfg) {
    config = cfg;
}

CompileResultPtr CHTLCompiler::compile(const std::string& chtl_code) {
    // 使用扫描器进行词法分析
    CHTLUnifiedScanner scanner(chtl_code);
    scanner.setConfiguration(config);
    
    auto token_stream = scanner.tokenize();
    return compile(token_stream);
}

CompileResultPtr CHTLCompiler::compile(const std::vector<TokenPtr>& token_stream) {
    auto result = std::make_shared<CompileResult>();
    
    try {
        // 解析Token流生成AST
        ASTNodePtr ast = parse(token_stream);
        
        if (!ast) {
            result->success = false;
            result->errors.push_back("解析失败：无法生成AST");
            return result;
        }
        
        // 生成代码
        std::string generated_code = generateCode(ast);
        
        result->success = errors.empty();
        result->output = generated_code;
        result->errors = errors;
        result->warnings = warnings;
        
    } catch (const std::exception& e) {
        result->success = false;
        result->errors.push_back("编译异常: " + std::string(e.what()));
    }
    
    return result;
}

ASTNodePtr CHTLCompiler::parse(const std::vector<TokenPtr>& token_stream) {
    tokens = token_stream;
    current_token_index = 0;
    errors.clear();
    warnings.clear();
    
    return parseProgram();
}

TokenPtr CHTLCompiler::currentToken() const {
    if (current_token_index >= tokens.size()) {
        return nullptr;
    }
    return tokens[current_token_index];
}

TokenPtr CHTLCompiler::peekToken(size_t offset) const {
    size_t index = current_token_index + offset;
    if (index >= tokens.size()) {
        return nullptr;
    }
    return tokens[index];
}

TokenPtr CHTLCompiler::advance() {
    if (current_token_index < tokens.size()) {
        return tokens[current_token_index++];
    }
    return nullptr;
}

bool CHTLCompiler::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool CHTLCompiler::check(TokenType type) const {
    if (isAtEnd()) return false;
    return currentToken()->type == type;
}

bool CHTLCompiler::isAtEnd() const {
    return current_token_index >= tokens.size() || 
           (currentToken() && currentToken()->type == TokenType::EOF_TOKEN);
}

ASTNodePtr CHTLCompiler::parseProgram() {
    auto root = std::make_shared<ASTNode>(ASTNodeType::ROOT, "program");
    current_ast = root;
    
    while (!isAtEnd()) {
        // 跳过无关的token
        if (currentToken()->type == TokenType::NEWLINE || 
            currentToken()->type == TokenType::WHITESPACE) {
            advance();
            continue;
        }
        
        ASTNodePtr node = nullptr;
        
        // 解析不同类型的顶级声明
        if (check(TokenType::TEMPLATE)) {
            node = parseTemplate();
        } else if (check(TokenType::CUSTOM)) {
            node = parseCustom();
        } else if (check(TokenType::ORIGIN)) {
            node = parseOrigin();
        } else if (check(TokenType::IMPORT)) {
            node = parseImport();
        } else if (check(TokenType::NAMESPACE)) {
            node = parseNamespace();
        } else if (check(TokenType::CONFIGURATION)) {
            node = parseConfiguration();
        } else if (check(TokenType::IDENTIFIER)) {
            // 可能是HTML元素
            node = parseElement();
        } else {
            addError("未知的顶级声明: " + currentToken()->value);
            advance(); // 跳过错误的token
        }
        
        if (node) {
            root->addChild(node);
        }
    }
    
    return root;
}

ASTNodePtr CHTLCompiler::parseElement() {
    if (!check(TokenType::IDENTIFIER)) {
        addError("期望元素名称");
        return nullptr;
    }
    
    auto element = std::make_shared<ASTNode>(ASTNodeType::ELEMENT, currentToken()->value);
    advance(); // 元素名称
    
    if (!match(TokenType::LEFT_BRACE)) {
        addError("期望 '{'");
        return element;
    }
    
    // 解析元素内容
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        ASTNodePtr child = nullptr;
        
        if (check(TokenType::KEYWORD_TEXT)) {
            child = parseTextNode();
        } else if (check(TokenType::KEYWORD_STYLE)) {
            child = parseStyleBlock();
        } else if (check(TokenType::KEYWORD_SCRIPT)) {
            child = parseScriptBlock();
        } else if (check(TokenType::IDENTIFIER)) {
            // 可能是属性或子元素
            TokenPtr next = peekToken(1);
            if (next && (next->type == TokenType::COLON || next->type == TokenType::EQUALS)) {
                child = parseAttribute();
            } else {
                child = parseElement(); // 递归解析子元素
            }
        } else if (check(TokenType::AT_STYLE) || check(TokenType::AT_ELEMENT) || check(TokenType::AT_VAR)) {
            // 模板或自定义使用
            std::string type_name = currentToken()->value;
            advance();
            
            if (check(TokenType::IDENTIFIER)) {
                std::string name = currentToken()->value;
                advance();
                
                // 创建对应的使用节点
                if (type_name == "@Style") {
                    child = std::make_shared<ASTNode>(ASTNodeType::TEMPLATE_STYLE, name);
                } else if (type_name == "@Element") {
                    child = std::make_shared<ASTNode>(ASTNodeType::TEMPLATE_ELEMENT, name);
                } else if (type_name == "@Var") {
                    child = std::make_shared<ASTNode>(ASTNodeType::TEMPLATE_VAR, name);
                }
                
                match(TokenType::SEMICOLON); // 可选的分号
            }
        } else {
            addError("未知的元素内容: " + currentToken()->value);
            advance();
        }
        
        if (child) {
            element->addChild(child);
        }
    }
    
    if (!match(TokenType::RIGHT_BRACE)) {
        addError("期望 '}'");
    }
    
    return element;
}

ASTNodePtr CHTLCompiler::parseTextNode() {
    if (!match(TokenType::KEYWORD_TEXT)) {
        addError("期望 'text' 关键字");
        return nullptr;
    }
    
    auto text_node = std::make_shared<ASTNode>(ASTNodeType::TEXT, "text");
    
    if (!match(TokenType::LEFT_BRACE)) {
        addError("期望 '{'");
        return text_node;
    }
    
    // 解析文本内容
    std::string text_content;
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        if (currentToken()->isLiteral()) {
            text_content += currentToken()->value;
            advance();
        } else {
            addError("文本节点中期望字面量");
            advance();
        }
    }
    
    text_node->value = text_content;
    
    if (!match(TokenType::RIGHT_BRACE)) {
        addError("期望 '}'");
    }
    
    return text_node;
}

ASTNodePtr CHTLCompiler::parseStyleBlock() {
    if (!match(TokenType::KEYWORD_STYLE)) {
        addError("期望 'style' 关键字");
        return nullptr;
    }
    
    auto style_block = std::make_shared<ASTNode>(ASTNodeType::STYLE_BLOCK, "style");
    
    if (!match(TokenType::LEFT_BRACE)) {
        addError("期望 '{'");
        return style_block;
    }
    
    // 解析样式内容
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        ASTNodePtr rule = parseCSSRule();
        if (rule) {
            style_block->addChild(rule);
        }
    }
    
    if (!match(TokenType::RIGHT_BRACE)) {
        addError("期望 '}'");
    }
    
    return style_block;
}

ASTNodePtr CHTLCompiler::parseScriptBlock() {
    if (!match(TokenType::KEYWORD_SCRIPT)) {
        addError("期望 'script' 关键字");
        return nullptr;
    }
    
    auto script_block = std::make_shared<ASTNode>(ASTNodeType::SCRIPT_BLOCK, "script");
    
    if (!match(TokenType::LEFT_BRACE)) {
        addError("期望 '{'");
        return script_block;
    }
    
    // 解析脚本内容 - 这里暂时作为原始内容处理
    std::string script_content;
    int brace_count = 1;
    
    while (brace_count > 0 && !isAtEnd()) {
        if (check(TokenType::LEFT_BRACE)) {
            brace_count++;
        } else if (check(TokenType::RIGHT_BRACE)) {
            brace_count--;
        }
        
        if (brace_count > 0) {
            script_content += currentToken()->value;
        }
        advance();
    }
    
    script_block->value = script_content;
    return script_block;
}

ASTNodePtr CHTLCompiler::parseAttribute() {
    if (!check(TokenType::IDENTIFIER)) {
        addError("期望属性名称");
        return nullptr;
    }
    
    std::string attr_name = currentToken()->value;
    advance();
    
    if (!match(TokenType::COLON) && !match(TokenType::EQUALS)) {
        addError("期望 ':' 或 '='");
        return nullptr;
    }
    
    std::string attr_value;
    if (currentToken() && (currentToken()->isLiteral() || check(TokenType::IDENTIFIER))) {
        attr_value = currentToken()->value;
        advance();
    } else {
        addError("期望属性值");
    }
    
    match(TokenType::SEMICOLON); // 可选的分号
    
    auto attr_node = std::make_shared<ASTNode>(ASTNodeType::ATTRIBUTE, attr_name, attr_value);
    return attr_node;
}

ASTNodePtr CHTLCompiler::parseTemplate() {
    if (!match(TokenType::TEMPLATE)) {
        addError("期望 '[Template]'");
        return nullptr;
    }
    
    // 解析模板类型
    if (!currentToken() || !currentToken()->isTypeIdentifier()) {
        addError("期望模板类型 (@Style, @Element, @Var)");
        return nullptr;
    }
    
    std::string type_str = currentToken()->value;
    advance();
    
    // 解析模板名称
    if (!check(TokenType::IDENTIFIER)) {
        addError("期望模板名称");
        return nullptr;
    }
    
    std::string template_name = currentToken()->value;
    advance();
    
    TemplateType template_type;
    if (type_str == "@Style") {
        template_type = TemplateType::STYLE;
    } else if (type_str == "@Element") {
        template_type = TemplateType::ELEMENT;
    } else if (type_str == "@Var") {
        template_type = TemplateType::VAR;
    } else {
        addError("未知的模板类型: " + type_str);
        return nullptr;
    }
    
    auto template_node = std::make_shared<TemplateNode>(template_type, template_name);
    
    if (!match(TokenType::LEFT_BRACE)) {
        addError("期望 '{'");
        return template_node;
    }
    
    // 解析模板内容
    if (template_type == TemplateType::STYLE) {
        return parseTemplateStyle();
    } else if (template_type == TemplateType::ELEMENT) {
        return parseTemplateElement();
    } else if (template_type == TemplateType::VAR) {
        return parseTemplateVar();
    }
    
    return template_node;
}

ASTNodePtr CHTLCompiler::parseTemplateStyle() {
    auto template_node = std::make_shared<ASTNode>(ASTNodeType::TEMPLATE_STYLE);
    
    // 解析CSS属性
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        ASTNodePtr property = parseCSSProperty();
        if (property) {
            template_node->addChild(property);
        }
    }
    
    if (!match(TokenType::RIGHT_BRACE)) {
        addError("期望 '}'");
    }
    
    return template_node;
}

ASTNodePtr CHTLCompiler::parseTemplateElement() {
    auto template_node = std::make_shared<ASTNode>(ASTNodeType::TEMPLATE_ELEMENT);
    
    // 解析元素内容
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        ASTNodePtr element = parseElement();
        if (element) {
            template_node->addChild(element);
        }
    }
    
    if (!match(TokenType::RIGHT_BRACE)) {
        addError("期望 '}'");
    }
    
    return template_node;
}

ASTNodePtr CHTLCompiler::parseTemplateVar() {
    auto template_node = std::make_shared<ASTNode>(ASTNodeType::TEMPLATE_VAR);
    
    // 解析变量定义
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        if (check(TokenType::IDENTIFIER)) {
            std::string var_name = currentToken()->value;
            advance();
            
            if (match(TokenType::COLON) || match(TokenType::EQUALS)) {
                std::string var_value;
                if (currentToken() && currentToken()->isLiteral()) {
                    var_value = currentToken()->value;
                    advance();
                }
                
                auto var_node = std::make_shared<ASTNode>(ASTNodeType::ATTRIBUTE, var_name, var_value);
                template_node->addChild(var_node);
                
                match(TokenType::SEMICOLON); // 可选的分号
            }
        } else {
            addError("期望变量名称");
            advance();
        }
    }
    
    if (!match(TokenType::RIGHT_BRACE)) {
        addError("期望 '}'");
    }
    
    return template_node;
}

ASTNodePtr CHTLCompiler::parseCustom() {
    if (!match(TokenType::CUSTOM)) {
        addError("期望 '[Custom]'");
        return nullptr;
    }
    
    // 解析自定义类型
    if (!currentToken() || !currentToken()->isTypeIdentifier()) {
        addError("期望自定义类型 (@Style, @Element, @Var)");
        return nullptr;
    }
    
    std::string type_str = currentToken()->value;
    advance();
    
    // 解析自定义名称
    if (!check(TokenType::IDENTIFIER)) {
        addError("期望自定义名称");
        return nullptr;
    }
    
    std::string custom_name = currentToken()->value;
    advance();
    
    CustomType custom_type;
    if (type_str == "@Style") {
        custom_type = CustomType::STYLE;
    } else if (type_str == "@Element") {
        custom_type = CustomType::ELEMENT;
    } else if (type_str == "@Var") {
        custom_type = CustomType::VAR;
    } else {
        addError("未知的自定义类型: " + type_str);
        return nullptr;
    }
    
    auto custom_node = std::make_shared<CustomNode>(custom_type, custom_name);
    
    if (!match(TokenType::LEFT_BRACE)) {
        addError("期望 '{'");
        return custom_node;
    }
    
    // 解析自定义内容 - 类似于模板解析
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        ASTNodePtr child = nullptr;
        
        if (custom_type == CustomType::STYLE) {
            child = parseCSSProperty();
        } else if (custom_type == CustomType::ELEMENT) {
            child = parseElement();
        } else if (custom_type == CustomType::VAR) {
            // 变量解析
            if (check(TokenType::IDENTIFIER)) {
                child = parseAttribute();
            }
        }
        
        if (child) {
            custom_node->addChild(child);
        } else {
            advance(); // 跳过无法解析的token
        }
    }
    
    if (!match(TokenType::RIGHT_BRACE)) {
        addError("期望 '}'");
    }
    
    return custom_node;
}

ASTNodePtr CHTLCompiler::parseOrigin() {
    if (!match(TokenType::ORIGIN)) {
        addError("期望 '[Origin]'");
        return nullptr;
    }
    
    // 解析原始类型
    if (!currentToken() || !currentToken()->isTypeIdentifier()) {
        addError("期望原始类型");
        return nullptr;
    }
    
    std::string type_str = currentToken()->value;
    advance();
    
    OriginType origin_type;
    if (type_str == "@Html") {
        origin_type = OriginType::HTML;
    } else if (type_str == "@Style") {
        origin_type = OriginType::STYLE;
    } else if (type_str == "@JavaScript") {
        origin_type = OriginType::JAVASCRIPT;
    } else {
        origin_type = OriginType::CUSTOM;
    }
    
    // 可选的名称
    std::string origin_name;
    if (check(TokenType::IDENTIFIER)) {
        origin_name = currentToken()->value;
        advance();
    }
    
    auto origin_node = std::make_shared<OriginNode>(origin_type, origin_name);
    
    if (!match(TokenType::LEFT_BRACE)) {
        addError("期望 '{'");
        return origin_node;
    }
    
    // 解析原始内容
    std::string raw_content;
    int brace_count = 1;
    
    while (brace_count > 0 && !isAtEnd()) {
        if (check(TokenType::LEFT_BRACE)) {
            brace_count++;
        } else if (check(TokenType::RIGHT_BRACE)) {
            brace_count--;
        }
        
        if (brace_count > 0) {
            raw_content += currentToken()->value;
        }
        advance();
    }
    
    origin_node->value = raw_content;
    return origin_node;
}

ASTNodePtr CHTLCompiler::parseImport() {
    if (!match(TokenType::IMPORT)) {
        addError("期望 '[Import]'");
        return nullptr;
    }
    
    auto import_node = std::make_shared<ASTNode>(ASTNodeType::IMPORT, "import");
    
    // 解析导入内容 - 这里简化处理
    while (!check(TokenType::SEMICOLON) && !isAtEnd()) {
        import_node->value += currentToken()->value + " ";
        advance();
    }
    
    match(TokenType::SEMICOLON);
    return import_node;
}

ASTNodePtr CHTLCompiler::parseNamespace() {
    if (!match(TokenType::NAMESPACE)) {
        addError("期望 '[Namespace]'");
        return nullptr;
    }
    
    if (!check(TokenType::IDENTIFIER)) {
        addError("期望命名空间名称");
        return nullptr;
    }
    
    std::string namespace_name = currentToken()->value;
    advance();
    
    auto namespace_node = std::make_shared<ASTNode>(ASTNodeType::NAMESPACE, namespace_name);
    
    // 进入命名空间
    std::string prev_namespace = current_namespace;
    current_namespace = namespace_name;
    
    if (match(TokenType::LEFT_BRACE)) {
        // 解析命名空间内容
        while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
            ASTNodePtr child = nullptr;
            
            if (check(TokenType::TEMPLATE)) {
                child = parseTemplate();
            } else if (check(TokenType::CUSTOM)) {
                child = parseCustom();
            } else if (check(TokenType::NAMESPACE)) {
                child = parseNamespace(); // 嵌套命名空间
            } else {
                advance(); // 跳过未知内容
            }
            
            if (child) {
                namespace_node->addChild(child);
            }
        }
        
        if (!match(TokenType::RIGHT_BRACE)) {
            addError("期望 '}'");
        }
    }
    
    // 退出命名空间
    current_namespace = prev_namespace;
    
    return namespace_node;
}

ASTNodePtr CHTLCompiler::parseConfiguration() {
    if (!match(TokenType::CONFIGURATION)) {
        addError("期望 '[Configuration]'");
        return nullptr;
    }
    
    auto config_node = std::make_shared<ASTNode>(ASTNodeType::CONFIGURATION, "configuration");
    
    if (!match(TokenType::LEFT_BRACE)) {
        addError("期望 '{'");
        return config_node;
    }
    
    // 解析配置项
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        if (check(TokenType::IDENTIFIER)) {
            std::string config_key = currentToken()->value;
            advance();
            
            if (match(TokenType::EQUALS)) {
                std::string config_value;
                if (currentToken() && currentToken()->isLiteral()) {
                    config_value = currentToken()->value;
                    advance();
                }
                
                config_node->addAttribute(config_key, config_value);
                match(TokenType::SEMICOLON);
            }
        } else {
            advance(); // 跳过未知内容
        }
    }
    
    if (!match(TokenType::RIGHT_BRACE)) {
        addError("期望 '}'");
    }
    
    return config_node;
}

ASTNodePtr CHTLCompiler::parseCSSRule() {
    ASTNodePtr rule = nullptr;
    
    if (check(TokenType::DOT) || check(TokenType::HASH) || check(TokenType::AMPERSAND)) {
        // CSS选择器规则
        rule = parseCSSSelector();
    } else if (check(TokenType::IDENTIFIER)) {
        // 可能是CSS属性或选择器
        TokenPtr next = peekToken(1);
        if (next && (next->type == TokenType::COLON || next->type == TokenType::EQUALS)) {
            rule = parseCSSProperty();
        } else {
            // 作为选择器处理
            rule = parseCSSSelector();
        }
    }
    
    return rule;
}

ASTNodePtr CHTLCompiler::parseCSSSelector() {
    std::string selector;
    
    // 解析选择器
    if (check(TokenType::DOT)) {
        selector += advance()->value; // '.'
        if (check(TokenType::IDENTIFIER)) {
            selector += advance()->value; // 类名
        }
    } else if (check(TokenType::HASH)) {
        selector += advance()->value; // '#'
        if (check(TokenType::IDENTIFIER)) {
            selector += advance()->value; // ID名
        }
    } else if (check(TokenType::AMPERSAND)) {
        selector += advance()->value; // '&'
        // 可能跟着伪类或伪元素
        while (currentToken() && (check(TokenType::COLON) || check(TokenType::IDENTIFIER))) {
            selector += advance()->value;
        }
    } else if (check(TokenType::IDENTIFIER)) {
        selector += advance()->value; // 元素选择器
    }
    
    auto selector_node = std::make_shared<ASTNode>(ASTNodeType::CSS_SELECTOR, selector);
    
    if (!match(TokenType::LEFT_BRACE)) {
        addError("期望 '{'");
        return selector_node;
    }
    
    // 解析CSS属性
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        ASTNodePtr property = parseCSSProperty();
        if (property) {
            selector_node->addChild(property);
        }
    }
    
    if (!match(TokenType::RIGHT_BRACE)) {
        addError("期望 '}'");
    }
    
    return selector_node;
}

ASTNodePtr CHTLCompiler::parseCSSProperty() {
    if (!check(TokenType::IDENTIFIER)) {
        addError("期望CSS属性名");
        return nullptr;
    }
    
    std::string property_name = currentToken()->value;
    advance();
    
    if (!match(TokenType::COLON) && !match(TokenType::EQUALS)) {
        addError("期望 ':' 或 '='");
        return nullptr;
    }
    
    std::string property_value;
    
    // 解析属性值 - 可能包含多个token
    while (!check(TokenType::SEMICOLON) && !check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        if (currentToken()->isLiteral() || check(TokenType::IDENTIFIER) || check(TokenType::NUMBER_LITERAL)) {
            property_value += currentToken()->value;
            advance();
        } else {
            break;
        }
        
        // 如果下一个token不是结束符，添加空格
        if (!check(TokenType::SEMICOLON) && !check(TokenType::RIGHT_BRACE) && !isAtEnd() &&
            (currentToken()->isLiteral() || check(TokenType::IDENTIFIER) || check(TokenType::NUMBER_LITERAL))) {
            property_value += " ";
        }
    }
    
    match(TokenType::SEMICOLON); // 可选的分号
    
    auto property_node = std::make_shared<ASTNode>(ASTNodeType::CSS_PROPERTY, property_name, property_value);
    return property_node;
}

ASTNodePtr CHTLCompiler::parseInherit() {
    // 解析继承语句
    if (!match(TokenType::KEYWORD_INHERIT)) {
        return nullptr;
    }
    
    auto inherit_node = std::make_shared<ASTNode>(ASTNodeType::INHERIT, "inherit");
    
    // 解析继承目标
    while (!check(TokenType::SEMICOLON) && !isAtEnd()) {
        inherit_node->value += currentToken()->value + " ";
        advance();
    }
    
    match(TokenType::SEMICOLON);
    return inherit_node;
}

ASTNodePtr CHTLCompiler::parseDelete() {
    if (!match(TokenType::KEYWORD_DELETE)) {
        return nullptr;
    }
    
    auto delete_node = std::make_shared<ASTNode>(ASTNodeType::DELETE, "delete");
    
    // 解析删除目标
    while (!check(TokenType::SEMICOLON) && !isAtEnd()) {
        delete_node->value += currentToken()->value + " ";
        advance();
    }
    
    match(TokenType::SEMICOLON);
    return delete_node;
}

ASTNodePtr CHTLCompiler::parseInsert() {
    if (!match(TokenType::KEYWORD_INSERT)) {
        return nullptr;
    }
    
    auto insert_node = std::make_shared<ASTNode>(ASTNodeType::INSERT, "insert");
    
    // 解析插入位置和内容
    while (!check(TokenType::LEFT_BRACE) && !isAtEnd()) {
        insert_node->value += currentToken()->value + " ";
        advance();
    }
    
    if (match(TokenType::LEFT_BRACE)) {
        // 解析插入内容
        while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
            ASTNodePtr child = parseElement();
            if (child) {
                insert_node->addChild(child);
            }
        }
        
        match(TokenType::RIGHT_BRACE);
    }
    
    return insert_node;
}

std::string CHTLCompiler::generateClassName() {
    return "chtl-class-" + std::to_string(++class_counter);
}

std::string CHTLCompiler::generateId() {
    return "chtl-id-" + std::to_string(++id_counter);
}

std::string CHTLCompiler::resolveTemplate(const std::string& template_name, TemplateType type) {
    // 查找模板并返回解析后的内容
    std::map<std::string, ASTNodePtr>* template_map = nullptr;
    
    switch (type) {
        case TemplateType::STYLE:
            template_map = &style_templates;
            break;
        case TemplateType::ELEMENT:
            template_map = &element_templates;
            break;
        case TemplateType::VAR:
            template_map = &var_templates;
            break;
    }
    
    if (template_map) {
        auto it = template_map->find(template_name);
        if (it != template_map->end()) {
            return generateCode(it->second);
        }
    }
    
    addWarning("未找到模板: " + template_name);
    return "";
}

std::string CHTLCompiler::generateCode(ASTNodePtr ast) {
    if (!ast) return "";
    
    // 生成HTML，同时收集CSS
    std::string html_output = generateHTML(ast);
    std::string css_output = generateGlobalCSS();
    
    // 如果有CSS，将其添加到输出中
    if (!css_output.empty()) {
        html_output = "<!-- CHTL Generated CSS -->\n<style>\n" + css_output + "</style>\n" + html_output;
    }
    
    return html_output;
}

std::string CHTLCompiler::generateHTML(ASTNodePtr node) {
    if (!node) return "";
    
    std::stringstream html;
    
    switch (node->type) {
        case ASTNodeType::ROOT: {
            // 根节点，生成所有子节点
            for (const auto& child : node->children) {
                html << generateHTML(child);
            }
            break;
        }
        
        case ASTNodeType::ELEMENT: {
            html << "<" << node->name;
            
            // 生成属性 - 从子节点中收集属性
            for (const auto& child : node->children) {
                if (child->type == ASTNodeType::ATTRIBUTE) {
                    html << " " << child->name << "=\"" << Utils::escapeHtml(child->value) << "\"";
                }
            }
            
            // 检查是否有自动生成的类名或ID
            bool has_style_block = node->findChild(ASTNodeType::STYLE_BLOCK) != nullptr;
            if (has_style_block) {
                // 自动生成类名
                std::string auto_class = generateClassName();
                html << " class=\"" << auto_class << "\"";
                auto_generated_classes[node->name] = auto_class;
            }
            
            html << ">";
            
            // 生成子元素内容
            for (const auto& child : node->children) {
                if (child->type != ASTNodeType::STYLE_BLOCK && 
                    child->type != ASTNodeType::SCRIPT_BLOCK) {
                    html << generateHTML(child);
                }
            }
            
            html << "</" << node->name << ">";
            break;
        }
        
        case ASTNodeType::TEXT: {
            html << Utils::escapeHtml(node->value);
            break;
        }
        
        default:
            // 对于其他类型，递归处理子节点
            for (const auto& child : node->children) {
                html << generateHTML(child);
            }
            break;
    }
    
    return html.str();
}

std::string CHTLCompiler::generateCSS(ASTNodePtr node) {
    if (!node) return "";
    
    std::stringstream css;
    
    switch (node->type) {
        case ASTNodeType::STYLE_BLOCK: {
            // 局部样式块
            for (const auto& child : node->children) {
                css << generateCSS(child);
            }
            break;
        }
        
        case ASTNodeType::CSS_SELECTOR: {
            css << node->name << " {\n";
            
            for (const auto& child : node->children) {
                if (child->type == ASTNodeType::CSS_PROPERTY) {
                    css << "  " << child->name << ": " << child->value << ";\n";
                }
            }
            
            css << "}\n";
            break;
        }
        
        case ASTNodeType::CSS_PROPERTY: {
            css << node->name << ": " << node->value << ";";
            break;
        }
        
        default:
            // 递归处理子节点
            for (const auto& child : node->children) {
                css << generateCSS(child);
            }
            break;
    }
    
    return css.str();
}

std::string CHTLCompiler::generateInlineStyle(ASTNodePtr style_node) {
    if (!style_node || style_node->type != ASTNodeType::STYLE_BLOCK) {
        return "";
    }
    
    std::stringstream style;
    
    for (const auto& child : style_node->children) {
        if (child->type == ASTNodeType::CSS_PROPERTY) {
            style << child->name << ": " << child->value << "; ";
        }
    }
    
    return Utils::trim(style.str());
}

std::string CHTLCompiler::generateGlobalCSS() {
    std::stringstream css;
    
    // 从当前AST中收集所有的样式块
    if (current_ast) {
        collectCSSFromNode(current_ast, css);
    }
    
    return css.str();
}

void CHTLCompiler::collectCSSFromNode(ASTNodePtr node, std::stringstream& css) {
    if (!node) return;
    
    // 如果是样式块，生成CSS
    if (node->type == ASTNodeType::STYLE_BLOCK) {
        // 查找父元素以获取自动生成的类名
        ASTNodePtr parent = node->parent.lock();
        if (parent && parent->type == ASTNodeType::ELEMENT) {
            std::string element_class = auto_generated_classes[parent->name];
            if (!element_class.empty()) {
                // 为内联样式生成CSS规则
                css << "." << element_class << " {\n";
                for (const auto& child : node->children) {
                    if (child->type == ASTNodeType::CSS_PROPERTY) {
                        css << "  " << child->name << ": " << child->value << ";\n";
                    } else if (child->type == ASTNodeType::CSS_SELECTOR) {
                        // 处理选择器规则，需要替换&为当前类名
                        std::string selector = child->name;
                        if (selector.find('&') != std::string::npos) {
                            selector = Utils::replace(selector, "&", "." + element_class);
                        }
                        css << selector << " {\n";
                        for (const auto& prop : child->children) {
                            if (prop->type == ASTNodeType::CSS_PROPERTY) {
                                css << "  " << prop->name << ": " << prop->value << ";\n";
                            }
                        }
                        css << "}\n";
                    }
                }
                css << "}\n";
            }
        }
    }
    
    // 递归处理子节点
    for (const auto& child : node->children) {
        collectCSSFromNode(child, css);
    }
}

void CHTLCompiler::addError(const std::string& message) {
    std::string formatted_error = formatError(message, currentToken());
    errors.push_back(formatted_error);
    
    if (config.debug_mode) {
        Utils::debugLog("CHTL_COMPILER_ERROR", formatted_error);
    }
}

void CHTLCompiler::addWarning(const std::string& message) {
    std::string formatted_warning = "警告: " + message;
    warnings.push_back(formatted_warning);
    
    if (config.debug_mode) {
        Utils::debugLog("CHTL_COMPILER_WARNING", formatted_warning);
    }
}

std::string CHTLCompiler::formatError(const std::string& message, TokenPtr token) {
    if (token) {
        return Utils::formatError(message, token->line, token->column);
    }
    return "错误: " + message;
}

void CHTLCompiler::registerTemplate(const std::string& name, TemplateType type, ASTNodePtr node) {
    switch (type) {
        case TemplateType::STYLE:
            style_templates[name] = node;
            break;
        case TemplateType::ELEMENT:
            element_templates[name] = node;
            break;
        case TemplateType::VAR:
            var_templates[name] = node;
            break;
    }
}

ASTNodePtr CHTLCompiler::getTemplate(const std::string& name, TemplateType type) const {
    const std::map<std::string, ASTNodePtr>* template_map = nullptr;
    
    switch (type) {
        case TemplateType::STYLE:
            template_map = &style_templates;
            break;
        case TemplateType::ELEMENT:
            template_map = &element_templates;
            break;
        case TemplateType::VAR:
            template_map = &var_templates;
            break;
    }
    
    if (template_map) {
        auto it = template_map->find(name);
        if (it != template_map->end()) {
            return it->second;
        }
    }
    
    return nullptr;
}

void CHTLCompiler::registerCustom(const std::string& name, CustomType type, ASTNodePtr node) {
    switch (type) {
        case CustomType::STYLE:
            custom_styles[name] = node;
            break;
        case CustomType::ELEMENT:
            custom_elements[name] = node;
            break;
        case CustomType::VAR:
            custom_vars[name] = node;
            break;
    }
}

ASTNodePtr CHTLCompiler::getCustom(const std::string& name, CustomType type) const {
    const std::map<std::string, ASTNodePtr>* custom_map = nullptr;
    
    switch (type) {
        case CustomType::STYLE:
            custom_map = &custom_styles;
            break;
        case CustomType::ELEMENT:
            custom_map = &custom_elements;
            break;
        case CustomType::VAR:
            custom_map = &custom_vars;
            break;
    }
    
    if (custom_map) {
        auto it = custom_map->find(name);
        if (it != custom_map->end()) {
            return it->second;
        }
    }
    
    return nullptr;
}

void CHTLCompiler::enterNamespace(const std::string& name) {
    if (!current_namespace.empty()) {
        current_namespace += ".";
    }
    current_namespace += name;
}

void CHTLCompiler::exitNamespace() {
    size_t pos = current_namespace.find_last_of('.');
    if (pos != std::string::npos) {
        current_namespace = current_namespace.substr(0, pos);
    } else {
        current_namespace.clear();
    }
}

void CHTLCompiler::reset() {
    current_token_index = 0;
    current_ast = nullptr;
    errors.clear();
    warnings.clear();
    current_namespace.clear();
    class_counter = 0;
    id_counter = 0;
    auto_generated_classes.clear();
    auto_generated_ids.clear();
}

void CHTLCompiler::dumpAST(ASTNodePtr node) const {
    if (!node) {
        node = current_ast;
    }
    
    if (node) {
        std::cout << "=== AST Dump ===" << std::endl;
        std::cout << node->toString() << std::endl;
        std::cout << "================" << std::endl;
    }
}

void CHTLCompiler::setDebugMode(bool enabled) {
    config.debug_mode = enabled;
}

} // namespace CHTL
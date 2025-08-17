#include "Parser/Parser.h"
#include "AST/ChtlJsNodes.h"
#include "Parser/ChtlJsParser.h"
#include <iostream>

namespace Chtl {

// 脚本块解析
ASTNodePtr Parser::parseScriptBlock() {
    Token scriptToken = consume(TokenType::SCRIPT, "Expected 'script'");
    auto scriptBlock = std::make_shared<ScriptBlockNode>(scriptToken);
    
    consume(TokenType::LBRACE, "Expected '{' after 'script'");
    
    // 已经通过StateManager管理状态
    std::string content;
    int braceCount = 1;
    
    // 收集脚本内容直到匹配的 }
    while (!isAtEnd() && braceCount > 0) {
        Token token = peek();
        
        if (token.type == TokenType::LBRACE) {
            braceCount++;
        } else if (token.type == TokenType::RBRACE) {
            braceCount--;
            if (braceCount == 0) break;
        }
        
        // 检查是否包含CHTL JS语法
        if (token.type == TokenType::DOUBLE_LBRACE || 
            token.type == TokenType::ARROW) {
            scriptBlock->setHasChtlJsSyntax(true);
        }
        
        content += token.value;
        if (token.type != TokenType::EOF_TOKEN && 
            token.type != TokenType::DOUBLE_LBRACE && 
            token.type != TokenType::DOUBLE_RBRACE) {
            content += " ";
        }
        advance();
    }
    
    scriptBlock->setContent(content);
    // 已经通过StateManager管理状态
    
    consume(TokenType::RBRACE, "Expected '}' after script content");
    
    // 如果包含CHTL JS语法，解析并转换
    if (scriptBlock->hasChtlJsSyntax()) {
        ChtlJsParser jsParser;
        
        // 转换为标准JavaScript
        std::string transformedJs = jsParser.transform(content);
        scriptBlock->setTransformedContent(transformedJs);
    }
    
    return scriptBlock;
}

// 模板解析
ASTNodePtr Parser::parseTemplate() {
    consume(TokenType::TEMPLATE_MARK, "Expected '[Template]'");
    
    // 检查模板类型
    if (check(TokenType::AT_STYLE)) {
        return parseStyleTemplate();
    } else if (check(TokenType::AT_ELEMENT)) {
        return parseElementTemplate();
    } else if (check(TokenType::AT_VAR)) {
        return parseVarTemplate();
    }
    
    error("Expected @Style, @Element, or @Var after [Template]");
    return nullptr;
}

// 样式组模板解析
ASTNodePtr Parser::parseStyleTemplate() {
    Token atStyle = consume(TokenType::AT_STYLE, "Expected '@Style'");
    Token name = consume(TokenType::IDENTIFIER, "Expected template name");
    
    auto styleTemplate = std::make_shared<StyleTemplateNode>(atStyle);
    styleTemplate->setName(name.value);
    
    consume(TokenType::LBRACE, "Expected '{' after template name");
    
    // 解析样式属性和继承
    while (!isAtEnd() && !check(TokenType::RBRACE)) {
        if (check(TokenType::AT_STYLE)) {
            // 继承其他样式组
            styleTemplate->addChild(parseTemplateUse());
        } else if (check(TokenType::INHERIT)) {
            // 显式继承
            styleTemplate->addChild(parseInherit());
        } else if (check(TokenType::IDENTIFIER)) {
            // 样式属性
            styleTemplate->addChild(parseStyleProperty());
        } else {
            error("Unexpected token in style template: " + peek().value);
            advance();
        }
    }
    
    consume(TokenType::RBRACE, "Expected '}' after template content");
    
    return styleTemplate;
}

// 元素模板解析
ASTNodePtr Parser::parseElementTemplate() {
    Token atElement = consume(TokenType::AT_ELEMENT, "Expected '@Element'");
    Token name = consume(TokenType::IDENTIFIER, "Expected template name");
    
    auto elementTemplate = std::make_shared<ElementTemplateNode>(atElement);
    elementTemplate->setName(name.value);
    
    consume(TokenType::LBRACE, "Expected '{' after template name");
    
    // 解析元素内容
    while (!isAtEnd() && !check(TokenType::RBRACE)) {
        if (check(TokenType::IDENTIFIER)) {
            elementTemplate->addChild(parseElement());
        } else if (check(TokenType::AT_ELEMENT)) {
            elementTemplate->addChild(parseTemplateUse());
        } else {
            error("Unexpected token in element template: " + peek().value);
            advance();
        }
    }
    
    consume(TokenType::RBRACE, "Expected '}' after template content");
    
    return elementTemplate;
}

// 变量组模板解析
ASTNodePtr Parser::parseVarTemplate() {
    Token atVar = consume(TokenType::AT_VAR, "Expected '@Var'");
    Token name = consume(TokenType::IDENTIFIER, "Expected template name");
    
    auto varTemplate = std::make_shared<VarTemplateNode>(atVar);
    varTemplate->setName(name.value);
    
    consume(TokenType::LBRACE, "Expected '{' after template name");
    
    // 解析变量定义
    while (!isAtEnd() && !check(TokenType::RBRACE)) {
        if (check(TokenType::IDENTIFIER)) {
            // 变量定义 name: value;
            auto varDef = parseAttribute(); // 复用属性解析
            varTemplate->addChild(varDef);
        } else {
            error("Unexpected token in var template: " + peek().value);
            advance();
        }
    }
    
    consume(TokenType::RBRACE, "Expected '}' after template content");
    
    return varTemplate;
}

// 自定义解析
ASTNodePtr Parser::parseCustom() {
    consume(TokenType::CUSTOM_MARK, "Expected '[Custom]'");
    
    if (check(TokenType::AT_STYLE)) {
        return parseCustomStyle();
    } else if (check(TokenType::AT_ELEMENT)) {
        return parseCustomElement();
    } else if (check(TokenType::AT_VAR)) {
        return parseCustomVar();
    }
    
    error("Expected @Style, @Element, or @Var after [Custom]");
    return nullptr;
}

// 自定义样式组解析
ASTNodePtr Parser::parseCustomStyle() {
    Token atStyle = consume(TokenType::AT_STYLE, "Expected '@Style'");
    Token name = consume(TokenType::IDENTIFIER, "Expected custom name");
    
    auto customStyle = std::make_shared<CustomStyleNode>(atStyle);
    customStyle->setName(name.value);
    
    consume(TokenType::LBRACE, "Expected '{' after custom name");
    
    // 解析内容
    while (!isAtEnd() && !check(TokenType::RBRACE)) {
        if (check(TokenType::AT_STYLE)) {
            customStyle->addChild(parseTemplateUse());
        } else if (check(TokenType::INHERIT)) {
            customStyle->addChild(parseInherit());
        } else if (check(TokenType::IDENTIFIER)) {
            Token next = peek(1);
            if (next.type == TokenType::SEMICOLON) {
                // 无值属性
                Token prop = peek();
                advance();
                advance(); // 跳过分号
                
                auto property = std::make_shared<StylePropertyNode>(prop);
                property->setProperty(prop.value);
                property->setValue(""); // 无值
                customStyle->addChild(property);
            } else {
                // 普通属性
                customStyle->addChild(parseStyleProperty());
            }
        } else {
            error("Unexpected token in custom style: " + peek().value);
            advance();
        }
    }
    
    consume(TokenType::RBRACE, "Expected '}' after custom content");
    
    return customStyle;
}

// 自定义元素解析
ASTNodePtr Parser::parseCustomElement() {
    Token atElement = consume(TokenType::AT_ELEMENT, "Expected '@Element'");
    Token name = consume(TokenType::IDENTIFIER, "Expected custom name");
    
    auto customElement = std::make_shared<CustomElementNode>(atElement);
    customElement->setName(name.value);
    
    consume(TokenType::LBRACE, "Expected '{' after custom name");
    
    // 解析内容
    while (!isAtEnd() && !check(TokenType::RBRACE)) {
        if (check(TokenType::DELETE)) {
            customElement->addChild(parseDelete());
        } else if (check(TokenType::INSERT)) {
            customElement->addChild(parseInsert());
        } else if (check(TokenType::IDENTIFIER)) {
            customElement->addChild(parseElement());
        } else if (check(TokenType::AT_ELEMENT)) {
            customElement->addChild(parseTemplateUse());
        } else {
            error("Unexpected token in custom element: " + peek().value);
            advance();
        }
    }
    
    consume(TokenType::RBRACE, "Expected '}' after custom content");
    
    return customElement;
}

// 自定义变量组解析
ASTNodePtr Parser::parseCustomVar() {
    Token atVar = consume(TokenType::AT_VAR, "Expected '@Var'");
    Token name = consume(TokenType::IDENTIFIER, "Expected custom name");
    
    auto customVar = std::make_shared<CustomVarNode>(atVar);
    customVar->setName(name.value);
    
    consume(TokenType::LBRACE, "Expected '{' after custom name");
    
    // 解析内容
    while (!isAtEnd() && !check(TokenType::RBRACE)) {
        if (check(TokenType::IDENTIFIER)) {
            customVar->addChild(parseAttribute());
        } else {
            error("Unexpected token in custom var: " + peek().value);
            advance();
        }
    }
    
    consume(TokenType::RBRACE, "Expected '}' after custom content");
    
    return customVar;
}

// 模板使用解析
ASTNodePtr Parser::parseTemplateUse() {
    TemplateUseNode::UseType useType;
    Token typeToken = peek();
    
    if (match(TokenType::AT_STYLE)) {
        useType = TemplateUseNode::STYLE;
    } else if (match(TokenType::AT_ELEMENT)) {
        useType = TemplateUseNode::ELEMENT;
    } else {
        error("Expected @Style or @Element");
        return nullptr;
    }
    
    Token name = consume(TokenType::IDENTIFIER, "Expected template name");
    
    auto templateUse = std::make_shared<TemplateUseNode>(typeToken, useType);
    templateUse->setTemplateName(name.value);
    
    // 检查是否有参数（用于自定义特例化）
    if (match(TokenType::LPAREN)) {
        while (!isAtEnd() && !check(TokenType::RPAREN)) {
            Token paramName = consume(TokenType::IDENTIFIER, "Expected parameter name");
            consume(TokenType::COLON, "Expected ':' after parameter name");
            
            // 解析参数值
            ASTNodePtr value;
            if (check(TokenType::STRING_DOUBLE_QUOTE) || 
                check(TokenType::STRING_SINGLE_QUOTE)) {
                Token str = peek();
                advance();
                value = std::make_shared<StringLiteralNode>(
                    str,
                    str.type == TokenType::STRING_DOUBLE_QUOTE ? 
                        StringLiteralNode::DOUBLE_QUOTE : 
                        StringLiteralNode::SINGLE_QUOTE
                );
            } else {
                Token val = peek();
                advance();
                value = std::make_shared<StringLiteralNode>(val, StringLiteralNode::UNQUOTED);
            }
            
            templateUse->addParameter(paramName.value, value);
            
            if (!match(TokenType::COMMA)) {
                break;
            }
        }
        consume(TokenType::RPAREN, "Expected ')' after parameters");
    }
    
    consume(TokenType::SEMICOLON, "Expected ';' after template use");
    
    return templateUse;
}

// 变量使用解析（ThemeColor(tableColor)）
ASTNodePtr Parser::parseVarUse() {
    Token groupName = consume(TokenType::IDENTIFIER, "Expected variable group name");
    auto varUse = std::make_shared<VarUseNode>(groupName);
    varUse->setVarGroupName(groupName.value);
    
    consume(TokenType::LPAREN, "Expected '(' after variable group name");
    
    Token varName = consume(TokenType::IDENTIFIER, "Expected variable name");
    varUse->setVariableName(varName.value);
    
    consume(TokenType::RPAREN, "Expected ')' after variable name");
    
    return varUse;
}

// 继承语句解析
ASTNodePtr Parser::parseInherit() {
    Token inheritToken = consume(TokenType::INHERIT, "Expected 'inherit'");
    auto inherit = std::make_shared<InheritStatementNode>(inheritToken);
    
    // 解析继承类型
    if (match(TokenType::AT_STYLE)) {
        inherit->setInheritType(InheritStatementNode::STYLE);
    } else if (match(TokenType::AT_ELEMENT)) {
        inherit->setInheritType(InheritStatementNode::ELEMENT);
    } else if (match(TokenType::AT_VAR)) {
        inherit->setInheritType(InheritStatementNode::VAR);
    } else {
        error("Expected @Style, @Element, or @Var after inherit");
    }
    
    Token name = consume(TokenType::IDENTIFIER, "Expected name to inherit");
    inherit->setInheritName(name.value);
    
    consume(TokenType::SEMICOLON, "Expected ';' after inherit statement");
    
    return inherit;
}

// 删除语句解析
ASTNodePtr Parser::parseDelete() {
    Token deleteToken = consume(TokenType::DELETE, "Expected 'delete'");
    auto deleteStmt = std::make_shared<DeleteStatementNode>(deleteToken);
    
    // 解析删除目标列表
    do {
        Token target = consume(TokenType::IDENTIFIER, "Expected delete target");
        deleteStmt->addTarget(target.value);
    } while (match(TokenType::COMMA));
    
    consume(TokenType::SEMICOLON, "Expected ';' after delete statement");
    
    return deleteStmt;
}

// 插入语句解析
ASTNodePtr Parser::parseInsert() {
    Token insertToken = consume(TokenType::INSERT, "Expected 'insert'");
    auto insert = std::make_shared<InsertStatementNode>(insertToken);
    
    // 解析位置
    if (match(TokenType::AFTER)) {
        insert->setPosition(InsertStatementNode::AFTER);
    } else if (match(TokenType::BEFORE)) {
        insert->setPosition(InsertStatementNode::BEFORE);
    } else if (match(TokenType::REPLACE)) {
        insert->setPosition(InsertStatementNode::REPLACE);
    } else if (match(TokenType::AT_TOP)) {
        insert->setPosition(InsertStatementNode::AT_TOP);
    } else if (match(TokenType::AT_BOTTOM)) {
        insert->setPosition(InsertStatementNode::AT_BOTTOM);
    } else {
        error("Expected position (after/before/replace/at top/at bottom) after insert");
    }
    
    // 解析目标元素和索引
    if (insert->getPosition() == InsertStatementNode::AFTER || 
        insert->getPosition() == InsertStatementNode::BEFORE ||
        insert->getPosition() == InsertStatementNode::REPLACE) {
        Token element = consume(TokenType::IDENTIFIER, "Expected element name");
        insert->setTargetElement(element.value);
        
        // 检查索引 [n]
        if (match(TokenType::LBRACKET)) {
            Token index = consume(TokenType::NUMBER, "Expected index number");
            insert->setIndex(std::stoi(index.value));
            consume(TokenType::RBRACKET, "Expected ']' after index");
        }
    }
    
    // 解析要插入的内容
    consume(TokenType::LBRACE, "Expected '{' before insert content");
    
    if (check(TokenType::IDENTIFIER)) {
        insert->setContent(parseElement());
    }
    
    consume(TokenType::RBRACE, "Expected '}' after insert content");
    
    return insert;
}

// 原始嵌入解析
ASTNodePtr Parser::parseOrigin() {
    Token originToken = consume(TokenType::ORIGIN_MARK, "Expected '[Origin]'");
    auto origin = std::make_shared<OriginBlockNode>(originToken);
    
    // 解析类型
    if (match(TokenType::AT_HTML)) {
        origin->setOriginType(OriginBlockNode::HTML);
    } else if (match(TokenType::AT_STYLE)) {
        origin->setOriginType(OriginBlockNode::STYLE);
    } else if (match(TokenType::AT_JAVASCRIPT)) {
        origin->setOriginType(OriginBlockNode::JAVASCRIPT);
    } else if (check(TokenType::IDENTIFIER)) {
        // 自定义类型
        Token customType = peek();
        advance();
        origin->setOriginType(OriginBlockNode::CUSTOM);
        origin->setCustomTypeName(customType.value);
    } else {
        error("Expected origin type after [Origin]");
    }
    
    // 可选名称
    if (check(TokenType::IDENTIFIER) && peek(1).type == TokenType::LBRACE) {
        Token name = peek();
        advance();
        origin->setName(name.value);
    }
    
    consume(TokenType::LBRACE, "Expected '{' after origin type");
    
    // 收集原始内容
    std::string content;
    int braceCount = 1;
    
    while (!isAtEnd() && braceCount > 0) {
        Token token = peek();
        
        if (token.type == TokenType::LBRACE) {
            braceCount++;
        } else if (token.type == TokenType::RBRACE) {
            braceCount--;
            if (braceCount == 0) break;
        }
        
        content += token.value;
        if (token.type != TokenType::EOF_TOKEN && 
            token.type != TokenType::NEWLINE) {
            content += " ";
        }
        advance();
    }
    
    origin->setContent(content);
    
    consume(TokenType::RBRACE, "Expected '}' after origin content");
    
    return origin;
}

// 配置解析
ASTNodePtr Parser::parseConfiguration() {
    Token configToken = consume(TokenType::CONFIGURATION_MARK, "Expected '[Configuration]'");
    auto config = std::make_shared<ConfigurationBlockNode>(configToken);
    
    consume(TokenType::LBRACE, "Expected '{' after [Configuration]");
    
    // 解析配置项
    while (!isAtEnd() && !check(TokenType::RBRACE)) {
        Token key = consume(TokenType::IDENTIFIER, "Expected configuration key");
        consume(TokenType::COLON, "Expected ':' after key");
        
        // 解析配置值
        ASTNodePtr value;
        if (check(TokenType::STRING_DOUBLE_QUOTE) || 
            check(TokenType::STRING_SINGLE_QUOTE)) {
            Token str = peek();
            advance();
            value = std::make_shared<StringLiteralNode>(
                str,
                str.type == TokenType::STRING_DOUBLE_QUOTE ? 
                    StringLiteralNode::DOUBLE_QUOTE : 
                    StringLiteralNode::SINGLE_QUOTE
            );
        } else if (check(TokenType::NUMBER)) {
            Token num = peek();
            advance();
            value = std::make_shared<NumberLiteralNode>(num);
        } else if (check(TokenType::IDENTIFIER)) {
            Token id = peek();
            advance();
            value = std::make_shared<IdentifierNode>(id);
        }
        
        config->addConfigItem(key.value, value);
        
        consume(TokenType::SEMICOLON, "Expected ';' after configuration value");
    }
    
    consume(TokenType::RBRACE, "Expected '}' after configuration items");
    
    return config;
}

// 导入语句解析
ASTNodePtr Parser::parseImport() {
    Token importToken = consume(TokenType::IMPORT_MARK, "Expected '[Import]'");
    auto import = std::make_shared<ImportStatementNode>(importToken);
    
    // 解析导入类型
    if (match(TokenType::AT_HTML)) {
        import->setImportType(ImportStatementNode::HTML);
    } else if (match(TokenType::AT_STYLE)) {
        import->setImportType(ImportStatementNode::STYLE);
    } else if (match(TokenType::AT_JAVASCRIPT)) {
        import->setImportType(ImportStatementNode::JAVASCRIPT);
    } else if (match(TokenType::AT_CHTL)) {
        import->setImportType(ImportStatementNode::CHTL);
    } else if (match(TokenType::AT_CJMOD)) {
        import->setImportType(ImportStatementNode::CJMOD);
    } else {
        error("Expected import type after [Import]");
    }
    
    // 对于CHTL导入，可能有特定项
    if (import->getImportType() == ImportStatementNode::CHTL && 
        check(TokenType::LBRACKET)) {
        advance();
        Token targetType = consume(TokenType::IDENTIFIER, "Expected target type");
        import->setTargetType(targetType.value);
        consume(TokenType::RBRACKET, "Expected ']'");
        
        if (check(TokenType::AT_STYLE) || check(TokenType::AT_ELEMENT) || check(TokenType::AT_VAR)) {
            Token type = peek();
            advance();
            import->setTargetType(import->getTargetType() + " " + type.value);
        }
        
        Token targetName = consume(TokenType::IDENTIFIER, "Expected target name");
        import->setTargetName(targetName.value);
    }
    
    // from 路径
    consume(TokenType::FROM, "Expected 'from' in import");
    
    Token path = consume(TokenType::STRING_DOUBLE_QUOTE, "Expected import path");
    import->setPath(path.value);
    
    // 可选 as 别名
    if (match(TokenType::AS)) {
        Token alias = consume(TokenType::IDENTIFIER, "Expected alias name");
        import->setAlias(alias.value);
    }
    
    return import;
}

// 命名空间解析
ASTNodePtr Parser::parseNamespace() {
    Token nsToken = consume(TokenType::NAMESPACE_MARK, "Expected '[Namespace]'");
    Token name = consume(TokenType::IDENTIFIER, "Expected namespace name");
    
    auto ns = std::make_shared<NamespaceBlockNode>(nsToken);
    ns->setName(name.value);
    
    consume(TokenType::LBRACE, "Expected '{' after namespace name");
    
    // 解析命名空间内容
    while (!isAtEnd() && !check(TokenType::RBRACE)) {
        auto node = parseTopLevel();
        if (node) {
            ns->addChild(node);
        }
    }
    
    consume(TokenType::RBRACE, "Expected '}' after namespace content");
    
    return ns;
}

// 其他未实现的占位方法
ASTNodePtr Parser::parseName() {
    // TODO: 实现[Name]解析
    error("[Name] parsing not implemented yet");
    return nullptr;
}

ASTNodePtr Parser::parseOriginType() {
    // TODO: 实现[OriginType]解析
    error("[OriginType] parsing not implemented yet");
    return nullptr;
}

ASTNodePtr Parser::parseExport() {
    // TODO: 实现[Export]解析
    error("[Export] parsing not implemented yet");
    return nullptr;
}

} // namespace Chtl
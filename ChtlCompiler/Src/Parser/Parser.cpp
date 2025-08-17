#include "Parser/Parser.h"
#include <fstream>
#include <sstream>
#include <filesystem>

namespace Chtl {

const std::string Parser::emptyError_ = "";

Parser::Parser() 
    : current_(0),
      hasError_(false) {
    lexer_ = std::make_unique<Lexer>();
    context_ = std::make_shared<Context>();
    stateManager_ = std::make_unique<StateManager>(context_.get());
    parseHelper_ = std::make_unique<ParseContextHelper>(stateManager_.get(), context_.get());
}

Parser::~Parser() {
}

ASTNodePtr Parser::parse(const std::string& source, const std::string& fileName) {
    // 重置状态
    current_ = 0;
    hasError_ = false;
    errors_.clear();
    tokens_.clear();
    
    // 重置上下文
    context_->reset();
    
    // 词法分析
    lexer_->setSource(source, fileName);
    tokens_ = lexer_->tokenize();
    
    if (lexer_->hasError()) {
        error("Lexical error: " + lexer_->getLastError());
        return nullptr;
    }
    
    // 语法分析
    return parseProgram();
}

ASTNodePtr Parser::parseFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        error("Cannot open file: " + filePath);
        return nullptr;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return parse(buffer.str(), filePath);
}

// Token管理方法
void Parser::advance() {
    if (!isAtEnd()) {
        current_++;
        parseHelper_->updatePosition(peek());
    }
}

Token Parser::peek(int offset) {
    size_t pos = current_ + offset;
    if (pos >= tokens_.size()) {
        return tokens_.back(); // 返回EOF token
    }
    return tokens_[pos];
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        Token token = peek();
        advance();
        return token;
    }
    
    parseHelper_->reportError(message, peek());
    return peek();
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::EOF_TOKEN;
}

// 程序解析
ASTNodePtr Parser::parseProgram() {
    auto program = std::make_shared<ProgramNode>();
    
    // 进入程序作用域
    auto scopeGuard = stateManager_->enterScope("program");
    
    while (!isAtEnd()) {
        auto node = parseTopLevel();
        if (node) {
            program->addChild(node);
        }
        
        if (hasError_) {
            synchronize();
            stateManager_->recoverToStableState();
        }
    }
    
    return program;
}

// 顶层解析
ASTNodePtr Parser::parseTopLevel() {
    // [Template]
    if (check(TokenType::TEMPLATE_MARK)) {
        return parseTemplate();
    }
    
    // [Custom]
    if (check(TokenType::CUSTOM_MARK)) {
        return parseCustom();
    }
    
    // [Origin]
    if (check(TokenType::ORIGIN_MARK)) {
        return parseOrigin();
    }
    
    // [Configuration]
    if (check(TokenType::CONFIGURATION_MARK)) {
        return parseConfiguration();
    }
    
    // [Name]
    if (check(TokenType::NAME_MARK)) {
        return parseName();
    }
    
    // [OriginType]
    if (check(TokenType::ORIGIN_TYPE_MARK)) {
        return parseOriginType();
    }
    
    // [Import]
    if (check(TokenType::IMPORT_MARK)) {
        return parseImport();
    }
    
    // [Export]
    if (check(TokenType::EXPORT_MARK)) {
        return parseExport();
    }
    
    // [Namespace]
    if (check(TokenType::NAMESPACE_MARK)) {
        return parseNamespace();
    }
    
    // 普通元素
    if (check(TokenType::IDENTIFIER)) {
        return parseElement();
    }
    
    // 注释
    if (check(TokenType::COMMENT_SINGLE) || 
        check(TokenType::COMMENT_MULTI) || 
        check(TokenType::COMMENT_GENERATOR)) {
        Token comment = peek();
        advance();
        auto node = std::make_shared<CommentNode>(
            comment,
            comment.type == TokenType::COMMENT_SINGLE ? CommentNode::SINGLE :
            comment.type == TokenType::COMMENT_MULTI ? CommentNode::MULTI :
            CommentNode::GENERATOR
        );
        node->setContent(comment.value);
        return node;
    }
    
    error("Unexpected token: " + peek().value);
    advance();
    return nullptr;
}

// 元素解析
ASTNodePtr Parser::parseElement() {
    Token tagName = consume(TokenType::IDENTIFIER, "Expected element name");
    auto element = std::make_shared<ElementNode>(tagName);
    
    // 使用RAII元素守卫
    auto elementGuard = stateManager_->enterElement(tagName.value);
    
    consume(TokenType::LBRACE, "Expected '{' after element name");
    
    // 解析元素内容
    while (!isAtEnd() && !check(TokenType::RBRACE)) {
        // 属性（id: value; 或 class = value;）
        if (check(TokenType::IDENTIFIER)) {
            Token next = peek(1);
            if (next.type == TokenType::COLON || next.type == TokenType::EQUALS) {
                element->addChild(parseAttribute());
                
                // 更新元素状态
                auto attr = element->getAttributes().back();
                if (attr) {
                    auto attrNode = std::static_pointer_cast<AttributeNode>(attr);
                    if (attrNode->getName() == "id") {
                        elementGuard.getElement().id = attrNode->getValue();
                    } else if (attrNode->getName() == "class") {
                        // 分割类名
                        std::stringstream ss(attrNode->getValue());
                        std::string className;
                        while (ss >> className) {
                            elementGuard.getElement().classes.push_back(className);
                        }
                    }
                }
                continue;
            }
        }
        
        // text块
        if (check(TokenType::TEXT)) {
            auto textGuard = parseHelper_->enterTextBlock();
            element->addChild(parseText());
            continue;
        }
        
        // style块
        if (check(TokenType::STYLE)) {
            auto styleGuard = parseHelper_->enterStyleBlock(true);
            element->addChild(parseStyleBlock());
            continue;
        }
        
        // script块
        if (check(TokenType::SCRIPT)) {
            auto scriptGuard = parseHelper_->enterScriptBlock(true);
            element->addChild(parseScriptBlock());
            continue;
        }
        
        // 模板使用 @Style/@Element
        if (check(TokenType::AT_STYLE) || check(TokenType::AT_ELEMENT)) {
            element->addChild(parseTemplateUse());
            continue;
        }
        
        // 嵌套元素
        if (check(TokenType::IDENTIFIER)) {
            element->addChild(parseElement());
            continue;
        }
        
        // 注释
        if (check(TokenType::COMMENT_SINGLE) || 
            check(TokenType::COMMENT_MULTI) || 
            check(TokenType::COMMENT_GENERATOR)) {
            element->addChild(parseTopLevel()); // 复用注释解析
            continue;
        }
        
        error("Unexpected token in element: " + peek().value);
        advance();
    }
    
    consume(TokenType::RBRACE, "Expected '}' after element content");
    
    return element;
}

// 属性解析
ASTNodePtr Parser::parseAttribute() {
    Token name = consume(TokenType::IDENTIFIER, "Expected attribute name");
    auto attr = std::make_shared<AttributeNode>(name);
    attr->setName(name.value);
    
    // CE对等式：: 和 = 完全等价
    if (!match(TokenType::COLON) && !match(TokenType::EQUALS)) {
        error("Expected ':' or '=' after attribute name");
    }
    
    // 解析属性值
    std::string value;
    if (check(TokenType::STRING_DOUBLE_QUOTE) || 
        check(TokenType::STRING_SINGLE_QUOTE)) {
        value = parseStringValue();
    } else if (check(TokenType::NUMBER)) {
        Token num = peek();
        advance();
        value = num.value;
    } else if (check(TokenType::IDENTIFIER) || 
               check(TokenType::UNQUOTED_LITERAL)) {
        Token lit = peek();
        advance();
        value = lit.value;
    } else {
        error("Expected attribute value");
    }
    
    attr->setValue(value);
    
    consume(TokenType::SEMICOLON, "Expected ';' after attribute");
    
    return attr;
}

// 文本解析
ASTNodePtr Parser::parseText() {
    Token textToken = consume(TokenType::TEXT, "Expected 'text'");
    auto textNode = std::make_shared<TextNode>(textToken);
    
    consume(TokenType::LBRACE, "Expected '{' after 'text'");
    
    // 解析文本内容
    std::string content;
    while (!isAtEnd() && !check(TokenType::RBRACE)) {
        if (check(TokenType::STRING_DOUBLE_QUOTE) || 
            check(TokenType::STRING_SINGLE_QUOTE)) {
            content += parseStringValue();
        } else if (check(TokenType::UNQUOTED_LITERAL)) {
            content += peek().value;
            advance();
        } else if (check(TokenType::IDENTIFIER)) {
            content += peek().value;
            advance();
        } else {
            error("Unexpected token in text block: " + peek().value);
            advance();
        }
        
        // 添加空格分隔多个文本片段
        if (!isAtEnd() && !check(TokenType::RBRACE)) {
            content += " ";
        }
    }
    
    textNode->setContent(content);
    
    consume(TokenType::RBRACE, "Expected '}' after text content");
    
    return textNode;
}

// 样式块解析
ASTNodePtr Parser::parseStyleBlock() {
    Token styleToken = consume(TokenType::STYLE, "Expected 'style'");
    auto styleBlock = std::make_shared<StyleBlockNode>(styleToken);
    
    consume(TokenType::LBRACE, "Expected '{' after 'style'");
    
    // 检查是否需要生成自动类名
    bool needsAutoClass = false;
    
    while (!isAtEnd() && !check(TokenType::RBRACE)) {
        // 内联样式属性 (property: value;)
        if (check(TokenType::IDENTIFIER)) {
            Token next = peek(1);
            if (next.type == TokenType::COLON) {
                styleBlock->addChild(parseStyleProperty());
                continue;
            }
        }
        
        // CSS选择器 (.class, #id, &, &:hover等)
        if (check(TokenType::DOT) || 
            check(TokenType::IDENTIFIER) ||  // #id会被识别为IDENTIFIER
            check(TokenType::AMPERSAND)) {
            
            // 检查是否需要自动类名
            if (!needsAutoClass) {
                Token selectorStart = peek();
                if (selectorStart.type == TokenType::DOT || 
                    selectorStart.type == TokenType::AMPERSAND) {
                    needsAutoClass = true;
                    std::string autoClass = parseHelper_->generateAutoClassIfNeeded();
                    parseHelper_->setAutoClassName(autoClass);
                }
            }
            
            styleBlock->addChild(parseStyleRule());
            continue;
        }
        
        // 模板使用 @Style
        if (check(TokenType::AT_STYLE)) {
            styleBlock->addChild(parseTemplateUse());
            continue;
        }
        
        error("Unexpected token in style block: " + peek().value);
        advance();
    }
    
    consume(TokenType::RBRACE, "Expected '}' after style content");
    
    return styleBlock;
}

// CSS规则解析
ASTNodePtr Parser::parseStyleRule() {
    auto rule = std::make_shared<StyleRuleNode>(peek());
    
    // 解析选择器
    auto selector = parseStyleSelector();
    rule->setSelector(selector);
    rule->addChild(selector);
    
    // 通知上下文开始CSS规则
    parseHelper_->beginCSSRule(
        std::static_pointer_cast<StyleSelectorNode>(selector)->getSelector()
    );
    
    consume(TokenType::LBRACE, "Expected '{' after selector");
    
    // 解析属性列表
    while (!isAtEnd() && !check(TokenType::RBRACE)) {
        rule->addChild(parseStyleProperty());
    }
    
    consume(TokenType::RBRACE, "Expected '}' after style rule");
    
    parseHelper_->endCSSRule();
    
    return rule;
}

// CSS选择器解析
ASTNodePtr Parser::parseStyleSelector() {
    Token selectorToken = peek();
    StyleSelectorNode::SelectorType type = StyleSelectorNode::TAG;
    std::string selector;
    
    if (match(TokenType::DOT)) {
        // 类选择器 .className
        type = StyleSelectorNode::CLASS;
        Token className = consume(TokenType::IDENTIFIER, "Expected class name after '.'");
        selector = "." + className.value;
    } else if (check(TokenType::IDENTIFIER) && peek().value.starts_with("#")) {
        // ID选择器 #id
        type = StyleSelectorNode::ID;
        selector = peek().value;
        advance();
    } else if (match(TokenType::AMPERSAND)) {
        // 上下文推导 &
        type = StyleSelectorNode::CONTEXT;
        selector = "&";
        
        // 检查是否是伪类 &:hover 或伪元素 &::before
        if (match(TokenType::COLON)) {
            if (match(TokenType::COLON)) {
                // 伪元素 &::before
                type = StyleSelectorNode::PSEUDO_ELEMENT;
                Token pseudo = consume(TokenType::IDENTIFIER, "Expected pseudo-element name");
                selector += "::" + pseudo.value;
            } else {
                // 伪类 &:hover
                type = StyleSelectorNode::PSEUDO_CLASS;
                Token pseudo = consume(TokenType::IDENTIFIER, "Expected pseudo-class name");
                selector += ":" + pseudo.value;
            }
        }
    } else if (check(TokenType::IDENTIFIER)) {
        // 标签选择器或复合选择器
        selector = peek().value;
        advance();
        
        // 检查是否有更多选择器部分（复合选择器）
        while (!isAtEnd() && 
               (check(TokenType::DOT) || 
                check(TokenType::IDENTIFIER) || 
                check(TokenType::LBRACKET))) {
            if (match(TokenType::DOT)) {
                Token className = consume(TokenType::IDENTIFIER, "Expected class name");
                selector += "." + className.value;
                type = StyleSelectorNode::COMPLEX;
            } else if (check(TokenType::IDENTIFIER)) {
                selector += " " + peek().value;
                advance();
                type = StyleSelectorNode::COMPLEX;
            } else if (match(TokenType::LBRACKET)) {
                // 属性选择器等
                selector += "[";
                while (!isAtEnd() && !check(TokenType::RBRACKET)) {
                    selector += peek().value;
                    advance();
                }
                consume(TokenType::RBRACKET, "Expected ']'");
                selector += "]";
                type = StyleSelectorNode::COMPLEX;
            }
        }
    }
    
    auto selectorNode = std::make_shared<StyleSelectorNode>(selectorToken, type);
    selectorNode->setSelector(selector);
    
    // 处理局部样式上下文
    if (stateManager_->isInLocalContext()) {
        parseHelper_->processLocalStyleContext(selector);
    }
    
    return selectorNode;
}

// CSS属性解析
ASTNodePtr Parser::parseStyleProperty() {
    Token propName = consume(TokenType::IDENTIFIER, "Expected property name");
    auto property = std::make_shared<StylePropertyNode>(propName);
    property->setProperty(propName.value);
    
    consume(TokenType::COLON, "Expected ':' after property name");
    
    // 解析属性值
    std::string value;
    while (!isAtEnd() && !check(TokenType::SEMICOLON)) {
        if (check(TokenType::STRING_DOUBLE_QUOTE) || 
            check(TokenType::STRING_SINGLE_QUOTE)) {
            value += parseStringValue();
        } else if (check(TokenType::LPAREN)) {
            // 处理函数调用，如 rgb(255, 192, 203)
            value += peek().value;
            advance();
            int parenDepth = 1;
            
            while (!isAtEnd() && parenDepth > 0) {
                if (match(TokenType::LPAREN)) {
                    parenDepth++;
                    value += "(";
                } else if (match(TokenType::RPAREN)) {
                    parenDepth--;
                    value += ")";
                } else {
                    // 检查变量使用
                    if (check(TokenType::IDENTIFIER) && peek(1).type == TokenType::LPAREN) {
                        // 可能是变量使用 ThemeColor(tableColor)
                        std::string varGroupName = peek().value;
                        advance();
                        advance(); // 跳过 (
                        
                        if (check(TokenType::IDENTIFIER)) {
                            std::string varName = peek().value;
                            advance();
                            
                            if (match(TokenType::RPAREN)) {
                                // 确实是变量使用，进行替换
                                std::string resolvedValue = 
                                    parseHelper_->resolveVariable(varGroupName, varName);
                                if (!resolvedValue.empty()) {
                                    value += resolvedValue;
                                } else {
                                    value += varGroupName + "(" + varName + ")";
                                }
                                continue;
                            }
                        }
                        
                        // 不是变量使用，回退
                        value += varGroupName + "(";
                    } else {
                        value += peek().value;
                        advance();
                    }
                }
            }
        } else {
            value += peek().value;
            advance();
        }
        
        // 添加空格分隔多个值
        if (!isAtEnd() && !check(TokenType::SEMICOLON) && !check(TokenType::LPAREN)) {
            value += " ";
        }
    }
    
    property->setValue(value);
    
    consume(TokenType::SEMICOLON, "Expected ';' after property value");
    
    return property;
}

// 字符串值解析
std::string Parser::parseStringValue() {
    if (check(TokenType::STRING_DOUBLE_QUOTE) || 
        check(TokenType::STRING_SINGLE_QUOTE)) {
        Token str = peek();
        advance();
        return str.value;
    }
    
    error("Expected string value");
    return "";
}

// 错误处理
void Parser::error(const std::string& message) {
    hasError_ = true;
    errors_.push_back(message);
    parseHelper_->reportError(message, peek());
}

void Parser::synchronize() {
    advance();
    
    while (!isAtEnd()) {
        // 同步到下一个语句开始
        if (peek().type == TokenType::RBRACE) {
            return;
        }
        
        switch (peek().type) {
            case TokenType::TEMPLATE_MARK:
            case TokenType::CUSTOM_MARK:
            case TokenType::ORIGIN_MARK:
            case TokenType::IMPORT_MARK:
            case TokenType::EXPORT_MARK:
            case TokenType::NAMESPACE_MARK:
            case TokenType::IDENTIFIER:
                return;
            default:
                advance();
        }
    }
}

} // namespace Chtl
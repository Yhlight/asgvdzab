#include "compilers/chtl_compiler.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

namespace chtl {

// 编译器内部AST节点实现 (Legacy)
std::string CompilerElementNode::toString() const {
    std::stringstream ss;
    ss << "<" << tagName;
    for (const auto& attr : attributes) {
        ss << " " << attr.first << "=\"" << attr.second << "\"";
    }
    if (children.empty() && localStyle.empty() && localScript.empty()) {
        ss << "/>";
    } else {
        ss << ">";
        for (const auto& child : children) {
            ss << child->toString();
        }
        ss << "</" << tagName << ">";
    }
    return ss.str();
}

std::string CompilerTextNode::toString() const {
    return content;
}

std::string CompilerTemplateNode::toString() const {
    return "[Template] " + templateType + " " + name + " { " + content + " }";
}

std::string CompilerCustomNode::toString() const {
    return "[Custom] " + customType + " " + name + " { " + content + " }";
}

// CHTLCompiler实现
CHTLCompiler::CHTLCompiler() : classCounter_(0) {
}

CompileResult CHTLCompiler::compile(const CodeSegment& segment, const CompileConfig& config) {
    config_ = config;
    state_ = ParserState{};
    generatedCSS_.clear();
    generatedJS_.clear();
    classCounter_ = 0;
    
    try {
        // 特殊处理TEXT类型
        if (segment.type == CodeSegmentType::CHTL_TEXT) {
            // 直接处理文本内容
            std::string content = segment.content;
            // 移除text{}包装
            size_t start = content.find('{');
            size_t end = content.rfind('}');
            if (start != std::string::npos && end != std::string::npos && end > start) {
                content = content.substr(start + 1, end - start - 1);
                // 移除前后空白
                content.erase(0, content.find_first_not_of(" \t\n\r"));
                content.erase(content.find_last_not_of(" \t\n\r") + 1);
                // 移除引号
                if ((content.front() == '"' && content.back() == '"') ||
                    (content.front() == '\'' && content.back() == '\'')) {
                    content = content.substr(1, content.length() - 2);
                }
            }
            
            state_.result.output = content;
            state_.result.metadata["type"] = "html";
            return state_.result;
        }
        
        // 根据segment的上下文重建完整的源代码
        std::string fullSource;
        if (!segment.context.empty()) {
            fullSource = segment.context + "\n{\n" + segment.content + "\n}";
        } else {
            fullSource = segment.content;
        }
        
        state_.source = fullSource;
        auto ast = parse(fullSource);
        if (ast && state_.result.success) {
            state_.result.output = generateHTML(ast.get());
            
            // 设置元数据
            state_.result.metadata["type"] = "html";
            if (!generatedCSS_.empty()) {
                state_.result.metadata["global_css"] = generatedCSS_;
            }
            if (!generatedJS_.empty()) {
                state_.result.metadata["global_js"] = generatedJS_;
            }
        }
    } catch (const std::exception& e) {
        reportError("编译异常: " + std::string(e.what()));
    }
    
    return state_.result;
}

std::string CHTLCompiler::getName() const {
    return "CHTL Compiler";
}

std::vector<CodeSegmentType> CHTLCompiler::getSupportedTypes() const {
    return {CodeSegmentType::CHTL_CORE, CodeSegmentType::CSS_LOCAL_STYLE, CodeSegmentType::CHTL_TEXT};
}

void CHTLCompiler::setTemplates(const std::unordered_map<std::string, std::string>& templates) {
    templates_ = templates;
}

void CHTLCompiler::setCustoms(const std::unordered_map<std::string, std::string>& customs) {
    customs_ = customs;
}

std::unique_ptr<ASTNode> CHTLCompiler::parse(const std::string& source) {
    state_.source = source;
    state_.position = 0;
    state_.line = 1;
    state_.column = 1;
    
    skipWhitespace();
    
    if (isAtEnd()) {
        return nullptr;
    }
    
    // 根据内容判断解析什么
    if (match("text")) {
        return parseText();
    } else if (match("[Template]")) {
        return parseTemplate();
    } else if (match("[Custom]")) {
        return parseCustom();
    } else {
        return parseElement();
    }
}

std::unique_ptr<CompilerElementNode> CHTLCompiler::parseElement() {
    skipWhitespace();
    
    // 读取元素名
    std::string tagName = readIdentifier();
    if (tagName.empty()) {
        reportError("期望元素名");
        return nullptr;
    }
    
    auto element = std::make_unique<CompilerElementNode>(tagName);
    
    skipWhitespace();
    
    // 期望大括号
    if (!expect('{')) {
        reportError("期望 '{'");
        return nullptr;
    }
    
    // 解析元素内容
    while (!isAtEnd() && currentChar() != '}') {
        skipWhitespace();
        
        if (isAtEnd() || currentChar() == '}') {
            break;
        }
        
        // 检查是否为特殊块
        if (match("style")) {
            parseLocalStyle(element.get());
        } else if (match("script")) {
            parseLocalScript(element.get());
        } else if (match("text")) {
            auto textNode = parseText();
            if (textNode) {
                element->children.push_back(std::move(textNode));
            }
        } else {
            // 检查是否为属性
            size_t savedPos = state_.position;
            std::string identifier = readIdentifier();
            skipWhitespace();
            
            if (!isAtEnd() && (currentChar() == ':' || currentChar() == '=')) {
                // 这是一个属性
                char separator = currentChar();
                advance(); // 跳过 : 或 =
                skipWhitespace();
                
                std::string value;
                if (currentChar() == '"' || currentChar() == '\'') {
                    value = readStringLiteral(currentChar());
                } else {
                    value = readUnquotedLiteral(";{}");
                }
                
                element->attributes[identifier] = value;
                
                skipWhitespace();
                if (!isAtEnd() && currentChar() == ';') {
                    advance(); // 跳过分号
                }
                         } else {
                 // 回退，这可能是一个子元素
                 state_.position = savedPos;
                 
                 // 重新计算列号
                 size_t lineStart = state_.source.rfind('\n', state_.position);
                 if (lineStart == std::string::npos) {
                     state_.column = state_.position + 1;
                 } else {
                     state_.column = state_.position - lineStart;
                 }
                 
                 auto childElement = parseElement();
                 if (childElement) {
                     element->children.push_back(std::move(childElement));
                 }
             }
        }
        
        skipWhitespace();
    }
    
    if (!expect('}')) {
        reportError("期望 '}'");
        return nullptr;
    }
    
    return element;
}

std::unique_ptr<CompilerTextNode> CHTLCompiler::parseText() {
    skipWhitespace();
    
    if (!expect('{')) {
        reportError("期望 '{'");
        return nullptr;
    }
    
    std::string content;
    while (!isAtEnd() && currentChar() != '}') {
        if (currentChar() == '"' || currentChar() == '\'') {
            content += readStringLiteral(currentChar());
        } else {
            content += currentChar();
            advance();
        }
    }
    
    if (!expect('}')) {
        reportError("期望 '}'");
        return nullptr;
    }
    
    // 移除前后空白
    content.erase(0, content.find_first_not_of(" \t\n\r"));
    content.erase(content.find_last_not_of(" \t\n\r") + 1);
    
    return std::make_unique<CompilerTextNode>(content);
}

std::unique_ptr<CompilerTemplateNode> CHTLCompiler::parseTemplate() {
    skipWhitespace();
    
    std::string templateType = readIdentifier();
    if (templateType.empty()) {
        reportError("期望模板类型");
        return nullptr;
    }
    
    skipWhitespace();
    
    std::string name = readIdentifier();
    if (name.empty()) {
        reportError("期望模板名称");
        return nullptr;
    }
    
    skipWhitespace();
    
    std::string content = readBlock();
    
    auto templateNode = std::make_unique<CompilerTemplateNode>(templateType, name);
    templateNode->content = content;
    
    // 存储模板
    templates_[templateType + "_" + name] = content;
    
    return templateNode;
}

std::unique_ptr<CompilerCustomNode> CHTLCompiler::parseCustom() {
    skipWhitespace();
    
    std::string customType = readIdentifier();
    if (customType.empty()) {
        reportError("期望自定义类型");
        return nullptr;
    }
    
    skipWhitespace();
    
    std::string name = readIdentifier();
    if (name.empty()) {
        reportError("期望自定义名称");
        return nullptr;
    }
    
    skipWhitespace();
    
    std::string content = readBlock();
    
    auto customNode = std::make_unique<CompilerCustomNode>(customType, name);
    customNode->content = content;
    
    // 存储自定义
    customs_[customType + "_" + name] = content;
    
    return customNode;
}

void CHTLCompiler::parseLocalStyle(CompilerElementNode* element) {
    skipWhitespace();
    
    if (!expect('{')) {
        reportError("期望 '{'");
        return;
    }
    
    std::string styleContent;
    int braceCount = 1;
    
    while (!isAtEnd() && braceCount > 0) {
        char ch = currentChar();
        if (ch == '{') {
            braceCount++;
        } else if (ch == '}') {
            braceCount--;
            if (braceCount == 0) {
                advance(); // 跳过最后的 '}'
                break;
            }
        }
        styleContent += ch;
        advance();
    }
    
    element->localStyle = styleContent;
}

void CHTLCompiler::parseLocalScript(CompilerElementNode* element) {
    skipWhitespace();
    
    if (!expect('{')) {
        reportError("期望 '{'");
        return;
    }
    
    std::string scriptContent;
    int braceCount = 1;
    
    while (!isAtEnd() && braceCount > 0) {
        char ch = currentChar();
        if (ch == '{') {
            braceCount++;
        } else if (ch == '}') {
            braceCount--;
            if (braceCount == 0) {
                advance(); // 跳过最后的 '}'
                break;
            }
        }
        scriptContent += ch;
        advance();
    }
    
    element->localScript = scriptContent;
}

std::pair<std::string, std::string> CHTLCompiler::processLocalStyle(const std::string& styleContent, const std::string& elementId) {
    std::string inlineStyle;
    std::string globalStyle;
    
    std::istringstream iss(styleContent);
    std::string line;
    bool inSelector = false;
    std::string currentSelector;
    std::string currentBlock;
    
    while (std::getline(iss, line)) {
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        
        if (line.empty()) continue;
        
        // 检查是否为选择器（以 . 或 # 开头，或者包含伪类/伪元素）
        if (line.find('.') == 0 || line.find('#') == 0 || line.find('&') != std::string::npos) {
            if (inSelector && !currentBlock.empty()) {
                // 完成前一个选择器
                globalStyle += currentSelector + " {\n" + currentBlock + "}\n";
            }
            
            currentSelector = line;
            currentBlock.clear();
            inSelector = true;
            
            // 处理 & 替换
            if (currentSelector.find('&') != std::string::npos) {
                std::string className = generateUniqueClassName();
                std::regex ampersandRegex("&");
                currentSelector = std::regex_replace(currentSelector, ampersandRegex, "." + className);
                // 自动添加类名到元素
            }
        } else if (line.find('{') != std::string::npos) {
            inSelector = true;
        } else if (line.find('}') != std::string::npos) {
            if (inSelector && !currentBlock.empty()) {
                globalStyle += currentSelector + " {\n" + currentBlock + "}\n";
            }
            inSelector = false;
            currentSelector.clear();
            currentBlock.clear();
        } else if (inSelector) {
            currentBlock += "    " + line + "\n";
        } else {
            // 直接样式属性，添加到内联样式
            if (line.back() != ';') {
                line += ";";
            }
            inlineStyle += line + " ";
        }
    }
    
    // 处理最后一个选择器
    if (inSelector && !currentBlock.empty()) {
        globalStyle += currentSelector + " {\n" + currentBlock + "}\n";
    }
    
    // 移除内联样式末尾的空格
    if (!inlineStyle.empty() && inlineStyle.back() == ' ') {
        inlineStyle.pop_back();
    }
    
    return {inlineStyle, globalStyle};
}

std::string CHTLCompiler::generateHTML(const ASTNode* node) {
    if (!node) {
        return "";
    }
    
    if (auto element = dynamic_cast<const CompilerElementNode*>(node)) {
        return generateElementHTML(element);
    } else if (auto text = dynamic_cast<const CompilerTextNode*>(node)) {
        return generateTextHTML(text);
    }
    
    return "";
}

std::string CHTLCompiler::generateElementHTML(const CompilerElementNode* element) {
    std::stringstream ss;
    
    // 生成开始标签
    ss << "<" << element->tagName;
    
    // 处理局部样式
    std::string inlineStyle;
    if (!element->localStyle.empty()) {
        auto styleResult = processLocalStyle(element->localStyle, element->tagName);
        inlineStyle = styleResult.first;
        if (!styleResult.second.empty()) {
            generatedCSS_ += styleResult.second + "\n";
        }
    }
    
    // 添加属性
    for (const auto& attr : element->attributes) {
        ss << " " << attr.first << "=\"" << attr.second << "\"";
    }
    
    // 添加内联样式
    if (!inlineStyle.empty()) {
        ss << " style=\"" << inlineStyle << "\"";
    }
    
    // 检查是否为自闭合标签
    std::vector<std::string> selfClosingTags = {
        "br", "hr", "img", "input", "meta", "link", "area", "base", "col", "embed", "source", "track", "wbr"
    };
    
    bool isSelfClosing = std::find(selfClosingTags.begin(), selfClosingTags.end(), element->tagName) != selfClosingTags.end();
    
    if (isSelfClosing && element->children.empty()) {
        ss << " />";
        return ss.str();
    }
    
    ss << ">";
    
    // 生成子元素
    for (const auto& child : element->children) {
        ss << generateHTML(child.get());
    }
    
    // 处理局部脚本
    if (!element->localScript.empty()) {
        generatedJS_ += element->localScript + "\n";
    }
    
    // 生成结束标签
    ss << "</" << element->tagName << ">";
    
    return ss.str();
}

std::string CHTLCompiler::generateTextHTML(const CompilerTextNode* text) {
    return text->content;
}

std::string CHTLCompiler::generateUniqueClassName() {
    return "chtl-auto-" + std::to_string(++classCounter_);
}

char CHTLCompiler::currentChar() {
    if (isAtEnd()) {
        return '\0';
    }
    return state_.source[state_.position];
}

char CHTLCompiler::advance() {
    if (isAtEnd()) {
        return '\0';
    }
    
    char ch = state_.source[state_.position++];
    if (ch == '\n') {
        state_.line++;
        state_.column = 1;
    } else {
        state_.column++;
    }
    return ch;
}

char CHTLCompiler::peek(size_t offset) {
    size_t pos = state_.position + offset;
    if (pos >= state_.source.size()) {
        return '\0';
    }
    return state_.source[pos];
}

void CHTLCompiler::skipWhitespace() {
    while (!isAtEnd() && std::isspace(currentChar())) {
        advance();
    }
}

std::string CHTLCompiler::readIdentifier() {
    std::string identifier;
    while (!isAtEnd() && (std::isalnum(currentChar()) || currentChar() == '_' || currentChar() == '-')) {
        identifier += advance();
    }
    return identifier;
}

std::string CHTLCompiler::readStringLiteral(char delimiter) {
    std::string content;
    advance(); // 跳过开始的引号
    
    while (!isAtEnd() && currentChar() != delimiter) {
        if (currentChar() == '\\') {
            advance(); // 跳过反斜杠
            if (!isAtEnd()) {
                char escaped = advance();
                switch (escaped) {
                    case 'n': content += '\n'; break;
                    case 't': content += '\t'; break;
                    case 'r': content += '\r'; break;
                    case '\\': content += '\\'; break;
                    case '"': content += '"'; break;
                    case '\'': content += '\''; break;
                    default: content += escaped; break;
                }
            }
        } else {
            content += advance();
        }
    }
    
    if (!isAtEnd()) {
        advance(); // 跳过结束的引号
    }
    
    return content;
}

std::string CHTLCompiler::readUnquotedLiteral(const std::string& stopChars) {
    std::string content;
    while (!isAtEnd() && stopChars.find(currentChar()) == std::string::npos && !std::isspace(currentChar())) {
        content += advance();
    }
    return content;
}

std::string CHTLCompiler::readBlock() {
    if (!expect('{')) {
        return "";
    }
    
    std::string content;
    int braceCount = 1;
    
    while (!isAtEnd() && braceCount > 0) {
        char ch = currentChar();
        if (ch == '{') {
            braceCount++;
        } else if (ch == '}') {
            braceCount--;
            if (braceCount == 0) {
                advance();
                break;
            }
        }
        content += advance();
    }
    
    return content;
}

bool CHTLCompiler::match(const std::string& str) {
    if (state_.position + str.length() > state_.source.size()) {
        return false;
    }
    
    for (size_t i = 0; i < str.length(); ++i) {
        if (state_.source[state_.position + i] != str[i]) {
            return false;
        }
    }
    
    // 检查后面是否为分隔符
    size_t nextPos = state_.position + str.length();
    if (nextPos < state_.source.size()) {
        char nextChar = state_.source[nextPos];
        if (std::isalnum(nextChar) || nextChar == '_') {
            return false; // 不是完整的单词匹配
        }
    }
    
    // 匹配成功，移动位置
    for (size_t i = 0; i < str.length(); ++i) {
        advance();
    }
    
    return true;
}

bool CHTLCompiler::expect(char expected) {
    if (currentChar() == expected) {
        advance();
        return true;
    }
    return false;
}

bool CHTLCompiler::isAtEnd() {
    return state_.position >= state_.source.size();
}

void CHTLCompiler::reportError(const std::string& message) {
    state_.result.errors.push_back(getCurrentPosition() + ": " + message);
    state_.result.success = false;
}

void CHTLCompiler::reportWarning(const std::string& message) {
    state_.result.warnings.push_back(getCurrentPosition() + ": " + message);
}

std::string CHTLCompiler::getCurrentPosition() {
    return "行" + std::to_string(state_.line) + "列" + std::to_string(state_.column);
}

} // namespace chtl
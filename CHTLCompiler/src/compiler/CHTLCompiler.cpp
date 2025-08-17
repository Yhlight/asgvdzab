#include "compiler/CHTLCompiler.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <regex>
#include <unordered_set>

namespace chtl {

// 私有实现类
class CHTLCompiler::Impl {
public:
    std::unordered_map<std::string, std::string> options;
    CompileStats stats;
    std::vector<std::string> errors;
    
    // 关键字集合
    std::unordered_set<std::string> keywords;
    
    Impl() {
        stats = CompileStats{};
        
        // 初始化关键字集合
        keywords = {
            "html", "head", "body", "div", "span", "p", "h1", "h2", "h3", "h4", "h5", "h6",
            "ul", "ol", "li", "table", "tr", "td", "th", "form", "input", "button", "select",
            "textarea", "a", "img", "br", "hr", "meta", "title", "link", "script", "style",
            "template", "custom", "origin", "configuration", "import", "namespace", "text",
            "var", "element", "style", "javascript", "html", "css", "chtl", "cjmod"
        };
    }
    
    ~Impl() = default;
};

CHTLCompiler::CHTLCompiler() : pImpl(std::make_unique<Impl>()) {}

CHTLCompiler::~CHTLCompiler() = default;

CompilationResult CHTLCompiler::compile(const CodeFragment& fragment) {
    CompilationResult result;
    
    try {
        // 解析CHTL代码
        auto ast = parseCHTL(fragment.content);
        if (!ast) {
            result.success = false;
            result.errors.push_back("解析CHTL代码失败");
            return result;
        }
        
        // 生成HTML
        std::string html = generateHTML(ast.get());
        
        // 生成CSS
        std::string css = generateCSS(ast.get());
        
        // 生成JavaScript
        std::string js = generateJavaScript(ast.get());
        
        // 合并输出
        std::stringstream output;
        if (!html.empty()) {
            output << "<!-- HTML Output -->\n" << html << "\n";
        }
        if (!css.empty()) {
            output << "<!-- CSS Output -->\n" << css << "\n";
        }
        if (!js.empty()) {
            output << "<!-- JavaScript Output -->\n" << js << "\n";
        }
        
        result.success = true;
        result.output = output.str();
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back("编译异常: " + std::string(e.what()));
    }
    
    return result;
}

void CHTLCompiler::setOptions(const std::unordered_map<std::string, std::string>& options) {
    pImpl->options = options;
}

CHTLCompiler::CompileStats CHTLCompiler::getCompileStats() const {
    return pImpl->stats;
}

std::vector<std::string> CHTLCompiler::getErrors() const {
    return pImpl->errors;
}

void CHTLCompiler::clearErrors() {
    pImpl->errors.clear();
}

std::unique_ptr<CHTLASTNode> CHTLCompiler::parseCHTL(const std::string& source) {
    auto root = std::make_unique<CHTLASTNode>(CHTLNodeType::ROOT, "root");
    size_t pos = 0;
    
    // 跳过开头的空白字符
    skipWhitespace(source, pos);
    
    while (pos < source.length()) {
        // 解析元素
        if (source[pos] == '[') {
            // 解析特殊块
            if (source.substr(pos, 8) == "[Template") {
                auto node = parseTemplate(source, pos);
                if (node) root->addChild(std::move(node));
            } else if (source.substr(pos, 7) == "[Custom") {
                auto node = parseCustom(source, pos);
                if (node) root->addChild(std::move(node));
            } else if (source.substr(pos, 7) == "[Origin") {
                auto node = parseOrigin(source, pos);
                if (node) root->addChild(std::move(node));
            } else if (source.substr(pos, 7) == "[Import") {
                auto node = parseImport(source, pos);
                if (node) root->addChild(std::move(node));
            } else if (source.substr(pos, 10) == "[Namespace") {
                auto node = parseNamespace(source, pos);
                if (node) root->addChild(std::move(node));
            } else {
                addError("未知的特殊块类型");
                break;
            }
        } else if (isalpha(source[pos])) {
            // 解析普通元素
            auto node = parseElement(source, pos);
            if (node) root->addChild(std::move(node));
        } else {
            pos++;
        }
        
        skipWhitespace(source, pos);
    }
    
    return root;
}

std::unique_ptr<CHTLASTNode> CHTLCompiler::parseElement(const std::string& source, size_t& pos) {
    // 提取元素名
    std::string elementName = extractIdentifier(source, pos);
    if (elementName.empty()) {
        addError("无法解析元素名");
        return nullptr;
    }
    
    auto element = std::make_unique<CHTLASTNode>(CHTLNodeType::ELEMENT, elementName);
    pImpl->stats.elements++;
    
    skipWhitespace(source, pos);
    
    // 解析属性
    while (pos < source.length() && source[pos] != '{' && source[pos] != '\n') {
        if (isalpha(source[pos])) {
            std::string attrName = extractIdentifier(source, pos);
            skipWhitespace(source, pos);
            
            if (pos < source.length() && (source[pos] == ':' || source[pos] == '=')) {
                pos++; // 跳过:或=
                skipWhitespace(source, pos);
                
                std::string attrValue = extractString(source, pos);
                element->addAttribute(attrName, attrValue);
            }
        } else {
            pos++;
        }
        skipWhitespace(source, pos);
    }
    
    // 解析子元素
    if (pos < source.length() && source[pos] == '{') {
        pos++; // 跳过{
        
        while (pos < source.length() && source[pos] != '}') {
            skipWhitespace(source, pos);
            
            if (pos < source.length() && source[pos] == '}') break;
            
            if (source.substr(pos, 4) == "text") {
                pos += 4;
                skipWhitespace(source, pos);
                
                if (pos < source.length() && source[pos] == '{') {
                    pos++; // 跳过{
                    skipWhitespace(source, pos);
                    
                    std::string textContent;
                    while (pos < source.length() && source[pos] != '}') {
                        textContent += source[pos];
                        pos++;
                    }
                    
                    if (pos < source.length() && source[pos] == '}') {
                        pos++; // 跳过}
                        
                        auto textNode = std::make_unique<CHTLASTNode>(CHTLNodeType::TEXT, "text");
                        textNode->value = textContent;
                        element->addChild(std::move(textNode));
                        pImpl->stats.textNodes++;
                    }
                }
            } else if (source.substr(pos, 5) == "style") {
                auto styleNode = parseStyleBlock(source, pos);
                if (styleNode) element->addChild(std::move(styleNode));
            } else if (isalpha(source[pos])) {
                auto childElement = parseElement(source, pos);
                if (childElement) element->addChild(std::move(childElement));
            } else {
                pos++;
            }
            
            skipWhitespace(source, pos);
        }
        
        if (pos < source.length() && source[pos] == '}') {
            pos++; // 跳过}
        }
    }
    
    return element;
}

std::unique_ptr<CHTLASTNode> CHTLCompiler::parseStyleBlock(const std::string& source, size_t& pos) {
    if (source.substr(pos, 5) != "style") {
        return nullptr;
    }
    
    pos += 5; // 跳过"style"
    skipWhitespace(source, pos);
    
    auto styleNode = std::make_unique<CHTLASTNode>(CHTLNodeType::STYLE_BLOCK, "style");
    pImpl->stats.styleBlocks++;
    
    if (pos < source.length() && source[pos] == '{') {
        pos++; // 跳过{
        
        std::string cssContent;
        while (pos < source.length() && source[pos] != '}') {
            cssContent += source[pos];
            pos++;
        }
        
        if (pos < source.length() && source[pos] == '}') {
            pos++; // 跳过}
            styleNode->value = cssContent;
        }
    }
    
    return styleNode;
}

std::unique_ptr<CHTLASTNode> CHTLCompiler::parseTemplate(const std::string& source, size_t& pos) {
    // 简化实现，跳过模板内容
    while (pos < source.length() && source[pos] != '}') {
        pos++;
    }
    if (pos < source.length() && source[pos] == '}') {
        pos++; // 跳过}
    }
    
    pImpl->stats.templates++;
    return std::make_unique<CHTLASTNode>(CHTLNodeType::TEMPLATE, "template");
}

std::unique_ptr<CHTLASTNode> CHTLCompiler::parseCustom(const std::string& source, size_t& pos) {
    // 简化实现，跳过自定义内容
    while (pos < source.length() && source[pos] != '}') {
        pos++;
    }
    if (pos < source.length() && source[pos] == '}') {
        pos++; // 跳过}
    }
    
    pImpl->stats.customBlocks++;
    return std::make_unique<CHTLASTNode>(CHTLNodeType::CUSTOM, "custom");
}

std::unique_ptr<CHTLASTNode> CHTLCompiler::parseOrigin(const std::string& source, size_t& pos) {
    // 简化实现，跳过原始内容
    while (pos < source.length() && source[pos] != '}') {
        pos++;
    }
    if (pos < source.length() && source[pos] == '}') {
        pos++; // 跳过}
    }
    
    pImpl->stats.originBlocks++;
    return std::make_unique<CHTLASTNode>(CHTLNodeType::ORIGIN, "origin");
}

std::unique_ptr<CHTLASTNode> CHTLCompiler::parseImport(const std::string& source, size_t& pos) {
    // 简化实现，跳过导入内容
    while (pos < source.length() && source[pos] != '}') {
        pos++;
    }
    if (pos < source.length() && source[pos] == '}') {
        pos++; // 跳过}
    }
    
    pImpl->stats.imports++;
    return std::make_unique<CHTLASTNode>(CHTLNodeType::IMPORT, "import");
}

std::unique_ptr<CHTLASTNode> CHTLCompiler::parseNamespace(const std::string& source, size_t& pos) {
    // 简化实现，跳过命名空间内容
    while (pos < source.length() && source[pos] != '}') {
        pos++;
    }
    if (pos < source.length() && source[pos] == '}') {
        pos++; // 跳过}
    }
    
    pImpl->stats.namespaces++;
    return std::make_unique<CHTLASTNode>(CHTLNodeType::NAMESPACE, "namespace");
}

std::string CHTLCompiler::generateHTML(const CHTLASTNode* node) {
    if (!node) return "";
    
    std::stringstream html;
    
    if (node->type == CHTLNodeType::ELEMENT) {
        html << "<" << node->name;
        
        // 添加属性
        for (const auto& attr : node->attributes) {
            html << " " << attr.first << "=\"" << attr.second << "\"";
        }
        
        if (node->children.empty()) {
            html << " />";
        } else {
            html << ">";
            
            // 处理子节点
            for (const auto& child : node->children) {
                if (child->type == CHTLNodeType::TEXT) {
                    html << child->value;
                } else if (child->type == CHTLNodeType::ELEMENT) {
                    html << generateHTML(child.get());
                }
                // 跳过style块等非HTML内容
            }
            
            html << "</" << node->name << ">";
        }
    }
    
    return html.str();
}

std::string CHTLCompiler::generateCSS(const CHTLASTNode* node) {
    if (!node) return "";
    
    std::stringstream css;
    
    if (node->type == CHTLNodeType::ELEMENT) {
        // 为元素生成CSS选择器
        if (!node->attributes.empty()) {
            std::string selector = node->name;
            
            // 添加类选择器
            auto classIt = node->attributes.find("class");
            if (classIt != node->attributes.end()) {
                selector = "." + classIt->second;
            }
            
            // 添加ID选择器
            auto idIt = node->attributes.find("id");
            if (idIt != node->attributes.end()) {
                selector = "#" + idIt->second;
            }
            
            css << selector << " {\n";
            
            // 处理style块
            for (const auto& child : node->children) {
                if (child->type == CHTLNodeType::STYLE_BLOCK) {
                    css << "    " << child->value << "\n";
                }
            }
            
            css << "}\n\n";
        }
        
        // 递归处理子元素
        for (const auto& child : node->children) {
            css << generateCSS(child.get());
        }
    }
    
    return css.str();
}

std::string CHTLCompiler::generateJavaScript(const CHTLASTNode* node) {
    if (!node) return "";
    
    std::stringstream js;
    
    if (node->type == CHTLNodeType::ELEMENT) {
        // 处理script块
        for (const auto& child : node->children) {
            if (child->type == CHTLNodeType::STYLE_BLOCK) {
                // 这里可以添加JavaScript代码生成逻辑
                // 暂时返回空字符串
            }
        }
        
        // 递归处理子元素
        for (const auto& child : node->children) {
            js << generateJavaScript(child.get());
        }
    }
    
    return js.str();
}

void CHTLCompiler::skipWhitespace(const std::string& source, size_t& pos) {
    while (pos < source.length() && std::isspace(source[pos])) {
        pos++;
    }
}

std::string CHTLCompiler::extractIdentifier(const std::string& source, size_t& pos) {
    std::string identifier;
    while (pos < source.length() && (std::isalnum(source[pos]) || source[pos] == '_')) {
        identifier += source[pos];
        pos++;
    }
    return identifier;
}

std::string CHTLCompiler::extractString(const std::string& source, size_t& pos) {
    std::string str;
    
    if (pos < source.length() && (source[pos] == '"' || source[pos] == '\'')) {
        char quote = source[pos];
        pos++; // 跳过引号
        
        while (pos < source.length() && source[pos] != quote) {
            str += source[pos];
            pos++;
        }
        
        if (pos < source.length() && source[pos] == quote) {
            pos++; // 跳过结束引号
        }
    } else {
        // 无引号字符串
        while (pos < source.length() && !std::isspace(source[pos]) && 
               source[pos] != ';' && source[pos] != '{' && source[pos] != '}') {
            str += source[pos];
            pos++;
        }
    }
    
    return str;
}

bool CHTLCompiler::isKeyword(const std::string& word) {
    return pImpl->keywords.find(word) != pImpl->keywords.end();
}

void CHTLCompiler::addError(const std::string& error) {
    pImpl->errors.push_back(error);
}

} // namespace chtl
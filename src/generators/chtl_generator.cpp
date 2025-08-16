#include "generators/chtl_generator.hpp"
#include <sstream>
#include <algorithm>
#include <regex>

namespace chtl {

// 构造函数
CHTLGenerator::CHTLGenerator(const CHTLGeneratorConfig& config) 
    : config_(config), indentLevel_(0), classCounter_(0), idCounter_(0) {}

// 生成HTML
CHTLGenerateResult CHTLGenerator::generate(CHTLASTNodePtr ast, const std::string& filename) {
    filename_ = filename;
    errors_.clear();
    warnings_.clear();
    
    // 清空输出流
    htmlStream_.str("");
    cssStream_.str("");
    jsStream_.str("");
    
    // 重置状态
    indentLevel_ = 0;
    globalStyles_.clear();
    globalScripts_.clear();
    classCounter_ = 0;
    idCounter_ = 0;
    
    CHTLGenerateResult result;
    
    try {
        if (!ast) {
            reportError("AST为空", Position{0, 0});
            result.success = false;
            result.errors = errors_;
            return result;
        }
        
        generateNode(ast);
        
        result.html = htmlStream_.str();
        result.css = buildCssOutput();
        result.javascript = buildJsOutput();
        
        // 后处理
        if (config_.enablePrettyPrint) {
            result.html = postProcessHtml(result.html);
            result.css = postProcessCss(result.css);
            result.javascript = postProcessJs(result.javascript);
        }
        
        if (config_.enableMinification) {
            result.html = minifyHtml(result.html);
            result.css = minifyCss(result.css);
            result.javascript = minifyJs(result.javascript);
        }
        
        result.success = errors_.empty();
        result.errors = errors_;
        result.warnings = warnings_;
        
    } catch (const std::exception& e) {
        reportError("生成过程中发生异常: " + std::string(e.what()), Position{0, 0});
        result.success = false;
        result.errors = errors_;
        result.warnings = warnings_;
    }
    
    return result;
}

// 设置外部库
void CHTLGenerator::setTemplateLibrary(const std::unordered_map<std::string, CHTLASTNodePtr>& templates) {
    templates_ = templates;
}

void CHTLGenerator::setCustomLibrary(const std::unordered_map<std::string, CHTLASTNodePtr>& customs) {
    customs_ = customs;
}

void CHTLGenerator::setVariableLibrary(const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& variables) {
    variables_ = variables;
}

// ===== 错误处理 =====

void CHTLGenerator::reportError(const std::string& message, const Position& pos, const std::string& suggestion) {
    CHTLGenerateError error;
    error.message = message;
    error.position = pos;
    error.context = "";
    error.suggestion = suggestion;
    errors_.push_back(error);
}

void CHTLGenerator::reportWarning(const std::string& message, const Position& pos) {
    warnings_.push_back(message + " at " + std::to_string(pos.line) + ":" + std::to_string(pos.column));
}

// ===== 主要生成方法 =====

void CHTLGenerator::generateNode(CHTLASTNodePtr node) {
    if (!node) return;
    
    switch (node->type) {
        case CHTLASTNodeType::PROGRAM:
            generateProgram(node);
            break;
        case CHTLASTNodeType::ELEMENT_NODE:
            generateElement(node);
            break;
        case CHTLASTNodeType::TEXT_NODE:
            generateTextNode(node);
            break;
        case CHTLASTNodeType::ATTRIBUTE_NODE:
            generateAttribute(node);
            break;
        case CHTLASTNodeType::STYLE_BLOCK:
            generateStyleBlock(node);
            break;
        case CHTLASTNodeType::SCRIPT_BLOCK:
            generateScriptBlock(node);
            break;
        case CHTLASTNodeType::TEMPLATE_DECLARATION:
        case CHTLASTNodeType::TEMPLATE_STYLE:
        case CHTLASTNodeType::TEMPLATE_ELEMENT:
        case CHTLASTNodeType::TEMPLATE_VAR:
            generateTemplateDeclaration(node);
            break;
        case CHTLASTNodeType::CUSTOM_DECLARATION:
        case CHTLASTNodeType::CUSTOM_STYLE:
        case CHTLASTNodeType::CUSTOM_ELEMENT:
        case CHTLASTNodeType::CUSTOM_VAR:
            generateCustomDeclaration(node);
            break;
        case CHTLASTNodeType::ORIGIN_DECLARATION:
        case CHTLASTNodeType::ORIGIN_HTML:
        case CHTLASTNodeType::ORIGIN_STYLE:
        case CHTLASTNodeType::ORIGIN_JAVASCRIPT:
            generateOriginDeclaration(node);
            break;
        case CHTLASTNodeType::IMPORT_DECLARATION:
            generateImportDeclaration(node);
            break;
        case CHTLASTNodeType::NAMESPACE_DECLARATION:
            generateNamespaceDeclaration(node);
            break;
        case CHTLASTNodeType::CONFIGURATION_DECLARATION:
            generateConfigurationDeclaration(node);
            break;
        case CHTLASTNodeType::COMMENT_LINE:
        case CHTLASTNodeType::COMMENT_MULTILINE:
        case CHTLASTNodeType::COMMENT_GENERATOR:
            if (config_.enableComments) {
                generateComment(node);
            }
            break;
        default:
            // 对于其他节点类型，递归处理子节点
            for (auto child : node->children) {
                generateNode(child);
            }
            break;
    }
}

// ===== 顶级节点生成 =====

void CHTLGenerator::generateProgram(CHTLASTNodePtr node) {
    for (auto child : node->children) {
        generateNode(child);
    }
}

void CHTLGenerator::generateTemplateDeclaration(CHTLASTNodePtr node) {
    // 模板声明在生成阶段只是注册到符号表，不直接输出HTML
    auto templateNode = std::dynamic_pointer_cast<TemplateDeclarationNode>(node);
    if (templateNode) {
        std::string fullName = templateNode->templateType + " " + templateNode->name;
        templates_[fullName] = node;
    }
}

void CHTLGenerator::generateCustomDeclaration(CHTLASTNodePtr node) {
    // 自定义声明也是注册到符号表
    auto customNode = std::dynamic_pointer_cast<CustomDeclarationNode>(node);
    if (customNode) {
        std::string fullName = customNode->customType + " " + customNode->name;
        customs_[fullName] = node;
    }
}

void CHTLGenerator::generateOriginDeclaration(CHTLASTNodePtr node) {
    auto originNode = std::dynamic_pointer_cast<OriginDeclarationNode>(node);
    if (!originNode) return;
    
    if (originNode->originType == "@Html") {
        // 直接输出HTML内容
        htmlStream_ << originNode->content;
    } else if (originNode->originType == "@Style") {
        // 添加到全局CSS
        addGlobalStyle("/* Origin CSS */", originNode->content, node->position);
    } else if (originNode->originType == "@JavaScript") {
        // 添加到全局JS
        addGlobalScript("/* Origin JavaScript */\n" + originNode->content, node->position);
    }
}

void CHTLGenerator::generateImportDeclaration(CHTLASTNodePtr node) {
    // 导入声明在生成阶段通常已经在预处理阶段处理过了
    // 这里可以生成相应的HTML注释
    if (config_.enableComments) {
        auto importNode = std::dynamic_pointer_cast<ImportDeclarationNode>(node);
        if (importNode) {
            writeIndent();
            htmlStream_ << "<!-- Import: " << importNode->importType << " from " << importNode->path;
            if (!importNode->alias.empty()) {
                htmlStream_ << " as " << importNode->alias;
            }
            htmlStream_ << " -->\n";
        }
    }
}

void CHTLGenerator::generateNamespaceDeclaration(CHTLASTNodePtr node) {
    auto namespaceNode = std::dynamic_pointer_cast<NamespaceDeclarationNode>(node);
    if (namespaceNode) {
        enterNamespace(namespaceNode->name);
        
        // 生成命名空间内的内容
        for (auto child : node->children) {
            generateNode(child);
        }
        
        exitNamespace();
    }
}

void CHTLGenerator::generateConfigurationDeclaration(CHTLASTNodePtr node) {
    // 配置声明影响生成器的行为，但不直接输出HTML
    // 这里可以根据配置调整生成器设置
}

// ===== 元素和文本生成 =====

void CHTLGenerator::generateElement(CHTLASTNodePtr node) {
    auto elementNode = std::dynamic_pointer_cast<ElementNode>(node);
    if (!elementNode) return;
    
    writeIndent();
    htmlStream_ << "<" << elementNode->tagName;
    
    // 生成属性
    for (auto attr : elementNode->attributes) {
        auto attrNode = std::dynamic_pointer_cast<AttributeNode>(attr);
        if (attrNode) {
            htmlStream_ << " " << attrNode->name << "=\"" << escapeHtml(attrNode->value) << "\"";
        }
    }
    
    // 处理局部样式 - 自动添加类名
    if (elementNode->styleBlock) {
        std::string className = generateUniqueClassName();
        htmlStream_ << " class=\"" << className << "\"";
        
        // 处理局部样式
        processLocalStyles(elementNode);
    }
    
    // 检查是否为自闭合标签
    bool isSelfClosing = elementNode->children.empty() && 
                        (elementNode->tagName == "br" || elementNode->tagName == "hr" || 
                         elementNode->tagName == "img" || elementNode->tagName == "input" ||
                         elementNode->tagName == "meta" || elementNode->tagName == "link");
    
    if (isSelfClosing) {
        htmlStream_ << " />";
        if (config_.enablePrettyPrint) htmlStream_ << "\n";
        return;
    }
    
    htmlStream_ << ">";
    
    // 如果有子元素，换行并增加缩进
    bool hasChildElements = !elementNode->children.empty();
    if (hasChildElements && config_.enablePrettyPrint) {
        htmlStream_ << "\n";
        increasedIndent();
    }
    
    // 生成子节点
    for (auto child : elementNode->children) {
        generateNode(child);
    }
    
    // 恢复缩进
    if (hasChildElements && config_.enablePrettyPrint) {
        decreaseIndent();
        writeIndent();
    }
    
    htmlStream_ << "</" << elementNode->tagName << ">";
    if (config_.enablePrettyPrint) htmlStream_ << "\n";
}

void CHTLGenerator::generateTextNode(CHTLASTNodePtr node) {
    auto textNode = std::dynamic_pointer_cast<TextNode>(node);
    if (!textNode) return;
    
    // 根据配置决定是否需要缩进
    if (config_.enablePrettyPrint && indentLevel_ > 0) {
        writeIndent();
    }
    
    htmlStream_ << escapeHtml(textNode->content);
    
    if (config_.enablePrettyPrint) {
        htmlStream_ << "\n";
    }
}

void CHTLGenerator::generateAttribute(CHTLASTNodePtr node) {
    // 属性在元素生成时已经处理
}

// ===== 样式生成 =====

void CHTLGenerator::generateStyleBlock(CHTLASTNodePtr node) {
    auto styleBlock = std::dynamic_pointer_cast<StyleBlockNode>(node);
    if (!styleBlock) return;
    
    // 样式块的内容会在processLocalStyles中处理
}

void CHTLGenerator::processLocalStyles(CHTLASTNodePtr elementNode) {
    auto element = std::dynamic_pointer_cast<ElementNode>(elementNode);
    if (!element || !element->styleBlock) return;
    
    std::string className = config_.cssPrefix + std::to_string(classCounter_++);
    std::string selector = "." + className;
    
    auto styleBlock = std::dynamic_pointer_cast<StyleBlockNode>(element->styleBlock);
    if (styleBlock) {
        std::ostringstream styleContent;
        
        for (auto child : styleBlock->children) {
            if (child->type == CHTLASTNodeType::INLINE_STYLE) {
                auto inlineStyle = std::dynamic_pointer_cast<InlineStyleNode>(child);
                if (inlineStyle) {
                    styleContent << "  " << inlineStyle->property << ": " << inlineStyle->value << ";\n";
                }
            } else if (child->type == CHTLASTNodeType::CLASS_SELECTOR) {
                auto classSelector = std::dynamic_pointer_cast<ClassSelectorNode>(child);
                if (classSelector) {
                    // 生成独立的CSS规则
                    addGlobalStyle(classSelector->className, classSelector->properties, child->position);
                }
            } else if (child->type == CHTLASTNodeType::ID_SELECTOR) {
                auto idSelector = std::dynamic_pointer_cast<IDSelectorNode>(child);
                if (idSelector) {
                    // 生成独立的CSS规则
                    addGlobalStyle(idSelector->idName, idSelector->properties, child->position);
                }
            }
        }
        
        if (!styleContent.str().empty()) {
            addGlobalStyle(selector, styleContent.str(), elementNode->position);
        }
    }
}

// ===== 脚本处理 =====

void CHTLGenerator::generateScriptBlock(CHTLASTNodePtr node) {
    auto scriptBlock = std::dynamic_pointer_cast<ScriptBlockNode>(node);
    if (!scriptBlock) return;
    
    addGlobalScript(scriptBlock->content, node->position);
}

// ===== 注释处理 =====

void CHTLGenerator::generateComment(CHTLASTNodePtr node) {
    auto commentNode = std::dynamic_pointer_cast<CommentNode>(node);
    if (!commentNode) return;
    
    writeIndent();
    
    switch (commentNode->commentType) {
        case CHTLASTNodeType::COMMENT_LINE:
            htmlStream_ << "<!-- " << commentNode->content << " -->";
            break;
        case CHTLASTNodeType::COMMENT_MULTILINE:
            htmlStream_ << "<!-- " << commentNode->content << " -->";
            break;
        case CHTLASTNodeType::COMMENT_GENERATOR:
            htmlStream_ << "<!-- " << commentNode->content << " -->";
            break;
    }
    
    if (config_.enablePrettyPrint) htmlStream_ << "\n";
}

// ===== 工具方法 =====

void CHTLGenerator::writeIndent() {
    if (config_.enablePrettyPrint) {
        for (int i = 0; i < indentLevel_; ++i) {
            htmlStream_ << config_.indentStyle;
        }
    }
}

void CHTLGenerator::increasedIndent() {
    indentLevel_++;
}

void CHTLGenerator::decreaseIndent() {
    if (indentLevel_ > 0) {
        indentLevel_--;
    }
}

std::string CHTLGenerator::generateUniqueClassName() {
    return config_.cssPrefix + "cls-" + std::to_string(classCounter_++);
}

std::string CHTLGenerator::generateUniqueId() {
    return config_.cssPrefix + "id-" + std::to_string(idCounter_++);
}

std::string CHTLGenerator::escapeHtml(const std::string& text) {
    std::string result = text;
    std::replace(result.begin(), result.end(), '&', '&');
    result = std::regex_replace(result, std::regex("&"), "&amp;");
    result = std::regex_replace(result, std::regex("<"), "&lt;");
    result = std::regex_replace(result, std::regex(">"), "&gt;");
    result = std::regex_replace(result, std::regex("\""), "&quot;");
    result = std::regex_replace(result, std::regex("'"), "&#39;");
    return result;
}

std::string CHTLGenerator::escapeCss(const std::string& text) {
    // 基本的CSS转义
    return text;
}

std::string CHTLGenerator::escapeJs(const std::string& text) {
    std::string result = text;
    result = std::regex_replace(result, std::regex("\\\\"), "\\\\");
    result = std::regex_replace(result, std::regex("\""), "\\\"");
    result = std::regex_replace(result, std::regex("'"), "\\'");
    result = std::regex_replace(result, std::regex("\n"), "\\n");
    result = std::regex_replace(result, std::regex("\r"), "\\r");
    result = std::regex_replace(result, std::regex("\t"), "\\t");
    return result;
}

// ===== CSS处理 =====

void CHTLGenerator::addGlobalStyle(const std::string& selector, const std::string& properties, const Position& pos) {
    StyleInfo styleInfo;
    styleInfo.selector = selector;
    styleInfo.properties = properties;
    styleInfo.isInline = false;
    styleInfo.sourcePosition = pos;
    globalStyles_.push_back(styleInfo);
}

std::string CHTLGenerator::buildCssOutput() {
    if (globalStyles_.empty()) return "";
    
    std::ostringstream css;
    
    for (const auto& style : globalStyles_) {
        css << style.selector << " {\n";
        css << style.properties;
        if (!style.properties.empty() && style.properties.back() != '\n') {
            css << "\n";
        }
        css << "}\n\n";
    }
    
    return css.str();
}

// ===== JavaScript处理 =====

void CHTLGenerator::addGlobalScript(const std::string& content, const Position& pos) {
    ScriptInfo scriptInfo;
    scriptInfo.content = content;
    scriptInfo.isInline = false;
    scriptInfo.sourcePosition = pos;
    globalScripts_.push_back(scriptInfo);
}

std::string CHTLGenerator::buildJsOutput() {
    if (globalScripts_.empty()) return "";
    
    std::ostringstream js;
    
    js << "(function() {\n";
    js << "  'use strict';\n\n";
    
    for (const auto& script : globalScripts_) {
        js << "  // Script from line " << script.sourcePosition.line << "\n";
        js << "  " << script.content << "\n\n";
    }
    
    js << "})();\n";
    
    return js.str();
}

// ===== 命名空间处理 =====

std::string CHTLGenerator::resolveNamespacedName(const std::string& name) {
    if (currentNamespace_.empty()) {
        return name;
    }
    return currentNamespace_ + "." + name;
}

void CHTLGenerator::enterNamespace(const std::string& name) {
    if (currentNamespace_.empty()) {
        currentNamespace_ = name;
    } else {
        currentNamespace_ += "." + name;
    }
}

void CHTLGenerator::exitNamespace() {
    size_t lastDot = currentNamespace_.find_last_of('.');
    if (lastDot != std::string::npos) {
        currentNamespace_ = currentNamespace_.substr(0, lastDot);
    } else {
        currentNamespace_.clear();
    }
}

// ===== 后处理 =====

std::string CHTLGenerator::postProcessHtml(const std::string& html) {
    return html; // 基本实现，可以添加格式化逻辑
}

std::string CHTLGenerator::postProcessCss(const std::string& css) {
    return css; // 基本实现，可以添加CSS优化
}

std::string CHTLGenerator::postProcessJs(const std::string& js) {
    return js; // 基本实现，可以添加JS优化
}

std::string CHTLGenerator::minifyHtml(const std::string& html) {
    // 简单的HTML最小化
    std::string result = std::regex_replace(html, std::regex("\\s+"), " ");
    result = std::regex_replace(result, std::regex("> <"), "><");
    return result;
}

std::string CHTLGenerator::minifyCss(const std::string& css) {
    // 简单的CSS最小化
    std::string result = std::regex_replace(css, std::regex("\\s+"), " ");
    result = std::regex_replace(result, std::regex("; "), ";");
    result = std::regex_replace(result, std::regex(" {"), "{");
    result = std::regex_replace(result, std::regex("} "), "}");
    return result;
}

std::string CHTLGenerator::minifyJs(const std::string& js) {
    // 简单的JS最小化
    std::string result = std::regex_replace(js, std::regex("\\s+"), " ");
    return result;
}

} // namespace chtl
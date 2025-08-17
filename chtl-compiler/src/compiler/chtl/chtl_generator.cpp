#include "compiler/chtl/chtl_generator.h"
#include <algorithm>
#include <unordered_set>

namespace chtl {

// 自闭合标签集合
static const std::unordered_set<std::string> SELF_CLOSING_TAGS = {
    "img", "br", "hr", "input", "meta", "link", "area", "base",
    "col", "embed", "source", "track", "wbr"
};

CHTLGenerator::CHTLGenerator(CompilerState& state)
    : state_(state), indent_level_(0) {}

std::string CHTLGenerator::generate(const ASTNode* ast) {
    output_.str("");
    output_.clear();
    indent_level_ = 0;
    
    visit(ast);
    
    return output_.str();
}

std::string CHTLGenerator::getGlobalStyles() const {
    const auto& styles = state_.getGlobalStyles();
    if (styles.empty()) {
        return "";
    }
    
    std::stringstream ss;
    ss << "<style>\n";
    for (const auto& style : styles) {
        ss << style << "\n";
    }
    ss << "</style>\n";
    
    return ss.str();
}

std::string CHTLGenerator::getGlobalScripts() const {
    const auto& scripts = state_.getGlobalScripts();
    if (scripts.empty()) {
        return "";
    }
    
    std::stringstream ss;
    ss << "<script>\n";
    for (const auto& script : scripts) {
        ss << script << "\n";
    }
    ss << "</script>\n";
    
    return ss.str();
}

void CHTLGenerator::indent() {
    indent_level_++;
}

void CHTLGenerator::dedent() {
    if (indent_level_ > 0) {
        indent_level_--;
    }
}

std::string CHTLGenerator::getIndent() const {
    return std::string(indent_level_ * 2, ' ');
}

void CHTLGenerator::visit(const ASTNode* node) {
    if (!node) return;
    
    switch (node->getType()) {
        case ASTNodeType::ROOT:
            visitRoot(static_cast<const RootNode*>(node));
            break;
        case ASTNodeType::ELEMENT:
            visitElement(static_cast<const ElementNode*>(node));
            break;
        case ASTNodeType::TEXT:
            visitText(static_cast<const TextNode*>(node));
            break;
        case ASTNodeType::STYLE_BLOCK:
            visitStyleBlock(static_cast<const StyleBlockNode*>(node));
            break;
        case ASTNodeType::SCRIPT_BLOCK:
            visitScriptBlock(static_cast<const ScriptBlockNode*>(node));
            break;
        case ASTNodeType::COMMENT:
            visitComment(node);
            break;
        case ASTNodeType::TEMPLATE_DEFINITION:
            visitTemplateDefinition(static_cast<const TemplateDefinitionNode*>(node));
            break;
        case ASTNodeType::TEMPLATE_USAGE:
            visitTemplateUsage(static_cast<const TemplateUsageNode*>(node));
            break;
        case ASTNodeType::CUSTOM_DEFINITION:
            visitCustomDefinition(static_cast<const CustomDefinitionNode*>(node));
            break;
        case ASTNodeType::CUSTOM_USAGE:
            visitCustomUsage(static_cast<const CustomUsageNode*>(node));
            break;
        case ASTNodeType::CONFIGURATION_BLOCK:
            visitConfiguration(static_cast<const ConfigurationBlockNode*>(node));
            break;
        case ASTNodeType::NAMESPACE_BLOCK:
            visitNamespace(static_cast<const NamespaceBlockNode*>(node));
            break;
        case ASTNodeType::ORIGIN_BLOCK:
            visitOrigin(static_cast<const OriginBlockNode*>(node));
            break;
        case ASTNodeType::IMPORT_STATEMENT:
            visitImport(static_cast<const ImportStatementNode*>(node));
            break;
        default:
            // 其他节点类型暂不处理
            for (const auto& child : node->getChildren()) {
                visit(child.get());
            }
            break;
    }
}

void CHTLGenerator::visitRoot(const RootNode* node) {
    // 直接访问所有子节点
    for (const auto& child : node->getChildren()) {
        visit(child.get());
    }
}

void CHTLGenerator::visitElement(const ElementNode* node) {
    const std::string& tag = node->getTagName();
    
    // 开始标签
    output_ << getIndent() << "<" << tag;
    
    // 生成属性
    generateAttributes(node);
    
    // 处理局部样式
    if (node->has_local_style) {
        // 收集局部样式到全局样式
        for (const auto& child : node->getChildren()) {
            if (child->getType() == ASTNodeType::STYLE_BLOCK) {
                collectLocalStyles(static_cast<const StyleBlockNode*>(child.get()), 
                                 node->generated_class_name);
            }
        }
    }
    
    if (isSelfClosingTag(tag)) {
        output_ << " />\n";
    } else {
        output_ << ">\n";
        
        // 访问子节点（跳过样式块和脚本块，它们已被处理）
        indent();
        for (const auto& child : node->getChildren()) {
            if (child->getType() != ASTNodeType::STYLE_BLOCK && 
                child->getType() != ASTNodeType::SCRIPT_BLOCK) {
                visit(child.get());
            }
        }
        dedent();
        
        // 结束标签
        output_ << getIndent() << "</" << tag << ">\n";
    }
}

void CHTLGenerator::visitText(const TextNode* node) {
    std::string content = node->getContent();
    
    // 移除前后空白
    content.erase(0, content.find_first_not_of(" \t\n\r"));
    content.erase(content.find_last_not_of(" \t\n\r") + 1);
    
    if (!content.empty()) {
        output_ << getIndent() << escapeHtml(content) << "\n";
    }
}

void CHTLGenerator::visitStyleBlock(const StyleBlockNode* node) {
    // 局部样式块在元素处理时已经被收集
    // 这里不直接输出
}

void CHTLGenerator::visitScriptBlock(const ScriptBlockNode* node) {
    // 将脚本内容添加到全局脚本
    state_.addGlobalScript(node->getContent());
}

void CHTLGenerator::visitComment(const ASTNode* node) {
    // 注释节点需要特殊处理
    // TODO: 实现CommentNode
}

void CHTLGenerator::visitTemplateDefinition(const TemplateDefinitionNode* node) {
    // 模板定义不生成输出，已经在解析时添加到GlobalMap
}

void CHTLGenerator::visitTemplateUsage(const TemplateUsageNode* node) {
    // TODO: 实现模板使用的展开
    auto* tmpl = state_.getGlobalMap().getTemplate(node->getName(), node->getTemplateType());
    if (tmpl && tmpl->content) {
        visit(tmpl->content.get());
    }
}

void CHTLGenerator::visitCustomDefinition(const CustomDefinitionNode* node) {
    // 自定义定义不生成输出，已经在解析时添加到GlobalMap
}

void CHTLGenerator::visitCustomUsage(const CustomUsageNode* node) {
    // TODO: 实现自定义使用的展开
    auto* custom = state_.getGlobalMap().getCustom(node->getName(), node->getCustomType());
    if (custom && custom->content) {
        visit(custom->content.get());
    }
}

void CHTLGenerator::visitConfiguration(const ConfigurationBlockNode* node) {
    // 配置块不生成输出
}

void CHTLGenerator::visitNamespace(const NamespaceBlockNode* node) {
    // 命名空间只影响作用域，不生成输出
    for (const auto& child : node->getChildren()) {
        visit(child.get());
    }
}

void CHTLGenerator::visitOrigin(const OriginBlockNode* node) {
    const std::string& type = node->getOriginType();
    const std::string& content = node->getContent();
    
    if (type == "@Html") {
        output_ << content << "\n";
    } else if (type == "@Style") {
        state_.addGlobalStyle(content);
    } else if (type == "@JavaScript") {
        state_.addGlobalScript(content);
    } else {
        // 自定义类型，直接输出
        output_ << content << "\n";
    }
}

void CHTLGenerator::visitImport(const ImportStatementNode* node) {
    // 导入语句不生成输出
}

std::string CHTLGenerator::escapeHtml(const std::string& text) {
    std::string result;
    for (char c : text) {
        switch (c) {
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '&': result += "&amp;"; break;
            case '"': result += "&quot;"; break;
            case '\'': result += "&#39;"; break;
            default: result += c; break;
        }
    }
    return result;
}

void CHTLGenerator::generateAttributes(const ElementNode* element) {
    const auto& attributes = element->getAttributes();
    
    // 合并class属性
    std::string class_value;
    for (const auto& [name, value] : attributes) {
        if (name == "class") {
            if (!class_value.empty()) class_value += " ";
            class_value += value;
        }
    }
    
    // 如果有生成的类名，添加它
    if (!element->generated_class_name.empty()) {
        if (!class_value.empty()) class_value += " ";
        class_value += element->generated_class_name;
    }
    
    // 输出所有属性
    for (const auto& [name, value] : attributes) {
        if (name == "class" && !class_value.empty()) {
            output_ << " " << name << "=\"" << escapeHtml(class_value) << "\"";
            class_value.clear(); // 防止重复输出
        } else if (name != "class") {
            output_ << " " << name << "=\"" << escapeHtml(value) << "\"";
        }
    }
    
    // 如果只有生成的类名
    if (!class_value.empty()) {
        output_ << " class=\"" << escapeHtml(class_value) << "\"";
    }
}

bool CHTLGenerator::isSelfClosingTag(const std::string& tag) {
    return SELF_CLOSING_TAGS.count(tag) > 0;
}

void CHTLGenerator::collectLocalStyles(const StyleBlockNode* style_block, 
                                      const std::string& class_name) {
    // 首先收集直接的样式属性作为元素的样式
    std::stringstream inline_styles;
    bool has_inline_styles = false;
    
    for (const auto& child : style_block->getChildren()) {
        if (child->getType() == ASTNodeType::STYLE_PROPERTY) {
            // 内联样式属性
            auto* prop = static_cast<const StylePropertyNode*>(child.get());
            if (has_inline_styles) inline_styles << "; ";
            inline_styles << prop->getProperty() << ": " << prop->getValue();
            has_inline_styles = true;
        } else if (child->getType() == ASTNodeType::STYLE_RULE) {
            auto* rule = static_cast<const StyleRuleNode*>(child.get());
            std::string selector = generateSelector(rule->getSelector(), class_name);
            
            std::stringstream rule_output;
            rule_output << selector << " {\n";
            
            // 输出规则内的属性
            for (const auto& prop_child : rule->getChildren()) {
                if (prop_child->getType() == ASTNodeType::STYLE_PROPERTY) {
                    auto* prop = static_cast<const StylePropertyNode*>(prop_child.get());
                    rule_output << "  " << prop->getProperty() << ": " << prop->getValue() << ";\n";
                }
            }
            
            rule_output << "}";
            state_.addGlobalStyle(rule_output.str());
        }
    }
    
    // 如果有内联样式，创建一个类规则
    if (has_inline_styles) {
        std::stringstream class_rule;
        class_rule << "." << class_name << " {\n";
        class_rule << "  " << inline_styles.str() << ";\n";
        class_rule << "}";
        state_.addGlobalStyle(class_rule.str());
    }
}

std::string CHTLGenerator::generateSelector(const std::string& selector, 
                                           const std::string& context_class) {
    std::string result = selector;
    
    // 处理&符号
    if (result.find('&') != std::string::npos) {
        // 替换&为上下文类
        size_t pos = 0;
        while ((pos = result.find('&', pos)) != std::string::npos) {
            result.replace(pos, 1, "." + context_class);
            pos += context_class.length() + 1;
        }
    } else if (result[0] == '.') {
        // 类选择器：自动添加到元素
        // 选择器本身已经完整，直接使用
    } else if (result[0] == '#') {
        // ID选择器：直接使用
    } else {
        // 其他情况，可能需要添加上下文
        result = "." + context_class + " " + result;
    }
    
    return result;
}

} // namespace chtl
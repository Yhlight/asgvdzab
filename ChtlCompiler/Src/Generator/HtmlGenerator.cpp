#include "Generator/HtmlGenerator.h"
#include "AST/ChtlNodes.h"
#include "AST/TemplateNodes.h"
#include "AST/ConfigNodes.h"
#include "Parser/ChtlJsParser.h"
#include <algorithm>

namespace Chtl {

HtmlGenerator::HtmlGenerator() 
    : currentOutput_(&htmlOutput_),
      context_(nullptr),
      indentLevel_(0),
      inLocalStyleBlock_(false),
      autoClassCounter_(0) {
}

HtmlGenerator::~HtmlGenerator() {
}

std::string HtmlGenerator::generate(ASTNodePtr ast, Context* context) {
    // 重置状态
    htmlOutput_.str("");
    htmlOutput_.clear();
    cssOutput_.str("");
    cssOutput_.clear();
    jsOutput_.str("");
    jsOutput_.clear();
    
    currentOutput_ = &htmlOutput_;
    context_ = context;
    indentLevel_ = 0;
    autoClassCounter_ = 0;
    
    // 访问AST
    if (ast) {
        ast->accept(this);
    }
    
    return htmlOutput_.str();
}

void HtmlGenerator::visitProgram(ProgramNode* node) {
    // 访问所有子节点
    for (const auto& child : node->getChildren()) {
        if (child) {
            child->accept(this);
        }
    }
}

void HtmlGenerator::visitElement(ElementNode* node) {
    // 清空当前属性
    currentAttributes_.clear();
    currentClasses_.clear();
    currentId_.clear();
    currentElementAutoClass_.clear();
    
    // 首先收集属性
    for (const auto& child : node->getChildren()) {
        if (child && child->getType() == ASTNodeType::ATTRIBUTE) {
            child->accept(this);
        }
    }
    
    // 检查是否有局部样式块需要自动类名
    bool hasLocalStyle = false;
    for (const auto& child : node->getChildren()) {
        if (child && child->getType() == ASTNodeType::STYLE_BLOCK) {
            hasLocalStyle = true;
            
            // 检查是否需要自动生成类名
            auto styleBlock = std::static_pointer_cast<StyleBlockNode>(child);
            for (const auto& styleChild : styleBlock->getChildren()) {
                if (styleChild && styleChild->getType() == ASTNodeType::STYLE_RULE) {
                    auto rule = std::static_pointer_cast<StyleRuleNode>(styleChild);
                    auto selector = rule->getSelector();
                    if (selector) {
                        auto selectorNode = std::static_pointer_cast<StyleSelectorNode>(selector);
                        if (selectorNode->getSelectorType() == StyleSelectorNode::CLASS ||
                            selectorNode->getSelectorType() == StyleSelectorNode::CONTEXT) {
                            // 需要自动类名
                            if (currentElementAutoClass_.empty()) {
                                currentElementAutoClass_ = generateUniqueClassName();
                                currentClasses_.push_back(currentElementAutoClass_);
                            }
                            break;
                        }
                    }
                }
            }
            break;
        }
    }
    
    // 写入开始标签
    writeIndent();
    *currentOutput_ << "<" << node->getTagName();
    
    // 写入id属性
    if (!currentId_.empty()) {
        *currentOutput_ << " id=\"" << escapeHtml(currentId_) << "\"";
    }
    
    // 写入class属性
    if (!currentClasses_.empty()) {
        *currentOutput_ << " class=\"";
        for (size_t i = 0; i < currentClasses_.size(); ++i) {
            if (i > 0) *currentOutput_ << " ";
            *currentOutput_ << escapeHtml(currentClasses_[i]);
        }
        *currentOutput_ << "\"";
    }
    
    // 写入其他属性
    for (const auto& [name, value] : currentAttributes_) {
        if (name != "id" && name != "class") {
            *currentOutput_ << " " << name << "=\"" << escapeHtml(value) << "\"";
        }
    }
    
    *currentOutput_ << ">\n";
    
    indent();
    
    // 处理子节点
    for (const auto& child : node->getChildren()) {
        if (child) {
            // 跳过已处理的属性
            if (child->getType() == ASTNodeType::ATTRIBUTE) {
                continue;
            }
            
            // 特殊处理局部样式块
            if (child->getType() == ASTNodeType::STYLE_BLOCK && hasLocalStyle) {
                processLocalStyles(std::static_pointer_cast<StyleBlockNode>(child).get(), 
                                 currentElementAutoClass_);
            } else {
                child->accept(this);
            }
        }
    }
    
    dedent();
    
    // 写入结束标签
    writeIndent();
    *currentOutput_ << "</" << node->getTagName() << ">\n";
}

void HtmlGenerator::visitText(TextNode* node) {
    writeIndent();
    *currentOutput_ << escapeHtml(node->getContent()) << "\n";
}

void HtmlGenerator::visitAttribute(AttributeNode* node) {
    // 收集属性
    std::string name = node->getName();
    std::string value = node->getValue();
    
    if (name == "id") {
        currentId_ = value;
    } else if (name == "class") {
        // 分割类名
        std::stringstream ss(value);
        std::string className;
        while (ss >> className) {
            currentClasses_.push_back(className);
        }
    } else {
        currentAttributes_[name] = value;
    }
}

void HtmlGenerator::visitStyleBlock(StyleBlockNode* node) {
    // 这个方法通常不会直接调用，而是通过processLocalStyles处理
    // 但为了完整性还是实现
    inLocalStyleBlock_ = true;
    
    for (const auto& child : node->getChildren()) {
        if (child) {
            child->accept(this);
        }
    }
    
    inLocalStyleBlock_ = false;
}

void HtmlGenerator::processLocalStyles(StyleBlockNode* node, const std::string& elementClass) {
    // 保存当前输出流
    auto savedOutput = currentOutput_;
    currentOutput_ = &cssOutput_;
    
    // 处理样式规则
    for (const auto& child : node->getChildren()) {
        if (child) {
            if (child->getType() == ASTNodeType::STYLE_PROPERTY) {
                // 内联样式 - 应该添加到元素的style属性中
                // 这里暂时忽略，因为需要修改元素生成逻辑
            } else if (child->getType() == ASTNodeType::STYLE_RULE) {
                auto rule = std::static_pointer_cast<StyleRuleNode>(child);
                auto selector = rule->getSelector();
                if (selector) {
                    auto selectorNode = std::static_pointer_cast<StyleSelectorNode>(selector);
                    std::string actualSelector = selectorNode->getSelector();
                    
                    // 处理上下文推导 &
                    if (actualSelector.find("&") != std::string::npos) {
                        // 替换 & 为自动生成的类名
                        size_t pos = 0;
                        while ((pos = actualSelector.find("&", pos)) != std::string::npos) {
                            actualSelector.replace(pos, 1, "." + elementClass);
                            pos += elementClass.length() + 1;
                        }
                    } else if (selectorNode->getSelectorType() == StyleSelectorNode::CLASS) {
                        // 类选择器需要添加到元素上
                        std::string className = actualSelector.substr(1); // 去掉 .
                        // 这里应该更新元素的类列表，但现在先生成CSS
                    }
                    
                    // 输出CSS规则
                    *currentOutput_ << actualSelector << " {\n";
                    indentLevel_++;
                    
                    // 输出属性
                    for (const auto& prop : rule->getChildren()) {
                        if (prop && prop->getType() == ASTNodeType::STYLE_PROPERTY) {
                            prop->accept(this);
                        }
                    }
                    
                    indentLevel_--;
                    *currentOutput_ << "}\n\n";
                }
            }
        }
    }
    
    // 恢复输出流
    currentOutput_ = savedOutput;
}

void HtmlGenerator::visitStyleRule(StyleRuleNode* node) {
    // 在CSS输出中处理
    if (currentOutput_ == &cssOutput_) {
        // 选择器已经在processLocalStyles中处理
        // 这里只处理属性
        for (const auto& child : node->getChildren()) {
            if (child && child->getType() == ASTNodeType::STYLE_PROPERTY) {
                child->accept(this);
            }
        }
    }
}

void HtmlGenerator::visitStyleSelector(StyleSelectorNode* node) {
    currentSelector_ = node->getSelector();
}

void HtmlGenerator::visitStyleProperty(StylePropertyNode* node) {
    if (currentOutput_ == &cssOutput_) {
        writeIndent();
        *currentOutput_ << node->getProperty() << ": " << node->getValue() << ";\n";
    }
}

void HtmlGenerator::visitScriptBlock(ScriptBlockNode* node) {
    // 保存当前输出流
    auto savedOutput = currentOutput_;
    currentOutput_ = &jsOutput_;
    
    std::string content = node->getContent();
    
    // 如果包含CHTL JS语法，进行转换
    if (node->hasChtlJsSyntax()) {
        ChtlJsParser jsParser;
        content = jsParser.transform(content);
    }
    
    *currentOutput_ << content << "\n\n";
    
    // 恢复输出流
    currentOutput_ = savedOutput;
}

void HtmlGenerator::visitComment(CommentNode* node) {
    if (node->getCommentType() == CommentNode::GENERATOR) {
        // 生成器注释 -- 会被输出到HTML
        writeIndent();
        *currentOutput_ << "<!-- " << escapeHtml(node->getContent()) << " -->\n";
    }
    // 其他注释类型不输出
}

void HtmlGenerator::visitOriginBlock(OriginBlockNode* node) {
    switch (node->getOriginType()) {
        case OriginBlockNode::HTML:
            // 直接输出到HTML
            *currentOutput_ << node->getContent() << "\n";
            break;
            
        case OriginBlockNode::STYLE:
            // 输出到CSS
            cssOutput_ << node->getContent() << "\n";
            break;
            
        case OriginBlockNode::JAVASCRIPT:
            // 输出到JS
            jsOutput_ << node->getContent() << "\n";
            break;
            
        case OriginBlockNode::CUSTOM:
            // 自定义类型 - 根据配置处理
            if (context_) {
                auto& config = context_->getGlobalMap().getConfiguration();
                auto it = config.originTypes.find(node->getCustomTypeName());
                if (it != config.originTypes.end()) {
                    // 根据映射类型处理
                    if (it->second == "@Html") {
                        *currentOutput_ << node->getContent() << "\n";
                    } else if (it->second == "@Style") {
                        cssOutput_ << node->getContent() << "\n";
                    } else if (it->second == "@JavaScript") {
                        jsOutput_ << node->getContent() << "\n";
                    }
                }
            }
            break;
    }
}

void HtmlGenerator::visitTemplateUse(TemplateUseNode* node) {
    if (node->getUseType() == TemplateUseNode::STYLE) {
        expandStyleTemplate(node->getTemplateName(), node->getParameters());
    } else if (node->getUseType() == TemplateUseNode::ELEMENT) {
        expandElementTemplate(node->getTemplateName());
    }
}

void HtmlGenerator::visitVarUse(VarUseNode* node) {
    // 变量使用应该在解析时就展开了
    // 这里输出原始值作为备份
    *currentOutput_ << node->getVarGroupName() << "(" << node->getVariableName() << ")";
}

// 辅助方法实现
void HtmlGenerator::indent() {
    indentLevel_++;
}

void HtmlGenerator::dedent() {
    if (indentLevel_ > 0) {
        indentLevel_--;
    }
}

void HtmlGenerator::writeIndent() {
    for (int i = 0; i < indentLevel_; ++i) {
        *currentOutput_ << "  ";
    }
}

std::string HtmlGenerator::escapeHtml(const std::string& str) {
    std::string result;
    result.reserve(str.size());
    
    for (char c : str) {
        switch (c) {
            case '&':  result += "&amp;"; break;
            case '<':  result += "&lt;"; break;
            case '>':  result += "&gt;"; break;
            case '"':  result += "&quot;"; break;
            case '\'': result += "&#39;"; break;
            default:   result += c; break;
        }
    }
    
    return result;
}

std::string HtmlGenerator::generateUniqueClassName() {
    return "chtl-" + std::to_string(++autoClassCounter_);
}

void HtmlGenerator::expandStyleTemplate(const std::string& templateName, 
                                       const std::unordered_map<std::string, ASTNodePtr>& params) {
    if (!context_) return;
    
    auto styleOpt = context_->getGlobalMap().getStyleGroup(templateName);
    if (styleOpt.has_value()) {
        auto& style = styleOpt.value();
        
        // 输出样式属性
        for (const auto& [prop, value] : style.properties) {
            // 检查是否有参数覆盖
            auto it = params.find(prop);
            if (it != params.end()) {
                // 使用参数值
                writeIndent();
                *currentOutput_ << prop << ": ";
                it->second->accept(this);
                *currentOutput_ << ";\n";
            } else {
                // 使用模板值
                writeIndent();
                *currentOutput_ << prop << ": " << value << ";\n";
            }
        }
    }
}

void HtmlGenerator::expandElementTemplate(const std::string& templateName) {
    if (!context_) return;
    
    auto elementOpt = context_->getGlobalMap().getElementGroup(templateName);
    if (elementOpt.has_value()) {
        // TODO: 实现元素模板展开
        // 需要重新解析模板内容并生成
    }
}

std::string HtmlGenerator::expandVarUse(const std::string& groupName, const std::string& varName) {
    if (!context_) return "";
    
    auto varOpt = context_->getGlobalMap().getVarGroup(groupName);
    if (varOpt.has_value()) {
        auto& varGroup = varOpt.value();
        auto it = varGroup.variables.find(varName);
        if (it != varGroup.variables.end()) {
            return it->second;
        }
    }
    
    return "";
}

// 其他访问方法的默认实现
void HtmlGenerator::visitStringLiteral(StringLiteralNode* node) {
    *currentOutput_ << node->getValue();
}

void HtmlGenerator::visitNumberLiteral(NumberLiteralNode* node) {
    *currentOutput_ << node->getValue();
}

void HtmlGenerator::visitIdentifier(IdentifierNode* node) {
    *currentOutput_ << node->getName();
}

void HtmlGenerator::visitStyleTemplate(StyleTemplateNode* node) {
    // 模板定义不生成输出，只存储到全局符号表
}

void HtmlGenerator::visitElementTemplate(ElementTemplateNode* node) {
    // 模板定义不生成输出
}

void HtmlGenerator::visitVarTemplate(VarTemplateNode* node) {
    // 模板定义不生成输出
}

} // namespace Chtl
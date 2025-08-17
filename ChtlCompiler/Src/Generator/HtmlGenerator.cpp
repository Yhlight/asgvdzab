#include "Generator/HtmlGenerator.h"
#include "AST/ChtlNodes.h"
#include "AST/TemplateNodes.h"
#include "AST/ConfigNodes.h"
#include "AST/ChtlJsNodes.h"
#include "Parser/ChtlJsParser.h"
#include "Parser/ChtlJsFunctionManager.h"
#include <algorithm>

namespace Chtl {

HtmlGenerator::HtmlGenerator() 
    : currentOutput_(&htmlOutput_),
      context_(nullptr),
      indentLevel_(0),
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
    
    // 创建生成器上下文助手
    genHelper_ = std::make_unique<GenerateContextHelper>(context_);
    
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
    
    for (const auto& child : node->getChildren()) {
        if (child) {
            child->accept(this);
        }
    }
}

void HtmlGenerator::processLocalStyles(StyleBlockNode* node, const std::string& elementClass) {
    // 切换到CSS输出
    switchToCss();
    
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
                    
                    // 使用生成器助手处理局部样式上下文
                    genHelper_->processLocalStyleContext(actualSelector);
                    
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
    
    // 切换回HTML输出
    switchToHtml();
}

void HtmlGenerator::visitStyleRule(StyleRuleNode* node) {
    // 在CSS输出中处理
    if (genHelper_->getCurrentTarget() == GenerateContextHelper::OutputTarget::CSS) {
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
    // 选择器处理在StyleRule中完成
}

void HtmlGenerator::visitStyleProperty(StylePropertyNode* node) {
    if (genHelper_->getCurrentTarget() == GenerateContextHelper::OutputTarget::CSS) {
        writeIndent();
        *currentOutput_ << node->getProperty() << ": " << node->getValue() << ";\n";
    }
}

void HtmlGenerator::visitScriptBlock(ScriptBlockNode* node) {
    // 切换到JS输出
    switchToJs();
    
    std::string content = node->getContent();
    
    // 如果包含CHTL JS语法，进行转换
    if (node->hasChtlJsSyntax()) {
        ChtlJsParser jsParser;
        content = jsParser.transform(content);
    }
    
    *currentOutput_ << content << "\n\n";
    
    // 切换回HTML输出
    switchToHtml();
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
    // 使用生成器助手确定输出目标
    auto target = genHelper_->determineOriginTarget(
        node->getOriginType() == OriginBlockNode::CUSTOM ? 
        node->getCustomTypeName() : 
        (node->getOriginType() == OriginBlockNode::HTML ? "@Html" :
         node->getOriginType() == OriginBlockNode::STYLE ? "@Style" :
         "@JavaScript")
    );
    
    switch (target) {
        case GenerateContextHelper::OutputTarget::HTML:
            *currentOutput_ << node->getContent() << "\n";
            break;
            
        case GenerateContextHelper::OutputTarget::CSS:
            cssOutput_ << node->getContent() << "\n";
            break;
            
        case GenerateContextHelper::OutputTarget::JAVASCRIPT:
            jsOutput_ << node->getContent() << "\n";
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
    // 使用生成器助手解析变量
    std::string resolvedValue = genHelper_->resolveVariable(
        node->getVarGroupName(), 
        node->getVariableName()
    );
    
    if (!resolvedValue.empty()) {
        *currentOutput_ << resolvedValue;
    } else {
        // 变量未找到，输出原始值
        *currentOutput_ << node->getVarGroupName() << "(" << node->getVariableName() << ")";
    }
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

void HtmlGenerator::switchToHtml() {
    currentOutput_ = &htmlOutput_;
    genHelper_->setOutputTarget(GenerateContextHelper::OutputTarget::HTML);
}

void HtmlGenerator::switchToCss() {
    currentOutput_ = &cssOutput_;
    genHelper_->setOutputTarget(GenerateContextHelper::OutputTarget::CSS);
}

void HtmlGenerator::switchToJs() {
    currentOutput_ = &jsOutput_;
    genHelper_->setOutputTarget(GenerateContextHelper::OutputTarget::JAVASCRIPT);
}

void HtmlGenerator::expandStyleTemplate(const std::string& templateName, 
                                       const std::unordered_map<std::string, ASTNodePtr>& params) {
    if (!context_) return;
    
    // 标记开始模板展开
    genHelper_->beginTemplateExpansion(templateName);
    
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
    
    // 标记结束模板展开
    genHelper_->endTemplateExpansion();
}

void HtmlGenerator::expandElementTemplate(const std::string& templateName) {
    if (!context_) return;
    
    // 标记开始模板展开
    genHelper_->beginTemplateExpansion(templateName);
    
    auto elementOpt = context_->getGlobalMap().getElementGroup(templateName);
    if (elementOpt.has_value()) {
        // TODO: 实现元素模板展开
        // 需要重新解析模板内容并生成
    }
    
    // 标记结束模板展开
    genHelper_->endTemplateExpansion();
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

// CHTL JS 节点实现
void HtmlGenerator::visitSelectorExpression(SelectorExpressionNode* node) {
    switchToJs();
    
    // 生成查询选择器代码
    std::string selector = node->getSelector();
    bool isMultiple = (node->getSelectorType() == SelectorExpressionNode::TAG && 
                      node->getIndex() < 0);
    
    ChtlJsParser parser;
    std::string jsCode = parser.generateQuerySelector(selector, isMultiple);
    
    if (node->hasIndex()) {
        jsCode = "(" + jsCode + ")[" + std::to_string(node->getIndex()) + "]";
    }
    
    jsOutput_ << jsCode;
}

void HtmlGenerator::visitArrowAccess(ArrowAccessNode* node) {
    switchToJs();
    
    // 处理左侧表达式
    if (node->getLeft()) {
        node->getLeft()->accept(this);
    }
    
    // 生成成员访问
    jsOutput_ << "." << node->getMember();
    
    // 如果是方法调用，处理参数
    if (node->isMethodCall()) {
        jsOutput_ << "(";
        const auto& args = node->getArguments();
        for (size_t i = 0; i < args.size(); ++i) {
            if (i > 0) jsOutput_ << ", ";
            args[i]->accept(this);
        }
        jsOutput_ << ")";
    }
}

void HtmlGenerator::visitListenCall(ListenCallNode* node) {
    switchToJs();
    
    // 获取目标元素
    if (node->getTarget()) {
        node->getTarget()->accept(this);
    }
    
    // 生成addEventListener调用
    for (const auto& config : node->getEventConfigs()) {
        jsOutput_ << ".addEventListener('" << config.eventName << "', ";
        
        if (config.handler) {
            config.handler->accept(this);
        } else {
            jsOutput_ << "function(e) {}";
        }
        
        if (config.useCapture || config.once || config.passive) {
            jsOutput_ << ", {";
            if (config.useCapture) jsOutput_ << "capture: true";
            if (config.once) {
                if (config.useCapture) jsOutput_ << ", ";
                jsOutput_ << "once: true";
            }
            if (config.passive) {
                if (config.useCapture || config.once) jsOutput_ << ", ";
                jsOutput_ << "passive: true";
            }
            jsOutput_ << "}";
        }
        
        jsOutput_ << ");\n";
    }
}

void HtmlGenerator::visitDelegateCall(DelegateCallNode* node) {
    switchToJs();
    
    // 生成事件委托辅助函数（如果还没有）
    static bool delegateHelperGenerated = false;
    if (!delegateHelperGenerated) {
        jsOutput_ << R"(
// CHTL JS 事件委托辅助函数
function _chtlDelegate(parent, config) {
    const targets = Array.isArray(config.targets) ? config.targets : [config.targets];
    const eventHandlers = {};
    
    // 收集所有事件处理器
    Object.keys(config).forEach(key => {
        if (key !== 'targets' && typeof config[key] === 'function') {
            eventHandlers[key] = config[key];
        }
    });
    
    // 为每个事件类型添加委托监听器
    Object.keys(eventHandlers).forEach(eventType => {
        parent.addEventListener(eventType, function(e) {
            targets.forEach(target => {
                const selector = target.selector || target;
                if (e.target.matches(selector)) {
                    eventHandlers[eventType].call(e.target, e);
                }
            });
        }, true);
    });
}
)";
        delegateHelperGenerated = true;
    }
    
    // 生成委托调用
    jsOutput_ << "_chtlDelegate(";
    if (node->getParent()) {
        node->getParent()->accept(this);
    }
    jsOutput_ << ", {\n";
    
    // 输出目标选择器
    jsOutput_ << "    targets: [";
    const auto& config = node->getDelegateConfig();
    for (size_t i = 0; i < config.targets.size(); ++i) {
        if (i > 0) jsOutput_ << ", ";
        config.targets[i]->accept(this);
    }
    jsOutput_ << "],\n";
    
    // 输出事件处理器
    jsOutput_ << "    " << config.eventName << ": ";
    if (config.handler) {
        config.handler->accept(this);
    }
    jsOutput_ << "\n});\n";
}

void HtmlGenerator::visitAnimateCall(AnimateCallNode* node) {
    switchToJs();
    
    // 生成动画辅助函数（如果还没有）
    static bool animateHelperGenerated = false;
    if (!animateHelperGenerated) {
        jsOutput_ << R"(
// CHTL JS 动画辅助函数
function _chtlAnimate(config) {
    const targets = Array.isArray(config.target) ? config.target : [config.target];
    const duration = config.duration || 1000;
    const easing = config.easing || 'linear';
    const startTime = performance.now();
    
    const easingFunctions = {
        linear: t => t,
        easeIn: t => t * t,
        easeOut: t => t * (2 - t),
        easeInOut: t => t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t
    };
    
    const easingFunc = easingFunctions[easing] || easingFunctions.linear;
    
    function animate(currentTime) {
        const elapsed = currentTime - startTime;
        const progress = Math.min(elapsed / duration, 1);
        const easedProgress = easingFunc(progress);
        
        // 应用动画属性
        if (config.properties) {
            targets.forEach(target => {
                Object.keys(config.properties).forEach(prop => {
                    const [from, to] = config.properties[prop];
                    const current = from + (to - from) * easedProgress;
                    target.style[prop] = current + (prop === 'opacity' ? '' : 'px');
                });
            });
        }
        
        if (config.onUpdate) config.onUpdate(progress);
        
        if (progress < 1) {
            requestAnimationFrame(animate);
        } else if (config.onComplete) {
            config.onComplete();
        }
    }
    
    requestAnimationFrame(animate);
}
)";
        animateHelperGenerated = true;
    }
    
    // 生成动画调用
    const auto& config = node->getConfig();
    jsOutput_ << "_chtlAnimate({\n";
    jsOutput_ << "    target: ";
    
    // 输出目标
    if (config.targets.size() == 1) {
        config.targets[0]->accept(this);
    } else {
        jsOutput_ << "[";
        for (size_t i = 0; i < config.targets.size(); ++i) {
            if (i > 0) jsOutput_ << ", ";
            config.targets[i]->accept(this);
        }
        jsOutput_ << "]";
    }
    jsOutput_ << ",\n";
    
    // 输出配置
    jsOutput_ << "    duration: " << config.duration << ",\n";
    jsOutput_ << "    easing: '" << config.easing << "',\n";
    
    // 输出属性动画
    if (!config.properties.empty()) {
        jsOutput_ << "    properties: {\n";
        for (const auto& [prop, values] : config.properties) {
            jsOutput_ << "        " << prop << ": [" 
                      << values.first << ", " << values.second << "],\n";
        }
        jsOutput_ << "    },\n";
    }
    
    // 回调函数
    if (config.onComplete) {
        jsOutput_ << "    onComplete: ";
        config.onComplete->accept(this);
        jsOutput_ << ",\n";
    }
    
    if (config.onUpdate) {
        jsOutput_ << "    onUpdate: ";
        config.onUpdate->accept(this);
        jsOutput_ << "\n";
    }
    
    jsOutput_ << "});\n";
}

void HtmlGenerator::visitChtlJsExpression(ChtlJsExpressionNode* node) {
    switchToJs();
    jsOutput_ << node->getJavaScriptCode();
}

void HtmlGenerator::visitVirtualObject(VirtualObjectNode* node) {
    // 虚对象不直接生成代码，而是注册到函数管理器
    // 初始化表达式会生成对应的代码
    if (node->getInitExpression()) {
        node->getInitExpression()->accept(this);
    }
}

void HtmlGenerator::visitINeverAway(INeverAwayNode* node) {
    switchToJs();
    
    // 为每个函数生成全局函数
    for (const auto& func : node->getFunctions()) {
        jsOutput_ << "function " << func.generatedName << "(";
        
        // 参数列表
        for (size_t i = 0; i < func.params.size(); ++i) {
            if (i > 0) jsOutput_ << ", ";
            jsOutput_ << "param" << i;
        }
        
        jsOutput_ << ") {\n";
        
        // 函数体
        if (func.body) {
            func.body->accept(this);
        }
        
        jsOutput_ << "\n}\n\n";
    }
}

void HtmlGenerator::visitVirtualCall(VirtualCallNode* node) {
    switchToJs();
    
    // 查找对应的全局函数
    auto& manager = ChtlJsFunctionRegistry::getInstance().getManager();
    auto funcInfo = manager.findFunction(node->getObjectName(), 
                                       node->getFunctionName(), 
                                       node->getState());
    
    if (funcInfo) {
        // 生成全局函数调用
        jsOutput_ << funcInfo->generatedName << "(";
        
        // 参数列表
        const auto& args = node->getArguments();
        for (size_t i = 0; i < args.size(); ++i) {
            if (i > 0) jsOutput_ << ", ";
            args[i]->accept(this);
        }
        
        jsOutput_ << ")";
    } else {
        // 错误：找不到对应的函数
        jsOutput_ << "/* ERROR: Virtual function not found: " 
                  << node->getObjectName() << "->" 
                  << node->getFunctionName();
        if (!node->getState().empty()) {
            jsOutput_ << "<" << node->getState() << ">";
        }
        jsOutput_ << " */";
    }
}

} // namespace Chtl
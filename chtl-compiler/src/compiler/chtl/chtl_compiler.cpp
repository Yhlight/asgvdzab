#include "compiler/chtl/chtl_compiler.h"
#include "compiler/chtl/chtl_parser.h"
#include "common/utils.h"
#include <sstream>

namespace chtl {
namespace compiler {

CHTLCompiler::CHTLCompiler() 
    : lexer_(std::make_unique<CHTLLexer>("", "")),
      parser_(std::make_unique<CHTLParser>()) {
}

CHTLCompiler::~CHTLCompiler() = default;

CompilationResult CHTLCompiler::compile(const CodeSegment& segment,
                                       const CompilerOptions& options) {
    CompilationResult result;
    
    try {
        // 重新初始化词法分析器
        lexer_ = std::make_unique<CHTLLexer>(segment.content, segment.location.filename);
        
        // 解析
        auto ast = parser_->parse(*lexer_, context_);
        if (!ast) {
            result.success = false;
            result.errors.push_back("Failed to parse CHTL code");
            return result;
        }
        
        // 设置当前元素上下文
        if (!segment.parentElement.empty()) {
            context_.currentElement = segment.parentElement;
            context_.elementStack.push(segment.parentElement);
        }
        
        // 生成HTML
        std::stringstream output;
        compileNode(ast, output);
        result.output = output.str();
        
        // 收集全局样式和脚本
        auto& globalMap = context_.globalMap;
        auto globalStyles = globalMap->getGlobalStyles();
        if (!globalStyles.empty()) {
            result.globalStyles = utils::join(globalStyles, "\n");
        }
        auto globalScripts = globalMap->getGlobalScripts();
        if (!globalScripts.empty()) {
            result.globalScripts = utils::join(globalScripts, "\n");
        }
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back(std::string("Compilation error: ") + e.what());
    }
    
    return result;
}

void CHTLCompiler::reset() {
    context_ = CHTLContext();
}

bool CHTLCompiler::supports(CodeSegmentType type) const {
    return type == CodeSegmentType::CHTL;
}

std::string CHTLCompiler::getName() const {
    return "CHTL Compiler";
}

void CHTLCompiler::compileNode(std::shared_ptr<ast::ASTNode> node, 
                              std::stringstream& output) {
    if (!node) return;
    
    switch (node->type) {
        case ast::NodeType::DOCUMENT:
            compileDocument(std::static_pointer_cast<ast::DocumentNode>(node), output);
            break;
        case ast::NodeType::ELEMENT:
            compileElement(std::static_pointer_cast<ast::ElementNode>(node), output);
            break;
        case ast::NodeType::TEXT:
            compileText(std::static_pointer_cast<ast::TextNode>(node), output);
            break;
        case ast::NodeType::STYLE:
            compileStyle(std::static_pointer_cast<ast::StyleNode>(node), output);
            break;
        case ast::NodeType::SCRIPT:
            compileScript(std::static_pointer_cast<ast::ScriptNode>(node), output);
            break;
        case ast::NodeType::TEMPLATE:
            compileTemplate(std::static_pointer_cast<ast::TemplateNode>(node), output);
            break;
        case ast::NodeType::CUSTOM:
            compileCustom(std::static_pointer_cast<ast::CustomNode>(node), output);
            break;
        case ast::NodeType::IMPORT:
            compileImport(std::static_pointer_cast<ast::ImportNode>(node), output);
            break;
        case ast::NodeType::NAMESPACE:
            compileNamespace(std::static_pointer_cast<ast::NamespaceNode>(node), output);
            break;
        case ast::NodeType::ORIGIN:
            compileOrigin(std::static_pointer_cast<ast::OriginNode>(node), output);
            break;
        case ast::NodeType::CONFIGURATION:
            compileConfiguration(std::static_pointer_cast<ast::ConfigurationNode>(node), output);
            break;
        case ast::NodeType::COMMENT:
            compileComment(std::static_pointer_cast<ast::CommentNode>(node), output);
            break;
    }
}

void CHTLCompiler::compileDocument(std::shared_ptr<ast::DocumentNode> node,
                                  std::stringstream& output) {
    for (const auto& child : node->children) {
        compileNode(child, output);
    }
}

void CHTLCompiler::compileElement(std::shared_ptr<ast::ElementNode> node,
                                 std::stringstream& output) {
    // 保存之前的元素上下文
    std::string prevElement = context_.currentElement;
    std::string prevClass = context_.currentElementClass;
    std::string prevId = context_.currentElementId;
    
    // 更新当前元素上下文
    context_.currentElement = node->tagName;
    context_.currentElementClass = node->className;
    context_.currentElementId = node->id;
    context_.elementStack.push(node->tagName);
    context_.pushState(CHTLCompilerState::IN_ELEMENT);
    
    // 生成开始标签
    output << "<" << node->tagName;
    
    // 处理属性
    for (const auto& attr : node->attributes) {
        output << " " << attr->name;
        if (!attr->value.empty()) {
            output << "=\"" << utils::escapeHtml(attr->value) << "\"";
        }
    }
    
    // 添加class属性
    if (!node->className.empty()) {
        output << " class=\"" << node->className << "\"";
    }
    
    // 添加id属性  
    if (!node->id.empty()) {
        output << " id=\"" << node->id << "\"";
    }
    
    output << ">";
    
    // 编译子节点
    for (const auto& child : node->children) {
        compileNode(child, output);
    }
    
    // 生成结束标签
    output << "</" << node->tagName << ">";
    
    // 恢复之前的上下文
    context_.popState();
    context_.elementStack.pop();
    context_.currentElement = prevElement;
    context_.currentElementClass = prevClass;
    context_.currentElementId = prevId;
}

void CHTLCompiler::compileText(std::shared_ptr<ast::TextNode> node,
                              std::stringstream& output) {
    output << utils::escapeHtml(node->content);
}

void CHTLCompiler::compileStyle(std::shared_ptr<ast::StyleNode> node,
                               std::stringstream& output) {
    if (node->isLocal) {
        // 局部样式 - 提取内联样式或生成class
        std::string inlineStyles = extractInlineStyles(node->content);
        if (!inlineStyles.empty()) {
            // TODO: 将内联样式应用到当前元素
        }
        
        // 将局部样式转换为全局样式
        std::string processedStyle = processLocalStyle(node->content);
        context_.globalMap->addGlobalStyle(processedStyle);
    } else {
        // 全局样式 - 直接添加到全局样式集合
        context_.globalMap->addGlobalStyle(node->content);
    }
}

void CHTLCompiler::compileScript(std::shared_ptr<ast::ScriptNode> node,
                                std::stringstream& output) {
    if (node->isLocal) {
        // 局部脚本 - 添加到局部脚本集合
        context_.localScripts += node->content + "\n";
    } else {
        // 全局脚本 - 添加到全局脚本集合
        context_.globalMap->addGlobalScript(node->content);
    }
}

void CHTLCompiler::compileTemplate(std::shared_ptr<ast::TemplateNode> node,
                                  std::stringstream& output) {
    // 模板定义 - 存储到全局映射表
    switch (node->templateType) {
        case ast::TemplateNode::TemplateType::STYLE_TEMPLATE: {
            StyleTemplate tmpl;
            tmpl.name = node->name;
            tmpl.location = node->location;
            // TODO: 解析样式属性
            context_.globalMap->addStyleTemplate(node->name, tmpl);
            break;
        }
        case ast::TemplateNode::TemplateType::ELEMENT_TEMPLATE: {
            ElementTemplate tmpl;
            tmpl.name = node->name;
            tmpl.elements = node->children;
            tmpl.location = node->location;
            context_.globalMap->addElementTemplate(node->name, tmpl);
            break;
        }
        case ast::TemplateNode::TemplateType::VAR_TEMPLATE: {
            VarTemplate tmpl;
            tmpl.name = node->name;
            tmpl.location = node->location;
            // TODO: 解析变量定义
            context_.globalMap->addVarTemplate(node->name, tmpl);
            break;
        }
    }
}

void CHTLCompiler::compileCustom(std::shared_ptr<ast::CustomNode> node,
                                std::stringstream& output) {
    // 自定义定义 - 存储到全局映射表
    CustomDefinition custom;
    custom.name = node->name;
    custom.baseElement = node->baseElement;
    custom.elements = node->children;
    custom.location = node->location;
    // TODO: 解析操作
    context_.globalMap->addCustomDefinition(node->name, custom);
}

void CHTLCompiler::compileImport(std::shared_ptr<ast::ImportNode> node,
                                std::stringstream& output) {
    // 导入处理
    ImportInfo import;
    import.path = node->path;
    import.alias = node->alias;
    import.exceptions = node->exceptions;
    import.importAll = node->exceptions.empty();
    import.location = node->location;
    context_.globalMap->addImport(import);
}

void CHTLCompiler::compileNamespace(std::shared_ptr<ast::NamespaceNode> node,
                                   std::stringstream& output) {
    // 命名空间处理
    std::string prevNamespace = context_.currentNamespace;
    context_.currentNamespace = node->name;
    context_.pushState(CHTLCompilerState::IN_NAMESPACE);
    
    NamespaceInfo ns;
    ns.name = node->name;
    ns.alias = node->alias;
    ns.content = node->children;
    ns.location = node->location;
    context_.globalMap->addNamespace(node->name, ns);
    
    // 编译命名空间内容
    for (const auto& child : node->children) {
        compileNode(child, output);
    }
    
    context_.popState();
    context_.currentNamespace = prevNamespace;
}

void CHTLCompiler::compileOrigin(std::shared_ptr<ast::OriginNode> node,
                                std::stringstream& output) {
    // 原始嵌入 - 直接输出内容
    output << node->content;
}

void CHTLCompiler::compileConfiguration(std::shared_ptr<ast::ConfigurationNode> node,
                                       std::stringstream& output) {
    // 配置组处理
    ConfigurationGroup config;
    config.name = node->name;
    // 将map转换为unordered_map
    for (const auto& [key, value] : node->settings) {
        config.settings[key] = value;
    }
    config.location = node->location;
    context_.globalMap->addConfigurationGroup(node->name, config);
}

void CHTLCompiler::compileComment(std::shared_ptr<ast::CommentNode> node,
                                 std::stringstream& output) {
    if (node->isSemantic) {
        // 语义注释 - 输出到HTML
        output << "<!-- " << node->content << " -->";
    }
    // 普通注释不输出
}

std::string CHTLCompiler::extractInlineStyles(const std::string& styleContent) {
    // TODO: 实现内联样式提取
    return "";
}

std::string CHTLCompiler::processLocalStyle(const std::string& styleContent) {
    // TODO: 实现局部样式处理
    // 1. 替换 & 为当前元素的类名或ID
    // 2. 添加作用域前缀
    return styleContent;
}

std::string CHTLCompiler::expandTemplate(const std::string& templateName,
                                       const std::map<std::string, std::string>& params) {
    // TODO: 实现模板展开
    return "";
}

std::string CHTLCompiler::replaceVarReferences(const std::string& content) {
    // TODO: 实现变量引用替换
    return content;
}

void CHTLCompiler::processImport(const std::string& path, const std::string& alias) {
    // TODO: 实现导入处理
}

} // namespace compiler
} // namespace chtl
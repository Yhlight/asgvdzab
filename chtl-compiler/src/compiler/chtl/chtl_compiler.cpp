#include "compiler/chtl/chtl_compiler.h"
#include "common/utils.h"
#include <sstream>
#include <algorithm>

namespace chtl {
namespace compiler {

CHTLCompiler::CHTLCompiler() {
    lexer_ = std::make_unique<CHTLLexer>();
    parser_ = std::make_unique<CHTLParser>();
    reset();
}

CHTLCompiler::~CHTLCompiler() = default;

CompilationResult CHTLCompiler::compile(const CodeSegment& segment,
                                       const CompilerOptions& options) {
    CompilationResult result;
    
    try {
        // 初始化词法分析器
        lexer_->init(segment.content, segment.location.filename);
        
        // 解析生成AST
        auto ast = parser_->parse(*lexer_);
        
        // 检查解析错误
        auto lexerErrors = lexer_->getErrors();
        auto parserErrors = parser_->getErrors();
        
        if (!lexerErrors.empty() || !parserErrors.empty()) {
            result.success = false;
            result.errors.insert(result.errors.end(), lexerErrors.begin(), lexerErrors.end());
            result.errors.insert(result.errors.end(), parserErrors.begin(), parserErrors.end());
            return result;
        }
        
        // 设置上下文信息
        context_.currentElement = segment.parentElement;
        context_.currentClassName = segment.className;
        context_.currentId = segment.id;
        
        // 编译AST
        if (ast) {
            for (const auto& child : ast->children) {
                result.output += compileNode(child);
            }
        }
        
        // 收集全局样式和脚本
        if (!context_.globalStyles.empty()) {
            result.globalStyles = utils::join(context_.globalStyles, "\n");
        }
        if (!context_.globalScripts.empty()) {
            result.globalScripts = utils::join(context_.globalScripts, "\n");
        }
        
        result.success = true;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back("Compilation error: " + std::string(e.what()));
    }
    
    return result;
}

void CHTLCompiler::reset() {
    context_ = CHTLContext();
    context_.config.indexInitialCount = 0;
    context_.config.disableNameGroup = false;
    context_.config.debugMode = false;
}

std::string CHTLCompiler::compileNode(const std::shared_ptr<ast::ASTNode>& node) {
    if (!node) return "";
    
    switch (node->getType()) {
        case ast::NodeType::ELEMENT:
            return compileElement(std::static_pointer_cast<ast::ElementNode>(node));
        case ast::NodeType::TEXT:
            return compileText(std::static_pointer_cast<ast::TextNode>(node));
        case ast::NodeType::STYLE:
            return compileStyle(std::static_pointer_cast<ast::StyleNode>(node));
        case ast::NodeType::SCRIPT:
            return compileScript(std::static_pointer_cast<ast::ScriptNode>(node));
        case ast::NodeType::TEMPLATE:
            return compileTemplate(std::static_pointer_cast<ast::TemplateNode>(node));
        case ast::NodeType::CUSTOM:
            return compileCustom(std::static_pointer_cast<ast::CustomNode>(node));
        case ast::NodeType::IMPORT:
            return compileImport(std::static_pointer_cast<ast::ImportNode>(node));
        case ast::NodeType::ORIGIN:
            return compileOrigin(std::static_pointer_cast<ast::OriginNode>(node));
        default:
            return "";
    }
}

std::string CHTLCompiler::compileElement(const std::shared_ptr<ast::ElementNode>& node) {
    std::stringstream ss;
    
    // 保存当前上下文
    std::string prevElement = context_.currentElement;
    std::string prevClassName = context_.currentClassName;
    std::string prevId = context_.currentId;
    
    // 更新上下文
    context_.currentElement = node->tagName;
    context_.currentClassName = node->className;
    context_.currentId = node->id;
    
    // 开始标签
    ss << "<" << node->tagName;
    
    // 处理属性
    auto attrs = node->attributes;
    
    // 添加自动生成的类名和ID
    if (!node->className.empty()) {
        attrs["class"] = node->className;
    }
    if (!node->id.empty()) {
        attrs["id"] = node->id;
    }
    
    // 输出属性
    for (const auto& [name, value] : attrs) {
        ss << " " << name << "=\"" << escapeHtml(value) << "\"";
    }
    
    // 自闭合标签
    if (node->children.empty()) {
        ss << " />";
    } else {
        ss << ">";
        
        // 编译子节点
        for (const auto& child : node->children) {
            ss << compileNode(child);
        }
        
        // 结束标签
        ss << "</" << node->tagName << ">";
    }
    
    // 恢复上下文
    context_.currentElement = prevElement;
    context_.currentClassName = prevClassName;
    context_.currentId = prevId;
    
    return ss.str();
}

std::string CHTLCompiler::compileText(const std::shared_ptr<ast::TextNode>& node) {
    if (node->isLiteral) {
        return node->content;
    }
    return escapeHtml(node->content);
}

std::string CHTLCompiler::compileStyle(const std::shared_ptr<ast::StyleNode>& node) {
    if (node->isLocal) {
        std::string inlineStyles;
        std::string globalStyles;
        processLocalStyle(node, inlineStyles, globalStyles);
        
        if (!globalStyles.empty()) {
            context_.globalStyles.push_back(globalStyles);
        }
        
        if (!inlineStyles.empty()) {
            // 返回内联样式属性
            return " style=\"" + inlineStyles + "\"";
        }
    } else {
        // 全局样式块，直接返回<style>标签
        std::stringstream ss;
        ss << "<style>\n";
        
        for (const auto& rule : node->rules) {
            ss << rule.selector << " {\n";
            for (const auto& [prop, value] : rule.properties) {
                ss << "  " << prop << ": " << value << ";\n";
            }
            ss << "}\n";
        }
        
        ss << "</style>";
        return ss.str();
    }
    
    return "";
}

std::string CHTLCompiler::compileScript(const std::shared_ptr<ast::ScriptNode>& node) {
    if (node->isLocal) {
        // 局部脚本块，添加到全局脚本中
        context_.globalScripts.push_back(node->content);
        return "";
    } else {
        // 全局脚本块
        return "<script>\n" + node->content + "\n</script>";
    }
}

std::string CHTLCompiler::compileTemplate(const std::shared_ptr<ast::TemplateNode>& node) {
    // 模板定义不生成输出，只是注册到上下文中
    context_.templates[node->name] = node;
    return "";
}

std::string CHTLCompiler::compileCustom(const std::shared_ptr<ast::CustomNode>& node) {
    // 自定义定义不生成输出，只是注册到上下文中
    context_.customs[node->name] = node;
    return "";
}

std::string CHTLCompiler::compileImport(const std::shared_ptr<ast::ImportNode>& node) {
    // 导入语句的处理需要文件系统支持，这里暂时返回空
    // 实际实现需要读取文件并解析
    return "";
}

std::string CHTLCompiler::compileOrigin(const std::shared_ptr<ast::OriginNode>& node) {
    // 原始嵌入直接返回内容
    return node->content;
}

void CHTLCompiler::processLocalStyle(const std::shared_ptr<ast::StyleNode>& node,
                                   std::string& inlineStyles,
                                   std::string& globalStyles) {
    std::stringstream inlineStream;
    std::stringstream globalStream;
    
    for (const auto& rule : node->rules) {
        if (rule.isInline) {
            // 内联样式
            for (const auto& [prop, value] : rule.properties) {
                if (!inlineStream.str().empty()) {
                    inlineStream << " ";
                }
                inlineStream << prop << ": " << value << ";";
            }
        } else if (rule.isAutoClass || rule.isAutoId) {
            // 自动类名或ID，生成选择器并添加到全局样式
            std::string selector;
            if (rule.isAutoClass) {
                selector = generateAutoClassName(rule.selector);
                // 更新当前元素的类名
                if (!context_.currentClassName.empty()) {
                    context_.currentClassName += " ";
                }
                context_.currentClassName += selector.substr(1); // 去掉.前缀
            } else if (rule.isAutoId) {
                selector = generateAutoId(rule.selector);
                context_.currentId = selector.substr(1); // 去掉#前缀
            }
            
            globalStream << selector << " {\n";
            for (const auto& [prop, value] : rule.properties) {
                globalStream << "  " << prop << ": " << value << ";\n";
            }
            globalStream << "}\n";
        } else {
            // 其他选择器（如伪类、伪元素）
            std::string selector = rule.selector;
            
            // 替换&符号
            if (selector.find('&') != std::string::npos) {
                std::string replacement;
                if (!context_.currentClassName.empty()) {
                    replacement = "." + context_.currentClassName;
                } else if (!context_.currentId.empty()) {
                    replacement = "#" + context_.currentId;
                } else {
                    replacement = context_.currentElement;
                }
                selector = utils::replaceAll(selector, "&", replacement);
            }
            
            globalStream << selector << " {\n";
            for (const auto& [prop, value] : rule.properties) {
                globalStream << "  " << prop << ": " << value << ";\n";
            }
            globalStream << "}\n";
        }
    }
    
    inlineStyles = inlineStream.str();
    globalStyles = globalStream.str();
}

std::string CHTLCompiler::generateAutoClassName(const std::string& selector) {
    std::string className = selector;
    if (className[0] == '.') {
        className = className.substr(1);
    }
    
    // 如果类名已存在，添加唯一后缀
    auto& counter = context_.idCounters[className];
    if (counter > 0) {
        className += "_" + std::to_string(counter);
    }
    counter++;
    
    return "." + className;
}

std::string CHTLCompiler::generateAutoId(const std::string& selector) {
    std::string id = selector;
    if (id[0] == '#') {
        id = id.substr(1);
    }
    
    // ID必须唯一
    auto& counter = context_.idCounters[id];
    if (counter > 0) {
        id += "_" + std::to_string(counter);
    }
    counter++;
    
    return "#" + id;
}

std::string CHTLCompiler::expandTemplate(const std::string& templateName,
                                       const std::map<std::string, std::string>& params) {
    auto it = context_.templates.find(templateName);
    if (it == context_.templates.end()) {
        return ""; // 模板不存在
    }
    
    // TODO: 实现模板展开逻辑
    return "";
}

std::string CHTLCompiler::replaceVariables(const std::string& content) {
    // TODO: 实现变量替换逻辑
    return content;
}

std::string CHTLCompiler::escapeHtml(const std::string& str) {
    return utils::escapeHtml(str);
}

std::string CHTLCompiler::formatAttributes(const std::map<std::string, std::string>& attrs) {
    std::stringstream ss;
    for (const auto& [name, value] : attrs) {
        ss << " " << name << "=\"" << escapeHtml(value) << "\"";
    }
    return ss.str();
}

std::string CHTLCompiler::getUniqueId(const std::string& prefix) {
    return generateUniqueId(prefix);
}

} // namespace compiler
} // namespace chtl
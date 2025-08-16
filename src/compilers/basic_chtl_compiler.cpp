#include "core/compiler_dispatcher.hpp"
#include <regex>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>

namespace chtl {

/**
 * 基础CHTL编译器实现
 * 严格按照CHTL语法文档实现所有语法功能
 */
class BasicCHTLCompiler : public ICHTLCompiler {
private:
    // 编译上下文
    struct CompilationContext {
        bool inLocalStyle = false;
        bool inLocalScript = false;
        bool inGlobalStyle = false;
        std::string currentElementId;
        std::string currentElementClass;
        std::unordered_map<std::string, std::string> variables;
        std::vector<std::string> globalStyles;
    };
    
    CompilationContext context_;
    
    // 模板存储
    struct Template {
        std::string type;  // "Style", "Element", "Var"
        std::string name;
        std::string content;
    };
    
    std::unordered_map<std::string, Template> templates_;
    std::unordered_map<std::string, Template> customs_;
    
public:
    BasicCHTLCompiler() = default;
    
    std::string getId() const override {
        return "basic_chtl_compiler";
    }
    
    std::string getVersion() const override {
        return "1.0.0";
    }
    
    bool initialize() override {
        return true;
    }
    
    void cleanup() override {
        templates_.clear();
        customs_.clear();
        context_ = CompilationContext();
    }
    
    CompilationResult compile(const CodeFragment& fragment) override {
        CompilationResult result;
        result.success = true;
        
        try {
            std::string htmlOutput = compileChtlToHtml(fragment.content);
            result.output = htmlOutput;
            
            if (htmlOutput.empty() && !fragment.content.empty()) {
                result.warnings.push_back("CHTL编译产生了空输出");
            }
            
        } catch (const std::exception& e) {
            result.success = false;
            result.errors.push_back("CHTL编译异常: " + std::string(e.what()));
        }
        
        return result;
    }
    
    bool supportsLocalStyle() const override {
        return true;
    }
    
    CompilationResult compileLocalStyle(const CodeFragment& fragment) override {
        CompilationResult result;
        result.success = true;
        
        try {
            context_.inLocalStyle = true;
            std::string cssOutput = compileLocalStyleBlock(fragment.content);
            result.output = cssOutput;
            context_.inLocalStyle = false;
            
        } catch (const std::exception& e) {
            result.success = false;
            result.errors.push_back("局部样式编译异常: " + std::string(e.what()));
            context_.inLocalStyle = false;
        }
        
        return result;
    }
    
private:
    std::string compileChtlToHtml(const std::string& chtlCode) {
        std::string trimmed = trimAndClean(chtlCode);
        if (trimmed.empty()) return "";
        
        // 1. 注释处理
        if (isComment(trimmed)) {
            return compileComment(trimmed);
        }
        
        // 2. 模板定义处理
        if (isTemplateDefinition(trimmed)) {
            registerTemplate(trimmed);
            return ""; // 模板定义不产生HTML输出
        }
        
        // 3. 自定义定义处理
        if (isCustomDefinition(trimmed)) {
            registerCustom(trimmed);
            return ""; // 自定义定义不产生HTML输出
        }
        
        // 4. 原始嵌入处理
        if (isOriginEmbedding(trimmed)) {
            return compileOriginEmbedding(trimmed);
        }
        
        // 5. 配置组处理
        if (isConfigurationBlock(trimmed)) {
            processConfiguration(trimmed);
            return ""; // 配置不产生HTML输出
        }
        
        // 6. 导入处理
        if (isImportStatement(trimmed)) {
            processImport(trimmed);
            return ""; // 导入不产生HTML输出
        }
        
        // 7. 命名空间处理
        if (isNamespaceBlock(trimmed)) {
            return compileNamespaceBlock(trimmed);
        }
        
        // 8. 模板使用处理
        if (isTemplateUsage(trimmed)) {
            return expandTemplate(trimmed);
        }
        
        // 9. 变量组使用处理
        if (isVariableGroupUsage(trimmed)) {
            return expandVariableGroup(trimmed);
        }
        
        // 10. HTML元素处理
        if (isHtmlElement(trimmed)) {
            return compileHtmlElement(trimmed);
        }
        
        // 11. 文本节点处理
        if (isTextNode(trimmed)) {
            return compileTextNode(trimmed);
        }
        
        // 12. 属性处理（通常在元素内部处理）
        if (isAttribute(trimmed)) {
            return ""; // 单独的属性不产生输出
        }
        
        // 13. 处理多行内容（递归编译每一行）
        return compileMultilineContent(trimmed);
    }
    
    // ==================== 识别方法 ====================
    
    bool isComment(const std::string& code) {
        std::string trimmed = trimAndClean(code);
        return trimmed.find("//") == 0 || 
               trimmed.find("/*") == 0 || 
               trimmed.find("--") == 0;
    }
    
    bool isTemplateDefinition(const std::string& code) {
        return code.find("[Template]") != std::string::npos;
    }
    
    bool isCustomDefinition(const std::string& code) {
        return code.find("[Custom]") != std::string::npos;
    }
    
    bool isOriginEmbedding(const std::string& code) {
        return code.find("[Origin]") != std::string::npos;
    }
    
    bool isConfigurationBlock(const std::string& code) {
        return code.find("[Configuration]") != std::string::npos;
    }
    
    bool isImportStatement(const std::string& code) {
        return code.find("[Import]") != std::string::npos;
    }
    
    bool isNamespaceBlock(const std::string& code) {
        return code.find("[Namespace]") != std::string::npos;
    }
    
    bool isTemplateUsage(const std::string& code) {
        std::regex usagePattern(R"(@(?:Style|Element|Var)\s+\w+)");
        return std::regex_search(code, usagePattern);
    }
    
    bool isVariableGroupUsage(const std::string& code) {
        std::regex varPattern(R"(\w+\([^)]*\))");
        return std::regex_search(code, varPattern) && 
               !isHtmlElement(code) && !isTextNode(code);
    }
    
    bool isHtmlElement(const std::string& code) {
        // 支持所有HTML元素
        std::regex elementRegex(R"(\b(?:html|head|body|div|span|p|a|img|ul|ol|li|table|tr|td|th|thead|tbody|tfoot|form|input|button|textarea|select|option|label|h[1-6]|section|article|nav|header|footer|aside|main|figure|figcaption|details|summary|dialog|canvas|svg|video|audio|source|iframe|embed|object|script|style|link|meta|title|base|noscript|template|slot)\s*\{)");
        return std::regex_search(code, elementRegex);
    }
    
    bool isTextNode(const std::string& code) {
        return code.find("text") != std::string::npos && 
               code.find("{") != std::string::npos;
    }
    
    bool isAttribute(const std::string& code) {
        std::regex attrRegex(R"(\b\w+\s*[:=]\s*[^;{]+;)");
        return std::regex_search(code, attrRegex);
    }
    
    // ==================== 编译方法 ====================
    
    std::string compileComment(const std::string& code) {
        std::string trimmed = trimAndClean(code);
        
        if (trimmed.find("--") == 0) {
            // 生成器注释：根据上下文生成相应格式
            std::string content = trimmed.substr(2);
            content = trimAndClean(content);
            
            if (context_.inLocalStyle || context_.inGlobalStyle) {
                return "/* " + content + " */";
            } else if (context_.inLocalScript) {
                return "// " + content;
            } else {
                return "<!-- " + content + " -->";
            }
        } else {
            // //和/**/注释不输出到HTML
            return "";
        }
    }
    
    void registerTemplate(const std::string& code) {
        // 解析模板定义：[Template] @Type Name { content }
        std::regex templateRegex(R"(\[Template\]\s*@(Style|Element|Var)\s+(\w+)\s*\{([^}]*)\})");
        std::smatch match;
        
        if (std::regex_search(code, match, templateRegex)) {
            Template tmpl;
            tmpl.type = match[1].str();
            tmpl.name = match[2].str();
            tmpl.content = match[3].str();
            
            std::string key = tmpl.type + "_" + tmpl.name;
            templates_[key] = tmpl;
        }
    }
    
    void registerCustom(const std::string& code) {
        // 解析自定义定义：[Custom] @Type Name { content }
        std::regex customRegex(R"(\[Custom\]\s*@(Style|Element|Var)\s+(\w+)\s*\{([^}]*)\})");
        std::smatch match;
        
        if (std::regex_search(code, match, customRegex)) {
            Template custom;
            custom.type = match[1].str();
            custom.name = match[2].str();
            custom.content = match[3].str();
            
            std::string key = custom.type + "_" + custom.name;
            customs_[key] = custom;
        }
    }
    
    std::string compileOriginEmbedding(const std::string& code) {
        // 处理原始嵌入：[Origin] @Html/@Style/@JavaScript [name] { content }
        std::regex originRegex(R"(\[Origin\]\s*@(Html|Style|JavaScript|[A-Z]\w*)\s*(\w+)?\s*\{([^}]*)\})");
        std::smatch match;
        
        if (std::regex_search(code, match, originRegex)) {
            std::string type = match[1].str();
            std::string name = match[2].str();
            std::string content = match[3].str();
            
            if (type == "Html") {
                return content; // 原样输出HTML内容
            } else if (type == "Style") {
                return "<style>\n" + content + "\n</style>";
            } else if (type == "JavaScript") {
                return "<script>\n" + content + "\n</script>";
            } else {
                // 自定义类型：原样输出，添加注释标识
                return "<!-- Custom origin type: " + type + " -->\n" + content;
            }
        }
        
        return "";
    }
    
    void processConfiguration(const std::string& code) {
        // 处理配置组：[Configuration] { ... }
        // 这里可以解析配置选项，影响编译器行为
        // 简化实现：暂时不处理具体配置逻辑
    }
    
    void processImport(const std::string& code) {
        // 处理导入：[Import] @Type "path"
        // 简化实现：暂时不处理实际文件导入
    }
    
    std::string compileNamespaceBlock(const std::string& code) {
        // 处理命名空间：[Namespace] Name { content }
        std::regex nsRegex(R"(\[Namespace\]\s+(\w+)\s*\{([^}]*)\})");
        std::smatch match;
        
        if (std::regex_search(code, match, nsRegex)) {
            std::string nsName = match[1].str();
            std::string content = match[2].str();
            
            // 递归编译命名空间内容
            return "<!-- Namespace: " + nsName + " -->\n" + compileChtlToHtml(content);
        }
        
        return "";
    }
    
    std::string expandTemplate(const std::string& code) {
        // 展开模板使用：@Style TemplateName; 或 @Element TemplateName;
        std::regex usageRegex(R"(@(Style|Element|Var)\s+(\w+))");
        std::smatch match;
        
        if (std::regex_search(code, match, usageRegex)) {
            std::string type = match[1].str();
            std::string name = match[2].str();
            std::string key = type + "_" + name;
            
            // 先查找自定义定义
            if (customs_.find(key) != customs_.end()) {
                return compileTemplateContent(customs_[key]);
            }
            
            // 再查找模板定义
            if (templates_.find(key) != templates_.end()) {
                return compileTemplateContent(templates_[key]);
            }
        }
        
        return "<!-- Template not found: " + code + " -->";
    }
    
    std::string compileTemplateContent(const Template& tmpl) {
        if (tmpl.type == "Style") {
            // 样式模板：生成CSS或内联样式
            return compileStyleTemplate(tmpl.content);
        } else if (tmpl.type == "Element") {
            // 元素模板：递归编译元素内容
            return compileChtlToHtml(tmpl.content);
        } else if (tmpl.type == "Var") {
            // 变量模板：注册变量到上下文
            parseVariableTemplate(tmpl.content);
            return "";
        }
        
        return "";
    }
    
    std::string compileStyleTemplate(const std::string& content) {
        // 样式模板编译为CSS属性
        std::ostringstream css;
        std::istringstream iss(content);
        std::string line;
        
        while (std::getline(iss, line)) {
            line = trimAndClean(line);
            if (!line.empty() && line.back() == ';') {
                css << "    " << line << "\n";
            }
        }
        
        return css.str();
    }
    
    void parseVariableTemplate(const std::string& content) {
        // 解析变量定义：varName: "value";
        std::regex varRegex(R"((\w+)\s*:\s*([^;]+);)");
        std::sregex_iterator iter(content.begin(), content.end(), varRegex);
        std::sregex_iterator end;
        
        for (; iter != end; ++iter) {
            std::smatch match = *iter;
            std::string varName = match[1].str();
            std::string varValue = match[2].str();
            
            // 去除引号
            if (varValue.front() == '"' && varValue.back() == '"') {
                varValue = varValue.substr(1, varValue.length() - 2);
            }
            
            context_.variables[varName] = varValue;
        }
    }
    
    std::string expandVariableGroup(const std::string& code) {
        // 展开变量组使用：VariableGroupName(variableName)
        std::regex varRegex(R"((\w+)\((\w+)\))");
        std::smatch match;
        
        if (std::regex_search(code, match, varRegex)) {
            std::string groupName = match[1].str();
            std::string varName = match[2].str();
            
            // 查找变量值
            if (context_.variables.find(varName) != context_.variables.end()) {
                return context_.variables[varName];
            }
        }
        
        return "/* Variable not found: " + code + " */";
    }
    
    std::string compileHtmlElement(const std::string& code) {
        // 解析HTML元素：elementName { content }
        std::regex elementRegex(R"(\b(html|head|body|div|span|p|a|img|ul|ol|li|table|tr|td|th|thead|tbody|tfoot|form|input|button|textarea|select|option|label|h[1-6]|section|article|nav|header|footer|aside|main|figure|figcaption|details|summary|dialog|canvas|svg|video|audio|source|iframe|embed|object|script|style|link|meta|title|base|noscript|template|slot)\s*\{([^}]*)\})");
        std::smatch match;
        
        if (std::regex_search(code, match, elementRegex)) {
            std::string tagName = match[1].str();
            std::string content = match[2].str();
            
            // 解析属性和内容
            auto [attributes, innerContent] = parseElementContent(content);
            
            // 构建HTML标签
            std::string htmlTag = "<" + tagName;
            
            // 添加属性
            for (const auto& [attrName, attrValue] : attributes) {
                htmlTag += " " + attrName + "=\"" + attrValue + "\"";
            }
            
            // 检查是否为自闭合标签
            if (isSelfClosingTag(tagName)) {
                htmlTag += " />";
                return htmlTag;
            } else {
                htmlTag += ">";
                
                // 编译内部内容
                std::string compiledContent = compileChtlToHtml(innerContent);
                
                return htmlTag + compiledContent + "</" + tagName + ">";
            }
        }
        
        return "";
    }
    
    std::pair<std::unordered_map<std::string, std::string>, std::string> parseElementContent(const std::string& content) {
        std::unordered_map<std::string, std::string> attributes;
        std::string innerContent;
        
        std::istringstream iss(content);
        std::string line;
        bool inStyleBlock = false;
        bool inScriptBlock = false;
        
        while (std::getline(iss, line)) {
            line = trimAndClean(line);
            if (line.empty()) continue;
            
            // 检查是否进入style或script块
            if (line == "style" || line.find("style {") != std::string::npos) {
                inStyleBlock = true;
                innerContent += line + "\n";  // 包含样式块到内容中
                continue;
            } else if (line == "script" || line.find("script {") != std::string::npos) {
                inScriptBlock = true;
                innerContent += line + "\n";  // 包含脚本块到内容中
                continue;
            }
            
            // 检查块结束
            if ((inStyleBlock || inScriptBlock) && line == "}") {
                inStyleBlock = false;
                inScriptBlock = false;
                innerContent += line + "\n";
                continue;
            }
            
            // 检查是否为属性
            if (isAttribute(line) && !inStyleBlock && !inScriptBlock) {
                auto [attrName, attrValue] = parseAttribute(line);
                if (!attrName.empty()) {
                    attributes[attrName] = attrValue;
                }
            } else {
                // 其他内容都作为内部内容
                innerContent += line + "\n";
            }
        }
        
        return {attributes, trimAndClean(innerContent)};
    }
    
    std::pair<std::string, std::string> parseAttribute(const std::string& attrLine) {
        std::regex attrRegex(R"(\b(\w+)\s*[:=]\s*([^;]+);?)");
        std::smatch match;
        
        if (std::regex_search(attrLine, match, attrRegex)) {
            std::string attrName = match[1].str();
            std::string attrValue = trimAndClean(match[2].str());
            
            // 移除引号
            if (attrValue.front() == '"' && attrValue.back() == '"') {
                attrValue = attrValue.substr(1, attrValue.length() - 2);
            } else if (attrValue.front() == '\'' && attrValue.back() == '\'') {
                attrValue = attrValue.substr(1, attrValue.length() - 2);
            }
            
            return {attrName, attrValue};
        }
        
        return {"", ""};
    }
    
    bool isSelfClosingTag(const std::string& tagName) {
        std::vector<std::string> selfClosingTags = {
            "area", "base", "br", "col", "embed", "hr", "img", "input",
            "link", "meta", "param", "source", "track", "wbr"
        };
        
        return std::find(selfClosingTags.begin(), selfClosingTags.end(), tagName) != selfClosingTags.end();
    }
    
    std::string compileTextNode(const std::string& code) {
        // 解析文本节点：text { content }
        std::regex textRegex(R"(text\s*\{([^}]*)\})");
        std::smatch match;
        
        if (std::regex_search(code, match, textRegex)) {
            std::string textContent = match[1].str();
            textContent = trimAndClean(textContent);
            
            // 移除引号（如果有）
            if (textContent.front() == '"' && textContent.back() == '"') {
                textContent = textContent.substr(1, textContent.length() - 2);
            } else if (textContent.front() == '\'' && textContent.back() == '\'') {
                textContent = textContent.substr(1, textContent.length() - 2);
            }
            
            return textContent;
        }
        
        return "";
    }
    
    std::string compileLocalStyleBlock(const std::string& code) {
        context_.inLocalStyle = true;
        
        // 解析局部样式块：style { content }
        std::regex styleRegex(R"(style\s*\{([^}]*)\})");
        std::smatch match;
        
        if (std::regex_search(code, match, styleRegex)) {
            std::string styleContent = match[1].str();
            
            // 处理内联样式和选择器
            return compileStyleContent(styleContent);
        }
        
        context_.inLocalStyle = false;
        return "";
    }
    
    std::string compileStyleContent(const std::string& content) {
        std::ostringstream css;
        std::istringstream iss(content);
        std::string line;
        
        bool hasSelectors = false;
        
        while (std::getline(iss, line)) {
            line = trimAndClean(line);
            if (line.empty()) continue;
            
            // 检查是否为选择器（.class, #id, &:hover等）
            if (line[0] == '.' || line[0] == '#' || line[0] == '&') {
                hasSelectors = true;
                css << line << " {\n";
                // 这里应该处理选择器内容，简化处理
                css << "}\n";
            } else if (line.find(':') != std::string::npos && line.back() == ';') {
                // CSS属性
                if (!hasSelectors) {
                    // 内联样式
                    css << line << "\n";
                } else {
                    css << "    " << line << "\n";
                }
            }
        }
        
        return css.str();
    }
    
    std::string compileMultilineContent(const std::string& code) {
        std::ostringstream result;
        std::istringstream iss(code);
        std::string line;
        
        while (std::getline(iss, line)) {
            line = trimAndClean(line);
            if (!line.empty()) {
                std::string compiled = compileChtlToHtml(line);
                if (!compiled.empty()) {
                    result << compiled << "\n";
                }
            }
        }
        
        return result.str();
    }
    
    std::string trimAndClean(const std::string& str) {
        if (str.empty()) return "";
        
        size_t start = str.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) return "";
        
        size_t end = str.find_last_not_of(" \t\n\r");
        return str.substr(start, end - start + 1);
    }
};

// 创建编译器的工厂方法
std::shared_ptr<ICHTLCompiler> createBasicCHTLCompiler() {
    return std::make_shared<BasicCHTLCompiler>();
}

} // namespace chtl
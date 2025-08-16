#include "core/compiler_dispatcher.hpp"
#include <regex>
#include <sstream>
#include <iostream>

namespace chtl {

/**
 * 基础CHTL编译器实现
 * 处理基本的CHTL语法：HTML元素、属性、文本节点、局部样式等
 */
class BasicCHTLCompiler : public ICHTLCompiler {
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
        // 清理资源
    }
    
    CompilationResult compile(const CodeFragment& fragment) override {
        CompilationResult result;
        result.success = true;
        
        try {
            std::string htmlOutput = compileChtlToHtml(fragment.content);
            result.output = htmlOutput;
            
            if (htmlOutput.empty()) {
                result.success = false;
                result.errors.push_back("CHTL编译产生了空输出");
            }
            
        } catch (const std::exception& e) {
            result.success = false;
            result.errors.push_back("CHTL编译异常: " + std::string(e.what()));
        }
        
        return result;
    }
    
    // ICHTLCompiler特定方法
    bool supportsLocalStyle() const override {
        return true;  // 这个编译器支持局部样式编译
    }
    
    CompilationResult compileLocalStyle(const CodeFragment& fragment) override {
        CompilationResult result;
        result.success = true;
        
        try {
            // 处理局部样式块：style { ... }
            std::string cssOutput = compileLocalStyleBlock(fragment.content);
            result.output = cssOutput;
            
        } catch (const std::exception& e) {
            result.success = false;
            result.errors.push_back("局部样式编译异常: " + std::string(e.what()));
        }
        
        return result;
    }
    
private:
    std::string compileChtlToHtml(const std::string& chtlCode) {
        std::string result;
        
        // 检查是否是模板定义
        if (isTemplateDefinition(chtlCode)) {
            return compileTemplateDefinition(chtlCode);
        }
        
        // 检查是否是原始嵌入
        if (isOriginEmbedding(chtlCode)) {
            return compileOriginEmbedding(chtlCode);
        }
        
        // 检查是否是HTML元素
        if (isHtmlElement(chtlCode)) {
            return compileHtmlElement(chtlCode);
        }
        
        // 检查是否是文本节点
        if (isTextNode(chtlCode)) {
            return compileTextNode(chtlCode);
        }
        
        // 检查是否是属性
        if (isAttribute(chtlCode)) {
            return compileAttribute(chtlCode);
        }
        
        // 处理模板变量引用
        if (isTemplateVariable(chtlCode)) {
            return compileTemplateVariable(chtlCode);
        }
        
        // 默认处理：去除多余空白，返回内容
        return trimAndClean(chtlCode);
    }
    
    bool isTemplateDefinition(const std::string& code) {
        return code.find("[Template]") != std::string::npos ||
               code.find("[Custom]") != std::string::npos;
    }
    
    std::string compileTemplateDefinition(const std::string& code) {
        // 模板定义不直接输出HTML，而是注册到模板系统
        return "<!-- Template registered: " + extractTemplateName(code) + " -->";
    }
    
    std::string extractTemplateName(const std::string& code) {
        std::regex templateRegex(R"(\[(?:Template|Custom)\]\s*@(?:Style|Element|Var)\s+(\w+))");
        std::smatch match;
        if (std::regex_search(code, match, templateRegex)) {
            return match[1].str();
        }
        return "unnamed";
    }
    
    bool isOriginEmbedding(const std::string& code) {
        return code.find("[Origin]") != std::string::npos;
    }
    
    std::string compileOriginEmbedding(const std::string& code) {
        // 提取原始嵌入的内容
        std::regex originRegex(R"(\[Origin\]\s*@Html[^{]*\{([^}]*)\})");
        std::smatch match;
        if (std::regex_search(code, match, originRegex)) {
            return match[1].str();
        }
        return code; // 如果不匹配，返回原始内容
    }
    
    bool isHtmlElement(const std::string& code) {
        // 检查是否匹配HTML元素语法：elementName { ... }
        std::regex elementRegex(R"(\b(?:html|head|body|div|span|p|a|img|ul|li|table|tr|td|th|form|input|button|h[1-6]|section|article|nav|header|footer|aside|main)\s*\{)");
        return std::regex_search(code, elementRegex);
    }
    
    std::string compileHtmlElement(const std::string& code) {
        // 提取元素名称
        std::regex elementRegex(R"(\b(html|head|body|div|span|p|a|img|ul|li|table|tr|td|th|form|input|button|h[1-6]|section|article|nav|header|footer|aside|main)\s*\{([^}]*)\})");
        std::smatch match;
        
        if (std::regex_search(code, match, elementRegex)) {
            std::string tagName = match[1].str();
            std::string content = match[2].str();
            
            // 编译内容
            std::string compiledContent = compileChtlToHtml(content);
            
            return "<" + tagName + ">" + compiledContent + "</" + tagName + ">";
        }
        
        return code;
    }
    
    bool isTextNode(const std::string& code) {
        return code.find("text") != std::string::npos && 
               code.find("{") != std::string::npos;
    }
    
    std::string compileTextNode(const std::string& code) {
        // 提取文本内容
        std::regex textRegex(R"(text\s*\{([^}]*)\})");
        std::smatch match;
        
        if (std::regex_search(code, match, textRegex)) {
            std::string textContent = match[1].str();
            return trimAndClean(textContent);
        }
        
        return code;
    }
    
    bool isAttribute(const std::string& code) {
        // 检查属性语法：name: value; 或 name = value;
        std::regex attrRegex(R"(\b\w+\s*[:=]\s*[^;]+;)");
        return std::regex_search(code, attrRegex);
    }
    
    std::string compileAttribute(const std::string& code) {
        // 属性编译通常在元素编译时处理
        // 这里返回HTML属性格式
        std::regex attrRegex(R"(\b(\w+)\s*[:=]\s*([^;]+);)");
        std::smatch match;
        
        if (std::regex_search(code, match, attrRegex)) {
            std::string attrName = match[1].str();
            std::string attrValue = trimAndClean(match[2].str());
            
            // 移除引号（如果有）
            if (attrValue.front() == '"' && attrValue.back() == '"') {
                attrValue = attrValue.substr(1, attrValue.length() - 2);
            }
            
            return attrName + "=\"" + attrValue + "\"";
        }
        
        return code;
    }
    
    bool isTemplateVariable(const std::string& code) {
        return code.find("{{") != std::string::npos && 
               code.find("}}") != std::string::npos;
    }
    
    std::string compileTemplateVariable(const std::string& code) {
        // 简单的模板变量处理：保留变量引用格式
        // 在实际应用中，这里应该从变量上下文中查找值
        return code; // 暂时保持原样
    }
    
    std::string compileLocalStyleBlock(const std::string& code) {
        // 处理局部样式块
        std::regex styleRegex(R"(style\s*\{([^}]*)\})");
        std::smatch match;
        
        if (std::regex_search(code, match, styleRegex)) {
            std::string styleContent = match[1].str();
            
            // 生成CSS
            std::ostringstream css;
            css << "/* Generated from CHTL local style */\n";
            css << ".chtl-generated {\n";
            css << styleContent;
            css << "\n}\n";
            
            return css.str();
        }
        
        return "/* No style content found */";
    }
    
    std::string trimAndClean(const std::string& str) {
        // 移除前后空白
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
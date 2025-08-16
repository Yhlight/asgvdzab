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
        
        // 基于CHTL语法文档的严格实现
        
        // 1. 注释处理 (优先处理，避免注释内容被误解析)
        if (isComment(chtlCode)) {
            return compileComment(chtlCode);
        }
        
        // 2. 模板定义处理
        if (isTemplateDefinition(chtlCode)) {
            return compileTemplateDefinition(chtlCode);
        }
        
        // 3. 自定义定义处理
        if (isCustomDefinition(chtlCode)) {
            return compileCustomDefinition(chtlCode);
        }
        
        // 4. 原始嵌入处理
        if (isOriginEmbedding(chtlCode)) {
            return compileOriginEmbedding(chtlCode);
        }
        
        // 5. 配置组处理
        if (isConfigurationBlock(chtlCode)) {
            return compileConfigurationBlock(chtlCode);
        }
        
        // 6. 导入处理
        if (isImportStatement(chtlCode)) {
            return compileImportStatement(chtlCode);
        }
        
        // 7. 命名空间处理
        if (isNamespaceBlock(chtlCode)) {
            return compileNamespaceBlock(chtlCode);
        }
        
        // 8. 约束处理
        if (isConstraintStatement(chtlCode)) {
            return compileConstraintStatement(chtlCode);
        }
        
        // 9. 变量组使用处理
        if (isVariableGroupUsage(chtlCode)) {
            return compileVariableGroupUsage(chtlCode);
        }
        
        // 10. 模板/自定义使用处理
        if (isTemplateUsage(chtlCode)) {
            return compileTemplateUsage(chtlCode);
        }
        
        // 11. HTML元素处理
        if (isHtmlElement(chtlCode)) {
            return compileHtmlElement(chtlCode);
        }
        
        // 12. 文本节点处理
        if (isTextNode(chtlCode)) {
            return compileTextNode(chtlCode);
        }
        
        // 13. 属性处理
        if (isAttribute(chtlCode)) {
            return compileAttribute(chtlCode);
        }
        
        // 14. 默认处理：去除多余空白，返回内容
        return trimAndClean(chtlCode);
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
    
    bool isConfigurationBlock(const std::string& code) {
        return code.find("[Configuration]") != std::string::npos;
    }
    
    bool isImportStatement(const std::string& code) {
        return code.find("[Import]") != std::string::npos;
    }
    
    bool isNamespaceBlock(const std::string& code) {
        return code.find("[Namespace]") != std::string::npos;
    }
    
    bool isConstraintStatement(const std::string& code) {
        return code.find("except") != std::string::npos;
    }
    
    bool isVariableGroupUsage(const std::string& code) {
        // 检查变量组使用：ThemeColor(tableColor) 或 ThemeColor(tableColor = value)
        std::regex varGroupPattern(R"(\w+\([^)]*\))");
        return std::regex_search(code, varGroupPattern) && 
               !isHtmlElement(code) && !isTextNode(code);
    }
    
    bool isTemplateUsage(const std::string& code) {
        // 检查模板/自定义使用：@Style DefaultText; 或 @Element Box;
        std::regex usagePattern(R"(@(?:Style|Element|Var)\s+\w+)");
        return std::regex_search(code, usagePattern);
    }
    
    // ==================== 编译方法 ====================
    
    std::string compileComment(const std::string& code) {
        std::string trimmed = trimAndClean(code);
        if (trimmed.find("--") == 0) {
            // 生成器注释：会被生成器识别，保留在HTML中
            return "<!-- " + trimmed.substr(2) + " -->";
        } else {
            // //和/**/注释：不会被生成器识别，不输出到HTML
            return "";
        }
    }
    
    std::string compileTemplateDefinition(const std::string& code) {
        // 模板定义不直接输出HTML，而是注册到模板系统
        return "<!-- Template registered: " + extractTemplateName(code) + " -->";
    }
    
    std::string compileCustomDefinition(const std::string& code) {
        // 自定义定义不直接输出HTML，而是注册到自定义系统
        return "<!-- Custom registered: " + extractCustomName(code) + " -->";
    }
    
    std::string compileConfigurationBlock(const std::string& code) {
        // 配置组不输出HTML，用于配置编译器行为
        return "<!-- Configuration block processed -->";
    }
    
    std::string compileImportStatement(const std::string& code) {
        // 导入语句不直接输出HTML，而是加载资源
        return "<!-- Import processed: " + extractImportInfo(code) + " -->";
    }
    
    std::string compileNamespaceBlock(const std::string& code) {
        // 命名空间块需要编译其内容
        return compileNamespaceContent(code);
    }
    
    std::string compileConstraintStatement(const std::string& code) {
        // 约束语句不输出HTML，用于编译时约束检查
        return "<!-- Constraint applied: " + extractConstraintInfo(code) + " -->";
    }
    
    std::string compileVariableGroupUsage(const std::string& code) {
        // 变量组使用：替换为实际值
        return expandVariableGroup(code);
    }
    
    std::string compileTemplateUsage(const std::string& code) {
        // 模板/自定义使用：展开模板内容
        return expandTemplate(code);
    }
    
    // ==================== 辅助方法 ====================
    
    std::string extractTemplateName(const std::string& code) {
        std::regex templateRegex(R"(\[Template\]\s*@(?:Style|Element|Var)\s+(\w+))");
        std::smatch match;
        if (std::regex_search(code, match, templateRegex)) {
            return match[1].str();
        }
        return "unnamed";
    }
    
    std::string extractCustomName(const std::string& code) {
        std::regex customRegex(R"(\[Custom\]\s*@(?:Style|Element|Var)\s+(\w+))");
        std::smatch match;
        if (std::regex_search(code, match, customRegex)) {
            return match[1].str();
        }
        return "unnamed";
    }
    
    std::string extractImportInfo(const std::string& code) {
        // 简化的导入信息提取
        if (code.find("@Html") != std::string::npos) return "HTML file";
        if (code.find("@Style") != std::string::npos) return "CSS file";
        if (code.find("@JavaScript") != std::string::npos) return "JS file";
        if (code.find("@Chtl") != std::string::npos) return "CHTL file";
        if (code.find("@CJmod") != std::string::npos) return "CJmod file";
        return "unknown";
    }
    
    std::string extractConstraintInfo(const std::string& code) {
        // 简化的约束信息提取
        return "constraint rules";
    }
    
    std::string compileNamespaceContent(const std::string& code) {
        // 命名空间内容编译：递归编译内部内容
        std::regex nsContentRegex(R"(\[Namespace\]\s+\w+\s*\{([^}]*)\})");
        std::smatch match;
        if (std::regex_search(code, match, nsContentRegex)) {
            std::string content = match[1].str();
            return compileChtlToHtml(content);
        }
        return "";
    }
    
    std::string expandVariableGroup(const std::string& code) {
        // 变量组展开：这里做简化处理，实际应该查找变量定义
        std::regex varPattern(R"((\w+)\(([^)]*)\))");
        std::smatch match;
        if (std::regex_search(code, match, varPattern)) {
            std::string varName = match[1].str();
            std::string params = match[2].str();
            return "<!-- Variable group " + varName + " with params: " + params + " -->";
        }
        return code;
    }
    
    std::string expandTemplate(const std::string& code) {
        // 模板展开：这里做简化处理，实际应该查找模板定义
        std::regex templatePattern(R"(@(Style|Element|Var)\s+(\w+))");
        std::smatch match;
        if (std::regex_search(code, match, templatePattern)) {
            std::string type = match[1].str();
            std::string name = match[2].str();
            return "<!-- Template " + type + " " + name + " expanded -->";
        }
        return code;
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
#include "compilers/css_compiler.h"
#include <sstream>
#include <regex>
#include <algorithm>
#include <set>
#include <map>

namespace chtl {

// CSS编译器实现类
class CSSCompiler::Impl {
public:
    Impl() {
        initializeVendorPrefixes();
    }
    
    bool compile(const std::string& input, 
                std::string& output,
                std::vector<BaseCompiler::CompileError>& errors) {
        errors_ = &errors;
        reset();
        
        try {
            // 预处理CSS
            std::string processed = preprocessCSS(input);
            
            // 验证CSS
            if (!validateCSS(processed)) {
                addError("CSS语法错误", 0, 0);
                return false;
            }
            
            // 应用转换
            output = transformCSS(processed);
            
            // 压缩输出（如果需要）
            if (config_.compressOutput) {
                output = minifyCSS(output);
            }
            
            return errors.empty();
            
        } catch (const std::exception& e) {
            addError("CSS编译错误: " + std::string(e.what()), 0, 0);
            return false;
        }
    }
    
    void setOptions(const BaseCompiler::CompileOptions& options) {
        options_ = options;
    }
    
    bool validate(const std::string& input) const {
        // 基本验证：检查大括号匹配
        int braceCount = 0;
        bool inComment = false;
        
        for (size_t i = 0; i < input.length(); ++i) {
            if (!inComment && i + 1 < input.length() && 
                input[i] == '/' && input[i + 1] == '*') {
                inComment = true;
                i++;
                continue;
            }
            
            if (inComment && i + 1 < input.length() && 
                input[i] == '*' && input[i + 1] == '/') {
                inComment = false;
                i++;
                continue;
            }
            
            if (!inComment) {
                if (input[i] == '{') braceCount++;
                else if (input[i] == '}') braceCount--;
                
                if (braceCount < 0) return false;
            }
        }
        
        return braceCount == 0;
    }
    
    void setCSSConfig(const CSSConfig& config) {
        config_ = config;
    }
    
    CSSFeatures getUsedFeatures() const {
        return features_;
    }

private:
    // 初始化厂商前缀
    void initializeVendorPrefixes() {
        vendorPrefixes_ = {
            {"transform", {"-webkit-transform", "-moz-transform", "-ms-transform", "-o-transform"}},
            {"transition", {"-webkit-transition", "-moz-transition", "-o-transition"}},
            {"animation", {"-webkit-animation", "-moz-animation", "-o-animation"}},
            {"box-shadow", {"-webkit-box-shadow", "-moz-box-shadow"}},
            {"border-radius", {"-webkit-border-radius", "-moz-border-radius"}},
            {"flex", {"-webkit-flex", "-ms-flex"}},
            {"flexbox", {"-webkit-box", "-moz-box", "-ms-flexbox", "-webkit-flex"}},
            {"user-select", {"-webkit-user-select", "-moz-user-select", "-ms-user-select"}}
        };
    }
    
    // 重置状态
    void reset() {
        features_ = CSSFeatures();
        currentLine_ = 1;
        currentColumn_ = 1;
    }
    
    // 预处理CSS
    std::string preprocessCSS(const std::string& input) {
        std::string result = input;
        
        // 移除注释（如果需要）
        if (!config_.preserveComments) {
            result = removeComments(result);
        }
        
        // 处理CSS变量
        if (config_.enableCSSVariables) {
            result = processCSSVariables(result);
        }
        
        return result;
    }
    
    // 验证CSS
    bool validateCSS(const std::string& css) {
        // 简单的CSS验证
        // 在实际ANTLR版本中，这里会进行完整的语法分析
        
        // 检查基本结构
        std::regex ruleRegex(R"(([^{}]+)\s*\{([^{}]*)\})");
        std::smatch match;
        std::string remaining = css;
        
        while (std::regex_search(remaining, match, ruleRegex)) {
            std::string selector = match[1].str();
            std::string declarations = match[2].str();
            
            if (!validateSelector(trim(selector))) {
                addError("无效的CSS选择器: " + selector, 0, 0);
                return false;
            }
            
            if (!validateDeclarations(declarations)) {
                return false;
            }
            
            remaining = match.suffix();
        }
        
        // 检查是否有未匹配的内容
        remaining = trim(remaining);
        if (!remaining.empty()) {
            addError("CSS包含无效内容", 0, 0);
            return false;
        }
        
        return true;
    }
    
    // 验证选择器
    bool validateSelector(const std::string& selector) {
        // 简单验证，检查是否为空或包含无效字符
        if (selector.empty()) return false;
        
        // 检查常见的选择器模式
        std::regex selectorRegex(R"(^[#.\w\s\[\]:,>+~*=-]+$)");
        return std::regex_match(selector, selectorRegex);
    }
    
    // 验证声明
    bool validateDeclarations(const std::string& declarations) {
        // 分割声明
        std::istringstream stream(declarations);
        std::string declaration;
        
        while (std::getline(stream, declaration, ';')) {
            declaration = trim(declaration);
            if (declaration.empty()) continue;
            
            size_t colonPos = declaration.find(':');
            if (colonPos == std::string::npos) {
                addError("无效的CSS声明: " + declaration, 0, 0);
                return false;
            }
            
            std::string property = trim(declaration.substr(0, colonPos));
            std::string value = trim(declaration.substr(colonPos + 1));
            
            if (property.empty() || value.empty()) {
                addError("CSS属性或值为空", 0, 0);
                return false;
            }
            
            // 检测使用的特性
            detectFeatures(property, value);
        }
        
        return true;
    }
    
    // 转换CSS
    std::string transformCSS(const std::string& css) {
        std::string result = css;
        
        // 添加厂商前缀
        if (config_.enableVendorPrefixes) {
            result = addVendorPrefixes(result);
        }
        
        // 处理CSS级别兼容性
        result = processCSSLevel(result);
        
        return result;
    }
    
    // 添加厂商前缀
    std::string addVendorPrefixes(const std::string& css) {
        std::regex propertyRegex(R"((\s*)([\w-]+)\s*:\s*([^;]+);)");
        std::string result = css;
        std::smatch match;
        std::string processed;
        
        while (std::regex_search(result, match, propertyRegex)) {
            processed += match.prefix();
            
            std::string indent = match[1].str();
            std::string property = match[2].str();
            std::string value = match[3].str();
            
            // 检查是否需要添加前缀
            if (vendorPrefixes_.find(property) != vendorPrefixes_.end()) {
                // 添加带前缀的版本
                for (const auto& prefix : vendorPrefixes_[property]) {
                    processed += indent + prefix + ": " + value + ";\n";
                }
            }
            
            // 添加原始属性
            processed += match[0].str();
            
            result = match.suffix();
        }
        
        processed += result;
        return processed;
    }
    
    // 处理CSS级别
    std::string processCSSLevel(const std::string& css) {
        // 根据配置的CSS级别进行转换
        // 这里是简化版本，实际应该更复杂
        return css;
    }
    
    // 压缩CSS
    std::string minifyCSS(const std::string& css) {
        std::string minified;
        bool inString = false;
        char stringChar = 0;
        bool lastWasSpace = false;
        
        for (size_t i = 0; i < css.length(); ++i) {
            char c = css[i];
            
            // 处理字符串
            if (!inString && (c == '"' || c == '\'')) {
                inString = true;
                stringChar = c;
                minified += c;
                continue;
            }
            
            if (inString && c == stringChar && (i == 0 || css[i-1] != '\\')) {
                inString = false;
                minified += c;
                continue;
            }
            
            if (inString) {
                minified += c;
                continue;
            }
            
            // 移除多余的空白
            if (std::isspace(c)) {
                if (!lastWasSpace && !minified.empty() && 
                    minified.back() != '{' && minified.back() != '}' &&
                    minified.back() != ';' && minified.back() != ':') {
                    minified += ' ';
                    lastWasSpace = true;
                }
            } else {
                minified += c;
                lastWasSpace = false;
            }
        }
        
        return minified;
    }
    
    // 移除注释
    std::string removeComments(const std::string& css) {
        std::string result;
        bool inComment = false;
        
        for (size_t i = 0; i < css.length(); ++i) {
            if (!inComment && i + 1 < css.length() && 
                css[i] == '/' && css[i + 1] == '*') {
                inComment = true;
                i++; // 跳过'*'
                continue;
            }
            
            if (inComment && i + 1 < css.length() && 
                css[i] == '*' && css[i + 1] == '/') {
                inComment = false;
                i++; // 跳过'/'
                continue;
            }
            
            if (!inComment) {
                result += css[i];
            }
        }
        
        return result;
    }
    
    // 处理CSS变量
    std::string processCSSVariables(const std::string& css) {
        features_.usesVariables = css.find("var(") != std::string::npos ||
                                 css.find("--") != std::string::npos;
        // 简化实现，实际应该解析和验证CSS变量
        return css;
    }
    
    // 检测使用的特性
    void detectFeatures(const std::string& property, const std::string& value) {
        if (property == "display" && value.find("flex") != std::string::npos) {
            features_.usesFlexbox = true;
        } else if (property == "display" && value.find("grid") != std::string::npos) {
            features_.usesGrid = true;
        } else if (property.find("transform") != std::string::npos) {
            features_.usesTransforms = true;
        } else if (property.find("animation") != std::string::npos ||
                   property.find("transition") != std::string::npos) {
            features_.usesAnimations = true;
        } else if (value.find("calc(") != std::string::npos) {
            features_.usesCalc = true;
        }
        
        // 检查未知属性
        static const std::set<std::string> knownProperties = {
            "display", "position", "width", "height", "margin", "padding",
            "color", "background", "background-color", "border", "font",
            "font-size", "font-family", "text-align", "opacity", "z-index",
            "transform", "transition", "animation", "flex", "grid"
            // ... 更多已知属性
        };
        
        if (knownProperties.find(property) == knownProperties.end()) {
            features_.unknownProperties.push_back(property);
        }
    }
    
    // 工具函数
    std::string trim(const std::string& str) const {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, last - first + 1);
    }
    
    void addError(const std::string& message, size_t line, size_t column) {
        BaseCompiler::CompileError error;
        error.severity = BaseCompiler::CompileError::ERROR;
        error.message = message;
        error.line = line;
        error.column = column;
        if (errors_) {
            errors_->push_back(error);
        }
    }

private:
    BaseCompiler::CompileOptions options_;
    CSSConfig config_;
    
    std::vector<BaseCompiler::CompileError>* errors_;
    CSSFeatures features_;
    
    std::map<std::string, std::vector<std::string>> vendorPrefixes_;
    
    size_t currentLine_;
    size_t currentColumn_;
};

// CSSCompiler 实现
CSSCompiler::CSSCompiler()
    : pImpl(std::make_unique<Impl>()) {}

CSSCompiler::~CSSCompiler() = default;

bool CSSCompiler::compile(const std::string& input, 
                         std::string& output,
                         std::vector<CompileError>& errors) {
    return pImpl->compile(input, output, errors);
}

void CSSCompiler::setOptions(const CompileOptions& options) {
    pImpl->setOptions(options);
}

bool CSSCompiler::validate(const std::string& input) const {
    return pImpl->validate(input);
}

void CSSCompiler::setCSSConfig(const CSSConfig& config) {
    pImpl->setCSSConfig(config);
}

CSSCompiler::CSSFeatures CSSCompiler::getUsedFeatures() const {
    return pImpl->getUsedFeatures();
}

} // namespace chtl
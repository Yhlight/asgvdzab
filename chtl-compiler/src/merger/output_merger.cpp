#include "merger/output_merger.h"
#include <sstream>
#include <algorithm>
#include <regex>
#include <iomanip>
#include <set>
#include <cctype>

namespace chtl {

// OutputMerger实现类
class OutputMerger::Impl {
public:
    explicit Impl(const MergerConfig& config) 
        : config_(config) {
        initializeDefaults();
    }
    
    std::string merge(const std::vector<CompileResult>& results) {
        reset();
        
        // 分类编译结果
        categorizeResults(results);
        
        // 构建HTML文档
        std::stringstream html;
        
        // 生成DOCTYPE和HTML开始标签
        html << documentInfo_.doctype << "\n";
        html << "<html lang=\"" << documentInfo_.language << "\">\n";
        
        // 生成HEAD部分
        generateHead(html);
        
        // 生成BODY部分
        generateBody(html);
        
        // HTML结束标签
        html << "</html>";
        
        // 后处理
        std::string output = html.str();
        if (config_.formatOutput) {
            output = formatHTML(output);
        }
        if (config_.minifyHTML) {
            output = minifyHTML(output);
        }
        
        // 更新统计信息
        updateStats(results, output);
        
        return output;
    }
    
    void setDocumentInfo(const HTMLDocument& doc) {
        documentInfo_ = doc;
    }
    
    void addStylesheet(const std::string& href) {
        externalStylesheets_.push_back(href);
    }
    
    void addScript(const std::string& src, bool defer, bool async) {
        ExternalScript script;
        script.src = src;
        script.defer = defer;
        script.async = async;
        externalScripts_.push_back(script);
    }
    
    void addMetaTag(const std::string& name, const std::string& content) {
        MetaTag meta;
        meta.name = name;
        meta.content = content;
        additionalMetaTags_.push_back(meta);
    }
    
    MergeStats getStats() const {
        return stats_;
    }
    
    bool validateOutput(const std::string& html, std::vector<std::string>& errors) const {
        errors.clear();
        
        // 基本HTML验证
        if (!validateHTMLStructure(html, errors)) {
            return false;
        }
        
        // 检查必要的标签
        if (!validateRequiredTags(html, errors)) {
            return false;
        }
        
        // 检查标签配对
        if (!validateTagPairing(html, errors)) {
            return false;
        }
        
        return errors.empty();
    }

private:
    // 初始化默认值
    void initializeDefaults() {
        // 默认meta标签
        defaultMetaTags_ = {
            {"viewport", "width=device-width, initial-scale=1.0"},
            {"X-UA-Compatible", "IE=edge"}
        };
    }
    
    // 重置状态
    void reset() {
        compiledHTML_.clear();
        compiledStyles_.clear();
        compiledScripts_.clear();
        stats_ = MergeStats();
    }
    
    // 分类编译结果
    void categorizeResults(const std::vector<CompileResult>& results) {
        for (const auto& result : results) {
            if (!result.success) {
                continue; // 跳过失败的结果
            }
            
            switch (result.fragmentType) {
                case CodeFragmentType::CHTL:
                    // CHTL编译结果可能包含HTML和内联样式
                    processHTMLResult(result.output);
                    break;
                    
                case CodeFragmentType::CSS:
                    compiledStyles_.push_back(result.output);
                    break;
                    
                case CodeFragmentType::CHTL_JS:
                case CodeFragmentType::JAVASCRIPT:
                    compiledScripts_.push_back(result.output);
                    break;
                    
                case CodeFragmentType::HTML_ORIGIN:
                    compiledHTML_.push_back(result.output);
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    // 处理HTML结果
    void processHTMLResult(const std::string& html) {
        // 提取内联的style标签
        std::regex styleRegex(R"(<style[^>]*>([\s\S]*?)</style>)");
        std::smatch match;
        std::string remaining = html;
        std::string htmlOnly;
        
        size_t lastPos = 0;
        while (std::regex_search(remaining, match, styleRegex)) {
            // 添加style标签之前的HTML
            htmlOnly += remaining.substr(0, match.position());
            
            // 提取style内容
            std::string styleContent = match[1].str();
            compiledStyles_.push_back(styleContent);
            
            remaining = match.suffix();
            lastPos = match.position() + match.length();
        }
        
        // 添加剩余的HTML
        htmlOnly += remaining;
        
        if (!htmlOnly.empty()) {
            compiledHTML_.push_back(htmlOnly);
        }
    }
    
    // 生成HEAD部分
    void generateHead(std::stringstream& html) {
        html << "<head>\n";
        
        // 字符集
        html << "  <meta charset=\"" << documentInfo_.charset << "\">\n";
        
        // 标题
        if (!documentInfo_.head.title.empty()) {
            html << "  <title>" << escapeHTML(documentInfo_.head.title) << "</title>\n";
        }
        
        // 默认meta标签
        for (const auto& meta : defaultMetaTags_) {
            html << "  <meta name=\"" << meta.name 
                 << "\" content=\"" << escapeHTML(meta.content) << "\">\n";
        }
        
        // 文档中的meta标签
        for (const auto& metaTag : documentInfo_.head.metaTags) {
            html << "  " << metaTag << "\n";
        }
        
        // 额外的meta标签
        for (const auto& meta : additionalMetaTags_) {
            html << "  <meta name=\"" << meta.name 
                 << "\" content=\"" << escapeHTML(meta.content) << "\">\n";
        }
        
        // 外部样式表
        for (const auto& href : externalStylesheets_) {
            html << "  <link rel=\"stylesheet\" href=\"" << escapeHTML(href) << "\">\n";
        }
        
        // 文档中的link标签
        for (const auto& linkTag : documentInfo_.head.linkTags) {
            html << "  " << linkTag << "\n";
        }
        
        // 合并的样式
        if (!compiledStyles_.empty() && config_.inlineStyles) {
            html << "  <style>\n";
            if (config_.combineStyles) {
                // 合并所有样式
                for (const auto& style : compiledStyles_) {
                    html << style << "\n";
                }
            } else {
                // 分别输出每个样式
                for (size_t i = 0; i < compiledStyles_.size(); ++i) {
                    if (i > 0) html << "\n";
                    html << compiledStyles_[i];
                }
            }
            html << "  </style>\n";
        }
        
        // 文档中的style标签
        for (const auto& styleTag : documentInfo_.head.styleTags) {
            html << "  <style>\n" << styleTag << "\n  </style>\n";
        }
        
        // 头部脚本
        for (const auto& scriptTag : documentInfo_.head.scriptTags) {
            html << "  <script>\n" << scriptTag << "\n  </script>\n";
        }
        
        html << "</head>\n";
    }
    
    // 生成BODY部分
    void generateBody(std::stringstream& html) {
        html << "<body>\n";
        
        // 主体HTML内容
        if (!documentInfo_.body.content.empty()) {
            html << documentInfo_.body.content << "\n";
        }
        
        // 编译的HTML内容
        for (const auto& htmlContent : compiledHTML_) {
            html << htmlContent << "\n";
        }
        
        // 外部脚本
        for (const auto& script : externalScripts_) {
            html << "  <script src=\"" << escapeHTML(script.src) << "\"";
            if (script.defer) html << " defer";
            if (script.async) html << " async";
            html << "></script>\n";
        }
        
        // 合并的脚本
        if (!compiledScripts_.empty() && config_.inlineScripts) {
            if (config_.combineScripts) {
                // 合并所有脚本
                html << "  <script>\n";
                for (const auto& script : compiledScripts_) {
                    html << script << "\n";
                }
                html << "  </script>\n";
            } else {
                // 分别输出每个脚本
                for (const auto& script : compiledScripts_) {
                    html << "  <script>\n" << script << "\n  </script>\n";
                }
            }
        }
        
        // 文档中的底部脚本
        for (const auto& scriptTag : documentInfo_.body.scriptTags) {
            html << "  <script>\n" << scriptTag << "\n  </script>\n";
        }
        
        html << "</body>\n";
    }
    
    // 格式化HTML
    std::string formatHTML(const std::string& html) {
        std::stringstream formatted;
        std::istringstream stream(html);
        std::string line;
        int indentLevel = 0;
        
        while (std::getline(stream, line)) {
            // 移除前后空白
            line = trim(line);
            if (line.empty() && config_.removeEmptyLines) {
                continue;
            }
            
            // 检查是否是结束标签
            bool isClosingTag = (line.find("</") == 0);
            bool isSelfClosing = (line.find("/>") != std::string::npos) ||
                                 (line.find("<meta") == 0) ||
                                 (line.find("<link") == 0) ||
                                 (line.find("<br") == 0) ||
                                 (line.find("<hr") == 0) ||
                                 (line.find("<img") == 0) ||
                                 (line.find("<input") == 0);
            
            // 调整缩进
            if (isClosingTag) {
                indentLevel = std::max(0, indentLevel - 1);
            }
            
            // 添加缩进
            formatted << getIndent(indentLevel) << line << "\n";
            
            // 更新缩进级别
            if (!isClosingTag && !isSelfClosing && line.find("<") == 0 && line.find(">") != std::string::npos) {
                indentLevel++;
            }
        }
        
        return formatted.str();
    }
    
    // 压缩HTML
    std::string minifyHTML(const std::string& html) {
        std::string minified;
        bool inTag = false;
        bool inString = false;
        bool inComment = false;
        bool inScript = false;
        bool inStyle = false;
        bool lastWasSpace = false;
        char stringChar = 0;
        
        for (size_t i = 0; i < html.length(); ++i) {
            char c = html[i];
            
            // 处理注释
            if (!inString && !inComment && i + 3 < html.length() &&
                html.substr(i, 4) == "<!--") {
                if (!config_.removeComments) {
                    minified += "<!--";
                }
                inComment = true;
                i += 3;
                continue;
            }
            
            if (inComment && i + 2 < html.length() &&
                html.substr(i, 3) == "-->") {
                if (!config_.removeComments) {
                    minified += "-->";
                }
                inComment = false;
                i += 2;
                continue;
            }
            
            if (inComment) {
                if (!config_.removeComments) {
                    minified += c;
                }
                continue;
            }
            
            // 检测script和style标签
            if (!inString && i + 7 < html.length() &&
                html.substr(i, 7) == "<script") {
                inScript = true;
            } else if (!inString && i + 6 < html.length() &&
                       html.substr(i, 6) == "<style") {
                inStyle = true;
            } else if (!inString && i + 8 < html.length() &&
                       html.substr(i, 9) == "</script>") {
                inScript = false;
            } else if (!inString && i + 7 < html.length() &&
                       html.substr(i, 8) == "</style>") {
                inStyle = false;
            }
            
            // 在script和style中保留内容
            if (inScript || inStyle) {
                minified += c;
                continue;
            }
            
            // 处理标签
            if (!inString && c == '<') {
                inTag = true;
                minified += c;
                lastWasSpace = false;
                continue;
            }
            
            if (!inString && c == '>') {
                inTag = false;
                minified += c;
                lastWasSpace = false;
                continue;
            }
            
            // 处理字符串
            if (inTag && !inString && (c == '"' || c == '\'')) {
                inString = true;
                stringChar = c;
                minified += c;
                continue;
            }
            
            if (inString && c == stringChar && (i == 0 || html[i-1] != '\\')) {
                inString = false;
                minified += c;
                continue;
            }
            
            if (inString) {
                minified += c;
                continue;
            }
            
            // 处理空白
            if (!inTag && std::isspace(c)) {
                if (!lastWasSpace && !minified.empty() && 
                    minified.back() != '>' && config_.optimizeWhitespace) {
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
    
    // 验证HTML结构
    bool validateHTMLStructure(const std::string& html, std::vector<std::string>& errors) const {
        // 检查基本结构
        if (html.find("<html") == std::string::npos) {
            errors.push_back("缺少<html>标签");
            return false;
        }
        
        if (html.find("<head") == std::string::npos) {
            errors.push_back("缺少<head>标签");
            return false;
        }
        
        if (html.find("<body") == std::string::npos) {
            errors.push_back("缺少<body>标签");
            return false;
        }
        
        return true;
    }
    
    // 验证必要的标签
    bool validateRequiredTags(const std::string& html, std::vector<std::string>& errors) const {
        // 检查charset
        if (html.find("charset=") == std::string::npos) {
            errors.push_back("缺少字符集声明");
        }
        
        // 检查title
        if (html.find("<title>") == std::string::npos) {
            errors.push_back("缺少<title>标签");
        }
        
        return true;
    }
    
    // 验证标签配对
    bool validateTagPairing(const std::string& html, std::vector<std::string>& errors) const {
        // 简化的标签配对检查
        std::vector<std::string> tagStack;
        std::regex tagRegex(R"(<(/?)(\w+)[^>]*>)");
        std::smatch match;
        std::string remaining = html;
        
        while (std::regex_search(remaining, match, tagRegex)) {
            std::string isClosing = match[1].str();
            std::string tagName = match[2].str();
            
            // 转换为小写
            std::transform(tagName.begin(), tagName.end(), tagName.begin(), ::tolower);
            
            // 自闭合标签
            static const std::set<std::string> selfClosingTags = {
                "meta", "link", "br", "hr", "img", "input", "area", "base", "col", "embed", "source", "track", "wbr"
            };
            
            if (selfClosingTags.find(tagName) != selfClosingTags.end()) {
                remaining = match.suffix();
                continue;
            }
            
            if (isClosing.empty()) {
                // 开始标签
                tagStack.push_back(tagName);
            } else {
                // 结束标签
                if (tagStack.empty() || tagStack.back() != tagName) {
                    errors.push_back("标签不匹配: </" + tagName + ">");
                } else {
                    tagStack.pop_back();
                }
            }
            
            remaining = match.suffix();
        }
        
        // 检查未闭合的标签
        for (const auto& tag : tagStack) {
            errors.push_back("未闭合的标签: <" + tag + ">");
        }
        
        return tagStack.empty();
    }
    
    // 更新统计信息
    void updateStats(const std::vector<CompileResult>& results, const std::string& output) {
        stats_.totalFragments = results.size();
        stats_.mergedStyles = compiledStyles_.size();
        stats_.mergedScripts = compiledScripts_.size();
        stats_.htmlFragments = compiledHTML_.size();
        stats_.outputSize = output.size();
    }
    
    // 工具函数
    std::string trim(const std::string& str) const {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, last - first + 1);
    }
    
    std::string getIndent(int level) const {
        if (!config_.useSpaces) {
            return std::string(level, '\t');
        }
        return std::string(level * config_.indentSize, ' ');
    }
    
    std::string escapeHTML(const std::string& str) const {
        std::string escaped;
        for (char c : str) {
            switch (c) {
                case '&': escaped += "&amp;"; break;
                case '<': escaped += "&lt;"; break;
                case '>': escaped += "&gt;"; break;
                case '"': escaped += "&quot;"; break;
                case '\'': escaped += "&#39;"; break;
                default: escaped += c; break;
            }
        }
        return escaped;
    }

private:
    MergerConfig config_;
    HTMLDocument documentInfo_;
    MergeStats stats_;
    
    // 编译结果分类
    std::vector<std::string> compiledHTML_;
    std::vector<std::string> compiledStyles_;
    std::vector<std::string> compiledScripts_;
    
    // 额外资源
    std::vector<std::string> externalStylesheets_;
    
    struct ExternalScript {
        std::string src;
        bool defer = false;
        bool async = false;
    };
    std::vector<ExternalScript> externalScripts_;
    
    struct MetaTag {
        std::string name;
        std::string content;
    };
    std::vector<MetaTag> defaultMetaTags_;
    std::vector<MetaTag> additionalMetaTags_;
};

// OutputMerger 实现
OutputMerger::OutputMerger(const MergerConfig& config)
    : pImpl(std::make_unique<Impl>(config)) {}

OutputMerger::~OutputMerger() = default;

std::string OutputMerger::merge(const std::vector<CompileResult>& results) {
    return pImpl->merge(results);
}

void OutputMerger::setDocumentInfo(const HTMLDocument& doc) {
    pImpl->setDocumentInfo(doc);
}

void OutputMerger::addStylesheet(const std::string& href) {
    pImpl->addStylesheet(href);
}

void OutputMerger::addScript(const std::string& src, bool defer, bool async) {
    pImpl->addScript(src, defer, async);
}

void OutputMerger::addMetaTag(const std::string& name, const std::string& content) {
    pImpl->addMetaTag(name, content);
}

OutputMerger::MergeStats OutputMerger::getStats() const {
    return pImpl->getStats();
}

bool OutputMerger::validateOutput(const std::string& html, std::vector<std::string>& errors) const {
    return pImpl->validateOutput(html, errors);
}

} // namespace chtl
#include "HTMLOutput.h"
#include <sstream>
#include <algorithm>

namespace CHTL {

HTMLOutput::HTMLOutput() : documentTitle_("CHTL Generated Page"), documentLang_("zh-CN") {
    // 添加默认的meta标签
    addMeta("charset", "UTF-8");
    addMeta("viewport", "width=device-width, initial-scale=1.0");
}

void HTMLOutput::setDocumentStructure(const std::string& title, const std::string& lang) {
    documentTitle_ = title;
    documentLang_ = lang;
}

void HTMLOutput::addCSS(const std::string& css, bool isInline) {
    if (isInline) {
        headCSS_.push_back(css);
    } else {
        // 外部CSS文件引用
        headCSS_.push_back("<link rel=\"stylesheet\" href=\"" + css + "\">");
    }
}

void HTMLOutput::addJavaScript(const std::string& js, bool isInline, const std::string& location) {
    std::string scriptTag;
    
    if (isInline) {
        scriptTag = "<script>\n" + indentText(js, 1) + "\n</script>";
    } else {
        // 外部JS文件引用
        scriptTag = "<script src=\"" + js + "\"></script>";
    }
    
    if (location == "head") {
        headJS_.push_back(scriptTag);
    } else {
        bodyJS_.push_back(scriptTag);
    }
}

void HTMLOutput::addHTMLContent(const std::string& html) {
    bodyContent_.push_back(html);
}

void HTMLOutput::addMeta(const std::string& name, const std::string& content) {
    if (name == "charset") {
        metaTags_.push_back("<meta charset=\"" + content + "\">");
    } else {
        metaTags_.push_back("<meta name=\"" + name + "\" content=\"" + content + "\">");
    }
}

std::string HTMLOutput::generateHTML() {
    std::ostringstream html;
    
    // DOCTYPE
    html << "<!DOCTYPE html>\n";
    
    // HTML标签开始
    html << "<html lang=\"" << documentLang_ << "\">\n";
    
    // HEAD部分
    html << "<head>\n";
    
    // Meta标签
    for (const auto& meta : metaTags_) {
        html << "    " << meta << "\n";
    }
    
    // 标题
    html << "    <title>" << escapeHTML(documentTitle_) << "</title>\n";
    
    // CSS样式
    if (!headCSS_.empty()) {
        html << "    <style>\n";
        for (const auto& css : headCSS_) {
            html << indentText(css, 2) << "\n";
        }
        html << "    </style>\n";
    }
    
    // Head中的JavaScript
    for (const auto& js : headJS_) {
        html << "    " << js << "\n";
    }
    
    html << "</head>\n";
    
    // BODY部分
    html << "<body>\n";
    
    // 主要内容
    for (const auto& content : bodyContent_) {
        html << indentText(content, 1) << "\n";
    }
    
    // Body中的JavaScript
    for (const auto& js : bodyJS_) {
        html << "    " << js << "\n";
    }
    
    html << "</body>\n";
    html << "</html>";
    
    return html.str();
}

void HTMLOutput::clear() {
    documentTitle_ = "CHTL Generated Page";
    documentLang_ = "zh-CN";
    metaTags_.clear();
    headCSS_.clear();
    headJS_.clear();
    bodyContent_.clear();
    bodyJS_.clear();
    
    // 重新添加默认meta标签
    addMeta("charset", "UTF-8");
    addMeta("viewport", "width=device-width, initial-scale=1.0");
}

std::string HTMLOutput::escapeHTML(const std::string& text) {
    std::string escaped = text;
    
    // 替换HTML特殊字符
    std::string replacements[][2] = {
        {"&", "&amp;"},
        {"<", "&lt;"},
        {">", "&gt;"},
        {"\"", "&quot;"},
        {"'", "&#39;"}
    };
    
    for (const auto& replacement : replacements) {
        size_t pos = 0;
        while ((pos = escaped.find(replacement[0], pos)) != std::string::npos) {
            escaped.replace(pos, replacement[0].length(), replacement[1]);
            pos += replacement[1].length();
        }
    }
    
    return escaped;
}

std::string HTMLOutput::indentText(const std::string& text, int indentLevel) {
    if (text.empty()) return text;
    
    std::string indent(indentLevel * 4, ' '); // 每级缩进4个空格
    std::string result;
    std::istringstream stream(text);
    std::string line;
    
    bool firstLine = true;
    while (std::getline(stream, line)) {
        if (!firstLine) {
            result += "\n";
        }
        result += indent + line;
        firstLine = false;
    }
    
    return result;
}

} // namespace CHTL
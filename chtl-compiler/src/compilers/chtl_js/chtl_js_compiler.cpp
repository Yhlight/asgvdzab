#include "compilers/chtl_js_compiler.h"
#include <sstream>
#include <algorithm>
#include <iostream>

namespace chtl {

CHTLJSCompiler::CHTLJSCompiler() 
    : debug_mode_(false),
      unique_id_counter_(0),
      selector_regex_(R"(\{\{([^}]+)\}\})"),
      arrow_regex_(R"(->)"),
      listen_regex_(R"(->listen\s*\()"),
      delegate_regex_(R"(->delegate\s*\()"),
      animate_regex_(R"(animate\s*\()") {
}

CHTLJSCompiler::~CHTLJSCompiler() {
}

CompileResult CHTLJSCompiler::compile(const CodeFragment& fragment,
                                     const CompileOptions& options) {
    CompileResult result;
    
    if (!validate(fragment)) {
        result.success = false;
        result.error_msg = "Invalid fragment type for CHTL JS compiler";
        return result;
    }
    
    try {
        // 重置状态
        current_output_.clear();
        delegate_registry_.clear();
        unique_id_counter_ = 0;
        
        std::string source = fragment.content;
        
        // 解析特殊语法
        auto elements = parseSpecialSyntax(source);
        
        if (debug_mode_) {
            std::cout << "Found " << elements.size() << " CHTL JS elements\n";
        }
        
        // 从后往前处理，避免位置偏移
        std::sort(elements.begin(), elements.end(),
                 [](const CHTLJSElement& a, const CHTLJSElement& b) {
                     return a.start_pos > b.start_pos;
                 });
        
        // 处理每个元素
        for (const auto& element : elements) {
            std::string replacement;
            
            switch (element.type) {
                case CHTLJSElement::SELECTOR:
                    replacement = transformSelector(element.content);
                    break;
                case CHTLJSElement::ARROW_ACCESS:
                    replacement = ".";
                    break;
                case CHTLJSElement::LISTEN:
                    replacement = transformListen(
                        source.substr(element.start_pos, 
                                    element.end_pos - element.start_pos));
                    break;
                case CHTLJSElement::DELEGATE:
                    replacement = transformDelegate(
                        source.substr(element.start_pos, 
                                    element.end_pos - element.start_pos));
                    break;
                case CHTLJSElement::ANIMATE:
                    replacement = transformAnimate(
                        source.substr(element.start_pos, 
                                    element.end_pos - element.start_pos));
                    break;
            }
            
            source.replace(element.start_pos, 
                          element.end_pos - element.start_pos, 
                          replacement);
        }
        
        // 生成最终输出
        current_output_ = source;
        
        // 如果有事件委托，生成注册代码
        if (!delegate_registry_.empty()) {
            current_output_ = generateDelegateRegistry() + "\n\n" + current_output_;
        }
        
        result.output = current_output_;
        result.success = true;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.error_msg = e.what();
    }
    
    return result;
}

std::vector<CHTLJSElement> CHTLJSCompiler::parseSpecialSyntax(const std::string& source) {
    std::vector<CHTLJSElement> elements;
    
    // 查找选择器 {{}}
    std::sregex_iterator selector_it(source.begin(), source.end(), selector_regex_);
    std::sregex_iterator selector_end;
    
    for (; selector_it != selector_end; ++selector_it) {
        CHTLJSElement element;
        element.type = CHTLJSElement::SELECTOR;
        element.content = (*selector_it)[1].str();
        element.start_pos = selector_it->position();
        element.end_pos = element.start_pos + selector_it->length();
        elements.push_back(element);
    }
    
    // 查找箭头操作符 ->
    size_t pos = 0;
    while ((pos = source.find("->", pos)) != std::string::npos) {
        // 检查是否是特殊方法
        bool is_special = false;
        
        if (source.substr(pos, 8) == "->listen") {
            CHTLJSElement element;
            element.type = CHTLJSElement::LISTEN;
            element.start_pos = pos;
            
            // 找到完整的listen调用
            size_t paren_count = 0;
            size_t i = pos + 8;
            while (i < source.length() && source[i] != '(') i++;
            if (i < source.length()) {
                i++; // 跳过(
                paren_count = 1;
                while (i < source.length() && paren_count > 0) {
                    if (source[i] == '(') paren_count++;
                    else if (source[i] == ')') paren_count--;
                    i++;
                }
            }
            element.end_pos = i;
            elements.push_back(element);
            is_special = true;
        }
        else if (source.substr(pos, 10) == "->delegate") {
            CHTLJSElement element;
            element.type = CHTLJSElement::DELEGATE;
            element.start_pos = pos;
            
            // 找到完整的delegate调用
            size_t paren_count = 0;
            size_t i = pos + 10;
            while (i < source.length() && source[i] != '(') i++;
            if (i < source.length()) {
                i++; // 跳过(
                paren_count = 1;
                while (i < source.length() && paren_count > 0) {
                    if (source[i] == '(') paren_count++;
                    else if (source[i] == ')') paren_count--;
                    i++;
                }
            }
            element.end_pos = i;
            elements.push_back(element);
            is_special = true;
        }
        
        if (!is_special) {
            // 普通箭头操作符
            CHTLJSElement element;
            element.type = CHTLJSElement::ARROW_ACCESS;
            element.start_pos = pos;
            element.end_pos = pos + 2;
            element.content = "->";
            elements.push_back(element);
        }
        
        pos += 2;
    }
    
    // 查找animate函数
    std::sregex_iterator animate_it(source.begin(), source.end(), animate_regex_);
    std::sregex_iterator animate_end;
    
    for (; animate_it != animate_end; ++animate_it) {
        CHTLJSElement element;
        element.type = CHTLJSElement::ANIMATE;
        element.start_pos = animate_it->position();
        
        // 找到完整的animate调用
        size_t paren_count = 0;
        size_t i = element.start_pos + animate_it->length();
        if (i < source.length()) {
            paren_count = 1;
            while (i < source.length() && paren_count > 0) {
                if (source[i] == '(') paren_count++;
                else if (source[i] == ')') paren_count--;
                i++;
            }
        }
        element.end_pos = i;
        elements.push_back(element);
    }
    
    return elements;
}

std::string CHTLJSCompiler::transformSelector(const std::string& selector) {
    std::string trimmed = trim(selector);
    
    // 处理索引访问，如 button[0]
    size_t bracket_pos = trimmed.find('[');
    std::string base_selector = trimmed;
    std::string index_part;
    
    if (bracket_pos != std::string::npos) {
        base_selector = trimmed.substr(0, bracket_pos);
        index_part = trimmed.substr(bracket_pos);
    }
    
    std::string query = generateQuerySelector(base_selector);
    
    // 如果有索引，只返回单个元素
    if (!index_part.empty()) {
        return query + index_part;
    }
    
    return query;
}

std::string CHTLJSCompiler::generateQuerySelector(const std::string& selector) {
    std::string trimmed = trim(selector);
    
    // 类选择器
    if (trimmed[0] == '.') {
        return "document.querySelectorAll('" + trimmed + "')";
    }
    // ID选择器
    else if (trimmed[0] == '#') {
        return "document.querySelector('" + trimmed + "')";
    }
    // 复合选择器（包含空格）
    else if (trimmed.find(' ') != std::string::npos) {
        return "document.querySelectorAll('" + trimmed + "')";
    }
    // 标签选择器或类名/ID（先检查是否是标签）
    else {
        // 检查是否是HTML标签
        static const std::set<std::string> html_tags = {
            "div", "span", "button", "input", "form", "a", "p", "h1", "h2", "h3",
            "h4", "h5", "h6", "ul", "ol", "li", "table", "tr", "td", "th", "img",
            "video", "audio", "canvas", "svg", "header", "footer", "nav", "main",
            "section", "article", "aside", "body", "head", "html"
        };
        
        if (html_tags.find(trimmed) != html_tags.end()) {
            return "document.querySelectorAll('" + trimmed + "')";
        } else {
            // 尝试作为类名或ID
            // 先尝试ID
            return "(document.getElementById('" + trimmed + "') || " +
                   "document.getElementsByClassName('" + trimmed + "')[0])";
        }
    }
}

std::string CHTLJSCompiler::transformListen(const std::string& code) {
    // 提取listen的参数
    size_t start = code.find('(');
    size_t end = code.rfind(')');
    
    if (start == std::string::npos || end == std::string::npos) {
        return code; // 语法错误，返回原始代码
    }
    
    std::string params = code.substr(start + 1, end - start - 1);
    
    // 生成addEventListener调用
    std::stringstream ss;
    ss << ".addEventListener";
    ss << "(" << params << ")";
    
    return ss.str();
}

std::string CHTLJSCompiler::transformDelegate(const std::string& code) {
    // 简化的delegate实现
    // 实际实现需要更复杂的参数解析
    
    std::stringstream ss;
    ss << "._chtlDelegate" << unique_id_counter_++;
    ss << code.substr(10); // 跳过"->delegate"
    
    return ss.str();
}

std::string CHTLJSCompiler::transformAnimate(const std::string& code) {
    // 提取animate的参数
    size_t start = code.find('(');
    size_t end = code.rfind(')');
    
    if (start == std::string::npos || end == std::string::npos) {
        return code;
    }
    
    std::string config = code.substr(start + 1, end - start - 1);
    
    return generateAnimateFunction(config);
}

std::string CHTLJSCompiler::generateAnimateFunction(const std::string& config) {
    std::stringstream ss;
    
    ss << "(function() {\n";
    ss << "  const config = " << config << ";\n";
    ss << "  const elements = Array.isArray(config.target) ? config.target : [config.target];\n";
    ss << "  const duration = config.duration || 1000;\n";
    ss << "  const easing = config.easing || 'linear';\n";
    ss << "  const startTime = performance.now();\n";
    ss << "  \n";
    ss << "  function animate(currentTime) {\n";
    ss << "    const elapsed = currentTime - startTime;\n";
    ss << "    const progress = Math.min(elapsed / duration, 1);\n";
    ss << "    \n";
    ss << "    // Apply animation\n";
    ss << "    elements.forEach(el => {\n";
    ss << "      // Apply styles based on progress\n";
    ss << "    });\n";
    ss << "    \n";
    ss << "    if (progress < 1) {\n";
    ss << "      requestAnimationFrame(animate);\n";
    ss << "    } else if (config.callback) {\n";
    ss << "      config.callback();\n";
    ss << "    }\n";
    ss << "  }\n";
    ss << "  \n";
    ss << "  requestAnimationFrame(animate);\n";
    ss << "})()";
    
    return ss.str();
}

std::string CHTLJSCompiler::generateDelegateRegistry() {
    std::stringstream ss;
    
    ss << "// CHTL Event Delegation Registry\n";
    ss << "(function() {\n";
    ss << "  const delegateRegistry = new WeakMap();\n";
    ss << "  \n";
    ss << "  function setupDelegate(parent, config) {\n";
    ss << "    const targets = Array.isArray(config.target) ? config.target : [config.target];\n";
    ss << "    \n";
    ss << "    Object.entries(config).forEach(([event, handler]) => {\n";
    ss << "      if (event === 'target') return;\n";
    ss << "      \n";
    ss << "      parent.addEventListener(event, function(e) {\n";
    ss << "        targets.forEach(target => {\n";
    ss << "          if (e.target.matches(target)) {\n";
    ss << "            handler.call(e.target, e);\n";
    ss << "          }\n";
    ss << "        });\n";
    ss << "      });\n";
    ss << "    });\n";
    ss << "  }\n";
    ss << "  \n";
    ss << "  // Expose delegate setup function\n";
    ss << "  window._chtlDelegate = setupDelegate;\n";
    ss << "})();\n";
    
    return ss.str();
}

std::string CHTLJSCompiler::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

std::string CHTLJSCompiler::escapeJsString(const std::string& str) {
    std::string result;
    for (char ch : str) {
        switch (ch) {
            case '"': result += "\\\""; break;
            case '\'': result += "\\'"; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default: result += ch; break;
        }
    }
    return result;
}

bool CHTLJSCompiler::isValidSelector(const std::string& selector) {
    // 简单的选择器验证
    if (selector.empty()) return false;
    
    // 检查是否是有效的CSS选择器格式
    char first = selector[0];
    if (first != '.' && first != '#' && !std::isalpha(first)) {
        return false;
    }
    
    return true;
}

} // namespace chtl
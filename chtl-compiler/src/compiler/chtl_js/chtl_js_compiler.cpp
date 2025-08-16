#include "compiler/chtl_js/chtl_js_compiler.h"
#include "common/utils.h"
#include <sstream>
#include <algorithm>

namespace chtl {
namespace compiler {

// 静态正则表达式定义
const std::regex CHTLJSCompiler::enhancedSelectorPattern_(R"(\{\{([^}]+)\}\})");
const std::regex CHTLJSCompiler::listenPattern_(R"((\{\{[^}]+\}\})\s*->\s*listen\s*\()");
const std::regex CHTLJSCompiler::delegatePattern_(R"((\{\{[^}]+\}\})\s*->\s*delegate\s*\()");
const std::regex CHTLJSCompiler::animatePattern_(R"(animate\s*\()");

CHTLJSCompiler::CHTLJSCompiler() {
    reset();
}

CHTLJSCompiler::~CHTLJSCompiler() = default;

CompilationResult CHTLJSCompiler::compile(const CodeSegment& segment,
                                        const CompilerOptions& options) {
    CompilationResult result;
    
    try {
        // 设置上下文
        context_.currentElement = segment.parentElement;
        context_.currentClass = segment.className;
        context_.currentId = segment.id;
        
        std::string code = segment.content;
        
        // 处理CHTL JS特性
        code = processEnhancedSelectors(code);
        code = processEnhancedListeners(code);
        code = processEventDelegation(code);
        code = processAnimations(code);
        
        // 生成最终代码
        std::stringstream output;
        
        // 包装在IIFE中，避免全局污染
        output << "(function() {\n";
        output << "  'use strict';\n\n";
        
        // 添加处理后的代码
        output << code << "\n";
        
        // 生成事件委托初始化代码
        if (!context_.delegates.empty()) {
            output << "\n  // Event delegation setup\n";
            output << generateDelegateCode();
        }
        
        output << "})();\n";
        
        result.output = output.str();
        result.success = true;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back("CHTL JS compilation error: " + std::string(e.what()));
    }
    
    return result;
}

void CHTLJSCompiler::reset() {
    context_ = CHTLJSContext();
}

std::string CHTLJSCompiler::processEnhancedSelectors(const std::string& code) {
    std::string result = code;
    std::smatch match;
    std::string::const_iterator searchStart(code.cbegin());
    
    std::stringstream processed;
    size_t lastPos = 0;
    
    while (std::regex_search(searchStart, code.cend(), match, enhancedSelectorPattern_)) {
        // 添加匹配前的代码
        size_t matchPos = match.position(0) + std::distance(code.cbegin(), searchStart);
        processed << code.substr(lastPos, matchPos - lastPos);
        
        // 转换选择器
        std::string selector = match[1].str();
        std::string jsCode = convertSelector(selector);
        processed << jsCode;
        
        // 更新位置
        lastPos = matchPos + match.length(0);
        searchStart = match.suffix().first;
    }
    
    // 添加剩余代码
    processed << code.substr(lastPos);
    
    return processed.str();
}

std::string CHTLJSCompiler::processEnhancedListeners(const std::string& code) {
    std::string result = code;
    std::regex listenRegex(R"(\{\{([^}]+)\}\}\s*->\s*listen\s*\(\s*\{([^}]+)\}\s*\))");
    
    // 使用迭代器而不是lambda来处理替换
    std::smatch match;
    std::string::const_iterator searchStart(result.cbegin());
    std::stringstream processed;
    size_t lastPos = 0;
    
    while (std::regex_search(searchStart, result.cend(), match, listenRegex)) {
        // 添加匹配前的代码
        size_t matchPos = match.position(0) + std::distance(result.cbegin(), searchStart);
        processed << result.substr(lastPos, matchPos - lastPos);
        
        // 处理匹配
        std::string selector = match[1].str();
        std::string listeners = match[2].str();
        
        std::string varName = generateVariableName("elem");
        std::stringstream ss;
        
        // 生成元素获取代码
        ss << "(function() {\n";
        ss << "    const " << varName << " = " << convertSelector(selector) << ";\n";
        ss << "    if (" << varName << ") {\n";
        
        // 解析事件监听器
        // 简化处理：假设格式为 event: handler, event2: handler2
        std::regex eventRegex(R"((\w+)\s*:\s*([^,]+))");
        std::string::const_iterator eventSearchStart(listeners.cbegin());
        std::smatch eventMatch;
        
        while (std::regex_search(eventSearchStart, listeners.cend(), eventMatch, eventRegex)) {
            std::string event = eventMatch[1].str();
            std::string handler = eventMatch[2].str();
            
            ss << "        " << varName << ".addEventListener('" << event << "', " 
               << utils::trim(handler) << ");\n";
            
            eventSearchStart = eventMatch.suffix().first;
        }
        
        ss << "    }\n";
        ss << "})()";
        
        processed << ss.str();
        
        // 更新位置
        lastPos = matchPos + match.length(0);
        searchStart = match.suffix().first;
    }
    
    // 添加剩余代码
    processed << result.substr(lastPos);
    
    return processed.str();
}

std::string CHTLJSCompiler::processEventDelegation(const std::string& code) {
    // TODO: 实现事件委托处理
    // 这需要更复杂的解析逻辑
    return code;
}

std::string CHTLJSCompiler::processAnimations(const std::string& code) {
    // TODO: 实现动画处理
    // 需要生成requestAnimationFrame代码
    return code;
}

std::string CHTLJSCompiler::convertSelector(const std::string& selector) {
    std::string trimmed = utils::trim(selector);
    
    // 处理索引访问 {{button[0]}}
    size_t bracketPos = trimmed.find('[');
    std::string baseSelector = trimmed;
    std::string index;
    
    if (bracketPos != std::string::npos) {
        baseSelector = trimmed.substr(0, bracketPos);
        size_t endBracket = trimmed.find(']', bracketPos);
        if (endBracket != std::string::npos) {
            index = trimmed.substr(bracketPos + 1, endBracket - bracketPos - 1);
        }
    }
    
    std::stringstream jsCode;
    
    // 处理不同类型的选择器
    if (baseSelector[0] == '.') {
        // 类选择器
        if (!index.empty()) {
            jsCode << "document.getElementsByClassName('" << baseSelector.substr(1) << "')[" << index << "]";
        } else {
            jsCode << "document.querySelector('" << baseSelector << "')";
        }
    } else if (baseSelector[0] == '#') {
        // ID选择器
        jsCode << "document.getElementById('" << baseSelector.substr(1) << "')";
    } else {
        // 标签选择器或复杂选择器
        // 首先检查是否是当前上下文的类名或ID
        if (baseSelector == context_.currentClass) {
            jsCode << "document.querySelector('." << baseSelector << "')";
        } else if (baseSelector == context_.currentId) {
            jsCode << "document.getElementById('" << baseSelector << "')";
        } else {
            // 假设是标签选择器
            if (!index.empty()) {
                jsCode << "document.getElementsByTagName('" << baseSelector << "')[" << index << "]";
            } else {
                jsCode << "document.querySelector('" << baseSelector << "')";
            }
        }
    }
    
    return jsCode.str();
}

std::string CHTLJSCompiler::generateDelegateCode() {
    std::stringstream ss;
    
    for (const auto& delegate : context_.delegates) {
        std::string parentVar = generateVariableName("parent");
        
        ss << "  const " << parentVar << " = " << convertSelector(delegate.parentSelector) << ";\n";
        ss << "  if (" << parentVar << ") {\n";
        
        for (const auto& [event, handler] : delegate.eventHandlers) {
            ss << "    " << parentVar << ".addEventListener('" << event << "', function(e) {\n";
            
            // 生成目标匹配逻辑
            ss << "      const target = e.target;\n";
            for (const auto& targetSelector : delegate.targetSelectors) {
                ss << "      if (target.matches('" << targetSelector << "')) {\n";
                ss << "        (" << handler << ").call(target, e);\n";
                ss << "      }\n";
            }
            
            ss << "    });\n";
        }
        
        ss << "  }\n\n";
    }
    
    return ss.str();
}

std::string CHTLJSCompiler::generateVariableName(const std::string& prefix) {
    return prefix + "_" + std::to_string(context_.idCounter++);
}

std::string CHTLJSCompiler::escapeJsString(const std::string& str) {
    std::string result;
    for (char c : str) {
        switch (c) {
            case '"': result += "\\\""; break;
            case '\'': result += "\\'"; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default: result += c;
        }
    }
    return result;
}

} // namespace compiler
} // namespace chtl
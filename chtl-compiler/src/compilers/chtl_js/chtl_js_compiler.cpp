#include "compilers/chtl_js_compiler.h"
#include <sstream>
#include <regex>
#include <algorithm>
#include <set>

namespace chtl {

// CHTL JS编译器实现类
class CHTLJSCompiler::Impl {
public:
    Impl() {
        // 初始化辅助函数模板
        initializeHelperFunctions();
    }
    
    bool compile(const std::string& input, 
                std::string& output,
                std::vector<BaseCompiler::CompileError>& errors) {
        errors_ = &errors;
        reset();
        
        try {
            // 预处理：提取CHTL JS特有语法
            std::string processed = preprocessCHTLJS(input);
            
            // 转换CHTL JS语法到标准JavaScript
            output = transformToJS(processed);
            
            // 添加必要的辅助函数
            if (!usedFeatures_.empty()) {
                output = generateHelperFunctions() + "\n" + output;
            }
            
            // 包装在IIFE中以避免全局污染
            if (config_.enableEnhancedSelector || config_.enableEventDelegation) {
                output = wrapInIIFE(output);
            }
            
            return errors.empty();
            
        } catch (const std::exception& e) {
            addError("CHTL JS编译错误: " + std::string(e.what()), 0, 0);
            return false;
        }
    }
    
    void setOptions(const BaseCompiler::CompileOptions& options) {
        options_ = options;
    }
    
    bool validate(const std::string& input) const {
        // 检查基本语法
        int braceCount = 0;
        int parenCount = 0;
        
        for (char c : input) {
            if (c == '{') braceCount++;
            else if (c == '}') braceCount--;
            else if (c == '(') parenCount++;
            else if (c == ')') parenCount--;
            
            if (braceCount < 0 || parenCount < 0) return false;
        }
        
        return braceCount == 0 && parenCount == 0;
    }
    
    void setCHTLJSConfig(const CHTLJSConfig& config) {
        config_ = config;
    }
    
    std::vector<EventDelegation> getEventDelegations() const {
        return eventDelegations_;
    }
    
    std::string getHelperFunctions() const {
        return generateHelperFunctions();
    }

private:
    // 重置状态
    void reset() {
        eventDelegations_.clear();
        usedFeatures_.clear();
        selectorCache_.clear();
        currentLine_ = 1;
        currentColumn_ = 1;
    }
    
    // 预处理CHTL JS语法
    std::string preprocessCHTLJS(const std::string& input) {
        std::string result = input;
        
        // 处理增强选择器 {{selector}}
        if (config_.enableEnhancedSelector) {
            result = processEnhancedSelectors(result);
        }
        
        // 处理 -> 语法
        if (config_.useArrowSyntax) {
            result = processArrowSyntax(result);
        }
        
        // 处理 listen 语法
        if (config_.enableEnhancedListener) {
            result = processListenSyntax(result);
        }
        
        // 处理 delegate 语法
        if (config_.enableEventDelegation) {
            result = processDelegateSyntax(result);
        }
        
        // 处理 animate 语法
        if (config_.enableAnimationAPI) {
            result = processAnimateSyntax(result);
        }
        
        return result;
    }
    
    // 处理增强选择器
    std::string processEnhancedSelectors(const std::string& input) {
        std::regex selectorRegex(R"(\{\{([^}]+)\}\})");
        std::string result = input;
        std::smatch match;
        
        while (std::regex_search(result, match, selectorRegex)) {
            std::string selector = match[1].str();
            std::string replacement = generateSelectorCode(selector);
            
            result = result.substr(0, match.position()) + 
                     replacement + 
                     result.substr(match.position() + match.length());
            
            usedFeatures_.insert("selector");
        }
        
        return result;
    }
    
    // 生成选择器代码
    std::string generateSelectorCode(const std::string& selector) {
        std::string trimmed = trim(selector);
        std::string cacheKey = trimmed;
        
        // 检查是否有索引访问
        std::regex indexRegex(R"((.+)\[(\d+)\]$)");
        std::smatch match;
        int index = -1;
        std::string baseSelector = trimmed;
        
        if (std::regex_match(trimmed, match, indexRegex)) {
            baseSelector = match[1].str();
            index = std::stoi(match[2].str());
        }
        
        // 生成选择器代码
        std::stringstream code;
        
        if (selectorCache_.find(cacheKey) == selectorCache_.end()) {
            selectorCache_[cacheKey] = "chtl_sel_" + std::to_string(selectorCache_.size());
        }
        
        std::string varName = selectorCache_[cacheKey];
        
        // 判断选择器类型
        if (baseSelector[0] == '.') {
            // 类选择器
            if (index >= 0) {
                code << "document.getElementsByClassName('" << baseSelector.substr(1) << "')[" << index << "]";
            } else {
                code << "document.querySelectorAll('" << baseSelector << "')";
            }
        } else if (baseSelector[0] == '#') {
            // ID选择器
            code << "document.getElementById('" << baseSelector.substr(1) << "')";
        } else if (baseSelector.find(' ') != std::string::npos) {
            // 复合选择器
            if (index >= 0) {
                code << "document.querySelectorAll('" << baseSelector << "')[" << index << "]";
            } else {
                code << "document.querySelectorAll('" << baseSelector << "')";
            }
        } else {
            // 标签选择器或类名/ID（优先查找）
            code << "_chtlSelect('" << baseSelector << "'";
            if (index >= 0) {
                code << ", " << index;
            }
            code << ")";
            usedFeatures_.insert("chtlSelect");
        }
        
        return code.str();
    }
    
    // 处理 -> 语法
    std::string processArrowSyntax(const std::string& input) {
        // 简单替换 -> 为 .
        std::string result = input;
        size_t pos = 0;
        
        while ((pos = result.find("->", pos)) != std::string::npos) {
            // 检查是否在字符串中
            if (!isInString(result, pos)) {
                result.replace(pos, 2, ".");
                pos += 1;
            } else {
                pos += 2;
            }
        }
        
        return result;
    }
    
    // 处理 listen 语法
    std::string processListenSyntax(const std::string& input) {
        std::regex listenRegex(R"((\w+)\.listen\s*\(\s*\{([^}]+)\}\s*\))");
        std::string result = input;
        std::smatch match;
        
        while (std::regex_search(result, match, listenRegex)) {
            std::string element = match[1].str();
            std::string listeners = match[2].str();
            
            std::string replacement = generateListenCode(element, listeners);
            
            result = result.substr(0, match.position()) + 
                     replacement + 
                     result.substr(match.position() + match.length());
            
            usedFeatures_.insert("listen");
        }
        
        return result;
    }
    
    // 生成 listen 代码
    std::string generateListenCode(const std::string& element, const std::string& listeners) {
        std::stringstream code;
        code << "_chtlListen(" << element << ", {\n";
        code << listeners;
        code << "})";
        return code.str();
    }
    
    // 处理 delegate 语法
    std::string processDelegateSyntax(const std::string& input) {
        std::regex delegateRegex(R"((\w+)\.delegate\s*\(\s*\{([^}]+)\}\s*\))");
        std::string result = input;
        std::smatch match;
        
        while (std::regex_search(result, match, delegateRegex)) {
            std::string parent = match[1].str();
            std::string config = match[2].str();
            
            std::string replacement = generateDelegateCode(parent, config);
            
            result = result.substr(0, match.position()) + 
                     replacement + 
                     result.substr(match.position() + match.length());
            
            usedFeatures_.insert("delegate");
        }
        
        return result;
    }
    
    // 生成 delegate 代码
    std::string generateDelegateCode(const std::string& parent, const std::string& config) {
        // 解析配置提取事件委托信息
        // 这里简化处理，实际需要更复杂的解析
        EventDelegation delegation;
        delegation.parentSelector = parent;
        // ... 解析 config 填充其他字段
        eventDelegations_.push_back(delegation);
        
        std::stringstream code;
        code << "_chtlDelegate(" << parent << ", {\n";
        code << config;
        code << "})";
        return code.str();
    }
    
    // 处理 animate 语法
    std::string processAnimateSyntax(const std::string& input) {
        std::regex animateRegex(R"(animate\s*\(\s*\{([^}]+(?:\{[^}]*\}[^}]*)*)\}\s*\))");
        std::string result = input;
        std::smatch match;
        
        while (std::regex_search(result, match, animateRegex)) {
            std::string animConfig = match[1].str();
            
            std::string replacement = generateAnimateCode(animConfig);
            
            result = result.substr(0, match.position()) + 
                     replacement + 
                     result.substr(match.position() + match.length());
            
            usedFeatures_.insert("animate");
        }
        
        return result;
    }
    
    // 生成 animate 代码
    std::string generateAnimateCode(const std::string& config) {
        std::stringstream code;
        code << "_chtlAnimate({\n";
        code << config;
        code << "})";
        return code.str();
    }
    
    // 转换为标准JavaScript
    std::string transformToJS(const std::string& processed) {
        // 这里已经处理了大部分CHTL JS语法
        // 可能需要做一些最终的清理或格式化
        return processed;
    }
    
    // 生成辅助函数
    std::string generateHelperFunctions() const {
        std::stringstream functions;
        
        if (usedFeatures_.count("chtlSelect")) {
            functions << helperFunctions_.at("chtlSelect") << "\n\n";
        }
        
        if (usedFeatures_.count("listen")) {
            functions << helperFunctions_.at("listen") << "\n\n";
        }
        
        if (usedFeatures_.count("delegate")) {
            functions << helperFunctions_.at("delegate") << "\n\n";
        }
        
        if (usedFeatures_.count("animate")) {
            functions << helperFunctions_.at("animate") << "\n\n";
        }
        
        return functions.str();
    }
    
    // 包装在IIFE中
    std::string wrapInIIFE(const std::string& code) const {
        std::stringstream wrapped;
        wrapped << "(function() {\n";
        wrapped << "'use strict';\n\n";
        wrapped << code;
        wrapped << "\n})();";
        return wrapped.str();
    }
    
    // 初始化辅助函数
    void initializeHelperFunctions() {
        // _chtlSelect 函数
        helperFunctions_["chtlSelect"] = R"(
function _chtlSelect(selector, index) {
    // 先尝试作为标签名
    let elements = document.getElementsByTagName(selector);
    if (elements.length > 0) {
        return index !== undefined ? elements[index] : elements;
    }
    
    // 再尝试作为类名
    elements = document.getElementsByClassName(selector);
    if (elements.length > 0) {
        return index !== undefined ? elements[index] : elements;
    }
    
    // 最后尝试作为ID
    let element = document.getElementById(selector);
    if (element) {
        return element;
    }
    
    // 都没找到，返回null或空NodeList
    return index !== undefined ? null : document.querySelectorAll('.chtl-not-found');
})";
        
        // _chtlListen 函数
        helperFunctions_["listen"] = R"(
function _chtlListen(element, listeners) {
    if (!element) return;
    
    // 处理单个元素
    function attachListeners(el) {
        for (let event in listeners) {
            if (listeners.hasOwnProperty(event)) {
                el.addEventListener(event, listeners[event]);
            }
        }
    }
    
    // 处理NodeList或单个元素
    if (element instanceof NodeList || element instanceof HTMLCollection) {
        Array.from(element).forEach(attachListeners);
    } else if (element instanceof Element) {
        attachListeners(element);
    }
})";
        
        // _chtlDelegate 函数
        helperFunctions_["delegate"] = R"(
// 全局委托注册表
const _chtlDelegateRegistry = new Map();

function _chtlDelegate(parent, config) {
    if (!parent) return;
    
    const parentEl = parent instanceof Element ? parent : 
                     parent instanceof NodeList ? parent[0] : 
                     document.querySelector(parent);
                     
    if (!parentEl) return;
    
    const target = config.target;
    const targets = Array.isArray(target) ? target : [target];
    
    // 为每个事件类型设置委托
    for (let eventType in config) {
        if (eventType === 'target') continue;
        if (typeof config[eventType] !== 'function') continue;
        
        // 获取或创建父元素的委托处理器
        const key = parentEl;
        if (!_chtlDelegateRegistry.has(key)) {
            _chtlDelegateRegistry.set(key, new Map());
        }
        
        const parentHandlers = _chtlDelegateRegistry.get(key);
        
        if (!parentHandlers.has(eventType)) {
            // 创建新的委托处理器
            const delegateHandler = function(e) {
                const handlers = parentHandlers.get(eventType);
                if (!handlers) return;
                
                // 检查事件目标是否匹配任何选择器
                for (let [selector, handler] of handlers) {
                    if (e.target.matches(selector)) {
                        handler.call(e.target, e);
                    }
                }
            };
            
            parentEl.addEventListener(eventType, delegateHandler);
            parentHandlers.set(eventType, new Map());
        }
        
        // 添加目标选择器和处理函数
        const eventHandlers = parentHandlers.get(eventType);
        targets.forEach(t => {
            const selector = typeof t === 'string' ? t : t.toString();
            eventHandlers.set(selector, config[eventType]);
        });
    }
})";
        
        // _chtlAnimate 函数
        helperFunctions_["animate"] = R"(
function _chtlAnimate(config) {
    const duration = config.duration || 1000;
    const easing = config.easing || 'linear';
    const loop = config.loop || 1;
    const direction = config.direction || 'normal';
    const delay = config.delay || 0;
    const callback = config.callback;
    
    const begin = config.begin || {};
    const end = config.end || {};
    const when = config.when || [];
    
    let startTime = null;
    let currentLoop = 0;
    
    function animate(timestamp) {
        if (!startTime) {
            startTime = timestamp + delay;
        }
        
        const elapsed = timestamp - startTime;
        if (elapsed < 0) {
            requestAnimationFrame(animate);
            return;
        }
        
        const progress = Math.min(elapsed / duration, 1);
        const easedProgress = applyEasing(progress, easing);
        
        // 应用动画
        applyStyles(begin, end, easedProgress, when);
        
        if (progress < 1) {
            requestAnimationFrame(animate);
        } else {
            currentLoop++;
            if (loop === -1 || currentLoop < loop) {
                startTime = null;
                requestAnimationFrame(animate);
            } else if (callback) {
                callback();
            }
        }
    }
    
    function applyEasing(progress, easing) {
        // 简化的缓动函数实现
        switch (easing) {
            case 'ease-in':
                return progress * progress;
            case 'ease-out':
                return progress * (2 - progress);
            case 'ease-in-out':
                return progress < 0.5 ?
                    2 * progress * progress :
                    -1 + (4 - 2 * progress) * progress;
            default:
                return progress;
        }
    }
    
    function applyStyles(begin, end, progress, when) {
        // 实现样式插值
        // 这里需要更复杂的实现来处理各种CSS属性
    }
    
    setTimeout(() => requestAnimationFrame(animate), 0);
})";
    }
    
    // 工具函数
    std::string trim(const std::string& str) const {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, last - first + 1);
    }
    
    bool isInString(const std::string& str, size_t pos) const {
        int singleQuotes = 0;
        int doubleQuotes = 0;
        
        for (size_t i = 0; i < pos && i < str.length(); ++i) {
            if (str[i] == '\'' && (i == 0 || str[i-1] != '\\')) {
                singleQuotes++;
            } else if (str[i] == '"' && (i == 0 || str[i-1] != '\\')) {
                doubleQuotes++;
            }
        }
        
        return (singleQuotes % 2 == 1) || (doubleQuotes % 2 == 1);
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
    CHTLJSConfig config_;
    
    std::vector<BaseCompiler::CompileError>* errors_;
    std::vector<EventDelegation> eventDelegations_;
    
    std::set<std::string> usedFeatures_;
    std::map<std::string, std::string> selectorCache_;
    std::map<std::string, std::string> helperFunctions_;
    
    size_t currentLine_;
    size_t currentColumn_;
};

// CHTLJSCompiler 实现
CHTLJSCompiler::CHTLJSCompiler()
    : pImpl(std::make_unique<Impl>()) {}

CHTLJSCompiler::~CHTLJSCompiler() = default;

bool CHTLJSCompiler::compile(const std::string& input, 
                           std::string& output,
                           std::vector<CompileError>& errors) {
    return pImpl->compile(input, output, errors);
}

void CHTLJSCompiler::setOptions(const CompileOptions& options) {
    pImpl->setOptions(options);
}

bool CHTLJSCompiler::validate(const std::string& input) const {
    return pImpl->validate(input);
}

void CHTLJSCompiler::setCHTLJSConfig(const CHTLJSConfig& config) {
    pImpl->setCHTLJSConfig(config);
}

std::vector<CHTLJSCompiler::EventDelegation> CHTLJSCompiler::getEventDelegations() const {
    return pImpl->getEventDelegations();
}

std::string CHTLJSCompiler::getHelperFunctions() const {
    return pImpl->getHelperFunctions();
}

} // namespace chtl
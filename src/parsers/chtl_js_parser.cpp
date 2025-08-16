#include "parsers/chtl_js_parser.hpp"
#include "parsers/chtl_basic_parser.hpp"
#include <regex>
#include <algorithm>
#include <sstream>

namespace chtl {

// ===== CHTLJSSelectorRegistry 实现 =====

CHTLJSSelectorRegistry::CHTLJSSelectorRegistry() = default;

CHTLJSSelectorRegistry::~CHTLJSSelectorRegistry() = default;

bool CHTLJSSelectorRegistry::registerSelector(const CHTLJSSelector& selector) {
    if (selectorIndexMap_.find(selector.selector) != selectorIndexMap_.end()) {
        return false; // 重复定义
    }
    
    selectors_.push_back(selector);
    selectorIndexMap_[selector.selector] = selectors_.size() - 1;
    
    return true;
}

const CHTLJSSelector* CHTLJSSelectorRegistry::findSelector(const std::string& selectorStr) const {
    auto it = selectorIndexMap_.find(selectorStr);
    return (it != selectorIndexMap_.end()) ? &selectors_[it->second] : nullptr;
}

const std::vector<CHTLJSSelector>& CHTLJSSelectorRegistry::getAllSelectors() const {
    return selectors_;
}

bool CHTLJSSelectorRegistry::isValidSelector(const std::string& selectorStr) const {
    // 基本的选择器验证
    if (selectorStr.empty()) return false;
    
    std::regex selectorRegex(R"(^(\.[\w-]+|#[\w-]+|[\w-]+|\[[\w-]+\]|[\w-]+\s+[\w-]+)$)");
    return std::regex_match(selectorStr, selectorRegex);
}

void CHTLJSSelectorRegistry::clear() {
    selectors_.clear();
    selectorIndexMap_.clear();
}

// ===== CHTLJSEventDelegateRegistry 实现 =====

CHTLJSEventDelegateRegistry::CHTLJSEventDelegateRegistry() = default;

CHTLJSEventDelegateRegistry::~CHTLJSEventDelegateRegistry() = default;

bool CHTLJSEventDelegateRegistry::registerDelegate(const std::string& parentSelector, const CHTLJSDelegate& delegate) {
    delegateMap_[parentSelector].push_back(delegate);
    return true;
}

std::vector<CHTLJSDelegate> CHTLJSEventDelegateRegistry::getDelegates(const std::string& parentSelector) const {
    auto it = delegateMap_.find(parentSelector);
    return (it != delegateMap_.end()) ? it->second : std::vector<CHTLJSDelegate>{};
}

void CHTLJSEventDelegateRegistry::mergeDelegates(const std::string& parentSelector) {
    auto it = delegateMap_.find(parentSelector);
    if (it == delegateMap_.end() || it->second.size() <= 1) {
        return; // 没有需要合并的委托
    }
    
    // 简单的合并逻辑：将所有委托的目标和监听器合并到第一个委托中
    auto& delegates = it->second;
    CHTLJSDelegate merged = delegates[0];
    
    for (size_t i = 1; i < delegates.size(); ++i) {
        const auto& delegate = delegates[i];
        merged.targets.insert(merged.targets.end(), delegate.targets.begin(), delegate.targets.end());
        merged.listeners.insert(merged.listeners.end(), delegate.listeners.begin(), delegate.listeners.end());
    }
    
    // 清空并添加合并后的委托
    delegates.clear();
    delegates.push_back(merged);
}

void CHTLJSEventDelegateRegistry::clear() {
    delegateMap_.clear();
}

// ===== CHTLJSScanner 实现 =====

CHTLJSScanner::CHTLJSScanner() = default;

CHTLJSScanner::~CHTLJSScanner() = default;

CHTLJSScriptBlock CHTLJSScanner::scanScriptBlock(const std::string& content) {
    CHTLJSScriptBlock scriptBlock;
    scriptBlock.rawCode = content;
    
    // 扫描增强选择器
    scriptBlock.selectors = scanSelectors(content);
    
    // 扫描监听器
    scriptBlock.listeners = scanListeners(content);
    
    // 扫描委托
    scriptBlock.delegates = scanDelegates(content);
    
    // 扫描动画
    scriptBlock.animations = scanAnimations(content);
    
    return scriptBlock;
}

std::vector<CHTLJSSelector> CHTLJSScanner::scanSelectors(const std::string& content) {
    std::vector<CHTLJSSelector> selectors;
    
    // 使用正则表达式查找所有增强选择器 {{...}}
    std::regex selectorRegex(R"(\{\{([^}]+)\}\})");
    std::sregex_iterator iter(content.begin(), content.end(), selectorRegex);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        std::smatch match = *iter;
        std::string selectorStr = match[1].str();
        
        // 去除空白
        selectorStr = trimWhitespace(selectorStr);
        
        if (!selectorStr.empty()) {
            CHTLJSSelector selector = parseSelector(selectorStr);
            selectors.push_back(selector);
        }
    }
    
    return selectors;
}

std::vector<CHTLJSListener> CHTLJSScanner::scanListeners(const std::string& content) {
    std::vector<CHTLJSListener> listeners;
    
    // 查找listen调用的起始位置
    size_t pos = 0;
    while ((pos = content.find("->listen", pos)) != std::string::npos) {
        // 寻找开始的花括号
        size_t braceStart = content.find("{", pos);
        if (braceStart == std::string::npos) {
            pos++;
            continue;
        }
        
        // 寻找匹配的结束花括号
        int braceCount = 1;
        size_t braceEnd = braceStart + 1;
        bool inString = false;
        char stringChar = '\0';
        
        while (braceEnd < content.length() && braceCount > 0) {
            char c = content[braceEnd];
            
            if (!inString && (c == '\'' || c == '"')) {
                inString = true;
                stringChar = c;
            } else if (inString && c == stringChar && (braceEnd == 0 || content[braceEnd-1] != '\\')) {
                inString = false;
            } else if (!inString) {
                if (c == '{') {
                    braceCount++;
                } else if (c == '}') {
                    braceCount--;
                }
            }
            braceEnd++;
        }
        
        if (braceCount == 0) {
            // 提取对象内容
            std::string listenConfig = content.substr(braceStart + 1, braceEnd - braceStart - 2);
            
            // 解析监听器配置
            auto properties = parseObjectProperties(listenConfig);
            for (const auto& prop : properties) {
                CHTLJSListener listener(prop.first, prop.second);
                listeners.push_back(listener);
            }
        }
        
        pos = braceEnd;
    }
    
    return listeners;
}

std::vector<CHTLJSDelegate> CHTLJSScanner::scanDelegates(const std::string& content) {
    std::vector<CHTLJSDelegate> delegates;
    
    // 查找delegate调用的起始位置
    size_t pos = 0;
    while ((pos = content.find("->delegate", pos)) != std::string::npos) {
        // 寻找开始的花括号
        size_t braceStart = content.find("{", pos);
        if (braceStart == std::string::npos) {
            pos++;
            continue;
        }
        
        // 寻找匹配的结束花括号
        int braceCount = 1;
        size_t braceEnd = braceStart + 1;
        bool inString = false;
        char stringChar = '\0';
        
        while (braceEnd < content.length() && braceCount > 0) {
            char c = content[braceEnd];
            
            if (!inString && (c == '\'' || c == '"')) {
                inString = true;
                stringChar = c;
            } else if (inString && c == stringChar && (braceEnd == 0 || content[braceEnd-1] != '\\')) {
                inString = false;
            } else if (!inString) {
                if (c == '{') {
                    braceCount++;
                } else if (c == '}') {
                    braceCount--;
                }
            }
            braceEnd++;
        }
        
        if (braceCount == 0) {
            // 提取对象内容
            std::string delegateConfig = content.substr(braceStart + 1, braceEnd - braceStart - 2);
            
            CHTLJSDelegate delegate = parseDelegate(delegateConfig);
            delegates.push_back(delegate);
        }
        
        pos = braceEnd;
    }
    
    return delegates;
}

std::vector<CHTLJSAnimation> CHTLJSScanner::scanAnimations(const std::string& content) {
    std::vector<CHTLJSAnimation> animations;
    
    // 查找animate调用的起始位置
    size_t pos = 0;
    while ((pos = content.find("animate", pos)) != std::string::npos) {
        // 寻找开始的花括号
        size_t braceStart = content.find("{", pos);
        if (braceStart == std::string::npos) {
            pos++;
            continue;
        }
        
        // 寻找匹配的结束花括号
        int braceCount = 1;
        size_t braceEnd = braceStart + 1;
        bool inString = false;
        char stringChar = '\0';
        
        while (braceEnd < content.length() && braceCount > 0) {
            char c = content[braceEnd];
            
            if (!inString && (c == '\'' || c == '"')) {
                inString = true;
                stringChar = c;
            } else if (inString && c == stringChar && (braceEnd == 0 || content[braceEnd-1] != '\\')) {
                inString = false;
            } else if (!inString) {
                if (c == '{') {
                    braceCount++;
                } else if (c == '}') {
                    braceCount--;
                }
            }
            braceEnd++;
        }
        
        if (braceCount == 0) {
            // 提取对象内容
            std::string animateConfig = content.substr(braceStart + 1, braceEnd - braceStart - 2);
            
            CHTLJSAnimation animation = parseAnimation(animateConfig);
            animations.push_back(animation);
        }
        
        pos = braceEnd;
    }
    
    return animations;
}

bool CHTLJSScanner::validateCHTLJSSyntax(const std::string& content) {
    // 基本的CHTL JS语法验证
    
    // 检查增强选择器语法
    std::regex selectorRegex(R"(\{\{[^}]+\}\})");
    if (std::regex_search(content, selectorRegex)) {
        // 验证选择器内容
        std::sregex_iterator iter(content.begin(), content.end(), selectorRegex);
        std::sregex_iterator end;
        
        for (; iter != end; ++iter) {
            std::smatch match = *iter;
            std::string selectorContent = match[0].str();
            
            // 移除外层的{{}}
            selectorContent = selectorContent.substr(2, selectorContent.length() - 4);
            selectorContent = trimWhitespace(selectorContent);
            
            if (!isValidSelectorName(selectorContent)) {
                return false;
            }
        }
    }
    
    // 检查箭头操作符语法
    std::regex arrowRegex(R"(->\s*\w+)");
    if (std::regex_search(content, arrowRegex)) {
        // 箭头操作符语法正确
    }
    
    return true;
}

CHTLJSSelector CHTLJSScanner::parseSelector(const std::string& selectorStr) {
    CHTLJSSelector selector;
    selector.selector = selectorStr;
    selector.type = determineSelectorType(selectorStr);
    
    // 解析索引选择器
    std::regex indexRegex(R"((.+)\[(\d+)\])");
    std::smatch match;
    if (std::regex_match(selectorStr, match, indexRegex)) {
        selector.selector = match[1].str();
        selector.index = std::stoi(match[2].str());
        selector.type = CHTLJSSelectorType::INDEXED;
    }
    
    // 解析后代选择器
    if (selectorStr.find(' ') != std::string::npos) {
        // 按空格分割，不是按逗号
        std::istringstream stream(selectorStr);
        std::vector<std::string> parts;
        std::string part;
        
        while (stream >> part) {
            parts.push_back(part);
        }
        
        if (parts.size() >= 2) {
            selector.parentSelector = parts[0];
            selector.selector = parts[1];
            selector.type = CHTLJSSelectorType::DESCENDANT;
        }
    }
    
    return selector;
}

CHTLJSSelectorType CHTLJSScanner::determineSelectorType(const std::string& selectorStr) {
    if (selectorStr.empty()) {
        return CHTLJSSelectorType::AUTO;
    }
    
    if (selectorStr[0] == '.') {
        return CHTLJSSelectorType::CLASS;
    } else if (selectorStr[0] == '#') {
        return CHTLJSSelectorType::ID;
    } else if (selectorStr.find(' ') != std::string::npos) {
        return CHTLJSSelectorType::DESCENDANT;
    } else if (selectorStr.find('[') != std::string::npos) {
        return CHTLJSSelectorType::INDEXED;
    } else {
        // 检查是否为HTML标签
        std::regex tagRegex(R"(^[a-zA-Z][a-zA-Z0-9]*$)");
        if (std::regex_match(selectorStr, tagRegex)) {
            return CHTLJSSelectorType::TAG;
        } else {
            return CHTLJSSelectorType::AUTO;
        }
    }
}

CHTLJSListener CHTLJSScanner::parseListener(const std::string& eventType, const std::string& handlerCode) {
    CHTLJSListener listener;
    listener.eventType = trimWhitespace(eventType);
    listener.handlerCode = trimWhitespace(handlerCode);
    
    return listener;
}

CHTLJSDelegate CHTLJSScanner::parseDelegate(const std::string& delegateCode) {
    CHTLJSDelegate delegate;
    
    // 手动解析delegate配置，特别处理target属性
    std::string trimmed = trimWhitespace(delegateCode);
    
    size_t pos = 0;
    while (pos < trimmed.length()) {
        // 寻找属性名
        size_t colonPos = trimmed.find(':', pos);
        if (colonPos == std::string::npos) break;
        
        std::string key = trimWhitespace(trimmed.substr(pos, colonPos - pos));
        
        // 寻找属性值
        pos = colonPos + 1;
        size_t valueStart = pos;
        
        if (key == "target") {
            // 特殊处理target属性，需要找到完整的数组
            int bracketCount = 0;
            int braceCount = 0;
            bool inString = false;
            char stringChar = '\0';
            
            while (pos < trimmed.length()) {
                char c = trimmed[pos];
                
                if (!inString && (c == '\'' || c == '"')) {
                    inString = true;
                    stringChar = c;
                } else if (inString && c == stringChar && (pos == 0 || trimmed[pos-1] != '\\')) {
                    inString = false;
                } else if (!inString) {
                    if (c == '[') {
                        bracketCount++;
                    } else if (c == ']') {
                        bracketCount--;
                    } else if (c == '{') {
                        braceCount++;
                    } else if (c == '}') {
                        braceCount--;
                    } else if (c == ',' && bracketCount == 0 && braceCount == 0) {
                        break;
                    }
                }
                pos++;
            }
            
            std::string targetValue = trimWhitespace(trimmed.substr(valueStart, pos - valueStart));
            delegate.targets = parseTargetSelectors(targetValue);
        } else {
            // 普通事件监听器
            int braceCount = 0;
            bool inString = false;
            char stringChar = '\0';
            
            while (pos < trimmed.length()) {
                char c = trimmed[pos];
                
                if (!inString && (c == '\'' || c == '"')) {
                    inString = true;
                    stringChar = c;
                } else if (inString && c == stringChar && (pos == 0 || trimmed[pos-1] != '\\')) {
                    inString = false;
                } else if (!inString) {
                    if (c == '{') {
                        braceCount++;
                    } else if (c == '}') {
                        braceCount--;
                    } else if (c == ',' && braceCount == 0) {
                        break;
                    }
                }
                pos++;
            }
            
            std::string value = trimWhitespace(trimmed.substr(valueStart, pos - valueStart));
            CHTLJSListener listener(key, value);
            delegate.listeners.push_back(listener);
        }
        
        // 跳过逗号
        if (pos < trimmed.length() && trimmed[pos] == ',') {
            pos++;
        }
    }
    
    return delegate;
}

std::vector<CHTLJSSelector> CHTLJSScanner::parseTargetSelectors(const std::string& targetsStr) {
    std::vector<CHTLJSSelector> targets;
    
    std::string trimmed = trimWhitespace(targetsStr);
    
    // 检查是否为数组格式 [{{selector1}}, {{selector2}}]
    if (!trimmed.empty() && trimmed.front() == '[' && trimmed.back() == ']') {
        trimmed = trimmed.substr(1, trimmed.length() - 2);
        
        // 手动分割，处理嵌套的{{}}
        size_t pos = 0;
        while (pos < trimmed.length()) {
            // 跳过空白和逗号
            while (pos < trimmed.length() && (std::isspace(trimmed[pos]) || trimmed[pos] == ',')) {
                pos++;
            }
            if (pos >= trimmed.length()) break;
            
            // 寻找{{开始
            if (pos + 1 < trimmed.length() && trimmed.substr(pos, 2) == "{{") {
                size_t selectorStart = pos + 2;
                size_t selectorEnd = trimmed.find("}}", selectorStart);
                
                if (selectorEnd != std::string::npos) {
                    std::string selectorStr = trimmed.substr(selectorStart, selectorEnd - selectorStart);
                    CHTLJSSelector selector = parseSelector(trimWhitespace(selectorStr));
                    targets.push_back(selector);
                    pos = selectorEnd + 2;
                } else {
                    break;
                }
            } else {
                pos++;
            }
        }
    } else {
        // 单个选择器
        if (trimmed.length() >= 4 && trimmed.substr(0, 2) == "{{" && trimmed.substr(trimmed.length() - 2) == "}}") {
            trimmed = trimmed.substr(2, trimmed.length() - 4);
            CHTLJSSelector selector = parseSelector(trimmed);
            targets.push_back(selector);
        }
    }
    
    return targets;
}

CHTLJSAnimation CHTLJSScanner::parseAnimation(const std::string& animationCode) {
    CHTLJSAnimation animation;
    
    // 手动解析动画配置，特别处理嵌套对象
    std::string trimmed = trimWhitespace(animationCode);
    
    size_t pos = 0;
    while (pos < trimmed.length()) {
        // 寻找属性名
        size_t colonPos = trimmed.find(':', pos);
        if (colonPos == std::string::npos) break;
        
        std::string key = trimWhitespace(trimmed.substr(pos, colonPos - pos));
        
        // 寻找属性值
        pos = colonPos + 1;
        size_t valueStart = pos;
        
        if (key == "begin" || key == "end") {
            // 特殊处理嵌套对象
            int braceCount = 0;
            bool inString = false;
            char stringChar = '\0';
            
            while (pos < trimmed.length()) {
                char c = trimmed[pos];
                
                if (!inString && (c == '\'' || c == '"')) {
                    inString = true;
                    stringChar = c;
                } else if (inString && c == stringChar && (pos == 0 || trimmed[pos-1] != '\\')) {
                    inString = false;
                } else if (!inString) {
                    if (c == '{') {
                        braceCount++;
                    } else if (c == '}') {
                        braceCount--;
                        if (braceCount == 0) {
                            pos++; // 包含结束花括号
                            break;
                        }
                    } else if (c == ',' && braceCount == 0) {
                        break;
                    }
                }
                pos++;
            }
            
            std::string objectValue = trimWhitespace(trimmed.substr(valueStart, pos - valueStart));
            // 去除外层花括号
            if (!objectValue.empty() && objectValue.front() == '{' && objectValue.back() == '}') {
                objectValue = objectValue.substr(1, objectValue.length() - 2);
            }
            
            auto properties = parseObjectProperties(objectValue);
            if (key == "begin") {
                animation.beginState = properties;
            } else {
                animation.endState = properties;
            }
        } else {
            // 普通属性
            int braceCount = 0;
            bool inString = false;
            char stringChar = '\0';
            
            while (pos < trimmed.length()) {
                char c = trimmed[pos];
                
                if (!inString && (c == '\'' || c == '"')) {
                    inString = true;
                    stringChar = c;
                } else if (inString && c == stringChar && (pos == 0 || trimmed[pos-1] != '\\')) {
                    inString = false;
                } else if (!inString) {
                    if (c == '{') {
                        braceCount++;
                    } else if (c == '}') {
                        braceCount--;
                    } else if (c == ',' && braceCount == 0) {
                        break;
                    }
                }
                pos++;
            }
            
            std::string value = trimWhitespace(trimmed.substr(valueStart, pos - valueStart));
            
            if (key == "duration") {
                animation.duration = std::stoi(value);
            } else if (key == "easing") {
                animation.easing = value;
            } else if (key == "loop") {
                animation.loop = std::stoi(value);
            } else if (key == "delay") {
                animation.delay = std::stoi(value);
            } else if (key == "callback") {
                animation.callback = value;
            } else if (key == "direction") {
                animation.direction = value;
            }
        }
        
        // 跳过逗号
        if (pos < trimmed.length() && trimmed[pos] == ',') {
            pos++;
        }
    }
    
    return animation;
}

CHTLJSAnimationKeyframe CHTLJSScanner::parseKeyframe(const std::string& keyframeCode) {
    CHTLJSAnimationKeyframe keyframe;
    
    auto properties = parseObjectProperties(keyframeCode);
    
    for (const auto& prop : properties) {
        if (prop.first == "at") {
            keyframe.at = std::stod(prop.second);
        } else {
            keyframe.properties[prop.first] = prop.second;
        }
    }
    
    return keyframe;
}

std::string CHTLJSScanner::trimWhitespace(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> CHTLJSScanner::splitByComma(const std::string& str) {
    std::vector<std::string> parts;
    std::istringstream stream(str);
    std::string part;
    
    while (std::getline(stream, part, ',')) {
        part = trimWhitespace(part);
        if (!part.empty()) {
            parts.push_back(part);
        }
    }
    
    return parts;
}

bool CHTLJSScanner::isValidSelectorName(const std::string& name) const {
    if (name.empty()) return false;
    
    // 基本的选择器名称验证
    std::regex nameRegex(R"(^[\.#]?[\w-]+(\s+[\w-]+)*(\[\d+\])?$)");
    return std::regex_match(name, nameRegex);
}

bool CHTLJSScanner::isValidEventType(const std::string& eventType) {
    // 常见的DOM事件类型
    std::unordered_set<std::string> validEvents = {
        "click", "dblclick", "mousedown", "mouseup", "mousemove", "mouseenter", "mouseleave",
        "keydown", "keyup", "keypress", "focus", "blur", "change", "input", "submit",
        "load", "unload", "resize", "scroll", "error"
    };
    
    return validEvents.find(eventType) != validEvents.end();
}

std::unordered_map<std::string, std::string> CHTLJSScanner::parseObjectProperties(const std::string& objStr) {
    std::unordered_map<std::string, std::string> properties;
    
    // 改进的对象属性解析，处理嵌套的花括号
    std::string trimmed = trimWhitespace(objStr);
    
    size_t pos = 0;
    while (pos < trimmed.length()) {
        // 寻找属性名
        size_t colonPos = trimmed.find(':', pos);
        if (colonPos == std::string::npos) break;
        
        std::string key = trimWhitespace(trimmed.substr(pos, colonPos - pos));
        
        // 寻找属性值
        pos = colonPos + 1;
        size_t valueStart = pos;
        int braceCount = 0;
        bool inString = false;
        char stringChar = '\0';
        
        while (pos < trimmed.length()) {
            char c = trimmed[pos];
            
            if (!inString && (c == '\'' || c == '"')) {
                inString = true;
                stringChar = c;
            } else if (inString && c == stringChar && (pos == 0 || trimmed[pos-1] != '\\')) {
                inString = false;
            } else if (!inString) {
                if (c == '{') {
                    braceCount++;
                } else if (c == '}') {
                    braceCount--;
                } else if (c == ',' && braceCount == 0) {
                    break;
                }
            }
            pos++;
        }
        
        std::string value = trimWhitespace(trimmed.substr(valueStart, pos - valueStart));
        if (!key.empty() && !value.empty()) {
            properties[key] = value;
        }
        
        // 跳过逗号
        if (pos < trimmed.length() && trimmed[pos] == ',') {
            pos++;
        }
    }
    
    return properties;
}

// ===== CHTLJSStateMachine 实现 =====

CHTLJSStateMachine::CHTLJSStateMachine() = default;

CHTLJSStateMachine::~CHTLJSStateMachine() = default;

void CHTLJSStateMachine::reset() {
    scriptBlocks_.clear();
    currentScriptBlock_ = CHTLJSScriptBlock{};
    currentContent_.clear();
}

bool CHTLJSStateMachine::processScriptToken(const Token& token, CHTLJSContext& context) {
    switch (context.state) {
        case CHTLJSParseState::INITIAL:
            handleInitialState(token, context);
            break;
        case CHTLJSParseState::READING_SCRIPT_HEADER:
            handleScriptHeaderState(token, context);
            break;
        case CHTLJSParseState::READING_SCRIPT_CONTENT:
            handleScriptContentState(token, context);
            break;
        case CHTLJSParseState::READING_SELECTOR:
            handleSelectorState(token, context);
            break;
        case CHTLJSParseState::READING_METHOD_CALL:
            handleMethodCallState(token, context);
            break;
        default:
            break;
    }
    
    return true;
}

const std::vector<CHTLJSScriptBlock>& CHTLJSStateMachine::getScriptBlocks() const {
    return scriptBlocks_;
}

void CHTLJSStateMachine::handleInitialState(const Token& token, CHTLJSContext& context) {
    if (token.type == TokenType::IDENTIFIER && token.value == "script") {
        context.state = CHTLJSParseState::READING_SCRIPT_HEADER;
        currentScriptBlock_ = CHTLJSScriptBlock{};
    }
}

void CHTLJSStateMachine::handleScriptHeaderState(const Token& token, CHTLJSContext& context) {
    if (token.type == TokenType::LEFT_BRACE) {
        context.state = CHTLJSParseState::READING_SCRIPT_CONTENT;
        context.nestingLevel = 1;
        context.inScriptBlock = true;
        currentContent_.clear();
    }
}

void CHTLJSStateMachine::handleScriptContentState(const Token& token, CHTLJSContext& context) {
    if (token.type == TokenType::LEFT_BRACE) {
        context.nestingLevel++;
        currentContent_ += token.value;
    } else if (token.type == TokenType::RIGHT_BRACE) {
        context.nestingLevel--;
        if (context.nestingLevel == 0) {
            // 脚本块结束
            context.inScriptBlock = false;
            context.state = CHTLJSParseState::INITIAL;
            
            // 解析脚本内容
            CHTLJSScanner scanner;
            currentScriptBlock_ = scanner.scanScriptBlock(currentContent_);
            addScriptBlock(currentScriptBlock_);
        } else {
            currentContent_ += token.value;
        }
    } else {
        currentContent_ += token.value;
    }
}

void CHTLJSStateMachine::handleSelectorState(const Token& token, CHTLJSContext& context) {
    // 处理增强选择器的解析
    context.tokenBuffer.push_back(token.value);
}

void CHTLJSStateMachine::handleMethodCallState(const Token& token, CHTLJSContext& context) {
    // 处理方法调用的解析
    context.tokenBuffer.push_back(token.value);
}

void CHTLJSStateMachine::addScriptBlock(const CHTLJSScriptBlock& scriptBlock) {
    scriptBlocks_.push_back(scriptBlock);
}

bool CHTLJSStateMachine::isValidCHTLJSToken(const Token& token) {
    // 检查是否为有效的CHTL JS token
    if (token.type == TokenType::IDENTIFIER) {
        return true;
    } else if (token.type == TokenType::OPERATOR) {
        // 检查特殊操作符
        return token.value == "->" || token.value == "{{" || token.value == "}}";
    }
    return false;
}

CHTLJSSelectorType CHTLJSStateMachine::detectSelectorType(const std::string& content) {
    if (content.empty()) return CHTLJSSelectorType::AUTO;
    
    if (content[0] == '.') {
        return CHTLJSSelectorType::CLASS;
    } else if (content[0] == '#') {
        return CHTLJSSelectorType::ID;
    } else {
        return CHTLJSSelectorType::TAG;
    }
}

// ===== CHTLJSParser 实现 =====

CHTLJSParser::CHTLJSParser() 
    : selectorRegistry_(std::make_shared<CHTLJSSelectorRegistry>())
    , eventDelegateRegistry_(std::make_shared<CHTLJSEventDelegateRegistry>())
    , strictMode_(true) {
}

CHTLJSParser::~CHTLJSParser() = default;

ParseResult CHTLJSParser::parseScriptBlock(const std::string& source, const std::string& filename) {
    errors_.clear();
    warnings_.clear();
    
    ParseResult result;
    result.success = false;
    
    // 词法分析
    auto lexResult = lexer_.tokenize(source, filename);
    if (!lexResult.success) {
        errors_.insert(errors_.end(), lexResult.errors.begin(), lexResult.errors.end());
        result.errors = errors_;
        return result;
    }
    
    try {
        // 解析脚本块定义
        CHTLJSScriptBlock scriptBlock = parseScriptBlockDefinition(lexResult.tokens);
        
        // 注册选择器
        for (const auto& selector : scriptBlock.selectors) {
            if (!selectorRegistry_->registerSelector(selector)) {
                reportWarning("选择器重复定义: " + selector.selector);
            }
        }
        
        // 创建脚本块节点
        auto scriptBlockNode = createScriptBlockNode(scriptBlock);
        
        result.success = errors_.empty();
        result.rootNode = scriptBlockNode;
        result.errors = errors_;
        result.warnings = warnings_;
        
    } catch (const std::exception& e) {
        reportError("脚本块解析失败: " + std::string(e.what()));
        result.success = false;
        result.errors = errors_;
    }
    
    return result;
}

void CHTLJSParser::setSelectorRegistry(std::shared_ptr<CHTLJSSelectorRegistry> registry) {
    selectorRegistry_ = registry;
}

std::shared_ptr<CHTLJSSelectorRegistry> CHTLJSParser::getSelectorRegistry() const {
    return selectorRegistry_;
}

void CHTLJSParser::setEventDelegateRegistry(std::shared_ptr<CHTLJSEventDelegateRegistry> registry) {
    eventDelegateRegistry_ = registry;
}

std::shared_ptr<CHTLJSEventDelegateRegistry> CHTLJSParser::getEventDelegateRegistry() const {
    return eventDelegateRegistry_;
}

void CHTLJSParser::setStrictMode(bool strict) {
    strictMode_ = strict;
}

const std::vector<ParseError>& CHTLJSParser::getErrors() const {
    return errors_;
}

const std::vector<ParseWarning>& CHTLJSParser::getWarnings() const {
    return warnings_;
}

CHTLJSScriptBlock CHTLJSParser::parseScriptBlockDefinition(const std::vector<Token>& tokens) {
    CHTLJSScriptBlock scriptBlock;
    
    // 收集所有tokens为字符串
    std::string content;
    for (const auto& token : tokens) {
        content += token.value;
        if (token.type == TokenType::WHITESPACE || token.type == TokenType::NEWLINE) {
            content += " ";
        }
    }
    
    // 使用扫描器解析内容
    scriptBlock = scanner_.scanScriptBlock(content);
    
    return scriptBlock;
}

std::vector<CHTLJSSelector> CHTLJSParser::parseSelectors(const std::string& content) {
    return scanner_.scanSelectors(content);
}

CHTLJSSelector CHTLJSParser::parseSelector(const std::string& selectorStr) {
    // 使用扫描器的公共方法
    auto selectors = scanner_.scanSelectors("{{" + selectorStr + "}}");
    if (!selectors.empty()) {
        return selectors[0];
    }
    
    // 如果扫描失败，创建一个基本的选择器
    CHTLJSSelector selector;
    selector.selector = selectorStr;
    selector.type = CHTLJSUtils::detectSelectorType(selectorStr);
    return selector;
}

std::vector<CHTLJSListener> CHTLJSParser::parseListeners(const std::string& content) {
    return scanner_.scanListeners(content);
}

CHTLJSListener CHTLJSParser::parseListener(const std::string& listenerCode) {
    // 简单的监听器解析
    CHTLJSListener listener;
    // 这里可以添加更复杂的解析逻辑
    return listener;
}

std::vector<CHTLJSDelegate> CHTLJSParser::parseDelegates(const std::string& content) {
    return scanner_.scanDelegates(content);
}

CHTLJSDelegate CHTLJSParser::parseDelegate(const std::string& delegateCode) {
    // 使用扫描器的公共方法
    auto delegates = scanner_.scanDelegates("->delegate({" + delegateCode + "})");
    if (!delegates.empty()) {
        return delegates[0];
    }
    
    // 如果扫描失败，返回空委托
    return CHTLJSDelegate{};
}

std::vector<CHTLJSAnimation> CHTLJSParser::parseAnimations(const std::string& content) {
    return scanner_.scanAnimations(content);
}

CHTLJSAnimation CHTLJSParser::parseAnimation(const std::string& animationCode) {
    // 使用扫描器的公共方法
    auto animations = scanner_.scanAnimations("animate({" + animationCode + "})");
    if (!animations.empty()) {
        return animations[0];
    }
    
    // 如果扫描失败，返回空动画
    return CHTLJSAnimation{};
}

CHTLASTNodePtr CHTLJSParser::createScriptBlockNode(const CHTLJSScriptBlock& scriptBlock) {
    auto scriptBlockNode = std::make_shared<ScriptBlockNode>(scriptBlock.position);
    scriptBlockNode->content = scriptBlock.rawCode;
    
    return scriptBlockNode;
}

void CHTLJSParser::reportError(const std::string& message, const Position& position) {
    errors_.emplace_back(message, position);
}

void CHTLJSParser::reportWarning(const std::string& message, const Position& position) {
    warnings_.emplace_back(message, position);
}

bool CHTLJSParser::isValidSelectorSyntax(const std::string& selectorStr) const {
    // 使用CHTLJSUtils的公共方法
    return CHTLJSUtils::validateSelectorSyntax(selectorStr);
}

bool CHTLJSParser::isValidMethodCall(const std::string& methodCall) const {
    // 基本的方法调用验证
    std::regex methodRegex(R"(^\w+\s*\()");
    return std::regex_search(methodCall, methodRegex);
}

std::string CHTLJSParser::extractSelectorFromBraces(const std::string& content) const {
    std::regex selectorRegex(R"(\{\{([^}]+)\}\})");
    std::smatch match;
    
    if (std::regex_search(content, match, selectorRegex)) {
        return match[1].str();
    }
    
    return "";
}

std::string CHTLJSParser::getMethodTypeString(CHTLJSMethodType type) const {
    switch (type) {
        case CHTLJSMethodType::STANDARD: return "standard";
        case CHTLJSMethodType::LISTEN: return "listen";
        case CHTLJSMethodType::DELEGATE: return "delegate";
        case CHTLJSMethodType::ANIMATE: return "animate";
    }
    return "standard";
}

std::string CHTLJSParser::getSelectorTypeString(CHTLJSSelectorType type) const {
    switch (type) {
        case CHTLJSSelectorType::TAG: return "tag";
        case CHTLJSSelectorType::CLASS: return "class";
        case CHTLJSSelectorType::ID: return "id";
        case CHTLJSSelectorType::DESCENDANT: return "descendant";
        case CHTLJSSelectorType::INDEXED: return "indexed";
        case CHTLJSSelectorType::AUTO: return "auto";
    }
    return "auto";
}

// ===== CHTLJSUtils 实现 =====

bool CHTLJSUtils::isScriptBlockDeclaration(const std::string& source) {
    std::regex scriptRegex(R"(script\s*\{)", std::regex_constants::icase);
    return std::regex_search(source, scriptRegex);
}

bool CHTLJSUtils::hasEnhancedSelectors(const std::string& source) {
    std::regex selectorRegex(R"(\{\{[^}]+\}\})");
    return std::regex_search(source, selectorRegex);
}

bool CHTLJSUtils::hasCHTLJSArrow(const std::string& source) {
    return source.find("->") != std::string::npos;
}

bool CHTLJSUtils::hasListenCall(const std::string& source) {
    std::regex listenRegex(R"(->listen\s*\()");
    return std::regex_search(source, listenRegex);
}

bool CHTLJSUtils::hasDelegateCall(const std::string& source) {
    std::regex delegateRegex(R"(->delegate\s*\()");
    return std::regex_search(source, delegateRegex);
}

bool CHTLJSUtils::hasAnimateCall(const std::string& source) {
    std::regex animateRegex(R"(animate\s*\()");
    return std::regex_search(source, animateRegex);
}

std::vector<std::string> CHTLJSUtils::extractEnhancedSelectors(const std::string& source) {
    std::vector<std::string> selectors;
    
    std::regex selectorRegex(R"(\{\{([^}]+)\}\})");
    std::sregex_iterator iter(source.begin(), source.end(), selectorRegex);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        std::smatch match = *iter;
        selectors.push_back(match[1].str());
    }
    
    return selectors;
}

bool CHTLJSUtils::validateSelectorSyntax(const std::string& selector) {
    if (selector.empty()) return false;
    
    std::regex selectorRegex(R"(^[\.#]?[\w-]+(\s+[\w-]+)*(\[\d+\])?$)");
    return std::regex_match(selector, selectorRegex);
}

bool CHTLJSUtils::validateEventType(const std::string& eventType) {
    std::unordered_set<std::string> validEvents = {
        "click", "dblclick", "mousedown", "mouseup", "mousemove", "mouseenter", "mouseleave",
        "keydown", "keyup", "keypress", "focus", "blur", "change", "input", "submit",
        "load", "unload", "resize", "scroll", "error"
    };
    
    return validEvents.find(eventType) != validEvents.end();
}

std::string CHTLJSUtils::normalizeSelector(const std::string& selector) {
    // 简单的选择器标准化
    std::string normalized = selector;
    
    // 移除多余空格
    normalized = std::regex_replace(normalized, std::regex(R"(\s+)"), " ");
    
    // 去除首尾空格
    size_t start = normalized.find_first_not_of(" \t");
    if (start == std::string::npos) return "";
    
    size_t end = normalized.find_last_not_of(" \t");
    return normalized.substr(start, end - start + 1);
}

CHTLJSSelectorType CHTLJSUtils::detectSelectorType(const std::string& selector) {
    if (selector.empty()) return CHTLJSSelectorType::AUTO;
    
    // 首先检查是否有空格，这表明是后代选择器
    if (selector.find(' ') != std::string::npos) {
        return CHTLJSSelectorType::DESCENDANT;
    }
    
    // 然后检查是否有索引
    if (selector.find('[') != std::string::npos && selector.find(']') != std::string::npos) {
        return CHTLJSSelectorType::INDEXED;
    }
    
    // 检查首字符
    if (selector[0] == '.') {
        return CHTLJSSelectorType::CLASS;
    } else if (selector[0] == '#') {
        return CHTLJSSelectorType::ID;
    } else {
        return CHTLJSSelectorType::TAG;
    }
}

std::string CHTLJSUtils::formatSelector(const CHTLJSSelector& selector) {
    std::string result = "{{" + selector.selector;
    
    if (selector.index >= 0) {
        result += "[" + std::to_string(selector.index) + "]";
    }
    
    result += "}}";
    return result;
}

std::string CHTLJSUtils::formatListener(const CHTLJSListener& listener) {
    return listener.eventType + ": " + listener.handlerCode;
}

std::string CHTLJSUtils::formatDelegate(const CHTLJSDelegate& delegate) {
    std::string result = "delegate({ ";
    
    if (!delegate.targets.empty()) {
        result += "target: ";
        if (delegate.targets.size() == 1) {
            result += formatSelector(delegate.targets[0]);
        } else {
            result += "[";
            for (size_t i = 0; i < delegate.targets.size(); ++i) {
                if (i > 0) result += ", ";
                result += formatSelector(delegate.targets[i]);
            }
            result += "]";
        }
        result += ", ";
    }
    
    for (size_t i = 0; i < delegate.listeners.size(); ++i) {
        if (i > 0) result += ", ";
        result += formatListener(delegate.listeners[i]);
    }
    
    result += " })";
    return result;
}

std::string CHTLJSUtils::formatAnimation(const CHTLJSAnimation& animation) {
    std::string result = "animate({ ";
    
    result += "duration: " + std::to_string(animation.duration);
    
    if (!animation.easing.empty()) {
        result += ", easing: " + animation.easing;
    }
    
    if (animation.loop != 1) {
        result += ", loop: " + std::to_string(animation.loop);
    }
    
    if (animation.delay > 0) {
        result += ", delay: " + std::to_string(animation.delay);
    }
    
    result += " })";
    return result;
}

} // namespace chtl
#include "compiler/chtljs/chtljs_preprocessor.h"
#include <sstream>
#include <cctype>
#include <algorithm>

namespace chtl {

CHTLJSPreprocessor::CHTLJSPreprocessor() {}
CHTLJSPreprocessor::~CHTLJSPreprocessor() {}

CompilationResult CHTLJSPreprocessor::compile(const CodeFragment& fragment, 
                                            const CompilerOptions& options) {
    CompilationResult result;
    result.success = true;
    
    PreprocessContext ctx;
    ctx.input = fragment.content;
    
    try {
        // 预处理CHTL JS语法
        result.output = preprocess(fragment.content, ctx);
        
        // 添加辅助函数
        if (!ctx.helper_functions.empty()) {
            std::string helpers = "\n// CHTL JS Helper Functions\n";
            for (const auto& func : ctx.helper_functions) {
                helpers += func + "\n\n";
            }
            result.output = helpers + result.output;
        }
        
        // 处理错误和警告
        if (!ctx.errors.empty()) {
            result.success = false;
            result.errors = ctx.errors;
        }
        result.warnings = ctx.warnings;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back(std::string("Preprocessing error: ") + e.what());
    }
    
    return result;
}

std::string CHTLJSPreprocessor::preprocess(const std::string& input, PreprocessContext& ctx) {
    ctx.position = 0;
    ctx.line = 1;
    ctx.column = 1;
    
    std::stringstream output;
    bool need_listen_helper = false;
    bool need_delegate_helper = false;
    bool need_animate_helper = false;
    
    while (ctx.position < input.size()) {
        // 处理空白
        if (std::isspace(input[ctx.position])) {
            if (input[ctx.position] == '\n') {
                ctx.line++;
                ctx.column = 1;
            } else {
                ctx.column++;
            }
            output << input[ctx.position];
            ctx.position++;
            continue;
        }
        
        // 处理注释
        if (lookAhead(ctx, "//")) {
            // 单行注释
            while (ctx.position < input.size() && input[ctx.position] != '\n') {
                output << input[ctx.position++];
            }
            continue;
        }
        
        if (lookAhead(ctx, "/*")) {
            // 多行注释
            output << "/*";
            advance(ctx, 2);
            while (ctx.position < input.size() - 1) {
                if (input[ctx.position] == '*' && input[ctx.position + 1] == '/') {
                    output << "*/";
                    advance(ctx, 2);
                    break;
                }
                if (input[ctx.position] == '\n') {
                    ctx.line++;
                    ctx.column = 1;
                }
                output << input[ctx.position];
                advance(ctx);
            }
            continue;
        }
        
        // 处理字符串
        if (input[ctx.position] == '"' || input[ctx.position] == '\'') {
            char quote = input[ctx.position];
            output << quote;
            advance(ctx);
            
            while (ctx.position < input.size()) {
                if (input[ctx.position] == quote) {
                    output << quote;
                    advance(ctx);
                    break;
                }
                if (input[ctx.position] == '\\' && ctx.position + 1 < input.size()) {
                    output << input[ctx.position] << input[ctx.position + 1];
                    advance(ctx, 2);
                    continue;
                }
                output << input[ctx.position];
                advance(ctx);
            }
            continue;
        }
        
        // 处理CHTL JS特有语法
        
        // 1. 处理选择器 {{...}}
        if (lookAhead(ctx, "{{")) {
            std::string selector = readSelector(ctx);
            output << generateSelectorCode(selector);
            continue;
        }
        
        // 2. 处理箭头操作符 ->
        if (lookAhead(ctx, "->")) {
            output << ".";  // 简单地替换为点号
            advance(ctx, 2);
            continue;
        }
        
        // 3. 处理vir关键字
        if (lookAhead(ctx, "vir ")) {
            processVirDeclaration(ctx);
            // vir声明被转换为注释，实际函数在使用时生成
            output << "// vir declaration processed";
            continue;
        }
        
        // 4. 处理特殊函数
        if (lookAhead(ctx, "listen(")) {
            need_listen_helper = true;
            output << "_chtljs_listen(";
            advance(ctx, 7); // 跳过 "listen("
            continue;
        }
        
        if (lookAhead(ctx, "delegate(")) {
            need_delegate_helper = true;
            output << "_chtljs_delegate(";
            advance(ctx, 9); // 跳过 "delegate("
            continue;
        }
        
        if (lookAhead(ctx, "animate(")) {
            need_animate_helper = true;
            output << "_chtljs_animate(";
            advance(ctx, 8); // 跳过 "animate("
            continue;
        }
        
        // 默认：原样输出
        output << input[ctx.position];
        advance(ctx);
    }
    
    // 添加需要的辅助函数
    if (need_listen_helper) {
        ctx.helper_functions.push_back(generateListenHelper());
    }
    if (need_delegate_helper) {
        ctx.helper_functions.push_back(generateDelegateHelper());
    }
    if (need_animate_helper) {
        ctx.helper_functions.push_back(generateAnimateHelper());
    }
    
    return output.str();
}

void CHTLJSPreprocessor::processVirDeclaration(PreprocessContext& ctx) {
    advance(ctx, 4); // 跳过 "vir "
    skipWhitespace(ctx);
    
    // 读取虚对象名称
    std::string virName = readIdentifier(ctx);
    if (virName.empty()) {
        reportError(ctx, "Expected identifier after 'vir'");
        return;
    }
    
    skipWhitespace(ctx);
    
    // 期待 =
    if (ctx.position >= ctx.input.size() || ctx.input[ctx.position] != '=') {
        reportError(ctx, "Expected '=' after vir identifier");
        return;
    }
    advance(ctx);
    skipWhitespace(ctx);
    
    // 分析右侧的对象，提取函数键
    // 这里简化处理，实际应该更复杂
    // 跳过整个赋值语句
    int braceDepth = 0;
    bool inString = false;
    char stringChar = 0;
    
    while (ctx.position < ctx.input.size()) {
        char ch = ctx.input[ctx.position];
        
        if (!inString) {
            if (ch == '"' || ch == '\'') {
                inString = true;
                stringChar = ch;
            } else if (ch == '{') {
                braceDepth++;
            } else if (ch == '}') {
                braceDepth--;
                if (braceDepth == 0) {
                    advance(ctx);
                    break;
                }
            } else if (ch == ';' && braceDepth == 0) {
                advance(ctx);
                break;
            }
        } else {
            if (ch == stringChar && ctx.position > 0 && ctx.input[ctx.position-1] != '\\') {
                inString = false;
            }
        }
        
        advance(ctx);
    }
    
    // 记录虚对象（简化版本）
    ctx.vir_objects[virName] = std::vector<std::string>();
}

std::string CHTLJSPreprocessor::readSelector(PreprocessContext& ctx) {
    advance(ctx, 2); // 跳过 {{
    
    std::string selector;
    while (ctx.position < ctx.input.size() - 1) {
        if (ctx.input[ctx.position] == '}' && ctx.input[ctx.position + 1] == '}') {
            advance(ctx, 2); // 跳过 }}
            break;
        }
        selector += ctx.input[ctx.position];
        advance(ctx);
    }
    
    return selector;
}

std::string CHTLJSPreprocessor::generateSelectorCode(const std::string& selector) {
    std::string trimmed = selector;
    trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));
    trimmed.erase(trimmed.find_last_not_of(" \t\n\r") + 1);
    
    if (trimmed.empty()) {
        return "document.querySelector('')";
    }
    
    // 处理特殊选择器语法
    if (trimmed[0] == '.') {
        // 类选择器
        return "document.querySelectorAll('" + trimmed + "')";
    } else if (trimmed[0] == '#') {
        // ID选择器
        return "document.querySelector('" + trimmed + "')";
    } else if (trimmed.find('[') != std::string::npos && trimmed.back() == ']') {
        // 数组索引访问
        size_t bracket = trimmed.find('[');
        std::string base = trimmed.substr(0, bracket);
        std::string index = trimmed.substr(bracket + 1, trimmed.length() - bracket - 2);
        
        if (base[0] == '.' || base[0] == '#') {
            return "document.querySelectorAll('" + base + "')[" + index + "]";
        } else {
            return "document.querySelectorAll('" + base + "')[" + index + "]";
        }
    } else {
        // 标签选择器或复合选择器
        return "document.querySelectorAll('" + trimmed + "')";
    }
}

std::string CHTLJSPreprocessor::generateListenHelper() {
    return R"(function _chtljs_listen(element, events) {
    if (!element) return;
    
    const elements = element.length !== undefined ? element : [element];
    
    Object.keys(events).forEach(eventName => {
        const handler = events[eventName];
        if (typeof handler === 'function') {
            elements.forEach(el => {
                el.addEventListener(eventName, handler);
            });
        }
    });
})";
}

std::string CHTLJSPreprocessor::generateDelegateHelper() {
    return R"(function _chtljs_delegate(parent, config) {
    if (!parent || !config.target) return;
    
    const targets = Array.isArray(config.target) ? config.target : [config.target];
    
    Object.keys(config).forEach(key => {
        if (key === 'target') return;
        
        const handler = config[key];
        if (typeof handler === 'function') {
            parent.addEventListener(key, function(event) {
                targets.forEach(target => {
                    const selector = typeof target === 'string' ? target : 
                                   target.length ? Array.from(target).map(el => 
                                   el.tagName.toLowerCase() + (el.id ? '#' + el.id : '') + 
                                   (el.className ? '.' + el.className.replace(/\s+/g, '.') : '')
                                   ).join(', ') : '';
                    
                    if (event.target.matches(selector)) {
                        handler.call(event.target, event);
                    }
                });
            });
        }
    });
})";
}

std::string CHTLJSPreprocessor::generateAnimateHelper() {
    return R"(function _chtljs_animate(config) {
    const targets = Array.isArray(config.target) ? config.target : [config.target];
    const duration = config.duration || 1000;
    const easing = config.easing || 'linear';
    const loop = config.loop || 1;
    const direction = config.direction || 'normal';
    const delay = config.delay || 0;
    
    const animations = [];
    
    targets.forEach(target => {
        if (!target) return;
        
        const keyframes = [];
        
        if (config.begin) keyframes.push(config.begin);
        if (config.when) {
            config.when.forEach(frame => {
                const offset = frame.at;
                delete frame.at;
                keyframes.push({...frame, offset});
            });
        }
        if (config.end) keyframes.push({...config.end, offset: 1});
        
        const animation = target.animate(keyframes, {
            duration,
            easing,
            iterations: loop === -1 ? Infinity : loop,
            direction,
            delay
        });
        
        if (config.callback) {
            animation.onfinish = config.callback;
        }
        
        animations.push(animation);
    });
    
    return animations;
})";
}

// 辅助函数实现
bool CHTLJSPreprocessor::lookAhead(const PreprocessContext& ctx, const std::string& text) const {
    if (ctx.position + text.length() > ctx.input.size()) {
        return false;
    }
    return ctx.input.substr(ctx.position, text.length()) == text;
}

void CHTLJSPreprocessor::advance(PreprocessContext& ctx, size_t count) {
    for (size_t i = 0; i < count && ctx.position < ctx.input.size(); ++i) {
        if (ctx.input[ctx.position] == '\n') {
            ctx.line++;
            ctx.column = 1;
        } else {
            ctx.column++;
        }
        ctx.position++;
    }
}

void CHTLJSPreprocessor::skipWhitespace(PreprocessContext& ctx) {
    while (ctx.position < ctx.input.size() && std::isspace(ctx.input[ctx.position])) {
        advance(ctx);
    }
}

std::string CHTLJSPreprocessor::readIdentifier(PreprocessContext& ctx) {
    std::string id;
    
    if (ctx.position < ctx.input.size() && 
        (std::isalpha(ctx.input[ctx.position]) || ctx.input[ctx.position] == '_')) {
        
        while (ctx.position < ctx.input.size() && 
               (std::isalnum(ctx.input[ctx.position]) || ctx.input[ctx.position] == '_')) {
            id += ctx.input[ctx.position];
            advance(ctx);
        }
    }
    
    return id;
}

void CHTLJSPreprocessor::reportError(PreprocessContext& ctx, const std::string& message) {
    std::stringstream ss;
    ss << "Line " << ctx.line << ", column " << ctx.column << ": " << message;
    ctx.errors.push_back(ss.str());
}

void CHTLJSPreprocessor::reportWarning(PreprocessContext& ctx, const std::string& message) {
    std::stringstream ss;
    ss << "Line " << ctx.line << ", column " << ctx.column << ": " << message;
    ctx.warnings.push_back(ss.str());
}

} // namespace chtl
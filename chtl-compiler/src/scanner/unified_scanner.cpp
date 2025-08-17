#include "scanner/unified_scanner.h"
#include <unordered_set>
#include <algorithm>
#include <cctype>
#include <sstream>

namespace chtl {

// 静态成员定义
const std::unordered_set<std::string> CHTLUnifiedScanner::keywords_ = {
    "style", "script", "text", "inherit", "delete", "insert",
    "after", "before", "replace", "at", "top", "bottom",
    "from", "as", "except", "vir"
};

const std::unordered_set<std::string> CHTLUnifiedScanner::html_elements_ = {
    // 常见HTML元素
    "html", "head", "body", "div", "span", "p", "a", "img",
    "ul", "ol", "li", "table", "tr", "td", "th", "form",
    "input", "button", "select", "option", "textarea",
    "h1", "h2", "h3", "h4", "h5", "h6", "header", "footer",
    "nav", "section", "article", "aside", "main", "figure",
    "figcaption", "video", "audio", "canvas", "svg", "iframe"
};

CHTLUnifiedScanner::CHTLUnifiedScanner(const ScannerConfig& config)
    : config_(config) {
}

CHTLUnifiedScanner::~CHTLUnifiedScanner() = default;

std::vector<CodeFragment> CHTLUnifiedScanner::scan(const std::string& input, 
                                                   const std::string& filename) {
    std::vector<CodeFragment> fragments;
    errors_.clear();
    
    ScanContext ctx;
    ctx.input = input;
    ctx.position = 0;
    ctx.line = 1;
    ctx.column = 1;
    ctx.filename = filename;
    ctx.state = ScannerState::NORMAL;
    ctx.current_fragment_type = CodeFragmentType::CHTL;
    ctx.fragment_start_line = 1;
    ctx.brace_depth = 0;
    ctx.style_depth = 0;
    ctx.script_depth = 0;
    ctx.string_delimiter = '\0';
    ctx.in_template_string = false;
    
    // 开始扫描
    while (ctx.position < ctx.input.length()) {
        switch (ctx.state) {
            case ScannerState::NORMAL:
                scanCHTL(ctx, fragments);
                break;
            case ScannerState::IN_STYLE_BLOCK:
                scanStyleBlock(ctx, fragments);
                break;
            case ScannerState::IN_SCRIPT_BLOCK:
                scanScriptBlock(ctx, fragments);
                break;
            case ScannerState::IN_TEXT_BLOCK:
                scanTextBlock(ctx, fragments);
                break;
            case ScannerState::IN_ORIGIN_BLOCK:
                scanOriginBlock(ctx, fragments);
                break;
            default:
                advance(ctx);
                break;
        }
    }
    
    // 完成最后一个片段
    if (!ctx.current_content.empty()) {
        finishFragment(ctx, fragments);
    }
    
    return fragments;
}

void CHTLUnifiedScanner::scanCHTL(ScanContext& ctx, std::vector<CodeFragment>& fragments) {
    while (ctx.position < ctx.input.length()) {
        // 跳过空白和注释
        if (std::isspace(peek(ctx))) {
            if (peek(ctx) == '\n') {
                newLine(ctx);
            }
            ctx.current_content += peek(ctx);
            advance(ctx);
            continue;
        }
        
        // 处理注释
        if (matchSequence(ctx, "//")) {
            skipComment(ctx);
            continue;
        }
        
        if (matchSequence(ctx, "/*")) {
            skipComment(ctx);
            continue;
        }
        
        // 处理特殊标记
        if (peek(ctx) == '[') {
            if (matchSequence(ctx, "[Origin]")) {
                // [Origin]块需要特殊处理
                ctx.state = ScannerState::IN_ORIGIN_BLOCK;
                return;
            }
            else if (matchSequence(ctx, "[Template]") || 
                     matchSequence(ctx, "[Custom]")) {
                // 这些仍然是CHTL的一部分
                ctx.current_content += peek(ctx);
                advance(ctx);
                continue;
            }
        }
        
        // 扫描标识符
        if (std::isalpha(peek(ctx)) || peek(ctx) == '_') {
            std::string id = scanIdentifier(ctx);
            
                            // 检查是否是关键字
            if (id == "style" && ctx.brace_depth > 0) {
                // 检查后面是否有 {
                size_t saved_pos = ctx.position;
                skipWhitespace(ctx);
                if (peek(ctx) == '{') {
                    // 进入局部style块，但不创建新片段
                    ctx.state = ScannerState::IN_STYLE_BLOCK;
                    ctx.style_depth = ctx.brace_depth;
                    ctx.current_content += id;
                    return;
                } else {
                    ctx.position = saved_pos;
                }
            }
            else if (id == "script" && ctx.brace_depth > 0) {
                // 检查后面是否有 {
                size_t saved_pos = ctx.position;
                skipWhitespace(ctx);
                if (peek(ctx) == '{') {
                    // 进入局部script块，但不创建新片段
                    ctx.state = ScannerState::IN_SCRIPT_BLOCK;
                    ctx.script_depth = ctx.brace_depth;
                    ctx.current_content += id;
                    return;
                } else {
                    ctx.position = saved_pos;
                }
            }
            else if (id == "text") {
                // 检查后面是否有 {
                size_t saved_pos = ctx.position;
                skipWhitespace(ctx);
                if (peek(ctx) == '{') {
                    ctx.state = ScannerState::IN_TEXT_BLOCK;
                    ctx.current_content += id;
                } else {
                    ctx.position = saved_pos;
                    ctx.current_content += id;
                }
            }
            else {
                ctx.current_content += id;
            }
            continue;
        }
        
        // 处理大括号
        if (peek(ctx) == '{') {
            ctx.brace_depth++;
            ctx.current_content += peek(ctx);
            advance(ctx);
            continue;
        }
        
        if (peek(ctx) == '}') {
            ctx.brace_depth--;
            ctx.current_content += peek(ctx);
            advance(ctx);
            continue;
        }
        
        // 处理字符串
        if (peek(ctx) == '"' || peek(ctx) == '\'') {
            ctx.current_content += scanString(ctx);
            continue;
        }
        
        // 其他字符
        ctx.current_content += peek(ctx);
        advance(ctx);
    }
}

void CHTLUnifiedScanner::scanStyleBlock(ScanContext& ctx, std::vector<CodeFragment>& fragments) {
    // style块的内容需要被识别为CHTL代码（局部样式块）
    ctx.current_content += "\n";
    skipWhitespace(ctx);
    
    if (peek(ctx) != '{') {
        reportError(ctx, "Expected '{' after 'style'");
        ctx.state = ScannerState::NORMAL;
        return;
    }
    
    ctx.current_content += peek(ctx);
    advance(ctx); // 消费 '{'
    
    int local_brace_depth = 1;
    
    while (ctx.position < ctx.input.length() && local_brace_depth > 0) {
        if (peek(ctx) == '{') {
            local_brace_depth++;
        } else if (peek(ctx) == '}') {
            local_brace_depth--;
            if (local_brace_depth == 0) {
                ctx.current_content += peek(ctx);
                advance(ctx);
                break;
            }
        } else if (peek(ctx) == '\n') {
            newLine(ctx);
        }
        
        // 处理字符串，避免大括号被误识别
        if (peek(ctx) == '"' || peek(ctx) == '\'') {
            ctx.current_content += scanString(ctx);
            continue;
        }
        
        ctx.current_content += peek(ctx);
        advance(ctx);
    }
    
    // 完成style片段，回到NORMAL状态
    ctx.state = ScannerState::NORMAL;
}

void CHTLUnifiedScanner::scanScriptBlock(ScanContext& ctx, std::vector<CodeFragment>& fragments) {
    // script块内容作为CHTL JS处理
    ctx.current_content += "\n";
    skipWhitespace(ctx);
    
    if (peek(ctx) != '{') {
        reportError(ctx, "Expected '{' after 'script'");
        ctx.state = ScannerState::NORMAL;
        return;
    }
    
    ctx.current_content += peek(ctx);
    advance(ctx); // 消费 '{'
    
    int local_brace_depth = 1;
    
    while (ctx.position < ctx.input.length() && local_brace_depth > 0) {
        // 处理增强选择器 {{}}
        if (matchSequence(ctx, "{{")) {
            ctx.current_content += "{{";
            advance(ctx, 2);
            // 扫描到 }}
            while (ctx.position < ctx.input.length() && !matchSequence(ctx, "}}")) {
                if (peek(ctx) == '\n') {
                    newLine(ctx);
                }
                ctx.current_content += peek(ctx);
                advance(ctx);
            }
            if (matchSequence(ctx, "}}")) {
                ctx.current_content += "}}";
                advance(ctx, 2);
            }
            continue;
        }
        
        if (peek(ctx) == '{') {
            local_brace_depth++;
        } else if (peek(ctx) == '}') {
            local_brace_depth--;
            if (local_brace_depth == 0) {
                ctx.current_content += peek(ctx);
                advance(ctx);
                break;
            }
        } else if (peek(ctx) == '\n') {
            newLine(ctx);
        }
        
        // 处理字符串
        if (peek(ctx) == '"' || peek(ctx) == '\'' || peek(ctx) == '`') {
            ctx.current_content += scanString(ctx);
            continue;
        }
        
        ctx.current_content += peek(ctx);
        advance(ctx);
    }
    
    // 完成script片段，回到NORMAL状态
    ctx.state = ScannerState::NORMAL;
}

void CHTLUnifiedScanner::scanTextBlock(ScanContext& ctx, std::vector<CodeFragment>& fragments) {
    // text块仍然是CHTL的一部分
    skipWhitespace(ctx);
    
    if (peek(ctx) != '{') {
        reportError(ctx, "Expected '{' after 'text'");
        ctx.state = ScannerState::NORMAL;
        return;
    }
    
    ctx.current_content += peek(ctx);
    advance(ctx); // 消费 '{'
    
    int local_brace_depth = 1;
    
    while (ctx.position < ctx.input.length() && local_brace_depth > 0) {
        if (peek(ctx) == '{') {
            local_brace_depth++;
        } else if (peek(ctx) == '}') {
            local_brace_depth--;
        } else if (peek(ctx) == '\n') {
            newLine(ctx);
        }
        
        ctx.current_content += peek(ctx);
        advance(ctx);
    }
    
    ctx.state = ScannerState::NORMAL;
}

void CHTLUnifiedScanner::scanOriginBlock(ScanContext& ctx, std::vector<CodeFragment>& fragments) {
    // 先完成当前的CHTL片段
    if (!ctx.current_content.empty()) {
        finishFragment(ctx, fragments);
    }
    
    // [Origin]块需要特殊处理
    ctx.current_content = "[Origin]";
    advance(ctx, 8); // 跳过 "[Origin]"
    
    skipWhitespace(ctx);
    
    // 读取类型 @Html, @Style, @JavaScript等
    if (peek(ctx) != '@') {
        reportError(ctx, "Expected type after [Origin]");
        ctx.state = ScannerState::NORMAL;
        return;
    }
    
    advance(ctx); // 跳过 '@'
    std::string originType = scanIdentifier(ctx);
    
    // 可能有名称
    skipWhitespace(ctx);
    std::string originName;
    if (std::isalpha(peek(ctx)) || peek(ctx) == '_') {
        originName = scanIdentifier(ctx);
    }
    
    skipWhitespace(ctx);
    
    if (peek(ctx) != '{') {
        reportError(ctx, "Expected '{' in [Origin] block");
        ctx.state = ScannerState::NORMAL;
        return;
    }
    
    advance(ctx); // 消费 '{'
    
    // 根据类型确定片段类型
    CodeFragmentType fragType = CodeFragmentType::HTML_RAW;
    if (originType == "Style") {
        fragType = CodeFragmentType::CSS;
    } else if (originType == "JavaScript") {
        fragType = CodeFragmentType::JAVASCRIPT;
    }
    
    // 读取原始内容直到 '}'
    int local_brace_depth = 1;
    std::string originContent;
    size_t originStartLine = ctx.line;
    
    while (ctx.position < ctx.input.length() && local_brace_depth > 0) {
        if (peek(ctx) == '{') {
            local_brace_depth++;
        } else if (peek(ctx) == '}') {
            local_brace_depth--;
            if (local_brace_depth == 0) {
                break;
            }
        } else if (peek(ctx) == '\n') {
            newLine(ctx);
        }
        
        originContent += peek(ctx);
        advance(ctx);
    }
    
    if (peek(ctx) == '}') {
        advance(ctx); // 消费 '}'
    }
    
    // 创建Origin内容的片段
    if (!originContent.empty()) {
        CodeFragment originFrag;
        originFrag.type = fragType;
        originFrag.content = originContent;
        originFrag.start_line = originStartLine;
        originFrag.end_line = ctx.line;
        originFrag.source_file = ctx.filename;
        fragments.push_back(originFrag);
    }
    
    // 开始新的CHTL片段
    startNewFragment(ctx, CodeFragmentType::CHTL);
    ctx.state = ScannerState::NORMAL;
}

void CHTLUnifiedScanner::skipWhitespace(ScanContext& ctx) {
    while (ctx.position < ctx.input.length() && std::isspace(peek(ctx))) {
        if (peek(ctx) == '\n') {
            newLine(ctx);
        } else {
            ctx.column++;
        }
        if (config_.preserve_whitespace) {
            ctx.current_content += peek(ctx);
        }
        ctx.position++;
    }
}

void CHTLUnifiedScanner::skipComment(ScanContext& ctx) {
    if (matchSequence(ctx, "//")) {
        if (config_.preserve_comments) {
            ctx.current_content += "//";
        }
        advance(ctx, 2);
        
        while (ctx.position < ctx.input.length() && peek(ctx) != '\n') {
            if (config_.preserve_comments) {
                ctx.current_content += peek(ctx);
            }
            advance(ctx);
        }
    }
    else if (matchSequence(ctx, "/*")) {
        if (config_.preserve_comments) {
            ctx.current_content += "/*";
        }
        advance(ctx, 2);
        
        while (ctx.position < ctx.input.length()) {
            if (matchSequence(ctx, "*/")) {
                if (config_.preserve_comments) {
                    ctx.current_content += "*/";
                }
                advance(ctx, 2);
                break;
            }
            if (peek(ctx) == '\n') {
                newLine(ctx);
            }
            if (config_.preserve_comments) {
                ctx.current_content += peek(ctx);
            }
            advance(ctx);
        }
    }
}

std::string CHTLUnifiedScanner::scanIdentifier(ScanContext& ctx) {
    std::string result;
    
    while (ctx.position < ctx.input.length() && 
           (std::isalnum(peek(ctx)) || peek(ctx) == '_')) {
        result += peek(ctx);
        advance(ctx);
    }
    
    return result;
}

std::string CHTLUnifiedScanner::scanString(ScanContext& ctx) {
    std::string result;
    char delimiter = peek(ctx);
    result += delimiter;
    advance(ctx);
    
    bool escaped = false;
    while (ctx.position < ctx.input.length()) {
        char ch = peek(ctx);
        
        if (escaped) {
            result += ch;
            escaped = false;
            advance(ctx);
            continue;
        }
        
        if (ch == '\\') {
            escaped = true;
            result += ch;
            advance(ctx);
            continue;
        }
        
        if (ch == delimiter) {
            result += ch;
            advance(ctx);
            break;
        }
        
        if (ch == '\n') {
            newLine(ctx);
        }
        
        result += ch;
        advance(ctx);
    }
    
    return result;
}

void CHTLUnifiedScanner::finishFragment(ScanContext& ctx, std::vector<CodeFragment>& fragments) {
    if (ctx.current_content.empty()) {
        return;
    }
    
    CodeFragment fragment;
    fragment.type = ctx.current_fragment_type;
    fragment.content = ctx.current_content;
    fragment.start_line = ctx.fragment_start_line;
    fragment.end_line = ctx.line;
    fragment.source_file = ctx.filename;
    
    fragments.push_back(fragment);
    
    ctx.current_content.clear();
    ctx.fragment_start_line = ctx.line;
}

void CHTLUnifiedScanner::startNewFragment(ScanContext& ctx, CodeFragmentType type) {
    ctx.current_fragment_type = type;
    ctx.current_content.clear();
    ctx.fragment_start_line = ctx.line;
}

bool CHTLUnifiedScanner::isKeyword(const std::string& word) const {
    return keywords_.find(word) != keywords_.end();
}

bool CHTLUnifiedScanner::matchSequence(const ScanContext& ctx, const std::string& seq) const {
    if (ctx.position + seq.length() > ctx.input.length()) {
        return false;
    }
    
    for (size_t i = 0; i < seq.length(); ++i) {
        if (ctx.input[ctx.position + i] != seq[i]) {
            return false;
        }
    }
    
    return true;
}

char CHTLUnifiedScanner::peek(const ScanContext& ctx, size_t offset) const {
    size_t pos = ctx.position + offset;
    if (pos >= ctx.input.length()) {
        return '\0';
    }
    return ctx.input[pos];
}

void CHTLUnifiedScanner::advance(ScanContext& ctx, size_t count) {
    for (size_t i = 0; i < count && ctx.position < ctx.input.length(); ++i) {
        if (ctx.input[ctx.position] == '\n') {
            ctx.line++;
            ctx.column = 1;
        } else {
            ctx.column++;
        }
        ctx.position++;
    }
}

void CHTLUnifiedScanner::newLine(ScanContext& ctx) {
    ctx.line++;
    ctx.column = 1;
}

void CHTLUnifiedScanner::reportError(const ScanContext& ctx, const std::string& message) {
    CompilerError error;
    error.severity = CompilerError::ERROR;
    error.message = message;
    error.location.line = ctx.line;
    error.location.column = ctx.column;
    error.location.file = ctx.filename;
    
    errors_.push_back(error);
    
    if (error_handler_) {
        error_handler_(error);
    }
}

void CHTLUnifiedScanner::reportWarning(const ScanContext& ctx, const std::string& message) {
    CompilerError warning;
    warning.severity = CompilerError::WARNING;
    warning.message = message;
    warning.location.line = ctx.line;
    warning.location.column = ctx.column;
    warning.location.file = ctx.filename;
    
    errors_.push_back(warning);
    
    if (error_handler_) {
        error_handler_(warning);
    }
}

} // namespace chtl
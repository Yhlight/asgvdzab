#include "core/generator_comment_system.hpp"
#include <algorithm>
#include <sstream>
#include <iostream>

namespace chtl {

// =============================================================================
// CommentContextAnalyzer Implementation
// =============================================================================

CommentContextAnalyzer::CommentContextAnalyzer() {
    initializePatterns();
}

void CommentContextAnalyzer::initializePatterns() {
    // HTML上下文模式
    htmlPatterns_ = {
        std::regex(R"(\b(?:html|head|body|div|span|p|a|img|ul|li|table|tr|td|th|form|input|button|h[1-6]|section|article|nav|header|footer|aside|main)\s*\{)"),
        std::regex(R"(\btext\s*\{)"),
        std::regex(R"(\w+\s*:\s*[^;]+;)"),  // 属性语法
        std::regex(R"(\[Origin\]\s*@Html)"),
    };
    
    // CSS上下文模式
    cssPatterns_ = {
        std::regex(R"(\bstyle\s*\{)"),  // 局部样式块
        std::regex(R"(\.[a-zA-Z_][\w-]*\s*\{)"),  // 类选择器
        std::regex(R"(#[a-zA-Z_][\w-]*\s*\{)"),   // ID选择器
        std::regex(R"(&:(?:hover|focus|active|visited))"), // 伪类
        std::regex(R"(\[Origin\]\s*@Style)"),
        std::regex(R"([\w-]+\s*:\s*[^;]+;)"),  // CSS属性
        std::regex(R"(@(?:media|import|keyframes|charset|supports|page))"), // CSS @规则
    };
    
    // JavaScript上下文模式
    jsPatterns_ = {
        std::regex(R"(\bscript\s*\{)"),  // 局部脚本块
        std::regex(R"(\{\{[^}]+\}\})"),  // CHTL JS增强选择器
        std::regex(R"(\b(?:function|var|let|const|if|else|for|while|return|class|new|this|typeof|instanceof)\b)"),
        std::regex(R"(\[Origin\]\s*@JavaScript)"),
        std::regex(R"(\w+\s*=\s*\([^)]*\)\s*=>)"), // 箭头函数
        std::regex(R"(\w+\.\w+\s*\()"), // 方法调用
        std::regex(R"(->(?:listen|delegate|animate))"), // CHTL JS特有语法
    };
}

CommentLanguageContext CommentContextAnalyzer::analyzeContext(
    const std::string& sourceCode, 
    const CommentPosition& commentPos
) {
    // 获取注释周围的上下文
    std::string context = getContextString(sourceCode, commentPos, 200);
    
    // 优先检查具体的代码块
    if (isInBlock(sourceCode, commentPos, "style {")) {
        return CommentLanguageContext::CSS_CONTEXT;
    }
    
    if (isInBlock(sourceCode, commentPos, "script {")) {
        return CommentLanguageContext::JAVASCRIPT_CONTEXT;
    }
    
    // 检查原始嵌入块
    if (context.find("[Origin] @Style") != std::string::npos) {
        return CommentLanguageContext::CSS_CONTEXT;
    }
    
    if (context.find("[Origin] @JavaScript") != std::string::npos) {
        return CommentLanguageContext::JAVASCRIPT_CONTEXT;
    }
    
    if (context.find("[Origin] @Html") != std::string::npos) {
        return CommentLanguageContext::HTML_CONTEXT;
    }
    
    // 基于模式匹配判断
    if (matchesPattern(context, jsPatterns_)) {
        return CommentLanguageContext::JAVASCRIPT_CONTEXT;
    }
    
    if (matchesPattern(context, cssPatterns_)) {
        return CommentLanguageContext::CSS_CONTEXT;
    }
    
    if (matchesPattern(context, htmlPatterns_)) {
        return CommentLanguageContext::HTML_CONTEXT;
    }
    
    // 默认为HTML上下文（因为CHTL主要生成HTML）
    return CommentLanguageContext::HTML_CONTEXT;
}

bool CommentContextAnalyzer::isInBlock(
    const std::string& sourceCode,
    const CommentPosition& commentPos,
    const std::string& blockPattern
) {
    // 从注释位置向前搜索最近的块开始
    size_t searchStart = (commentPos.startPos > 1000) ? commentPos.startPos - 1000 : 0;
    std::string searchArea = sourceCode.substr(searchStart, commentPos.startPos - searchStart + 200);
    
    // 查找块开始模式
    size_t blockStart = searchArea.rfind(blockPattern);
    if (blockStart == std::string::npos) {
        return false;
    }
    
    // 查找对应的块结束
    size_t braceCount = 0;
    bool inBlock = false;
    
    for (size_t i = blockStart + blockPattern.length(); i < searchArea.length(); ++i) {
        char ch = searchArea[i];
        
        if (ch == '{') {
            if (!inBlock) {
                inBlock = true;
            }
            braceCount++;
        } else if (ch == '}') {
            braceCount--;
            if (braceCount == 0) {
                // 检查注释是否在这个块内
                size_t absoluteBlockStart = searchStart + blockStart;
                size_t absoluteBlockEnd = searchStart + i;
                return (commentPos.startPos > absoluteBlockStart && commentPos.startPos < absoluteBlockEnd);
            }
        }
    }
    
    return inBlock; // 如果还在块内且没有找到结束
}

std::string CommentContextAnalyzer::getContextString(
    const std::string& sourceCode,
    const CommentPosition& commentPos,
    size_t contextRadius
) {
    size_t start = (commentPos.startPos > contextRadius) ? commentPos.startPos - contextRadius : 0;
    size_t end = std::min(commentPos.endPos + contextRadius, sourceCode.length());
    
    return sourceCode.substr(start, end - start);
}

bool CommentContextAnalyzer::matchesPattern(
    const std::string& context, 
    const std::vector<std::regex>& patterns
) {
    for (const auto& pattern : patterns) {
        if (std::regex_search(context, pattern)) {
            return true;
        }
    }
    return false;
}

// =============================================================================
// CommentGeneratorStateMachine Implementation
// =============================================================================

CommentGeneratorStateMachine::CommentGeneratorStateMachine() 
    : currentState_(State::PARSING_NORMAL)
    , previousState_(State::PARSING_NORMAL)
    , currentPosition_(0)
    , braceLevel_(0)
    , blockLevel_(0)
{
}

void CommentGeneratorStateMachine::processCharacter(char ch, size_t position) {
    currentPosition_ = position;
    previousState_ = currentState_;
    
    updateStateBasedOnContext(ch, position);
}

CommentLanguageContext CommentGeneratorStateMachine::getCurrentLanguageContext() const {
    switch (currentState_) {
        case State::IN_LOCAL_STYLE:
        case State::IN_GLOBAL_STYLE:
            return CommentLanguageContext::CSS_CONTEXT;
            
        case State::IN_LOCAL_SCRIPT:
            return CommentLanguageContext::JAVASCRIPT_CONTEXT;
            
        case State::IN_CHTL_ELEMENT:
        case State::PARSING_NORMAL:
            return CommentLanguageContext::HTML_CONTEXT;
            
        case State::IN_ORIGIN_BLOCK:
            // 需要进一步分析原始嵌入的类型
            if (sourceCode_.find("@Style") != std::string::npos) {
                return CommentLanguageContext::CSS_CONTEXT;
            } else if (sourceCode_.find("@JavaScript") != std::string::npos) {
                return CommentLanguageContext::JAVASCRIPT_CONTEXT;
            } else {
                return CommentLanguageContext::HTML_CONTEXT;
            }
            
        default:
            return CommentLanguageContext::HTML_CONTEXT;
    }
}

void CommentGeneratorStateMachine::reset() {
    currentState_ = State::PARSING_NORMAL;
    previousState_ = State::PARSING_NORMAL;
    currentPosition_ = 0;
    braceLevel_ = 0;
    blockLevel_ = 0;
}

void CommentGeneratorStateMachine::setSourceCode(const std::string& sourceCode) {
    sourceCode_ = sourceCode;
}

void CommentGeneratorStateMachine::updateStateBasedOnContext(char ch, size_t position) {
    // 简化的状态机逻辑
    if (ch == '{') {
        braceLevel_++;
        
        if (isEnteringStyleBlock(position)) {
            currentState_ = State::IN_LOCAL_STYLE;
        } else if (isEnteringScriptBlock(position)) {
            currentState_ = State::IN_LOCAL_SCRIPT;
        }
    } else if (ch == '}') {
        braceLevel_--;
        
        if (isLeavingBlock(ch)) {
            currentState_ = State::PARSING_NORMAL;
        }
    }
}

bool CommentGeneratorStateMachine::isEnteringStyleBlock(size_t position) {
    if (position < 10) return false;
    
    size_t start = (position > 20) ? position - 20 : 0;
    std::string context = sourceCode_.substr(start, position - start);
    
    return context.find("style") != std::string::npos;
}

bool CommentGeneratorStateMachine::isEnteringScriptBlock(size_t position) {
    if (position < 10) return false;
    
    size_t start = (position > 20) ? position - 20 : 0;
    std::string context = sourceCode_.substr(start, position - start);
    
    return context.find("script") != std::string::npos;
}

bool CommentGeneratorStateMachine::isLeavingBlock(char ch) {
    return (ch == '}' && braceLevel_ == 0);
}

// =============================================================================
// GeneratorCommentSystem Implementation
// =============================================================================

GeneratorCommentSystem::GeneratorCommentSystem() 
    : contextAnalyzer_(std::make_unique<CommentContextAnalyzer>())
    , stateMachine_(std::make_unique<CommentGeneratorStateMachine>())
{
    initializePatterns();
}

void GeneratorCommentSystem::initializePatterns() {
    // 匹配 -- 注释的正则表达式
    // 匹配: -- 注释内容 (到行尾或下一个 -- 或其他终止符)
    generatorCommentPattern_ = std::regex(R"(--(.*?)(?:\n|$|--))");
}

std::vector<CommentPosition> GeneratorCommentSystem::scanGeneratorComments(const std::string& sourceCode) {
    std::vector<CommentPosition> positions;
    
    // 使用正则表达式查找所有 -- 注释
    std::sregex_iterator iter(sourceCode.begin(), sourceCode.end(), generatorCommentPattern_);
    std::sregex_iterator end;
    
    size_t lineNumber = 1;
    size_t columnNumber = 1;
    
    for (; iter != end; ++iter) {
        const auto& match = *iter;
        
        CommentPosition pos;
        pos.startPos = match.position();
        pos.endPos = match.position() + match.length();
        pos.originalContent = match.str(1); // 捕获组1是注释内容
        
        // 计算行号和列号
        pos.lineNumber = std::count(sourceCode.begin(), sourceCode.begin() + pos.startPos, '\n') + 1;
        
        // 计算列号
        size_t lastNewline = sourceCode.rfind('\n', pos.startPos);
        pos.columnNumber = (lastNewline == std::string::npos) ? pos.startPos + 1 : pos.startPos - lastNewline;
        
        positions.push_back(pos);
    }
    
    return positions;
}

std::vector<GeneratedComment> GeneratorCommentSystem::generateComments(
    const std::string& sourceCode,
    const std::vector<CommentPosition>& commentPositions
) {
    std::vector<GeneratedComment> generatedComments;
    
    // 设置状态机的源代码
    stateMachine_->setSourceCode(sourceCode);
    
    for (const auto& pos : commentPositions) {
        GeneratedComment comment;
        comment.position = pos;
        
        // 分析注释的语言上下文
        comment.targetLanguage = contextAnalyzer_->analyzeContext(sourceCode, pos);
        
        // 根据目标语言生成相应的注释
        std::string content = pos.originalContent;
        
        // 去除前后空白
        content.erase(0, content.find_first_not_of(" \t"));
        content.erase(content.find_last_not_of(" \t") + 1);
        
        switch (comment.targetLanguage) {
            case CommentLanguageContext::HTML_CONTEXT:
                comment.content = generateHtmlComment(content);
                comment.escapedContent = escapeHtmlComment(content);
                break;
                
            case CommentLanguageContext::CSS_CONTEXT:
                comment.content = generateCssComment(content);
                comment.escapedContent = escapeCssComment(content);
                break;
                
            case CommentLanguageContext::JAVASCRIPT_CONTEXT:
                comment.content = generateJavaScriptComment(content);
                comment.escapedContent = escapeJavaScriptComment(content);
                break;
                
            default:
                comment.content = generateHtmlComment(content);
                comment.escapedContent = escapeHtmlComment(content);
                break;
        }
        
        comment.needsEscaping = (content != comment.escapedContent);
        
        generatedComments.push_back(comment);
    }
    
    return generatedComments;
}

std::string GeneratorCommentSystem::replaceComments(
    const std::string& sourceCode,
    const std::vector<GeneratedComment>& generatedComments
) {
    std::string result = sourceCode;
    
    // 从后往前替换，避免位置偏移问题
    for (auto it = generatedComments.rbegin(); it != generatedComments.rend(); ++it) {
        const auto& comment = *it;
        
        // 构建原始匹配模式
        std::string originalPattern = "--" + comment.position.originalContent;
        
        // 查找并替换
        size_t pos = result.find(originalPattern, comment.position.startPos);
        if (pos != std::string::npos) {
            result.replace(pos, originalPattern.length(), comment.content);
        }
    }
    
    return result;
}

std::string GeneratorCommentSystem::processGeneratorComments(const std::string& sourceCode) {
    // 扫描所有 -- 注释
    auto commentPositions = scanGeneratorComments(sourceCode);
    
    if (commentPositions.empty()) {
        return sourceCode; // 没有 -- 注释，直接返回
    }
    
    // 生成适当格式的注释
    auto generatedComments = generateComments(sourceCode, commentPositions);
    
    // 替换原始注释
    return replaceComments(sourceCode, generatedComments);
}

// 注释生成方法
std::string GeneratorCommentSystem::generateHtmlComment(const std::string& content) {
    return "<!-- " + content + " -->";
}

std::string GeneratorCommentSystem::generateCssComment(const std::string& content) {
    return "/* " + content + " */";
}

std::string GeneratorCommentSystem::generateJavaScriptComment(const std::string& content, bool useBlockComment) {
    if (useBlockComment || content.find('\n') != std::string::npos) {
        return "/* " + content + " */";
    } else {
        return "// " + content;
    }
}

// 内容转义方法
std::string GeneratorCommentSystem::escapeHtmlComment(const std::string& content) {
    std::string escaped = content;
    
    // 转义HTML注释中的特殊序列
    size_t pos = 0;
    while ((pos = escaped.find("--", pos)) != std::string::npos) {
        escaped.replace(pos, 2, "&#45;&#45;");
        pos += 10; // 跳过替换的内容
    }
    
    return escaped;
}

std::string GeneratorCommentSystem::escapeCssComment(const std::string& content) {
    std::string escaped = content;
    
    // 转义CSS注释中的特殊序列
    size_t pos = 0;
    while ((pos = escaped.find("*/", pos)) != std::string::npos) {
        escaped.replace(pos, 2, "*\\/");
        pos += 3;
    }
    
    return escaped;
}

std::string GeneratorCommentSystem::escapeJavaScriptComment(const std::string& content) {
    std::string escaped = content;
    
    // 对于单行注释，移除换行符
    std::replace(escaped.begin(), escaped.end(), '\n', ' ');
    std::replace(escaped.begin(), escaped.end(), '\r', ' ');
    
    return escaped;
}

} // namespace chtl
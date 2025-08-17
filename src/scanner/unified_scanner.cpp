#include "scanner/unified_scanner.hpp"
#include <algorithm>
#include <sstream>
#include <stack>
#include <cctype>

namespace CHTL {

// SliceValidator 实现
bool SliceValidator::isValidSliceBoundary(const std::string& text, size_t position) {
    if (position >= text.length()) return true;
    
    // 检查是否在字符串内部
    if (isInsideString(text, position)) return false;
    
    // 检查是否在注释内部
    if (isInsideComment(text, position)) return false;
    
    return true;
}

bool SliceValidator::isInsideString(const std::string& text, size_t position) {
    if (position >= text.length()) return false;
    
    bool inSingle = false, inDouble = false;
    for (size_t i = 0; i < position; ++i) {
        if (text[i] == '\\' && i + 1 < text.length()) {
            ++i; // 跳过转义字符
            continue;
        }
        if (text[i] == '"' && !inSingle) {
            inDouble = !inDouble;
        } else if (text[i] == '\'' && !inDouble) {
            inSingle = !inSingle;
        }
    }
    
    return inSingle || inDouble;
}

bool SliceValidator::isInsideComment(const std::string& text, size_t position) {
    if (position >= text.length()) return false;
    
    // 查找当前位置之前最近的注释开始/结束
    auto bounds = findCommentBounds(text, position);
    return bounds.first != std::string::npos && bounds.second == std::string::npos;
}

bool SliceValidator::isBalancedBraces(const std::string& text, size_t start, size_t end) {
    std::stack<char> stack;
    std::unordered_map<char, char> pairs = {{')', '('}, {'}', '{'}, {']', '['}};
    
    for (size_t i = start; i < end && i < text.length(); ++i) {
        char c = text[i];
        if (c == '(' || c == '{' || c == '[') {
            stack.push(c);
        } else if (c == ')' || c == '}' || c == ']') {
            if (stack.empty() || stack.top() != pairs[c]) {
                return false;
            }
            stack.pop();
        }
    }
    
    return stack.empty();
}

std::pair<char, char> SliceValidator::findStringDelimiters(const std::string& text, size_t position) {
    // 实现字符串分隔符查找逻辑
    // 简化实现，返回默认值
    return {'\0', '\0'};
}

std::pair<size_t, size_t> SliceValidator::findCommentBounds(const std::string& text, size_t position) {
    // 查找注释边界
    size_t commentStart = std::string::npos;
    size_t commentEnd = std::string::npos;
    
    // 向前查找注释开始
    for (size_t i = position; i > 0; --i) {
        if (i >= 2 && text.substr(i-2, 2) == "/*") {
            commentStart = i - 2;
            break;
        }
        if (i >= 2 && text.substr(i-2, 2) == "//") {
            // 单行注释，查找换行符
            for (size_t j = i; j < text.length(); ++j) {
                if (text[j] == '\n') {
                    if (position < j) {
                        commentStart = i - 2;
                        commentEnd = j;
                    }
                    break;
                }
            }
            break;
        }
    }
    
    // 如果找到注释开始，查找结束
    if (commentStart != std::string::npos && commentEnd == std::string::npos) {
        if (text.substr(commentStart, 2) == "/*") {
            for (size_t i = commentStart + 2; i < text.length() - 1; ++i) {
                if (text.substr(i, 2) == "*/") {
                    commentEnd = i + 2;
                    break;
                }
            }
        }
    }
    
    return {commentStart, commentEnd};
}

// MinimalUnitDetector 实现
MinimalUnitDetector::MinimalUnitDetector(const CompilerConfig& config) : config_(config) {
    initializePatterns();
}

void MinimalUnitDetector::initializePatterns() {
    // CHTL JS 选择器模式 {{...}}
    unitPatterns_.emplace_back(R"(\{\{[^}]*\}\})");
    
    // CHTL JS 箭头操作符 ->
    unitPatterns_.emplace_back(R"(->)");
    
    // CHTL JS 关键字
    unitPatterns_.emplace_back(R"(\blisten\b)");
    unitPatterns_.emplace_back(R"(\bdelegate\b)");
    unitPatterns_.emplace_back(R"(\banimate\b)");
    
    // 模板变量 VariableName(...)
    unitPatterns_.emplace_back(R"([A-Za-z_][A-Za-z0-9_]*\([^)]*\))");
    
    // 命名空间 from
    unitPatterns_.emplace_back(R"(\bfrom\s+[A-Za-z_][A-Za-z0-9_.]*\b)");
    
    // 初始化关键字映射
    keywordToType_["listen"] = MinimalUnitType::CHTL_LISTEN;
    keywordToType_["delegate"] = MinimalUnitType::CHTL_DELEGATE;
    keywordToType_["animate"] = MinimalUnitType::CHTL_ANIMATE;
    keywordToType_["->"] = MinimalUnitType::CHTL_ARROW;
}

std::optional<MinimalUnit> MinimalUnitDetector::detectUnit(const std::string& text, size_t position) {
    if (position >= text.length()) return std::nullopt;
    
    for (const auto& pattern : unitPatterns_) {
        std::sregex_iterator iter(text.begin() + position, text.end(), pattern);
        if (iter != std::sregex_iterator{}) {
            const auto& match = *iter;
            if (static_cast<size_t>(match.position()) == 0) { // 从当前位置开始匹配
                std::string content = match.str();
                MinimalUnitType type = classifyUnit(content);
                SourceLocation loc("", 1, 1, position);
                return MinimalUnit(type, content, loc, content.length());
            }
        }
    }
    
    return std::nullopt;
}

MinimalUnitType MinimalUnitDetector::classifyUnit(const std::string& content) {
    // CHTL JS 选择器
    if (content.length() >= 4 && content.substr(0, 2) == "{{" && content.substr(content.length() - 2) == "}}") {
        return MinimalUnitType::CHTL_SELECTOR;
    }
    
    // 检查关键字映射
    auto it = keywordToType_.find(content);
    if (it != keywordToType_.end()) {
        return it->second;
    }
    
    // 模板变量或自定义变量
    if (std::regex_match(content, std::regex(R"([A-Za-z_][A-Za-z0-9_]*\([^)]*\))"))) {
        if (content.find('=') != std::string::npos) {
            return MinimalUnitType::CUSTOM_VAR;
        } else {
            return MinimalUnitType::TEMPLATE_VAR;
        }
    }
    
    // 命名空间 from
    if (content.length() >= 5 && content.substr(0, 5) == "from ") {
        return MinimalUnitType::NAMESPACE_FROM;
    }
    
    return MinimalUnitType::UNKNOWN;
}

// ContextAnalyzer 实现
ContextAnalyzer::ContextAnalyzer(const CompilerConfig& config) : config_(config) {}

std::unordered_map<std::string, std::string> ContextAnalyzer::analyzeContext(
    const std::string& text, size_t start, size_t end) {
    
    std::unordered_map<std::string, std::string> context;
    
    // 检测块类型
    std::string blockType = detectBlockType(text, start);
    context["blockType"] = blockType;
    
    // 计算嵌套级别
    int nestingLevel = calculateNestingLevel(text, start);
    context["nestingLevel"] = std::to_string(nestingLevel);
    
    // 检测是否在特定语法块中
    if (blockType == "style") {
        context["allowsCHTL"] = "true";
        context["allowsCSS"] = "true";
    } else if (blockType == "script") {
        context["allowsCHTLJS"] = "true";
        context["allowsJS"] = "true";
    } else {
        context["allowsCHTL"] = "true";
    }
    
    return context;
}

bool ContextAnalyzer::allowsCHTLSyntax(const std::unordered_map<std::string, std::string>& context) {
    auto it = context.find("allowsCHTL");
    return it != context.end() && it->second == "true";
}

bool ContextAnalyzer::allowsCHTLJSSyntax(const std::unordered_map<std::string, std::string>& context) {
    auto it = context.find("allowsCHTLJS");
    return it != context.end() && it->second == "true";
}

std::string ContextAnalyzer::detectBlockType(const std::string& text, size_t position) {
    // 向前查找最近的块开始标记
    for (size_t i = position; i > 0; --i) {
        if (text.substr(i, 5) == "style") {
            return "style";
        }
        if (text.substr(i, 6) == "script") {
            return "script";
        }
        if (text.substr(i, 4) == "text") {
            return "text";
        }
    }
    return "element";
}

int ContextAnalyzer::calculateNestingLevel(const std::string& text, size_t position) {
    int level = 0;
    for (size_t i = 0; i < position && i < text.length(); ++i) {
        if (text[i] == '{') level++;
        else if (text[i] == '}') level--;
    }
    return level;
}

// CHTLUnifiedScanner 实现
CHTLUnifiedScanner::CHTLUnifiedScanner(const CompilerConfig& config) 
    : config_(config),
      unitDetector_(std::make_unique<MinimalUnitDetector>(config)),
      contextAnalyzer_(std::make_unique<ContextAnalyzer>(config)) {
}

std::vector<CodeFragment> CHTLUnifiedScanner::scan(const std::string& source, const std::string& filename) {
    clearErrors();
    return performVariableLengthSlicing(source, filename);
}

void CHTLUnifiedScanner::setConfig(const CompilerConfig& config) {
    config_ = config;
    unitDetector_ = std::make_unique<MinimalUnitDetector>(config);
    contextAnalyzer_ = std::make_unique<ContextAnalyzer>(config);
}

std::vector<CodeFragment> CHTLUnifiedScanner::performVariableLengthSlicing(const std::string& source, const std::string& filename) {
    std::vector<CodeFragment> fragments;
    size_t position = 0;
    const size_t chunkSize = 1024; // 初始切片大小
    
    while (position < source.length()) {
        size_t initialEnd = std::min(position + chunkSize, source.length());
        size_t optimalEnd = findOptimalSliceEnd(source, position, initialEnd);
        
        // 提取内容
        std::string content = extractContent(source, position, optimalEnd);
        SourceLocation location = calculateLocation(source, position, filename);
        
        // 分析上下文
        auto context = contextAnalyzer_->analyzeContext(source, position, optimalEnd);
        
        // 检测片段类型
        FragmentType type = detectFragmentType(content, context);
        
        // 创建初始片段
        CodeFragment fragment(type, content, location);
        fragment.context = context;
        
        // 如果是CHTL或CHTL JS片段，进行最小单元二次切割
        if (type == FragmentType::CHTL || type == FragmentType::CHTL_JS) {
            auto subFragments = performMinimalUnitCutting(fragment);
            fragments.insert(fragments.end(), subFragments.begin(), subFragments.end());
        } else {
            fragments.push_back(fragment);
        }
        
        position = optimalEnd;
    }
    
    // 验证和优化片段
    return validateAndOptimizeFragments(std::move(fragments));
}

FragmentType CHTLUnifiedScanner::detectFragmentType(const std::string& content, const std::unordered_map<std::string, std::string>& context) {
    // 检查是否包含CHTL语法
    if (containsCHTLSyntax(content)) {
        return FragmentType::CHTL;
    }
    
    // 检查是否包含CHTL JS语法
    if (containsCHTLJSSyntax(content)) {
        return FragmentType::CHTL_JS;
    }
    
    // 根据上下文判断
    auto blockType = context.find("blockType");
    if (blockType != context.end()) {
        if (blockType->second == "style") {
            return FragmentType::CSS;
        } else if (blockType->second == "script") {
            return FragmentType::JAVASCRIPT;
        }
    }
    
    return FragmentType::CHTL; // 默认为CHTL
}

size_t CHTLUnifiedScanner::findOptimalSliceEnd(const std::string& source, size_t start, size_t initialEnd) {
    size_t optimalEnd = initialEnd;
    
    // 检查初始切割位置是否合理
    if (!SliceValidator::isValidSliceBoundary(source, initialEnd)) {
        // 向前查找合理的切割点
        for (size_t i = initialEnd; i < source.length() && i < start + 2048; ++i) {
            if (SliceValidator::isValidSliceBoundary(source, i)) {
                optimalEnd = i;
                break;
            }
        }
    }
    
    // 检查是否在CHTL/CHTL JS最小单元中间切断
    auto unit = unitDetector_->detectUnit(source, optimalEnd);
    if (unit && optimalEnd < start + unit->length) {
        // 向前扩展到单元结束
        optimalEnd = start + unit->length;
    }
    
    return optimalEnd;
}

std::vector<CodeFragment> CHTLUnifiedScanner::performMinimalUnitCutting(const CodeFragment& fragment) {
    std::vector<CodeFragment> subFragments;
    const std::string& content = fragment.content;
    size_t position = 0;
    
    while (position < content.length()) {
        auto unit = unitDetector_->detectUnit(content, position);
        if (unit) {
            // 如果前面有非单元内容，先添加
            if (position < static_cast<size_t>(unit->location.offset)) {
                std::string beforeContent = content.substr(position, unit->location.offset - position);
                if (!beforeContent.empty()) {
                    SourceLocation loc = fragment.location;
                    loc.offset += position;
                    CodeFragment beforeFrag(fragment.type, beforeContent, loc);
                    beforeFrag.context = fragment.context;
                    subFragments.push_back(beforeFrag);
                }
            }
            
            // 添加最小单元片段
            SourceLocation unitLoc = fragment.location;
            unitLoc.offset += unit->location.offset;
            CodeFragment unitFrag(fragment.type, unit->content, unitLoc);
            unitFrag.context = fragment.context;
            unitFrag.context["minimalUnit"] = "true";
            subFragments.push_back(unitFrag);
            
            position = unit->location.offset + unit->length;
        } else {
            // 没有找到单元，添加剩余内容
            std::string remaining = content.substr(position);
            if (!remaining.empty()) {
                SourceLocation loc = fragment.location;
                loc.offset += position;
                CodeFragment remainingFrag(fragment.type, remaining, loc);
                remainingFrag.context = fragment.context;
                subFragments.push_back(remainingFrag);
            }
            break;
        }
    }
    
    return subFragments;
}

bool CHTLUnifiedScanner::shouldMergeWithNext(const CodeFragment& current, const CodeFragment& next) {
    // 如果类型不同，不合并
    if (current.type != next.type) return false;
    
    // 如果不是CHTL或CHTL JS，不需要特殊处理
    if (current.type != FragmentType::CHTL && current.type != FragmentType::CHTL_JS) return false;
    
    // 检查是否连续
    auto currentMinimal = current.context.find("minimalUnit");
    auto nextMinimal = next.context.find("minimalUnit");
    
    // 如果都是最小单元且位置连续，考虑合并
    if (currentMinimal != current.context.end() && nextMinimal != next.context.end()) {
        return (current.location.offset + current.content.length()) == next.location.offset;
    }
    
    return false;
}

std::vector<CodeFragment> CHTLUnifiedScanner::validateAndOptimizeFragments(std::vector<CodeFragment> fragments) {
    // 合并连续的相同类型片段
    std::vector<CodeFragment> optimized;
    
    for (size_t i = 0; i < fragments.size(); ++i) {
        if (i + 1 < fragments.size() && shouldMergeWithNext(fragments[i], fragments[i + 1])) {
            // 合并当前片段和下一个片段
            std::string mergedContent = fragments[i].content + fragments[i + 1].content;
            CodeFragment merged(fragments[i].type, mergedContent, fragments[i].location);
            merged.context = fragments[i].context;
            merged.context.erase("minimalUnit"); // 移除最小单元标记
            optimized.push_back(merged);
            ++i; // 跳过下一个片段
        } else {
            optimized.push_back(fragments[i]);
        }
    }
    
    return optimized;
}

bool CHTLUnifiedScanner::containsCHTLSyntax(const std::string& content) {
    // 检查CHTL特有语法
    std::vector<std::regex> patterns = {
        std::regex(R"(\[Template\])"),
        std::regex(R"(\[Custom\])"),
        std::regex(R"(\[Origin\])"),
        std::regex(R"(\[Import\])"),
        std::regex(R"(\[Namespace\])"),
        std::regex(R"(\[Configuration\])"),
        std::regex(R"(@Style|@Element|@Var)"),
        std::regex(R"(\bdelete\b|\binherit\b|\binsert\b)"),
        std::regex(R"(\bexcept\b)"),
        std::regex(R"(\bfrom\b.*\bas\b)")
    };
    
    for (const auto& pattern : patterns) {
        if (std::regex_search(content, pattern)) {
            return true;
        }
    }
    
    return false;
}

bool CHTLUnifiedScanner::containsCHTLJSSyntax(const std::string& content) {
    // 检查CHTL JS特有语法
    std::vector<std::regex> patterns = {
        std::regex(R"(\{\{[^}]*\}\})"), // 选择器
        std::regex(R"(->)"),            // 箭头操作符
        std::regex(R"(\blisten\b)"),     // 事件监听
        std::regex(R"(\bdelegate\b)"),   // 事件委托
        std::regex(R"(\banimate\b)")     // 动画
    };
    
    for (const auto& pattern : patterns) {
        if (std::regex_search(content, pattern)) {
            return true;
        }
    }
    
    return false;
}

bool CHTLUnifiedScanner::containsTemplateSyntax(const std::string& content) {
    return std::regex_search(content, std::regex(R"([A-Za-z_][A-Za-z0-9_]*\([^)]*\))"));
}

bool CHTLUnifiedScanner::containsNamespaceSyntax(const std::string& content) {
    return std::regex_search(content, std::regex(R"(\bfrom\s+[A-Za-z_][A-Za-z0-9_.]*\b)"));
}

SourceLocation CHTLUnifiedScanner::calculateLocation(const std::string& source, size_t offset, const std::string& filename) {
    size_t line = 1;
    size_t column = 1;
    
    for (size_t i = 0; i < offset && i < source.length(); ++i) {
        if (source[i] == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
    }
    
    return SourceLocation(filename, line, column, offset);
}

std::string CHTLUnifiedScanner::extractContent(const std::string& source, size_t start, size_t end) {
    if (start >= source.length()) return "";
    end = std::min(end, source.length());
    return source.substr(start, end - start);
}

void CHTLUnifiedScanner::reportError(const std::string& message, const SourceLocation& location) {
    errors_.emplace_back(message, location, "error");
}

void CHTLUnifiedScanner::reportWarning(const std::string& message, const SourceLocation& location) {
    errors_.emplace_back(message, location, "warning");
}

CHTLUnifiedScanner::BlockType CHTLUnifiedScanner::detectBlockType(const std::string& content, const SourceLocation& location) {
    // 简化实现，根据内容检测块类型
    if (content.find("style") != std::string::npos) {
        return BlockType::LOCAL_STYLE;
    }
    if (content.find("script") != std::string::npos) {
        return BlockType::LOCAL_SCRIPT;
    }
    if (content.find("[Template]") != std::string::npos) {
        return BlockType::TEMPLATE_BLOCK;
    }
    if (content.find("[Custom]") != std::string::npos) {
        return BlockType::CUSTOM_BLOCK;
    }
    if (content.find("[Import]") != std::string::npos) {
        return BlockType::IMPORT_BLOCK;
    }
    if (content.find("[Namespace]") != std::string::npos) {
        return BlockType::NAMESPACE_BLOCK;
    }
    if (content.find("[Configuration]") != std::string::npos) {
        return BlockType::CONFIGURATION_BLOCK;
    }
    if (content.find("[Origin]") != std::string::npos) {
        return BlockType::ORIGIN_BLOCK;
    }
    
    return BlockType::ELEMENT_CONTENT;
}

std::string CHTLUnifiedScanner::getBlockTypeString(BlockType type) {
    switch (type) {
        case BlockType::GLOBAL_STYLE: return "global_style";
        case BlockType::LOCAL_STYLE: return "local_style";
        case BlockType::LOCAL_SCRIPT: return "local_script";
        case BlockType::GLOBAL_SCRIPT: return "global_script";
        case BlockType::ELEMENT_CONTENT: return "element";
        case BlockType::TEMPLATE_BLOCK: return "template";
        case BlockType::CUSTOM_BLOCK: return "custom";
        case BlockType::IMPORT_BLOCK: return "import";
        case BlockType::NAMESPACE_BLOCK: return "namespace";
        case BlockType::CONFIGURATION_BLOCK: return "configuration";
        case BlockType::ORIGIN_BLOCK: return "origin";
        default: return "unknown";
    }
}

} // namespace CHTL
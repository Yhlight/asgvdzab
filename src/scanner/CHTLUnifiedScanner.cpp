#include "CHTLUnifiedScanner.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <sstream>

namespace CHTL {

CHTLUnifiedScanner::CHTLUnifiedScanner() 
    : debugMode_(false), initialSliceSize_(1024), maxExpansions_(5), expansionFactor_(1.5) {
    initializeKeywords();
}

void CHTLUnifiedScanner::initializeKeywords() {
    // 初始化HTML元素集合
    htmlElements_ = {
        // 基本HTML元素
        "html", "head", "body", "title", "meta", "link", "script", "style", "base",
        "div", "span", "p", "h1", "h2", "h3", "h4", "h5", "h6", "br", "hr",
        "a", "img", "ul", "ol", "li", "dl", "dt", "dd",
        "table", "thead", "tbody", "tfoot", "tr", "td", "th", "caption",
        "form", "input", "textarea", "select", "option", "button", "label",
        "fieldset", "legend", "datalist", "output", "progress", "meter",
        "section", "article", "aside", "nav", "header", "footer", "main",
        "figure", "figcaption", "details", "summary", "dialog",
        "canvas", "svg", "audio", "video", "source", "track", "embed", "object",
        "iframe", "area", "map", "param", "wbr", "ruby", "rt", "rp",
        "bdi", "bdo", "abbr", "address", "cite", "code", "dfn", "em", "i",
        "kbd", "mark", "q", "s", "samp", "small", "strong", "sub", "sup",
        "time", "u", "var", "del", "ins", "blockquote", "pre"
    };
    
    // 初始化CHTL关键字集合
    chtlKeywords_ = {
        "text", "style", "script", "inherit", "delete", "insert", "after", 
        "before", "replace", "except", "from", "as", "vir", "listen", "animate"
    };
}

FragmentList CHTLUnifiedScanner::scan(const std::string& sourceCode, const std::string& filename) {
    currentFilename_ = filename;
    clearErrors();
    
    debugOutput("开始扫描文件: " + filename);
    
    return scanImpl(sourceCode);
}

FragmentList CHTLUnifiedScanner::scanImpl(const std::string& sourceCode) {
    debugOutput("使用新的可变长度切片扫描机制");
    return scanWithVariableSlicing(sourceCode);
}

FragmentList CHTLUnifiedScanner::scanWithVariableSlicing(const std::string& sourceCode) {
    debugOutput("开始可变长度切片扫描，初始切片大小: " + std::to_string(initialSliceSize_));
    
    // 第一步：创建初始切片
    std::vector<std::string> initialSlices = createInitialSlices(sourceCode);
    debugOutput("创建了 " + std::to_string(initialSlices.size()) + " 个初始切片");
    
    // 第二步：检查完整性并扩增切片
    std::vector<std::string> expandedSlices;
    for (size_t i = 0; i < initialSlices.size(); ++i) {
        std::string slice = initialSlices[i];
        std::string nextSlice = (i + 1 < initialSlices.size()) ? initialSlices[i + 1] : "";
        
        size_t expansions = 0;
        std::string expandedSlice = expandSliceIfNeeded(slice, nextSlice, expansions);
        
        if (expansions > 0) {
            debugOutput("切片 " + std::to_string(i) + " 经过 " + std::to_string(expansions) + " 次扩增");
            // 需要合并后续切片
            for (size_t j = 0; j < expansions && i + 1 + j < initialSlices.size(); ++j) {
                i++; // 跳过已合并的切片
            }
        }
        
        expandedSlices.push_back(expandedSlice);
    }
    
    // 第三步：识别片段类型并进行二次切割
    FragmentList allFragments;
    size_t currentOffset = 0;
    size_t currentLine = 1;
    size_t currentColumn = 1;
    
    for (const auto& slice : expandedSlices) {
        // 分析片段类型
        FragmentType type = analyzeBlockContentType(slice, "");
        
        // 创建基础位置信息
        FragmentPosition basePos(currentLine, currentLine, currentColumn, 
                                currentColumn + slice.length(), currentOffset, 
                                currentOffset + slice.length());
        
        if (type == FragmentType::CHTL || type == FragmentType::CHTL_JS) {
            // 对CHTL和CHTL JS进行二次切割
            FragmentList subFragments = performSecondarySlicing(slice, type, basePos);
            allFragments.insert(allFragments.end(), subFragments.begin(), subFragments.end());
        } else {
            // 其他类型直接创建片段
            FragmentPtr fragment;
            switch (type) {
                case FragmentType::CSS:
                    fragment = std::make_shared<CSSFragment>(slice, basePos);
                    break;
                case FragmentType::JAVASCRIPT:
                    fragment = std::make_shared<JSFragment>(slice, basePos);
                    break;
                default:
                    fragment = std::make_shared<CHTLFragment>(slice, basePos);
                    break;
            }
            allFragments.push_back(fragment);
        }
        
        // 更新位置信息
        currentOffset += slice.length();
        // 简化的行列号更新
        for (char ch : slice) {
            if (ch == '\n') {
                currentLine++;
                currentColumn = 1;
            } else {
                currentColumn++;
            }
        }
    }
    
    // 第四步：上下文感知的片段合并
    FragmentList finalFragments = mergeRelatedFragments(allFragments);
    
    debugOutput("扫描完成，最终生成 " + std::to_string(finalFragments.size()) + " 个代码片段");
    
    return finalFragments;
}

void CHTLUnifiedScanner::skipWhitespaceAndComments(const std::string& source, ScanContext& ctx) {
    while (ctx.position < source.length()) {
        char ch = source[ctx.position];
        
        // 跳过空白字符
        if (std::isspace(ch)) {
            updatePosition(ctx, ch);
            continue;
        }
        
        // 处理注释
        if (ch == '/' && ctx.position + 1 < source.length()) {
            char next = source[ctx.position + 1];
            
            // 单行注释
            if (next == '/') {
                ctx.position += 2;
                ctx.column += 2;
                while (ctx.position < source.length() && source[ctx.position] != '\n') {
                    updatePosition(ctx, source[ctx.position]);
                }
                continue;
            }
            
            // 多行注释
            if (next == '*') {
                ctx.position += 2;
                ctx.column += 2;
                while (ctx.position + 1 < source.length()) {
                    if (source[ctx.position] == '*' && source[ctx.position + 1] == '/') {
                        ctx.position += 2;
                        ctx.column += 2;
                        break;
                    }
                    updatePosition(ctx, source[ctx.position]);
                }
                continue;
            }
        }
        
        // 处理生成器注释 --
        if (ch == '-' && ctx.position + 1 < source.length() && source[ctx.position + 1] == '-') {
            ctx.position += 2;
            ctx.column += 2;
            while (ctx.position < source.length() && source[ctx.position] != '\n') {
                updatePosition(ctx, source[ctx.position]);
            }
            continue;
        }
        
        break;
    }
}

FragmentPtr CHTLUnifiedScanner::scanBlock(const std::string& source, ScanContext& ctx) {
    size_t startPos = ctx.position;
    size_t startLine = ctx.line;
    size_t startCol = ctx.column;
    
    // 检查是否为特殊块开始标记
    if (source[ctx.position] == '[') {
        // 可能是 [Template], [Custom], [Origin], [Import] 等
        return scanTemplateOrCustomBlock(source, ctx);
    }
    
    // 扫描标识符
    std::string identifier = scanIdentifier(source, ctx);
    if (identifier.empty()) {
        updatePosition(ctx, source[ctx.position]);
        return nullptr;
    }
    
    skipWhitespaceAndComments(source, ctx);
    
    // 检查是否有大括号开始
    if (ctx.position >= source.length() || source[ctx.position] != '{') {
        // 不是代码块，回退
        ctx.position = startPos;
        ctx.line = startLine;
        ctx.column = startCol;
        return nullptr;
    }
    
    // 根据标识符类型决定扫描方式
    if (identifier == "style") {
        return scanStyleBlock(source, ctx);
    } else if (identifier == "script") {
        return scanScriptBlock(source, ctx);
    } else if (isHTMLElement(identifier) || isCHTLKeyword(identifier)) {
        ctx.currentElement = identifier;
        return scanCHTLElement(source, ctx);
    }
    
    // 默认作为CHTL元素处理
    ctx.currentElement = identifier;
    return scanCHTLElement(source, ctx);
}

FragmentPtr CHTLUnifiedScanner::scanCHTLElement(const std::string& source, ScanContext& ctx) {
    size_t startPos = ctx.position;
    size_t startLine = ctx.line;
    size_t startCol = ctx.column;
    
    // 查找匹配的大括号
    size_t endPos = findMatchingBrace(source, ctx.position);
    if (endPos == std::string::npos) {
        recordError("未找到匹配的大括号", ctx.line, ctx.column);
        return nullptr;
    }
    
    // 提取内容
    std::string content = source.substr(startPos, endPos - startPos + 1);
    
    // 更新位置
    size_t endLine = ctx.line;
    size_t endCol = ctx.column;
    
    // 计算结束位置的行列号
    for (size_t i = ctx.position; i <= endPos; ++i) {
        updatePosition(ctx, source[i]);
    }
    endLine = ctx.line;
    endCol = ctx.column;
    
    // 创建CHTL片段
    auto fragment = std::make_shared<CHTLFragment>(content, 
        createPosition(startPos, endPos, startLine, endLine, startCol, endCol));
    
    fragment->elementType = ctx.currentElement;
    fragment->context = ctx.currentElement;
    
    // 分析是否包含局部样式或脚本
    fragment->hasLocalStyle = content.find("style") != std::string::npos;
    fragment->hasLocalScript = content.find("script") != std::string::npos;
    
    return fragment;
}

FragmentPtr CHTLUnifiedScanner::scanStyleBlock(const std::string& source, ScanContext& ctx) {
    size_t startPos = ctx.position;
    size_t startLine = ctx.line;
    size_t startCol = ctx.column;
    
    // 跳过 '{'
    updatePosition(ctx, source[ctx.position]);
    
    size_t contentStart = ctx.position;
    size_t endPos = findMatchingBrace(source, startPos);
    
    if (endPos == std::string::npos) {
        recordError("未找到样式块的结束大括号", ctx.line, ctx.column);
        return nullptr;
    }
    
    // 提取样式内容（不包括大括号）
    std::string content = source.substr(contentStart, endPos - contentStart);
    
    // 更新位置到结束大括号之后
    ctx.position = endPos + 1;
    
    // 分析样式内容类型
    FragmentType styleType = analyzeBlockContentType(content, "style");
    
    FragmentPtr fragment;
    if (styleType == FragmentType::CSS) {
        fragment = std::make_shared<CSSFragment>(content,
            createPosition(startPos, endPos, startLine, ctx.line, startCol, ctx.column));
    } else {
        // 包含CHTL特性的样式块
        fragment = std::make_shared<CHTLFragment>(source.substr(startPos, endPos - startPos + 1),
            createPosition(startPos, endPos, startLine, ctx.line, startCol, ctx.column));
        static_cast<CHTLFragment*>(fragment.get())->hasLocalStyle = true;
    }
    
    fragment->context = "style";
    
    return fragment;
}

FragmentPtr CHTLUnifiedScanner::scanScriptBlock(const std::string& source, ScanContext& ctx) {
    size_t startPos = ctx.position;
    size_t startLine = ctx.line;
    size_t startCol = ctx.column;
    
    // 跳过 '{'
    updatePosition(ctx, source[ctx.position]);
    
    size_t contentStart = ctx.position;
    size_t endPos = findMatchingBrace(source, startPos);
    
    if (endPos == std::string::npos) {
        recordError("未找到脚本块的结束大括号", ctx.line, ctx.column);
        return nullptr;
    }
    
    // 提取脚本内容（不包括大括号）
    std::string content = source.substr(contentStart, endPos - contentStart);
    
    // 更新位置到结束大括号之后
    ctx.position = endPos + 1;
    
    // 分析脚本内容类型
    FragmentType scriptType = analyzeBlockContentType(content, "script");
    
    FragmentPtr fragment;
    if (scriptType == FragmentType::CHTL_JS) {
        fragment = std::make_shared<CHTLJSFragment>(content,
            createPosition(startPos, endPos, startLine, ctx.line, startCol, ctx.column));
        static_cast<CHTLJSFragment*>(fragment.get())->isLocalScript = true;
    } else {
        fragment = std::make_shared<JSFragment>(content,
            createPosition(startPos, endPos, startLine, ctx.line, startCol, ctx.column));
        static_cast<JSFragment*>(fragment.get())->isGlobal = false;
    }
    
    fragment->context = "script";
    
    return fragment;
}

FragmentPtr CHTLUnifiedScanner::scanOriginBlock(const std::string& source, ScanContext& ctx) {
    // 实现原始嵌入块扫描
    size_t startPos = ctx.position;
    size_t startLine = ctx.line;
    size_t startCol = ctx.column;
    
    // 这里需要解析 [Origin] @Type 的格式
    // 简化实现，直接扫描到匹配的大括号
    size_t endPos = findMatchingBrace(source, ctx.position);
    if (endPos == std::string::npos) {
        recordError("未找到原始嵌入块的结束大括号", ctx.line, ctx.column);
        return nullptr;
    }
    
    std::string content = source.substr(startPos, endPos - startPos + 1);
    ctx.position = endPos + 1;
    
    auto fragment = std::make_shared<HTMLFragment>(content,
        createPosition(startPos, endPos, startLine, ctx.line, startCol, ctx.column));
    
    return fragment;
}

FragmentPtr CHTLUnifiedScanner::scanTemplateOrCustomBlock(const std::string& source, ScanContext& ctx) {
    size_t startPos = ctx.position;
    size_t startLine = ctx.line;
    size_t startCol = ctx.column;
    
    // 扫描到 ']'
    while (ctx.position < source.length() && source[ctx.position] != ']') {
        updatePosition(ctx, source[ctx.position]);
    }
    
    if (ctx.position >= source.length()) {
        recordError("未找到 ']'", ctx.line, ctx.column);
        return nullptr;
    }
    
    // 跳过 ']'
    updatePosition(ctx, source[ctx.position]);
    
    // 继续扫描到大括号或分号
    skipWhitespaceAndComments(source, ctx);
    
    size_t endPos = findMatchingBrace(source, ctx.position);
    if (endPos == std::string::npos) {
        // 可能是单行声明
        while (ctx.position < source.length() && source[ctx.position] != ';' && source[ctx.position] != '\n') {
            updatePosition(ctx, source[ctx.position]);
        }
        endPos = ctx.position;
    } else {
        ctx.position = endPos + 1;
    }
    
    std::string content = source.substr(startPos, endPos - startPos + 1);
    
    // 根据内容判断类型
    FragmentType type = FragmentType::CHTL;
    if (content.find("[Origin]") != std::string::npos) {
        type = FragmentType::HTML;
    }
    
    FragmentPtr fragment = std::make_shared<CodeFragment>(type, FragmentLevel::BLOCK, content,
        createPosition(startPos, endPos, startLine, ctx.line, startCol, ctx.column));
    
    return fragment;
}

std::string CHTLUnifiedScanner::scanIdentifier(const std::string& source, ScanContext& ctx) {
    size_t start = ctx.position;
    
    if (ctx.position >= source.length() || (!std::isalpha(source[ctx.position]) && source[ctx.position] != '_')) {
        return "";
    }
    
    while (ctx.position < source.length() && 
           (std::isalnum(source[ctx.position]) || source[ctx.position] == '_' || source[ctx.position] == '-')) {
        updatePosition(ctx, source[ctx.position]);
    }
    
    return source.substr(start, ctx.position - start);
}

std::string CHTLUnifiedScanner::scanString(const std::string& source, ScanContext& ctx, char quote) {
    size_t start = ctx.position;
    updatePosition(ctx, source[ctx.position]); // 跳过开始引号
    
    while (ctx.position < source.length()) {
        char ch = source[ctx.position];
        if (ch == quote) {
            updatePosition(ctx, ch);
            break;
        } else if (ch == '\\' && ctx.position + 1 < source.length()) {
            updatePosition(ctx, ch);
            updatePosition(ctx, source[ctx.position]);
        } else {
            updatePosition(ctx, ch);
        }
    }
    
    return source.substr(start, ctx.position - start);
}

bool CHTLUnifiedScanner::matchKeyword(const std::string& source, size_t pos, const std::string& keyword) {
    if (pos + keyword.length() > source.length()) {
        return false;
    }
    
    return source.substr(pos, keyword.length()) == keyword;
}

size_t CHTLUnifiedScanner::findMatchingBrace(const std::string& source, size_t startPos) {
    if (startPos >= source.length() || source[startPos] != '{') {
        return std::string::npos;
    }
    
    int depth = 1;
    size_t pos = startPos + 1;
    bool inString = false;
    char stringChar = 0;
    bool inComment = false;
    bool inLineComment = false;
    
    while (pos < source.length() && depth > 0) {
        char ch = source[pos];
        
        if (inLineComment) {
            if (ch == '\n') {
                inLineComment = false;
            }
            pos++;
            continue;
        }
        
        if (inComment) {
            if (ch == '*' && pos + 1 < source.length() && source[pos + 1] == '/') {
                inComment = false;
                pos += 2;
                continue;
            }
            pos++;
            continue;
        }
        
        if (inString) {
            if (ch == stringChar && (pos == 0 || source[pos - 1] != '\\')) {
                inString = false;
            }
            pos++;
            continue;
        }
        
        if (ch == '"' || ch == '\'') {
            inString = true;
            stringChar = ch;
        } else if (ch == '/' && pos + 1 < source.length()) {
            if (source[pos + 1] == '/') {
                inLineComment = true;
                pos += 2;
                continue;
            } else if (source[pos + 1] == '*') {
                inComment = true;
                pos += 2;
                continue;
            }
        } else if (ch == '{') {
            depth++;
        } else if (ch == '}') {
            depth--;
        }
        
        pos++;
    }
    
    return (depth == 0) ? pos - 1 : std::string::npos;
}

bool CHTLUnifiedScanner::isHTMLElement(const std::string& identifier) const {
    return htmlElements_.find(identifier) != htmlElements_.end();
}

bool CHTLUnifiedScanner::isCHTLKeyword(const std::string& identifier) const {
    return chtlKeywords_.find(identifier) != chtlKeywords_.end();
}

FragmentType CHTLUnifiedScanner::analyzeBlockContentType(const std::string& content, const std::string& context) {
    // 分析代码块内容，判断是CHTL扩展语法还是标准CSS/JS
    
    if (context == "style") {
        // 检查是否包含CHTL样式特性
        if (content.find("{{") != std::string::npos ||  // 增强选择器
            content.find("&") != std::string::npos ||   // 上下文推导
            content.find(".") == 0 ||                   // 自动类名
            content.find("#") == 0) {                   // 自动ID
            return FragmentType::CHTL;  // CHTL增强样式
        }
        return FragmentType::CSS;
    } else if (context == "script") {
        // 检查是否包含CHTL JS特性
        if (content.find("{{") != std::string::npos ||  // 增强选择器
            content.find("->") != std::string::npos ||  // CHTL JS语法
            content.find("listen") != std::string::npos ||
            content.find("animate") != std::string::npos ||
            content.find("vir ") != std::string::npos) {
            return FragmentType::CHTL_JS;
        }
        return FragmentType::JAVASCRIPT;
    }
    
    return FragmentType::CHTL;
}

FragmentPosition CHTLUnifiedScanner::createPosition(size_t start, size_t end, size_t startLine, 
                                                    size_t endLine, size_t startCol, size_t endCol) {
    return FragmentPosition(startLine, endLine, startCol, endCol, start, end);
}

void CHTLUnifiedScanner::recordError(const std::string& message, size_t line, size_t column) {
    std::ostringstream oss;
    oss << currentFilename_ << ":" << line << ":" << column << ": " << message;
    errors_.push_back(oss.str());
    
    if (debugMode_) {
        std::cerr << "错误: " << oss.str() << std::endl;
    }
}

void CHTLUnifiedScanner::debugOutput(const std::string& message) {
    if (debugMode_) {
        std::cout << "[CHTLUnifiedScanner] " << message << std::endl;
    }
}

void CHTLUnifiedScanner::updatePosition(ScanContext& ctx, char ch) {
    if (ch == '\n') {
        ctx.line++;
        ctx.column = 1;
    } else {
        ctx.column++;
    }
    ctx.position++;
}

std::vector<std::string> CHTLUnifiedScanner::createInitialSlices(const std::string& sourceCode) {
    std::vector<std::string> slices;
    size_t pos = 0;
    
    while (pos < sourceCode.length()) {
        size_t sliceSize = std::min(initialSliceSize_, sourceCode.length() - pos);
        std::string slice = sourceCode.substr(pos, sliceSize);
        slices.push_back(slice);
        pos += sliceSize;
    }
    
    return slices;
}

std::string CHTLUnifiedScanner::expandSliceIfNeeded(const std::string& slice, const std::string& nextSlice, 
                                                   size_t& expansions) {
    std::string currentSlice = slice;
    expansions = 0;
    
    // 检查当前切片是否可能包含不完整的CHTL或CHTL JS代码
    while (expansions < maxExpansions_ && !nextSlice.empty()) {
        // 检查是否需要扩增
        bool needsExpansion = false;
        
        // 检查大括号平衡
        int braceCount = 0;
        bool inString = false;
        char stringChar = 0;
        
        for (char ch : currentSlice) {
            if (!inString) {
                if (ch == '"' || ch == '\'') {
                    inString = true;
                    stringChar = ch;
                } else if (ch == '{') {
                    braceCount++;
                } else if (ch == '}') {
                    braceCount--;
                }
            } else if (ch == stringChar) {
                inString = false;
            }
        }
        
        // 如果大括号不平衡，需要扩增
        if (braceCount > 0) {
            needsExpansion = true;
        }
        
        // 检查是否在CHTL JS特殊语法中间截断
        if (currentSlice.find("{{") != std::string::npos) {
            size_t lastDoubleOpen = currentSlice.rfind("{{");
            size_t lastDoubleClose = currentSlice.rfind("}}", lastDoubleOpen);
            if (lastDoubleClose == std::string::npos) {
                needsExpansion = true;
            }
        }
        
        // 检查是否在箭头操作符中间截断
        if (currentSlice.length() > 0 && currentSlice.back() == '-') {
            needsExpansion = true;
        }
        
        if (!needsExpansion) {
            break;
        }
        
        // 执行扩增
        size_t expansionSize = static_cast<size_t>(initialSliceSize_ * expansionFactor_);
        size_t actualExpansionSize = std::min(expansionSize, nextSlice.length());
        
        currentSlice += nextSlice.substr(0, actualExpansionSize);
        expansions++;
        
        debugOutput("扩增切片，新长度: " + std::to_string(currentSlice.length()));
    }
    
    return currentSlice;
}

bool CHTLUnifiedScanner::isFragmentComplete(const std::string& fragment, FragmentType type) {
    if (type == FragmentType::CHTL || type == FragmentType::CHTL_JS) {
        // 检查大括号平衡
        int braceCount = 0;
        bool inString = false;
        char stringChar = 0;
        
        for (char ch : fragment) {
            if (!inString) {
                if (ch == '"' || ch == '\'') {
                    inString = true;
                    stringChar = ch;
                } else if (ch == '{') {
                    braceCount++;
                } else if (ch == '}') {
                    braceCount--;
                }
            } else if (ch == stringChar) {
                inString = false;
            }
        }
        
        return braceCount == 0 && !inString;
    }
    
    return true; // 其他类型暂时认为总是完整的
}

FragmentList CHTLUnifiedScanner::performSecondarySlicing(const std::string& content, FragmentType type, 
                                                        const FragmentPosition& basePosition) {
    FragmentList fragments;
    
    if (type == FragmentType::CHTL) {
        // CHTL二次切割
        std::vector<std::string> units = identifyCHTLMinimalUnits(content);
        
        size_t offset = 0;
        for (const auto& unit : units) {
            FragmentPosition pos(
                basePosition.startLine,
                basePosition.startLine, // 简化：假设单行
                basePosition.startColumn + offset,
                basePosition.startColumn + offset + unit.length(),
                basePosition.startOffset + offset,
                basePosition.startOffset + offset + unit.length()
            );
            
            auto fragment = std::make_shared<CHTLFragment>(unit, pos, FragmentLevel::TOKEN);
            fragments.push_back(fragment);
            
            offset += unit.length();
        }
    } else if (type == FragmentType::CHTL_JS) {
        // CHTL JS二次切割
        std::vector<std::string> units = identifyCHTLJSMinimalUnits(content);
        
        size_t offset = 0;
        for (const auto& unit : units) {
            FragmentPosition pos(
                basePosition.startLine,
                basePosition.startLine,
                basePosition.startColumn + offset,
                basePosition.startColumn + offset + unit.length(),
                basePosition.startOffset + offset,
                basePosition.startOffset + offset + unit.length()
            );
            
            auto fragment = std::make_shared<CHTLJSFragment>(unit, pos, FragmentLevel::TOKEN);
            fragments.push_back(fragment);
            
            offset += unit.length();
        }
    }
    
    return fragments;
}

std::vector<std::string> CHTLUnifiedScanner::identifyCHTLMinimalUnits(const std::string& content) {
    std::vector<std::string> units;
    std::string currentUnit;
    
    size_t i = 0;
    while (i < content.length()) {
        char ch = content[i];
        
        // 处理文本块
        if (i + 4 <= content.length() && content.substr(i, 4) == "text") {
            if (!currentUnit.empty()) {
                units.push_back(currentUnit);
                currentUnit.clear();
            }
            
            // 查找text块的完整结构
            size_t textStart = i;
            i += 4; // 跳过"text"
            
            // 跳过空白
            while (i < content.length() && std::isspace(content[i])) {
                i++;
            }
            
            // 查找大括号
            if (i < content.length() && content[i] == '{') {
                size_t braceEnd = findMatchingBrace(content, i);
                if (braceEnd != std::string::npos) {
                    units.push_back(content.substr(textStart, braceEnd - textStart + 1));
                    i = braceEnd + 1;
                    continue;
                }
            }
        }
        
        currentUnit += ch;
        i++;
    }
    
    if (!currentUnit.empty()) {
        units.push_back(currentUnit);
    }
    
    return units;
}

std::vector<std::string> CHTLUnifiedScanner::identifyCHTLJSMinimalUnits(const std::string& content) {
    std::vector<std::string> units;
    std::string currentUnit;
    
    size_t i = 0;
    while (i < content.length()) {
        char ch = content[i];
        
        // 处理增强选择器 {{...}}
        if (i + 1 < content.length() && content.substr(i, 2) == "{{") {
            if (!currentUnit.empty()) {
                units.push_back(currentUnit);
                currentUnit.clear();
            }
            
            size_t selectorStart = i;
            i += 2; // 跳过"{{"
            
            // 查找结束标记
            while (i + 1 < content.length() && content.substr(i, 2) != "}}") {
                i++;
            }
            
            if (i + 1 < content.length()) {
                i += 2; // 跳过"}}"
                units.push_back(content.substr(selectorStart, i - selectorStart));
                continue;
            }
        }
        
        // 处理箭头操作符 ->
        if (i + 1 < content.length() && content.substr(i, 2) == "->") {
            if (!currentUnit.empty()) {
                units.push_back(currentUnit);
                currentUnit.clear();
            }
            
            units.push_back("->");
            i += 2;
            continue;
        }
        
        currentUnit += ch;
        i++;
    }
    
    if (!currentUnit.empty()) {
        units.push_back(currentUnit);
    }
    
    return units;
}

FragmentList CHTLUnifiedScanner::mergeRelatedFragments(const FragmentList& fragments) {
    FragmentList mergedFragments;
    
    if (fragments.empty()) {
        return mergedFragments;
    }
    
    // 简化的合并策略：连续的相同类型TOKEN级片段合并为COMPOSITE级片段
    FragmentPtr currentComposite = nullptr;
    std::string compositeContent;
    
    for (const auto& fragment : fragments) {
        if (fragment->level == FragmentLevel::TOKEN) {
            if (currentComposite == nullptr || currentComposite->type != fragment->type) {
                // 开始新的复合片段
                if (currentComposite != nullptr) {
                    currentComposite->content = compositeContent;
                    mergedFragments.push_back(currentComposite);
                }
                
                currentComposite = std::make_shared<CodeFragment>(
                    fragment->type, FragmentLevel::COMPOSITE, "", fragment->position);
                compositeContent = fragment->content;
            } else {
                // 合并到当前复合片段
                compositeContent += fragment->content;
            }
        } else {
            // 非TOKEN级片段直接添加
            if (currentComposite != nullptr) {
                currentComposite->content = compositeContent;
                mergedFragments.push_back(currentComposite);
                currentComposite = nullptr;
                compositeContent.clear();
            }
            
            mergedFragments.push_back(fragment);
        }
    }
    
    // 处理最后的复合片段
    if (currentComposite != nullptr) {
        currentComposite->content = compositeContent;
        mergedFragments.push_back(currentComposite);
    }
    
    return mergedFragments;
}

} // namespace CHTL
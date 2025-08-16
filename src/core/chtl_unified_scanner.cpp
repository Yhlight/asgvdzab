#include "core/chtl_unified_scanner.hpp"
#include <regex>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <set>
#include <cctype>

namespace chtl {

// ==================== MinimalUnit 实现 ====================

const std::vector<std::string>& MinimalUnit::getChtlMinimalPatterns() {
    static std::vector<std::string> patterns = {
        // CHTL模板和自定义语法（基于语法文档）
        R"(\[(?:Template|Custom)\]\s*@(?:Style|Element|Var)\s+\w+)",  // [Template] @Style Name
        R"(\[(?:Origin|Import|Namespace|Configuration)\])",          // 其他块标记
        
        // 变量组语法（正确的CHTL语法）
        R"(\w+\([^)]*\))",                     // ThemeColor(tableColor) 或 ThemeColor(tableColor = value)
        
        // 样式组和元素模板使用
        R"(@(?:Style|Element|Var)\s+\w+)",     // @Style DefaultText;
        
        // 继承和删除操作
        R"(\binherit\s+@?\w+)",                // inherit @Style ThemeColor
        R"(\bdelete\s+@?\w+)",                 // delete @Style WhiteText
        R"(\bdelete\s+[\w-]+(?:\s*,\s*[\w-]+)*)", // delete line-height, border
        
        // 插入操作
        R"(\binsert\s+(?:after|before|replace|at\s+(?:top|bottom))\s+[^{]+)", // insert after div[0]
        
        // from语句和命名空间
        R"(\w+\s+from\s+[\w./]+)",             // @Element Box from space
        
        // HTML元素语法
        R"(\b(?:html|head|body|div|span|p|a|img|ul|li|table|tr|td|th|form|input|button|h[1-6]|section|article|nav|header|footer|aside|main)\s*\{)", // div {
        
        // text节点
        R"(\btext\s*\{)",                      // text {
        
        // 属性语法
        R"(\b\w+\s*[:=]\s*[^;]+;)",            // id: box; class: welcome;
        
        // 局部样式和脚本块
        R"(\bstyle\s*\{)",                     // style {
        R"(\bscript\s*\{)",                    // script {
        
        // 生成器注释
        R"(--[^\r\n]*)",                       // -- 注释
        
        // 控制结构分隔符（不包含大括号，避免过度切割模板变量）
        R"([();,])",                           // 基本分隔符（去除大括号）
    };
    return patterns;
}

const std::vector<std::string>& MinimalUnit::getChtlJsMinimalPatterns() {
    static std::vector<std::string> patterns = {
        // CHTL JS增强选择器（核心语法）
        R"(\{\{[^}]+\}\})",                    // {{box}}, {{.box}}, {{#box}}, {{button[0]}}
        
        // CHTL JS箭头操作符组合（关键切割点！）
        R"(\{\{[^}]+\}\}\s*->)",               // {{box}}-> （作为一个完整单元）
        
        // CHTL JS特殊方法调用
        R"(->listen\s*\([^)]*\))",             // ->listen({ click: ... })
        R"(->delegate\s*\([^)]*\))",           // ->delegate({ target: ... })
        R"(\banimate\s*\([^)]*\))",            // animate({ duration: ... })
        
        // 在script块中的CHTL语法
        R"(\w+\([^)]*\))",                     // ColorGroup(primary = value) 变量组语法
        R"(\w+\s+from\s+[\w./]+)",             // from语句
        R"(\[Origin\]\s*@(?:Html|Style|JavaScript))", // 原始嵌入
        R"(--[^\r\n]*)",                       // -- 生成器注释
        
        // JavaScript标准语法（在script块中）
        R"(\bfunction\s+\w+\s*\([^)]*\))",     // function declaration
        R"(\w+\s*=\s*\([^)]*\)\s*=>)",         // arrow function
        R"(\w+\.\w+\s*\([^)]*\))",             // method call
        R"(\bnew\s+\w+\s*\([^)]*\))",          // constructor
        R"(\w+\[[\w"']+\])",                   // array/object access
        
        // JavaScript关键字和控制结构
        R"(\b(?:if|else|for|while|do|switch|case|break|continue|return|try|catch|finally|throw)\b)",
        R"(\b(?:var|let|const)\s+\w+)",        // variable declarations
        
        // 运算符和表达式（分离->以便精确切割）
        R"(->)",                               // CHTL JS箭头操作符（单独匹配）
        R"(=>)",                               // JavaScript箭头函数
        R"([+\-*/=<>!&|%^]+)",                 // 其他操作符
        R"([();,\[\]])",                       // 分隔符（去除大括号，保持{{}}完整性）
    };
    return patterns;
}

bool MinimalUnit::isCompleteChtlUnit(const std::string& fragment) {
    // 检查CHTL语法的完整性
    
    // 检查变量引用完整性
    if (fragment.find("{{") != std::string::npos) {
        size_t start = fragment.find("{{");
        size_t end = fragment.find("}}", start);
        if (end == std::string::npos) return false;
    }
    
    // 检查块标记完整性
    if (fragment.find("[") != std::string::npos) {
        size_t start = fragment.find("[");
        size_t end = fragment.find("]", start);
        if (end == std::string::npos) return false;
    }
    
    // 检查变量组语法完整性
    std::regex varGroupPattern(R"(\w+\()");
    std::sregex_iterator iter(fragment.begin(), fragment.end(), varGroupPattern);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        size_t start = iter->position() + iter->length() - 1;
        size_t parenEnd = fragment.find(")", start);
        if (parenEnd == std::string::npos) return false;
    }
    
    return true;
}

bool MinimalUnit::isCompleteChtlJsUnit(const std::string& fragment) {
    // 先检查CHTL部分的完整性
    if (!isCompleteChtlUnit(fragment)) return false;
    
    // 检查JavaScript语法的完整性
    
    // 检查函数定义完整性
    if (fragment.find("function") != std::string::npos) {
        std::regex funcPattern(R"(function\s+\w+\s*\()");
        if (std::regex_search(fragment, funcPattern)) {
            size_t braceStart = fragment.find("{");
            if (braceStart == std::string::npos) return false;
            // 检查大括号平衡
            if (!SliceValidator::areBracesBalanced(fragment.substr(braceStart))) return false;
        }
    }
    
    // 检查箭头函数完整性
    if (fragment.find("=>") != std::string::npos) {
        std::regex arrowPattern(R"(\([^)]*\)\s*=>)");
        if (std::regex_search(fragment, arrowPattern)) {
            size_t arrowPos = fragment.find("=>");
            size_t braceStart = fragment.find("{", arrowPos);
            if (braceStart != std::string::npos) {
                if (!SliceValidator::areBracesBalanced(fragment.substr(braceStart))) return false;
            }
        }
    }
    
    return true;
}

std::vector<size_t> MinimalUnit::findMinimalUnitBoundaries(const std::string& content, CodeFragmentType type) {
    std::vector<size_t> boundaries;
    boundaries.push_back(0); // 起始边界
    
    const auto& patterns = (type == CodeFragmentType::CHTL_FRAGMENT) ? 
                          getChtlMinimalPatterns() : getChtlJsMinimalPatterns();
    
    // 收集所有匹配的边界
    std::vector<std::pair<size_t, size_t>> matches; // (start, end)
    
    for (const auto& pattern : patterns) {
        std::regex regex(pattern);
        std::sregex_iterator iter(content.begin(), content.end(), regex);
        std::sregex_iterator end;
        
        for (; iter != end; ++iter) {
            const auto& match = *iter;
            matches.emplace_back(match.position(), match.position() + match.length());
        }
    }
    
    // 按起始位置排序
    std::sort(matches.begin(), matches.end());
    
    // 应用智能组合逻辑，避免过度细分
    std::set<size_t> addedBoundaries;
    addedBoundaries.insert(0);
    addedBoundaries.insert(content.length());
    
    for (const auto& match : matches) {
        std::string matchStr = content.substr(match.first, match.second - match.first);
        
        // 组合逻辑1: CHTL JS增强选择器 + 箭头操作符
        if (type == CodeFragmentType::CHTL_JS_FRAGMENT) {
            // 检查是否是增强选择器
            if (std::regex_match(matchStr, std::regex(R"(\{\{[^}]+\}\})"))) {
                size_t nextPos = match.second;
                // 跳过空白
                while (nextPos < content.length() && std::isspace(content[nextPos])) {
                    nextPos++;
                }
                // 检查是否紧跟->
                if (nextPos + 1 < content.length() && content.substr(nextPos, 2) == "->") {
                    // 组合为 {{selector}}->
                    addedBoundaries.insert(match.first);
                    addedBoundaries.insert(nextPos + 2);
                    continue;
                }
            }
        }
        
        // 组合逻辑2: 变量组语法保持完整
        if (std::regex_match(matchStr, std::regex(R"(\w+\([^)]*\))"))) {
            // ThemeColor(tableColor) 作为一个完整单元
            addedBoundaries.insert(match.first);
            addedBoundaries.insert(match.second);
            continue;
        }
        
        // 组合逻辑3: 模板和自定义块保持完整
        if (std::regex_match(matchStr, std::regex(R"(\[(?:Template|Custom|Origin|Import)\]\s*@\w+\s+\w+)"))) {
            addedBoundaries.insert(match.first);
            addedBoundaries.insert(match.second);
            continue;
        }
        
        // 默认处理：添加匹配的边界（但避免重复）
        addedBoundaries.insert(match.first);
        addedBoundaries.insert(match.second);
    }
    
    // 将set转换为vector（set已经自动排序和去重）
    boundaries.assign(addedBoundaries.begin(), addedBoundaries.end());
    
    return boundaries;
}

// ==================== CHTLUnifiedScanner 实现 ====================

CHTLUnifiedScanner::CHTLUnifiedScanner() 
    : initialSliceSize_(1024), maxExpansions_(10), debugMode_(false) {
}

std::vector<CodeFragment> CHTLUnifiedScanner::scanAndSlice(const std::string& sourceCode) {
    debugLog("开始精准代码切割，源代码长度: " + std::to_string(sourceCode.length()));
    
    // 第一阶段：基于可变长度切片的粗切割
    auto slices = performInitialSlicing(sourceCode);
    debugLog("完成初始切片，共 " + std::to_string(slices.size()) + " 个切片");
    
    std::vector<CodeFragment> allFragments;
    
    // 第二阶段：对每个切片进行处理
    for (auto& slice : slices) {
        // 提取切片内容
        std::string sliceContent = sourceCode.substr(slice.position, slice.length);
        
        // 识别片段类型
        CodeFragmentType fragmentType = identifyFragmentType(sliceContent);
        
        // 第三阶段：最小单元精确切割
        auto fragments = performMinimalUnitCutting(sliceContent, fragmentType, slice.position);
        
        // 添加到结果中
        allFragments.insert(allFragments.end(), fragments.begin(), fragments.end());
    }
    
    // 第四阶段：合并连续的相同类型片段（基于上下文）
    auto mergedFragments = mergeContextualFragments(allFragments);
    
    debugLog("切割完成，共产生 " + std::to_string(mergedFragments.size()) + " 个精确片段");
    
    return mergedFragments;
}

std::vector<SliceInfo> CHTLUnifiedScanner::performInitialSlicing(const std::string& sourceCode) {
    std::vector<SliceInfo> slices;
    size_t currentPos = 0;
    size_t totalLength = sourceCode.length();
    
    while (currentPos < totalLength) {
        SliceInfo slice;
        slice.position = currentPos;
        slice.length = std::min(initialSliceSize_, totalLength - currentPos);
        
        debugSliceInfo(slice, sourceCode);
        
        // 验证切片边界并进行扩增
        size_t expansions = 0;
        while (validateSliceBoundary(sourceCode, slice) && expansions < maxExpansions_) {
            if (!expandSlice(sourceCode, slice)) {
                break;
            }
            expansions++;
            debugLog("切片扩增 " + std::to_string(expansions) + " 次，原因: " + slice.reason);
        }
        
        slices.push_back(slice);
        currentPos = slice.position + slice.length;
    }
    
    return slices;
}

bool CHTLUnifiedScanner::validateSliceBoundary(const std::string& sourceCode, SliceInfo& slice) {
    size_t endPos = slice.position + slice.length;
    
    // 检查是否在源代码末尾
    if (endPos >= sourceCode.length()) {
        return false;
    }
    
    // 检查是否在字符串或注释中间切断
    if (SliceValidator::isInStringOrComment(sourceCode, endPos)) {
        slice.needsExpansion = true;
        slice.reason = "在字符串或注释中间切断";
        return true;
    }
    
    // 检查是否可能开始新的CHTL或CHTL JS片段
    if (mightStartNewFragment(sourceCode, endPos)) {
        slice.needsExpansion = true;
        slice.reason = "可能截断CHTL/CHTL JS片段开始";
        return true;
    }
    
    // 检查当前切片的完整性
    std::string sliceContent = sourceCode.substr(slice.position, slice.length);
    CodeFragmentType type = identifyFragmentType(sliceContent);
    
    if (type == CodeFragmentType::CHTL_FRAGMENT || type == CodeFragmentType::CHTL_JS_FRAGMENT) {
        if (!isChtlFragmentComplete(sliceContent, type)) {
            slice.needsExpansion = true;
            slice.reason = "CHTL/CHTL JS片段不完整";
            return true;
        }
    }
    
    return false;
}

bool CHTLUnifiedScanner::expandSlice(const std::string& sourceCode, SliceInfo& slice) {
    size_t totalLength = sourceCode.length();
    size_t currentEnd = slice.position + slice.length;
    
    if (currentEnd >= totalLength) {
        return false;
    }
    
    // 计算扩增大小（初始切片大小的25%）
    size_t expansionSize = initialSliceSize_ / 4;
    if (expansionSize < 64) expansionSize = 64;
    
    size_t newLength = std::min(slice.length + expansionSize, totalLength - slice.position);
    
    if (newLength == slice.length) {
        return false; // 无法再扩增
    }
    
    slice.length = newLength;
    slice.expansionSize += expansionSize;
    
    return true;
}

CodeFragmentType CHTLUnifiedScanner::identifyFragmentType(const std::string& content, const std::string& context) {
    // 优先检查上下文信息
    if (context == "local_style" || context.find("style {") != std::string::npos) {
        // 局部样式块：这里是CHTL语法，不是CSS
        return CodeFragmentType::CHTL_FRAGMENT;
    }
    
    if (context == "local_script" || context.find("script {") != std::string::npos) {
        // 局部脚本块：检查是否包含CHTL JS语法
        if (hasChtlJsSyntax(content)) {
            return CodeFragmentType::CHTL_JS_FRAGMENT;
        } else {
            return CodeFragmentType::JS_FRAGMENT;
        }
    }
    
    if (context == "global_style") {
        return CodeFragmentType::CSS_FRAGMENT;
    }
    
    // 检查原始嵌入
    if (content.find("[Origin]") != std::string::npos) {
        if (content.find("@Html") != std::string::npos) {
            return CodeFragmentType::CHTL_FRAGMENT; // HTML内容作为CHTL处理
        } else if (content.find("@Style") != std::string::npos) {
            return CodeFragmentType::CSS_FRAGMENT;
        } else if (content.find("@JavaScript") != std::string::npos) {
            return CodeFragmentType::JS_FRAGMENT;
        }
    }
    
    // 优先检查CHTL JS增强选择器语法
    // 这很重要，因为{{title}}、{{message}}等应该被识别为CHTL JS，不是CSS
    if (hasChtlJsSyntax(content)) {
        return CodeFragmentType::CHTL_JS_FRAGMENT;
    }
    
    // 检查CHTL特有语法
    if (hasChtlSyntax(content)) {
        return CodeFragmentType::CHTL_FRAGMENT;
    }
    
    // 检查纯CSS语法（全局样式）
    if (isPureCssSyntax(content)) {
        return CodeFragmentType::CSS_FRAGMENT;
    }
    
    // 检查纯JavaScript语法
    if (isPureJavaScriptSyntax(content)) {
        return CodeFragmentType::JS_FRAGMENT;
    }
    
    // 默认为CHTL片段（因为CHTL是基础语言）
    return CodeFragmentType::CHTL_FRAGMENT;
}

bool CHTLUnifiedScanner::hasChtlSyntax(const std::string& content) {
    // 基于CHTL语法文档的完整语法检查
    std::vector<std::regex> chtlPatterns = {
        // 模板和自定义语法
        std::regex(R"(\[(?:Template|Custom)\]\s*@(?:Style|Element|Var)\s+\w+)"),
        
        // 原始嵌入
        std::regex(R"(\[Origin\]\s*@(?:Html|Style|JavaScript))"),
        
        // 导入和命名空间
        std::regex(R"(\[(?:Import|Namespace|Configuration)\])"),
        
        // 样式组和元素模板使用
        std::regex(R"(@(?:Style|Element|Var)\s+\w+)"),
        
        // 变量组使用（如 ThemeColor(tableColor)）
        std::regex(R"(\w+\([^)]*\))"),
        
        // 继承和删除
        std::regex(R"(\b(?:inherit|delete)\s+@?\w+)"),
        
        // 插入操作
        std::regex(R"(\binsert\s+(?:after|before|replace|at\s+(?:top|bottom)))"),
        
        // from子句
        std::regex(R"(\w+\s+from\s+[\w./]+)"),
        
        // 生成器注释
        std::regex(R"(--.*)"),
        
        // HTML元素语法（div { }, span { }等）
        std::regex(R"(\b(?:html|head|body|div|span|p|a|img|ul|li|table|tr|td|th|form|input|button|h[1-6]|section|article|nav|header|footer|aside|main)\s*\{)"),
        
        // text节点
        std::regex(R"(\btext\s*\{)"),
        
        // 属性语法（id: value; class: value;）
        std::regex(R"(\b\w+\s*[:=]\s*[^;]+;)"),
        
        // 局部样式块
        std::regex(R"(\bstyle\s*\{)"),
        
        // 局部脚本块
        std::regex(R"(\bscript\s*\{)"),
    };
    
    for (const auto& pattern : chtlPatterns) {
        if (std::regex_search(content, pattern)) {
            return true;
        }
    }
    
    return false;
}

bool CHTLUnifiedScanner::hasChtlJsSyntax(const std::string& content) {
    // 基于CHTL语法文档的CHTL JS语法检查
    std::vector<std::regex> chtlJsPatterns = {
        // 增强选择器 {{box}}, {{.box}}, {{#box}}, {{button[0]}}
        std::regex(R"(\{\{[^}]+\}\})"),
        
        // 明确使用CHTL语法的箭头操作符 ->
        std::regex(R"(\{\{[^}]+\}\}\s*->\s*\w+)"),
        
        // listen监听器
        std::regex(R"(->listen\s*\()"),
        
        // delegate事件委托
        std::regex(R"(->delegate\s*\()"),
        
        // animate动画
        std::regex(R"(\banimate\s*\()"),
    };
    
    for (const auto& pattern : chtlJsPatterns) {
        if (std::regex_search(content, pattern)) {
            return true;
        }
    }
    
    return false;
}

bool CHTLUnifiedScanner::isPureCssSyntax(const std::string& content) {
    // 检查是否为纯CSS语法（全局样式）
    std::vector<std::regex> cssPatterns = {
        // CSS选择器和规则（但不包含CHTL的局部样式语法）
        std::regex(R"(^[^{]*\{[^}]*(?:[\w-]+\s*:\s*[^;]+;[^}]*)*\})"),
        
        // CSS @规则
        std::regex(R"(@(?:media|import|keyframes|charset|supports|page))"),
        
        // CSS伪类和伪元素（非CHTL的&语法）
        std::regex(R"(:[a-zA-Z-]+(?:\([^)]*\))?(?:\s*\{|[,\s]))"),
        std::regex(R"(::[a-zA-Z-]+(?:\s*\{|[,\s]))"),
    };
    
    // 排除CHTL语法
    if (hasChtlSyntax(content)) {
        return false;
    }
    
    for (const auto& pattern : cssPatterns) {
        if (std::regex_search(content, pattern)) {
            return true;
        }
    }
    
    return false;
}

bool CHTLUnifiedScanner::isPureJavaScriptSyntax(const std::string& content) {
    // 检查是否为纯JavaScript语法（不包含CHTL JS语法）
    std::vector<std::regex> jsPatterns = {
        // JavaScript关键字
        std::regex(R"(\b(?:function|var|let|const|if|else|for|while|return|class|new|this|typeof|instanceof)\b)"),
        
        // 箭头函数
        std::regex(R"(\w+\s*=\s*\([^)]*\)\s*=>)"),
        
        // 方法调用
        std::regex(R"(\w+\.\w+\s*\()"),
        
        // 对象字面量
        std::regex(R"(\{\s*\w+\s*:\s*[^}]+\})"),
        
        // 数组字面量
        std::regex(R"(\[[^\]]*\])"),
    };
    
    // 排除CHTL和CHTL JS语法
    if (hasChtlSyntax(content) || hasChtlJsSyntax(content)) {
        return false;
    }
    
    for (const auto& pattern : jsPatterns) {
        if (std::regex_search(content, pattern)) {
            return true;
        }
    }
    
    return false;
}

std::vector<CodeFragment> CHTLUnifiedScanner::performMinimalUnitCutting(const std::string& content, CodeFragmentType type, size_t basePos) {
    std::vector<CodeFragment> fragments;
    
    if (type == CodeFragmentType::CSS_FRAGMENT || type == CodeFragmentType::JS_FRAGMENT) {
        // CSS和JS片段不需要最小单元切割，直接返回
        CodeFragment fragment;
        fragment.type = type;
        fragment.content = content;
        fragment.startPos = basePos;
        fragment.endPos = basePos + content.length();
        fragment.startLine = calculateLineNumber(content, 0);
        fragment.endLine = calculateLineNumber(content, content.length());
        fragment.isMinimalUnit = false;
        
        fragments.push_back(fragment);
        return fragments;
    }
    
    // 对CHTL和CHTL JS进行最小单元切割
    auto boundaries = MinimalUnit::findMinimalUnitBoundaries(content, type);
    
    for (size_t i = 0; i < boundaries.size() - 1; ++i) {
        size_t start = boundaries[i];
        size_t end = boundaries[i + 1];
        
        if (start >= end) continue;
        
        std::string fragmentContent = content.substr(start, end - start);
        
        // 跳过空白片段
        if (fragmentContent.find_first_not_of(" \t\n\r") == std::string::npos) {
            continue;
        }
        
        CodeFragment fragment;
        fragment.type = type;
        fragment.content = fragmentContent;
        fragment.startPos = basePos + start;
        fragment.endPos = basePos + end;
        fragment.startLine = calculateLineNumber(content, start);
        fragment.endLine = calculateLineNumber(content, end);
        fragment.isMinimalUnit = true;
        
        fragments.push_back(fragment);
    }
    
    return fragments;
}

bool CHTLUnifiedScanner::isChtlFragmentComplete(const std::string& fragment, CodeFragmentType type) {
    if (type == CodeFragmentType::CHTL_FRAGMENT) {
        return MinimalUnit::isCompleteChtlUnit(fragment);
    } else if (type == CodeFragmentType::CHTL_JS_FRAGMENT) {
        return MinimalUnit::isCompleteChtlJsUnit(fragment);
    }
    return true;
}

bool CHTLUnifiedScanner::mightStartNewFragment(const std::string& text, size_t position) {
    if (position >= text.length()) return false;
    
    // 检查可能的CHTL语法开始模式
    std::vector<std::string> startPatterns = {
        "{{", "@", "[", "inherit", "delete", "--"
    };
    
    for (const auto& pattern : startPatterns) {
        if (position + pattern.length() <= text.length()) {
            if (text.substr(position, pattern.length()) == pattern) {
                return true;
            }
        }
    }
    
    return false;
}

std::vector<CodeFragment> CHTLUnifiedScanner::mergeContextualFragments(const std::vector<CodeFragment>& fragments) {
    if (fragments.empty()) return fragments;
    
    std::vector<CodeFragment> merged;
    CodeFragment current = fragments[0];
    
    for (size_t i = 1; i < fragments.size(); ++i) {
        const auto& next = fragments[i];
        
        if (shouldMergeWithNext(current, next)) {
            // 合并片段
            current.content += next.content;
            current.endPos = next.endPos;
            current.endLine = next.endLine;
            current.isContextDependent = true;
            
            debugLog("合并片段: " + current.content.substr(0, 50) + "...");
        } else {
            // 不合并，添加当前片段并开始新片段
            merged.push_back(current);
            current = next;
        }
    }
    
    merged.push_back(current);
    return merged;
}

bool CHTLUnifiedScanner::shouldMergeWithNext(const CodeFragment& current, const CodeFragment& next) {
    // 类型相同且都是最小单元
    if (current.type != next.type || !current.isMinimalUnit || !next.isMinimalUnit) {
        return false;
    }
    
    // 位置连续
    if (current.endPos != next.startPos) {
        return false;
    }
    
    // CHTL和CHTL JS片段的特殊合并规则
    if (current.type == CodeFragmentType::CHTL_FRAGMENT || current.type == CodeFragmentType::CHTL_JS_FRAGMENT) {
        // 检查是否应该保持分离（例如，操作符前后）
        std::string currentTrimmed = current.content;
        std::string nextTrimmed = next.content;
        
        // 去除前后空白
        currentTrimmed.erase(0, currentTrimmed.find_first_not_of(" \t\n\r"));
        currentTrimmed.erase(currentTrimmed.find_last_not_of(" \t\n\r") + 1);
        nextTrimmed.erase(0, nextTrimmed.find_first_not_of(" \t\n\r"));
        nextTrimmed.erase(nextTrimmed.find_last_not_of(" \t\n\r") + 1);
        
        // 特殊分离规则：操作符独立
        if (currentTrimmed == "->" || nextTrimmed == "->" ||
            currentTrimmed == "++" || nextTrimmed == "++" ||
            currentTrimmed == "--" || nextTrimmed == "--") {
            return false;
        }
        
        // 变量引用应该保持完整
        if (currentTrimmed.find("{{") != std::string::npos && nextTrimmed.find("}}") != std::string::npos) {
            return true;
        }
    }
    
    return false;
}

size_t CHTLUnifiedScanner::calculateLineNumber(const std::string& content, size_t position) {
    if (position >= content.length()) {
        return std::count(content.begin(), content.end(), '\n') + 1;
    }
    
    return std::count(content.begin(), content.begin() + position, '\n') + 1;
}

void CHTLUnifiedScanner::debugLog(const std::string& message) {
    if (debugMode_) {
        std::cout << "[CHTLUnifiedScanner] " << message << std::endl;
    }
}

void CHTLUnifiedScanner::debugSliceInfo(const SliceInfo& slice, const std::string& content) {
    if (debugMode_) {
        std::string preview = content.substr(slice.position, std::min(slice.length, size_t(50)));
        std::replace(preview.begin(), preview.end(), '\n', ' ');
        debugLog("切片 [" + std::to_string(slice.position) + ":" + 
                std::to_string(slice.position + slice.length) + "] \"" + preview + "...\"");
    }
}

// ==================== SliceValidator 实现 ====================

bool SliceValidator::validateChtlFragment(const std::string& fragment) {
    return MinimalUnit::isCompleteChtlUnit(fragment);
}

bool SliceValidator::validateChtlJsFragment(const std::string& fragment) {
    return MinimalUnit::isCompleteChtlJsUnit(fragment);
}

bool SliceValidator::isInStringOrComment(const std::string& content, size_t position) {
    if (position >= content.length()) return false;
    
    bool inSingleString = false;
    bool inDoubleString = false;
    bool inTemplateString = false;
    bool inSingleComment = false;
    bool inMultiComment = false;
    bool escaped = false;
    
    for (size_t i = 0; i < position; ++i) {
        char ch = content[i];
        
        if (escaped) {
            escaped = false;
            continue;
        }
        
        if (inSingleComment) {
            if (ch == '\n' || ch == '\r') {
                inSingleComment = false;
            }
            continue;
        }
        
        if (inMultiComment) {
            if (ch == '*' && i + 1 < content.length() && content[i + 1] == '/') {
                inMultiComment = false;
                i++; // 跳过 '/'
            }
            continue;
        }
        
        if (ch == '\\' && (inSingleString || inDoubleString || inTemplateString)) {
            escaped = true;
            continue;
        }
        
        if (!inSingleString && !inDoubleString && !inTemplateString) {
            if (ch == '/' && i + 1 < content.length()) {
                if (content[i + 1] == '/') {
                    inSingleComment = true;
                    i++; // 跳过第二个 '/'
                    continue;
                } else if (content[i + 1] == '*') {
                    inMultiComment = true;
                    i++; // 跳过 '*'
                    continue;
                }
            }
        }
        
        if (!inSingleComment && !inMultiComment) {
            if (ch == '\'' && !inDoubleString && !inTemplateString) {
                inSingleString = !inSingleString;
            } else if (ch == '"' && !inSingleString && !inTemplateString) {
                inDoubleString = !inDoubleString;
            } else if (ch == '`' && !inSingleString && !inDoubleString) {
                inTemplateString = !inTemplateString;
            }
        }
    }
    
    return inSingleString || inDoubleString || inTemplateString || inSingleComment || inMultiComment;
}

bool SliceValidator::areBracesBalanced(const std::string& fragment) {
    int braceCount = 0;
    bool inString = false;
    bool escaped = false;
    
    for (char ch : fragment) {
        if (escaped) {
            escaped = false;
            continue;
        }
        
        if (ch == '\\' && inString) {
            escaped = true;
            continue;
        }
        
        if (ch == '"' || ch == '\'' || ch == '`') {
            inString = !inString;
            continue;
        }
        
        if (!inString) {
            if (ch == '{') {
                braceCount++;
            } else if (ch == '}') {
                braceCount--;
            }
        }
    }
    
    return braceCount == 0;
}

bool SliceValidator::areParenthesesBalanced(const std::string& fragment) {
    int parenCount = 0;
    bool inString = false;
    bool escaped = false;
    
    for (char ch : fragment) {
        if (escaped) {
            escaped = false;
            continue;
        }
        
        if (ch == '\\' && inString) {
            escaped = true;
            continue;
        }
        
        if (ch == '"' || ch == '\'' || ch == '`') {
            inString = !inString;
            continue;
        }
        
        if (!inString) {
            if (ch == '(') {
                parenCount++;
            } else if (ch == ')') {
                parenCount--;
            }
        }
    }
    
    return parenCount == 0;
}

} // namespace chtl
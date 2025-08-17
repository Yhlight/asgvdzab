#include "scanner/unified_scanner.h"
#include <algorithm>
#include <cctype>
#include <iostream>

namespace chtl {

CHTLUnifiedScanner::CHTLUnifiedScanner(const ScannerConfig& config) 
    : config_(config) {
}

CHTLUnifiedScanner::~CHTLUnifiedScanner() = default;

std::vector<CodeFragment> CHTLUnifiedScanner::scan(const std::string& code) {
    ScanContext ctx;
    ctx.content = code;
    ctx.position = 0;
    ctx.line_number = 1;
    
    scanLoop(ctx);
    mergeAdjacentFragments(ctx.fragments);
    
    return ctx.fragments;
}

void CHTLUnifiedScanner::scanLoop(ScanContext& ctx) {
    while (ctx.position < ctx.content.size()) {
        // 获取下一个切片
        SliceInfo slice = getNextSlice(ctx);
        
        if (config_.enable_intelligent_slicing) {
            std::cout << "[Debug] Initial slice: " << slice.start << "-" << slice.end 
                     << " (size: " << slice.end - slice.start << ")\n";
        }
        
        // 检查切片边界是否合理
        if (!isSliceBoundaryValid(ctx, slice.end)) {
            // 扩展切片以包含完整的语法单元
            slice = expandSlice(ctx, slice);
            if (config_.enable_intelligent_slicing) {
                std::cout << "[Debug] Expanded slice to: " << slice.start << "-" << slice.end 
                         << " (size: " << slice.end - slice.start << ")\n";
            }
        }
        
        // 二次切割：将切片细分为最小语法单元
        std::vector<SliceInfo> subslices = subdivideSlice(ctx, slice);
        
        if (config_.enable_intelligent_slicing) {
            std::cout << "[Debug] Subdivided into " << subslices.size() << " subslices\n";
        }
        
        // 创建片段
        for (const auto& subslice : subslices) {
            if (subslice.start < subslice.end) {
                CodeFragment fragment = createFragment(ctx, subslice.start, 
                                                     subslice.end, subslice.type);
                ctx.fragments.push_back(fragment);
                
                if (config_.enable_intelligent_slicing) {
                    std::cout << "[Debug] Created fragment type " << static_cast<int>(subslice.type) 
                             << " at " << subslice.start << "-" << subslice.end << "\n";
                }
            }
        }
        
        // 更新位置
        ctx.position = slice.end;
    }
}

SliceInfo CHTLUnifiedScanner::getNextSlice(const ScanContext& ctx) {
    SliceInfo slice;
    slice.start = ctx.position;
    
    // 计算切片大小
    size_t remaining = ctx.content.size() - ctx.position;
    size_t slice_size = std::min(config_.initial_slice_size, remaining);
    
    slice.end = ctx.position + slice_size;
    slice.type = detectFragmentType(ctx.content, slice.start, slice.end);
    
    return slice;
}

bool CHTLUnifiedScanner::isSliceBoundaryValid(const ScanContext& ctx, size_t boundary) {
    if (boundary >= ctx.content.size()) {
        return true; // 文件结束总是有效边界
    }
    
    // 检查是否在字符串中
    int quote_count = 0;
    bool in_string = false;
    for (size_t i = ctx.slice_start; i < boundary; ++i) {
        if (ctx.content[i] == '"' || ctx.content[i] == '\'') {
            if (i == 0 || ctx.content[i-1] != '\\') {
                quote_count++;
            }
        }
    }
    in_string = (quote_count % 2 == 1);
    
    if (in_string) {
        return false; // 不能在字符串中间切割
    }
    
    // 检查是否在注释中
    if (boundary > 0 && ctx.content[boundary-1] == '/') {
        if (boundary < ctx.content.size() && 
            (ctx.content[boundary] == '/' || ctx.content[boundary] == '*')) {
            return false; // 不能在注释开始处切割
        }
    }
    
    // 检查是否在CHTL JS增强语法中
    if (isCHTLJSEnhancedSyntax(ctx.content, boundary)) {
        return false;
    }
    
    // 检查是否在代码块中间
    int brace_depth = 0;
    for (size_t i = ctx.slice_start; i < boundary; ++i) {
        if (ctx.content[i] == '{') brace_depth++;
        else if (ctx.content[i] == '}') brace_depth--;
    }
    
    if (brace_depth != 0) {
        // 在代码块中，检查边界是否合理
        if (boundary < ctx.content.size()) {
            char ch = ctx.content[boundary];
            // 允许在某些字符后切割
            if (ch == '}' || ch == ';' || ch == '\n') {
                return true;
            }
            return false;
        }
    }
    
    return true;
}

SliceInfo CHTLUnifiedScanner::expandSlice(const ScanContext& ctx, SliceInfo slice) {
    // 向前扩展，直到找到有效边界
    size_t new_end = slice.end;
    size_t max_expansion = std::min(slice.end + config_.max_slice_size - (slice.end - slice.start),
                                   ctx.content.size());
    
    while (new_end < max_expansion) {
        if (isSliceBoundaryValid(ctx, new_end)) {
            break;
        }
        
        // 查找下一个可能的边界点
        if (ctx.content[new_end] == '}' || ctx.content[new_end] == ';') {
            new_end++;
            if (isSliceBoundaryValid(ctx, new_end)) {
                break;
            }
        }
        
        new_end++;
    }
    
    slice.end = new_end;
    slice.needs_expansion = true;
    
    return slice;
}

std::vector<SliceInfo> CHTLUnifiedScanner::subdivideSlice(const ScanContext& ctx, 
                                                         const SliceInfo& slice) {
    std::vector<SliceInfo> subslices;
    size_t pos = slice.start;
    
    if (config_.enable_intelligent_slicing) {
        std::cout << "[Debug] subdivideSlice called for slice " << slice.start << "-" << slice.end << "\n";
    }
    
    while (pos < slice.end) {
        // 跳过空白
        pos = skipWhitespace(ctx.content, pos);
        if (pos >= slice.end) break;
        
        if (config_.enable_intelligent_slicing) {
            std::cout << "[Debug] subdivide at pos " << pos << ", looking at: " 
                     << ctx.content.substr(pos, std::min(size_t(20), slice.end - pos)) << "\n";
        }
        
        SliceInfo subslice;
        subslice.start = pos;
        
        // 检测片段类型
        CodeFragmentType type = detectFragmentType(ctx.content, pos, slice.end);
        subslice.type = type;
        
        // 根据类型查找单元结束位置
        if (type == CodeFragmentType::CHTL) {
            if (config_.enable_intelligent_slicing) {
                std::cout << "[Debug] Checking for special blocks at pos " << pos << "\n";
                std::cout << "[Debug] lookAhead(script) = " << lookAhead(ctx.content, pos, "script") << "\n";
            }
            // 检查特殊块
            if (lookAhead(ctx.content, pos, "script") && pos + 6 < slice.end) {
                if (config_.enable_intelligent_slicing) {
                    std::cout << "[Debug] Found 'script' at position " << pos << "\n";
                }
                // 首先添加script标签本身作为CHTL片段
                size_t script_start = pos;
                pos += 6; // 跳过 "script"
                pos = skipWhitespace(ctx.content, pos);
                
                if (pos < slice.end && ctx.content[pos] == '{') {
                    // 添加 "script {" 作为CHTL片段
                    subslice.end = pos + 1;
                    subslice.type = CodeFragmentType::CHTL;
                    subslices.push_back(subslice);
                    
                    // 查找script块的内容
                    size_t content_start = pos + 1;
                    size_t script_end = scanScriptBlock(ctx.content, script_start);
                    
                    if (script_end > content_start + 1) {
                        // 添加script内容作为CHTL_JS片段
                        SliceInfo js_slice;
                        js_slice.start = content_start;
                        js_slice.end = std::min(script_end - 1, slice.end); // 不包含结束的 }
                        js_slice.type = CodeFragmentType::CHTL_JS;
                        subslices.push_back(js_slice);
                        
                        // 添加结束的 } 作为CHTL片段
                        SliceInfo end_slice;
                        end_slice.start = script_end - 1;
                        end_slice.end = script_end;
                        end_slice.type = CodeFragmentType::CHTL;
                        subslices.push_back(end_slice);
                        
                        pos = script_end;
                        continue;
                    }
                }
                
                // 如果没有正确的script块结构，作为普通CHTL处理
                subslice.end = scanScriptBlock(ctx.content, script_start);
            } else if (lookAhead(ctx.content, pos, "style") && pos + 5 < slice.end) {
                // 处理style块，类似script块
                size_t style_start = pos;
                pos += 5; // 跳过 "style"
                pos = skipWhitespace(ctx.content, pos);
                
                if (pos < slice.end && ctx.content[pos] == '{') {
                    // 添加 "style {" 作为CHTL片段
                    subslice.end = pos + 1;
                    subslice.type = CodeFragmentType::CHTL;
                    subslices.push_back(subslice);
                    
                    // 查找style块的内容
                    size_t content_start = pos + 1;
                    size_t style_end = scanStyleBlock(ctx.content, style_start);
                    
                    if (style_end > content_start + 1) {
                        // 添加style内容作为CSS片段
                        SliceInfo css_slice;
                        css_slice.start = content_start;
                        css_slice.end = style_end - 1; // 不包含结束的 }
                        css_slice.type = CodeFragmentType::CSS;
                        subslices.push_back(css_slice);
                        
                        // 添加结束的 } 作为CHTL片段
                        SliceInfo end_slice;
                        end_slice.start = style_end - 1;
                        end_slice.end = style_end;
                        end_slice.type = CodeFragmentType::CHTL;
                        subslices.push_back(end_slice);
                        
                        pos = style_end;
                        continue;
                    }
                }
                
                // 如果没有正确的style块结构，作为普通CHTL处理
                subslice.end = scanStyleBlock(ctx.content, style_start);
            } else if (lookAhead(ctx.content, pos, "[Origin]")) {
                size_t origin_end = scanOriginBlock(ctx.content, pos);
                subslice.end = origin_end;
                subslice.type = CodeFragmentType::HTML_RAW;
            } else {
                subslice.end = findCHTLUnitEnd(ctx.content, pos);
            }
        } else if (type == CodeFragmentType::CHTL_JS) {
            subslice.end = findCHTLJSUnitEnd(ctx.content, pos);
        } else {
            // 其他类型，找到下一个边界
            subslice.end = slice.end;
        }
        
        // 确保不超出切片范围
        subslice.end = std::min(subslice.end, slice.end);
        
        if (subslice.start < subslice.end) {
            subslices.push_back(subslice);
        }
        
        pos = subslice.end;
    }
    
    return subslices;
}

CodeFragmentType CHTLUnifiedScanner::detectFragmentType(const std::string& content, 
                                                       size_t start, size_t end) {
    size_t pos = skipWhitespace(content, start);
    if (pos >= end) return CodeFragmentType::CHTL;
    
    // 检查是否是CHTL JS增强语法
    if (isCHTLJSEnhancedSyntax(content, pos)) {
        return CodeFragmentType::CHTL_JS;
    }
    
    // 默认为CHTL
    return CodeFragmentType::CHTL;
}

bool CHTLUnifiedScanner::isCHTLJSEnhancedSyntax(const std::string& content, size_t pos) {
    // 检查 {{ 选择器
    if (lookAhead(content, pos, "{{")) {
        return true;
    }
    
    // 检查 -> 箭头操作符
    if (lookAhead(content, pos, "->")) {
        return true;
    }
    
    // 检查特殊函数
    if (lookAhead(content, pos, "listen(") || 
        lookAhead(content, pos, "delegate(") ||
        lookAhead(content, pos, "animate(")) {
        return true;
    }
    
    // 检查 vir 关键字
    if (lookAhead(content, pos, "vir ")) {
        return true;
    }
    
    return false;
}

size_t CHTLUnifiedScanner::findCHTLUnitEnd(const std::string& content, size_t start) {
    size_t pos = start;
    int brace_depth = 0;
    
    while (pos < content.size()) {
        char ch = content[pos];
        
        // 处理字符串
        if (ch == '"' || ch == '\'') {
            pos = skipString(content, pos, ch);
            continue;
        }
        
        // 处理注释
        if (ch == '/' && pos + 1 < content.size()) {
            if (content[pos + 1] == '/' || content[pos + 1] == '*') {
                pos = skipComment(content, pos);
                continue;
            }
        }
        
        // 处理大括号
        if (ch == '{') {
            brace_depth++;
        } else if (ch == '}') {
            brace_depth--;
            if (brace_depth == 0) {
                return pos + 1; // 包含闭合大括号
            }
        }
        
        // 在顶层，分号或闭合大括号是单元结束
        if (brace_depth == 0 && (ch == ';' || ch == '}')) {
            return pos + 1;
        }
        
        pos++;
    }
    
    return pos;
}

size_t CHTLUnifiedScanner::findCHTLJSUnitEnd(const std::string& content, size_t start) {
    size_t pos = start;
    
    // 处理 {{ }} 选择器
    if (lookAhead(content, pos, "{{")) {
        pos += 2;
        while (pos < content.size() - 1) {
            if (content[pos] == '}' && content[pos + 1] == '}') {
                return pos + 2;
            }
            pos++;
        }
    }
    
    // 处理 -> 操作符
    if (lookAhead(content, pos, "->")) {
        pos += 2;
        // 继续到下一个边界
        while (pos < content.size() && isIdentifierChar(content[pos])) {
            pos++;
        }
        return pos;
    }
    
    // 处理特殊函数调用
    if (lookAhead(content, pos, "listen(") || 
        lookAhead(content, pos, "delegate(") ||
        lookAhead(content, pos, "animate(")) {
        // 找到函数名结束
        while (pos < content.size() && isIdentifierChar(content[pos])) {
            pos++;
        }
        
        // 处理参数列表
        if (pos < content.size() && content[pos] == '(') {
            int paren_depth = 1;
            pos++;
            
            while (pos < content.size() && paren_depth > 0) {
                if (content[pos] == '"' || content[pos] == '\'') {
                    pos = skipString(content, pos, content[pos]);
                } else if (content[pos] == '(') {
                    paren_depth++;
                    pos++;
                } else if (content[pos] == ')') {
                    paren_depth--;
                    pos++;
                } else {
                    pos++;
                }
            }
        }
        
        return pos;
    }
    
    // 默认：找到下一个语句结束
    return findCHTLUnitEnd(content, start);
}

void CHTLUnifiedScanner::mergeAdjacentFragments(std::vector<CodeFragment>& fragments) {
    if (fragments.size() < 2) return;
    
    std::vector<CodeFragment> merged;
    merged.push_back(fragments[0]);
    
    for (size_t i = 1; i < fragments.size(); ++i) {
        CodeFragment& last = merged.back();
        const CodeFragment& current = fragments[i];
        
        // 合并相邻的相同类型片段
        if (last.type == current.type && 
            last.location.end_line == current.location.start_line) {
            // 扩展最后一个片段
            last.content += current.content;
            last.location.end_line = current.location.end_line;
            last.location.end_column = current.location.end_column;
        } else {
            merged.push_back(current);
        }
    }
    
    fragments = std::move(merged);
}

// 特殊块扫描实现
size_t CHTLUnifiedScanner::scanScriptBlock(const std::string& content, size_t start) {
    size_t pos = start;
    
    // 跳过 "script"
    pos += 6;
    pos = skipWhitespace(content, pos);
    
    // 查找 {
    if (pos < content.size() && content[pos] == '{') {
        pos++;
        int brace_depth = 1;
        
        while (pos < content.size() && brace_depth > 0) {
            // 在script块内，需要处理字符串和注释
            if (content[pos] == '"' || content[pos] == '\'') {
                pos = skipString(content, pos, content[pos]);
            } else if (content[pos] == '/' && pos + 1 < content.size()) {
                if (content[pos + 1] == '/' || content[pos + 1] == '*') {
                    pos = skipComment(content, pos);
                } else {
                    pos++;
                }
            } else if (content[pos] == '{') {
                brace_depth++;
                pos++;
            } else if (content[pos] == '}') {
                brace_depth--;
                pos++;
            } else {
                pos++;
            }
        }
    }
    
    return pos;
}

size_t CHTLUnifiedScanner::scanStyleBlock(const std::string& content, size_t start) {
    size_t pos = start;
    
    // 跳过 "style"
    pos += 5;
    pos = skipWhitespace(content, pos);
    
    // 查找 {
    if (pos < content.size() && content[pos] == '{') {
        pos++;
        int brace_depth = 1;
        
        while (pos < content.size() && brace_depth > 0) {
            // 在style块内，处理CSS语法
            if (content[pos] == '"' || content[pos] == '\'') {
                pos = skipString(content, pos, content[pos]);
            } else if (content[pos] == '/' && pos + 1 < content.size() && content[pos + 1] == '*') {
                pos = skipComment(content, pos);
            } else if (content[pos] == '{') {
                brace_depth++;
                pos++;
            } else if (content[pos] == '}') {
                brace_depth--;
                pos++;
            } else {
                pos++;
            }
        }
    }
    
    return pos;
}

size_t CHTLUnifiedScanner::scanOriginBlock(const std::string& content, size_t start) {
    size_t pos = start;
    
    // 跳过 "[Origin]"
    pos += 8;
    pos = skipWhitespace(content, pos);
    
    // 查找 {
    if (pos < content.size() && content[pos] == '{') {
        pos++;
        int brace_depth = 1;
        
        // Origin块内的内容被视为原始HTML，不进行解析
        while (pos < content.size() && brace_depth > 0) {
            if (content[pos] == '{') {
                brace_depth++;
            } else if (content[pos] == '}') {
                brace_depth--;
            }
            pos++;
        }
    }
    
    return pos;
}

// 工具函数实现
bool CHTLUnifiedScanner::isWhitespace(char c) const {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool CHTLUnifiedScanner::isIdentifierChar(char c) const {
    return std::isalnum(c) || c == '_' || c == '-' || c == '$';
}

bool CHTLUnifiedScanner::lookAhead(const std::string& content, size_t pos, 
                                   const std::string& target) const {
    if (pos + target.length() > content.size()) {
        return false;
    }
    
    return content.substr(pos, target.length()) == target;
}

size_t CHTLUnifiedScanner::skipWhitespace(const std::string& content, size_t pos) const {
    while (pos < content.size() && isWhitespace(content[pos])) {
        pos++;
    }
    return pos;
}

size_t CHTLUnifiedScanner::skipString(const std::string& content, size_t start, char quote) {
    size_t pos = start + 1; // 跳过开始引号
    
    while (pos < content.size()) {
        if (content[pos] == quote) {
            return pos + 1; // 包含结束引号
        }
        if (content[pos] == '\\' && pos + 1 < content.size()) {
            pos += 2; // 跳过转义字符
        } else {
            pos++;
        }
    }
    
    return pos;
}

size_t CHTLUnifiedScanner::skipComment(const std::string& content, size_t start) {
    if (start + 1 >= content.size()) return start;
    
    if (content[start] == '/' && content[start + 1] == '/') {
        // 单行注释
        size_t pos = start + 2;
        while (pos < content.size() && content[pos] != '\n') {
            pos++;
        }
        return pos;
    } else if (content[start] == '/' && content[start + 1] == '*') {
        // 多行注释
        size_t pos = start + 2;
        while (pos + 1 < content.size()) {
            if (content[pos] == '*' && content[pos + 1] == '/') {
                return pos + 2;
            }
            pos++;
        }
        return content.size();
    }
    
    return start;
}

CodeFragment CHTLUnifiedScanner::createFragment(const ScanContext& ctx, size_t start, 
                                               size_t end, CodeFragmentType type) {
    CodeFragment fragment;
    fragment.type = type;
    fragment.content = ctx.content.substr(start, end - start);
    
    // 计算行号
    int line_number = 1;
    for (size_t i = 0; i < start; ++i) {
        if (ctx.content[i] == '\n') {
            line_number++;
        }
    }
    
    fragment.location.start_line = line_number;
    fragment.location.start_column = 1;
    
    // 计算结束行号
    for (size_t i = start; i < end; ++i) {
        if (ctx.content[i] == '\n') {
            line_number++;
        }
    }
    
    fragment.location.end_line = line_number;
    fragment.location.end_column = 1;
    
    return fragment;
}

} // namespace chtl
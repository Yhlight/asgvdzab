#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace chtl {

/**
 * 代码片段类型枚举
 * 明确分为4个独立编译器处理的类型
 */
enum class CodeSegmentType {
    // === CHTL编译器处理 (手写) ===
    CHTL_CORE,          // CHTL核心语法：模板、自定义、配置、命名空间、导入等
    CHTL_TEMPLATE,      // [Template] 块
    CHTL_CUSTOM,        // [Custom] 块  
    CHTL_CONFIGURATION, // [Configuration] 块
    CHTL_NAMESPACE,     // [Namespace] 块
    CHTL_IMPORT,        // [Import] 块
    CHTL_TEXT,          // text { } 块
    
    // === CHTL JS编译器处理 (手写) ===
    CHTL_JS_SCRIPT,     // script块中的CHTL JS代码（包含{{}}、->、listen、delegate、animate）
    
    // === CSS编译器处理 (ANTLR) ===
    CSS_GLOBAL,         // 全局CSS文件
    CSS_LOCAL_STYLE,    // style块中的纯CSS代码
    CSS_ORIGIN,         // [Origin] @Style块
    
    // === JavaScript编译器处理 (ANTLR) ===
    JAVASCRIPT_STANDARD, // 标准JavaScript代码
    JAVASCRIPT_ORIGIN,   // [Origin] @JavaScript块
    
    // === 原始嵌入处理 ===
    ORIGIN_HTML,        // [Origin] @Html块
    
    // === 其他 ===
    COMMENT,            // 注释
    UNKNOWN             // 未知类型
};

/**
 * 代码片段位置信息
 */
struct Position {
    size_t line;        // 行号（从1开始）
    size_t column;      // 列号（从1开始）
    size_t offset;      // 在源文件中的字符偏移量

    Position(size_t l = 1, size_t c = 1, size_t o = 0) 
        : line(l), column(c), offset(o) {}
};

/**
 * 代码片段范围
 */
struct Range {
    Position start;     // 开始位置
    Position end;       // 结束位置

    Range() = default;
    Range(const Position& s, const Position& e) : start(s), end(e) {}
};

/**
 * 代码片段
 */
struct CodeSegment {
    CodeSegmentType type;       // 片段类型
    std::string content;        // 片段内容
    Range range;                // 位置范围
    std::string context;        // 上下文信息（如所属元素名等）
    std::unordered_map<std::string, std::string> attributes; // 附加属性

    CodeSegment(CodeSegmentType t = CodeSegmentType::UNKNOWN) 
        : type(t) {}
    
    CodeSegment(CodeSegmentType t, const std::string& c, const Range& r) 
        : type(t), content(c), range(r) {}
};

/**
 * 解析错误信息
 */
struct ParseError {
    std::string message;        // 错误消息
    Position position;          // 错误位置
    std::string filename;       // 文件名
    
    ParseError() = default;
    ParseError(const std::string& msg, const Position& pos, const std::string& file = "")
        : message(msg), position(pos), filename(file) {}
};

/**
 * 解析警告信息
 */
struct ParseWarning {
    std::string message;        // 警告消息
    Position position;          // 警告位置
    std::string filename;       // 文件名
    
    ParseWarning() = default;
    ParseWarning(const std::string& msg, const Position& pos, const std::string& file = "")
        : message(msg), position(pos), filename(file) {}
};

// 前向声明
class CHTLASTNode;
using CHTLASTNodePtr = std::shared_ptr<CHTLASTNode>;

/**
 * 解析结果
 */
struct ParseResult {
    bool success = false;                   // 解析是否成功
    CHTLASTNodePtr rootNode = nullptr;      // AST根节点
    std::vector<ParseError> errors;         // 错误列表
    std::vector<ParseWarning> warnings;     // 警告列表
};

/**
 * 扫描结果
 */
struct ScanResult {
    std::vector<CodeSegment> segments;      // 代码片段列表
    std::vector<std::string> errors;        // 错误信息
    std::vector<std::string> warnings;      // 警告信息
    bool success;                           // 扫描是否成功

    ScanResult() : success(true) {}
};

/**
 * 编译配置
 */
struct CompileConfig {
    // 索引起始计数
    int indexInitialCount = 0;
    
    // 是否禁用名称配置组
    bool disableNameGroup = false;
    
    // Debug模式
    bool debugMode = false;
    
    // 自定义关键字映射
    std::unordered_map<std::string, std::vector<std::string>> customKeywords;
    
    // 选项数量限制
    int optionCount = 3;

    CompileConfig() {
        // 初始化默认关键字
        customKeywords["CUSTOM_STYLE"] = {"@Style", "@style", "@CSS", "@Css", "@css"};
        customKeywords["CUSTOM_ELEMENT"] = {"@Element"};
        customKeywords["CUSTOM_VAR"] = {"@Var"};
        customKeywords["TEMPLATE_STYLE"] = {"@Style"};
        customKeywords["TEMPLATE_ELEMENT"] = {"@Element"};
        customKeywords["TEMPLATE_VAR"] = {"@Var"};
        customKeywords["ORIGIN_HTML"] = {"@Html"};
        customKeywords["ORIGIN_STYLE"] = {"@Style"};
        customKeywords["ORIGIN_JAVASCRIPT"] = {"@JavaScript"};
        customKeywords["IMPORT_HTML"] = {"@Html"};
        customKeywords["IMPORT_STYLE"] = {"@Style"};
        customKeywords["IMPORT_JAVASCRIPT"] = {"@JavaScript"};
        customKeywords["IMPORT_CHTL"] = {"@Chtl"};
        customKeywords["IMPORT_CJMOD"] = {"@CJmod"};
        customKeywords["KEYWORD_INHERIT"] = {"inherit"};
        customKeywords["KEYWORD_DELETE"] = {"delete"};
        customKeywords["KEYWORD_INSERT"] = {"insert"};
        customKeywords["KEYWORD_AFTER"] = {"after"};
        customKeywords["KEYWORD_BEFORE"] = {"before"};
        customKeywords["KEYWORD_REPLACE"] = {"replace"};
        customKeywords["KEYWORD_ATTOP"] = {"at top"};
        customKeywords["KEYWORD_ATBOTTOM"] = {"at bottom"};
        customKeywords["KEYWORD_FROM"] = {"from"};
        customKeywords["KEYWORD_AS"] = {"as"};
        customKeywords["KEYWORD_EXCEPT"] = {"except"};
        customKeywords["KEYWORD_TEXT"] = {"text"};
        customKeywords["KEYWORD_STYLE"] = {"style"};
        customKeywords["KEYWORD_SCRIPT"] = {"script"};
        customKeywords["KEYWORD_CUSTOM"] = {"[Custom]"};
        customKeywords["KEYWORD_TEMPLATE"] = {"[Template]"};
        customKeywords["KEYWORD_ORIGIN"] = {"[Origin]"};
        customKeywords["KEYWORD_IMPORT"] = {"[Import]"};
        customKeywords["KEYWORD_NAMESPACE"] = {"[Namespace]"};
    }
};

/**
 * 将CodeSegmentType转换为字符串
 */
std::string toString(CodeSegmentType type);

/**
 * 从字符串解析CodeSegmentType
 */
CodeSegmentType parseCodeSegmentType(const std::string& str);

} // namespace chtl
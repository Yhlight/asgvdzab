#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <variant>
#include <optional>

namespace CHTL {
namespace Core {

// 前向声明
class Token;
class ASTNode;
class Context;
class State;

// 智能指针类型定义
using TokenPtr = std::shared_ptr<Token>;
using ASTNodePtr = std::shared_ptr<ASTNode>;
using ContextPtr = std::shared_ptr<Context>;
using StatePtr = std::shared_ptr<State>;

// 基础类型定义
using String = std::string;
using StringView = std::string_view;
using StringList = std::vector<String>;
using StringMap = std::unordered_map<String, String>;

// 代码片段类型枚举
enum class FragmentType {
    CHTL,           // CHTL片段
    CHTL_JS,        // CHTL JS片段  
    CSS,            // CSS片段
    JAVASCRIPT,     // JavaScript片段
    HTML,           // HTML片段
    UNKNOWN         // 未知片段
};

// Token类型枚举
enum class TokenType {
    // 基础token
    IDENTIFIER,     // 标识符
    STRING_LITERAL, // 字符串字面量
    NUMBER_LITERAL, // 数字字面量
    COMMENT,        // 注释
    
    // 分隔符
    LBRACE,         // {
    RBRACE,         // }
    LPAREN,         // (
    RPAREN,         // )
    LBRACKET,       // [
    RBRACKET,       // ]
    SEMICOLON,      // ;
    COLON,          // :
    COMMA,          // ,
    DOT,            // .
    ARROW,          // ->
    EQUAL,          // =
    
    // CHTL关键字
    TEXT,           // text
    STYLE,          // style
    SCRIPT,         // script
    
    // 模板和自定义关键字
    TEMPLATE,       // [Template]
    CUSTOM,         // [Custom]
    ORIGIN,         // [Origin]
    IMPORT,         // [Import]
    NAMESPACE,      // [Namespace]
    CONFIGURATION,  // [Configuration]
    
    // 类型关键字
    AT_STYLE,       // @Style
    AT_ELEMENT,     // @Element
    AT_VAR,         // @Var
    AT_HTML,        // @Html
    AT_JAVASCRIPT,  // @JavaScript
    AT_CHTL,        // @Chtl
    AT_CJMOD,       // @CJmod
    
    // 特殊关键字
    INHERIT,        // inherit
    DELETE,         // delete
    INSERT,         // insert
    FROM,           // from
    AS,             // as
    EXCEPT,         // except
    
    // 位置关键字
    AFTER,          // after
    BEFORE,         // before
    REPLACE,        // replace
    AT_TOP,         // at top
    AT_BOTTOM,      // at bottom
    
    // CHTL JS特殊token
    VIR,            // vir (虚对象)
    LISTEN,         // listen
    ANIMATE,        // animate
    DELEGATE,       // delegate
    SELECTOR_START, // {{
    SELECTOR_END,   // }}
    
    // 文件结束
    EOF_TOKEN,
    
    // 错误token
    ERROR
};

// AST节点类型枚举
enum class ASTNodeType {
    // 根节点
    ROOT,
    DOCUMENT,
    
    // 基础节点
    ELEMENT,        // HTML元素
    TEXT_NODE,      // 文本节点
    ATTRIBUTE,      // 属性
    
    // CHTL特殊节点
    STYLE_BLOCK,    // 样式块
    SCRIPT_BLOCK,   // 脚本块
    
    // 模板节点
    TEMPLATE_STYLE, // 样式模板
    TEMPLATE_ELEMENT, // 元素模板
    TEMPLATE_VAR,   // 变量模板
    
    // 自定义节点
    CUSTOM_STYLE,   // 自定义样式
    CUSTOM_ELEMENT, // 自定义元素
    CUSTOM_VAR,     // 自定义变量
    
    // 原始嵌入节点
    ORIGIN_HTML,    // 原始HTML
    ORIGIN_CSS,     // 原始CSS
    ORIGIN_JS,      // 原始JavaScript
    
    // 导入节点
    IMPORT_STMT,    // 导入语句
    
    // 命名空间节点
    NAMESPACE_DECL, // 命名空间声明
    
    // 配置节点
    CONFIG_BLOCK,   // 配置块
    
    // CHTL JS节点
    VIR_OBJECT,     // 虚对象
    SELECTOR_EXPR,  // 选择器表达式
    LISTEN_EXPR,    // 监听表达式
    ANIMATE_EXPR,   // 动画表达式
    DELEGATE_EXPR,  // 委托表达式
    
    // 特殊操作节点
    DELETE_STMT,    // 删除语句
    INSERT_STMT,    // 插入语句
    INHERIT_STMT,   // 继承语句
    
    // 表达式节点
    IDENTIFIER_EXPR, // 标识符表达式
    LITERAL_EXPR,   // 字面量表达式
    CALL_EXPR,      // 调用表达式
    MEMBER_EXPR,    // 成员表达式
    
    // 错误节点
    ERROR_NODE
};

// 编译器状态枚举
enum class CompilerState {
    IDLE,           // 空闲
    SCANNING,       // 扫描中
    PARSING,        // 解析中
    COMPILING,      // 编译中
    MERGING,        // 合并中
    ERROR           // 错误状态
};

// 扫描器状态枚举
enum class ScannerState {
    NORMAL,         // 正常状态
    IN_STRING,      // 字符串中
    IN_COMMENT,     // 注释中
    IN_CHTL_BLOCK,  // CHTL块中
    IN_CSS_BLOCK,   // CSS块中
    IN_JS_BLOCK,    // JS块中
    IN_SELECTOR     // 选择器中
};

// 代码片段结构
struct CodeFragment {
    FragmentType type;
    String content;
    size_t startPos;
    size_t endPos;
    size_t lineNumber;
    size_t columnNumber;
};

using CodeFragmentPtr = std::shared_ptr<CodeFragment>;
using CodeFragmentList = std::vector<CodeFragmentPtr>;

// 编译选项
struct CompileOptions {
    bool debugMode = false;
    bool generateSourceMap = false;
    bool minifyOutput = false;
    bool enableOptimization = true;
    String outputPath = "./output";
    String modulePath = "./modules";
    StringList includePaths;
};

// 错误信息
struct ErrorInfo {
    String message;
    String file;
    size_t line;
    size_t column;
    String code; // 错误代码
};

using ErrorInfoPtr = std::shared_ptr<ErrorInfo>;
using ErrorList = std::vector<ErrorInfoPtr>;

// 位置信息
struct Position {
    size_t line = 1;
    size_t column = 1;
    size_t offset = 0;
    String filename;
    
    Position() = default;
    Position(size_t l, size_t c, size_t o = 0, const String& f = "") 
        : line(l), column(c), offset(o), filename(f) {}
};

// 源码范围
struct SourceRange {
    Position start;
    Position end;
    
    SourceRange() = default;
    SourceRange(const Position& s, const Position& e) : start(s), end(e) {}
};

} // namespace Core
} // namespace CHTL
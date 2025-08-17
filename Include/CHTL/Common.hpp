#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <variant>
#include <optional>

namespace CHTL {

/**
 * 源码位置信息
 */
struct SourceLocation {
    std::string fileName;
    int line = 1;
    int column = 1;
    int offset = 0;
    
    SourceLocation() = default;
    SourceLocation(const std::string& file, int ln, int col, int off = 0)
        : fileName(file), line(ln), column(col), offset(off) {}
        
    std::string toString() const {
        return fileName + ":" + std::to_string(line) + ":" + std::to_string(column);
    }
};

/**
 * 编译错误信息
 */
struct CompileError {
    enum class Type {
        LEXICAL_ERROR,      // 词法错误
        SYNTAX_ERROR,       // 语法错误
        SEMANTIC_ERROR,     // 语义错误
        TYPE_ERROR,         // 类型错误
        SCOPE_ERROR,        // 作用域错误
        IMPORT_ERROR,       // 导入错误
        TEMPLATE_ERROR,     // 模板错误
        CONSTRAINT_ERROR    // 约束错误
    };
    
    Type type;
    std::string message;
    SourceLocation location;
    std::string suggestion;
    
    CompileError(Type t, const std::string& msg, const SourceLocation& loc, 
                 const std::string& hint = "")
        : type(t), message(msg), location(loc), suggestion(hint) {}
        
    std::string toString() const;
};

/**
 * 编译器配置
 */
struct CompilerConfig {
    // 输出配置
    bool generateSourceMap = true;
    bool minifyOutput = false;
    bool preserveComments = false;
    
    // 调试配置
    bool enableDebugInfo = false;
    bool strictMode = false;
    int warningLevel = 1;  // 0=关闭, 1=基础, 2=详细, 3=全部
    
    // 语言特性
    bool allowExperimentalFeatures = false;
    bool enableTemplateInheritance = true;
    bool enableNamespaceSystem = true;
    bool enableConstraintSystem = true;
    
    // 路径配置
    std::vector<std::string> includePaths;
    std::vector<std::string> modulePaths;
    std::string outputDirectory;
    
    // 代码生成配置
    std::string htmlVersion = "HTML5";     // HTML5, XHTML
    std::string cssVersion = "CSS3";       // CSS3, CSS4
    std::string jsVersion = "ES2020";      // ES5, ES6, ES2017, ES2020
    
    // 自定义关键字映射（用于多语言支持）
    std::unordered_map<std::string, std::string> keywordMappings;
};

/**
 * 字面量类型
 */
enum class LiteralType {
    STRING,         // "字符串"
    SINGLE_STRING,  // '字符串'
    UNQUOTED,       // 无引号字符串
    NUMBER,         // 数字
    BOOLEAN,        // 布尔值
    NULL_VALUE      // null
};

/**
 * HTML元素类型
 */
enum class ElementType {
    STANDARD_HTML,  // 标准HTML元素 (div, span, etc.)
    SELF_CLOSING,   // 自闭合元素 (br, hr, img, etc.)
    TEMPLATE,       // 模板元素
    CUSTOM,         // 自定义元素
    TEXT_NODE,      // 文本节点
    COMMENT_NODE    // 注释节点
};

/**
 * 样式选择器类型
 */
enum class SelectorType {
    ELEMENT,        // 元素选择器 (div)
    CLASS,          // 类选择器 (.class)
    ID,             // ID选择器 (#id)
    ATTRIBUTE,      // 属性选择器 ([attr=value])
    PSEUDO_CLASS,   // 伪类 (:hover)
    PSEUDO_ELEMENT, // 伪元素 (::before)
    DESCENDANT,     // 后代选择器 (div p)
    CHILD,          // 子选择器 (div > p)
    ADJACENT,       // 相邻选择器 (div + p)
    GENERAL,        // 通用兄弟选择器 (div ~ p)
    CONTEXT         // 上下文选择器 (&)
};

/**
 * 模板类型
 */
enum class TemplateType {
    STYLE,          // @Style
    ELEMENT,        // @Element
    VARIABLE        // @Var
};

/**
 * 导入类型
 */
enum class ImportType {
    HTML_FILE,      // HTML文件
    CSS_FILE,       // CSS文件
    JS_FILE,        // JavaScript文件
    CHTL_FILE,      // CHTL文件
    CJMOD_FILE,     // CJMod文件
    TEMPLATE,       // 模板
    CUSTOM,         // 自定义组件
    NAMESPACE       // 命名空间
};

/**
 * 约束类型
 */
enum class ConstraintType {
    ELEMENT_CONSTRAINT,  // 元素约束
    TYPE_CONSTRAINT,     // 类型约束
    SCOPE_CONSTRAINT,    // 作用域约束
    TEMPLATE_CONSTRAINT, // 模板约束
    GLOBAL_CONSTRAINT    // 全局约束
};

/**
 * 代码生成目标
 */
enum class GenerationTarget {
    HTML,           // 生成HTML
    CSS,            // 生成CSS
    JAVASCRIPT,     // 生成JavaScript
    ALL             // 生成所有
};

/**
 * 字符串工具函数
 */
namespace StringUtils {
    std::string trim(const std::string& str);
    std::string toLower(const std::string& str);
    std::string toUpper(const std::string& str);
    std::string toCamelCase(const std::string& str);
    std::string toPascalCase(const std::string& str);
    std::string toKebabCase(const std::string& str);
    std::string toSnakeCase(const std::string& str);
    
    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string join(const std::vector<std::string>& parts, const std::string& separator);
    
    bool startsWith(const std::string& str, const std::string& prefix);
    bool endsWith(const std::string& str, const std::string& suffix);
    bool contains(const std::string& str, const std::string& substring);
    
    std::string replaceAll(const std::string& str, const std::string& from, const std::string& to);
    std::string escapeHtml(const std::string& str);
    std::string escapeCss(const std::string& str);
    std::string escapeJs(const std::string& str);
}

/**
 * 文件系统工具函数
 */
namespace FileUtils {
    bool fileExists(const std::string& path);
    bool isDirectory(const std::string& path);
    std::string readFile(const std::string& path);
    bool writeFile(const std::string& path, const std::string& content);
    
    std::string getFileExtension(const std::string& path);
    std::string getFileName(const std::string& path);
    std::string getDirectoryName(const std::string& path);
    std::string getAbsolutePath(const std::string& path);
    
    std::vector<std::string> listFiles(const std::string& directory, const std::string& extension = "");
    bool createDirectory(const std::string& path);
    bool removeFile(const std::string& path);
}

/**
 * HTML工具函数
 */
namespace HtmlUtils {
    bool isValidTagName(const std::string& tagName);
    bool isSelfClosingTag(const std::string& tagName);
    bool isValidAttributeName(const std::string& attrName);
    
    std::string formatAttribute(const std::string& name, const std::string& value);
    std::string formatElement(const std::string& tagName, const std::string& attributes, 
                             const std::string& content, bool selfClosing = false);
}

/**
 * CSS工具函数
 */
namespace CssUtils {
    bool isValidProperty(const std::string& property);
    bool isValidSelector(const std::string& selector);
    bool isValidValue(const std::string& property, const std::string& value);
    
    std::string formatProperty(const std::string& property, const std::string& value);
    std::string formatRule(const std::string& selector, const std::string& properties);
    std::string minifyCss(const std::string& css);
}

/**
 * 调试工具函数
 */
namespace DebugUtils {
    void enableDebugMode();
    void disableDebugMode();
    bool isDebugMode();
    
    void logInfo(const std::string& message);
    void logWarning(const std::string& message);
    void logError(const std::string& message);
    void logDebug(const std::string& message);
    
    std::string getCurrentTimestamp();
    void printCompilationStats(const std::string& phase, double timeMs, size_t memoryKb);
}

} // namespace CHTL
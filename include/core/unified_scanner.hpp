#pragma once

#include "core/chtl_configuration.hpp"
#include "constraints/raw_embedding_parser.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

namespace chtl {

/**
 * 语言类型枚举
 */
enum class LanguageType {
    CHTL,           // CHTL主语言
    CHTL_JS,        // CHTL JavaScript (局部script)
    CSS,            // 标准CSS
    JAVASCRIPT,     // 标准JavaScript
    HTML,           // HTML内容
    MIXED           // 混合内容
};

/**
 * 扫描结果中的代码块
 */
struct CodeBlock {
    LanguageType language;      // 语言类型
    std::string content;        // 内容
    size_t startLine;          // 起始行号
    size_t endLine;            // 结束行号
    size_t startPos;           // 起始位置
    size_t endPos;             // 结束位置
    std::string context;       // 上下文信息
    
    // CHTL特有信息
    std::string blockType;     // 块类型（如 "style", "script", "Origin"）
    std::string blockName;     // 块名称（如原始嵌入的名称）
    bool isLocalBlock;         // 是否为局部块
    bool isOriginBlock;        // 是否为原始嵌入块
    
    CodeBlock() : language(LanguageType::CHTL), startLine(0), endLine(0), 
                  startPos(0), endPos(0), isLocalBlock(false), isOriginBlock(false) {}
};

/**
 * CHTL语法元素
 */
struct ChtlElement {
    enum Type {
        TEMPLATE_VARIABLE,      // 模板变量
        CUSTOM_VARIABLE,        // 自定义变量
        TEMPLATE_STYLE_GROUP,   // 模板样式组
        CUSTOM_STYLE_GROUP,     // 自定义样式组
        TEMPLATE_ELEMENT,       // 模板元素
        CUSTOM_ELEMENT,         // 自定义元素
        IMPORT_STATEMENT,       // 导入语句
        NAMESPACE_DECLARATION,  // 命名空间声明
        CONFIGURATION_BLOCK,    // 配置块
        RAW_EMBEDDING,          // 原始嵌入
        INHERITANCE,            // 继承
        DELETE_OPERATION,       // 删除操作
        GENERATOR_COMMENT       // 生成器注释
    };
    
    Type type;
    std::string name;
    std::string value;
    std::string namespace_;
    std::vector<std::string> parameters;
    size_t line;
    size_t position;
    
    ChtlElement() : type(TEMPLATE_VARIABLE), line(0), position(0) {}
};

/**
 * 扫描结果
 */
struct ScanResult {
    std::vector<CodeBlock> codeBlocks;          // 代码块列表
    std::vector<ChtlElement> chtlElements;      // CHTL语法元素
    std::unordered_map<std::string, std::string> metadata;  // 元数据
    std::vector<std::string> errors;           // 错误信息
    std::vector<std::string> warnings;         // 警告信息
    
    bool hasErrors() const { return !errors.empty(); }
    bool hasWarnings() const { return !warnings.empty(); }
};

/**
 * 语言检测器
 */
class LanguageDetector {
public:
    /**
     * 检测代码块的语言类型
     */
    static LanguageType detectLanguage(const std::string& content, const std::string& context = "");
    
    /**
     * 检测是否为CHTL语法
     */
    static bool isChtlSyntax(const std::string& content);
    
    /**
     * 检测是否为CSS语法
     */
    static bool isCssSyntax(const std::string& content);
    
    /**
     * 检测是否为JavaScript语法
     */
    static bool isJavaScriptSyntax(const std::string& content);
};

/**
 * 统一扫描器 - CHTL、CHTL JS、CSS、JS大统一
 */
class UnifiedScanner {
public:
    UnifiedScanner();
    explicit UnifiedScanner(CHtlConfiguration* config);
    
    /**
     * 扫描CHTL文件内容
     */
    ScanResult scanChtlFile(const std::string& content, const std::string& filename = "");
    
    /**
     * 扫描代码块
     */
    ScanResult scanCodeBlock(const std::string& content, LanguageType expectedType = LanguageType::MIXED);
    
    /**
     * 设置配置
     */
    void setConfiguration(CHtlConfiguration* config) { config_ = config; }
    
    /**
     * 启用调试模式
     */
    void setDebugMode(bool enabled) { debugMode_ = enabled; }
    
    /**
     * 设置错误处理回调
     */
    void setErrorHandler(std::function<void(const std::string&)> handler) { errorHandler_ = handler; }

private:
    CHtlConfiguration* config_;
    RawEmbeddingParser rawEmbeddingParser_;
    bool debugMode_;
    std::function<void(const std::string&)> errorHandler_;
    
    /**
     * 扫描CHTL主体内容
     */
    void scanChtlMainContent(const std::string& content, ScanResult& result);
    
    /**
     * 扫描局部样式块
     */
    void scanLocalStyleBlock(const std::string& content, size_t startPos, ScanResult& result);
    
    /**
     * 扫描局部脚本块
     */
    void scanLocalScriptBlock(const std::string& content, size_t startPos, ScanResult& result);
    
    /**
     * 扫描原始嵌入块
     */
    void scanRawEmbeddingBlocks(const std::string& content, ScanResult& result);
    
    /**
     * 扫描CHTL语法元素
     */
    void scanChtlElements(const std::string& content, ScanResult& result);
    
    /**
     * 解析模板变量
     */
    bool parseTemplateVariable(const std::string& line, ChtlElement& element);
    
    /**
     * 解析自定义变量
     */
    bool parseCustomVariable(const std::string& line, ChtlElement& element);
    
    /**
     * 解析样式组
     */
    bool parseStyleGroup(const std::string& line, ChtlElement& element);
    
    /**
     * 解析元素定义
     */
    bool parseElementDefinition(const std::string& line, ChtlElement& element);
    
    /**
     * 解析导入语句
     */
    bool parseImportStatement(const std::string& line, ChtlElement& element);
    
    /**
     * 解析命名空间声明
     */
    bool parseNamespaceDeclaration(const std::string& line, ChtlElement& element);
    
    /**
     * 解析配置块
     */
    bool parseConfigurationBlock(const std::string& content, ChtlElement& element);
    
    /**
     * 查找局部块（style/script）
     */
    std::vector<std::pair<size_t, size_t>> findLocalBlocks(const std::string& content, const std::string& blockType);
    
    /**
     * 计算行号
     */
    size_t calculateLineNumber(const std::string& content, size_t position);
    
    /**
     * 查找匹配的右大括号
     */
    size_t findMatchingBrace(const std::string& content, size_t leftBracePos);
    
    /**
     * 添加错误
     */
    void addError(ScanResult& result, const std::string& message);
    
    /**
     * 添加警告
     */
    void addWarning(ScanResult& result, const std::string& message);
    
    /**
     * 调试输出
     */
    void debugLog(const std::string& message);
};

/**
 * 扫描器工厂
 */
class ScannerFactory {
public:
    /**
     * 创建统一扫描器
     */
    static std::unique_ptr<UnifiedScanner> createUnifiedScanner(CHtlConfiguration* config = nullptr);
    
    /**
     * 创建语言特定扫描器
     */
    static std::unique_ptr<UnifiedScanner> createLanguageScanner(LanguageType language, CHtlConfiguration* config = nullptr);
};

} // namespace chtl
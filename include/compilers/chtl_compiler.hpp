#pragma once

#include "dispatcher/compiler_dispatcher.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace chtl {

/**
 * CHTL抽象语法树节点基类
 */
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual std::string toString() const = 0;
};

/**
 * 编译器内部元素节点 (Legacy)
 */
class CompilerElementNode : public ASTNode {
public:
    std::string tagName;                                    // 标签名
    std::unordered_map<std::string, std::string> attributes; // 属性
    std::vector<std::unique_ptr<ASTNode>> children;         // 子节点
    std::string localStyle;                                 // 局部样式
    std::string localScript;                                // 局部脚本

    CompilerElementNode(const std::string& tag) : tagName(tag) {}
    std::string toString() const override;
};

/**
 * 编译器内部文本节点 (Legacy)
 */
class CompilerTextNode : public ASTNode {
public:
    std::string content;

    CompilerTextNode(const std::string& text) : content(text) {}
    std::string toString() const override;
};

/**
 * 编译器内部模板节点 (Legacy)
 */
class CompilerTemplateNode : public ASTNode {
public:
    std::string templateType; // @Style, @Element, @Var
    std::string name;
    std::string content;
    
    CompilerTemplateNode(const std::string& type, const std::string& templateName) 
        : templateType(type), name(templateName) {}
    std::string toString() const override;
};

/**
 * 编译器内部自定义节点 (Legacy)
 */
class CompilerCustomNode : public ASTNode {
public:
    std::string customType; // @Style, @Element, @Var
    std::string name;
    std::string content;
    
    CompilerCustomNode(const std::string& type, const std::string& customName) 
        : customType(type), name(customName) {}
    std::string toString() const override;
};

/**
 * CHTL编译器
 * 负责解析和编译CHTL语法，处理局部样式
 */
class CHTLCompiler : public ICompiler {
public:
    /**
     * 构造函数
     */
    CHTLCompiler();

    /**
     * 析构函数
     */
    ~CHTLCompiler() = default;

    /**
     * 编译代码片段
     * @param segment 代码片段
     * @param config 编译配置
     * @return 编译结果
     */
    CompileResult compile(const CodeSegment& segment, const CompileConfig& config) override;

    /**
     * 获取编译器名称
     * @return 编译器名称
     */
    std::string getName() const override;

    /**
     * 获取支持的代码片段类型
     * @return 支持的类型列表
     */
    std::vector<CodeSegmentType> getSupportedTypes() const override;

    /**
     * 设置模板库
     * @param templates 模板映射
     */
    void setTemplates(const std::unordered_map<std::string, std::string>& templates);

    /**
     * 设置自定义库
     * @param customs 自定义映射
     */
    void setCustoms(const std::unordered_map<std::string, std::string>& customs);

private:
    /**
     * 解析器状态
     */
    struct ParserState {
        std::string source;
        size_t position;
        size_t line;
        size_t column;
        CompileResult result;

        ParserState() : position(0), line(1), column(1) {}
    };

    ParserState state_;
    CompileConfig config_;
    std::unordered_map<std::string, std::string> templates_;
    std::unordered_map<std::string, std::string> customs_;
    std::string generatedCSS_;     // 生成的全局CSS
    std::string generatedJS_;      // 生成的全局JS
    size_t classCounter_;          // 自动生成的类名计数器

    /**
     * 解析CHTL代码
     * @param source 源代码
     * @return AST根节点
     */
    std::unique_ptr<ASTNode> parse(const std::string& source);

    /**
     * 解析元素
     * @return 元素节点
     */
    std::unique_ptr<CompilerElementNode> parseElement();

    /**
     * 解析文本节点
     * @return 文本节点
     */
    std::unique_ptr<CompilerTextNode> parseText();

    /**
     * 解析模板
     * @return 模板节点
     */
    std::unique_ptr<CompilerTemplateNode> parseTemplate();

    /**
     * 解析自定义
     * @return 自定义节点
     */
    std::unique_ptr<CompilerCustomNode> parseCustom();

    /**
     * 解析属性
     * @param element 元素节点
     */
    void parseAttributes(CompilerElementNode* element);

    /**
     * 解析局部样式块
     * @param element 元素节点
     */
    void parseLocalStyle(CompilerElementNode* element);

    /**
     * 解析局部脚本块
     * @param element 元素节点
     */
    void parseLocalScript(CompilerElementNode* element);

    /**
     * 处理局部样式
     * @param styleContent 样式内容
     * @param elementId 元素ID（用于生成唯一类名）
     * @return 处理后的内联样式和全局样式
     */
    std::pair<std::string, std::string> processLocalStyle(const std::string& styleContent, const std::string& elementId);

    /**
     * 生成HTML
     * @param node AST节点
     * @return HTML字符串
     */
    std::string generateHTML(const ASTNode* node);

    /**
     * 生成元素HTML
     * @param element 元素节点
     * @return HTML字符串
     */
    std::string generateElementHTML(const CompilerElementNode* element);

    /**
     * 生成文本HTML
     * @param text 文本节点
     * @return HTML字符串
     */
    std::string generateTextHTML(const CompilerTextNode* text);

    /**
     * 处理模板引用
     * @param templateRef 模板引用
     * @return 处理后的内容
     */
    std::string processTemplateReference(const std::string& templateRef);

    /**
     * 处理自定义引用
     * @param customRef 自定义引用
     * @return 处理后的内容
     */
    std::string processCustomReference(const std::string& customRef);

    /**
     * 生成唯一类名
     * @return 类名
     */
    std::string generateUniqueClassName();

    /**
     * 当前字符
     * @return 当前字符
     */
    char currentChar();

    /**
     * 前进一个字符
     * @return 前进后的字符
     */
    char advance();

    /**
     * 查看下一个字符
     * @param offset 偏移量
     * @return 字符
     */
    char peek(size_t offset = 1);

    /**
     * 跳过空白字符
     */
    void skipWhitespace();

    /**
     * 读取标识符
     * @return 标识符
     */
    std::string readIdentifier();

    /**
     * 读取字符串字面量
     * @param delimiter 分隔符
     * @return 字符串内容
     */
    std::string readStringLiteral(char delimiter);

    /**
     * 读取无修饰字面量
     * @param stopChars 停止字符
     * @return 字面量内容
     */
    std::string readUnquotedLiteral(const std::string& stopChars = ";{}:");

    /**
     * 读取块内容
     * @return 块内容
     */
    std::string readBlock();

    /**
     * 匹配字符串
     * @param str 要匹配的字符串
     * @return 是否匹配成功
     */
    bool match(const std::string& str);

    /**
     * 期望字符
     * @param expected 期望的字符
     * @return 是否匹配成功
     */
    bool expect(char expected);

    /**
     * 是否到达文件末尾
     * @return 是否到达末尾
     */
    bool isAtEnd();

    /**
     * 报告错误
     * @param message 错误消息
     */
    void reportError(const std::string& message);

    /**
     * 报告警告
     * @param message 警告消息
     */
    void reportWarning(const std::string& message);

    /**
     * 获取当前位置信息
     * @return 位置字符串
     */
    std::string getCurrentPosition();
};

} // namespace chtl
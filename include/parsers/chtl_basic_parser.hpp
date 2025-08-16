#pragma once

#include "ast/chtl_ast.hpp"
#include "lexer/chtl_lexer.hpp"
#include "common/types.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

namespace chtl {

/**
 * CHTL基础语法解析器
 * 严格按照CHTL语法文档实现基础语法解析
 * 实现: 注释、文本节点、字面量、CE对等式、元素节点、属性解析
 */
class CHTLBasicParser {
public:
    CHTLBasicParser();
    ~CHTLBasicParser();

    /**
     * 解析CHTL源代码
     * @param source 源代码字符串
     * @param filename 文件名
     * @return 解析结果的AST根节点
     */
    ParseResult parse(const std::string& source, const std::string& filename = "");

    /**
     * 设置是否启用严格模式 (默认启用)
     * @param strict 是否严格按照语法文档
     */
    void setStrictMode(bool strict);

    /**
     * 获取解析错误
     * @return 错误列表
     */
    const std::vector<ParseError>& getErrors() const;

    /**
     * 获取解析警告
     * @return 警告列表
     */
    const std::vector<ParseWarning>& getWarnings() const;

private:
    CHTLLexer lexer_;
    std::vector<Token> tokens_;
    size_t currentTokenIndex_;
    bool strictMode_;
    std::vector<ParseError> errors_;
    std::vector<ParseWarning> warnings_;

    // 当前文件名 (用于错误报告)
    std::string currentFile_;

    // HTML元素集合 (用于验证元素节点)
    std::unordered_set<std::string> htmlElements_;

    // ===== 核心解析方法 =====

    /**
     * 解析程序根节点
     * @return 程序节点
     */
    CHTLASTNodePtr parseProgram();

    /**
     * 解析顶层语句
     * @return 语句节点列表
     */
    std::vector<CHTLASTNodePtr> parseTopLevelStatements();

    /**
     * 解析单个语句
     * @return 语句节点
     */
    CHTLASTNodePtr parseStatement();

    // ===== 注释解析 =====

    /**
     * 解析注释
     * @return 注释节点
     */
    CHTLASTNodePtr parseComment();

    /**
     * 解析单行注释 // 
     * @return 单行注释节点
     */
    CHTLASTNodePtr parseLineComment();

    /**
     * 解析多行注释
     * @return 多行注释节点
     */
    CHTLASTNodePtr parseMultilineComment();

    /**
     * 解析生成器注释 --
     * @return 生成器注释节点
     */
    CHTLASTNodePtr parseGeneratorComment();

    // ===== 文本节点解析 =====

    /**
     * 解析文本节点 text { }
     * @return 文本节点
     */
    CHTLASTNodePtr parseTextNode();

    /**
     * 解析文本内容 (支持各种字面量类型)
     * @return 文本内容和字面量类型
     */
    std::pair<std::string, LiteralType> parseTextContent();

    // ===== 字面量解析 =====

    /**
     * 解析字面量 (双引号、单引号、无修饰)
     * @return 字面量节点
     */
    CHTLASTNodePtr parseLiteral();

    /**
     * 解析双引号字符串 "text"
     * @return 字符串内容
     */
    std::string parseDoubleQuotedString();

    /**
     * 解析单引号字符串 'text'
     * @return 字符串内容
     */
    std::string parseSingleQuotedString();

    /**
     * 解析无修饰字面量 text
     * @return 字符串内容
     */
    std::string parseUnquotedLiteral();

    // ===== 元素节点解析 =====

    /**
     * 解析元素节点 div { }
     * @return 元素节点
     */
    CHTLASTNodePtr parseElementNode();

    /**
     * 解析元素体 (元素内部的内容)
     * @return 子节点列表
     */
    std::vector<CHTLASTNodePtr> parseElementBody();

    /**
     * 验证HTML元素名是否有效
     * @param tagName 标签名
     * @return 是否有效
     */
    bool isValidHtmlElement(const std::string& tagName) const;

    // ===== 属性解析 =====

    /**
     * 解析属性 id: "value"; 或 class = value;
     * @return 属性节点
     */
    CHTLASTNodePtr parseAttribute();

    /**
     * 解析属性值 (支持CE对等式)
     * @return 属性值和字面量类型
     */
    std::pair<std::string, LiteralType> parseAttributeValue();

    /**
     * 检查是否为CE对等式 (: 和 = 等价)
     * @return 是否为分隔符 (: 或 =)
     */
    bool isCEEquivalent() const;

    // ===== 工具方法 =====

    /**
     * 获取当前Token
     * @return 当前Token
     */
    const Token& getCurrentToken() const;

    /**
     * 获取下一个Token
     * @return 下一个Token
     */
    const Token& getNextToken();

    /**
     * 检查当前Token类型
     * @param type Token类型
     * @return 是否匹配
     */
    bool isCurrentToken(TokenType type) const;

    /**
     * 检查当前Token值
     * @param value Token值
     * @return 是否匹配
     */
    bool isCurrentToken(const std::string& value) const;

    /**
     * 消费指定类型的Token
     * @param type 期望的Token类型
     * @return 是否成功消费
     */
    bool consume(TokenType type);

    /**
     * 消费指定值的Token
     * @param value 期望的Token值
     * @return 是否成功消费
     */
    bool consume(const std::string& value);

    /**
     * 期望并消费指定类型的Token
     * @param type 期望的Token类型
     * @return 是否成功
     */
    bool expect(TokenType type);

    /**
     * 期望并消费指定值的Token
     * @param value 期望的Token值
     * @return 是否成功
     */
    bool expect(const std::string& value);

    /**
     * 跳过空白Token和注释
     */
    void skipWhitespaceAndComments();

    /**
     * 向前查看Token
     * @param offset 偏移量
     * @return Token
     */
    const Token& peekToken(size_t offset = 1) const;

    /**
     * 检查是否到达文件末尾
     * @return 是否EOF
     */
    bool isEOF() const;

    /**
     * 报告解析错误
     * @param message 错误消息
     * @param position 错误位置
     */
    void reportError(const std::string& message, const Position& position = Position{});

    /**
     * 报告解析警告
     * @param message 警告消息
     * @param position 警告位置
     */
    void reportWarning(const std::string& message, const Position& position = Position{});

    /**
     * 初始化HTML元素集合
     */
    void initializeHtmlElements();

    /**
     * 获取当前位置信息
     * @return 位置信息
     */
    Position getCurrentPosition() const;

    /**
     * 同步解析器状态 (错误恢复)
     */
    void synchronize();
};

} // namespace chtl
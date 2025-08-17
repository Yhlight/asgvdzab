#pragma once

#include "../common/Types.h"
#include "../common/Token.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

/**
 * CHTL统一扫描器 - 精准代码切割器
 * 负责将CHTL源代码精确切割成不同类型的代码片段
 * 支持识别：CHTL片段、CHTL JS片段、CSS片段、JS片段
 */
class CHTLUnifiedScanner {
private:
    std::string source_code;
    size_t current_position;
    size_t current_line;
    size_t current_column;
    
    // 扫描状态
    bool in_string;
    bool in_comment;
    char string_delimiter;
    
    // 配置
    Configuration config;
    
    // 内部扫描方法
    char peek(size_t offset = 0) const;
    char advance();
    void skipWhitespace();
    void skipComment();
    
    // Token识别方法
    TokenPtr scanString();
    TokenPtr scanNumber();
    TokenPtr scanIdentifier();
    TokenPtr scanOperator();
    TokenPtr scanUnquotedLiteral();
    
    // 关键字识别
    TokenType getKeywordType(const std::string& identifier) const;
    bool isKeyword(const std::string& identifier) const;
    
    // 代码块识别
    bool isStyleBlock() const;
    bool isScriptBlock() const;
    bool isOriginBlock() const;
    bool isTemplateBlock() const;
    bool isCustomBlock() const;
    
    // 代码片段切割
    std::string extractStyleContent();
    std::string extractScriptContent();
    std::string extractOriginContent();
    std::string extractBlockContent();
    
    // 嵌套层级管理
    int brace_level;
    int paren_level;
    int bracket_level;
    
    void updateNestingLevel(char c);
    bool isAtTopLevel() const;
    
public:
    CHTLUnifiedScanner();
    explicit CHTLUnifiedScanner(const std::string& source);
    
    // 设置源代码
    void setSource(const std::string& source);
    
    // 设置配置
    void setConfiguration(const Configuration& cfg);
    
    // 主要扫描方法
    std::vector<FragmentPtr> scanAndSplit();
    
    // 词法分析 - 生成Token流
    std::vector<TokenPtr> tokenize();
    
    // 获取下一个Token
    TokenPtr nextToken();
    
    // 检查是否到达文件末尾
    bool isAtEnd() const;
    
    // 重置扫描器状态
    void reset();
    
    // 获取当前位置信息
    size_t getCurrentLine() const { return current_line; }
    size_t getCurrentColumn() const { return current_column; }
    size_t getCurrentPosition() const { return current_position; }
    
    // 错误处理
    std::string formatError(const std::string& message) const;
    
    // 调试方法
    void dumpTokens(const std::vector<TokenPtr>& tokens) const;
    void dumpFragments(const std::vector<FragmentPtr>& fragments) const;
};

} // namespace CHTL
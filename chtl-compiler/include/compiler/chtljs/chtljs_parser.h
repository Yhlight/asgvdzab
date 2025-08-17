#ifndef CHTLJS_PARSER_H
#define CHTLJS_PARSER_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "compiler/chtljs/chtljs_tokens.h"
#include "compiler/chtljs/chtljs_ast.h"
#include "compiler/chtljs/chtljs_lexer.h"

namespace chtl {

// CHTL JS语法分析器
class CHTLJSParser {
public:
    CHTLJSParser(CHTLJSLexer& lexer);
    ~CHTLJSParser() = default;
    
    // 解析整个CHTL JS代码块
    std::unique_ptr<CHTLJSASTNode> parse();
    
    // 错误处理
    bool hasErrors() const { return !errors_.empty(); }
    const std::vector<std::string>& getErrors() const { return errors_; }
    const std::vector<std::string>& getWarnings() const { return warnings_; }
    
private:
    CHTLJSLexer& lexer_;
    
    // 当前token
    CHTLJSToken current_;
    CHTLJSToken previous_;
    
    // 错误和警告
    std::vector<std::string> errors_;
    std::vector<std::string> warnings_;
    
    // Token操作
    void advance();
    bool check(CHTLJSTokenType type);
    bool match(CHTLJSTokenType type);
    bool match(const std::string& value);
    CHTLJSToken consume(CHTLJSTokenType type, const std::string& message);
    CHTLJSToken consume(const std::string& value, const std::string& message);
    
    // 顶层解析
    std::unique_ptr<CHTLJSASTNode> parseStatement();
    std::unique_ptr<CHTLJSASTNode> parseExpression();
    
    // CHTL JS特殊语法解析
    std::unique_ptr<CHTLJSASTNode> parseEnhancedSelector();
    std::unique_ptr<CHTLJSASTNode> parseArrowAccess(std::unique_ptr<CHTLJSASTNode> left);
    std::unique_ptr<CHTLJSASTNode> parseListenFunction();
    std::unique_ptr<CHTLJSASTNode> parseDelegateFunction();
    std::unique_ptr<CHTLJSASTNode> parseAnimateFunction();
    std::unique_ptr<CHTLJSASTNode> parseVirDeclaration();
    std::unique_ptr<CHTLJSASTNode> parseVirAccess();
    
    // 通用JavaScript解析
    std::unique_ptr<CHTLJSASTNode> parsePrimary();
    std::unique_ptr<CHTLJSASTNode> parseIdentifier();
    std::unique_ptr<CHTLJSASTNode> parseStringLiteral();
    std::unique_ptr<CHTLJSASTNode> parseNumberLiteral();
    std::unique_ptr<CHTLJSASTNode> parseObjectLiteral();
    std::unique_ptr<CHTLJSASTNode> parseArrayLiteral();
    std::unique_ptr<CHTLJSASTNode> parseFunctionExpression();
    std::unique_ptr<CHTLJSASTNode> parseMemberAccess(std::unique_ptr<CHTLJSASTNode> object);
    std::unique_ptr<CHTLJSASTNode> parseFunctionCall(std::unique_ptr<CHTLJSASTNode> function);
    
    // 辅助方法
    bool isAtEnd();
    bool isSpecialFunction();
    bool isEnhancedSelector();
    
    // 错误处理
    void error(const std::string& message);
    void warning(const std::string& message);
    void synchronize();
};

} // namespace chtl

#endif // CHTLJS_PARSER_H
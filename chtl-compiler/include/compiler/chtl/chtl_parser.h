#ifndef CHTL_PARSER_H
#define CHTL_PARSER_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "compiler/chtl/chtl_tokens.h"
#include "compiler/chtl/chtl_ast.h"
#include "compiler/chtl/chtl_lexer.h"
#include "compiler/chtl/chtl_state.h"

namespace chtl {

// CHTL语法分析器
class CHTLParser {
public:
    CHTLParser(CHTLLexer& lexer, GlobalMap& global_map);
    ~CHTLParser() = default;
    
    // 解析整个文件
    std::unique_ptr<ASTNode> parse();
    
    // 错误处理
    bool hasErrors() const { return !errors_.empty(); }
    const std::vector<std::string>& getErrors() const { return errors_; }
    const std::vector<std::string>& getWarnings() const { return warnings_; }
    
private:
    CHTLLexer& lexer_;
    GlobalMap& global_map_;
    CompilerState state_;
    
    // 当前token
    Token current_;
    Token previous_;
    
    // 错误和警告
    std::vector<std::string> errors_;
    std::vector<std::string> warnings_;
    
    // Token操作
    void advance();
    bool check(TokenType type);
    bool match(TokenType type);
    bool match(const std::string& value);
    Token consume(TokenType type, const std::string& message);
    Token consume(const std::string& value, const std::string& message);
    
    // 顶层解析
    std::unique_ptr<ASTNode> parseTopLevel();
    std::unique_ptr<ASTNode> parseStatement();
    
    // 特殊块解析
    std::unique_ptr<ASTNode> parseSpecialBlock();
    std::unique_ptr<ASTNode> parseConfiguration();
    std::unique_ptr<ASTNode> parseNamespace();
    std::unique_ptr<ASTNode> parseTemplate();
    std::unique_ptr<ASTNode> parseCustom();
    std::unique_ptr<ASTNode> parseOrigin();
    std::unique_ptr<ASTNode> parseImport();
    
    // 元素解析
    std::unique_ptr<ASTNode> parseElement();
    std::unique_ptr<ASTNode> parseTextNode();
    std::unique_ptr<ASTNode> parseAttribute();
    
    // 样式解析
    std::unique_ptr<ASTNode> parseStyleBlock();
    std::unique_ptr<ASTNode> parseStyleRule();
    std::unique_ptr<ASTNode> parseStyleProperty();
    
    // 脚本解析
    std::unique_ptr<ASTNode> parseScriptBlock();
    
    // 模板使用解析
    std::unique_ptr<ASTNode> parseTemplateUsage();
    std::unique_ptr<ASTNode> parseCustomUsage();
    
    // 表达式解析
    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseLiteral();
    std::unique_ptr<ASTNode> parseVariableAccess();
    
    // 特殊操作解析
    std::unique_ptr<ASTNode> parseDeleteOperation();
    std::unique_ptr<ASTNode> parseInsertOperation();
    std::unique_ptr<ASTNode> parseInheritOperation();
    std::unique_ptr<ASTNode> parseExceptClause();
    
    // 辅助方法
    bool isAtEnd();
    bool isElementStart();
    bool isAttributeStart();
    bool isSpecialBlockStart();
    bool isTemplateUsage();
    bool isCustomUsage();
    
    // 错误处理
    void error(const std::string& message);
    void warning(const std::string& message);
    void synchronize();
    
    // CE对等式处理
    bool matchColonOrEquals();
    
    // 字面量处理
    std::string parseLiteralValue();
    std::string parseStringLiteral();
    std::string parseUnquotedLiteral();
    
    // 辅助方法
    bool isWhitespace(char c);
};

} // namespace chtl

#endif // CHTL_PARSER_H
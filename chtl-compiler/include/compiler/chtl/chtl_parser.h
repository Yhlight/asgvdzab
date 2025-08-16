#ifndef CHTL_COMPILER_CHTL_PARSER_H
#define CHTL_COMPILER_CHTL_PARSER_H

#include <memory>
#include <vector>
#include <string>
#include "compiler/chtl/chtl_ast.h"
#include "compiler/chtl/chtl_lexer.h"
#include "common/types.h"

namespace chtl {
namespace compiler {

class CHTLLexer;
struct CHTLContext;

class CHTLParser {
public:
    CHTLParser();
    virtual ~CHTLParser();  // 添加虚析构函数
    
    // 解析CHTL代码，返回抽象语法树
    std::shared_ptr<ast::DocumentNode> parse(CHTLLexer& lexer, CHTLContext& context);
    
    // 获取解析错误
    const std::vector<std::string>& getErrors() const { return errors_; }
    
    // 获取解析警告
    const std::vector<std::string>& getWarnings() const { return warnings_; }

    // 清空错误和警告
    void clearDiagnostics() {
        errors_.clear();
        warnings_.clear();
    }
    
protected:
    CHTLLexer* lexer_;
    std::vector<std::string> errors_;
    std::vector<std::string> warnings_;
    Token currentToken_;
    Token previousToken_;  // 添加前一个token的跟踪
    
    // 解析辅助方法
    virtual std::shared_ptr<ast::ASTNode> parseTopLevel();
    virtual std::shared_ptr<ast::ElementNode> parseElement();
    std::shared_ptr<ast::TextNode> parseText();
    std::shared_ptr<ast::StyleNode> parseStyle();
    std::shared_ptr<ast::ScriptNode> parseScript();
    std::shared_ptr<ast::TemplateNode> parseTemplate();
    std::shared_ptr<ast::CustomNode> parseCustom();
    std::shared_ptr<ast::ImportNode> parseImport();
    std::shared_ptr<ast::NamespaceNode> parseNamespace();
    std::shared_ptr<ast::OriginNode> parseOrigin();
    std::shared_ptr<ast::ConfigurationNode> parseConfiguration();
    
    // 属性解析
    std::shared_ptr<ast::AttributeNode> parseAttribute();
    std::vector<std::shared_ptr<ast::AttributeNode>> parseAttributes();
    
    // 表达式解析
    std::string parseStringLiteral();
    std::string parseUnquotedLiteral();
    
    // Token匹配和消费
    bool match(TokenType type);
    bool check(TokenType type) const;
    Token consume(TokenType type, const std::string& message);
    Token advance();
    Token peek() const;
    Token previous() const;
    bool isAtEnd() const;
    
    // 辅助方法
    void skipWhitespaceAndComments();
    bool matchKeyword(const std::string& keyword);
    bool checkKeyword(const std::string& keyword) const;
    
    // 错误处理
    void error(const std::string& message);
    void warning(const std::string& message);
    void synchronize();
    
    // 工具方法
    std::string parseIdentifier();
};

} // namespace compiler
} // namespace chtl

#endif // CHTL_COMPILER_CHTL_PARSER_H
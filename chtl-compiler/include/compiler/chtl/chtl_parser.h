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

class CHTLParser {
public:
    CHTLParser();
    ~CHTLParser();
    
    // 解析CHTL代码，返回AST根节点
    std::shared_ptr<ast::DocumentNode> parse(CHTLLexer& lexer);
    
    // 获取解析错误
    const std::vector<std::string>& getErrors() const { return errors_; }
    
    // 获取解析警告
    const std::vector<std::string>& getWarnings() const { return warnings_; }

private:
    CHTLLexer* lexer_;
    std::vector<std::string> errors_;
    std::vector<std::string> warnings_;
    Token currentToken_;
    
    // 解析方法
    std::shared_ptr<ast::ASTNode> parseTopLevel();
    std::shared_ptr<ast::ElementNode> parseElement();
    std::shared_ptr<ast::TextNode> parseText();
    std::shared_ptr<ast::StyleNode> parseStyle();
    std::shared_ptr<ast::ScriptNode> parseScript();
    std::shared_ptr<ast::TemplateNode> parseTemplate();
    std::shared_ptr<ast::CustomNode> parseCustom();
    std::shared_ptr<ast::ImportNode> parseImport();
    std::shared_ptr<ast::NamespaceNode> parseNamespace();
    std::shared_ptr<ast::OriginNode> parseOrigin();
    std::shared_ptr<ast::ConfigurationNode> parseConfiguration();
    
    // 解析属性
    std::map<std::string, std::string> parseAttributes();
    std::pair<std::string, std::string> parseAttribute();
    
    // 解析样式规则
    std::vector<ast::StyleNode::StyleRule> parseStyleRules();
    ast::StyleNode::StyleRule parseStyleRule();
    
    // 解析模板参数
    std::map<std::string, std::string> parseTemplateParams();
    
    // 辅助方法
    bool match(TokenType type);
    bool consume(TokenType type, const std::string& message);
    void advance();
    Token peek();
    bool check(TokenType type);
    bool checkKeyword(const std::string& keyword);
    
    // 错误处理
    void error(const std::string& message);
    void warning(const std::string& message);
    void synchronize();
    
    // 工具方法
    std::string parseStringLiteral();
    std::string parseIdentifier();
    bool isAtEnd();
};

} // namespace compiler
} // namespace chtl

#endif // CHTL_COMPILER_CHTL_PARSER_H
#ifndef CHTL_PARSER_PARSER_H
#define CHTL_PARSER_PARSER_H

#include <memory>
#include <vector>
#include <string>
#include "Lexer/Lexer.h"
#include "Lexer/Token.h"
#include "AST/ASTNode.h"
#include "AST/ChtlNodes.h"
#include "AST/TemplateNodes.h"
#include "AST/ConfigNodes.h"
#include "Common/Context.h"

namespace Chtl {

class Parser {
public:
    Parser();
    ~Parser();
    
    // 主解析方法
    ASTNodePtr parse(const std::string& source, const std::string& fileName = "");
    ASTNodePtr parseFile(const std::string& filePath);
    
    // 获取错误信息
    bool hasError() const { return hasError_; }
    const std::vector<std::string>& getErrors() const { return errors_; }
    
    // 设置上下文
    void setContext(std::shared_ptr<Context> context) { context_ = context; }

private:
    // Token管理
    void advance();
    Token peek(int offset = 0);
    bool match(TokenType type);
    bool check(TokenType type);
    Token consume(TokenType type, const std::string& message);
    bool isAtEnd();
    
    // 顶层解析
    ASTNodePtr parseProgram();
    ASTNodePtr parseTopLevel();
    
    // 元素解析
    ASTNodePtr parseElement();
    ASTNodePtr parseAttributes();
    ASTNodePtr parseAttribute();
    ASTNodePtr parseElementContent();
    
    // 文本解析
    ASTNodePtr parseText();
    ASTNodePtr parseTextContent();
    
    // 样式解析
    ASTNodePtr parseStyleBlock();
    ASTNodePtr parseStyleContent();
    ASTNodePtr parseStyleRule();
    ASTNodePtr parseStyleSelector();
    ASTNodePtr parseStyleProperty();
    
    // 脚本解析
    ASTNodePtr parseScriptBlock();
    
    // 模板解析
    ASTNodePtr parseTemplate();
    ASTNodePtr parseStyleTemplate();
    ASTNodePtr parseElementTemplate();
    ASTNodePtr parseVarTemplate();
    
    // 自定义解析
    ASTNodePtr parseCustom();
    ASTNodePtr parseCustomStyle();
    ASTNodePtr parseCustomElement();
    ASTNodePtr parseCustomVar();
    
    // 模板使用
    ASTNodePtr parseTemplateUse();
    ASTNodePtr parseVarUse();
    
    // 继承和操作
    ASTNodePtr parseInherit();
    ASTNodePtr parseDelete();
    ASTNodePtr parseInsert();
    
    // 原始嵌入
    ASTNodePtr parseOrigin();
    
    // 配置相关
    ASTNodePtr parseConfiguration();
    ASTNodePtr parseName();
    ASTNodePtr parseOriginType();
    
    // 导入导出
    ASTNodePtr parseImport();
    ASTNodePtr parseExport();
    ASTNodePtr parseNamespace();
    
    // 字面量和表达式
    ASTNodePtr parseLiteral();
    ASTNodePtr parseStringLiteral();
    ASTNodePtr parseNumberLiteral();
    ASTNodePtr parseIdentifier();
    
    // 辅助方法
    std::string parseStringValue();
    bool isStyleBlockStart();
    bool isScriptBlockStart();
    bool isTemplateStart();
    bool isCustomStart();
    void skipToBlockEnd();
    
    // 错误处理
    void error(const std::string& message);
    void synchronize();

private:
    // 词法分析器
    std::unique_ptr<Lexer> lexer_;
    
    // Token列表
    std::vector<Token> tokens_;
    size_t current_;
    
    // 上下文
    std::shared_ptr<Context> context_;
    
    // 错误状态
    bool hasError_;
    std::vector<std::string> errors_;
    
    // 解析状态
    bool inStyleBlock_;
    bool inScriptBlock_;
    bool inTemplate_;
    int braceDepth_;
};

} // namespace Chtl

#endif // CHTL_PARSER_PARSER_H
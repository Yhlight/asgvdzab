#ifndef CHTL_PARSER_CHTLJSPARSER_H
#define CHTL_PARSER_CHTLJSPARSER_H

#include <memory>
#include <string>
#include <vector>
#include "AST/ASTNode.h"
#include "AST/ChtlJsNodes.h"
#include "Lexer/Token.h"

namespace Chtl {

// CHTL JS 解析器
// 负责解析script块中的CHTL JS特殊语法
class ChtlJsParser {
public:
    ChtlJsParser();
    ~ChtlJsParser();
    
    // 解析CHTL JS代码
    std::vector<ASTNodePtr> parse(const std::string& jsCode);
    
    // 检查是否包含CHTL JS语法
    bool hasChtlJsSyntax(const std::string& jsCode) const;
    
    // 转换CHTL JS到标准JavaScript
    std::string transform(const std::string& jsCode);

private:
    // 解析选择器表达式 {{selector}}
    ASTNodePtr parseSelectorExpression(const std::string& selector, size_t pos);
    
    // 解析箭头访问 ->
    ASTNodePtr parseArrowAccess(const std::string& code, size_t pos);
    
    // 解析listen调用
    ASTNodePtr parseListenCall(const std::string& code, size_t pos);
    
    // 解析delegate调用
    ASTNodePtr parseDelegateCall(const std::string& code, size_t pos);
    
    // 解析animate调用
    ASTNodePtr parseAnimateCall(const std::string& code, size_t pos);
    
    // 解析虚对象声明 vir name = expr
    ASTNodePtr parseVirtualObject(const std::string& code, size_t pos);
    
    // 解析iNeverAway调用
    ASTNodePtr parseINeverAway(const std::string& code, size_t pos);
    
    // 解析虚对象函数调用 obj->func<state>()
    ASTNodePtr parseVirtualCall(const std::string& code, size_t pos);
    
    // 辅助方法
    std::string extractSelector(const std::string& code, size_t& pos);
    std::string extractObjectLiteral(const std::string& code, size_t& pos);
    std::string extractFunctionBody(const std::string& code, size_t& pos);
    
    // 选择器类型判断
    SelectorExpressionNode::SelectorType determineSelectorType(const std::string& selector);
    
    // 生成JavaScript代码
    std::string generateQuerySelector(const std::string& selector, bool multiple = false);
    std::string generateListenCode(const ListenCallNode* node);
    std::string generateDelegateCode(const DelegateCallNode* node);
    std::string generateAnimateCode(const AnimateCallNode* node);
    
    // 工具方法
    std::string trim(const std::string& str);
    bool startsWith(const std::string& str, const std::string& prefix);
    std::string replaceAll(const std::string& str, const std::string& from, const std::string& to);
    
private:
    // 当前解析位置
    size_t currentPos_;
    
    // 错误列表
    std::vector<std::string> errors_;
};

} // namespace Chtl

#endif // CHTL_PARSER_CHTLJSPARSER_H
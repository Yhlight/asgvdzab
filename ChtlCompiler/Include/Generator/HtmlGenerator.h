#ifndef CHTL_GENERATOR_HTMLGENERATOR_H
#define CHTL_GENERATOR_HTMLGENERATOR_H

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "AST/ASTVisitor.h"
#include "AST/ASTNode.h"
#include "Common/Context.h"
#include "Common/StateManager.h"

namespace Chtl {

// HTML生成器 - 负责生成HTML输出
class HtmlGenerator : public DefaultASTVisitor {
public:
    HtmlGenerator();
    ~HtmlGenerator();
    
    // 生成HTML
    std::string generate(ASTNodePtr ast, Context* context = nullptr);
    
    // 获取生成的CSS和JS
    std::string getGeneratedCss() const { return cssOutput_.str(); }
    std::string getGeneratedJs() const { return jsOutput_.str(); }
    
    // 访问者方法实现
    void visitProgram(ProgramNode* node) override;
    void visitElement(ElementNode* node) override;
    void visitText(TextNode* node) override;
    void visitAttribute(AttributeNode* node) override;
    void visitStyleBlock(StyleBlockNode* node) override;
    void visitStyleRule(StyleRuleNode* node) override;
    void visitStyleSelector(StyleSelectorNode* node) override;
    void visitStyleProperty(StylePropertyNode* node) override;
    void visitScriptBlock(ScriptBlockNode* node) override;
    void visitComment(CommentNode* node) override;
    void visitStringLiteral(StringLiteralNode* node) override;
    void visitNumberLiteral(NumberLiteralNode* node) override;
    void visitIdentifier(IdentifierNode* node) override;
    
    // 模板相关
    void visitStyleTemplate(StyleTemplateNode* node) override;
    void visitElementTemplate(ElementTemplateNode* node) override;
    void visitVarTemplate(VarTemplateNode* node) override;
    void visitTemplateUse(TemplateUseNode* node) override;
    void visitVarUse(VarUseNode* node) override;
    
    // 原始嵌入
    void visitOriginBlock(OriginBlockNode* node) override;
    
    // CHTL JS相关
    void visitSelectorExpression(SelectorExpressionNode* node) override;
    void visitArrowAccess(ArrowAccessNode* node) override;
    void visitListenCall(ListenCallNode* node) override;
    void visitDelegateCall(DelegateCallNode* node) override;
    void visitAnimateCall(AnimateCallNode* node) override;
    void visitChtlJsExpression(ChtlJsExpressionNode* node) override;
    void visitVirtualObject(VirtualObjectNode* node) override;
    void visitVirtualCall(VirtualCallNode* node) override;
    
private:
    // 输出流
    std::stringstream htmlOutput_;
    std::stringstream cssOutput_;
    std::stringstream jsOutput_;
    
    // 当前输出流
    std::stringstream* currentOutput_;
    
    // 上下文
    Context* context_;
    std::unique_ptr<GenerateContextHelper> genHelper_;
    
    // 缩进管理
    int indentLevel_;
    void indent();
    void dedent();
    void writeIndent();
    
    // HTML特殊字符转义
    std::string escapeHtml(const std::string& str);
    
    // 属性收集（用于元素生成）
    std::unordered_map<std::string, std::string> currentAttributes_;
    std::vector<std::string> currentClasses_;
    std::string currentId_;
    
    // 样式管理
    std::string currentElementAutoClass_;
    
    // 生成唯一类名
    std::string generateUniqueClassName();
    int autoClassCounter_;
    
    // 处理局部样式
    void processLocalStyles(StyleBlockNode* node, const std::string& elementClass);
    
    // 切换输出目标
    void switchToHtml();
    void switchToCss();
    void switchToJs();
    
    // 处理模板展开
    void expandStyleTemplate(const std::string& templateName, 
                           const std::unordered_map<std::string, ASTNodePtr>& params);
    void expandElementTemplate(const std::string& templateName);
};

} // namespace Chtl

#endif // CHTL_GENERATOR_HTMLGENERATOR_H
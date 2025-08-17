#ifndef CHTL_GENERATOR_H
#define CHTL_GENERATOR_H

#include <string>
#include <sstream>
#include <memory>
#include "compiler/chtl/chtl_ast.h"
#include "compiler/chtl/chtl_state.h"

namespace chtl {

// CHTL代码生成器
class CHTLGenerator {
public:
    CHTLGenerator(CompilerState& state);
    ~CHTLGenerator() = default;
    
    // 生成HTML代码
    std::string generate(const ASTNode* ast);
    
    // 获取生成的全局样式
    std::string getGlobalStyles() const;
    
    // 获取生成的全局脚本
    std::string getGlobalScripts() const;
    
private:
    CompilerState& state_;
    std::stringstream output_;
    int indent_level_;
    
    // 缩进管理
    void indent();
    void dedent();
    std::string getIndent() const;
    
    // 节点访问
    void visit(const ASTNode* node);
    void visitRoot(const RootNode* node);
    void visitElement(const ElementNode* node);
    void visitText(const TextNode* node);
    void visitAttribute(const AttributeNode* node);
    void visitStyleBlock(const StyleBlockNode* node);
    void visitStyleRule(const StyleRuleNode* node);
    void visitStyleProperty(const StylePropertyNode* node);
    void visitScriptBlock(const ScriptBlockNode* node);
    void visitComment(const ASTNode* node);
    
    // 模板和自定义
    void visitTemplateDefinition(const TemplateDefinitionNode* node);
    void visitTemplateUsage(const TemplateUsageNode* node);
    void visitCustomDefinition(const CustomDefinitionNode* node);
    void visitCustomUsage(const CustomUsageNode* node);
    
    // 特殊块
    void visitConfiguration(const ConfigurationBlockNode* node);
    void visitNamespace(const NamespaceBlockNode* node);
    void visitOrigin(const OriginBlockNode* node);
    void visitImport(const ImportStatementNode* node);
    void visitVarProperty(const VarPropertyNode* node);
    
    // 辅助方法
    std::string escapeHtml(const std::string& text);
    std::string processLocalStyles(const ElementNode* element);
    void generateAttributes(const ElementNode* element);
    bool isSelfClosingTag(const std::string& tag);
    
    // 样式处理
    void collectLocalStyles(const StyleBlockNode* style_block, const std::string& class_name);
    std::string generateSelector(const std::string& selector, const std::string& context_class);
};

} // namespace chtl

#endif // CHTL_GENERATOR_H
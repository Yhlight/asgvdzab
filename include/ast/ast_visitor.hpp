#pragma once

#include "ast_nodes.hpp"

namespace chtl {
namespace ast {

/**
 * AST访问者接口
 * 实现访问者模式，用于遍历和处理AST节点
 */
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    // 基础节点访问
    virtual void visit(Program& node) = 0;
    virtual void visit(Identifier& node) = 0;
    virtual void visit(Literal& node) = 0;
    
    // HTML元素相关
    virtual void visit(Element& node) = 0;
    virtual void visit(Attribute& node) = 0;
    virtual void visit(TextNode& node) = 0;
    
    // 样式相关
    virtual void visit(StyleBlock& node) = 0;
    virtual void visit(StyleProperty& node) = 0;
    virtual void visit(Selector& node) = 0;
    
    // 模板相关
    virtual void visit(TemplateStyle& node) = 0;
    virtual void visit(TemplateElement& node) = 0;
    virtual void visit(TemplateVar& node) = 0;
    
    // 自定义相关
    virtual void visit(CustomStyle& node) = 0;
    
    // 命名空间和导入
    virtual void visit(Namespace& node) = 0;
    virtual void visit(Import& node) = 0;
    
    // 模板使用和特例化
    virtual void visit(TemplateUsage& node) = 0;
    virtual void visit(Specialization& node) = 0;
    
    // 注释和配置
    virtual void visit(Comment& node) = 0;
    virtual void visit(Configuration& node) = 0;
    virtual void visit(Constraint& node) = 0;
    
    // 变量访问
    virtual void visit(VariableAccess& node) = 0;
};

/**
 * AST基础访问者
 * 提供默认的访问行为，子类可以选择性重写需要的方法
 */
class BaseASTVisitor : public ASTVisitor {
public:
    // 基础节点访问
    void visit(Program& node) override;
    void visit(Identifier& node) override;
    void visit(Literal& node) override;
    
    // HTML元素相关
    void visit(Element& node) override;
    void visit(Attribute& node) override;
    void visit(TextNode& node) override;
    
    // 样式相关
    void visit(StyleBlock& node) override;
    void visit(StyleProperty& node) override;
    void visit(Selector& node) override;
    
    // 模板相关
    void visit(TemplateStyle& node) override;
    void visit(TemplateElement& node) override;
    void visit(TemplateVar& node) override;
    
    // 自定义相关
    void visit(CustomStyle& node) override;
    
    // 命名空间和导入
    void visit(Namespace& node) override;
    void visit(Import& node) override;
    
    // 模板使用和特例化
    void visit(TemplateUsage& node) override;
    void visit(Specialization& node) override;
    
    // 注释和配置
    void visit(Comment& node) override;
    void visit(Configuration& node) override;
    void visit(Constraint& node) override;
    
    // 变量访问
    void visit(VariableAccess& node) override;

protected:
    /**
     * 访问节点的所有子节点
     */
    void visitChildren(const std::vector<ASTNodePtr>& children);
    
    /**
     * 访问单个子节点
     */
    void visitChild(ASTNodePtr child);
};

/**
 * AST打印访问者
 * 用于调试输出AST结构
 */
class ASTPrintVisitor : public BaseASTVisitor {
public:
    explicit ASTPrintVisitor(int indentSize = 2);
    
    void visit(Program& node) override;
    void visit(Identifier& node) override;
    void visit(Literal& node) override;
    void visit(Element& node) override;
    void visit(Attribute& node) override;
    void visit(TextNode& node) override;
    void visit(StyleBlock& node) override;
    void visit(StyleProperty& node) override;
    void visit(Selector& node) override;
    void visit(TemplateStyle& node) override;
    void visit(TemplateElement& node) override;
    void visit(TemplateVar& node) override;
    void visit(CustomStyle& node) override;
    void visit(Namespace& node) override;
    void visit(Import& node) override;
    void visit(TemplateUsage& node) override;
    void visit(Specialization& node) override;
    void visit(Comment& node) override;
    void visit(Configuration& node) override;
    void visit(Constraint& node) override;
    void visit(VariableAccess& node) override;
    
    std::string getOutput() const { return output_; }
    void clear() { output_.clear(); indent_ = 0; }

private:
    void printIndent();
    void print(const std::string& text);
    void println(const std::string& text = "");
    void increaseIndent() { indent_ += indentSize_; }
    void decreaseIndent() { indent_ = std::max(0, indent_ - indentSize_); }
    
    std::string output_;
    int indent_ = 0;
    int indentSize_;
};

/**
 * AST验证访问者
 * 用于验证AST结构的正确性
 */
class ASTValidationVisitor : public BaseASTVisitor {
public:
    struct ValidationError {
        std::string message;
        SourceLocation location;
        std::string nodeType;
    };
    
    void visit(Program& node) override;
    void visit(Element& node) override;
    void visit(Attribute& node) override;
    void visit(TemplateStyle& node) override;
    void visit(TemplateElement& node) override;
    void visit(TemplateVar& node) override;
    void visit(CustomStyle& node) override;
    void visit(Namespace& node) override;
    void visit(Import& node) override;
    void visit(TemplateUsage& node) override;
    void visit(Configuration& node) override;
    void visit(Constraint& node) override;
    
    const std::vector<ValidationError>& getErrors() const { return errors_; }
    bool hasErrors() const { return !errors_.empty(); }
    void clearErrors() { errors_.clear(); }

private:
    void addError(const std::string& message, const SourceLocation& location, const std::string& nodeType);
    bool isValidIdentifier(const std::string& name);
    bool isValidHtmlTag(const std::string& tagName);
    bool isValidCssProperty(const std::string& property);
    
    std::vector<ValidationError> errors_;
};

/**
 * AST统计访问者
 * 用于收集AST的统计信息
 */
class ASTStatisticsVisitor : public BaseASTVisitor {
public:
    struct Statistics {
        int totalNodes = 0;
        int elements = 0;
        int textNodes = 0;
        int attributes = 0;
        int styleBlocks = 0;
        int templateStyles = 0;
        int templateElements = 0;
        int templateVars = 0;
        int customStyles = 0;
        int namespaces = 0;
        int imports = 0;
        int comments = 0;
        int maxDepth = 0;
        int currentDepth = 0;
    };
    
    void visit(Program& node) override;
    void visit(Element& node) override;
    void visit(TextNode& node) override;
    void visit(Attribute& node) override;
    void visit(StyleBlock& node) override;
    void visit(TemplateStyle& node) override;
    void visit(TemplateElement& node) override;
    void visit(TemplateVar& node) override;
    void visit(CustomStyle& node) override;
    void visit(Namespace& node) override;
    void visit(Import& node) override;
    void visit(Comment& node) override;
    
    const Statistics& getStatistics() const { return stats_; }
    void reset() { stats_ = Statistics{}; }

private:
    void enterNode();
    void exitNode();
    
    Statistics stats_;
};

} // namespace ast
} // namespace chtl
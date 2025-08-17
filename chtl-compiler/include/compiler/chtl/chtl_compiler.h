#ifndef CHTL_CHTL_COMPILER_H
#define CHTL_CHTL_COMPILER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <memory>
#include "compiler/compiler_interface.h"
#include "compiler/chtl/chtl_tokens.h"
#include "compiler/chtl/chtl_state.h"

namespace chtl {

// CHTL编译器的AST节点类型
enum class ASTNodeType {
    ROOT,
    ELEMENT,
    TEXT,
    STYLE,
    SCRIPT,
    ATTRIBUTE,
    TEMPLATE_DEF,
    CUSTOM_DEF,
    IMPORT,
    ORIGIN_BLOCK,
    COMMENT
};

// AST节点基类
struct ASTNode {
    ASTNodeType type;
    std::string name;
    std::string value;
    std::vector<std::unique_ptr<ASTNode>> children;
    std::unordered_map<std::string, std::string> attributes;
    SourceLocation location;
    
    ASTNode(ASTNodeType t) : type(t) {}
    virtual ~ASTNode() = default;
};

// 元素节点
struct ElementNode : public ASTNode {
    std::string tag_name;
    bool has_local_style = false;
    bool has_local_script = false;
    std::string generated_class_name; // 自动生成的类名
    
    ElementNode() : ASTNode(ASTNodeType::ELEMENT) {}
};

// 模板定义节点
struct TemplateNode : public ASTNode {
    enum TemplateType { STYLE, ELEMENT, VAR };
    TemplateType template_type;
    std::string template_name;
    std::vector<std::string> inherited_templates;
    
    TemplateNode() : ASTNode(ASTNodeType::TEMPLATE_DEF) {}
};

// 编译上下文
struct CompilationContext {
    // 模板存储
    std::unordered_map<std::string, std::unique_ptr<TemplateNode>> style_templates;
    std::unordered_map<std::string, std::unique_ptr<TemplateNode>> element_templates;
    std::unordered_map<std::string, std::unique_ptr<TemplateNode>> var_templates;
    
    // 自定义存储
    std::unordered_map<std::string, std::unique_ptr<ASTNode>> custom_styles;
    std::unordered_map<std::string, std::unique_ptr<ASTNode>> custom_elements;
    std::unordered_map<std::string, std::unique_ptr<ASTNode>> custom_vars;
    
    // 全局样式收集
    std::vector<std::string> global_styles;
    
    // 类名生成计数器
    size_t class_counter = 0;
    
    // 命名空间栈
    std::vector<std::string> namespace_stack;
    
    // 错误和警告
    std::vector<CompilerError> errors;
    std::vector<CompilerError> warnings;
};

// CHTL编译器
class CHTLCompiler : public ICompiler {
public:
    CHTLCompiler();
    ~CHTLCompiler() override;
    
    // 实现ICompiler接口
    CompilationResult compile(const CodeFragment& fragment, 
                            const CompilerOptions& options = {}) override;
    
    std::string getName() const override { return "CHTL Compiler"; }
    std::string getVersion() const override { return "1.0.0"; }
    bool supports(CodeFragmentType type) const override {
        return type == CodeFragmentType::CHTL;
    }
    
private:
    
    class Lexer {
    public:
        Lexer(const std::string& input, size_t start_line = 1);
        Token nextToken();
        Token peekToken();
        void consumeToken();
        bool isAtEnd() const { return position_ >= input_.length(); }
        
    private:
        std::string input_;
        size_t position_;
        size_t line_;
        size_t column_;
        std::optional<Token> peeked_token_;
        
        void skipWhitespace();
        void skipComment();
        Token scanIdentifier();
        Token scanString();
        Token scanNumber();
        char peek(size_t offset = 0) const;
        void advance();
    };
    
    // 语法分析
    class Parser {
    public:
        Parser(Lexer& lexer, CompilationContext& context);
        std::unique_ptr<ASTNode> parse();
        
    private:
        Lexer& lexer_;
        CompilationContext& context_;
        
        // 解析方法
        std::unique_ptr<ASTNode> parseRoot();
        std::unique_ptr<ASTNode> parseElement();
        void parseElementContent(ElementNode* element);
        std::unique_ptr<ASTNode> parseText();
        std::unique_ptr<ASTNode> parseStyle();
        std::unique_ptr<ASTNode> parseScript();
        std::unique_ptr<ASTNode> parseTemplate();
        std::unique_ptr<ASTNode> parseCustom();
        std::unique_ptr<ASTNode> parseImport();
        std::unique_ptr<ASTNode> parseOrigin();
        std::unique_ptr<ASTNode> parseAttribute();
        
        // 辅助方法
        bool expect(TokenType type);
        bool match(TokenType type);
        bool matchKeyword(const std::string& keyword);
        void synchronize(); // 错误恢复
        
        void reportError(const std::string& message);
        void reportWarning(const std::string& message);
    };
    
    // 代码生成
    class CodeGenerator {
    public:
        CodeGenerator(CompilationContext& context, const CompilerOptions& options);
        std::string generate(const std::unique_ptr<ASTNode>& ast);
        
    private:
        CompilationContext& context_;
        CompilerOptions options_;
        std::string output_;
        size_t indent_level_;
        
        // 生成方法
        void generateNode(const std::unique_ptr<ASTNode>& node);
        void generateElement(const ElementNode* element);
        void generateText(const ASTNode* text);
        void generateAttributes(const std::unordered_map<std::string, std::string>& attrs);
        void generateLocalStyle(const ElementNode* element);
        
        // 模板处理
        void expandTemplate(const std::string& template_name, const std::string& type);
        
        // 辅助方法
        void indent();
        void dedent();
        void emit(const std::string& code);
        void emitLine(const std::string& code);
        std::string generateClassName();
        std::string escapeHtml(const std::string& text);
    };
    
private:
    // 编译流程
    std::unique_ptr<ASTNode> parseInput(const std::string& input, CompilationContext& context);
    std::string generateOutput(const std::unique_ptr<ASTNode>& ast, 
                              CompilationContext& context,
                              const CompilerOptions& options);
};

} // namespace chtl

#endif // CHTL_CHTL_COMPILER_H
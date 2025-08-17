#ifndef CHTL_CHTLJS_COMPILER_H
#define CHTL_CHTLJS_COMPILER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <optional>
#include "compiler/compiler_interface.h"

namespace chtl {

// CHTL JS编译器
class CHTLJSCompiler : public ICompiler {
public:
    CHTLJSCompiler();
    ~CHTLJSCompiler() override;
    
    // 实现ICompiler接口
    CompilationResult compile(const CodeFragment& fragment, 
                            const CompilerOptions& options = {}) override;
    
    std::string getName() const override { return "CHTL JS Compiler"; }
    std::string getVersion() const override { return "1.0.0"; }
    bool supports(CodeFragmentType type) const override {
        return type == CodeFragmentType::CHTL_JS;
    }
    
private:
    // AST节点类型
    enum class ASTNodeType {
        PROGRAM,
        SELECTOR_EXPR,      // {{选择器}}
        MEMBER_ACCESS,      // . 或 ->
        FUNCTION_CALL,      // 函数调用
        OBJECT_LITERAL,     // 对象字面量
        ARRAY_LITERAL,      // 数组字面量
        VIR_DECLARATION,    // vir声明
        IDENTIFIER,         // 标识符
        STRING,             // 字符串
        NUMBER,             // 数字
        BINARY_OP,          // 二元操作
        ASSIGNMENT,         // 赋值
        BLOCK,              // 代码块
        IF_STATEMENT,       // if语句
        FOR_STATEMENT,      // for语句
        FUNCTION_EXPR,      // 函数表达式
        RETURN_STATEMENT,   // return语句
        EXPRESSION_STATEMENT, // 表达式语句
        VARIABLE_DECLARATION // 变量声明
    };
    
    // AST节点
    struct ASTNode {
        ASTNodeType type;
        std::string value;
        std::vector<std::unique_ptr<ASTNode>> children;
        SourceLocation location;
        
        // 特定类型的额外数据
        std::optional<std::string> selector_type; // 对于SELECTOR_EXPR: "tag", "class", "id", "complex"
        std::optional<int> array_index; // 对于数组访问
        
        ASTNode(ASTNodeType t) : type(t) {}
    };
    
    // 虚对象信息
    struct VirObject {
        std::string name;
        std::unordered_map<std::string, std::string> methods; // 方法名 -> 生成的函数名
    };
    
    // 编译上下文
    struct CompilationContext {
        // 虚对象注册表
        std::unordered_map<std::string, VirObject> vir_objects;
        
        // 生成的辅助函数
        std::vector<std::string> helper_functions;
        
        // 事件委托注册表
        std::unordered_map<std::string, std::vector<std::string>> delegation_registry;
        
        // 唯一ID生成器
        size_t unique_id = 0;
        
        // 错误和警告
        std::vector<CompilerError> errors;
        std::vector<CompilerError> warnings;
        
        // 当前作用域中的元素类名（来自父元素）
        std::string current_element_class;
    };
    
    // 词法分析器
    class Lexer {
    public:
        enum TokenType {
            // 字面量
            IDENTIFIER, STRING, NUMBER,
            
            // 关键字
            VIR, CONST, LET, VAR, FUNCTION, RETURN, IF, ELSE, FOR, WHILE,
            LISTEN, DELEGATE, ANIMATE,
            
            // 操作符
            LBRACE, RBRACE,         // { }
            LPAREN, RPAREN,         // ( )
            LBRACKET, RBRACKET,     // [ ]
            SELECTOR_START,         // {{
            SELECTOR_END,           // }}
            DOT,                    // .
            ARROW,                  // ->
            COMMA,                  // ,
            SEMICOLON,              // ;
            COLON,                  // :
            EQUALS,                 // =
            PLUS, MINUS, STAR, SLASH,  // + - * /
            LT, GT, LE, GE, EQ, NE,    // < > <= >= == !=
            AND, OR, NOT,              // && || !
            
            // 特殊
            EOF_TOKEN, ERROR
        };
        
        struct Token {
            TokenType type;
            std::string value;
            size_t line;
            size_t column;
        };
        
        Lexer(const std::string& input, size_t start_line = 1);
        Token nextToken();
        Token peekToken();
        bool isAtEnd() const { return position_ >= input_.length(); }
        
    private:
        std::string input_;
        size_t position_;
        size_t line_;
        size_t column_;
        std::optional<Token> peeked_token_;
        
        void skipWhitespace();
        Token scanIdentifier();
        Token scanString();
        Token scanNumber();
        Token scanSelector();
        char peek(size_t offset = 0) const;
        void advance();
    };
    
    // 语法分析器
    class Parser {
    public:
        Parser(Lexer& lexer, CompilationContext& context);
        std::unique_ptr<ASTNode> parse();
        
    private:
        Lexer& lexer_;
        CompilationContext& context_;
        
        // 解析方法
        std::unique_ptr<ASTNode> parseProgram();
        std::unique_ptr<ASTNode> parseStatement();
        std::unique_ptr<ASTNode> parseVirDeclaration();
        std::unique_ptr<ASTNode> parseVariableDeclaration();
        std::unique_ptr<ASTNode> parseExpressionStatement();
        std::unique_ptr<ASTNode> parseExpression();
        std::unique_ptr<ASTNode> parsePrimaryExpression();
        std::unique_ptr<ASTNode> parsePostfixExpression();
        std::unique_ptr<ASTNode> parseMemberExpression(std::unique_ptr<ASTNode> object);
        std::unique_ptr<ASTNode> parseCallExpression(std::unique_ptr<ASTNode> callee);
        std::unique_ptr<ASTNode> parseSelectorExpression();
        std::unique_ptr<ASTNode> parseObjectLiteral();
        std::unique_ptr<ASTNode> parseFunctionExpression();
        std::unique_ptr<ASTNode> parseArrowFunction();
        
        // 辅助方法
        bool match(Lexer::TokenType type);
        bool expect(Lexer::TokenType type);
        void synchronize();
        void reportError(const std::string& message);
    };
    
    // 代码生成器
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
        void generateSelectorExpression(const std::unique_ptr<ASTNode>& node);
        void generateVirDeclaration(const std::unique_ptr<ASTNode>& node);
        void generateListenCall(const std::unique_ptr<ASTNode>& node);
        void generateDelegateCall(const std::unique_ptr<ASTNode>& node);
        void generateAnimateCall(const std::unique_ptr<ASTNode>& node);
        void generateMemberAccess(const std::unique_ptr<ASTNode>& node);
        void generateFunctionCall(const std::unique_ptr<ASTNode>& node);
        
        // 辅助方法
        std::string generateUniqueId();
        std::string convertSelectorToJS(const std::string& selector, const std::string& type);
        void emit(const std::string& code);
        void emitLine(const std::string& code);
        void indent();
        void dedent();
        
        // 生成辅助函数
        void generateHelperFunctions();
        std::string generateListenHelper();
        std::string generateDelegateHelper();
        std::string generateAnimateHelper();
    };
    
private:
    // 主要编译流程
    std::unique_ptr<ASTNode> parseInput(const std::string& input, CompilationContext& context);
    std::string generateOutput(const std::unique_ptr<ASTNode>& ast, 
                              CompilationContext& context,
                              const CompilerOptions& options);
    
    // CHTL JS特殊函数集合
    static const std::unordered_set<std::string> chtljs_functions_;
};

} // namespace chtl

#endif // CHTL_CHTLJS_COMPILER_H
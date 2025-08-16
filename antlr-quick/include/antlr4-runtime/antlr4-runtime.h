#pragma once

// 简化的ANTLR4运行时兼容接口
// 用于支持CHTL项目的基本需求

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>

namespace antlr4 {

// 基础异常类
class RuntimeException : public std::runtime_error {
public:
    explicit RuntimeException(const std::string& message) : std::runtime_error(message) {}
};

// 输入流类
class ANTLRInputStream {
public:
    explicit ANTLRInputStream(const std::string& input) : input_(input) {}
    const std::string& toString() const { return input_; }
private:
    std::string input_;
};

// Token类
class Token {
public:
    virtual ~Token() = default;
    virtual std::string getText() const = 0;
    virtual size_t getType() const = 0;
};

// 基础Token实现
class CommonToken : public Token {
public:
    CommonToken(size_t type, const std::string& text) : type_(type), text_(text) {}
    std::string getText() const override { return text_; }
    size_t getType() const override { return type_; }
private:
    size_t type_;
    std::string text_;
};

// Token流
class TokenStream {
public:
    virtual ~TokenStream() = default;
    virtual Token* LT(int k) = 0;
    virtual void consume() = 0;
};

class CommonTokenStream : public TokenStream {
public:
    explicit CommonTokenStream(class Lexer* lexer) : lexer_(lexer) {}
    Token* LT(int k) override { return nullptr; }
    void consume() override {}
private:
    class Lexer* lexer_;
};

// 词法分析器基类
class Lexer {
public:
    explicit Lexer(ANTLRInputStream* input) : input_(input) {}
    virtual ~Lexer() = default;
    virtual void removeErrorListeners() {}
    virtual void addErrorListener(class BaseErrorListener* listener) {}
protected:
    ANTLRInputStream* input_;
};

// 解析器基类
class Parser {
public:
    explicit Parser(TokenStream* input) : input_(input) {}
    virtual ~Parser() = default;
    virtual void removeErrorListeners() {}
    virtual void addErrorListener(class BaseErrorListener* listener) {}
protected:
    TokenStream* input_;
};

// 解析树节点
namespace tree {
    class ParseTree {
    public:
        virtual ~ParseTree() = default;
        virtual std::string getText() const { return ""; }
        virtual size_t getChildCount() const { return 0; }
        virtual ParseTree* getChild(size_t index) const { return nullptr; }
    };
    
    class ParseTreeWalker {
    public:
        void walk(class ParseTreeListener* listener, ParseTree* tree) {
            // 简化实现：什么都不做
        }
    };
}

// 错误监听器
class BaseErrorListener {
public:
    virtual ~BaseErrorListener() = default;
    virtual void syntaxError(class Recognizer* recognizer, Token* offendingSymbol,
                           size_t line, size_t charPositionInLine, 
                           const std::string& msg, std::exception_ptr e) {}
};

// 识别器基类
class Recognizer {
public:
    virtual ~Recognizer() = default;
};

// 解析树监听器
class ParseTreeListener {
public:
    virtual ~ParseTreeListener() = default;
};

} // namespace antlr4

// 简化的生成代码兼容性
class CSS3BaseListener : public antlr4::ParseTreeListener {
public:
    virtual ~CSS3BaseListener() = default;
};


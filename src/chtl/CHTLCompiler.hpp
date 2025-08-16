#pragma once

#include <string>
#include <memory>

class CHTLLexer;
class CHTLParser;
class CHTLGenerator;

/**
 * CHTL编译器
 * 负责编译CHTL代码片段
 */
class CHTLCompiler {
public:
    CHTLCompiler();
    ~CHTLCompiler();
    
    /**
     * 编译CHTL代码片段
     * @param source CHTL源代码
     * @return 编译后的HTML代码
     */
    std::string compile(const std::string& source);
    
private:
    // 暂时注释掉，避免incomplete type错误
    // std::unique_ptr<CHTLLexer> lexer_;
    // std::unique_ptr<CHTLParser> parser_;
    // std::unique_ptr<CHTLGenerator> generator_;
};
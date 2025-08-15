#ifndef BASE_COMPILER_H
#define BASE_COMPILER_H

#include <string>
#include <memory>
#include <vector>

namespace chtl {

// 编译器基类接口
class BaseCompiler {
public:
    // 编译选项
    struct CompileOptions {
        bool enableOptimization = true;
        bool generateSourceMap = false;
        bool minify = false;
        bool strictMode = true;
        std::string outputFormat = "default";
    };
    
    // 编译错误信息
    struct CompileError {
        enum Severity {
            WARNING,
            ERROR,
            FATAL
        };
        
        Severity severity;
        std::string message;
        size_t line;
        size_t column;
        std::string context;
        
        CompileError() : severity(ERROR), line(0), column(0) {}
    };
    
    BaseCompiler() = default;
    virtual ~BaseCompiler() = default;
    
    // 编译接口
    virtual bool compile(const std::string& input, 
                        std::string& output,
                        std::vector<CompileError>& errors) = 0;
    
    // 设置编译选项
    virtual void setOptions(const CompileOptions& options) = 0;
    
    // 获取编译器名称
    virtual std::string getName() const = 0;
    
    // 获取编译器版本
    virtual std::string getVersion() const = 0;
    
    // 验证输入是否有效
    virtual bool validate(const std::string& input) const = 0;
    
protected:
    CompileOptions options_;
};

} // namespace chtl

#endif // BASE_COMPILER_H
#ifndef CHTL_COMPILER_INTERFACE_H
#define CHTL_COMPILER_INTERFACE_H

#include <string>
#include <memory>
#include <functional>
#include "utils/common.h"

namespace chtl {

// 编译器接口基类
class ICompiler {
public:
    virtual ~ICompiler() = default;
    
    // 编译代码片段
    virtual CompilationResult compile(const CodeFragment& fragment, 
                                    const CompilerOptions& options = {}) = 0;
    
    // 获取编译器名称
    virtual std::string getName() const = 0;
    
    // 获取编译器版本
    virtual std::string getVersion() const = 0;
    
    // 检查是否支持特定的代码片段类型
    virtual bool supports(CodeFragmentType type) const = 0;
    
    // 设置错误处理回调
    virtual void setErrorHandler(std::function<void(const CompilerError&)> handler) {
        error_handler_ = handler;
    }
    
protected:
    // 错误处理
    void reportError(const std::string& message, const SourceLocation& location) {
        CompilerError error;
        error.severity = CompilerError::ERROR;
        error.message = message;
        error.location = location;
        
        if (error_handler_) {
            error_handler_(error);
        }
    }
    
    void reportWarning(const std::string& message, const SourceLocation& location) {
        CompilerError warning;
        warning.severity = CompilerError::WARNING;
        warning.message = message;
        warning.location = location;
        
        if (error_handler_) {
            error_handler_(warning);
        }
    }
    
private:
    std::function<void(const CompilerError&)> error_handler_;
};

// 编译器工厂
class CompilerFactory {
public:
    static std::unique_ptr<ICompiler> createCompiler(CodeFragmentType type);
};

} // namespace chtl

#endif // CHTL_COMPILER_INTERFACE_H
#ifndef CHTL_BASE_COMPILER_H
#define CHTL_BASE_COMPILER_H

#include "common/types.h"
#include <string>
#include <memory>

namespace chtl {

// 编译器基类接口
class ICompiler {
public:
    virtual ~ICompiler() = default;
    
    // 编译代码片段
    virtual CompileResult compile(const CodeFragment& fragment, 
                                 const CompileOptions& options) = 0;
    
    // 获取编译器名称
    virtual std::string getName() const = 0;
    
    // 获取支持的代码片段类型
    virtual CodeFragmentType getSupportedType() const = 0;
    
    // 设置调试模式
    virtual void setDebugMode(bool debug) = 0;
    
    // 验证代码片段
    virtual bool validate(const CodeFragment& fragment) const {
        return fragment.type == getSupportedType();
    }
};

// 编译器工厂
class CompilerFactory {
public:
    static std::unique_ptr<ICompiler> createCompiler(CodeFragmentType type);
};

} // namespace chtl

#endif // CHTL_BASE_COMPILER_H
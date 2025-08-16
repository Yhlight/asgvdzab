#ifndef CHTL_COMPILER_BASE_COMPILER_H
#define CHTL_COMPILER_BASE_COMPILER_H

#include <string>
#include <memory>
#include <atomic>
#include "common/types.h"

namespace chtl {
namespace compiler {

// 编译器基类接口
class BaseCompiler {
public:
    virtual ~BaseCompiler() = default;
    
    // 编译代码片段
    virtual CompilationResult compile(const CodeSegment& segment, 
                                     const CompilerOptions& options = CompilerOptions()) = 0;
    
    // 获取编译器名称
    virtual std::string getName() const = 0;
    
    // 获取编译器版本
    virtual std::string getVersion() const { return "1.0.0"; }
    
    // 检查是否支持特定的代码片段类型
    virtual bool supports(CodeSegmentType type) const = 0;
    
    // 重置编译器状态（用于处理多个文件）
    virtual void reset() {}
    
protected:
    // 辅助方法：生成唯一标识符
    static std::string generateUniqueId(const std::string& prefix);
    
private:
    static std::atomic<size_t> idCounter_;
};

// 编译器工厂
class CompilerFactory {
public:
    // 根据代码片段类型创建相应的编译器
    static std::unique_ptr<BaseCompiler> createCompiler(CodeSegmentType type);
    
    // 注册自定义编译器（扩展用）
    static void registerCompiler(CodeSegmentType type, 
                                std::unique_ptr<BaseCompiler> compiler);
};

} // namespace compiler
} // namespace chtl

#endif // CHTL_COMPILER_BASE_COMPILER_H
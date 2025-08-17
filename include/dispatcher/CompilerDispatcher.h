#pragma once

#include "../common/Types.h"
#include "../compilers/CHTLCompiler.h"
#include "../compilers/CHTLJSCompiler.h"
#include "../compilers/CSSCompiler.h"
#include "../compilers/JavaScriptCompiler.h"
#include <memory>
#include <vector>

namespace CHTL {

// 前向声明
class CHTLCompiler;
class CHTLJSCompiler;
class CSSCompiler;
class JavaScriptCompiler;

/**
 * 编译器调度器
 * 负责将代码片段分发给对应的编译器进行处理
 * 管理四个编译器：CHTL编译器、CHTL JS编译器、CSS编译器、JavaScript编译器
 */
class CompilerDispatcher {
private:
    // 各个编译器实例
    std::unique_ptr<CHTLCompiler> chtl_compiler;
    std::unique_ptr<CHTLJSCompiler> chtl_js_compiler;
    std::unique_ptr<CSSCompiler> css_compiler;
    std::unique_ptr<JavaScriptCompiler> js_compiler;
    
    // 配置
    Configuration config;
    
    // 编译结果缓存
    std::vector<CompileResultPtr> compile_results;
    
    // 内部方法
    CompilerType getCompilerType(FragmentType fragment_type) const;
    CompileResultPtr dispatchToCompiler(FragmentPtr fragment);
    
    // 编译器初始化
    void initializeCompilers();
    
public:
    CompilerDispatcher();
    explicit CompilerDispatcher(const Configuration& cfg);
    ~CompilerDispatcher();
    
    // 设置配置
    void setConfiguration(const Configuration& cfg);
    
    // 主要编译方法
    std::vector<CompileResultPtr> compileFragments(const std::vector<FragmentPtr>& fragments);
    
    // 单个片段编译
    CompileResultPtr compileFragment(FragmentPtr fragment);
    
    // 编译器管理
    void resetCompilers();
    void configureCompilers(const Configuration& cfg);
    
    // 获取编译器实例(用于高级操作)
    CHTLCompiler* getCHTLCompiler() const { return chtl_compiler.get(); }
    CHTLJSCompiler* getCHTLJSCompiler() const { return chtl_js_compiler.get(); }
    CSSCompiler* getCSSCompiler() const { return css_compiler.get(); }
    JavaScriptCompiler* getJSCompiler() const { return js_compiler.get(); }
    
    // 错误和警告收集
    std::vector<std::string> getAllErrors() const;
    std::vector<std::string> getAllWarnings() const;
    
    // 统计信息
    size_t getCompiledFragmentCount() const;
    size_t getSuccessfulCompileCount() const;
    size_t getFailedCompileCount() const;
    
    // 调试方法
    void dumpCompileResults() const;
    void setDebugMode(bool enabled);
};

} // namespace CHTL
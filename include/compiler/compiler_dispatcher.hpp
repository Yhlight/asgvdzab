#ifndef CHTL_COMPILER_DISPATCHER_HPP
#define CHTL_COMPILER_DISPATCHER_HPP

#include "core/types.hpp"
#include "compiler/compiler_interface.hpp"
#include <memory>
#include <unordered_map>
#include <functional>

namespace CHTL {

// 编译管道数据
struct CompilationPipeline {
    std::vector<CodeFragment> fragments;
    std::unordered_map<std::string, std::string> globalContext;
    CompilerConfig config;
    std::string sourceFilename;
    
    // 编译状态
    bool hasErrors = false;
    std::vector<CompileError> errors;
    std::vector<CompileError> warnings;
    
    // 输出缓存
    std::string htmlOutput;
    std::string cssOutput;
    std::string jsOutput;
    
    void addError(const CompileError& error) {
        errors.push_back(error);
        hasErrors = true;
    }
    
    void addWarning(const CompileError& warning) {
        warnings.push_back(warning);
    }
};

// 编译器调度器
class CompilerDispatcher {
public:
    explicit CompilerDispatcher(const CompilerConfig& config = CompilerConfig{});
    ~CompilerDispatcher() = default;
    
    // 注册编译器
    void registerCHTLCompiler(std::unique_ptr<ICHTLCompiler> compiler);
    void registerCHTLJSCompiler(std::unique_ptr<ICHTLJSCompiler> compiler);
    void registerCSSCompiler(std::unique_ptr<ICSSCompiler> compiler);
    void registerJavaScriptCompiler(std::unique_ptr<IJavaScriptCompiler> compiler);
    
    // 主要编译接口
    CompileResult compile(const std::vector<CodeFragment>& fragments, const std::string& filename = "");
    
    // 设置配置
    void setConfig(const CompilerConfig& config);
    const CompilerConfig& getConfig() const { return config_; }
    
    // 编译器状态查询
    bool hasCHTLCompiler() const { return chtlCompiler_ != nullptr; }
    bool hasCHTLJSCompiler() const { return chtlJSCompiler_ != nullptr; }
    bool hasCSSCompiler() const { return cssCompiler_ != nullptr; }
    bool hasJavaScriptCompiler() const { return jsCompiler_ != nullptr; }
    
    // 错误处理
    const std::vector<CompileError>& getErrors() const { return errors_; }
    const std::vector<CompileError>& getWarnings() const { return warnings_; }
    void clearErrors() { errors_.clear(); warnings_.clear(); }
    
private:
    CompilerConfig config_;
    
    // 编译器实例
    std::unique_ptr<ICHTLCompiler> chtlCompiler_;
    std::unique_ptr<ICHTLJSCompiler> chtlJSCompiler_;
    std::unique_ptr<ICSSCompiler> cssCompiler_;
    std::unique_ptr<IJavaScriptCompiler> jsCompiler_;
    
    // 错误收集
    std::vector<CompileError> errors_;
    std::vector<CompileError> warnings_;
    
    // 编译管道处理
    CompilationPipeline createPipeline(const std::vector<CodeFragment>& fragments, const std::string& filename);
    void preprocessFragments(CompilationPipeline& pipeline);
    void dispatchFragments(CompilationPipeline& pipeline);
    void postprocessResults(CompilationPipeline& pipeline);
    CompileResult generateFinalResult(const CompilationPipeline& pipeline);
    
    // 片段分组和排序
    struct FragmentGroup {
        std::vector<CodeFragment> chtlFragments;
        std::vector<CodeFragment> chtlJSFragments;
        std::vector<CodeFragment> cssFragments;
        std::vector<CodeFragment> jsFragments;
        std::vector<CodeFragment> htmlFragments;
    };
    
    FragmentGroup groupFragments(const std::vector<CodeFragment>& fragments);
    void sortFragmentsByLocation(std::vector<CodeFragment>& fragments);
    
    // 单个编译器调用
    CompileResult compileCHTLFragments(const std::vector<CodeFragment>& fragments, CompilationPipeline& pipeline);
    CompileResult compileCHTLJSFragments(const std::vector<CodeFragment>& fragments, CompilationPipeline& pipeline);
    CompileResult compileCSSFragments(const std::vector<CodeFragment>& fragments, CompilationPipeline& pipeline);
    CompileResult compileJavaScriptFragments(const std::vector<CodeFragment>& fragments, CompilationPipeline& pipeline);
    
    // 结果合并
    void mergeCompileResult(const CompileResult& result, CompilationPipeline& pipeline);
    std::string generateHTMLOutput(const CompilationPipeline& pipeline);
    
    // 依赖解析
    void resolveDependencies(CompilationPipeline& pipeline);
    void validateFragmentOrder(const std::vector<CodeFragment>& fragments);
    
    // 错误处理
    void reportError(const std::string& message, const SourceLocation& location);
    void reportWarning(const std::string& message, const SourceLocation& location);
    
    // 上下文管理
    void updateGlobalContext(CompilationPipeline& pipeline);
    std::unordered_map<std::string, std::string> extractContextFromFragment(const CodeFragment& fragment);
    
    // 优化
    void optimizeOutput(CompilationPipeline& pipeline);
    void removeRedundantCSS(std::string& css);
    void removeRedundantJS(std::string& js);
    void minifyOutput(CompilationPipeline& pipeline);
    
    // 调试支持
    void dumpPipelineState(const CompilationPipeline& pipeline, const std::string& stage);
    void validatePipelineIntegrity(const CompilationPipeline& pipeline);
};

// 编译器工厂
class CompilerFactory {
public:
    // 创建默认编译器实例
    static std::unique_ptr<ICHTLCompiler> createCHTLCompiler(const CompilerConfig& config);
    static std::unique_ptr<ICHTLJSCompiler> createCHTLJSCompiler(const CompilerConfig& config);
    static std::unique_ptr<ICSSCompiler> createCSSCompiler(const CompilerConfig& config);
    static std::unique_ptr<IJavaScriptCompiler> createJavaScriptCompiler(const CompilerConfig& config);
    
    // 创建完整配置的调度器
    static std::unique_ptr<CompilerDispatcher> createFullDispatcher(const CompilerConfig& config = CompilerConfig{});
    
    // 注册自定义编译器工厂函数
    using CHTLCompilerFactory = std::function<std::unique_ptr<ICHTLCompiler>(const CompilerConfig&)>;
    using CHTLJSCompilerFactory = std::function<std::unique_ptr<ICHTLJSCompiler>(const CompilerConfig&)>;
    using CSSCompilerFactory = std::function<std::unique_ptr<ICSSCompiler>(const CompilerConfig&)>;
    using JavaScriptCompilerFactory = std::function<std::unique_ptr<IJavaScriptCompiler>(const CompilerConfig&)>;
    
    static void registerCHTLCompilerFactory(CHTLCompilerFactory factory);
    static void registerCHTLJSCompilerFactory(CHTLJSCompilerFactory factory);
    static void registerCSSCompilerFactory(CSSCompilerFactory factory);
    static void registerJavaScriptCompilerFactory(JavaScriptCompilerFactory factory);
    
private:
    static CHTLCompilerFactory chtlCompilerFactory_;
    static CHTLJSCompilerFactory chtlJSCompilerFactory_;
    static CSSCompilerFactory cssCompilerFactory_;
    static JavaScriptCompilerFactory jsCompilerFactory_;
};

} // namespace CHTL

#endif // CHTL_COMPILER_DISPATCHER_HPP
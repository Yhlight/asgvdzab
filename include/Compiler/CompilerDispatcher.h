#pragma once

#include "../Core/Types.h"
#include "../Core/Context.h"
#include "../Scanner/UnifiedScanner.h"
#include <memory>
#include <functional>
#include <chrono>

namespace CHTL {
namespace Compiler {

// 前向声明
class CHTLCompiler;
class CHTLJSCompiler;
class CSSCompiler;
class JavaScriptCompiler;

// 编译结果
struct CompileResult {
    Core::String output;               // 编译输出
    bool success = false;              // 编译是否成功
    Core::ErrorList errors;            // 编译错误
    Core::ErrorList warnings;          // 编译警告
    Core::StringMap metadata;          // 元数据
    
    CompileResult() = default;
    CompileResult(const Core::String& out, bool succ = true)
        : output(out), success(succ) {}
};

using CompileResultPtr = std::shared_ptr<CompileResult>;

// 编译器接口
class ICompiler {
public:
    virtual ~ICompiler() = default;
    
    // 编译接口
    virtual CompileResultPtr compile(const Core::CodeFragmentPtr& fragment, 
                                   Core::Context& context) = 0;
    
    // 编译器信息
    virtual Core::String getName() const = 0;
    virtual Core::String getVersion() const = 0;
    virtual Core::FragmentType getSupportedType() const = 0;
    
    // 编译器配置
    virtual void setOptions(const Core::StringMap& options) = 0;
    virtual Core::StringMap getOptions() const = 0;
    
    // 编译器状态
    virtual bool isReady() const = 0;
    virtual void reset() = 0;
};

using CompilerPtr = std::shared_ptr<ICompiler>;

// 编译器调度器
class CompilerDispatcher {
public:
    CompilerDispatcher();
    ~CompilerDispatcher() = default;

    // 主要编译接口
    CompileResultPtr compileSource(const Core::String& source, 
                                  const Core::String& filename,
                                  Core::Context& context);
    
    CompileResultPtr compileFragments(const Core::CodeFragmentList& fragments,
                                     Core::Context& context);
    
    CompileResultPtr compileFragment(const Core::CodeFragmentPtr& fragment,
                                    Core::Context& context);
    
    // 编译器管理
    void registerCompiler(Core::FragmentType type, CompilerPtr compiler);
    void unregisterCompiler(Core::FragmentType type);
    CompilerPtr getCompiler(Core::FragmentType type) const;
    bool hasCompiler(Core::FragmentType type) const;
    
    // 扫描器管理
    void setScanner(std::shared_ptr<Scanner::CHTLUnifiedScanner> scanner);
    std::shared_ptr<Scanner::CHTLUnifiedScanner> getScanner() const;
    
    // 结果合并器
    using ResultMerger = std::function<CompileResultPtr(
        const std::vector<CompileResultPtr>&, Core::Context&)>;
    
    void setResultMerger(ResultMerger merger) { resultMerger_ = merger; }
    ResultMerger getResultMerger() const { return resultMerger_; }
    
    // 编译选项
    void setGlobalOptions(const Core::StringMap& options);
    void setCompilerOptions(Core::FragmentType type, const Core::StringMap& options);
    Core::StringMap getGlobalOptions() const { return globalOptions_; }
    
    // 编译统计
    struct CompileStats {
        size_t totalFragments = 0;
        size_t successfulCompiles = 0;
        size_t failedCompiles = 0;
        std::chrono::milliseconds totalTime{0};
        Core::StringMap compilerStats;
    };
    
    const CompileStats& getStats() const { return stats_; }
    void resetStats();
    
    // 错误处理
    const Core::ErrorList& getErrors() const { return errors_; }
    const Core::ErrorList& getWarnings() const { return warnings_; }
    bool hasErrors() const { return !errors_.empty(); }
    bool hasWarnings() const { return !warnings_.empty(); }
    void clearErrors() { errors_.clear(); }
    void clearWarnings() { warnings_.clear(); }
    
    // 调试和日志
    void enableDebug(bool enable) { debugMode_ = enable; }
    bool isDebugEnabled() const { return debugMode_; }
    
    // 编译流水线配置
    struct PipelineConfig {
        bool parallelCompilation = true;   // 并行编译
        bool cacheResults = true;          // 缓存结果
        bool optimizeOutput = true;        // 优化输出
        size_t maxThreads = 4;             // 最大线程数
    };
    
    void setPipelineConfig(const PipelineConfig& config) { pipelineConfig_ = config; }
    const PipelineConfig& getPipelineConfig() const { return pipelineConfig_; }

private:
    // 编译器注册表
    std::unordered_map<Core::FragmentType, CompilerPtr> compilers_;
    
    // 扫描器
    std::shared_ptr<Scanner::CHTLUnifiedScanner> scanner_;
    
    // 结果合并器
    ResultMerger resultMerger_;
    
    // 配置和选项
    Core::StringMap globalOptions_;
    std::unordered_map<Core::FragmentType, Core::StringMap> compilerOptions_;
    PipelineConfig pipelineConfig_;
    
    // 统计和错误
    CompileStats stats_;
    Core::ErrorList errors_;
    Core::ErrorList warnings_;
    bool debugMode_ = false;
    
    // 缓存
    mutable std::unordered_map<Core::String, CompileResultPtr> resultCache_;
    
    // 初始化
    void initializeCompilers();
    void initializeDefaultMerger();
    
    // 编译辅助函数
    CompileResultPtr compileFragmentInternal(const Core::CodeFragmentPtr& fragment,
                                            Core::Context& context);
    
    std::vector<CompileResultPtr> compileFragmentsParallel(
        const Core::CodeFragmentList& fragments, Core::Context& context);
    
    std::vector<CompileResultPtr> compileFragmentsSequential(
        const Core::CodeFragmentList& fragments, Core::Context& context);
    
    // 缓存管理
    Core::String generateCacheKey(const Core::CodeFragmentPtr& fragment) const;
    bool isCacheValid(const Core::String& key) const;
    void cacheResult(const Core::String& key, CompileResultPtr result);
    CompileResultPtr getCachedResult(const Core::String& key) const;
    void clearCache();
    
    // 错误处理
    void addError(const Core::String& message, const Core::Position& pos);
    void addWarning(const Core::String& message, const Core::Position& pos);
    void collectCompilerErrors(const CompileResultPtr& result);
    
    // 统计更新
    void updateStats(const CompileResultPtr& result, 
                    std::chrono::milliseconds duration);
    
    // 调试输出
    void debugLog(const Core::String& message) const;
};

// 默认结果合并器
class DefaultResultMerger {
public:
    static CompileResultPtr merge(const std::vector<CompileResultPtr>& results,
                                 Core::Context& context);

private:
    static Core::String mergeHTML(const std::vector<CompileResultPtr>& results);
    static Core::String mergeCSS(const std::vector<CompileResultPtr>& results);
    static Core::String mergeJS(const std::vector<CompileResultPtr>& results);
    static void collectMetadata(const std::vector<CompileResultPtr>& results,
                               CompileResultPtr& merged);
};

// 编译器工厂
class CompilerFactory {
public:
    static std::unique_ptr<CompilerDispatcher> createDispatcher();
    static CompilerPtr createCHTLCompiler();
    static CompilerPtr createCHTLJSCompiler();
    static CompilerPtr createCSSCompiler();
    static CompilerPtr createJavaScriptCompiler();
};

} // namespace Compiler
} // namespace CHTL
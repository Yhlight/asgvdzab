#pragma once

#include "core/chtl_unified_scanner.hpp"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace chtl {

// 前向声明编译器接口
class ICHTLCompiler;
class ICHTLJSCompiler;
class ICSSCompiler;
class IJavaScriptCompiler;

/**
 * 编译结果
 */
struct CompilationResult {
    bool success;                               // 编译是否成功
    std::string output;                         // 编译输出
    std::vector<std::string> errors;            // 错误信息
    std::vector<std::string> warnings;          // 警告信息
    std::unordered_map<std::string, std::string> metadata;  // 元数据
    
    CompilationResult() : success(false) {}
};

/**
 * 片段编译结果
 */
struct FragmentCompilationResult {
    CodeFragment fragment;                      // 原始片段
    CompilationResult result;                   // 编译结果
    std::string compilerId;                     // 使用的编译器ID
    
    FragmentCompilationResult() = default;
    FragmentCompilationResult(const CodeFragment& frag) : fragment(frag) {}
};

/**
 * 编译器接口基类
 */
class ICompiler {
public:
    virtual ~ICompiler() = default;
    virtual std::string getId() const = 0;
    virtual std::string getVersion() const = 0;
    virtual bool initialize() = 0;
    virtual void cleanup() = 0;
    virtual CompilationResult compile(const CodeFragment& fragment) = 0;
};

/**
 * CHTL编译器接口
 * 负责编译CHTL片段（局部style使用）
 */
class ICHTLCompiler : public ICompiler {
public:
    virtual ~ICHTLCompiler() = default;
    virtual bool supportsLocalStyle() const = 0;
    virtual CompilationResult compileLocalStyle(const CodeFragment& fragment) = 0;
};

/**
 * CHTL JS编译器接口
 * 负责编译CHTL JS混合片段
 */
class ICHTLJSCompiler : public ICompiler {
public:
    virtual ~ICHTLJSCompiler() = default;
    virtual bool supportsVariableGroup() const = 0;
    virtual bool supportsFromClause() const = 0;
    virtual CompilationResult compileChtlJsMix(const CodeFragment& fragment) = 0;
};

/**
 * CSS编译器接口
 * 负责编译CSS片段（全局style使用，基于ANTLR）
 */
class ICSSCompiler : public ICompiler {
public:
    virtual ~ICSSCompiler() = default;
    virtual bool supportsANTLR() const = 0;
    virtual CompilationResult compileGlobalStyle(const CodeFragment& fragment) = 0;
};

/**
 * JavaScript编译器接口
 * 负责编译标准JavaScript片段（基于ANTLR）
 */
class IJavaScriptCompiler : public ICompiler {
public:
    virtual ~IJavaScriptCompiler() = default;
    virtual bool supportsANTLR() const = 0;
    virtual bool supportsES6() const = 0;
    virtual CompilationResult compileScript(const CodeFragment& fragment) = 0;
};

/**
 * 编译器工厂
 */
class CompilerFactory {
public:
    static std::shared_ptr<ICHTLCompiler> createCHTLCompiler();
    static std::shared_ptr<ICHTLJSCompiler> createCHTLJSCompiler();
    static std::shared_ptr<ICSSCompiler> createCSSCompiler();
    static std::shared_ptr<IJavaScriptCompiler> createJavaScriptCompiler();
    
    static void setDebugMode(bool enabled) { debugMode_ = enabled; }
    
private:
    static bool debugMode_;
};

/**
 * CompilerDispatcher - 编译器调度器
 * 
 * 核心机制：
 * 1. 接收CHTLUnifiedScanner切割的精确片段
 * 2. 根据片段类型调度到对应的编译器
 * 3. 管理编译器的生命周期和状态
 * 4. 收集和合并编译结果
 * 5. 生成最终的HTML输出
 * 
 * 编译器对应关系：
 * - 局部style -> CHTL编译器
 * - 全局style -> CSS编译器 (ANTLR)
 * - script -> 由CHTL, CHTL JS, JS编译器共同管理
 */
class CompilerDispatcher {
public:
    CompilerDispatcher();
    ~CompilerDispatcher();
    
    /**
     * 初始化调度器和所有编译器
     */
    bool initialize();
    
    /**
     * 清理资源
     */
    void cleanup();
    
    /**
     * 编译CHTL源代码
     * @param sourceCode 完整的CHTL源代码
     * @return 编译结果
     */
    CompilationResult compileSource(const std::string& sourceCode);
    
    /**
     * 编译代码片段列表
     * @param fragments 精确切割的代码片段列表
     * @return 片段编译结果列表
     */
    std::vector<FragmentCompilationResult> compileFragments(const std::vector<CodeFragment>& fragments);
    
    /**
     * 合并编译结果为最终HTML
     * @param fragmentResults 片段编译结果列表
     * @return 最终编译结果
     */
    CompilationResult mergeResults(const std::vector<FragmentCompilationResult>& fragmentResults);
    
    /**
     * 设置调试模式
     */
    void setDebugMode(bool enabled) { debugMode_ = enabled; }
    
    /**
     * 获取编译器状态
     */
    std::unordered_map<std::string, std::string> getCompilerStatus() const;

private:
    // 编译器实例
    std::shared_ptr<ICHTLCompiler> chtlCompiler_;
    std::shared_ptr<ICHTLJSCompiler> chtlJsCompiler_;
    std::shared_ptr<ICSSCompiler> cssCompiler_;
    std::shared_ptr<IJavaScriptCompiler> jsCompiler_;
    
    // 统一扫描器
    std::unique_ptr<CHTLUnifiedScanner> scanner_;
    
    // 状态
    bool initialized_;
    bool debugMode_;
    
    /**
     * 根据片段类型选择编译器
     */
    std::shared_ptr<ICompiler> selectCompiler(const CodeFragment& fragment);
    
    /**
     * 分析script片段的具体类型和处理方式
     */
    CodeFragmentType analyzeScriptFragment(const CodeFragment& fragment);
    
    /**
     * 分析style片段的具体类型和处理方式
     */
    CodeFragmentType analyzeStyleFragment(const CodeFragment& fragment);
    
    /**
     * 编译单个片段
     */
    FragmentCompilationResult compileSingleFragment(const CodeFragment& fragment);
    
    /**
     * 调试输出
     */
    void debugLog(const std::string& message);
    
    /**
     * 输出片段信息
     */
    void debugFragmentInfo(const CodeFragment& fragment);
    
    /**
     * 验证编译器兼容性
     */
    bool validateCompilerCompatibility();
    
    /**
     * 初始化各个编译器
     */
    bool initializeCompilers();
    
    /**
     * 生成编译报告
     */
    std::string generateCompilationReport(const std::vector<FragmentCompilationResult>& results);
    
    /**
     * 处理编译错误
     */
    void handleCompilationError(const std::string& compilerId, const CompilationResult& result);
    
    /**
     * 优化输出HTML
     */
    std::string optimizeHtmlOutput(const std::string& html);
};

/**
 * 编译器注册器
 * 支持插件式编译器扩展
 */
class CompilerRegistry {
public:
    template<typename T>
    static void registerCompiler(const std::string& id) {
        static_assert(std::is_base_of_v<ICompiler, T>, "T must inherit from ICompiler");
        // 注册逻辑
    }
    
    static std::shared_ptr<ICompiler> createCompiler(const std::string& id);
    static std::vector<std::string> getAvailableCompilers();
};

} // namespace chtl
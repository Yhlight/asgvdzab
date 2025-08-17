#pragma once

#include "FragmentTypes.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

namespace Chtl {

/**
 * 编译结果类型
 */
enum class CompileResultType {
    HTML,           // HTML输出
    CSS,            // CSS输出
    JAVASCRIPT,     // JavaScript输出
    ERROR           // 错误信息
};

/**
 * 编译结果类
 */
class CompileResult {
public:
    CompileResult(CompileResultType type, const std::string& content = "")
        : Type_(type), Content_(content), Success_(type != CompileResultType::ERROR) {}
    
    CompileResultType GetType() const { return Type_; }
    const std::string& GetContent() const { return Content_; }
    bool IsSuccess() const { return Success_; }
    
    void SetContent(const std::string& content) { Content_ = content; }
    void SetSuccess(bool success) { Success_ = success; }
    
    // 错误信息相关
    void AddError(const std::string& message, const SourceLocation& location = SourceLocation{});
    void AddWarning(const std::string& message, const SourceLocation& location = SourceLocation{});
    
    const std::vector<std::string>& GetErrors() const { return Errors_; }
    const std::vector<std::string>& GetWarnings() const { return Warnings_; }
    
    bool HasErrors() const { return !Errors_.empty(); }
    bool HasWarnings() const { return !Warnings_.empty(); }

private:
    CompileResultType Type_;
    std::string Content_;
    bool Success_;
    std::vector<std::string> Errors_;
    std::vector<std::string> Warnings_;
};

/**
 * 编译器基础接口
 */
class ICompiler {
public:
    virtual ~ICompiler() = default;
    
    /**
     * 编译单个代码片段
     */
    virtual std::unique_ptr<CompileResult> Compile(const CodeFragment& fragment) = 0;
    
    /**
     * 批量编译代码片段
     */
    virtual std::vector<std::unique_ptr<CompileResult>> Compile(
        const std::vector<std::unique_ptr<CodeFragment>>& fragments) = 0;
    
    /**
     * 获取编译器名称
     */
    virtual std::string GetName() const = 0;
    
    /**
     * 获取编译器版本
     */
    virtual std::string GetVersion() const = 0;
    
    /**
     * 获取支持的片段类型
     */
    virtual std::vector<FragmentType> GetSupportedTypes() const = 0;
    
    /**
     * 检查是否支持指定类型
     */
    virtual bool SupportsType(FragmentType type) const = 0;
    
    /**
     * 初始化编译器
     */
    virtual bool Initialize() = 0;
    
    /**
     * 清理编译器资源
     */
    virtual void Cleanup() = 0;
};

/**
 * 手写编译器基类
 */
class HandwrittenCompiler : public ICompiler {
public:
    HandwrittenCompiler(const std::string& name, const std::string& version)
        : Name_(name), Version_(version) {}
    
    std::string GetName() const override { return Name_; }
    std::string GetVersion() const override { return Version_; }
    
    bool Initialize() override { return true; }
    void Cleanup() override {}

protected:
    std::string Name_;
    std::string Version_;
};

/**
 * ANTLR编译器基类
 */
class AntlrCompiler : public ICompiler {
public:
    AntlrCompiler(const std::string& name, const std::string& version)
        : Name_(name), Version_(version) {}
    
    std::string GetName() const override { return Name_; }
    std::string GetVersion() const override { return Version_; }
    
    bool Initialize() override;
    void Cleanup() override;

protected:
    std::string Name_;
    std::string Version_;
    bool Initialized_ = false;
    
    /**
     * 初始化ANTLR相关资源
     */
    virtual bool InitializeAntlr() = 0;
    
    /**
     * 清理ANTLR相关资源
     */
    virtual void CleanupAntlr() = 0;
};

/**
 * 编译器工厂接口
 */
class ICompilerFactory {
public:
    virtual ~ICompilerFactory() = default;
    
    /**
     * 创建编译器实例
     */
    virtual std::unique_ptr<ICompiler> CreateCompiler(FragmentType type) = 0;
    
    /**
     * 获取支持的类型
     */
    virtual std::vector<FragmentType> GetSupportedTypes() const = 0;
    
    /**
     * 注册编译器创建函数
     */
    virtual void RegisterCompiler(FragmentType type, 
        std::function<std::unique_ptr<ICompiler>()> creator) = 0;
};

/**
 * 编译器注册表
 */
class CompilerRegistry {
public:
    static CompilerRegistry& GetInstance();
    
    void RegisterCompiler(FragmentType type, 
        std::function<std::unique_ptr<ICompiler>()> creator);
    
    std::unique_ptr<ICompiler> CreateCompiler(FragmentType type);
    
    std::vector<FragmentType> GetSupportedTypes() const;
    
    bool IsSupported(FragmentType type) const;

private:
    CompilerRegistry() = default;
    std::unordered_map<FragmentType, std::function<std::unique_ptr<ICompiler>()>> Creators_;
};

/**
 * 编译器配置类
 */
class CompilerConfig {
public:
    // 通用配置
    bool DebugMode = false;
    bool Optimize = true;
    std::string OutputFormat = "html5";
    
    // CHTL特有配置
    int IndexInitialCount = 0;
    bool DisableNameGroup = false;
    bool DisableCustomOriginType = false;
    
    // 样式相关配置
    bool MinifyCss = false;
    bool Autoprefixer = true;
    
    // JavaScript相关配置
    bool MinifyJs = false;
    bool SourceMap = false;
    std::string JsTarget = "es2017";
    
    // 获取配置的字符串表示
    std::string ToString() const;
    
    // 从字符串加载配置
    bool FromString(const std::string& configStr);
};

} // namespace Chtl
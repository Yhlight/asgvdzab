#pragma once

#include "fragment_types.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace chtl {

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
        : type_(type), content_(content), success_(type != CompileResultType::ERROR) {}
    
    CompileResultType getType() const { return type_; }
    const std::string& getContent() const { return content_; }
    bool isSuccess() const { return success_; }
    
    void setContent(const std::string& content) { content_ = content; }
    void setSuccess(bool success) { success_ = success; }
    
    // 错误信息相关
    void addError(const std::string& message, const SourceLocation& location = SourceLocation{});
    void addWarning(const std::string& message, const SourceLocation& location = SourceLocation{});
    
    const std::vector<std::string>& getErrors() const { return errors_; }
    const std::vector<std::string>& getWarnings() const { return warnings_; }
    
    bool hasErrors() const { return !errors_.empty(); }
    bool hasWarnings() const { return !warnings_.empty(); }

private:
    CompileResultType type_;
    std::string content_;
    bool success_;
    std::vector<std::string> errors_;
    std::vector<std::string> warnings_;
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
    virtual std::unique_ptr<CompileResult> compile(const CodeFragment& fragment) = 0;
    
    /**
     * 批量编译代码片段
     */
    virtual std::vector<std::unique_ptr<CompileResult>> compile(
        const std::vector<std::unique_ptr<CodeFragment>>& fragments) = 0;
    
    /**
     * 获取编译器名称
     */
    virtual std::string getName() const = 0;
    
    /**
     * 获取编译器版本
     */
    virtual std::string getVersion() const = 0;
    
    /**
     * 获取支持的片段类型
     */
    virtual std::vector<FragmentType> getSupportedTypes() const = 0;
    
    /**
     * 检查是否支持指定类型
     */
    virtual bool supportsType(FragmentType type) const = 0;
    
    /**
     * 初始化编译器
     */
    virtual bool initialize() = 0;
    
    /**
     * 清理编译器资源
     */
    virtual void cleanup() = 0;
};

/**
 * 手写编译器基类
 */
class HandwrittenCompiler : public ICompiler {
public:
    HandwrittenCompiler(const std::string& name, const std::string& version)
        : name_(name), version_(version) {}
    
    std::string getName() const override { return name_; }
    std::string getVersion() const override { return version_; }
    
    bool initialize() override { return true; }
    void cleanup() override {}

protected:
    std::string name_;
    std::string version_;
};

/**
 * ANTLR编译器基类
 */
class ANTLRCompiler : public ICompiler {
public:
    ANTLRCompiler(const std::string& name, const std::string& version)
        : name_(name), version_(version) {}
    
    std::string getName() const override { return name_; }
    std::string getVersion() const override { return version_; }
    
    bool initialize() override;
    void cleanup() override;

protected:
    std::string name_;
    std::string version_;
    bool initialized_ = false;
    
    /**
     * 初始化ANTLR相关资源
     */
    virtual bool initializeANTLR() = 0;
    
    /**
     * 清理ANTLR相关资源
     */
    virtual void cleanupANTLR() = 0;
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
    virtual std::unique_ptr<ICompiler> createCompiler(FragmentType type) = 0;
    
    /**
     * 获取支持的类型
     */
    virtual std::vector<FragmentType> getSupportedTypes() const = 0;
    
    /**
     * 注册编译器创建函数
     */
    virtual void registerCompiler(FragmentType type, 
        std::function<std::unique_ptr<ICompiler>()> creator) = 0;
};

/**
 * 编译器注册表
 */
class CompilerRegistry {
public:
    static CompilerRegistry& getInstance();
    
    void registerCompiler(FragmentType type, 
        std::function<std::unique_ptr<ICompiler>()> creator);
    
    std::unique_ptr<ICompiler> createCompiler(FragmentType type);
    
    std::vector<FragmentType> getSupportedTypes() const;
    
    bool isSupported(FragmentType type) const;

private:
    CompilerRegistry() = default;
    std::unordered_map<FragmentType, std::function<std::unique_ptr<ICompiler>()>> creators_;
};

/**
 * 编译器配置类
 */
class CompilerConfig {
public:
    // 通用配置
    bool debug_mode = false;
    bool optimize = true;
    std::string output_format = "html5";
    
    // CHTL特有配置
    int index_initial_count = 0;
    bool disable_name_group = false;
    bool disable_custom_origin_type = false;
    
    // 样式相关配置
    bool minify_css = false;
    bool autoprefixer = true;
    
    // JavaScript相关配置
    bool minify_js = false;
    bool source_map = false;
    std::string js_target = "es2017";
    
    // 获取配置的字符串表示
    std::string toString() const;
    
    // 从字符串加载配置
    bool fromString(const std::string& config_str);
};

} // namespace chtl
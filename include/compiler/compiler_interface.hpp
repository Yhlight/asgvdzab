#ifndef CHTL_COMPILER_INTERFACE_HPP
#define CHTL_COMPILER_INTERFACE_HPP

#include "core/types.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

namespace CHTL {

// 基础编译器接口
class ICompiler {
public:
    virtual ~ICompiler() = default;
    
    // 编译接口
    virtual CompileResult compile(const std::vector<CodeFragment>& fragments) = 0;
    
    // 配置管理
    virtual void setConfig(const CompilerConfig& config) = 0;
    virtual const CompilerConfig& getConfig() const = 0;
    
    // 错误处理
    virtual const std::vector<CompileError>& getErrors() const = 0;
    virtual const std::vector<CompileError>& getWarnings() const = 0;
    virtual void clearErrors() = 0;
    
    // 编译器信息
    virtual std::string getName() const = 0;
    virtual std::string getVersion() const = 0;
    virtual std::vector<std::string> getSupportedExtensions() const = 0;
};

// CHTL编译器接口（手写）
class ICHTLCompiler : public ICompiler {
public:
    virtual ~ICHTLCompiler() = default;
    
    // CHTL特定功能
    virtual CompileResult compileElement(const CodeFragment& fragment) = 0;
    virtual CompileResult compileTemplate(const CodeFragment& fragment) = 0;
    virtual CompileResult compileCustom(const CodeFragment& fragment) = 0;
    virtual CompileResult compileLocalStyle(const CodeFragment& fragment) = 0;
    virtual CompileResult compileImport(const CodeFragment& fragment) = 0;
    virtual CompileResult compileNamespace(const CodeFragment& fragment) = 0;
    virtual CompileResult compileConfiguration(const CodeFragment& fragment) = 0;
    virtual CompileResult compileOrigin(const CodeFragment& fragment) = 0;
    
    // 模板和自定义管理
    virtual void registerTemplate(const std::string& name, const std::string& type, const std::string& content) = 0;
    virtual void registerCustom(const std::string& name, const std::string& type, const std::string& content) = 0;
    virtual bool hasTemplate(const std::string& name, const std::string& type) const = 0;
    virtual bool hasCustom(const std::string& name, const std::string& type) const = 0;
    
    // 命名空间管理
    virtual void pushNamespace(const std::string& name) = 0;
    virtual void popNamespace() = 0;
    virtual std::string getCurrentNamespace() const = 0;
    
    // 约束管理
    virtual void addConstraint(const std::string& constraint, const SourceLocation& location) = 0;
    virtual bool isConstraintViolated(const std::string& element, const SourceLocation& location) const = 0;
    
    // 上下文信息
    virtual void setGlobalContext(const std::unordered_map<std::string, std::string>& context) = 0;
    virtual std::unordered_map<std::string, std::string> getGeneratedClasses() const = 0;
    virtual std::unordered_map<std::string, std::string> getGeneratedIds() const = 0;
};

// CHTL JS编译器接口（手写）
class ICHTLJSCompiler : public ICompiler {
public:
    virtual ~ICHTLJSCompiler() = default;
    
    // CHTL JS特定功能
    virtual CompileResult compileLocalScript(const CodeFragment& fragment) = 0;
    virtual CompileResult compileSelector(const CodeFragment& fragment) = 0;
    virtual CompileResult compileListen(const CodeFragment& fragment) = 0;
    virtual CompileResult compileDelegate(const CodeFragment& fragment) = 0;
    virtual CompileResult compileAnimate(const CodeFragment& fragment) = 0;
    virtual CompileResult compileTemplateVar(const CodeFragment& fragment) = 0;
    virtual CompileResult compileCustomVar(const CodeFragment& fragment) = 0;
    
    // 选择器管理
    virtual void registerSelector(const std::string& selector, const std::string& jsCode) = 0;
    virtual std::string resolveSelector(const std::string& selector) const = 0;
    
    // 事件委托管理
    virtual void registerEventDelegate(const std::string& parent, const std::string& target, const std::string& event) = 0;
    virtual std::string generateDelegateCode() const = 0;
    
    // 动画管理
    virtual void registerAnimation(const std::string& name, const std::string& config) = 0;
    virtual std::string generateAnimationCode() const = 0;
    
    // 变量管理
    virtual void setTemplateVariables(const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& vars) = 0;
    virtual void setCustomVariables(const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& vars) = 0;
    
    // 上下文信息
    virtual void setAvailableSelectors(const std::unordered_map<std::string, std::string>& selectors) = 0;
    virtual void setGlobalContext(const std::unordered_map<std::string, std::string>& context) = 0;
};

// CSS编译器接口（ANTLR）
class ICSSCompiler : public ICompiler {
public:
    virtual ~ICSSCompiler() = default;
    
    // CSS特定功能
    virtual CompileResult compileStyleSheet(const CodeFragment& fragment) = 0;
    virtual CompileResult compileRule(const CodeFragment& fragment) = 0;
    virtual CompileResult compileSelector(const CodeFragment& fragment) = 0;
    virtual CompileResult compileDeclaration(const CodeFragment& fragment) = 0;
    
    // CSS优化
    virtual CompileResult optimizeCSS(const std::string& css) = 0;
    virtual CompileResult minifyCSS(const std::string& css) = 0;
    
    // CSS验证
    virtual std::vector<CompileError> validateCSS(const std::string& css) = 0;
    
    // 预处理器支持
    virtual bool supportsSass() const = 0;
    virtual bool supportsLess() const = 0;
    virtual CompileResult compileSass(const std::string& sass) = 0;
    virtual CompileResult compileLess(const std::string& less) = 0;
};

// JavaScript编译器接口（ANTLR）
class IJavaScriptCompiler : public ICompiler {
public:
    virtual ~IJavaScriptCompiler() = default;
    
    // JavaScript特定功能
    virtual CompileResult compileScript(const CodeFragment& fragment) = 0;
    virtual CompileResult compileFunction(const CodeFragment& fragment) = 0;
    virtual CompileResult compileExpression(const CodeFragment& fragment) = 0;
    virtual CompileResult compileStatement(const CodeFragment& fragment) = 0;
    
    // JavaScript优化
    virtual CompileResult optimizeJS(const std::string& js) = 0;
    virtual CompileResult minifyJS(const std::string& js) = 0;
    
    // JavaScript验证
    virtual std::vector<CompileError> validateJS(const std::string& js) = 0;
    
    // 模块系统支持
    virtual bool supportsES6Modules() const = 0;
    virtual bool supportsCommonJS() const = 0;
    virtual CompileResult transpileES6(const std::string& es6) = 0;
    
    // TypeScript支持
    virtual bool supportsTypeScript() const = 0;
    virtual CompileResult compileTypeScript(const std::string& ts) = 0;
};

// 编译器能力查询
enum class CompilerCapability {
    SYNTAX_HIGHLIGHTING,
    ERROR_RECOVERY,
    INCREMENTAL_COMPILATION,
    SOURCE_MAPS,
    MINIFICATION,
    OPTIMIZATION,
    VALIDATION,
    PREPROCESSING,
    TRANSPILATION,
    MODULE_RESOLUTION
};

// 编译器元数据
struct CompilerMetadata {
    std::string name;
    std::string version;
    std::string description;
    std::vector<std::string> authors;
    std::string license;
    std::vector<CompilerCapability> capabilities;
    std::unordered_map<std::string, std::string> properties;
};

// 编译器注册表
class CompilerRegistry {
public:
    static CompilerRegistry& getInstance();
    
    // 注册编译器
    void registerCHTLCompiler(const std::string& name, std::function<std::unique_ptr<ICHTLCompiler>()> factory);
    void registerCHTLJSCompiler(const std::string& name, std::function<std::unique_ptr<ICHTLJSCompiler>()> factory);
    void registerCSSCompiler(const std::string& name, std::function<std::unique_ptr<ICSSCompiler>()> factory);
    void registerJavaScriptCompiler(const std::string& name, std::function<std::unique_ptr<IJavaScriptCompiler>()> factory);
    
    // 创建编译器实例
    std::unique_ptr<ICHTLCompiler> createCHTLCompiler(const std::string& name = "default");
    std::unique_ptr<ICHTLJSCompiler> createCHTLJSCompiler(const std::string& name = "default");
    std::unique_ptr<ICSSCompiler> createCSSCompiler(const std::string& name = "default");
    std::unique_ptr<IJavaScriptCompiler> createJavaScriptCompiler(const std::string& name = "default");
    
    // 查询可用编译器
    std::vector<std::string> getAvailableCHTLCompilers() const;
    std::vector<std::string> getAvailableCHTLJSCompilers() const;
    std::vector<std::string> getAvailableCSSCompilers() const;
    std::vector<std::string> getAvailableJavaScriptCompilers() const;
    
    // 编译器元数据
    CompilerMetadata getCHTLCompilerMetadata(const std::string& name) const;
    CompilerMetadata getCHTLJSCompilerMetadata(const std::string& name) const;
    CompilerMetadata getCSSCompilerMetadata(const std::string& name) const;
    CompilerMetadata getJavaScriptCompilerMetadata(const std::string& name) const;
    
private:
    CompilerRegistry() = default;
    
    std::unordered_map<std::string, std::function<std::unique_ptr<ICHTLCompiler>()>> chtlCompilers_;
    std::unordered_map<std::string, std::function<std::unique_ptr<ICHTLJSCompiler>()>> chtlJSCompilers_;
    std::unordered_map<std::string, std::function<std::unique_ptr<ICSSCompiler>()>> cssCompilers_;
    std::unordered_map<std::string, std::function<std::unique_ptr<IJavaScriptCompiler>()>> jsCompilers_;
    
    std::unordered_map<std::string, CompilerMetadata> chtlMetadata_;
    std::unordered_map<std::string, CompilerMetadata> chtlJSMetadata_;
    std::unordered_map<std::string, CompilerMetadata> cssMetadata_;
    std::unordered_map<std::string, CompilerMetadata> jsMetadata_;
};

} // namespace CHTL

#endif // CHTL_COMPILER_INTERFACE_HPP
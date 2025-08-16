#pragma once

#include "common/types.hpp"
#include "dispatcher/compiler_dispatcher.hpp"
#include "network/dependency_manager.hpp"
#include "compilers/css_compiler.hpp"  // 复用NodeJSProcessManager
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace chtl {

/**
 * JavaScript处理类型
 */
enum class JSProcessType {
    PARSE_ONLY,        // 仅解析
    MINIFY,           // 压缩
    TRANSPILE_ES5,    // 转译到ES5
    TRANSPILE_ES6,    // 转译到ES6
    TYPESCRIPT_COMPILE, // TypeScript编译
    BABEL_TRANSFORM,   // Babel转换
    WEBPACK_BUNDLE     // Webpack打包
};

/**
 * JavaScript编译选项
 */
struct JSCompileOptions {
    bool minify = false;                    // 是否压缩
    bool sourceMaps = false;                // 是否生成source maps
    std::string targetVersion = "es2020";   // 目标版本
    std::vector<std::string> presets;       // Babel预设
    std::vector<std::string> plugins;       // Babel插件
    std::unordered_map<std::string, std::string> babelOptions; // Babel选项
    bool preserveComments = false;          // 是否保留注释
    bool mangle = true;                     // 是否混淆变量名
    std::string moduleSystem = "commonjs";  // 模块系统 (commonjs, es6, amd, umd)
};

/**
 * JavaScript编译结果
 */
struct JSCompileResult {
    bool success;
    std::string javascript;       // 编译后的JavaScript
    std::string sourceMap;        // Source map（如果启用）
    std::string errorMessage;     // 错误信息
    std::vector<std::string> warnings; // 警告信息
    size_t originalSize;          // 原始大小
    size_t compiledSize;          // 编译后大小
    std::string targetVersion;    // 实际目标版本
};

/**
 * JavaScript语法检查结果
 */
struct JSLintResult {
    bool isValid;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    std::vector<std::string> suggestions;
};

/**
 * JavaScript编译器实现
 * 支持多种JavaScript转译器和压缩器
 */
class JSCompiler : public ICompiler {
public:
    /**
     * 构造函数
     * @param dependencyManager 依赖管理器
     */
    explicit JSCompiler(std::shared_ptr<DependencyManager> dependencyManager);
    
    /**
     * 析构函数
     */
    ~JSCompiler() override;

    /**
     * 编译JavaScript代码
     * @param source JavaScript源代码
     * @param options 编译选项
     * @return 编译结果
     */
    JSCompileResult compile(const std::string& source, const JSCompileOptions& options = {});

    /**
     * 编译JavaScript文件
     * @param inputFile 输入文件路径
     * @param outputFile 输出文件路径
     * @param options 编译选项
     * @return 编译结果
     */
    JSCompileResult compileFile(const std::string& inputFile, 
                               const std::string& outputFile,
                               const JSCompileOptions& options = {});

    /**
     * TypeScript编译
     * @param source TypeScript源代码
     * @param options 编译选项
     * @return 编译结果
     */
    JSCompileResult compileTypeScript(const std::string& source, const JSCompileOptions& options = {});

    /**
     * 检测JavaScript语法类型
     * @param source JavaScript源代码
     * @return 语法类型 ("js", "ts", "jsx", "tsx")
     */
    std::string detectSyntaxType(const std::string& source);

    /**
     * 验证JavaScript语法
     * @param source JavaScript源代码
     * @return 验证结果
     */
    JSLintResult validateSyntax(const std::string& source);

    /**
     * 获取支持的处理类型
     */
    std::vector<JSProcessType> getSupportedProcessTypes() const;

    /**
     * 初始化编译器（下载必要的依赖）
     * @param callback 进度回调
     * @return 是否成功
     */
    std::future<bool> initialize(ProgressCallback callback = nullptr);

    /**
     * 打包多个JavaScript文件
     * @param entryFiles 入口文件列表
     * @param outputFile 输出文件
     * @param options 编译选项
     * @return 编译结果
     */
    JSCompileResult bundle(const std::vector<std::string>& entryFiles,
                          const std::string& outputFile,
                          const JSCompileOptions& options = {});

    // ICompiler接口实现
    std::string getName() const override { return "JSCompiler"; }
    std::vector<CodeSegmentType> getSupportedTypes() const override;
    CompileResult compile(const CodeSegment& segment, const CompileConfig& config) override;

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl_;

    // 私有方法
    JSCompileResult compileWithBabel(const std::string& source, const JSCompileOptions& options);
    JSCompileResult compileWithTypeScript(const std::string& source, const JSCompileOptions& options);
    JSCompileResult minifyWithTerser(const std::string& source, const JSCompileOptions& options);
    JSCompileResult bundleWithWebpack(const std::vector<std::string>& entryFiles,
                                     const std::string& outputFile,
                                     const JSCompileOptions& options);
    std::string generateCompilerScript(JSProcessType type, const JSCompileOptions& options);
    bool ensureDependenciesAvailable();
};

/**
 * JavaScript工具类
 * 提供JavaScript相关的实用功能
 */
class JSUtils {
public:
    /**
     * 压缩JavaScript
     * @param js JavaScript代码
     * @param preserveComments 是否保留注释
     * @return 压缩后的JavaScript
     */
    static std::string minifyJS(const std::string& js, bool preserveComments = false);

    /**
     * 美化JavaScript
     * @param js JavaScript代码
     * @param indent 缩进字符
     * @return 美化后的JavaScript
     */
    static std::string beautifyJS(const std::string& js, const std::string& indent = "  ");

    /**
     * 提取JavaScript中的函数
     * @param js JavaScript代码
     * @return 函数名列表
     */
    static std::vector<std::string> extractFunctions(const std::string& js);

    /**
     * 提取JavaScript中的变量
     * @param js JavaScript代码
     * @return 变量名列表
     */
    static std::vector<std::string> extractVariables(const std::string& js);

    /**
     * 检查JavaScript语法
     * @param js JavaScript代码
     * @return 是否有效
     */
    static bool isValidSyntax(const std::string& js);

    /**
     * 检测ES版本
     * @param js JavaScript代码
     * @return ES版本 ("es5", "es6", "es2017", etc.)
     */
    static std::string detectESVersion(const std::string& js);

    /**
     * 提取imports/requires
     * @param js JavaScript代码
     * @return 依赖模块列表
     */
    static std::vector<std::string> extractDependencies(const std::string& js);

    /**
     * 转换模块系统
     * @param js JavaScript代码
     * @param fromSystem 源模块系统
     * @param toSystem 目标模块系统
     * @return 转换后的代码
     */
    static std::string convertModuleSystem(const std::string& js, 
                                          const std::string& fromSystem,
                                          const std::string& toSystem);

    /**
     * 添加严格模式
     * @param js JavaScript代码
     * @return 添加严格模式后的代码
     */
    static std::string addStrictMode(const std::string& js);

    /**
     * 移除调试语句
     * @param js JavaScript代码
     * @return 移除调试语句后的代码
     */
    static std::string removeDebugStatements(const std::string& js);
};

/**
 * Webpack配置生成器
 */
class WebpackConfigGenerator {
public:
    /**
     * 生成基础Webpack配置
     * @param entryFiles 入口文件
     * @param outputFile 输出文件
     * @param options 编译选项
     * @return Webpack配置JSON
     */
    static std::string generateBasicConfig(const std::vector<std::string>& entryFiles,
                                          const std::string& outputFile,
                                          const JSCompileOptions& options);

    /**
     * 生成Webpack配置文件
     * @param configPath 配置文件路径
     * @param entryFiles 入口文件
     * @param outputFile 输出文件
     * @param options 编译选项
     * @return 是否成功
     */
    static bool generateConfigFile(const std::string& configPath,
                                  const std::vector<std::string>& entryFiles,
                                  const std::string& outputFile,
                                  const JSCompileOptions& options);
};

/**
 * Babel配置生成器
 */
class BabelConfigGenerator {
public:
    /**
     * 生成Babel配置
     * @param options 编译选项
     * @return Babel配置JSON
     */
    static std::string generateConfig(const JSCompileOptions& options);

    /**
     * 生成.babelrc文件
     * @param configPath 配置文件路径
     * @param options 编译选项
     * @return 是否成功
     */
    static bool generateConfigFile(const std::string& configPath, const JSCompileOptions& options);

    /**
     * 获取推荐的预设
     * @param targetVersion 目标版本
     * @return 预设列表
     */
    static std::vector<std::string> getRecommendedPresets(const std::string& targetVersion);
};

} // namespace chtl
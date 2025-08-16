#pragma once

#include "common/types.hpp"
#include "dispatcher/compiler_dispatcher.hpp"
#include "network/dependency_manager.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace chtl {

/**
 * CSS处理类型
 */
enum class CSSProcessType {
    PARSE_ONLY,        // 仅解析
    MINIFY,           // 压缩
    AUTOPREFIXER,     // 自动添加前缀
    SASS_COMPILE,     // Sass编译
    LESS_COMPILE,     // Less编译
    POSTCSS_PROCESS   // PostCSS处理
};

/**
 * CSS编译选项
 */
struct CSSCompileOptions {
    bool minify = false;                    // 是否压缩
    bool autoprefixer = true;               // 是否自动添加前缀
    bool sourceMaps = false;                // 是否生成source maps
    std::vector<std::string> browserslist;  // 浏览器兼容列表
    std::unordered_map<std::string, std::string> variables; // CSS变量
    std::string outputFormat = "css";       // 输出格式
};

/**
 * CSS编译结果
 */
struct CSSCompileResult {
    bool success;
    std::string css;              // 编译后的CSS
    std::string sourceMap;        // Source map（如果启用）
    std::string errorMessage;     // 错误信息
    std::vector<std::string> warnings; // 警告信息
    size_t originalSize;          // 原始大小
    size_t compiledSize;          // 编译后大小
};

/**
 * Node.js进程管理器
 * 用于执行CSS编译器的Node.js脚本
 */
class NodeJSProcessManager {
public:
    explicit NodeJSProcessManager(const std::string& nodePath);
    ~NodeJSProcessManager();

    /**
     * 执行Node.js脚本
     * @param scriptPath 脚本路径
     * @param args 参数
     * @param input 标准输入内容
     * @return 执行结果 {exitCode, stdout, stderr}
     */
    struct ProcessResult {
        int exitCode;
        std::string stdout;
        std::string stderr;
    };
    
    ProcessResult executeScript(const std::string& scriptPath, 
                               const std::vector<std::string>& args = {},
                               const std::string& input = "");

    /**
     * 检查Node.js是否可用
     */
    bool isNodeJSAvailable() const;

private:
    std::string nodePath_;
    bool available_;
};

/**
 * CSS编译器实现
 * 支持多种CSS预处理器和后处理器
 */
class CSSCompiler : public ICompiler {
public:
    /**
     * 构造函数
     * @param dependencyManager 依赖管理器
     */
    explicit CSSCompiler(std::shared_ptr<DependencyManager> dependencyManager);
    
    /**
     * 析构函数
     */
    ~CSSCompiler() override;

    /**
     * 编译CSS代码
     * @param source CSS源代码
     * @param options 编译选项
     * @return 编译结果
     */
    CSSCompileResult compile(const std::string& source, const CSSCompileOptions& options = {});

    /**
     * 编译CSS文件
     * @param inputFile 输入文件路径
     * @param outputFile 输出文件路径
     * @param options 编译选项
     * @return 编译结果
     */
    CSSCompileResult compileFile(const std::string& inputFile, 
                                const std::string& outputFile,
                                const CSSCompileOptions& options = {});

    /**
     * 检测CSS语法类型
     * @param source CSS源代码
     * @return 语法类型 ("css", "scss", "sass", "less")
     */
    std::string detectSyntaxType(const std::string& source);

    /**
     * 验证CSS语法
     * @param source CSS源代码
     * @return 是否有效
     */
    bool validateSyntax(const std::string& source);

    /**
     * 获取支持的处理类型
     */
    std::vector<CSSProcessType> getSupportedProcessTypes() const;

    /**
     * 初始化编译器（下载必要的依赖）
     * @param callback 进度回调
     * @return 是否成功
     */
    std::future<bool> initialize(ProgressCallback callback = nullptr);

    // ICompiler接口实现
    std::string getName() const override { return "CSSCompiler"; }
    std::vector<CodeSegmentType> getSupportedTypes() const override;
    CompileResult compile(const CodeSegment& segment, const CompileConfig& config) override;

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl_;

    // 私有方法
    CSSCompileResult compileSass(const std::string& source, const CSSCompileOptions& options);
    CSSCompileResult compileLess(const std::string& source, const CSSCompileOptions& options);
    CSSCompileResult processWithPostCSS(const std::string& source, const CSSCompileOptions& options);
    std::string generateCompilerScript(CSSProcessType type, const CSSCompileOptions& options);
    bool ensureDependenciesAvailable();
};

/**
 * CSS工具类
 * 提供CSS相关的实用功能
 */
class CSSUtils {
public:
    /**
     * 压缩CSS
     * @param css CSS代码
     * @return 压缩后的CSS
     */
    static std::string minifyCSS(const std::string& css);

    /**
     * 美化CSS
     * @param css CSS代码
     * @param indent 缩进字符
     * @return 美化后的CSS
     */
    static std::string beautifyCSS(const std::string& css, const std::string& indent = "  ");

    /**
     * 提取CSS中的选择器
     * @param css CSS代码
     * @return 选择器列表
     */
    static std::vector<std::string> extractSelectors(const std::string& css);

    /**
     * 提取CSS中的属性
     * @param css CSS代码
     * @return 属性映射 {selector: {property: value}}
     */
    static std::unordered_map<std::string, std::unordered_map<std::string, std::string>> 
        extractProperties(const std::string& css);

    /**
     * 验证CSS选择器
     * @param selector 选择器
     * @return 是否有效
     */
    static bool isValidSelector(const std::string& selector);

    /**
     * 验证CSS属性
     * @param property 属性名
     * @param value 属性值
     * @return 是否有效
     */
    static bool isValidProperty(const std::string& property, const std::string& value);

    /**
     * 获取浏览器前缀
     * @param property CSS属性
     * @return 需要的前缀列表
     */
    static std::vector<std::string> getBrowserPrefixes(const std::string& property);
};

} // namespace chtl
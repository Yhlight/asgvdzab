#pragma once

#include "dispatcher/compiler_dispatcher.hpp"
#include <unordered_set>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace chtl {

/**
 * JavaScript编译器
 * 负责解析和编译JavaScript代码片段，支持标准JS和CHTL JS增强语法
 */
class JavaScriptCompiler : public ICompiler {
public:
    /**
     * 构造函数
     */
    JavaScriptCompiler();

    /**
     * 析构函数
     */
    ~JavaScriptCompiler() = default;

    /**
     * 编译JavaScript代码片段
     * @param segment JavaScript代码片段
     * @param config 编译配置
     * @return 编译结果
     */
    CompileResult compile(const CodeSegment& segment, const CompileConfig& config) override;

    /**
     * 获取编译器名称
     * @return 编译器名称
     */
    std::string getName() const override;

    /**
     * 获取编译器版本
     * @return 编译器版本
     */
    std::string getVersion() const override;

    /**
     * 检查是否支持指定的代码片段类型
     * @param type 代码片段类型
     * @return 是否支持
     */
    bool supportsType(CodeSegmentType type) const override;

    /**
     * 获取支持的代码片段类型列表
     * @return 支持的类型列表
     */
    std::vector<CodeSegmentType> getSupportedTypes() const override;

private:
    /**
     * 解析JavaScript代码
     * @param jsCode JavaScript代码
     * @return 解析后的AST节点
     */
    std::shared_ptr<void> parseJavaScript(const std::string& jsCode);

    /**
     * 验证JavaScript语法
     * @param jsCode JavaScript代码
     * @return 验证结果
     */
    bool validateJavaScriptSyntax(const std::string& jsCode);

    /**
     * 优化JavaScript代码
     * @param jsCode JavaScript代码
     * @param config 编译配置
     * @return 优化后的JavaScript代码
     */
    std::string optimizeJavaScript(const std::string& jsCode, const CompileConfig& config);

    /**
     * 处理ES6+特性
     * @param jsCode JavaScript代码
     * @return 处理后的JavaScript代码
     */
    std::string processES6Features(const std::string& jsCode);

    /**
     * 处理模块导入导出
     * @param jsCode JavaScript代码
     * @return 处理后的JavaScript代码
     */
    std::string processModules(const std::string& jsCode);

    /**
     * 处理异步代码
     * @param jsCode JavaScript代码
     * @return 处理后的JavaScript代码
     */
    std::string processAsyncCode(const std::string& jsCode);

    /**
     * 生成JavaScript包装器
     * @param jsCode JavaScript代码
     * @param config 编译配置
     * @return 包装后的JavaScript代码
     */
    std::string generateJavaScriptWrapper(const std::string& jsCode, const CompileConfig& config);

    /**
     * 压缩JavaScript代码
     * @param jsCode JavaScript代码
     * @return 压缩后的JavaScript代码
     */
    std::string minifyJavaScript(const std::string& jsCode);

    /**
     * 格式化JavaScript代码
     * @param jsCode JavaScript代码
     * @return 格式化后的JavaScript代码
     */
    std::string formatJavaScript(const std::string& jsCode);

    /**
     * 添加JavaScript polyfill
     * @param jsCode JavaScript代码
     * @return 添加polyfill后的JavaScript代码
     */
    std::string addJavaScriptPolyfills(const std::string& jsCode);

    /**
     * 检查JavaScript兼容性
     * @param jsCode JavaScript代码
     * @return 兼容性检查结果
     */
    std::vector<std::string> checkJavaScriptCompatibility(const std::string& jsCode);

    /**
     * 生成JavaScript统计信息
     * @param jsCode JavaScript代码
     * @return 统计信息
     */
    std::unordered_map<std::string, size_t> generateJavaScriptStatistics(const std::string& jsCode);

    /**
     * 处理CHTL JS增强语法
     * @param jsCode JavaScript代码
     * @return 处理后的JavaScript代码
     */
    std::string processCHTLJSExtensions(const std::string& jsCode);

    /**
     * 处理增强选择器语法 {{}}
     * @param jsCode JavaScript代码
     * @return 处理后的JavaScript代码
     */
    std::string processEnhancedSelectors(const std::string& jsCode);

    /**
     * 处理箭头操作符 ->
     * @param jsCode JavaScript代码
     * @return 处理后的JavaScript代码
     */
    std::string processArrowOperators(const std::string& jsCode);

    /**
     * 处理增强监听器语法 listen()
     * @param jsCode JavaScript代码
     * @return 处理后的JavaScript代码
     */
    std::string processEnhancedListeners(const std::string& jsCode);

    /**
     * 处理事件委托语法 delegate()
     * @param jsCode JavaScript代码
     * @return 处理后的JavaScript代码
     */
    std::string processEventDelegation(const std::string& jsCode);

    /**
     * 处理动画语法 animate()
     * @param jsCode JavaScript代码
     * @return 处理后的JavaScript代码
     */
    std::string processAnimationSyntax(const std::string& jsCode);

private:
    /**
     * 支持的JavaScript特性
     */
    std::unordered_set<std::string> supportedJSFeatures_;

    /**
     * JavaScript解析器状态
     */
    bool parserInitialized_;

    /**
     * 调试模式
     */
    bool debugMode_;

    /**
     * JavaScript代码缓存
     */
    std::unordered_map<std::string, std::string> jsCodeCache_;

    /**
     * 错误收集器
     */
    std::vector<std::string> errors_;

    /**
     * 警告收集器
     */
    std::vector<std::string> warnings_;

    /**
     * CHTL JS扩展支持
     */
    bool enableCHTLJSExtensions_;
};

} // namespace chtl
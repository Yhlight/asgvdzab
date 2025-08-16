#pragma once

#include "dispatcher/compiler_dispatcher.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

namespace chtl {

/**
 * CSS编译器
 * 负责解析和编译CSS代码片段，支持全局CSS和局部样式
 */
class CSSCompiler : public ICompiler {
public:
    /**
     * 构造函数
     */
    CSSCompiler();

    /**
     * 析构函数
     */
    ~CSSCompiler() = default;

    /**
     * 编译CSS代码片段
     * @param segment CSS代码片段
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
     * 解析CSS规则
     * @param cssCode CSS代码
     * @return 解析后的CSS规则列表
     */
    std::vector<std::string> parseCSSRules(const std::string& cssCode);

    /**
     * 验证CSS语法
     * @param cssCode CSS代码
     * @return 验证结果
     */
    bool validateCSSSyntax(const std::string& cssCode);

    /**
     * 优化CSS代码
     * @param cssCode CSS代码
     * @param config 编译配置
     * @return 优化后的CSS代码
     */
    std::string optimizeCSS(const std::string& cssCode, const CompileConfig& config);

    /**
     * 处理CSS变量
     * @param cssCode CSS代码
     * @return 处理后的CSS代码
     */
    std::string processCSSVariables(const std::string& cssCode);

    /**
     * 处理CSS嵌套
     * @param cssCode CSS代码
     * @return 处理后的CSS代码
     */
    std::string processCSSNesting(const std::string& cssCode);

    /**
     * 生成CSS选择器
     * @param elementName 元素名称
     * @param attributes 属性
     * @return 生成的CSS选择器
     */
    std::string generateCSSSelector(const std::string& elementName, 
                                   const std::unordered_map<std::string, std::string>& attributes);

    /**
     * 压缩CSS代码
     * @param cssCode CSS代码
     * @return 压缩后的CSS代码
     */
    std::string minifyCSS(const std::string& cssCode);

    /**
     * 格式化CSS代码
     * @param cssCode CSS代码
     * @return 格式化后的CSS代码
     */
    std::string formatCSS(const std::string& cssCode);

    /**
     * 添加CSS前缀
     * @param cssCode CSS代码
     * @return 添加前缀后的CSS代码
     */
    std::string addCSSPrefixes(const std::string& cssCode);

    /**
     * 检查CSS兼容性
     * @param cssCode CSS代码
     * @return 兼容性检查结果
     */
    std::vector<std::string> checkCSSCompatibility(const std::string& cssCode);

    /**
     * 生成CSS统计信息
     * @param cssCode CSS代码
     * @return 统计信息
     */
    std::unordered_map<std::string, size_t> generateCSSStatistics(const std::string& cssCode);

private:
    /**
     * 支持的CSS特性
     */
    std::unordered_set<std::string> supportedCSSFeatures_;

    /**
     * CSS解析器状态
     */
    bool parserInitialized_;

    /**
     * 调试模式
     */
    bool debugMode_;

    /**
     * CSS规则缓存
     */
    std::unordered_map<std::string, std::string> cssRuleCache_;

    /**
     * 错误收集器
     */
    std::vector<std::string> errors_;

    /**
     * 警告收集器
     */
    std::vector<std::string> warnings_;
};

} // namespace chtl
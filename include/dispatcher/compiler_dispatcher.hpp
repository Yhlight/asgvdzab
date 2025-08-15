#pragma once

#include "common/types.hpp"
#include <memory>
#include <unordered_map>
#include <functional>

namespace chtl {

// 前向声明
class CHTLCompiler;
class CHTLJSCompiler;
class CSSCompiler;
class JavaScriptCompiler;

/**
 * 编译结果
 */
struct CompileResult {
    std::string output;                         // 编译输出
    std::vector<std::string> errors;            // 错误信息
    std::vector<std::string> warnings;          // 警告信息
    bool success;                               // 编译是否成功
    std::unordered_map<std::string, std::string> metadata; // 元数据

    CompileResult() : success(true) {}
};

/**
 * 编译器接口
 */
class ICompiler {
public:
    virtual ~ICompiler() = default;
    
    /**
     * 编译代码片段
     * @param segment 代码片段
     * @param config 编译配置
     * @return 编译结果
     */
    virtual CompileResult compile(const CodeSegment& segment, const CompileConfig& config) = 0;
    
    /**
     * 获取编译器名称
     * @return 编译器名称
     */
    virtual std::string getName() const = 0;
    
    /**
     * 获取支持的代码片段类型
     * @return 支持的类型列表
     */
    virtual std::vector<CodeSegmentType> getSupportedTypes() const = 0;
};

/**
 * 编译器调度器
 * 负责管理和调度不同类型的编译器
 */
class CompilerDispatcher {
public:
    /**
     * 构造函数
     */
    CompilerDispatcher();
    
    /**
     * 析构函数
     */
    ~CompilerDispatcher() = default;

    /**
     * 编译代码片段列表
     * @param segments 代码片段列表
     * @param config 编译配置
     * @return 编译结果
     */
    CompileResult compile(const std::vector<CodeSegment>& segments, const CompileConfig& config = CompileConfig{});

    /**
     * 编译单个代码片段
     * @param segment 代码片段
     * @param config 编译配置
     * @return 编译结果
     */
    CompileResult compileSegment(const CodeSegment& segment, const CompileConfig& config = CompileConfig{});

    /**
     * 注册编译器
     * @param compiler 编译器实例
     */
    void registerCompiler(std::unique_ptr<ICompiler> compiler);

    /**
     * 获取指定类型的编译器
     * @param type 代码片段类型
     * @return 编译器指针，如果未找到返回nullptr
     */
    ICompiler* getCompiler(CodeSegmentType type);

    /**
     * 设置全局编译配置
     * @param config 编译配置
     */
    void setGlobalConfig(const CompileConfig& config);

    /**
     * 获取全局编译配置
     * @return 编译配置
     */
    const CompileConfig& getGlobalConfig() const;

    /**
     * 启用调试模式
     * @param enabled 是否启用
     */
    void setDebugMode(bool enabled);

    /**
     * 获取编译统计信息
     * @return 统计信息
     */
    std::unordered_map<std::string, size_t> getCompileStatistics() const;

    /**
     * 重置统计信息
     */
    void resetStatistics();

private:
    /**
     * 编译器映射
     */
    std::unordered_map<CodeSegmentType, std::unique_ptr<ICompiler>> compilers_;
    
    /**
     * 全局编译配置
     */
    CompileConfig globalConfig_;
    
    /**
     * 调试模式
     */
    bool debugMode_;
    
    /**
     * 编译统计信息
     */
    std::unordered_map<std::string, size_t> statistics_;

    /**
     * 初始化默认编译器
     */
    void initializeDefaultCompilers();

    /**
     * 预处理代码片段
     * @param segments 代码片段列表
     * @return 预处理后的代码片段列表
     */
    std::vector<CodeSegment> preprocessSegments(const std::vector<CodeSegment>& segments);

    /**
     * 后处理编译结果
     * @param results 编译结果列表
     * @return 合并后的编译结果
     */
    CompileResult postprocessResults(const std::vector<CompileResult>& results);

    /**
     * 分析代码片段依赖关系
     * @param segments 代码片段列表
     * @return 排序后的代码片段列表
     */
    std::vector<CodeSegment> analyzeDependencies(const std::vector<CodeSegment>& segments);

    /**
     * 更新统计信息
     * @param type 代码片段类型
     * @param success 是否成功
     */
    void updateStatistics(CodeSegmentType type, bool success);

    /**
     * 记录调试信息
     * @param message 调试消息
     */
    void logDebug(const std::string& message);
};

/**
 * 编译器工厂
 */
class CompilerFactory {
public:
    /**
     * 创建CHTL编译器
     * @return CHTL编译器实例
     */
    static std::unique_ptr<ICompiler> createCHTLCompiler();

    /**
     * 创建CHTL JS编译器
     * @return CHTL JS编译器实例
     */
    static std::unique_ptr<ICompiler> createCHTLJSCompiler();

    /**
     * 创建CSS编译器
     * @return CSS编译器实例
     */
    static std::unique_ptr<ICompiler> createCSSCompiler();

    /**
     * 创建JavaScript编译器
     * @return JavaScript编译器实例
     */
    static std::unique_ptr<ICompiler> createJavaScriptCompiler();
};

} // namespace chtl
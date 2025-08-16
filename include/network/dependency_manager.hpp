#pragma once

#include "common/types.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <future>

namespace chtl {

/**
 * 依赖类型
 */
enum class DependencyType {
    CSS_COMPILER,           // CSS编译器
    JS_COMPILER,           // JavaScript编译器
    CSS_PREPROCESSOR,      // CSS预处理器（如Sass）
    JS_TRANSPILER,         // JS转译器（如TypeScript）
    LIBRARY,               // 通用库
    PLUGIN                 // 插件
};

/**
 * 依赖信息
 */
struct DependencyInfo {
    std::string name;           // 依赖名称
    std::string version;        // 版本号
    DependencyType type;        // 依赖类型
    std::string downloadUrl;    // 下载地址
    std::string localPath;      // 本地存储路径
    std::string checksum;       // 校验和
    bool isRequired;            // 是否必需
    std::vector<std::string> dependencies; // 子依赖
};

/**
 * 下载进度回调
 */
using ProgressCallback = std::function<void(const std::string& name, double progress, const std::string& status)>;

/**
 * 下载结果
 */
struct DownloadResult {
    bool success;
    std::string errorMessage;
    std::string localPath;
    size_t downloadedBytes;
};

/**
 * 网络依赖管理器
 * 负责下载、缓存和管理外部编译器和依赖
 */
class DependencyManager {
public:
    /**
     * 构造函数
     * @param cacheDir 缓存目录
     */
    explicit DependencyManager(const std::string& cacheDir = "./cache");
    
    /**
     * 析构函数
     */
    ~DependencyManager();

    /**
     * 添加依赖定义
     * @param dependency 依赖信息
     */
    void addDependency(const DependencyInfo& dependency);

    /**
     * 下载依赖
     * @param name 依赖名称
     * @param callback 进度回调
     * @return 下载结果的future
     */
    std::future<DownloadResult> downloadDependency(const std::string& name, 
                                                   ProgressCallback callback = nullptr);

    /**
     * 批量下载依赖
     * @param names 依赖名称列表
     * @param callback 进度回调
     * @return 是否全部成功
     */
    std::future<bool> downloadDependencies(const std::vector<std::string>& names,
                                          ProgressCallback callback = nullptr);

    /**
     * 检查依赖是否已下载
     * @param name 依赖名称
     * @return 是否存在
     */
    bool isDependencyAvailable(const std::string& name) const;

    /**
     * 获取依赖的本地路径
     * @param name 依赖名称
     * @return 本地路径，如果不存在返回空字符串
     */
    std::string getDependencyPath(const std::string& name) const;

    /**
     * 验证依赖文件完整性
     * @param name 依赖名称
     * @return 是否完整
     */
    bool verifyDependency(const std::string& name) const;

    /**
     * 清理缓存
     * @param olderThanDays 清理多少天前的缓存，-1表示全部清理
     */
    void cleanCache(int olderThanDays = 7);

    /**
     * 获取所有已注册的依赖
     * @return 依赖列表
     */
    std::vector<DependencyInfo> getAllDependencies() const;

    /**
     * 设置网络超时时间
     * @param seconds 超时秒数
     */
    void setNetworkTimeout(int seconds);

    /**
     * 设置代理服务器
     * @param proxy 代理地址（格式: http://host:port）
     */
    void setProxy(const std::string& proxy);

    /**
     * 加载预定义的编译器依赖
     */
    void loadDefaultCompilerDependencies();

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl_;

    // 私有方法
    DownloadResult downloadFile(const std::string& url, const std::string& localPath,
                               ProgressCallback callback = nullptr);
    bool createDirectories(const std::string& path);
    std::string calculateChecksum(const std::string& filePath);
    bool validateChecksum(const std::string& filePath, const std::string& expectedChecksum) const;
};

/**
 * 预定义的编译器依赖配置
 */
class CompilerDependencies {
public:
    /**
     * 获取CSS编译器依赖配置
     */
    static std::vector<DependencyInfo> getCSSCompilerDependencies();

    /**
     * 获取JS编译器依赖配置
     */
    static std::vector<DependencyInfo> getJSCompilerDependencies();

    /**
     * 获取Node.js依赖配置
     */
    static DependencyInfo getNodeJSDependency();

    /**
     * 获取Sass编译器依赖配置
     */
    static DependencyInfo getSassCompilerDependency();

    /**
     * 获取TypeScript编译器依赖配置
     */
    static DependencyInfo getTypeScriptCompilerDependency();
};

} // namespace chtl
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <any>

namespace chtl {

// 前向声明
class CJMODContext;
class CJMODRuntime;

/**
 * CJMOD函数参数类型
 */
using CJMODValue = std::any;
using CJMODArguments = std::vector<CJMODValue>;
using CJMODResult = CJMODValue;

/**
 * CJMOD函数签名
 */
using CJMODFunction = std::function<CJMODResult(const CJMODArguments&, CJMODContext*)>;

/**
 * CJMOD模块信息
 */
struct CJMODModuleInfo {
    std::string name;               // 模块名称
    std::string version;            // 版本号
    std::string author;             // 作者
    std::string description;        // 描述
    std::string apiVersion;         // API版本
    std::vector<std::string> dependencies; // 依赖的其他CJMOD模块
    std::unordered_map<std::string, std::string> metadata; // 元数据
    
    CJMODModuleInfo() : apiVersion("1.0.0") {}
};

/**
 * CJMOD函数信息
 */
struct CJMODFunctionInfo {
    std::string name;               // 函数名
    std::string description;        // 函数描述
    std::vector<std::string> paramTypes;    // 参数类型列表
    std::string returnType;         // 返回值类型
    bool isAsync;                   // 是否为异步函数
    std::unordered_map<std::string, std::string> metadata; // 函数元数据
    
    CJMODFunctionInfo() : isAsync(false) {}
};

/**
 * CJMOD运行时上下文
 * 提供模块与CHTL运行时交互的接口
 */
class CJMODContext {
public:
    virtual ~CJMODContext() = default;
    
    /**
     * 获取CHTL变量值
     * @param name 变量名
     * @return 变量值
     */
    virtual CJMODValue getCHTLVariable(const std::string& name) = 0;
    
    /**
     * 设置CHTL变量值
     * @param name 变量名
     * @param value 变量值
     */
    virtual void setCHTLVariable(const std::string& name, const CJMODValue& value) = 0;
    
    /**
     * 调用CHTL函数
     * @param name 函数名
     * @param args 参数列表
     * @return 函数返回值
     */
    virtual CJMODResult callCHTLFunction(const std::string& name, const CJMODArguments& args) = 0;
    
    /**
     * 获取DOM元素
     * @param selector CSS选择器
     * @return 元素列表
     */
    virtual std::vector<CJMODValue> getElementsBySelector(const std::string& selector) = 0;
    
    /**
     * 创建DOM元素
     * @param tagName 标签名
     * @param attributes 属性映射
     * @return 创建的元素
     */
    virtual CJMODValue createElement(const std::string& tagName, 
                                    const std::unordered_map<std::string, std::string>& attributes = {}) = 0;
    
    /**
     * 执行JavaScript代码
     * @param code JavaScript代码
     * @return 执行结果
     */
    virtual CJMODResult executeJavaScript(const std::string& code) = 0;
    
    /**
     * 记录日志
     * @param level 日志级别
     * @param message 日志消息
     */
    virtual void log(const std::string& level, const std::string& message) = 0;
    
    /**
     * 获取当前模块信息
     * @return 模块信息
     */
    virtual const CJMODModuleInfo& getCurrentModuleInfo() const = 0;
};

/**
 * CJMOD模块基类
 * 所有CJMOD模块都必须继承此类
 */
class ICJMODModule {
public:
    virtual ~ICJMODModule() = default;
    
    /**
     * 获取模块信息
     * @return 模块信息
     */
    virtual CJMODModuleInfo getModuleInfo() const = 0;
    
    /**
     * 模块初始化
     * @param context 运行时上下文
     * @return 是否初始化成功
     */
    virtual bool initialize(CJMODContext* context) = 0;
    
    /**
     * 模块清理
     */
    virtual void cleanup() = 0;
    
    /**
     * 获取导出的函数列表
     * @return 函数名列表
     */
    virtual std::vector<std::string> getExportedFunctions() const = 0;
    
    /**
     * 获取函数信息
     * @param functionName 函数名
     * @return 函数信息
     */
    virtual CJMODFunctionInfo getFunctionInfo(const std::string& functionName) const = 0;
    
    /**
     * 调用模块函数
     * @param functionName 函数名
     * @param args 参数列表
     * @param context 运行时上下文
     * @return 函数返回值
     */
    virtual CJMODResult callFunction(const std::string& functionName, 
                                    const CJMODArguments& args, 
                                    CJMODContext* context) = 0;
    
    /**
     * 注册事件监听器
     * @param eventType 事件类型
     * @param callback 回调函数
     */
    virtual void addEventListener(const std::string& eventType, 
                                 const CJMODFunction& callback) = 0;
    
    /**
     * 移除事件监听器
     * @param eventType 事件类型
     */
    virtual void removeEventListener(const std::string& eventType) = 0;
};

/**
 * CJMOD模块创建函数类型
 */
using CreateCJMODModuleFunc = ICJMODModule*(*)();
using DestroyCJMODModuleFunc = void(*)(ICJMODModule*);

/**
 * CJMOD动态库加载器
 */
class CJMODLoader {
public:
    CJMODLoader();
    ~CJMODLoader();
    
    /**
     * 加载CJMOD模块
     * @param modulePath 模块文件路径(.so/.dll/.dylib)
     * @return 是否加载成功
     */
    bool loadModule(const std::string& modulePath);
    
    /**
     * 卸载CJMOD模块
     * @param moduleName 模块名称
     * @return 是否卸载成功
     */
    bool unloadModule(const std::string& moduleName);
    
    /**
     * 获取已加载的模块
     * @param moduleName 模块名称
     * @return 模块指针，未找到返回nullptr
     */
    ICJMODModule* getModule(const std::string& moduleName);
    
    /**
     * 获取所有已加载模块的名称
     * @return 模块名称列表
     */
    std::vector<std::string> getLoadedModules() const;
    
    /**
     * 检查模块是否已加载
     * @param moduleName 模块名称
     * @return 是否已加载
     */
    bool isModuleLoaded(const std::string& moduleName) const;
    
    /**
     * 获取加载错误信息
     * @return 错误信息
     */
    const std::string& getLastError() const { return lastError_; }

private:
    struct LoadedModule {
        void* libraryHandle;            // 动态库句柄
        ICJMODModule* moduleInstance;   // 模块实例
        CreateCJMODModuleFunc createFunc;
        DestroyCJMODModuleFunc destroyFunc;
    };
    
    std::unordered_map<std::string, LoadedModule> loadedModules_;
    std::string lastError_;
    
    /**
     * 获取动态库中的符号地址
     * @param handle 库句柄
     * @param symbolName 符号名称
     * @return 符号地址
     */
    void* getSymbol(void* handle, const std::string& symbolName);
    
    /**
     * 关闭动态库
     * @param handle 库句柄
     */
    void closeLibrary(void* handle);
};

/**
 * CJMOD运行时环境
 */
class CJMODRuntime : public CJMODContext {
public:
    CJMODRuntime();
    ~CJMODRuntime();
    
    /**
     * 初始化运行时
     * @return 是否成功
     */
    bool initialize();
    
    /**
     * 清理运行时
     */
    void cleanup();
    
    /**
     * 加载CJMOD模块
     * @param modulePath 模块路径
     * @return 是否成功
     */
    bool loadModule(const std::string& modulePath);
    
    /**
     * 卸载CJMOD模块
     * @param moduleName 模块名称
     * @return 是否成功
     */
    bool unloadModule(const std::string& moduleName);
    
    /**
     * 调用CJMOD函数
     * @param moduleName 模块名称
     * @param functionName 函数名称
     * @param args 参数列表
     * @return 函数返回值
     */
    CJMODResult callModuleFunction(const std::string& moduleName, 
                                  const std::string& functionName, 
                                  const CJMODArguments& args);
    
    /**
     * 获取模块信息
     * @param moduleName 模块名称
     * @return 模块信息
     */
    CJMODModuleInfo getModuleInfo(const std::string& moduleName) const;
    
    /**
     * 列出模块的所有函数
     * @param moduleName 模块名称
     * @return 函数信息列表
     */
    std::vector<CJMODFunctionInfo> getModuleFunctions(const std::string& moduleName) const;
    
    // CJMODContext接口实现
    CJMODValue getCHTLVariable(const std::string& name) override;
    void setCHTLVariable(const std::string& name, const CJMODValue& value) override;
    CJMODResult callCHTLFunction(const std::string& name, const CJMODArguments& args) override;
    std::vector<CJMODValue> getElementsBySelector(const std::string& selector) override;
    CJMODValue createElement(const std::string& tagName, 
                            const std::unordered_map<std::string, std::string>& attributes = {}) override;
    CJMODResult executeJavaScript(const std::string& code) override;
    void log(const std::string& level, const std::string& message) override;
    const CJMODModuleInfo& getCurrentModuleInfo() const override;

private:
    std::unique_ptr<CJMODLoader> loader_;
    std::unordered_map<std::string, CJMODValue> chtlVariables_;
    std::unordered_map<std::string, CJMODFunction> chtlFunctions_;
    CJMODModuleInfo currentModuleInfo_;
    bool initialized_;
};

/**
 * CJMOD模块工厂
 * 提供便捷的模块创建和注册功能
 */
class CJMODModuleFactory {
public:
    /**
     * 创建简单的CJMOD模块
     * @param info 模块信息
     * @param functions 函数映射
     * @return 模块实例
     */
    static std::unique_ptr<ICJMODModule> createSimpleModule(
        const CJMODModuleInfo& info,
        const std::unordered_map<std::string, CJMODFunction>& functions);
    
    /**
     * 注册CJMOD类型转换器
     * @param typeName 类型名称
     * @param converter 转换函数
     */
    static void registerTypeConverter(const std::string& typeName, 
                                     std::function<CJMODValue(const std::string&)> converter);
    
    /**
     * 转换字符串到CJMOD值
     * @param typeName 类型名称
     * @param valueStr 值字符串
     * @return CJMOD值
     */
    static CJMODValue convertStringToValue(const std::string& typeName, const std::string& valueStr);
};

/**
 * CJMOD工具函数
 */
namespace CJMODUtils {
    /**
     * 提取any中的值
     * @param value CJMODValue
     * @return 转换后的值
     */
    template<typename T>
    T extractValue(const CJMODValue& value) {
        try {
            return std::any_cast<T>(value);
        } catch (const std::bad_any_cast&) {
            return T{};
        }
    }
    
    /**
     * 检查值是否为指定类型
     * @param value CJMODValue
     * @return 是否为指定类型
     */
    template<typename T>
    bool isType(const CJMODValue& value) {
        return value.type() == typeid(T);
    }
    
    /**
     * 创建CJMODValue
     * @param value 值
     * @return CJMODValue
     */
    template<typename T>
    CJMODValue createValue(const T& value) {
        return std::make_any<T>(value);
    }
    
    /**
     * 转换参数列表
     * @param args 原始参数
     * @return 转换后的参数
     */
    std::vector<std::string> argumentsToStrings(const CJMODArguments& args);
    
    /**
     * 从字符串创建参数列表
     * @param strings 字符串列表
     * @return 参数列表
     */
    CJMODArguments stringsToArguments(const std::vector<std::string>& strings);
}

} // namespace chtl

/**
 * CJMOD模块导出宏
 * 每个CJMOD模块都应该使用这个宏来导出标准接口
 */
#define CJMOD_EXPORT_MODULE(ModuleClass) \
    extern "C" { \
        chtl::ICJMODModule* createCJMODModule() { \
            return new ModuleClass(); \
        } \
        void destroyCJMODModule(chtl::ICJMODModule* module) { \
            delete module; \
        } \
    }

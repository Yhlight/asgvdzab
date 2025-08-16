#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <any>

namespace chtl {

// CJmod函数类型枚举
enum class CJmodFunctionType {
    NATIVE_FUNCTION,        // 原生C++函数
    WRAPPED_FUNCTION,       // 包装的JavaScript函数
    BUILTIN_FUNCTION,       // 内置函数
    CUSTOM_FUNCTION         // 自定义函数
};

// CJmod函数参数类型
enum class CJmodParameterType {
    STRING,                 // 字符串
    NUMBER,                 // 数字
    BOOLEAN,                // 布尔值
    OBJECT,                 // 对象
    ARRAY,                  // 数组
    FUNCTION,               // 函数
    UNDEFINED,              // 未定义
    NULL_VALUE,             // 空值
    ANY                     // 任意类型
};

// CJmod函数参数信息
struct CJmodParameter {
    std::string name;                   // 参数名
    CJmodParameterType type;            // 参数类型
    std::string description;            // 参数描述
    bool is_required;                   // 是否必需
    std::any default_value;             // 默认值
    std::vector<std::any> valid_values; // 有效值列表
    
    CJmodParameter(const std::string& n, CJmodParameterType t, const std::string& desc = "", bool required = true)
        : name(n), type(t), description(desc), is_required(required) {}
};

// CJmod函数返回值信息
struct CJmodReturnValue {
    CJmodParameterType type;            // 返回值类型
    std::string description;            // 返回值描述
    std::any example_value;             // 示例值
    
    CJmodReturnValue(CJmodParameterType t, const std::string& desc = "")
        : type(t), description(desc) {}
};

// CJmod函数信息
struct CJmodFunction {
    std::string name;                   // 函数名
    std::string description;            // 函数描述
    CJmodFunctionType type;             // 函数类型
    std::vector<CJmodParameter> parameters; // 参数列表
    CJmodReturnValue return_value;      // 返回值
    std::string category;               // 函数分类
    std::string version;                // 函数版本
    bool is_deprecated;                 // 是否已弃用
    std::string deprecation_note;       // 弃用说明
    
    CJmodFunction(const std::string& n, const std::string& desc = "", CJmodFunctionType t = CJmodFunctionType::NATIVE_FUNCTION)
        : name(n), description(desc), type(t), is_deprecated(false) {}
};

// CJmod属性信息
struct CJmodProperty {
    std::string name;                   // 属性名
    std::string description;            // 属性描述
    CJmodParameterType type;            // 属性类型
    bool is_readonly;                   // 是否只读
    bool is_static;                     // 是否静态
    std::any default_value;             // 默认值
    std::string category;               // 属性分类
    
    CJmodProperty(const std::string& n, const std::string& desc = "", CJmodParameterType t = CJmodParameterType::ANY)
        : name(n), description(desc), type(t), is_readonly(false), is_static(false) {}
};

// CJmod类信息
struct CJmodClass {
    std::string name;                   // 类名
    std::string description;            // 类描述
    std::string base_class;             // 基类名
    std::vector<std::string> interfaces; // 接口列表
    std::vector<CJmodFunction> methods; // 方法列表
    std::vector<CJmodProperty> properties; // 属性列表
    std::vector<CJmodFunction> static_methods; // 静态方法列表
    std::vector<CJmodProperty> static_properties; // 静态属性列表
    std::string category;               // 类分类
    std::string version;                // 类版本
    
    CJmodClass(const std::string& n, const std::string& desc = "")
        : name(n), description(desc) {}
};

// CJmod模块信息
struct CJmodModule {
    std::string name;                   // 模块名
    std::string description;            // 模块描述
    std::string version;                // 模块版本
    std::string author;                 // 作者
    std::string license;                // 许可证
    std::vector<std::string> dependencies; // 依赖列表
    std::vector<CJmodFunction> functions; // 函数列表
    std::vector<CJmodClass> classes;   // 类列表
    std::vector<CJmodProperty> constants; // 常量列表
    std::string category;               // 模块分类
    std::string documentation_url;      // 文档URL
    std::string repository_url;         // 仓库URL
    
    CJmodModule(const std::string& n, const std::string& desc = "")
        : name(n), description(desc) {}
};

// CJmod函数调用上下文
struct CJmodContext {
    std::string module_name;            // 模块名
    std::string function_name;          // 函数名
    std::unordered_map<std::string, std::any> variables; // 变量表
    std::vector<std::string> call_stack; // 调用栈
    bool is_debug_mode;                 // 是否调试模式
    std::string source_file;            // 源文件
    size_t line_number;                 // 行号
    
    CJmodContext(const std::string& module = "", const std::string& function = "")
        : module_name(module), function_name(function), is_debug_mode(false), line_number(0) {}
};

// CJmod函数指针类型
using CJmodFunctionPtr = std::function<std::any(const std::vector<std::any>&, CJmodContext&)>;

// CJmod接口基类
class ICJmodInterface {
public:
    virtual ~ICJmodInterface() = default;
    
    // 模块信息
    virtual CJmodModule getModuleInfo() const = 0;
    virtual std::string getModuleName() const = 0;
    virtual std::string getModuleVersion() const = 0;
    
    // 函数管理
    virtual bool registerFunction(const CJmodFunction& func_info, CJmodFunctionPtr func_ptr) = 0;
    virtual bool unregisterFunction(const std::string& function_name) = 0;
    virtual CJmodFunctionPtr getFunction(const std::string& function_name) const = 0;
    virtual std::vector<CJmodFunction> getFunctions() const = 0;
    virtual bool hasFunction(const std::string& function_name) const = 0;
    
    // 类管理
    virtual bool registerClass(const CJmodClass& class_info) = 0;
    virtual bool unregisterClass(const std::string& class_name) = 0;
    virtual CJmodClass* getClass(const std::string& class_name) const = 0;
    virtual std::vector<CJmodClass> getClasses() const = 0;
    virtual bool hasClass(const std::string& class_name) const = 0;
    
    // 属性管理
    virtual bool registerProperty(const CJmodProperty& prop_info) = 0;
    virtual bool unregisterProperty(const std::string& property_name) = 0;
    virtual CJmodProperty* getProperty(const std::string& property_name) const = 0;
    virtual std::vector<CJmodProperty> getProperties() const = 0;
    virtual bool hasProperty(const std::string& property_name) const = 0;
    
    // 函数调用
    virtual std::any callFunction(const std::string& function_name, const std::vector<std::any>& arguments, CJmodContext& context) = 0;
    virtual std::any callMethod(const std::string& class_name, const std::string& method_name, const std::vector<std::any>& arguments, CJmodContext& context) = 0;
    
    // 错误处理
    virtual bool hasErrors() const = 0;
    virtual std::vector<std::string> getErrors() const = 0;
    virtual void clearErrors() = 0;
    virtual void addError(const std::string& error_message) = 0;
    
    // 调试和诊断
    virtual std::string getDebugInfo() const = 0;
    virtual void enableDebugMode(bool enable) = 0;
    virtual bool isDebugMode() const = 0;
};

// CJmod接口管理器
class CJmodInterfaceManager {
public:
    CJmodInterfaceManager();
    ~CJmodInterfaceManager();
    
    // 接口管理
    bool registerInterface(std::shared_ptr<ICJmodInterface> interface);
    bool unregisterInterface(const std::string& module_name);
    std::shared_ptr<ICJmodInterface> getInterface(const std::string& module_name);
    std::vector<std::string> getRegisteredModules() const;
    bool hasInterface(const std::string& module_name) const;
    
    // 函数调用
    std::any callFunction(const std::string& module_name, const std::string& function_name, const std::vector<std::any>& arguments);
    std::any callMethod(const std::string& module_name, const std::string& class_name, const std::string& method_name, const std::vector<std::any>& arguments);
    
    // 查询功能
    std::vector<CJmodFunction> getAllFunctions() const;
    std::vector<CJmodClass> getAllClasses() const;
    std::vector<CJmodProperty> getAllProperties() const;
    
    // 官方模块支持
    bool isOfficialModule(const std::string& module_name) const;
    std::vector<std::string> getOfficialModules() const;
    
    // 错误处理
    bool hasErrors() const;
    std::vector<std::string> getErrors() const;
    void clearErrors();
    void addError(const std::string& error_message);
    
    // 调试和诊断
    std::string getDebugInfo() const;
    void enableDebugMode(bool enable);
    bool isDebugMode() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 内部方法
    bool validateInterface(std::shared_ptr<ICJmodInterface> interface);
    void updateOfficialModules();
    bool checkCircularDependencies();
    std::string getLastError() const;
};

} // namespace chtl
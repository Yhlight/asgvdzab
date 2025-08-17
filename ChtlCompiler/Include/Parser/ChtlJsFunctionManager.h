#ifndef CHTL_PARSER_CHTLJSFUNCTIONMANAGER_H
#define CHTL_PARSER_CHTLJSFUNCTIONMANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace Chtl {

// CHTL JS函数管理器
// 负责管理虚对象和生成的全局函数之间的映射关系
class ChtlJsFunctionManager {
public:
    // 函数信息
    struct FunctionInfo {
        std::string originalName;      // 原始函数名（如 click、mouseenter）
        std::string generatedName;    // 生成的全局函数名
        std::string virtualObject;    // 所属虚对象名
    };
    
    // 虚对象信息
    struct VirtualObjectInfo {
        std::string name;             // 虚对象名称
        std::string sourceType;       // 来源类型（listen、iNeverAway等）
        std::unordered_map<std::string, FunctionInfo> functions;  // 函数名->函数信息
    };

    ChtlJsFunctionManager();
    ~ChtlJsFunctionManager();
    
    // 注册虚对象
    void registerVirtualObject(const std::string& name, const std::string& sourceType);
    
    // 添加函数到虚对象
    void addFunction(const std::string& virtualObject, const FunctionInfo& funcInfo);
    
    // 生成唯一的全局函数名
    std::string generateUniqueFunctionName(const std::string& base);
    
    // 查找函数
    const FunctionInfo* findFunction(const std::string& virtualObject, 
                                   const std::string& functionName) const;
    
    // 获取虚对象信息
    const VirtualObjectInfo* getVirtualObject(const std::string& name) const;
    
    // 检查虚对象是否存在
    bool hasVirtualObject(const std::string& name) const;
    
    // 获取所有虚对象
    const std::unordered_map<std::string, VirtualObjectInfo>& getAllVirtualObjects() const {
        return virtualObjects_;
    }
    
    // 清空所有数据
    void clear();

private:
    std::unordered_map<std::string, VirtualObjectInfo> virtualObjects_;
    size_t functionCounter_;  // 用于生成唯一函数名
};

// 全局函数管理器实例（单例）
class ChtlJsFunctionRegistry {
public:
    static ChtlJsFunctionRegistry& getInstance() {
        static ChtlJsFunctionRegistry instance;
        return instance;
    }
    
    ChtlJsFunctionManager& getManager() { return manager_; }
    
private:
    ChtlJsFunctionRegistry() = default;
    ~ChtlJsFunctionRegistry() = default;
    ChtlJsFunctionRegistry(const ChtlJsFunctionRegistry&) = delete;
    ChtlJsFunctionRegistry& operator=(const ChtlJsFunctionRegistry&) = delete;
    
    ChtlJsFunctionManager manager_;
};

} // namespace Chtl

#endif // CHTL_PARSER_CHTLJSFUNCTIONMANAGER_H
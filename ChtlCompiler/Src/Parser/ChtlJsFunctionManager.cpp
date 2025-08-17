#include "Parser/ChtlJsFunctionManager.h"
#include <sstream>

namespace Chtl {

ChtlJsFunctionManager::ChtlJsFunctionManager() 
    : functionCounter_(0) {
}

ChtlJsFunctionManager::~ChtlJsFunctionManager() {
}

void ChtlJsFunctionManager::registerVirtualObject(const std::string& name, 
                                                 const std::string& sourceType) {
    VirtualObjectInfo info;
    info.name = name;
    info.sourceType = sourceType;
    virtualObjects_[name] = info;
}

void ChtlJsFunctionManager::addFunction(const std::string& virtualObject, 
                                       const FunctionInfo& funcInfo) {
    auto it = virtualObjects_.find(virtualObject);
    if (it != virtualObjects_.end()) {
        it->second.functions[funcInfo.originalName] = funcInfo;
    }
}

std::string ChtlJsFunctionManager::generateUniqueFunctionName(const std::string& base) {
    std::stringstream ss;
    ss << "_chtl_js_func_" << base << "_" << functionCounter_++;
    return ss.str();
}

const ChtlJsFunctionManager::FunctionInfo* 
ChtlJsFunctionManager::findFunction(const std::string& virtualObject,
                                   const std::string& functionName) const {
    auto objIt = virtualObjects_.find(virtualObject);
    if (objIt == virtualObjects_.end()) {
        return nullptr;
    }
    
    auto funcIt = objIt->second.functions.find(functionName);
    if (funcIt != objIt->second.functions.end()) {
        return &funcIt->second;
    }
    
    return nullptr;
}

const ChtlJsFunctionManager::VirtualObjectInfo* 
ChtlJsFunctionManager::getVirtualObject(const std::string& name) const {
    auto it = virtualObjects_.find(name);
    if (it != virtualObjects_.end()) {
        return &it->second;
    }
    return nullptr;
}

bool ChtlJsFunctionManager::hasVirtualObject(const std::string& name) const {
    return virtualObjects_.find(name) != virtualObjects_.end();
}

void ChtlJsFunctionManager::clear() {
    virtualObjects_.clear();
    functionCounter_ = 0;
}

} // namespace Chtl
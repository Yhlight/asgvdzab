#include "cjmod/cjmod_interface.hpp"
#include <sstream>

namespace chtl {

namespace CJMODUtils {

std::vector<std::string> argumentsToStrings(const CJMODArguments& args) {
    std::vector<std::string> result;
    
    for (const auto& arg : args) {
        try {
            // 尝试转换为字符串
            if (arg.type() == typeid(std::string)) {
                result.push_back(std::any_cast<std::string>(arg));
            } else if (arg.type() == typeid(int)) {
                result.push_back(std::to_string(std::any_cast<int>(arg)));
            } else if (arg.type() == typeid(double)) {
                result.push_back(std::to_string(std::any_cast<double>(arg)));
            } else if (arg.type() == typeid(bool)) {
                result.push_back(std::any_cast<bool>(arg) ? "true" : "false");
            } else {
                result.push_back("unknown_type");
            }
        } catch (const std::bad_any_cast&) {
            result.push_back("conversion_error");
        }
    }
    
    return result;
}

CJMODArguments stringsToArguments(const std::vector<std::string>& strings) {
    CJMODArguments result;
    
    for (const auto& str : strings) {
        // 简化实现：所有字符串都作为std::string类型处理
        result.push_back(createValue(str));
    }
    
    return result;
}

} // namespace CJMODUtils

} // namespace chtl
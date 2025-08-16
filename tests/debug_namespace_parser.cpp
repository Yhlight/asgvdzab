#include "parsers/chtl_namespace_parser.hpp"
#include <iostream>
#include <string>

using namespace chtl;

int main() {
    std::cout << "🔍 调试命名空间约束解析器" << std::endl;
    
    CHTLNamespaceParser parser;
    
    // 测试精确约束：禁止span和自定义元素Box
    std::string source = "except span, [Custom] @Element Box;";
    
    std::cout << "解析源代码:\n" << source << std::endl;
    
    auto result = parser.parseConstraintDeclaration(source, "test.chtl");
    
    std::cout << "\n解析结果: " << (result.success ? "成功" : "失败") << std::endl;
    
    if (result.success) {
        auto constraintRegistry = parser.getConstraintRegistry();
        
        // 检查约束违反
        ConstraintTarget spanTarget(ConstraintTargetType::HTML_ELEMENT, "span");
        bool spanViolation = constraintRegistry->hasConstraintViolation("global", spanTarget);
        std::cout << "span违反约束: " << (spanViolation ? "是" : "否") << std::endl;
        
        ConstraintTarget boxTarget(ConstraintTargetType::CUSTOM_ELEMENT, "Box");
        bool boxViolation = constraintRegistry->hasConstraintViolation("global", boxTarget);
        std::cout << "Box违反约束: " << (boxViolation ? "是" : "否") << std::endl;
        
        ConstraintTarget divTarget(ConstraintTargetType::HTML_ELEMENT, "div");
        bool divViolation = constraintRegistry->hasConstraintViolation("global", divTarget);
        std::cout << "div违反约束: " << (divViolation ? "是" : "否") << std::endl;
        
        // 获取约束信息
        auto constraints = constraintRegistry->getConstraints("global");
        std::cout << "注册的约束数量: " << constraints.size() << std::endl;
        
        for (size_t i = 0; i < constraints.size(); ++i) {
            const auto& constraint = constraints[i];
            std::cout << "约束 " << i << " 目标数量: " << constraint.targets.size() << std::endl;
            for (size_t j = 0; j < constraint.targets.size(); ++j) {
                const auto& target = constraint.targets[j];
                std::cout << "  目标 " << j << ": type=" << static_cast<int>(target.type) << " name='" << target.name << "'" << std::endl;
            }
        }
    }
    
    return 0;
}
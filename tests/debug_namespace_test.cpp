#include <iostream>
#include <string>

// 模拟引入命名空间管理器
#include "namespace/enhanced_namespace_manager.hpp"

using namespace chtl;

int main() {
    std::cout << "开始调试测试..." << std::endl;
    
    try {
        std::cout << "创建EnhancedNamespaceManager..." << std::endl;
        EnhancedNamespaceManager manager;
        std::cout << "✅ 管理器创建成功" << std::endl;
        
        std::cout << "测试简单命名空间声明..." << std::endl;
        bool result = manager.declareNamespace("Test", Position{1, 1}, "test.chtl");
        std::cout << "✅ 简单命名空间声明: " << (result ? "成功" : "失败") << std::endl;
        
        std::cout << "测试嵌套命名空间声明..." << std::endl;
        result = manager.declareNamespace("A.B", Position{1, 1}, "test.chtl");
        std::cout << "✅ 嵌套命名空间声明: " << (result ? "成功" : "失败") << std::endl;
        
        std::cout << "检查命名空间是否存在..." << std::endl;
        bool exists = manager.namespaceExists("A.B");
        std::cout << "✅ A.B存在: " << (exists ? "是" : "否") << std::endl;
        
        std::cout << "测试三层嵌套命名空间..." << std::endl;
        result = manager.declareNamespace("X.Y.Z", Position{1, 1}, "test.chtl");
        std::cout << "✅ 三层嵌套命名空间: " << (result ? "成功" : "失败") << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ 异常: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "🎉 调试测试完成" << std::endl;
    return 0;
}
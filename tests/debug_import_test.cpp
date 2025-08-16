#include <iostream>
#include <string>

// 模拟引入导入管理器
#include "import/enhanced_import_manager.hpp"

using namespace chtl;

int main() {
    std::cout << "开始调试Import测试..." << std::endl;
    
    try {
        std::cout << "创建EnhancedImportManager..." << std::endl;
        EnhancedImportManager manager;
        std::cout << "✅ 管理器创建成功" << std::endl;
        
        std::cout << "测试PathNormalizer::parsePathInfo..." << std::endl;
        auto pathInfo = PathNormalizer::parsePathInfo("test.chtl");
        std::cout << "✅ 路径解析成功，类型: " << static_cast<int>(pathInfo.pathType) << std::endl;
        
        std::cout << "测试CircularDependencyDetector..." << std::endl;
        CircularDependencyDetector detector;
        bool result1 = detector.addDependency("A.chtl", "B.chtl");
        std::cout << "✅ 添加依赖A->B: " << (result1 ? "成功" : "失败") << std::endl;
        
        bool result2 = detector.addDependency("B.chtl", "C.chtl");
        std::cout << "✅ 添加依赖B->C: " << (result2 ? "成功" : "失败") << std::endl;
        
        std::cout << "测试导入类型解析..." << std::endl;
        auto htmlType = EnhancedImportManager::parseImportType("@Html");
        std::cout << "✅ @Html类型: " << static_cast<int>(htmlType) << std::endl;
        
        auto chtlType = EnhancedImportManager::parseImportType("@Chtl");
        std::cout << "✅ @Chtl类型: " << static_cast<int>(chtlType) << std::endl;
        
        std::cout << "测试扩展名获取..." << std::endl;
        auto extensions = EnhancedImportManager::getImportTypeExtensions(ImportType::CHTL);
        std::cout << "✅ CHTL扩展名数量: " << extensions.size() << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ 异常: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "🎉 Import调试测试完成" << std::endl;
    return 0;
}
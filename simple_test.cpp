#include <iostream>
#include <string>
#include <memory>

// 直接包含必要的头文件
#include "include/Core/Types.h"

int main() {
    std::cout << "测试CHTL基础类型..." << std::endl;
    
    // 测试Position
    CHTL::Core::Position pos(10, 5, 100, "test.chtl");
    std::cout << "Position: " << pos.line << ":" << pos.column 
              << " in " << pos.filename << std::endl;
    
    // 测试FragmentType枚举
    CHTL::Core::FragmentType type = CHTL::Core::FragmentType::CHTL;
    std::cout << "FragmentType: " << static_cast<int>(type) << std::endl;
    
    // 测试TokenType枚举
    CHTL::Core::TokenType tokenType = CHTL::Core::TokenType::IDENTIFIER;
    std::cout << "TokenType: " << static_cast<int>(tokenType) << std::endl;
    
    std::cout << "基础类型测试完成!" << std::endl;
    
    return 0;
}
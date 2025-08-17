#include "constraints/raw_embedding_parser.hpp"
#include <iostream>

int main() {
    std::cout << "调试测试开始..." << std::endl;
    
    chtl::RawEmbeddingParser parser;
    
    // 测试包含字符串的情况
    std::string test1 = R"(const str = "[Origin] @Html { fake }";)";
    std::cout << "测试1: " << test1 << std::endl;
    auto blocks1 = parser.parseRawEmbeddings(test1);
    std::cout << "找到 " << blocks1.size() << " 个块" << std::endl;
    
    // 测试包含注释的情况
    std::string test2 = R"(/* [Origin] @Style { fake } */)";
    std::cout << "测试2: " << test2 << std::endl;
    auto blocks2 = parser.parseRawEmbeddings(test2);
    std::cout << "找到 " << blocks2.size() << " 个块" << std::endl;
    
    // 测试真正的原始嵌入
    std::string test3 = R"([Origin] @Html { <div>Real content</div> })";
    std::cout << "测试3: " << test3 << std::endl;
    auto blocks3 = parser.parseRawEmbeddings(test3);
    std::cout << "找到 " << blocks3.size() << " 个块" << std::endl;
    
    std::cout << "调试测试完成" << std::endl;
    return 0;
}
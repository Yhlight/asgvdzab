#include "constraints/raw_embedding_parser.hpp"
#include <iostream>

int main() {
    std::cout << "开始简单解析器测试..." << std::endl;
    
    chtl::RawEmbeddingParser parser;
    
    std::string test = "[Origin] @Html { <div>test</div> }";
    std::cout << "测试字符串: " << test << std::endl;
    
    auto blocks = parser.parseRawEmbeddings(test);
    std::cout << "找到 " << blocks.size() << " 个块" << std::endl;
    
    if (!blocks.empty()) {
        std::cout << "第一个块: 类型=" << blocks[0].type 
                  << ", 内容=" << blocks[0].content << std::endl;
    }
    
    std::cout << "测试完成" << std::endl;
    return 0;
}
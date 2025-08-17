#include <iostream>
#include <string>
#include "../Include/Parser/ChtlJsParser.h"

int main() {
    Chtl::ChtlJsParser parser;
    
    std::string testCode = "var el = {{#test}};";
    std::cout << "Input: " << testCode << std::endl;
    
    try {
        std::string result = parser.transform(testCode);
        std::cout << "Output: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}
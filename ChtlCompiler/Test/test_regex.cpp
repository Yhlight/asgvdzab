#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string input = "var el = {{#test}};";
    std::regex selectorRegex(R"(\{\{([^}]+)\}\})");
    
    std::smatch match;
    std::string searchStr = input;
    size_t lastPos = 0;
    
    std::cout << "Input: " << input << std::endl;
    
    while (std::regex_search(searchStr, match, selectorRegex)) {
        std::cout << "Found match at position " << match.position() << ": " << match[0] << std::endl;
        std::cout << "Selector: " << match[1] << std::endl;
        
        lastPos += match.position() + match.length();
        searchStr = input.substr(lastPos);
        std::cout << "Remaining: " << searchStr << std::endl;
        
        if (lastPos >= input.length()) break;
    }
    
    return 0;
}
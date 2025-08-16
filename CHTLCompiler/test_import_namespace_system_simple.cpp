#include <iostream>
#include <vector>
#include "parser/ImportSystem.h"
#include "parser/NamespaceSystem.h"
#include "parser/ModuleSystem.h"
#include "lexer/CHTLToken.h"

void testImportSystem() {
    std::cout << "=== Testing ImportSystem ===" << std::endl;
    
    chtl::ImportSystem import_system;
    
    // Test HTML import parsing
    std::vector<chtl::Token> html_tokens;
    html_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "[Import]", 1, 1, 0));
    html_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "@Html", 1, 10, 9));
    html_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "from", 1, 16, 15));
    html_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "styles.css", 1, 21, 20));
    
    std::cout << "HTML import tokens: " << html_tokens.size() << std::endl;
    
    // Test CSS import parsing
    std::vector<chtl::Token> css_tokens;
    css_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "[Import]", 1, 10, 9));
    css_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "@Style", 1, 17, 16));
    css_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "from", 1, 24, 23));
    css_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "styles.css", 1, 29, 28));
    css_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "as", 1, 40, 39));
    css_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "main_styles", 1, 43, 42));
    
    std::cout << "CSS import tokens: " << css_tokens.size() << std::endl;
    
    // Test CHTL import parsing
    std::vector<chtl::Token> chtl_tokens;
    chtl_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "[Import]", 1, 1, 0));
    chtl_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "@Chtl", 1, 10, 9));
    chtl_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "from", 1, 16, 15));
    chtl_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "components.chtl", 1, 21, 20));
    
    std::cout << "CHTL import tokens: " << chtl_tokens.size() << std::endl;
    
    // Test Custom import parsing
    std::vector<chtl::Token> custom_tokens;
    custom_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "[Import]", 1, 1, 0));
    custom_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "[Custom]", 1, 10, 9));
    custom_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "@Element", 1, 19, 18));
    custom_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "Button", 1, 28, 27));
    custom_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "from", 1, 35, 34));
    custom_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "ui.chtl", 1, 40, 39));
    custom_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "as", 1, 48, 47));
    custom_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "CustomButton", 1, 51, 50));
    
    std::cout << "Custom import tokens: " << custom_tokens.size() << std::endl;
    
    // Test basic management functions
    chtl::ImportInfo import_info;
    import_info.type = chtl::ImportType::HTML_IMPORT;
    import_info.file_path = "test.html";
    import_info.name = "test_import";
    
    import_system.addImport(import_info);
    std::cout << "Added import: " << import_system.getAllImports().size() << std::endl;
    
    std::cout << "ImportSystem test completed." << std::endl;
}

void testNamespaceSystem() {
    std::cout << "=== Testing NamespaceSystem ===" << std::endl;
    
    chtl::NamespaceSystem namespace_system;
    
    // Test simple namespace parsing
    std::vector<chtl::Token> namespace_tokens;
    namespace_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "[Namespace]", 1, 1, 0));
    namespace_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "ui", 1, 13, 12));
    
    std::cout << "Namespace tokens: " << namespace_tokens.size() << std::endl;
    
    // Test nested namespace parsing
    std::vector<chtl::Token> nested_tokens;
    nested_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "[Namespace]", 1, 1, 0));
    nested_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "ui", 1, 13, 12));
    nested_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "{", 1, 16, 15));
    nested_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "[Namespace]", 2, 5, 4));
    nested_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "components", 2, 17, 16));
    nested_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "}", 3, 1, 0));
    
    std::cout << "Nested namespace tokens: " << nested_tokens.size() << std::endl;
    
    // Test namespace items
    std::vector<chtl::Token> custom_tokens;
    custom_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "[Custom]", 1, 1, 0));
    custom_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "@Element", 1, 10, 9));
    custom_tokens.push_back(chtl::Token(chtl::TokenType::IDENTIFIER, "Button", 1, 19, 18));
    
    std::cout << "Custom element tokens: " << custom_tokens.size() << std::endl;
    
    // Test basic management functions
    chtl::NamespaceInfo namespace_info;
    namespace_info.name = "test_namespace";
    
    namespace_system.addNamespace(namespace_info);
    std::cout << "Added namespace: " << namespace_system.getAllNamespaces().size() << std::endl;
    
    std::cout << "NamespaceSystem test completed." << std::endl;
}

void testModuleSystem() {
    std::cout << "=== Testing ModuleSystem ===" << std::endl;
    
    chtl::ModuleSystem module_system;
    
    // Test basic management functions
    chtl::ModuleSystemInfo module_info;
    module_info.name = "test_module";
    module_info.type = chtl::ModuleType::CMOD;
    module_info.file_path = "test.cmod";
    
    module_system.addModule(module_info);
    std::cout << "Added module: " << module_system.getAllModules().size() << std::endl;
    
    std::cout << "ModuleSystem test completed." << std::endl;
}

int main() {
    std::cout << "Starting Import, Namespace, and Module System tests..." << std::endl;
    
    try {
        testImportSystem();
        testNamespaceSystem();
        testModuleSystem();
        
        std::cout << "\nAll tests completed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
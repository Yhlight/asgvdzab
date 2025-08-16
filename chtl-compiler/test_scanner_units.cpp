#include <iostream>
#include <fstream>
#include <sstream>
#include "scanner/unified_scanner.h"

using namespace chtl;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <chtl-file>\n";
        return 1;
    }
    
    // 读取文件
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << argv[1] << "\n";
        return 1;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    
    // 创建扫描器
    scanner::UnifiedScanner scanner;
    scanner.setSliceSize(256); // 使用较小的切片大小进行测试
    
    // 扫描
    auto segments = scanner.scan(source, argv[1]);
    
    std::cout << "Total segments: " << segments.size() << "\n\n";
    
    // 显示每个片段
    for (size_t i = 0; i < segments.size(); ++i) {
        const auto& seg = segments[i];
        std::cout << "=== Segment " << i + 1 << " ===\n";
        std::cout << "Type: ";
        switch (seg.type) {
            case CodeSegmentType::CHTL: std::cout << "CHTL"; break;
            case CodeSegmentType::CHTL_JS: std::cout << "CHTL_JS"; break;
            case CodeSegmentType::CSS: std::cout << "CSS"; break;
            case CodeSegmentType::JAVASCRIPT: std::cout << "JAVASCRIPT"; break;
            case CodeSegmentType::HTML_RAW: std::cout << "HTML_RAW"; break;
            default: std::cout << "UNKNOWN"; break;
        }
        std::cout << "\n";
        std::cout << "Location: " << seg.location.filename << ":" 
                  << seg.location.line << ":" << seg.location.column << "\n";
        std::cout << "Length: " << seg.content.length() << " bytes\n";
        
        // 显示内容（限制长度）
        std::string preview = seg.content;
        if (preview.length() > 100) {
            preview = preview.substr(0, 97) + "...";
        }
        // 替换换行符以便显示
        for (char& c : preview) {
            if (c == '\n') c = '\\';
        }
        std::cout << "Content: \"" << preview << "\"\n\n";
    }
    
    // 显示错误和警告
    auto errors = scanner.getErrors();
    if (!errors.empty()) {
        std::cout << "\nErrors:\n";
        for (const auto& error : errors) {
            std::cout << "  " << error << "\n";
        }
    }
    
    return 0;
}
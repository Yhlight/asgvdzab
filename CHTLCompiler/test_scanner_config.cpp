#include "scanner/CHTLUnifiedScanner.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace chtl;

void printFragmentInfo(const CodeFragment& fragment, size_t index) {
    std::cout << "片段 " << std::setw(3) << index << ": "
              << "类型=" << static_cast<int>(fragment.type)
              << " 行=" << fragment.startLine << "-" << fragment.endLine
              << " 列=" << fragment.startColumn << "-" << fragment.endColumn
              << " 长度=" << fragment.content.length()
              << " 完整=" << (fragment.isComplete ? "是" : "否")
              << " 需扩增=" << (fragment.needsExpansion ? "是" : "否")
              << "\n";
    
    // 显示片段内容的前50个字符
    std::string preview = fragment.content.substr(0, 50);
    if (fragment.content.length() > 50) {
        preview += "...";
    }
    std::cout << "      内容: " << preview << "\n";
}

int main() {
    std::cout << "=== CHTL智能扫描器配置测试 ===\n\n";
    
    // 测试不同的配置
    std::vector<ScannerConfig> configs = {
        ScannerConfig(), // 默认配置
        ScannerConfig{500, 5000, 200, 50, true, true},   // 小切片配置
        ScannerConfig{2000, 15000, 1000, 200, true, true}, // 大切片配置
        ScannerConfig{1000, 10000, 500, 100, true, false}, // 禁用智能切割
    };
    
    std::vector<std::string> configNames = {
        "默认配置",
        "小切片配置", 
        "大切片配置",
        "禁用智能切割"
    };
    
    for (size_t i = 0; i < configs.size(); i++) {
        std::cout << "\n--- " << configNames[i] << " ---\n";
        std::cout << "初始切片大小: " << configs[i].initialSliceSize << "\n";
        std::cout << "最大切片大小: " << configs[i].maxSliceSize << "\n";
        std::cout << "扩增步长: " << configs[i].expansionStep << "\n";
        std::cout << "最小片段大小: " << configs[i].minFragmentSize << "\n";
        std::cout << "启用上下文分析: " << (configs[i].enableContextAnalysis ? "是" : "否") << "\n";
        std::cout << "启用智能切割: " << (configs[i].enableSmartSplitting ? "是" : "否") << "\n";
        
        // 创建扫描器
        CHTLUnifiedScanner scanner(configs[i]);
        
        // 读取测试文件
        std::ifstream file("test_smart_scanning.chtl");
        if (!file.is_open()) {
            std::cerr << "无法打开测试文件\n";
            continue;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        
        // 执行扫描
        auto fragments = scanner.scanString(buffer.str());
        auto stats = scanner.getScanStats();
        
        std::cout << "\n扫描结果:\n";
        std::cout << "总片段数: " << stats.totalFragments << "\n";
        std::cout << "CHTL片段: " << stats.chtlFragments << "\n";
        std::cout << "CHTL JS片段: " << stats.chtlJsFragments << "\n";
        std::cout << "CSS片段: " << stats.cssFragments << "\n";
        std::cout << "JavaScript片段: " << stats.javascriptFragments << "\n";
        std::cout << "总行数: " << stats.totalLines << "\n";
        std::cout << "总扩增次数: " << stats.totalExpansions << "\n";
        std::cout << "总切割次数: " << stats.totalSplits << "\n";
        
        // 显示前10个片段的信息
        std::cout << "\n前10个片段详情:\n";
        for (size_t j = 0; j < std::min(size_t(10), fragments.size()); j++) {
            printFragmentInfo(fragments[j], j);
        }
        
        if (fragments.size() > 10) {
            std::cout << "... 还有 " << (fragments.size() - 10) << " 个片段\n";
        }
        
        // 检查错误
        auto errors = scanner.getErrors();
        if (!errors.empty()) {
            std::cout << "\n扫描错误:\n";
            for (const auto& error : errors) {
                std::cout << "  " << error << "\n";
            }
        }
        
        std::cout << "\n" << std::string(50, '-') << "\n";
    }
    
    return 0;
}
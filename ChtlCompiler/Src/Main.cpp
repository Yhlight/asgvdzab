#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "Parser/Parser.h"
#include "Generator/HtmlGenerator.h"
#include "Module/CmodPackager.h"

using namespace Chtl;
namespace fs = std::filesystem;

void printUsage(const char* programName) {
    std::cout << "CHTL Compiler v1.0.0" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  Compile: " << programName << " <input.chtl> [output.html]" << std::endl;
    std::cout << "  Pack CMOD: " << programName << " pack <directory> <output.cmod>" << std::endl;
    std::cout << "  Unpack CMOD: " << programName << " unpack <input.cmod> <output_directory>" << std::endl;
    std::cout << "  List CMOD: " << programName << " list <input.cmod>" << std::endl;
}

int handleCompile(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string inputFile = argv[1];
    std::string outputFile = argc >= 3 ? argv[2] : "output.html";
    
    // 创建解析器
    Parser parser;
    
    // 设置模块搜索路径
    auto context = parser.getContext();
    if (context && context->getImportResolver()) {
        ModuleSearchPaths paths;
        
        // 官方模块路径（编译器安装目录/module）
        fs::path compilerPath = fs::path(argv[0]).parent_path();
        paths.officialModulePath = (compilerPath / "module").string();
        paths.compilerPath = compilerPath.string();
        
        // 本地模块路径（当前目录/module）
        paths.localModulePath = (fs::current_path() / "module").string();
        
        // 当前文件所在目录
        paths.currentPath = fs::path(inputFile).parent_path().string();
        
        context->getImportResolver()->setModuleSearchPaths(paths);
        context->getImportResolver()->setCurrentFilePath(inputFile);
    }
    
    // 解析文件
    auto ast = parser.parseFile(inputFile);
    if (!ast) {
        std::cerr << "Parse error: " << parser.getLastError() << std::endl;
        return 1;
    }
    
    // 创建HTML生成器
    HtmlGenerator generator;
    
    // 生成HTML
    std::string html = generator.generate(ast, context);
    
    // 写入输出文件
    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << "Cannot create output file: " << outputFile << std::endl;
        return 1;
    }
    
    out << html;
    out.close();
    
    std::cout << "Successfully compiled to " << outputFile << std::endl;
    
    // 如果有CSS或JS，也写入相应文件
    std::string css = generator.getGeneratedCss();
    if (!css.empty()) {
        std::string cssFile = fs::path(outputFile).replace_extension(".css").string();
        std::ofstream cssOut(cssFile);
        cssOut << css;
        cssOut.close();
        std::cout << "Generated " << cssFile << std::endl;
    }
    
    std::string js = generator.getGeneratedJs();
    if (!js.empty()) {
        std::string jsFile = fs::path(outputFile).replace_extension(".js").string();
        std::ofstream jsOut(jsFile);
        jsOut << js;
        jsOut.close();
        std::cout << "Generated " << jsFile << std::endl;
    }
    
    return 0;
}

int handlePack(int argc, char* argv[]) {
    if (argc < 4) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string sourceDir = argv[2];
    std::string outputFile = argv[3];
    
    CmodPackager packager;
    
    // 可选：启用压缩
    packager.setCompression(true);
    
    std::cout << "Packing " << sourceDir << " to " << outputFile << "..." << std::endl;
    
    if (packager.packDirectory(sourceDir, outputFile)) {
        std::cout << "Successfully created " << outputFile << std::endl;
        
        // 显示包信息
        CmodHeader header;
        if (packager.readHeader(outputFile, header)) {
            std::cout << "Module: " << header.moduleName << std::endl;
            std::cout << "Files: " << header.fileCount << std::endl;
            std::cout << "Size: " << header.totalSize << " bytes" << std::endl;
        }
    } else {
        std::cerr << "Pack error: " << packager.getLastError() << std::endl;
        return 1;
    }
    
    return 0;
}

int handleUnpack(int argc, char* argv[]) {
    if (argc < 4) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string cmodFile = argv[2];
    std::string outputDir = argv[3];
    
    CmodPackager packager;
    
    std::cout << "Unpacking " << cmodFile << " to " << outputDir << "..." << std::endl;
    
    if (packager.unpack(cmodFile, outputDir)) {
        std::cout << "Successfully unpacked to " << outputDir << std::endl;
    } else {
        std::cerr << "Unpack error: " << packager.getLastError() << std::endl;
        return 1;
    }
    
    return 0;
}

int handleList(int argc, char* argv[]) {
    if (argc < 3) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string cmodFile = argv[2];
    
    CmodPackager packager;
    
    // 读取头信息
    CmodHeader header;
    if (!packager.readHeader(cmodFile, header)) {
        std::cerr << "Error: " << packager.getLastError() << std::endl;
        return 1;
    }
    
    std::cout << "CMOD File: " << cmodFile << std::endl;
    std::cout << "Module Name: " << header.moduleName << std::endl;
    std::cout << "Author: " << header.author << std::endl;
    std::cout << "Description: " << header.description << std::endl;
    std::cout << "Version: " << header.version << std::endl;
    std::cout << "Files: " << header.fileCount << std::endl;
    std::cout << "Total Size: " << header.totalSize << " bytes" << std::endl;
    
    // TODO: 列出文件详情
    
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string command = argv[1];
    
    if (command == "pack") {
        return handlePack(argc, argv);
    } else if (command == "unpack") {
        return handleUnpack(argc, argv);
    } else if (command == "list") {
        return handleList(argc, argv);
    } else {
        // 默认为编译命令
        return handleCompile(argc, argv);
    }
}
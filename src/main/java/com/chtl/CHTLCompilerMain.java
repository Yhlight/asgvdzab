package com.chtl;
import java.nio.file.Files;
import java.nio.file.Paths;

import com.chtl.model.CompilationResult;
import com.chtl.compiler.cmod.CmodPackager;
import com.chtl.compiler.cmod.CmodUnpacker;


import com.chtl.scanner.CHTLUnifiedScanner;
import com.chtl.model.CodeFragment;
import com.chtl.compiler.dispatcher.CompilerDispatcher;
import com.chtl.model.CompilationResult;
import com.chtl.output.HTMLMerger;
import com.chtl.context.CompilationContext;
import com.chtl.compiler.cmod.*;

import java.io.IOException;
import java.nio.file.*;
import java.util.List;
import java.util.concurrent.ExecutionException;

/**
 * CHTL编译器主入口
 * 增强版：支持Cmod打包和解包
 */
public class CHTLCompilerMain {
    
    public static void main(String[] args) {
        if (args.length < 1) {
            printUsage();
            System.exit(1);
        }
        
        String command = args[0];
        
        try {
            switch (command) {
                case "compile":
                    if (args.length < 3) {
                        System.err.println("用法: compile <输入文件> <输出文件>");
                        System.exit(1);
                    }
                    compileCHTL(args[1], args[2]);
                    break;
                    
                case "package":
                    if (args.length < 4) {
                        System.err.println("用法: package <源目录> <输出文件> <模块名> [命名空间]");
                        System.exit(1);
                    }
                    String namespace = args.length > 4 ? args[4] : args[3];
                    packageCmod(args[1], args[2], args[3], namespace);
                    break;
                    
                case "unpack":
                    if (args.length < 3) {
                        System.err.println("用法: unpack <cmod文件> <目标目录>");
                        System.exit(1);
                    }
                    unpackCmod(args[1], args[2]);
                    break;
                    
                case "info":
                    if (args.length < 2) {
                        System.err.println("用法: info <cmod文件>");
                        System.exit(1);
                    }
                    showCmodInfo(args[1]);
                    break;
                    
                default:
                    System.err.println("未知命令: " + command);
                    printUsage();
                    System.exit(1);
            }
        } catch (Exception e) {
            System.err.println("错误: " + e.getMessage());
            e.printStackTrace();
            System.exit(1);
        }
    }
    
    private static void printUsage() {
        System.out.println("CHTL编译器使用方法:");
        System.out.println("  java CHTLCompilerMain compile <输入文件> <输出文件>");
        System.out.println("    - 编译CHTL文件为HTML");
        System.out.println();
        System.out.println("  java CHTLCompilerMain package <源目录> <输出文件> <模块名> [命名空间]");
        System.out.println("    - 将CHTL源文件打包为Cmod格式");
        System.out.println();
        System.out.println("  java CHTLCompilerMain unpack <cmod文件> <目标目录>");
        System.out.println("    - 解包Cmod文件");
        System.out.println();
        System.out.println("  java CHTLCompilerMain info <cmod文件>");
        System.out.println("    - 显示Cmod文件信息");
    }
    
    /**
     * 编译CHTL文件
     */
    private static void compileCHTL(String inputFile, String outputFile) 
            throws IOException, ExecutionException, InterruptedException {
        
        System.out.println("开始编译CHTL文件...");
        
        // 读取源文件
        String sourceCode = Files.readString(Paths.get(inputFile));
        
        // 创建编译上下文
        CompilationContext context = new CompilationContext();
        
        // 扫描代码
        CHTLUnifiedScanner scanner = new CHTLUnifiedScanner();
        List<CodeFragment> fragments = scanner.scan(sourceCode);
        
        System.out.println("扫描完成，发现 " + fragments.size() + " 个代码片段");
        
        // 编译片段
        CompilerDispatcher dispatcher = new CompilerDispatcher();
        List<CompilationResult> results = dispatcher.compile(fragments);
        
        System.out.println("编译完成，生成 " + results.size() + " 个结果");
        
        // 合并结果
        HTMLMerger merger = new HTMLMerger();
        String html = merger.merge(results);
        
        // 写入输出文件
        Files.writeString(Paths.get(outputFile), html);
        
        System.out.println("输出文件已生成: " + outputFile);
    }
    
    /**
     * 打包Cmod
     */
    private static void packageCmod(String sourceDir, String outputFile, 
                                   String moduleName, String namespace) throws IOException {
        
        System.out.println("开始打包Cmod...");
        System.out.println("源目录: " + sourceDir);
        System.out.println("模块名: " + moduleName);
        System.out.println("命名空间: " + namespace);
        
        // 创建编译上下文
        CompilationContext context = new CompilationContext();
        
        // 创建打包器
        CmodPackager packager = new CmodPackager(
            context,
            Paths.get(sourceDir),
            moduleName,
            namespace
        );
        
        // 打包
        packager.packageCmod(Paths.get(outputFile));
        
        System.out.println("Cmod打包完成: " + outputFile);
    }
    
    /**
     * 解包Cmod
     */
    private static void unpackCmod(String cmodFile, String targetDir) throws IOException {
        
        System.out.println("开始解包Cmod...");
        System.out.println("Cmod文件: " + cmodFile);
        System.out.println("目标目录: " + targetDir);
        
        // 创建编译上下文
        CompilationContext context = new CompilationContext();
        
        // 创建解包器
        CmodUnpacker unpacker = new CmodUnpacker(context);
        
        // 解包
        CmodUnpacker.UnpackResult result = unpacker.unpackCmod(
            Paths.get(cmodFile),
            Paths.get(targetDir)
        );
        
        System.out.println("\n解包完成!");
        System.out.println("模块名: " + result.getModuleName());
        System.out.println("命名空间: " + result.getModuleNamespace());
        System.out.println("版本: " + result.getVersion());
        System.out.println("源文件数: " + result.getSourceFiles().size());
        System.out.println("资源文件数: " + result.getResourceFiles().size());
        System.out.println("编译模块数: " + result.getCompiledModules().size());
    }
    
    /**
     * 显示Cmod信息
     */
    private static void showCmodInfo(String cmodFile) throws IOException {
        
        System.out.println("读取Cmod信息...");
        
        // 创建编译上下文
        CompilationContext context = new CompilationContext();
        
        // 创建解包器
        CmodUnpacker unpacker = new CmodUnpacker(context);
        
        // 读取信息
        CmodUnpacker.CmodInfo info = unpacker.readCmodInfo(Paths.get(cmodFile));
        
        System.out.println("\nCmod信息:");
        System.out.println("  名称: " + info.getName());
        System.out.println("  命名空间: " + info.getNamespace());
        System.out.println("  版本: " + info.getVersion());
        System.out.println("  导出符号: " + info.getExports());
        System.out.println("  依赖: " + (info.getDependencies() != null ? info.getDependencies() : "无"));
        System.out.println("  创建时间: " + info.getCreated());
        System.out.println("  编译器版本: " + info.getCompilerVersion());
    }
}
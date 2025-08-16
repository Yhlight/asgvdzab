package com.chtl.cli;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Map;
import com.chtl.compiler.CHTLCompiler;
import com.chtl.compiler.CompilerResult;
import com.chtl.compiler.CompilerOptions;
import com.chtl.compiler.*;
import com.chtl.compiler.debug.ColorUtil;
import com.chtl.compiler.optimization.OptimizationOptions;

/**
 * CHTL命令行接口
 * 提供完整的命令行编译功能
 */
public class CHTLCLI {
    private static final String VERSION = "1.0.0";
    private static final String USAGE = """
        CHTL编译器 v%s
        
        用法: chtl [选项] <源文件>
        
        选项:
          -o, --output <文件>      指定输出文件路径
          -O, --optimize           启用优化（默认）
          --no-optimize            禁用优化
          --production             生产模式（最大优化）
          --development            开发模式（调试友好）
          
        调试选项:
          -d, --debug              启用调试模式
          -v, --verbose            详细输出
          --show-tokens            显示词法分析结果
          --show-ast               显示语法树
          --show-states            显示状态变化
          
        错误处理:
          --max-errors <数量>      最大错误数（默认100）
          --fail-fast              遇到第一个错误时停止
          --no-warnings            不显示警告
          
        并行选项:
          -j, --jobs <数量>        并行任务数
          --timeout <秒>           编译超时时间（默认60）
          
        Source Map:
          --source-map             生成Source Map（默认）
          --no-source-map          不生成Source Map
          --source-map-path <路径> Source Map输出路径
          
        模块选项:
          --cmod-path <路径>       Cmod模块路径
          --official-path <路径>   官方模块路径
          
        其他:
          -h, --help               显示帮助信息
          -V, --version            显示版本信息
          --watch                  监视文件变化并自动编译
          --stats                  显示编译统计信息
        
        示例:
          chtl input.chtl                     # 编译文件
          chtl -o output.html input.chtl      # 指定输出
          chtl --production -o dist.html app.chtl  # 生产构建
          chtl -d --show-ast test.chtl        # 调试模式
        """;
    
    public static void main(String[] args) {
        if (args.length == 0) {
            printUsage();
            System.exit(1);
        }
        
        try {
            CLIOptions cliOptions = parseArgs(args);
            
            if (cliOptions.showHelp) {
                printUsage();
                return;
            }
            
            if (cliOptions.showVersion) {
                printVersion();
                return;
            }
            
            if (cliOptions.sourceFile == null) {
                System.err.println(ColorUtil.red("错误: 未指定源文件"));
                printUsage();
                System.exit(1);
            }
            
            // 执行编译
            if (cliOptions.watchMode) {
                runWatchMode(cliOptions);
            } else {
                runCompilation(cliOptions);
            }
            
        } catch (Exception e) {
            System.err.println(ColorUtil.red("错误: ") + e.getMessage());
            System.exit(1);
        }
    }
    
    /**
     * 解析命令行参数
     */
    private static CLIOptions parseArgs(String[] args) {
        CLIOptions options = new CLIOptions();
        
        for (int i = 0; i < args.length; i++) {
            String arg = args[i];
            
            switch (arg) {
                case "-h":
                case "--help":
                    options.showHelp = true;
                    return options;
                    
                case "-V":
                case "--version":
                    options.showVersion = true;
                    return options;
                    
                case "-o":
                case "--output":
                    if (i + 1 < args.length) {
                        options.outputFile = args[++i];
                    } else {
                        throw new IllegalArgumentException("选项 " + arg + " 需要参数");
                    }
                    break;
                    
                case "-O":
                case "--optimize":
                    options.optimize = true;
                    break;
                    
                case "--no-optimize":
                    options.optimize = false;
                    break;
                    
                case "--production":
                    options.mode = CompilationMode.PRODUCTION;
                    break;
                    
                case "--development":
                    options.mode = CompilationMode.DEVELOPMENT;
                    break;
                    
                case "-d":
                case "--debug":
                    options.debug = true;
                    break;
                    
                case "-v":
                case "--verbose":
                    options.verbose = true;
                    break;
                    
                case "--show-tokens":
                    options.showTokens = true;
                    break;
                    
                case "--show-ast":
                    options.showAST = true;
                    break;
                    
                case "--show-states":
                    options.showStates = true;
                    break;
                    
                case "--max-errors":
                    if (i + 1 < args.length) {
                        options.maxErrors = Integer.parseInt(args[++i]);
                    } else {
                        throw new IllegalArgumentException("选项 " + arg + " 需要参数");
                    }
                    break;
                    
                case "--fail-fast":
                    options.failFast = true;
                    break;
                    
                case "--no-warnings":
                    options.showWarnings = false;
                    break;
                    
                case "-j":
                case "--jobs":
                    if (i + 1 < args.length) {
                        options.parallelism = Integer.parseInt(args[++i]);
                    } else {
                        throw new IllegalArgumentException("选项 " + arg + " 需要参数");
                    }
                    break;
                    
                case "--timeout":
                    if (i + 1 < args.length) {
                        options.timeout = Integer.parseInt(args[++i]);
                    } else {
                        throw new IllegalArgumentException("选项 " + arg + " 需要参数");
                    }
                    break;
                    
                case "--source-map":
                    options.generateSourceMap = true;
                    break;
                    
                case "--no-source-map":
                    options.generateSourceMap = false;
                    break;
                    
                case "--source-map-path":
                    if (i + 1 < args.length) {
                        options.sourceMapPath = args[++i];
                    } else {
                        throw new IllegalArgumentException("选项 " + arg + " 需要参数");
                    }
                    break;
                    
                case "--cmod-path":
                    if (i + 1 < args.length) {
                        options.cmodPath = args[++i];
                    } else {
                        throw new IllegalArgumentException("选项 " + arg + " 需要参数");
                    }
                    break;
                    
                case "--official-path":
                    if (i + 1 < args.length) {
                        options.officialPath = args[++i];
                    } else {
                        throw new IllegalArgumentException("选项 " + arg + " 需要参数");
                    }
                    break;
                    
                case "--watch":
                    options.watchMode = true;
                    break;
                    
                case "--stats":
                    options.showStats = true;
                    break;
                    
                default:
                    if (arg.startsWith("-")) {
                        throw new IllegalArgumentException("未知选项: " + arg);
                    }
                    // 源文件
                    if (options.sourceFile == null) {
                        options.sourceFile = arg;
                    } else {
                        throw new IllegalArgumentException("只能指定一个源文件");
                    }
            }
        }
        
        return options;
    }
    
    /**
     * 执行编译
     */
    private static void runCompilation(CLIOptions cliOptions) throws IOException {
        long startTime = System.currentTimeMillis();
        
        // 创建编译器选项
        CompilerOptions compilerOptions = createCompilerOptions(cliOptions);
        
        // 创建编译器
        CHTLCompiler compiler = new CHTLCompiler(compilerOptions);
        
        try {
            // 编译
            System.out.println(ColorUtil.cyan("编译 " + cliOptions.sourceFile + "..."));
            CompilationResult result = compiler.compile(cliOptions.sourceFile);
            
            // 处理结果
            if (result.isSuccess()) {
                // 写入输出文件
                String outputPath = cliOptions.outputFile;
                if (outputPath == null) {
                    // 默认输出文件名
                    Path sourcePath = Paths.get(cliOptions.sourceFile);
                    String baseName = sourcePath.getFileName().toString();
                    if (baseName.endsWith(".chtl")) {
                        baseName = baseName.substring(0, baseName.length() - 5);
                    }
                    outputPath = baseName + ".html";
                }
                
                Files.writeString(Paths.get(outputPath), result.getOutput());
                System.out.println(ColorUtil.green("✓ 编译成功: ") + outputPath);
                
                // 写入Source Map
                if (compilerOptions.isGenerateSourceMap() && !result.getSourceMaps().isEmpty()) {
                    for (Map.Entry<String, String> entry : result.getSourceMaps().entrySet()) {
                        String mapPath = cliOptions.sourceMapPath != null ? 
                            cliOptions.sourceMapPath : entry.getKey() + ".map";
                        Files.writeString(Paths.get(mapPath), entry.getValue());
                        System.out.println(ColorUtil.green("✓ Source Map: ") + mapPath);
                    }
                }
                
                // 显示警告
                if (cliOptions.showWarnings && result.hasWarnings()) {
                    System.out.println(ColorUtil.yellow("\n警告 (" + result.getWarningCount() + "):"));
                    for (var warning : result.getWarnings()) {
                        System.out.println("  " + warning);
                    }
                }
                
            } else {
                // 编译失败
                System.err.println(ColorUtil.red("✗ 编译失败"));
                
                // 显示错误
                if (result.hasErrors()) {
                    System.err.println(ColorUtil.red("\n错误 (" + result.getErrorCount() + "):"));
                    for (var error : result.getErrors()) {
                        System.err.println("  " + error);
                    }
                }
                
                System.exit(1);
            }
            
            // 显示统计信息
            if (cliOptions.showStats) {
                long endTime = System.currentTimeMillis();
                printStats(result, endTime - startTime);
            }
            
        } finally {
            compiler.shutdown();
        }
    }
    
    /**
     * 监视模式
     */
    private static void runWatchMode(CLIOptions cliOptions) throws IOException {
        System.out.println(ColorUtil.cyan("进入监视模式..."));
        System.out.println("按 Ctrl+C 退出");
        
        Path sourcePath = Paths.get(cliOptions.sourceFile);
        long lastModified = Files.getLastModifiedTime(sourcePath).toMillis();
        
        while (true) {
            try {
                Thread.sleep(1000); // 每秒检查一次
                
                long currentModified = Files.getLastModifiedTime(sourcePath).toMillis();
                if (currentModified != lastModified) {
                    lastModified = currentModified;
                    System.out.println(ColorUtil.yellow("\n检测到文件变化，重新编译..."));
                    runCompilation(cliOptions);
                }
                
            } catch (InterruptedException e) {
                break;
            } catch (Exception e) {
                System.err.println(ColorUtil.red("监视错误: ") + e.getMessage());
            }
        }
    }
    
    /**
     * 创建编译器选项
     */
    private static CompilerOptions createCompilerOptions(CLIOptions cliOptions) {
        CompilerOptions options;
        
        // 根据模式创建基础选项
        switch (cliOptions.mode) {
            case PRODUCTION:
                options = CompilerOptions.production();
                break;
            case DEVELOPMENT:
                options = CompilerOptions.development();
                break;
            default:
                options = new CompilerOptions();
        }
        
        // 应用CLI选项
        if (cliOptions.optimize != null) {
            options.setOptimizationEnabled(cliOptions.optimize);
        }
        
        options.setDebugMode(cliOptions.debug);
        options.setVerboseMode(cliOptions.verbose);
        options.setShowTokens(cliOptions.showTokens);
        options.setShowAST(cliOptions.showAST);
        options.setShowStates(cliOptions.showStates);
        
        options.setMaxErrors(cliOptions.maxErrors);
        options.setFailFast(cliOptions.failFast);
        options.setShowWarnings(cliOptions.showWarnings);
        
        if (cliOptions.parallelism > 0) {
            options.setParallelism(cliOptions.parallelism);
        }
        if (cliOptions.timeout > 0) {
            options.setCompilationTimeout(cliOptions.timeout);
        }
        
        if (cliOptions.generateSourceMap != null) {
            options.setGenerateSourceMap(cliOptions.generateSourceMap);
        }
        
        if (cliOptions.cmodPath != null) {
            options.setCmodPath(cliOptions.cmodPath);
        }
        if (cliOptions.officialPath != null) {
            options.setOfficialModulePath(cliOptions.officialPath);
        }
        
        return options;
    }
    
    /**
     * 打印使用说明
     */
    private static void printUsage() {
        System.out.println(String.format(USAGE, VERSION));
    }
    
    /**
     * 打印版本信息
     */
    private static void printVersion() {
        System.out.println("CHTL编译器 v" + VERSION);
        System.out.println("Java版本: " + System.getProperty("java.version"));
        System.out.println("操作系统: " + System.getProperty("os.name") + " " + System.getProperty("os.version"));
    }
    
    /**
     * 打印统计信息
     */
    private static void printStats(CompilationResult result, long totalTime) {
        System.out.println("\n" + ColorUtil.cyan("=== 编译统计 ==="));
        System.out.println("总耗时: " + totalTime + "ms");
        System.out.println("编译耗时: " + result.getCompilationTime() + "ms");
        
        if (result.getOutput() != null) {
            System.out.println("输出大小: " + formatBytes(result.getOutput().length()));
        }
        
        System.out.println("错误数: " + result.getErrorCount());
        System.out.println("警告数: " + result.getWarningCount());
        
        if (!result.getSourceMaps().isEmpty()) {
            System.out.println("Source Maps: " + result.getSourceMaps().size());
        }
    }
    
    /**
     * 格式化字节数
     */
    private static String formatBytes(long bytes) {
        if (bytes < 1024) {
            return bytes + " B";
        } else if (bytes < 1024 * 1024) {
            return String.format("%.2f KB", bytes / 1024.0);
        } else {
            return String.format("%.2f MB", bytes / (1024.0 * 1024));
        }
    }
    
    /**
     * CLI选项
     */
    private static class CLIOptions {
        String sourceFile;
        String outputFile;
        
        boolean showHelp = false;
        boolean showVersion = false;
        
        CompilationMode mode = CompilationMode.DEFAULT;
        Boolean optimize = null;
        
        boolean debug = false;
        boolean verbose = false;
        boolean showTokens = false;
        boolean showAST = false;
        boolean showStates = false;
        
        int maxErrors = 100;
        boolean failFast = false;
        boolean showWarnings = true;
        
        int parallelism = -1;
        int timeout = -1;
        
        Boolean generateSourceMap = null;
        String sourceMapPath;
        
        String cmodPath;
        String officialPath;
        
        boolean watchMode = false;
        boolean showStats = false;
    }
    
    /**
     * 编译模式
     */
    private enum CompilationMode {
        DEFAULT,
        DEVELOPMENT,
        PRODUCTION
    }
}

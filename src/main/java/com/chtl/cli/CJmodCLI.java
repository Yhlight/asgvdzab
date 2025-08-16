package com.chtl.cli;
import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CompletableFuture;
import com.chtl.cjmod.ModulePackager;
import com.chtl.cjmod.ModuleRepository;
import com.chtl.cjmod.ModuleSigner;
import com.chtl.cjmod.ModuleVersion;
import com.chtl.compiler.cjmod.CJmodManager;
import com.chtl.cjmod.*;

/**
 * CJMOD命令行工具
 * 提供模块创建、打包、签名、安装等功能
 */
public class CJmodCLI {
    
    private static final String VERSION = "1.0.0";
    private static final String TOOL_NAME = "cjmod";
    
    public static void main(String[] args) {
        if (args.length == 0) {
            printUsage();
            System.exit(0);
        }
        
        String command = args[0];
        String[] commandArgs = Arrays.copyOfRange(args, 1, args.length);
        
        try {
            switch (command) {
                case "create":
                    handleCreate(commandArgs);
                    break;
                case "package":
                    handlePackage(commandArgs);
                    break;
                case "sign":
                    handleSign(commandArgs);
                    break;
                case "verify":
                    handleVerify(commandArgs);
                    break;
                case "install":
                    handleInstall(commandArgs);
                    break;
                case "uninstall":
                    handleUninstall(commandArgs);
                    break;
                case "list":
                    handleList(commandArgs);
                    break;
                case "info":
                    handleInfo(commandArgs);
                    break;
                case "search":
                    handleSearch(commandArgs);
                    break;
                case "version":
                    handleVersion();
                    break;
                case "help":
                    printUsage();
                    break;
                default:
                    System.err.println("Unknown command: " + command);
                    printUsage();
                    System.exit(1);
            }
        } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
            if (System.getenv("CJMOD_DEBUG") != null) {
                e.printStackTrace();
            }
            System.exit(1);
        }
    }
    
    /**
     * 创建新模块
     */
    private static void handleCreate(String[] args) throws Exception {
        if (args.length < 1) {
            System.err.println("Usage: cjmod create <module-name> [options]");
            System.exit(1);
        }
        
        String moduleName = args[0];
        Path modulePath = Paths.get(moduleName);
        
        // 解析选项
        String author = "Unknown";
        String description = "A CJMOD module";
        String mainClass = moduleName + ".Main";
        
        for (int i = 1; i < args.length; i++) {
            if (args[i].equals("--author") && i + 1 < args.length) {
                author = args[++i];
            } else if (args[i].equals("--description") && i + 1 < args.length) {
                description = args[++i];
            } else if (args[i].equals("--main") && i + 1 < args.length) {
                mainClass = args[++i];
            }
        }
        
        // 创建目录结构
        Files.createDirectories(modulePath);
        Files.createDirectories(modulePath.resolve("src"));
        Files.createDirectories(modulePath.resolve("lib"));
        Files.createDirectories(modulePath.resolve("resources"));
        Files.createDirectories(modulePath.resolve("docs"));
        
        // 创建module.json
        String moduleJson = String.format("""
            {
                "name": "%s",
                "version": "1.0.0",
                "mainClass": "%s",
                "description": "%s",
                "author": "%s",
                "license": "MIT",
                "dependencies": []
            }
            """, moduleName, mainClass, description, author);
        
        Files.writeString(modulePath.resolve("module.json"), moduleJson);
        
        // 创建主类文件
        String packageName = mainClass.substring(0, mainClass.lastIndexOf('.'));
        String className = mainClass.substring(mainClass.lastIndexOf('.') + 1);
        Path mainClassPath = modulePath.resolve("src")
            .resolve(packageName.replace('.', File.separatorChar))
            .resolve(className + ".java");
        
        Files.createDirectories(mainClassPath.getParent());
        
        String mainClassContent = String.format("""
            package %s;
            
import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CompletableFuture;
import com.chtl.cjmod.ModulePackager;
import com.chtl.cjmod.ModuleRepository;
import com.chtl.cjmod.ModuleSigner;
import com.chtl.cjmod.ModuleVersion;
import com.chtl.compiler.cjmod.CJmodManager;
import com.chtl.cjmod.*;

            public class %s extends CJmodBase {
                
                @Override
                public void initialize(CJmodManager manager) {
                    super.initialize(manager);
                    System.out.println("Module %s initialized");
                }
                
                @Override
                protected void registerFunctions() {
                    // Register your functions here
                    registerFunction("hello", this::hello);
                }
                
                private Object hello(Map<String, Object> params) {
                    String name = (String) params.getOrDefault("name", "World");
                    return "Hello, " + name + "!";
                }
            }
            """, packageName, className, moduleName);
        
        Files.writeString(mainClassPath, mainClassContent);
        
        // 创建README
        String readme = String.format("""
            # %s
            
            %s
            
            ## Installation
            
            ```bash
            cjmod install %s
            ```
            
            ## Usage
            
            ```chtl
            Import @CJmod %s
            
            script {
                var result = hello({ name: "CHTL" });
                console.log(result);
            }
            ```
            
            ## License
            
            MIT
            """, moduleName, description, moduleName, moduleName);
        
        Files.writeString(modulePath.resolve("README.md"), readme);
        
        System.out.println("Created module: " + moduleName);
        System.out.println("Directory: " + modulePath.toAbsolutePath());
    }
    
    /**
     * 打包模块
     */
    private static void handlePackage(String[] args) throws Exception {
        if (args.length < 1) {
            System.err.println("Usage: cjmod package <module-path> [options]");
            System.exit(1);
        }
        
        Path modulePath = Paths.get(args[0]);
        Path outputPath = Paths.get(".");
        
        // 解析选项
        ModulePackager.PackageOptions options = new ModulePackager.PackageOptions();
        
        for (int i = 1; i < args.length; i++) {
            switch (args[i]) {
                case "--output":
                case "-o":
                    if (i + 1 < args.length) {
                        outputPath = Paths.get(args[++i]);
                    }
                    break;
                case "--no-source":
                    options.withSource(false);
                    break;
                case "--no-docs":
                    options.withDocs(false);
                    break;
                case "--no-compress":
                    options.withCompression(false);
                    break;
                case "--exclude":
                    if (i + 1 < args.length) {
                        options.excludePattern(args[++i]);
                    }
                    break;
            }
        }
        
        // 执行打包
        ModulePackager packager = new ModulePackager();
        ModulePackager.PackageResult result = packager.packageModule(modulePath, outputPath, options);
        
        System.out.println("Module packaged successfully:");
        System.out.println("  File: " + result.getPackagePath());
        System.out.println("  Size: " + formatSize(result.getSize()));
        System.out.println("  Checksum: " + result.getChecksum());
    }
    
    /**
     * 签名模块
     */
    private static void handleSign(String[] args) throws Exception {
        if (args.length < 1) {
            System.err.println("Usage: cjmod sign <module-path> [options]");
            System.exit(1);
        }
        
        Path modulePath = Paths.get(args[0]);
        String signer = System.getProperty("user.name");
        Path privateKeyPath = null;
        Path publicKeyPath = null;
        
        // 解析选项
        for (int i = 1; i < args.length; i++) {
            switch (args[i]) {
                case "--signer":
                    if (i + 1 < args.length) {
                        signer = args[++i];
                    }
                    break;
                case "--private-key":
                    if (i + 1 < args.length) {
                        privateKeyPath = Paths.get(args[++i]);
                    }
                    break;
                case "--public-key":
                    if (i + 1 < args.length) {
                        publicKeyPath = Paths.get(args[++i]);
                    }
                    break;
            }
        }
        
        ModuleSigner signer2 = new ModuleSigner();
        ModuleSigner.KeyPairInfo keyPair;
        
        // 生成或加载密钥对
        if (privateKeyPath != null && publicKeyPath != null) {
            keyPair = signer2.loadKeyPair(publicKeyPath, privateKeyPath);
        } else {
            System.out.println("Generating new key pair...");
            keyPair = signer2.generateKeyPair();
            
            // 保存密钥
            Path keysDir = Paths.get(System.getProperty("user.home"), ".cjmod", "keys");
            Files.createDirectories(keysDir);
            
            publicKeyPath = keysDir.resolve("public.key");
            privateKeyPath = keysDir.resolve("private.key");
            
            signer2.saveKeyPair(keyPair, publicKeyPath, privateKeyPath);
            System.out.println("Keys saved to: " + keysDir);
        }
        
        // 签名模块
        signer2.signModule(modulePath, keyPair, signer);
        
        System.out.println("Module signed successfully");
        System.out.println("  Signer: " + signer);
        System.out.println("  Key fingerprint: " + keyPair.getFingerprint());
    }
    
    /**
     * 验证模块签名
     */
    private static void handleVerify(String[] args) throws Exception {
        if (args.length < 1) {
            System.err.println("Usage: cjmod verify <module-path>");
            System.exit(1);
        }
        
        Path modulePath = Paths.get(args[0]);
        
        ModuleSigner signer = new ModuleSigner();
        ModuleSigner.VerificationResult result = signer.verifyModule(modulePath);
        
        if (result.isValid()) {
            System.out.println("Module signature is VALID");
            if (result.getSignatureInfo() != null) {
                System.out.println("  Signer: " + result.getSignatureInfo().getSigner());
                System.out.println("  Timestamp: " + result.getSignatureInfo().getTimestamp());
                System.out.println("  Algorithm: " + result.getSignatureInfo().getAlgorithm());
            }
        } else {
            System.out.println("Module signature is INVALID");
            for (String error : result.getErrors()) {
                System.err.println("  Error: " + error);
            }
            System.exit(1);
        }
        
        for (String warning : result.getWarnings()) {
            System.out.println("  Warning: " + warning);
        }
    }
    
    /**
     * 安装模块
     */
    private static void handleInstall(String[] args) throws Exception {
        if (args.length < 1) {
            System.err.println("Usage: cjmod install <module-file|module-name[@version]>");
            System.exit(1);
        }
        
        String moduleSpec = args[0];
        boolean force = false;
        
        // 解析选项
        for (int i = 1; i < args.length; i++) {
            if (args[i].equals("--force") || args[i].equals("-f")) {
                force = true;
            }
        }
        
        ModuleRepository repository = new ModuleRepository();
        
        // 判断是文件还是模块名
        Path modulePath = Paths.get(moduleSpec);
        if (Files.exists(modulePath)) {
            // 安装本地文件
            CompletableFuture<ModuleRepository.ModuleInfo> future = 
                repository.installModule(modulePath, force);
            
            ModuleRepository.ModuleInfo info = future.get();
            System.out.println("Module installed successfully:");
            System.out.println("  Name: " + info.getName());
            System.out.println("  Version: " + info.getVersion());
            System.out.println("  Path: " + info.getPath());
        } else {
            // 从仓库安装
            System.err.println("Remote repository installation not yet implemented");
            System.exit(1);
        }
    }
    
    /**
     * 卸载模块
     */
    private static void handleUninstall(String[] args) throws Exception {
        if (args.length < 1) {
            System.err.println("Usage: cjmod uninstall <module-name[@version]>");
            System.exit(1);
        }
        
        String moduleSpec = args[0];
        String moduleName;
        ModuleVersion version = null;
        
        // 解析模块名和版本
        int atIndex = moduleSpec.indexOf('@');
        if (atIndex >= 0) {
            moduleName = moduleSpec.substring(0, atIndex);
            version = ModuleVersion.parse(moduleSpec.substring(atIndex + 1));
        } else {
            moduleName = moduleSpec;
        }
        
        ModuleRepository repository = new ModuleRepository();
        
        if (version == null) {
            // 获取最新版本
            ModuleRepository.ModuleInfo info = repository.getLatestVersion(moduleName);
            if (info == null) {
                System.err.println("Module not found: " + moduleName);
                System.exit(1);
            }
            version = info.getVersion();
        }
        
        CompletableFuture<Boolean> future = repository.removeModule(moduleName, version);
        boolean removed = future.get();
        
        if (removed) {
            System.out.println("Module uninstalled: " + moduleName + "@" + version);
        } else {
            System.err.println("Failed to uninstall module");
            System.exit(1);
        }
    }
    
    /**
     * 列出已安装的模块
     */
    private static void handleList(String[] args) throws Exception {
        ModuleRepository repository = new ModuleRepository();
        List<ModuleRepository.ModuleInfo> modules = repository.getAllModules();
        
        if (modules.isEmpty()) {
            System.out.println("No modules installed");
            return;
        }
        
        System.out.println("Installed modules:");
        System.out.println();
        
        String lastModule = null;
        for (ModuleRepository.ModuleInfo info : modules) {
            if (!info.getName().equals(lastModule)) {
                if (lastModule != null) {
                    System.out.println();
                }
                System.out.println(info.getName());
                lastModule = info.getName();
            }
            System.out.println("  " + info.getVersion() + " - " + 
                info.getMetadata().getOrDefault("description", "No description"));
        }
    }
    
    /**
     * 显示模块信息
     */
    private static void handleInfo(String[] args) throws Exception {
        if (args.length < 1) {
            System.err.println("Usage: cjmod info <module-name[@version]>");
            System.exit(1);
        }
        
        String moduleSpec = args[0];
        String moduleName;
        ModuleVersion.VersionRange versionRange;
        
        // 解析模块名和版本
        int atIndex = moduleSpec.indexOf('@');
        if (atIndex >= 0) {
            moduleName = moduleSpec.substring(0, atIndex);
            versionRange = new ModuleVersion.VersionRange(moduleSpec.substring(atIndex + 1));
        } else {
            moduleName = moduleSpec;
            versionRange = new ModuleVersion.VersionRange("*");
        }
        
        ModuleRepository repository = new ModuleRepository();
        ModuleRepository.ModuleInfo info = repository.findModule(moduleName, versionRange);
        
        if (info == null) {
            System.err.println("Module not found: " + moduleSpec);
            System.exit(1);
        }
        
        System.out.println("Module: " + info.getName());
        System.out.println("Version: " + info.getVersion());
        System.out.println("Path: " + info.getPath());
        System.out.println("Installed: " + new Date(info.getTimestamp()));
        
        if (!info.getMetadata().isEmpty()) {
            System.out.println("\nMetadata:");
            for (Map.Entry<String, String> entry : info.getMetadata().entrySet()) {
                System.out.println("  " + entry.getKey() + ": " + entry.getValue());
            }
        }
        
        if (!info.getDependencies().isEmpty()) {
            System.out.println("\nDependencies:");
            for (ModuleRepository.Dependency dep : info.getDependencies()) {
                System.out.println("  " + dep.getModuleName() + "@" + dep.getVersionRange() +
                    (dep.isOptional() ? " (optional)" : ""));
            }
        }
    }
    
    /**
     * 搜索模块
     */
    private static void handleSearch(String[] args) throws Exception {
        if (args.length < 1) {
            System.err.println("Usage: cjmod search <query>");
            System.exit(1);
        }
        
        String query = args[0].toLowerCase();
        
        ModuleRepository repository = new ModuleRepository();
        List<ModuleRepository.ModuleInfo> modules = repository.getAllModules();
        
        List<ModuleRepository.ModuleInfo> results = modules.stream()
            .filter(info -> info.getName().toLowerCase().contains(query) ||
                          info.getMetadata().getOrDefault("description", "")
                              .toLowerCase().contains(query))
            .distinct()
            .toList();
        
        if (results.isEmpty()) {
            System.out.println("No modules found matching: " + query);
            return;
        }
        
        System.out.println("Found " + results.size() + " module(s):");
        System.out.println();
        
        for (ModuleRepository.ModuleInfo info : results) {
            System.out.println(info.getName() + "@" + info.getVersion());
            String description = info.getMetadata().get("description");
            if (description != null) {
                System.out.println("  " + description);
            }
            System.out.println();
        }
    }
    
    /**
     * 显示版本信息
     */
    private static void handleVersion() {
        System.out.println(TOOL_NAME + " version " + VERSION);
        System.out.println("CHTL Module Manager");
        System.out.println("Copyright (c) 2024 CHTL Project");
    }
    
    /**
     * 打印使用说明
     */
    private static void printUsage() {
        System.out.println("Usage: " + TOOL_NAME + " <command> [options]");
        System.out.println();
        System.out.println("Commands:");
        System.out.println("  create <name>              Create a new module");
        System.out.println("  package <path>             Package a module");
        System.out.println("  sign <path>                Sign a module");
        System.out.println("  verify <path>              Verify module signature");
        System.out.println("  install <module>           Install a module");
        System.out.println("  uninstall <module>         Uninstall a module");
        System.out.println("  list                       List installed modules");
        System.out.println("  info <module>              Show module information");
        System.out.println("  search <query>             Search for modules");
        System.out.println("  version                    Show version information");
        System.out.println("  help                       Show this help message");
        System.out.println();
        System.out.println("Options:");
        System.out.println("  --help, -h                 Show help for a command");
        System.out.println("  --version, -v              Show version information");
        System.out.println();
        System.out.println("Examples:");
        System.out.println("  " + TOOL_NAME + " create mymodule --author \"John Doe\"");
        System.out.println("  " + TOOL_NAME + " package mymodule -o dist/");
        System.out.println("  " + TOOL_NAME + " install mymodule-1.0.0.cjmod");
        System.out.println("  " + TOOL_NAME + " list");
    }
    
    /**
     * 格式化文件大小
     */
    private static String formatSize(long size) {
        if (size < 1024) {
            return size + " B";
        } else if (size < 1024 * 1024) {
            return String.format("%.1f KB", size / 1024.0);
        } else if (size < 1024 * 1024 * 1024) {
            return String.format("%.1f MB", size / (1024.0 * 1024));
        } else {
            return String.format("%.1f GB", size / (1024.0 * 1024 * 1024));
        }
    }
}

package com.chtl.compiler.importer;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.chtl.ast.CHTLASTNode;
import com.chtl.ast.node.ImportStatementNode;
import com.chtl.ast.node.OriginBlockNode;
import com.chtl.ast.node.RootNode;
import com.chtl.compiler.chtl.CHTLParser;
import com.chtl.compiler.importer.ImportManager.ImportNode;
import com.chtl.compiler.importer.ImportManager.ImportResult;


import com.chtl.ast.*;
import com.chtl.ast.node.*;
import com.chtl.context.CompilationContext;
import com.chtl.context.CompilationError;

import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.stream.Collectors;

/**
 * 导入管理器
 * 处理CHTL的各种导入语法
 * 严格按照CHTL语法文档实现
 */
public class ImportManager {
    private final CompilationContext context;
    private final Path currentDirectory;
    private final Path officialModuleDirectory;
    private final Set<String> importedPaths; // 防止重复导入
    private final Map<String, ImportNode> importCache; // 导入缓存
    private final Stack<String> importStack; // 检测循环依赖
    
    // 文件扩展名映射
    private static final Map<ImportStatementNode.ImportType, List<String>> EXTENSION_MAP = Map.of(
        ImportStatementNode.ImportType.HTML, Arrays.asList(".html", ".htm"),
        ImportStatementNode.ImportType.STYLE, Arrays.asList(".css"),
        ImportStatementNode.ImportType.JAVASCRIPT, Arrays.asList(".js"),
        ImportStatementNode.ImportType.CHTL, Arrays.asList(".cmod", ".chtl"),
        ImportStatementNode.ImportType.CJMOD, Arrays.asList(".cjmod")
    );
    
    public ImportManager(CompilationContext context, Path currentDirectory) {
        this.context = context;
        this.currentDirectory = currentDirectory;
        this.officialModuleDirectory = getOfficialModuleDirectory();
        this.importedPaths = new HashSet<>();
        this.importCache = new HashMap<>();
        this.importStack = new Stack<>();
    }
    
    /**
     * 处理导入语句
     */
    public ImportResult processImport(ImportStatementNode importNode) {
        String fromPath = importNode.getFromPath();
        ImportStatementNode.ImportType type = importNode.getImportType();
        
        // 检查是否使用官方模块前缀
        boolean isOfficialModule = false;
        if (fromPath.startsWith("chtl::")) {
            isOfficialModule = true;
            fromPath = fromPath.substring(6); // 移除 "chtl::" 前缀
        }
        
        // 标准化路径表达
        fromPath = normalizePath(fromPath);
        
        // 更新importNode中的fromPath（移除前缀后的）
        // 这样后续方法使用的都是标准化后的路径
        importNode.setFromPath(fromPath);
        
        // 检查重复导入
        String importKey = generateImportKey(type, fromPath);
        if (importedPaths.contains(importKey)) {
            return new ImportResult(true, "Already imported: " + fromPath);
        }
        
        // 检查循环依赖
        if (importStack.contains(fromPath)) {
            context.addError(new CompilationError(
                "Circular dependency detected: " + String.join(" -> ", importStack) + " -> " + fromPath,
                CompilationError.ErrorType.IMPORT_ERROR
            ));
            return new ImportResult(false, "Circular dependency");
        }
        
        // 处理不同类型的导入
        ImportResult result;
        importStack.push(fromPath);
        try {
            switch (type) {
                case HTML:
                case STYLE:
                case JAVASCRIPT:
                    result = processResourceImport(importNode, isOfficialModule);
                    break;
                case CHTL:
                    result = processCHTLImport(importNode, isOfficialModule);
                    break;
                case CJMOD:
                    result = processCJModImport(importNode, isOfficialModule);
                    break;
                default:
                    result = processOtherImport(importNode);
            }
            
            if (result.isSuccess()) {
                importedPaths.add(importKey);
            }
            
            return result;
        } finally {
            importStack.pop();
        }
    }
    
    /**
     * 处理资源导入（HTML, Style, JavaScript）
     */
    private ImportResult processResourceImport(ImportStatementNode importNode, boolean isOfficialModule) {
        String fromPath = importNode.getFromPath();
        String asName = importNode.getAsName();
        ImportStatementNode.ImportType type = importNode.getImportType();
        
        // 资源导入不支持官方模块前缀
        if (isOfficialModule) {
            context.addError(new CompilationError(
                "Official module prefix 'chtl::' is not supported for resource imports",
                CompilationError.ErrorType.IMPORT_ERROR
            ));
            return new ImportResult(false, "Invalid prefix");
        }
        
        // 如果没有as语法，直接跳过
        if (asName == null) {
            return new ImportResult(true, "Skipped resource import without 'as' clause");
        }
        
        // 查找文件
        List<Path> foundFiles = findResourceFiles(fromPath, type);
        
        if (foundFiles.isEmpty()) {
            context.addError(new CompilationError(
                "Resource file not found: " + fromPath,
                CompilationError.ErrorType.IMPORT_ERROR
            ));
            return new ImportResult(false, "File not found");
        }
        
        if (foundFiles.size() > 1) {
            context.addError(new CompilationError(
                "Multiple files found for: " + fromPath + ". Please specify exact filename.",
                CompilationError.ErrorType.IMPORT_ERROR
            ));
            return new ImportResult(false, "Ambiguous import");
        }
        
        // 读取文件内容
        Path file = foundFiles.get(0);
        try {
            String content = Files.readString(file);
            
            // 创建带名原始嵌入节点
            OriginBlockNode.OriginType originType = mapToOriginType(type);
            OriginBlockNode originNode = new OriginBlockNode(originType, asName);
            originNode.setContent(content);
            
            // 缓存导入结果
            ImportNode importResult = new ImportNode(importNode, originNode);
            importCache.put(generateImportKey(type, fromPath), importResult);
            
            return new ImportResult(true, originNode);
            
        } catch (IOException e) {
            context.addError(new CompilationError(
                "Failed to read file: " + file + " - " + e.getMessage(),
                CompilationError.ErrorType.IMPORT_ERROR
            ));
            return new ImportResult(false, "Read error");
        }
    }
    
    /**
     * 处理CHTL导入
     */
    private ImportResult processCHTLImport(ImportStatementNode importNode, boolean isOfficialModule) {
        String fromPath = importNode.getFromPath();
        
        // 检查是否是通配符导入
        if (fromPath.equals("*") || fromPath.endsWith(".*") || fromPath.endsWith("/*")) {
            return processWildcardImport(importNode, isOfficialModule);
        }
        
        // 查找CHTL文件
        List<Path> foundFiles = isOfficialModule ? 
            findOfficialCHTLFiles(fromPath) : findCHTLFiles(fromPath);
        
        if (foundFiles.isEmpty()) {
            context.addError(new CompilationError(
                "CHTL module not found: " + fromPath,
                CompilationError.ErrorType.IMPORT_ERROR
            ));
            return new ImportResult(false, "Module not found");
        }
        
        // 导入找到的文件
        List<CHTLASTNode> importedNodes = new ArrayList<>();
        for (Path file : foundFiles) {
            ImportResult fileResult = importCHTLFile(file, importNode);
            if (fileResult.isSuccess() && fileResult.getNode() != null) {
                importedNodes.add(fileResult.getNode());
            }
        }
        
        return new ImportResult(true, importedNodes);
    }
    
    /**
     * 处理CJMod导入
     */
    private ImportResult processCJModImport(ImportStatementNode importNode, boolean isOfficialModule) {
        String fromPath = importNode.getFromPath();
        
        // 查找CJMod文件
        List<Path> foundFiles = isOfficialModule ? 
            findOfficialCJModFiles(fromPath) : findCJModFiles(fromPath);
        
        if (foundFiles.isEmpty()) {
            context.addError(new CompilationError(
                "CJMod module not found: " + fromPath,
                CompilationError.ErrorType.IMPORT_ERROR
            ));
            return new ImportResult(false, "Module not found");
        }
        
        // 导入找到的文件
        List<CHTLASTNode> importedNodes = new ArrayList<>();
        for (Path file : foundFiles) {
            ImportResult fileResult = importCJModFile(file, importNode);
            if (fileResult.isSuccess() && fileResult.getNode() != null) {
                importedNodes.add(fileResult.getNode());
            }
        }
        
        return new ImportResult(true, importedNodes);
    }
    
    /**
     * 处理通配符导入
     */
    private ImportResult processWildcardImport(ImportStatementNode importNode, boolean isOfficialModule) {
        String fromPath = importNode.getFromPath();
        
        // 解析通配符模式
        String basePath;
        String pattern;
        
        if (fromPath.equals("*")) {
            // 处理单独的 * (来自 chtl::*)
            basePath = "";
            pattern = "*";
        } else if (fromPath.contains("/*")) {
            // 处理 path/* 格式
            basePath = fromPath.substring(0, fromPath.indexOf("/*"));
            pattern = "*";
        } else if (fromPath.contains("/*.")) {
            // 处理 path/*.ext 格式
            int slashIdx = fromPath.indexOf("/*.");
            basePath = fromPath.substring(0, slashIdx);
            pattern = fromPath.substring(slashIdx + 1); // 包含 *
        } else if (fromPath.contains(".*")) {
            // 处理 path.* 格式
            int dotIdx = fromPath.lastIndexOf(".*");
            basePath = fromPath.substring(0, dotIdx);
            pattern = fromPath.substring(dotIdx + 1);
        } else {
            // 不是通配符格式
            return new ImportResult(false, "Invalid wildcard pattern: " + fromPath);
        }
        
        Path targetDir;
        if (isOfficialModule) {
            // 官方模块只在官方目录查找
            targetDir = basePath.isEmpty() ? officialModuleDirectory : 
                                           officialModuleDirectory.resolve(basePath);
        } else {
            targetDir = basePath.isEmpty() ? currentDirectory : resolvePath(basePath);
        }
        
        if (!Files.isDirectory(targetDir)) {
            context.addError(new CompilationError(
                "Directory not found for wildcard import: " + basePath,
                CompilationError.ErrorType.IMPORT_ERROR
            ));
            return new ImportResult(false, "Directory not found");
        }
        
        // 查找匹配的文件
        List<Path> matchedFiles = new ArrayList<>();
        try {
            if (pattern.equals("*")) {
                // 导入所有cmod和chtl文件
                matchedFiles.addAll(Files.list(targetDir)
                    .filter(path -> path.toString().endsWith(".cmod") || 
                                   path.toString().endsWith(".chtl"))
                    .collect(Collectors.toList()));
            } else if (pattern.equals("*.cmod")) {
                // 只导入cmod文件
                matchedFiles.addAll(Files.list(targetDir)
                    .filter(path -> path.toString().endsWith(".cmod"))
                    .collect(Collectors.toList()));
            } else if (pattern.equals("*.chtl")) {
                // 只导入chtl文件
                matchedFiles.addAll(Files.list(targetDir)
                    .filter(path -> path.toString().endsWith(".chtl"))
                    .collect(Collectors.toList()));
            }
        } catch (IOException e) {
            context.addError(new CompilationError(
                "Failed to list directory: " + targetDir,
                CompilationError.ErrorType.IMPORT_ERROR
            ));
            return new ImportResult(false, "IO error");
        }
        
        // 导入所有匹配的文件
        List<CHTLASTNode> importedNodes = new ArrayList<>();
        for (Path file : matchedFiles) {
            ImportResult fileResult = importCHTLFile(file, importNode);
            if (fileResult.isSuccess() && fileResult.getNode() != null) {
                importedNodes.add(fileResult.getNode());
            }
        }
        
        return new ImportResult(true, importedNodes);
    }
    
    /**
     * 查找资源文件
     */
    private List<Path> findResourceFiles(String fromPath, ImportStatementNode.ImportType type) {
        List<Path> foundFiles = new ArrayList<>();
        List<String> extensions = EXTENSION_MAP.get(type);
        
        // 检查是否带后缀名
        boolean hasExtension = extensions.stream().anyMatch(fromPath::endsWith);
        
        if (hasExtension) {
            // 带后缀名，直接查找
            Path file = currentDirectory.resolve(fromPath);
            if (Files.exists(file) && Files.isRegularFile(file)) {
                foundFiles.add(file);
            }
        } else {
            // 不带后缀名，尝试所有相关扩展名
            for (String ext : extensions) {
                Path file = currentDirectory.resolve(fromPath + ext);
                if (Files.exists(file) && Files.isRegularFile(file)) {
                    foundFiles.add(file);
                }
            }
        }
        
        return foundFiles;
    }
    
    /**
     * 查找CHTL文件
     */
    private List<Path> findCHTLFiles(String fromPath) {
        List<Path> foundFiles = new ArrayList<>();
        
        // 检查是否是具体路径
        if (fromPath.contains("/") || fromPath.contains("\\")) {
            Path file = Paths.get(fromPath);
            if (Files.exists(file) && Files.isRegularFile(file)) {
                foundFiles.add(file);
            }
            return foundFiles;
        }
        
        // 检查是否带后缀名
        boolean hasExtension = fromPath.endsWith(".cmod") || fromPath.endsWith(".chtl");
        
        if (hasExtension) {
            // 带后缀名，按优先级查找
            foundFiles.addAll(searchInOfficialModules(fromPath));
            if (foundFiles.isEmpty()) {
                foundFiles.addAll(searchInLocalModules(fromPath));
            }
            if (foundFiles.isEmpty()) {
                foundFiles.addAll(searchInCurrentDirectory(fromPath));
            }
        } else {
            // 不带后缀名，cmod优先
            foundFiles.addAll(searchInOfficialModules(fromPath + ".cmod"));
            if (foundFiles.isEmpty()) {
                foundFiles.addAll(searchInOfficialModules(fromPath + ".chtl"));
            }
            if (foundFiles.isEmpty()) {
                foundFiles.addAll(searchInLocalModules(fromPath + ".cmod"));
            }
            if (foundFiles.isEmpty()) {
                foundFiles.addAll(searchInLocalModules(fromPath + ".chtl"));
            }
            if (foundFiles.isEmpty()) {
                foundFiles.addAll(searchInCurrentDirectory(fromPath + ".cmod"));
            }
            if (foundFiles.isEmpty()) {
                foundFiles.addAll(searchInCurrentDirectory(fromPath + ".chtl"));
            }
        }
        
        return foundFiles;
    }
    
    /**
     * 查找CJMod文件
     */
    private List<Path> findCJModFiles(String fromPath) {
        List<Path> foundFiles = new ArrayList<>();
        
        // 检查是否是具体路径
        if (fromPath.contains("/") || fromPath.contains("\\")) {
            Path file = Paths.get(fromPath);
            if (Files.exists(file) && Files.isRegularFile(file)) {
                foundFiles.add(file);
            }
            return foundFiles;
        }
        
        // 检查是否带后缀名
        String searchName = fromPath.endsWith(".cjmod") ? fromPath : fromPath + ".cjmod";
        
        // 按优先级查找
        foundFiles.addAll(searchInOfficialModules(searchName));
        if (foundFiles.isEmpty()) {
            foundFiles.addAll(searchInLocalModules(searchName));
        }
        if (foundFiles.isEmpty()) {
            foundFiles.addAll(searchInCurrentDirectory(searchName));
        }
        
        return foundFiles;
    }
    
    /**
     * 在官方模块目录中搜索
     */
    private List<Path> searchInOfficialModules(String filename) {
        if (officialModuleDirectory == null || !Files.exists(officialModuleDirectory)) {
            return Collections.emptyList();
        }
        
        Path file = officialModuleDirectory.resolve(filename);
        if (Files.exists(file) && Files.isRegularFile(file)) {
            return Arrays.asList(file);
        }
        
        return Collections.emptyList();
    }
    
    /**
     * 在本地模块目录中搜索
     */
    private List<Path> searchInLocalModules(String filename) {
        Path localModuleDir = currentDirectory.resolve("module");
        if (!Files.exists(localModuleDir) || !Files.isDirectory(localModuleDir)) {
            return Collections.emptyList();
        }
        
        Path file = localModuleDir.resolve(filename);
        if (Files.exists(file) && Files.isRegularFile(file)) {
            return Arrays.asList(file);
        }
        
        return Collections.emptyList();
    }
    
    /**
     * 在当前目录中搜索
     */
    private List<Path> searchInCurrentDirectory(String filename) {
        Path file = currentDirectory.resolve(filename);
        if (Files.exists(file) && Files.isRegularFile(file)) {
            return Arrays.asList(file);
        }
        
        return Collections.emptyList();
    }
    
    /**
     * 查找官方CHTL模块文件
     * 只在官方模块目录中查找
     */
    private List<Path> findOfficialCHTLFiles(String fromPath) {
        List<Path> foundFiles = new ArrayList<>();
        
        // 检查是否带后缀名
        boolean hasExtension = fromPath.endsWith(".cmod") || fromPath.endsWith(".chtl");
        
        if (hasExtension) {
            // 带后缀名，直接在官方模块目录查找
            foundFiles.addAll(searchInOfficialModules(fromPath));
        } else {
            // 不带后缀名，优先查找.cmod文件
            foundFiles.addAll(searchInOfficialModules(fromPath + ".cmod"));
            if (foundFiles.isEmpty()) {
                foundFiles.addAll(searchInOfficialModules(fromPath + ".chtl"));
            }
        }
        
        return foundFiles;
    }
    
    /**
     * 查找官方CJMod模块文件
     * 只在官方模块目录中查找
     */
    private List<Path> findOfficialCJModFiles(String fromPath) {
        List<Path> foundFiles = new ArrayList<>();
        
        // 检查是否带后缀名
        boolean hasExtension = fromPath.endsWith(".cjmod");
        
        if (hasExtension) {
            // 带后缀名，直接在官方模块目录查找
            foundFiles.addAll(searchInOfficialModules(fromPath));
        } else {
            // 不带后缀名，添加.cjmod后缀
            foundFiles.addAll(searchInOfficialModules(fromPath + ".cjmod"));
        }
        
        return foundFiles;
    }
    
    /**
     * 导入CHTL文件
     */
    private ImportResult importCHTLFile(Path file, ImportStatementNode importNode) {
        try {
            String content = Files.readString(file);
            
            // 解析CHTL文件
            com.chtl.parser.CHTLParser parser = new com.chtl.parser.CHTLParser();
            RootNode ast = parser.parse(content);
            
            if (!parser.getErrors().isEmpty()) {
                context.addError(new CompilationError(
                    "Parse errors in imported file: " + file,
                    CompilationError.ErrorType.IMPORT_ERROR
                ));
                return new ImportResult(false, "Parse error");
            }
            
            return new ImportResult(true, ast);
            
        } catch (IOException e) {
            context.addError(new CompilationError(
                "Failed to read CHTL file: " + file,
                CompilationError.ErrorType.IMPORT_ERROR
            ));
            return new ImportResult(false, "Read error");
        }
    }
    
    /**
     * 导入CJMod文件
     */
    private ImportResult importCJModFile(Path file, ImportStatementNode importNode) {
        // CJMod是编译后的模块，需要特殊处理
        // 这里简化处理，实际需要反序列化CJMod格式
        try {
            // TODO: 实现CJMod文件的反序列化
            return new ImportResult(true, "CJMod import placeholder");
            
        } catch (Exception e) {
            context.addError(new CompilationError(
                "Failed to load CJMod file: " + file,
                CompilationError.ErrorType.IMPORT_ERROR
            ));
            return new ImportResult(false, "Load error");
        }
    }
    
    /**
     * 处理其他类型的导入
     */
    private ImportResult processOtherImport(ImportStatementNode importNode) {
        // 处理模板和自定义的导入
        ImportStatementNode.ImportType type = importNode.getImportType();
        String targetName = importNode.getTargetName();
        String fromPath = importNode.getFromPath();
        
        // 查找并导入指定的定义
        // TODO: 实现具体的模板/自定义导入逻辑
        
        return new ImportResult(true, "Other import placeholder");
    }
    
    /**
     * 标准化路径表达
     */
    private String normalizePath(String path) {
        // 将 . 替换为 /
        path = path.replace('.', '/');
        
        // 处理 .* 后缀
        if (path.endsWith("/*")) {
            path = path.substring(0, path.length() - 2) + ".*";
        }
        
        return path;
    }
    
    /**
     * 解析路径
     */
    private Path resolvePath(String path) {
        // 处理相对路径和绝对路径
        Path resolved = Paths.get(path);
        if (resolved.isAbsolute()) {
            return resolved;
        }
        
        return currentDirectory.resolve(path);
    }
    
    /**
     * 生成导入键
     */
    private String generateImportKey(ImportStatementNode.ImportType type, String path) {
        return type.name() + ":" + normalizePath(path);
    }
    
    /**
     * 映射到原始嵌入类型
     */
    private OriginBlockNode.OriginType mapToOriginType(ImportStatementNode.ImportType type) {
        switch (type) {
            case HTML:
                return OriginBlockNode.OriginType.HTML;
            case STYLE:
                return OriginBlockNode.OriginType.STYLE;
            case JAVASCRIPT:
                return OriginBlockNode.OriginType.JAVASCRIPT;
            default:
                throw new IllegalArgumentException("Cannot map import type to origin type: " + type);
        }
    }
    
    /**
     * 获取官方模块目录
     */
    private Path getOfficialModuleDirectory() {
        // 假设编译器在固定位置
        Path compilerPath = Paths.get(System.getProperty("chtl.compiler.home", "."));
        return compilerPath.resolve("module");
    }
    
    /**
     * 导入结果
     */
    public static class ImportResult {
        private final boolean success;
        private final Object result;
        
        public ImportResult(boolean success, Object result) {
            this.success = success;
            this.result = result;
        }
        
        public boolean isSuccess() {
            return success;
        }
        
        public CHTLASTNode getNode() {
            if (result instanceof CHTLASTNode) {
                return (CHTLASTNode) result;
            }
            return null;
        }
        
        @SuppressWarnings("unchecked")
        public List<CHTLASTNode> getNodes() {
            if (result instanceof List) {
                return (List<CHTLASTNode>) result;
            }
            return Collections.emptyList();
        }
        
        public String getMessage() {
            if (result instanceof String) {
                return (String) result;
            }
            return "";
        }
    }
    
    /**
     * 导入节点包装
     */
    private static class ImportNode {
        private final ImportStatementNode importStatement;
        private final CHTLASTNode importedNode;
        
        public ImportNode(ImportStatementNode importStatement, CHTLASTNode importedNode) {
            this.importStatement = importStatement;
            this.importedNode = importedNode;
        }
        
        public ImportStatementNode getImportStatement() {
            return importStatement;
        }
        
        public CHTLASTNode getImportedNode() {
            return importedNode;
        }
    }
}
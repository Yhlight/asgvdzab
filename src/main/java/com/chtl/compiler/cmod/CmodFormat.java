package com.chtl.compiler.cmod;

/**
 * Cmod文件格式定义
 * 严格按照CHTL语法文档实现
 */
public class CmodFormat {
    
    // Cmod文件扩展名
    public static final String CMOD_EXTENSION = ".cmod";
    public static final String CJMOD_EXTENSION = ".cjmod";
    
    // Cmod包结构
    public static final String MANIFEST_FILE = "cmod.manifest";
    public static final String META_DIR = "META-INF";
    public static final String SOURCE_DIR = "src";
    public static final String COMPILED_DIR = "compiled";
    public static final String RESOURCES_DIR = "resources";
    
    // Manifest字段
    public static final String MANIFEST_VERSION = "Cmod-Version";
    public static final String MANIFEST_NAME = "Module-Name";
    public static final String MANIFEST_NAMESPACE = "Module-Namespace";
    public static final String MANIFEST_AUTHOR = "Module-Author";
    public static final String MANIFEST_DESCRIPTION = "Module-Description";
    public static final String MANIFEST_DEPENDENCIES = "Module-Dependencies";
    public static final String MANIFEST_EXPORTS = "Module-Exports";
    public static final String MANIFEST_CREATED = "Created-Time";
    public static final String MANIFEST_COMPILER_VERSION = "Compiler-Version";
    
    // 版本信息
    public static final String CMOD_VERSION = "1.0";
    public static final String COMPILER_VERSION = "1.0.0";
    
    /**
     * Cmod包结构：
     * module.cmod/
     * ├── META-INF/
     * │   └── cmod.manifest
     * ├── src/
     * │   └── *.chtl (原始CHTL源文件)
     * ├── compiled/
     * │   ├── ast.dat (序列化的AST)
     * │   ├── symbols.dat (符号表)
     * │   └── metadata.dat (元数据)
     * └── resources/
     *     └── (相关资源文件)
     */
    
    /**
     * CJmod包结构（仅包含CHTL JS模块）：
     * module.cjmod/
     * ├── META-INF/
     * │   └── cmod.manifest
     * ├── src/
     * │   └── *.chtljs (原始CHTL JS源文件)
     * └── compiled/
     *     ├── ast.dat (序列化的AST)
     *     └── metadata.dat (元数据)
     */
}
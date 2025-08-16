package com.chtl.cjmod;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.io.*;

/**
 * 模块打包工具
 * 将CJMOD模块打包成标准格式
 */
public class ModulePackager {
    
    // 打包选项
    public static class PackageOptions {
        private boolean includeSource = true;
        private boolean includeDocs = true;
        private boolean compress = true;
        private boolean sign = false;
        private String signingKey = null;
        private Set<String> excludePatterns = new HashSet<Object>();
        private Map<String, String> metadata = new HashMap<Object, Object>();
        
        // Builder pattern
        public PackageOptions withSource(boolean include) {
            this.includeSource = include;
            return this;
        }
        
        public PackageOptions withDocs(boolean include) {
            this.includeDocs = include;
            return this;
        }
        
        public PackageOptions withCompression(boolean compress) {
            this.compress = compress;
            return this;
        }
        
        public PackageOptions withSigning(String key) {
            this.sign = true;
            this.signingKey = key;
            return this;
        }
        
        public PackageOptions excludePattern(String pattern) {
            this.excludePatterns.add(pattern);
            return this;
        }
        
        public PackageOptions addMetadata(String key, String value) {
            this.metadata.put(key, value);
            return this;
        }
    }
    
    /**
     * 打包结果
     */
    public static class PackageResult {
        private final Path packagePath;
        private final long size;
        private final String checksum;
        private final Map<String, Object> metadata;
        
        public PackageResult(Path packagePath, long size, String checksum) {
            this.packagePath = packagePath;
            this.size = size;
            this.checksum = checksum;
            this.metadata = new HashMap<Object, Object>();
        }
        
        // Getters
        public Path getPackagePath() { return packagePath; }
        public long getSize() { return size; }
        public String getChecksum() { return checksum; }
        public Map<String, Object> getMetadata() { return metadata; }
    }
    
    private static final String MODULE_DESCRIPTOR = "module.json";
    private static final String MANIFEST_FILE = "META-INF/MANIFEST.MF";
    private static final String SIGNATURE_FILE = "META-INF/SIGNATURE";
    private static final String CHECKSUM_FILE = "META-INF/CHECKSUM.SHA256";
    
    /**
     * 打包模块
     */
    public PackageResult packageModule(Path modulePath, Path outputPath, PackageOptions options) 
            throws Exception {
        
        // 验证模块路径
        if (!Files.exists(modulePath) || !Files.isDirectory(modulePath)) {
            throw new IllegalArgumentException("Invalid module path: " + modulePath);
        }
        
        // 读取模块描述符
        DirectoryModuleLoader.ModuleDescriptor descriptor = 
            readModuleDescriptor(modulePath.resolve(MODULE_DESCRIPTOR));
        
        // 创建输出文件名
        String fileName = descriptor.name + "-" + descriptor.version + ".cjmod";
        Path packagePath = outputPath.resolve(fileName);
        
        // 确保输出目录存在
        Files.createDirectories(outputPath);
        
        // 创建打包文件
        if (packagePath.toString().endsWith(".jar") || packagePath.toString().endsWith(".cjmod")) {
            return packageAsJar(modulePath, packagePath, descriptor, options);
        } else {
            return packageAsZip(modulePath, packagePath, descriptor, options);
        }
    }
    
    /**
     * 打包为JAR格式
     */
    private PackageResult packageAsJar(Path modulePath, Path outputPath, 
                                     DirectoryModuleLoader.ModuleDescriptor descriptor,
                                     PackageOptions options) throws Exception {
        
        try (JarOutputStream jos = new JarOutputStream(
                new FileOutputStream(outputPath.toFile()))) {
            
            // 设置压缩级别
            if (options.compress) {
                jos.setLevel(Deflater.BEST_COMPRESSION);
            } else {
                jos.setLevel(Deflater.NO_COMPRESSION);
            }
            
            // 创建清单文件
            jos.putNextEntry(new JarEntry("META-INF/"));
            jos.closeEntry();
            
            jos.putNextEntry(new JarEntry(MANIFEST_FILE));
            writeManifest(jos, descriptor, options);
            jos.closeEntry();
            
            // 添加模块文件
            MessageDigest digest = MessageDigest.getInstance("SHA-256");
            addModuleFiles(modulePath, modulePath, jos, options, digest);
            
            // 计算校验和
            String checksum = bytesToHex(digest.digest());
            
            // 添加校验和文件
            jos.putNextEntry(new JarEntry(CHECKSUM_FILE));
            jos.write(checksum.getBytes());
            jos.closeEntry();
            
            // 签名（如果需要）
            if (options.sign && options.signingKey != null) {
                signPackage(jos, checksum, options.signingKey);
            }
            
            jos.finish();
            
            // 获取文件大小
            long size = Files.size(outputPath);
            
            return new PackageResult(outputPath, size, checksum);
        }
    }
    
    /**
     * 打包为ZIP格式
     */
    private PackageResult packageAsZip(Path modulePath, Path outputPath,
                                     DirectoryModuleLoader.ModuleDescriptor descriptor,
                                     PackageOptions options) throws Exception {
        
        try (ZipOutputStream zos = new ZipOutputStream(
                new FileOutputStream(outputPath.toFile()))) {
            
            // 设置压缩级别
            if (options.compress) {
                zos.setLevel(Deflater.BEST_COMPRESSION);
            } else {
                zos.setLevel(Deflater.NO_COMPRESSION);
            }
            
            // 添加模块描述符
            zos.putNextEntry(new ZipEntry(MODULE_DESCRIPTOR));
            Files.copy(modulePath.resolve(MODULE_DESCRIPTOR), zos);
            zos.closeEntry();
            
            // 添加模块文件
            MessageDigest digest = MessageDigest.getInstance("SHA-256");
            addModuleFilesZip(modulePath, modulePath, zos, options, digest);
            
            // 计算校验和
            String checksum = bytesToHex(digest.digest());
            
            // 添加元数据
            zos.putNextEntry(new ZipEntry("META-INF/"));
            zos.closeEntry();
            
            zos.putNextEntry(new ZipEntry("META-INF/package.info"));
            writePackageInfo(zos, descriptor, options, checksum);
            zos.closeEntry();
            
            zos.finish();
            
            // 获取文件大小
            long size = Files.size(outputPath);
            
            return new PackageResult(outputPath, size, checksum);
        }
    }
    
    /**
     * 添加模块文件到JAR
     */
    private void addModuleFiles(Path rootPath, Path currentPath, JarOutputStream jos,
                               PackageOptions options, MessageDigest digest) throws IOException {
        
        try (DirectoryStream<Path> stream = Files.newDirectoryStream(currentPath)) {
            for (Path entry : stream) {
                Path relativePath = rootPath.relativize(entry);
                String entryName = relativePath.toString().replace(File.separatorChar, '/');
                
                // 检查排除模式
                if (shouldExclude(entryName, options)) {
                    continue;
                }
                
                if (Files.isDirectory(entry)) {
                    // 添加目录条目
                    if (!entryName.endsWith("/")) {
                        entryName += "/";
                    }
                    jos.putNextEntry(new JarEntry(entryName));
                    jos.closeEntry();
                    
                    // 递归处理子目录
                    addModuleFiles(rootPath, entry, jos, options, digest);
                } else {
                    // 添加文件
                    jos.putNextEntry(new JarEntry(entryName));
                    
                    // 读取并写入文件内容
                    byte[] buffer = new byte[8192];
                    try (InputStream is = Files.newInputStream(entry)) {
                        int len;
                        while ((len = is.read(buffer)) > 0) {
                            jos.write(buffer, 0, len);
                            digest.update(buffer, 0, len);
                        }
                    }
                    
                    jos.closeEntry();
                }
            }
        }
    }
    
    /**
     * 添加模块文件到ZIP
     */
    private void addModuleFilesZip(Path rootPath, Path currentPath, ZipOutputStream zos,
                                  PackageOptions options, MessageDigest digest) throws IOException {
        
        try (DirectoryStream<Path> stream = Files.newDirectoryStream(currentPath)) {
            for (Path entry : stream) {
                Path relativePath = rootPath.relativize(entry);
                String entryName = relativePath.toString().replace(File.separatorChar, '/');
                
                // 跳过模块描述符（已经添加）
                if (entryName.equals(MODULE_DESCRIPTOR)) {
                    continue;
                }
                
                // 检查排除模式
                if (shouldExclude(entryName, options)) {
                    continue;
                }
                
                if (Files.isDirectory(entry)) {
                    // 递归处理子目录
                    addModuleFilesZip(rootPath, entry, zos, options, digest);
                } else {
                    // 添加文件
                    zos.putNextEntry(new ZipEntry(entryName));
                    
                    // 读取并写入文件内容
                    byte[] buffer = new byte[8192];
                    try (InputStream is = Files.newInputStream(entry)) {
                        int len;
                        while ((len = is.read(buffer)) > 0) {
                            zos.write(buffer, 0, len);
                            digest.update(buffer, 0, len);
                        }
                    }
                    
                    zos.closeEntry();
                }
            }
        }
    }
    
    /**
     * 检查是否应该排除文件
     */
    private boolean shouldExclude(String path, PackageOptions options) {
        // 检查是否排除源代码
        if (!options.includeSource && path.startsWith("src/")) {
            return true;
        }
        
        // 检查是否排除文档
        if (!options.includeDocs && (path.startsWith("docs/") || path.endsWith(".md"))) {
            return true;
        }
        
        // 检查自定义排除模式
        for (String pattern : options.excludePatterns) {
            if (path.matches(pattern)) {
                return true;
            }
        }
        
        // 排除临时文件和构建输出
        return path.startsWith("build/") || 
               path.startsWith("target/") ||
               path.startsWith(".") ||
               path.endsWith("~") ||
               path.endsWith(".tmp");
    }
    
    /**
     * 写入清单文件
     */
    private void writeManifest(OutputStream os, DirectoryModuleLoader.ModuleDescriptor descriptor,
                              PackageOptions options) throws IOException {
        Manifest manifest = new Manifest();
        Attributes attrs = manifest.getMainAttributes();
        
        attrs.put(Attributes.Name.MANIFEST_VERSION, "1.0");
        attrs.putValue("Module-Name", descriptor.name);
        attrs.putValue("Module-Version", descriptor.version);
        attrs.putValue("Module-Main-Class", descriptor.mainClass);
        attrs.putValue("Module-Author", descriptor.author != null ? descriptor.author : "Unknown");
        attrs.putValue("Module-License", descriptor.license != null ? descriptor.license : "Unknown");
        attrs.putValue("Package-Time", new Date().toString());
        attrs.putValue("Package-Tool", "CHTL Module Packager 1.0");
        
        // 添加自定义元数据
        for (Map.Entry<String, String> entry : options.metadata.entrySet()) {
            attrs.putValue("X-" + entry.getKey(), entry.getValue());
        }
        
        manifest.write(os);
    }
    
    /**
     * 写入打包信息
     */
    private void writePackageInfo(OutputStream os, DirectoryModuleLoader.ModuleDescriptor descriptor,
                                PackageOptions options, String checksum) throws IOException {
        Properties props = new Properties();
        
        props.setProperty("module.name", descriptor.name);
        props.setProperty("module.version", descriptor.version);
        props.setProperty("module.mainClass", descriptor.mainClass);
        props.setProperty("package.time", new Date().toString());
        props.setProperty("package.checksum", checksum);
        props.setProperty("package.compressed", String.valueOf(options.compress));
        props.setProperty("package.includesSource", String.valueOf(options.includeSource));
        props.setProperty("package.includesDocs", String.valueOf(options.includeDocs));
        
        // 添加自定义元数据
        for (Map.Entry<String, String> entry : options.metadata.entrySet()) {
            props.setProperty("custom." + entry.getKey(), entry.getValue());
        }
        
        props.store(os, "CJMOD Package Information");
    }
    
    /**
     * 签名打包文件
     */
    private void signPackage(JarOutputStream jos, String checksum, String signingKey) 
            throws Exception {
        // TODO: 实现数字签名
        // 这里简单地添加一个签名文件
        jos.putNextEntry(new JarEntry(SIGNATURE_FILE));
        String signature = "SIGNED:" + checksum + ":" + signingKey;
        jos.write(signature.getBytes());
        jos.closeEntry();
    }
    
    /**
     * 读取模块描述符
     */
    private DirectoryModuleLoader.ModuleDescriptor readModuleDescriptor(Path descriptorPath) 
            throws Exception {
        String content = Files.readString(descriptorPath);
        return parseModuleDescriptor(content);
    }
    
    /**
     * 解析模块描述符
     */
    private DirectoryModuleLoader.ModuleDescriptor parseModuleDescriptor(String json) {
        // 复用DirectoryModuleLoader的解析逻辑
        DirectoryModuleLoader loader = new DirectoryModuleLoader();
        try {
            java.lang.reflect.Method method = DirectoryModuleLoader.class
                .getDeclaredMethod("parseModuleDescriptor", String.class);
            method.setAccessible(true);
            return (DirectoryModuleLoader.ModuleDescriptor) method.invoke(loader, json);
        } catch (Exception e) {
            throw new RuntimeException("Failed to parse module descriptor", e);
        }
    }
    
    /**
     * 字节数组转十六进制字符串
     */
    private String bytesToHex(byte[] bytes) {
        StringBuilder result = new StringBuilder();
        for (byte b : bytes) {
            result.append(String.format("%02x", b));
        }
        return result.toString();
    }
    
    /**
     * 解包模块
     */
    public void unpackModule(Path packagePath, Path outputPath) throws Exception {
        if (!Files.exists(packagePath)) {
            throw new IllegalArgumentException("Package file not found: " + packagePath);
        }
        
        Files.createDirectories(outputPath);
        
        if (packagePath.toString().endsWith(".jar") || packagePath.toString().endsWith(".cjmod")) {
            unpackJar(packagePath, outputPath);
        } else {
            unpackZip(packagePath, outputPath);
        }
    }
    
    /**
     * 解包JAR文件
     */
    private void unpackJar(Path jarPath, Path outputPath) throws Exception {
        try (JarFile jarFile = new JarFile(jarPath.toFile())) {
            Enumeration<JarEntry> entries = jarFile.entries();
            
            while (entries.hasMoreElements()) {
                JarEntry entry = entries.nextElement();
                Path entryPath = outputPath.resolve(entry.getName());
                
                if (entry.isDirectory()) {
                    Files.createDirectories(entryPath);
                } else {
                    Files.createDirectories(entryPath.getParent());
                    
                    try (InputStream is = jarFile.getInputStream(entry)) {
                        Files.copy(is, entryPath, StandardCopyOption.REPLACE_EXISTING);
                    }
                }
            }
        }
    }
    
    /**
     * 解包ZIP文件
     */
    private void unpackZip(Path zipPath, Path outputPath) throws Exception {
        try (ZipFile zipFile = new ZipFile(zipPath.toFile())) {
            Enumeration<? extends ZipEntry> entries = zipFile.entries();
            
            while (entries.hasMoreElements()) {
                ZipEntry entry = entries.nextElement();
                Path entryPath = outputPath.resolve(entry.getName());
                
                if (entry.isDirectory()) {
                    Files.createDirectories(entryPath);
                } else {
                    Files.createDirectories(entryPath.getParent());
                    
                    try (InputStream is = zipFile.getInputStream(entry)) {
                        Files.copy(is, entryPath, StandardCopyOption.REPLACE_EXISTING);
                    }
                }
            }
        }
    }
}

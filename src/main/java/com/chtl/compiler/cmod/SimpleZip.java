package com.chtl.compiler.cmod;

import java.io.*;
import java.nio.file.*;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.zip.*;

/**
 * 简单的Zip工具类
 * 用于Cmod文件的打包和解包
 */
public class SimpleZip {
    
    /**
     * 压缩文件夹为zip文件
     */
    public static void zipFolder(Path sourceFolder, Path targetZipFile) throws IOException {
        try (ZipOutputStream zos = new ZipOutputStream(
                new BufferedOutputStream(Files.newOutputStream(targetZipFile)))) {
            
            // 设置压缩级别
            zos.setLevel(Deflater.DEFAULT_COMPRESSION);
            
            // 遍历文件夹
            Files.walkFileTree(sourceFolder, new SimpleFileVisitor<Path>() {
                @Override
                public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) throws IOException {
                    // 计算相对路径
                    Path relativePath = sourceFolder.relativize(file);
                    String entryName = relativePath.toString().replace('\\', '/');
                    
                    // 添加zip条目
                    ZipEntry entry = new ZipEntry(entryName);
                    entry.setTime(attrs.lastModifiedTime().toMillis());
                    zos.putNextEntry(entry);
                    
                    // 写入文件内容
                    Files.copy(file, zos);
                    zos.closeEntry();
                    
                    return FileVisitResult.CONTINUE;
                }
                
                @Override
                public FileVisitResult preVisitDirectory(Path dir, BasicFileAttributes attrs) throws IOException {
                    // 跳过根目录
                    if (dir.equals(sourceFolder)) {
                        return FileVisitResult.CONTINUE;
                    }
                    
                    // 计算相对路径
                    Path relativePath = sourceFolder.relativize(dir);
                    String entryName = relativePath.toString().replace('\\', '/') + "/";
                    
                    // 添加目录条目
                    ZipEntry entry = new ZipEntry(entryName);
                    entry.setTime(attrs.lastModifiedTime().toMillis());
                    zos.putNextEntry(entry);
                    zos.closeEntry();
                    
                    return FileVisitResult.CONTINUE;
                }
            });
        }
    }
    
    /**
     * 解压zip文件到指定文件夹
     */
    public static void unzipFile(Path zipFile, Path targetFolder) throws IOException {
        // 创建目标文件夹
        Files.createDirectories(targetFolder);
        
        try (ZipInputStream zis = new ZipInputStream(
                new BufferedInputStream(Files.newInputStream(zipFile)))) {
            
            ZipEntry entry;
            while ((entry = zis.getNextEntry()) != null) {
                Path targetPath = targetFolder.resolve(entry.getName());
                
                if (entry.isDirectory()) {
                    // 创建目录
                    Files.createDirectories(targetPath);
                } else {
                    // 确保父目录存在
                    Files.createDirectories(targetPath.getParent());
                    
                    // 写入文件
                    try (OutputStream out = Files.newOutputStream(targetPath)) {
                        byte[] buffer = new byte[8192];
                        int len;
                        while ((len = zis.read(buffer)) > 0) {
                            out.write(buffer, 0, len);
                        }
                    }
                    
                    // 设置文件时间
                    if (entry.getTime() != -1) {
                        Files.setLastModifiedTime(targetPath, 
                            java.nio.file.attribute.FileTime.fromMillis(entry.getTime()));
                    }
                }
                
                zis.closeEntry();
            }
        }
    }
    
    /**
     * 添加文件到已存在的zip文件
     */
    public static void addFileToZip(Path zipFile, Path fileToAdd, String entryName) throws IOException {
        // 创建临时文件
        Path tempFile = Files.createTempFile("temp", ".zip");
        
        try {
            // 复制原zip内容到临时文件，并添加新文件
            try (ZipInputStream zis = new ZipInputStream(Files.newInputStream(zipFile));
                 ZipOutputStream zos = new ZipOutputStream(Files.newOutputStream(tempFile))) {
                
                // 复制原有条目
                ZipEntry entry;
                while ((entry = zis.getNextEntry()) != null) {
                    // 跳过同名条目（覆盖）
                    if (!entry.getName().equals(entryName)) {
                        zos.putNextEntry(new ZipEntry(entry.getName()));
                        
                        byte[] buffer = new byte[8192];
                        int len;
                        while ((len = zis.read(buffer)) > 0) {
                            zos.write(buffer, 0, len);
                        }
                        
                        zos.closeEntry();
                    }
                    zis.closeEntry();
                }
                
                // 添加新文件
                ZipEntry newEntry = new ZipEntry(entryName);
                zos.putNextEntry(newEntry);
                Files.copy(fileToAdd, zos);
                zos.closeEntry();
            }
            
            // 替换原文件
            Files.move(tempFile, zipFile, StandardCopyOption.REPLACE_EXISTING);
            
        } finally {
            // 清理临时文件
            Files.deleteIfExists(tempFile);
        }
    }
    
    /**
     * 检查zip文件中是否包含指定条目
     */
    public static boolean containsEntry(Path zipFile, String entryName) throws IOException {
        try (ZipInputStream zis = new ZipInputStream(Files.newInputStream(zipFile))) {
            ZipEntry entry;
            while ((entry = zis.getNextEntry()) != null) {
                if (entry.getName().equals(entryName)) {
                    return true;
                }
                zis.closeEntry();
            }
        }
        return false;
    }
    
    /**
     * 从zip文件中读取指定条目的内容
     */
    public static byte[] readEntry(Path zipFile, String entryName) throws IOException {
        try (ZipInputStream zis = new ZipInputStream(Files.newInputStream(zipFile))) {
            ZipEntry entry;
            while ((entry = zis.getNextEntry()) != null) {
                if (entry.getName().equals(entryName)) {
                    ByteArrayOutputStream baos = new ByteArrayOutputStream();
                    byte[] buffer = new byte[8192];
                    int len;
                    while ((len = zis.read(buffer)) > 0) {
                        baos.write(buffer, 0, len);
                    }
                    return baos.toByteArray();
                }
                zis.closeEntry();
            }
        }
        throw new FileNotFoundException("Entry not found: " + entryName);
    }
}
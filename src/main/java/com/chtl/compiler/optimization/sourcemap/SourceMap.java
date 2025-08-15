package com.chtl.compiler.optimization.sourcemap;
import java.util.ArrayList;
import java.util.List;

import com.chtl.compiler.optimization.sourcemap.SourceMap.Mapping;


import java.util.*;

/**
 * 源码映射
 */
public class SourceMap {
    private int version = 3;
    private String file;
    private String sourceRoot = "";
    private List<String> sources = new ArrayList<>();
    private List<String> sourcesContent = new ArrayList<>();
    private List<String> names = new ArrayList<>();
    private String mappings = "";
    private boolean includeSourceContent = true;
    
    // 映射数据
    private List<Mapping> mappingList = new ArrayList<>();
    
    public void addSource(String source) {
        sources.add(source);
    }
    
    public void addSourceContent(String content) {
        sourcesContent.add(content);
    }
    
    public void addName(String name) {
        names.add(name);
    }
    
    public void addMapping(int genLine, int genCol, int sourceIndex, int origLine, int origCol) {
        mappingList.add(new Mapping(genLine, genCol, sourceIndex, origLine, origCol));
    }
    
    /**
     * 生成JSON格式
     */
    public String toJSON() {
        // 生成VLQ编码的mappings字符串
        generateMappings();
        
        StringBuilder json = new StringBuilder();
        json.append("{\n");
        json.append("  \"version\": ").append(version).append(",\n");
        json.append("  \"file\": \"").append(file).append("\",\n");
        json.append("  \"sourceRoot\": \"").append(sourceRoot).append("\",\n");
        json.append("  \"sources\": ").append(toJSONArray(sources)).append(",\n");
        
        if (includeSourceContent && !sourcesContent.isEmpty()) {
            json.append("  \"sourcesContent\": ").append(toJSONArray(sourcesContent)).append(",\n");
        }
        
        if (!names.isEmpty()) {
            json.append("  \"names\": ").append(toJSONArray(names)).append(",\n");
        }
        
        json.append("  \"mappings\": \"").append(mappings).append("\"\n");
        json.append("}");
        
        return json.toString();
    }
    
    private String toJSONArray(List<String> list) {
        StringBuilder sb = new StringBuilder("[");
        for (int i = 0; i < list.size(); i++) {
            if (i > 0) sb.append(", ");
            sb.append("\"").append(escapeJSON(list.get(i))).append("\"");
        }
        sb.append("]");
        return sb.toString();
    }
    
    private String escapeJSON(String str) {
        return str.replace("\\", "\\\\")
                  .replace("\"", "\\\"")
                  .replace("\n", "\\n")
                  .replace("\r", "\\r")
                  .replace("\t", "\\t");
    }
    
    private void generateMappings() {
        // 简化的VLQ编码实现
        // 实际实现需要完整的VLQ编码
        StringBuilder sb = new StringBuilder();
        
        int lastGenLine = 0;
        for (Mapping m : mappingList) {
            // 新行
            while (lastGenLine < m.genLine) {
                sb.append(";");
                lastGenLine++;
            }
            
            // 简化的编码（实际需要VLQ）
            if (sb.length() > 0 && sb.charAt(sb.length() - 1) != ';') {
                sb.append(",");
            }
            sb.append("AAAA"); // 占位符
        }
        
        this.mappings = sb.toString();
    }
    
    // Getters and setters
    public int getVersion() { return version; }
    public void setVersion(int version) { this.version = version; }
    public String getFile() { return file; }
    public void setFile(String file) { this.file = file; }
    public boolean isIncludeSourceContent() { return includeSourceContent; }
    public void setIncludeSourceContent(boolean include) { this.includeSourceContent = include; }
    
    /**
     * 映射条目
     */
    private static class Mapping {
        int genLine;
        int genCol;
        int sourceIndex;
        int origLine;
        int origCol;
        
        Mapping(int genLine, int genCol, int sourceIndex, int origLine, int origCol) {
            this.genLine = genLine;
            this.genCol = genCol;
            this.sourceIndex = sourceIndex;
            this.origLine = origLine;
            this.origCol = origCol;
        }
    }
}
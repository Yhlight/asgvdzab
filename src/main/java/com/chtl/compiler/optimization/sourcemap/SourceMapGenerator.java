package com.chtl.compiler.optimization.sourcemap;

import com.chtl.context.CompilationContext;
import com.chtl.compiler.optimization.OutputOptimizer.*;

import java.util.*;

/**
 * 源码映射生成器
 * 生成Source Map v3格式的映射文件
 * 严格遵循CHTL语法文档
 */
public class SourceMapGenerator {
    private final CompilationContext context;
    
    public SourceMapGenerator(CompilationContext context) {
        this.context = context;
    }
    
    /**
     * 生成HTML源码映射
     */
    public SourceMap generateHtmlMap(String originalHtml, String optimizedHtml, List<SourceFile> sourceFiles) {
        SourceMap map = new SourceMap();
        map.setVersion(3);
        map.setFile("output.html");
        
        // 添加源文件
        for (SourceFile source : sourceFiles) {
            map.addSource(source.getPath());
            if (map.isIncludeSourceContent()) {
                map.addSourceContent(source.getContent());
            }
        }
        
        // 生成映射（简化实现）
        generateMappings(map, originalHtml, optimizedHtml);
        
        return map;
    }
    
    /**
     * 生成CSS源码映射
     */
    public SourceMap generateCssMap(List<CssFile> cssFiles, String optimizedCss) {
        SourceMap map = new SourceMap();
        map.setVersion(3);
        map.setFile("output.css");
        
        // 添加源文件
        for (CssFile file : cssFiles) {
            map.addSource(file.getSourcePath());
            if (map.isIncludeSourceContent()) {
                map.addSourceContent(file.getContent());
            }
        }
        
        // 生成映射
        generateCssMappings(map, cssFiles, optimizedCss);
        
        return map;
    }
    
    /**
     * 生成JavaScript源码映射
     */
    public SourceMap generateJsMap(List<JsFile> jsFiles, String optimizedJs) {
        SourceMap map = new SourceMap();
        map.setVersion(3);
        map.setFile("output.js");
        
        // 添加源文件
        for (JsFile file : jsFiles) {
            map.addSource(file.getSourcePath());
            if (map.isIncludeSourceContent()) {
                map.addSourceContent(file.getContent());
            }
        }
        
        // 生成映射
        generateJsMappings(map, jsFiles, optimizedJs);
        
        return map;
    }
    
    /**
     * 生成映射关系
     */
    private void generateMappings(SourceMap map, String original, String optimized) {
        // 简化的映射生成
        // 实际实现需要更复杂的行列跟踪
        
        String[] originalLines = original.split("\n");
        String[] optimizedLines = optimized.split("\n");
        
        int origLine = 0;
        int origCol = 0;
        
        for (int optLine = 0; optLine < optimizedLines.length; optLine++) {
            String line = optimizedLines[optLine];
            
            // 查找对应的原始行
            if (origLine < originalLines.length) {
                map.addMapping(
                    optLine,
                    0,
                    0, // source index
                    origLine,
                    origCol
                );
                origLine++;
            }
        }
    }
    
    /**
     * 生成CSS映射关系
     */
    private void generateCssMappings(SourceMap map, List<CssFile> cssFiles, String optimized) {
        // 跟踪每个CSS文件在优化后文件中的位置
        int currentLine = 0;
        int sourceIndex = 0;
        
        for (CssFile file : cssFiles) {
            String[] lines = file.getContent().split("\n");
            
            for (int i = 0; i < lines.length; i++) {
                map.addMapping(
                    currentLine++,
                    0,
                    sourceIndex,
                    i,
                    0
                );
            }
            
            sourceIndex++;
        }
    }
    
    /**
     * 生成JavaScript映射关系
     */
    private void generateJsMappings(SourceMap map, List<JsFile> jsFiles, String optimized) {
        // 类似CSS的处理
        int currentLine = 0;
        int sourceIndex = 0;
        
        for (JsFile file : jsFiles) {
            String[] lines = file.getContent().split("\n");
            
            for (int i = 0; i < lines.length; i++) {
                map.addMapping(
                    currentLine++,
                    0,
                    sourceIndex,
                    i,
                    0
                );
            }
            
            sourceIndex++;
        }
    }
}
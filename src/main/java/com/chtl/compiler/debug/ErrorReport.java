package com.chtl.compiler.debug;

import java.io.*;
import java.util.*;

/**
 * 错误报告
 */
public class ErrorReport {
    private List<DetailedError> errors = new ArrayList<>();
    private List<DetailedWarning> warnings = new ArrayList<>();
    private ErrorStatistics statistics;
    
    /**
     * 生成HTML报告
     */
    public void generateHtmlReport(String outputPath) throws IOException {
        try (PrintWriter writer = new PrintWriter(new FileWriter(outputPath))) {
            writer.println("<!DOCTYPE html>");
            writer.println("<html>");
            writer.println("<head>");
            writer.println("<meta charset=\"UTF-8\">");
            writer.println("<title>CHTL 编译错误报告</title>");
            writer.println("<style>");
            writer.println(getCSS());
            writer.println("</style>");
            writer.println("</head>");
            writer.println("<body>");
            
            // 标题
            writer.println("<h1>CHTL 编译错误报告</h1>");
            writer.println("<p>生成时间: " + new Date() + "</p>");
            
            // 统计信息
            if (statistics != null) {
                writer.println("<h2>统计信息</h2>");
                writer.println("<div class='statistics'>");
                writer.println("<p>总错误数: " + statistics.getTotalErrors() + "</p>");
                writer.println("<p>总警告数: " + statistics.getTotalWarnings() + "</p>");
                writer.println("</div>");
            }
            
            // 错误列表
            if (!errors.isEmpty()) {
                writer.println("<h2>错误</h2>");
                for (DetailedError error : errors) {
                    writeError(writer, error);
                }
            }
            
            // 警告列表
            if (!warnings.isEmpty()) {
                writer.println("<h2>警告</h2>");
                for (DetailedWarning warning : warnings) {
                    writeWarning(writer, warning);
                }
            }
            
            writer.println("</body>");
            writer.println("</html>");
        }
    }
    
    /**
     * 生成文本报告
     */
    public void generateTextReport(String outputPath) throws IOException {
        try (PrintWriter writer = new PrintWriter(new FileWriter(outputPath))) {
            writer.println("CHTL 编译错误报告");
            writer.println("================");
            writer.println("生成时间: " + new Date());
            writer.println();
            
            // 统计信息
            if (statistics != null) {
                writer.println("统计信息");
                writer.println("--------");
                writer.println("总错误数: " + statistics.getTotalErrors());
                writer.println("总警告数: " + statistics.getTotalWarnings());
                writer.println();
            }
            
            // 错误列表
            if (!errors.isEmpty()) {
                writer.println("错误");
                writer.println("----");
                for (DetailedError error : errors) {
                    writeTextError(writer, error);
                }
            }
            
            // 警告列表
            if (!warnings.isEmpty()) {
                writer.println("警告");
                writer.println("----");
                for (DetailedWarning warning : warnings) {
                    writeTextWarning(writer, warning);
                }
            }
        }
    }
    
    private void writeError(PrintWriter writer, DetailedError error) {
        writer.println("<div class='error'>");
        writer.println("<h3>" + escapeHtml(error.getOriginalError().getMessage()) + "</h3>");
        
        if (error.getOriginalError().getLocation() != null) {
            writer.println("<p class='location'>位置: " + 
                escapeHtml(error.getOriginalError().getLocation().toString()) + "</p>");
        }
        
        if (!error.getSuggestions().isEmpty()) {
            writer.println("<div class='suggestions'>");
            writer.println("<h4>建议:</h4>");
            writer.println("<ul>");
            for (String suggestion : error.getSuggestions()) {
                writer.println("<li>" + escapeHtml(suggestion) + "</li>");
            }
            writer.println("</ul>");
            writer.println("</div>");
        }
        
        writer.println("</div>");
    }
    
    private void writeWarning(PrintWriter writer, DetailedWarning warning) {
        writer.println("<div class='warning'>");
        writer.println("<h3>" + escapeHtml(warning.getOriginalWarning().getMessage()) + "</h3>");
        
        if (warning.getOriginalWarning().getLocation() != null) {
            writer.println("<p class='location'>位置: " + 
                escapeHtml(warning.getOriginalWarning().getLocation().toString()) + "</p>");
        }
        
        writer.println("</div>");
    }
    
    private void writeTextError(PrintWriter writer, DetailedError error) {
        writer.println("错误: " + error.getOriginalError().getMessage());
        
        if (error.getOriginalError().getLocation() != null) {
            writer.println("位置: " + error.getOriginalError().getLocation());
        }
        
        if (!error.getSuggestions().isEmpty()) {
            writer.println("建议:");
            for (String suggestion : error.getSuggestions()) {
                writer.println("  - " + suggestion);
            }
        }
        
        writer.println();
    }
    
    private void writeTextWarning(PrintWriter writer, DetailedWarning warning) {
        writer.println("警告: " + warning.getOriginalWarning().getMessage());
        
        if (warning.getOriginalWarning().getLocation() != null) {
            writer.println("位置: " + warning.getOriginalWarning().getLocation());
        }
        
        writer.println();
    }
    
    private String getCSS() {
        return """
            body {
                font-family: Arial, sans-serif;
                margin: 20px;
                background-color: #f5f5f5;
            }
            h1, h2 {
                color: #333;
            }
            .statistics {
                background-color: #e0e0e0;
                padding: 10px;
                border-radius: 5px;
                margin-bottom: 20px;
            }
            .error, .warning {
                background-color: white;
                padding: 15px;
                margin-bottom: 10px;
                border-radius: 5px;
                border-left: 5px solid;
            }
            .error {
                border-color: #f44336;
            }
            .warning {
                border-color: #ff9800;
            }
            .location {
                color: #666;
                font-size: 0.9em;
            }
            .suggestions {
                margin-top: 10px;
                background-color: #f9f9f9;
                padding: 10px;
                border-radius: 3px;
            }
            .suggestions h4 {
                margin-top: 0;
                color: #2196F3;
            }
            """;
    }
    
    private String escapeHtml(String text) {
        return text.replace("&", "&amp;")
                  .replace("<", "&lt;")
                  .replace(">", "&gt;")
                  .replace("\"", "&quot;")
                  .replace("'", "&#39;");
    }
    
    // Getters and setters
    public List<DetailedError> getErrors() {
        return errors;
    }
    
    public void setErrors(List<DetailedError> errors) {
        this.errors = errors;
    }
    
    public List<DetailedWarning> getWarnings() {
        return warnings;
    }
    
    public void setWarnings(List<DetailedWarning> warnings) {
        this.warnings = warnings;
    }
    
    public ErrorStatistics getStatistics() {
        return statistics;
    }
    
    public void setStatistics(ErrorStatistics statistics) {
        this.statistics = statistics;
    }
}
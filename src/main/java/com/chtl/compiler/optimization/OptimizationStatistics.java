package com.chtl.compiler.optimization;

/**
 * 优化统计信息
 */
public class OptimizationStatistics {
    // HTML统计
    private double htmlCompressionRatio;
    private int htmlSizeSaved;
    
    // CSS统计
    private double cssCompressionRatio;
    private int cssSizeSaved;
    private int cssFilesMerged;
    
    // JavaScript统计
    private double jsCompressionRatio;
    private int jsSizeSaved;
    private int jsFilesMerged;
    
    // 总体统计
    private int totalOriginalSize;
    private int totalOptimizedSize;
    private double totalCompressionRatio;
    
    /**
     * 生成统计报告
     */
    public String generateReport() {
        StringBuilder report = new StringBuilder();
        report.append("=== 优化统计报告 ===\n\n");
        
        report.append("HTML优化:\n");
        report.append(String.format("  压缩率: %.1f%%\n", htmlCompressionRatio));
        report.append(String.format("  节省大小: %d bytes\n", htmlSizeSaved));
        report.append("\n");
        
        report.append("CSS优化:\n");
        report.append(String.format("  压缩率: %.1f%%\n", cssCompressionRatio));
        report.append(String.format("  节省大小: %d bytes\n", cssSizeSaved));
        report.append(String.format("  合并文件数: %d\n", cssFilesMerged));
        report.append("\n");
        
        report.append("JavaScript优化:\n");
        report.append(String.format("  压缩率: %.1f%%\n", jsCompressionRatio));
        report.append(String.format("  节省大小: %d bytes\n", jsSizeSaved));
        report.append(String.format("  合并文件数: %d\n", jsFilesMerged));
        report.append("\n");
        
        report.append("总体优化:\n");
        report.append(String.format("  原始大小: %d bytes\n", totalOriginalSize));
        report.append(String.format("  优化后大小: %d bytes\n", totalOptimizedSize));
        report.append(String.format("  总压缩率: %.1f%%\n", totalCompressionRatio));
        report.append(String.format("  总节省: %d bytes\n", totalOriginalSize - totalOptimizedSize));
        
        return report.toString();
    }
    
    // Getters and setters
    public double getHtmlCompressionRatio() { return htmlCompressionRatio; }
    public void setHtmlCompressionRatio(double ratio) { this.htmlCompressionRatio = ratio; }
    
    public int getHtmlSizeSaved() { return htmlSizeSaved; }
    public void setHtmlSizeSaved(int size) { this.htmlSizeSaved = size; }
    
    public double getCssCompressionRatio() { return cssCompressionRatio; }
    public void setCssCompressionRatio(double ratio) { this.cssCompressionRatio = ratio; }
    
    public int getCssSizeSaved() { return cssSizeSaved; }
    public void setCssSizeSaved(int size) { this.cssSizeSaved = size; }
    
    public int getCssFilesMerged() { return cssFilesMerged; }
    public void setCssFilesMerged(int count) { this.cssFilesMerged = count; }
    
    public double getJsCompressionRatio() { return jsCompressionRatio; }
    public void setJsCompressionRatio(double ratio) { this.jsCompressionRatio = ratio; }
    
    public int getJsSizeSaved() { return jsSizeSaved; }
    public void setJsSizeSaved(int size) { this.jsSizeSaved = size; }
    
    public int getJsFilesMerged() { return jsFilesMerged; }
    public void setJsFilesMerged(int count) { this.jsFilesMerged = count; }
    
    public int getTotalOriginalSize() { return totalOriginalSize; }
    public void setTotalOriginalSize(int size) { this.totalOriginalSize = size; }
    
    public int getTotalOptimizedSize() { return totalOptimizedSize; }
    public void setTotalOptimizedSize(int size) { this.totalOptimizedSize = size; }
    
    public double getTotalCompressionRatio() { return totalCompressionRatio; }
    public void setTotalCompressionRatio(double ratio) { this.totalCompressionRatio = ratio; }
}
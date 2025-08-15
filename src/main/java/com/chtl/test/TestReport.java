package com.chtl.test;

import java.io.*;
import java.nio.file.Path;
import java.util.*;

/**
 * 测试报告生成器
 */
public class TestReport {
    private final List<TestResult> results = new ArrayList<>();
    private long totalTime;
    
    public void addResult(TestResult result) {
        results.add(result);
    }
    
    public void setTotalTime(long totalTime) {
        this.totalTime = totalTime;
    }
    
    public int getTotalTests() {
        return results.size();
    }
    
    public int getPassedTests() {
        return (int) results.stream()
            .filter(r -> r.getStatus() == TestStatus.PASSED)
            .count();
    }
    
    public int getFailedTests() {
        return (int) results.stream()
            .filter(r -> r.getStatus() == TestStatus.FAILED)
            .count();
    }
    
    public int getErrorTests() {
        return (int) results.stream()
            .filter(r -> r.getStatus() == TestStatus.ERROR)
            .count();
    }
    
    public long getTotalTime() {
        return totalTime;
    }
    
    public double getPassRate() {
        if (getTotalTests() == 0) return 0;
        return (getPassedTests() * 100.0) / getTotalTests();
    }
    
    /**
     * 生成HTML报告
     */
    public void generateReport(Path outputPath) {
        try (PrintWriter writer = new PrintWriter(new FileWriter(outputPath.toFile()))) {
            writer.println("<!DOCTYPE html>");
            writer.println("<html>");
            writer.println("<head>");
            writer.println("<meta charset=\"UTF-8\">");
            writer.println("<title>CHTL 集成测试报告</title>");
            writer.println("<style>");
            writer.println(getCSS());
            writer.println("</style>");
            writer.println("</head>");
            writer.println("<body>");
            
            // 标题和摘要
            writer.println("<h1>CHTL 集成测试报告</h1>");
            writer.println("<div class='summary'>");
            writer.println("<h2>测试摘要</h2>");
            writer.println("<div class='stats'>");
            writer.println("<div class='stat'>");
            writer.println("<div class='stat-value'>" + getTotalTests() + "</div>");
            writer.println("<div class='stat-label'>总测试数</div>");
            writer.println("</div>");
            writer.println("<div class='stat passed'>");
            writer.println("<div class='stat-value'>" + getPassedTests() + "</div>");
            writer.println("<div class='stat-label'>通过</div>");
            writer.println("</div>");
            writer.println("<div class='stat failed'>");
            writer.println("<div class='stat-value'>" + getFailedTests() + "</div>");
            writer.println("<div class='stat-label'>失败</div>");
            writer.println("</div>");
            writer.println("<div class='stat error'>");
            writer.println("<div class='stat-value'>" + getErrorTests() + "</div>");
            writer.println("<div class='stat-label'>错误</div>");
            writer.println("</div>");
            writer.println("</div>");
            writer.println("<div class='progress-bar'>");
            writer.println("<div class='progress-fill' style='width: " + getPassRate() + "%'></div>");
            writer.println("</div>");
            writer.println("<p>通过率: " + String.format("%.2f%%", getPassRate()) + "</p>");
            writer.println("<p>总耗时: " + totalTime + "ms</p>");
            writer.println("</div>");
            
            // 测试详情
            writer.println("<h2>测试详情</h2>");
            writer.println("<table>");
            writer.println("<thead>");
            writer.println("<tr>");
            writer.println("<th>测试名称</th>");
            writer.println("<th>状态</th>");
            writer.println("<th>耗时</th>");
            writer.println("<th>详情</th>");
            writer.println("</tr>");
            writer.println("</thead>");
            writer.println("<tbody>");
            
            for (TestResult result : results) {
                writer.println("<tr class='" + result.getStatus().toString().toLowerCase() + "'>");
                writer.println("<td>" + result.getTestName() + "</td>");
                writer.println("<td><span class='status'>" + result.getStatus() + "</span></td>");
                writer.println("<td>" + result.getExecutionTime() + "ms</td>");
                writer.println("<td>");
                if (result.getMessage() != null) {
                    writer.println("<pre>" + escapeHtml(result.getMessage()) + "</pre>");
                }
                if (result.getException() != null) {
                    writer.println("<details>");
                    writer.println("<summary>异常堆栈</summary>");
                    writer.println("<pre>" + getStackTrace(result.getException()) + "</pre>");
                    writer.println("</details>");
                }
                writer.println("</td>");
                writer.println("</tr>");
            }
            
            writer.println("</tbody>");
            writer.println("</table>");
            
            writer.println("<div class='footer'>");
            writer.println("<p>生成时间: " + new Date() + "</p>");
            writer.println("</div>");
            
            writer.println("</body>");
            writer.println("</html>");
        } catch (IOException e) {
            System.err.println("生成报告失败: " + e.getMessage());
        }
    }
    
    private String getCSS() {
        return """
            body {
                font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
                margin: 0;
                padding: 20px;
                background-color: #f5f5f5;
            }
            h1 {
                color: #333;
                text-align: center;
            }
            .summary {
                background: white;
                padding: 20px;
                border-radius: 8px;
                box-shadow: 0 2px 4px rgba(0,0,0,0.1);
                margin-bottom: 20px;
            }
            .stats {
                display: flex;
                justify-content: space-around;
                margin: 20px 0;
            }
            .stat {
                text-align: center;
            }
            .stat-value {
                font-size: 36px;
                font-weight: bold;
                color: #333;
            }
            .stat-label {
                color: #666;
                margin-top: 5px;
            }
            .stat.passed .stat-value { color: #4CAF50; }
            .stat.failed .stat-value { color: #f44336; }
            .stat.error .stat-value { color: #ff9800; }
            .progress-bar {
                width: 100%;
                height: 20px;
                background: #e0e0e0;
                border-radius: 10px;
                overflow: hidden;
                margin: 20px 0;
            }
            .progress-fill {
                height: 100%;
                background: #4CAF50;
                transition: width 0.3s ease;
            }
            table {
                width: 100%;
                background: white;
                border-radius: 8px;
                box-shadow: 0 2px 4px rgba(0,0,0,0.1);
                border-collapse: collapse;
            }
            th, td {
                padding: 12px;
                text-align: left;
                border-bottom: 1px solid #e0e0e0;
            }
            th {
                background: #f5f5f5;
                font-weight: 600;
            }
            tr.passed .status { color: #4CAF50; }
            tr.failed { background: #ffebee; }
            tr.failed .status { color: #f44336; }
            tr.error { background: #fff3e0; }
            tr.error .status { color: #ff9800; }
            pre {
                background: #f5f5f5;
                padding: 10px;
                border-radius: 4px;
                overflow-x: auto;
                margin: 5px 0;
            }
            details {
                margin-top: 10px;
            }
            summary {
                cursor: pointer;
                color: #2196F3;
            }
            .footer {
                text-align: center;
                color: #666;
                margin-top: 40px;
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
    
    private String getStackTrace(Exception e) {
        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        e.printStackTrace(pw);
        return escapeHtml(sw.toString());
    }
}
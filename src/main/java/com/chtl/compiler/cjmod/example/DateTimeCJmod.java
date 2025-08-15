package com.chtl.compiler.cjmod.example;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.chtl.ast.chtljs.CHTLJSASTNode;
import com.chtl.ast.chtljs.ExpressionNode;
import com.chtl.compiler.cjmod.AbstractCJmodModule;
import com.chtl.compiler.cjmod.CJmodInterface.MethodDescriptor;
import com.chtl.compiler.cjmod.CJmodInterface.ObjectDescriptor;


import com.chtl.compiler.cjmod.*;
import com.chtl.ast.chtljs.*;
import com.chtl.context.CHTLJSContext;

import java.time.*;
import java.time.format.DateTimeFormatter;
import java.util.*;

/**
 * DateTime CJmod模块示例
 * 为CHTL JS提供增强的日期时间功能
 */
public class DateTimeCJmod extends AbstractCJmodModule {
    
    public DateTimeCJmod() {
        super("DateTime", "1.0.0", 
              "Enhanced date and time functionality for CHTL JS",
              "CHTL Team");
    }
    
    @Override
    protected void registerMethods() {
        // 注册now方法
        registerMethod(createMethod(
            "now",
            "Get current date and time in various formats",
            Arrays.asList(
                createParameter("format", "string", "Output format (iso, unix, locale)", false)
            ),
            "string",
            "DateTime.now('iso') // Returns '2024-01-20T10:30:00Z'"
        ));
        
        // 注册format方法
        registerMethod(createMethod(
            "format",
            "Format a date string or timestamp",
            Arrays.asList(
                createParameter("date", "string|number", "Date string or timestamp"),
                createParameter("pattern", "string", "Format pattern")
            ),
            "string",
            "DateTime.format('2024-01-20', 'yyyy年MM月dd日')"
        ));
        
        // 注册parse方法
        registerMethod(createMethod(
            "parse",
            "Parse a date string into timestamp",
            Arrays.asList(
                createParameter("dateStr", "string", "Date string to parse"),
                createParameter("pattern", "string", "Input pattern", false)
            ),
            "number",
            "DateTime.parse('2024-01-20') // Returns timestamp"
        ));
        
        // 注册add方法
        registerMethod(createMethod(
            "add",
            "Add time to a date",
            Arrays.asList(
                createParameter("date", "string|number", "Base date"),
                createParameter("amount", "number", "Amount to add"),
                createParameter("unit", "string", "Unit (days, hours, minutes, etc.)")
            ),
            "string",
            "DateTime.add('2024-01-20', 7, 'days')"
        ));
        
        // 注册diff方法
        registerMethod(createMethod(
            "diff",
            "Calculate difference between two dates",
            Arrays.asList(
                createParameter("date1", "string|number", "First date"),
                createParameter("date2", "string|number", "Second date"),
                createParameter("unit", "string", "Unit for difference", false)
            ),
            "number",
            "DateTime.diff('2024-01-20', '2024-01-27', 'days') // Returns 7"
        ));
    }
    
    @Override
    protected void registerObjects() {
        // 注册DateRange对象
        Map<String, MethodDescriptor> rangeMethods = new HashMap<>();
        rangeMethods.put("contains", createMethod(
            "contains",
            "Check if a date is within the range",
            Arrays.asList(createParameter("date", "string|number", "Date to check")),
            "boolean",
            "range.contains('2024-01-25')"
        ));
        
        rangeMethods.put("getDays", createMethod(
            "getDays",
            "Get all days in the range",
            Collections.emptyList(),
            "array",
            "range.getDays()"
        ));
        
        Map<String, String> rangeProperties = new HashMap<>();
        rangeProperties.put("start", "Start date of the range");
        rangeProperties.put("end", "End date of the range");
        rangeProperties.put("duration", "Duration in milliseconds");
        
        registerObject(new ObjectDescriptor(
            "DateRange",
            "Represents a date range",
            rangeMethods,
            rangeProperties
        ));
    }
    
    @Override
    protected String doExecuteMethod(String methodName, List<String> args, 
                                   CHTLJSContext context) {
        switch (methodName) {
            case "now":
                return generateNowMethod(args);
                
            case "format":
                return generateFormatMethod(args);
                
            case "parse":
                return generateParseMethod(args);
                
            case "add":
                return generateAddMethod(args);
                
            case "diff":
                return generateDiffMethod(args);
                
            default:
                throw new IllegalArgumentException("Unknown method: " + methodName);
        }
    }
    
    private String generateNowMethod(List<String> args) {
        String format = args.isEmpty() ? "'iso'" : args.get(0);
        
        return String.format(
            "(function() {" +
            "  var now = new Date();" +
            "  switch(%s) {" +
            "    case 'iso': return now.toISOString();" +
            "    case 'unix': return Math.floor(now.getTime() / 1000);" +
            "    case 'locale': return now.toLocaleString();" +
            "    default: return now.toString();" +
            "  }" +
            "})()",
            format
        );
    }
    
    private String generateFormatMethod(List<String> args) {
        String date = args.get(0);
        String pattern = args.get(1);
        
        return String.format(
            "_chtl.DateTime.format(%s, %s)",
            date, pattern
        );
    }
    
    private String generateParseMethod(List<String> args) {
        String dateStr = args.get(0);
        String pattern = args.size() > 1 ? args.get(1) : "null";
        
        return String.format(
            "_chtl.DateTime.parse(%s, %s)",
            dateStr, pattern
        );
    }
    
    private String generateAddMethod(List<String> args) {
        String date = args.get(0);
        String amount = args.get(1);
        String unit = args.get(2);
        
        return String.format(
            "_chtl.DateTime.add(%s, %s, %s)",
            date, amount, unit
        );
    }
    
    private String generateDiffMethod(List<String> args) {
        String date1 = args.get(0);
        String date2 = args.get(1);
        String unit = args.size() > 2 ? args.get(2) : "'ms'";
        
        return String.format(
            "_chtl.DateTime.diff(%s, %s, %s)",
            date1, date2, unit
        );
    }
    
    @Override
    public String processASTNode(CHTLJSASTNode node, CHTLJSContext context) {
        // 检查是否是DateRange创建
        if (node instanceof ExpressionNode) {
            ExpressionNode expr = (ExpressionNode) node;
            String code = expr.getExpression();
            
            if (code.startsWith("new DateRange(")) {
                // 转换为_chtl.DateRange
                return code.replace("new DateRange(", "new _chtl.DateRange(");
            }
        }
        
        return null;
    }
    
    @Override
    public String getRuntimeCode() {
        return """
            // DateTime CJmod Runtime
            _chtl.DateTime = {
                format: function(date, pattern) {
                    var d = typeof date === 'number' ? new Date(date) : new Date(date);
                    // 简单的格式化实现
                    return pattern
                        .replace('yyyy', d.getFullYear())
                        .replace('MM', String(d.getMonth() + 1).padStart(2, '0'))
                        .replace('dd', String(d.getDate()).padStart(2, '0'))
                        .replace('HH', String(d.getHours()).padStart(2, '0'))
                        .replace('mm', String(d.getMinutes()).padStart(2, '0'))
                        .replace('ss', String(d.getSeconds()).padStart(2, '0'));
                },
                
                parse: function(dateStr, pattern) {
                    return new Date(dateStr).getTime();
                },
                
                add: function(date, amount, unit) {
                    var d = typeof date === 'number' ? new Date(date) : new Date(date);
                    var ms = amount;
                    
                    switch(unit) {
                        case 'seconds': ms *= 1000; break;
                        case 'minutes': ms *= 60000; break;
                        case 'hours': ms *= 3600000; break;
                        case 'days': ms *= 86400000; break;
                        case 'weeks': ms *= 604800000; break;
                    }
                    
                    return new Date(d.getTime() + ms).toISOString();
                },
                
                diff: function(date1, date2, unit) {
                    var d1 = typeof date1 === 'number' ? new Date(date1) : new Date(date1);
                    var d2 = typeof date2 === 'number' ? new Date(date2) : new Date(date2);
                    var diff = d2.getTime() - d1.getTime();
                    
                    switch(unit) {
                        case 'seconds': return Math.floor(diff / 1000);
                        case 'minutes': return Math.floor(diff / 60000);
                        case 'hours': return Math.floor(diff / 3600000);
                        case 'days': return Math.floor(diff / 86400000);
                        case 'weeks': return Math.floor(diff / 604800000);
                        default: return diff;
                    }
                }
            };
            
            _chtl.DateRange = function(start, end) {
                this.start = typeof start === 'number' ? new Date(start) : new Date(start);
                this.end = typeof end === 'number' ? new Date(end) : new Date(end);
                this.duration = this.end.getTime() - this.start.getTime();
            };
            
            _chtl.DateRange.prototype.contains = function(date) {
                var d = typeof date === 'number' ? new Date(date) : new Date(date);
                return d >= this.start && d <= this.end;
            };
            
            _chtl.DateRange.prototype.getDays = function() {
                var days = [];
                var current = new Date(this.start);
                
                while (current <= this.end) {
                    days.push(current.toISOString().split('T')[0]);
                    current.setDate(current.getDate() + 1);
                }
                
                return days;
            };
            """;
    }
}
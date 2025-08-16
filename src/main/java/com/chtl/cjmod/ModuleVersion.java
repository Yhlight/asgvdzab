package com.chtl.cjmod;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.*;

/**
 * 模块版本管理
 * 支持语义化版本控制 (Semantic Versioning)
 */
public class ModuleVersion implements Comparable<ModuleVersion> {
    
    private final int major;
    private final int minor;
    private final int patch;
    private final String preRelease;
    private final String buildMetadata;
    
    // 版本格式: major.minor.patch[-prerelease][+build]
    private static final Pattern VERSION_PATTERN = Pattern.compile(
        "^(\\d+)\\.(\\d+)\\.(\\d+)(?:-(\\w+(?:\\.\\w+)*))?(?:\\+([\\w\\.]+))?$"
    );
    
    /**
     * 解析版本字符串
     */
    public static ModuleVersion parse(String version) {
        if (version == null || version.isEmpty()) {
            throw new IllegalArgumentException("Version string cannot be null or empty");
        }
        
        Matcher matcher = VERSION_PATTERN.matcher(version);
        if (!matcher.matches()) {
            throw new IllegalArgumentException("Invalid version format: " + version);
        }
        
        int major = Integer.parseInt(matcher.group(1));
        int minor = Integer.parseInt(matcher.group(2));
        int patch = Integer.parseInt(matcher.group(3));
        String preRelease = matcher.group(4);
        String buildMetadata = matcher.group(5);
        
        return new ModuleVersion(major, minor, patch, preRelease, buildMetadata);
    }
    
    public ModuleVersion(int major, int minor, int patch) {
        this(major, minor, patch, null, null);
    }
    
    public ModuleVersion(int major, int minor, int patch, String preRelease, String buildMetadata) {
        if (major < 0 || minor < 0 || patch < 0) {
            throw new IllegalArgumentException("Version numbers cannot be negative");
        }
        
        this.major = major;
        this.minor = minor;
        this.patch = patch;
        this.preRelease = preRelease;
        this.buildMetadata = buildMetadata;
    }
    
    /**
     * 检查版本兼容性
     */
    public boolean isCompatibleWith(ModuleVersion other) {
        // 主版本号相同才兼容
        if (this.major != other.major) {
            return false;
        }
        
        // 如果是0.x.x版本，则次版本号也必须相同
        if (this.major == 0) {
            return this.minor == other.minor && this.patch >= other.patch;
        }
        
        // 否则只要次版本号和补丁版本号不小于要求即可
        return this.compareTo(other) >= 0;
    }
    
    /**
     * 检查是否满足版本范围
     */
    public boolean satisfies(VersionRange range) {
        return range.contains(this);
    }
    
    @Override
    public int compareTo(ModuleVersion other) {
        // 比较主版本号
        int result = Integer.compare(this.major, other.major);
        if (result != 0) return result;
        
        // 比较次版本号
        result = Integer.compare(this.minor, other.minor);
        if (result != 0) return result;
        
        // 比较补丁版本号
        result = Integer.compare(this.patch, other.patch);
        if (result != 0) return result;
        
        // 比较预发布版本
        if (this.preRelease == null && other.preRelease == null) {
            return 0;
        } else if (this.preRelease == null) {
            return 1; // 正式版本大于预发布版本
        } else if (other.preRelease == null) {
            return -1;
        } else {
            return comparePreRelease(this.preRelease, other.preRelease);
        }
    }
    
    /**
     * 比较预发布版本
     */
    private int comparePreRelease(String pre1, String pre2) {
        String[] parts1 = pre1.split("\\.");
        String[] parts2 = pre2.split("\\.");
        
        int minLength = Math.min(parts1.length, parts2.length);
        
        for (int i = 0; i < minLength; i++) {
            String part1 = parts1[i];
            String part2 = parts2[i];
            
            // 尝试作为数字比较
            try {
                int num1 = Integer.parseInt(part1);
                int num2 = Integer.parseInt(part2);
                int result = Integer.compare(num1, num2);
                if (result != 0) return result;
            } catch (NumberFormatException e) {
                // 作为字符串比较
                int result = part1.compareTo(part2);
                if (result != 0) return result;
            }
        }
        
        // 较短的预发布版本较小
        return Integer.compare(parts1.length, parts2.length);
    }
    
    /**
     * 获取下一个版本号
     */
    public ModuleVersion nextMajor() {
        return new ModuleVersion(major + 1, 0, 0);
    }
    
    public ModuleVersion nextMinor() {
        return new ModuleVersion(major, minor + 1, 0);
    }
    
    public ModuleVersion nextPatch() {
        return new ModuleVersion(major, minor, patch + 1);
    }
    
    // Getters
    public int getMajor() { return major; }
    public int getMinor() { return minor; }
    public int getPatch() { return patch; }
    public String getPreRelease() { return preRelease; }
    public String getBuildMetadata() { return buildMetadata; }
    
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append(major).append(".").append(minor).append(".").append(patch);
        
        if (preRelease != null) {
            sb.append("-").append(preRelease);
        }
        
        if (buildMetadata != null) {
            sb.append("+").append(buildMetadata);
        }
        
        return sb.toString();
    }
    
    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (!(obj instanceof ModuleVersion)) return false;
        
        ModuleVersion other = (ModuleVersion) obj;
        return major == other.major &&
               minor == other.minor &&
               patch == other.patch &&
               Objects.equals(preRelease, other.preRelease) &&
               Objects.equals(buildMetadata, other.buildMetadata);
    }
    
    @Override
    public int hashCode() {
        return Objects.hash(major, minor, patch, preRelease, buildMetadata);
    }
    
    /**
     * 版本范围
     */
    public static class VersionRange {
        private final String expression;
        private final List<RangeCondition> conditions;
        
        public VersionRange(String expression) {
            this.expression = expression;
            this.conditions = parseExpression(expression);
        }
        
        /**
         * 检查版本是否在范围内
         */
        public boolean contains(ModuleVersion version) {
            return conditions.stream().allMatch(c -> c.test(version));
        }
        
        /**
         * 解析版本范围表达式
         * 支持的格式:
         * - ^1.2.3 (兼容版本)
         * - ~1.2.3 (相似版本)
         * - >=1.2.3
         * - >1.2.3
         * - <=1.2.3
         * - <1.2.3
         * - 1.2.3 - 2.0.0 (范围)
         * - 1.2.x (通配符)
         */
        private List<RangeCondition> parseExpression(String expr) {
            List<RangeCondition> result = new ArrayList<>();
            
            // 处理复合条件（用空格或逗号分隔）
            String[] parts = expr.split("[\\s,]+");
            
            for (int i = 0; i < parts.length; i++) {
                String part = parts[i].trim();
                if (part.isEmpty()) continue;
                
                // 处理范围表达式 (1.0.0 - 2.0.0)
                if (i + 2 < parts.length && parts[i + 1].equals("-")) {
                    ModuleVersion from = ModuleVersion.parse(part);
                    ModuleVersion to = ModuleVersion.parse(parts[i + 2]);
                    result.add(new RangeCondition(">=", from));
                    result.add(new RangeCondition("<=", to));
                    i += 2;
                    continue;
                }
                
                // 处理兼容版本 (^1.2.3)
                if (part.startsWith("^")) {
                    ModuleVersion v = ModuleVersion.parse(part.substring(1));
                    result.add(new RangeCondition(">=", v));
                    if (v.getMajor() > 0) {
                        result.add(new RangeCondition("<", new ModuleVersion(v.getMajor() + 1, 0, 0)));
                    } else if (v.getMinor() > 0) {
                        result.add(new RangeCondition("<", new ModuleVersion(0, v.getMinor() + 1, 0)));
                    } else {
                        result.add(new RangeCondition("<", new ModuleVersion(0, 0, v.getPatch() + 1)));
                    }
                    continue;
                }
                
                // 处理相似版本 (~1.2.3)
                if (part.startsWith("~")) {
                    ModuleVersion v = ModuleVersion.parse(part.substring(1));
                    result.add(new RangeCondition(">=", v));
                    result.add(new RangeCondition("<", new ModuleVersion(v.getMajor(), v.getMinor() + 1, 0)));
                    continue;
                }
                
                // 处理比较操作符
                Pattern opPattern = Pattern.compile("^(>=?|<=?|=)(.+)$");
                Matcher matcher = opPattern.matcher(part);
                if (matcher.matches()) {
                    String op = matcher.group(1);
                    ModuleVersion v = ModuleVersion.parse(matcher.group(2));
                    result.add(new RangeCondition(op, v));
                    continue;
                }
                
                // 处理通配符 (1.2.x)
                if (part.contains("x") || part.contains("*")) {
                    String[] vParts = part.split("\\.");
                    if (vParts.length >= 2 && !vParts[0].contains("x") && !vParts[1].contains("x")) {
                        int major = Integer.parseInt(vParts[0]);
                        int minor = Integer.parseInt(vParts[1]);
                        result.add(new RangeCondition(">=", new ModuleVersion(major, minor, 0)));
                        result.add(new RangeCondition("<", new ModuleVersion(major, minor + 1, 0)));
                    } else if (vParts.length >= 1 && !vParts[0].contains("x")) {
                        int major = Integer.parseInt(vParts[0]);
                        result.add(new RangeCondition(">=", new ModuleVersion(major, 0, 0)));
                        result.add(new RangeCondition("<", new ModuleVersion(major + 1, 0, 0)));
                    }
                    continue;
                }
                
                // 默认为精确匹配
                result.add(new RangeCondition("=", ModuleVersion.parse(part)));
            }
            
            return result;
        }
        
        @Override
        public String toString() {
            return expression;
        }
    }
    
    /**
     * 范围条件
     */
    private static class RangeCondition {
        private final String operator;
        private final ModuleVersion version;
        
        public RangeCondition(String operator, ModuleVersion version) {
            this.operator = operator;
            this.version = version;
        }
        
        public boolean test(ModuleVersion v) {
            int cmp = v.compareTo(version);
            
            switch (operator) {
                case "=":
                    return cmp == 0;
                case ">":
                    return cmp > 0;
                case ">=":
                    return cmp >= 0;
                case "<":
                    return cmp < 0;
                case "<=":
                    return cmp <= 0;
                default:
                    return false;
            }
        }
    }
}

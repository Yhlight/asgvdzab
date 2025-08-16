# 扫描器 V2 架构设计

## 核心设计理念

### 1. 语法使用范围限定

根据CHTL语法文档，严格限定CHTL语法的使用范围：

| 上下文 | CHTL语法支持 | 说明 |
|--------|--------------|------|
| 全局 style 块 | ✅ 完全支持 | 变量、样式组、原始嵌入 |
| 局部 style 块 | ✅ 完全支持 | 包括`&`等特有功能 |
| CHTL 主体 | ✅ 完全支持 | 元素、模板、配置等 |
| script 块 | ❌ 不支持 | 纯 JavaScript |
| CHTL JS | ❌ 不支持 | 仅支持CHTL JS特有语法 |

### 2. 动态块扫描机制

从字符级扫描升级为智能块扫描：

```
传统方式：逐字符扫描
H -> T -> M -> L -> 空格 -> { -> ...

新方式：动态块扫描
[HTML {] -> [style {内容}] -> [script {内容}]
```

### 3. 自适应截取策略

```java
class AdaptiveBlockScanner {
    // 基础块大小
    private static final int BASE_BLOCK_SIZE = 256;
    
    // 根据上下文动态调整块大小
    private int getBlockSize(Context context) {
        switch (context) {
            case STYLE_BLOCK:
                return 512;  // CSS规则通常较长
            case SCRIPT_BLOCK:
                return 1024; // JS代码块更大
            case CHTL_ELEMENT:
                return 256;  // CHTL元素较短
            default:
                return BASE_BLOCK_SIZE;
        }
    }
    
    // 智能边界检测，避免截断关键语法
    private int adjustBoundary(String content, int position) {
        // 检查是否在关键字中间
        if (isInKeyword(content, position)) {
            return findKeywordEnd(content, position);
        }
        
        // 检查是否在字符串中
        if (isInString(content, position)) {
            return findStringEnd(content, position);
        }
        
        // 检查是否在注释中
        if (isInComment(content, position)) {
            return findCommentEnd(content, position);
        }
        
        return position;
    }
}
```

## 实现方案

### 第一阶段：粗粒度扫描

```java
public class BlockScanner {
    public List<RawBlock> scanBlocks(String source) {
        List<RawBlock> blocks = new ArrayList<>();
        int position = 0;
        
        while (position < source.length()) {
            // 动态确定块大小
            int blockSize = determineBlockSize(source, position);
            
            // 提取块
            String blockContent = extractBlock(source, position, blockSize);
            
            // 识别块类型
            BlockType type = identifyBlockType(blockContent);
            
            blocks.add(new RawBlock(type, blockContent, position));
            position += blockContent.length();
        }
        
        return blocks;
    }
}
```

### 第二阶段：精细化处理

```java
public class BlockProcessor {
    public List<CodeFragment> processBlock(RawBlock block) {
        switch (block.getType()) {
            case STYLE:
                return processStyleBlock(block);
            case SCRIPT:
                return processScriptBlock(block);
            case CHTL:
                return processCHTLBlock(block);
            default:
                return Collections.singletonList(
                    new CodeFragment(FragmentType.UNKNOWN, block.getContent())
                );
        }
    }
    
    private List<CodeFragment> processStyleBlock(RawBlock block) {
        // 在style块中识别CHTL语法
        StyleProcessor processor = new StyleProcessor();
        return processor.process(block.getContent());
    }
    
    private List<CodeFragment> processScriptBlock(RawBlock block) {
        // script块作为整体，不识别CHTL语法
        return Collections.singletonList(
            new CodeFragment(FragmentType.JS, block.getContent())
        );
    }
}
```

### 第三阶段：CHTL语法识别

```java
public class StyleProcessor {
    private static final Pattern VAR_FUNCTION = Pattern.compile(
        "([A-Z][a-zA-Z0-9_]*?)\\s*\\("
    );
    
    private static final Pattern STYLE_TEMPLATE = Pattern.compile(
        "@Style\\s+([A-Z][a-zA-Z0-9_]*?)\\s*;"
    );
    
    private static final Pattern ORIGIN_BLOCK = Pattern.compile(
        "\\[Origin\\]\\s*@Style"
    );
    
    public List<CodeFragment> process(String styleContent) {
        List<CodeFragment> fragments = new ArrayList<>();
        
        // 使用正则表达式快速定位CHTL语法
        Matcher varMatcher = VAR_FUNCTION.matcher(styleContent);
        Matcher styleMatcher = STYLE_TEMPLATE.matcher(styleContent);
        Matcher originMatcher = ORIGIN_BLOCK.matcher(styleContent);
        
        // 构建语法位置映射
        TreeMap<Integer, SyntaxInfo> syntaxMap = new TreeMap<>();
        
        while (varMatcher.find()) {
            syntaxMap.put(varMatcher.start(), 
                new SyntaxInfo(SyntaxType.VARIABLE, varMatcher.group()));
        }
        
        // ... 处理其他语法
        
        // 根据语法位置切割片段
        return splitBySyntax(styleContent, syntaxMap);
    }
}
```

## 性能优化

### 1. 缓存机制

```java
public class ScannerCache {
    // 缓存已扫描的块
    private final Map<String, List<CodeFragment>> blockCache = 
        new ConcurrentHashMap<>();
    
    // 缓存正则表达式匹配结果
    private final Map<String, List<SyntaxInfo>> syntaxCache = 
        new ConcurrentHashMap<>();
}
```

### 2. 并行处理

```java
public class ParallelScanner {
    private final ExecutorService executor = 
        ForkJoinPool.commonPool();
    
    public CompletableFuture<List<CodeFragment>> scanAsync(String source) {
        // 将源代码分割为独立的顶级块
        List<String> topLevelBlocks = splitTopLevel(source);
        
        // 并行处理每个块
        List<CompletableFuture<List<CodeFragment>>> futures = 
            topLevelBlocks.stream()
                .map(block -> CompletableFuture.supplyAsync(
                    () -> scanBlock(block), executor))
                .collect(Collectors.toList());
        
        // 合并结果
        return CompletableFuture.allOf(
            futures.toArray(new CompletableFuture[0]))
            .thenApply(v -> futures.stream()
                .map(CompletableFuture::join)
                .flatMap(List::stream)
                .collect(Collectors.toList()));
    }
}
```

## 文件扩展名处理

```java
public class FileTypeDetector {
    public FileType detectType(String filename) {
        String extension = getExtension(filename);
        
        switch (extension.toLowerCase()) {
            case "chtl":
                return FileType.CHTL;
            case "chtljs":
                return FileType.CHTL_JS;
            case "cmod":
                return FileType.CMOD;
            case "cjmod":
                return FileType.CJMOD;
            default:
                return FileType.UNKNOWN;
        }
    }
    
    public ScannerConfig getConfig(FileType type) {
        switch (type) {
            case CHTL:
                return CHTLScannerConfig.DEFAULT;
            case CHTL_JS:
                return CHTLJSScannerConfig.DEFAULT;
            default:
                return BasicScannerConfig.DEFAULT;
        }
    }
}
```

## 预期效果

### 1. 性能提升
- 块扫描比字符扫描快 5-10 倍
- 并行处理进一步提升 2-3 倍
- 缓存机制减少重复计算

### 2. 准确性提升
- 避免CSS函数误判（translateY问题解决）
- 上下文感知更精确
- 语法边界识别更准确

### 3. 可维护性提升
- 模块化设计
- 清晰的职责分离
- 易于扩展新语法
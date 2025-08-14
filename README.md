# CHTL Compiler (Java 17)

- Build: `mvn -q -e -DskipTests package`
- Run: `java -jar target/chtl-compiler-0.1.0-SNAPSHOT-shaded.jar -o out.html input.chtl`

架构：Scanner -> CompilerDispatcher -> {CHTL, CHTL JS, CSS, JS} -> ResultMerger -> HTML 输出。

## 元素模板继承与特例化

```chtl
[Template] @Element Base {
  div{ text{base} }
}

[Template] @Element Child {
  inherit @Element Base;
  span{ text{child} }
}

body{
  @Element Child {
    insert after div { p{ text{after div} } }
    delete div[0];
  }
}
```

- 继承顺序：父在前、子在后；支持多级继承与循环检测（严格模式报错）。
- 特例化：insert before/after/replace/at top/at bottom；delete tag[n] 或 delete @Element Name。

## 局部 style 使用模板

```chtl
body{
  style{
    @Style MyStyle;
  }
}
```

- @Style 支持 include/inherit/delete 顺序展开；属性最后写入元素 `style`。

## 严格模式

```bash
java -jar target/chtl-compiler-0.1.0-SNAPSHOT-shaded.jar --strict -o out.html input.chtl
```

- 解析或约束/特例化目标未命中将退出码 2，并输出错误信息。

## Import 规则（增强）

- @Html/@Style/@JavaScript
  - 无 `as`：跳过。
  - 有 `as`：读取资源并生成带名原始嵌入节点（Html 注入 body、Style 注入全局 CSS、JavaScript 注入脚本）。
  - 路径：
    - 仅文件名（不带后缀）：在输入文件所在目录按类型后缀补全（.html/.css/.js），不递归。
    - 具体文件名（带后缀）：在输入文件所在目录查找该文件。
    - 如果给的是文件夹：报错（严格模式）。

- @Chtl（模块/子模块导入）
  - 模块名点号映射路径：`Chtholly.Space` 等同 `Chtholly/Space`；`Chtholly.*` 等同 `Chtholly/*`。
  - 搜索优先级：官方 `module` → 当前目录下 `module` → 当前目录；`cmod` 优先于 `chtl`。
  - 名称（不带后缀）：按优先级查找 `name.cmod`，否则 `name.chtl`。
  - 指定后缀名：按优先级查找具体文件。
  - 指定路径：
    - `.*`/`/*`/`*.cmod`/`*.chtl`：导入匹配的全部文件（仅当前层级）。
    - 具体文件：直接导入。
    - 目录或不含文件信息：报错（严格模式）。
  - 去重与循环：同一路径不同写法去重；循环依赖保护（已导入即跳过）。
  - 命名空间：同名命名空间合并；模板命名冲突严格模式报错。

- @CJmod
  - 搜索优先级：官方 `module` → 当前目录下 `module` → 当前目录，仅 `.cjmod`。
  - 指定路径须为具体文件；目录或不含文件信息报错（严格模式）。
  - 导入为命名空间下的原始 JavaScript 嵌入。

- 示例

```chtl
[Import] @Html from card as CardHtml;
[Import] @Style from theme as ThemeStyle;
[Import] @JavaScript from util as UtilJs;
[Import] @Chtl from Chtholly.Space;     // 点号映射到路径
[Import] @Chtl from modules.*.cmod;      // 批量导入 cmod
[Import] @Chtl from lib/*;               // 批量导入 cmod 与 chtl
[Import] @CJmod from Anim.Core;          // Anim/Core.cjmod
```

- 错误提示（严格模式）
  - 资源未找到 / 路径为目录 / 路径不含文件信息：精确报错并定位。
  - 模板命名冲突：列出冲突名与命名空间。

## CLI 示例

```bash
# 严格模式 + 输出到 out.html
java -jar target/chtl-compiler-0.1.0-SNAPSHOT-shaded.jar --strict -o out.html input.chtl

# 打印扫描片段
java -jar target/chtl-compiler-0.1.0-SNAPSHOT-shaded.jar --dump-fragments -o out.html input.chtl

# 动态关键字（在 input.chtl 顶部添加）
# [Configuration] @NameGroup{ KEYWORD_ORIGIN: Or,H; ORIGIN_HTML: @H; }

# 通配导入模块
# [Import] @Chtl from lib/*.cmod; 或 [Import] @Chtl from lib/*

# 命名空间引用模板
# body{ @Element Card from MyNs; }
```

## 常见错误与修复

- 资源未找到
  - 现象：严格模式报错 “资源未找到: <path> @line:col”。
  - 原因：路径不含文件信息或文件不存在。
  - 修复：确保提供具体文件，或按规则放置在 module/ 或当前目录。
  - 截图：docs/errors/not-found.png（占位）

- 路径是目录
  - 现象：严格模式报错 “路径不含文件信息: <path>”。
  - 修复：改为具体文件，或使用通配 `*.cmod`/`*.chtl`。
  - 截图：docs/errors/dir-path.png（占位）

- 模板命名冲突
  - 现象：严格模式报错 “命名冲突: <namespace.template>”。
  - 原因：通配导入多个文件中定义了同名模板。
  - 修复：重命名模板或划分命名空间；或在导入时使用 alias。
  - 截图：docs/errors/conflict.png（占位）

- 循环导入
  - 现象：未死循环但内容重复或被跳过。
  - 修复：检查导入链路；工具已做去重与循环保护，避免重复导入同一路径。
  - 截图：docs/errors/cycle.png（占位）

## 示例：通配导入与命名空间引用

```chtl
[Import] @Chtl from ui/*.cmod;        // 导入 ui 目录下所有 cmod
[Import] @Chtl from lib/*;            // 导入 lib 目录下所有 cmod 与 chtl
[Import] @Chtl from MyNs/*.chtl;      // 导入命名空间 MyNs 下所有 chtl 文件

body{
  @Element Button from MyNs;
}
```

### 小结
- 通配默认仅当前层级，不递归；按 cmod 优先再 chtl。
- 命名空间默认取父目录名或 alias；引用时使用 `from Namespace`。
- 循环与重复导入自动去重；严格模式下资源与路径错误立即失败并包含行列定位。
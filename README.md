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
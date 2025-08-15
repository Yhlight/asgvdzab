# TODO（严格依据 CHTL语法文档.md）

- 词法
  - [ ] CHTL：补全 [Template]/[Custom]/[Origin]/[Import]/[Namespace]/[Configuration] 相关的词法上下文切换与必要标记组合识别（不扩展语法）
  - [ ] CHTL JS：在 {{...}} 内部的选择器词法优化（保留文档限定：.class、#id、tag、后代、索引）

- 解析器（CHTL）
  - [ ] 元素节点递归解析（已初版）：标签名、子元素、text/style/script 局部块
  - [ ] 局部样式块：内联样式（属性: 值;）与 .class/#id/& 相关片段识别并分类（全局/局部）
  - [ ] 文本节点：无修饰字面量与字符串
  - [ ] [Template]：@Style/@Element/@Var 定义与 inherit 继承链
  - [ ] [Custom]：@Style/@Element/@Var 特例化（delete/insert/replace 等）
  - [ ] [Origin]：@Html/@Style/@JavaScript 原始嵌入内容块
  - [ ] [Import]：@Html/@Style/@JavaScript/@Chtl/@CJmod 语句（from/as）
  - [ ] [Namespace]：命名空间与嵌套命名空间块
  - [ ] except 约束的三类（精确/类型/全局）
  - [ ] [Configuration] 与 [Name] 子块（覆盖 GlobalMap）

- 解析器（CHTL JS）
  - [ ] 增强选择器 {{...}}：.class、#id、tag、后代、button[0] 等
  - [ ] 链式访问 -> 和 .
  - [ ] listen({ ... })：事件与处理器映射
  - [ ] delegate({ target: 单个/数组, 事件... })
  - [ ] animate({ duration/easing/begin/when/end/loop/direction/delay/callback })

- 生成器（CHTL → HTML/CSS/JS）
  - [ ] 元素与文本输出（已初版）
  - [ ] 局部样式块：内联样式输出；.class/#id/& 选择器内容汇总到全局样式
  - [ ] [Template]/[Custom] 展开与应用（按出现顺序覆盖）
  - [ ] [Origin] 原样注入
  - [ ] [Import] 资源合并（路径解析到打包器）
  - [ ] [Namespace] 名称限定
  - [ ] except 约束校验
  - [ ] [Configuration]/[Name] 覆盖到 GlobalMap

- 生成器（CHTL JS → JavaScript）
  - [ ] 选择器到 DOM API：#id → getElementById；其余 → querySelector/All（遵循文档优先级）
  - [ ] 链式方法与参数透传
  - [ ] listen/delegate/animate 的对象参数转译

- 测试
  - [ ] 覆盖文档所有示例（正向/负向）
  - [ ] 性能与错误恢复策略（不改变语法）
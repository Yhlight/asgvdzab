# CHTL 实施计划（8 次工作）

- 第1次：基础设施与RAII状态机（已完成）
  - 词法器（CHTL/CHTL JS）最小实现，关键字受 GlobalMap 管控
  - AST 基类与核心节点（文档涉及的节点骨架）
  - 解析器骨架（元素/text/style/script）
  - 生成器骨架（HTML/JS 基本生成）
  - RAII 状态机与上下文协助器并接入解析/生成
  - 冒烟测试

- 第2次：元素与文本解析完善（进行中）
  - 元素嵌套与属性字面量解析（内联样式属性：键值对与分号）
  - 文本节点字符串与无修饰字面量差异处理

- 第3次：局部样式块完善
  - style{} 内属性收集、.class/#id/& 处理（全局/局部归类规则）
  - & 的上下文推导（优先类名，再次为 id）

- 第4次：[Template] 支持
  - @Style/@Element/@Var 定义与使用
  - inherit 明确继承链处理；组合式继承按顺序覆盖

- 第5次：[Custom] 支持
  - @Style/@Element/@Var 的 delete/insert/replace/特例化
  - 组件内特定元素样式追加

- 第6次：[Origin]/[Import]/[Namespace]
  - 原始嵌入三个类型
  - Import 家族及 from/as、命名空间及嵌套

- 第7次：约束 except 与 [Configuration]/[Name]
  - except 三类（精确/类型/全局）
  - 配置组覆盖 GlobalMap 与 Name 集

- 第8次：CHTL JS 完整化
  - 选择器、链式访问、listen、delegate、animate 到 JS 的生成
  - 与扫描器耦合的上下文，保证 JS/CSS 宽判下的分割点精确
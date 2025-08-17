# CHTL项目仓库清理计划

## 🚨 当前状况分析

项目仓库确实需要彻底清理！现状问题：

### 😱 主要问题
1. **编译产物污染**：大量`.o`、可执行文件散落在根目录
2. **测试文件泛滥**：20+个测试文件和可执行文件
3. **重复的构建目录**：`build/`、`build-chtl/`、`antlr-4.13.1-build/`等
4. **临时文件残留**：`core`、`debug_test`等调试文件
5. **文档散乱**：多个重复的实现报告
6. **ANTLR相关混乱**：多个版本的ANTLR目录和文件
7. **命名不一致**：新旧命名风格混合

### 📊 需要清理的文件统计
- **可执行文件**: 20+ 个 (总计 ~50MB)
- **测试源码**: 15+ 个 `.cpp` 文件
- **重复文档**: 8+ 个 `.md` 报告文件
- **构建目录**: 5+ 个不同的构建文件夹
- **ANTLR文件**: 10+ 个相关目录和文件

## 🗂️ 清理策略

### 第一步：保留重要文件
```
保留列表:
✅ CHTL语法文档.md           # 核心语法文档
✅ LICENSE.txt              # 许可证
✅ README.md               # 项目说明
✅ VERSION                 # 版本文件
✅ .gitignore              # Git忽略规则
✅ CMakeLists.txt          # 主构建文件

保留目录:
✅ .git/                   # Git仓库
✅ Include/                # 新架构头文件
✅ CHTL/                   # 新架构CHTL模块
✅ CHTLJS/                 # 新架构CHTL JS模块
✅ src/                    # 当前源码（待迁移）
✅ include/                # 当前头文件（待迁移）
```

### 第二步：删除废弃文件
```
删除的可执行文件:
❌ test_* (所有测试可执行文件)
❌ *_test (所有测试可执行文件)
❌ core
❌ debug_test

删除的测试源码:
❌ test_*.cpp (迁移有价值的到Tests/目录)

删除的重复文档:
❌ FINAL_IMPLEMENTATION_REPORT.md
❌ ENHANCED_IMPORT_IMPLEMENTATION_REPORT.md
❌ CHTL_IMPLEMENTATION_REPORT.md
❌ CHTL_*_CONSTRAINTS_SUMMARY.md (5个约束总结文档)

删除的构建相关:
❌ build/
❌ build-chtl/
❌ antlr-4.13.1-build/
❌ generated/

删除的ANTLR混乱:
❌ antlr-prebuilt/
❌ antlr4-linux/
❌ antlr-4.13.1-runtime/
❌ antlr-quick/
❌ Antlr4/
❌ antlr4-source/
❌ antlr4-cpp-runtime-4.9.2-source.zip

删除的临时文件:
❌ test.html
❌ test.chtl
❌ test_slice.*
❌ CMakeLists.txt.antlr
```

### 第三步：重新组织保留内容
```
新的目录结构:
├── LICENSE.txt
├── README.md
├── VERSION
├── .gitignore
├── CMakeLists.txt
├── CHTL语法文档.md
│
├── Include/           # 新架构头文件
├── CHTL/             # CHTL编译器实现
├── CHTLJS/           # CHTL JS编译器实现
│
├── Tests/            # 清理后的测试
│   ├── CHTL/
│   ├── CHTLJS/
│   └── Integration/
│
├── Examples/         # 示例代码
├── Documentation/    # 统一文档
├── Tools/           # 开发工具
├── ThirdParty/      # 第三方依赖（整理后的ANTLR）
│
└── Build/           # 构建相关
    ├── CMake/
    └── Scripts/
```

## 🧹 清理执行计划

### Phase 1: 立即删除明显废弃文件
1. 删除所有可执行文件
2. 删除重复的构建目录
3. 删除混乱的ANTLR相关文件
4. 删除重复的文档

### Phase 2: 迁移有价值内容
1. 提取有价值的测试代码到Tests/
2. 整理文档到Documentation/
3. 保留一个干净的ANTLR版本到ThirdParty/

### Phase 3: 重建项目结构
1. 完善新的目录架构
2. 更新CMakeLists.txt
3. 更新.gitignore
4. 创建新的README.md

### Phase 4: 验证清理结果
1. 确保新架构完整
2. 验证构建系统
3. 确认无重要文件丢失

## 📋 清理检查清单

- [ ] 删除20+个可执行文件
- [ ] 删除15+个测试源码文件（保留有价值的）
- [ ] 删除8+个重复文档
- [ ] 删除5+个构建目录
- [ ] 清理10+个ANTLR相关混乱
- [ ] 重新组织目录结构
- [ ] 更新构建系统
- [ ] 更新文档
- [ ] 验证新架构完整性

## 🎯 清理后的预期效果

### 文件数量减少
- 根目录文件：从80+个减少到10个以内
- 总体文件大小：减少~100MB+
- 目录层次：更加清晰和标准化

### 质量提升
- 清晰的模块分离
- 标准的命名规范
- 专业的项目结构
- 易于维护和扩展

让我们开始执行这个清理计划！
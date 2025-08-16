# ANTLR4 预编译包

## 📦 包含内容

这个预编译包包含了完整的ANTLR4工具链，可以直接使用，无需编译：

- **ANTLR4工具** (v4.13.1): 用于从语法文件生成C++代码
- **C++运行时库**: 静态库和动态库版本
- **完整头文件**: 所有必需的C++头文件
- **CMake配置**: 开箱即用的CMake集成

## 📊 包结构

```
antlr-prebuilt/
├── bin/                    # 可执行工具
│   ├── antlr4.jar         # ANTLR4工具 (Java)
│   ├── antlr4             # Linux/Mac启动脚本
│   └── antlr4.bat         # Windows启动脚本
├── lib/                    # 运行时库文件
│   ├── libantlr4-runtime.a   # 静态库 (11MB)
│   └── libantlr4-runtime.so  # 动态库 (4.2MB)
├── include/                # C++头文件
│   ├── antlr4-runtime.h   # 主头文件
│   ├── tree/              # 解析树相关
│   ├── atn/               # ATN (增强转换网络)
│   └── ...                # 其他头文件
├── ANTLR4Config.cmake      # CMake配置文件
└── README.md               # 本文件
```

## 🚀 快速开始

### 1. 检查环境要求

- **Java 8+**: 运行ANTLR4工具
- **C++17编译器**: GCC 7+, Clang 5+, Visual Studio 2017+
- **CMake 3.10+**: 构建系统 (可选)

### 2. 验证安装

```bash
# 测试ANTLR4工具
java -jar bin/antlr4.jar

# 或使用启动脚本
./bin/antlr4

# Windows
bin\antlr4.bat
```

### 3. 生成代码示例

```bash
# 从语法文件生成C++代码
./bin/antlr4 MyGrammar.g4 -Dlanguage=Cpp -o generated/

# 生成监听器模式 (推荐)
./bin/antlr4 MyGrammar.g4 -Dlanguage=Cpp -listener -o generated/

# 生成访问者模式
./bin/antlr4 MyGrammar.g4 -Dlanguage=Cpp -visitor -o generated/
```

## 🔧 CMake集成

### 方法1: 使用预配置的CMake文件

```cmake
# 设置ANTLR4路径
set(CMAKE_PREFIX_PATH "/path/to/antlr-prebuilt" ${CMAKE_PREFIX_PATH})

# 查找ANTLR4包
find_package(ANTLR4 REQUIRED)

# 链接到你的目标
target_link_libraries(your_target ANTLR4::antlr4_runtime)
```

### 方法2: 使用辅助函数自动生成代码

```cmake
# 包含ANTLR4配置
include(/path/to/antlr-prebuilt/ANTLR4Config.cmake)

# 自动生成代码并创建库
antlr4_generate(
    TARGET my_grammar_lib
    GRAMMAR_FILES grammar/MyGrammar.g4
    PACKAGE com::example
)

# 使用生成的库
target_link_libraries(your_target my_grammar_lib)
```

### 方法3: 手动配置

```cmake
# 设置路径
set(ANTLR4_ROOT "/path/to/antlr-prebuilt")
set(ANTLR4_INCLUDE_DIR "${ANTLR4_ROOT}/include")
set(ANTLR4_LIBRARY_DIR "${ANTLR4_ROOT}/lib")

# 查找库
find_library(ANTLR4_LIB 
    NAMES antlr4-runtime
    PATHS ${ANTLR4_LIBRARY_DIR}
    NO_DEFAULT_PATH
)

# 配置目标
target_include_directories(your_target PRIVATE ${ANTLR4_INCLUDE_DIR})
target_link_libraries(your_target ${ANTLR4_LIB})
```

## 💻 C++代码示例

### 基本使用模式

```cpp
#include "antlr4-runtime.h"
#include "MyGrammarLexer.h"
#include "MyGrammarParser.h"
#include "MyGrammarBaseListener.h"

using namespace antlr4;

int main() {
    // 创建输入流
    ANTLRInputStream input("your input text");
    
    // 创建词法分析器
    MyGrammarLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    
    // 创建解析器
    MyGrammarParser parser(&tokens);
    
    // 解析 (startRule是你的起始规则)
    auto tree = parser.startRule();
    
    // 使用监听器遍历解析树
    MyGrammarBaseListener listener;
    tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);
    
    return 0;
}
```

### 错误处理

```cpp
#include "antlr4-runtime.h"

class MyErrorListener : public BaseErrorListener {
public:
    void syntaxError(Recognizer *recognizer, Token *offendingSymbol,
                     size_t line, size_t charPositionInLine,
                     const std::string &msg, std::exception_ptr e) override {
        std::cerr << "语法错误 第" << line << "行第" << charPositionInLine 
                  << "列: " << msg << std::endl;
    }
};

// 使用自定义错误监听器
MyGrammarParser parser(&tokens);
MyErrorListener errorListener;
parser.removeErrorListeners();
parser.addErrorListener(&errorListener);
```

## 🎯 与CHTL项目集成

### 更新CHTL项目的CMakeLists.txt

```cmake
# 在CHTL项目中使用预编译包
set(ANTLR4_ROOT "/path/to/antlr-prebuilt")
include(${ANTLR4_ROOT}/ANTLR4Config.cmake)

# 更新现有的ANTLR查找逻辑
if(ANTLR4_FOUND)
    message(STATUS "✓ 使用ANTLR4预编译包")
    set(ANTLR4_LIB ${ANTLR4_LIBRARIES})
    set(ANTLR4_INCLUDE_DIRS ${ANTLR4_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "未找到ANTLR4预编译包")
endif()

# 链接到CHTL编译器
target_link_libraries(chtl_compiler ${ANTLR4_LIBRARIES})
target_include_directories(chtl_compiler PRIVATE ${ANTLR4_INCLUDE_DIRS})
```

### 重新生成CSS和JavaScript解析器

```bash
# 进入CHTL项目目录
cd /path/to/chtl-project

# 使用预编译包生成代码
/path/to/antlr-prebuilt/bin/antlr4 grammars/CSS3.g4 \
    -Dlanguage=Cpp -listener -o generated/antlr/css/

/path/to/antlr-prebuilt/bin/antlr4 grammars/JavaScript.g4 \
    -Dlanguage=Cpp -listener -o generated/antlr/js/
```

## 🔍 故障排除

### 常见问题

#### 1. "java: command not found"
```bash
# 安装Java
sudo apt-get install openjdk-11-jdk  # Ubuntu/Debian
brew install openjdk@11              # macOS
```

#### 2. "undefined reference to antlr4"
```bash
# 确保链接了正确的库
target_link_libraries(your_target ${ANTLR4_ROOT}/lib/libantlr4-runtime.a)
```

#### 3. "fatal error: antlr4-runtime.h: No such file"
```cmake
# 添加包含目录
target_include_directories(your_target PRIVATE ${ANTLR4_ROOT}/include)
```

#### 4. "version mismatch"
```bash
# 检查ANTLR工具和运行时版本
java -jar bin/antlr4.jar 2>&1 | grep "Version"
# 应显示: ANTLR Parser Generator Version 4.13.1
```

### 性能优化

#### 使用静态库 (推荐)
```cmake
# 优先使用静态库减少运行时依赖
find_library(ANTLR4_LIB NAMES libantlr4-runtime.a antlr4-runtime)
```

#### 多线程支持
```cpp
// ANTLR4运行时是线程安全的，可以并行解析
#include <thread>
#include <future>

std::vector<std::future<void>> futures;
for (const auto& input : inputs) {
    futures.push_back(std::async(std::launch::async, [&input]() {
        // 解析逻辑
    }));
}
```

## 📚 相关链接

- [ANTLR4官方文档](https://github.com/antlr/antlr4/blob/master/doc/index.md)
- [C++运行时API文档](https://github.com/antlr/antlr4/blob/master/runtime/Cpp/README.md)
- [语法文件示例](https://github.com/antlr/grammars-v4)

## 📄 版本信息

- **ANTLR版本**: 4.13.1
- **C++标准**: C++17
- **编译平台**: Linux x86_64
- **编译器**: GCC 14
- **打包日期**: $(date)

## 📞 支持

如果遇到问题，请检查：
1. Java环境是否正确安装
2. 库文件路径是否正确
3. C++编译器是否支持C++17
4. CMake版本是否满足要求

---

*这个预编译包由CHTL项目自动生成，包含完整的ANTLR4工具链。*
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Isrc
LDFLAGS = 

# 源文件
SOURCES = src/main.cpp \
          src/utils/CommandLineParser.cpp \
          src/scanner/CHTLUnifiedScanner.cpp \
          src/dispatcher/CompilerDispatcher.cpp \
          src/chtl/CHTLCompiler.cpp \
          src/chtl/CHTLToken.cpp \
          src/chtl/CHTLGlobalMap.cpp \
          src/chtl/CHTLState.cpp \
          src/chtl/CHTLContext.cpp \
          src/chtl/CHTLLexer.cpp \
          src/chtl/ast/CHTLASTNodes.cpp \
          src/chtl/ast/DefaultASTVisitor.cpp \
          src/chtljs/CHTLJSCompiler.cpp \
          src/chtljs/ast/CHTLJSASTNodes.cpp \
          src/css/CSSCompiler.cpp \
          src/javascript/JavaScriptCompiler.cpp

# 目标文件
OBJECTS = $(SOURCES:.cpp=.o)

# 可执行文件
TARGET = chtl

# 默认目标
all: $(TARGET)

# 链接
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# 编译
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理
clean:
	rm -f $(OBJECTS) $(TARGET)

# 安装
install: $(TARGET)
	mkdir -p /usr/local/bin
	cp $(TARGET) /usr/local/bin/

.PHONY: all clean install
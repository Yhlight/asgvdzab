# FindANTLR4.cmake - 查找ANTLR4库
# 为简化运行时设计

set(ANTLR4_FOUND TRUE)
set(ANTLR4_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")
set(ANTLR4_LIBRARIES "")  # 头文件库，无需链接

message(STATUS "Found ANTLR4 (简化版本): ${ANTLR4_INCLUDE_DIRS}")

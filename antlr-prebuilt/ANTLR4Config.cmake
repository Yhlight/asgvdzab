# ANTLR4 预编译包 CMake 配置文件
# 版本: 4.13.1
# 编译日期: $(date)

# 获取包路径
get_filename_component(ANTLR4_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

# 设置路径
set(ANTLR4_INCLUDE_DIRS "${ANTLR4_ROOT_DIR}/include")
set(ANTLR4_LIBRARY_DIRS "${ANTLR4_ROOT_DIR}/lib")
set(ANTLR4_BINARY_DIR "${ANTLR4_ROOT_DIR}/bin")

# 查找库文件
find_library(ANTLR4_STATIC_LIB
    NAMES antlr4-runtime
    PATHS ${ANTLR4_LIBRARY_DIRS}
    NO_DEFAULT_PATH
)

find_library(ANTLR4_SHARED_LIB
    NAMES antlr4-runtime
    PATHS ${ANTLR4_LIBRARY_DIRS}
    NO_DEFAULT_PATH
)

# 设置库变量
if(ANTLR4_STATIC_LIB)
    set(ANTLR4_LIBRARIES ${ANTLR4_STATIC_LIB})
    set(ANTLR4_LIBRARY ${ANTLR4_STATIC_LIB})
elseif(ANTLR4_SHARED_LIB)
    set(ANTLR4_LIBRARIES ${ANTLR4_SHARED_LIB})
    set(ANTLR4_LIBRARY ${ANTLR4_SHARED_LIB})
endif()

# 设置ANTLR4工具路径
set(ANTLR4_JAR_LOCATION "${ANTLR4_BINARY_DIR}/antlr4.jar")

# 检查是否找到所有组件
if(ANTLR4_LIBRARIES AND ANTLR4_INCLUDE_DIRS AND EXISTS ${ANTLR4_JAR_LOCATION})
    set(ANTLR4_FOUND TRUE)
    message(STATUS "Found ANTLR4 (预编译版本)")
    message(STATUS "  版本: 4.13.1")
    message(STATUS "  包含目录: ${ANTLR4_INCLUDE_DIRS}")
    message(STATUS "  库文件: ${ANTLR4_LIBRARIES}")
    message(STATUS "  工具JAR: ${ANTLR4_JAR_LOCATION}")
else()
    set(ANTLR4_FOUND FALSE)
    message(FATAL_ERROR "未找到完整的ANTLR4预编译包")
endif()

# 定义ANTLR4::antlr4_runtime目标
if(ANTLR4_FOUND AND NOT TARGET ANTLR4::antlr4_runtime)
    if(ANTLR4_STATIC_LIB)
        add_library(ANTLR4::antlr4_runtime STATIC IMPORTED)
        set_target_properties(ANTLR4::antlr4_runtime PROPERTIES
            IMPORTED_LOCATION "${ANTLR4_STATIC_LIB}"
        )
    elseif(ANTLR4_SHARED_LIB)
        add_library(ANTLR4::antlr4_runtime SHARED IMPORTED)
        set_target_properties(ANTLR4::antlr4_runtime PROPERTIES
            IMPORTED_LOCATION "${ANTLR4_SHARED_LIB}"
        )
    endif()
    
    set_target_properties(ANTLR4::antlr4_runtime PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${ANTLR4_INCLUDE_DIRS}"
    )
endif()

# 辅助函数：添加ANTLR4语法文件
function(antlr4_generate)
    cmake_parse_arguments(
        ANTLR4
        ""
        "TARGET;NAMESPACE;PACKAGE"
        "GRAMMAR_FILES;DEPENDS"
        ${ARGN}
    )
    
    if(NOT ANTLR4_TARGET)
        message(FATAL_ERROR "antlr4_generate: TARGET 参数是必需的")
    endif()
    
    if(NOT ANTLR4_GRAMMAR_FILES)
        message(FATAL_ERROR "antlr4_generate: GRAMMAR_FILES 参数是必需的")
    endif()
    
    set(ANTLR4_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/antlr4_generated")
    
    foreach(GRAMMAR_FILE ${ANTLR4_GRAMMAR_FILES})
        get_filename_component(GRAMMAR_NAME ${GRAMMAR_FILE} NAME_WE)
        
        set(ANTLR4_COMMAND 
            java -jar ${ANTLR4_JAR_LOCATION}
            -Dlanguage=Cpp
            -o ${ANTLR4_OUTPUT_DIR}
            ${GRAMMAR_FILE}
        )
        
        if(ANTLR4_PACKAGE)
            list(APPEND ANTLR4_COMMAND -package ${ANTLR4_PACKAGE})
        endif()
        
        # 预期的输出文件
        set(GENERATED_FILES
            "${ANTLR4_OUTPUT_DIR}/${GRAMMAR_NAME}Lexer.cpp"
            "${ANTLR4_OUTPUT_DIR}/${GRAMMAR_NAME}Lexer.h"
            "${ANTLR4_OUTPUT_DIR}/${GRAMMAR_NAME}Parser.cpp"
            "${ANTLR4_OUTPUT_DIR}/${GRAMMAR_NAME}Parser.h"
            "${ANTLR4_OUTPUT_DIR}/${GRAMMAR_NAME}BaseListener.cpp"
            "${ANTLR4_OUTPUT_DIR}/${GRAMMAR_NAME}BaseListener.h"
        )
        
        add_custom_command(
            OUTPUT ${GENERATED_FILES}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${ANTLR4_OUTPUT_DIR}
            COMMAND ${ANTLR4_COMMAND}
            DEPENDS ${GRAMMAR_FILE} ${ANTLR4_DEPENDS}
            COMMENT "生成ANTLR4代码: ${GRAMMAR_FILE}"
        )
        
        list(APPEND ALL_GENERATED_FILES ${GENERATED_FILES})
    endforeach()
    
    # 创建目标
    add_library(${ANTLR4_TARGET} ${ALL_GENERATED_FILES})
    target_include_directories(${ANTLR4_TARGET} PUBLIC ${ANTLR4_OUTPUT_DIR})
    target_link_libraries(${ANTLR4_TARGET} PUBLIC ANTLR4::antlr4_runtime)
    
    if(ANTLR4_NAMESPACE)
        target_compile_definitions(${ANTLR4_TARGET} PRIVATE 
            ANTLR4_NAMESPACE=${ANTLR4_NAMESPACE}
        )
    endif()
    
endfunction()

# 版本信息
set(ANTLR4_VERSION "4.13.1")
set(ANTLR4_VERSION_MAJOR 4)
set(ANTLR4_VERSION_MINOR 13)
set(ANTLR4_VERSION_PATCH 1)
cmake_minimum_required(VERSION 3.16)
project(CHTL_Compiler VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
if(MSVC)
    add_compile_options(/W4 /permissive-)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
    else()
        add_compile_options(-O3 -DNDEBUG)
    endif()
endif()

# 包含目录
include_directories(include)

# 查找ANTLR4（如果可用）
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(ANTLR4 QUIET antlr4-runtime)
endif()

# 设置源文件
set(CORE_SOURCES
    src/core/types.cpp
    src/core/lexer.cpp
    src/core/ast_nodes.cpp
    src/core/ast_visitor.cpp
    src/core/state_context.cpp
    src/core/import_manager.cpp
    src/core/namespace_manager.cpp
    src/core/constraint_system.cpp
    src/core/chtl_parser.cpp
    src/core/style_block_parser.cpp
    src/core/template_parser.cpp
    src/core/custom_parser.cpp
)

set(UTILS_SOURCES
    src/utils/simple_zip.cpp
)

set(MODULE_SOURCES
    src/module/module_manager.cpp
)

set(SCANNER_SOURCES
    src/scanner/unified_scanner.cpp
)

set(COMPILER_SOURCES
    src/compiler/compiler_interface.cpp
    src/compiler/compiler_dispatcher.cpp
    src/compiler/chtl_compiler.cpp
    src/compiler/chtl_js_compiler.cpp
)

# CSS和JavaScript编译器（使用ANTLR或fallback）
if(ANTLR4_FOUND)
    set(CSS_JS_SOURCES
        src/compiler/css_compiler_antlr.cpp
        src/compiler/js_compiler_antlr.cpp
    )
    add_compile_definitions(CHTL_USE_ANTLR)
    message(STATUS "Using ANTLR4 for CSS/JavaScript compilation")
else()
    set(CSS_JS_SOURCES
        src/compiler/css_compiler_regex.cpp
        src/compiler/js_compiler_regex.cpp
    )
    message(STATUS "Using regex-based fallback for CSS/JavaScript compilation")
endif()

set(ALL_SOURCES
    ${CORE_SOURCES}
    ${UTILS_SOURCES}
    ${MODULE_SOURCES}
    ${SCANNER_SOURCES}
    ${COMPILER_SOURCES}
    ${CSS_JS_SOURCES}
)

# 过滤存在的源文件
set(EXISTING_SOURCES)
foreach(source ${ALL_SOURCES})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${source}")
        list(APPEND EXISTING_SOURCES ${source})
    else()
        message(STATUS "Source file not found, will be created: ${source}")
    endif()
endforeach()

# 创建核心库
if(EXISTING_SOURCES)
    add_library(chtl-core STATIC ${EXISTING_SOURCES})
    
    # 链接ANTLR4库（如果可用）
    if(ANTLR4_FOUND)
        target_link_libraries(chtl-core ${ANTLR4_LIBRARIES})
        target_include_directories(chtl-core PRIVATE ${ANTLR4_INCLUDE_DIRS})
        target_compile_options(chtl-core PRIVATE ${ANTLR4_CFLAGS_OTHER})
    endif()
    
    # 设置库属性
    set_target_properties(chtl-core PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        POSITION_INDEPENDENT_CODE ON
    )
    
    list(LENGTH EXISTING_SOURCES source_count)
    message(STATUS "CHTL Core Library created with ${source_count} source files")
else()
    message(STATUS "No source files found yet, creating header-only target")
    add_library(chtl-core INTERFACE)
    target_include_directories(chtl-core INTERFACE include)
endif()

# 创建主可执行文件
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
    add_executable(chtl-compiler src/main.cpp)
    if(TARGET chtl-core AND EXISTING_SOURCES)
        target_link_libraries(chtl-compiler chtl-core)
    else()
        target_include_directories(chtl-compiler PRIVATE include)
    endif()
    
    # 设置可执行文件属性
    set_target_properties(chtl-compiler PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
endif()

# 测试可执行文件
file(GLOB_RECURSE TEST_FILES "tests/*.cpp")
if(TEST_FILES)
    foreach(test_file ${TEST_FILES})
        get_filename_component(test_name ${test_file} NAME_WE)
        add_executable(${test_name} ${test_file})
        
        if(TARGET chtl-core AND EXISTING_SOURCES)
            target_link_libraries(${test_name} chtl-core)
        else()
            target_include_directories(${test_name} PRIVATE include)
        endif()
        
        set_target_properties(${test_name} PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
        )
    endforeach()
    
    list(LENGTH TEST_FILES test_count)
    message(STATUS "Created ${test_count} test executables")
endif()

# 示例可执行文件
file(GLOB_RECURSE EXAMPLE_FILES "examples/*.cpp")
if(EXAMPLE_FILES)
    foreach(example_file ${EXAMPLE_FILES})
        get_filename_component(example_name ${example_file} NAME_WE)
        add_executable(example_${example_name} ${example_file})
        
        if(TARGET chtl-core AND EXISTING_SOURCES)
            target_link_libraries(example_${example_name} chtl-core)
        else()
            target_include_directories(example_${example_name} PRIVATE include)
        endif()
        
        set_target_properties(example_${example_name} PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
        )
    endforeach()
    
    list(LENGTH EXAMPLE_FILES example_count)
    message(STATUS "Created ${example_count} example executables")
endif()

# 安装规则
if(TARGET chtl-core AND EXISTING_SOURCES)
    install(TARGETS chtl-core
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
    )
endif()

if(TARGET chtl-compiler)
    install(TARGETS chtl-compiler
        RUNTIME DESTINATION bin
    )
endif()

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)

# 创建配置文件
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# 设置安装目录变量
set(CMAKE_INSTALL_INCLUDEDIR "include")
set(CMAKE_INSTALL_LIBDIR "lib")
set(CMAKE_INSTALL_BINDIR "bin")

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/CHTLConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CHTLConfig.cmake.in")
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/CHTLConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/CHTLConfigVersion.cmake"
        DESTINATION lib/cmake/CHTL
    )
endif()

# 调试信息
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")

# Doxygen文档（可选）
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile")
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
        COMMENT "Generating documentation with Doxygen"
        VERBATIM
    )
endif()

# 打包（可选）
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE" AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
    set(CPACK_PACKAGE_NAME "CHTL-Compiler")
    set(CPACK_PACKAGE_VENDOR "CHTL Team")
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CHTL Language Compiler")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
    
    include(CPack)
endif()
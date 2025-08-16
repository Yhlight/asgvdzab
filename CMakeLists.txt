# -*- mode:cmake -*-
cmake_minimum_required (VERSION 2.8)
# 2.8 needed because of ExternalProject

# Detect build type, fallback to release and throw a warning if use didn't specify any
if(NOT CMAKE_BUILD_TYPE)
  message(WARNING "Build type not set, falling back to Release mode.
 To specify build type use:
 -DCMAKE_BUILD_TYPE=<mode> where <mode> is Debug or Release.")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
       "Choose the type of build, options are: Debug Release."
       FORCE)
endif(NOT CMAKE_BUILD_TYPE)

if(NOT WITH_DEMO)
  message(STATUS "Building without demo. To enable demo build use: -DWITH_DEMO=True")
  set(WITH_DEMO False CACHE STRING
    "Chose to build with or without demo executable"
    FORCE)
endif(NOT WITH_DEMO)

option(WITH_LIBCXX "Building with clang++ and libc++(in Linux). To enable with: -DWITH_LIBCXX=On" Off)
option(WITH_STATIC_CRT "(Visual C++) Enable to statically link CRT, which avoids requiring users to install the redistribution package.
 To disable with: -DWITH_STATIC_CRT=Off" On)

project(LIBANTLR4)

if(CMAKE_VERSION VERSION_EQUAL "3.0.0" OR
   CMAKE_VERSION VERSION_GREATER "3.0.0")
  CMAKE_POLICY(SET CMP0026 NEW)
  CMAKE_POLICY(SET CMP0054 OLD)
  CMAKE_POLICY(SET CMP0045 OLD)
  CMAKE_POLICY(SET CMP0042 OLD)
endif()

if(CMAKE_VERSION VERSION_EQUAL "3.3.0" OR
   CMAKE_VERSION VERSION_GREATER "3.3.0")
  CMAKE_POLICY(SET CMP0059 OLD)
  CMAKE_POLICY(SET CMP0054 OLD)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(UUID REQUIRED uuid)
endif()
if(APPLE)
  find_library(COREFOUNDATION_LIBRARY CoreFoundation)
endif()

file(STRINGS "VERSION" ANTLR_VERSION)

if(WITH_DEMO)
  # Java is not necessary if building without demos.
  find_package(Java COMPONENTS Runtime REQUIRED)

  if(NOT ANTLR_JAR_LOCATION)
    message(FATAL_ERROR "Missing antlr4.jar location. You can specify it's path using: -DANTLR_JAR_LOCATION=<path>")
  else()
    get_filename_component(ANTLR_NAME ${ANTLR_JAR_LOCATION} NAME_WE)
    if(NOT EXISTS "${ANTLR_JAR_LOCATION}")
      message(FATAL_ERROR "Unable to find ${ANTLR_NAME} in ${ANTLR_JAR_LOCATION}")
    else()
      message(STATUS "Found ${ANTLR_NAME}: ${ANTLR_JAR_LOCATION}")
    endif()
  endif()
endif(WITH_DEMO)

if(MSVC_VERSION)
  set(MY_CXX_WARNING_FLAGS "  /W4")
else()
  set(MY_CXX_WARNING_FLAGS "  -Wall -pedantic -W")
endif()

# Define USE_UTF8_INSTEAD_OF_CODECVT macro.
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_UTF8_INSTEAD_OF_CODECVT")

# Initialize CXXFLAGS.
if("${CMAKE_VERSION}" VERSION_GREATER 3.1.0)
  if(NOT DEFINED CMAKE_CXX_STANDARD)
    # only set CMAKE_CXX_STANDARD if not already set
    # this allows the standard to be set by the caller, for example with -DCMAKE_CXX_STANDARD:STRING=17
    set(CMAKE_CXX_STANDARD 17)
  endif()
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
else()
  set(CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} -std=c++17")
  set(CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} -std=c++17")
  set(CMAKE_CXX_FLAGS_MINSIZEREL     "${CMAKE_CXX_FLAGS_MINSIZEREL} -std=c++17")
  set(CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE} -std=c++17")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -std=c++17")
endif()

set(CMAKE_CXX_FLAGS                  "${CMAKE_CXX_FLAGS} ${MY_CXX_WARNING_FLAGS}")
if(MSVC_VERSION)
  set(CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} /Od /Zi /MP ${MY_CXX_WARNING_FLAGS}")
  set(CMAKE_CXX_FLAGS_MINSIZEREL     "${CMAKE_CXX_FLAGS_MINSIZEREL} /O1 /Oi /Ob2 /Gy /MP /DNDEBUG ${MY_CXX_WARNING_FLAGS}")
  set(CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Oi /Ob2 /Gy /MP /DNDEBUG ${MY_CXX_WARNING_FLGAS}")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /O2 /Oi /Ob2 /Gy /MP /Zi ${MY_CXX_WARNING_FLAGS}")
else()
  set(CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g ${MY_CXX_WARNING_FLAGS}")
  set(CMAKE_CXX_FLAGS_MINSIZEREL     "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -DNDEBUG ${MY_CXX_WARNING_FLAGS}")
  set(CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG ${MY_CXX_WARNING_FLGAS}")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g ${MY_CXX_WARNING_FLAGS}")
endif()

# Compiler-specific C++11 activation.
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    # Just g++-5.0 and greater contain <codecvt> header. (test in ubuntu)
    if(NOT (GCC_VERSION VERSION_GREATER 5.0 OR GCC_VERSION VERSION_EQUAL 5.0))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 5.0 or greater.")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" AND ANDROID) 
	# Need -Os cflag and cxxflags here to work with exception handling on armeabi.
	# see  https://github.com/android-ndk/ndk/issues/573
	# and without -stdlib=libc++ cxxflags
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" AND APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" AND ( CMAKE_SYSTEM_NAME MATCHES "Linux" OR CMAKE_SYSTEM_NAME MATCHES "FreeBSD") )
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE CLANG_VERSION)
    if(NOT (CLANG_VERSION VERSION_GREATER 4.2.1 OR CLANG_VERSION VERSION_EQUAL 4.2.1))
        message(FATAL_ERROR "${PROJECT_NAME} requires clang 4.2.1 or greater.")
    endif()
    # You can use libc++ to compile this project when g++ is NOT greater than or equal to 5.0.
    if(WITH_LIBCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    endif()
elseif(MSVC_VERSION GREATER 1800 OR MSVC_VERSION EQUAL 1800)
  # Visual Studio 2012+ supports c++11 features 
elseif(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
else()
    message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif()


# 收集所有源文件，包括ANTLR生成的文件
file(GLOB_RECURSE SOURCES 
    "src/ast/*.cpp"
    "src/lexer/*.cpp"
    "src/parsers/*.cpp"
    "src/generators/*.cpp"
    "src/common/*.cpp"
    "src/dispatcher/*.cpp"
    "src/compilers/*.cpp"
    "src/scanner/*.cpp"
    "src/tools/*.cpp"
    "generated/antlr/css/*.cpp"
    "generated/antlr/js/*.cpp"
)

# 移除有main函数的文件，避免冲突
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/parsers/chtl_parser.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/tools/chtl_cmod_tool.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/tools/chtl_cjmod_tool.cpp")

# ========================================
# ANTLR4 集成配置
# ========================================

# 查找ANTLR4库文件（支持多种命名约定）
function(find_antlr4_library)
    set(ANTLR4_SEARCH_PATHS
        ${CMAKE_SOURCE_DIR}/third_party/antlr4/lib
        /usr/local/lib
        /usr/lib
        /opt/antlr4/lib
    )
    
    # 尝试查找不同命名约定的库文件
    set(ANTLR4_LIB_NAMES
        antlr4-runtime
        antlr4-runtime-static
        libantlr4-runtime
        antlr4_runtime
    )
    
    foreach(lib_name ${ANTLR4_LIB_NAMES})
        find_library(ANTLR4_FOUND_LIB 
            NAMES ${lib_name}
            PATHS ${ANTLR4_SEARCH_PATHS}
            NO_DEFAULT_PATH
        )
        if(ANTLR4_FOUND_LIB)
            set(ANTLR4_LIB ${ANTLR4_FOUND_LIB} PARENT_SCOPE)
            message(STATUS "Found ANTLR4 library: ${ANTLR4_FOUND_LIB}")
            return()
        endif()
    endforeach()
    
    # 如果没找到，报错
    message(FATAL_ERROR "ANTLR4 library not found. Please install ANTLR4 development files or place them in third_party/antlr4/lib/")
endfunction()

# 查找ANTLR4头文件
function(find_antlr4_headers)
    set(ANTLR4_INCLUDE_SEARCH_PATHS
        ${CMAKE_SOURCE_DIR}/third_party/antlr4/include
        /usr/local/include/antlr4-runtime
        /usr/include/antlr4-runtime
        /opt/antlr4/include
    )
    
    foreach(include_path ${ANTLR4_INCLUDE_SEARCH_PATHS})
        if(EXISTS "${include_path}/antlr4-runtime.h")
            set(ANTLR4_INCLUDE_DIR ${include_path} PARENT_SCOPE)
            message(STATUS "Found ANTLR4 headers: ${include_path}")
            return()
        endif()
    endforeach()
    
    # 如果没找到，报错
    message(FATAL_ERROR "ANTLR4 headers not found. Please install ANTLR4 development files or place them in third_party/antlr4/include/")
endfunction()

# 执行查找
find_antlr4_library()
find_antlr4_headers()

# ANTLR4集成状态
message(STATUS "✓ ANTLR4 integration enabled - Full CSS/JS syntax support with standard selectors")
message(STATUS "  Library: ${ANTLR4_LIB}")
message(STATUS "  Headers: ${ANTLR4_INCLUDE_DIR}")

# 包含ANTLR生成的源文件
file(GLOB_RECURSE ANTLR_GENERATED_SRC
    "generated/antlr/css/*.cpp"
    "generated/antlr/js/*.cpp"
)

# 设置CSS/JS编译器源文件和包含目录
set(CSS_JS_COMPILER_SOURCES
    src/compilers/antlr_css_compiler.cpp
    src/compilers/antlr_js_compiler.cpp
    ${ANTLR_GENERATED_SRC}
)

set(CSS_JS_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/generated/antlr/css
    ${CMAKE_SOURCE_DIR}/generated/antlr/js
    ${ANTLR4_INCLUDE_DIR}
)

# 添加编译器定义
add_definitions(-DUSE_ANTLR_COMPILERS=1)

# 创建CHTL编译器可执行文件
add_executable(chtl_compiler ${SOURCES} src/main.cpp)

# 设置包含目录
target_include_directories(chtl_compiler PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CSS_JS_INCLUDE_DIRS}
)

# 链接ANTLR4库
target_link_libraries(chtl_compiler ${ANTLR4_LIB})

# 为测试程序收集源文件，包括ANTLR生成的文件
file(GLOB_RECURSE TEST_LIB_SOURCES 
    "src/ast/*.cpp"
    "src/lexer/*.cpp"
    "src/generators/*.cpp"
    "src/common/*.cpp"
    "src/dispatcher/*.cpp"
    "src/compilers/*.cpp"
    "src/scanner/*.cpp"
    "generated/antlr/css/*.cpp"
    "generated/antlr/js/*.cpp"
)

# 移除有main函数的文件
list(REMOVE_ITEM TEST_LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/parsers/chtl_parser.cpp")
list(REMOVE_ITEM TEST_LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/tools/chtl_cmod_tool.cpp")
list(REMOVE_ITEM TEST_LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/tools/chtl_cjmod_tool.cpp")

# 创建测试库
add_library(chtl_test_lib ${TEST_LIB_SOURCES})

target_include_directories(chtl_test_lib PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CSS_JS_INCLUDE_DIRS}
)

# 链接ANTLR4库到测试库
target_link_libraries(chtl_test_lib ${ANTLR4_LIB})

# ANTLR编译器测试
add_executable(antlr_compilers_test tests/antlr_compilers_test.cpp)
target_link_libraries(antlr_compilers_test chtl_test_lib)
target_include_directories(antlr_compilers_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CSS_JS_INCLUDE_DIRS}
)

# ANTLR简化测试
add_executable(antlr_simple_test tests/antlr_simple_test.cpp)
target_link_libraries(antlr_simple_test chtl_test_lib)
target_include_directories(antlr_simple_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CSS_JS_INCLUDE_DIRS}
)

message(STATUS "✓ ANTLR compilers test enabled")


add_subdirectory(runtime)
if(WITH_DEMO)
 add_subdirectory(demo)
endif(WITH_DEMO)

# Generate CMake Package Files only if install is active
if (ANTLR4_INSTALL)

  include(GNUInstallDirs)
  include(CMakePackageConfigHelpers)

  if(NOT ANTLR4_CMAKE_DIR)
    set(ANTLR4_CMAKE_DIR ${CMAKE_INSTALL_LIBDIR}/cmake CACHE STRING
      "Installation directory for cmake files." FORCE )
  endif(NOT ANTLR4_CMAKE_DIR)

  set(version_runtime_config ${PROJECT_BINARY_DIR}/antlr4-runtime-config-version.cmake)
  set(version_generator_config ${PROJECT_BINARY_DIR}/antlr4-generator-config-version.cmake)
  set(project_runtime_config ${PROJECT_BINARY_DIR}/antlr4-runtime-config.cmake)
  set(project_generator_config ${PROJECT_BINARY_DIR}/antlr4-generator-config.cmake)
  set(targets_export_name antlr4-targets)

  set(ANTLR4_LIB_DIR ${CMAKE_INSTALL_LIBDIR} CACHE STRING
      "Installation directory for libraries, relative to ${CMAKE_INSTALL_PREFIX}.")
  
  set(ANTLR4_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR}/antlr4-runtime CACHE STRING
      "Installation directory for include files, relative to ${CMAKE_INSTALL_PREFIX}.")

  configure_package_config_file(
    cmake/antlr4-runtime.cmake.in
    ${project_runtime_config}
    INSTALL_DESTINATION ${ANTLR4_CMAKE_DIR}/antlr4-runtime
    PATH_VARS 
    ANTLR4_INCLUDE_DIR
    ANTLR4_LIB_DIR )
  
  configure_package_config_file(
    cmake/antlr4-generator.cmake.in
    ${project_generator_config}
    INSTALL_DESTINATION ${ANTLR4_CMAKE_DIR}/antlr4-generator
    PATH_VARS 
    ANTLR4_INCLUDE_DIR
    ANTLR4_LIB_DIR )
  
  write_basic_package_version_file(
    ${version_runtime_config}
    VERSION ${ANTLR_VERSION}
    COMPATIBILITY SameMajorVersion )

  write_basic_package_version_file(
    ${version_generator_config}
    VERSION ${ANTLR_VERSION}
    COMPATIBILITY SameMajorVersion )

  install(EXPORT ${targets_export_name}
          DESTINATION ${ANTLR4_CMAKE_DIR}/antlr4-runtime )

  install(FILES ${project_runtime_config}
                ${version_runtime_config}
          DESTINATION ${ANTLR4_CMAKE_DIR}/antlr4-runtime )

  install(FILES ${project_generator_config}
                ${version_generator_config}
          DESTINATION ${ANTLR4_CMAKE_DIR}/antlr4-generator )

endif(ANTLR4_INSTALL)

if(EXISTS LICENSE.txt)
install(FILES LICENSE.txt
        DESTINATION "share/doc/libantlr4")
elseif(EXISTS ../../LICENSE.txt) 
install(FILES ../../LICENSE.txt
    DESTINATION "share/doc/libantlr4")
endif()

install(FILES README.md VERSION 
    DESTINATION "share/doc/libantlr4")

set(CPACK_PACKAGE_CONTACT "antlr-discussion@googlegroups.com")
set(CPACK_PACKAGE_VERSION ${ANTLR_VERSION})
include(CPack)

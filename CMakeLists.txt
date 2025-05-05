cmake_minimum_required(VERSION 3.29)
project(cero)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (WIN32)
    set(system_extension win)
    add_compile_definitions(CERO_SYSTEM_WINDOWS)
elseif (UNIX)
    set(system_extension posix)
    add_compile_definitions(CERO_SYSTEM_POSIX)
endif ()

set(source_filter_regex .+/[^.]+\\.cpp|.+\\.${system_extension}\\.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_definitions(CERO_COMPILER_MSVC)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_definitions(CERO_COMPILER_CLANG)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_definitions(CERO_COMPILER_GCC)
endif ()

add_subdirectory(src)
add_subdirectory(tests)

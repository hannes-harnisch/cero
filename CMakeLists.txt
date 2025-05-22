cmake_minimum_required(VERSION 3.29)
if (CMAKE_TOOLCHAIN_FILE)
	message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
endif ()

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

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	add_compile_definitions(CERO_COMPILER_MSVC)
	add_compile_options(/W4)
else ()
	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		add_compile_definitions(CERO_COMPILER_CLANG)
	elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		if (MINGW)
			add_compile_definitions(CERO_COMPILER_MINGW)
		else ()
			add_compile_definitions(CERO_COMPILER_GCC)
		endif ()
	endif ()
	add_compile_options(
		-Wall
		-Wextra
		-Wpedantic
		-Wconversion
		-Wsign-conversion
		-Wshadow
	)
endif ()

macro(filter_sources sources)
	list(FILTER ${sources} INCLUDE REGEX ".+/[^.]+(\\.${system_extension})?\\.[^.]+$")
endmacro()

add_subdirectory(cero)
add_subdirectory(tests)

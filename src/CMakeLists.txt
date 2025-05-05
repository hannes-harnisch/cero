file(GLOB_RECURSE sources CONFIGURE_DEPENDS *)
list(FILTER sources INCLUDE REGEX ${source_filter_regex})

add_library(cero STATIC ${sources})
target_precompile_headers(cero PUBLIC Pch.hpp)
target_include_directories(cero PUBLIC .)

find_package(cxxopts REQUIRED)
find_package(fmt CONFIG REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(gmp REQUIRED IMPORTED_TARGET gmp)

target_link_libraries(cero PUBLIC
        cxxopts::cxxopts
        fmt::fmt
        PkgConfig::gmp
)

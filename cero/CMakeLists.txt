file(GLOB_RECURSE sources CONFIGURE_DEPENDS *)
filter_sources(sources)

add_library(cero STATIC ${sources})
target_precompile_headers(cero PUBLIC pch.hh)
target_include_directories(cero PUBLIC ..)

find_package(fmt CONFIG REQUIRED)

target_link_libraries(cero PUBLIC
	fmt::fmt
)

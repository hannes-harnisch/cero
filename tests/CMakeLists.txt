file(GLOB_RECURSE sources CONFIGURE_DEPENDS *)
filter_sources(sources)

add_executable(cero_tests ${sources})

target_precompile_headers(cero_tests REUSE_FROM cero)
target_include_directories(cero_tests PRIVATE .)

find_package(doctest REQUIRED)

target_link_libraries(cero_tests PRIVATE
	cero
	doctest::doctest
)

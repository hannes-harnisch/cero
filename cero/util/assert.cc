#include "assert.hh"

namespace util {

namespace {

[[noreturn]] void fail(std::source_location location) {
	fmt::println("\tFile:     {}", location.file_name());
	fmt::println("\tFunction: {}", location.function_name());
	std::abort();
}

} // namespace

void assert_fail(std::string_view msg, std::source_location location) {
	fmt::println("assertion failed: {}", msg);
	fail(location);
}

void assert_unreachable(std::source_location location) {
	cero_debug_break();
	fmt::println("reached code that should be unreachable");
	fail(location);
}

} // namespace util

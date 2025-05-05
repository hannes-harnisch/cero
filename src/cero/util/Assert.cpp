#include "Assert.hpp"

namespace cero {

[[noreturn]] static void fail(std::source_location location) {
	fmt::println("\tFile:     {}", location.file_name());
	fmt::println("\tFunction: {}", location.function_name());
	std::abort();
}

void assert_fail(std::string_view msg, std::source_location location) {
	fmt::println("Assertion failed: {}", msg);
	fail(location);
}

void assert_unreachable(std::source_location location) {
	cero_debug_break();
	fmt::println("The compiler reached code that should be unreachable.");
	fail(location);
}

void to_do(std::source_location location) {
	cero_debug_break();
	fmt::println("Not yet implemented.");
	fail(location);
}

} // namespace cero

#pragma once

namespace util {

/// Terminates the compiler immediately and unconditionally. To be used when an expected precondition, result or invariant
/// was not upheld.
[[noreturn]] void assert_fail(std::string_view msg, std::source_location location = std::source_location::current());

/// Terminates the compiler immediately and unconditionally. To be used in control flow branches that are only reachable if
/// there are logic bugs in the compiler.
[[noreturn]] void assert_unreachable(std::source_location location = std::source_location::current());

} // namespace util

#if CERO_COMPILER_CLANG
	#define cero_debug_break() __builtin_debugtrap()
#elif CERO_COMPILER_MSVC
	#define cero_debug_break() __debugbreak()
#elif CERO_COMPILER_GCC
	#include <csignal>
	#define cero_debug_break() ::raise(SIGTRAP)
#elif CERO_COMPILER_MINGW
	#define cero_debug_break() __debugbreak()
#else
	#error not implemented
#endif

/// Terminates the compiler immediately if the condition is false. To be used when an expected precondition, result or invariant
/// was not upheld.
#define cero_assert(condition, ...)                                                                                            \
	do {                                                                                                                       \
		if (!(condition)) {                                                                                                    \
			cero_debug_break();                                                                                                \
			util::assert_fail(fmt::format(__VA_ARGS__));                                                                       \
		}                                                                                                                      \
	}                                                                                                                          \
	while (false)

#ifndef NDEBUG
	#define cero_assert_debug(condition, ...) cero_assert(condition, __VA_ARGS__)
#else
	#define cero_assert_debug(condition, ...) (void) (condition)
#endif

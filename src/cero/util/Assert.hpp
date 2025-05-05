#pragma once

namespace cero {

/// Terminates the compiler immediately and unconditionally. To be used when an expected precondition, result or invariant
/// was not upheld.
[[noreturn]] void assert_fail(std::string_view msg, std::source_location location = std::source_location::current());

/// Terminates the compiler immediately and unconditionally. To be used in control flow branches that are only reachable if
/// there are logic bugs in the compiler.
[[noreturn]] void assert_unreachable(std::source_location location = std::source_location::current());

/// Terminates the compiler immediately and unconditionally. To be used where something is not yet implemented.
[[noreturn]] void to_do(std::source_location location = std::source_location::current());

} // namespace cero

#if CERO_COMPILER_CLANG
	#define cero_debug_break() __builtin_debugtrap()
#elif CERO_COMPILER_MSVC
	#define cero_debug_break() __debugbreak()
#elif CERO_COMPILER_GCC
	#include <csignal>
	#define cero_debug_break() std::raise(SIGTRAP)
#else
	#error unknown compiler
#endif

/// Terminates the compiler immediately if the condition is false. To be used when an expected precondition, result or invariant
/// was not upheld.
#define cero_assert(condition, msg, ...)                                                                                       \
	do {                                                                                                                       \
		if (!(condition)) {                                                                                                    \
			cero_debug_break();                                                                                                \
			cero::assert_fail(fmt::format(msg, __VA_ARGS__));                                                                  \
		}                                                                                                                      \
	} while (false)

#ifndef NDEBUG
	#define cero_assert_debug(condition, msg, ...) cero_assert(condition, msg, __VA_ARGS__)
#else
	#define cero_assert_debug(condition, msg, ...) static_cast<void>(condition)
#endif

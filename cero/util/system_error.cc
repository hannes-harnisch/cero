#include "system_error.hh"

#ifdef CERO_SYSTEM_WINDOWS
	#include "cero/util/windows_api.hh"
#endif

namespace util {

std::error_condition get_system_error() {
#ifdef CERO_SYSTEM_WINDOWS
	int last_error = (int) ::GetLastError();
#else
	int last_error = errno;
#endif

	// using default_error_condition here because it prevents translation of error messages on Windows with MSVC STL
	return std::system_category().default_error_condition(last_error);
}

std::string get_system_error_msg() {
	std::error_condition error = get_system_error();
	return fmt::format("system error 0x{:x} (\"{}\")", error.value(), error.message());
}

} // namespace util

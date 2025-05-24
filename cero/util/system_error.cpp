#include "system_error.hpp"

#ifdef CERO_SYSTEM_WINDOWS
	#include "cero/util/windows_api.hpp"
#endif

namespace cero {

std::error_condition get_system_error() {
#ifdef CERO_SYSTEM_WINDOWS
	const int last_error = (int) ::GetLastError();
#else
	const int last_error = errno;
#endif
	return std::system_category().default_error_condition(last_error);
}

std::string get_system_error_msg() {
	std::error_condition error = get_system_error();
	return fmt::format("system error 0x{:x} (\"{}\")", error.value(), error.message());
}

} // namespace cero

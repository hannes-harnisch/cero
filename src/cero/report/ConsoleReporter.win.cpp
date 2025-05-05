#include "ConsoleReporter.hpp"

#include "cero/util/Assert.hpp"
#include "cero/util/SystemError.hpp"
#include "cero/util/WindowsApi.hpp"

namespace cero {

void ConsoleReporter::init_console_environment() {
	BOOL success = ::SetConsoleOutputCP(CP_UTF8);
	cero_assert(success, "could not set console output code page, {}", get_last_system_error_message());
}

} // namespace cero

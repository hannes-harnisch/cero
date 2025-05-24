#include "console_reporter.hh"

#include "cero/util/system_error.hh"
#include "cero/util/windows_api.hh"

namespace cero {

namespace {

void enable_utf8_output() {
	BOOL success = ::SetConsoleCP(CP_UTF8);
	if (!success) {
		fmt::println("failed to set console output code page to UTF-8, {}", util::get_system_error_msg());
	}
}

void enable_virtual_terminal_sequences() {
	HANDLE std_out = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if (std_out == INVALID_HANDLE_VALUE) {
		fmt::println("failed to get stdout handle to enable virtual terminal sequences, {}", util::get_system_error_msg());
		return;
	}

	DWORD mode = 0;
	BOOL success = ::GetConsoleMode(std_out, &mode);
	if (!success) {
		// this can fail in CLion debug mode when terminal emulation is not enabled, or with GDB on Windows
		fmt::println("failed to get console mode to enable virtual terminal sequences, {}", util::get_system_error_msg());
		return;
	}

	mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	success = ::SetConsoleMode(std_out, mode);
	if (!success) {
		fmt::println("failed to set console mode to enable virtual terminal sequences, {}", util::get_system_error_msg());
	}
}

} // namespace

void ConsoleReporter::init_console_environment() {
	enable_utf8_output();
	enable_virtual_terminal_sequences(); // for color output, used by fmt
}

} // namespace cero

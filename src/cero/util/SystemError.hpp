#pragma once

namespace cero {

std::error_condition get_last_system_error();

inline std::string get_last_system_error_message() {
	auto error = get_last_system_error();
	return fmt::format("system error 0x{:x} (\"{}\")", error.value(), error.message());
}

} // namespace cero

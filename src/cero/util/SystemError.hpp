#pragma once

namespace cero {

/// Gets the most recent system API error.
std::error_condition get_last_system_error();

/// Gets the message associated with the most recent system API error.
inline std::string get_last_system_error_message() {
	auto error = get_last_system_error();
	return fmt::format("system error 0x{:x} (\"{}\")", error.value(), error.message());
}

} // namespace cero

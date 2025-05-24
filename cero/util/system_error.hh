#pragma once

namespace util {

/// Gets the most recent system API error, based on GetLastError for Windows, and errno on Posix platforms.
std::error_condition get_system_error();

/// Gets the message associated with the most recent system API error.
std::string get_system_error_msg();

} // namespace util

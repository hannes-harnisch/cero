#pragma once

namespace util {

/// Gets the most recent system API error.
std::error_condition get_system_error();

/// Gets the message associated with the most recent system API error.
std::string get_system_error_msg();

} // namespace util

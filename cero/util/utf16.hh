#pragma once

namespace util {

/// Converts a UTF-8 string to a UTF-16 wide string, intended for passing strings to Windows API functions.
std::wstring utf8_to_utf16(std::string_view input);

/// Converts a UTF-16 wide string to a UTF-8 string, intended for converting strings received from Windows API functions.
std::string utf16_to_utf8(std::wstring_view input);

} // namespace util

#pragma once

namespace cero {

inline bool is_ascii(char c) {
	return (uint8_t) c >> 7 == 0;
}

inline bool is_ascii_identifier_char(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

} // namespace cero

#pragma once

namespace cero {

/// Determines whether the given UTF-8 character has the XID_Start property, which decides whether a character should be allowed
/// to begin a Unicode identifier according to Unicode Standard Annex #31.
bool is_utf8_xid_start_char(uint32_t utf8_char);

/// Determines whether the given UTF-8 character has the XID_Continue property, which decides whether a character should be
/// allowed to continue a Unicode identifier according to Unicode Standard Annex #31.
bool is_utf8_xid_continue_char(uint32_t utf8_char);

inline bool is_ascii(char c) {
	return (uint8_t) c >> 7 == 0;
}

inline bool is_ascii_identifier_char(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

inline bool is_ascii_whitespace(char c) {
	return c == ' ' || (c >= '\t' && c <= '\r');
}

inline bool is_decimal_digit(char c) {
	return c >= '0' && c <= '9';
}

inline bool is_hexadecimal_digit(char c) {
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

} // namespace cero

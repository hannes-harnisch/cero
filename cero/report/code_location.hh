#pragma once

namespace cero {

/// A location in Cero source code.
struct CodeLocation {
	std::string_view path;
	uint32_t line = 0;
	uint32_t column = 0;

	/// Creates a string for diagnostic messages.
	std::string to_string() const;

	/// Creates a string containing only the line and column.
	std::string to_short_string() const;

	bool operator==(const CodeLocation&) const = default;
};

inline constexpr uint8_t default_tab_size = 4;

} // namespace cero

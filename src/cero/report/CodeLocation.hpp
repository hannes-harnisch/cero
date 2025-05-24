#pragma once

namespace cero {

/// A location in Cero source code.
struct CodeLocation {
	std::string_view source_path;
	uint32_t line = 0;
	uint32_t column = 0;

	bool operator==(const CodeLocation&) const = default;

	/// Creates a string for diagnostic messages.
	std::string to_string() const;

	/// Creates a string containing only the line and column, such as for AST dumps.
	std::string to_short_string() const;
};

constexpr inline uint8_t DefaultTabSize = 4;

} // namespace cero

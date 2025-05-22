#pragma once

namespace cero {

/// A location in Cero source code.
struct CodeLocation {
	std::string_view path;
	uint32_t line = 0;
	uint32_t column = 0;
};

inline constexpr uint8_t default_tab_size = 4;

} // namespace cero

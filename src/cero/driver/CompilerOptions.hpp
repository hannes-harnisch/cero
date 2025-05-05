#pragma once

namespace cero {

struct CompilerOptions {
	std::string_view path;
	uint8_t tab_size = 4;
	bool print_tokens = false;
	bool print_ast = false;

	static std::optional<CompilerOptions> from(std::span<char*> args) {
		return CompilerOptions {};
	}
};

} // namespace cero

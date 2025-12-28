#pragma once

namespace cero {

enum class SymbolKind {
	function,
};

struct Symbol {
	SymbolKind kind;
	uint32_t index;
};

class SymbolTable {
public:

private:
	std::unordered_map<std::string, Symbol> symbol_map_;
};

} // namespace cero

#pragma once

#include "cero/syntax/ast.hpp"
#include "cero/type/symbol_table.hpp"

namespace cero {

enum class SymbolState {
	declared,
	incomplete,
	complete,
};

class SymbolContext {
public:
	explicit SymbolContext(std::string_view source_text);

	void add_declared_definition(const AstDefinition* def);

	SymbolTable get_table() &&;

private:
	const std::string_view source_;
	SymbolTable table_;
};

} // namespace cero

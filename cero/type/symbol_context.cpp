#include "symbol_context.hpp"

namespace cero {

SymbolContext::SymbolContext(std::string_view source_text) :
    source_(source_text) {
}

void SymbolContext::add_declared_definition(const AstDefinition* def) {
	std::string_view name = def->name.get(source_);
}

SymbolTable SymbolContext::get_table() && {
	return std::move(table_);
}

} // namespace cero

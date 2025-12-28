#include "symbol_table.hh"

#include "cero/util/assert.hh"

namespace cero {

SymbolName::SymbolName(std::string_view identifier) :
    name(identifier),
    hash(std::hash<std::string_view> {}(identifier)) {
}

bool SymbolName::operator==(const SymbolName& other) const {
	return name == other.name;
}

size_t SymbolName::Hash::operator()(const SymbolName& key) {
	return key.hash;
}

void Scope::put(std::string_view identifier, SymbolKind kind) {
	symbols_.emplace(SymbolName(identifier), kind);
}

const Symbol* Scope::find(const SymbolName& key) const {
	auto it = symbols_.find(key);
	if (it == symbols_.end()) {
		return nullptr;
	}
	else {
		return &it->second;
	}
}

void SymbolTable::push_scope() {
	scope_stack_.emplace_back();
}

void SymbolTable::pop_scope() {
	cero_assert(!scope_stack_.empty(), "must not pop empty scope stack");
	scope_stack_.pop_back();
}

void SymbolTable::put(std::string_view identifier, SymbolKind kind) {
	cero_assert(!scope_stack_.empty(), "must not push empty scope stack");
}

const Symbol* SymbolTable::find(std::string_view identifier) const {
	SymbolName key(identifier);

	for (const Scope& scope : scope_stack_ | std::views::reverse) {
		if (const Symbol* symbol = scope.find(key)) {
			return symbol;
		}
	}

	return nullptr;
}

} // namespace cero

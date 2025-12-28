#pragma once

#include "cero/sema/type_context.hh"

namespace cero {

enum class SymbolKind {
	function,
	parameter,
	output,
};

struct Symbol {
	SymbolKind kind;
	TypeId type;
};

/// Key type to find symbols in scopes. The hash is stored with the name to prevent computing the hash multiple times when
/// looking through multiple scopes in the symbol table.
struct SymbolName {
	const std::string_view name;
	const size_t hash;

	explicit SymbolName(std::string_view identifier);

	bool operator==(const SymbolName& other) const;

	struct Hash {
		static size_t operator()(const SymbolName& key);
	};
};

/// Maps identifiers to symbols within a particular lexical scope.
class Scope {
public:
	void put(std::string_view identifier, SymbolKind kind);
	const Symbol* find(const SymbolName& key) const;

private:
	std::unordered_map<SymbolName, Symbol, SymbolName::Hash> symbols_;
};

/// Keeps track of how identifiers map to symbols across multiple lexical scopes.
class SymbolTable {
public:
	void push_scope();
	void pop_scope();

	void put(std::string_view identifier, SymbolKind kind);

	const Symbol* find(std::string_view identifier) const;

private:
	std::vector<Scope> scope_stack_;
};

} // namespace cero

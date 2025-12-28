#pragma once

#include "cero/sema/type_context.hh"
#include "cero/syntax/ast.hh"
#include "cero/util/memory_arena.hh"

namespace cero {

enum class TirNodeKind {
	function,
};

struct TirNode {
	const TirNodeKind node_kind   : 8;
	const SourceSize token_offset : source_size_bits;

	TirNode(TirNodeKind kind, SourceSize offset) :
	    node_kind(kind),
	    token_offset(offset & 0xffffff) {
	}
};

struct TirSymbol : TirNode {};

struct TirExpression : TirNode {
	TypeId type;
};

struct TirRoot {
	NodeList<TirSymbol> symbols;
};

struct TirFunction : TirSymbol {
	explicit TirFunction(SourceSize offset) :
	    TirSymbol({TirNodeKind::function, offset}) {
	}
};

struct TirIdExpr : TirExpression {};

/// The typed intermediate representation (TIR) produced by the typechecker stage. Contains essentially just a symbol table and
/// a desugared, more explicit form of the AST, with only functions and globals at the top level, as all type descriptions are
/// in the symbol table. Owns the memory for all of its nodes.
class TirTree {
public:
	const TirRoot& get_root() const;

private:
	util::MemoryArena arena_;
	TirRoot* root_;

	/// Reserves arena_size amount of bytes in the arena for the TIR.
	explicit TirTree(size_t arena_size);

	friend struct TypeChecker;
};

} // namespace cero

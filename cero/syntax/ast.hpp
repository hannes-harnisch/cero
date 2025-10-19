#pragma once

#include "cero/syntax/token_list.hpp"
#include "cero/util/assert.hpp"
#include "cero/util/memory_arena.hpp"

namespace cero {

enum class AstNodeKind {
	root,
	function_def,
	function_parameter,
	function_output,
};

struct AstNode {
	const AstNodeKind node_kind : 8 = {};
	const SourceSize offset : source_size_bits = {};

	CodeLocation locate_in(const SourceView& source, uint8_t tab_size) const {
		return source.locate(offset, tab_size);
	}
};

struct AstDefinition : AstNode {};

struct AstExpression : AstNode {};

struct AstRoot : AstNode {
	static constexpr AstNodeKind kind = AstNodeKind::root;

	ArenaArray<AstDefinition*> definitions;
};

enum class AccessModifier : uint8_t {
	none,
	private_,
	public_,
};

struct AstName {
	SourceSize offset = 0;
	SourceSize length = 0;

	static AstName from(Token token) {
		cero_assert(token.kind == TokenKind::identifier, "AstName must be created from an identifier token");
		return AstName {
		    .offset = token.offset,
		    .length = token.variable_length,
		};
	}
};

struct AstFunctionParameter : AstNode {
	static constexpr AstNodeKind kind = AstNodeKind::function_parameter;

	AstName type;
	AstName name;
};

struct AstFunctionOutput : AstNode {
	static constexpr AstNodeKind kind = AstNodeKind::function_output;

	AstName type;
	AstName name;
};

struct AstFunctionDefinition : AstDefinition {
	static constexpr AstNodeKind kind = AstNodeKind::function_def;

	AccessModifier access_modifier = {};
	AstName name;
	ArenaArray<AstFunctionParameter*> parameters;
	ArenaArray<AstFunctionOutput*> outputs;
	ArenaArray<AstExpression*> statements;
};

class Ast {
public:

private:
	MemoryArena arena_;
	AstRoot* root_ = nullptr;

	/// Reserves storage for the AST based on the number of tokens.
	explicit Ast(const TokenList& token_list);

	/// Creates a node of type T in storage owned by the AST.
	/// @param offset Offset of the token where the given AST node begins.
	template<typename T>
	T* create(SourceSize offset) {
		void* storage = arena_.allocate(sizeof(T), alignof(T));

		AstNode base {
		    .node_kind = T::kind,
		    .offset = offset & 0x00ffffffu,
		};
		return new (storage) T {{base}};
	}

	friend struct Parser;
};

} // namespace cero

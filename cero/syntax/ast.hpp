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
	const SourceSize offset     : source_size_bits = {};

	CodeLocation locate_in(const SourceView& source, uint8_t tab_size) const;
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

	std::string_view get(std::string_view source_text) const {
		return source_text.substr(offset, length);
	}
};

enum class AccessModifier : uint8_t {
	none,
	private_,
	public_,
};

struct AstDefinition : AstNode {
	AccessModifier access_modifier = {};
	AstName name;
};

struct AstExpression : AstNode {};

struct AstRoot : AstNode {
	static constexpr AstNodeKind kind = AstNodeKind::root;

	util::ArenaArray<AstDefinition*> definitions;
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

	util::ArenaArray<AstFunctionParameter*> parameters;
	util::ArenaArray<AstFunctionOutput*> outputs;
	util::ArenaArray<AstExpression*> statements;
};

class Ast {
public:

private:
	util::MemoryArena arena_;
	AstRoot* root_ = nullptr;

	/// Reserves storage for the AST based on the number of tokens.
	explicit Ast(const TokenList& token_list);

	/// Creates a node of type T in storage owned by the AST.
	/// @param  offset  Offset of the token where the given AST node begins.
	template<std::derived_from<AstNode> T>
	T* create(SourceSize offset) {
		void* storage = arena_.allocate(sizeof(T), alignof(T));

		// construct node and initialize AstNode base
		T* node = new (storage) T {{AstNode {
		    .node_kind = T::kind,
		    .offset = offset & 0x00ffffffu,
		}}};
		return node;
	}

	friend struct Parser;
};

} // namespace cero

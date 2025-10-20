#pragma once

#include "cero/syntax/token.hh"
#include "cero/util/assert.hh"
#include "cero/util/memory_arena.hh"

namespace cero {

enum class AstNodeKind {
	function_def,
	function_parameter,
	function_output,
};

struct AstNode {
	const AstNodeKind node_kind   : 8;
	const SourceSize token_offset : source_size_bits;

	AstNode(AstNodeKind kind, SourceSize offset) :
	    node_kind(kind),
	    token_offset(offset & 0xffffff) {
	}

	CodeLocation locate_in(const SourceView& source, uint8_t tab_size) const;
};

template<typename T>
using NodeList = util::ArenaArray<const T*>;

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

	bool is_empty() const {
		return length == 0;
	}
};

enum class AccessModifier : uint8_t {
	none,
	private_,
	public_,
};

struct AstDefinition : AstNode {
	AccessModifier access_modifier;
	AstName name;
};

struct AstExpression : AstNode {};

struct AstRoot {
	NodeList<AstDefinition> definitions;
};

struct AstFunctionParameter : AstNode {
	AstName type;
	AstName name;

	explicit AstFunctionParameter(SourceSize offset) :
	    AstNode(AstNodeKind::function_parameter, offset) {
	}
};

struct AstFunctionOutput : AstNode {
	AstName type;
	AstName name;

	explicit AstFunctionOutput(SourceSize offset) :
	    AstNode(AstNodeKind::function_output, offset) {
	}
};

struct AstFunctionDefinition : AstDefinition {
	NodeList<AstFunctionParameter> parameters;
	NodeList<AstFunctionOutput> outputs;
	NodeList<AstNode> statements;

	explicit AstFunctionDefinition(SourceSize offset) :
	    AstDefinition({AstNodeKind::function_def, offset}) {
	}
};

/// The abstract syntax tree (AST) produced by the parser stage. Owns the memory for all AST nodes.
class Ast {
public:
	const AstRoot& get_root() const;

private:
	util::MemoryArena arena_;
	AstRoot* root_;

	/// Reserves arena_size amount of bytes in the arena for the AST.
	explicit Ast(size_t arena_size);

	friend struct Parser;
};

} // namespace cero

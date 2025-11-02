#pragma once

#include "cero/syntax/token.hh"
#include "cero/util/assert.hh"
#include "cero/util/memory_arena.hh"

namespace cero {

enum class AstNodeKind {
	function_def,
	function_parameter,
	function_output,
	ident_expr,
	literal_expr,
	block_expr,
	return_expr,
	unary_expr,
	binary_expr,
	call_expr,
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
	const AstExpression* type = {};
	AstName name;

	explicit AstFunctionParameter(SourceSize offset) :
	    AstNode(AstNodeKind::function_parameter, offset) {
	}
};

struct AstFunctionOutput : AstNode {
	const AstExpression* type = {};
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

struct AstIdentExpr : AstExpression {
	SourceSize length = {};

	explicit AstIdentExpr(SourceSize offset) :
	    AstExpression({AstNodeKind::ident_expr, offset}) {
	}

	std::string_view get(std::string_view source_text) const {
		return source_text.substr(token_offset, length);
	}
};

enum class LiteralKind {
	dec_int,
	hex_int,
	bin_int,
	oct_int,
	float_,
	char_,
	string,
};

struct AstLiteralExpr : AstExpression {
	LiteralKind literal_kind = {};
	SourceSize length = {};

	explicit AstLiteralExpr(SourceSize offset) :
	    AstExpression({AstNodeKind::literal_expr, offset}) {
	}

	std::string_view get(std::string_view source_text) const {
		return source_text.substr(token_offset, length);
	}
};

struct AstBlockExpr : AstExpression {
	NodeList<AstNode> statements;

	explicit AstBlockExpr(SourceSize offset) :
	    AstExpression({AstNodeKind::block_expr, offset}) {
	}
};

struct AstReturnExpr : AstExpression {
	NodeList<AstExpression> ret_values;

	explicit AstReturnExpr(SourceSize offset) :
	    AstExpression({AstNodeKind::return_expr, offset}) {
	}
};

enum class UnaryOperator {
	pre_increment,
	pre_decrement,
	post_increment,
	post_decrement,
	negate,
	bit_not,
	logic_not,
	addr_of,
	deref,
};

struct AstUnaryExpr : AstExpression {
	UnaryOperator op = {};
	const AstExpression* operand = {};

	explicit AstUnaryExpr(SourceSize offset) :
	    AstExpression({AstNodeKind::unary_expr, offset}) {
	}
};

enum class BinaryOperator {
	add,
	sub,
	mul,
	div,
	rem,
	bit_and,
	bit_or,
	bit_xor,
	shl,
	shr,
	logic_and,
	logic_or,
	eq,
	neq,
	less,
	less_eq,
	greater,
	greater_eq,
	assign,
	add_assign,
	sub_assign,
	mul_assign,
	div_assign,
	rem_assign,
	bit_and_assign,
	bit_or_assign,
	xor_assign,
	shl_assign,
	shr_assign,
	logic_and_assign,
	logic_or_assign,
};

struct AstBinaryExpr : AstExpression {
	BinaryOperator op = {};
	const AstExpression* left = {};
	const AstExpression* right = {};

	explicit AstBinaryExpr(SourceSize offset) :
	    AstExpression({AstNodeKind::binary_expr, offset}) {
	}
};

struct AstCallExpr : AstExpression {
	const AstExpression* callee = {};
	NodeList<AstExpression> args;

	explicit AstCallExpr(SourceSize offset) :
	    AstExpression({AstNodeKind::call_expr, offset}) {
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

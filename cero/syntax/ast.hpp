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
	id_expr,
	literal_expr,
	block_expr,
	return_expr,
	unary_expr,
	binary_expr,
	call_expr,
};

struct AstNode {
	const AstNodeKind node_kind : 8 = {};
	const SourceSize offset : source_size_bits = {};

	CodeLocation locate_in(const SourceView& source, uint8_t tab_size) const {
		return source.locate(offset, tab_size);
	}
};

template<std::derived_from<AstNode> T>
using NodeList = ArenaArray<const T*>;

struct AstDefinition : AstNode {};

struct AstExpression : AstNode {};

struct AstRoot : AstNode {
	static constexpr AstNodeKind kind = AstNodeKind::root;

	NodeList<AstDefinition> definitions;
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

	bool is_empty() const {
		return length == 0;
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
	NodeList<AstFunctionParameter> parameters;
	NodeList<AstFunctionOutput> outputs;
	NodeList<AstExpression> statements;
};

struct AstIdExpr : AstExpression {
	static constexpr AstNodeKind kind = AstNodeKind::id_expr;

	SourceSize length = {};
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
	static constexpr AstNodeKind kind = AstNodeKind::literal_expr;

	LiteralKind literal_kind = {};
	SourceSize length = {};
};

struct AstBlockExpr : AstExpression {
	static constexpr AstNodeKind kind = AstNodeKind::block_expr;

	NodeList<AstExpression> statements;
};

struct AstReturnExpr : AstExpression {
	static constexpr AstNodeKind kind = AstNodeKind::return_expr;

	NodeList<AstExpression> ret_values;
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
	static constexpr AstNodeKind kind = AstNodeKind::unary_expr;

	UnaryOperator op = {};
	const AstExpression* operand = {};
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
	static constexpr AstNodeKind kind = AstNodeKind::binary_expr;

	BinaryOperator op = {};
	const AstExpression* left = {};
	const AstExpression* right = {};
};

struct AstCallExpr : AstExpression {
	static constexpr AstNodeKind kind = AstNodeKind::call_expr;

	const AstExpression* callee = {};
	NodeList<AstExpression> args;
};

class Ast {
public:
	const AstRoot& get_root() const;

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

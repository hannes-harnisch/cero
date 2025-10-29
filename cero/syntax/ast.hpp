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
	const SourceSize offset     : source_size_bits = {};

	CodeLocation locate_in(const SourceView& source, uint8_t tab_size) const;
};

template<std::derived_from<AstNode> T>
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
	AccessModifier access_modifier = {};
	AstName name;
};

struct AstExpression : AstNode {};

struct AstRoot : AstNode {
	static constexpr AstNodeKind kind = AstNodeKind::root;

	NodeList<AstDefinition> definitions;
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

#pragma once

#include "cero/syntax/Source.hpp"

namespace cero {

using StringId = std::string_view;

#define CERO_AST_NODE_KINDS                                                                                                    \
	CERO_AST_NODE_KIND(Root)                                                                                                   \
	CERO_AST_NODE_KIND(StructDefinition)                                                                                       \
	CERO_AST_NODE_KIND(EnumDefinition)                                                                                         \
	CERO_AST_NODE_KIND(FunctionDefinition)                                                                                     \
	CERO_AST_NODE_KIND(FunctionParameter)                                                                                      \
	CERO_AST_NODE_KIND(FunctionOutput)                                                                                         \
	CERO_AST_NODE_KIND(BlockStatement)                                                                                         \
	CERO_AST_NODE_KIND(BindingStatement)                                                                                       \
	CERO_AST_NODE_KIND(IfExpr)                                                                                                 \
	CERO_AST_NODE_KIND(WhileLoop)                                                                                              \
	CERO_AST_NODE_KIND(ForLoop)                                                                                                \
	CERO_AST_NODE_KIND(NameExpr)                                                                                               \
	CERO_AST_NODE_KIND(GenericNameExpr)                                                                                        \
	CERO_AST_NODE_KIND(MemberExpr)                                                                                             \
	CERO_AST_NODE_KIND(GroupExpr)                                                                                              \
	CERO_AST_NODE_KIND(CallExpr)                                                                                               \
	CERO_AST_NODE_KIND(IndexExpr)                                                                                              \
	CERO_AST_NODE_KIND(ArrayLiteralExpr)                                                                                       \
	CERO_AST_NODE_KIND(UnaryExpr)                                                                                              \
	CERO_AST_NODE_KIND(BinaryExpr)                                                                                             \
	CERO_AST_NODE_KIND(ReturnExpr)                                                                                             \
	CERO_AST_NODE_KIND(ThrowExpr)                                                                                              \
	CERO_AST_NODE_KIND(BreakExpr)                                                                                              \
	CERO_AST_NODE_KIND(ContinueExpr)                                                                                           \
	CERO_AST_NODE_KIND(NumericLiteralExpr)                                                                                     \
	CERO_AST_NODE_KIND(StringLiteralExpr)                                                                                      \
	CERO_AST_NODE_KIND(PermissionExpr)                                                                                         \
	CERO_AST_NODE_KIND(PointerTypeExpr)                                                                                        \
	CERO_AST_NODE_KIND(ArrayTypeExpr)                                                                                          \
	CERO_AST_NODE_KIND(FunctionTypeExpr)

enum class AstNodeKind {
#define CERO_AST_NODE_KIND(X) X,
	CERO_AST_NODE_KINDS
#undef CERO_AST_NODE_KIND
};

template<AstNodeKind K>
struct AstNodeHeader {
	const AstNodeKind kind : 8 = K;
	const SourceOffset offset : SourceOffsetBits = 0;

	AstNodeHeader() = default;

	AstNodeHeader(SourceOffset source_offset) :
		offset(source_offset & 0x00ffffffu) {
	}

	CodeLocation locate_in(const SourceGuard& source, uint8_t tab_size) const {
		return source.locate(offset, tab_size);
	}
};

struct AstRoot {
	AstNodeHeader<AstNodeKind::Root> header;
	uint16_t num_definitions = 0;

	uint32_t num_children() const {
		return num_definitions;
	}
};

enum class AccessSpecifier : uint8_t {
	None,
	Private,
	Public
};

struct AstStructDefinition {
	AstNodeHeader<AstNodeKind::StructDefinition> header;
	AccessSpecifier access = {};
	StringId name;

	uint32_t num_children() const {
		(void) this;
		return 0;
	}
};

struct AstEnumDefinition {
	AstNodeHeader<AstNodeKind::EnumDefinition> header;
	AccessSpecifier access = {};
	StringId name;

	uint32_t num_children() const {
		(void) this;
		return 0;
	}
};

struct AstFunctionDefinition {
	AstNodeHeader<AstNodeKind::FunctionDefinition> header;
	AccessSpecifier access = {};
	StringId name;
	uint16_t num_parameters = 0;
	uint16_t num_outputs = 0;
	uint32_t num_statements = 0;

	uint32_t num_children() const {
		return num_parameters + num_outputs + num_statements;
	}
};

enum class ParameterSpecifier : uint8_t {
	None,
	In,
	Var
};

struct AstFunctionParameter {
	AstNodeHeader<AstNodeKind::FunctionParameter> header;
	ParameterSpecifier specifier = {};
	StringId name;
	bool has_default_argument = false;

	uint32_t num_children() const {
		return has_default_argument ? 2 : 1;
	}
};

struct AstFunctionOutput {
	AstNodeHeader<AstNodeKind::FunctionOutput> header;
	StringId name;

	static uint32_t num_children() {
		return 1;
	}
};

struct AstBlockStatement {
	AstNodeHeader<AstNodeKind::BlockStatement> header;
	uint32_t num_statements = 0;

	uint32_t num_children() const {
		return num_statements;
	}
};

enum class BindingSpecifier : uint8_t {
	Let,
	Var,
	Const,
	Static,
	StaticVar
};

struct AstBindingStatement {
	AstNodeHeader<AstNodeKind::BindingStatement> header;
	BindingSpecifier specifier = {};
	bool has_type = false;
	StringId name;
	bool has_initializer = false;

	uint32_t num_children() const {
		return (has_type ? 1u : 0u) + (has_initializer ? 1 : 0);
	}
};

struct AstIfExpr {
	AstNodeHeader<AstNodeKind::IfExpr> header;
	uint32_t num_then_statements = 0;
	uint32_t num_else_statements = 0;

	uint32_t num_children() const {
		return 1 + num_then_statements + num_else_statements;
	}
};

struct AstWhileLoop {
	AstNodeHeader<AstNodeKind::WhileLoop> header;
	uint32_t num_statements = 0;

	uint32_t num_children() const {
		return 1 + num_statements;
	}
};

struct AstForLoop {
	AstNodeHeader<AstNodeKind::ForLoop> header;
	uint32_t num_statements = 0;

	uint32_t num_children() const {
		return 2 + num_statements;
	}
};

struct AstNameExpr {
	AstNodeHeader<AstNodeKind::NameExpr> header;
	StringId name;

	static uint32_t num_children() {
		return 0;
	}
};

struct AstGenericNameExpr {
	AstNodeHeader<AstNodeKind::GenericNameExpr> header;
	StringId name;
	uint16_t num_generic_args = 0;

	uint32_t num_children() const {
		return num_generic_args;
	}
};

struct AstMemberExpr {
	AstNodeHeader<AstNodeKind::MemberExpr> header;
	StringId member;
	uint16_t num_generic_args = 0;

	uint32_t num_children() const {
		return 1 + num_generic_args;
	}
};

struct AstGroupExpr {
	AstNodeHeader<AstNodeKind::GroupExpr> header;
	uint16_t num_args = 0;

	uint32_t num_children() const {
		return num_args;
	}
};

struct AstCallExpr {
	AstNodeHeader<AstNodeKind::CallExpr> header;
	uint16_t num_args = 0;

	uint32_t num_children() const {
		return 1 + num_args;
	}
};

struct AstIndexExpr {
	AstNodeHeader<AstNodeKind::IndexExpr> header;
	uint16_t num_args = 0;

	uint32_t num_children() const {
		return 1 + num_args;
	}
};

struct AstArrayLiteralExpr {
	AstNodeHeader<AstNodeKind::ArrayLiteralExpr> header;
	uint16_t num_elements = 0;

	uint32_t num_children() const {
		return num_elements;
	}
};

enum class UnaryOperator : uint8_t {
	PreInc,
	PreDec,
	PostInc,
	PostDec,
	Addr,
	Deref,
	Neg,
	Not,
};

std::string_view unary_operator_to_string(UnaryOperator op);

struct AstUnaryExpr {
	AstNodeHeader<AstNodeKind::UnaryExpr> header;
	UnaryOperator op = {};

	static uint32_t num_children() {
		return 1;
	}
};

enum class BinaryOperator : uint8_t {
	Add,
	Sub,
	Mul,
	Div,
	Rem,
	Pow,
	BitAnd,
	BitOr,
	Xor,
	Shl,
	Shr,
	LogicAnd,
	LogicOr,
	Eq,
	NotEq,
	Less,
	LessEq,
	Greater,
	GreaterEq,
	Assign,
	AddAssign,
	SubAssign,
	MulAssign,
	DivAssign,
	RemAssign,
	PowAssign,
	BitAndAssign,
	BitOrAssign,
	XorAssign,
	ShlAssign,
	ShrAssign,
	LogicAndAssign,
	LogicOrAssign
};

std::string_view binary_operator_to_string(BinaryOperator op);

struct AstBinaryExpr {
	AstNodeHeader<AstNodeKind::BinaryExpr> header;
	BinaryOperator op = {};

	static uint32_t num_children() {
		return 2;
	}
};

struct AstReturnExpr {
	AstNodeHeader<AstNodeKind::ReturnExpr> header;
	uint16_t num_expressions = 0;

	uint32_t num_children() const {
		return num_expressions;
	}
};

struct AstThrowExpr {
	AstNodeHeader<AstNodeKind::ThrowExpr> header;
	bool has_expression = false;

	uint32_t num_children() const {
		return has_expression ? 1 : 0;
	}
};

struct AstBreakExpr {
	AstNodeHeader<AstNodeKind::BreakExpr> header;
	bool has_label = false;

	uint32_t num_children() const {
		return has_label ? 1 : 0;
	}
};

struct AstContinueExpr {
	AstNodeHeader<AstNodeKind::ContinueExpr> header;
	bool has_label = false;

	uint32_t num_children() const {
		return has_label ? 1 : 0;
	}
};

enum class NumericLiteralKind : uint8_t {
	Decimal,
	Hexadecimal,
	Binary,
	Octal,
	Float,
	Character
};

struct AstNumericLiteralExpr {
	AstNodeHeader<AstNodeKind::NumericLiteralExpr> header;
	NumericLiteralKind kind = {};

	static uint32_t num_children() {
		return 0;
	}
};

struct AstStringLiteralExpr {
	AstNodeHeader<AstNodeKind::StringLiteralExpr> header;
	std::string value;

	static uint32_t num_children() {
		return 0;
	}
};

enum class PermissionSpecifier : uint8_t {
	In,
	Var,
	VarBounded,
	VarUnbounded
};

struct AstPermissionExpr {
	AstNodeHeader<AstNodeKind::PermissionExpr> header;
	PermissionSpecifier specifier = {};
	uint16_t num_args = 0;

	uint32_t num_children() const {
		return num_args;
	}
};

/// AST node for pointer type expressions, i.e. `^var List<int32>` or `^bool`. The first child node is optional and is the
/// expression provided for the permission. The second child node is the type expression provided for the pointed-to type.
struct AstPointerTypeExpr {
	AstNodeHeader<AstNodeKind::PointerTypeExpr> header;
	bool has_permission = false;

	static uint32_t num_children() {
		return 2;
	}
};

/// AST node for array type expressions, i.e. `[4]int32` or `[]int32`. The first child node is optional and is the expression
/// provided for the array bound. The second child node is the type expression provided for the element type.
struct AstArrayTypeExpr {
	AstNodeHeader<AstNodeKind::ArrayTypeExpr> header;
	bool has_bound = false;

	uint32_t num_children() const {
		return has_bound ? 2 : 1;
	}
};

/// AST node for function type expressions, i.e. `(int32 a, int32)->void`. The first set of children is the parameters, the
/// second set of children are the outputs.
struct AstFunctionTypeExpr { // TODO: handle the fact that parameters are allowed to be completely without name
	AstNodeHeader<AstNodeKind::FunctionTypeExpr> header;
	uint16_t num_parameters = 0;
	uint16_t num_outputs = 0;

	uint32_t num_children() const {
		return num_parameters + num_outputs;
	}
};

} // namespace cero

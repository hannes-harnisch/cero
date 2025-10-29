#pragma once

#include "cero/syntax/ast.hh"

namespace cero {

template<typename T>
class AstVisitor {
protected:
	virtual ~AstVisitor() = default;

	T visit(const AstNode& node) {
		using enum AstNodeKind;

		switch (node.node_kind) {
		case function_def:       return visit_function_definition(static_cast<const AstFunctionDefinition&>(node));
		case function_parameter: return visit_function_parameter(static_cast<const AstFunctionParameter&>(node));
		case function_output:    return visit_function_output(static_cast<const AstFunctionOutput&>(node));
		case ident_expr:         return visit_ident_expr(static_cast<const AstIdentExpr&>(node));
		case literal_expr:       return visit_literal_expr(static_cast<const AstLiteralExpr&>(node));
		case block_expr:         return visit_block_expr(static_cast<const AstBlockExpr&>(node));
		case return_expr:        return visit_return_expr(static_cast<const AstReturnExpr&>(node));
		case unary_expr:         return visit_unary_expr(static_cast<const AstUnaryExpr&>(node));
		case binary_expr:        return visit_binary_expr(static_cast<const AstBinaryExpr&>(node));
		case call_expr:          return visit_call_expr(static_cast<const AstCallExpr&>(node));
		}

		util::assert_unreachable();
	}

	virtual T visit_function_definition(const AstFunctionDefinition& fn_def) = 0;
	virtual T visit_function_parameter(const AstFunctionParameter& param) = 0;
	virtual T visit_function_output(const AstFunctionOutput& output) = 0;
	virtual T visit_ident_expr(const AstIdentExpr& ident_expr) = 0;
	virtual T visit_literal_expr(const AstLiteralExpr& literal_expr) = 0;
	virtual T visit_block_expr(const AstBlockExpr& block_expr) = 0;
	virtual T visit_return_expr(const AstReturnExpr& return_expr) = 0;
	virtual T visit_unary_expr(const AstUnaryExpr& unary_expr) = 0;
	virtual T visit_binary_expr(const AstBinaryExpr& binary_expr) = 0;
	virtual T visit_call_expr(const AstCallExpr& call_expr) = 0;
};

} // namespace cero

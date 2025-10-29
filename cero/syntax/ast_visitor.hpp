#pragma once

#include <cero/syntax/ast.hpp>

namespace cero {

template<typename Derived>
class AstVisitor {
public:
	void visit(const AstNode& node) {
		Derived& self = static_cast<Derived&>(*this);
		using enum AstNodeKind;
		switch (node.node_kind) {
		case root:               return self.visit_root(static_cast<const AstRoot&>(node));
		case function_def:       return self.visit_function_definition(static_cast<const AstFunctionDefinition&>(node));
		case function_parameter: return self.visit_function_parameter(static_cast<const AstFunctionParameter&>(node));
		case function_output:    return self.visit_function_output(static_cast<const AstFunctionOutput&>(node));
		case id_expr:            return self.visit_id_expr(static_cast<const AstIdExpr&>(node));
		case literal_expr:       return self.visit_literal_expr(static_cast<const AstLiteralExpr&>(node));
		case block_expr:         return self.visit_block_expr(static_cast<const AstBlockExpr&>(node));
		case return_expr:        return self.visit_return_expr(static_cast<const AstReturnExpr&>(node));
		case unary_expr:         return self.visit_unary_expr(static_cast<const AstUnaryExpr&>(node));
		case binary_expr:        return self.visit_binary_expr(static_cast<const AstBinaryExpr&>(node));
		case call_expr:          return self.visit_call_expr(static_cast<const AstCallExpr&>(node));
		}
	}
};

} // namespace cero

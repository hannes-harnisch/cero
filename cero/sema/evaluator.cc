#include "evaluator.hh"

#include "cero/syntax/ast_visitor.hh"

namespace cero {

struct Evaluator final : AstVisitor {
	void visit_function_definition(const AstFunctionDefinition& fn_def) override {
	}

	void visit_function_parameter(const AstFunctionParameter& param) override {
	}

	void visit_function_output(const AstFunctionOutput& output) override {
	}

	void visit_ident_expr(const AstIdentExpr& ident_expr) override {
	}

	void visit_literal_expr(const AstLiteralExpr& literal_expr) override {
	}

	void visit_block_expr(const AstBlockExpr& block_expr) override {
	}

	void visit_return_expr(const AstReturnExpr& return_expr) override {
	}

	void visit_unary_expr(const AstUnaryExpr& unary_expr) override {
	}

	void visit_binary_expr(const AstBinaryExpr& binary_expr) override {
	}

	void visit_call_expr(const AstCallExpr& call_expr) override {
	}
};

TypeId evaluate_type() {
}

} // namespace cero

#include "type_checker.hpp"

#include "cero/syntax/ast_visitor.hpp"
#include "cero/type/symbol_context.hpp"

namespace cero {

struct TypeChecker final : AstVisitor {
	const SourceView& source_;
	const std::string_view source_text_;
	SymbolContext ctx_;
	Reporter& reporter_;
	const uint8_t tab_size_;

	TypeChecker(const SourceView& source, Reporter& reporter, uint8_t tab_size) :
	    source_(source),
	    source_text_(source_.get_text()),
	    ctx_(source_text_),
	    reporter_(reporter),
	    tab_size_(tab_size) {
	}

	SymbolTree run(const Ast& ast) && {
		visit_root(ast.get_root());

		return SymbolTree {
		    .table = std::move(ctx_).get_table(),
		};
	}

	void visit_root(const AstRoot& root) override {
		// make all definition names available in symbol table
		for (auto def : root.definitions) {
			ctx_.add_declared_definition(def);
		}

		for (auto def : root.definitions) {
			visit(*def);
		}
	}

	void visit_function_definition(const AstFunctionDefinition& fn_def) override {
	}

	void visit_function_parameter(const AstFunctionParameter& param) override {
	}

	void visit_function_output(const AstFunctionOutput& output) override {
	}

	void visit_id_expr(const AstIdExpr& id_expr) override {
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

SymbolTree run_type_checker(const Ast& ast, const SourceView& source, Reporter& reporter, uint8_t tab_size) {
	return TypeChecker(source, reporter, tab_size).run(ast);
}

} // namespace cero

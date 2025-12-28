#include "type_checker.hh"

#include "cero/sema/symbol_table.hh"
#include "cero/syntax/ast_visitor.hh"

namespace cero {

struct TypeChecker final : AstVisitor {
	TirTree tir_;
	SymbolTable sym_table_;

	const SourceView& source_;
	const std::string_view source_text_;
	Reporter& reporter_;
	const uint8_t tab_size_;

	TypeChecker(const Ast& ast, const SourceView& source, Reporter& reporter, uint8_t tab_size) :
	    tir_(ast.get_memory_usage()),
	    source_(source),
	    source_text_(source_.get_text()),
	    reporter_(reporter),
	    tab_size_(tab_size) {
	}

	void run(const Ast& ast) {
		const AstRoot& root = ast.get_root();
		for (auto def : root.definitions) {
			visit(*def);
		}
	}

	void visit_function_definition(const AstFunctionDefinition& fn_def) override {
		TirFunction* fn = create<TirFunction>(fn_def.token_offset);

		sym_table_.push_scope();

		for (auto param : fn_def.parameters) {
			visit_function_parameter(*param);
		}

		for (auto output : fn_def.outputs) {
			visit_function_output(*output);
		}

		for (auto statement : fn_def.statements) {
			visit(*statement);
		}

		put(tir_.root_->symbols, fn);

		sym_table_.pop_scope();
	}

	void visit_function_parameter(const AstFunctionParameter& param) override {
		std::string_view name = param.name.get(source_text_);
		sym_table_.put(name, SymbolKind::parameter);
	}

	void visit_function_output(const AstFunctionOutput& output) override {
		std::string_view name = output.name.get(source_text_);
		sym_table_.put(name, SymbolKind::output);
	}

	void visit_ident_expr(const AstIdentExpr& ident_expr) override {
		std::string_view name = ident_expr.get(source_text_);
		auto symbol = sym_table_.find(name);
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

	// TIR utilities

	/// Creates a node of type T in storage owned by the TIR tree.
	/// @param  offset  Offset of the token where the given TIR node begins.
	template<std::derived_from<TirNode> T>
	T* create(SourceSize offset) {
		return tir_.arena_.create<T>(offset);
	}

	template<typename T>
	void put(NodeList<T>& list, const std::type_identity_t<T>* node) {
		list.put(node, tir_.arena_);
	}
};

TirTree run_type_checker(const Ast& ast, const SourceView& source, Reporter& reporter, uint8_t tab_size) {
	TypeChecker type_checker(ast, source, reporter, tab_size);
	type_checker.run(ast);
	return std::move(type_checker.tir_);
}

} // namespace cero

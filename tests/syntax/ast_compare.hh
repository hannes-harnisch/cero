#pragma once

#include <cero/syntax/ast_visitor.hh>

namespace tests {

/// Simple implementation of a function reference. Used to reference closures that capture the AstCompare and use it to build up
/// the structure of the deeper levels of the AST.
class ChildScope {
public:
	template<typename F>
	requires(!std::same_as<std::remove_reference_t<F>, ChildScope>)
	explicit(false) ChildScope(F&& f) :
	    object_(&f),
	    fn_(thunk<F>) {
	}

	void operator()() const {
		fn_(object_);
	}

private:
	void* object_;
	void (*fn_)(void*);

	template<typename F>
	static void thunk(void* object) {
		(*static_cast<F*>(object))();
	}
};

using NodeQueue = std::deque<const cero::AstNode*>;

/// Test utility to compare the structure of an AST created by the parser against the structure of nodes recorded via the public
/// methods of this class.
class AstCompare final : public cero::AstVisitor<void> {
public:
	explicit AstCompare(const char* source);
	~AstCompare() override;

	void function_definition(cero::AccessModifier access, std::string_view name, ChildScope cs);
	void function_parameter(std::string_view name, ChildScope cs);
	void function_output(std::string_view name, ChildScope cs);
	void id_expr(std::string_view name);

	AstCompare(AstCompare&&) = delete;

private:
	template<typename T>
	class ExpectedState {
	public:
		ExpectedState() = default;

		~ExpectedState() {
			cero_assert(!value_.has_value(), "test is incorrectly constructed if this fails");
		}

		void set(T val) {
			cero_assert(!value_.has_value(), "test is incorrectly constructed if this fails");
			value_.emplace(std::move(val));
		}

		T get() {
			cero_assert(value_.has_value(), "test is incorrectly constructed if this fails");
			return *std::exchange(value_, std::nullopt);
		}

		ExpectedState(ExpectedState&&) = delete;

	private:
		std::optional<T> value_;
	};

	const cero::Ast ast_;
	const std::string_view source_;
	size_t root_child_count_ = 0;
	size_t recorded_child_count_ = 0;
	NodeQueue next_children_;
	ExpectedState<NodeQueue> expected_children_;
	ExpectedState<cero::AstNodeKind> node_kind_;
	ExpectedState<cero::AccessModifier> access_;
	ExpectedState<std::string_view> name_;
	ExpectedState<cero::UnaryOperator> unary_op_;
	ExpectedState<cero::BinaryOperator> binary_op_;
	ExpectedState<cero::LiteralKind> literal_kind_;

	void visit_root(const cero::AstRoot& root);
	void visit_function_definition(const cero::AstFunctionDefinition& fn_def) override;
	void visit_function_parameter(const cero::AstFunctionParameter& param) override;
	void visit_function_output(const cero::AstFunctionOutput& output) override;
	void visit_ident_expr(const cero::AstIdentExpr& ident_expr) override;
	void visit_literal_expr(const cero::AstLiteralExpr& literal_expr) override;
	void visit_block_expr(const cero::AstBlockExpr& block_expr) override;
	void visit_return_expr(const cero::AstReturnExpr& return_expr) override;
	void visit_unary_expr(const cero::AstUnaryExpr& unary_expr) override;
	void visit_binary_expr(const cero::AstBinaryExpr& binary_expr) override;
	void visit_call_expr(const cero::AstCallExpr& call_expr) override;

	void record(cero::AstNodeKind kind);
	void expect(const cero::AstNode& node, NodeQueue children);
	void visit_node(ChildScope child_scope = [] {});
};

} // namespace tests

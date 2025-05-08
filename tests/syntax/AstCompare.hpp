#pragma once

#include <cero/syntax/AstCursor.hpp>
#include <cero/syntax/AstVisitor.hpp>

#include <doctest/doctest.h>

namespace tests {

class AstCompare : public cero::AstVisitor {
public:
	using ChildScope = void (*)(AstCompare&);

	explicit AstCompare(const cero::Ast& ast);
	~AstCompare() override;

	void struct_definition(cero::AccessSpecifier access, std::string_view name, ChildScope cs);
	void enum_definition(cero::AccessSpecifier access, std::string_view name, ChildScope cs);
	void function_definition(cero::AccessSpecifier access, std::string_view name, ChildScope cs);
	void function_parameter(cero::ParameterSpecifier specifier, std::string_view name, ChildScope cs);
	void function_output(std::string_view name, ChildScope cs);
	void block_statement(ChildScope cs);
	void binding_statement(cero::BindingSpecifier specifier, std::string_view name, ChildScope cs);
	void if_expr(ChildScope cs);
	void while_loop(ChildScope cs);
	void name_expr(std::string_view name);
	void generic_name_expr(std::string_view name, ChildScope cs);
	void member_expr(std::string_view name);
	void group_expr(ChildScope cs);
	void call_expr(ChildScope cs);
	void unary_expr(cero::UnaryOperator op, ChildScope cs);
	void binary_expr(cero::BinaryOperator op, ChildScope cs);
	void return_expr(ChildScope cs);
	void numeric_literal_expr(cero::NumericLiteralKind kind);

	AstCompare(AstCompare&&) = delete;

private:
	template<typename T>
	class RecordState {
	public:
		RecordState() = default;
		RecordState(RecordState&&) = delete;

		~RecordState() {
			cero_assert(!value_.has_value(), "test is misconstructed if this fails");
		}

		void push(T val) {
			cero_assert(!value_.has_value(), "test is misconstructed if this fails");
			value_.emplace(std::move(val));
		}

		T pop() {
			cero_assert(value_.has_value(), "test is misconstructed if this fails");
			return *std::exchange(value_, std::nullopt);
		}

	private:
		std::optional<T> value_;
	};

	cero::AstCursor cursor_;
	uint32_t current_level_ = 0;
	uint32_t current_child_count_ = 0;
	uint32_t root_count_ = 0;
	RecordState<uint32_t> expected_level_;
	RecordState<uint32_t> expected_child_count_;
	RecordState<cero::AstNodeKind> expected_kind_;
	RecordState<cero::AccessSpecifier> expected_access_;
	RecordState<std::string_view> expected_name_;
	RecordState<cero::ParameterSpecifier> expected_param_specifier_;
	RecordState<cero::BindingSpecifier> expected_binding_specifier_;
	RecordState<cero::UnaryOperator> expected_unary_op_;
	RecordState<cero::BinaryOperator> expected_binary_op_;
	RecordState<cero::NumericLiteralKind> expected_numeric_literal_kind_;
	RecordState<std::string> expected_string_literal_;
	RecordState<cero::PermissionSpecifier> expected_permission_specifier_;

	void visit(const cero::AstRoot& root) override;
	void visit(const cero::AstStructDefinition& struct_def) override;
	void visit(const cero::AstEnumDefinition& enum_def) override;
	void visit(const cero::AstFunctionDefinition& func_def) override;
	void visit(const cero::AstFunctionParameter& param) override;
	void visit(const cero::AstFunctionOutput& output) override;
	void visit(const cero::AstBlockStatement& block_stmt) override;
	void visit(const cero::AstBindingStatement& binding) override;
	void visit(const cero::AstIfExpr& if_stmt) override;
	void visit(const cero::AstWhileLoop& while_loop) override;
	void visit(const cero::AstForLoop& for_loop) override;
	void visit(const cero::AstNameExpr& name_expr) override;
	void visit(const cero::AstGenericNameExpr& generic_name_expr) override;
	void visit(const cero::AstMemberExpr& member_expr) override;
	void visit(const cero::AstGroupExpr& group_expr) override;
	void visit(const cero::AstCallExpr& call_expr) override;
	void visit(const cero::AstIndexExpr& index_expr) override;
	void visit(const cero::AstArrayLiteralExpr& array_literal) override;
	void visit(const cero::AstUnaryExpr& unary_expr) override;
	void visit(const cero::AstBinaryExpr& binary_expr) override;
	void visit(const cero::AstReturnExpr& return_expr) override;
	void visit(const cero::AstThrowExpr& throw_expr) override;
	void visit(const cero::AstBreakExpr& break_expr) override;
	void visit(const cero::AstContinueExpr& continue_expr) override;
	void visit(const cero::AstNumericLiteralExpr& numeric_literal) override;
	void visit(const cero::AstStringLiteralExpr& string_literal) override;
	void visit(const cero::AstPermissionExpr& permission) override;
	void visit(const cero::AstPointerTypeExpr& ptr_type) override;
	void visit(const cero::AstArrayTypeExpr& array_type) override;
	void visit(const cero::AstFunctionTypeExpr& func_type) override;

	void record(cero::AstNodeKind kind);
	void visit_children(ChildScope child_scope);

	template<typename N>
	void expect(const N& node);
};

} // namespace tests

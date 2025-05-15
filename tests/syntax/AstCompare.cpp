#include "AstCompare.hpp"

#include <cero/syntax/Ast.hpp>
#include <doctest/doctest.h>

namespace tests {

AstCompare::AstCompare(const cero::Ast& ast) :
	cursor_(ast) {
	expected_kind_.push(cero::AstNodeKind::Root);

	cursor_.visit_one(*this);
	root_count_ = expected_child_count_.pop();
}

AstCompare::~AstCompare() {
	CHECK_EQ(current_child_count_, root_count_);
}

void AstCompare::visit(const cero::AstRoot& root) {
	expect(root);
}

void AstCompare::struct_definition(cero::AccessSpecifier access, std::string_view name, ChildScope cs) {
	record(cero::AstNodeKind::StructDefinition);
	expected_access_.push(access);
	expected_name_.push(name);

	visit_children(cs);
}

void AstCompare::visit(const cero::AstStructDefinition& struct_def) {
	expect(struct_def);
	CHECK_EQ(expected_access_.pop(), struct_def.access);
	CHECK_EQ(expected_name_.pop(), struct_def.name);
}

void AstCompare::enum_definition(cero::AccessSpecifier access, std::string_view name, ChildScope cs) {
	record(cero::AstNodeKind::EnumDefinition);
	expected_access_.push(access);
	expected_name_.push(name);

	visit_children(cs);
}

void AstCompare::visit(const cero::AstEnumDefinition& enum_def) {
	expect(enum_def);
	CHECK_EQ(expected_access_.pop(), enum_def.access);
	CHECK_EQ(expected_name_.pop(), enum_def.name);
}

void AstCompare::function_definition(cero::AccessSpecifier access, std::string_view name, ChildScope cs) {
	record(cero::AstNodeKind::FunctionDefinition);
	expected_access_.push(access);
	expected_name_.push(name);

	visit_children(cs);
}

void AstCompare::visit(const cero::AstFunctionDefinition& func_def) {
	expect(func_def);
	CHECK_EQ(expected_access_.pop(), func_def.access);
	CHECK_EQ(expected_name_.pop(), func_def.name);
}

void AstCompare::function_parameter(cero::ParameterSpecifier specifier, std::string_view name, ChildScope cs) {
	record(cero::AstNodeKind::FunctionParameter);
	expected_param_specifier_.push(specifier);
	expected_name_.push(name);

	visit_children(cs);
}

void AstCompare::visit(const cero::AstFunctionParameter& param) {
	expect(param);
	CHECK_EQ(expected_param_specifier_.pop(), param.specifier);
	CHECK_EQ(expected_name_.pop(), param.name);
}

void AstCompare::function_output(std::string_view name, ChildScope cs) {
	record(cero::AstNodeKind::FunctionOutput);
	expected_name_.push(name);

	visit_children(cs);
}

void AstCompare::visit(const cero::AstFunctionOutput& output) {
	expect(output);
	CHECK_EQ(expected_name_.pop(), output.name);
}

void AstCompare::block_statement(ChildScope cs) {
	record(cero::AstNodeKind::BlockStatement);

	visit_children(cs);
}

void AstCompare::visit(const cero::AstBlockStatement& block_stmt) {
	expect(block_stmt);
}

void AstCompare::binding_statement(cero::BindingSpecifier specifier, std::string_view name, ChildScope cs) {
	record(cero::AstNodeKind::BindingStatement);
	expected_binding_specifier_.push(specifier);
	expected_name_.push(name);

	visit_children(cs);
}

void AstCompare::visit(const cero::AstBindingStatement& binding) {
	expect(binding);
	CHECK_EQ(expected_binding_specifier_.pop(), binding.specifier);
	CHECK_EQ(expected_name_.pop(), binding.name);
}

void AstCompare::if_expr(ChildScope cs) {
	record(cero::AstNodeKind::IfExpr);

	visit_children(cs);
}

void AstCompare::visit(const cero::AstIfExpr& if_stmt) {
	expect(if_stmt);
}

void AstCompare::while_loop(ChildScope cs) {
	record(cero::AstNodeKind::WhileLoop);

	visit_children(cs);
}

void AstCompare::visit(const cero::AstWhileLoop& while_loop) {
	expect(while_loop);
}

void AstCompare::visit(const cero::AstForLoop& for_loop) {
	expect(for_loop);
}

void AstCompare::name_expr(std::string_view name) {
	record(cero::AstNodeKind::NameExpr);
	expected_name_.push(name);

	visit_children(nullptr);
}

void AstCompare::visit(const cero::AstNameExpr& name_expr) {
	expect(name_expr);
	CHECK_EQ(expected_name_.pop(), name_expr.name);
}

void AstCompare::generic_name_expr(std::string_view name, ChildScope cs) {
	record(cero::AstNodeKind::GenericNameExpr);
	expected_name_.push(name);

	visit_children(cs);
}

void AstCompare::visit(const cero::AstGenericNameExpr& generic_name_expr) {
	expect(generic_name_expr);
	CHECK_EQ(expected_name_.pop(), generic_name_expr.name);
}

void AstCompare::member_expr(std::string_view name) {
	record(cero::AstNodeKind::MemberExpr);
	expected_name_.push(name);

	visit_children(nullptr);
}

void AstCompare::visit(const cero::AstMemberExpr& member_expr) {
	expect(member_expr);
	CHECK_EQ(expected_name_.pop(), member_expr.member);
}

void AstCompare::group_expr(ChildScope cs) {
	record(cero::AstNodeKind::GroupExpr);

	visit_children(cs);
}

void AstCompare::visit(const cero::AstGroupExpr& group_expr) {
	expect(group_expr);
}

void AstCompare::call_expr(ChildScope cs) {
	record(cero::AstNodeKind::CallExpr);

	visit_children(cs);
}

void AstCompare::visit(const cero::AstCallExpr& call_expr) {
	expect(call_expr);
}

void AstCompare::visit(const cero::AstIndexExpr& index_expr) {
	expect(index_expr);
}

void AstCompare::visit(const cero::AstArrayLiteralExpr& array_literal) {
	expect(array_literal);
}

void AstCompare::unary_expr(cero::UnaryOperator op, ChildScope cs) {
	record(cero::AstNodeKind::UnaryExpr);
	expected_unary_op_.push(op);

	visit_children(cs);
}

void AstCompare::visit(const cero::AstUnaryExpr& unary_expr) {
	expect(unary_expr);
	CHECK_EQ(expected_unary_op_.pop(), unary_expr.op);
}

void AstCompare::binary_expr(cero::BinaryOperator op, ChildScope cs) {
	record(cero::AstNodeKind::BinaryExpr);
	expected_binary_op_.push(op);

	visit_children(cs);
}

void AstCompare::visit(const cero::AstBinaryExpr& binary_expr) {
	expect(binary_expr);
	CHECK_EQ(expected_binary_op_.pop(), binary_expr.op);
}

void AstCompare::return_expr(ChildScope cs) {
	record(cero::AstNodeKind::ReturnExpr);

	visit_children(cs);
}

void AstCompare::visit(const cero::AstReturnExpr& return_expr) {
	expect(return_expr);
}

void AstCompare::visit(const cero::AstThrowExpr& throw_expr) {
	expect(throw_expr);
}

void AstCompare::visit(const cero::AstBreakExpr& break_expr) {
	expect(break_expr);
}

void AstCompare::visit(const cero::AstContinueExpr& continue_expr) {
	expect(continue_expr);
}

void AstCompare::numeric_literal_expr(cero::NumericLiteralKind kind) {
	record(cero::AstNodeKind::NumericLiteralExpr);
	expected_numeric_literal_kind_.push(kind);

	visit_children(nullptr);
}

void AstCompare::visit(const cero::AstNumericLiteralExpr& numeric_literal) {
	expect(numeric_literal);
	CHECK_EQ(expected_numeric_literal_kind_.pop(), numeric_literal.kind);
}

void AstCompare::visit(const cero::AstStringLiteralExpr& string_literal) {
	expect(string_literal);
	CHECK_EQ(expected_string_literal_.pop(), string_literal.value);
}

void AstCompare::visit(const cero::AstPermissionExpr& permission) {
	expect(permission);
	CHECK_EQ(expected_permission_specifier_.pop(), permission.specifier);
}

void AstCompare::visit(const cero::AstPointerTypeExpr& ptr_type) {
	expect(ptr_type);
}

void AstCompare::visit(const cero::AstArrayTypeExpr& array_type) {
	expect(array_type);
}

void AstCompare::visit(const cero::AstFunctionTypeExpr& func_type) {
	expect(func_type);
}

void AstCompare::record(cero::AstNodeKind kind) {
	expected_kind_.push(kind);
	++current_child_count_;
}

void AstCompare::visit_children(ChildScope child_scope) {
	cursor_.visit_one(*this);

	uint32_t expected_child_count = expected_child_count_.pop();
	uint32_t old_current_count = std::exchange(current_child_count_, 0);

	if (child_scope != nullptr) {
		child_scope(*this);
	}

	CHECK_EQ(current_child_count_, expected_child_count);
	current_child_count_ = old_current_count;
}

template<typename N>
void AstCompare::expect(const N& node) {
	CHECK_EQ(expected_kind_.pop(), node.header.kind);

	expected_child_count_.push(node.num_children());
}

} // namespace tests

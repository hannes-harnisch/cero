#include "AstCompare.hpp"

#include <cero/syntax/Ast.hpp>
#include <doctest/doctest.h>

namespace tests {

AstCompare::AstCompare(const cero::Ast& ast) :
	cursor_(ast) {
}

AstCompare::~AstCompare() {
	CHECK_EQ(current_level_, 0);
}

void AstCompare::root() {
	record(cero::AstNodeKind::Root);

	cursor_.visit_one(*this);
}

void AstCompare::visit(const cero::AstRoot& root) {
	expect(cero::AstNodeKind::Root);
}

void AstCompare::struct_definition(cero::AccessSpecifier access, std::string_view name, ChildScope cs) {
	record(cero::AstNodeKind::StructDefinition);
	next_access.push(access);
	next_name.push(name);
	visit_child();
	record_children(cs);
}

void AstCompare::visit(const cero::AstStructDefinition& struct_def) {
	expect(cero::AstNodeKind::StructDefinition);

	CHECK_EQ(next_access.pop(), struct_def.access);
	CHECK_EQ(next_name.pop(), struct_def.name);
}

void AstCompare::enum_definition(cero::AccessSpecifier access, std::string_view name, ChildScope cs) {
	record(cero::AstNodeKind::EnumDefinition);

	next_access.push(access);
	next_name.push(name);
	record_children(cs);
}

void AstCompare::visit(const cero::AstEnumDefinition& enum_def) {
	expect(cero::AstNodeKind::EnumDefinition);

	CHECK_EQ(next_access.pop(), enum_def.access);
	CHECK_EQ(next_name.pop(), enum_def.name);
}

void AstCompare::function_definition(cero::AccessSpecifier access, std::string_view name, ChildScope cs) {
	record(cero::AstNodeKind::FunctionDefinition);

	next_access.push(access);
	next_name.push(name);
	record_children(cs);
}

void AstCompare::function_parameter(cero::ParameterSpecifier specifier, std::string_view name, ChildScope cs) {
	record(cero::AstNodeKind::FunctionParameter);

	next_param_specifier.push(specifier);
	next_name.push(name);
	record_children(cs);
}

void AstCompare::function_output(std::string_view name, ChildScope cs) {
	record(cero::AstNodeKind::FunctionOutput);

	next_name.push(name);
	record_children(cs);
}

void AstCompare::visit(const cero::AstFunctionDefinition& func_def) {
	expect(cero::AstNodeKind::FunctionDefinition);

	CHECK_EQ(next_access.pop(), func_def.access);
	CHECK_EQ(next_name.pop(), func_def.name);

	visit_children(func_def.num_parameters);
	visit_children(func_def.num_outputs);
	visit_children(func_def.num_statements);
}

void AstCompare::visit(const cero::AstFunctionParameter& param) {
	expect(cero::AstNodeKind::FunctionParameter);

	CHECK_EQ(next_param_specifier.pop(), param.specifier);
	CHECK_EQ(next_name.pop(), param.name);

	visit_child(); // type
	visit_child_if(param.has_default_argument);
}

void AstCompare::visit(const cero::AstFunctionOutput& output) {
	expect(cero::AstNodeKind::FunctionOutput);

	CHECK_EQ(next_name.pop(), output.name);

	visit_child(); // type
}

void AstCompare::block_statement(ChildScope cs) {
	record(cero::AstNodeKind::BlockStatement);
	record_children(cs);
}

void AstCompare::visit(const cero::AstBlockStatement& block_stmt) {
	expect(cero::AstNodeKind::BlockStatement);

	visit_children(block_stmt.num_statements);
}

void AstCompare::binding_statement(cero::BindingSpecifier specifier, std::string_view name, ChildScope cs) {
	record(cero::AstNodeKind::BindingStatement);

	next_binding_specifier.push(specifier);
	next_name.push(name);
	record_children(cs);
}

void AstCompare::visit(const cero::AstBindingStatement& binding) {
	expect(cero::AstNodeKind::BindingStatement);

	CHECK_EQ(next_binding_specifier.pop(), binding.specifier);
	CHECK_EQ(next_name.pop(), binding.name);

	visit_child_if(binding.has_type);
	visit_child_if(binding.has_initializer);
}

void AstCompare::if_expr(ChildScope cs) {
	record(cero::AstNodeKind::IfExpr);
	record_children(cs);
}

void AstCompare::visit(const cero::AstIfExpr& if_stmt) {
	expect(cero::AstNodeKind::IfExpr);

	visit_child(); // condition
	visit_children(if_stmt.num_then_statements);
	visit_children(if_stmt.num_else_statements);
}

void AstCompare::while_loop(ChildScope cs) {
	record(cero::AstNodeKind::WhileLoop);
	record_children(cs);
}

void AstCompare::visit(const cero::AstWhileLoop& while_loop) {
	expect(cero::AstNodeKind::WhileLoop);

	visit_child();
	visit_children(while_loop.num_statements);
}

void AstCompare::visit(const cero::AstForLoop& for_loop) {
	expect(cero::AstNodeKind::ForLoop);

	visit_child();
	visit_child();
	visit_children(for_loop.num_statements);
}

void AstCompare::name_expr(std::string_view name) {
	record(cero::AstNodeKind::NameExpr);

	next_name.push(name);
}

void AstCompare::generic_name_expr(std::string_view name, ChildScope cs) {
	record(cero::AstNodeKind::GenericNameExpr);

	next_name.push(name);
	record_children(cs);
}

void AstCompare::visit(const cero::AstNameExpr& name_expr) {
	expect(cero::AstNodeKind::NameExpr);

	CHECK_EQ(next_name.pop(), name_expr.name);
}

void AstCompare::visit(const cero::AstGenericNameExpr& generic_name_expr) {
	expect(cero::AstNodeKind::GenericNameExpr);

	CHECK_EQ(next_name.pop(), generic_name_expr.name);

	visit_children(generic_name_expr.num_generic_args);
}

void AstCompare::member_expr(std::string_view name) {
	record(cero::AstNodeKind::MemberExpr);

	next_name.push(name);
}

void AstCompare::visit(const cero::AstMemberExpr& member_expr) {
	expect(cero::AstNodeKind::MemberExpr);

	CHECK_EQ(next_name.pop(), member_expr.member);

	visit_child();
	visit_children(member_expr.num_generic_args);
}

void AstCompare::group_expr(ChildScope cs) {
	record(cero::AstNodeKind::GroupExpr);
	record_children(cs);
}

void AstCompare::visit(const cero::AstGroupExpr& group_expr) {
	expect(cero::AstNodeKind::GroupExpr);

	visit_children(group_expr.num_args);
}

void AstCompare::call_expr(ChildScope cs) {
	record(cero::AstNodeKind::CallExpr);
	record_children(cs);
}

void AstCompare::visit(const cero::AstCallExpr& call_expr) {
	expect(cero::AstNodeKind::CallExpr);

	visit_child();
	visit_children(call_expr.num_args);
}

void AstCompare::visit(const cero::AstIndexExpr& index_expr) {
	expect(cero::AstNodeKind::IndexExpr);

	visit_child();
	visit_children(index_expr.num_args);
}

void AstCompare::visit(const cero::AstArrayLiteralExpr& array_literal) {
	expect(cero::AstNodeKind::ArrayLiteralExpr);

	visit_children(array_literal.num_elements);
}

void AstCompare::unary_expr(cero::UnaryOperator op, ChildScope cs) {
	record(cero::AstNodeKind::UnaryExpr);

	next_unary_op.push(op);
	record_children(cs);
}

void AstCompare::visit(const cero::AstUnaryExpr& unary_expr) {
	expect(cero::AstNodeKind::UnaryExpr);

	CHECK_EQ(next_unary_op.pop(), unary_expr.op);

	visit_child();
}

void AstCompare::binary_expr(cero::BinaryOperator op, ChildScope cs) {
	record(cero::AstNodeKind::BinaryExpr);

	next_binary_op.push(op);
	record_children(cs);
}

void AstCompare::visit(const cero::AstBinaryExpr& binary_expr) {
	expect(cero::AstNodeKind::BinaryExpr);

	CHECK_EQ(next_binary_op.pop(), binary_expr.op);

	visit_child();
	visit_child();
}

void AstCompare::return_expr(ChildScope cs) {
	record(cero::AstNodeKind::ReturnExpr);
	record_children(cs);
}

void AstCompare::visit(const cero::AstReturnExpr& return_expr) {
	expect(cero::AstNodeKind::ReturnExpr);

	visit_children(return_expr.num_expressions);
}

void AstCompare::visit(const cero::AstThrowExpr& throw_expr) {
	expect(cero::AstNodeKind::ThrowExpr);

	visit_child_if(throw_expr.has_expression);
}

void AstCompare::visit(const cero::AstBreakExpr& break_expr) {
	expect(cero::AstNodeKind::BreakExpr);

	visit_child_if(break_expr.has_label);
}

void AstCompare::visit(const cero::AstContinueExpr& continue_expr) {
	expect(cero::AstNodeKind::ContinueExpr);

	visit_child_if(continue_expr.has_label);
}

void AstCompare::numeric_literal_expr(cero::NumericLiteralKind kind) {
	record(cero::AstNodeKind::NumericLiteralExpr);

	next_numeric_literal_kind.push(kind);
}

void AstCompare::visit(const cero::AstNumericLiteralExpr& numeric_literal) {
	expect(cero::AstNodeKind::NumericLiteralExpr);

	CHECK_EQ(next_numeric_literal_kind.pop(), numeric_literal.kind);
}

void AstCompare::visit(const cero::AstStringLiteralExpr& string_literal) {
	expect(cero::AstNodeKind::StringLiteralExpr);

	CHECK_EQ(next_string_literal.pop(), string_literal.value);
}

void AstCompare::visit(const cero::AstPermissionExpr& permission) {
	expect(cero::AstNodeKind::PermissionExpr);

	CHECK_EQ(next_permission_specifier.pop(), permission.specifier);

	visit_children(permission.num_args);
}

void AstCompare::visit(const cero::AstPointerTypeExpr& ptr_type) {
	expect(cero::AstNodeKind::PointerTypeExpr);

	visit_child_if(ptr_type.has_permission);
	visit_child();
}

void AstCompare::visit(const cero::AstArrayTypeExpr& array_type) {
	expect(cero::AstNodeKind::ArrayTypeExpr);

	visit_child_if(array_type.has_bound);
	visit_child();
}

void AstCompare::visit(const cero::AstFunctionTypeExpr& func_type) {
	expect(cero::AstNodeKind::FunctionTypeExpr);

	visit_children(func_type.num_parameters);
	visit_children(func_type.num_outputs);
}

void AstCompare::visit_child() {
	++current_level_;
	cursor_.visit_one(*this);
	--current_level_;
}

void AstCompare::visit_child_if(bool condition) {
	if (condition) {
		visit_child();
	}
}

void AstCompare::visit_children(uint32_t n) {
	++current_level_;
	cursor_.visit_children(n, *this);
	--current_level_;
}

void AstCompare::expect(cero::AstNodeKind kind) {
	CHECK_EQ(next_kind.pop(), kind);
	CHECK_EQ(next_level.pop(), current_level_);
}

void AstCompare::record(cero::AstNodeKind kind) {
	next_kind.push(kind);
	next_level.push(current_level_);
}

void AstCompare::record_children(ChildScope child_scope) {
	++current_level_;
	child_scope(*this);
	--current_level_;
}

} // namespace tests

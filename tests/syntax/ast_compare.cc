#include "ast_compare.hh"

#include "common/doctest.hh"
#include "syntax/common.hh"

#include <cero/util/push_state.hh>

namespace tests {

namespace {

template<typename... Ranges>
NodeQueue gather_children(const Ranges&... ranges) {
	NodeQueue children;
	(children.insert(children.end(), std::begin(ranges), std::end(ranges)), ...);
	return children;
}

} // namespace

AstCompare::AstCompare(const char* source) :
    ast_(parse_source(source)),
    source_(source) {
	visit_root(ast_.get_root());
}

AstCompare::~AstCompare() {
	CHECK_EQ(recorded_child_count_, root_child_count_);
}

void AstCompare::visit_root(const cero::AstRoot& root) {
	next_children_ = gather_children(root.definitions);
	root_child_count_ = next_children_.size();
}

void AstCompare::function_definition(cero::AccessModifier access, std::string_view name, ChildScope cs) {
	record(cero::AstNodeKind::function_def);

	access_.set(access);
	str_.set(name);
	visit_node(cs);
}

void AstCompare::visit_function_definition(const cero::AstFunctionDefinition& fn_def) {
	expect(fn_def, gather_children(fn_def.parameters, fn_def.outputs, fn_def.statements));

	CHECK_EQ(access_.get(), fn_def.access_modifier);
	CHECK_EQ(str_.get(), fn_def.name.get(source_));
}

void AstCompare::function_parameter(std::string_view name, ChildScope cs) {
	record(cero::AstNodeKind::function_parameter);

	str_.set(name);
	visit_node(cs);
}

void AstCompare::visit_function_parameter(const cero::AstFunctionParameter& param) {
	expect(param, gather_children(std::array {param.type}));

	CHECK_EQ(str_.get(), param.name.get(source_));
}

void AstCompare::function_output(std::string_view name, ChildScope cs) {
	record(cero::AstNodeKind::function_output);

	str_.set(name);
	visit_node(cs);
}

void AstCompare::visit_function_output(const cero::AstFunctionOutput& output) {
	expect(output, gather_children(std::array {output.type}));
	CHECK_EQ(str_.get(), output.name.get(source_));
}

void AstCompare::ident_expr(std::string_view ident) {
	record(cero::AstNodeKind::ident_expr);

	str_.set(ident);
	visit_node();
}

void AstCompare::visit_ident_expr(const cero::AstIdentExpr& ident_expr) {
	expect(ident_expr, {});

	std::string_view ident = ident_expr.get(source_);
	CHECK_EQ(str_.get(), ident);
}

void AstCompare::literal_expr(cero::LiteralKind literal_kind, std::string_view literal) {
	record(cero::AstNodeKind::literal_expr);

	literal_kind_.set(literal_kind);
	str_.set(literal);
}

void AstCompare::visit_literal_expr(const cero::AstLiteralExpr& literal_expr) {
	expect(literal_expr, {});

	std::string_view literal = literal_expr.get(source_);
	CHECK_EQ(str_.get(), literal);
	CHECK_EQ(literal_kind_.get(), literal_expr.literal_kind);
}

void AstCompare::block_expr(ChildScope cs) {
	record(cero::AstNodeKind::block_expr);
	visit_node(cs);
}

void AstCompare::visit_block_expr(const cero::AstBlockExpr& block_expr) {
	expect(block_expr, gather_children(block_expr.statements));
}

void AstCompare::return_expr(ChildScope cs) {
	record(cero::AstNodeKind::return_expr);
	visit_node(cs);
}

void AstCompare::visit_return_expr(const cero::AstReturnExpr& return_expr) {
	expect(return_expr, gather_children(return_expr.ret_values));
}

void AstCompare::unary_expr(cero::UnaryOperator op, ChildScope cs) {
	record(cero::AstNodeKind::unary_expr);

	unary_op_.set(op);
	visit_node(cs);
}

void AstCompare::visit_unary_expr(const cero::AstUnaryExpr& unary_expr) {
	expect(unary_expr, gather_children(std::array {unary_expr.operand}));

	CHECK_EQ(unary_op_.get(), unary_expr.op);
}

void AstCompare::binary_expr(cero::BinaryOperator op, ChildScope cs) {
	record(cero::AstNodeKind::binary_expr);

	binary_op_.set(op);
	visit_node(cs);
}

void AstCompare::visit_binary_expr(const cero::AstBinaryExpr& binary_expr) {
	expect(binary_expr, gather_children(std::array {binary_expr.left, binary_expr.right}));

	CHECK_EQ(binary_op_.get(), binary_expr.op);
}

void AstCompare::call_expr(ChildScope cs) {
	record(cero::AstNodeKind::call_expr);

	visit_node(cs);
}

void AstCompare::visit_call_expr(const cero::AstCallExpr& call_expr) {
	expect(call_expr, gather_children(std::array {call_expr.callee}, call_expr.args));
}

void AstCompare::record(cero::AstNodeKind kind) {
	node_kind_.set(kind);
	++recorded_child_count_;
}

void AstCompare::expect(const cero::AstNode& node, NodeQueue children) {
	REQUIRE_EQ(node_kind_.get(), node.node_kind);
	expected_children_.set(std::move(children));
}

void AstCompare::visit_node(ChildScope child_scope) {
	REQUIRE(!next_children_.empty());
	const cero::AstNode* next = next_children_.front();
	next_children_.pop_front();
	visit(*next);

	NodeQueue expected_children = expected_children_.get();
	size_t expected_child_count = expected_children.size();

	util::PushState rcc(recorded_child_count_, 0);
	util::PushState nc(next_children_, std::move(expected_children));

	child_scope();

	REQUIRE_EQ(recorded_child_count_, expected_child_count);
}

} // namespace tests

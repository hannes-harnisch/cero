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
	name_.set(name);

	visit_node(cs);
}

void AstCompare::visit_function_definition(const cero::AstFunctionDefinition& fn_def) {
	expect(fn_def, gather_children(fn_def.parameters, fn_def.outputs, fn_def.statements));

	CHECK_EQ(access_.get(), fn_def.access_modifier);
	CHECK_EQ(name_.get(), fn_def.name.get(source_));
}

void AstCompare::function_parameter(std::string_view name, ChildScope cs) {
	record(cero::AstNodeKind::function_parameter);
	name_.set(name);

	visit_node(cs);
}

void AstCompare::visit_function_parameter(const cero::AstFunctionParameter& param) {
	// TODO

	CHECK_EQ(name_.get(), param.name.get(source_));
}

void AstCompare::function_output(std::string_view, ChildScope) {
}

void AstCompare::visit_function_output(const cero::AstFunctionOutput&) {
}

void AstCompare::id_expr(std::string_view) {
}

void AstCompare::visit_ident_expr(const cero::AstIdentExpr&) {
}

void AstCompare::visit_literal_expr(const cero::AstLiteralExpr&) {
}

void AstCompare::visit_block_expr(const cero::AstBlockExpr&) {
}

void AstCompare::visit_return_expr(const cero::AstReturnExpr&) {
}

void AstCompare::visit_unary_expr(const cero::AstUnaryExpr&) {
}

void AstCompare::visit_binary_expr(const cero::AstBinaryExpr& binary_expr) {
	expect(binary_expr, gather_children(std::array {binary_expr.left, binary_expr.right}));
}

void AstCompare::visit_call_expr(const cero::AstCallExpr&) {
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

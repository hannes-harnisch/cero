#include "AstToString.hpp"

#include "cero/syntax/AstCursor.hpp"
#include "cero/util/Assert.hpp"

namespace cero {

namespace {

std::string_view parameter_specifier_to_string(ParameterSpecifier specifier) {
	switch (specifier) {
		using enum ParameterSpecifier;
	case None: return "value";
	case In:   return "in";
	case Var:  return "var";
	}
	assert_unreachable();
}

std::string_view permission_specifier_to_string(PermissionSpecifier spec) {
	switch (spec) {
		using enum PermissionSpecifier;
	case In:		   return "in";
	case Var:		   return "var";
	case VarBounded:   return "var (bounded)";
	case VarUnbounded: return "var (unbounded)";
	}
	assert_unreachable();
}

std::string_view binding_specifier_to_string(BindingSpecifier spec) {
	switch (spec) {
		using enum BindingSpecifier;
	case Let:		return "let";
	case Var:		return "var";
	case Const:		return "const";
	case Static:	return "static";
	case StaticVar: return "static var";
	}
	assert_unreachable();
}

std::string_view numeric_literal_kind_to_string(NumericLiteralKind kind) {
	switch (kind) {
		using enum NumericLiteralKind;
	case Decimal:	  return "decimal";
	case Hexadecimal: return "hexadecimal";
	case Binary:	  return "binary";
	case Octal:		  return "octal";
	case Float:		  return "float";
	case Character:	  return "character";
	}
	assert_unreachable();
}

class AstToString : public AstVisitor {
public:
	AstToString(const Ast& ast, const SourceGuard& source, uint8_t tab_size) :
		string_(fmt::format("AST for {} ({} node{})\n", source.get_name(), ast.num_nodes(), ast.num_nodes() == 1 ? "" : "s")),
		cursor_(ast),
		source_(source),
		tab_size_(tab_size),
		edge_(&Body) {
		prefixes_.emplace();
	}

	std::string make_string() && {
		cursor_.visit_all(*this);
		return std::move(string_);
	}

private:
	struct Edge {
		std::string_view branch;
		std::string_view prefix;
	};

	static constexpr Edge Body {"├── ", "│   "};
	static constexpr Edge Tail {"└── ", "    "};

	std::string string_;
	AstCursor cursor_;
	const SourceGuard& source_;
	const uint8_t tab_size_;

	std::stack<std::string> prefixes_;
	const Edge* edge_;

	template<typename T>
	std::string locate(const T& t) const {
		return t.header.locate_in(source_, tab_size_).to_short_string();
	}

	void push_level() {
		std::string prefix = prefixes_.top();
		prefix.append(edge_->prefix);
		prefixes_.push(std::move(prefix));
	}

	void pop_level() {
		prefixes_.pop();
	}

	void set_tail(bool at_tail) {
		edge_ = at_tail ? &Tail : &Body;
	}

	void add_line(std::string_view text) {
		string_.append(prefixes_.top());
		string_.append(edge_->branch);
		string_.append(text);
		string_.append("\n");
	}

	void add_body_line(std::string_view text) {
		set_tail(false);
		add_line(text);
	}

	void add_tail_line(std::string_view text) {
		set_tail(true);
		add_line(text);
	}

	void visit_child_at_body() {
		set_tail(false);
		visit_child();
	}

	void visit_child_at_tail() {
		set_tail(true);
		visit_child();
	}

	void visit_child() {
		cursor_.visit_child(*this);
	}

	void visit_child_if(bool condition) {
		if (condition) {
			push_level();
			visit_child_at_tail();
			pop_level();
		}
	}

	void visit_children(uint32_t n) {
		while (n > 0) {
			set_tail(--n == 0);
			cursor_.visit_child(*this);
		}
	}

	void visit(const AstRoot& root) override {
		visit_children(root.num_definitions);
	}

	void visit(const AstStructDefinition& struct_def) override {
		add_line(fmt::format("struct `{}`", struct_def.name));
		push_level();

		pop_level();
	}

	void visit(const AstEnumDefinition& enum_def) override {
		add_line(fmt::format("enum `{}`", enum_def.name));
		push_level();

		pop_level();
	}

	void visit(const AstFunctionDefinition& func_def) override {
		add_line(fmt::format("function `{}` {}", func_def.name, locate(func_def)));
		push_level();

		add_body_line("parameters");
		push_level();
		visit_children(func_def.num_parameters);
		pop_level();

		add_body_line("outputs");
		push_level();
		visit_children(func_def.num_outputs);
		pop_level();

		add_tail_line("statements");
		push_level();
		visit_children(func_def.num_statements);
		pop_level();

		pop_level();
	}

	void visit(const AstFunctionParameter& param) override {
		auto specifier = parameter_specifier_to_string(param.specifier);
		add_line(fmt::format("{} parameter `{}` {}", specifier, param.name, locate(param)));
		push_level();

		set_tail(!param.has_default_argument);
		visit_child(); // visit type
		if (param.has_default_argument) {
			visit_child_at_tail();
		}

		pop_level();
	}

	void visit(const AstFunctionOutput& output) override {
		if (output.name.empty()) {
			add_line(fmt::format("output {}", locate(output)));
		} else {
			add_line(fmt::format("output `{}` {}", output.name, locate(output)));
		}

		push_level();
		visit_child_at_tail();
		pop_level();
	}

	void visit(const AstBlockStatement& block_stmt) override {
		add_line(fmt::format("block {}", locate(block_stmt)));

		push_level();
		visit_children(block_stmt.num_statements);
		pop_level();
	}

	void visit(const AstBindingStatement& binding) override {
		auto specifier = binding_specifier_to_string(binding.specifier);
		add_line(fmt::format("{} binding `{}` {}", specifier, binding.name, locate(binding)));
		push_level();

		if (binding.has_type) {
			set_tail(!binding.has_initializer);
			add_line("type");
			push_level();
			visit_child_at_tail();
			pop_level();
		}

		if (binding.has_initializer) {
			add_tail_line("initializer");
			push_level();
			visit_child_at_tail();
			pop_level();
		}

		pop_level();
	}

	void visit(const AstIfExpr& if_stmt) override {
		add_line(fmt::format("if {}", locate(if_stmt)));
		push_level();

		add_body_line("condition");
		push_level();
		visit_child_at_tail();
		pop_level();

		set_tail(if_stmt.num_else_statements == 0);
		add_line("then");
		push_level();
		visit_children(if_stmt.num_then_statements);
		pop_level();

		if (if_stmt.num_else_statements > 0) {
			add_tail_line("else");
			push_level();
			visit_children(if_stmt.num_else_statements);
			pop_level();
		}

		pop_level();
	}

	void visit(const AstWhileLoop& while_loop) override {
		add_line(fmt::format("while {}", locate(while_loop)));
		push_level();

		visit_child_at_body();

		add_tail_line("statements");
		push_level();
		visit_children(while_loop.num_statements);
		pop_level();

		pop_level();
	}

	void visit(const AstForLoop& for_loop) override {
		add_line(fmt::format("for {}", locate(for_loop)));
		push_level();

		visit_child_at_body(); // binding
		visit_child_at_body(); // range

		add_tail_line("statements");
		push_level();
		visit_children(for_loop.num_statements);
		pop_level();

		pop_level();
	}

	void visit(const AstNameExpr& name_expr) override {
		add_line(fmt::format("name `{}` {}", name_expr.name, locate(name_expr)));
	}

	void visit(const AstGenericNameExpr& generic_name_expr) override {
		add_line(fmt::format("generic name `{}` {}", generic_name_expr.name, locate(generic_name_expr)));

		push_level();
		visit_children(generic_name_expr.num_generic_args);
		pop_level();
	}

	void visit(const AstMemberExpr& member_expr) override {
		add_line(fmt::format("member `{}` {}", member_expr.member, locate(member_expr)));

		if (member_expr.num_generic_args > 0) {
			add_tail_line("generic arguments");
			push_level();
			visit_children(member_expr.num_generic_args);
			pop_level();
		}
	}

	void visit(const AstGroupExpr& group_expr) override {
		if (group_expr.num_args == 1) {
			visit_child();
			return;
		}

		add_line(fmt::format("group expression {}", locate(group_expr)));
		push_level();
		visit_children(group_expr.num_args);
		pop_level();
	}

	void visit(const AstCallExpr& call_expr) override {
		add_line(fmt::format("call expression {}", locate(call_expr)));
		push_level();

		visit_child_at_body();

		add_tail_line("arguments");
		push_level();
		visit_children(call_expr.num_args);
		pop_level();

		pop_level();
	}

	void visit(const AstIndexExpr& index_expr) override {
		add_line(fmt::format("index expression {}", locate(index_expr)));
		push_level();

		visit_child_at_body();

		add_tail_line("arguments");
		push_level();
		visit_children(index_expr.num_args);
		pop_level();

		pop_level();
	}

	void visit(const AstArrayLiteralExpr& array_literal) override {
		add_line(fmt::format("array literal {}", locate(array_literal)));

		push_level();
		visit_children(array_literal.num_elements);
		pop_level();
	}

	void visit(const AstUnaryExpr& unary_expr) override {
		auto op = unary_operator_to_string(unary_expr.op);
		add_line(fmt::format("`{}` {}", op, locate(unary_expr)));
		push_level();

		visit_child_at_tail();

		pop_level();
	}

	void visit(const AstBinaryExpr& binary_expr) override {
		auto op = binary_operator_to_string(binary_expr.op);
		add_line(fmt::format("`{}` {}", op, locate(binary_expr)));
		push_level();

		visit_child_at_body();
		visit_child_at_tail();

		pop_level();
	}

	void visit(const AstReturnExpr& return_expr) override {
		add_line(fmt::format("return {}", locate(return_expr)));

		push_level();
		visit_children(return_expr.num_expressions);
		pop_level();
	}

	void visit(const AstThrowExpr& throw_expr) override {
		add_line(fmt::format("throw {}", locate(throw_expr)));
		visit_child_if(throw_expr.has_expression);
	}

	void visit(const AstBreakExpr& break_expr) override {
		add_line(fmt::format("break {}", locate(break_expr)));
		visit_child_if(break_expr.has_label);
	}

	void visit(const AstContinueExpr& continue_expr) override {
		add_line(fmt::format("continue {}", locate(continue_expr)));
		visit_child_if(continue_expr.has_label);
	}

	void visit(const AstNumericLiteralExpr& numeric_literal) override {
		auto kind = numeric_literal_kind_to_string(numeric_literal.kind);
		add_line(fmt::format("{} literal `{}` {}", kind, " ---TODO--- ", locate(numeric_literal))); // TODO: add number
	}

	void visit(const AstStringLiteralExpr& string_literal) override {
		add_line(fmt::format("string literal `{}` {}", string_literal.value, locate(string_literal)));
	}

	void visit(const AstPermissionExpr& permission) override {
		auto specifier = permission_specifier_to_string(permission.specifier);
		add_line(fmt::format("permission `{}` {}", specifier, locate(permission)));

		push_level();
		visit_children(permission.num_args);
		pop_level();
	}

	void visit(const AstPointerTypeExpr& ptr_type) override {
		add_line(fmt::format("pointer type {}", locate(ptr_type)));
		push_level();

		if (ptr_type.has_permission) {
			add_body_line("permission");
			push_level();
			visit_child_at_tail();
			pop_level();
		}

		add_tail_line("type");
		push_level();
		visit_child_at_tail();
		pop_level();

		pop_level();
	}

	void visit(const AstArrayTypeExpr& array_type) override {
		add_line(fmt::format("array type {}", locate(array_type)));
		push_level();

		if (array_type.has_bound) {
			add_body_line("bound");
			push_level();
			visit_child_at_tail();
			pop_level();
		}

		add_tail_line("element type");
		push_level();
		visit_child_at_tail();
		pop_level();

		pop_level();
	}

	void visit(const AstFunctionTypeExpr& func_type) override {
		add_line(fmt::format("function type {}", locate(func_type)));
		push_level();

		add_body_line("parameters");
		push_level();
		visit_children(func_type.num_parameters);
		pop_level();

		add_tail_line("outputs");
		push_level();
		visit_children(func_type.num_outputs);
		pop_level();

		pop_level();
	}
};

} // namespace

std::string ast_to_string(const Ast& ast, const SourceGuard& source, uint8_t tab_size) {
	return AstToString(ast, source, tab_size).make_string();
}

} // namespace cero

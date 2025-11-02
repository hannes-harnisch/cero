#include "parser.hh"

#include "cero/syntax/token_cursor.hh"

namespace cero {

enum class Precedence {
	statement,
	assignment,
	logical,
	comparison,
	additive_or_bit,
	multiplicative,
	prefix,
	postfix
};

consteval Precedence lookup_precedence_for_associativity(BinaryOperator op) {
	using enum BinaryOperator;

	switch (op) {
	case add:
	case sub: return Precedence::additive_or_bit;

	case mul:
	case div:
	case rem: return Precedence::multiplicative;

	case bit_and:
	case bit_or:
	case bit_xor:
	case shl:
	case shr:     return Precedence::additive_or_bit;

	case eq:
	case neq:
	case less:
	case less_eq:
	case greater:
	case greater_eq: return Precedence::comparison;

	case logic_and:
	case logic_or:  return Precedence::logical;

	case assign:
	case add_assign:
	case sub_assign:
	case mul_assign:
	case div_assign:
	case rem_assign:
	case bit_and_assign:
	case bit_or_assign:
	case xor_assign:
	case shl_assign:
	case shr_assign:
	case logic_and_assign:
	case logic_or_assign:  return Precedence::assignment;
	}

	util::assert_unreachable();
}

struct ParseError {};

struct Parser {
	Ast ast_;
	TokenCursor cursor_;
	const SourceView& source_;
	Reporter& reporter_;
	const uint8_t tab_size_;

	Parser(const TokenList& token_list, const SourceView& source, Reporter& reporter, uint8_t tab_size) :
	    ast_(token_list.get_array().size() * sizeof(AstNode)), // heuristic memory usage, should be fine-tuned
	    cursor_(token_list),
	    source_(source),
	    reporter_(reporter),
	    tab_size_(tab_size) {
	}

	void run() {
		while (!cursor_.match(TokenKind::end_of_file)) {
			try {
				AstDefinition* definition = parse_definition();
				put(ast_.root_->definitions, definition);
			}
			catch (ParseError) {
				recover_at_definition_level();
			}
		}
	}

	void recover_at_definition_level() {
		static constexpr TokenKind recovery_tokens[] {
		    TokenKind::kw_public,
		    TokenKind::kw_private,
		    TokenKind::kw_struct,
		    TokenKind::kw_enum,
		    TokenKind::end_of_file,
		};

		TokenKind kind;
		do {
			cursor_.advance();
			kind = cursor_.peek_kind();
		}
		while (!std::ranges::contains(recovery_tokens, kind));
	}

	AstDefinition* parse_definition() {
		SourceSize offset = cursor_.peek_offset();

		AccessModifier access_modifier = AccessModifier::none;
		if (cursor_.match(TokenKind::kw_private)) {
			access_modifier = AccessModifier::private_;
		}
		else if (cursor_.match(TokenKind::kw_public)) {
			access_modifier = AccessModifier::public_;
		}

		if (std::optional token = cursor_.match_token(TokenKind::identifier)) {
			return parse_function_definition(offset, access_modifier, *token);
		}
		else {
			report_expected(Message::expect_function_struct_or_enum);
			throw ParseError();
		}
	}

	AstFunctionDefinition* parse_function_definition(SourceSize offset, AccessModifier access_modifier, Token identifier) {
		AstFunctionDefinition* fn_def = create<AstFunctionDefinition>(offset);
		fn_def->access_modifier = access_modifier;
		fn_def->name = AstName::from(identifier);

		expect(TokenKind::l_paren, Message::expect_paren_after_function_name);
		fn_def->parameters = parse_function_definition_parameters();
		fn_def->outputs = parse_function_definition_outputs();

		expect(TokenKind::l_brace, Message::expect_brace_before_function_body);
		fn_def->statements = parse_block();

		return fn_def;
	}

	NodeList<AstFunctionParameter> parse_function_definition_parameters() {
		NodeList<AstFunctionParameter> parameters;

		if (!cursor_.match(TokenKind::r_paren)) {
			do {
				AstFunctionParameter* param = parse_function_definition_parameter();
				put(parameters, param);
			}
			while (cursor_.match(TokenKind::comma));

			expect(TokenKind::r_paren, Message::expect_paren_after_parameters);
		}

		return parameters;
	}

	AstFunctionParameter* parse_function_definition_parameter() {
		SourceSize offset = cursor_.peek_offset();
		AstFunctionParameter* param = create<AstFunctionParameter>(offset);

		param->type = parse_type(Message::expect_parameter_type);
		param->name = expect_identifier(Message::expect_parameter_name);

		// abort parsing and recover at the next definition so errors don't accumulate from a malformed function signature
		if (param->name.is_empty()) {
			throw ParseError();
		}

		return param;
	}

	NodeList<AstFunctionOutput> parse_function_definition_outputs() {
		NodeList<AstFunctionOutput> outputs;

		if (cursor_.match(TokenKind::thin_arrow)) {
			do {
				AstFunctionOutput* output = parse_function_definition_output();
				put(outputs, output);
			}
			while (cursor_.match(TokenKind::comma));
		}

		return outputs;
	}

	AstFunctionOutput* parse_function_definition_output() {
		SourceSize offset = cursor_.peek_offset();
		AstFunctionOutput* output = create<AstFunctionOutput>(offset);

		output->type = parse_type(Message::expect_return_type);
		// TODO: named return types
		return output;
	}

	const AstExpression* parse_type(Message error_message) {
		if (cursor_.peek_kind() == TokenKind::identifier) {
			return on_identifier();
		}

		report_expected(error_message);
		throw ParseError();
	}

	NodeList<AstNode> parse_block() {
		NodeList<AstNode> statements;

		while (!cursor_.match(TokenKind::r_brace)) {
			try {
				const AstExpression* statement = parse_statement();
				put(statements, statement);
			}
			catch (ParseError) {
				bool at_end = recover_at_statement_level();
				if (at_end) {
					break;
				}
			}
		}

		return statements;
	}

	bool recover_at_statement_level() {
		TokenKind kind = cursor_.peek_kind();

		while (kind != TokenKind::end_of_file) {
			if (kind == TokenKind::semicolon) {
				cursor_.advance();
				return false;
			}

			if (kind == TokenKind::r_brace) {
				return false;
			}

			cursor_.advance();
			kind = cursor_.peek_kind();
		}

		return true;
	}

	const AstExpression* parse_statement() {
		StatementRule rule = lookup_statement_rule();

		const AstExpression* last_expr = (this->*rule.parse_fn)();

		if (rule.needs_semicolon) {
			expect(TokenKind::semicolon, Message::expect_semicolon);
		}

		return last_expr;
	}

	/// Parse method for grammar rules determined by the kind of their first token, like prefix operators
	using HeadParseFunction = const AstExpression* (Parser::*) ();

	/// Parse method for grammar rules determined by the kind of token appearing after another expression, like infix operators
	using TailParseFunction = const AstExpression* (Parser::*) (const AstExpression* head);

	struct TailParseRule {
		Precedence precedence;
		TailParseFunction fn;
	};

	struct StatementRule {
		HeadParseFunction parse_fn;
		bool needs_semicolon;
	};

	StatementRule lookup_statement_rule() const {
		TokenKind next = cursor_.peek_kind();

		using enum TokenKind;
		// TODO
		switch (next) {
		case l_brace: return {&Parser::on_prefix_left_brace, false};
		default:      return {&Parser::parse_expression_statement, true};
		}
	}

	const AstExpression* parse_expression_statement() {
		return parse_expression(Precedence::statement);
	}

	const AstExpression* parse_expression(Precedence precedence = Precedence::statement) {
		Token next = cursor_.peek();

		HeadParseFunction head_fn = lookup_head_parse_fn(next.kind);
		if (head_fn == nullptr) {
			report_expected(Message::expect_expression);
			throw ParseError();
		}

		const AstExpression* expr = (this->*head_fn)();
		while (TailParseFunction tail_fn = get_next_tail_parse_fn(precedence)) {
			expr = (this->*tail_fn)(expr);
		}

		return expr;
	}

	static HeadParseFunction lookup_head_parse_fn(TokenKind kind) {
		using enum TokenKind;

		switch (kind) {
		case identifier:      return &Parser::on_identifier;
		case dec_int_literal: return &Parser::on_literal<LiteralKind::dec_int>;
		case hex_int_literal: return &Parser::on_literal<LiteralKind::hex_int>;
		case bin_int_literal: return &Parser::on_literal<LiteralKind::bin_int>;
		case oct_int_literal: return &Parser::on_literal<LiteralKind::oct_int>;
		case float_literal:   return &Parser::on_literal<LiteralKind::float_>;
		case char_literal:    return &Parser::on_literal<LiteralKind::char_>;
		case string_literal:  return &Parser::on_literal<LiteralKind::string>;
		case l_brace:         return &Parser::on_prefix_left_brace;
		case kw_return:       return &Parser::on_kw_return;
		case ampersand:       return &Parser::on_prefix_operator<UnaryOperator::addr_of>;
		case minus:           return &Parser::on_prefix_operator<UnaryOperator::negate>;
		case tilde:           return &Parser::on_prefix_operator<UnaryOperator::bit_not>;
		case bang:            return &Parser::on_prefix_operator<UnaryOperator::logic_not>;
		case plus_plus:       return &Parser::on_prefix_operator<UnaryOperator::pre_increment>;
		case minus_minus:     return &Parser::on_prefix_operator<UnaryOperator::pre_decrement>;
		default:              return nullptr;
		}
	}

	TailParseFunction get_next_tail_parse_fn(Precedence current_precedence) const {
		Token token = cursor_.peek();
		TailParseRule rule = lookup_tail_parse_rule(token);

		if (current_precedence < rule.precedence) {
			return rule.fn;
		}
		else {
			return nullptr;
		}
	}

	TailParseRule lookup_tail_parse_rule(Token token) const {
		using enum TokenKind;
		using enum Precedence;

		switch (token.kind) {
		default:               return {statement, nullptr};
		case eq:               return {assignment, &Parser::on_binary_operator<BinaryOperator::assign>};
		case plus_eq:          return {assignment, &Parser::on_binary_operator<BinaryOperator::add_assign>};
		case minus_eq:         return {assignment, &Parser::on_binary_operator<BinaryOperator::sub_assign>};
		case star_eq:          return {assignment, &Parser::on_binary_operator<BinaryOperator::mul_assign>};
		case slash_eq:         return {assignment, &Parser::on_binary_operator<BinaryOperator::div_assign>};
		case percent_eq:       return {assignment, &Parser::on_binary_operator<BinaryOperator::rem_assign>};
		case ampersand_eq:     return {assignment, &Parser::on_binary_operator<BinaryOperator::bit_and_assign>};
		case pipe_eq:          return {assignment, &Parser::on_binary_operator<BinaryOperator::bit_or_assign>};
		case tilde_eq:         return {assignment, &Parser::on_binary_operator<BinaryOperator::xor_assign>};
		case l_angle_angle_eq: return {assignment, &Parser::on_binary_operator<BinaryOperator::shl_assign>};
		case r_angle_angle_eq: return {assignment, &Parser::on_binary_operator<BinaryOperator::shr_assign>};
		case and_and_eq:       return {assignment, &Parser::on_binary_operator<BinaryOperator::logic_and_assign>};
		case pipe_pipe_eq:     return {assignment, &Parser::on_binary_operator<BinaryOperator::logic_or_assign>};
		case and_and:          return {logical, &Parser::on_binary_operator<BinaryOperator::logic_and>};
		case pipe_pipe:        return {logical, &Parser::on_binary_operator<BinaryOperator::logic_or>};
		case eq_eq:            return {comparison, &Parser::on_binary_operator<BinaryOperator::eq>};
		case bang_eq:          return {comparison, &Parser::on_binary_operator<BinaryOperator::neq>};
		case l_angle:          return {comparison, &Parser::on_binary_operator<BinaryOperator::less>};
		case l_angle_eq:       return {comparison, &Parser::on_binary_operator<BinaryOperator::less_eq>};
		case r_angle_eq:       return {comparison, &Parser::on_binary_operator<BinaryOperator::greater_eq>};
		case plus:             return {additive_or_bit, &Parser::on_binary_operator<BinaryOperator::add>};
		case minus:            return {additive_or_bit, &Parser::on_binary_operator<BinaryOperator::sub>};
		case ampersand:        return {additive_or_bit, &Parser::on_binary_operator<BinaryOperator::bit_and>};
		case pipe:             return {additive_or_bit, &Parser::on_binary_operator<BinaryOperator::bit_or>};
		case tilde:            return {additive_or_bit, &Parser::on_binary_operator<BinaryOperator::bit_xor>};
		case l_angle_angle:    return {additive_or_bit, &Parser::on_binary_operator<BinaryOperator::shl>};
		case star:             return {multiplicative, &Parser::on_binary_operator<BinaryOperator::mul>};
		case slash:            return {multiplicative, &Parser::on_binary_operator<BinaryOperator::div>};
		case percent:          return {multiplicative, &Parser::on_binary_operator<BinaryOperator::rem>};
		case caret:            return {postfix, &Parser::on_postfix_operator<UnaryOperator::deref>};
		case plus_plus:        return {postfix, &Parser::on_postfix_operator<UnaryOperator::post_increment>};
		case minus_minus:      return {postfix, &Parser::on_postfix_operator<UnaryOperator::post_decrement>};
		case l_paren:          return {postfix, &Parser::on_infix_left_paren};
		case r_angle:
			// check for a `>` directly afterward, since we parse `>>` as two tokens to close angle brackets more easily
			Token next = cursor_.peek_ahead();
			if (next.kind == r_angle && next.offset == token.offset + 1) {
				return {additive_or_bit, &Parser::on_binary_operator<BinaryOperator::shr>};
			}
			else {
				return {comparison, &Parser::on_binary_operator<BinaryOperator::greater>};
			}
		}
	}

	const AstExpression* on_identifier() {
		Token token = cursor_.next();
		AstIdentExpr* expr = create<AstIdentExpr>(token.offset);

		expr->length = token.variable_length;
		return expr;
	}

	template<LiteralKind literal_kind>
	const AstExpression* on_literal() {
		Token token = cursor_.next();

		AstLiteralExpr* expr = create<AstLiteralExpr>(token.offset);
		expr->literal_kind = literal_kind;

		expr->length = token.variable_length;
		return expr;
	}

	const AstExpression* on_prefix_left_brace() {
		Token token = cursor_.next();

		AstBlockExpr* expr = create<AstBlockExpr>(token.offset);
		expr->statements = parse_block();

		return expr;
	}

	const AstExpression* on_kw_return() {
		Token token = cursor_.next();

		AstReturnExpr* expr = create<AstReturnExpr>(token.offset);

		if (is_expression_next()) {
			do {
				const AstExpression* ret_value = parse_expression();
				put(expr->ret_values, ret_value);
			}
			while (cursor_.match(TokenKind::comma));
		}

		return expr;
	}

	bool is_expression_next() const {
		TokenKind kind = cursor_.peek_kind();
		return lookup_head_parse_fn(kind) != nullptr;
	}

	template<UnaryOperator op>
	const AstExpression* on_prefix_operator() {
		Token token = cursor_.next();

		AstUnaryExpr* expr = create<AstUnaryExpr>(token.offset);
		expr->op = op;
		expr->operand = parse_expression(Precedence::prefix);

		return expr;
	}

	template<BinaryOperator op>
	const AstExpression* on_binary_operator(const AstExpression* left) {
		cursor_.advance();

		if constexpr (op == BinaryOperator::shr) {
			// consume the second right angle bracket since `>>` is lexed as two tokens
			cursor_.advance();
		}

		AstBinaryExpr* expr = create<AstBinaryExpr>(left->token_offset);
		expr->op = op;
		expr->left = left;

		static constexpr Precedence precedence = lookup_precedence_for_associativity(op);
		expr->right = parse_expression(precedence);

		return expr;
	}

	template<UnaryOperator op>
	const AstExpression* on_postfix_operator(const AstExpression* left) {
		cursor_.advance();

		AstUnaryExpr* expr = create<AstUnaryExpr>(left->token_offset);
		expr->op = op;
		expr->operand = left;

		return expr;
	}

	const AstExpression* on_infix_left_paren(const AstExpression* left) {
		cursor_.advance();

		AstCallExpr* expr = create<AstCallExpr>(left->token_offset);
		expr->callee = left;

		if (!cursor_.match(TokenKind::r_paren)) {
			do {
				const AstExpression* argument = parse_expression();
				put(expr->args, argument);
			}
			while (cursor_.match(TokenKind::comma));

			expect(TokenKind::r_paren, Message::expect_closing_paren_in_call);
		}

		return expr;
	}

	// Reporting utilities

	AstName expect_identifier(Message message) {
		if (std::optional token = cursor_.match_token(TokenKind::identifier)) {
			return AstName::from(*token);
		}
		else {
			report_expected(message);
			return {};
		}
	}

	void expect(TokenKind kind, Message message) {
		if (!cursor_.match(kind)) {
			report_expected(message);
			throw ParseError();
		}
	}

	void report_expected(Message message) const {
		Token token = cursor_.peek();
		CodeLocation location = token.locate_in(source_, tab_size_);

		std::string_view format = get_token_message_format(token.kind);
		std::string_view lexeme = token.get_text(source_);
		std::string message_str = fmt::vformat(format, fmt::make_format_args(lexeme));

		report(message, location, MessageArgs(message_str));
	}

	void report(Message message, const CodeLocation& location, const MessageArgs& args) const {
		reporter_.report(location, message, args);
	}

	// AST utilities

	/// Creates a node of type T in storage owned by the AST.
	/// @param  offset  Offset of the token where the given AST node begins.
	template<std::derived_from<AstNode> T>
	T* create(SourceSize offset) {
		return ast_.arena_.create<T>(offset);
	}

	template<typename T>
	void put(NodeList<T>& list, const std::type_identity_t<T>* node) {
		list.put(node, ast_.arena_);
	}
};

Ast run_parser(const TokenList& token_list, const SourceView& source, Reporter& reporter, uint8_t tab_size) {
	Parser parser(token_list, source, reporter, tab_size);
	parser.run();
	return std::move(parser.ast_);
}

} // namespace cero

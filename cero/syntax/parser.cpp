#include "parser.hpp"

#include "cero/syntax/token_cursor.hpp"

namespace cero {

struct ParseError {};

struct Parser {
	Ast ast_;
	TokenCursor cursor_;
	const SourceView& source_;
	Reporter& reporter_;
	const uint8_t tab_size_;

	Parser(const TokenList& token_list, const SourceView& source, Reporter& reporter, uint8_t tab_size) :
	    ast_(token_list),
	    cursor_(token_list),
	    source_(source),
	    reporter_(reporter),
	    tab_size_(tab_size) {
	}

	Ast run() && {
		parse_root();
		return std::move(ast_);
	}

	void parse_root() {
		ast_.root_ = ast_.create<AstRoot>(cursor_.peek_offset());

		while (!cursor_.match(TokenKind::end_of_file)) {
			try {
				AstDefinition* definition = parse_definition();
				ast_.root_->definitions.add(definition, ast_.arena_);
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
		AstFunctionDefinition* func_def = ast_.create<AstFunctionDefinition>(offset);
		func_def->access_modifier = access_modifier;
		func_def->name = AstName::from(identifier);

		expect(TokenKind::l_paren, Message::expect_paren_after_function_name);
		func_def->parameters = parse_function_definition_parameters();
		func_def->outputs = parse_function_definition_outputs();

		expect(TokenKind::l_brace, Message::expect_brace_before_function_body);
		func_def->statements = parse_block();

		return func_def;
	}

	util::ArenaArray<AstFunctionParameter*> parse_function_definition_parameters() {
		util::ArenaArray<AstFunctionParameter*> parameters;

		if (!cursor_.match(TokenKind::r_paren)) {
			do {
				AstFunctionParameter* param = parse_function_definition_parameter();
				parameters.add(param, ast_.arena_);
			}
			while (cursor_.match(TokenKind::comma));

			expect(TokenKind::r_paren, Message::expect_paren_after_parameters);
		}

		return parameters;
	}

	AstFunctionParameter* parse_function_definition_parameter() {
		SourceSize offset = cursor_.peek_offset();
		AstFunctionParameter* param = ast_.create<AstFunctionParameter>(offset);

		param->type = expect_identifier(Message::expect_parameter_type);

		// abort parsing and recover at the next definition so errors don't accumulate from a malformed function signature
		if (param->type.is_empty()) {
			throw ParseError();
		}

		param->name = expect_identifier(Message::expect_parameter_name);
		if (param->name.is_empty()) {
			throw ParseError();
		}

		return param;
	}

	util::ArenaArray<AstFunctionOutput*> parse_function_definition_outputs() {
		util::ArenaArray<AstFunctionOutput*> outputs;

		if (cursor_.match(TokenKind::thin_arrow)) {
			do {
				AstFunctionOutput* output = parse_function_definition_output();
				outputs.add(output, ast_.arena_);
			}
			while (cursor_.match(TokenKind::comma));
		}

		return outputs;
	}

	AstFunctionOutput* parse_function_definition_output() {
		SourceSize offset = cursor_.peek_offset();
		AstFunctionOutput* output = ast_.create<AstFunctionOutput>(offset);

		output->type = expect_identifier(Message::expect_return_type);
		return output;
	}

	util::ArenaArray<AstExpression*> parse_block() {
		util::ArenaArray<AstExpression*> statements;

		while (!cursor_.match(TokenKind::r_brace)) {
			// TODO
			cursor_.advance();
		}

		return statements;
	}

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

	void report(Message message, CodeLocation location, MessageArgs args) const {
		reporter_.report(location, message, std::move(args));
	}
};

Ast run_parser(const TokenList& token_list, const SourceView& source, Reporter& reporter, uint8_t tab_size) {
	return Parser(token_list, source, reporter, tab_size).run();
}

} // namespace cero

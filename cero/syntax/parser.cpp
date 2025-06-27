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
		ast_.root_ = parse_root();
		return std::move(ast_);
	}

	AstRoot* parse_root() {
		AstRoot* root = ast_.create<AstRoot>(0);

		while (!cursor_.match(TokenKind::end_of_file)) {
			try {
				AstDefinition* definition = parse_definition();
				ast_.add_child(root->definitions, definition);
			} catch (ParseError) {
				recover_at_definition_scope();
			}
		}

		return root;
	}

	void recover_at_definition_scope() {
		static constexpr TokenKind recovery_tokens[] {TokenKind::kw_public, TokenKind::kw_private, TokenKind::kw_struct,
		                                              TokenKind::kw_enum, TokenKind::end_of_file};

		TokenKind kind;
		do {
			cursor_.advance();
			kind = cursor_.peek_kind();
		} while (!std::ranges::contains(recovery_tokens, kind));
	}

	AstDefinition* parse_definition() {
		SourceSize offset = cursor_.peek_offset();

		AccessModifier access_modifier;
		if (cursor_.match(TokenKind::kw_private)) {
			access_modifier = AccessModifier::private_;
		} else if (cursor_.match(TokenKind::kw_public)) {
			access_modifier = AccessModifier::public_;
		} else {
			access_modifier = AccessModifier::none;
		}

		if (std::optional token = cursor_.match_token(TokenKind::identifier)) {
			return parse_function_definition(offset, access_modifier, *token);
		} else {
			report_expected(Message::expect_function_struct_or_enum);
			throw ParseError();
		}
	}

	AstFunctionDefinition* parse_function_definition(SourceSize offset, AccessModifier access_modifier, Token identifier) {
		AstFunctionDefinition* function_def = ast_.create<AstFunctionDefinition>(offset);
		function_def->access_modifier = access_modifier;
		function_def->name = AstName::from(identifier);

		expect(TokenKind::l_paren, Message::expect_paren_after_function_name);
		function_def->parameters = parse_function_parameters();
		function_def->outputs = parse_function_outputs();

		return function_def;
	}

	ArenaArray<AstFunctionParameter*> parse_function_parameters() {
		ArenaArray<AstFunctionParameter*> parameters;
		return parameters;
	}

	ArenaArray<AstFunctionOutput*> parse_function_outputs() {
		ArenaArray<AstFunctionOutput*> outputs;
		return outputs;
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

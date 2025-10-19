#include "parser.hh"

#include "cero/syntax/token_cursor.hh"

namespace cero {

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

		// TODO
		cursor_.advance();
		cursor_.advance();

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

		// TODO
		cursor_.advance();

		return output;
	}

	NodeList<AstNode> parse_block() {
		NodeList<AstNode> statements;

		while (!cursor_.match(TokenKind::r_brace)) {
			// TODO
			cursor_.advance();
		}

		return statements;
	}

	// Reporting utilities

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

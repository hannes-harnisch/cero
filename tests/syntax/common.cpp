#include "common.hpp"

#include "common/test.hpp"
#include "common/test_reporter.hpp"

#include <cero/syntax/lexer.hpp>
#include <cero/syntax/parser.hpp>

namespace tests {

void check_tokens_equal(const char* source_text, std::span<const cero::Token> expected_tokens) {
	cero::SourceView source = make_test_source(source_text);

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

	// checking for token equality first
	std::span<const cero::Token> received_tokens = tokens.get_array();
	size_t count = std::min(expected_tokens.size(), received_tokens.size());
	for (size_t i = 0; i < count; ++i) {
		cero::Token expected = expected_tokens[i];
		cero::Token received = received_tokens[i];
		CHECK_EQ(received, expected);
	}

	// checking for extra or missing tokens
	REQUIRE_EQ(received_tokens.size(), expected_tokens.size());
}

cero::Ast parse_source(const char* source_text) {
	cero::SourceView source = make_test_source(source_text);

	TestReporter r;
	cero::TokenList token_list = cero::run_lexer(source, r, cero::LexerFlags::none);
	cero::Ast ast = cero::run_parser(token_list, source, r);

	return ast;
}

} // namespace tests

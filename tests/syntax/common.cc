#include "common.hh"

#include "common/test.hh"
#include "common/test_reporter.hh"

#include <cero/syntax/lexer.hh>

namespace tests {

void check_tokens_equal(const char* source_text, std::span<const cero::Token> expected_tokens) {
	cero::SourceView source = make_test_source(source_text);

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

	std::span<const cero::Token> token_array = tokens.get_array();
	REQUIRE_EQ(token_array.size(), expected_tokens.size());

	size_t i = 0;
	for (cero::Token expected : expected_tokens) {
		cero::Token token = token_array[i++];
		CHECK_EQ(token, expected);
	}
}

} // namespace tests

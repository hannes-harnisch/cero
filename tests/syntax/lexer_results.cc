#include "common/test.hh"
#include "common/test_reporter.hh"

#include <cero/syntax/lexer.hh>

namespace tests {

namespace {

void expect_tokens(const cero::TokenList& tokens, std::span<const cero::Token> expected) {
	CHECK_EQ(tokens.get_array().size(), expected.size());
	CHECK(std::ranges::equal(tokens.get_array(), expected));
}

} // namespace

CERO_TEST(lexer_empty_source) {
	cero::SourceView source = make_test_source("");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, true);

	cero::Token expected[] = {
	    {cero::TokenKind::end_of_file, 0, 0},
	};
	expect_tokens(tokens, expected);
}

} // namespace tests

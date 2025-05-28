#include "common/test.hh"
#include "common/test_reporter.hh"
#include "syntax/common.hh"

#include <cero/syntax/lexer.hh>

namespace tests {

using enum cero::TokenKind;

CERO_TEST(lexer_empty_source) {
	cero::SourceView source = make_test_source("");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

	cero::Token expected[] = {
	    {end_of_file, 0, 0},
	};
	check_tokens_equal(tokens, expected);
}

} // namespace tests

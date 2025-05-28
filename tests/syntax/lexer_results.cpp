#include "common/test.hpp"
#include "common/test_reporter.hpp"
#include "syntax/common.hpp"

#include <cero/syntax/lexer.hpp>

using enum cero::TokenKind;

namespace tests {

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

#include "common/test.hh"
#include "syntax/common.hh"

namespace tests {

using enum cero::TokenKind;

CERO_TEST(lexer_empty_source) {
	const char* source = "";

	cero::Token expected[] = {
	    {end_of_file, 0, 0},
	};
	check_tokens_equal(source, expected);
}

} // namespace tests

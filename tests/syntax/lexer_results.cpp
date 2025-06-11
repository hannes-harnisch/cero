#include "common/test.hpp"
#include "syntax/common.hpp"

using enum cero::TokenKind;

namespace tests {

CERO_TEST(lexer_empty_source) {
	const char* source = "";

	cero::Token expected[] = {
		{end_of_file, 0, 0},
	};
	check_tokens_equal(source, expected);
}

} // namespace tests

#include "common.hh"

#include "common/doctest.hh"

namespace tests {

void check_tokens_equal(const cero::TokenList& tokens, std::span<const cero::Token> expected_tokens) {
	auto token_array = tokens.get_array();
	REQUIRE_EQ(token_array.size(), expected_tokens.size());

	size_t i = 0;
	for (cero::Token expected : expected_tokens) {
		cero::Token token = token_array[i++];
		CHECK_EQ(token, expected);
	}
}

} // namespace tests

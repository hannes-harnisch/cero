#include "common/test.hh"
#include "syntax/common.hh"

namespace tests {

using enum cero::TokenKind;

CERO_TEST(lexer_ascii_identifiers) {
	const char* source = "a "
	                     "_ "
	                     "_a "
	                     "z_ "
	                     "abc "
	                     "abc09 "
	                     "a0b0c0 "
	                     "A1B2C3 "
	                     "_0 "
	                     "_111 "
	                     "_1ac "
	                     "something_something_123 "
	                     "___________________________________";

	cero::Token expected[] = {
	    {identifier,  0,   1 },
	    {identifier,  2,   1 },
	    {identifier,  4,   2 },
	    {identifier,  7,   2 },
	    {identifier,  10,  3 },
	    {identifier,  14,  5 },
	    {identifier,  20,  6 },
	    {identifier,  27,  6 },
	    {identifier,  34,  2 },
	    {identifier,  37,  4 },
	    {identifier,  42,  4 },
	    {identifier,  47,  23},
	    {identifier,  71,  35},
	    {end_of_file, 106, 0 },
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_unicode_identifiers) {
	const char* source = reinterpret_cast<const char*>(u8"Москва()"
	                                                   "東京()"
	                                                   "München()"
	                                                   "Vrå()"
	                                                   "Østerbro()"
	                                                   "Δ()");

	cero::Token expected[] = {
	    {identifier,  0,  12},
	    {l_paren,     12, 0 },
	    {r_paren,     13, 0 },
	    {identifier,  14, 6 },
	    {l_paren,     20, 0 },
	    {r_paren,     21, 0 },
	    {identifier,  22, 8 },
	    {l_paren,     30, 0 },
	    {r_paren,     31, 0 },
	    {identifier,  32, 4 },
	    {l_paren,     36, 0 },
	    {r_paren,     37, 0 },
	    {identifier,  38, 9 },
	    {l_paren,     47, 0 },
	    {r_paren,     48, 0 },
	    {identifier,  49, 2 },
	    {l_paren,     51, 0 },
	    {r_paren,     52, 0 },
	    {end_of_file, 53, 0 },
	};
	check_tokens_equal(source, expected);
}

} // namespace tests

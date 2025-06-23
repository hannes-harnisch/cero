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

CERO_TEST(lexer_pi_const) {
	const char* source = reinterpret_cast<const char*>(u8R"_____(
const π = 3.1415926535 8979323846 2643383279 5028841971 6939937510 5820974944 5923078164 0628620899 8628034825 3421170679
            8214808651 3282306647 0938446095 5058223172 5359408128 4811174502 8410270193 8521105559 6446229489 5493038196
            4428810975 6659334461 2847564823 3786783165 2712019091 4564856692 3460348610 4543266482;
)_____");

	cero::Token expected[] = {
	    {kw_const,      1,   0  },
	    {identifier,    7,   2  },
	    {eq,            10,  0  },
	    {float_literal, 12,  333},
	    {semicolon,     345, 0  },
	    {end_of_file,   347, 0  },
	};
	check_tokens_equal(source, expected);
}

} // namespace tests

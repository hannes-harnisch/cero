#include "common/test.hh"
#include "syntax/common.hh"

namespace tests {

using enum cero::TokenKind;

CERO_TEST(lexer_dec_int_literals) {
	const char* source = "0;"
	                     "1;"
	                     "15;"
	                     "100;"
	                     "1234567890;"
	                     "0123456789;"
	                     "1 000 000 000;"
	                     "1 234 567 890;"
	                     "0 000 000 000;"
	                     "2 000 125 000 ;"
	                     "  123 456  ";

	cero::Token expected[] = {
	    {dec_int_literal, 0,   1 },
	    {semicolon,       1,   0 },
	    {dec_int_literal, 2,   1 },
	    {semicolon,       3,   0 },
	    {dec_int_literal, 4,   2 },
	    {semicolon,       6,   0 },
	    {dec_int_literal, 7,   3 },
	    {semicolon,       10,  0 },
	    {dec_int_literal, 11,  10},
	    {semicolon,       21,  0 },
	    {dec_int_literal, 22,  10},
	    {semicolon,       32,  0 },
	    {dec_int_literal, 33,  13},
	    {semicolon,       46,  0 },
	    {dec_int_literal, 47,  13},
	    {semicolon,       60,  0 },
	    {dec_int_literal, 61,  13},
	    {semicolon,       74,  0 },
	    {dec_int_literal, 75,  13},
	    {semicolon,       89,  0 },
	    {dec_int_literal, 92,  7 },
	    {end_of_file,     101, 0 },
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_hex_int_literals) {
	const char* source = "0x1234567890ABCDEF;"
	                     "0x1 A B 3 C;"
	                     "0x FA 34 B3 FB;"
	                     "0xAC5 FB2 67A 91E;"
	                     "0x;"
	                     "0xABCD  0197  2562  7810;"
	                     "0xABCDEFGHIJKLMNOPQRSTUVWXYZ;"
	                     "0x0;"
	                     "0x1i;"
	                     "0x02 ";

	cero::Token expected[] = {
	    {hex_int_literal, 0,   18},
	    {semicolon,       18,  0 },
	    {hex_int_literal, 19,  11},
	    {semicolon,       30,  0 },
	    {hex_int_literal, 31,  14},
	    {semicolon,       45,  0 },
	    {hex_int_literal, 46,  17},
	    {semicolon,       63,  0 },
	    {hex_int_literal, 64,  2 },
	    {semicolon,       66,  0 },
	    {hex_int_literal, 67,  24},
	    {semicolon,       91,  0 },
	    {hex_int_literal, 92,  8 },
	    {identifier,      100, 20},
	    {semicolon,       120, 0 },
	    {hex_int_literal, 121, 3 },
	    {semicolon,       124, 0 },
	    {hex_int_literal, 125, 3 },
	    {identifier,      128, 1 },
	    {semicolon,       129, 0 },
	    {hex_int_literal, 130, 4 },
	    {end_of_file,     135, 0 },
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_bin_int_literals) {
	const char* source = "0b1010 ; "
	                     "  0b1111 0000  ;"
	                     "0b1010 1100 0011;"
	                     "0b0;"
	                     "0b1 ; "
	                     "0b;"
	                     "0b2 ; "
	                     "0b01101102; "
	                     "0b1104 3100; "
	                     "0b1i;"
	                     "0b 001";

	cero::Token expected[] = {
	    {bin_int_literal, 0,  6 },
	    {semicolon,       7,  0 },
	    {bin_int_literal, 11, 11},
	    {semicolon,       24, 0 },
	    {bin_int_literal, 25, 16},
	    {semicolon,       41, 0 },
	    {bin_int_literal, 42, 3 },
	    {semicolon,       45, 0 },
	    {bin_int_literal, 46, 3 },
	    {semicolon,       50, 0 },
	    {bin_int_literal, 52, 2 },
	    {semicolon,       54, 0 },
	    {bin_int_literal, 55, 3 },
	    {semicolon,       59, 0 },
	    {bin_int_literal, 61, 10},
	    {semicolon,       71, 0 },
	    {bin_int_literal, 73, 11},
	    {semicolon,       84, 0 },
	    {bin_int_literal, 86, 3 },
	    {identifier,      89, 1 },
	    {semicolon,       90, 0 },
	    {bin_int_literal, 91, 6 },
	    {end_of_file,     97, 0 },
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_oct_int_literals) {
	const char* source = "0o1234567;"
	                     "  0o1 2 3 4  ;"
	                     "0o765 432 100;"
	                     "0o0;"
	                     "0o7 ; "
	                     "0o;"
	                     "0o8 ; "
	                     "0o01234568; "
	                     "0o 7i";

	cero::Token expected[] = {
	    {oct_int_literal, 0,  9 },
	    {semicolon,       9,  0 },
	    {oct_int_literal, 12, 9 },
	    {semicolon,       23, 0 },
	    {oct_int_literal, 24, 13},
	    {semicolon,       37, 0 },
	    {oct_int_literal, 38, 3 },
	    {semicolon,       41, 0 },
	    {oct_int_literal, 42, 3 },
	    {semicolon,       46, 0 },
	    {oct_int_literal, 48, 2 },
	    {semicolon,       50, 0 },
	    {oct_int_literal, 51, 3 },
	    {semicolon,       55, 0 },
	    {oct_int_literal, 57, 10},
	    {semicolon,       67, 0 },
	    {oct_int_literal, 69, 4 },
	    {identifier,      73, 1 },
	    {end_of_file,     74, 0 },
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_float_literals) {
	const char* source = "1.0;"
	                     "1.;"
	                     " .4 ;"
	                     " .045;"
	                     "100 000.000 231;"
	                     "  234 5 . 23 948  ;"
	                     "1..z;"
	                     "234.s;"
	                     "1.0.a";

	cero::Token expected[] = {
	    {float_literal,   0,  3 },
	    {semicolon,       3,  0 },
	    {dec_int_literal, 4,  1 },
	    {dot,             5,  0 },
	    {semicolon,       6,  0 },
	    {float_literal,   8,  2 },
	    {semicolon,       11, 0 },
	    {float_literal,   13, 4 },
	    {semicolon,       17, 0 },
	    {float_literal,   18, 15},
	    {semicolon,       33, 0 },
	    {float_literal,   36, 14},
	    {semicolon,       52, 0 },
	    {dec_int_literal, 53, 1 },
	    {dot,             54, 0 },
	    {dot,             55, 0 },
	    {identifier,      56, 1 },
	    {semicolon,       57, 0 },
	    {dec_int_literal, 58, 3 },
	    {dot,             61, 0 },
	    {identifier,      62, 1 },
	    {semicolon,       63, 0 },
	    {float_literal,   64, 3 },
	    {dot,             67, 0 },
	    {identifier,      68, 1 },
	    {end_of_file,     69, 0 },
	};
	check_tokens_equal(source, expected);
}

} // namespace tests

#include "common/test.hh"
#include "syntax/common.hh"

namespace tests {

using enum cero::TokenKind;

CERO_TEST(lexer_keyword_break) {
	const char* source = "b br bre brea break break_";

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {identifier,  9,  4},
	    {kw_break,    14, 0},
	    {identifier,  20, 6},
	    {end_of_file, 26, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_catch) {
	const char* source = "c ca cat catc catch catch_";

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {identifier,  9,  4},
	    {kw_catch,    14, 0},
	    {identifier,  20, 6},
	    {end_of_file, 26, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_const) {
	const char* source = "c co con cons const const_";

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {identifier,  9,  4},
	    {kw_const,    14, 0},
	    {identifier,  20, 6},
	    {end_of_file, 26, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_continue) {
	const char* source = "c co con cont conti contin continu continue continue_";

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {identifier,  9,  4},
	    {identifier,  14, 5},
	    {identifier,  20, 6},
	    {identifier,  27, 7},
	    {kw_continue, 35, 0},
	    {identifier,  44, 9},
	    {end_of_file, 53, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_do) {
	const char* source = "d do do_";

	cero::Token expected[] = {
	    {identifier,  0, 1},
	    {kw_do,       2, 0},
	    {identifier,  5, 3},
	    {end_of_file, 8, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_else) {
	const char* source = "e el els else else_";

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {kw_else,     9,  0},
	    {identifier,  14, 5},
	    {end_of_file, 19, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_enum) {
	const char* source = "e en enu enum enum_";

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {kw_enum,     9,  0},
	    {identifier,  14, 5},
	    {end_of_file, 19, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_for) {
	const char* source = "f fo for for_";

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {kw_for,      5,  0},
	    {identifier,  9,  4},
	    {end_of_file, 13, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_if) {
	const char* source = "i if if_";

	cero::Token expected[] = {
	    {identifier,  0, 1},
	    {kw_if,       2, 0},
	    {identifier,  5, 3},
	    {end_of_file, 8, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_in) {
	const char* source = "i in in_";

	cero::Token expected[] = {
	    {identifier,  0, 1},
	    {kw_in,       2, 0},
	    {identifier,  5, 3},
	    {end_of_file, 8, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_interface) {
	const char* source = "i in int inte inter interf interfa interfac interface interface_";

	cero::Token expected[] = {
	    {identifier,   0,  1 },
	    {kw_in,        2,  0 },
	    {identifier,   5,  3 },
	    {identifier,   9,  4 },
	    {identifier,   14, 5 },
	    {identifier,   20, 6 },
	    {identifier,   27, 7 },
	    {identifier,   35, 8 },
	    {kw_interface, 44, 0 },
	    {identifier,   54, 10},
	    {end_of_file,  64, 0 },
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_let) {
	const char* source = "l le let let_";

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {kw_let,      5,  0},
	    {identifier,  9,  4},
	    {end_of_file, 13, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_mut) {
	const char* source = "m mu mut mut_";

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {kw_mut,      5,  0},
	    {identifier,  9,  4},
	    {end_of_file, 13, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_private) {
	const char* source = "p pr pri priv priva privat private private_";

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {identifier,  9,  4},
	    {identifier,  14, 5},
	    {identifier,  20, 6},
	    {kw_private,  27, 0},
	    {identifier,  35, 8},
	    {end_of_file, 43, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_public) {
	const char* source = "p pu pub publ publi public public_";

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {identifier,  9,  4},
	    {identifier,  14, 5},
	    {kw_public,   20, 0},
	    {identifier,  27, 7},
	    {end_of_file, 34, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_raise) {
	const char* source = "r ra rai rais raise raise_";

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {identifier,  9,  4},
	    {kw_raise,    14, 0},
	    {identifier,  20, 6},
	    {end_of_file, 26, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_return) {
	const char* source = "r re ret retu retur return return_";

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {identifier,  9,  4},
	    {identifier,  14, 5},
	    {kw_return,   20, 0},
	    {identifier,  27, 7},
	    {end_of_file, 34, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_static) {
	const char* source = "s st sta stat stati static static_";

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {identifier,  9,  4},
	    {identifier,  14, 5},
	    {kw_static,   20, 0},
	    {identifier,  27, 7},
	    {end_of_file, 34, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_struct) {
	const char* source = "s st str stru struc struct struct_";

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {identifier,  9,  4},
	    {identifier,  14, 5},
	    {kw_struct,   20, 0},
	    {identifier,  27, 7},
	    {end_of_file, 34, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_switch) {
	const char* source = "s sw swi swit switc switch switch_";

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {identifier,  9,  4},
	    {identifier,  14, 5},
	    {kw_switch,   20, 0},
	    {identifier,  27, 7},
	    {end_of_file, 34, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_try) {
	const char* source = "t tr try try_";

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {kw_try,      5,  0},
	    {identifier,  9,  4},
	    {end_of_file, 13, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_unchecked) {
	const char* source = "u un unc unch unche unchec uncheck unchecke unchecked unchecked_";

	cero::Token expected[] = {
	    {identifier,   0,  1 },
	    {identifier,   2,  2 },
	    {identifier,   5,  3 },
	    {identifier,   9,  4 },
	    {identifier,   14, 5 },
	    {identifier,   20, 6 },
	    {identifier,   27, 7 },
	    {identifier,   35, 8 },
	    {kw_unchecked, 44, 0 },
	    {identifier,   54, 10},
	    {end_of_file,  64, 0 },
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_keyword_while) {
	const char* source = "w wh whi whil while while_";

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {identifier,  9,  4},
	    {kw_while,    14, 0},
	    {identifier,  20, 6},
	    {end_of_file, 26, 0},
	};
	check_tokens_equal(source, expected);
}

} // namespace tests

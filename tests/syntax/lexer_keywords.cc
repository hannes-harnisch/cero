#include "common/test.hh"
#include "common/test_reporter.hh"
#include "syntax/common.hh"

#include <cero/syntax/lexer.hh>

namespace tests {

using enum cero::TokenKind;

CERO_TEST(lexer_keyword_break) {
	cero::SourceView source = make_test_source("b br bre brea break break_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {identifier,  9,  4},
	    {kw_break,    14, 0},
	    {identifier,  20, 6},
	    {end_of_file, 26, 0},
	};
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_catch) {
	cero::SourceView source = make_test_source("c ca cat catc catch catch_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {identifier,  9,  4},
	    {kw_catch,    14, 0},
	    {identifier,  20, 6},
	    {end_of_file, 26, 0},
	};
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_const) {
	cero::SourceView source = make_test_source("c co con cons const const_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {identifier,  9,  4},
	    {kw_const,    14, 0},
	    {identifier,  20, 6},
	    {end_of_file, 26, 0},
	};
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_continue) {
	cero::SourceView source = make_test_source("c co con cont conti contin continu continue continue_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

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
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_do) {
	cero::SourceView source = make_test_source("d do do_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

	cero::Token expected[] = {
	    {identifier,  0, 1},
	    {kw_do,       2, 0},
	    {identifier,  5, 3},
	    {end_of_file, 8, 0},
	};
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_else) {
	cero::SourceView source = make_test_source("e el els else else_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {kw_else,     9,  0},
	    {identifier,  14, 5},
	    {end_of_file, 19, 0},
	};
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_enum) {
	cero::SourceView source = make_test_source("e en enu enum enum_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {kw_enum,     9,  0},
	    {identifier,  14, 5},
	    {end_of_file, 19, 0},
	};
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_for) {
	cero::SourceView source = make_test_source("f fo for for_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {kw_for,      5,  0},
	    {identifier,  9,  4},
	    {end_of_file, 13, 0},
	};
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_if) {
	cero::SourceView source = make_test_source("i if if_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

	cero::Token expected[] = {
	    {identifier,  0, 1},
	    {kw_if,       2, 0},
	    {identifier,  5, 3},
	    {end_of_file, 8, 0},
	};
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_in) {
	cero::SourceView source = make_test_source("i in in_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

	cero::Token expected[] = {
	    {identifier,  0, 1},
	    {kw_in,       2, 0},
	    {identifier,  5, 3},
	    {end_of_file, 8, 0},
	};
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_interface) {
	cero::SourceView source = make_test_source("i in int inte inter interf interfa interfac interface interface_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

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
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_let) {
	cero::SourceView source = make_test_source("l le let let_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {kw_let,      5,  0},
	    {identifier,  9,  4},
	    {end_of_file, 13, 0},
	};
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_mut) {
	cero::SourceView source = make_test_source("m mu mut mut_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {kw_mut,      5,  0},
	    {identifier,  9,  4},
	    {end_of_file, 13, 0},
	};
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_private) {
	cero::SourceView source = make_test_source("p pr pri priv priva privat private private_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

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
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_public) {
	cero::SourceView source = make_test_source("p pu pub publ publi public public_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

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
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_raise) {
	cero::SourceView source = make_test_source("r ra rai rais raise raise_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {identifier,  9,  4},
	    {kw_raise,    14, 0},
	    {identifier,  20, 6},
	    {end_of_file, 26, 0},
	};
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_return) {
	cero::SourceView source = make_test_source("r re ret retu retur return return_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

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
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_static) {
	cero::SourceView source = make_test_source("s st sta stat stati static static_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

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
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_struct) {
	cero::SourceView source = make_test_source("s st str stru struc struct struct_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

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
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_switch) {
	cero::SourceView source = make_test_source("s sw swi swit switc switch switch_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

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
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_try) {
	cero::SourceView source = make_test_source("t tr try try_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {kw_try,      5,  0},
	    {identifier,  9,  4},
	    {end_of_file, 13, 0},
	};
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_unchecked) {
	cero::SourceView source = make_test_source("u un unc unch unche unchec uncheck unchecke unchecked unchecked_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

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
	check_tokens_equal(tokens, expected);
}

CERO_TEST(lexer_keyword_while) {
	cero::SourceView source = make_test_source("w wh whi whil while while_");

	TestReporter r;
	cero::TokenList tokens = cero::run_lexer(source, r, cero::LexerFlags::emit_comments);

	cero::Token expected[] = {
	    {identifier,  0,  1},
	    {identifier,  2,  2},
	    {identifier,  5,  3},
	    {identifier,  9,  4},
	    {kw_while,    14, 0},
	    {identifier,  20, 6},
	    {end_of_file, 26, 0},
	};
	check_tokens_equal(tokens, expected);
}

} // namespace tests

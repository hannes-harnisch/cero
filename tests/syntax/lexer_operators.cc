#include "common/test.hh"
#include "syntax/common.hh"

namespace tests {

using enum cero::TokenKind;

CERO_TEST(lexer_brackets) {
	const char* source = "()[]{}(())[[]]{{}}";

	cero::Token expected[] = {
	    {l_paren,     0,  0},
	    {r_paren,     1,  0},
	    {l_bracket,   2,  0},
	    {r_bracket,   3,  0},
	    {l_brace,     4,  0},
	    {r_brace,     5,  0},
	    {l_paren,     6,  0},
	    {l_paren,     7,  0},
	    {r_paren,     8,  0},
	    {r_paren,     9,  0},
	    {l_bracket,   10, 0},
	    {l_bracket,   11, 0},
	    {r_bracket,   12, 0},
	    {r_bracket,   13, 0},
	    {l_brace,     14, 0},
	    {l_brace,     15, 0},
	    {r_brace,     16, 0},
	    {r_brace,     17, 0},
	    {end_of_file, 18, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_comma) {
	const char* source = ", ,, ,,, ,a, b,b ,c,c d,d,d";

	cero::Token expected[] = {
	    {comma,       0,  0},
	    {comma,       2,  0},
	    {comma,       3,  0},
	    {comma,       5,  0},
	    {comma,       6,  0},
	    {comma,       7,  0},
	    {comma,       9,  0},
	    {identifier,  10, 1},
	    {comma,       11, 0},
	    {identifier,  13, 1},
	    {comma,       14, 0},
	    {identifier,  15, 1},
	    {comma,       17, 0},
	    {identifier,  18, 1},
	    {comma,       19, 0},
	    {identifier,  20, 1},
	    {identifier,  22, 1},
	    {comma,       23, 0},
	    {identifier,  24, 1},
	    {comma,       25, 0},
	    {identifier,  26, 1},
	    {end_of_file, 27, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_semicolon) {
	const char* source = "; ;; ;;; ;a; b;b ;c;c d;d;d";

	cero::Token expected[] = {
	    {semicolon,   0,  0},
	    {semicolon,   2,  0},
	    {semicolon,   3,  0},
	    {semicolon,   5,  0},
	    {semicolon,   6,  0},
	    {semicolon,   7,  0},
	    {semicolon,   9,  0},
	    {identifier,  10, 1},
	    {semicolon,   11, 0},
	    {identifier,  13, 1},
	    {semicolon,   14, 0},
	    {identifier,  15, 1},
	    {semicolon,   17, 0},
	    {identifier,  18, 1},
	    {semicolon,   19, 0},
	    {identifier,  20, 1},
	    {identifier,  22, 1},
	    {semicolon,   23, 0},
	    {identifier,  24, 1},
	    {semicolon,   25, 0},
	    {identifier,  26, 1},
	    {end_of_file, 27, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_caret) {
	const char* source = "^ ^^ ^^^ ^a^ b^b ^c^c d^d^d";

	cero::Token expected[] = {
	    {caret,       0,  0},
	    {caret,       2,  0},
	    {caret,       3,  0},
	    {caret,       5,  0},
	    {caret,       6,  0},
	    {caret,       7,  0},
	    {caret,       9,  0},
	    {identifier,  10, 1},
	    {caret,       11, 0},
	    {identifier,  13, 1},
	    {caret,       14, 0},
	    {identifier,  15, 1},
	    {caret,       17, 0},
	    {identifier,  18, 1},
	    {caret,       19, 0},
	    {identifier,  20, 1},
	    {identifier,  22, 1},
	    {caret,       23, 0},
	    {identifier,  24, 1},
	    {caret,       25, 0},
	    {identifier,  26, 1},
	    {end_of_file, 27, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_left_angle) {
	const char* source = "< << <<=a<<b<c<<<<=<<< <= <=<";

	cero::Token expected[] = {
	    {l_angle,          0,  0},
	    {l_angle_angle,    2,  0},
	    {l_angle_angle_eq, 5,  0},
	    {identifier,       8,  1},
	    {l_angle_angle,    9,  0},
	    {identifier,       11, 1},
	    {l_angle,          12, 0},
	    {identifier,       13, 1},
	    {l_angle_angle,    14, 0},
	    {l_angle_angle_eq, 16, 0},
	    {l_angle_angle,    19, 0},
	    {l_angle,          21, 0},
	    {l_angle_eq,       23, 0},
	    {l_angle_eq,       26, 0},
	    {l_angle,          28, 0},
	    {end_of_file,      29, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_right_angle) {
	const char* source = "> >> >>=a>>b>c>>>>=>>> >= >=>";

	cero::Token expected[] = {
	    {r_angle,          0,  0},
	    {r_angle,          2,  0},
	    {r_angle,          3,  0},
	    {r_angle_angle_eq, 5,  0},
	    {identifier,       8,  1},
	    {r_angle,          9,  0},
	    {r_angle,          10, 0},
	    {identifier,       11, 1},
	    {r_angle,          12, 0},
	    {identifier,       13, 1},
	    {r_angle,          14, 0},
	    {r_angle,          15, 0},
	    {r_angle_angle_eq, 16, 0},
	    {r_angle,          19, 0},
	    {r_angle,          20, 0},
	    {r_angle,          21, 0},
	    {r_angle_eq,       23, 0},
	    {r_angle_eq,       26, 0},
	    {r_angle,          28, 0},
	    {end_of_file,      29, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_dot) {
	const char* source = ". .. ... .5.25.. .5 .25.";

	cero::Token expected[] = {
	    {dot,           0,  0},
	    {dot,           2,  0},
	    {dot,           3,  0},
	    {dot,           5,  0},
	    {dot,           6,  0},
	    {dot,           7,  0},
	    {float_literal, 9,  2},
	    {float_literal, 11, 3},
	    {dot,           14, 0},
	    {dot,           15, 0},
	    {float_literal, 17, 2},
	    {float_literal, 20, 3},
	    {dot,           23, 0},
	    {end_of_file,   24, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_colon) {
	const char* source = ": :: :::a::b:c::d: :e:";

	cero::Token expected[] = {
	    {colon,       0,  0},
	    {colon_colon, 2,  0},
	    {colon_colon, 5,  0},
	    {colon,       7,  0},
	    {identifier,  8,  1},
	    {colon_colon, 9,  0},
	    {identifier,  11, 1},
	    {colon,       12, 0},
	    {identifier,  13, 1},
	    {colon_colon, 14, 0},
	    {identifier,  16, 1},
	    {colon,       17, 0},
	    {colon,       19, 0},
	    {identifier,  20, 1},
	    {colon,       21, 0},
	    {end_of_file, 22, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_equals) {
	const char* source = "= == => === ==> =>= >== ====";

	cero::Token expected[] = {
	    {eq,          0,  0},
	    {eq_eq,       2,  0},
	    {thick_arrow, 5,  0},
	    {eq_eq,       8,  0},
	    {eq,          10, 0},
	    {eq_eq,       12, 0},
	    {r_angle,     14, 0},
	    {thick_arrow, 16, 0},
	    {eq,          18, 0},
	    {r_angle_eq,  20, 0},
	    {eq,          22, 0},
	    {eq_eq,       24, 0},
	    {eq_eq,       26, 0},
	    {end_of_file, 28, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_plus) {
	const char* source = "+ ++ +++ += +=+ =+= +a++b+++c+=d";

	cero::Token expected[] = {
	    {plus,        0,  0},
	    {plus_plus,   2,  0},
	    {plus_plus,   5,  0},
	    {plus,        7,  0},
	    {plus_eq,     9,  0},
	    {plus_eq,     12, 0},
	    {plus,        14, 0},
	    {eq,          16, 0},
	    {plus_eq,     17, 0},
	    {plus,        20, 0},
	    {identifier,  21, 1},
	    {plus_plus,   22, 0},
	    {identifier,  24, 1},
	    {plus_plus,   25, 0},
	    {plus,        27, 0},
	    {identifier,  28, 1},
	    {plus_eq,     29, 0},
	    {identifier,  31, 1},
	    {end_of_file, 32, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_minus) {
	const char* source = "-> --> ->- >-> ---> - -- --- -= -=- =-= -a--b---c-=d";

	cero::Token expected[] = {
	    {thin_arrow,  0,  0},
	    {minus_minus, 3,  0},
	    {r_angle,     5,  0},
	    {thin_arrow,  7,  0},
	    {minus,       9,  0},
	    {r_angle,     11, 0},
	    {thin_arrow,  12, 0},
	    {minus_minus, 15, 0},
	    {thin_arrow,  17, 0},
	    {minus,       20, 0},
	    {minus_minus, 22, 0},
	    {minus_minus, 25, 0},
	    {minus,       27, 0},
	    {minus_eq,    29, 0},
	    {minus_eq,    32, 0},
	    {minus,       34, 0},
	    {eq,          36, 0},
	    {minus_eq,    37, 0},
	    {minus,       40, 0},
	    {identifier,  41, 1},
	    {minus_minus, 42, 0},
	    {identifier,  44, 1},
	    {minus_minus, 45, 0},
	    {minus,       47, 0},
	    {identifier,  48, 1},
	    {minus_eq,    49, 0},
	    {identifier,  51, 1},
	    {end_of_file, 52, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_star) {
	const char* source = "*a**b***c*=d* ** *** *= *=* =*=";

	cero::Token expected[] = {
	    {star,        0,  0},
	    {identifier,  1,  1},
	    {star,        2,  0},
	    {star,        3,  0},
	    {identifier,  4,  1},
	    {star,        5,  0},
	    {star,        6,  0},
	    {star,        7,  0},
	    {identifier,  8,  1},
	    {star_eq,     9,  0},
	    {identifier,  11, 1},
	    {star,        12, 0},
	    {star,        14, 0},
	    {star,        15, 0},
	    {star,        17, 0},
	    {star,        18, 0},
	    {star,        19, 0},
	    {star_eq,     21, 0},
	    {star_eq,     24, 0},
	    {star,        26, 0},
	    {eq,          28, 0},
	    {star_eq,     29, 0},
	    {end_of_file, 31, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_slash) {
	const char* source = "/ /a/b/c/=d/ / /a/b/c/=/=/ =";

	cero::Token expected[] = {
	    {slash,       0,  0},
	    {slash,       2,  0},
	    {identifier,  3,  1},
	    {slash,       4,  0},
	    {identifier,  5,  1},
	    {slash,       6,  0},
	    {identifier,  7,  1},
	    {slash_eq,    8,  0},
	    {identifier,  10, 1},
	    {slash,       11, 0},
	    {slash,       13, 0},
	    {slash,       15, 0},
	    {identifier,  16, 1},
	    {slash,       17, 0},
	    {identifier,  18, 1},
	    {slash,       19, 0},
	    {identifier,  20, 1},
	    {slash_eq,    21, 0},
	    {slash_eq,    23, 0},
	    {slash,       25, 0},
	    {eq,          27, 0},
	    {end_of_file, 28, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_percent) {
	const char* source = "%=d% % %=%=% = %a%b%c% %a%b%c%";

	cero::Token expected[] = {
	    {percent_eq,  0,  0},
	    {identifier,  2,  1},
	    {percent,     3,  0},
	    {percent,     5,  0},
	    {percent_eq,  7,  0},
	    {percent_eq,  9,  0},
	    {percent,     11, 0},
	    {eq,          13, 0},
	    {percent,     15, 0},
	    {identifier,  16, 1},
	    {percent,     17, 0},
	    {identifier,  18, 1},
	    {percent,     19, 0},
	    {identifier,  20, 1},
	    {percent,     21, 0},
	    {percent,     23, 0},
	    {identifier,  24, 1},
	    {percent,     25, 0},
	    {identifier,  26, 1},
	    {percent,     27, 0},
	    {identifier,  28, 1},
	    {percent,     29, 0},
	    {end_of_file, 30, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_ampersand) {
	const char* source = "& && &= &&= &a&&b&&&c&=d&&=e &&&&=";

	cero::Token expected[] = {
	    {ampersand,    0,  0},
	    {and_and,      2,  0},
	    {ampersand_eq, 5,  0},
	    {and_and_eq,   8,  0},
	    {ampersand,    12, 0},
	    {identifier,   13, 1},
	    {and_and,      14, 0},
	    {identifier,   16, 1},
	    {and_and,      17, 0},
	    {ampersand,    19, 0},
	    {identifier,   20, 1},
	    {ampersand_eq, 21, 0},
	    {identifier,   23, 1},
	    {and_and_eq,   24, 0},
	    {identifier,   27, 1},
	    {and_and,      29, 0},
	    {and_and_eq,   31, 0},
	    {end_of_file,  34, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_pipe) {
	const char* source = "| || ||||| ||= |= a|b||c|||d|=e||=f";

	cero::Token expected[] = {
	    {pipe,         0,  0},
	    {pipe_pipe,    2,  0},
	    {pipe_pipe,    5,  0},
	    {pipe_pipe,    7,  0},
	    {pipe,         9,  0},
	    {pipe_pipe_eq, 11, 0},
	    {pipe_eq,      15, 0},
	    {identifier,   18, 1},
	    {pipe,         19, 0},
	    {identifier,   20, 1},
	    {pipe_pipe,    21, 0},
	    {identifier,   23, 1},
	    {pipe_pipe,    24, 0},
	    {pipe,         26, 0},
	    {identifier,   27, 1},
	    {pipe_eq,      28, 0},
	    {identifier,   30, 1},
	    {pipe_pipe_eq, 31, 0},
	    {identifier,   34, 1},
	    {end_of_file,  35, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_tilde) {
	const char* source = "~ ~= ~a~b~~c~=d ~~= ~=~";

	cero::Token expected[] = {
	    {tilde,       0,  0},
	    {tilde_eq,    2,  0},
	    {tilde,       5,  0},
	    {identifier,  6,  1},
	    {tilde,       7,  0},
	    {identifier,  8,  1},
	    {tilde,       9,  0},
	    {tilde,       10, 0},
	    {identifier,  11, 1},
	    {tilde_eq,    12, 0},
	    {identifier,  14, 1},
	    {tilde,       16, 0},
	    {tilde_eq,    17, 0},
	    {tilde_eq,    20, 0},
	    {tilde,       22, 0},
	    {end_of_file, 23, 0},
	};
	check_tokens_equal(source, expected);
}

CERO_TEST(lexer_bang) {
	const char* source = "! != !a!b!!c!=d !!= !=! !!!";

	cero::Token expected[] = {
	    {bang,        0,  0},
	    {bang_eq,     2,  0},
	    {bang,        5,  0},
	    {identifier,  6,  1},
	    {bang,        7,  0},
	    {identifier,  8,  1},
	    {bang,        9,  0},
	    {bang,        10, 0},
	    {identifier,  11, 1},
	    {bang_eq,     12, 0},
	    {identifier,  14, 1},
	    {bang,        16, 0},
	    {bang_eq,     17, 0},
	    {bang_eq,     20, 0},
	    {bang,        22, 0},
	    {bang,        24, 0},
	    {bang,        25, 0},
	    {bang,        26, 0},
	    {end_of_file, 27, 0},
	};
	check_tokens_equal(source, expected);
}

} // namespace tests

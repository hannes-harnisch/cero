#pragma once

#include "cero/syntax/source.hpp"

namespace cero {

/// The category of a given token (literals, keywords, punctuation, etc.).
enum class TokenKind {
	// variable-length tokens

	identifier,
	line_comment,
	block_comment,
	dec_int_literal,
	hex_int_literal,
	bin_int_literal,
	oct_int_literal,
	float_literal,
	char_literal,
	string_literal,

	// fixed-length tokens

	kw_break,
	kw_catch,
	kw_const,
	kw_continue,
	kw_do,
	kw_else,
	kw_enum,
	kw_for,
	kw_if,
	kw_in,
	kw_interface,
	kw_let,
	kw_private,
	kw_public,
	kw_raise,
	kw_return,
	kw_static,
	kw_struct,
	kw_switch,
	kw_try,
	kw_unchecked,
	kw_var,
	kw_while,

	// one-character tokens
	dot,       // .
	comma,     // ,
	colon,     // :
	semicolon, // ;
	l_brace,   // {
	r_brace,   // }
	l_paren,   // (
	r_paren,   // )
	l_bracket, // [
	r_bracket, // ]
	l_angle,   // <
	r_angle,   // >
	eq,        // =
	plus,      // +
	minus,     // -
	star,      // *
	slash,     // /
	percent,   // %
	ampersand, // &
	pipe,      // |
	tilde,     // ~
	caret,     // ^
	bang,      // !

	// two-character tokens
	thin_arrow,    // ->
	thick_arrow,   // =>
	colon_colon,   // ::
	plus_plus,     // ++
	minus_minus,   // --
	l_angle_angle, // <<
	and_and,       // &&
	pipe_pipe,     // ||
	eq_eq,         // ==
	bang_eq,       // !=
	l_angle_eq,    // <=
	r_angle_eq,    // >=
	plus_eq,       // +=
	minus_eq,      // -=
	star_eq,       // *=
	slash_eq,      // /=
	percent_eq,    // %=
	ampersand_eq,  // &=
	pipe_eq,       // |=
	tilde_eq,      // ~=

	// three-character tokens
	l_angle_angle_eq, // <<=
	r_angle_angle_eq, // >>=
	and_and_eq,       // &&=
	pipe_pipe_eq,     // ||=

	end_of_file,
};

/// A unit of lexical analysis. It only represents a view (offset and length) into the source input it is associated with. The
/// semantic value of a token is derived at a later stage from its token kind and content.
struct Token {
	TokenKind kind : 8;
	SourceSize offset : source_size_bits;
	SourceSize variable_length; /// Only set for variable-length tokens.

	friend bool operator==(Token, Token) = default;
};

static_assert(sizeof(Token) == 8);

} // namespace cero

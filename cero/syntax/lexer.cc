#include "lexer.hh"

#include "cero/syntax/ascii.hh"
#include "cero/syntax/keywords.hh"
#include "cero/syntax/source_cursor.hh"

namespace cero {

static size_t estimate_token_count(const SourceView& source) {
	// TODO: find the most common ratio between source length and token count, for now this is an upper bound
	return source.get_text().length();
}

struct Lexer {
	TokenList tokens_;
	SourceCursor cursor_;
	const std::string_view source_text_;
	const std::string_view source_path_;
	Reporter& reporter_;
	const LexerFlags flags_;
	const uint8_t tab_size_;

	Lexer(const SourceView& source, Reporter& reporter, LexerFlags flags, uint8_t tab_size) :
	    tokens_(estimate_token_count(source)),
	    cursor_(source),
	    source_text_(source.get_text()),
	    source_path_(source.get_path()),
	    reporter_(reporter),
	    flags_(flags),
	    tab_size_(tab_size) {
	}

	void run() {
		// do not try to lex a source above the limit, otherwise offset values might overflow
		if (source_text_.length() <= source_size_max) {
			handle_source();
		}
		else {
			reporter_.report(CodeLocation {source_path_, 0, 0}, Message::source_file_too_large, MessageArgs(source_size_max));
		}

		tokens_.put({TokenKind::end_of_file, cursor_.offset()});
	}

	void handle_source() {
		while (std::optional<char> next = cursor_.peek()) {
			handle_next_character(*next);
		}
	}

	void handle_next_character(char character) {
		switch (character) {
		case ' ':
		case '\t':
		case '\n':
		case '\v':
		case '\f':
		case '\r': cursor_.advance(); break;

		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
		case '_': on_alphabetic(); break;

		case 'a': (this->*keyword_handler<'a'>) (); break;
		case 'b': (this->*keyword_handler<'b'>) (); break;
		case 'c': (this->*keyword_handler<'c'>) (); break;
		case 'd': (this->*keyword_handler<'d'>) (); break;
		case 'e': (this->*keyword_handler<'e'>) (); break;
		case 'f': (this->*keyword_handler<'f'>) (); break;
		case 'g': (this->*keyword_handler<'g'>) (); break;
		case 'h': (this->*keyword_handler<'h'>) (); break;
		case 'i': (this->*keyword_handler<'i'>) (); break;
		case 'j': (this->*keyword_handler<'j'>) (); break;
		case 'k': (this->*keyword_handler<'k'>) (); break;
		case 'l': (this->*keyword_handler<'l'>) (); break;
		case 'm': (this->*keyword_handler<'m'>) (); break;
		case 'n': (this->*keyword_handler<'n'>) (); break;
		case 'o': (this->*keyword_handler<'o'>) (); break;
		case 'p': (this->*keyword_handler<'p'>) (); break;
		case 'q': (this->*keyword_handler<'q'>) (); break;
		case 'r': (this->*keyword_handler<'r'>) (); break;
		case 's': (this->*keyword_handler<'s'>) (); break;
		case 't': (this->*keyword_handler<'t'>) (); break;
		case 'u': (this->*keyword_handler<'u'>) (); break;
		case 'v': (this->*keyword_handler<'v'>) (); break;
		case 'w': (this->*keyword_handler<'w'>) (); break;
		case 'x': (this->*keyword_handler<'x'>) (); break;
		case 'y': (this->*keyword_handler<'y'>) (); break;
		case 'z': (this->*keyword_handler<'z'>) (); break;

		case '0': on_zero(); break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': on_nonzero(); break;

		case '(': on_one_char_token(TokenKind::l_paren); break;
		case ')': on_one_char_token(TokenKind::r_paren); break;
		case '[': on_one_char_token(TokenKind::l_bracket); break;
		case ']': on_one_char_token(TokenKind::r_bracket); break;
		case '{': on_one_char_token(TokenKind::l_brace); break;
		case '}': on_one_char_token(TokenKind::r_brace); break;
		case ',': on_one_char_token(TokenKind::comma); break;
		case ';': on_one_char_token(TokenKind::semicolon); break;
		case '^': on_one_char_token(TokenKind::caret); break;
		case '<': on_left_angle(); break;
		case '>': on_right_angle(); break;
		case '.': on_dot(); break;
		case ':': on_colon(); break;
		case '=': on_equals(); break;
		case '+': on_plus(); break;
		case '-': on_minus(); break;
		case '*': on_star(); break;
		case '/': on_slash(); break;
		case '%': on_percent(); break;
		case '&': on_ampersand(); break;
		case '|': on_pipe(); break;
		case '~': on_tilde(); break;
		case '!': on_bang(); break;

		default: cursor_.advance(); break;
		}
	}

	void on_alphabetic() {
		SourceSize offset = get_offset_and_advance();
		consume_word();

		SourceSize length = cursor_.offset() - offset;
		tokens_.put({TokenKind::identifier, offset, length});
	}

	template<auto keyword_tails>
	void on_alphabetic_maybe_keyword() {
		SourceSize offset = get_offset_and_advance();
		consume_word();

		// Keywords are checked by copying the current word, except its first character, into a uint64 and comparing it against
		// uint64s that represent those keywords, also with their first character dropped, that start with the same character as
		// the current word.

		SourceSize length = cursor_.offset() - offset;

		// keywords can't be more than 9 characters, or else the whole approach won't work
		if (length <= sizeof(uint64_t) + 1) {
			std::string_view tail = source_text_.substr(offset + 1, length - 1);

			uint64_t encoded = 0;
			::memcpy(&encoded, tail.data(), tail.length()); // length is guaranteed to be <= 8 here

			for (KeywordTail kw_tail : keyword_tails) {
				if (kw_tail.tail == encoded) {
					tokens_.put({kw_tail.kind, offset});
					return;
				}
			}
		}

		tokens_.put({TokenKind::identifier, offset, length});
	}

	// We use some metaprogramming here to select a function to call for each character that only checks for keywords that start
	// with that character, or just falling back to the normal on_alphabetic function where there is no keyword that starts with
	// that character, avoiding unnecessary identical template instantiations for the keyword-checking function.

	template<char first>
	static constexpr void (Lexer::*keyword_handler)() = [] {
		static constexpr auto keyword_tails = get_keyword_tails<first>();

		if constexpr (keyword_tails.empty()) {
			return &Lexer::on_alphabetic;
		}
		else {
			return &Lexer::on_alphabetic_maybe_keyword<keyword_tails>;
		}
	}();

	void consume_word() {
		while (std::optional<char> next = cursor_.peek()) {
			const char c = *next;
			if (is_ascii_identifier_char(c)) {
				cursor_.advance();
			}
			else if (is_ascii(c)) {
				break;
			}
			else {
				// TODO: Unicode
			}
		}
	}

	void on_zero() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('x')) {
			SourceSize length = consume_digit_sequence<is_hexadecimal_digit>(offset);
			tokens_.put({TokenKind::hex_int_literal, offset, length});
			return;
		}

		if (cursor_.match('b')) {
			// consume any decimal digit for better errors during literal parsing later
			SourceSize length = consume_digit_sequence<is_decimal_digit>(offset);
			tokens_.put({TokenKind::bin_int_literal, offset, length});
			return;
		}

		if (cursor_.match('o')) {
			// consume any decimal digit for better errors during literal parsing later
			SourceSize length = consume_digit_sequence<is_decimal_digit>(offset);
			tokens_.put({TokenKind::oct_int_literal, offset, length});
			return;
		}

		handle_decimal_literal(offset);
	}

	void on_nonzero() {
		SourceSize offset = get_offset_and_advance();
		handle_decimal_literal(offset);
	}

	void handle_decimal_literal(SourceSize offset) {
		SourceSize length = consume_digit_sequence<is_decimal_digit>(offset);

		std::optional<char> next = cursor_.peek();
		if (next.has_value() && *next == '.') {
			SourceSize dot_offset = get_offset_and_advance();
			SourceSize length_after_dot = consume_digit_sequence<is_decimal_digit>(dot_offset);

			// means there is a float literal of the form `62.375`
			if (length_after_dot > 1) {
				length = dot_offset - offset + length_after_dot;
				tokens_.put({TokenKind::float_literal, offset, length});
			}
			else {
				tokens_.put({TokenKind::dec_int_literal, offset, length});
				tokens_.put({TokenKind::dot, dot_offset});
			}
		}
		else {
			tokens_.put({TokenKind::dec_int_literal, offset, length});
		}
	}

	/// Consumes a sequence of digits, where digits are decided by the given predicate. Whitespace within the sequence is
	/// allowed, but trailing whitespace is not counted towards the length.
	/// @return The length of the digit sequence including any characters starting from the given offset.
	template<bool digit_predicate(char)>
	SourceSize consume_digit_sequence(SourceSize offset) {
		while (std::optional<char> next = cursor_.peek()) {
			char c = *next;

			if (digit_predicate(c)) {
				cursor_.advance();
				continue;
			}

			if (!is_ascii_whitespace(c)) {
				break;
			}

			// handle whitespace within numeric literals, but don't count it towards the length if it's trailing
			SourceSize length = cursor_.offset() - offset;
			cursor_.advance();

			while ((next = cursor_.peek())) {
				c = *next;
				if (!is_ascii_whitespace(c)) {
					break;
				}
				cursor_.advance();
			}

			if (!digit_predicate(c)) {
				return length;
			}

			cursor_.advance();
		}

		SourceSize length = cursor_.offset() - offset;
		return length;
	}

	void on_one_char_token(TokenKind kind) {
		SourceSize offset = get_offset_and_advance();
		tokens_.put({kind, offset});
	}

	void on_left_angle() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('<')) {
			if (cursor_.match('=')) {
				tokens_.put({TokenKind::l_angle_angle_eq, offset});
			}
			else {
				tokens_.put({TokenKind::l_angle_angle, offset});
			}
		}
		else if (cursor_.match('=')) {
			tokens_.put({TokenKind::l_angle_eq, offset});
		}
		else {
			tokens_.put({TokenKind::l_angle, offset});
		}
	}

	void on_right_angle() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('>')) {
			if (cursor_.match('=')) {
				tokens_.put({TokenKind::r_angle_angle_eq, offset});
			}
			else {
				// emit two tokens to parse their use as angle brackets more easily
				tokens_.put({TokenKind::r_angle, offset});
				tokens_.put({TokenKind::r_angle, offset + 1});
			}
		}
		else if (cursor_.match('=')) {
			tokens_.put({TokenKind::r_angle_eq, offset});
		}
		else {
			tokens_.put({TokenKind::r_angle, offset});
		}
	}

	void on_dot() {
		SourceSize offset = get_offset_and_advance();
		SourceSize length = consume_digit_sequence<is_decimal_digit>(offset);

		// means there is a float literal of the form `.375`
		if (length > 1) {
			tokens_.put({TokenKind::float_literal, offset, length});
		}
		else {
			tokens_.put({TokenKind::dot, offset});
		}
	}

	void on_colon() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match(':')) {
			tokens_.put({TokenKind::colon_colon, offset});
		}
		else {
			tokens_.put({TokenKind::colon, offset});
		}
	}

	void on_equals() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('=')) {
			tokens_.put({TokenKind::eq_eq, offset});
		}
		else if (cursor_.match('>')) {
			tokens_.put({TokenKind::thick_arrow, offset});
		}
		else {
			tokens_.put({TokenKind::eq, offset});
		}
	}

	void on_plus() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('+')) {
			tokens_.put({TokenKind::plus_plus, offset});
		}
		else if (cursor_.match('=')) {
			tokens_.put({TokenKind::plus_eq, offset});
		}
		else {
			tokens_.put({TokenKind::plus, offset});
		}
	}

	void on_minus() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('>')) {
			tokens_.put({TokenKind::thin_arrow, offset});
		}
		else if (cursor_.match('-')) {
			tokens_.put({TokenKind::minus_minus, offset});
		}
		else if (cursor_.match('=')) {
			tokens_.put({TokenKind::minus_eq, offset});
		}
		else {
			tokens_.put({TokenKind::minus, offset});
		}
	}

	void on_star() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('=')) {
			tokens_.put({TokenKind::star_eq, offset});
		}
		else {
			tokens_.put({TokenKind::star, offset});
		}
	}

	void on_slash() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('=')) {
			tokens_.put({TokenKind::slash_eq, offset});
		}
		else {
			tokens_.put({TokenKind::slash, offset});
		}
	}

	void on_percent() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('=')) {
			tokens_.put({TokenKind::percent_eq, offset});
		}
		else {
			tokens_.put({TokenKind::percent, offset});
		}
	}

	void on_ampersand() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('&')) {
			if (cursor_.match('=')) {
				tokens_.put({TokenKind::and_and_eq, offset});
			}
			else {
				tokens_.put({TokenKind::and_and, offset});
			}
		}
		else if (cursor_.match('=')) {
			tokens_.put({TokenKind::ampersand_eq, offset});
		}
		else {
			tokens_.put({TokenKind::ampersand, offset});
		}
	}

	void on_pipe() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('|')) {
			if (cursor_.match('=')) {
				tokens_.put({TokenKind::pipe_pipe_eq, offset});
			}
			else {
				tokens_.put({TokenKind::pipe_pipe, offset});
			}
		}
		else if (cursor_.match('=')) {
			tokens_.put({TokenKind::pipe_eq, offset});
		}
		else {
			tokens_.put({TokenKind::pipe, offset});
		}
	}

	void on_tilde() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('=')) {
			tokens_.put({TokenKind::tilde_eq, offset});
		}
		else {
			tokens_.put({TokenKind::tilde, offset});
		}
	}

	void on_bang() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('=')) {
			tokens_.put({TokenKind::bang_eq, offset});
		}
		else {
			tokens_.put({TokenKind::bang, offset});
		}
	}

	SourceSize get_offset_and_advance() {
		SourceSize offset = cursor_.offset();
		cursor_.advance();
		return offset;
	}
};

TokenList run_lexer(const SourceView& source, Reporter& reporter, LexerFlags flags, uint8_t tab_size) {
	Lexer lexer(source, reporter, flags, tab_size);
	lexer.run();
	return std::move(lexer.tokens_);
}

} // namespace cero

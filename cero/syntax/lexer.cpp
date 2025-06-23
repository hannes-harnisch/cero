#include "lexer.hpp"

#include "cero/syntax/keywords.hpp"
#include "cero/syntax/source_cursor.hpp"
#include "cero/syntax/unicode.hpp"

namespace cero {

struct Lexer {
	TokenList tokens_;
	SourceCursor cursor_;
	const SourceView& source_;
	const std::string_view source_text_;
	Reporter& reporter_;
	const LexerFlags flags_;
	const uint8_t tab_size_;

	Lexer(const SourceView& source, Reporter& reporter, LexerFlags flags, uint8_t tab_size) :
		tokens_(source),
		cursor_(source),
		source_(source),
		source_text_(source.get_text()),
		reporter_(reporter),
		flags_(flags),
		tab_size_(tab_size) {
	}

	TokenList run() && {
		// do not try to lex a source above the limit, otherwise offset values might overflow
		if (source_text_.length() <= source_size_max) {
			run_on_source();
		} else {
			reporter_.report({source_.get_path(), 0, 0}, Message::source_file_too_large, MessageArgs(source_size_max));
		}

		tokens_.add(TokenKind::end_of_file, cursor_.offset(), 0);
		return std::move(tokens_);
	}

	void run_on_source() {
		while (char c = cursor_.peek()) {
			handle_next_character(c);
		}

		// report early return due to embedded null character
		if (cursor_.valid()) {
			report(Message::invalid_character, cursor_.offset(), MessageArgs(0));
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

		case 'a': keyword_handler<'a'>(*this); break;
		case 'b': keyword_handler<'b'>(*this); break;
		case 'c': keyword_handler<'c'>(*this); break;
		case 'd': keyword_handler<'d'>(*this); break;
		case 'e': keyword_handler<'e'>(*this); break;
		case 'f': keyword_handler<'f'>(*this); break;
		case 'g': keyword_handler<'g'>(*this); break;
		case 'h': keyword_handler<'h'>(*this); break;
		case 'i': keyword_handler<'i'>(*this); break;
		case 'j': keyword_handler<'j'>(*this); break;
		case 'k': keyword_handler<'k'>(*this); break;
		case 'l': keyword_handler<'l'>(*this); break;
		case 'm': keyword_handler<'m'>(*this); break;
		case 'n': keyword_handler<'n'>(*this); break;
		case 'o': keyword_handler<'o'>(*this); break;
		case 'p': keyword_handler<'p'>(*this); break;
		case 'q': keyword_handler<'q'>(*this); break;
		case 'r': keyword_handler<'r'>(*this); break;
		case 's': keyword_handler<'s'>(*this); break;
		case 't': keyword_handler<'t'>(*this); break;
		case 'u': keyword_handler<'u'>(*this); break;
		case 'v': keyword_handler<'v'>(*this); break;
		case 'w': keyword_handler<'w'>(*this); break;
		case 'x': keyword_handler<'x'>(*this); break;
		case 'y': keyword_handler<'y'>(*this); break;
		case 'z': keyword_handler<'z'>(*this); break;

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

		case '"':  on_quote(TokenKind::string_literal, '"'); break;
		case '\'': on_quote(TokenKind::char_literal, '\''); break;

		default: on_other_char(character); break;
		}
	}

	void on_alphabetic() {
		SourceSize offset = get_offset_and_advance();
		consume_word();

		SourceSize length = cursor_.offset() - offset;
		tokens_.add(TokenKind::identifier, offset, length);
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
					tokens_.add(kw_tail.kind, offset, 0);
					return;
				}
			}
		}

		tokens_.add(TokenKind::identifier, offset, length);
	}

	// We use some metaprogramming here to select a function to call for each character that only checks for keywords that start
	// with that character, or just falling back to the normal on_alphabetic function where there is no keyword that starts with
	// that character, avoiding unnecessary identical template instantiations for the keyword-checking function.

	template<void (Lexer::*method)()>
	static constexpr void (*lexer_fn)(Lexer&) = [](Lexer& l) { (l.*method)(); };

	template<char first>
	static constexpr void (*keyword_handler)(Lexer&) = [] {
		static constexpr auto keyword_tails = get_keyword_tails<first>();

		if constexpr (keyword_tails.empty()) {
			return lexer_fn<&Lexer::on_alphabetic>;
		} else {
			return lexer_fn<&Lexer::on_alphabetic_maybe_keyword<keyword_tails>>;
		}
	}();

	void consume_word() {
		while (char c = cursor_.peek()) {
			if (is_ascii_identifier_char(c)) {
				cursor_.advance();
			} else if (is_ascii(c)) {
				// early return for ASCII characters after identifiers
				break;
			} else [[unlikely]] {
				SourceSize offset = get_offset_and_advance();
				if (!consume_multibyte_utf8_char(c, offset, is_utf8_xid_continue_char)) {
					break;
				}
			}
		}
	}

	void on_zero() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('x')) {
			SourceSize length = consume_digit_sequence<is_hexadecimal_digit>(offset);
			tokens_.add(TokenKind::hex_int_literal, offset, length);
			return;
		}

		if (cursor_.match('b')) {
			// consume any decimal digit for better errors during literal parsing later
			SourceSize length = consume_digit_sequence<is_decimal_digit>(offset);
			tokens_.add(TokenKind::bin_int_literal, offset, length);
			return;
		}

		if (cursor_.match('o')) {
			// consume any decimal digit for better errors during literal parsing later
			SourceSize length = consume_digit_sequence<is_decimal_digit>(offset);
			tokens_.add(TokenKind::oct_int_literal, offset, length);
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

		if (cursor_.peek() == '.') {
			SourceSize dot_offset = get_offset_and_advance();
			SourceSize length_after_dot = consume_digit_sequence<is_decimal_digit>(dot_offset);

			// means there is a float literal of the form `62.375`
			if (length_after_dot > 1) {
				length = dot_offset - offset + length_after_dot;
				tokens_.add(TokenKind::float_literal, offset, length);
			} else {
				tokens_.add(TokenKind::dec_int_literal, offset, length);
				tokens_.add(TokenKind::dot, dot_offset, 0);
			}
		} else {
			tokens_.add(TokenKind::dec_int_literal, offset, length);
		}
	}

	/// Consumes a sequence of digits, where digits are decided by the given predicate. Whitespace within the sequence is
	/// allowed, but trailing whitespace is not counted towards the length.
	/// @return The length of the digit sequence including any characters starting from the given offset.
	template<bool (&digit_predicate)(char)>
	SourceSize consume_digit_sequence(SourceSize offset) {
		while (char c = cursor_.peek()) {
			if (digit_predicate(c)) {
				cursor_.advance();
				continue;
			}

			if (!is_ascii_whitespace(c)) {
				break;
			}

			// handle whitespace within numeric literals, but don't count it towards the length if it's trailing
			SourceSize length = cursor_.offset() - offset;

			do {
				cursor_.advance();
				c = cursor_.peek();
			} while (is_ascii_whitespace(c));

			if (digit_predicate(c)) {
				cursor_.advance();
			} else {
				return length;
			}
		}

		SourceSize length = cursor_.offset() - offset;
		return length;
	}

	void on_one_char_token(TokenKind kind) {
		SourceSize offset = get_offset_and_advance();
		tokens_.add(kind, offset, 0);
	}

	void on_left_angle() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('<')) {
			if (cursor_.match('=')) {
				tokens_.add(TokenKind::l_angle_angle_eq, offset, 0);
			} else {
				tokens_.add(TokenKind::l_angle_angle, offset, 0);
			}
		} else if (cursor_.match('=')) {
			tokens_.add(TokenKind::l_angle_eq, offset, 0);
		} else {
			tokens_.add(TokenKind::l_angle, offset, 0);
		}
	}

	void on_right_angle() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('>')) {
			if (cursor_.match('=')) {
				tokens_.add(TokenKind::r_angle_angle_eq, offset, 0);
			} else {
				tokens_.add(TokenKind::r_angle, offset, 0); // emit two tokens to parse their use as angle brackets more easily
				tokens_.add(TokenKind::r_angle, offset + 1, 0);
			}
		} else if (cursor_.match('=')) {
			tokens_.add(TokenKind::r_angle_eq, offset, 0);
		} else {
			tokens_.add(TokenKind::r_angle, offset, 0);
		}
	}

	void on_dot() {
		SourceSize offset = get_offset_and_advance();

		SourceSize length = consume_digit_sequence<is_decimal_digit>(offset);

		// means there is a float literal of the form `.375`
		if (length > 1) {
			tokens_.add(TokenKind::float_literal, offset, length);
		} else {
			tokens_.add(TokenKind::dot, offset, 0);
		}
	}

	void on_colon() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match(':')) {
			tokens_.add(TokenKind::colon_colon, offset, 0);
		} else {
			tokens_.add(TokenKind::colon, offset, 0);
		}
	}

	void on_equals() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('=')) {
			tokens_.add(TokenKind::eq_eq, offset, 0);
		} else if (cursor_.match('>')) {
			tokens_.add(TokenKind::thick_arrow, offset, 0);
		} else {
			tokens_.add(TokenKind::eq, offset, 0);
		}
	}

	void on_plus() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('+')) {
			tokens_.add(TokenKind::plus_plus, offset, 0);
		} else if (cursor_.match('=')) {
			tokens_.add(TokenKind::plus_eq, offset, 0);
		} else {
			tokens_.add(TokenKind::plus, offset, 0);
		}
	}

	void on_minus() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('>')) {
			tokens_.add(TokenKind::thin_arrow, offset, 0);
		} else if (cursor_.match('-')) {
			tokens_.add(TokenKind::minus_minus, offset, 0);
		} else if (cursor_.match('=')) {
			tokens_.add(TokenKind::minus_eq, offset, 0);
		} else {
			tokens_.add(TokenKind::minus, offset, 0);
		}
	}

	void on_star() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('=')) {
			tokens_.add(TokenKind::star_eq, offset, 0);
		} else {
			tokens_.add(TokenKind::star, offset, 0);
		}
	}

	void on_slash() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('/')) {
			consume_line_comment();

			if (has_flags(flags_, LexerFlags::emit_comments)) {
				SourceSize length = cursor_.offset() - offset;
				tokens_.add(TokenKind::line_comment, offset, length);
			}
		} else if (cursor_.match('*')) {
			consume_block_comment(offset);

			if (has_flags(flags_, LexerFlags::emit_comments)) {
				SourceSize length = cursor_.offset() - offset;
				tokens_.add(TokenKind::block_comment, offset, length);
			}
		} else if (cursor_.match('=')) {
			tokens_.add(TokenKind::slash_eq, offset, 0);
		} else {
			tokens_.add(TokenKind::slash, offset, 0);
		}
	}

	void consume_line_comment() {
		while (char c = cursor_.peek()) {
			if (c == '\n') {
				break;
			}
			cursor_.advance();
		}
	}

	void consume_block_comment(SourceSize offset) {
		uint32_t unclosed_count = 1;

		while (cursor_.valid()) {
			if (cursor_.match('*')) {
				if (cursor_.match('/') && --unclosed_count == 0) {
					return;
				}
			} else if (cursor_.match('/')) {
				if (cursor_.match('*')) {
					++unclosed_count;
				}
			} else {
				cursor_.advance();
			}
		}

		report(Message::unterminated_block_comment, offset, MessageArgs());
	}

	void on_percent() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('=')) {
			tokens_.add(TokenKind::percent_eq, offset, 0);
		} else {
			tokens_.add(TokenKind::percent, offset, 0);
		}
	}

	void on_ampersand() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('&')) {
			if (cursor_.match('=')) {
				tokens_.add(TokenKind::and_and_eq, offset, 0);
			} else {
				tokens_.add(TokenKind::and_and, offset, 0);
			}
		} else if (cursor_.match('=')) {
			tokens_.add(TokenKind::ampersand_eq, offset, 0);
		} else {
			tokens_.add(TokenKind::ampersand, offset, 0);
		}
	}

	void on_pipe() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('|')) {
			if (cursor_.match('=')) {
				tokens_.add(TokenKind::pipe_pipe_eq, offset, 0);
			} else {
				tokens_.add(TokenKind::pipe_pipe, offset, 0);
			}
		} else if (cursor_.match('=')) {
			tokens_.add(TokenKind::pipe_eq, offset, 0);
		} else {
			tokens_.add(TokenKind::pipe, offset, 0);
		}
	}

	void on_tilde() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('=')) {
			tokens_.add(TokenKind::tilde_eq, offset, 0);
		} else {
			tokens_.add(TokenKind::tilde, offset, 0);
		}
	}

	void on_bang() {
		SourceSize offset = get_offset_and_advance();

		if (cursor_.match('=')) {
			tokens_.add(TokenKind::bang_eq, offset, 0);
		} else {
			tokens_.add(TokenKind::bang, offset, 0);
		}
	}

	void on_quote(TokenKind kind, char quote) {
		SourceSize offset = get_offset_and_advance();

		bool ignore_quote = false;
		while (char c = cursor_.peek()) {
			if (c == '\n') {
				report(Message::missing_closing_quote, cursor_.offset(), MessageArgs());
				break;
			}

			cursor_.advance();

			if (c == '\\') {
				ignore_quote ^= true; // flip bool so we correctly handle an escaped backslash within the literal
			} else if (c == quote && !ignore_quote) {
				break;
			} else if (ignore_quote) {
				ignore_quote = false;
			}
		}

		SourceSize length = cursor_.offset() - offset;
		tokens_.add(kind, offset, length);
	}

	void on_other_char(char character) {
		SourceSize offset = get_offset_and_advance();

		if (consume_multibyte_utf8_char(character, offset, is_utf8_xid_start_char)) {
			consume_word();

			SourceSize length = cursor_.offset() - offset;
			tokens_.add(TokenKind::identifier, offset, length);
		}
	}

	bool consume_multibyte_utf8_char(char begin, SourceSize offset, bool (&utf8_predicate)(uint32_t utf8_char)) {
		const uint8_t leading_byte = (uint8_t) begin;
		const uint8_t leading_ones = (uint8_t) std::countl_one(leading_byte);

		uint32_t utf8_char = leading_byte; // fill in first byte

		if (leading_ones >= 2 && leading_ones <= 4) {
			for (uint8_t i = 1; i < leading_ones; ++i) {
				uint8_t c = (uint8_t) cursor_.next();
				utf8_char |= (uint32_t) (c << (i * 8));
			}

			if (utf8_predicate(utf8_char)) {
				return true;
			}
		}

		report(Message::invalid_character, offset, MessageArgs(utf8_char));
		return false;
	}

	SourceSize get_offset_and_advance() {
		SourceSize offset = cursor_.offset();
		cursor_.advance();
		return offset;
	}

	void report(Message message, SourceSize offset, MessageArgs args) {
		CodeLocation location = source_.locate(offset, tab_size_);
		reporter_.report(location, message, std::move(args));
	}
};

TokenList run_lexer(const SourceView& source, Reporter& reporter, LexerFlags flags, uint8_t tab_size) {
	return Lexer(source, reporter, flags, tab_size).run();
}

} // namespace cero

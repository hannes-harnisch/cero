#include "lexer.hpp"

#include "cero/syntax/ascii.hpp"
#include "cero/syntax/keywords.hpp"
#include "cero/syntax/source_cursor.hpp"

namespace cero {

struct Lexer {
	TokenList tokens_;
	SourceCursor cursor_;
	const std::string_view source_text_;
	const std::string_view source_path_;
	Reporter& reporter_;
	const LexerFlags flags_;
	const uint8_t tab_size_;

	Lexer(const SourceView& source, Reporter& reporter, LexerFlags flags, uint8_t tab_size) :
	    tokens_(source),
	    cursor_(source),
	    source_text_(source.get_text()),
	    source_path_(source.get_path()),
	    reporter_(reporter),
	    flags_(flags),
	    tab_size_(tab_size) {
	}

	TokenList run() && {
		// do not try to lex a source above the limit, otherwise offset values might overflow
		if (source_text_.length() <= source_size_max) {
			while (std::optional<char> next = cursor_.peek()) {
				handle_next_character(*next);
			}
		}
		else {
			reporter_.report(CodeLocation {source_path_, 0, 0}, Message::source_file_too_large, MessageArgs(source_size_max));
		}

		tokens_.add(TokenKind::end_of_file, cursor_.offset(), 0);
		return std::move(tokens_);
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
		case '_': on_alphabetic(*this); break;

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

		default: cursor_.advance(); break;
		}
	}

	static void on_alphabetic(Lexer& l) {
		SourceSize offset = l.get_offset_and_advance();
		l.consume_word();

		SourceSize length = l.cursor_.offset() - offset;
		l.tokens_.add(TokenKind::identifier, offset, length);
	}

	template<auto keyword_tails>
	static void on_alphabetic_maybe_keyword(Lexer& l) {
		SourceSize offset = l.get_offset_and_advance();
		l.consume_word();

		// Keywords are checked by copying the current word, except its first character, into a uint64 and comparing it against
		// uint64s that represent those keywords, also with their first character dropped, that start with the same character as
		// the current word.

		SourceSize length = l.cursor_.offset() - offset;

		// keywords can't be more than 9 characters, or else the whole approach won't work
		if (length <= sizeof(uint64_t) + 1) {
			std::string_view tail = l.source_text_.substr(offset + 1, length - 1);

			uint64_t encoded = 0;
			::memcpy(&encoded, tail.data(), tail.length()); // length is guaranteed to be <= 8 here

			for (KeywordTail kw_tail : keyword_tails) {
				if (kw_tail.tail == encoded) {
					l.tokens_.add(kw_tail.kind, offset, 0);
					return;
				}
			}
		}

		l.tokens_.add(TokenKind::identifier, offset, length);
	}

	// We use some metaprogramming here to select a function to call for each character that only checks for keywords that start
	// with that character, or just falling back to the normal on_alphabetic function where there is no keyword that starts with
	// that character, avoiding unnecessary identical template instantiations for the keyword-checking function.

	template<char first>
	static constexpr void (*keyword_handler)(Lexer&) = [] {
		static constexpr auto keyword_tails = get_keyword_tails<first>();

		if constexpr (keyword_tails.empty()) {
			return on_alphabetic;
		}
		else {
			return on_alphabetic_maybe_keyword<keyword_tails>;
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

	SourceSize get_offset_and_advance() {
		SourceSize offset = cursor_.offset();
		cursor_.advance();
		return offset;
	}
};

TokenList run_lexer(const SourceView& source, Reporter& reporter, LexerFlags flags, uint8_t tab_size) {
	return Lexer(source, reporter, flags, tab_size).run();
}

} // namespace cero

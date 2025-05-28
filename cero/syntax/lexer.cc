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

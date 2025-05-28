#pragma once

#include "cero/syntax/token.hh"

namespace cero {

/// Used to map a keyword string to its corresponding token kind.
struct Keyword {
	std::string_view lexeme;
	TokenKind kind;
};

/// Keywords cannot be longer than 9 characters, otherwise the scheme of copying the rest of the word into a 64-bit uint and
/// comparing won't work anymore.
inline constexpr Keyword keywords[] = {
    {"break",     TokenKind::kw_break    },
    {"catch",     TokenKind::kw_catch    },
    {"const",     TokenKind::kw_const    },
    {"continue",  TokenKind::kw_continue },
    {"do",        TokenKind::kw_do       },
    {"else",      TokenKind::kw_else     },
    {"enum",      TokenKind::kw_enum     },
    {"for",       TokenKind::kw_for      },
    {"if",        TokenKind::kw_if       },
    {"in",        TokenKind::kw_in       },
    {"interface", TokenKind::kw_interface},
    {"let",       TokenKind::kw_let      },
    {"mut",       TokenKind::kw_mut      },
    {"private",   TokenKind::kw_private  },
    {"public",    TokenKind::kw_public   },
    {"raise",     TokenKind::kw_raise    },
    {"return",    TokenKind::kw_return   },
    {"static",    TokenKind::kw_static   },
    {"struct",    TokenKind::kw_struct   },
    {"switch",    TokenKind::kw_switch   },
    {"try",       TokenKind::kw_try      },
    {"unchecked", TokenKind::kw_unchecked},
    {"while",     TokenKind::kw_while    },
};

/// Drops the first character of a keyword and returns the rest of its characters copied into a 64-bit uint.
consteval uint64_t encode_keyword_tail(std::string_view keyword) {
	std::string_view tail = keyword.substr(1);
	if (tail.length() > sizeof(uint64_t)) {
		throw "keyword too long";
	}

	std::array<uint8_t, sizeof(uint64_t)> bytes {};
	std::ranges::copy(tail, bytes.begin());
	return std::bit_cast<uint64_t>(bytes);
}

/// Represents a keyword string without its first character (the tail), mapping it onto its corresponding token kind.
struct KeywordTail {
	uint64_t tail;
	TokenKind kind;
};

/// @return An array of keyword tails that correspond to keywords starting with the given first character.
template<char first>
consteval auto get_keyword_tails() {
	static constexpr size_t count = (size_t) std::ranges::count_if(keywords, [](const Keyword& keyword) {
		return keyword.lexeme.starts_with(first);
	});

	std::array<KeywordTail, count> tails;
	size_t i = 0;
	for (auto [lexeme, kind] : keywords) {
		if (lexeme.starts_with(first)) {
			KeywordTail& tail = tails[i++];
			tail.tail = encode_keyword_tail(lexeme);
			tail.kind = kind;
		}
	}
	return tails;
}

} // namespace cero

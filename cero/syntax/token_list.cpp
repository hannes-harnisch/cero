#include "token_list.hpp"

namespace cero {

std::span<const Token> TokenList::get_array() const {
	return tokens_;
}

TokenList::TokenList(const SourceView& source) {
	// TODO: find the most common ratio between source length and token count and then reserve based on that
	tokens_.reserve(source.get_text().length());
}

void TokenList::add(TokenKind kind, SourceSize offset, SourceSize variable_length) {
	tokens_.emplace_back(Token {kind, offset & 0x00ffffffu, variable_length});
}

} // namespace cero

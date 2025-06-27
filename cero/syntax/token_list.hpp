#pragma once

#include "cero/syntax/token.hpp"

namespace cero {

/// An array-backed sequence of tokens resulting from the lexer stage.
class TokenList
{
public:
	/// Gets a view of the underlying token array.
	std::span<const Token> get_array() const;

private:
	std::vector<Token> tokens_;

	/// Reserves storage for the token stream based on the length of the source code input.
	explicit TokenList(const SourceView &source);

	/// Appends a token to the list.
	void add(TokenKind kind, SourceSize offset, SourceSize variable_length);

	friend struct Lexer;
};

} // namespace cero

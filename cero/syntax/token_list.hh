#pragma once

#include "cero/syntax/token.hh"

namespace cero {

/// An array-backed sequence of tokens produced by the lexer stage.
class TokenList {
public:
	/// Gets a view of the underlying token array.
	std::span<const Token> get_array() const;

private:
	std::vector<Token> list_;

	/// Reserves memory for the token stream.
	explicit TokenList(size_t initial_capacity);

	/// Appends a token to the list.
	void put(Token token);

	friend struct Lexer;
};

} // namespace cero

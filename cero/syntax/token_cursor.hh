#pragma once

#include "cero/syntax/token_list.hh"

namespace cero {

/// Iterates over a token list. Must not outlive the token stream it is initialized with.
class TokenCursor {
public:
	/// Creates a cursor positioned at the first token of the given token stream.
	explicit TokenCursor(const TokenList& token_list) :
	    it_(token_list.get_array().begin()) {
	}

	/// Returns the current token.
	Token peek() const {
		return *it_;
	}

	/// Returns the current token kind.
	TokenKind peek_kind() const {
		return it_->kind;
	}

	/// Returns the current token offset.
	SourceSize peek_offset() const {
		return it_->offset;
	}

	/// Returns the token after the current token.
	Token peek_ahead() const {
		if (it_->kind == TokenKind::end_of_file) {
			return *it_;
		}
		else {
			return it_[1];
		}
	}

	/// Returns the current token and then advances if not at the end.
	Token next() {
		const Token token = *it_;
		advance();
		return token;
	}

	/// Returns true and advances if the current token kind equals the expected, otherwise returns false.
	bool match(TokenKind kind) {
		if (it_->kind == kind) {
			advance();
			return true;
		}
		else {
			return false;
		}
	}

	/// Returns the current token and advances if the current token kind equals the expected, otherwise returns null.
	std::optional<Token> match_token(TokenKind kind) {
		Token token = *it_;
		if (token.kind == kind) {
			advance();
			return token;
		}
		return std::nullopt;
	}

	/// Moves cursor to the next token.
	void advance() {
		if (it_->kind != TokenKind::end_of_file) {
			++it_;
		}
	}

	/// Advance to the next non-comment token.
	void skip_comments() {
		TokenKind kind = it_->kind;
		while (kind == TokenKind::line_comment || kind == TokenKind::block_comment) {
			++it_;
			kind = it_->kind;
		}
	}

private:
	std::span<const Token>::iterator it_;
};

} // namespace cero

#include "TokenStream.hpp"

#include "cero/syntax/TokenCursor.hpp"

namespace cero {

uint32_t TokenStream::num_tokens() const {
	return static_cast<uint32_t>(stream_.size());
}

bool TokenStream::has_errors() const {
	return has_errors_;
}

std::span<const Token> TokenStream::array() const {
	return {stream_};
}

std::string TokenStream::to_string(const SourceGuard& source, uint8_t tab_size) const {
	auto num_tokens = stream_.size();
	auto str = fmt::format("Token stream for {} ({} token{})\n", source.get_name(), num_tokens, num_tokens == 1 ? "" : "s");

	TokenCursor cursor(*this);
	while (true) {
		auto lexeme = cursor.get_lexeme(source);
		auto token = cursor.next();

		auto kind_str = token_kind_to_string(token.kind);

		// This loop is technically quadratic since SourceGuard::locate will linearly search the source for the code location
		// every time, but it really doesn't matter.
		auto location_str = token.locate_in(source, tab_size).to_short_string();

		str += fmt::format("\t{} `{}` {}\n", kind_str, lexeme, location_str);
		if (token.kind == TokenKind::EndOfFile) {
			break;
		}
	}

	return str;
}

TokenStream::TokenStream(const SourceGuard& source) {
	// TODO: find the most common ratio between source length and token count and then reserve based on that
	stream_.reserve(source.get_length());
}

void TokenStream::add_token(TokenKind kind, SourceOffset offset) {
	stream_.emplace_back(Token {kind, offset & 0x00ffffffu});
}

} // namespace cero

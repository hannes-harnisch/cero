#include "lexer.hpp"

namespace cero {

struct Lexer {
	TokenList tokens_;
	const SourceView& source_;
	const bool emit_comments_;
	const uint8_t tab_size_;

	Lexer(const SourceView& source, bool emit_comments, uint8_t tab_size) :
	    tokens_(source),
	    source_(source),
	    emit_comments_(emit_comments),
	    tab_size_(tab_size) {
	}

	TokenList run() && {
		tokens_.add(TokenKind::end_of_file, (SourceSize) source_.get_text().length(), 0);
		return std::move(tokens_);
	}
};

TokenList run_lexer(const SourceView& source, bool emit_comments, uint8_t tab_size) {
	return Lexer(source, emit_comments, tab_size).run();
}

} // namespace cero

#include "lexer.hh"

namespace cero {

static size_t estimate_token_count(const SourceView& source) {
	// TODO: find the most common ratio between source length and token count, for now this is an upper bound
	return source.get_text().length();
}

struct Lexer {
	TokenList tokens_;
	const SourceView& source_;
	const bool emit_comments_;
	const uint8_t tab_size_;

	Lexer(const SourceView& source, bool emit_comments, uint8_t tab_size) :
	    tokens_(estimate_token_count(source)),
	    source_(source),
	    emit_comments_(emit_comments),
	    tab_size_(tab_size) {
	}

	void run() {
		tokens_.put({TokenKind::end_of_file, (SourceSize) source_.get_text().length()});
	}
};

TokenList run_lexer(const SourceView& source, bool emit_comments, uint8_t tab_size) {
	Lexer lexer(source, emit_comments, tab_size);
	lexer.run();
	return std::move(lexer.tokens_);
}

} // namespace cero

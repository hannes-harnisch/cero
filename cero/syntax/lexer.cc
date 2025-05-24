#include "lexer.hh"

namespace cero {

static size_t estimate_token_count(const SourceView& source) {
	// TODO: find the most common ratio between source length and token count, for now this is an upper bound
	return source.get_text().length();
}

struct Lexer {
	TokenList tokens_;
	const std::string_view source_text_;
	const std::string_view source_path_;
	Reporter& reporter_;
	const bool emit_comments_;
	const uint8_t tab_size_;

	Lexer(const SourceView& source, Reporter& reporter, bool emit_comments, uint8_t tab_size) :
	    tokens_(estimate_token_count(source)),
	    source_text_(source.get_text()),
	    source_path_(source.get_path()),
	    reporter_(reporter),
	    emit_comments_(emit_comments),
	    tab_size_(tab_size) {
	}

	void run() {
		if (source_text_.length() <= source_size_max) {
			handle_source();
		}
		else {
			reporter_.report(CodeLocation {source_path_, 0, 0}, Message::source_file_too_large, MessageArgs(source_size_max));
		}

		tokens_.put({TokenKind::end_of_file, (SourceSize) source_text_.length()});
	}

	void handle_source() {
	}
};

TokenList run_lexer(const SourceView& source, Reporter& reporter, bool emit_comments, uint8_t tab_size) {
	Lexer lexer(source, reporter, emit_comments, tab_size);
	lexer.run();
	return std::move(lexer.tokens_);
}

} // namespace cero

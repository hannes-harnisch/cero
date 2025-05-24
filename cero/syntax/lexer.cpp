#include "lexer.hpp"

namespace cero {

struct Lexer {
	TokenList tokens_;
	const std::string_view source_text_;
	const std::string_view source_path_;
	Reporter& reporter_;
	const bool emit_comments_;
	const uint8_t tab_size_;

	Lexer(const SourceView& source, Reporter& reporter, bool emit_comments, uint8_t tab_size) :
		tokens_(source),
		source_text_(source.get_text()),
		source_path_(source.get_path()),
		reporter_(reporter),
		emit_comments_(emit_comments),
		tab_size_(tab_size) {
	}

	TokenList run() && {
		if (source_text_.length() <= source_size_max) {
			run_on_source();
		} else {
			reporter_.report(CodeLocation {source_path_, 0, 0}, Message::source_file_too_large, MessageArgs(source_size_max));
		}

		tokens_.add(TokenKind::end_of_file, (SourceSize) source_text_.length(), 0);
		return std::move(tokens_);
	}

	void run_on_source() {
	}
};

TokenList run_lexer(const SourceView& source, Reporter& reporter, bool emit_comments, uint8_t tab_size) {
	return Lexer(source, reporter, emit_comments, tab_size).run();
}

} // namespace cero

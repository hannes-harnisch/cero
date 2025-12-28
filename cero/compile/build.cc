#include "build.hh"

#include "cero/sema/semantic_analyzer.hh"
#include "cero/syntax/lexer.hh"
#include "cero/syntax/parser.hh"

namespace cero {

void build_source(const Source& source, Reporter& reporter) {
	if (auto lock_result = source.lock()) {
		const SourceView& source_view = *lock_result;

		TokenList token_list = run_lexer(source_view, reporter, LexerFlags::none);

		Ast ast = run_parser(token_list, source_view, reporter);

		IrGraph cfg = run_semantic_analyzer(ast, source_view, reporter);
		(void) cfg;
	}
	else {
		std::error_condition& error = lock_result.error();
		const std::errc error_code = (std::errc) error.value();

		const CodeLocation location {source.get_path(), 0, 0};
		if (error_code == std::errc::no_such_file_or_directory) {
			reporter.report(location, Message::file_not_found, MessageArgs());
		}
		else {
			reporter.report(location, Message::could_not_open_file, MessageArgs(error.message()));
		}
	}
}

} // namespace cero

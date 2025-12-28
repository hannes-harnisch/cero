#include "build.hpp"

#include "cero/syntax/lexer.hpp"
#include "cero/syntax/parser.hpp"
#include "cero/type/type_checker.hpp"

namespace cero {

void build_source(const Source& source, Reporter& reporter) {
	if (auto lock_result = source.lock()) {
		const SourceView& source_view = *lock_result;

		TokenList token_list = run_lexer(source_view, reporter, LexerFlags::none);

		Ast ast = run_parser(token_list, source_view, reporter);

		SymbolTree sym_tree = run_type_checker(ast, source_view, reporter);
		(void) sym_tree;
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

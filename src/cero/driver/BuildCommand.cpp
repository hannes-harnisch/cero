#include "BuildCommand.hpp"

#include "cero/report/ConsoleReporter.hpp"
#include "cero/report/MessageArgs.hpp"
#include "cero/syntax/Lexer.hpp"
#include "cero/syntax/Parser.hpp"

namespace cero {

namespace {

void build_source(const SourceGuard& source, Reporter& reporter, const CompilerOptions& options) {
	auto token_stream = run_lexer(source, reporter, false, options.tab_size);
	if (options.print_tokens) {
		fmt::println("{}", token_stream.to_string(source, options.tab_size));
	}

	auto ast = run_parser(token_stream, source, reporter, options.tab_size);
	if (options.print_ast) {
		fmt::println("{}", ast.to_string(source, options.tab_size));
	}
}

} // namespace

bool run_build_command(const CompilerOptions& options) {
	auto source = Source::from_file(options.path);

	ConsoleReporter reporter;
	try_build_source(source, reporter, options);

	return !reporter.has_errors();
}

void try_build_source(const Source& source, Reporter& reporter, const CompilerOptions& options) {
	if (auto lock_result = source.lock()) {
		build_source(*lock_result, reporter, options);
	} else {
		auto& error = lock_result.error();
		const auto error_code = static_cast<std::errc>(error.value());

		const CodeLocation blank {source.get_name()};
		if (error_code == std::errc::no_such_file_or_directory) {
			reporter.report(blank, Message::FileNotFound, MessageArgs());
		} else {
			reporter.report(blank, Message::CouldNotOpenFile, MessageArgs(error.message()));
		}
	}
}

} // namespace cero

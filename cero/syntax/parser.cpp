#include "parser.hpp"

#include "cero/syntax/token_cursor.hpp"

namespace cero {

struct Parser {
	Ast ast_;
	TokenCursor cursor_;
	const SourceView& source_;
	Reporter& reporter_;
	const uint8_t tab_size_;

	Parser(const TokenList& token_list, const SourceView& source, Reporter& reporter, uint8_t tab_size) :
		ast_(token_list),
		cursor_(token_list),
		source_(source),
		reporter_(reporter),
		tab_size_(tab_size) {
	}

	Ast run() && {
		return std::move(ast_);
	}
};

Ast run_parser(const TokenList& token_list, const SourceView& source, Reporter& reporter, uint8_t tab_size) {
	return Parser(token_list, source, reporter, tab_size).run();
}

} // namespace cero

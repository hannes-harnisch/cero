#include "ast.hpp"

namespace cero {

CodeLocation AstNode::locate_in(const SourceView& source, uint8_t tab_size) const {
	return source.locate(offset, tab_size);
}

const AstRoot& Ast::get_root() const {
	return *root_;
}

Ast::Ast(const TokenList& token_list) {
	// TODO: initialize size of allocator based on token list
	(void) token_list;
}

} // namespace cero

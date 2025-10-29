#include "ast.hpp"

namespace cero {

const AstRoot& Ast::get_root() const {
	return *root_;
}

Ast::Ast(const TokenList& token_list) {
	// TODO: initialize size of allocator based on token list
	(void) token_list;
}

} // namespace cero

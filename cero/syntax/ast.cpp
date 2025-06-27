#include "ast.hpp"

namespace cero {

Ast::Ast(const TokenList& token_list) {
	nodes_.reserve(token_list.get_array().size());
}

} // namespace cero

#pragma once

#include "cero/syntax/ast_node.hpp"
#include "cero/syntax/token_list.hpp"

namespace cero {

class Ast {
public:

private:
	std::vector<AstNode> nodes_;

	/// Reserves storage for the AST based on the number of tokens.
	explicit Ast(const TokenList& token_list);

	friend struct Parser;
};

} // namespace cero

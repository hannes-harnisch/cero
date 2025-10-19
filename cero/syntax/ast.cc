#include "ast.hh"

namespace cero {

CodeLocation AstNode::locate_in(const SourceView& source, uint8_t tab_size) const {
	return source.locate(token_offset, tab_size);
}

const AstRoot& Ast::get_root() const {
	return *root_;
}

Ast::Ast(size_t arena_size) :
    arena_(arena_size),
    root_(arena_.create<AstRoot>()) {
}

} // namespace cero

#include "token_list.hh"

namespace cero {

std::span<const Token> TokenList::get_array() const {
	return list_;
}

TokenList::TokenList(size_t initial_capacity) {
	list_.reserve(initial_capacity);
}

void TokenList::put(Token token) {
	list_.emplace_back(token);
}

} // namespace cero

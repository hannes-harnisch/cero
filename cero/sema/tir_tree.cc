#include "tir_tree.hh"

namespace cero {

const TirRoot& TirTree::get_root() const {
	return *root_;
}

TirTree::TirTree(size_t arena_size) :
    arena_(arena_size),
    root_(arena_.create<TirRoot>()) {
}

} // namespace cero

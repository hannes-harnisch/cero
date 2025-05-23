#include "AstCursor.hpp"

#include "cero/util/Assert.hpp"
#include "cero/util/ScopedAssign.hpp"

namespace cero {

AstCursor::AstCursor(const Ast& ast) :
	it_(ast.array().begin()),
	end_(ast.array().end()),
	num_children_to_visit_(it_->num_children()) {
}

void AstCursor::visit_one(AstVisitor& visitor) {
	cero_assert(it_ != end_, "Cursor is at end.");
	it_++->visit(visitor);
}

void AstCursor::visit_all(AstVisitor& visitor) {
	ScopedAssign _(num_children_to_visit_, it_->num_children());
	it_++->visit(visitor);

	while (num_children_to_visit_ > 0) {
		visit_all(visitor);
		--num_children_to_visit_;
	}
}

void AstCursor::visit_child(AstVisitor& visitor) {
	cero_assert_debug(num_children_to_visit_ > 0, "Attempted to visit child but current node has no children left to visit.");

	if (num_children_to_visit_ > 0) {
		visit_all(visitor);
		--num_children_to_visit_;
	}
}

void AstCursor::visit_children(uint32_t n, AstVisitor& visitor) {
	cero_assert_debug(n <= num_children_to_visit_, "Attempted to visit more children than the current node has left to visit.");
	n = std::min(n, num_children_to_visit_);

	while (n > 0) {
		visit_all(visitor);
		--num_children_to_visit_;
		--n;
	}
}

uint32_t AstCursor::num_children_to_visit() const {
	return num_children_to_visit_;
}

} // namespace cero

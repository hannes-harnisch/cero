#pragma once

#include "cero/syntax/source.hpp"
#include "cero/util/assert.hpp"

namespace cero {

class SourceCursor {
public:
	/// Creates a cursor positioned at the start of the given source.
	explicit SourceCursor(const SourceView& source) :
	    begin_(source.get_text().begin()),
	    end_(source.get_text().end()),
	    it_(begin_) {
	}

	/// Returns the current character or null if the cursor is at the end.
	char peek() const {
		if (it_ != end_) {
			return *it_;
		}
		else {
			return '\0';
		}
	}

	/// Moves the cursor to the next character.
	void advance() {
		if (it_ != end_) {
			++it_;
		}
		else {
			cero_assert_debug(false, "tried to advance past end of source");
		}
	}

	/// Returns true and advances if the current character equals the expected, otherwise false.
	bool match(char expected) {
		if (it_ != end_ && *it_ == expected) {
			++it_;
			return true;
		}
		else {
			return false;
		}
	}

	/// True if the cursor is not at the end.
	bool valid() const {
		return it_ != end_;
	}

	/// Current offset from the beginning of the source text.
	SourceSize offset() const {
		return (SourceSize) (it_ - begin_);
	}

private:
	const std::string_view::iterator begin_;
	const std::string_view::iterator end_;
	std::string_view::iterator it_;
};

} // namespace cero

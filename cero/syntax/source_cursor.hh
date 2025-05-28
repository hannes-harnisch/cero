#pragma once

#include "cero/syntax/source.hh"

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
	std::optional<char> peek() const {
		if (it_ != end_) {
			return *it_;
		}
		else {
			return std::nullopt;
		}
	}

	/// Moves the cursor to the next character.
	void advance() {
		if (it_ != end_) {
			++it_;
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

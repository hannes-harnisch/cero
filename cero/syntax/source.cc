#include "source.hh"

namespace cero {

std::string_view SourceView::get_text() const {
	return text_;
}

std::string_view SourceView::get_path() const {
	return path_;
}

CodeLocation SourceView::locate(SourceSize offset, uint8_t tab_size) const {
	std::string_view range = get_text().substr(0, offset);

	size_t last_line = std::string_view::npos;
	uint32_t line = 1;
	for (size_t i = 0; i < range.size(); ++i) {
		if (range[i] == '\n') {
			++line;
			last_line = i;
		}
	}

	range = range.substr(last_line + 1); // will overflow to 0 if range is empty

	uint32_t column = 1;
	for (char c : range) {
		column += c == '\t' ? tab_size : 1;
	}

	return CodeLocation {
	    .path = path_,
	    .line = line,
	    .column = column,
	};
}

Source Source::from_file(std::string path) {
	Source source;
	source.path_ = std::move(path);
	return source; // text is intentionally null here to indicate the source is from a file
}

Source Source::from_string(const char* text, std::string name) {
	Source source;
	source.text_ = text;
	source.path_ = std::move(name);
	return source;
}

std::expected<SourceView, std::error_condition> Source::lock() const {
	SourceView view;
	view.text_ = text_;
	view.path_ = path_;
	return view;
}

std::string_view Source::get_path() const {
	return path_;
}

} // namespace cero

#include "source.hh"

namespace cero {

std::string_view SourceView::get_text() const {
	return text_;
}

std::string_view SourceView::get_path() const {
	return path_;
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

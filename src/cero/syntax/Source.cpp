#include "Source.hpp"

namespace cero {

std::string_view SourceGuard::get_text() const {
	return _source_text;
}

size_t SourceGuard::get_length() const {
	return _source_text.length();
}

std::string_view SourceGuard::get_name() const {
	return _name;
}

CodeLocation SourceGuard::locate(SourceOffset offset, uint8_t tab_size) const {
	auto range = _source_text.substr(0, offset);

	size_t last_line = std::string_view::npos;
	uint32_t line = 1;
	for (size_t i = 0; i < range.size(); ++i) {
		if (range[i] == '\n') {
			++line;
			last_line = i;
		}
	}

	range = range.substr(last_line + 1);

	uint32_t column = 1;
	for (char c : range) {
		column += c == '\t' ? tab_size : 1;
	}
	return {
		.source_name = _name,
		.line = line,
		.column = column,
	};
}

SourceGuard::SourceGuard(std::string_view source_text, std::string_view name) :
	_mapping(std::nullopt),
	_source_text(source_text),
	_name(name) {
}

SourceGuard::SourceGuard(FileMapping&& mapping, std::string_view name) :
	_mapping(std::move(mapping)),
	_source_text(_mapping->get_text()),
	_name(name) {
}

Source Source::from_file(std::string_view path) {
	return Source(path, {});
}

Source Source::from_string(std::string_view name, std::string_view source_text) {
	return Source(name, source_text);
}

std::expected<SourceGuard, std::error_condition> Source::lock() const {
	if (_source_text.data() == nullptr) {
		return FileMapping::from(_name).transform([&](FileMapping&& file_mapping) {
			return SourceGuard(std::move(file_mapping), _name);
		});
	} else {
		return SourceGuard(_source_text, _name);
	}
}

std::string_view Source::get_name() const {
	return _name;
}

Source::Source(std::string_view name, std::string_view source_text) :
	_name(name),
	_source_text(source_text) {
}

} // namespace cero

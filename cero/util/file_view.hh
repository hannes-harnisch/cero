#pragma once

#include "cero/util/impl_storage.hh"

namespace util {

/// A read-only view into a file via memory mapping.
class FileView {
public:
	/// Creates a file view for the given file path, or else fails with a system error.
	static std::expected<FileView, std::error_condition> from(std::string_view path);

	/// Gets the content of the file as a string view.
	std::string_view get_text() const;

	~FileView();
	FileView(FileView&& other) noexcept;
	FileView& operator=(FileView&& other) noexcept;

private:
	struct Impl;
	ImplStorage<Impl, 32> impl_;

	FileView();
};

} // namespace util

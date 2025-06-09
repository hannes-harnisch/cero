#include "file_view.hh"

#include "cero/util/assert.hh"
#include "cero/util/system_error.hh"
#include "cero/util/utf16.hh"
#include "cero/util/windows_api.hh"

namespace util {

struct FileView::Impl {
	HANDLE file = INVALID_HANDLE_VALUE;
	size_t size = 0;
	HANDLE mapping = nullptr; // note, the invalid value of a mapping is null, not INVALID_HANDLE_VALUE
	LPVOID addr = nullptr;

	void destroy() const {
		if (addr != nullptr) {
			BOOL success = ::UnmapViewOfFile(addr);
			cero_assert(success, "could not unmap file, {}", get_system_error_msg());
		}

		if (mapping != nullptr) {
			BOOL success = ::CloseHandle(mapping);
			cero_assert(success, "could not close file mapping handle, {}", get_system_error_msg());
		}

		if (file != INVALID_HANDLE_VALUE) {
			BOOL success = ::CloseHandle(file);
			cero_assert(success, "could not close file handle, {}", get_system_error_msg());
		}
	}
};

std::expected<FileView, std::error_condition> FileView::from(std::string_view path) {
	std::wstring path_utf16 = utf8_to_utf16(path);

	FileView f;

	f.impl_->file = ::CreateFileW(path_utf16.c_str(),
	    GENERIC_READ,
	    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, // other processes should be able to do anything with the file
	    nullptr,
	    OPEN_EXISTING,
	    FILE_ATTRIBUTE_NORMAL,
	    nullptr);

	if (f.impl_->file == INVALID_HANDLE_VALUE) {
		return std::unexpected(get_system_error());
	}

	LARGE_INTEGER file_size;
	BOOL success = ::GetFileSizeEx(f.impl_->file, &file_size);
	if (!success) {
		return std::unexpected(get_system_error());
	}

	f.impl_->size = (size_t) file_size.QuadPart;

	// mapping zero-sized files is not allowed
	if (f.impl_->size > 0) {
		f.impl_->mapping = ::CreateFileMappingW(f.impl_->file, nullptr, PAGE_READONLY, 0, 0, nullptr);
		if (f.impl_->mapping == nullptr) {
			return std::unexpected(get_system_error());
		}

		// create a copy-on-write mapping so our view does not change if another process changes the file
		f.impl_->addr = ::MapViewOfFile(f.impl_->mapping, FILE_MAP_COPY, 0, 0, 0);
		if (f.impl_->addr == nullptr) {
			return std::unexpected(get_system_error());
		}
	}

	return f;
}

std::string_view FileView::get_text() const {
	if (impl_->size == 0) {
		return "";
	}
	else {
		return std::string_view(static_cast<const char*>(impl_->addr), impl_->size);
	}
}

FileView::~FileView() {
	impl_->destroy();
}

FileView::FileView(FileView&& other) noexcept :
    impl_(std::exchange(other.impl_, {})) {
}

FileView& FileView::operator=(FileView&& other) noexcept {
	if (this != &other) {
		impl_->destroy();
		impl_ = std::exchange(other.impl_, {});
	}
	return *this;
}

FileView::FileView() = default;

} // namespace util

#include "FileMapping.hpp"

#include "Assert.hpp"
#include "SystemError.hpp"
#include "WindowsApi.hpp"
#include "WindowsUtil.hpp"

namespace cero {

struct FileMappingImpl {
	HANDLE file = INVALID_HANDLE_VALUE;
	size_t size = 0;
	HANDLE mapping = INVALID_HANDLE_VALUE;
	LPVOID addr = nullptr;

	void destroy() const {
		if (addr != nullptr) {
			BOOL success = ::UnmapViewOfFile(addr);
			cero_assert(success, "could not unmap file, {}", get_last_system_error_message());
		}
		if (mapping != INVALID_HANDLE_VALUE) {
			BOOL success = ::CloseHandle(mapping);
			cero_assert(success, "could not close file mapping handle, {}", get_last_system_error_message());
		}
		if (file != INVALID_HANDLE_VALUE) {
			BOOL success = ::CloseHandle(file);
			cero_assert(success, "could not close file handle, {}", get_last_system_error_message());
		}
	}
};

std::expected<FileMapping, std::error_condition> FileMapping::from(std::string_view path) {
	auto path_utf16 = windows::utf8_to_utf16(path);

	FileMapping f;
	f._impl->file = ::CreateFileW(path_utf16.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
								  FILE_ATTRIBUTE_NORMAL, nullptr);
	if (f._impl->file == INVALID_HANDLE_VALUE) {
		return std::unexpected(get_last_system_error());
	}

	LARGE_INTEGER file_size;
	BOOL success = ::GetFileSizeEx(f._impl->file, &file_size);
	if (!success) {
		return std::unexpected(get_last_system_error());
	}
	f._impl->size = static_cast<size_t>(file_size.QuadPart);

	if (f._impl->size > 0) {
		HANDLE mapping = ::CreateFileMappingW(f._impl->file, nullptr, PAGE_READONLY, 0, 0, nullptr);
		if (mapping == nullptr) {
			return std::unexpected(get_last_system_error());
		}
		f._impl->mapping = mapping;

		f._impl->addr = ::MapViewOfFile(f._impl->mapping, FILE_MAP_READ, 0, 0, 0);
		if (f._impl->addr == nullptr) {
			return std::unexpected(get_last_system_error());
		}
	}

	return f;
}

std::string_view FileMapping::get_text() const {
	if (_impl->size == 0) {
		return "";
	} else {
		return std::string_view(static_cast<const char*>(_impl->addr), _impl->size);
	}
}

size_t FileMapping::get_size() const {
	return _impl->size;
}

FileMapping::FileMapping() = default;
FileMapping::~FileMapping() = default;
FileMapping::FileMapping(FileMapping&&) noexcept = default;
FileMapping& FileMapping::operator=(FileMapping&&) noexcept = default;

} // namespace cero

#include "FileMapping.hpp"

#include "Assert.hpp"
#include "SystemError.hpp"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace cero {

struct FileMappingImpl {
	int fd = -1;
	size_t size = 0;
	void* addr = MAP_FAILED;

	void destroy() const {
		if (addr != MAP_FAILED) {
			int ret = ::munmap(addr, size);
			cero_assert(ret != -1, "could not unmap file, {}", get_last_system_error_message());
		}
		if (fd != -1) {
			int ret = ::close(fd);
			cero_assert(ret != -1, "could not close file, {}", get_last_system_error_message());
		}
	}
};

std::unexpected<FileMapping, std::error_condition> FileMapping::from(std::string_view path) {
	std::string path_nt(path);

	FileMapping f;
	f._impl->fd = ::open(path_nt.c_str(), O_RDONLY);
	if (f._impl->fd == -1) {
		return std::unexpected(get_last_system_error());
	}

	struct stat file_stats = {};
	int ret = ::fstat(f._impl->fd, &file_stats);
	if (ret == -1) {
		return std::unexpected(get_last_system_error());
	}
	f._impl->size = static_cast<size_t>(file_stats.st_size);

	if (f._impl->size > 0) {
		f._impl->addr = ::mmap(nullptr, f._impl->size, PROT_READ, MAP_PRIVATE, f._impl->fd, 0);
		if (f._impl->addr == MAP_FAILED) {
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

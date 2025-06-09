#include "file_view.hh"

#include "cero/util/assert.hh"
#include "cero/util/system_error.hh"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace util {

struct FileView::Impl {
	int fd = -1;
	size_t size = 0;
	void* addr = MAP_FAILED;

	void destroy() const {
		if (addr != MAP_FAILED) {
			int ret = ::munmap(addr, size);
			cero_assert(ret != -1, "could not unmap file, {}", get_system_error_msg());
		}

		if (fd != -1) {
			int ret = ::close(fd);
			cero_assert(ret != -1, "could not close file, {}", get_system_error_msg());
		}
	}
};

std::expected<FileView, std::error_condition> FileView::from(std::string_view path) {
	std::string path_nt(path);

	FileView f;

	f.impl_->fd = ::open(path_nt.c_str(), O_RDONLY);
	if (f.impl_->fd == -1) {
		return std::unexpected(get_system_error());
	}

	struct stat file_stats = {};
	int ret = ::fstat(f.impl_->fd, &file_stats);
	if (ret == -1) {
		return std::unexpected(get_system_error());
	}

	f.impl_->size = (size_t) file_stats.st_size;

	// mapping zero-sized files is not allowed
	if (f.impl_->size > 0) {
		// create a copy-on-write mapping so our view does not change if another process changes the file
		f.impl_->addr = ::mmap(nullptr, f.impl_->size, PROT_READ, MAP_PRIVATE, f.impl_->fd, 0);
		if (f.impl_->addr == MAP_FAILED) {
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

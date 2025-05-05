#pragma once

#include "UniqueImpl.hpp"

namespace cero {

class FileMapping {
public:
	static std::expected<FileMapping, std::error_condition> from(std::string_view path);

	std::string_view get_text() const;
	size_t get_size() const;

	~FileMapping();
	FileMapping(FileMapping&&) noexcept;
	FileMapping& operator=(FileMapping&&) noexcept;

private:
	UniqueImpl<struct FileMappingImpl, 32> _impl;

	FileMapping();
};

} // namespace cero

#pragma once

#include "cero/report/CodeLocation.hpp"
#include "cero/util/FileMapping.hpp"

namespace cero {

/// Recommended type to use for values representing offsets into source code.
using SourceOffset = uint32_t;

/// Minimum number of bits required to represent any offset into a Cero source file. It is 24 bits so that an offset value can
/// fit into a 32-bit integer, leaving 8 bits available for metadata serving various purposes.
constexpr inline size_t SourceOffsetBits = 24;

/// Maximum allowed size of a Cero source file in bytes (circa 16 MiB).
constexpr inline SourceOffset MaxSourceLength = (1 << SourceOffsetBits) - 1;

/// Allows access to the source code for processing, possibly from a memory-mapped file. Closes the memory-mapped file when
/// going out of scope.
class SourceGuard {
public:
	/// Gets a view of the source code.
	std::string_view get_text() const;

	/// Gets the length of the source input.
	size_t get_length() const;

	/// Gets the name of the original source input.
	std::string_view get_name() const;

	/// Determine the line and column that a given source offset corresponds to.
	CodeLocation locate(SourceOffset offset, uint8_t tab_size) const;

private:
	std::optional<FileMapping> _mapping;
	std::string_view _source_text;
	std::string_view _name;

	SourceGuard(std::string_view source_text, std::string_view name);
	SourceGuard(FileMapping&& mapping, std::string_view name);

	friend class Source;
};

/// Represents a source input for the compiler, either originating from a file or from a given string of source code. Accessing
/// the source code requires using the lock method.
class Source {
public:
	/// Creates a source representing a source file at the given path. The path will become the name of the source.
	static Source from_file(std::string_view path);

	/// Creates a source directly from a string containing source code. Useful for testing so that there doesn't have to be an
	/// extra source file for a test.
	static Source from_string(std::string_view name, std::string_view source_text);

	/// If the source represents a file, tries to open it as a memory-mapped file that will be closed when the guard goes out of
	/// scope. If the operation fails, the system error code is returned. Locking source objects created directly from strings
	/// will never fail.
	std::expected<SourceGuard, std::error_condition> lock() const;

	/// Gets the name of the source input.
	std::string_view get_name() const;

private:
	std::string_view _name;
	std::string_view _source_text;

	Source(std::string_view name, std::string_view source_text);
};

} // namespace cero

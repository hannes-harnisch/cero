#pragma once

#include "cero/report/code_location.hh"

namespace cero {

/// Used for offsets into source code or lengths of source code sequences.
using SourceSize = uint32_t;

/// A lock guard to allow access to source code for processing, possibly from a memory-mapped file. Closes the memory-mapped
/// file when going out of scope. It is safe for this object to outlive the source it came from.
class SourceView {
public:
	/// Gets a view of the source code.
	std::string_view get_text() const;

	/// Gets the path of the original source input.
	std::string_view get_path() const;

private:
	const char* text_ = nullptr;
	std::string path_;

	friend class Source;
};

/// Represents a source input for the compiler, either originating from a file or from a given string of source code. Accessing
/// the source code requires using the lock method.
class Source {
public:
	/// Creates a source representing a source file at the given path.
	static Source from_file(std::string path);

	/// Creates a source directly from a string containing source code. Useful for testing so that there doesn't have to be an
	/// extra source file for a test. The given name will be used as the path.
	static Source from_string(const char* text, std::string name);

	/// If the source represents a file, tries to open it as a memory-mapped file that will be closed when the view goes out of
	/// scope. If the operation fails, the system error code is returned. Locking source objects created directly from strings
	/// will never fail.
	std::expected<SourceView, std::error_condition> lock() const;

	/// Gets the path of the source input, or the name given to it.
	std::string_view get_path() const;

private:
	const char* text_ = nullptr;
	std::string path_;

	Source() = default;
};

/// Minimum number of bits required to represent any offset into a Cero source file. It is 24 bits so that an offset value can
/// fit into a 32-bit integer, leaving 8 bits available for metadata serving various purposes.
inline constexpr size_t source_size_bits = 24;

/// Maximum allowed size of a Cero source file in bytes (circa 16 MiB).
inline constexpr SourceSize source_size_max = (1 << source_size_bits) - 1;

} // namespace cero

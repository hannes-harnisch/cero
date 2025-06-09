#include "utf16.hh"

#include "cero/util/windows_api.hh"

namespace util {

std::wstring utf8_to_utf16(std::string_view input) {
	const size_t len = input.length();

	// empty strings are not allowed by MultiByteToWideChar
	if (len == 0) {
		return {};
	}

	std::wstring output(len, L'\0');
	int written = ::MultiByteToWideChar(CP_UTF8, 0, input.data(), (int) len, output.data(), (int) len);

	// cut down to actual number of code units
	output.resize((size_t) written);
	return output;
}

std::string utf16_to_utf8(std::wstring_view input) {
	const size_t len = input.length();

	// empty strings are not allowed by WideCharToMultiByte
	if (len == 0) {
		return {};
	}

	// maximum number of UTF-8 code units that a UTF-16 sequence could convert to
	const size_t cap = 3 * len;

	std::string output(cap, '\0');
	int written = ::WideCharToMultiByte(CP_UTF8, 0, input.data(), (int) len, output.data(), (int) cap, nullptr, nullptr);

	// cut down to actual number of code units
	output.resize((size_t) written);
	return output;
}

} // namespace util

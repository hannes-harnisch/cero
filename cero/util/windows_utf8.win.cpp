#include "windows_utf8.hpp"

#include "cero/util/windows_api.hpp"

namespace cero::windows {

std::wstring utf8_to_utf16(std::string_view input) {
	const size_t len = input.length();
	if (len == 0) { // empty strings are not allowed by the conversion function
		return {};
	}

	std::wstring output(len, L'\0');

	int written = ::MultiByteToWideChar(CP_UTF8, 0, input.data(), (int) len, output.data(), (int) len);
	output.resize((size_t) written); // cut down to actual number of code units
	return output;
}

std::string utf16_to_utf8(std::wstring_view input) {
	const size_t len = input.length();
	if (len == 0) { // empty strings are not allowed by the conversion function
		return {};
	}

	const size_t cap = 3 * len; // maximum number of UTF-8 code units that a UTF-16 sequence could convert to
	std::string output(cap, '\0');

	int written = ::WideCharToMultiByte(CP_UTF8, 0, input.data(), (int) len, output.data(), (int) cap, nullptr, nullptr);
	output.resize((size_t) written); // cut down to actual number of code units
	return output;
}

} // namespace cero::windows

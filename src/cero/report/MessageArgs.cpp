#include "MessageArgs.hpp"

namespace cero {

// TODO: Remove this hack when vcpkg has fmt at 11.1.0
using data_member_ptr_t = std::vector<fmt::basic_format_arg<fmt::format_context>>
	fmt::dynamic_format_arg_store<fmt::format_context>::*;

data_member_ptr_t get_data_member_ptr();

template<data_member_ptr_t Ptr>
struct public_cast {
	friend data_member_ptr_t get_data_member_ptr() {
		return Ptr;
	}
};

template struct public_cast<&fmt::dynamic_format_arg_store<fmt::format_context>::data_>;

bool MessageArgs::valid_for_message(Message message) const {
	std::string_view msg_format = get_message_format_string(message);

	size_t placeholder_count = 0;
	bool open = false;
	for (char c : msg_format) {
		if (c == '{') {
			open ^= true; // flip value, so escaped brace is not counted as an open brace
		} else if (c == '}' && open) {
			open = false;
			++placeholder_count;
		}
	}

	return (store.*get_data_member_ptr()).size() == placeholder_count;
}

} // namespace cero

#include "message_args.hh"

namespace cero {

bool MessageArgs::valid_for_message(Message message) const {
	std::string_view msg_format = get_message_format_string(message);

	size_t placeholder_count = 0;
	bool open = false;
	for (char c : msg_format) {
		if (c == '{') {
			open ^= true; // flip value, so the escaped brace is not counted as an open brace
		}
		else if (c == '}' && open) {
			open = false;
			++placeholder_count;
		}
	}

	return store.size() == placeholder_count;
}

} // namespace cero

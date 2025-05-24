#include "reporter.hh"

#include "cero/util/assert.hh"

namespace cero {

void Reporter::report(const CodeLocation& location, Message message, const MessageArgs& args) {
	cero_assert(args.valid_for_message(message), "incorrect number of message arguments");

	MessageLevel message_level = get_default_message_level(message);
	std::string_view format = get_message_format_string(message);
	handle_report(location, message_level, fmt::vformat(format, args.store));
}

} // namespace cero

#include "ConsoleReporter.hpp"

#include "Message.hpp"

namespace cero {

void ConsoleReporter::handle_report(CodeLocation location, MessageLevel message_level, std::string message_text) {
	if (message_level == MessageLevel::Error) {
		has_errors_ = true;
	}

	auto location_str = location.to_string();
	auto msg_level_str = get_message_level_string(message_level);
	fmt::println("{}: {}: {}", location_str, msg_level_str, message_text);
}

bool ConsoleReporter::has_errors() const {
	return has_errors_;
}

} // namespace cero

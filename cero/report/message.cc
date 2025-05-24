#include "message.hh"

#include "cero/util/assert.hh"

namespace cero {

std::string_view get_message_format_string(Message message) {
	using enum Message;
	switch (message) {
	case file_not_found:             return "file not found";
	case could_not_open_file:        return "could not open file, system error: \"{}\"";
	case source_file_too_large:      return "source file is too large, maximum allowed is {} bytes";
	case invalid_character:          return "invalid character `0x{:x}`";
	case missing_closing_quote:      return "missing closing quote";
	case unterminated_block_comment: return "block comment must be closed with `*/`";
	}
	util::assert_unreachable();
}

std::string_view get_message_level_string(MessageLevel message_level) {
	switch (message_level) {
	case MessageLevel::error:   return "error";
	case MessageLevel::warning: return "warning";
	case MessageLevel::help:    return "help";
	case MessageLevel::note:    return "note";
	}
	util::assert_unreachable();
}

MessageLevel get_default_message_level(Message message) {
	using enum Message;
	switch (message) {
	case unterminated_block_comment: return MessageLevel::warning;
	default:                         return MessageLevel::error;
	}
}

} // namespace cero

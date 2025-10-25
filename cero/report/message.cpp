#include "message.hpp"

#include "cero/util/assert.hpp"

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

	case expect_function_struct_or_enum:    return "expected function, struct or enum, but found {}";
	case expect_paren_after_function_name:  return "expected `(` after function name, but found {}";
	case expect_parameter_type:             return "expected type for parameter, but found {}";
	case expect_parameter_name:             return "expected name for parameter, but found {}";
	case expect_paren_after_parameters:     return "expected `)` after parameters, but found {}";
	case expect_return_type:                return "expected return type, but found {}";
	case expect_brace_before_function_body: return "expected `{{` before function body, but found {}";
	case expect_expression:                 return "expected expression, but found {}";
	case expect_semicolon:                  return "expected a `;`, but found {}";
	case expect_closing_paren_in_call:      return "expected closing `)` in function call, but found {}";
	}
	assert_unreachable();
}

std::string_view get_message_level_string(MessageLevel message_level) {
	switch (message_level) {
	case MessageLevel::error:   return "error";
	case MessageLevel::warning: return "warning";
	case MessageLevel::help:    return "help";
	case MessageLevel::note:    return "note";
	}
	assert_unreachable();
}

MessageLevel get_default_message_level(Message message) {
	using enum Message;
	switch (message) {
	case unterminated_block_comment: return MessageLevel::warning;
	default:                         return MessageLevel::error;
	}
}

} // namespace cero

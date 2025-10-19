#pragma once

namespace cero {

/// A distinct diagnostic emitted by the compiler.
enum class Message : uint8_t {
	file_not_found,
	could_not_open_file,
	source_file_too_large,
	invalid_character,
	missing_closing_quote,
	unterminated_block_comment,

	expect_function_struct_or_enum,
	expect_paren_after_function_name,
	expect_paren_after_parameters,
	expect_brace_before_function_body,
};

/// Gets the format string for outputting a specific diagnostic.
std::string_view get_message_format_string(Message message);

/// Indicates how severe a diagnostic is.
enum class MessageLevel : uint8_t {
	error,
	warning,
	help,
	note,
};

/// Creates a user-facing string representation of the message level.
std::string_view get_message_level_string(MessageLevel message_level);

/// Gets the message level a message should be emitted as by default.
MessageLevel get_default_message_level(Message message);

} // namespace cero

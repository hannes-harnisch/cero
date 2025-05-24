#pragma once

namespace cero {

/// A distinct diagnostic emitted by the compiler.
enum class Message : uint8_t {
	FileNotFound,
	CouldNotOpenFile,
	SourceFileTooLarge,
	InvalidCharacter,
	MissingClosingQuote,
	UnterminatedBlockComment,
	ExpectFuncStructEnum,
	ExpectParenAfterFuncName,
	ExpectType,
	ExpectParamName,
	ExpectParenAfterParams,
	ExpectParenAfterOutputs,
	ExpectBraceBeforeFuncBody,
	ExpectNameAfterLet,
	ExpectNameAfterDeclarationType,
	ExpectExpr,
	ExpectSemicolon,
	UnnecessarySemicolon,
	NameCannotAppearHere,
	ExpectNameAfterDot,
	ExpectColonOrBlockInIfExpr,
	ExpectBlockAfterIfCond,
	ExpectBlockAfterElse,
	UnnecessaryColonBeforeBlock,
	ExpectElse,
	ExpectBlockAfterWhileCond,
	ExpectClosingParen,
	ExpectBracketAfterIndex,
	ExpectBracketAfterArrayBound,
	ExpectBraceAfterPermission,
	ExpectArrowAfterFuncTypeParams,
	FuncTypeDefaultArgument,
	AmbiguousOperatorMixing,
	ExpectNameForStruct,
	ExpectNameForEnum,
};

/// Get the format string for outputting a specific diagnostic.
std::string_view get_message_format_string(Message message);

/// Indicates how severe a diagnostic is.
enum class MessageLevel : uint8_t {
	Error,
	Warning,
	Help,
	Note,
};

/// Creates a user-facing string representation of the message level.
std::string_view get_message_level_string(MessageLevel message_level);

/// Gets the message level a message should be emitted as by default.
MessageLevel get_default_message_level(Message message);

} // namespace cero

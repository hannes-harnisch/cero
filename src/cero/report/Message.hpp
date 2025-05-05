#pragma once

namespace cero {

/// A distinct diagnostic message emitted by the compiler.
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

/// Looks up the format string for a given message.
std::string_view get_message_format_string(Message message);

/// Decides how severe a diagnostic message is, which will influence whether a build is considered successful.
enum class MessageLevel : uint8_t {
	Error,
	Warning,
	Help,
	Note,
};

/// Creates a user-facing string representation of the message level.
std::string_view get_message_level_string(MessageLevel message_level);

/// Looks up the default message level for a given message.
MessageLevel get_default_message_level(Message message);

} // namespace cero

#pragma once

#include "CodeLocation.hpp"
#include "Message.hpp"
#include "MessageArgs.hpp"

namespace cero {

/// Interface for implementing different ways to report diagnostics.
class Reporter {
public:
	virtual ~Reporter() = default;

	/// Reports a diagnostic message.
	void report(CodeLocation location, Message message, MessageArgs args);

	virtual bool has_errors() const = 0;

private:
	/// Will be called by the report method. Override to handle how the report is actually emitted.
	virtual void handle_report(CodeLocation location, MessageLevel message_level, std::string message_text) = 0;
};

} // namespace cero

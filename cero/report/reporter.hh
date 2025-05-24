#pragma once

#include "cero/report/code_location.hh"
#include "cero/report/message.hh"
#include "cero/report/message_args.hh"

namespace cero {

/// Interface for implementing different ways to report diagnostics.
class Reporter {
public:
	virtual ~Reporter() = default;

	/// Reports a diagnostic message.
	void report(const CodeLocation& location, Message message, const MessageArgs& args);

private:
	virtual void handle_report(const CodeLocation& location, MessageLevel message_level, std::string message_text) = 0;
};

} // namespace cero

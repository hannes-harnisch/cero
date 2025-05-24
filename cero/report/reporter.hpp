#pragma once

#include "cero/report/code_location.hpp"
#include "cero/report/message.hpp"
#include "cero/report/message_args.hpp"

namespace cero {

/// Interface for implementing different ways to report diagnostics.
class Reporter {
public:
	virtual ~Reporter() = default;

	/// Reports a diagnostic message.
	void report(CodeLocation location, Message message, MessageArgs args);

private:
	virtual void handle_report(CodeLocation location, MessageLevel message_level, std::string message_text) = 0;
};

} // namespace cero

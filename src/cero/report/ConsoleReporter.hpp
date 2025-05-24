#pragma once

#include "Reporter.hpp"

namespace cero {

/// Prints reports to the console.
class ConsoleReporter : public Reporter {
public:
	/// Sets up color output and UTF-8 output.
	static void init_console_environment();

	bool has_errors() const override;

private:
	bool has_errors_ = false;

	void handle_report(CodeLocation location, MessageLevel message_level, std::string message_text) override;
};

} // namespace cero

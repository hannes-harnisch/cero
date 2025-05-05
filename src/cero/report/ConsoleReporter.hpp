#pragma once

#include "Reporter.hpp"

namespace cero {

class ConsoleReporter : public Reporter {
public:
	static void init_console_environment();

	bool has_errors() const override;

private:
	bool has_errors_ = false;

	void handle_report(CodeLocation location, MessageLevel message_level, std::string message_text) override;
};

} // namespace cero

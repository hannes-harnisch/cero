#pragma once

#include "cero/report/reporter.hh"

namespace cero {

/// Prints reports to the console.
class ConsoleReporter : public Reporter {
public:
	/// Sets up color output and UTF-8 output.
	static void init_console_environment();
};

} // namespace cero

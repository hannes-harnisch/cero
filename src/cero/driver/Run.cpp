#include "Run.hpp"

#include "cero/driver/BuildCommand.hpp"
#include "cero/report/ConsoleReporter.hpp"
#include "cero/util/Assert.hpp"

namespace cero {

bool run(std::span<char*> args) {
	ConsoleReporter::init_console_environment();

	if (auto options = CompilerOptions::from(args)) {
		return run_build_command(*options);
	} else {
		return false;
	}
}

} // namespace cero

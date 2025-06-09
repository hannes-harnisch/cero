#include "common/test.hh"
#include "common/test_reporter.hh"

#include <cero/compile/build.hh>

namespace tests {

CERO_TEST(build_source_file_not_found) {
	TestReporter r;
	r.set_source_name("file_should_not_exist.ce");
	r.expect(0, 0, cero::Message::file_not_found, cero::MessageArgs());

	cero::Source source = cero::Source::from_file("file_should_not_exist.ce");
	cero::build_source(source, r);
}

CERO_TEST(build_source_could_not_open_file) {
	// Windows and Posix report different error codes when trying to open '.'
#ifdef CERO_SYSTEM_WINDOWS
	static constexpr std::errc err_code = std::errc::permission_denied;
#else
	static constexpr std::errc err_code = std::errc::no_such_device;
#endif

	const std::string err_msg = std::make_error_condition(err_code).message();

	TestReporter r;
	r.set_source_name(".");
	r.expect(0, 0, cero::Message::could_not_open_file, cero::MessageArgs(err_msg));

	cero::Source source = cero::Source::from_file(".");
	cero::build_source(source, r);
}

} // namespace tests

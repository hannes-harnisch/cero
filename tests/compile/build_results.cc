#include "common/test.hh"
#include "common/test_reporter.hh"

#include <cero/compile/build.hh>

namespace tests {

CERO_TEST(build_source_empty_file) {
	TestReporter r;

	std::string path = std::filesystem::path(__FILE__).remove_filename().append("empty_file.ce").string();
	cero::Source source = cero::Source::from_file(path);
	cero::build_source(source, r);
}

CERO_TEST(build_source_non_empty_file) {
	TestReporter r;

	std::string path = std::filesystem::path(__FILE__).remove_filename().append("non_empty_file.ce").string();
	cero::Source source = cero::Source::from_file(path);
	cero::build_source(source, r);
}

} // namespace tests

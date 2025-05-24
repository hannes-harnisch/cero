#include "common/test.hh"
#include "common/test_reporter.hh"

#include <cero/syntax/lexer.hh>

namespace tests {

CERO_TEST(lexer_source_too_large) {
	TestReporter r;
	r.expect(0, 0, cero::Message::source_file_too_large, cero::MessageArgs(cero::source_size_max));

	std::string too_large(cero::source_size_max + 1, ' ');
	build_test_source(r, too_large.c_str());
}

} // namespace tests

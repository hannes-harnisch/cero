#include "common/test.hpp"
#include "common/test_reporter.hpp"

#include <cero/syntax/lexer.hpp>

namespace tests {

CERO_TEST(lexer_source_too_large) {
	TestReporter r;
	r.expect(0, 0, cero::Message::source_file_too_large, cero::MessageArgs(cero::source_size_max));

	std::string too_large(cero::source_size_max + 1, ' ');
	build_test_source(r, too_large.c_str());
}

} // namespace tests

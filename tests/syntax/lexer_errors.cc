#include "common/test.hh"
#include "common/test_reporter.hh"

#include <cero/compile/build.hh>

namespace tests {

CERO_TEST(lexer_source_too_large) {
	TestReporter r;
	r.expect(0, 0, cero::Message::source_file_too_large, cero::MessageArgs(cero::source_size_max));

	std::string too_large(cero::source_size_max + 1, ' ');
	build_test_source(r, too_large.c_str());
}

CERO_TEST(lexer_embedded_null) {
	std::string path = std::filesystem::path(__FILE__).remove_filename().append("embedded_null.ce").string();

	TestReporter r;
	r.set_source_name(path);
	r.expect(1, 1, cero::Message::invalid_character, cero::MessageArgs(0));

	cero::Source source = cero::Source::from_file(path);
	cero::build_source(source, r);
}

CERO_TEST(lexer_string_literal_missing_closing_quote) {
	TestReporter r;
	r.expect(3, 28, cero::Message::missing_closing_quote, cero::MessageArgs());

	build_test_source(r, R"_____(
foo() {
	let string = "Oh no...;
	;
}
)_____");
}

CERO_TEST(lexer_char_literal_missing_closing_quote) {
	TestReporter r;
	r.expect(3, 19, cero::Message::missing_closing_quote, cero::MessageArgs());

	build_test_source(r, R"_____(
foo() {
	let char = 'x;
	;
}
)_____");
}

} // namespace tests

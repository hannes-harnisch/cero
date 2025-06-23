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

CERO_TEST(lexer_unterminated_block_comment) {
	TestReporter r;
	r.expect(2, 1, cero::Message::unterminated_block_comment, cero::MessageArgs());

	build_test_source(r, R"_____(
/* abc
)_____");
}

CERO_TEST(lexer_invalid_character_ascii_control_char) {
	TestReporter r;
	r.expect(1, 5, cero::Message::invalid_character, cero::MessageArgs(0x7));
	r.expect(1, 6, cero::Message::invalid_character, cero::MessageArgs(0x7));
	r.expect(1, 7, cero::Message::invalid_character, cero::MessageArgs(0x7));

	build_test_source(r, "main\a\a\a() {}");
}

CERO_TEST(lexer_invalid_character_non_unicode) {
	TestReporter r;
	r.expect(1, 10, cero::Message::invalid_character, cero::MessageArgs(0xff));
	r.expect(1, 11, cero::Message::invalid_character, cero::MessageArgs(0xff));
	r.expect(1, 12, cero::Message::invalid_character, cero::MessageArgs(0xff));

	build_test_source(r, "something\xff\xff\xff() {}");
}

CERO_TEST(lexer_invalid_character_emoji) {
	TestReporter r;
	r.expect(1, 1, cero::Message::invalid_character, cero::MessageArgs(0xa0929ff0));
	r.expect(1, 5, cero::Message::invalid_character, cero::MessageArgs(0xa0929ff0));
	r.expect(1, 9, cero::Message::invalid_character, cero::MessageArgs(0xa0929ff0));
	r.expect(1, 13, cero::Message::invalid_character, cero::MessageArgs(0xa0929ff0));
	r.expect(1, 17, cero::Message::invalid_character, cero::MessageArgs(0xa0929ff0));
	r.expect(1, 21, cero::Message::invalid_character, cero::MessageArgs(0xa0929ff0));
	r.expect(1, 25, cero::Message::invalid_character, cero::MessageArgs(0xa0929ff0));

	build_test_source(r, reinterpret_cast<const char*>(u8"💠💠💠💠💠💠💠"));
}

CERO_TEST(lexer_invalid_character_non_xid) {
	TestReporter r;
	r.expect(1, 11, cero::Message::invalid_character, cero::MessageArgs(0xa793e2));

	build_test_source(r, reinterpret_cast<const char*>(u8"oopsie() {ⓧ}"));
}

} // namespace tests

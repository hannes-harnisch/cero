#include "common/test.hh"
#include "common/test_reporter.hh"

namespace tests {

CERO_TEST(parser_expect_function_struct_or_enum) {
	TestReporter r;
	r.expect(5, 1, cero::Message::expect_function_struct_or_enum, cero::MessageArgs("`(`"));

	build_test_source(r, R"_____(
main() {
}

() {
}

foo() {
}
)_____");
}

CERO_TEST(parser_expect_paren_after_function_name) {
	TestReporter r;
	r.expect(2, 5, cero::Message::expect_paren_after_function_name, cero::MessageArgs("`)`"));

	build_test_source(r, R"_____(
main) {
}
)_____");
}

CERO_TEST(parser_expect_paren_after_parameters) {
	TestReporter r;
	r.expect(2, 20, cero::Message::expect_paren_after_parameters, cero::MessageArgs("`->`"));
	r.expect(6, 19, cero::Message::expect_paren_after_parameters, cero::MessageArgs("`}`"));

	build_test_source(r, R"_____(
private foo(bool x -> bool {
	return x;
}

private goo(bool x} -> bool {
	return x;
}
)_____");
}

CERO_TEST(parser_expect_brace_before_function_body) {
	TestReporter r;
	r.expect(3, 5, cero::Message::expect_brace_before_function_body, cero::MessageArgs("`return`"));
	r.expect(7, 1, cero::Message::expect_brace_before_function_body, cero::MessageArgs("`}`"));
	r.expect(9, 14, cero::Message::expect_brace_before_function_body, cero::MessageArgs("`-`"));

	build_test_source(r, R"_____(
foo(bool x) -> bool
	return x;
}

public goo() -> void
}

public hoo() -< void {
}
)_____");
}

} // namespace tests

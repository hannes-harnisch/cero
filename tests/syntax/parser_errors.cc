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

CERO_TEST(parser_expect_parameter_type) {
	TestReporter r;
	r.expect(2, 5, cero::Message::expect_parameter_type, cero::MessageArgs("`,`"));

	build_test_source(r, R"_____(
foo(, bool x) -> bool {
	return x;
}
)_____");
}

CERO_TEST(parser_expect_parameter_name) {
	TestReporter r;
	r.expect(2, 9, cero::Message::expect_parameter_name, cero::MessageArgs("`,`"));

	build_test_source(r, R"_____(
foo(bool, bool x) -> bool {
	return x;
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

CERO_TEST(parser_expect_return_type) {
	TestReporter r;
	r.expect(2, 24, cero::Message::expect_return_type, cero::MessageArgs("`{`"));
	r.expect(6, 18, cero::Message::expect_return_type, cero::MessageArgs("`%`"));
	r.expect(6, 18, cero::Message::expect_brace_before_function_body, cero::MessageArgs("`%`"));

	build_test_source(r, R"_____(
private foo(bool x) -> {
	return x;
}

private goo() -> % {
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

CERO_TEST(expect_expression) {
	TestReporter r;
	r.expect(3, 5, cero::Message::expect_expression, cero::MessageArgs("`]`"));
	r.expect(8, 1, cero::Message::expect_expression, cero::MessageArgs("`}`"));
	r.expect(11, 5, cero::Message::expect_expression, cero::MessageArgs("`+=`"));

	build_test_source(r, R"_____(
a() {
    ]
}

c() {
	foo(
}

b() {
    += x
}

foo() {
}
)_____");
}

CERO_TEST(expect_semicolon) {
	TestReporter r;
	r.expect(4, 1, cero::Message::expect_semicolon, cero::MessageArgs("`}`"));

	build_test_source(r, R"_____(
a() {
    return 0
}

b() {
    return 0;
}
)_____");
}

CERO_TEST(expect_closing_paren_after_function_call) {
	TestReporter r;
	r.expect(7, 1, cero::Message::expect_closing_paren_in_call, cero::MessageArgs("`}`"));

	build_test_source(r, R"_____(
foo(int32 _) {
}

f() {
	foo(1
}

g() {
	foo(2);
}
)_____");
}

} // namespace tests

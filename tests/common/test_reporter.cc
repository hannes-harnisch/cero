#include "test_reporter.hh"

#include "common/test.hh"

namespace tests {

TestReporter::TestReporter() :
    source_name_(get_current_test_name()) {
}

TestReporter::~TestReporter() {
	// If this fails, not all expected reports were seen.
	CHECK(expected_reports_.empty());
}

void TestReporter::expect(uint32_t line, uint32_t column, cero::Message message, cero::MessageArgs args) {
	CHECK(args.valid_for_message(message));
	auto format = cero::get_message_format_string(message);
	auto message_text = fmt::vformat(format, args.store);

	cero::CodeLocation location {source_name_, line, column};
	expected_reports_.emplace(location, std::move(message_text));
}

void TestReporter::set_source_name(std::string source_name) {
	source_name_ = std::move(source_name);
}

void TestReporter::handle_report(const cero::CodeLocation& location, cero::MessageLevel, std::string message_text) {
	const bool reports_are_done = expected_reports_.empty();
	REQUIRE(!reports_are_done); // If this fails, every expected report was already seen and an unexpected one was received.

	const Report& expected = expected_reports_.front();
	const Report received {location, std::move(message_text)};
	const bool report_matches = expected == received;
	CHECK(report_matches); // If this fails, the received report does not match the report expected to be seen next.

	if (report_matches) {
		expected_reports_.pop();
	}
}

} // namespace tests

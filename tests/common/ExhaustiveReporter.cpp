#include "ExhaustiveReporter.hpp"

#include "common/Test.hpp"

#include <doctest/doctest.h>

namespace tests {

ExhaustiveReporter::ExhaustiveReporter() :
	source_name_(get_current_test_name()) {
}

ExhaustiveReporter::~ExhaustiveReporter() {
	// If this fails, not all expected reports were seen.
	CHECK(expected_reports_.empty());
}

bool ExhaustiveReporter::has_errors() const {
	return has_errors_;
}

void ExhaustiveReporter::expect(uint32_t line, uint32_t column, cero::Message message, cero::MessageArgs args) {
	CHECK(args.valid_for_message(message));
	auto format = cero::get_message_format_string(message);
	auto message_text = fmt::vformat(format, args.store);

	cero::CodeLocation location {source_name_, line, column};
	expected_reports_.emplace(location, std::move(message_text));
}

void ExhaustiveReporter::set_source_name(std::string_view source_name) {
	source_name_ = source_name;
}

void ExhaustiveReporter::handle_report(cero::CodeLocation location, cero::MessageLevel, std::string message_text) {
	const bool reports_are_done = expected_reports_.empty();
	REQUIRE(!reports_are_done); // If this fails, every expected report was already seen and an unexpected one was received.

	const Report& expected = expected_reports_.front();
	const Report received {location, std::move(message_text)};
	const bool report_matches = expected == received;
	CHECK(report_matches); // If this fails, the received report does not match the report that is expected to be seen next.

	if (report_matches) {
		expected_reports_.pop();
	}
}

} // namespace tests

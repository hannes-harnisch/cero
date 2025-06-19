#pragma once

#include <cero/report/code_location.hh>
#include <cero/report/message.hh>
#include <cero/report/reporter.hh>

namespace tests {

/// A test utility that triggers a test failure if there are unexpected reports left after testing a function that uses a
/// reporter or if not all expected reports were received.
class TestReporter : public cero::Reporter {
public:
	/// Uses the current test name as the source path for received reports.
	TestReporter();

	/// Checks whether all expected reports were seen.
	~TestReporter() override;

	/// Marks a report as expected with the currently set source name, given line, column, message kind and arguments. Instead
	/// of expecting specific strings, this way allows decoupling the tests from the exact text of a diagnostic message.
	void expect(uint32_t line, uint32_t column, cero::Message message, cero::MessageArgs args);

	/// Sets the current source name used for expected reports.
	void set_source_name(std::string source_name);

	TestReporter(TestReporter&&) = delete;

private:
	struct Report {
		cero::CodeLocation location;
		std::string message_text;

		bool operator==(const Report&) const = default;
	};

	std::queue<Report> expected_reports_;
	std::string source_name_;

	void handle_report(const cero::CodeLocation& location, cero::MessageLevel message_level, std::string message_text) override;
};

} // namespace tests

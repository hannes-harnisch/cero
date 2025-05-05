#pragma once

#include <cero/report/CodeLocation.hpp>
#include <cero/report/Message.hpp>
#include <cero/report/Reporter.hpp>

namespace tests {

/// A test utility that triggers a test failure if there are unexpected messages left after running the compiler.
class ExhaustiveReporter : public cero::Reporter {
public:
	/// Creates a new exhaustive reporter where the current test name is the source name for reports.
	ExhaustiveReporter();

	/// Checks whether all expected reports were seen.
	~ExhaustiveReporter() override;

	bool has_errors() const override;

	/// Marks a report as expected with the currently set source name, given line, column, message kind and arguments. Instead
	/// of expecting specific strings, this way allows decoupling the tests from the exact text of a diagnostic message.
	void expect(uint32_t line, uint32_t column, cero::Message message, cero::MessageArgs args);

	/// Sets the current source name used for expected reports.
	void set_source_name(std::string_view source_name);

	ExhaustiveReporter(ExhaustiveReporter&&) = delete;
	ExhaustiveReporter& operator=(ExhaustiveReporter&&) = delete;

private:
	struct Report {
		cero::CodeLocation location;
		std::string message_text;

		bool operator==(const Report&) const = default;
	};

	std::queue<Report> expected_reports_;
	std::string_view source_name_;
	bool has_errors_ = false;

	void handle_report(cero::CodeLocation location, cero::MessageLevel message_level, std::string message_text) override;
};

} // namespace tests

#include <cero/report/ConsoleReporter.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <fmt/ostream.h>

namespace tests {

std::string_view get_current_test_name() {
	// Workaround because Doctest doesn't have an API for this yet. Also, it must be implemented in the file that defines
	// DOCTEST_CONFIG_IMPLEMENT as described in https://github.com/doctest/doctest/issues/345.
	return doctest::detail::g_cs->currentTest->m_name;
}
 
/// Implements custom console output for the testing framework.
class TestRunner : public doctest::IReporter {
public:
	explicit TestRunner(const doctest::ContextOptions& in) :
		out(*in.cout) {
		cero::ConsoleReporter::init_console_environment();
	}

	void report_query(const doctest::QueryData&) override {
	}

	void test_run_start() override {
	}

	void test_run_end(const doctest::TestRunStats&) override {
	}

	void test_case_start(const doctest::TestCaseData& test_case) override {
		fmt::print(out, "Test: {}", test_case.m_name);
	}

	void test_case_reenter(const doctest::TestCaseData&) override {
	}

	void test_case_end(const doctest::CurrentTestCaseStats& stats) override {
		if (stats.testCaseSuccess) {
			fmt::println(out, "--- PASSED");
		}
	}

	void test_case_exception(const doctest::TestCaseException&) override {
	}

	void subcase_start(const doctest::SubcaseSignature&) override {
	}

	void subcase_end() override {
	}

	void log_assert(const doctest::AssertData&) override {
	}

	void log_message(const doctest::MessageData&) override {
	}

	void test_case_skipped(const doctest::TestCaseData&) override {
	}

private:
	std::ostream& out;
};

} // namespace tests

DOCTEST_REGISTER_REPORTER("tests::TestRunner", 1, tests::TestRunner);

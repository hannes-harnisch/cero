#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hh"

#include <cero/report/console_reporter.hh>

#include <iostream>

#include <fmt/color.h>
#include <fmt/ostream.h>

namespace tests {

/// Workaround because Doctest doesn't have an API for this yet. Also, it must be implemented in the file that defines
/// DOCTEST_CONFIG_IMPLEMENT as described in https://github.com/doctest/doctest/issues/345.
const char* get_current_test_name() {
	return doctest::detail::g_cs->currentTest->m_name;
}

/// Implements custom console output for the testing framework.
struct DoctestListener : doctest::IReporter {
	std::ostream& out_;
	FILE* file_ = nullptr;

	explicit DoctestListener(const doctest::ContextOptions& in) :
	    out_(*in.cout) {
		std::streambuf* buf = out_.rdbuf();
		if (buf == std::cout.rdbuf()) {
			file_ = stdout;
		}
		else if (buf == std::cerr.rdbuf()) {
			file_ = stderr;
		}

		cero::ConsoleReporter::init_console_environment();
	}

	void report_query(const doctest::QueryData&) override {
	}

	void test_run_start() override {
	}

	void test_run_end(const doctest::TestRunStats&) override {
	}

	void test_case_start(const doctest::TestCaseData& test_case) override {
		print(fmt::fg(fmt::terminal_color::bright_cyan), "Test: ");
		print({}, "{}: ", test_case.m_name);
	}

	void test_case_reenter(const doctest::TestCaseData&) override {
	}

	void test_case_end(const doctest::CurrentTestCaseStats& stats) override {
		if (stats.testCaseSuccess) {
			print(fmt::fg(fmt::terminal_color::green), "passed\n");
		}
		else {
			print(fmt::fg(fmt::terminal_color::red), "failed\n");
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

	template<typename... Args>
	void print(const fmt::text_style& style, fmt::format_string<Args...> fmt, Args&&... args) {
		if (file_ != nullptr) {
			fmt::print(file_, style, fmt, std::forward<Args>(args)...);
		}
		else {
			fmt::print(out_, fmt, std::forward<Args>(args)...);
		}
	}
};

} // namespace tests

DOCTEST_REGISTER_LISTENER("tests::DoctestListener", 1, tests::DoctestListener);

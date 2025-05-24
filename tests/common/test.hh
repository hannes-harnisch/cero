#pragma once

#include "common/doctest.hh"

#include <cero/report/reporter.hh>
#include <cero/syntax/source.hh>

namespace tests {

/// Creates and registers a test case.
#define CERO_TEST(name) DOCTEST_CREATE_AND_REGISTER_FUNCTION(name, #name)

/// Get the name of the currently executing test function.
const char* get_current_test_name();

/// Creates a source from the given string using the current test name for code locations. Helper for passing strings directly
/// to functions that process Cero source code.
cero::SourceView make_test_source(const char* source_text);

/// Runs the build pipeline on a single source created from the given string.
void build_test_source(cero::Reporter& reporter, const char* source_text);

} // namespace tests

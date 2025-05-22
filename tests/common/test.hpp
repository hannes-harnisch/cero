#pragma once

#include <cero/syntax/source.hpp>

#include <doctest/doctest.h>

namespace tests {

/// Creates and registers a test case.
#define CERO_TEST(name) DOCTEST_CREATE_AND_REGISTER_FUNCTION(name, #name)

/// Get the name of the currently executing test function.
const char* get_current_test_name();

/// Creates a source from the given string using the current test name for code locations. Helper for passing strings directly
/// to functions that process Cero source code.
cero::SourceView make_test_source(const char* source_text);

} // namespace tests

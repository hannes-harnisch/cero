#pragma once

#include <cero/report/Reporter.hpp>
#include <cero/syntax/Source.hpp>

#include <doctest/doctest.h>

namespace tests {

cero::SourceGuard make_test_source(std::string_view source_text);

void build_test_source(cero::Reporter& reporter, std::string_view source_text);

std::string_view get_current_test_name();

/// Creates and registers a test case.
#define CERO_TEST(NAME) DOCTEST_CREATE_AND_REGISTER_FUNCTION(NAME, #NAME)

constexpr inline uint8_t TabSize = 4;

} // namespace tests
